#include <iostream>
#include <string>

int main() {

  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;
  while(true){
    std::cout << "$ ";
    std::string command;
    std::getline(std::cin,command);
    if(command=="exit") return false;
    std::cout<< command << ": command not found" <<std::endl;
    
  }
}
