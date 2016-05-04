// Implementation of the templated Vector class
// ECE4893/8893 lab 3
// Balaji Mamidala

#include <iostream> // debugging
#include "Vector.h"

// Your implementation here
// Fill in all the necessary functions below
using namespace std;

// Default constructor
template <typename T>
Vector<T>::Vector()
{
  elements = NULL;
  count = 0;
  reserved = 0;
}

// Copy constructor
template <typename T>
Vector<T>::Vector(const Vector& rhs)
{
  if(0 != rhs.reserved)
  {
    elements = (T*) malloc(sizeof(T)*rhs.reserved);
    reserved = rhs.reserved;

    for(size_t i=0; i<rhs.count; i++)
    {
      new (&elements[i]) T((rhs.elements)[i]);
    }
    count =  rhs.count;
  }
  else
  {
    elements = NULL;
    count = 0;
    reserved = 0;
  }
}

// Assignment operator
template <typename T>
Vector<T>& Vector<T>::operator=(const Vector& rhs)
{
  if(&rhs != this)
  {
    if(0 != reserved)
    { 
      this->~Vector();
      elements = NULL;
      reserved = 0;
      count = 0;
    }
    if(0 != rhs.reserved)
    {
      elements = (T*) malloc(sizeof(T)*rhs.reserved);
      reserved = rhs.reserved;
      for(size_t i=0; i<rhs.count; i++)
      {
        new (&elements[i]) T((rhs.elements)[i]);
      }
      count = rhs.count;
    } 
  }

  return *this;
}

#ifdef GRAD_STUDENT
// Other constructors
template <typename T>
Vector<T>::Vector(size_t nReserved)
{ // Initialize with reserved memory
  if(0 != nReserved)
  {
    elements = (T*) malloc(sizeof(T)*nReserved);
    reserved = nReserved;
    count = 0;
  }
  else
  {
    elements = NULL;
    reserved = 0;
    count = 0;
  }
}

template <typename T>
Vector<T>::Vector(size_t n, const T& t)
{ // Initialize with "n" copies of "t"
  elements = (T*) malloc(sizeof(T)*n);
  reserved = n;

  for(size_t i=0; i<n; i++)
  {
    new (&elements[i]) T(t);
  }
  count = n;
}

template <typename T>
void Vector<T>::Reserve(size_t n)       
{ // Ensure enough memory for "n" elements
  if(n > reserved)
  {
    T* temp = (T*) malloc(sizeof(T)*n);

    for(size_t i=0; i<count; i++)
    {
      new (&temp[i]) T(elements[i]);
    }

    this->~Vector();

    elements = temp;
    reserved = n;
  }
}
#endif

// Destructor
template <typename T>
Vector<T>::~Vector()
{
  for(size_t i=0; i<count; i++)
  {
    elements[i].~T();
  }

  free(elements);
}

// Add and access front and back
template <typename T>
void Vector<T>::Push_Back(const T& rhs)
{
  if(count == reserved)
  {
    Reserve(count+1);  
  }

  new(&elements[count]) T(rhs);
  count++;
}

template <typename T>
void Vector<T>::Push_Front(const T& rhs)
{
  T* temp;
  if(count == reserved)
  {
    temp = (T*) malloc(sizeof(T)*(count+1));
    reserved = count + 1;
  }
  else
  {
    temp = (T*) malloc(sizeof(T)*reserved);
  }
  
  for(size_t i=0; i<count; i++)
  {
    new (&temp[i+1]) T(elements[i]);
  }

  this->~Vector();

  elements = temp; 
  new (&elements[0]) T(rhs);
  count++;
}

template <typename T>
void Vector<T>::Pop_Back()
{ // Remove last element
  if(0 != count)
  {
    elements[count-1].~T();
    count--;
  }
  else
  {
    return;
  }
}

template <typename T>
void Vector<T>::Pop_Front()
{ // Remove first element
  if(0 != count)
  {
    T *temp;
    if(count == reserved)
    {
      temp = (T*) malloc(sizeof(T)*count);
    }
    else
    {
      temp = (T*) malloc(sizeof(T)*reserved);
    }
    
    for(size_t i=0; i<count-1; i++)
    {
      new (&temp[i]) T(elements[i+1]);
    }

    this->~Vector();

    elements = temp;
    count--;  
  }
  else
  {
    return;
  }
}

// Element Access
template <typename T>
T& Vector<T>::Front() const
{
  return elements[0];
}

// Element Access
template <typename T>
T& Vector<T>::Back() const
{
  return elements[count-1];
}

template <typename T>
T& Vector<T>::operator[](size_t i) const
{
  return elements[i];
}

template <typename T>
size_t Vector<T>::Size() const
{
  return count;
}

template <typename T>
bool Vector<T>::Empty() const
{
  if(0 == count)
  {
    return true;
  }

  return false;
}

// Implement clear
template <typename T>
void Vector<T>::Clear()
{
  for(size_t i=0; i<count; i++)
  {
    elements[i].~T();
  }

  count = 0;
}

// Iterator access functions
template <typename T>
VectorIterator<T> Vector<T>::Begin() const
{
  return VectorIterator<T>(elements);
}

template <typename T>
VectorIterator<T> Vector<T>::End() const
{
  return VectorIterator<T>(elements+count);
}

#ifdef GRAD_STUDENT
// Erase and insert
template <typename T>
void Vector<T>::Erase(const VectorIterator<T>& it)
{  
  for(size_t i=0; i<count; i++)
  {
    if(&elements[i] > it.current)
    {
      elements[i-1].~T();
      new(&elements[i-1]) T(elements[i]);
    }
  }
  
  elements[count-1].~T();
  count--;
}

template <typename T>
void Vector<T>::Insert(const T& rhs, const VectorIterator<T>& it)
{
  T* temp;
  if(count == reserved)
  {
    temp = (T*) malloc(sizeof(T)*(reserved+1));
    reserved++;
  }
  else
  {
    temp = (T*) malloc(sizeof(T)*(reserved));
  }

  for(size_t i=0; i<count; i++)
  {
    if(&elements[i] == it.current)
    {
      new (&temp[i]) T(rhs);
      new (&temp[i+1]) T(elements[i]);
    }
    else if(&elements[i] > it.current)
    {
      new (&temp[i+1]) T(elements[i]);
    }
    else
    {
      new (&temp[i]) T(elements[i]);
    }
  }

  this->~Vector();
  elements = temp;
  count++;  
}
#endif

// Implement the iterators

// Constructors
template <typename T>
VectorIterator<T>::VectorIterator()
{
  current = NULL;
}

template <typename T>
VectorIterator<T>::VectorIterator(T* c)
{
  current = c;
}

// Copy constructor
template <typename T>
VectorIterator<T>::VectorIterator(const VectorIterator<T>& rhs)
{
  current = rhs.current;  
}

// Iterator defeferencing operator
template <typename T>
T& VectorIterator<T>::operator*() const
{
  return *current;
}

// Prefix increment
template <typename T>
VectorIterator<T>  VectorIterator<T>::operator++()
{
  return ++current;
}

// Postfix increment
template <typename T>
VectorIterator<T> VectorIterator<T>::operator++(int)
{
  return current++;
}

// Comparison operators
template <typename T>
bool VectorIterator<T>::operator !=(const VectorIterator<T>& rhs) const
{
  if(current != rhs.current)
  {
    return true;
  }
  
  return false;
}

template <typename T>
bool VectorIterator<T>::operator ==(const VectorIterator<T>& rhs) const
{
  if(current == rhs.current)
  {
    return true;
  }
  
  return false;
}


