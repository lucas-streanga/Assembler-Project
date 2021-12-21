#include"lookup_tables.h"
#include<iostream>

Lookup * Lookup::m_lookup = nullptr;

Lookup & Lookup::get()
{
  if(!m_lookup)
    m_lookup = new Lookup();

  return *m_lookup;
}

void Lookup::destroy()
{
  if(m_lookup)
    delete m_lookup;

  m_lookup = nullptr;
}

int Lookup::label_table::size()
{
  return label_map.size();
}
