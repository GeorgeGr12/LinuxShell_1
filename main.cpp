#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "commands.hpp"
using namespace std;

// Function to check if input contains a pipe
bool hasPipe(const string& input) {
    return input.find('|') != string::npos;
}

// Function to parse a pipeline command
vector<pair<string, vector<string> > > parsePipeline(const string& input) {
    vector<pair<string, vector<string> > > pipeline;
    istringstream fullStream(input);
    string segment;
    
    // Split by '|'
    while(getline(fullStream, segment, '|')) {
        istringstream segmentStream(segment);
        string word;
        string command = "";
        vector<string> args;
        
        // Parse each segment
        while(segmentStream >> word) {
            if(command == "") {
                command = word;
            } else {
                args.push_back(word);
            }
        }
        
        if(command != "") {
            pipeline.push_back(make_pair(command, args));
        }
    }
    
    return pipeline;
}

int main(int argc, char* argv[]){
    string userInput;
    cout<<"****Welcome to 3320 Shell****"<<endl;
    Commands system_command=Commands();
    
    while(true){    
        cout<<"$ ";        
        getline(cin,userInput);
        
        // Check if the command contains a pipe
        if(hasPipe(userInput)) {
            // Parse and execute pipeline
            vector<pair<string, vector<string> > > pipeline = parsePipeline(userInput);
            system_command.execute_pipeline(pipeline);
        }
        else {
            // Regular command parsing (no pipe)
            istringstream inputStream(userInput);
            string current_string;
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
            else if(userCommand != ""){
                system_command.command_execute(userCommand,userArguments);
            }
        }
    }
    
    return 0;
}