#include "iter.hpp"

class Test
{
private :
    int val;
public :
    Test(int val) : val(val) {}
    bool    operator>(const Test& b) { return (this->val > b.val); }
    bool    operator<(const Test& b) { return (this->val < b.val); }

    int    getVal() const { return (val); }
};

std::ostream &operator<<(std::ostream &out, Test const &a) 
{
	out << a.getVal();
	return (out);
}

void    printInteger(int n)
{
    std::cout << n << std::endl;
}

void    printFloat(float r)
{
    std::cout << r << std::endl;
}

void    printChar(char c)
{
    std::cout << c << std::endl;
}

void    printTest(Test& a)
{
    std::cout << a << std::endl;
}

int main()
{
    {
        int arr1[] = {1, 2, 3, 4};
        iter(arr1, 4, printInteger);

        std::cout << "\n===============\n";
                
        float arr2[] = {1, 2.4, 3.69, 4};
        iter(arr2, 4, printFloat);

        std::cout << "\n===============\n";

        char arr3[] = {'1', '3', '3', '7'};
        iter(arr3, 4, printChar);
    }

    {
        std::cout << "\n====== Test class =========\n";
        Test arrayTest[] =  {Test(5), Test(2), Test(8), Test(1)};
        iter(arrayTest, 4, printTest);
    }

    {
        std::cout << "\n====== Overflow : =========\n";
        Test arrayTest[] =  {Test(5), Test(2), Test(8), Test(1)};
        iter(arrayTest, -4, printTest);
    }

    {
        /*
            "The function passed as the third parameter may take 
            its argument by const reference
            or non-const reference, depending on the context.

            Think carefully about how to support both const and non-const
            elements in your iter func".
        */


       // *** CHECK THE FILE testing.cpp To understand what Happened! *** //
    }

    return (0);
}
