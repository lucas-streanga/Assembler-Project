#include"Virtual_memory.h"
#include<stdio.h>
#include"error_handler.h"

Virtual_memory::Virtual_memory(size_t size)
{
  LOG("Initializing Memory...");
  this->size = size;
  data = (byte *) calloc(size, 4);
  if(data == NULL)
    error_handler(ERR_UTA, 0, NULL, NULL);

  LOG("Memory Initialized Successfully to " << size << " bytes\n");
}

Virtual_memory::~Virtual_memory()
{
  if(data != NULL)
    free(this->data);
}

byte& Virtual_memory::operator[](dword index)
{
  return this->data[index];
}
