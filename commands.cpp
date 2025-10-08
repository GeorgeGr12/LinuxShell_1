#include "commands.hpp"
#include <limits.h> // For PATH_MAX
using namespace std;

/**
 * @brief Constructor for the Commands class.
 * 
 * This constructor initializes an instance of the Commands class.
 */
Commands::Commands(){}

/**
 * @brief Manages the system's execution path.
 * 
 * If arguments are provided, add them to the list of executable paths.
 * If no arguments are provided, print the current list of paths.
 * 
 * @param userArguments Vector containing the paths to add or an empty vector to print current paths.
 */
void Commands::system_path(vector<string> userArguments){
    if(userArguments.size() == 0) {
        if(filePaths.size() == 0) {
            cout << "No Existing Files in the Path" << endl;
        }
        else {
            for(size_t i = 0; i < filePaths.size() ; i++) {
                cout << i << ". " << filePaths[i] << endl;
            }
        }
        
    }
    else {
        for(size_t i = 0; i < userArguments.size() ; i++) {
            string arg = userArguments[i];
            filePaths.push_back(arg);
            cout << arg << " added to filePaths" << endl;           
        }
    }
}

/** 
 * @brief Executes a user command if present in the list of paths we know.
 * 
 * Search for the executable file in the stored paths. If found,forks a child process to execute the command.
 * 
 * @param userCommand The command to execute.
 * @param userArguments Arguments for the command.
*/
void Commands::command_execute(string userCommand,vector<string> userArguments){
    if(filePaths.size() == 0) { cout << "Error: No Existing Files in the Path" << endl;}
    else if(userCommand.length() == 0) {cout << "Error: No User Command" << endl;}
    else if(userArguments.size() > 1) {cout << "Error: More Than One User Argument" << endl;}
    else{
        for(size_t i = 0 ; i < filePaths.size() ; i++) {
            string files = filePaths[i] + "/" + userCommand;
            ifstream file;
            file.open(files);
            if(!file) {
                cout << "Error: File Does Not Exist" << endl;
                return;
            }
        }
        
        
        //cout << "file exists" << endl;
        fflush(stdout);
        int rc = fork();
        if(rc < 0) { perror("Fork Failed"); }
        else if (rc == 0) {
            //cout << "child: " << rc << endl;
                
            if (userArguments.size() == 0) {
                char *arg_no_param[] = { (char*) userCommand.c_str(), NULL };
                execvp(arg_no_param[0], arg_no_param);
            }
            else{
                char *myargs[3];
                myargs[0] = (char*) userCommand.c_str();
                myargs[1] = (char*) userArguments[0].c_str();
                myargs[2] = NULL;
                
                    
                execvp(myargs[0], myargs);
            }


        }
            else{
                //cout << rc << endl;
                wait(NULL);
            }
                
        
    }

}
    

/**
 * @brief Changes the current working directory.
 * 
 * This function takes a single directory as an argument and changes the working directory to it.
 * 
 * @param userArguments Vector containing a single directory name.
 */
void Commands::system_cd(std::vector <std::string> userArguments){
    if (userArguments.size() > 1) { cout << "Error: Passed More Than One Argument" << endl;
        cout << "hello" << endl;}
    else if (userArguments.size() == 0) { cout << "Error: Passed Less Than One Argument" << endl;}
    else { 
        string arg = userArguments[0];
        const char* argc = arg.c_str();
        int result = chdir(argc);
        //cout << result << endl;
        if(result != 0) {cout << "Error: " << errno << " " << stderr << endl;}
        
        // -------
        char cwd[PATH_MAX];
        if (getcwd(cwd, sizeof(cwd)) != nullptr) {
            std::cout << "Current working directory: " << cwd << std::endl;
        }
        // -------
    }
    
    

}

/**
 * @brief Exits the shell.
 * 
 * This function terminates the program.
 */
void Commands::system_exit(){

    cout << "Exiting Shell" << endl;
    exit(0);

}

/**
 * @brief Displays help information.
 * 
 * This function should provide details about the available commands.
 */

void Commands::system_help(){
      cout << "Available Commands: " << endl;
      cout << "path <directories>   - Set or display execution paths" << endl;
      cout << "cd <directory>       - Change working directory" << endl;
      cout << "help                 - Shows this help menu" << endl;
      cout << "exit                 - Exits the shell" << endl;
      cout << "<command> [args]     - Executes a command if it is found in path" << endl;
}