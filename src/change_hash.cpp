/*\
 *
 * Programa que calcula el script que cortara todos los cambios existentes
 * entre 2 versiones de WARC de una pagina.
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

using namespace std;

map<string, pair<string, int> > OLD_WARC;
map<string, pair<string, int> > NEW_WARC;
map<int, pair<string, string> > NEW_DOCS;
vector<int> OFFSET;
int TAM_MAX; 
int LAST_BYTE;
char *FILE_NAME_NEW;
int NUM_SEGMENT;
ofstream *output, *output_cdx, *output_offsets;

void read_warc_hashcode(map<string, pair<string, int> >* warc_hash, char *fname){
 
  std::ifstream infile(fname); 
 
  string url, hashcode;
  int i = 0;
  while (infile >> url >> hashcode){
  
    (*warc_hash)[url] = make_pair(hashcode, i);
    ++i;
  } 
 

 
}

 

void read_warc_hashcode_offset(map<string, pair<string, int> >* warc_hash, char *fname, char *fname_offset){
 
  std::ifstream infile(fname); 
  std::ifstream infileoff(fname_offset); 
 
  string url, hashcode, offsetstr;
  int offsetcdx, offset;
  int i = 0;
  while (infile >> url >> hashcode >> offsetstr){
   
   offsetcdx = atoi(offsetstr.c_str());   
   

    while (infileoff >> offset){
  
      OFFSET.push_back(offset);
      if(offsetcdx==offset) break;

    
    }   
  

  
    (*warc_hash)[url] = make_pair(hashcode, OFFSET.size()-1);

  } 
 
  while (infileoff >> offset){
  
    OFFSET.push_back(offset);    
  } 

 
}

void create_script( int begin, int end){

  (*output) << "head -c "<<end <<" "<< FILE_NAME_NEW
            << ".warc | tail -c "<<(end-begin) 
            << " > tmp_warc/"<< NUM_SEGMENT++<<".txt\n";

}

void create_cdx_script(string url, string hash){

  (*output_cdx) << "grep \""<< url <<" \" "<< FILE_NAME_NEW 
                << ".cdx | grep " << hash 
                << " >> tmp_warc/new.cdx\n";

}

void create_offset_script(int oset){

  (*output_offsets) << oset <<endl;

}

void read_warc_offsets(char *fname){
 
  std::ifstream infile(fname); 
 
  int offset;
  while (infile >> offset){
  
    OFFSET.push_back(offset);
  } 
 

 
}


int main(int argc, char * argv[]) {
  
  NUM_SEGMENT= 0;
  int last_cut = 0;
  read_warc_hashcode(&OLD_WARC, argv[1]);  
  read_warc_hashcode_offset(&NEW_WARC, argv[2], argv[3]);
  //read_warc_offsets(argv[3]);
  FILE_NAME_NEW = argv[4];
  map<string, pair<string, int> >::iterator itold;
  for(itold = OLD_WARC.begin();itold!=OLD_WARC.end(); ++itold){

    if(NEW_WARC.count(itold->first)>0){
        
      if(NEW_WARC[itold->first].first!=itold->second.first){
      
        NEW_DOCS[NEW_WARC[itold->first].second]= make_pair(itold->first, NEW_WARC[itold->first].first);                    
      }
    
      NEW_WARC.erase(itold->first);
    
    }
    
  }

  map<string, pair<string, int> >::iterator itnew;
  for(itnew = NEW_WARC.begin(); itnew!=NEW_WARC.end(); ++itnew){
   
    NEW_DOCS[NEW_WARC[itnew->first].second]= make_pair(itnew->first, NEW_WARC[itnew->first].first);                    
    
  }    
  
  //if hay cambio
  output = new ofstream("cut_script");
  output_cdx = new ofstream("cut_cdx");  
  output_offsets = new ofstream("tmp_offsets");  
  create_script(OFFSET[0], OFFSET[1]); 
  int offset = OFFSET[1];
  map<int, pair<string, string> >::iterator it;

  for(it = NEW_DOCS.begin(); it!=NEW_DOCS.end(); ++it){
   
    create_script(OFFSET[it->first - 1], OFFSET[it->first + 1]);
    create_cdx_script(it->second.first, it->second.second);    
    create_offset_script(offset);
    offset = offset+(OFFSET[it->first + 1] - OFFSET[it->first - 1]);                  

  }  
 
  output->close();  
  output_cdx->close();    
  output_offsets->close();      
  

}

