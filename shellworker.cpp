#include <algorithm>
#include "shellworker.h"
using namespace std;
extern char ** environ;
map<string, string> ShellWorker::var_val_map;

/////////////////////////////////////cd command////////////////////////////////////////////////

void ShellWorker::changeDirectory(){
	// Error handling for input arguments 
	if(argList.size() == 1){
	    cerr << "Error: there's no target path" << endl;
    	return;						
	}
	else if(argList.size() > 2){
	    cerr << "Error: more than one target path" << endl;
    	return;						
	}

	// cd ~ should return to home
	else if(argList[1].compare("~") == 0){
		if(chdir(getenv("HOME")) != 0){
	    	cerr << "Error: can't return to HOME" << endl;
    		return;				
		}
	}
	// Make sure cd is a success. Or error should be reported
	else if(chdir(argList[1].c_str()) == 0){
    	return;	
	}
	else{
    	cerr << "Error: no such directory, chdir fails" << endl;
    	return;	
	}
}

/////////////////////////////////////set command////////////////////////////////////////////////

void ShellWorker::insertIntoMap(){

	// Check the variable name, make sure the user uses a valid name
	for(size_t i = 0; i < argList[1].length(); i++){
		const char c = argList[1][i];
    	if ((!isdigit(c)) && (!isalpha(c)) && (c != '_')) {
      		cerr << "Error: " << argList[1] << " is not a valid name" << endl;
      		return;
    	}
	}

	// Use the iterator to find the variable in the map
	map<string, string>::iterator it = var_val_map.find(argList[1]);
	// If really find it in the map, we update the value
	if(it != var_val_map.end()){
		cout << "Update the value as: " << argList[2] << endl;
		it->second = argList[2];	
	}
	// Else we insert it into the map as new.
	else{
		var_val_map[argList[1]] = argList[2];
	}
}

/////////////////////////////////////export command/////////////////////////////////////////////
void ShellWorker::exportToEnv(){
	// Error handling for input arguments 
	if(argList.size() == 1){
		cerr << "Error: missing variable name" << endl;
	}
	if(argList.size() > 2){
		cerr << "Error: more than one variable" << endl;
    	return;		
	}

	// Use the iterator to find the variable in the map
	map<string, string>::iterator it = var_val_map.find(argList[1]);
	// If fails to find it in the map, report error
	if(it == var_val_map.end()){
		cerr << "Error: no such variable in the map" << endl;
	}
	// Else, export it to the environment by setenv
	else{
		string varValue = var_val_map[argList[1]];
		if(setenv(argList[1].c_str(), varValue.c_str(), 1) != 0){
			cerr << "Error: fail to export" << endl;
		}
	}
}


/////////////////////////////////////rev command////////////////////////////////////////////////

void ShellWorker::reverseValStr(){
	// Error handling for input arguments
	if(argList.size() == 1){
		cerr << "Error: missing content after rev command" << endl;
	}
	if(argList.size() > 2){
		cerr << "Error: more than one variable" << endl;
    	return;		
	}
	// Use the iterator to find the variable in the map
	map<string, string>::iterator it = var_val_map.find(argList[1]);
	// If fails to find it in the map, report error
	if(it == var_val_map.end()){
		cerr << "Error: no such variable in the map" << endl;
	}	
	// Else, reverse this variable's value string
	else{
		string varValue = var_val_map[argList[1]];
		// Reference from cplusplus
		// std::reverse will "Reverses the order of the elements in the range [first,last)."
		reverse(varValue.begin(), varValue.end()); 
		cout << "Reverse the value string as: " << varValue << endl;
		it->second = varValue;
	}	
}



/////////////////////////////////////Execute program////////////////////////////////////////////////

bool ShellWorker::searchExistence(string & pathLine){

	// Search the target in the pathList created by ECE551PATH
	for(size_t i = 0; i < pathList.size(); i++){
		DIR * dirp = opendir(pathList[i].c_str());
		struct dirent * dp;
		// Error reported if its return value is NULL
		if (dirp == NULL){
			cerr << "Error: can't open the directory, opendir fails" << endl;
			return false;
		}
		// Else the shell should make the passed-in name complete, 
		// by using the path stored in the pathlist.
		while ((dp = readdir(dirp)) != NULL) {
			if (pathLine.compare(dp->d_name)==0) {
				pathLine = pathList[i] + "/" + pathLine;
				closedir(dirp);
				return true;
			}
		}
		closedir(dirp);									
	}		
	
	return false;
}

/////////////////////////////////////////

void ShellWorker::executeProgram(){

	// Initialize the char ** argv with the argument list created
	char ** argv = new char *[argList.size() + 1];
	for(size_t i = 0; i < argList.size(); i++){
		argv[i] = strdup(argList[i].c_str());
	}
	// the man page of execve tells that it's null-terminated
	argv[argList.size()] = NULL;
	execve(argv[0], argv, environ);
	cerr << "Error: execve" << endl;
	// Avoid memory leak
	for (size_t i = 0; i < argList.size(); i++) {
    	free(argv[i]);
  	}
  	delete[] argv;
  	exit(EXIT_FAILURE);

}

/////////////////////////////////////////

void ShellWorker::runProgram(){

	size_t fSlash_index = argList[0].find('/');
	//if it doesn't contain a forward slash
	if (fSlash_index == string::npos){
		if (!searchExistence(argList[0])) {
			cerr << "Error: the program doesn't exist" << endl;
			return;
		}
	}
	pid_t fork_value = fork();
	pid_t w;
	int wstatus;
	if (fork_value == -1) {
		cerr << "Error: fork failed" << endl;
		exit(EXIT_FAILURE);
	}
	else if (fork_value == 0) {
	    //this is child process
		executeProgram();
	}
	else {
		//parent process
		do {
			w = waitpid(fork_value, &wstatus, WUNTRACED | WCONTINUED);
			if (w == -1) {
				cerr << "Error: waitpid" << endl;
				exit(EXIT_FAILURE);
			}
			// Print-out results should follow the guidelines in commandShell.txt
			if (WIFEXITED(wstatus)){
				if(WEXITSTATUS(wstatus) == 0){
					cout << "Program was successful" << endl;
				}
				else{
					cout << "Program failed with code " << WEXITSTATUS(wstatus) << endl;
				}
			}
			else if (WIFSIGNALED(wstatus)) {
				cout << "Terminated by signal " << WTERMSIG(wstatus) << endl;
			}
		} while (!WIFEXITED(wstatus) && !WIFSIGNALED(wstatus));
	}		
}
/////////////////////////////////////abstraction/////////////////////////////////////////////////////
void ShellWorker::startWorking(){
  
	// Declare an object of the vectorHelper
	// Initialize it with the input string and the map in this shellworker 

	vectorHelper vH(inputStr, var_val_map);

	// vectorHelper will help to process the path list and the argument list
	// Assign these two lists to the argList and pathList in this class object
	// so that the shell worker can use them in the above methods
	vH.processingLists();
	argList = vH.argList;
	pathList = vH.pathList;
	string command = vH.cmdStr;

	// Based on the command types, call different methods.
	if(command.compare("cd") == 0){
		changeDirectory();
		return;
	}
	else if(command.compare("set") == 0){
		insertIntoMap();
		return;			
	}		
	else if(command.compare("export") == 0){
		exportToEnv();
		return;		
	}
	else if(command.compare("rev") == 0){
		reverseValStr();	
		return;	
	}	
	else{
		runProgram();
		return;
	}
}


