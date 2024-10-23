#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

// Function to list files or count entries in a directory
void list_directory(const char *dirname, int count_entries) {
    struct dirent *entry;
    DIR *dp = opendir(dirname);
    int count = 0;

    if (dp == NULL) {
        perror("Could not open directory");
        return;
    }

    while ((entry = readdir(dp))) {
        if (count_entries) {
            count++;
        } else {
            printf("%s\n", entry->d_name);
        }
    }
    closedir(dp);

    if (count_entries) {
        printf("Number of entries in directory '%s': %d\n", dirname, count);
    }
}

int main() {
    char input[1024];

    while (1) {
        printf("myshell$ ");
        if (!fgets(input, sizeof(input), stdin)) break; // Exit on EOF
        input[strcspn(input, "\n")] = 0; // Remove newline character

        char *args[4]; // Array to hold command and arguments
        char *token = strtok(input, " ");
        int i = 0;
        while (token && i < 3) {
            args[i++] = token;
            token = strtok(NULL, " ");
        }
        args[i] = NULL;

        // Check for 'list' command
        if (strcmp(args[0], "list") == 0 && i == 3) {
            if (strcmp(args[1], "f") == 0) {
                list_directory(args[2], 0); // List file names
            } else if (strcmp(args[1], "n") == 0) {
                list_directory(args[2], 1); // Count entries
            } else {
                printf("Invalid option for 'list' command.\n");
            }
        } else {
            printf("Invalid command or arguments.\n");
        }
    }
    return 0;
}
