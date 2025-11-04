#include "whatever.hpp"

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

int main()
{
    {
        std::cout <<"======= testing swap function : =======\n";

        int x = -69;
        int a = 69;
        float y = 69.69f;
        float z = 9.94f;

        swap(x, a);
        std::cout <<"x: "<< x << " | a: " << a << std::endl;

        swap(y, z);
        std::cout <<"y: "<< y << " | z: " << z << std::endl;
    }

    {
        std::cout <<"\n======= testing min function : =======\n";

        int x = -69;
        int a = 69;
        float y = 69.69f;
        float z = 9.94f;


        std::cout << "Min(x, a) => " << (float)min(x, a) << std::endl;
        std::cout << "Min(y, z) => " << min(y, z) << std::endl;
    }

    {
        std::cout <<"\n======= testing Max function : =======\n";

        int x = -69;
        int a = 69;
        float y = 69.69f;
        float z = 9.94f;


        std::cout << "Max(x, a) => " << min(x, a) << std::endl;
        std::cout << "Max(y, z) => " << min(y, z) << std::endl;
    }

    {
        int b = 3;
        int a = 2;

        ::swap( a, b );
        std::cout << "a = " << a << ", b = " << b << std::endl;
        std::cout << "min( a, b ) = " << ::min( a, b ) << std::endl;
        std::cout << "max( a, b ) = " << ::max( a, b ) << std::endl;

        std::string c = "chaine1";
        std::string d = "chaine2";

        ::swap(c, d);
        std::cout << "c = " << c << ", d = " << d << std::endl;
        std::cout << "min( c, d ) = " << ::min( c, d ) << std::endl;
        std::cout << "max( c, d ) = " << ::max( c, d ) << std::endl;
    }

    {
        std::cout <<"\n======= Test Class : =======\n";
        Test x(10), y(42);
        ::swap(x, y);
        std::cout << "x = " << x << ", y = " << y << std::endl;
        std::cout << "min(x, y) = " << ::min(x, y) << std::endl;
        std::cout << "max(x, y) = " << ::max(x, y) << std::endl;
    }

    std::cout << "\n=======  End of testing!  =======" << std::endl;

    return (0);
}
