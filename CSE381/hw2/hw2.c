#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>

int main(int argc, char **argv) {
   unsigned seconds = 20;  // default 20 seconds

   if (argc >= 2) {
      seconds = atoi(argv[1]);   // update the seconds, using command line argument
   }

   srand(time(NULL));

   pid_t processId = getpid();   // get the id of this process

   fprintf(stdout, "START\n");

   unsigned counter = 0;
   int textColor = rand() % 8 + 30;
   int bgColor = rand() % 8 + 40;
   while (counter < seconds) {
       fprintf(stdout, "%c[%d;%d;%dm %i; tick %u\n", 27, 1, textColor, bgColor, processId, (counter + 1));
       sleep(1);
       ++counter;
   }
   
   fprintf(stdout, "%c[%dm", 27, 0);
   
   return 0;
}

