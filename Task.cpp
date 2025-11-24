#include "Task.h"


void TaskSystem::PrintTaskList(int sort_type, bool hide_complete)
{
	if (TaskList.empty())
	{
		std::cout << "There are no tasks! Why not add one?\n";
		return;
	}
	int incomplete_tasks = TaskList.size();
	if (sort_type == 1 || sort_type == 2)
	{
		// Only enter here if sorting to not use extra memory.
		std::vector<Task> sorted_list{ TaskList };
		if (sort_type == 1)
		{
			std::stable_sort(sorted_list.begin(), sorted_list.end(), [](Task a, Task b) { return a.DueDate < b.DueDate; });
		}
		else
		{
			std::stable_sort(sorted_list.begin(), sorted_list.end(), [](Task a, Task b) { return a.DueDate > b.DueDate; });
		}
		for (int i = 0; i < sorted_list.size(); ++i)
		{
			if (hide_complete && sorted_list[i].Status == COMPLETE)
			{
				--incomplete_tasks;
				continue;
			}
			std::cout << sorted_list[i].ToString() << std::endl;
		}
		if (hide_complete && incomplete_tasks == 0)
		{
			std::cout << "There are no incomplete tasks! Hooray!\n";
		}
	}
	else
	{
		for (int i = 0; i < TaskList.size(); ++i)
		{
			if (hide_complete && TaskList[i].Status == COMPLETE)
			{
				--incomplete_tasks;
				continue;
			}
			std::cout << TaskList[i].ToString() << std::endl;
		}
		if (hide_complete && incomplete_tasks == 0)
		{
			std::cout << "There are no incomplete tasks! Hooray!\n";
		}
	}
	
}

void TaskSystem::AddTask(std::string name, std::chrono::year_month_day due_date)
{
	TaskList.push_back(Task(name, due_date, TaskList.size() + 1));
	std::cout << "Task added.\n";
}

void TaskSystem::CompleteTask(int index)
{
	if (index < 1 || index > TaskList.size()) return;
	TaskList[index - 1].Status = COMPLETE;
}

void TaskSystem::DeleteTask(int index)
{
	if (index < 1 || index > TaskList.size()) return;
	TaskList.erase(TaskList.begin() + (index - 1));
	// Reset IDs
	for (int i = 0; i < TaskList.size(); ++i)
	{
		TaskList[i].ID = i + 1;
	}
}

void TaskSystem::UncompleteTask(int index)
{
	if (index < 1 || index > TaskList.size()) return;
	TaskList[index - 1].Status = INCOMPLETE;
}

Task::Task(std::string name, std::chrono::year_month_day due, const int id) : TaskName{ name }, DueDate{ due }, Status{ INCOMPLETE }, ID{ id }
{
}

Task::Task(const Task& rhs) : TaskName{ rhs.TaskName }, DueDate{ rhs.DueDate }, Status{ rhs.Status }, ID{ rhs.ID }
{
}

const std::string Task::ToString()
{
	return ("[" + std::to_string(ID) + "] [" + TaskName + "] [Due: " + std::to_string(static_cast<unsigned>(DueDate.day()))
		+ "/" + std::to_string(static_cast<unsigned>(DueDate.month()))
		+ "/" + std::to_string(static_cast<int>(DueDate.year()))
		+ "] [Status: " + (Status == INCOMPLETE ? "INCOMPLETE" : "COMPLETE") + "]");
}
