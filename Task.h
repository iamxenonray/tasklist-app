#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <algorithm>

enum TaskStatus
{
	INCOMPLETE,
	COMPLETE
};

class Task
{
public:
	Task() = delete;
	Task(std::string name, std::chrono::year_month_day due, const int id);
	Task(const Task& rhs);
	std::string TaskName;
	std::chrono::year_month_day DueDate;
	TaskStatus Status;

	// Internal use only
	int ID;

	const std::string ToString();
};

class TaskSystem
{
public:
	std::vector<Task> TaskList;

	/*
	Prints out the task list.
	sort_type: Sorting to be used. 0 = no sort, 1 = closest due first, 2 = furthest due first
	hide_complete: Hide completed items
	*/
	void PrintTaskList(int sort_type = 0, bool hide_complete = false);
	void AddTask(std::string name, std::chrono::year_month_day due_date);
	void CompleteTask(int index);
	void DeleteTask(int index);
	void UncompleteTask(int index);
};

// In theory could be implemented nicer with pointers, but for specialised programs this works.
static TaskSystem TaskList;