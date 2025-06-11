# Mini Shell Design Document

## Purpose
Build a minimal shell in C that supports basic commands.

## Planned Features
- [x] Read user input
- [x] Parse input
- [x] Execute commands
- [x] Wait for child processes

## Architechture
### General Formatting
- Function that contains all other functions in the input loop for cleanliness

### Input Loop
- Using `getline`()` for dynamic input
- Prompts: `user@domain ~> ` via a function
- Parsed using `strtok()` into `commands` array

## Command Execution
- Using `execvp()` and `fork()` for executing commands
- Using `wait()` to let child processes execute


Currently lightweight, but I plan on making the shell more robust as I learn about c and its intricacies.
