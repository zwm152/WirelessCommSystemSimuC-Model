//
//  File = parmfile.cpp
//

#include <stdlib.h>
#include <fstream>
#include <string.h>
#include <direct.h>
#include "parmfile.h"


bool isnumeric(char* str_ptr)
{
   char* c_ptr;

   for(c_ptr=str_ptr; c_ptr<str_ptr+strlen(str_ptr);c_ptr++) {
      if( !isdigit(*c_ptr) 
         && *c_ptr != '.'
         && *c_ptr != '-') {
         return(false);
         }
   }
   return(true);
}
//======================================================
ParmFile::ParmFile( const char* input_file_name )
{
//Input_File = new ifstream(input_file_name, ios::in);
  Input_File_Name = new char[strlen(input_file_name)+2];
  Input_File = NULL;
  strcpy(Input_File_Name, input_file_name);
  Block_Name = new char[50];
}
//======================================================
ParmFile::~ParmFile( void ){ };

//======================================================
void ParmFile::FindBlock(const char* block_nam)
{
  char linebuf[80];
  strcpy(Block_Name, block_nam);
  
   char buffer[_MAX_PATH];

   /* Get the current working directory: */
   if( _getcwd( buffer, _MAX_PATH ) == NULL )
      cout << "_getcwd error" << endl;
   else
      cout << "%s\n" << buffer << endl;

  if(Input_File != NULL) Input_File->close();
  Input_File = new ifstream(Input_File_Name, ios::in);
  //
  // find block
  //
  for(;;)
    {
    Input_File->getline(linebuf,80);
    cout << linebuf << endl;
    if(!strcmp("$EOF",linebuf))
      {
      cout << "Error- block '" << block_nam << "' not found" << endl;
      exit(0);
      }
    if(!strcmp(block_nam, linebuf)) break;
    }
  cout << "matched " << block_nam << endl;
}
//======================================================
void ParmFile::RestartBlock(void)
{
  FindBlock(Block_Name);
}
//======================================================
void ParmFile::GetPlotSpec( char *sig_name,
                            double *start_time,
                            double *stop_time,
                            int *plot_decim_rate,
                            bool *count_vice_time,
                            bool *file_header_req)
{
  char linebuf[80], scanbuf[80];
  char *token;

  Input_File->getline(linebuf,80);
  cout << linebuf << endl;
  strcpy(scanbuf,linebuf);
  token = strtok( scanbuf, " ,\n");
  strcpy(sig_name, token);
  token = strtok( NULL, " ,\n");
  *start_time = atof(token);
  token = strtok( NULL, " ,\n");
  *stop_time = atof(token);
  token = strtok( NULL, " ,\n");
  *plot_decim_rate = atoi(token);

  token = strtok( NULL, " ,\n");
  int temp = atoi(token);
  if(temp == 1)
    *count_vice_time = true;
  else
    *count_vice_time = false;

  token = strtok( NULL, " ,\n");
  temp = atoi(token);
  if(temp == 1)
    *file_header_req = true;
  else
    *file_header_req = false;
  return;
}

//===========================================================
int ParmFile::GetParmStr(const char* parm_nam, char* ret_str)
{
  char linebuf[80], scanbuf[80];
  char *pCpos, *token;
  char *scan_ptr;
  for(;;)
    {
    Input_File->getline(linebuf,80);
    cout << linebuf << endl;
    pCpos = strstr(linebuf,"$");
    if(pCpos != NULL)
      {
      // this is the end of the block and parm has not been found
      cout << "Warning: parameter '" << parm_nam << "' not found in block '" 
            << Block_Name << "'" << endl;
      BasicResults << "Warning: parameter '" << parm_nam << "' not found in block '"
            << Block_Name << "'" << endl;
      return(-1);
      }
    strcpy(scanbuf,linebuf);
    token = strtok( scanbuf, " =\n");
    if(strcmp(token,parm_nam)) continue;
      cout << "matched " << parm_nam << endl;
      scan_ptr = linebuf+strlen(parm_nam);
      strcpy(scanbuf,scan_ptr);
      scan_ptr = strstr(scanbuf, "=\0");
      //token = strtok( scanbuf, "=");
      //token = strtok( scanbuf, " 0123456789-+ft");
      cout << "xfound " << token << endl;
//      if(strcmp(token,"="))
        if(scan_ptr==NULL)
        {
        // error- expected equals sign not found
        cout << "error- expected equals sign not found" << endl;
        exit(0);
        }
        else
        {
      scan_ptr += 1;
//      strcpy(scanbuf, scan_ptr);
      memmove(scanbuf, scan_ptr, strlen(scan_ptr)+1);
      token = strtok( scanbuf," ;\n");
      cout << "token = " << token << endl;
      cout << "breaking" << endl;
      break;
      }
    }
  //cout << "num = " << num << endl;
  cout << "token = " << token << endl;
  strcpy(ret_str, token);
  return(0);
}

