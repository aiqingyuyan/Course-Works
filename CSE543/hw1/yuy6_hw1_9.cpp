/**
 * @Author - Yan Yu
 * @Copyright - Miami University, 2014
 */

#include <cstdio>
#include <initializer_list>
#include <algorithm>

void enterIntegers(int& x, int& y, int& z) {
    printf("Enter 3 integers: ");
    scanf("%d", &x);
    scanf("%d", &y);
    scanf("%d", &z);
}

void doComparison(const int& x, const int& y, const int& z) {
    printf("smallest: %d\n", std::min({x, y, z}));
    printf("largesr : %d\n", std::max({x, y, z}));
}

int main(int argc, char** argv) {
    int a, b, c;
    enterIntegers(a, b, c);
    doComparison(a, b, c);
    
    return 0;
}