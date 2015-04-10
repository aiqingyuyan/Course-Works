#include "imgChecker.h"
#include <algorithm>

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
    
}

void
ImgChecker::init(const string& searchImg, const string& mask,
                 const string& out) {
    _searchImg.load(search);
    _mask.load(mask);
    _out.create(_searchImg);
    
    _searchImgWidth  = _searchImg.getWidth();
    _searchImgHeight = _searchImg.getHeight();
    _maskWidth       = _mask.getWidth();
    _maskHeight      = _mask.getHeight();
    
    _searchImgBuff    = _searchImg.getBuffer();
    _maskBuff         = _mask.getBuffer();
}

void
ImgChecker::silideMaskOverImg() {
    for (int row = 0; (row + _maskHeight) < _searchImgHeight; ) {
        for (int col = 0; (col + _maskWidth) < _searchImgWidth; ) {
            bool result = isMatch(row, col);
            
            if (result) {
                drawBox(row, col, _maskWidth, _maskHeight);
            }
        }
    }
}

bool
ImgChecker::isMatch(int row, int col) {
    // first compute average background
    imgBuff averageBackground = computeAverageBackground(row, col);
    // verify again with mask
    return verifyWithMask(averageBackground));
}

imgBuff
ImgChecker::computeAverageBackground(int row, int col) {
    int maskIndex, searchImgIndex, blackPixelCount = 0;
    imgBuffer channelsValue(4, 0);
    for (int i = 0; i < _maskHeight; ++i) {
        for (int j = 0; j < _maskWidth; ++j) {
            maskIndex = getPixelInFlatBuffer(i, j, _maskWidth);
            searchImgIndex = getPixelInFlatBuffer(row + i, col + j,
                                                  _searchImgWidth);
            if(isBlackPixel(maskIndex)) {
                std::transform(_searchImgBuff.begin() + searchImgIndex,
                               _searchImgBuff.begin() + 4,
                               channelsValue.begin(),
                               channelsValue.begin(), std::plus);
                ++blackPixelCount;
            }
        }
    }
    
    for_each(channelsValue.begin(), channelsValue.end(),
             [&blackPixelCount](channel& value) {
                 value /= blackPixelCount;
             });
             
    return channelsValue;
}

bool
ImgChecker::verifyWithMask(const imgBuff& averageBackground) {
    int maskIndex, searchImgIndex, misMatchPixels, matchPixels = 0,;
    
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
                                           (_maskHeight *  _maskWidth)
    
    return  matchPercentage > _percentageMath;
}

bool
ImgChecker::isBlackPixel(int pixelIndex) {
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
                                        <= _pixelTolerance
}

int
ImgChecker::getPixelInFlatBuffer(int row, int col, int imgWidth) {
    return row * imgWidth + col;
}