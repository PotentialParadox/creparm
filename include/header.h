/* This header class is used to manage the first three lines
 * in a gaussian input.
 * Created by Dustin Tracy (dtracy.uf@gmail.com) 2016
 */

#ifndef HEADER_H
#define HEADER_H

#include <string>

namespace reparm{

class Header{
  private:
    std::string command_;
    std::string title_;

  public:
    Header()
      : command_{"None"}
      , title_{"None"}
      {}

    Header(const std::string&);
    Header(const reparm::Header&);
    void SetCommand(const std::string&);
    void SetTitle(const std::string&);
    std::string ReadCommand(const std::string&);
    std::string ReadTitle(const std::string&);
    std::string GetCommand() const;
    std::string GetTitle() const;

    std::string str() const;
};

}

#endif /* HEADER_H */
