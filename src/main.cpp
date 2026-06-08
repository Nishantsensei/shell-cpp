#include <iostream>
#include <string>

int main() {

  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;
  while(true){
    std::cout << "$ ";
    std::string command;
    std::getline(std::cin,command);
    if(command=="exit"){ 
      break;
    }
    else if(command.substr(0,5) =="echo "){
      std::cout<<command.substr(5)<<'\n';
    }
    else if(command.substr(0,5)=="type "){
      if(command.substr(5)=="echo" || command.substr(5)=="exit" || command.substr(5)=="type" ){
        std::cout<<command.substr(5)<<" is a shell builtin"<< std::endl;
      }else{
        std::cout<< command.substr(5)<< ":  not found" <<std::endl;
      }
    }
    else{
      std::cout<< command << ": command not found" <<std::endl;
    }
  }
}
