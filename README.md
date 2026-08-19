# Introduction
CGPL_C is a personal solo implementation project separate from the original fourth semester [Aalborg University (AAU)](https://www.en.aau.dk) bachelor project to design and implement a compiler/interpreter. It was originally implemented in Java with a generated parse tree using ANTLR. This solution implements manual lexing and parsing. I am rewriting this in C because I think it's a fun learning experience to learn about both interpreters and to gain experience in C.

CGPL stands for _Card Game Prototyping Language_ and is a niche attempt at creating an interpreter specifically designed for rapid card game prototyping, although a very significant critiscism of the language at examination is that the entire initial language may as well have been implemented as a library in any existing arbitrary OOP-supporting language as opposed to as a standalone language.

The project is currently very WIP and unfinished.

# CGPL
WIP...

## Features
WIP...

## Syntax
WIP...

## Installation
To install the project, download or clone the repository and build it using the build instructions, then run the outputted executable and follow it's usage instructions.

## Build instructions
The project is built using the [make](https://makefiletutorial.com) build system. This requires _make_ to be installed on your machine. Navigate to the project root folder (make sure to unzip if downloaded manually) and run one of the following build targets below...

NOTE: Was not initially developed with -pedantic flag, so it may not work with all compilers, but it should be guaranteed to work with GCC. Make version: _GNU Make 4.4.1
Built for x86_64-w64-mingw32_

### Build targets

There exists four total build targets, _release_, _debug_, _clean_ and _test_ respectively. Copy paste and edit one of the following commands as desired:

```make
# Just typing 'make' will build the release target
make

# Build either release or debug
make (release|debug)

# Clean the '../obj' directory of objects (*.o) files and the exectuable in the root directory (can be combined with any other build target as desired)
make clean

# Builds and runs all the unit tests located in '../tests/unit_tests'. Both release and debug builds can be used for testing, although the debug target is most useful. Note that swapping between build targets might cause mismatched object files if not all source files are recompiled into their respective object files, so if there are suddenly errors just run _clean_ before trying again
make (release|debug) test
```

## Usage
WIP...

## License

As this is a for-fun/learning project and not a commercial product, feel free to edit and use it for whatever you want under the
[MIT](https://choosealicense.com/licenses/mit/) license.