//======================================================
int ParmFile::GetParmArrayStr(const char* parm_nam, char* ret_str)
{
  char linebuf[80], scanbuf[80];
  char *pCpos, *token;
  for(;;)
    {
    Input_File->getline(linebuf,80);
    cout << linebuf << endl;
    pCpos = strstr(linebuf,"$");
    if(pCpos != NULL)
      {
      // this is the end of the block and parm has not been found
      cout << "Warning: parameter '" << parm_nam << "' not found" << endl;
      BasicResults << "Warning: parameter '" << parm_nam << "' not found" << endl;
      return(-1);
      }
    strcpy(scanbuf,linebuf);
    token = strtok( scanbuf, " =\n");
    if(strcmp(token,parm_nam)) continue;
      cout << "matched " << parm_nam << endl;
      strcpy(scanbuf,linebuf+strlen(parm_nam));
      token = strtok( scanbuf, " 0123456789-+");
      cout << "xfound " << token << endl;
      if(strcmp(token,"="))
        {
        // error- expected equals sign not found
        cout << "error- expected equals sign not found" << endl;
        exit(0);
        }
      //token = strtok( NULL," ;\n");
      //cout << "token = " << token << endl;
      cout << "breaking" << endl;
      break;
    }
  //cout << "num = " << num << endl;
  //cout << "token = " << token << endl;
  strcpy(ret_str, linebuf+strlen(parm_nam)+3);
  return(0);
}

//======================================================
int ParmFile::GetIntParm(const char* parm_nam)
{
  int num;
  char parm_str[20];

  if(GetParmStr(parm_nam, parm_str)!=0)
    {
    FindBlock(Block_Name);
    if(GetParmStr(parm_nam, parm_str) !=0)
      {
      ErrorStream <<  "Error: parameter '" << parm_nam 
                  << "' not found after 2 attempts" << endl;
      exit(-1);
      }
    }
  cout << "str = " << parm_str << endl;
//  for(c_ptr=parm_str; c_ptr<parm_str+strlen(parm_str);c_ptr++) {
//   if(!isdigit(*c_ptr)) {
//      ErrorStream << "Error: non-numeric data where numeric expected" << endl;
//      exit(-1);
//      }
//   }
   if(!isnumeric(parm_str)) {
      ErrorStream << "Error: non-numeric data where numeric expected" << endl;
      exit(-1);
      }
  num = atoi(parm_str);
  cout << "num = " << num << endl;
  return(num);
}

//======================================================
int* ParmFile::GetIntParmArray( const char* parm_nam, 
                                int* array_ptr, 
                                const int array_len)
{
  int num;
  int idx;
  char *token;
  char parm_str[80];

  if(GetParmArrayStr(parm_nam, parm_str)!=0)
    {
    FindBlock(Block_Name);
    if(GetParmArrayStr(parm_nam, parm_str) !=0)
      {
      ErrorStream <<  "Error: parameter '" << parm_nam 
                  << "' not found after 2 attempts" << endl;
      exit(-1);
      }
    }
  cout << "xxstr = " << parm_str << endl;
  token = strtok(parm_str," ,\n");
   if(!isnumeric(token)) {
      ErrorStream << "Error: non-numeric data where numeric expected" << endl;
      exit(-1);
      }
  num = atoi(token);
  cout << "num = " << num << endl;
  array_ptr[0] = num;
  for(idx=1; idx<array_len; idx++)
    {
    token = strtok( NULL, " ,\n");
   if(!isnumeric(token)) {
      ErrorStream << "Error: non-numeric data where numeric expected" << endl;
      exit(-1);
      }
    array_ptr[idx] = atoi(token);
    }
  return(array_ptr);
}

