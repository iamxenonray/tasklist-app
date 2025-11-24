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
	Map["debug"] = DebugTest;
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
			std::cout << "Specifying the \"--sort-desc\" flag will sort tasks by descending order of due date (furthest first).\n";
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

/*

Debug code below
Warning: Long!

*/

void DebugTest(std::vector<std::string> s)
{
	std::cout << "---------------------\n";
	std::cout << "RUNNING DEBUG TEST:\n";

	std::cout << "Preparing environment... clearing task list...\n";
	TaskList.TaskList.clear();
	std::vector<std::string> command_list;

	std::cout << "---------------------\n";
	std::cout << "STAGE 1: CORRECT USAGE TEST\n";
	std::cout << "---------------------\n";
	std::cout << "Test 1: help\n";
	std::cout << "Expected output: General help menu displayed\n";
	std::cout << "Running command help\n";
	command_list.clear();
	command_list = { "help" };
	PrintHelp(command_list);

	std::cout << "---------------------\n";
	std::cout << "Test 2: help view\n";
	std::cout << "Expected output: Help menu for view displayed\n";
	std::cout << "Running command help view\n";
	command_list.clear();
	command_list = { "help", "view" };
	PrintHelp(command_list);

	std::cout << "---------------------\n";
	std::cout << "Test 3: help add\n";
	std::cout << "Expected output: Help menu for add displayed\n";
	std::cout << "Running command help add\n";
	command_list.clear();
	command_list = { "help", "add" };
	PrintHelp(command_list);

	std::cout << "---------------------\n";
	std::cout << "Test 4: help delete\n";
	std::cout << "Expected output: Help menu for delete displayed\n";
	std::cout << "Running command help delete\n";
	command_list.clear();
	command_list = { "help", "delete" };
	PrintHelp(command_list);

	std::cout << "---------------------\n";
	std::cout << "Test 5: help complete\n";
	std::cout << "Expected output: Help menu for complete displayed\n";
	std::cout << "Running command help complete\n";
	command_list.clear();
	command_list = { "help", "complete" };
	PrintHelp(command_list);

	std::cout << "---------------------\n";
	std::cout << "Test 6: help uncomplete\n";
	std::cout << "Expected output: Help menu for uncomplete displayed\n";
	std::cout << "Running command help uncomplete\n";
	command_list.clear();
	command_list = { "help", "uncomplete" };
	PrintHelp(command_list);

	std::cout << "---------------------\n";
	std::cout << "Test 7: help quit\n";
	std::cout << "Expected output: Help menu for quit displayed\n";
	std::cout << "Running command help quit\n";
	command_list.clear();
	command_list = { "help", "quit" };
	PrintHelp(command_list);

	std::cout << "---------------------\n";
	std::cout << "Test 8: view empty list\n";
	std::cout << "Expected output: Show no tasks message\n";
	std::cout << "Running command view\n";
	command_list.clear();
	command_list = { "view" };
	ViewTasks(command_list);

	std::cout << "---------------------\n";
	std::cout << "Test 9: add task (DD/MM/YYYY, 1 word desc)\n";
	std::cout << "Expected output: Task added (add 25/12/2025 Testing)\n";
	std::cout << "Running command add 25/12/2025 Testing\n";
	command_list.clear();
	command_list = { "add", "25/12/2025", "Testing" };
	AddTask(command_list);
	std::cout << "Running command view\n";
	command_list.clear();
	command_list = { "view" };
	ViewTasks(command_list);

	std::cout << "---------------------\n";
	std::cout << "Test 10: add task (DD/MM, 1 word desc)\n";
	std::cout << "Expected output: Task added with date of current year (add 24/12 Testing2)\n";
	std::cout << "Running command add 24/12 Testing2\n";
	command_list.clear();
	command_list = { "add", "24/12", "Testing2" };
	AddTask(command_list);
	std::cout << "Running command view\n";
	command_list.clear();
	command_list = { "view" };
	ViewTasks(command_list);

	std::cout << "---------------------\n";
	std::cout << "Test 11: add task (DD/MM/YYYY, multi word desc)\n";
	std::cout << "Expected output: Task added (add 1/1/2026 Testing long@ words123)\n";
	std::cout << "Running command add 1/1/2026 Testing long@ words123\n";
	command_list.clear();
	command_list = { "add", "1/1/2026", "Testing", "long@", "words123" };
	AddTask(command_list);
	std::cout << "Running command view\n";
	command_list.clear();
	command_list = { "view" };
	ViewTasks(command_list);

	std::cout << "---------------------\n";
	std::cout << "Test 12: add task (DD/MM, multi word desc)\n";
	std::cout << "Expected output: Task added with date of current year (add 30/03 Multiple word sentence)\n";
	std::cout << "Running command add 30/03 Multiple word sentence\n";
	command_list.clear();
	command_list = { "add", "30/03", "Multiple", "word", "sentence" };
	AddTask(command_list);
	std::cout << "Running command view\n";
	command_list.clear();
	command_list = { "view" };
	ViewTasks(command_list);

	std::cout << "---------------------\n";
	std::cout << "Test 13: mark task 2 as completed\n";
	std::cout << "Expected output: Task 2 will be marked as completed\n";
	std::cout << "Running command complete 2\n";
	command_list.clear();
	command_list = { "complete", "2" };
	CompleteTask(command_list);
	std::cout << "Running command view\n";
	command_list.clear();
	command_list = { "view" };
	ViewTasks(command_list);

	std::cout << "---------------------\n";
	std::cout << "Test 14: mark task 3 as completed\n";
	std::cout << "Expected output: Task 3 will be marked as completed\n";
	std::cout << "Running command complete 3\n";
	command_list.clear();
	command_list = { "complete", "3" };
	CompleteTask(command_list);
	std::cout << "Running command view\n";
	command_list.clear();
	command_list = { "view" };
	ViewTasks(command_list);

	std::cout << "---------------------\n";
	std::cout << "Test 15: mark task 2 as incomplete\n";
	std::cout << "Expected output: Task 2 will be marked as incomplete\n";
	std::cout << "Running command uncomplete 2\n";
	command_list.clear();
	command_list = { "uncomplete", "2" };
	UncompleteTask(command_list);
	std::cout << "Running command view\n";
	command_list.clear();
	command_list = { "view" };
	ViewTasks(command_list);

	std::cout << "---------------------\n";
	std::cout << "Test 16: delete task 2\n";
	std::cout << "Expected output: Task 2 will be deleted, ID of tasks after will be moved up\n";
	std::cout << "Running command delete 2\n";
	command_list.clear();
	command_list = { "delete", "2" };
	DeleteTask(command_list);
	std::cout << "Running command view\n";
	command_list.clear();
	command_list = { "view" };
	ViewTasks(command_list);

	std::cout << "---------------------\n";
	std::cout << "Test 17: view tasks with --hide-complete\n";
	std::cout << "Expected output: Task 2 not be shown (2 tasks total)\n";
	std::cout << "Running command view --hide-complete\n";
	command_list.clear();
	command_list = { "view" , "--hide-complete" };
	ViewTasks(command_list);

	std::cout << "---------------------\n";
	std::cout << "Test 18: view tasks with --sort-asc\n";
	std::cout << "Expected output: Tasks will be shown in ascending due date order (soonest first)\n";
	std::cout << "Running command view --sort-asc\n";
	command_list.clear();
	command_list = { "view" , "--sort-asc" };
	ViewTasks(command_list);

	std::cout << "---------------------\n";
	std::cout << "Test 19: view tasks with --sort-desc\n";
	std::cout << "Expected output: Tasks will be shown in descending due date order (furthest first)\n";
	std::cout << "Running command view --sort-desc\n";
	command_list.clear();
	command_list = { "view" , "--sort-desc" };
	ViewTasks(command_list);

	std::cout << "---------------------\n";
	std::cout << "Test 20: view tasks with --hide-complete and --sort-asc\n";
	std::cout << "Expected output: Tasks will be shown in ascending due date order (soonest first), hiding complete tasks\n";
	std::cout << "Running command view --hide-complete --sort-asc\n";
	command_list.clear();
	command_list = { "view" , "--hide-complete", "--sort-asc"};
	ViewTasks(command_list);

	std::cout << "---------------------\n";
	std::cout << "Test 21: view tasks with --hide-complete and --sort-desc\n";
	std::cout << "Expected output: Tasks will be shown in descending due date order (furthest first), hiding complete tasks\n";
	std::cout << "Running command view --hide-complete --sort-desc\n";
	command_list.clear();
	command_list = { "view" , "--hide-complete", "--sort-desc" };
	ViewTasks(command_list);

	std::cout << "---------------------\n";
	std::cout << "Test 22: switching order of --hide-complete and --sort-asc\n";
	std::cout << "Expected output: Tasks will be shown in ascending due date order (soonest first), hiding complete tasks\n";
	std::cout << "Running command view --sort-asc --hide-complete\n";
	command_list.clear();
	command_list = { "view", "--sort-asc", "--hide-complete" };
	ViewTasks(command_list);

	std::cout << "---------------------\n";
	std::cout << "Test 23: switching order of --hide-complete and --sort-desc\n";
	std::cout << "Expected output: Tasks will be shown in ascending due date order (furthest first), hiding complete tasks\n";
	std::cout << "Running command view --sort-desc --hide-complete\n";
	command_list.clear();
	command_list = { "view", "--sort-desc", "--hide-complete" };
	ViewTasks(command_list);

	std::cout << "\n\n---------------------\n";
	std::cout << "STAGE 2: INCORRECT USAGE TEST\n";
	std::cout << "---------------------\n";
	std::cout << "Test 24: viewing with incorrect arguments\n";
	std::cout << "Expected output: Viewing will continue as normal as if the incorrect arguments did not exist\n";
	std::cout << "Running command view testing random hehe\n";
	command_list.clear();
	command_list = { "view", "testing", "random", "hehe" };
	ViewTasks(command_list);

	std::cout << "---------------------\n";
	std::cout << "Test 25: viewing with incorrect arguments between correct arguments\n";
	std::cout << "Expected output: Viewing will ignore incorrect arguments but parse correct arguments\n";
	std::cout << "Running command view testing --hide-complete random hehe\n";
	command_list.clear();
	command_list = { "view", "testing", "--hide-complete", "random", "hehe"};
	ViewTasks(command_list);
	std::cout << "Running command view testing --hide-complete random --sort-desc hehe\n";
	command_list.clear();
	command_list = { "view", "testing", "--hide-complete", "random", "--sort-desc", "hehe"};
	ViewTasks(command_list);

	std::cout << "---------------------\n";
	std::cout << "Test 26: adding with incorrect date format\n";
	std::cout << "Expected output: Task will not be added, with error showing that the format is not accepted.\n";
	std::cout << "Running command add today do work\n";
	command_list.clear();
	command_list = { "add", "today", "do", "work"};
	AddTask(command_list);
	std::cout << "Running command view\n";
	command_list.clear();
	command_list = { "view" };
	ViewTasks(command_list);

	std::cout << "---------------------\n";
	std::cout << "Test 27: adding with correct date format but invalid date\n";
	std::cout << "Expected output: Task will not be added, with error showing that the date is not valid.\n";
	std::cout << "Running command add 42/12/3024 do work\n";
	command_list.clear();
	command_list = { "add", "42/12/3024", "do", "work" };
	AddTask(command_list);
	std::cout << "Running command view\n";
	command_list.clear();
	command_list = { "view" };
	ViewTasks(command_list);

	std::cout << "---------------------\n";
	std::cout << "Test 28: completing a non-number index\n";
	std::cout << "Expected output: Error message saying not a number\n";
	std::cout << "Running command complete this\n";
	command_list.clear();
	command_list = { "complete", "this"};
	CompleteTask(command_list);

	std::cout << "---------------------\n";
	std::cout << "Test 29: completing an out of range index\n";
	std::cout << "Expected output: Error message saying index is out of range\n";
	std::cout << "Running command complete 0\n";
	command_list.clear();
	command_list = { "complete", "0" };
	CompleteTask(command_list);
	std::cout << "Running command complete 1000\n";
	command_list.clear();
	command_list = { "complete", "1000" };
	CompleteTask(command_list);

	std::cout << "---------------------\n";
	std::cout << "Test 30: completing with trailing rubbish\n";
	std::cout << "Expected output: Completing as normal, trailing values are ignored\n";
	std::cout << "Running command complete 1 extra value\n";
	command_list.clear();
	command_list = { "complete", "1", "extra", "value"};
	CompleteTask(command_list);
	std::cout << "Running command view\n";
	command_list.clear();
	command_list = { "view" };
	ViewTasks(command_list);

	std::cout << "---------------------\n";
	std::cout << "Test 31: uncompleting a non-number index\n";
	std::cout << "Expected output: Error message saying not a number\n";
	std::cout << "Running command uncomplete this\n";
	command_list.clear();
	command_list = { "uncomplete", "this" };
	UncompleteTask(command_list);

	std::cout << "---------------------\n";
	std::cout << "Test 32: uncompleting an out of range index\n";
	std::cout << "Expected output: Error message saying index is out of range\n";
	std::cout << "Running command uncomplete 0\n";
	command_list.clear();
	command_list = { "uncomplete", "0" };
	UncompleteTask(command_list);
	std::cout << "Running command uncomplete 1000\n";
	command_list.clear();
	command_list = { "uncomplete", "1000" };
	UncompleteTask(command_list);

	std::cout << "---------------------\n";
	std::cout << "Test 33: uncompleting with trailing rubbish\n";
	std::cout << "Expected output: Uncompleting as normal, trailing values are ignored\n";
	std::cout << "Running command uncomplete 1 extra value\n";
	command_list.clear();
	command_list = { "uncomplete", "1", "extra", "value" };
	UncompleteTask(command_list);
	std::cout << "Running command view\n";
	command_list.clear();
	command_list = { "view" };
	ViewTasks(command_list);

	std::cout << "---------------------\n";
	std::cout << "Test 34: deleting a non-number index\n";
	std::cout << "Expected output: Error message saying not a number\n";
	std::cout << "Running command delete this\n";
	command_list.clear();
	command_list = { "delete", "this" };
	DeleteTask(command_list);

	std::cout << "---------------------\n";
	std::cout << "Test 35: deleting an out of range index\n";
	std::cout << "Expected output: Error message saying index is out of range\n";
	std::cout << "Running command delete 0\n";
	command_list.clear();
	command_list = { "delete", "0" };
	DeleteTask(command_list);
	std::cout << "Running command delete 1000\n";
	command_list.clear();
	command_list = { "delete", "1000" };
	DeleteTask(command_list);

	std::cout << "---------------------\n";
	std::cout << "Test 36: deleting with trailing rubbish\n";
	std::cout << "Expected output: deleting as normal, trailing values are ignored\n";
	std::cout << "Running command delete 1 extra value\n";
	command_list.clear();
	command_list = { "delete", "1", "extra", "value" };
	DeleteTask(command_list);
	std::cout << "Running command view\n";
	command_list.clear();
	command_list = { "view" };
	ViewTasks(command_list);

	std::cout << "\n\n---------------------\n";
	std::cout << "STAGE 3: QUITTING TEST\n";
	std::cout << "---------------------\n";
	std::cout << "Test 37: quitting the application\n";
	std::cout << "Expected output: application closes\n";
	std::cout << "Please run the \"quit\" command manually. Thank you.\n";
}

/*

End of debug code

*/