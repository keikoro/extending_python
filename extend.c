/**
 * extend.c
 *
 * Extending Python with C code
 * A first attempt.
 *
 * Create a module with a method to look up Pokémon by their index number.
 *
 * Copyright (c) 2016 Kerstin Kollmann <code∆kerstinkollmann·com>
 */
#include <Python.h>

const char *INPUT_FILE_PATH = "pokedex.txt";

const size_t MAX_LINE_LENGTH = 255;

// Using int, because Python parses the corresponding index as int
const int MAX_POKEMON_INDEX = 720;

// Function declarations
PyMODINIT_FUNC PyInit_pokemon_lookup(void);
static PyObject* no_pokemon_search(PyObject *self, PyObject *args);
static PyObject* find_pokemon_by_index(PyObject *self, PyObject *args);

// Module name
static const char module_name[] = "pokemon_lookup";

// Docstrings for Python (module + methods)
static const char docstring_module[] =
    "A module to test extending Python with C. "
    "Lets you look up Pokémon by number.";
static const char docstring_method_no_pokemon_search[] =
    "Prints out a test message.";
static const char docstring_method_find_pokemon_by_index[] =
    "Finds a Pokémon by its index number.";

static PyObject *no_pokemon_search(PyObject *const self, PyObject *const args)
{
    puts("No Pokémon for you!");

    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject *find_pokemon_by_index(PyObject *const self, PyObject *const args)
{
    PyObject* ret_obj = NULL;

    char *const line = malloc(MAX_LINE_LENGTH);
    if (line != NULL)
    {
        int pokemon_idx;
        if (PyArg_ParseTuple(args, "i", &pokemon_idx))
        {
            if (pokemon_idx >= 1 && pokemon_idx <= MAX_POKEMON_INDEX)
            {
                FILE *const data_file = fopen(INPUT_FILE_PATH, "r");
                // Check whether the file was opened or not
                if (data_file != NULL)
                {
                    // Read the input file line by line,
                    // stop at the line number that matches pokemon_idx
                    int line_nr = 1;
                    char *rc_ptr = NULL;
                    for (rc_ptr = fgets(line, MAX_LINE_LENGTH, data_file);
                         rc_ptr != NULL && line_nr < pokemon_idx;
                         rc_ptr = fgets(line, MAX_LINE_LENGTH, data_file))
                    {
                        ++line_nr;
                    }
                    if (rc_ptr != NULL)
                    {
                        // Unless this was the last line in the file,
                        // the line will end with '\n' or '\r\n'
                        // Remove \n or \r\n
                        size_t length = strlen(line);
                        if (length >= 2)
                        {
                            if (line[length - 2] == '\r')
                            {
                                line[length - 2] = '\0';
                            }
                        }
                        if (length >= 1)
                        {
                            if (line[length - 1] == '\n')
                            {
                                line[length - 1] = '\0';
                            }
                        }
                        ret_obj = Py_BuildValue("s", line);
                    }
                    else
                    if (ferror(data_file) != 0)
                    {
                        snprintf(
                            line, MAX_LINE_LENGTH,
                            "I/O error while reading the Pokémon index file %s",
                            INPUT_FILE_PATH
                        );
                        PyErr_SetString(
                            PyExc_IOError,
                            line
                        );
                    }
                    else
                    {
                        PyErr_SetString(
                            PyExc_IndexError,
                            "Specified index not contained in index file"
                        );
                    }
                    fclose(data_file);
                }
                else
                {
                    snprintf(
                        line, MAX_LINE_LENGTH,
                        "Cannot open Pokémon index file %s",
                        INPUT_FILE_PATH
                    );
                    PyErr_SetString(
                        PyExc_FileNotFoundError,
                        line
                    );
                }
            }
            else
            {
                PyErr_SetString(
                    PyExc_ValueError,
                    "Invalid index. A valid index must be within the interval [1 - 720]."
                );
            }
        }
        else
        {
            PyErr_SetString(
                PyExc_ValueError,
                "Unparseable number"
            );
        }
        free(line);
    }
    else
    {
        PyErr_NoMemory();
    }

    return ret_obj;
}


/**
 * Method descriptions
 * https://docs.python.org/3/c-api/structures.html#c.PyMethodDef
 * 
 * Per method include:
 * - name of the method (when called in Python), char *
 * - pointer to C code of method, PyCFunction
 * - flags for call construction, int
 * - docstring contents, char *
 * 
 * flags:
 * METH_VARARGS: 2 PyObject values - self object + args
 * METH_KEYWORDS: self, args and dict with keyword arguments
 * METH_NOARGS: method without args
 * METH_O: method with a single object argument
 * & more
 */
static PyMethodDef module_methods[] =
{
    {
        "no_search",
        no_pokemon_search,
        METH_NOARGS,
        docstring_method_no_pokemon_search
    },
    {
        "lookup_no",
        find_pokemon_by_index,
        METH_VARARGS,
        docstring_method_find_pokemon_by_index
    },
    {
        NULL,
        NULL,
        0,
        NULL
    }
};

/**
 * Module specification
 * https://docs.python.org/3/c-api/module.html#c.PyModuleDef
 * 
 * - base, always PyModuleDef_HEAD_INIT
 * - name for the module (name with which it is imported), char *
 * - docstring for the module, char *
 * - module state, Py_ssize_t
 * - pointer to module-level functions, PyMethodDef
 * (more optional)
 * 
 * module states:
 * -1 for modules which don't support sub-interpreters
 * non-negative vals to be able to re-initialise modules
 */
static struct PyModuleDef module_definition =
{
    PyModuleDef_HEAD_INIT,
    module_name,
    docstring_module,
    -1,
    module_methods
};


/**
 * Initialization function
 *
 * - Must start with PyInit_ followed by module name
 * - Must match the 'name' in setup.py
 */
PyMODINIT_FUNC PyInit_pokemon_lookup(void)
{
    Py_Initialize();

    return PyModule_Create(&module_definition);
}

