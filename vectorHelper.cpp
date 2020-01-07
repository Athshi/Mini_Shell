#include "vectorHelper.h"
using namespace std;
extern char ** environ;

void vectorHelper::replaceVar(string & input){
	// cleanInput refers to the input string that is cleaned up after replacements.
	string cleanInput;

	size_t i = 0; 
	while(i < input.length()){
		// If a '$' is encountered, the shell should read the entire sequence of characters 
		// till another $ shows up or a whitespace is encountered.
		// store that sequence of characters into test_key for later search
		if(input[i] == '$'){
			string test_key;
			i++;
			while(input[i] != ' ' && input[i] != '$' && i < input.length()){
				const char c = input[i];
				// only letters, numbers and '_' are valid characters for variable name
				// so here the shell searches for the longest valid sequence of characters
    			if (isdigit(c) || isalpha(c) || c == '_'){
					test_key += input[i];
					i++;
    			}
    			else{
    				break;
    			}							
			}
			// Use map's iterator to find whether the test_key is in the map
			// If find it in the map, store its value by appending it to the cleanInput string
			// Else, report errors
			map<string, string>::iterator it = var_val_map.find(test_key);
			if(it != var_val_map.end()){
				cleanInput.append(it->second);
			}		
			else{
				cerr << "Error: fail to find the variable name in the map" << endl;
				exit(EXIT_FAILURE);	
			}	
		}
		// Still need to append other characters 
		// which are not part of the variable name to the cleanInput string. keep it that way
			if(i < input.length()){
				cleanInput += input[i];
				i++;
			}
	}
	// assign the cleaned up cleanInput to the input. 
	// we are done with the replacement.
	input = cleanInput;	
}
/////////////////////////////////////////
// use this function as a helper for dealing with unquoted arguments
string vectorHelper::getUnQuotedArg(string & argstring){
	size_t i = 0;
	string currArg;	
	// corner case handling for single character
	if(argstring.length() == 1){
		if(argstring[i] != '\\'){
			currArg += argstring[i];
		}
		argstring.clear();		
	}
	else{
		// In this while-loop, the shell performs pair-wise elements check for escaping
		// If an escaped character is identified, the index should jump two (i.e. i+=2)
		// All the escaped whitespace should have been detected in the pairwirse checks
		// So as long as a single whitespace is detected, the shell should finish processing and return
		while(i + 1 < argstring.length()){
			if(argstring[i] == ' '){			
				argstring = argstring.substr(i);
				return currArg;
			}
			else if(argstring[i] == '\\'){
				if(argstring[i+1] == '\"' || argstring[i+1] == '\\' || argstring[i+1] == ' '){
					currArg += argstring[i+1];
					i += 2;
				}
				// Note that in my setting, a single backslash is ignored
				else{
					i++;
				}
			}
			else if(argstring[i] == '\"' ){
				// Note that in my setting, a single quotation mark that is not at the end should be replaced with a single whitespace
				if(argstring[i+1] != ' '){
					currArg += ' ';
					i++;
				}
				// This is the case where a single quotation mark is at the end
				// Error should be reported for this unclosed quotation mark
				else{
					cerr << "Error: unclosed quotation mark" << endl;
					exit(EXIT_FAILURE);	
				}
			}			
			// Other "normal" characters should simply be appended to the currArg 
			// currArg is a string for storing all the characters we are returning 
			else{
				currArg += argstring[i];
				i++;			
			}

			// This block is for the case where the index has made 2-step jump(i += 2 is implemented before)
			// Besides, the while-loop's condition is i+1<length, so the shell needs to handle this case here.
			if(i == argstring.length()-1){
				if(argstring[i] == '\"'){
  					cerr << "Error: unclosed quotation mark" << endl;
  					exit(EXIT_FAILURE);		
				}
				else if(argstring[i] != ' ' && argstring[i] != '\\'){
					currArg += argstring[i];
				}				
				argstring.clear();
				return currArg;
			}
		}	
	}
	argstring.clear();
	return currArg;
}

/////////////////////////////////////////

string vectorHelper::getQuotedArg(string & argstring) {
  //already have '/"' at [0], so we start from i = 1                                                                                                          
  size_t i = 1;
  string currArg;
  while (i + 1 < argstring.length()) {
	    if(argstring[i] == '\"'){
  			// If another quotation mark is encountered, and it's followed by whitespace
  			// then this quoted argument has ended, the shell should return it	    	
	    	if(argstring[i+1] == ' '){
	    		argstring = argstring.substr(i+1);
	    		return currArg;	    		
	    	}
	    	// These three conditional statements are for escaping checks
	    	else{
	    		currArg += ' ';
	    		i++;	    		
	    	}
	    }
	    // Escape characters handling
	    if(argstring[i] == '\\'){
			if(argstring[i+1] == '\"' || argstring[i+1] == '\\' || argstring[i+1] == ' '){
				currArg += argstring[i+1];
				i += 2;
			}
	    	// A single backslash should be ignored
	    	else{
	    		i++;
	    	}
	    }

	    // "normal" characters should simply be appended to currArg
	    else {
	      	currArg += argstring[i];
	      	i++;
	    }
		// This block is for the case where the index has made 2-step jump(i += 2 is implemented before)
		// Besides, the while-loop's condition is i+1<length, so the shell needs to handle this case here.
	    if(i == argstring.length()-1){
	    	if(argstring[i] == '\"'){
	    		argstring.clear();
	    		return currArg;
	    	}
	    }

  }
  cerr << "Error: unclosed quotation mark" << endl;
  exit(EXIT_FAILURE);
}

