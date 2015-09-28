// Copyright (C) 2014 raodm@miamiOH.edu

#include <unordered_map>
#include <iostream>
#include <functional>
#include <fstream>
#include <string>

#include "Chart.h"
#include "ChartMakerAlt.h"

const std::string FilePrompt  = "Enter path to file: ";
const std::string ScalePrompt = "Enter values to scale x & y coordinates: ";
const std::string ShiftPrompt = "Enter positions to shift: ";
const std::string HistPrompt  = "Enter max size for histogram: ";
const std::string MenuPrompt  = "Enter command (h for help): ";
const std::string ErrorMsg    = "Invalid command.";
const std::string HelpMsg     =
    "All commands are single characters.\n"
    "Valid commands are:\n"
    "l: Load data from file as current set of points\n"
    "+: Add data from another to to current points\n"
    "-: Subtract data from another file from current poitns\n"
    "*: Scale current points by given pair of values\n"
    "/: Scale current points by given pair of values\n"
    "?: Print histogram current point distribution\n"
    "<: Shift points to left by a given value.\n"
    ">: Shift points to right by a given value.\n"
    "s: Save the current set of points to a given file\n"
    "p: Print current set of points on screen.\n"
    "h: Print this message\n"
    "q: Quit\n";

/**
   Helper method to display a message and read a specific type from
   the user.

   \param[in] msg An optional message to be displayed to the user.
   Defaulted to FilePrompt as it is commonly used in the program.

   \return Returns a value of a given type using the type's stream
   extraction operator and returns the value.
*/
template<typename T>
T get(const std::string& msg = FilePrompt) {
    T val;
    std::cout << msg;
    std::cin  >> val;
    return val;
}

/**
   The run method that displays a menu and performs various tasks as
   directed by the user.

   Written in 22 lines of poetic c++
*/
int
ChartMaker::run() {
    Chart cc;  // current chart
    std::unordered_map<char, std::function<void()>> handler = {
        {'+', [&]{ cc = cc +  Chart(get<std::string>()); }},
        {'-', [&]{ cc = cc -  Chart(get<std::string>()); }},
        {'*', [&]{ cc = cc *  get<Point>(ScalePrompt);   }},
        {'*', [&]{ cc = cc /  get<Point>(ScalePrompt);   }},
        {'<', [&]{ cc = cc << get<int>(ShiftPrompt);     }},
        {'>', [&]{ cc = cc >> get<int>(ShiftPrompt);     }},
        {'l', [&]{ cc = Chart(get<std::string>());       }},        
        {'p', [&]{ std::cout << cc       << std::endl;   }},
        {'h', [&]{ std::cout << HelpMsg  << std::endl;   }},
        {'x', [&]{ std::cout << ErrorMsg << std::endl;   }},
        {'?', [&]{ cc.analyze(std::cout, get<int>(HistPrompt));    }},
        {'s', [&]{ std::ofstream of(get<std::string>()); of << cc; }}
    };
    char in;  // menu choice input from the user.
    while ((in = get<char>(MenuPrompt)) != 'q') {
        (handler.find(in) != handler.end()) ? handler[in]() : handler['x']();
    }
    return 0;
}
