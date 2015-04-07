/**
 * Copyright <Yan Yu>, 2015
 */

#include "PNG.h"
#include <iostream>

void run() {
    
}

int
main (int argc, char * argv[]) {
    if (argc < 3) {
        std::cout << "Input format: "
                  << "<Search image> "
                  << "<Sub image> "
                  << "<Output image> "
                  << "<true/false> - optional "
                  << "<percentage pixel match> - optional "
                  << "<pixel color tolerance> - optional \n";
        return 1;
    }
    
}