#include "commands.hpp"
#include <limits.h> // For PATH_MAX
#include <cstring>  // For strerror
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
 * Search for the executable file in the stored paths. If found, forks a child process to execute the command.
 * 
 * @param userCommand The command to execute.
 * @param userArguments Arguments for the command.
*/
void Commands::command_execute(string userCommand, vector<string> userArguments){
    if(filePaths.size() == 0) { 
        cout << "Error: No Existing Files in the Path" << endl;
        return;
    }
    else if(userCommand.length() == 0) {
        cout << "Error: No User Command" << endl;
        return;
    }
    
    bool found = false;
    string fullPath;
    for(size_t i = 0; i < filePaths.size(); i++) {
        string testPath = filePaths[i] + "/" + userCommand;
        ifstream file(testPath.c_str()); 
        if(file.good()) {
            found = true;
            fullPath = testPath;
            file.close();
            break;
        }
    }
    
    if(!found) {
        cout << "Error: Command '" << userCommand << "' not found in path" << endl;
        return;
    }
    
    fflush(stdout);
    int rc = fork();
    if(rc < 0) { 
        perror("Fork Failed");
        return;
    }
    else if (rc == 0) {
        // Child process
        // Build argument array for execvp - supports unlimited arguments
        char** args = new char*[userArguments.size() + 2];
        args[0] = strdup(userCommand.c_str());
        for(size_t i = 0; i < userArguments.size(); i++) {
            args[i + 1] = strdup(userArguments[i].c_str());
        }
        args[userArguments.size() + 1] = NULL;
        
        execvp(args[0], args);
        // If execvp returns, it failed
        perror("execvp failed");
        exit(1);
    }
    else {
        // Parent process
        wait(NULL);
    }
}

/**
 * @brief Executes a pipeline of commands.
 * 
 * Takes a vector of commands (each with their arguments) and pipes them together.
 * Example: ls | grep txt | wc -l
 * 
 * @param commands Vector of pairs where each pair contains (command_name, vector_of_arguments)
 */
void Commands::execute_pipeline(vector<pair<string, vector<string> > > commands){
    if(commands.size() == 0) {
        cout << "Error: No commands in pipeline" << endl;
        return;
    }
    
    if(filePaths.size() == 0) {
        cout << "Error: No Existing Files in the Path" << endl;
        return;
    }
    
    int numCommands = commands.size();
    int pipes[numCommands - 1][2]; // Create pipes between commands
    
    // Create all pipes
    for(int i = 0; i < numCommands - 1; i++) {
        if(pipe(pipes[i]) < 0) {
            perror("Pipe creation failed");
            return;
        }
    }
    
    for(int i = 0; i < numCommands; i++) {
        string cmd = commands[i].first;
        vector<string> args = commands[i].second;
        
        bool found = false;
        for(size_t j = 0; j < filePaths.size(); j++) {
            string testPath = filePaths[j] + "/" + cmd;
            ifstream file(testPath.c_str()); 
            if(file.good()) {
                found = true;
                file.close();
                break;
            }
        }
        
        if(!found) {
            cout << "Error: Command '" << cmd << "' not found in path" << endl;
            // Close all pipes
            for(int j = 0; j < numCommands - 1; j++) {
                close(pipes[j][0]);
                close(pipes[j][1]);
            }
            return;
        }
        
        pid_t pid = fork();
        
        if(pid < 0) {
            perror("Fork failed");
            return;
        }
        else if(pid == 0) {
            // Child process
            
            if(i > 0) {
                dup2(pipes[i-1][0], STDIN_FILENO);
            }
            
            if(i < numCommands - 1) {
                dup2(pipes[i][1], STDOUT_FILENO);
            }
            
            for(int j = 0; j < numCommands - 1; j++) {
                close(pipes[j][0]);
                close(pipes[j][1]);
            }
            
            char** execArgs = new char*[args.size() + 2];
            execArgs[0] = strdup(cmd.c_str());
            for(size_t j = 0; j < args.size(); j++) {
                execArgs[j + 1] = strdup(args[j].c_str());
            }
            execArgs[args.size() + 1] = NULL;
            
            execvp(execArgs[0], execArgs);
            
            perror("execvp failed");
            exit(1);
        }
    }
    
    for(int i = 0; i < numCommands - 1; i++) {
        close(pipes[i][0]);
        close(pipes[i][1]);
    }
    
    for(int i = 0; i < numCommands; i++) {
        wait(NULL);
    }
}

/**
 * @brief Changes the current working directory.
 * 
 * This function takes a single directory as an argument and changes the working directory to it.
 * 
 * @param userArguments Vector containing a single directory name.
 */
void Commands::system_cd(std::vector<std::string> userArguments){
    if (userArguments.size() > 1) { 
        cout << "Error: Passed More Than One Argument" << endl;
        return;
    }
    else if (userArguments.size() == 0) { 
        cout << "Error: Passed Less Than One Argument" << endl;
        return;
    }
    
    string arg = userArguments[0];
    const char* argc = arg.c_str();
    int result = chdir(argc);
    
    if(result != 0) {
        cout << "Error: " << strerror(errno) << endl;
    }
    else {
        char cwd[PATH_MAX];
        if (getcwd(cwd, sizeof(cwd)) != NULL) {  // Changed nullptr to NULL
            cout << "Current working directory: " << cwd << endl;
        }
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