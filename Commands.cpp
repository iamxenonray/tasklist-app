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

/*

All mappings are to be made here.
All implementations must be done in this file below this.

*/
void CommandMap::InitialiseMap()
{
	Map["quit"] = Quit;
	Map["help"] = PrintHelp;
	Map["view"] = ViewTasks;
	Map["add"] = AddTask;
	Map["complete"] = CompleteTask;
	Map["delete"] = DeleteTask;
	Map["uncomplete"] = UncompleteTask;
}

// Help command
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
			std::cout << "view [--hide-complete]: Shows all tasks stored in the application.\n";
			std::cout << "Specifying the \"--hide-complete\" flag will not show completed tasks.\n";
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
		else if (s[1] == "uncomplete")
		{
			std::cout << "uncomplete <index>: Marks a task as incomplete. Tasks are identified using their numerical ID shown in the \"view\" command.\n";
			std::cout << "<index> must be a valid ID.\n";
		}
		else if (s[1] == "help")
		{
			std::cout << "You're already here...\n";
		}
		else
		{
			std::cout << "Unknown command specified.\n";
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
		std::cout << "uncomplete: Marks a task as incomplete.\n";
		std::cout << "quit: Quits the application.\n";
	}
	std::cout << "---------------------\n";
}


// Quit
void Quit(std::vector<std::string> s)
{
	std::cout << "Quitting Application...\n";
	exit(0);
}

// Viewing tasks
// TODO: Sorting by due date, hiding completed tasks
void ViewTasks(std::vector<std::string> s)
{
	std::cout << "---------------------\n";
	// Flags
	if (s.size() > 1)
	{
		if (s[1] == "--hide-complete")
		{
			std::cout << "Viewing incomplete tasks...\n";
			TaskList.PrintTaskList(0, true);
		}
		else
		{
			std::cout << "Unknown additional arguments, viewing default task list.\n";
			std::cout << "Viewing tasks...\n";
			TaskList.PrintTaskList();
		}
	}
	else
	{
		std::cout << "Viewing tasks...\n";
		TaskList.PrintTaskList();
	}
	
	std::cout << "---------------------\n";
}

// Adding a task
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

		// Construct the date from string
		// Format must be DD/MM/YYYY or DD/MM (and YYYY defaults to current year)
		std::istringstream date_stream(s[1]);
		std::string dd, mm, yyyy;
		std::getline(date_stream, dd, '/');
		std::getline(date_stream, mm, '/');
		if (!std::getline(date_stream, yyyy))
		{
			// Get the current year as a string. Due to no straight conversions, is pretty complex.
			// Gets current time, converts to days, passes it into YMD format, then extracts year and casts to int then to string.
			yyyy = std::to_string(static_cast<int>(std::chrono::year_month_day(std::chrono::floor<std::chrono::days>(std::chrono::system_clock::now())).year()));
		}
		// Construct as a ymd
		std::chrono::year_month_day due_date(std::chrono::year(std::stoi(yyyy)), std::chrono::month(std::stoi(mm)), std::chrono::day(std::stoi(dd)));
		TaskList.AddTask(description, due_date);
	}
	std::cout << "---------------------\n";
}

// Marking a task as complete
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

// Deleting a task
void DeleteTask(std::vector<std::string> s)
{
	std::cout << "---------------------\n";
	if (s.size() < 2)
	{
		std::cout << "Invalid usage. 2 arguments required. Usage: \"delete <ID>\"\n";
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
				TaskList.DeleteTask(idx);
				std::cout << "Task ID " + std::to_string(idx) + " deleted! Please note that other Task IDs may have changed!\n";
			}
		}
		catch (std::exception const& e)
		{
			std::cout << "Error! Task ID is not a valid number!\n";
		}
	}
	std::cout << "---------------------\n";
}

// Marking a task as incomplete
void UncompleteTask(std::vector<std::string> s)
{
	std::cout << "---------------------\n";
	if (s.size() < 2)
	{
		std::cout << "Invalid usage. 2 arguments required. Usage: \"uncomplete <ID>\"\n";
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
				TaskList.UncompleteTask(idx);
				std::cout << "Task ID " + std::to_string(idx) + " marked as incomplete!\n";
			}
		}
		catch (std::exception const& e)
		{
			std::cout << "Error! Task ID is not a valid number!\n";
		}
	}
	std::cout << "---------------------\n";
}
