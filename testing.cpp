#include <iostream>

template <typename T1, typename T2>
void    iter(T1* array, int len, T2 function)
{
    if (len < 0 || !array)
        return ;

    for(int i = 0; i < len; i++)
        function(array[i]);
}

template <typename T, typename T1, typename T2>
T Add(const T1& a, const T2& b)
{
    return (a + b);
}
/**************************************************************************/

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


void    printTest(Test& a)
{
    std::cout << a << std::endl;
}

void doubleVal(Test& t)
{
    t = Test(t.getVal() * 2);
}


int main()
{
    int x = 59;
    double y = 10.5;

    int res1 = Add<int>(x, y);
    std::cout << res1 << std::endl;

    char res2 = ::Add<char>(x, y);
    std::cout << res2 << std::endl;


    std::cout << "======================\n";

    Test arr[] = {Test(1), Test(2), Test(3)};

    iter(arr, 3, printTest);       // ok
    iter(arr, 3, doubleVal);   // ok

    const Test arr2[] = {Test(4),Test(5), Test(6)};
    iter(arr2, 3, printTest);      // ok
    iter(arr2, 3, doubleVal);  // 'X' error, cannot modify const element

}
