# Shell

A simple bash shell I made for the sake of learning C programming.

## Table of Contents
- [Features](#features)
- [Installation](#installation)
- [Usage](#usage)
- [License](#License)


## Features

- **Write, Read, Update, and Delete** files from the system.
- **Execute bash commands** from the command line
- **Displays the path from the root** on the command line
- **Redirect input, output, and commands** in the same manner it's done in bash


## Installation

### Prerequisites

- You need to have `git` and `gcc` installed on your system.

### Steps

1.  **Clone the repository:**
    ```bash
    git clone [https://github.com/calargen05/shell](https://github.com/calargen05/shell)
    cd shell

2.  **Build the project:**
    ```bash
    make
    ```
    This will generate an object file called `shell.o` and an executable called `shell`.

3. **Clean the directory of compilation files:**
    ```bash
    make clean
    ```
    This will delete `shell.o` and `shell`.


## Usage

Run the executable from your terminal:

```bash
./shell
```


## License

This project is licensed under the [MIT License](LICENSE).