//======================================================
double* ParmFile::GetDoubleParmArray(  const char* parm_nam, 
                                double* array_ptr, 
                                const int array_len)
{
  double num;
  int idx;
  char *token;
  char parm_str[80];

  if(GetParmArrayStr(parm_nam, parm_str)!=0)
    {
    FindBlock(Block_Name);
    if(GetParmArrayStr(parm_nam, parm_str) !=0)
      {
      ErrorStream <<  "Error: parameter '" << parm_nam 
                  << "' not found after 2 attempts" << endl;
      exit(-1);
      }
    }
  cout << "xxstr = " << parm_str << endl;
  token = strtok(parm_str," ,\n");
   if(!isnumeric(token)) {
      ErrorStream << "Error: non-numeric data where numeric expected [GetDoubleParmArray(1)]" << endl;
      exit(-1);
      }
  num = atof(token);
  cout << "num = " << num << endl;
  array_ptr[0] = num;
  for(idx=1; idx<array_len; idx++)
    {
    token = strtok( NULL, " ,\n");
   if(!isnumeric(token)) {
      ErrorStream << "Error: non-numeric data where numeric expected [GetDoubleParmArray(2)]" << endl;
      exit(-1);
      }
    array_ptr[idx] = atof(token);
    }
  return(array_ptr);
}

//======================================================
char* ParmFile::GetStringParm(const char* parm_nam)
{
  //int num;
  char parm_str[30];
  char *ret_str;

  if(GetParmStr(parm_nam, parm_str)!=0)
    {
    FindBlock(Block_Name);
    if(GetParmStr(parm_nam, parm_str) !=0)
      {
      ErrorStream <<  "Error: parameter '" << parm_nam 
                  << "' not found after 2 attempts" << endl;
      exit(-1);
      }
    }
  cout << "str = " << parm_str << endl;
  ret_str = new char[strlen(parm_str)+20];
  strcpy(ret_str, parm_str);
  return(ret_str);
}

//======================================================
long ParmFile::GetLongParm(const char* parm_nam)
{
  long num;
  char parm_str[20];

  if(GetParmStr(parm_nam, parm_str)!=0)
    {
    FindBlock(Block_Name);
    if(GetParmStr(parm_nam, parm_str) !=0)
      {
      ErrorStream <<  "Error: parameter '" << parm_nam 
                  << "' not found after 2 attempts" << endl;
      exit(-1);
      }
    }
  cout << "str = " << parm_str << endl;
   if(!isnumeric(parm_str)) {
      ErrorStream << "Error: non-numeric data where numeric expected" << endl;
      exit(-1);
      }
  num = atol(parm_str);
  cout << "num = " << num << endl;
  return(num);
}

//======================================================
float ParmFile::GetFloatParm(const char* parm_nam)
{
  float num;
  char parm_str[20];

  if(GetParmStr(parm_nam, parm_str)!=0)
    {
    FindBlock(Block_Name);
    if(GetParmStr(parm_nam, parm_str) !=0)
      {
      ErrorStream <<  "Error: parameter '" << parm_nam 
                  << "' not found after 2 attempts" << endl;
      exit(-1);
      }
    }
  cout << "str = " << parm_str << endl;
   if(!isnumeric(parm_str)) {
      ErrorStream << "Error: non-numeric data where numeric expected" << endl;
      exit(-1);
      }
  num = float(atof(parm_str));
  cout << "num = " << num << endl;
  return(num);
}

//======================================================
double ParmFile::GetDoubleParm(const char* parm_nam)
{
  double num;
  char parm_str[20];

  if(GetParmStr(parm_nam, parm_str)!=0)
    {
    FindBlock(Block_Name);
    if(GetParmStr(parm_nam, parm_str) !=0)
      {
      ErrorStream <<  "Error: parameter '" << parm_nam 
                  << "' not found after 2 attempts" << endl;
      exit(-1);
      }
    }
   if(!isnumeric(parm_str)) {
      ErrorStream << "Error: non-numeric data where numeric expected" << endl;
      exit(-1);
      }
  num = atof(parm_str);
  cout << "num = " << num << endl;
  return(num);
}
//======================================================
bool ParmFile::GetBoolParm(const char* parm_nam)
{
  bool num;
  char parm_str[20];

  if(GetParmStr(parm_nam, parm_str)!=0)
    {
    FindBlock(Block_Name);
    if(GetParmStr(parm_nam, parm_str) !=0)
      {
      ErrorStream <<  "Error: parameter '" << parm_nam 
                  << "' not found after 2 attempts" << endl;
      exit(-1);
      }
    }
  if(!strcmp(parm_str,"true"))
    num = true;
  else
    num = false;
  cout << "num = " << num << endl;
  return(num);
}

