#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <cstdlib>
#include <unistd.h>

bool isBuiltin(const std::string& cmd){
  std::vector<std::string> builtins ={"echo","exit","type","pwd","cd"};
  for(const auto&b : builtins){
    if(cmd == b) return true;
  }
  return false;
}

std::string findInPath(const std::string& cmd){
  const char* pathEnv = std::getenv("PATH");
  if(!pathEnv) return "";

  std::string pathStr(pathEnv);
  std::stringstream ss(pathStr);
  std::string dir;

  while(std::getline(ss,dir,':')){
    std::string fullPath =dir + "/"+cmd;

    std::ifstream file(fullPath);
    if(file.good()){
      if(access(fullPath.c_str(), X_OK)==0){
        return fullPath;
      }
    }
  }
  return "";
}

void handleType(const std::string& cmd){
  if(isBuiltin(cmd)){
    std::cout <<cmd<< "is a shell builtin\n";
  }else{
    std::string path=findInPath(cmd);
    if(!path.empty()){
      std::cout << cmd <<" is " << path<<"\n";
    }else{
      std::cout <<cmd<< ": not found\n";
    }
  }
}


int main() {

  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;
  while(true){
    std::cout << "$ ";
    std::string input;
    std::getline(std::cin,input);

    std::vector<std::string> args;
    std::stringstream ss(input);
    std::string word;
    while(ss>>word){
      args.push_back(word);
    }
    if(args.empty())continue;
    std::string command = args[0];

    if(command=="exit"){ 
      break;
    }
    else if(command =="echo"){
       for(int i = 1; i < args.size(); i++){
        std::cout << args[i];
        if(i < args.size() - 1) std::cout << " "; 
      }
      std::cout << "\n";
    }
    else if(command=="type"){
      if (args.size() > 1) {
        handleType(args[1]);  
      }
    }
    else{
      std::cout<< command << ": command not found" <<std::endl;
    }
  }
}
