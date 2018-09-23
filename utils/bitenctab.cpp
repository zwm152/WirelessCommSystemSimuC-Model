// bitenctab.cpp
//

#include "iostream.h"
#include "bitenctab.h"
#include "xor.h"

BitEncodingTable::BitEncodingTable(int k, int poly)
{
  Table_Len = 1<<k;
  Out_Table = new int[Table_Len];

  for(int n=0; n<Table_Len; n++)
    {
    Out_Table[n] = xor(n & poly);
    }
}
BitEncodingTable::~BitEncodingTable()
{
  delete [] Out_Table;
}

int BitEncodingTable::GetOutput(int n)
{
  return(Out_Table[n]);
}
