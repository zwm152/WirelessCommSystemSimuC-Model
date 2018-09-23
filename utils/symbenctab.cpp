// symbenctab.cpp
//

#include "iostream.h"
#include "symbenctab.h"
#include "xor.h"
#include "gen_tools.h"

SymbEncodingTable::SymbEncodingTable( unsigned int rate_numer, 
                                      unsigned int rate_denom,
                                      unsigned int *inp_polys)
{
  //  rate_numer is the number of input bits processed each cycle
  //  rate_denom is the number of output bits generated each cycle
  //  polys points to a set of rate_numer*rate_denom polynomials
  //        the polynomial pointed to by polys[i][j] operates on
  //        the i-th input subsequence and contributes to the
  //        j-th output subsequence

  unsigned int j;
  unsigned int out_val;
  int composite;
  int *polys;
  int num_active_bits;

  polys = new int[rate_numer];

  composite = 0;
  for(j=0; j<rate_denom; j++)
    {
    composite |= inp_polys[j];
    }
  num_active_bits = ActiveBitCount(composite);

  for( j=0; j<rate_denom; j++)
    {
    polys[j] = ReverseBits(inp_polys[j], num_active_bits);
    cout << "P[" << j << "] = " << polys[j] << endl;
    }

  //---------------------------
  Table_Len = 1<<num_active_bits;
  Out_Table = new int[Table_Len];

  for(int n=0; n<Table_Len; n++)
    {
    out_val=0;
    for(unsigned int out_num=0; out_num<rate_denom; out_num++)
      {
      out_val <<= 1;
      out_val |= xor(n&polys[out_num]);
      }
    Out_Table[n] = out_val;
    }
}
//=====================================================
SymbEncodingTable::~SymbEncodingTable()
{
  delete [] Out_Table;
}

//====================================================
int SymbEncodingTable::GetOutput(int n)
{
  return(Out_Table[n]);
}
//=====
//fill
//fill
//fill
//fill
//fill
//fill
//fill
//fill
//fill
//fill
//fill
//fill
//fill
//fill
//fill
//fill
//fill
//fill
//fill
//fill
//fill
//fill
//fill
//fill
//fill
//fill
//fill
//fill
