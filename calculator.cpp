#include <iostream>


// the calc functions
double add(double a, double b)    { return a + b; }
double subtract(double a, double b) { return a - b; }
double multiply(double a, double b) { return a * b; }
double divide(double a, double b)   { return b != 0 ? a / b : throw std::invalid_argument("div by 0"); }

int main() {
    std::cout << "3 + 5 = " << add(3, 5) << "\n";
    std::cout << "10 - 7 = " << subtract(10, 7) << "\n";
    std::cout << "4 * 2.5 = " << multiply(4, 2.5) << "\n";
    std::cout << "9 / 3 = " << divide(9, 3) << "\n";
}

bool test1() {
    :)
}