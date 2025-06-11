/* 
 * Im going to approach this project in this manner:
 * Mini Shell:
 *  first: make a for loop in int main that loops until the user inputs 'exit'
 *  second: make a function that prints the shell statement (ex: largen@localhost~>)
 *  third: make a function that parses the input to check for an input that the shell will recognize
 *  fourth: execute the command
 *
 * Formatting:
 *  I will make a 'container function' that has the other functions all run inside of it to keep the main function clean
 *  I think this is a good idea so that the code stays clean where it needs to and I can just add comments where I need to
 *
 * Questions:
 *  how will i make my program link to my system?
 *  how will i check for buffer overflow in the shell_input function?
 *  what datatype will i use for my shell_parse function?
 *  how will i parse the string for the command in the shell_parse function?
 *  
 * */

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

int main(int argc, char** argv) {
    // while loop to
    while (argv != "exit") {
        shell_main();
    }

    return 0;
}
