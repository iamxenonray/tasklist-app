#pragma once
#include <map>
#include <string>
#include <vector>
#include <iostream>
#include <stdexcept>
#include "Task.h"

class CommandMap
{
public:
	std::map<std::string, void (*)(std::vector<std::string>)> Map;
	TaskSystem* TaskList;

	CommandMap();
	void RunCommand(std::string key, std::vector<std::string> args);

private:

	void InitialiseMap();
};
/*

All functions which are mappable must follow the signature: void foo(std::vector<std::string>) .
The remaining arguments are passed into the param, using them is optional.

*/

void PrintHelp(std::vector<std::string> s);

void Quit(std::vector<std::string> s);

void ViewTasks(std::vector<std::string> s);

void AddTask(std::vector<std::string> s);

void CompleteTask(std::vector<std::string> s);