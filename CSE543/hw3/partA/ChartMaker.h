#ifndef CHART_MAKER_H
#define CHART_MAKER_H

#include "Chart.h"
/** A top-level class to perform operations on chart objects based on
	user input.

	NOTE: You may add as many methods as needed to design this class.
	
	Copyright (C) 2014 raodm@miamiOH.edu
*/
class ChartMaker {
public:
    ChartMaker() {}
    ~ChartMaker() {}

    int run();

protected:
    void ReceiveCommand(const char command);

    void DisplayHelpMenu();

    void LoadDataFromFile();

    void Add_Sub_DataFromAnotherToCurrentPoints(char op);

    void ScaleCurrentPoints(char op);

    void PrintHistogramCurrentPoints();

    void ShiftPointsToLeft();

    void ShiftPointsToRight();

    void SaveCurrentPointsToFile();

    void PrintCurrentPoints();

private:
    Chart current;

    bool run_flag;
};

#endif
