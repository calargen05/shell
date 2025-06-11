#include <stdio.h> // for input and output
#include <stdlib.h> // for exit(), EXIT_SUCCESS, malloc(), etc.
#include <string.h> // for strtok()
#include <unistd.h> // for fork(), execvp(), etc.
#include <sys/wait.h> // for wait()
#include <stdbool.h> // for true, false

// container function; contains all the other functions in it
void shell_main();

// prints the shell statement
void shell_print();

// get the user input and return it
ssize_t shell_input(char** in, size_t* len);

// parses the input for a command and returns a list of tokens
char** shell_parse(char* in, size_t buff);

// executes the input in the args list
void shell_execute(char** args);


int main(int argc, char* argv[]) {

    do {
        shell_main();
    }
    while(true);

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
    input = strtok(input, "\n"); // gets rid of the newline character at the end of the input
    commands = shell_parse(input, length);
    shell_execute(commands);

    // free the buffer
    free(input);
    free(commands);
}

void shell_print() {
    printf("user@domain ~> ");
}

ssize_t shell_input(char** in, size_t* len) {
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

    if (token == "exit") {
        printf("Successfully Exited\n");
        exit(EXIT_SUCCESS);
    }

    while (token != NULL) {
        tokens[n] = token; // append the token to tokens
        ++n; // iterate n
        token = strtok(NULL, " "); // get next token
    }
    return tokens;
}

void shell_execute(char** args) {
    // TODO: make function and comment for clarity
    // add code here
    pid_t pid = fork(); // process id; allows for this c program and the user-inputted command to run simultaneously

    if (pid < 0) {
        perror("Error: couldn't fork");
        exit(EXIT_FAILURE);
    }
    else if (pid == 0) {
        // child process    
        if (execvp(args[0], args) == -1) {
            perror("execution failed");
            exit(EXIT_FAILURE);
        }
    }
    else { 
        wait(NULL);
    }

}
