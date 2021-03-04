#include"Virtual_memory.h"
#include<stdio.h>
#include"error_handler.h"

Virtual_memory::Virtual_memory(size_t size)
{
  LOG("Initializing Memory...");
  this->size = size;
  data = (byte *) malloc(size * 4);
  if(data == NULL)
    error_handler(ERR_UTA, 0, NULL, NULL);
  for(dword i = 0; i < size; i++)
    data[i] = 0;

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
