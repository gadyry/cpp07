#include <iostream>

template <typename T, typename T1, typename T2>
T Add(const T1& a, const T2& b)
{
    return (a + b);
}

int main()
{
    int x = 59;
    double y = 10.5;

    int res1 = Add<int>(x, y);
    std::cout << res1 << std::endl;

    char res2 = ::Add<char>(x, y);
    std::cout << res2 << std::endl;
}