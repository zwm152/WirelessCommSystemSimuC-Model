//
// primpoly.cpp
//

#include <iostream>
#include <fstream>
#include "cyclo_partit.h"
#include "ipow.h"
//using namespace std;
extern ::ofstream DebugFile;


//=====================================================
CyclotomicPartition::CyclotomicPartition( int prime_field_base,
                                                int degree )
{
   int first_element;
   int total_count;
   int element_count;
   int i;
   int element;
   int base;

   Specific_Coset = new CyclotomicCoset;
   Modulus = int(ipow(prime_field_base, degree)-1);
   total_count = 0;
   Coset_Start = new int[(Modulus+1)/2];
   Coset_Size = new int[(Modulus+1)/2];
   Element_Vector = new int[Modulus];
   bool* used = new bool[Modulus];
   for(i=0; i<Modulus; i++){
      used[i] = false;
   }
   Coset_Start[0] = 0;
   Coset_Size[0] = 1;
   Element_Vector[0] = 0;
   Num_Cosets = 1;
   total_count++;

   first_element = -1;
   do{
      do{
         first_element += 2;
      }while(used[first_element]);
      element = first_element;
      element_count = 0;

      Coset_Start[Num_Cosets] = Coset_Start[Num_Cosets-1] + Coset_Size[Num_Cosets-1];
      base = Coset_Start[Num_Cosets];

      do{
         Element_Vector[base + element_count] = element;
         used[element] = true;
         element_count++;
         total_count++;
         element = (prime_field_base * element)%Modulus;
      } while(element != first_element);
      Coset_Size[Num_Cosets] = element_count;
      Num_Cosets++;
   }while(total_count <(Modulus-1));
}
//=======================================================
int CyclotomicPartition::GetNumCosets(void)
{
   return(Num_Cosets);
}
//=======================================================
CyclotomicCoset *CyclotomicPartition::GetSpecificCoset(int coset_index)
{
   Specific_Coset->start = &(Element_Vector[Coset_Start[coset_index]]);
   Specific_Coset->size = Coset_Size[coset_index];
   return(Specific_Coset);
}
//===========================================================
int CyclotomicPartition::GetCosetIndex(int element)
{
   int i, j, index;
   i=-1;
   do{
      i++;
   }while( (i<(Modulus-1)) && (element != Element_Vector[i]));
   for(j=0; j<Num_Cosets; j++){
      if( (i>=Coset_Start[j]) && (i<(Coset_Start[j]+Coset_Size[j]))){
         index = j;
         break;
      }
   }
   return(index);
}
