#include "shellworker.h"
extern char ** environ;
using namespace std;

int main(){
	// Initilize "ECE551PATH" with the value of "PATH". 
	// Thereafter the shell should be independent of "PATH"
	setenv("ECE551PATH", getenv("PATH"), 1);
	while(1){		
		// Print out the current directory	
	    char * curr_dir;
      	curr_dir = getcwd(NULL, 0);		
		cout << "ffosh:" << curr_dir << " $";
		free(curr_dir);

		// Get user's input
		string inputStr;

		// If EOF is encountered reading from stdin, 
		// then the shell should exit.
		getline(cin, inputStr);
		if(cin.eof()){
			cout << endl;
			return EXIT_SUCCESS;			
		}
		// If the input is empty
		// the shell should start all over
		// and wait for another input
		if(inputStr.empty()){
			continue;
		}

		// clean up all the whitespace before something real shows up
		size_t realStart = inputStr.find_first_not_of(" ", 0);
		inputStr = inputStr.substr(realStart);

		// If the user types the command "exit" 
		// then the shell should exit.
		if(inputStr.compare("exit") == 0){
			cout << endl;
			return EXIT_SUCCESS;
		}

		// Initialize shellworker with the input string
		// make it start working
		ShellWorker shell_wkr(inputStr);
		shell_wkr.startWorking();
	}

}

