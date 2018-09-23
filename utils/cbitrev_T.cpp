 //
 //  File = cbitrev_T.cpp
 //

 #include "cbitrev_T.h"
 using std::complex;

 template <class T>
 void ComplexBitReverse( complex<T> *array, int size)
 {
  complex<T> tt;
  int nv2, nm1, i, j, k;
  
  nv2 = size/2;
  nm1 = size - 1;
  
  j=0;
  for (i=0; i<nm1; i++)
    {
     if (i<j)
       {
        tt = array[j];
        array[j] = array[i];
        array[i] = tt;
       }
     k = nv2;
     while (k<=j)
       {
        j -= k;
        k /= 2;
       }
     j += k;
    }
 }
 //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  template void ComplexBitReverse<double>( complex<double> *array, int size);
  template void ComplexBitReverse<float>( complex<float> *array, int size);

