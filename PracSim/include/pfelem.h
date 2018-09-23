//
//  File = pfelem.h
//

#ifndef _PFELEM_H_
#define _PFELEM_H_

class PrimeFieldElem
{
public:

  PrimeFieldElem(void);
  PrimeFieldElem(int modulus);
  PrimeFieldElem(int modulus, int value);
  //void operator=(const int);
  bool operator==(const int);
  bool operator!=(const int);
  void operator=(const PrimeFieldElem&);
  void operator+=(const PrimeFieldElem&);
  void operator*=(const PrimeFieldElem&);
  void operator-=(const PrimeFieldElem&);
//  operator int(){return Value;}
  friend PrimeFieldElem operator+( const PrimeFieldElem,
                            const PrimeFieldElem );
  friend PrimeFieldElem operator-( const PrimeFieldElem,
                            const PrimeFieldElem );
  friend PrimeFieldElem operator*( const PrimeFieldElem,
                            const PrimeFieldElem );
  friend PrimeFieldElem operator/( const PrimeFieldElem,
                            const PrimeFieldElem );
  friend PrimeFieldElem operator*( const int,
                            const PrimeFieldElem );
  friend PrimeFieldElem neg(PrimeFieldElem);
  friend PrimeFieldElem recip(PrimeFieldElem);
  int Value;
  int Modulus;

private:
};

//inline void PrimeFieldElem::operator=(const int value)
//{Value=value;}

inline bool PrimeFieldElem::operator==(const int value)
{return(Value==value);}

inline bool PrimeFieldElem::operator!=(const int value)
{return(Value!=value);}

inline void PrimeFieldElem::operator=(const PrimeFieldElem &right)
  {Modulus = right.Modulus;
  Value = right.Value;}

inline void PrimeFieldElem::operator+=(const PrimeFieldElem &right)
  { Value = (Value + right.Value)%Modulus;}

inline void PrimeFieldElem::operator*=(const PrimeFieldElem &right)
  { Value = (Value * right.Value)%Modulus;}

inline void PrimeFieldElem::operator-=(const PrimeFieldElem &right)
  { Value = (Modulus + Value - right.Value)%Modulus;}

inline PrimeFieldElem operator+( const PrimeFieldElem _e1,
                          const PrimeFieldElem _e2)
  {return PrimeFieldElem(_e1.Modulus,(_e1.Value + _e2.Value));}

inline PrimeFieldElem operator-( const PrimeFieldElem _e1,
                          const PrimeFieldElem _e2)
  {return PrimeFieldElem(_e1.Modulus,(_e1.Value + _e1.Modulus - _e2.Value));}

inline PrimeFieldElem operator*( const PrimeFieldElem _e1,
                          const PrimeFieldElem _e2)
  {return PrimeFieldElem(_e1.Modulus,(_e1.Value * _e2.Value));}

inline PrimeFieldElem operator*( const int _e1,
                                  const PrimeFieldElem _e2)
  {return PrimeFieldElem(_e2.Modulus,(_e1 * _e2.Value));}

#include <iostream>
  using namespace std;
ostream& operator<< (ostream&, const PrimeFieldElem&);
#endif
