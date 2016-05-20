#include <genesis.h>
#include <gin_reader.h>
#include <fstream>
#include <iostream>

namespace reparm{

  Genesis::Genesis(std::shared_ptr<ReparmData> &reparm_data)
    :reparm_data_(reparm_data)
  {
    ReadUserInput();
  }

  void Genesis::ReadUserInput(){
    std::string in_file_name;
    in_file_name = reparm_data_->GetReparmInput().GetInputFile();
    GaussianInput input(in_file_name);
    opt_coord_ = input.GetCoordinates();

    /* We'll try to get the parameters from the user.
       If we can't, we make our own. */
    try {
      init_parameters_ = input.GetParameters();
    } catch(...){
    }
      
      
  };

  void Genesis::CreateHLTOptFreq(){};

  void Genesis::RunHLTOptFreq(){};
  
  void Genesis::FindOptFreq(){};



}
