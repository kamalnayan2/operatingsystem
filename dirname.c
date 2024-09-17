/*
Q 1 (6 )
Write a program to implement the shell. It should display the command
prompt “myshell$”. Tokenize the command line and execute the given
command by creating the child process. Additionally it should interpret the
following ‘list’ commands as
myshell$ list f dirname :- To print names of all the files in current
directory.
myshell$ list n dirname :- To print the number of all entries in the current
directory
*/
/// f dirname and list n dirname 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <sys/wait.h>

#define MAX_LINE_LENGTH 1024
#define MAX_ARGS 100

// Function to tokenize the command line
void tokenize(char *line, char **args) {
    char *token;
    int i = 0;
    token = strtok(line, " ");
    while (token != NULL && i < MAX_ARGS) {
        args[i] = token;
        token = strtok(NULL, " ");
        i++;
    }
    args[i] = NULL;
}

// Function to execute the given command
void execute_command(char **args) {
    pid_t pid;
    pid = fork();
    if (pid < 0) {
        perror("fork");
        exit(1);
    } else if (pid == 0) {
        // Child process
        if (execvp(args[0], args) < 0) {
            perror("execvp");
            exit(1);
        }
    } else {
        // Parent process
        wait(NULL);
    }
}

// Function to list files in the current directory
void list_files(char *dirname, int option) {
    DIR *dir;
    struct dirent *ent;
    struct stat statbuf;

    dir = opendir(dirname);
    if (dir == NULL) {
        perror("opendir");
        return;
    }

    int count = 0;
    while ((ent = readdir(dir)) != NULL) {
        if (option == 0) {
            // Print only file names
            printf("%s\n", ent->d_name);
        } else if (option == 1) {
            // Print the number of all entries in the current directory
            count++;
        }
    }

    if (option == 1) {
        printf("Total entries: %d\n", count);
    }

    closedir(dir);
}

int main() {
    char line[MAX_LINE_LENGTH];
    char *args[MAX_ARGS];

    while (1) {
        printf("myshell$ ");
        fgets(line, MAX_LINE_LENGTH, stdin);
        line[strlen(line) - 1] = '\0'; // Remove newline character

        tokenize(line, args);

        if (strcmp(args[0], "list") == 0) {
            if (strcmp(args[1], "f") == 0) {
                list_files(".", 0);
            } else if (strcmp(args[1], "n") == 0) {
                list_files(".", 1);
            } else {
                printf("Invalid option\n");
            }
        } else {
            execute_command(args);
        }
    }

    return 0;
}
