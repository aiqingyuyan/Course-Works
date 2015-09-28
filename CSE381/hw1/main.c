/*
About this program:
- This program counts words.
- The specific words that will be counted are passed in as command-line
  arguments.
- The program reads words (one word per line) from standard input until EOF or
  an input line starting with a dot '.'
- The program prints out a summary of the number of times each word has
  appeared.
- Various command-line options alter the behavior of the program.

E.g., count the number of times 'cat', 'nap' or 'dog' appears.
> ./main cat nap dog
Given input:
 cat
 .
Expected output:
 Looking for 3 words
 Result:
 cat:1
 nap:0
 dog:0
 
 
 Note: this code was automatically formatted (styled) using 'indent main.c -kr'.
 
 This assignment was adapted from operating system programming problems by
 Lawrence Angrave at the University of Illinois at Champaign-Urbana (UIUC).
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "smp0_tests.h"

#define LENGTH(s) (sizeof(s) / sizeof(*s))

/* Structures */
typedef struct {
  char *word;
  int counter;
} WordCountEntry;

/* Global Variables */
FILE *outStream = NULL;
const char* delimiters = " ,\\/\"'";

int process_stream(WordCountEntry entries[], int entry_count)
{
  short line_count = 0;
  char buffer[30];

  while (fgets(buffer, 30, stdin)) {
    if (*buffer == '.')
      break;

    /* trim buffer */
    int length = strlen(buffer);
    if (buffer[length - 1] == '\n') {
    	buffer[length - 1] = '\0';
    }

    /* tokenize string */
    char *str = strtok(buffer, delimiters);
    while (str) {
       /* Compare against each entry */
       int i = 0;
       while (i < entry_count) {
         if (!strcmp(entries[i].word, str))
           entries[i].counter++;
         i++;
       }
       str = strtok(NULL, delimiters);
    }

    line_count++;
  }
  return line_count;
}


void print_result(WordCountEntry *entries, int entry_count)
{
  fprintf(outStream, "Result:\n");
  while (entry_count-- > 0) {
    fprintf(outStream ,"%s:%d\n", entries->word, entries->counter);
    ++entries;
  }
}


void printHelp(const char *name)
{
  fprintf(outStream ,"usage: %s [-h] <word1> ... <wordN>\n", name);
}


int main(int argc, char **argv)
{
  const char *prog_name = *argv;

  WordCountEntry *entries = NULL;
  int entryCount = 0;

  /* Entry point for the testrunner program */
  if (argc > 1 && !strcmp(argv[1], "-test")) {
    run_smp0_tests(argc - 1, argv + 1);
    return EXIT_SUCCESS;
  }
  
  outStream = stdout;
  int wordsLength = argc - 1;  
  argv++;
  while (*argv != NULL) {
    if (**argv == '-') {

      switch ((*argv)[1]) {
        case 'h':
          --wordsLength;
          printHelp(prog_name);
	  break;
        case 'f': ;
            char fileName[100];
            // +2 to escape -f two characters
            strcpy(fileName, *(argv) + 2);
            outStream = fopen(fileName, "w");
            if (!outStream) {
               fprintf(stderr, "ERROR: Cannot open target file %s", *argv);
               return EXIT_FAILURE;
            }
            --wordsLength;
            break;
        default:
          fprintf(stderr, "%s: Invalid option %s. Use -h for help.\n",
                 prog_name, *argv);
      }
    } else {
      if (!entries) {
         entries = (WordCountEntry *)malloc(sizeof(WordCountEntry) * wordsLength);
      }

      if (entryCount < /*LENGTH(entries)*/ wordsLength) {
        entries[entryCount].word = *argv;
        entries[entryCount++].counter = 0;
      }
    }
    argv++;
  }

  if (entryCount == 0) {
    fprintf(stderr, "%s: Please supply at least one word. Use -h for help.\n",
           prog_name);
    free(entries);
    return EXIT_FAILURE;
  }
  if (entryCount == 1) {
    fprintf(outStream, "Looking for a single word\n");
  } else {
    fprintf(outStream, "Looking for %d words\n", entryCount);
  }

  process_stream(entries, entryCount);
  print_result(entries, entryCount);

  free(entries);

  return EXIT_SUCCESS;
}
