// Copyright (C) 2014 raodm@miamiOH.edu

#include "Chart.h"
#include <iterator>
#include <algorithm>
#include <fstream>
#include <vector>
#include <map>
#include <string>

Chart::Chart() {
    pointList = std::vector<Point>(0);
}

Chart::Chart(const Chart& other) :  pointList(other.pointList) {}

Chart::Chart(Chart&& other) : pointList(std::move(other.pointList)) {}

Chart::Chart(const std::string& fileName) {
    std::ifstream fin(fileName, std::ios_base::binary);
    std::istream_iterator<Point> in_iter(fin), eof;
    std::copy(in_iter, eof, std::back_inserter(pointList));
}

Chart::~Chart() {}

std::ostream&
operator<<(std::ostream& os, const Chart& c) {
    std::for_each(c.pointList.begin(), c.pointList.end(),
        [&os](const Point& p) {
           os << p << "\n";
        });
    return os;
}

std::istream&
operator>>(std::istream& is, Chart& c) {
    std::istream_iterator<Point> in_iter(is), eof;
    std::copy(in_iter, eof, std::back_inserter(c.pointList));
    return is;
}

Chart&
Chart::operator=(const Chart& src) {
    pointList = src.pointList;
    return *this;
}

bool
Chart::contains(const Point& p) const {
    if (std::find(pointList.begin(), pointList.end(), p)
        != pointList.end())
        return true;
    return false;
}

Chart
Chart::operator+(const Chart& other) const {
     Chart result(*this);
     std::copy_if(other.pointList.begin(), other.pointList.end(),
                  std::back_inserter(result.pointList),
                  [&result](const Point& p){
                      return std::find(result.pointList.begin(),
                             result.pointList.end(), p) ==
                             result.pointList.end();
                  });
     return result;
}

Chart
Chart::operator-(const Chart& other) const {
    Chart result(*this);
    const std::vector<Point>& pList = other.pointList;
    for_each(pList.begin(), pList.end(),
           [&result](const Point& p){
             result.pointList.erase(
                    std::remove(result.pointList.begin(),
                                result.pointList.end(),
                                p),
                    result.pointList.end());
           });
    return result;
}

Chart
Chart::operator*(const Point& scale) const {
    Chart result;
    const std::vector<Point>& pList = this->pointList;
    result.pointList.resize(pointList.size());
    std::generate(result.pointList.begin(), result.pointList.end(),
                  [&scale, &pList](){
                     static size_t i = 0;
                     Point p(pList[i].getX() * scale.getX(),
                                  pList[i].getY() * scale.getY());
                     i++;
                     return p;
                  });
    return result;
}

Chart
Chart::operator/(const Point& scale) const {
    Chart result;
    const std::vector<Point>& pList = this->pointList;
    result.pointList.resize(pointList.size());
    std::generate(result.pointList.begin(), result.pointList.end(),
                  [&scale, &pList](){
                     static size_t i = 0;
                     Point p(pList[i].getX() / scale.getX(),
                                  pList[i].getY() / scale.getY());
                     i++;
                     return p;
                  });
    return result;
}

Chart
Chart::operator>>(int value) const {
    Chart result(*this);
    for (int i = 0; i < value; i++) {
        result.pointList.insert(result.pointList.begin(), Point(0, 0));
    }
    return result;
}

Chart
Chart::operator<<(int value) const {
    Chart result;
    result.pointList = std::vector<Point>(pointList.begin() + value,
                                     pointList.end());
    return result;
}

void
Chart::analyze(std::ostream& os, const int scale) const {
    std::vector<Point> target;
    std::copy_if(pointList.begin(), pointList.end(), std::back_inserter(target),
                 [&target](const Point& p) {
                    return std::find(target.begin(), target.end(), p) ==
                           target.end();
                 });
    std::map<int, int> result = {
     {1, 0}, {2, 0}, {3, 0}, {4, 0}
    };
    std::for_each(target.begin(), target.end(),
             [&result](const Point& p) {
               std::cout << p << "\n";
               ++result[p.getQuadrant()];
             });
    int max = 0;
    std::for_each(result.begin(), result.end(),
             [&max](const val& v) {
               max = v.second > max ? v.second : max;
             });
    printHistogram(result, max, scale);
}

void
Chart::printHistogram(const std::map<int, int>& result, const int& max,
                      const int& scale) const {
    for (const val& v : result) {
        switch (v.first) {
          case 1:
             std::cout << "I\t: "; printStars(v.second, max, scale);
             break;
          case 2:
             std::cout << "II\t: "; printStars(v.second, max, scale);
             break;
          case 3:
             std::cout << "III\t: "; printStars(v.second, max, scale);
             break;
          case 4:
             std::cout << "IV\t: "; printStars(v.second, max, scale);
             break;
        }
    }
}

void
Chart::printStars(const int& num, const int& max, const int& scale) const {
    int i = 0, numStars = num * scale / max;
    while (i < numStars) {
       std::cout << "*";
       i++;
    }
    std::cout << "\n";
}
