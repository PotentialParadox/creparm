#ifndef REPARM_EXCETPIONS_H
#define REPARM_EXCETPIONS_H

#include <exception>

namespace reparm{

class inputerror : public std::exception{
  public:
    virtual const char* what() const throw(){
      return "There was an error with your input file\n";
    }
};

class gaussian_error : public std::exception{
  public:
    virtual const char* what() const throw(){
      return "There was an error with gaussian\n";
    }
};

class gaussian_critical_error : public std::exception{
  public:
    virtual const char* what() const throw(){
      return "Critical error with gaussian\n";
    }
};

class gaussian_input_error : public std::exception{
  public:
    virtual const char* what() const throw(){
      return "There was an error with gaussian\n";
    }
};

}

#endif /* REPARM_EXCEPTIONS_H */
