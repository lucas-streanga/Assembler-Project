#ifndef _H_BUFFER
#define _H_BUFFER

#include<cstddef>
#include<cstdlib>
#include<stdexcept>

template<class T>
class buffer
{
private:
  std::size_t size;
  T * data;
public:

  buffer(std::size_t size)
  {
    this->size = size;
    data = (T*) malloc(sizeof(T) * size);
    if(data == NULL)
    {
      size = 0;
      throw std::bad_alloc();
    }
  }

  std::size_t get_size()
  {
    return size;
  }
  T * get_buffer()
  {
    return data;
  }

  T & operator[](std::size_t index)
  {
    if(index >= size)
    {
      throw std::out_of_range();
    }
    return data[index];
  }

  ~buffer()
  {
    free(data);
  }
};

#endif
