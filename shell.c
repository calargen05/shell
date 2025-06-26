#include <stdio.h> // for input and output
#include <stdlib.h> // for exit(), EXIT_SUCCESS, malloc(), etc.
#include <string.h> // for strtok()
#include <unistd.h> // for fork(), execvp(), etc.
#include <sys/wait.h> // for wait()
#include <stdbool.h>
#define CWD_BUFFSIZE 4096

// ** NOTES AT THE BOTTOM **

// container function; contains all the other functions in it
void shell_main();

// prints the shell statement
void shell_print(char** curr);

// get the user input and return it
ssize_t shell_input(char** in, size_t* len);

// parses the input for a command and returns a list of tokens
char** shell_parse(char* in, size_t buff);

// executes the input in the args list
void shell_execute(char** args, char* in);

// checks the input commands for input/output redirection
bool check_io(char** args);

// executes the redirection command(s)
void io_redirection(char** args);


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
    char* cwd; // current working directory

    shell_print(&cwd);
    num = shell_input(&input, &length);
    input = strtok(input, "\n"); // gets rid of the newline character at the end of the input
    commands = shell_parse(input, length);
    shell_execute(commands, input);

    // free the buffer
    free(input);
    free(commands);
    free(cwd);
}

void shell_print(char** curr) {
    // TODO: get the current working directory and add it to the print statement at the end of the function
    *curr = malloc(CWD_BUFFSIZE);
    getcwd(*curr, CWD_BUFFSIZE);

    printf("user@domain %s~> ", *curr);
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
    // exits if the user enters "exit" as their command
    if (strcmp(args[0], "exit") == 0) {
        exit(EXIT_SUCCESS);
    }
    
    // checks if the user entered
    if (strcmp(args[0], "cd") == 0) {
        // variable to get the status of chdir()
        int changed_dir;
            // loop through the args array and find the path
        int n = 1;
        while (args[n] != NULL) {
            if (args[n][0] != '-')
                break;
            else
                n++;
        }
        // debugging print statement
        changed_dir = chdir(args[n]);
        // checks if chdir() failed
        if (changed_dir == -1) {
            perror("chdir");
            exit(EXIT_FAILURE);
        }
        return;
    }

    pid_t pid = fork(); // process id; allows for this c program and the user-inputted command to run simultaneously

    // checks for errors in forking
    if (pid < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    // checks to see if there is a command other than 'cd'
    else if (pid == 0) {
        // child process;
        
        // check if the args contains i/o redirection
        if (check_io(args)) {
            // execute the commmands
            // io_redirection(args);
        execvp(args[0], args);
        perror("execvp"); // prints if execvp fails
        exit(EXIT_FAILURE);
    }
    else {
        // parent process; waits for the child process to finish
        wait(NULL);
    }

}

bool check_io(char** args) {
    // get the length of the args list
    int n = 0;
    while args[n] != NULL
        ++n;

    // check for i/o redirection
    int i;
    for (i = 0; i < n: i++) {
        if (args[i] == "<" || args[i] == "<<" || args[i] == ">" || args[i] == ">>")
            return true;
    }
    return false;
}

void io_redirection(char** args) {
    // write this function
}

/* NOTES
 *
 * I moved the cd execution to before the fork so the command would actually work
 * If I had left it there, the directory wouldn't change because it was a child process in this program. Child processes only affect the child, and the intention is for the command to change in the parent (the shell).
 *
 * I also found a double free (thanks to chatgpt for the assist). Needless to say, I got rid of it and implemented a safer approach to memory management by just freeing the memory in the shell_main() function.
 *
 * CURRENT: I'm currently learning how to handle i/o redirection and ai is helping me learn without writing the code for me ;)
 *
 * */
