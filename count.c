// Q5(1 )
// Write a program to implement the toy shell. It should display the command
// prompt “myshell$”. Tokenize the command line and execute the given
// command by creating the child process. Additionally it should interpret the
// following commands.
// count c filename :- To print number of characters in the file.
// count w filename :- To print number of words in the file.
// count l filename :- To print number of lines in the file.
// character count length,word,line


#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Function to tokenize the command line input
void make_toks(char *s, char *tok[]) {
    int i = 0;
    char *p;
    p = strtok(s, " "); // split the input string into tokens separated by spaces
    while (p != NULL) {
        tok[i++] = p;
        p = strtok(NULL, " ");
    }
    tok[i] = NULL; // mark the end of the token array
}

// Function to count characters, words, or lines in a file
void count(char *fn, char op) {
    int fh, cc = 0, wc = 0, lc = 0;
    char c;
    fh = open(fn,"r"); // open the file in read-only mode
    if (fh == -1) {
        printf("File %s not found.\n", fn);
        return;
    }
    while (read(fh, &c, 1) > 0) {
        if (c == ' ') wc++; // count words
        else if (c == '\n') {
            wc++; // count words
            lc++; // count lines
        }
        cc++; // count characters
    }
    close(fh);
    switch (op) {
        case 'c':
            printf("No. of characters: %d\n", cc - 1);
            break;
        case 'w':
            printf("No. of words: %d\n", wc);
            break;
        case 'l':
            printf("No. of lines: %d\n", lc + 1);
            break;
    }
}

int main() {
    char buff[80]; // input buffer
    char *args[10]; // token array
    int pid;
    while (1) {
        printf("myshell$ "); // display the command prompt
        fflush(stdin);
        fgets(buff, 80, stdin); // read input from the user
        buff[strlen(buff) - 1] = '\0'; // remove the newline character
        make_toks(buff, args); // tokenize the input

        if (strcmp(args[0], "count") == 0) {
            // handle the "count" command
            count(args[2], args[1][0]);
        } else {
            // handle other commands by forking a new process
            pid = fork();
            if (pid > 0) {
                wait(); // parent process waits for the child to finish
            } else {
                if (execvp(args[0], args) == -1) {
                    printf("Bad command.\n");
                }
            }
        }
    }
    return 0;
}

