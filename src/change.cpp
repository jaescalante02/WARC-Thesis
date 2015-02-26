/*\
 *
 * Programa que la primera diferencia entre 2 WARCs.
 *
 * Autores:
 *
 * Samantha Campisi (samanthac07@gmail.com)
 * Juan A. Escalante (jaescalante02@gmail.com)
\*/

#include <iostream>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <fstream>
#include <vector>
#include <map>

#define HEADER "#! /bin/bash"

using namespace std;

vector< pair<string, string> > OLD_WARC;
vector< pair<string, string> > NEW_WARC;
vector<int> OFFSET;
int TAM_MAX; 
int LAST_BYTE;
char *FILE_NAME;
int NUM_SEGMENT;
ofstream *output;

void read_warc_hashcode(vector<pair<string, string> >* vector_hash, char *fname){
 
  std::ifstream infile(fname); 
 
  string url, hashcode;
  while (infile >> url >> hashcode){
  
    vector_hash->push_back(make_pair(url, hashcode));
    //cout<< url<< endl << hashcode << endl << endl;
  } 
 

 
}


void read_warc_offsets(char *fname){
 
  std::ifstream infile(fname); 
 
  int offset;
  while (infile >> offset){
  
    OFFSET.push_back(offset);
  } 
 

 
}


int main(int argc, char * argv[]) {


  
  NUM_SEGMENT=0;
  int last_cut = 0;
  read_warc_hashcode(&NEW_WARC, argv[1]);  
  read_warc_hashcode(&OLD_WARC, argv[2]);
  
  int i;
  for(i=0;i<NEW_WARC.size(); ++i){
  
    if(NEW_WARC[i]!=OLD_WARC[i]){ 
      
      cout << "new: "<< NEW_WARC[i].first <<" "<< NEW_WARC[i].second << endl; 
      cout << "old: "<< OLD_WARC[i].first <<" "<< OLD_WARC[i].second << endl;
      break;
    }
    
  }
  
  
  cout << i << endl;
  read_warc_offsets(argv[3]);
  
  output = new ofstream("first_change.txt");
  (*output) << OFFSET[2*i-1];
   
  output->close();  

}

