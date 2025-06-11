#include <stdio.h>
#include <stdlib.h>

// container function; contains all the other functions in it
void shell_main() {
    // add code for shell function here
    shell_print();
}

// prints the shell statement
void shell_print() {
    printf("user@domain ~> ");
}

/*
// get the user input and return it
char* shell_input(char[] input) {
    // TODO: figure out how to check for buffer overflow
    scanf("%s", input);
    return input;
}
*/

/* TODO: figure out what datatype this function should be
 * 
// parses the input
datatype shell_parse() {
    // add code for shell
}
*/

int main(int argc, char* argv[]) {
    // while loop to
    while (argv != "exit") {
        shell_main();
    }

    return 0;
}
