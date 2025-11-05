#include "Array.tpp"
#include <cstdlib>

int main()
{
    srand(time(NULL));

    Array<int> array1(5);
    Array<float> array2(5);
    Array<char> array3(10);
    Array<int>  empty;
    // Array<double> Over(2147483669);

    std::cout << "======== Testing the empty array ========\n";
    try
    {
        std::cout << "Empty array size: " << empty.size() << std::endl;
        empty[0] = 68;
        std::cout << empty[0] << std::endl;
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }

    std::cout << "======== Tetsing int : ========\n";
    for (unsigned int i = 0; i < array1.size(); i++)
        array1[i] = i * 10;

    for (unsigned int i = 0; i < array1.size(); i++)
        std::cout << array1[i] << std::endl;
    
    try
    {
        array1[5] = 69;
        std::cout << array1[5] << std::endl;
    }
    catch (const std::exception& e) 
    {
        std::cerr << "Caught exception: " << e.what() << std::endl;
    }

    std::cout << "======== Tetsing float : ========\n";
    for (unsigned int i = 0; i < array2.size(); i++)
        array2[i] = 69 + ( rand() % 1337 ) + 0.4578f;
    
    for (unsigned int i = 0; i < array2.size(); i++)
        std::cout << array2[i] << std::endl;

    std::cout << "======== Tetsing char array : ========\n";

    const char str[] = "1337";
    for(unsigned int i = 0; i < array3.size() && str[i]; i++)
        array3[i] = str[i];

    for (unsigned int i = 0; i < array3.size(); i++)
        std::cout << array3[i];

    std::cout << "\n======================================\n";

    try
    {
        Array<int> a(3);
        std::cout << a[69] << std::endl;
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception caught: " << e.what() << std::endl;
    }

    std::cout << "\n====== End Tests: =======\n";

    return (0);
}