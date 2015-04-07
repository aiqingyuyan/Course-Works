/**
 * @Author - Yan Yu
 * @Copyright - Miami University, 2014
 */

#include <unordered_map>
#include <string>
#include <iostream>

typedef std::unordered_map<std::string, unsigned int> Dict;

Dict dict = {{"january", 1}, {"february", 2}, {"march", 3},
             {"april", 4}, {"may", 5}, {"june", 6}, {"july", 7},
             {"august", 8}, {"september", 9}, {"october", 10},
             {"november", 11}, {"december", 12}};
             
int main(int argc, char** argv) {
    std::cout << "Enter month: ";
    std::string mon;
    std::cin >> mon;
    printf("Month: %d\n", dict.at(mon));
}