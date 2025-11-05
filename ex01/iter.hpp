#ifndef ITER_HPP
#define ITER_HPP

#include <iostream>

template <typename T1, typename T2>
void    iter(const T1 array[], int len, T2 function)
{
    if (len < 0 || !array)
        return ;

    for(int i = 0; i < len; i++)
        function(array[i]);
}

template <typename T1, typename T2>
void    iter(T1* array, int len, T2 function)
{
    if (len < 0 || !array)
        return ;

    for(int i = 0; i < len; i++)
        function(array[i]);
}

#endif