#include <stdio.h> // for input and output
#include <stdlib.h> // for exit(), EXIT_SUCCESS, malloc(), etc.
#include <string.h> // for strtok()
#include <unistd.h> // for fork(), execvp(), etc.
#include <sys/wait.h> // for wait()

// container function; contains all the other functions in it
void shell_main();

// prints the shell statement
void shell_print();

// get the user input and return it
ssize_t shell_input(char** in, size_t* len);

// parses the input for a command and returns a list of tokens
char** shell_parse(char* in, size_t buff);

// executes the input in the args list
void shell_execute(char** args, char* in);


int main(int argc, char* argv[]) {

    do {
        shell_main();
    }
    while(1);

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
    shell_execute(commands, input);

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
        free(tokens); free(in);
        exit(EXIT_FAILURE);
    }

    while (token != NULL) {
        tokens[n] = token; // append the token to tokens
        ++n; // iterate n
        token = strtok(NULL, " "); // get next token
    }
    tokens[n] = NULL; // null terminaton for the execution function below
    return tokens;
}

void shell_execute(char** args, char* in) {
    // TODO: make function and comment for clarity
    
    // exits if the user enters "exit" as their command
    if (strcmp(args[0], "exit") == 0) {
        free(args); free(in);
        exit(EXIT_SUCCESS);
    }

    pid_t pid = fork(); // process id; allows for this c program and the user-inputted command to run simultaneously

    // checks for errors in forking
    if (pid < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    else if (pid == 0) {
        // child process;
        // check if the command entered is 'cd'
        if (strcmp(args[0], "cd") == 0) {
            // variable to get the status of chdir()
            int changed_dir;
            // loop through the args array and find the path
            int n = 1;
            while(args[n] != NULL) {
                if (args[n][0] != '-')
                    break;
                else
                    n++;
            }
            // debugging print statement
            printf("%s\n", args[n]);
            changed_dir = chdir(args[n]);
            // checks if chdir() failed
            if (changed_dir == -1) {
                perror("chdir");
                exit(EXIT_FAILURE);
            }
            if (changed_dir == 0)
                printf("directory was changed to: %s\n", args[n]);
        }
        else { 
            execvp(args[0], args);
            perror("execvp"); // prints if execvp fails
            exit(EXIT_FAILURE);
        }
    }
    else {
        // parent process; waits for the child process to finish
        wait(NULL);
    }

}
