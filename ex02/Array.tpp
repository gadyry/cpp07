#include "Array.hpp"

/*============== Implementation: ==============*/

template <typename T>
Array<T>::Array() : mData(NULL), mSize(0) {}

template <typename T>
Array<T>::Array(unsigned int n) : mData(NULL), mSize(n)
{
    if (n <= 0)
    {
        mData = NULL;
        return;
    }
    
    this->mData = new T[mSize]();
}

template <typename T>
Array<T>::Array(const Array& other) : mData(NULL), mSize(other.mSize)
{
    if (mSize <=  0)
    {
        mData = NULL;
        return;
    }

    mData = new T[mSize];
    for(unsigned int i = 0; i < mSize; i++)
        mData[i] = other.mData[i];
}

template <typename T>
Array<T>& Array<T>::operator=(const Array& other)
{
    if (this != &other)
    {
        delete[] mData;

        this->mSize = other.mSize;
        if (this->mSize <=  0)
        {
            mData = NULL;
            return;
        }

        this->mData = new T[mSize];
        for(int i = 0; i < mSize; i++)
            this->mData[i] = other.mData[i];
    }

    return (*this);
}

template <typename T>
Array<T>::~Array() { delete[] mData;}

template <typename T>
T&  Array<T>::operator[](unsigned int index)
{
    if (index < 0 || index >= mSize)
        throw std::out_of_range("Array::operator[] index out of range");
    return (mData[index]);
}

template <typename T>
unsigned int Array<T>::size() const
{
    return (mSize);
}