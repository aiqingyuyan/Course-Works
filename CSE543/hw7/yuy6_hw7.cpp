#include "imgChecker.h"

void preprocess(ImgChecker& imgChecker, char** const argv, int argc) {
    std::string searchImg(argv[1]), mask(argv[2]), outImg(argv[3]);
    if (argc < 5) {
        imgChecker = ImgChecker(searchImg, mask, outImg, 75, 32);
    } else if (argc < 6) {
        imgChecker = ImgChecker(searchImg, mask, outImg,
                                std::stoi(argv[5]), 32);
    } else if (argc == 6) {
        imgChecker = ImgChecker(searchImg, mask, outImg,
                                std::stoi(argv[5]), std::stoi(argv[6]);
    }
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
    
    ImgChecker imgChecker;
    // Preprocess arguments
    preprocess(imgChecker, argv, argc);
    // Run checker
    imgChecker.run();
}