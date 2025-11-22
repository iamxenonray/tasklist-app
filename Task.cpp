#include "Task.h"

void TaskSystem::PrintTaskList()
{
	for (int i = 0; i < TaskList.size(); ++i)
	{
		std::cout << "[" << i + 1 << "] " << TaskList[i].ToString() << std::endl;
	}
}

void TaskSystem::AddTask(std::string name, std::string due_date)
{
	TaskList.push_back(Task(name, due_date));
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
}

Task::Task(std::string name, std::string due)
{
	TaskName = name;
	DueDate = due;
	Status = INCOMPLETE;
}

std::string Task::ToString()
{
	return (TaskName + DueDate + (Status == INCOMPLETE ? "INCOMPLETE" : "COMPLETE"));
}
