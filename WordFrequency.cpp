/***************************************************************************************
* Akhror Rakhmatov,1794798 arakhmat@ucsc.edu, PA8
*  WordFrequency.cpp
*  Uses functions in Dictionary.cpp  //
****************************************************************************************/
#include<iostream>
#include<algorithm>
#include<fstream>
#include<string>
#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<string.h>
#include<vector>
#include"Dictionary.h"

using namespace std;
string to_lower(string s) {        
    for(char &c : s)
        c = tolower(c);
    return s;
}

int main(int argc, char **argv) {
    if (argc != 3) {
        fprintf( stderr, "Incorrect number of command line arguments, please include a file in and file out only\n" );
        exit(1);
    }
    std::ifstream in;
    std::ofstream out;
    in.open(argv[1]);
    if( !in.is_open() ){
      std::cerr << "Unable to open file " << argv[1] << " for reading" << std::endl;
      return(EXIT_FAILURE);
    }
    out.open(argv[2]);
    if( !out.is_open() ){
      std::cerr << "Unable to open file " << argv[2] << " for writing" << std::endl;
      return(EXIT_FAILURE);
    }
    Dictionary A;
    std::string key;
    std::string delim = " \t\\\"\',<.>/?;:[{]}|`~!@#$^&*()-_=+0123456789";
    std::string tokenBuffer;
    std::string line;
    size_t begin, end, len;
    while(std::getline(in, line) )  {
        len = line.length();
        tokenBuffer = "";
        begin = std::min(line.find_first_not_of(delim, 0), len);
        end = std::min(line.find_first_of(delim, begin), len);
        key = line.substr(begin, end-begin);
        while( key!="" ){  // we have a token
         // update token buffer
            tokenBuffer += "   "+key+"\n";
            key = to_lower(key);
            if (!A.contains(key)) {
                A.setValue(key,1);
            }
            else {
                A.getValue(key) += 1;
            }
            begin = min(line.find_first_not_of(delim, end+1), len);
            end   = min(line.find_first_of(delim, begin), len);
            key = line.substr(begin, end-begin);
        }
    }
    out << A << end;
    in.close();
    out.close();
    return(EXIT_SUCCESS);

}