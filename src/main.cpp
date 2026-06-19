#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <cstdlib>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

bool isBuiltin(const std::string& cmd)
{
  std::vector<std::string> builtins ={"echo","exit","type","pwd","cd"};
  for(const auto&b : builtins)
  {
    if(cmd == b) return true;
  }
  return false;
}

std::string findInPath(const std::string& cmd)
{
  const char* pathEnv = std::getenv("PATH");
  if(!pathEnv) return "";

  std::string pathStr(pathEnv);
  std::stringstream ss(pathStr);
  std::string dir;

  while(std::getline(ss,dir,':'))
  {
    std::string fullPath =dir + "/"+cmd;

    std::ifstream file(fullPath);
    if(file.good()){
      if(access(fullPath.c_str(), X_OK)==0)
      {
        return fullPath;
      }
    }
  }
  return "";
}

void handleType(const std::string& cmd)
{
  if(isBuiltin(cmd))
  {
    std::cout <<cmd<< " is a shell builtin\n";
  }
  else
  {
    std::string path=findInPath(cmd);
    if(!path.empty())
    {
      std::cout << cmd <<" is " << path<<"\n";
    }
    else
    {
      std::cout <<cmd<< ": not found\n";
    }
  }
}

void executeExternal(const std::string& executable,
                     const std::vector<std::string>& args)
{
    pid_t pid = fork();

    if (pid == 0) {
        // Child process

        std::vector<char*> argv;

        for (const auto& arg : args) 
        {
            argv.push_back(const_cast<char*>(arg.c_str()));
        }

        argv.push_back(nullptr);

        execvp(executable.c_str(), argv.data());

        // Only reaches here if execvp fails
        perror("execvp");
        exit(1);
    }
    else if (pid > 0) 
    {
        // Parent process waits
        waitpid(pid, nullptr, 0);
    }
    else 
    {
        perror("fork");
    }
}


int main() 
{
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;
  while(true)
  {
    std::cout << "$ ";
    std::string input;
    std::getline(std::cin,input);

    std::vector<std::string> args;
    std::string current;
    bool inSingleQuote = false;

    for(char c : input)
    {
        if(c == '\'')
        {
          inSingleQuote = !inSingleQuote;
        }
        else if(c == ' ' && !inSingleQuote)
        {
          if(!current.empty())
          {
            args.push_back(current);
            current.clear();
          }
        }
        else
        {
          current += c;
        }
    }

    if(!current.empty())
    {
      args.push_back(current);
    }
    
    if(args.empty())continue;
    std::string command = args[0];

    if(command=="exit")
    { 
      break;
    }

    else if(command =="echo")
    {
      for(int i = 1; i < args.size(); i++)
      {
        std::cout << args[i];
        if(i < args.size() - 1) std::cout << " "; 
      }
      std::cout << "\n";
      
    }

    else if(command=="type")
    {
      if (args.size() > 1) 
      {
        handleType(args[1]);  
      }
    }

    else if(command == "pwd")
    {
      char cwd[1024];

      if(getcwd(cwd, sizeof(cwd)) != nullptr)
      {
        std::cout << cwd << "\n";
      }
    }

    else if(command == "cd")
    {
      if(args.size() < 2)
      {
        continue;
      }
      int result;

      if(args[1] == "~")
      {
        result = chdir(getenv("HOME"));
      }
      else
      {
        result = chdir(args[1].c_str());
      }

      if(result != 0)
      {
        std::cout << "cd: " << args[1] << ": No such file or directory\n";
      }
    }
  
    else 
    {
    std::string executable = findInPath(command);

      if (!executable.empty()) 
      {
        executeExternal(executable, args);
      }
      else 
      {
        std::cout << command<< ": command not found"<< std::endl;
      }
    }
  }
}
