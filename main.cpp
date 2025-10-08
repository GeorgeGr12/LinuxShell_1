#include <iostream>
#include <sstream>
#include <string>

#include "commands.hpp"
using namespace std;

int main(int argc, char* argv[]){
    string userInput;
    cout<<"****Welcome to 3320 Shell****"<<endl;
    Commands system_command=Commands();
    while(true){    
        cout<<"$";        
        getline(cin,userInput);
        std::istringstream inputStream(userInput);
        std::string current_string;
        string userCommand="";
        vector<string> userArguments;
        while (inputStream>>current_string){
            if (userCommand==""){
                userCommand=current_string;
            }
            else{
                userArguments.push_back(current_string);
            }

        }
        if (userCommand=="path"){
            system_command.system_path(userArguments);
        }
        else if(userCommand=="cd"){
            system_command.system_cd(userArguments);
        }
        else if(userCommand=="help"){
            system_command.system_help();
        }
        else if(userCommand=="exit"){
            system_command.system_exit();
        }
        else{
            system_command.command_execute(userCommand,userArguments);
        }

    }


}