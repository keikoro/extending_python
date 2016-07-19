/*
extend.c

Extending Python with C code
A first attempt.

Create a module with a method to look up Pokémon by their index number.

Copyright (c) 2016 Kerstin Kollmann <code∆kerstinkollmann·com>
*/

#include <Python.h>

#define MAXCHARS 255
#define MAXPOKE 720


/* function declarations */
static PyObject* no_poke_search(PyObject *self, PyObject *args);
static PyObject* find_pokemon_byno(PyObject *self, PyObject *args);


/* module name */
static char module_name[] = "pokemon_lookup";


/* docstrings for Python (module + methods) */
static char docstring_module[]
            = "A module to test extending Python with C. "
              "Let's you look up Pokémon by number.";
static char docstring_method_no_poke_search[]
            = "Prints out a test message.";
static char docstring_method_find_pokemon_byno[]
            = "Finds a Pokémon by its index number.";


/* C code of test print method */
static PyObject* no_poke_search(PyObject *self, PyObject *args)
{
    char *text = (char *) malloc(MAXCHARS * sizeof(char));

    text = "No Pokémon for you!";
    printf("%s\n", text);

    Py_RETURN_NONE;
}

/* C code of number print method */
static PyObject* find_pokemon_byno(PyObject *self, PyObject *args)
{
    FILE *input;
    char inputfilename[] = "pokedex.txt";

    int count = 1, pi = 1;
    char *pokemon = (char *) malloc(MAXCHARS * sizeof(char));
    char line[MAXCHARS] = "\0";

    if (!PyArg_ParseTuple(args, "i", &pi))
    {
        return NULL;
    }

    /* out of poke range */
    if (pi < 1 || pi > MAXPOKE)
    {
        PyErr_SetString(PyExc_ValueError,
                        "a value between 1 and 720 is required");
        return NULL;
    }
    /* within poke range */
    else
    {
        /*  check for existence of inputfilename */
        if ((input=fopen(inputfilename, "r")) != NULL)
        {
            /*
                loop through lines of input file,
                stop when input number matches line number
                ! line will include the newline character !
            */
            input=fopen(inputfilename, "r");
            while (fgets(line, sizeof(line) + 2, input)) {
                if (count == pi)
                {
                    pokemon = line;
                    break;
                }
                count++;
            }
            fclose(input);
        }
        /* throw a FileNotFoundError if input file is not found */
        else
        {
            PyErr_SetString(PyExc_FileNotFoundError,
                            "file containing Pokémon index not found");
            return NULL;
        }
    }

    /* remove trailing newline from pokemon string */
    pokemon[strcspn(pokemon, "\n")] = 0;

    return Py_BuildValue("s", pokemon);
}


/*
Method descriptions
https://docs.python.org/3/c-api/structures.html#c.PyMethodDef

Per method include:
- name of the method (when called in Python), char *
- pointer to C code of method, PyCFunction
- flags for call construction, int
- docstring contents, char *

flags:
METH_VARARGS: 2 PyObject values - self object + args
METH_KEYWORDS: self, args and dict with keyword arguments
METH_NOARGS: method without args
METH_O: method with a single object argument
& more
*/
static PyMethodDef module_methods[] =
{
    {
        "no_search",
        no_poke_search,
        METH_NOARGS,
        docstring_method_no_poke_search
    },
    {
        "lookup_no",
        find_pokemon_byno,
        METH_VARARGS,
        docstring_method_find_pokemon_byno
     },
    {
        NULL,
        NULL,
        0,
        NULL
    }
};

/*
Module specification
https://docs.python.org/3/c-api/module.html#c.PyModuleDef

- base, always PyModuleDef_HEAD_INIT
- name for the module (name with which it is imported), char *
- docstring for the module, char *
- module state, Py_ssize_t
- pointer to module-level functions, PyMethodDef
(more optional)

module states:
-1 for modules which don't support sub-interpreters
non-negative vals to be able to re-initialise modules
*/
static struct PyModuleDef module_definition =
{
    PyModuleDef_HEAD_INIT,
    module_name,
    docstring_module,
    -1,
    module_methods
};


/*
initialisation function
needs to
- start with PyInit_ followed by module name
- match the 'name' in setup.py
*/
PyMODINIT_FUNC PyInit_pokemon_lookup(void)
{
    Py_Initialize();

    return PyModule_Create(&module_definition);
}