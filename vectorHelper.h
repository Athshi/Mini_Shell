#ifndef __VECTORHELPER_H__
#define __VECTORHELPER_H__
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

class vectorHelper{
private:
	string inputStr;
	string argStr;	

public:
	string cmdStr; // command name
	vector<string> argList;	// store the command name and the array of arguments
	vector<string> pathList; // a list of paths for search
	map<string, string> var_val_map; // a map to store the variables' names and values
	
public:
	vectorHelper();

	vectorHelper(string inputStr, map<string, string> var_val_map) :
	      inputStr(inputStr),
	      var_val_map(var_val_map) {};

	~vectorHelper(){
		vector<string>().swap(argList);
		vector<string>().swap(pathList);
	}


	// replaceVar() will help to replace any $... that we have stored in the map
	void replaceVar(string & input);

	// A helper for the function getArgList. Handling unquoted arguments
	string getUnQuotedArg(string & argstring);
	// A helper for the function getArgList. Handling quoted arguments
	string getQuotedArg(string & argstring);
	// get the vector argList, serves all kinds of commands except "set"
	void getArgList();

	// get the vector pathList, 
	void getPathList();

	// integrate all the functions above
	// get the pathList and the argList
	void processingLists();
};

#endif
