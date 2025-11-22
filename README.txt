Unix Shell

A simple Unix shell built in C++ with command execution and pipeline support.

Features

- Execute external programs with unlimited arguments
- Chain commands with pipes (`ls | wc -l`)
- Built-in commands: `cd`, `path`, `help`, `exit`
- Dynamic PATH management

Build & Run

make
./shell

Quick Start

$ path /bin /usr/bin          # Set up your PATH
$ ls                          # Run commands
$ ls | wc -l                  # Use pipes
$ cd Documents                # Change directory
$ exit                        # Exit shell

Examples

$ ls | wc -l                  # Count files
$ ls -la | head -5            # Show first 5 files
$ cat file.txt | sort         # Sort file contents

Technical Details

System calls used: fork(), execvp(), pipe(), dup2(), wait(), chdir(), getcwd()

Files:
- `commands.cpp` - Command execution and pipeline logic
- `commands.hpp` - Header file
- `main.cpp` - Input parsing and routing
- `Makefile` - Build config

Working On:

- I/O redirection (`>`, `<`)
- background processes (`&`)
- signal handling
- environment variables

Compatible with C++98 and later.