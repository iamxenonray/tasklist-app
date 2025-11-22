#pragma once
#include <iostream>
#include <string>
#include <vector>

enum TaskStatus
{
	INCOMPLETE,
	COMPLETE
};

class Task
{
public:
	Task() = delete;
	Task(std::string name, std::string due);
	std::string TaskName;
	std::string DueDate;
	TaskStatus Status;

	std::string ToString();
};

class TaskSystem
{
public:
	std::vector<Task> TaskList;

	void PrintTaskList();
	void AddTask(std::string name, std::string due_date);
	void CompleteTask(int index);
	void DeleteTask(int index);
};

// In theory could be implemented nicer with pointers, but for specialised programs this works.
static TaskSystem TaskList;