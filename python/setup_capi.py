#!/usr/bin/env python3
"""
Setup script for C API version of OmNomNum Python wrapper.

This builds a native Python C extension using Python.h instead of CFFI.
"""

from setuptools import setup, Extension
import os

# Get the parent directory (where omnomnum C files are)
parent_dir = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))

# Source files (same as CFFI uses)
sources = [
    'omnomnum_capi.c',
    '../omnomnum.c',
    '../parser.c',
    '../scanner.c',
    '../scanner.def.c',
    '../scan.c',
    '../sds.c',
    '../dtoa.c',
    '../itoa.c',
    '../util.c',
    '../parser_compat.c',
    '../grisu2/grisu2.c',
    '../branchlut/branchlut.c',
]

# Include directories
include_dirs = [
    parent_dir,  # For omnomnum.h and sds.h
    os.path.join(parent_dir, 'grisu2'),
    os.path.join(parent_dir, 'branchlut'),
]

# Extension definition
omnomnum_capi = Extension(
    'omnomnum_capi',
    sources=sources,
    include_dirs=include_dirs,
    extra_compile_args=['-O3', '-std=c99'],
)

setup(
    name='omnomnum_capi',
    version='0.1.0',
    description='OmNomNum Python C API wrapper (for benchmarking)',
    ext_modules=[omnomnum_capi],
    zip_safe=False,
)
