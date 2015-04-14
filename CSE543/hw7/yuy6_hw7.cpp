/**
 * Copyright [2015] <Yan Yu>
 */
#include <iostream>
#include <string>
#include "imgChecker.h"

void run(char** const argv, int argc) {
    std::string searchImg(argv[1]), mask(argv[2]), outImg(argv[3]);
    // create an imgChceker object
    ImgChecker imgChecker;
    // process arguments
    if (argc < 6) {
        imgChecker = ImgChecker(searchImg, mask, outImg, 75, 32);
    } else if (argc < 7) {
        imgChecker= ImgChecker(searchImg, mask, outImg,
                                std::stoi(argv[5]), 32);
    } else if (argc == 7) {
        imgChecker = ImgChecker(searchImg, mask, outImg,
                                std::stoi(argv[5]), std::stoi(argv[6]));
    }
    // run checker
    imgChecker.run();
}

int main(int argc, char* argv[]) {
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

    // process arguments and run
    run(argv, argc);

    return 0;
}
