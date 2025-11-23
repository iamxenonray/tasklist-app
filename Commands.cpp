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
			std::cout << "view [flags...]: Shows all tasks stored in the application.\n";
			std::cout << "Specifying the \"--hide-complete\" flag will not show completed tasks.\n";
			std::cout << "Specifying the \"--sort-asc\" flag will sort tasks by ascending order of due date (soonest first).\n";
			std::cout << "Specifying the \"--sort-desc\" flag will sort tasks by ascending order of due date (furthest first).\n";
			std::cout << "The order which you specify flags does not matter\n";
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
void ViewTasks(std::vector<std::string> s)
{
	std::cout << "---------------------\n";
	// Flags
	if (s.size() > 1)
	{
		bool hide_comp = false;
		int sort_order = 0;
		for (int i = 1; i < s.size(); ++i)
		{
			if (s[i] == "--hide-complete")
			{
				hide_comp = true;
			}
			else if (s[i] == "--sort-asc")
			{
				sort_order = 1;
			}
			else if (s[i] == "--sort-desc")
			{
				sort_order = 2;
			}
			else
			{
				std::cout << "Unknown argument " + s[i] + ", ignoring...\n";
			}
		}

		if (hide_comp)
		{
			std::cout << "Viewing incomplete tasks...\n";
		}
		else
		{
			std::cout << "Viewing tasks...\n";
		}

		if (sort_order == 1)
		{
			std::cout << "Sorting by ascending due date...\n";
		}
		else if (sort_order == 2)
		{
			std::cout << "Sorting by descending due date...\n";
		}

		TaskList.PrintTaskList(sort_order, hide_comp);
		
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
		try
		{
			// Construct as a ymd
			int year_val = std::stoi(yyyy);
			unsigned int month_val = std::stoi(mm);
			unsigned int day_val = std::stoi(dd);

			std::chrono::year_month_day due_date{ std::chrono::year(year_val), std::chrono::month(month_val), std::chrono::day(day_val) };
			// Bounds checking, helpfully provided by the chrono library.
			if (due_date.ok())
			{
				TaskList.AddTask(description, due_date);
			}
			else
			{
				std::cout << "Error! Date range is invalid. Please make sure you enter a valid date in the \"DD/MM/YYYY\" or \"DD/MM\" format.\n";
			}
			
		}
		catch (std::exception const& e)
		{
			std::cout << "Error! Date format could not be read. Ensure that you are using the \"DD/MM/YYYY\" format or \"DD/MM\" format, including the / characters.\n";
		}
		
		
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
