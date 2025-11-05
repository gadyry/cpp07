#include <iostream>

class Test {
private:
    int val;
public:
    Test() : val(69) {}  // default constructor sets val = 69
    int getVal() const { return val; }
};

int main() {
    const int size = 5;

    std::cout << "=== Primitive type: int ===\n";
    
    // Without parentheses: garbage values
    int* arr1 = new int[size];
    std::cout << "new int[size]: ";
    for (int i = 0; i < size; ++i) std::cout << arr1[i] << " ";
    std::cout << "\n";

    // With parentheses: zero-initialized
    int* arr2 = new int[size]();
    std::cout << "new int[size](): ";
    for (int i = 0; i < size; ++i) std::cout << arr2[i] << " ";
    std::cout << "\n";

    delete[] arr1;
    delete[] arr2;

    std::cout << "\n=== Class type: Test ===\n";

    // Without parentheses
    Test* tests1 = new Test[size];  // default constructor called anyway
    std::cout << "new Test[size]: ";
    for (int i = 0; i < size; ++i) std::cout << tests1[i].getVal() << " ";
    std::cout << "\n";

    // With parentheses
    Test* tests2 = new Test[size]();  // same as above
    std::cout << "new Test[size](): ";
    for (int i = 0; i < size; ++i) std::cout << tests2[i].getVal() << " ";
    std::cout << "\n";

    delete[] tests1;
    delete[] tests2;

    return 0;
}
