// xfelem.cpp
//

#include <iostream>
#include "galfield.h"
#include "xfelem.h"

ExtenFieldElem::ExtenFieldElem(void)
{
  Field=NULL;
  Value=0;
  FieldDegree=0;
}
ExtenFieldElem::ExtenFieldElem(GaloisField* field)
{
  //get m from field and use it to allocate Value[] array
  // initialize Value[] to zeros
  Field=field;
  FieldDegree = field->GetDegree();
  int base = field->GetBase();

  Value = new PrimeFieldElem[FieldDegree];
  for(int n=0; n<FieldDegree; n++)
    {
    Value[n]=PrimeFieldElem(base,0);
    }
}
ExtenFieldElem::ExtenFieldElem(GaloisField* field, int expon)
{
  //get m from field and use it to allocate Value[] array
  // initialize Value[] to zeros
   ExtenFieldElem rval;
  Field=field;
  FieldDegree = field->GetDegree();
  Value = new PrimeFieldElem[FieldDegree];
  rval = field->GetElement(expon);
  for(int n=0; n<FieldDegree; n++)
    {
    Value[n]=rval.Value[n];
    }
}

ExtenFieldElem operator+( const ExtenFieldElem e1,
                      const ExtenFieldElem e2)
  {
  ExtenFieldElem result;
  // we need to add (modulo "base") the coefficients of like
  // powered terms from _e1 and _e2
  int base = e1.Field->GetBase();
  result.Field=e1.Field;
  result.FieldDegree = e1.FieldDegree;
  result.Value = new PrimeFieldElem[result.FieldDegree];
  for(int n=0; n<e1.FieldDegree; n++)
    {
    result.Value[n] = (e1.Value[n] + e2.Value[n]);//%base;
    }
  return (result);
  }
ExtenFieldElem operator-( const ExtenFieldElem e1,
                      const ExtenFieldElem e2)
  {
  ExtenFieldElem result;
  // we need to add (modulo "base") the coefficients of like
  // powered terms from _e1 and _e2
  int base = e1.Field->GetBase();
  result.Field=e1.Field;
  result.FieldDegree = e1.FieldDegree;
  result.Value = new PrimeFieldElem[result.FieldDegree];
  for(int n=0; n<e1.FieldDegree; n++)
    {
    result.Value[n] = (e1.Value[n] - e2.Value[n]);//%base;
    }
  return (result);
  }

ExtenFieldElem operator*( const ExtenFieldElem e1,
                      const ExtenFieldElem e2)
  {
  ExtenFieldElem result;
  int field_degree, d1, d2;
  PrimeFieldElem *work, *temp_res;

  int base = e1.Field->GetBase();
  result.Field=e1.Field;
  field_degree = e1.FieldDegree;
  result.FieldDegree = field_degree;
  result.Value = new PrimeFieldElem[field_degree];

  work = new PrimeFieldElem[field_degree+1];
  temp_res = new PrimeFieldElem[field_degree+1];

  for(d1=0; d1<=field_degree; d1++)
    {
    work[d1] = e2.Value[d1];
    temp_res[d1] = PrimeFieldElem(base,0);
    }
  for(d1=0; d1<field_degree; d1++)
    {
    for(d2=0; d2<field_degree; d2++)
      {
//      temp_res[d2] = (temp_res[d2] + e1.Value[d1]*work[d2])%base;
      temp_res[d2] = temp_res[d2] + e1.Value[d1]*work[d2];
      }
    for(d2=field_degree; d2>=1; d2--)
      {
      work[d2] = work[d2-1];
      }
    work[0]=PrimeFieldElem(base,0);
    while(work[field_degree] !=0)
      {
      for(d2=0; d2<=field_degree; d2++)
        {
        work[d2] = ((result.Field)->Reduc_Poly[d2]+work[d2]);//%base;
        }
      }
    }
  for(d1=0; d1<field_degree; d1++)
    {
    result.Value[d1] = temp_res[d1];
    }
  return (result);
  }

ostream& operator<< ( ostream& s, const ExtenFieldElem& element)
{
  int deg = element.FieldDegree;
  for(int dgt_idx=deg-1; dgt_idx>=0; dgt_idx--)
    {
    s << (element.Value[dgt_idx]);
    }
  s << endl;
  return s;
}
