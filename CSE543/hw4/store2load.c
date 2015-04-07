#include <stdio.h>

/* The following #define compiler directive is used to define a named
   constant to replace magic numbers (like 1000000) in the source code
   with more meaningful terms.  This named constant is used in the
   main() function to determine the number of times an experimental
   code fragement is invoked.  The experiment is repeated a large
   number of times so that the overall runtime is at an acceptable (in
   10s of seconds) value so that the timing measurement is more
   reliable and less noisy.
*/
#define REPETITIONS 1000000

// Define a named literal constant to avoid magic numbers (like 1000)
// in the code.
#define VECLEN 1000

double x_store[VECLEN], y_store[VECLEN], z_store[VECLEN];

/*
  This is the reference implementation approach to perform a specific
  calculation with 3 vectors, namely: x, y, and z.  Such
  transformations are commonly performed in graphical applications and
  many mathematical simulations involving Ordinary Differential
  Equations (ODEs).

  Note that in the two for loops in this function, there is a data
  dependency (specifically a Store-to-Load dependency) between x[k]
  and x[k-1].  Consequently, until x[k-1] is computed and written,
  x[k] cannot be computed.  This introduces a stall in the pipeline
  operation.
*/
void approach1(double x[], const double y[], 
               const double z[]) {
  unsigned int k;

  // Update value in x based on previous value of x and y
  for (k = 1; k < VECLEN; k++) {
    x[k] = x[k-1] + y[k];
  }

  // Update x based on values of x, y, and z
  for (k = 1; k < VECLEN; k++) {
    x[k] = z[k] * (y[k] - x[k-1]);
  }
}

/*
  This function is simply a rewrite of the function approach1().  In
  this function however, the dependency between x[k] and x[k-1] is
  removed by introducing an intermediate variable t, that simply
  tracks the value of x[k-1].  Since t and x are unrelated there is no
  data dependency between these variables and hopefully this should
  resolve some of the data dependencies (specifically Store-to-Load
  dependencies) in the body of the for loops.
*/
void approach2(double x[], const double y[], 
               const double z[]) {
  unsigned int k;
  double t;
  
  t = x[0];
  for (k = 1; k < VECLEN; k++) {
    t = t + y[k];
    x[k] = t;
  }
  
  t = x[0];
  for (k = 1; k < VECLEN; k++) {
    t = z[k] * (y[k] - t);
    x[k] = t;
  }
}

/*
  A simple function to reset and initialize values in three arrays used
  for testing in this code.
*/
void initialize(double x[], double y[], double z[]) {
  int index;
  for(index = 0; (index < VECLEN); index++) {
    x_store[index] = y_store[index] = z_store[index] = 1;
  }
}

/*
  The main function is just used to control the number of times
  approach1() or approach2() functions are called. 
*/
int main() {
  int counter;
  for(counter = 0; (counter < REPETITIONS); counter++) {
    initialize(x_store, y_store, z_store);
    approach1(x_store, y_store, z_store);  // <----- CHANGE ONLY THIS LINE
  }
  
  return 0;
}
