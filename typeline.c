#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <limits.h> // For INT_MAX
#include <ctype.h>  // For isdigit

// Function to print the first n lines of a file
void print_lines(char *filename, int n) {
    FILE *file = fopen(filename, "r"); // Open the file for reading
    if (!file) {
        perror("Could not open file"); // Print error if file cannot be opened
        return;
    }
    char line[1024]; // Buffer to hold each line
    for (int i = 0; i < n && fgets(line, sizeof(line), file); i++) {
        printf("%s", line); // Print each line
    }
    fclose(file); // Close the file
}

int main() {
    char input[1024]; // Buffer for user input
    char *args[100];  // Array to hold command and arguments

    while (1) { // Infinite loop to keep the shell running
        printf("myshell$ "); // Prompt user for input
        fgets(input, sizeof(input), stdin);// Exit on EOF
        input[strcspn(input, "\n")] = 0; // Remove newline character

        // Tokenize the input into command and arguments
        char *token = strtok(input, " ");
        int i = 0;
        while (token && i < 99) { // Limit to 99 arguments
            args[i++] = token; // Store each token in args array
            token = strtok(NULL, " "); // Get the next token
        }
        args[i] = NULL; // Null-terminate the argument list

        // Handle custom command 'typeline'
        if (strcmp(args[0], "typeline") == 0) {
            if (strcmp(args[1], "+") == 0) { // Print first n lines
                if (args[2] == NULL || !isdigit(args[2][0])) {
                    fprintf(stderr, "Invalid number of lines specified.\n");
                    continue;
                }
                int n = atoi(args[2]); // Convert argument to integer
                print_lines(args[3], n); // Call function to print lines
            } else if (strcmp(args[1], "-a") == 0) { // Print all lines
                print_lines(args[2], INT_MAX); // Call function to print all lines
            }
            continue; // Skip to next iteration of the loop
        }

        // Fork a new process to execute other commands
        pid_t pid = fork();
        if (pid == 0) { // Child process
            execvp(args[0], args); // Execute the command
            perror("execvp failed"); // Print error if execvp fails
            exit(EXIT_FAILURE); // Exit child process on failure
        } else if (pid > 0) { // Parent process
            wait(NULL); // Wait for child process to finish
        }
    }
    return 0; // Exit the shell
}
