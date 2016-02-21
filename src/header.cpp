#include <header.h>
#include <string>
#include <iostream>
#include <regex>

using namespace reparm;

Header::Header(const std::string& header){
  using namespace std;
  try{
    this->command_ = ReadCommand(header);
    this->title_ = ReadTitle(header);
  }
  catch(const char *e){
    throw e;
  }
}

std::string Header::ReadCommand(const std::string &header){
  using namespace std;
  try{
    regex p_command{"^.*"};
    smatch m;
    if (!regex_search(header, m, p_command)){throw "Can't find command in inputfile";}
    return m[0];
  }
  catch(const char *e){
    cerr << e << endl;
    throw e;
  }
}

std::string Header::ReadTitle(const std::string &header){
  using namespace std;
  try{
    regex p_title{"^(.*\n){3}"};
    smatch m;
    if (!regex_search(header, m, p_title)){throw "Can't find command in inputfile";}
    return m[1];
  }
  catch(const char *e){
    cerr << e << endl;
    throw e;
  }
}

std::string Header::GetCommand() const{
  return this->command_;
}

std::string Header::str() const{
  return this->command_ + "\n\n" +  this->title_ + "\n";
}
