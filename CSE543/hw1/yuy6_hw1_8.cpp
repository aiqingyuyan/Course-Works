/**
 * @Author - Yan Yu
 * @Copyright - Miami University, 2014
 */

#include <cstdio>
#include <cstdlib>

void enterIntegers(int& x, int& y) {
    printf("Enter 2 integers: ");
    scanf("%d", &x);
    scanf("%d", &y);
}

void doMathCalculations(int& x, int& y) {
    printf("sum\t\t: %d\n", x + y);
    printf("product\t\t: %d\n", x * y);
    printf("difference\t: %d\n", std::abs(x - y));
    printf("quotient\t: \n");
}

int main(int argc, char** argv) {
    int a, b;
    enterIntegers(a, b);
    doMathCalculations(a, b);
    
    return 0;
}