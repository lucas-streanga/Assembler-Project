#ifndef _H_ALLOC_DETAILS
#define _H_ALLOC_DETAILS

#if SHOW_ALLOC == 1
#include <new>

struct alloc_details
{
  word total_bytes_allocated = 0;
  word total_bytes_free = 0;
  word number_of_allocs = 0;
  word number_of_frees = 0;

  word current_usage()
  {
    return total_bytes_allocated - total_bytes_free;
  }
};

static alloc_details s_alloc_details;

void* operator new(std::size_t size)
{
    s_alloc_details.total_bytes_allocated += size;
    s_alloc_details.number_of_allocs++;
    ALLOC_PRINT("New object created on heap with size " << size);
    void* result = malloc(size);
    if (result) return result;
    else
      throw std::bad_alloc();
}
void operator delete(void* ptr)
{
  s_alloc_details.number_of_frees++;
  ALLOC_PRINT("Object deleted off of heap.");
  if (ptr)
    free(ptr);
}
void operator delete(void * ptr, std::size_t size)
{
  s_alloc_details.number_of_frees++;
  s_alloc_details.total_bytes_free += size;
  ALLOC_PRINT("Object deleted off of heap with size " << size);
  if (ptr)
    free(ptr);
}
void* operator new[](std::size_t size)
{
  return operator new(size);  // defer to non-array version
}
void operator delete[](void* ptr)
{
  operator delete(ptr);  // defer to non-array version
}
void operator delete [](void * ptr, std::size_t size)
{
  operator delete(ptr, size);
}

#endif

void inline print_alloc_details()
{
  ALLOC_PRINT("*** Total objects allocated " << s_alloc_details.number_of_allocs);
  ALLOC_PRINT("*** Total objects freed " << s_alloc_details.number_of_frees);
  ALLOC_PRINT("*** Total bytes allocated " << s_alloc_details.total_bytes_allocated);
}

#endif
