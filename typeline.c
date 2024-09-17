// typline command program
// Q3 (4 )
// Write a C program to implement the shell which displays the command
// prompt “myshell$”. It accepts the command, tokenize the command line and
// execute it by creating the child process. Also implement the additional command
// ‘typeline’ as
// typeline +n filename :- To print first n lines in the file.
// typeline -a filename :- To print all lines in the file.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

// Define constants for maximum line length and maximum number of arguments
#define MAX_LINE 1024
#define MAX_ARGS 100

// Function prototype for typeline_command
void typeline_command(char *arg1, char *arg2);

int main() {
    // Initialize variables
    char line[MAX_LINE];
    char *command;

    // Infinite loop to continuously prompt user for input
    while (1) {
        // Print the prompt
        printf("myshell$ ");
        fflush(stdout);

        // Read input from user
        if (fgets(line, sizeof(line), stdin) == NULL) {
            perror("fgets");
            exit(EXIT_FAILURE);
        }

        // Remove trailing newline character
        line[strcspn(line, "\n")] = '\0';

        // Tokenize the input
        command = strtok(line, " ");

        // Check for exit command
        if (strcmp(command, "exit") == 0) {
            break;
        }

        // Check for typeline command
        if (strcmp(command, "typeline") == 0) {
            // Get the arguments for the typeline command
            char *arg1 = strtok(NULL, " ");
            char *arg2 = strtok(NULL, " ");
            if (!arg1 || !arg2) {
                printf("Usage: typeline +n filename OR typeline -a filename\n");
                continue;
            }
            typeline_command(arg1, arg2);
        } else {
            // Create a child process to execute the command
            pid_t pid = fork();
            if (pid == 0) {
                // In child process
                char *args[MAX_ARGS];
                int i = 0;

                // Tokenize the input into an array of arguments
                args[i++] = command;
                while ((args[i++] = strtok(NULL, " ")) != NULL);

                args[i] = NULL; // execvp expects a NULL-terminated array

                // Execute the command
                execvp(command, args);
                perror("execvp");
                exit(EXIT_FAILURE);
            } else if (pid > 0) {
                // In parent process
                wait(NULL);
            } else {
                perror("fork");
                exit(EXIT_FAILURE);
            }
        }
    }

    return 0;
}

void typeline_command(char *arg1, char *arg2) {
    // Initialize variables
    FILE *file;
    int lines_to_print = -1;
    char line[MAX_LINE];

    // Check for +n or -a option
    if (arg1[0] == '+') {
        lines_to_print = atoi(arg1 + 1);
    } else if (strcmp(arg1, "-a") == 0) {
        lines_to_print = -1;
    } else {
        printf("Invalid option. Use +n or -a.\n");
        return;
    }

    // Open the file
    file = fopen(arg2, "r");
    if (file==NULL
    ) {
        perror("fopen");
        return;
    }

    // Initialize line count
    int num_lines = 0;

    // Read and print the file contents
    while (fgets(line, sizeof(line), file)) {
        if (lines_to_print == 0) break;
        printf("%s", line);
        if (lines_to_print > 0) lines_to_print--;
        num_lines++;
    }

    // Close the file
    fclose(file);
}
