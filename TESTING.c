/*************************************************************************************************************************************/
1.  Run ./ffosh
    Output: ffosh:/home/yl559/ece551/mp_miniproject $
    Reason: It is correct because running this shell should first print out the current directory

/*************************************************************************************************************************************/
2.  Run \n
    Output: ffosh:/home/yl559/ece551/mp_miniproject $
    Reason: It is correct because the input is empty, the shell should print out the current directory and wait for the next input.

/*************************************************************************************************************************************/
3.  Run control+D
    Output: yl559@ece551:~/ece551/mp_miniproject$
    Reason: It is correct because EOF is encountered when reading from stdin, then the shell should exit.

/*************************************************************************************************************************************/
4.  Run exit
    Output: yl559@ece551:~/ece551/mp_miniproject$
    Reason: It is correct because the user types the command "exit" then the shell should exit.

/*************************************************************************************************************************************/
5.  Run /bin/ls
    Output: commandShell.txt  ffosh.cpp   Makefile       numerics.txt  shellworker.cpp  TESTING.txt   try               vectorHelper.h
            ffosh             ffosh.cpp~  Makefile~      README        shellworker.h    TESTING.txt~  vectorHelper.cpp
            Program was successful
            ffosh:/home/yl559/ece551/mp_miniproject $
    Reason: It is correct because the shell execute ls and list all the files within the current directory. After finish "ls", the shell should print
            out the current directory and wait for the next input.

/*************************************************************************************************************************************/
6.  Run	ls
    Output: commandShell.txt  ffosh.cpp   Makefile       numerics.txt  shellworker.cpp  TESTING.txt   try               vectorHelper.h
            ffosh             ffosh.cpp~  Makefile~      README        shellworker.h    TESTING.txt~  vectorHelper.cpp
            Program was successful
            ffosh:/home/yl559/ece551/mp_miniproject $
    Reason: It is correct because the shell search the path list for ls, then execute ls and list all the files within the current directory.
            After finish "ls", the shell should print out the current directory and wait for the next input.

/*************************************************************************************************************************************/
7.  Run	/bin
    Output: Error: execve
            Program failed with code 1
    Reason: It is correct because /bin is a directory, not an executable program. So the program failed.

/*************************************************************************************************************************************/
8.  Run cd 
    Output: Error: there's no target path
            ffosh:/home/yl559/ece551/mp_miniproject $
    Reason: It is correct because 

/*************************************************************************************************************************************/
8.  Run cd /bin
    Output: 
    Reason: It is correct because 

/*************************************************************************************************************************************/
8.  Run cd /bin/ls
    Output: 
    Reason: It is correct because 

/*************************************************************************************************************************************/
8.  Run cd /bin 
    Output: 
    Reason: It is correct because 

/*************************************************************************************************************************************/
8.  Run cd 
    Output: 
    Reason: It is correct because 

/*************************************************************************************************************************************/
8.  Run 
    Output: 
    Reason: It is correct because 

/*************************************************************************************************************************************/
8.  Run 
    Output: 
    Reason: It is correct because 

/*************************************************************************************************************************************/
8.  Run 
    Output: 
    Reason: It is correct because 

/*************************************************************************************************************************************/
8.  Run 
    Output: 
    Reason: It is correct because 

/*************************************************************************************************************************************/
8.  Run 
    Output: 
    Reason: It is correct because 







