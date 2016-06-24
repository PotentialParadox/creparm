//
// Created by Dustin Tracy on 6/24/16.
//
#include <string>
#include <iostream>
#include <regex>
#include "../include/header.h"

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

Header::Header(const reparm::Header& rhs)
        : command_(rhs.command_)
        , title_(rhs.title_)
{}


std::string Header::ReadCommand(const std::string &header){
    using namespace std;
    try{
        regex p_command{"^(.+\n)+"};
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
        regex p_title{"^(.+\n)+\n(.+\n)"};
        smatch m;
        if (!regex_search(header, m, p_title)){throw "Can't find title in inputfile";}
        return m[2];
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
    return this->command_ + "\n" +  this->title_ + "\n";
}