/////////////////////////////////////////

void vectorHelper::getArgList(){
    // find the space's position, so that we can have the command name                                                                                        
    size_t CmdEnd = inputStr.find_first_of(" ", 0);
    cmdStr = inputStr.substr(0, CmdEnd);
   	// here the shell will only do replacement on the command name (in case the user sets a variable's value as a command's name)
   	// because the shell need to know the command name to decide in which way the shell should handle the arguments
   	// basically there are two ways: 
   	// one specific way for "set" command, where everything in value should be kept
   	// one general way for all the other commands
    replaceVar(cmdStr);
    // Re-organize the input string with the already-processed command name
    inputStr.replace(0, CmdEnd, cmdStr);

    CmdEnd = inputStr.find_first_of(" ", 0);
    cmdStr = inputStr.substr(0, CmdEnd);
    size_t argStart = inputStr.find_first_not_of(" ", CmdEnd);

    // there are no arguments in the command line,   
    // unless the non-space position isn't at the end of the command line                                                                                     
    if (argStart != string::npos) {
      argStr = inputStr.substr(argStart);
    }

    argList.push_back(cmdStr);

    // This whole IF-ELSE is for handling the arguments in two ways that I said above
    // One way for "set", the other way for all the other commands
	if(argList[0].compare("set") == 0){
		// Error handling: the argument string that is after "Set" is empty
		if(argStr.empty()){
	    	cerr << "Error: missing varName and varValue" << endl;
	    	return;			
		}

		string varvalLine(argStr);
		size_t varname_end = varvalLine.find_first_of(" ");
		string varName = varvalLine.substr(0, varname_end);
		argList.push_back(varName);

		if(varname_end == string::npos){
	    	cerr << "Error: missing varValue" << endl;
	    	return;			
		}
		// Note that here the shell would put everything in the command line that is after the varname as its value
		size_t varval_start = varvalLine.find_first_not_of(" ", varname_end);
		string varValue = varvalLine.substr(varval_start);
		argList.push_back(varValue);
	}
	// ELSE block: for all the other command types, process argument list for those types
	else{
		if(argStr.empty()){   
	    	return;			
		}	
		// Note that here argStr is processed in such a way that 
		// the already-processed part will be stored in the argument list
		// and their corresponding part in the argStr will be erased.
		// So, if all the processing is done, the argStr should be empty
		while(!argStr.empty()){
			size_t nextArgStart = argStr.find_first_not_of(" ", 0);	
			if(nextArgStart == string::npos){
				break;
			}	
			argStr = argStr.substr(nextArgStart);
		  	if(argStr[0] == '\"'){
		  		//enter quotation-surrounded all-included argument
		  		argList.push_back(getQuotedArg(argStr));	  			  		
		  	}
		  	else{
		  		argList.push_back(getUnQuotedArg(argStr));	  		
		  	}		
		}			
	}

	// Finally we do the replacement argument by argument. 
	for(size_t i = 0; i < argList.size(); i++){
		replaceVar(argList[i]);
	}
}

/////////////////////////////////////////

void vectorHelper::getPathList(){
	int path_start = 0;
	size_t path_len = 0;
	string ECE551PATH(getenv("ECE551PATH"));
	// Here the shell gets the index of the last colon
	// because colon is a separator for the sequence of paths
	// the shell can use them to know the length of each path
	size_t lastColonInd = ECE551PATH.find_last_of(':');
	string temp_path;
	for(size_t i = 0; i <= lastColonInd; i++){
	    if(ECE551PATH[i] == ':'){
	    	path_len = i - path_start;
	    	temp_path = ECE551PATH.substr(path_start, path_len);	  	
	    	pathList.push_back(temp_path);
	    	temp_path.clear();
	    	path_start = i+1;
	    	continue;
	    }
	}
	temp_path = ECE551PATH.substr(path_start);	
	pathList.push_back(temp_path);
}

/////////////////////////////////////////
// Absraction: integrate all the above methods
// Setting up the path list and the argument list.
void vectorHelper::processingLists() {
  	getPathList();
    getArgList();
  	return;
}
