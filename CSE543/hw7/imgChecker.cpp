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
    // bool result;
    // Used to remember all of the indntified region
    std::vector<Rect> identifiedRegions(0);
    for (int row = 0; (row + _maskHeight) <= _searchImgHeight; ++row) {
        for (int col = 0; (col + _maskWidth) <= _searchImgWidth; ++col) {
            // if this region of pixeles doesn't overlap
            // with other identified regions
            if (!isRegionOverlapWith(col, row, identifiedRegions)) {
                // if a match is found
                if (isMatch(row, col)) {
                    ++numOfMatches;
                    // mark identified regions
                    identifiedRegions.push_back(Rect(col, row,
                                      col + _maskWidth - 1,
                                      row + _maskHeight - 1));
                    std::cout << "sub-image matched at: "
                              << row << ", " << col <<"\n";
                    drawBox(row, col, _maskWidth, _maskHeight);
                }
            }
        }
    }
    std::cout << "Number of matches found: " << numOfMatches << "\n";
    _outImg.write("Result.png");
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
    std::vector<unsigned int> channelsValue(4, 0);
    
    for (int i = 0; i < _maskHeight; ++i) {          // row
        for (int j = 0; j < _maskWidth; ++j) {       // col
            maskIndex = getPixelInFlatBuffer(i, j, _maskWidth);
            searchImgIndex = getPixelInFlatBuffer(row + i, col + j,
                                                  _searchImgWidth);
            if(isBlackPixel(maskIndex)) {
                channelsValue[0] += _searchImgBuff[searchImgIndex];
                channelsValue[1] += _searchImgBuff[searchImgIndex + 1];
                channelsValue[2] += _searchImgBuff[searchImgIndex + 2];
                ++blackPixelCount;
            }
        }
    }
    // std::cout << blackPixelCount << "\n";
    
    imgBuff averageBackground(4, 0);
    averageBackground[0] = channelsValue[0] / blackPixelCount;
    averageBackground[1] = channelsValue[1] / blackPixelCount;
    averageBackground[2] = channelsValue[2] / blackPixelCount;
    
    // std::cout << "Channels Value: " << (int)averageBackground[0] << " "
    //           << (int)averageBackground[1] << " " 
    //           << (int)averageBackground[2] << "\n";
             
    return averageBackground;
}

bool
ImgChecker::verifyWithMask(const imgBuff& averageBackground,
                           int row, int col) {
    int maskIndex, searchImgIndex, matchPixels = 0;
    
    for (int i = 0; i < _maskHeight; ++i) {
        for (int j = 0; j < _maskWidth; ++j) {
            maskIndex = getPixelInFlatBuffer(i, j, _maskWidth);
            searchImgIndex = getPixelInFlatBuffer(row + i, col + j,
                                                  _searchImgWidth);
            if (isBlackPixel(maskIndex)) {
                if(isSameShade(searchImgIndex, averageBackground)) {
                    ++matchPixels;
                } else {
                    --matchPixels;
                }
            } else {
                if (!isSameShade(searchImgIndex, averageBackground)) {
                    ++matchPixels;
                } else {
                    --matchPixels;
                }
            }
        }
    }

    float matchPercentage = (float)(matchPixels * 100) /
                            (_maskHeight * _maskWidth);
    
    return  matchPercentage > _percentageMath;
}

void
ImgChecker::drawBox(int row, int col, int width, int height) {
   // Draw horizontal lines
    for(int i = 0; (i < width - 1); ++i) {
        _outImg.setRed(row, col + i);
        _outImg.setRed(row + height, col + i);
    }
    // Draw vertical lines
    for(int i = 0; (i < height - 1); ++i) {
        _outImg.setRed(row + i, col);
        _outImg.setRed(row + i, col + width);
    }
}

bool
ImgChecker::isRegionOverlapWith(int x, int y,
                     const std::vector<Rect>& Regions) const {
    for (auto& rect : Regions) {
        if (rect.isOverlapWith(x, y,
                 x + _maskWidth - 1,
                 y + _maskHeight - 1))
           return true;
    }
    return false;
}

bool
ImgChecker::isBlackPixel(int pixelIndex) {
    return (_maskBuff[pixelIndex] == 0) &&      // Red channel
           (_maskBuff[pixelIndex + 1] == 0) &&  // Green channel
           (_maskBuff[pixelIndex + 2] == 0);    // Blue channel
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
    return 4 * row * imgWidth + 4 * col;
}