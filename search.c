/*Q4 (4 )
Write a program to implement the shell. It should display the command
prompt “myshell$”. Tokenize the command line and execute the given
command by creating the child process. Additionally it should interpret the
following commands.
myshell$ search a filename pattern :- To search all the occurrence of
pattern in the file.
myshell$ search c filename pattern :- To count the number of occurrence
of pattern in the file.
myshell$ search f filename pattern :- To display first occurrence of
pattern in the file.*/

/// pattern occurence and number of time occurence all occurence (a,f,c)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_LINE 1024
#define MAX_TOKENS 64
#define MAX_TOKEN_LENGTH 128

void tokenize(char *line, char **tokens) {
    int i = 0;
    char *token = strtok(line, " \t\n");
    while (token != NULL && i < MAX_TOKENS) {
        tokens[i++] = token;
        token = strtok(NULL, " \t\n");
    }
    tokens[i] = NULL;
}

void execute_command(char **tokens) {
    pid_t pid = fork();
    if (pid == 0) {
        // Child process
        execvp(tokens[0], tokens);
        perror("execvp");
        exit(1);
    } else {
        // Parent process
        wait(NULL);
    }
}

void search_f(char *filename, char *pattern) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("fopen");
        return;
    }
    char line[MAX_LINE];
    while (fgets(line, MAX_LINE, file) != NULL) {
        if (strstr(line, pattern) != NULL) {
            printf("%s", line);
            break;
        }
    }
    fclose(file);
}

void search_c(char *filename, char *pattern) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("fopen");
        return;
    }
    char line[MAX_LINE];
    int count = 0;
    while (fgets(line, MAX_LINE, file) != NULL) {
        if (strstr(line, pattern) != NULL) {
            count++;
        }
    }
    printf("Count: %d\n", count);
    fclose(file);
}

void search_a(char *filename, char *pattern) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("fopen");
        return;
    }
    char line[MAX_LINE];
    int count = 0;
    while (fgets(line, MAX_LINE, file) != NULL) {
        if (strstr(line, pattern) != NULL) {
            printf("%s", line);
            count++;
        }
    }
    printf("Total occurrences: %d\n", count);
    fclose(file);
}

int main() {
    char line[MAX_LINE];
    char *tokens[MAX_TOKENS];

    while (1) {
        printf("myshell$ ");
        fgets(line, MAX_LINE, stdin);
        line[strlen(line) - 1] = '\0'; // Remove newline character

        tokenize(line, tokens);

        if (strcmp(tokens[0], "search") == 0) {
            if (strcmp(tokens[1], "f") == 0) {
                search_f(tokens[2], tokens[3]);
            } else if (strcmp(tokens[1], "c") == 0) {
                search_c(tokens[2], tokens[3]);
            } else if (strcmp(tokens[1], "a") == 0) {
                search_a(tokens[2], tokens[3]);
            } else {
                printf("Invalid search command\n");
            }
        } else {
            execute_command(tokens);
        }
    }

    return 0;
}
