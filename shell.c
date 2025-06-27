#include <stdio.h> // for input and output
#include <stdlib.h> // for exit(), EXIT_SUCCESS, malloc(), etc.
#include <string.h> // for strtok()
#include <unistd.h> // for fork(), execvp(), etc.
#include <sys/wait.h> // for wait()
#include <stdbool.h> // for bool datatype, true, and false
#include <fcntl.h> // for open() and its flags
#include <sys/stat.h> // For the mode_t argument in open()
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
void shell_execute(char** args, char* in, size_t buff);

// checks the input commands for input/output redirection
bool check_io(char** args);

// executes the redirection command(s)
void io_redirection(char** args);

// check the input commands for pipe redirection
bool check_pipe(char** args);

// allows for piping
void piping(char** args, size_t buff);


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
    (void)num; // marks num as intentionally unused to suppress warning
    input = strtok(input, "\n"); // gets rid of the newline character at the end of the input
    commands = shell_parse(input, length);
    shell_execute(commands, input, length);

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

void shell_execute(char** args, char* in, size_t buff) {
    // exits if the user enters "exit" as their command
    if (strcmp(args[0], "exit") == 0) {
        exit(EXIT_SUCCESS);
    }
    
    // checks for pipe command
    if (check_pipe(args)) {
        piping(args, buff);
        return;
    }
    
    // checks if the user entered cd
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

    pid_t pid = fork(); // process id; fork() allows for this c program and the user-inputted command to run simultaneously

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
            io_redirection(args);
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

bool check_io(char** args) {
    // get the length of the args list
    int n = 0;
    while (args[n] != NULL) {
        ++n;
    }

    // check for i/o redirection
    int i;
    for (i = 0; i < n; i++) {
        if (strcmp(args[i], "<") == 0 ||
            strcmp(args[i], ">") == 0 ||
            strcmp(args[i], ">>") == 0
            )
            return true;
    }
    return false;
}

void io_redirection(char** args) {
    // iteration var to loop through the args list
    int i = 0;

    while (args[i] != NULL) {
        if (strcmp(args[i], "<") == 0) {
            // check for missing filename
            if (args[i+1] == NULL) {
                fprintf(stderr, "shell: syntax error: missing file for input redirection\n");
                _exit(1);
            }
            // args[i+1] has to be the file since < is args[i]
            char* file_name = args[i+1];
            // opens the file and returns an integer
            int fd = open(file_name, O_RDONLY);
            if (fd == -1) {
                perror("open");
                _exit(EXIT_FAILURE);
            }
            // duplicates the file and redirects input from stdin to the file
            int duplicate = dup2(fd, 0);
            if (duplicate == -1) {
                perror("dup2");
                _exit(EXIT_FAILURE);
            }

            close(fd);

            int remove_iterable = i;
            while (args[remove_iterable + 2] != NULL) {
                args[remove_iterable] = args[remove_iterable + 2];
                ++remove_iterable;
            }
            args[remove_iterable] = NULL;
                
        }

        else if (strcmp(args[i], ">") == 0) {
            // check for missing filename
            if (args[i+1] == NULL) {
                fprintf(stderr, "shell: syntax error: missing file for output redirection\n");
                _exit(1);
            }
            // args[i+1] has to be the file since > is args[i]
            char* file_name = args[i+1];
            // opens the file and returns an integer
            int fd = open(file_name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd == -1) {
                perror("open");
                _exit(EXIT_FAILURE);
            }
            // duplicates the file and redirects output from stdout to the file
            int duplicate = dup2(fd, 1);
            if (duplicate == -1) {
                perror("dup2");
                _exit(EXIT_FAILURE);
            }

            close(fd);

            int remove_iterable = i;
            while (args[remove_iterable + 2] != NULL) {
                args[remove_iterable] = args[remove_iterable + 2];
                ++remove_iterable;
            }
            args[remove_iterable] = NULL; 
        }
        
        else if (strcmp(args[i], ">>") == 0) {
            // check for missing filename
            if (args[i+1] == NULL) {
                fprintf(stderr, "shell: syntax error: missing file for output redirection\n");
                _exit(1);
            }
            // args[i+1] has to be the file since < is args[i]
            char* file_name = args[i+1];
            // opens the file and returns an integer
            int fd = open(file_name, O_WRONLY | O_CREAT | O_APPEND, 0644);
            if (fd == -1) {
                perror("open");
                _exit(EXIT_FAILURE);
            }
            // duplicates the file and redirects input from stdin to the file
            int duplicate = dup2(fd, 1);
            if (duplicate == -1) {
                perror("dup2");
                _exit(EXIT_FAILURE);
            }

            close(fd);

            int remove_iterable = i;
            while (args[remove_iterable + 2] != NULL) {
                args[remove_iterable] = args[remove_iterable + 2];
                ++remove_iterable;
            }
            args[remove_iterable] = NULL;            
        }
        else {
            // incase no i/o redirection string was found, just increment the loop by 1
            ++i;
        }
    }
    execvp(args[0],args);
    perror("execvp");
    _exit(1);
}

bool check_pipe(char** args) {
    int n = 0;
    while (args[n] != NULL) {
        if (strcmp(args[n], "|") == 0)
            return true;
        ++n;
    }
    return false;
}

void piping(char** args, size_t buff) {
    // add function content here
    
    // pipe file descriptor
    int pipefd[2];
    int pipe_out = pipe(pipefd); // pipe function to pipe output from one command to another
    
    // list of strings for the left side of the pipe
    char** left = malloc(buff*sizeof(char*));
    // list of strings for the right side of the pipe
    char** right = malloc(buff*sizeof(char*));
    
    // loops to get the left and right lists
    int i = 0; int left_i = 0; int right_i = 0;
    while (strcmp(args[i], "|") != 0) {
        left[left_i] = args[i];
        ++left_i;
        ++i;
    }
    left[left_i] = NULL; // null termination
    
    // advances i past the "|" string
    if (args[i] != NULL && strcmp(args[i], "|") == 0)
        i++;

    // checks for missing command on the right side of the pipe
    if (args[i] == NULL) {
        fprintf(stderr, "shell: syntax error: missing command on right side of the pipe.");
        exit(1);
    }
    while (args[i] != NULL) {
        right[right_i] = args[i];
        right_i++;
        i++;
    }
    right[right_i] = NULL; // null termination



    // error checking
    if (pipe_out == -1) {
        perror("pipe");
        exit(1);
    }
    
    // forks the process
    pid_t p_id = fork();
    
    // error checking
    if (p_id < 0) {
        perror("fork");
        exit(1);
    }
    // child process
    if (p_id == 0) {
        close(pipefd[0]); // closes read end of the pipe
        int duplicate = dup2(pipefd[1], 1); // uses dup2() to redirect stdout to the write end of the pipe
        
        // error checking
        if (duplicate == -1) {
            perror("dup2");
            _exit(1);
        }
        // closes the write end of the pipe
        close(pipefd[1]);

        execvp(left[0], left); // executes command
        perror("execvp");
        _exit(1);
    }
    else {
        p_id = fork(); // second fork for read end of pipe
        
        // error checking
        if (p_id < 0) {
            perror("fork");
            exit(1);
        }
        // child process
        if (p_id == 0) {
            close(pipefd[1]); // closes write end of the pipe
            int duplicate = dup2(pipefd[0], 0); // uses dup2() to redirect stdin to the read end of the pipe

            // error checking
            if (duplicate == -1) {
                perror("dup2");
                _exit(1);
            }
            
            // closes read end of the pipe
            close(pipefd[0]);
            execvp(right[0], right); // execute commands
            perror("execvp");
            _exit(1);
        }
        else {
            // close both ends of the pipe
            close(pipefd[0]);
            close(pipefd[1]);
            free(left); free(right);
            
            // two wait calls because the parent needs to wait for both children
            wait(NULL);
            wait(NULL);
        }
    }
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
