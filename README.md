# Extending Python (with C) – example project

C extension for a module with a method to **look up Pokémon by their index number**.

Project based on information found in [a blog post on writing extensions](http://adamlamers.com/post/NUBSPFQJ50J1) by Adam Lamers, the official [Python documentation on building C and C++ extension](https://docs.python.org/3/extending/building.html) and various other bits and pieces on the topic found online.


## Installation

Make sure to work in a virtual environment if you want to install the extension with `pip`.

In your venv, run:

```
$ pip install .
```

to have `pip` install the extension using `setup.py`.

Alternatively, you can run `setup.py` manually with:

```
$ python3 setup.py build_ext --inplace
```

to have the C code compiled into a shared object file (`.so`) in your venv.


## Usage

To start the Python script with which you can test the module, use:

```
$ python3 test_extension.py
```

## Resources

The Pokédex (index or "catalogue" of all Pokémon) used in this example project is based on the [list of Pokémon](http://pokemon.wikia.com/wiki/List_of_Pok%C3%A9mon) found on [The Pokémon Wiki](http://pokemon.wikia.com/wiki/Pok%C3%A9mon_Wiki).