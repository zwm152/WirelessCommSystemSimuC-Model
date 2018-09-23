//
//  file = prog_14a.cpp
//
 #include <stdlib.h> 
 #include <iostream> 
 #include <fstream>
 #include <math.h>
 #include "misdefs.h" 
 #include "fir_resp.h"
 #include "fir_dsgn.h"
 #include "BP_remezalg.h"
 #include "string.h"
 #include <strstream>
   

 #ifdef _DEBUG
 ofstream DebugFile("remez.out", ios::out);
 #endif

 main()
 {
  BpRemezAlgorithm *remez;
  FirFilterDesign *fir_filter;
  FirFilterResponse *filter_response;
  strstream *fnam_stream;
  char fnam_strm_buf[255];
  int filter_length;
  int coeff_indx;
  double ripple_ratio;
  double passband_edge_freq_low;
  double stopband_edge_freq_low;
  double passband_edge_freq_high;
  double stopband_edge_freq_high;
  double *extremal_freqs;
  double *filter_coeffs;
  double noise_equiv_bw;
  fstream *seq_file;
  int file_seq_num;
//  double *magnitude_response;
//  double quant_factor;
  int grid_density;
  int i;
  int op_mode;
  fstream *coeff_file;

//  char* coeff_fname;

  cout << "desired mode ?\n" << endl;
  cout << "1 = remez" << endl;
  cout << "2 = plot for existing coeff\n" << endl;
  cin >> op_mode;

  //-------------------------------------------------------------
  //  Plot Response of existing filter

if(op_mode == 2) {
      //coeff_file = new ifstream(coeff_fname, ios::in);
      coeff_file = new fstream("interp_filt_coeff.txt", ios::in);
      *coeff_file >> filter_length;

      filter_coeffs = new double[filter_length];

      for( i=0; i<filter_length; i++) {
         *coeff_file >> filter_coeffs[i];
      }

   fir_filter = new FirFilterDesign( filter_length, 
                                    FIR_SYM_EVEN_LEFT, 
                                    filter_coeffs);

  filter_response = new FirFilterResponse( fir_filter,
                                           cin, cout);
  filter_response->ComputeMagResp();
  filter_response->NormalizeResponse();
  filter_response->DumpMagResp();
  exit(88);
  }  // end of if(op_mode == 2)


  //------------------------------------------------------------
  //  Design filter using Remez 
   if(op_mode == 1) {
      seq_file = new fstream("FilterUtilMemory.txt", ios::in);
      *seq_file >> file_seq_num;
      seq_file->close();
      delete seq_file;

      file_seq_num++;
      seq_file = new fstream("FilterUtilMemory.txt", ios::out);
      *seq_file << file_seq_num << endl;
      seq_file->close();

      fnam_stream = new strstream(fnam_strm_buf, 255, ios::out);
      for(i=0; i<255; i++) {
         fnam_strm_buf[i] = NULL;
      }
      *fnam_stream << "remez_filt_coeff_" << file_seq_num << ".txt";
      coeff_file = new fstream(fnam_strm_buf, ios::out);
      delete fnam_stream;

      read_filter_length:
      cout << "filter length ?\n (must be odd)" << endl;
      cin >> filter_length;
      if((filter_length%2) == 0) goto read_filter_length;
  
  cout << "ripple ratio?" << endl;
  cin >> ripple_ratio;
  
  cout << "low stopband edge frequency?" << endl;
  cin >> stopband_edge_freq_low;

  cout << "low passband edge frequency?" << endl;
  cin >> passband_edge_freq_low;
  
  cout << "high passband edge frequency?" << endl;
  cin >> passband_edge_freq_high;
  
  cout << "high stopband edge frequency?" << endl;
  cin >> stopband_edge_freq_high;
  
//  cout << "quantizing factor?\n"
//       << " ( 256 for 8 bits, 1024 for 10 bits, etc. )"
//       << endl;
//  cin >> quant_factor;
    
  cout << "Remez grid density?\n"
       << endl;
  cin >> grid_density;
    
  extremal_freqs = new double[(filter_length+1)/2];
  filter_coeffs = new double[filter_length];
  
  cout << "ripple_ratio = " << ripple_ratio << endl;
  cout << "stopband_edge_freq_low = " << stopband_edge_freq_low << endl;
  cout << "passband_edge_freq_low = " << passband_edge_freq_low << endl;
  cout << "passband_edge_freq_high = " << passband_edge_freq_high << endl;
  cout << "stopband_edge_freq_high = " << stopband_edge_freq_high << endl;
  cout << "filter_length = " << filter_length << endl;
//  cout << "quant_factor = " << quant_factor << endl;
  
  remez = new BpRemezAlgorithm( //cin, cout,
                              grid_density,
                              filter_length,
                              stopband_edge_freq_low,
                              passband_edge_freq_low,
                              passband_edge_freq_high,
                              stopband_edge_freq_high,
                              ripple_ratio,
                              &fir_filter);

  fir_filter->CopyCoefficients(filter_coeffs);  
  
   #ifdef _DEBUG
      for( coeff_indx=0; coeff_indx< filter_length; coeff_indx++){
         DebugFile << "coeff[" << coeff_indx << "] = "
               << filter_coeffs[coeff_indx] << endl;
      }
   #endif

   *coeff_file << filter_length << endl;

   for( coeff_indx=0; coeff_indx< filter_length; coeff_indx++) {
      *coeff_file << filter_coeffs[coeff_indx] << endl;
   }

   *coeff_file << "ripple_ratio = " << ripple_ratio << endl;
   *coeff_file << "passband_edge_freq = " << passband_edge_freq << endl;
   *coeff_file << "stopband_edge_freq = " << stopband_edge_freq << endl;
   *coeff_file << "grid_density = " << grid_density << endl;
//   coeff_file->close();
 

   fnam_stream = new strstream(fnam_strm_buf, 255, ios::out);
   for(i=0; i<255; i++) {
      fnam_strm_buf[i] = NULL;
   }
   *fnam_stream << "remez_mag_resp_" << file_seq_num << ".txt";
   //coeff_file = new fstream(fnam_strm_buf, ios::out);

   filter_response = new FirFilterResponse( fir_filter,
                                             1500, // num response points
                                             1, // dB scale enabled
                                             1, // normalize enabled
                                             fnam_strm_buf);
   noise_equiv_bw = filter_response->ComputeMagResp();
   *coeff_file << "noise_equiv_bw = " << noise_equiv_bw << endl;
   coeff_file->close();

   filter_response->NormalizeResponse();
   filter_response->DumpMagResp();
   delete remez;
   delete coeff_file;
   exit(88);
   }  // end of if(op_mode == 1)
 
//  delete [] magnitude_response;
//  delete [] extremal_freqs;
//  delete [] filter_coeffs;
  
  return 0;
 }  

