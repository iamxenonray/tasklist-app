#include "Commands.h"



CommandMap::CommandMap()
{
	InitialiseMap();
}

void CommandMap::RunCommand(std::string key, std::vector<std::string> args)
{
	if (!Map.contains(key))
	{
		std::cout << "Unknown command. Type \"help\" to see a list of commands.\n";
		return;
	}
	Map[key](args);
}

// All mappings are to be made here
void CommandMap::InitialiseMap()
{
	Map["quit"] = Quit;
	Map["help"] = PrintHelp;
	Map["view"] = ViewTasks;
	Map["add"] = AddTask;
	Map["complete"] = CompleteTask;
}

void PrintHelp(std::vector<std::string> s)
{
	std::cout << "---------------------\n";

	// Help for a command
	if (s.size() > 1)
	{
		if (s[1] == "quit")
		{
			std::cout << "quit: Quits the application. Stored tasks will be lost!\n";
		}
		else if (s[1] == "view")
		{
			std::cout << "view: Shows all tasks stored in the application.\n";
		}
		else if (s[1] == "add")
		{
			std::cout << "add <duedate> <description>: Adds a task. Due date is specified in DD/MM/YYYY, and description can be whatever you want.\n";
			std::cout << "Due date can also be specified as DD/MM, and the year will default to the current year.\n";
			std::cout << "Tasks are added and marked as INCOMPLETE by default.\n";
		}
		else if (s[1] == "delete")
		{
			std::cout << "delete <index>: Deletes a task. Tasks are identified using their numerical ID shown in the \"view\" command.\n";
			std::cout << "<index> must be a valid ID.\n";
		}
		else if (s[1] == "complete")
		{
			std::cout << "complete <index>: Marks a task as complete. Tasks are identified using their numerical ID shown in the \"view\" command.\n";
			std::cout << "<index> must be a valid ID.\n";
		}
	}
	// Default case
	else
	{
		std::cout << "HELP\n";
		std::cout << "To use the app, type in a command. Some commands take in more than 1 argument.\n";
		std::cout << "For individual command help, type \"help <command>\" to see its usage.\n";
		std::cout << "Available commands:\n";
		std::cout << "help [command]: Views help for a specified command. If no command is specified, brings you here.\n";
		std::cout << "view: Views the list of tasks stored.\n";
		std::cout << "add: Adds a task.\n";
		std::cout << "delete: Removes a task.\n";
		std::cout << "complete: Marks a task as complete.\n";
		std::cout << "quit: Quits the application.\n";
	}
	std::cout << "---------------------\n";
}

void Quit(std::vector<std::string> s)
{
	std::cout << "Quitting Application...\n";
	exit(0);
}

void ViewTasks(std::vector<std::string> s)
{
	std::cout << "---------------------\n";
	std::cout << "Viewing tasks...\n";
	TaskList.PrintTaskList();
	std::cout << "---------------------\n";
}

void AddTask(std::vector<std::string> s)
{
	std::cout << "---------------------\n";
	if (s.size() < 3)
	{
		std::cout << "Invalid usage. 3 arguments required. Usage: \"add <duedate> <description>\"\n";
	}
	else
	{
		// Reconstruct all trailing arguments
		std::string description;
		for (int i = 2; i < s.size(); ++i)
		{
			description += s[i];
			if (i != s.size() - 1)
			{
				description += " ";
			}
		}
		TaskList.AddTask(description, s[1]);
	}
	std::cout << "---------------------\n";
}

void CompleteTask(std::vector<std::string> s)
{
	std::cout << "---------------------\n";
	if (s.size() < 2)
	{
		std::cout << "Invalid usage. 2 arguments required. Usage: \"complete <ID>\"\n";
	}
	else
	{
		try
		{
			int idx = std::stoi(s[1]);
			if (idx < 1 || idx > TaskList.TaskList.size())
			{
				std::cout << "Task ID is invalid! There is no task with that ID!\n";
			}
			else
			{
				TaskList.CompleteTask(idx);
				std::cout << "Task ID " + std::to_string(idx) + " marked as complete!\n";
			}
		}
		catch (std::exception const& e)
		{
			std::cout << "Error! Task ID is not a valid number!\n";
		}
	}
	std::cout << "---------------------\n";
}
