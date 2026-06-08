#include <iostream>
#include <string>

int main() {

  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;
  int i=1;
  while( i!=0){
    std::cout << "$ ";
    std::string command;
    std::getline(std::cin,command);
    std::cout<< command << ": command not found" <<std::endl;
  }
}
