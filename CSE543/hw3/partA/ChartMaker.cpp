// Copyright (C) 2014 raodm@miamiOH.edu

#include "ChartMaker.h"
#include <fstream>
#include <string>

int
ChartMaker::run() {
    run_flag = true;
    char command;
    while (run_flag) {
       std::cout << "Enter command (h for help): ";
       std::cin >> command;
       try {
          ReceiveCommand(command);
       } catch(...) {
         return 1;
       }
    }
    return 0;
}

void
ChartMaker::ReceiveCommand(const char command) {
    switch (command) {
       case 'l': LoadDataFromFile(); break;
       case '+': Add_Sub_DataFromAnotherToCurrentPoints('+'); break;
       case '-': Add_Sub_DataFromAnotherToCurrentPoints('-'); break;
       case '*': ScaleCurrentPoints('*'); break;
       case '/': ScaleCurrentPoints('/'); break;
       case '?': PrintHistogramCurrentPoints(); break;
       case '<': ShiftPointsToLeft(); break;
       case '>': ShiftPointsToRight(); break;
       case 's': SaveCurrentPointsToFile(); break;
       case 'p': PrintCurrentPoints(); break;
       case 'h': DisplayHelpMenu(); break;
       case 'q': run_flag = false; break;
       default : std::cout << "Invalid command\n"; break;
    }
}

void
ChartMaker::DisplayHelpMenu() {
    printf("All commands are single characters.\n"
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
              "q: Quit\n");
}

void
ChartMaker::LoadDataFromFile() {
    std::string file;
    std::cout << "Enter path to file: ";
    std::cin >> file;
    std::ifstream fin(file, std::ios_base::binary);
    fin >> current;
}

void
ChartMaker::Add_Sub_DataFromAnotherToCurrentPoints(char op) {
    std::string anotheFile;
    std::cout << "Enter path to file: ";
    std::cin >> anotheFile;
    Chart tmp(anotheFile);
    switch (op) {
        case '+':
          current = current + tmp; break;
        case '-':
          current = current - tmp; break;
    }
}

void
ChartMaker::ScaleCurrentPoints(char op) {
    Point p;
    std::cout << "Enter values to scale x & y coordinates: ";
    std::cin >> p;
    switch (op) {
        case '*':
          current = current * p; break;
        case '/':
          current = current / p; break;
    }
}

void
ChartMaker::PrintHistogramCurrentPoints() {
    char command;
    std::cout << "a: Print to Console\nb: Print to file";
    std::cin >> command;
    switch (command) {
       case 'a': {
          current.analyze(std::cout, 5);
          break;
       }
       case 'b': {
          std::string fileName;
          std::cout << "Enter file name: ";
          std::cin >> fileName;
          std::ofstream fout(fileName, std::ios_base::binary);
          current.analyze(fout, 5);
          break;
       }
       default: {
          std::cout << "Invaild command\n";
          break;
       }
    }
}

void
ChartMaker::ShiftPointsToLeft() {
    int pos;
    std::cout << "Enter positions to shift ";
    std::cin >> pos;
    current = current << pos;
}

void
ChartMaker::ShiftPointsToRight() {
    int pos;
    std::cout << "Enter positions to shift ";
    std::cin >> pos;
    current = current >> pos;
}

void
ChartMaker::SaveCurrentPointsToFile() {
    std::string fileName;
    std::cout << "Enter file name to which the data will be saved: ";
    std::cin >> fileName;
}

void
ChartMaker::PrintCurrentPoints() {
    std::cout << current;
}
