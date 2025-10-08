#include <cstdlib>
#include <iostream>
#include<fstream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <vector>
class Commands{

    private:
        std::vector <std::string> filePaths;

    public:
        std::string value;
        Commands();
        void system_cd(std::vector <std::string> userArguments);
        void system_path(std::vector<std::string> userArguments);
        void command_execute(std::string userCommand, std::vector<std::string> userArguments);
        void system_exit();
        void system_help();

};