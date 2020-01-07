#ifndef __SHELLWORKER_H__
#define __SHELLWORKER_H__
#include <dirent.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#include "vectorHelper.h"
using namespace std;
extern char ** environ;

class ShellWorker{
private:
	string inputStr;

public:
	vector<string> argList;	
	vector<string> pathList;
	// We wish to store the variable names and values
	// Here use static so that we can access to them in each run
	static map<string, string> var_val_map;	

public:
	ShellWorker();
	ShellWorker(string inputStr): inputStr(inputStr){};
	~ShellWorker(){
		vector<string>().swap(argList);
		vector<string>().swap(pathList);
	}

	// when the command is cd, we use this function to change directory
	void changeDirectory();
	
	// when the command is set, we use this function to insert	
	void insertIntoMap();

	// when the command is export, we use this function to export
	void exportToEnv();

	// when the command is rev, we use this function to reverse
	void reverseValStr();
	
	// Search for a program's existence
	// a helper for the function "runProgram"
	bool searchExistence(string & pathLine);

	// when fork()=0 -> child process
	// a helper for the function "runProgram"
	void executeProgram();//use execve

	// Integrate all the things in step 1
	// also integrate the above three functions
	void runProgram();//synthesize all these functions above

	// Use abstraction, integrate all the functions above
	// so that I only need to call this function
	void startWorking();
};

#endif
