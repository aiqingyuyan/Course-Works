/**
 * shell.c
 **/

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/wait.h>
#include <sys/types.h>
#include "shell_tests.h"

/* Your stuff here */

#define MAX_LINE_LEN 256
#define MAX_ARGS (MAX_LINE_LEN / 2)
#define WHITESPACE " \t\n"

typedef struct {
    char* name;
    char* argv[MAX_ARGS + 1];
    int argc;
} command_t;

const char const * promptMsg = "Shell(pid=%d)%d>";

void printPrompt(int counter) {
     printf(promptMsg, getpid(), ++counter);
}

void waitForCommand() {

}

void parseCommand(char * cmd, command_t * cmd_t) {
     int argc = 0;
     char * word;

     word = strtok(cmd, WHITESPACE);
     while (word) {
         cmd_t->argv[argc] = (char *) malloc(strlen(word) + 1);
         strcpy(cmd_t->argv[argc], word);
         word = strtok(NULL, WHITESPACE);
         ++argc;
     }
     cmd_t->argv[argc] = NULL;

     if (cmd_t->argv[0]) {
         cmd_t->argc = argc;
         cmd_t->name = (char *) malloc(strlen(cmd_t->argv[0]) + 1);
         strcpy(cmd_t->name, cmd_t->argv[0]);
     }
}

void freeCommand(command_t * cmd) {
    int i;
    for (i = 0; (i < cmd->argc) && (cmd->argv[i] != NULL); ++i) {
        free(cmd->argv[i]);
    }

    free(cmd->name);
}

int launch_commands() {
	char cmd[MAX_LINE_LEN];
	command_t command;
    static int counter;
    int pid, status;

	printPrompt(counter);

	while (fgets(cmd, MAX_LINE_LEN, stdin)) {
	    parseCommand(cmd, &command);

	    if (!command.argv[0]) {
            continue ;
	    }

        if (strcmp(command.name, "exit")) {
            // printf("command --> %s\n", command.name);
            // create a process
            pid = fork();
            if (pid == 0) {
                // child executes the command
                execv(command.name, command.argv);
            } else if (pid < 0) {
            	fprintf(stderr, "launch: Error executing command '%s'\n", command.name);
            	return EXIT_FAILURE;
            }

            // wait for command to finish
            pid = wait(&status);
            if (pid < 0) {
                fprintf(stderr, "launch: Error while waitting for child to terminate\n");
                return EXIT_FAILURE;
            } else {
            	printf("launch: Child %d terminated\n", pid);
            }

            freeCommand(&command); 
        } else {
        	// exit
        	printf("exiting\n");
        	// wait for command to finish
            pid = wait(&status);
            if (pid > 0) {
            	printf("launch: Child %d terminated\n", pid);
            }

            freeCommand(&command);

            break ;
        }

        ++counter;
        printPrompt(counter);
    }

    return EXIT_SUCCESS;
}

int main (int argc, char** argv) {
    /* Declare variables here */

    /* Entry point for the testrunner program */
    /* DO NOT DELETE */
    if ((argc > 1) && !strcmp(argv[1], "-test")) {
        run_shell_tests(argc - 1, argv + 1);
        return EXIT_SUCCESS;
    }
    /* Your code here */
    launch_commands();

    return EXIT_SUCCESS;
}
