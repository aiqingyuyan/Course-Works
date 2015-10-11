/**
 * shell.c
 **/

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
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
const char const * forkMsg   = "  Parent says 'child process has been forked this pid=%d'\n";
const char const * waitMsg   = "  Parent says 'wait() returned so the child with pid=%d is finished'\n";

void printPrompt(int counter) {
     printf(promptMsg, getpid(), ++counter);
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
	char oldpwd[100];
	oldpwd[0] = '\0';
	
	printPrompt(counter);

	while (fgets(cmd, MAX_LINE_LEN, stdin)) {
	    parseCommand(cmd, &command);

	    if (!command.argv[0]) {
	    	printPrompt(counter);
            continue ;
	    }

        if (!strcmp(command.name, "cd")) {
        	// change directory command
        	if (command.argc > 2) {
                fprintf(stderr, "Error changing directory, too many arguments\n");
        	} else if (command.argc == 2){
				if(!strcmp(command.argv[1],"-")) {
					if(oldpwd[0] == '\0') {
						fprintf(stderr,"bash: cd: OLDPWD not set\n");
					} else {
						char tmp2[100];
						tmp2[0]=NULL;
						strcpy(tmp2,oldpwd);
						oldpwd[0] = NULL;
						getwd(oldpwd);
						if (chdir(tmp2) < 0) {
                    		fprintf(stderr, "Error changing directory, invalid directory\n");
                		}
						
						char* temp1 = get_current_dir_name();
						printf("%s\n",temp1);
						free(temp1);
					}
				} else {
					oldpwd[0] = NULL;
					getwd(oldpwd);
					if((chdir(command.argv[1]) < 0)) { 
                    	fprintf(stderr, "Error changing directory, invalid directory\n");
					}
                }
        	} else {
                if (chdir(getenv("HOME")) < 0) {
                    fprintf(stderr, "Error changing to home directory\n");
                }
        	}

        	freeCommand(&command);
        } else if (strcmp(command.name, "exit")) {
            // create a process
            pid = fork();
            if (pid == 0) {
                // child executes the command
				int i = 0;
                execvp(command.name, command.argv);
                fprintf(stderr, "launch: Error executing command '%s'\n", command.name);
                return EXIT_FAILURE;
            } else if (pid < 0) {
            	fprintf(stderr, "launch: Error while forking '%s'\n", command.name);
            	return EXIT_FAILURE;
            }

            printf(forkMsg, pid);
            // wait for command to finish
            pid = waitpid(pid,&status,0);
	    if(status != EXIT_SUCCESS) fprintf(stderr,"shell: Process %d exited with status %d\n",pid,status);
		//printf("!!!!!!%d %d\n",status,pid);
            if (pid < 0) {
                fprintf(stderr, "launch: Error while waitting for child to terminate\n");
                return EXIT_FAILURE;
            } else {
            	printf(waitMsg, pid);
            }

            freeCommand(&command);
        } else {
        	// exit
        	printf("exiting\n");
        	// wait for command to finish
            pid = wait(&status);
            if (pid > 0) {
            	printf(waitMsg, pid);
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
