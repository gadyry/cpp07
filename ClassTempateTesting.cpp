#include <iostream>
#include <cstring>

template <typename T1>
class Container
{
public:
    T1* m_data;

    Container(int N)
    {
        if (N < 0)
            return ;
        this->m_data = new T1[N];
    }
    ~Container() { delete[] m_data;}

};

int main()
{
    Container<int> c1(5);
    Container<float> c2(5);
    Container<char> c3(10);

    std::cout << "======== Tetsing int : ========\n";
    for (int i = 0; i < 5; i++)
        c1.m_data[i] = (i != 0) ? 69 % i : 0;

    for (int i = 0; i < 5; i++)
        std::cout << c1.m_data[i] << std::endl;

    std::cout << "======== Tetsing float : ========\n";
    for (int i = 0; i < 5; i++)
        c2.m_data[i] = 69 + ( rand() % 69 );
    
    for (int i = 0; i < 5; i++)
        std::cout << c2.m_data[i] << std::endl;

    std::cout << "======== Tetsing char : ========\n";

        c3.m_data[0] = '1';
        c3.m_data[1] = '3';
        c3.m_data[2] = '3';
        c3.m_data[3] = '7';
        c3.m_data[4] = '\0';

    for (int i = 0; i < 5; i++)
        std::cout << c3.m_data[i];

    std::cout << "\n====== End Tests: =======\n";
}
