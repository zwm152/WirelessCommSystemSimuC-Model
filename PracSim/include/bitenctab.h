//
//  File = bitenctab.h
//

#ifndef _BITENCTAB_H_
#define _BITENCTAB_H_

class BitEncodingTable
{
public:

  BitEncodingTable( int k, int poly);
  ~BitEncodingTable();
  int GetOutput(int n);

private:
  int *Out_Table;
  int Table_Len;
};
#endif
