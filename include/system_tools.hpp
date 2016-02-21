//
// Created by Justin Smith and Dustin Tracy on 7/5/15.
//
#ifndef SYSTEM_TOOLS_HPP
#define SYSTEM_TOOLS_HPP

#include <string>
#include <iostream>

/*   The System Tools Namespace   */
namespace systls {

/*----------------------------------------
    Execute Command on the System
    Function opens a pipe and runs the
    command while storing any cout data
    in a buffer. This buffer is then
    returned.
------------------------------------------*/
inline std::string exec(const std::string &cmd,size_t maxbuf) throw(const char*) {

  try{
    // Open a pipe and run command
    FILE* pipe = popen(cmd.c_str(), "r");

    // Throw an exception if pipe not open
    if (!pipe) throw("Unable to open pipe.");

    // Extract buffer data and return it.
    char buffer[maxbuf];
    std::string result = "";
    while(!feof(pipe)) {
        if(fgets(buffer, maxbuf, pipe) != NULL)
            result += buffer;
    }
    pclose(pipe);
    return result;
  } catch (const char* e){
    std::cerr << "Error with pipe" << std::endl;
    return 0;
  }
};

};

#endif
