#ifndef _IMG_CHECKER_H_
#define _IMG_CHECKER_H_

#include "PNG.h"
#include <string>
using namespace std;

using channel = unsigned char;
using imgBuff = std::vector<channel>;

typedef struct _Rect {
  int _x1, _y1, _x2, _y2;
  _Rect() : _x1(0), _y1(0), _x2(0), _y2(0) { }
  _Rect(int x1, int y1, int x2, int y2) : 
            _x1(x1), _y1(y1), _x2(x2), _y2(y2) { }
            
  // bool isOverlapWith(const Rect& rect) const {
  //     
  // }
            
  // return true if is overlap with
  bool isOverlapWith(int x3, int y3, int x4, int y4) const {
      // p1: _x1, _y1, p2: _x2, _y2
      // p3: x3, y3, p4: x4, y4
      return !(_y2 < y3 || _y1 > y4 || _x2 < x3 || _x1 > x4);
  }
  
} Rect;

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
    
    string outImgName;
    
private:
    //
    bool isRegionOverlapWith(int x, int y,
                             const std::vector<Rect>& Regions) const;
    
    // 
    inline int getPixelInFlatBuffer(int row, int col, int imgWidth);
    
    //
    inline bool isBlackPixel(int pixelIndex);
    
    //
    inline bool isSameShade(int pixelIndex, const imgBuff& rgba);
    
    PNG _searchImg, _mask, _outImg;
    int _percentageMath, _pixelTolerance;
    bool _useMask;
};

#endif