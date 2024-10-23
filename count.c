#include <stdio.h>      // Standard input/output library
#include <stdlib.h>     // Standard library for memory allocation and process control
#include <string.h>     // String handling functions
#include <unistd.h>     // POSIX operating system API
#include <sys/types.h>  // Data types used in system calls
#include <sys/wait.h>   // Macros related to process termination

// Function to count characters in a file
void count_characters(const char *filename) {
    // Open the file for reading
    FILE *file = fopen(filename, "r");
    if (!file) {  // Check if the file opened successfully
        perror("Could not open file");  // Print error message if it fails
        return;  // Exit the function
    }

    int count = 0;  // Initialize character count
    // Read each character until EOF (End Of File)
    while (fgetc(file) != EOF) {
        count++;  // Increment count for each character read
    }
    // Print the total number of characters
    printf("Number of characters in %s: %d\n", filename, count);
    fclose(file);  // Close the file
}

// Function to count words in a file
void count_words(const char *filename) {
    // Open the file for reading
    FILE *file = fopen(filename, "r");
    if (!file) {  // Check if the file opened successfully
        perror("Could not open file");  // Print error message if it fails
        return;  // Exit the function
    }

    int count = 0;  // Initialize word count
    char word[100];  // Temporary buffer for reading words
    // Read words from the file until EOF
    while (fscanf(file, "%s", word) != EOF) {
        count++;  // Increment count for each word read
    }
    // Print the total number of words
    printf("Number of words in %s: %d\n", filename, count);
    fclose(file);  // Close the file
}

// Function to count lines in a file
void count_lines(const char *filename) {
    // Open the file for reading
    FILE *file = fopen(filename, "r");
    if (!file) {  // Check if the file opened successfully
        perror("Could not open file");  // Print error message if it fails
        return;  // Exit the function
    }

    int count = 0;  // Initialize line count
    char c;  // Variable to store each character
    // Read each character until EOF
    while ((c = fgetc(file)) != EOF) {
        if (c == '\n') {  // Check for newline character
            count++;  // Increment count for each line found
        }
    }
    // Print the total number of lines
    printf("Number of lines in %s: %d\n", filename, count);
    fclose(file);  // Close the file
}

int main() {
    char input[1024];  // Buffer for user input

    // Infinite loop to keep the shell running
    while (1) {
        printf("myshell$ ");  // Display the shell prompt
        // Read user input and exit on EOF
        if (!fgets(input, sizeof(input), stdin)) break; 
        input[strcspn(input, "\n")] = 0;  // Remove newline character from input

        // Tokenize the input into command and arguments
        char *args[4];  // Array to hold command and arguments
        char *token = strtok(input, " ");  // Get the first token
        int i = 0;  // Index for args array
        while (token && i < 3) {  // Loop to extract tokens
            args[i++] = token;  // Store token in args array
            token = strtok(NULL, " ");  // Get the next token
        }
        args[i] = NULL;  // Null-terminate the args array

        // Check for 'count' command
        if (strcmp(args[0], "count") == 0 && i == 3) {
            const char *command = args[1];  // Get the command (c, w, l)
            const char *filename = args[2];  // Get the filename

            // Execute the appropriate counting function based on command
            if (strcmp(command, "c") == 0) {
                count_characters(filename);  // Count characters
            } else if (strcmp(command, "w") == 0) {
                count_words(filename);  // Count words
            } else if (strcmp(command, "l") == 0) {
                count_lines(filename);  // Count lines 
                } 
              else {
                printf("Invalid option for 'count' command.\n");
            }
            
           }
           else if(strcmp(args[0],"exit"))
           {
           printf("Exit");
           break;
           }
    }
    return 0;  // Exit the shell
}
