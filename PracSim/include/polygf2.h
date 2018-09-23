//
//  File = polygf2.h
//

#ifndef _POLYGF2_H_
#define _POLYGF2_H_

class PolyOvrGF2
{
public:

  PolyOvrGF2(void);
  PolyOvrGF2(int degree);
  PolyOvrGF2(int degree, int coeff);
  int MaxDegree(void);
  int Coefficient(int degree);
  void SetRemainder( PolyOvrGF2* poly );
  void SetRemainder( int degree, int* coeff );
  PolyOvrGF2& operator* (const PolyOvrGF2 &right);
  PolyOvrGF2& operator*= (const PolyOvrGF2 &right);
  PolyOvrGF2& operator/ (const PolyOvrGF2 &divisor);
  PolyOvrGF2& operator/= (const PolyOvrGF2 &divisor);
  void DumpToStream( ostream* output_stream);

private:
  int Degree;
  int* Coeff;
  int Rem_Degree;
  int* Rem_Coeff;
};


#endif
