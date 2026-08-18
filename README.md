# Introduction
CGPL_C is a personal solo implementation project separate from the original fourth semester [Aalborg University (AAU)](https://www.en.aau.dk) bachelor project to design and implement a compiler/interpreter. It was originally implemented in Java with a generated parse tree using ANTLR. This solution implements manual lexing and parsing.

# CGPL
WIP...

## Syntax
WIP...

## Installation
To install the project, download or clone the repository and build it using the build instructions, then run the outputted executable and follow it's usage instructions.

## Build instructions
The project is built using the [make](https://makefiletutorial.com) build system. This requires _make_ to be installed on your machine. Navigate to the project root folder (make sure to unzip if downloaded manually) and run one of the following build targets below...

### Build targets

There exists four total build targets, _release_, _debug_, _clean_ and _test_ respectively. Copy paste and edit one of the following commands as desired:

```make
# Just typing 'make' will build the release target
make

# Build either release or debug
make (release|debug)

# Clean the directory of objects (*.o) files and exectuables (can be combined with any other build target as desired)
make clean

# Builds and runs the unit tests in '../tests/unit_tests'. Both release and debug builds can be used for testing, although debug target is most useful.
make (release|debug) test
```

## Usage
WIP...

## License

As this is a for-fun/learning project and not a commercial product, feel free to edit and use it for whatever you want under the
[MIT](https://choosealicense.com/licenses/mit/) license.