#ifndef _H_BUFFER
#define _H_BUFFER

#include<cstddef>
#include<cstdlib>

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
      size = 0;
  }

  std::size_t get_size()
  {
    return size;
  }
  T * get_buffer()
  {
    return data;
  }

  ~buffer()
  {
    free(data);
  }
};

#endif
