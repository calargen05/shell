#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// container function; contains all the other functions in it
void shell_main();

// prints the shell statement
void shell_print();

// get the user input and return it
ssize_t shell_input(char** in, size_t* len);

// parses the input for a command and returns a list of tokens
char** shell_parse(char* in, size_t buff);


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
    
    char** commands; // command to execute

    shell_print();
    num = shell_input(&input, &length);
    commands = shell_parse(input, length);

    /* test
    int n = 0;
    while (commands[n] != NULL) {
        printf("command%d: %s\n", n, commands[n]);
        ++n;
    }
    */
    

    // free the buffer
    free(input);
    free(commands);
}

void shell_print() {
    printf("user@domain ~> ");
}

ssize_t shell_input(char** in, size_t* len) {
    // TODO: figure out how to check for buffer overflow
    return getline(in, len, stdin);
}


char** shell_parse(char* in, size_t buff) {
    // vars
    char* token = strtok(in, " "); // tokens for tokenizing the input
    char** tokens = malloc(buff*sizeof(char*)); // list of tokens
    int n=0; // num of tokens; used to index 'tokens'

    if (!token) {
        perror("Error: couldn't tokenize input");
        exit(EXIT_FAILURE);
    }

    while (token != NULL) {
        tokens[n] = token; // append the token to tokens
        ++n; // iterate n
        token = strtok(NULL, " "); // get next token
    }
    return tokens;
}
