//
//  File = aux_sig_buf.h
//

#ifndef _AUX_SIG_BUF_H_
#define _AUX_SIG_BUF_H_


template <class T>
class AuxSignalBuffer
{
public:
  AuxSignalBuffer(  T sample, int nominal_block_size );

  ~AuxSignalBuffer(void);

   T* Load(  T *in_sig_ptr,
               int in_sig_block_size,
               int *in_sig_buf_count);

   void Release(  int num_samps_to_release);

private:
  T* Buffer_Start;
  T* Write_Ptr;
  int Sample_Count;
  int Max_Sample_Count;

};

#endif
