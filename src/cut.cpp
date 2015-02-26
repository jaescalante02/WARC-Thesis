/*\
 *
 * Programa que escribe el script a ejecutar para picar un WARC de la
 * manera deseada.
 *
 * Autores:
 *
 * Samantha Campisi (samanthac07@gmail.com)
 * Juan A. Escalante (jaescalante02@gmail.com)
\*/

#include<iostream>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <fstream>
#include <vector>

#define HEADER "#! /bin/bash"

using namespace std;

vector<long long int> WARC;
vector<long long int> CUT_POINT;
long long int TAM_MAX; 
long long int LAST_BYTE;
long long int LIMIT_BYTE;
char *FILE_NAME;
long long int NUM_SEGMENT;
ofstream *output;


void read_warc_offsets(char *fname){
 
  std::ifstream infile(fname); 
 
  long long int offset;
  while (infile >> offset){
  
    WARC.push_back(offset);
  } 
 

 
}

void verify_tam_max(){

  long long int max_diff = 0ll;
  for(long long int j=1ll; j<CUT_POINT.size(); ++j){
  
    if(CUT_POINT[j]-CUT_POINT[j-1]>max_diff) 
        max_diff= CUT_POINT[j]-CUT_POINT[j-1]; 
  
  
  } 

  if(TAM_MAX<max_diff){
    cout << "The minimum size is "<< max_diff <<" bytes\n";
    exit(1);
  }

}

void take_cut_point(){

    CUT_POINT.push_back(WARC[0]);
    for(long long int j=1ll; j<WARC.size(); j=j+2ll) CUT_POINT.push_back(WARC[j]);
    CUT_POINT.push_back(LAST_BYTE);
    
    verify_tam_max();
}


void create_script( long long int begin, long long int end){

  NUM_SEGMENT++;
  cout << "end "<<end<<"<LIMIT "<<LIMIT_BYTE<<endl;
  
  if((long long int)end < LIMIT_BYTE) return;

  (*output) << "head -c "<<end <<" "<<FILE_NAME
            << " | tail -c "<<((long long int) end - (long long int) begin) 
            << " > tmp_warc/"<< NUM_SEGMENT<<".txt\n";

    //dd skip=3715 count=4852 if=juan.warc of=t1.txt bs=1
}


int main(int argc, char * argv[]) {

  TAM_MAX = atoll(argv[3]); 
  LAST_BYTE = atoll(argv[4]);
  LIMIT_BYTE = atoll(argv[5]);  
  
  NUM_SEGMENT=-1ll;
  long long int last_cut = 0ll;
  FILE_NAME=argv[1];
  read_warc_offsets(argv[2]);
  take_cut_point();
  //exit(0);

  output = new ofstream("cut_script");
  (*output) << HEADER << endl;
  
  long long int i;  
  for(i = 0ll;i<CUT_POINT.size()-1ll;++i){

    if(CUT_POINT[i+1]-last_cut>TAM_MAX){
    
      create_script(last_cut, CUT_POINT[i]);
      last_cut = CUT_POINT[i];
    
    }
  
  }
  
  if(CUT_POINT[CUT_POINT.size()-1]-last_cut>TAM_MAX){
  
    create_script(last_cut, CUT_POINT[CUT_POINT.size()-2]);
    create_script(CUT_POINT[CUT_POINT.size()-2], CUT_POINT[CUT_POINT.size()-1]); 
  } else {
  
    create_script(last_cut, CUT_POINT[CUT_POINT.size()-1]); 
  
  }

  output->close();  

}

