#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import os
from setuptools import Command, setup
from distutils.core import Extension

class cleanup(Command):
    """Get rid of unneeded files after running setup.py."""
    user_options = []

    def initialize_options(self):
        pass

    def finalize_options(self):
        pass

    def run(self):
        os.system("rm -rfv ./build ./dist ./*.pyc ./*.tgz ./*.egg-info")

setup(
    name='pokemon_lookup',
    description='Extending Python with a Pokemon lookup function.',
    ext_modules=[Extension('pokemon_lookup', ['extend.c'])],
    version='1.0',
    author='K Kollmann',
    author_email='code∆k.kollmann·moe',
    url='https://github.com/kerstin/extending_python',
    license='',

    cmdclass={
        'clean': cleanup
    }
)
