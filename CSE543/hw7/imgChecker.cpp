#include "imgChecker.h"
#include <algorithm>
#include <iostream>

ImgChecker::ImgChecker(const string& search, const string& mask,
                       const string& out, int percentageMath,
                       int pixelTolerance) : _percentageMath(percentageMath),
                                             _pixelTolerance(pixelTolerance),
                                             _useMask(true) {
    init(search, mask, out);
}

ImgChecker::~ImgChecker() { }

void
ImgChecker::run() {
    silideMaskOverImg();
}

void
ImgChecker::init(const string& searchImg, const string& mask,
                 const string& out) {
    try {
        _searchImg.load(searchImg);
    } catch(...) {
        std::cout << "cannot load search image\n";
    }
    try {
        _mask.load(mask);
    } catch(...) {
        std::cout << "cannot load mask image\n";
    }
    
    _outImg           = PNG(_searchImg); 
    _searchImgWidth   = _searchImg.getWidth();
    _searchImgHeight  = _searchImg.getHeight();
    _maskWidth        = _mask.getWidth();
    _maskHeight       = _mask.getHeight();
    
    _searchImgBuff    = _searchImg.getBuffer();
    _maskBuff         = _mask.getBuffer();
}

void
ImgChecker::silideMaskOverImg() {
    int numOfMatches = 0;
    bool result;
    for (int row = 0; (row + _maskHeight) < _searchImgHeight; ++row) {
        for (int col = 0; (col + _maskWidth) < _searchImgWidth;
                               result ? col += _maskWidth : ++col) {
            // if a match is found
            result = isMatch(row, col);
            
            if (result) {
                ++numOfMatches;
                std::cout << "sub-image matched at: " 
                          << row << ", " << col <<"\n";
                drawBox(row, col, _maskWidth, _maskHeight);
            }
            std::cout << row << ", " << col << "\n";
        }
    }
    std::cout << "Number of matches found: " << numOfMatches << "\n";
}

bool
ImgChecker::isMatch(int row, int col) {
    // first compute average background
    imgBuff averageBackground = computeAverageBackground(row, col);
    // verify again with mask
    return verifyWithMask(averageBackground, row, col);
}

imgBuff
ImgChecker::computeAverageBackground(int row, int col) {
    int maskIndex, searchImgIndex, blackPixelCount = 0;
    imgBuff channelsValue(4, 0);
    
    for (int i = 0; i < _maskHeight; ++i) {
        for (int j = 0; j < _maskWidth; ++j) {
            maskIndex = getPixelInFlatBuffer(i, j, _maskWidth);
            searchImgIndex = getPixelInFlatBuffer(row + i, col + j,
                                                  _searchImgWidth);
            if(isBlackPixel(maskIndex)) {
                //std::cout << "Enter\n";
                std::transform(_searchImgBuff.begin() + searchImgIndex,
                               _searchImgBuff.begin() + searchImgIndex + 4,
                               channelsValue.begin(),
                               channelsValue.end(),
                               std::plus<unsigned char>());
                ++blackPixelCount;
            }
            //std::cout << i << ", " << j << "\n";
            // std::cout << blackPixelCount << "\n";
        }
    }
    for_each(channelsValue.begin(), channelsValue.end(),
             [&blackPixelCount](channel& value) {
                 value /= blackPixelCount;
             });
             
    return channelsValue;
}

bool
ImgChecker::verifyWithMask(const imgBuff& averageBackground,
                           int row, int col) {
    int maskIndex, searchImgIndex, misMatchPixels, matchPixels = 0;
    
    for (int i = 0; i < _maskHeight; ++i) {
        for (int j = 0; j < _maskWidth; ++j) {
            maskIndex = getPixelInFlatBuffer(i, j, _maskWidth);
            searchImgIndex = getPixelInFlatBuffer(row + i, col + j,
                                                  _searchImgWidth);
            if (isBlackPixel(maskIndex)) {
                if(isSameShade(searchImgIndex, averageBackground)) {
                    ++matchPixels;
                }
            } else {
                if (!isSameShade(searchImgIndex, averageBackground)) {
                    ++matchPixels;
                }
            }
        }
    }
    
    misMatchPixels = _maskHeight *  _maskWidth - matchPixels;
    
    float matchPercentage = (float)std::abs(matchPixels - misMatchPixels) /
                                           (_maskHeight *  _maskWidth);
    
    return  matchPercentage > _percentageMath;
}

void
ImgChecker::drawBox(int row, int col, int width, int height) {
   // Draw horizontal lines
    for(int i = 0; (i < width); i++) {
        _outImg.setRed(row, col + i);
        _outImg.setRed(row + height, col + i);
    }
    // Draw vertical lines
    for(int i = 0; (i < height); i++) {
        _outImg.setRed(row + i, col);
        _outImg.setRed(row + i, col + width);
    }
}

bool
ImgChecker::isBlackPixel(int pixelIndex) {
   // std::cout << (int)_maskBuff[pixelIndex] << " " << (int)_maskBuff[pixelIndex+1] 
   //           << " " << (int)_maskBuff[pixelIndex+2] << "\n";
    return (_maskBuff[pixelIndex] == 255) &&      // Red channel
           (_maskBuff[pixelIndex + 1] == 255) &&  // Green channel
           (_maskBuff[pixelIndex + 2] == 255);    // Blue channel
}

bool
ImgChecker::isSameShade(int pixelIndex, const imgBuff& rgba) {
    return std::abs(_searchImgBuff[pixelIndex] - rgba[0]) 
                                        <= _pixelTolerance &&
           std::abs(_searchImgBuff[pixelIndex + 1] - rgba[1])
                                        <= _pixelTolerance &&
           std::abs(_searchImgBuff[pixelIndex + 2] - rgba[2])
                                        <= _pixelTolerance;
}

int
ImgChecker::getPixelInFlatBuffer(int row, int col, int imgWidth) {
    return row * imgWidth + col;
}