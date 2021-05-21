pdbrn
=====
![build workflow](https://github.com/stcmz/pdbrn/actions/workflows/build.yml/badge.svg)
![release workflow](https://github.com/stcmz/pdbrn/actions/workflows/release.yml/badge.svg)

pdbrn is a command line tool for renumbering protein sequence in PDB/PDBQT molecular models.


Features
--------

* allow multiple rules for renumbering residues (amino acids)
* ability to delete unwanted partial protein sequence
* support PDB and PDBQT formats


Supported operating systems and compilers
-----------------------------------------

All systems with compilers in conformance with the C++17 standard, e.g.
* Linux x86_64 and g++ 8.3.1 or higher
* Mac OS X x86_64 and clang 7 or higher
* Windows x86_64 and msvc 19.14 or higher


Compilation from source code
----------------------------

### Build with CMake

This project uses cross-platform build system CMake to build from source. It detects your environment and decides the most appropriate compiler toolset. The minimum version of CMake required is `3.15`. To build, simply run
```
cmake -B build
cmake --build build --config Release
```

The generated objects and executable will be placed in the `build` folder.

Optionally, on Linux or macOS one may install the output binary to the system (usually `/usr/local/bin`) by running
```
sudo cmake --install build
```

On Windows, the script should be run without sudo but under Administrator. The executable will be copied to an individual directory under `Program Files`.


### Build with Visual Studio

Visual Studio 2019 solution and project files are provided. To compile, simply run
```
msbuild /t:Build /p:Configuration=Release
```

Or one may open `pdbrn.sln` in Visual Studio 2019 and do a full rebuild.

The generated objects will be placed in the `obj` folder, and the generated executable will be placed in the `bin` folder.


Usage
-----

First add pdbrn to the PATH environment variable.

To display a full list of available options, simply run the program with the `--help` argument
```
pdbrn --help
```

See the [Features section](#features) above for usages in different input ways.


Author
--------------

[Maozi Chen]


[Maozi Chen]: https://www.linkedin.com/in/maozichen/
