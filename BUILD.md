
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

### Conan options

Here are the available options:

| Option           | Default | Possible Values  | Explanations |
| -----------------|:------- |:----------------:|--------------|
| shared           | True    |  [True, False]   | Build as shared library |

### Using Conan profiles

When using Conan for dependency management,
it is recommended to use Conan profiles.
This permits to have personal binary repository,
avoiding to recompile everything everytime.
This becomes more important for dependencies like Qt.

This requires modifications in the `settings.yml` Conan configuration,
and also some profile files.
See my [conan-config repository](https://gitlab.com/scandyna/conan-config) for more informations.

Some following sections will rely on Conan profiles.

### Install the dependencies with Conan and configure the build

This is a example on Linux using gcc:
```bash
conan install --profile:build linux_gcc8_x86_64 --profile:host linux_ubuntu-18.04_gcc8_x86_64_qt_widgets_modules_boost --settings:build build_type=Release --settings:host build_type=Debug --build=missing ..
```

Configure MdtExecutableFile:
```bash
source conanbuild.sh
cmake -DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake -DBUILD_TESTS=ON -DCMAKE_BUILD_TYPE=Debug ..
cmake-gui .
```

## Build and run the tests

Those examples use cmake to run the build,
which should work everywhere.

Build:
```bash
cmake --build . --config Debug -j4
```

To run the tests:
```bash
ctest --output-on-failure -C Debug -j4
```

## Restore your environment

On Unix:
```bash
source deactivate_conanbuild.sh
```
On Windows:
```bash
.\deactivate_conanbuild
```
