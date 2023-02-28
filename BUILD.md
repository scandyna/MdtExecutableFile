
[[_TOC_]]

# Build MdtExecutableFile

This section explains how to build MdtExecutableFile
and run the unit tests.

## Required tools and libraries

Some tools are required to build MdtExecutableFile:
 - Git
 - Conan
 - CMake
 - Make (can be other, like Ninja)
 - C++ compiler (like Gcc or Clang or MSVC)

Additional tools are required to generate the documentation:
 - Doxygen
 - Sphinx

For a overview how to install them, see https://gitlab.com/scandyna/build-and-install-cpp

Tools and libraries that are managed by Conan:
 - [Catch2](https://github.com/catchorg/Catch2)
 - Qt
 - [MdtCMakeModules](https://gitlab.com/scandyna/mdt-cmake-modules)
 - [MdtCMakeConfig](https://gitlab.com/scandyna/mdtcmakeconfig)


## Get the source code

Get the sources:
```bash
git clone https://gitlab.com/scandyna/mdtexecutablefile.git
```

Create a build directory and cd to it:
```bash
mkdir build
cd build
```

## Install the dependencies

The recommended way is to use Conan to install the dependencies.
The examples are based on that.

Note: there is a `conanfile.py` at the root of the project tree.
This file is only used to install dependencies to work on MdtExecutableFile.
To create Conan packages, see [README](packaging/conan/README.md) in the `packaging/conan` directory.
