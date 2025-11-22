// TaskListApp.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "Task.h"
#include <sstream>

int main()
{
    std::vector<std::string> args;
    bool quit = false;

    std::cout << "Welcome to the Task List Application.\n";

    while (!quit)
    {
        std::cout << "Please enter a command; for help, type \"help\".\n";
        std::string raw_cmd{};
        std::getline(std::cin, raw_cmd);
        std::istringstream words(raw_cmd);

        for (std::string word; std::getline(words, word, ' ');)
        {
            std::cout << word << "\n";
            args.push_back(word);
        }

        for (std::string s : args)
        {
            std::cout << s << " ";
        }
        std::cout << "\n";
        if (raw_cmd == "quit")
        {
            std::cout << "Quitting Application...";
            quit = true;
        }
    }
}
