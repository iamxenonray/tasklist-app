#include "Task.h"
#include "Commands.h"
#include <sstream>


int main()
{
    std::vector<std::string> args;
    CommandMap Commands;

    std::cout << "Welcome to the Task List Application.\n";

    while (true)
    {
        args.clear();
        std::cout << "Please enter a command; for help, type \"help\".\n";

        // Reading input
        std::string raw_cmd{};
        std::getline(std::cin, raw_cmd);
        std::istringstream words(raw_cmd);

        // Processing input
        for (std::string word; std::getline(words, word, ' ');)
        {
            args.push_back(word);
        }

        Commands.RunCommand(args[0], args);
    }
}
