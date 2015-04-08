/**
 * Copyright <Yan Yu>, 2015
 */

#include "PNG.h"
#include <iostream>

using unsigned char = channel;
using std::vector<channel> = imgBuffer;
// 
// struct Pixel {
//     unsigned char r, g, b, a;
//     
//     Pixel(const channel& R, const channel& G,
//           const channel& B, const channel& A) : r(R), g(G), b(B), a(A)
//     { }
// };
// 
// const 

// function forward declarations

void
run(const int& argc, char** const argv);


void
preprocess(const int& argc, char** const argv,
           PNG& SearchImg, PNG& Mask, PNG& OutImg,
           int& PercentagePixelMatch, int& PixelColorTolerance);

void
startImgSearch(const PNG& SearchImg, const PNG& Mask, PNG& OutImg,
              const int PercentagePixelMatch,
              const int PixelColorTolerance);

bool
checkRegion(const PNG& SearchImg, const PNG& Mask, int row, int col,
            int regionWidth, int regionHeight,
            int searchImgWidth, int searchImgheight);

bool
verifyWithMask();

bool
isBlackPixel(const PNG& img, int imgIndex);

int
getPixelIndex(int row, int col, int imgWidth);


// Entry Point
int
main (int argc, char* argv[]) {
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
    // Start
    run(argc, argv);
}

void
run(const int& argc, char** const argv) {
    PNG SearchImg, Mask, OutImg;
    int PercentagePixelMatch, PixelColorTolerance;
    // Pre-process 
    preprocess(argc, argv, SearchImg, Mask, OutImg,
               PercentagePixelMatch, PixelColorTolerance);
    // search image
    startImgSearch(SearchImg, Mask, OutImg,
                   PercentagePixelMatch, PixelColorTolerance);
    
}

void
preprocess(const int& argc, char** const argv,
           PNG& SearchImg, PNG& Mask, PNG& OutImg,
           int& PercentagePixelMatch, int& PixelColorTolerance) {
    std::string searchImg(argv[1]), mask(argv[2]), outImg(argv[3]);
    // assume fourth parameter is always true
    bool isMask = true;
    // parameters handling
    if (argc < 5) {
        PercentagePixelMatch = 75;
        PixelColorTolerance  = 32;
    } else if (argc < 6) {
        PercentagePixelMatch = std::stoi(argv[5]);
        PixelColorTolerance  = 32;
    } else if (argc == 6) {
        PercentagePixelMatch = std::stoi(argv[5]);
        PixelColorTolerance  = std::stoi(argv[6]);
    }
    // load search img and mask img, at the same time
    // create output img
    SearchImg.load(searchImg);
    Mask.load(mask);
    OutImg.create(searchImg);
}

void
startImgSearch(const PNG& SearchImg, const PNG& Mask, PNG& OutImg,
               const int PercentagePixelMatch,
               const int PixelColorTolerance) {
    int searchImgWidth  = SearchImg.getWidth();
    int searchImgHeight = SearchImg.getHeight();
    int maskWidth       = Mask.getWidth();
    int maskHeight      = Mask.getHeight();
    imgBuffer searchImgFlatBuffer =
                                 SearchImg.getBuffer();
    imgBuffer maskFlatBuffer =
                                 Mask.getBuffer();
    // Start Loop
    for(int row = 0; (row + maskHeight) <= searchImgHeight; ++row) {
        for (int col = 0; (col + maskWidth) <= searchImgWidth; ++col) {
            // check region
        }
    }
}

bool
checkRegion(const imgBuffer& SearchImg, const imgBuffer& Mask,
            int row, int col, int regionWidth, int regionHeight,
            int searchImgWidth, int searchImgheight) {
    // how many black pixels in mask
    
    // average pixel valuses in search region
    std::vector<channel> averageChannelsValue(4, 0); 
    // loop the mask and the region of search img
    for(int i = 0; i < regionHeight; ++i) {  // rows
        for (j = 0; j < regionWidth; ++j) {  // cols
            // get pixel index in the flat buffer of mask
            int pixelIndexMask = getPixelIndex(i, j, regionWidth);
            int pixelIndexSearchImg = getPixelIndex(row + i, col + j, searchImgWidth);
            // if this pixel is black in Mask
            if (isBlackPixel(Mask, pixelIndexMask)) {
                averageChannelsValue[0] += SearchImg[pixelIndexSearchImg];
                averageChannelsValue[1] += SearchImg[pixelIndexSearchImg + 1];
                averageChannelsValue[2] += SearchImg[pixelIndexSearchImg + 2];
                averageChannelsValue[3] += SearchImg[pixelIndexSearchImg + 3];
                
            }
        }
    }
    // average the averageChannelsValue
    
    // verify region with mask
    verifyWithMask();
}

bool
isBlackPixel(const imgBuffer& img, int imgIndex) {
    // check r, g, b channels
    return (img[imgIndex] == 255) && (img[imgIndex + 1] == 255) &&
           (img[imgIndex + 2] == 255);
}

int
getPixelIndex(int row, int col, int imgWidth) {
    return row * imgWidth + col;
}
