#ifndef _IMG_CHECKER_H_
#define _IMG_CHECKER_H_

#include "PNG.h"

class ImgChecker {
    using channel = unsigned char;
    using imgBuff = std::vector<channel>
public:
    // Constructor
    ImgChecker(const string& search, const string& mask, const string& out,
               int percentageMath, int pixelTolerance);
    
    // destructor
    virtual ~ImgChecker();
    
    void run();
    
protected:
    // initialization
    void init(const string& searchImg, const string& mask, const string& out);
    
    // 
    void silideMaskOverImg();
    
    // 
    bool isMatch(int row, int col);
    
    imgBuff computeAverageBackground(int row, int col);
    
    bool verifyWithMask(const imgBuff& averageBackground);
    
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