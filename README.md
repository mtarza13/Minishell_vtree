# Minishell

A simple shell implementation in C that mimics the behavior of bash, created as part of the 42 School curriculum.

## Table of Contents

- [Overview](#overview)
- [Features](#features)
- [Requirements](#requirements)
- [Installation](#installation)
- [Usage](#usage)
- [Built-in Commands](#built-in-commands)
- [Error Handling](#error-handling)
- [Project Structure](#project-structure)
- [Testing](#testing)
- [Authors](#authors)
- [License](#license)

## Overview

Minishell is a simplified version of a Unix shell that provides basic command-line functionality. This project demonstrates understanding of process management, file descriptors, signal handling, and parsing in C.

## Features

### Mandatory Features

- **Prompt Display**: Shows a prompt when waiting for new commands
- **Command History**: Working history of executed commands
- **Executable Search**: Finds and launches executables based on PATH variable or relative/absolute paths
- **Global Variables**: Minimal use of global variables (only for signal handling)
- **Quote Handling**: 
  - Single quotes (`'`) prevent interpretation of meta-characters
  - Double quotes (`"`) prevent interpretation except for `$`
- **Redirections**:
  - `<` redirects input
  - `>` redirects output
  - `<<` heredoc (reads input until delimiter)
  - `>>` redirects output in append mode
- **Pipes**: `|` character pipes output of each command to input of next command
- **Environment Variables**: `$` followed by variable name expands to variable value
- **Exit Status**: `$?` expands to exit status of most recently executed foreground pipeline
- **Signal Handling**:
  - `ctrl-C` displays new prompt on new line
  - `ctrl-D` exits the shell
  - `ctrl-\` does nothing

### Built-in Commands

- `echo` with option `-n`
- `cd` with only relative or absolute path
- `pwd` with no options
- `export` with no options
- `unset` with no options
- `env` with no options or arguments
- `exit` with no options

## Requirements

- **Language**: C
- **Compiler**: gcc with flags `-Wall -Wextra -Werror`
- **External Libraries**: 
  - readline (for command-line editing and history)
  - Standard C library functions
- **System**: Unix-like operating system (Linux, macOS)

## Installation

1. Clone the repository:
```bash
git clone <repository-url>
cd minishell
```

2. Compile the project:
```bash
make
```

3. Run the executable:
```bash
./minishell
```

## Usage

### Basic Commands
```bash
$ ls -la
$ echo "Hello World"
$ pwd
$ cd /path/to/directory
```

### Redirections
```bash
$ echo "Hello" > file.txt
$ cat < file.txt
$ echo "World" >> file.txt
$ cat << EOF
heredoc content
EOF
```

### Pipes
```bash
$ ls -la | grep "txt"
$ cat file.txt | wc -l
```

### Environment Variables
```bash
$ echo $HOME
$ echo $USER
$ export MY_VAR="value"
$ echo $MY_VAR
```

### Exit Status
```bash
$ ls non_existent_file
$ echo $?
```

## Built-in Commands

### `echo`
Displays a line of text.
```bash
$ echo "Hello World"
$ echo -n "No newline"
```

### `cd`
Changes the current directory.
```bash
$ cd /path/to/directory
$ cd ..
$ cd ~
```

### `pwd`
Prints the current working directory.
```bash
$ pwd
```

### `export`
Sets environment variables.
```bash
$ export VAR="value"
```

### `unset`
Removes environment variables.
```bash
$ unset VAR
```

### `env`
Displays environment variables.
```bash
$ env
```

### `exit`
Exits the shell.
```bash
$ exit
$ exit 42
```

## Error Handling

The shell handles various error conditions gracefully:

- **Command not found**: Displays appropriate error message
- **Permission denied**: Shows permission error for non-executable files
- **Invalid redirections**: Handles file access errors
- **Syntax errors**: Reports parsing errors for invalid command syntax
- **Memory allocation**: Proper cleanup on memory allocation failures

## Project Structure

```
minishell/
├── src/
│   ├── main.c
│   ├── parser/
│   ├── executor/
│   ├── builtins/
│   ├── signals/
│   └── utils/
├── includes/
│   └── minishell.h
├── Makefile
└── README.md
```

## Testing

### Basic Functionality
- Test all built-in commands
- Verify pipe functionality with multiple commands
- Test all redirection types
- Check quote handling (single and double)
- Verify environment variable expansion

### Edge Cases
- Empty commands
- Multiple consecutive spaces/tabs
- Unclosed quotes
- Invalid redirections
- Signal handling during command execution

### Memory Management
- Run with valgrind to check for memory leaks
- Test extensive command sequences
- Verify proper cleanup on exit

## Authors

- [Your Name] - [Your GitHub](https://github.com/yourusername)

## License

This project is part of the 42 School curriculum and is subject to the school's academic policies.

---

**Note**: This implementation focuses on the mandatory requirements of the minishell project. Advanced features like logical operators (`&&`, `||`), wildcards, or advanced job control are not included in this version.