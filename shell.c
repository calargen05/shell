#include <stdio.h>
#include <stdlib.h>

// container function; contains all the other functions in it
void shell_main();

// prints the shell statement
void shell_print();

// get the user input and return it
ssize_t shell_input(char** in, size_t* len);


/* TODO: figure out what datatype this function should be
 * 
   parses the input
datatype shell_parse();
*/

int main(int argc, char* argv[]) {

    do {
        shell_main();
    }
    while(0);

    return 0;
}

void shell_main() {
    // add code for shell function here

    //variables
    ssize_t num; // input from the user
    char* input = NULL; // input from the user (pointer to the buffer)
    size_t length = 0; // length of the buffer

    shell_print();
    num = shell_input(&input, &length);

    // test
    printf("inputted string: %s\nnumber of characters inputted: %zu\nlength of the buffer: %zd\n", input, num, length);


    // free the buffer
    free(input);
}

void shell_print() {
    printf("user@domain ~> ");
}

ssize_t shell_input(char** in, size_t* len) {
    // TODO: figure out how to check for buffer overflow
    return getline(in, len, stdin);
}

/*
datatype shell_parse() {
    add code for shell here

}
*/
