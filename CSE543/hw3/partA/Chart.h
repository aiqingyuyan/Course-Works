// Copyright Yan Yu <2014>, Miami University

#ifndef CHART
#define CHART

#include <vector>
#include <map>
#include <utility>
#include <string>
#include "Point.h"

/** A top-level class to perform operations on chart objects based on
	user input.

	NOTE: You may add as many methods as needed to design this class.
	
	Copyright (C) 2014 raodm@miamiOH.edu
*/
class Chart {
  using val = std::pair<int, int>;
	 /**
     * Stream insertion operator: print each point in the chart to the
     * given stream os followed by a newline
     * @param - os: output stream
     * @param - c: an instance of Chart
     */
     friend std::ostream& operator<<(std::ostream& os, const Chart& c);

     /**
     * Stream extraction operator: load all points(until EOF) from the
     * given input stream into chart c
     * @param - is: input stream
     * @param - c: an instance of Chart
     */
     friend std::istream& operator>>(std::istream& is, Chart& c);

public:
    /**
     * Default constructor
     */
    Chart();

     /**
     * Copy constructor
     */
    Chart(const Chart& other);

    /**
    * Move constructor
    */
    Chart(Chart&& other);

    /**
    * Constructor to create a chart using points in a gievn text file
    * @param - fileName: name of text file containing points
    */
    explicit Chart(const std::string& fileName);

    ~Chart();

    /**
    * Assignment operator overload: copy points from the source into this
    * @param - src: source Chart object to copy from
    */
    Chart& operator=(const Chart& src);

    /**
    * Detect if a given point is in this chart
    * @param - p: point to check if it exists in chart
    */
    bool contains(const Point& p) const;

    /**
     * Adds points of two charts together
     * param - other: another instance of Chart
     */
     Chart operator+(const Chart& other) const;

    /**
     * Returns a new Chart that contains all
     * points in other removed from points in
     * this chart
     @ param - other: another instance of Chart
     */
     Chart operator-(const Chart& other) const;

     /**
      * Create a new Chart with x & y coordinates of each point
      * in this chart multiplied by the x and y values in scale
      * @param - scale: scaler point used to multiply each point
      */
     Chart operator*(const Point& scale) const;

     /**
      * Create a new Chart with x & y coordinates of each point
      * in this chart divided by the x and y values in scale
      * @param - scale: scaler point used to divide each point
      */
     Chart operator/(const Point& scale) const;

     /**
      * Shift the points in the chart to the right by the gievn amount
      * @param - value: num of locations by inserting zeros at the beginning
      */
      Chart operator>>(int value) const;

      /**
      * Shift the point in the chart to the left by the given amount
      * @param - value: num of locations by removing points at the beginning
      */
      Chart operator<<(int value) const;

      /**
      * Disply a histogram of distribution of unique points in
      * the 4 quadrants
      */
      void analyze(std::ostream& os, const int scale) const;

protected:
      void printHistogram(const std::map<int, int>& result, const int& max,
                          const int& scale) const;

      void printStars(const int& num, const int& max, const int& scale) const;

private:
      std::vector<Point> pointList;
};

#endif
