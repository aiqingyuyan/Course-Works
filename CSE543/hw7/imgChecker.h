#ifndef _IMG_CHECKER_H_
#define _IMG_CHECKER_H_

#include "PNG.h"
#include <string>
using namespace std;

using channel = unsigned char;
using imgBuff = std::vector<channel>;

class ImgChecker {
public:
    // Constructors
    ImgChecker() {}
    
    ImgChecker(const string& search, const string& mask, const string& out,
               int percentageMath, int pixelTolerance);
    
    // destructor
    virtual ~ImgChecker();
    
    void run();
    
protected:
    // initialization
    void init(const string& searchImg, const string& mask, const string& out);
    
    // Logically slide the mask over the image, row-by-row and 
    // column-by-column  to identify the region of the image to 
    // search and check for potential match
    void silideMaskOverImg();
    
    // Check if a match is found
    bool isMatch(int row, int col);
    
    imgBuff computeAverageBackground(int row, int col);
    
    bool verifyWithMask(const imgBuff& averageBackground, int row, int col);
    
    void drawBox(int row, int col, int width, int height);
    
    int _searchImgWidth, _searchImgHeight, _maskWidth, _maskHeight;
    
    imgBuff _searchImgBuff, _maskBuff;
    
private:
    // 
    int getPixelInFlatBuffer(int row, int col, int imgWidth);
    
    //
    bool isBlackPixel(int pixelIndex);
    
    //
    bool isSameShade(int pixelIndex, const imgBuff& rgba);
    
    PNG _searchImg, _mask, _outImg;
    int _percentageMath, _pixelTolerance;
    bool _useMask;
};

#endif