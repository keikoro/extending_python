#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Look up Pokémon by their index number.

Companion Python code to extend.c, which contains the underlying C code.
First try at extending Python with C.

Copyright (c) 2016 Kerstin Kollmann <code∆kerstinkollmann·com>
"""

import pokemon_lookup as pi

# prompt for a number
print("Find a Pokémon based on its index number?")
print("Input YES or NO:")
choice = input()

if choice:
    if choice.lower().startswith('y', 0, 1):
        index = input("Enter a number to look up: ")
        if index:
            poke = pi.lookup_no(int(index))
            print("Pokémon #{} is {}.".format(index, poke))
        else:
            print("Not a valid number.")
    elif choice.lower().startswith('n', 0, 1):
        pi.no_search()
    else:
        print("That... was not a valid choice.")
else:
    print("No valid input, quitting program.")