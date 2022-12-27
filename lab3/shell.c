#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <pwd.h>
#include <ctype.h>
#include <sys/wait.h>
#include <sys/stat.h>

//+
// File:	shell.c
//
// Pupose:	This program implements a simple shell program. It does not start
//		processes at this point in time. However, it will change directory
//		and list the contents of the current directory.
//
//		The commands are:
//		   cd name -> change to directory name, print an error if the directory doesn't exist.
//		              If there is no parameter, then change to the home directory.
//		   ls -> list the entries in the current directory.
//			      If no arguments, then ignores entries starting with .
//			      If -a then all entries
//		   pwd -> print the current directory.
//		   exit -> exit the shell (default exit value 0)
//				any argument must be numeric and is the exit value
//
//		if the command is not recognized an error is printed.
//-d

#define CMD_BUFFSIZE 1024
#define MAXARGS 10

// Prototypes
int splitCommandLine(char *commandBuffer, char *args[], int maxargs);
int doInternalCommand(char *args[], int nargs);
int doProgram(char *args[], int nargs);

//+
// Function:	main
//
// Purpose:	The main function. Contains the read
//		eval print loop for the shell.
//
// Parameters:	(none)
//
// Returns:	integer (exit status of shell)
//-

int main()
{

    char commandBuffer[CMD_BUFFSIZE];
    // note the plus one, allows for an extra null
    char *args[MAXARGS + 1];

    // print prompt.. fflush is needed because
    // stdout is line buffered, and won't
    // write to terminal until newline
    printf("%%> ");
    fflush(stdout);

    while (fgets(commandBuffer, CMD_BUFFSIZE, stdin) != NULL)
    {

        // remove newline at end of buffer
        int cmdLen = strlen(commandBuffer);
        if (commandBuffer[cmdLen - 1] == '\n')
        {
            commandBuffer[cmdLen - 1] = '\0';
            cmdLen--;
        }

        // split command line into words.(Step 2)
        int nargs = splitCommandLine(commandBuffer, args, MAXARGS + 1); // Returns the number of arguments
        // TODO

        // add a null to end of array (Step 2)
        args[nargs] = NULL;

        // TODO

        // // debugging
        // printf("%d\n", nargs);
        // int i;
        // for (i = 0; i < nargs; i++)
        // {
        //     printf("%d: %s\n", i, args[i]);
        // }
        // // element just past nargs
        // printf("%d: %x\n", i, args[i]);

        // TODO: check if 1 or more args (Step 3)
        if (args[0] != NULL)
        {
            // TODO: if one or more args, call doInternalCommand  (Step 3)
            if (doInternalCommand(args, nargs) == 0)
            {
                // TODO: if doInternalCommand returns 0, call doProgram  (Step 4)
                if (doProgram(args, nargs) == 0)
                {
                    // TODO: if doProgram returns 0, print error message (Step 3 & 4)
                    fprintf(stderr, "Error command not found.\n");
                }
            }
        }

        // print prompt
        printf("%%> ");
        fflush(stdout);
    }
    return 0;
}

////////////////////////////// String Handling (Step 1) ///////////////////////////////////

//+
// Function:	skipChar
//
// Purpose:	TODO: finish description of function
//
// Parameters:
//    charPtr	Pointer to string
//    skip	character to skip
//
// Returns:	Pointer to first character after skipped chars
//		ID function if the string doesn't start with skip,
//		or skip is the null character
//-

char *skipChar(char *charPtr, char skip)
{
    // TODO: contents of function
    // TODO: replace null with proper value

    // Initialize a temporary pointer to the character array
    char *pTemp = charPtr;

    // If there is no skip argument passed into the function just return instantly
    if (skip == NULL)
        return charPtr;

    // Check if the character is equal to the character we want to skip
    while (*pTemp == skip)
    {
        pTemp++; // Move to the next character in array
    }

    return pTemp; // Return the pointer to the next non-skip character
}

//+
// Funtion:	splitCommandLine
//
// Purpose:	TODO: give descritption of function
//  The function splits the command line arguments from the user into individual words which will be used for executing commands.
// Parameters:
//	TODO: parameters and purpose
//  commandBuffer - char array that holds the command inputs from the terminal
//  args - pointer to the args array to store the seperated arguments.
//  maxargs - the maximum number of arguments in the 'args' array
//
// Returns:	Number of arguments (< maxargs).
//
//-

int splitCommandLine(char *commandBuffer, char *args[], int maxargs)
{
    // TODO: Contents of function

    int i = 0; // Used for indexing args array

    // Checks for empty command line input
    if (*skipChar(commandBuffer, ' ') == NULL)
    {
        return 0;
    }

    char *pChar = skipChar(commandBuffer, ' '); // Finds the next nonspace
    args[i++] = pChar;                          // Adds the pointer of the first char in the command line to the args arrayu

    // Loop through entire character array until we cant find a pointer to an empty space
    while (strchr(pChar, ' ') != NULL)
    {
        pChar = strchr(pChar, ' '); // Stores pointer to the next empty space

        *pChar = '\0'; // Sets space to "\0"

        pChar++; // Increment the pointer char by 1 byte

        pChar = skipChar(pChar, ' '); // Find the pointer to the next NON-empty space

        // If the pointer is at the end
        if (*pChar == NULL)
        {
            break;
        }
        args[i++] = pChar; // Store the pointer to the character in the args array
    }

    // TODO: return proper value
    return i; // Returns the number of arguments (aka char pointers)
}

////////////////////////////// External Program  (Note this is step 4, complete doeInternalCommand first!!) ///////////////////////////////////

// list of directorys to check for command.
// terminated by null value
char *path[] = {
    ".",
    "/usr/bin",
    NULL};

//+
// Funtion:	doProgram
//
// Purpose:	TODO: add description of funciton
//  Executes the internal command provided by the user from the terminal
// Parameters:
//	TODO: add paramters and description
//      *args - list of commands inputted by user
//      nargs - number of arguments inputted by user
// Returns	int
//		1 = found and executed the file
//		0 = could not find and execute the file
//-

int doProgram(char *args[], int nargs)
{
    // find the executable
    // TODO: add body.
    // Note this is step 4, complete doInternalCommand first!!!

    int i = 0; // Index used to access the "path" array elements
    char *buf; // A char pointer that is used to store malloc
    while (path[i] != NULL)
    {                                     // path[i] is command path
        int lenElement = strlen(path[i]); // Length of the current element of path
        int lenCommand = strlen(args[0]); // Length of the command

        buf = (char *)malloc(strlen(path[i]) + 1 + strlen(args[0]) + 1); // Mallocs enough memory for the path, '/', command, and null byte.

        sprintf(buf, "%s/%s", path[i], args[0]); // Formats the string for the command

        struct stat status;
        int statResult = stat(buf, &status); // Gets the file attributes and returns int

        // Checks if the file exists and information was retrieved
        if (statResult == 0)
        {
            // Checks if its a regular file and if the file is executable
            if (S_ISREG(status.st_mode) & (status.st_mode & S_IXUSR != 0))
            {
                break;
            }
        }

        i++;
    }

    // If the directory doesn't exist
    if (path[i] == NULL)
    {
        return 0;
    }

    int forkStatus = fork(); // Causes process to split into a parent and child process

    // Process is the child and the pid of the process is the child process
    if (forkStatus == 0)
    {
        execv(buf, args); // Takes the path to the file to execute, replacing the current running program
    }
    // Process is the parent & forkstatus is the pid
    else if (forkStatus > 0)
    {
        wait(NULL); // Waits for the child process to complete executing
        free(buf);  // Free the buf pointer memory
    }
    else
    {
        fprintf(stderr, "Child process could not be created oof.\n");
    }
    return 1;
}

////////////////////////////// Internal Command Handling (Step 3) ///////////////////////////////////

// define command handling function pointer type
typedef void (*commandFunc)(char *args[], int nargs);

// associate a command name with a command handling function
struct cmdStruct
{
    char *cmdName;
    commandFunc cmdFunc;
};

// prototypes for command handling functions
// TODO: add prototype for each comamand function

commandFunc exitFunc(char *args[], int nargs);
commandFunc pwdFunc(char *args[], int nargs);
commandFunc lsFunc(char *args[], int nargs);
commandFunc cdFunc(char *args[], int nargs);

// list commands and functions
// must be terminated by {NULL, NULL}
// in a real shell, this would be a hashtable.
struct cmdStruct commands[] = {
    // TODO: add entry for each command
    {"cd", cdFunc},
    {"ls", lsFunc},
    {"pwd", pwdFunc},
    {"exit", exitFunc},
    {NULL, NULL} // terminator
};

//+
// Function:	doInternalCommand
//
// Purpose:	TODO: add description
//  Runs through list of user inputted commands and executes the associated command function.
// Parameters:
//	TODO: add parameter names and descriptions
//  *args - list of commands inputted by user
//  nargs - number of arguments inputted by user
//
// Returns	int
//		1 = args[0] is an internal command
//		0 = args[0] is not an internal command
//-

int doInternalCommand(char *args[], int nargs)
{
    // TODO: function contents (step 3)
    int i = 0;

    // Infinite loop to execute each args element
    while (1)
    {
        // Error checking for non-existant command name.
        if (commands[i].cmdName == NULL)
            break;

        if (strcmp(commands[i].cmdName, args[0]) == 0) // Is there a valid command
        {
            commands[i].cmdFunc(args, nargs); // Execute associate function
            return 1;                         // Return 1 meaning we did find the common match (internal command)
        }
        i++;
    }

    return 0; // Return 0 meaning we couldn't find the match (not internal command)
}

///////////////////////////////
// comand Handling Functions //
///////////////////////////////

// TODO: a function for each command handling function
// goes here. Also make sure a comment block prefaces
// each of the command handling functions.

// COMMENT THIS FUNCTION

//+
// Function:	exitFunc
//
// Purpose:	TODO: add description
//  Exits the program (stops the shell program).
// Parameters:
//	TODO: add parameter names and descriptions
//  *args - list of commands inputted by user
//  nargs - number of arguments inputted by user
// Returns	int
//		1 = directory name doesn't start with dot
//		0 = directory name starts with dot
//-
commandFunc exitFunc(char *args[], int nargs)
{
    exit(0); // Terminate program
}

//+
// Function:	pwdFunc
//
// Purpose:	TODO: add description
//  Prints the working directory the shell is currently in.
// Parameters:
//	TODO: add parameter names and descriptions
//  *args - list of commands inputted by user
//  nargs - number of arguments inputted by user
//
// Returns	void
//-
commandFunc pwdFunc(char *args[], int nargs)
{
    char *cwd = getcwd(NULL, 0);                // Stores working directory in char array
    printf("The current directory: %s\n", cwd); // Prints current working directory
    free(cwd);
}

//+
// Function:	dotFile
//
// Purpose:	TODO: add description
//  Checks if the directory name has a dot at the beginning
// Parameters:
//	TODO: add parameter names and descriptions
//  *d - A pointer to a struct dirent which holds information about directory
//
// Returns	int
//		1 = directory name doesn't start with dot
//		0 = directory name starts with dot
//-
int dotFile(const struct dirent *d)
{
    // Checks for dot at beginning of directory name
    if (d->d_name[0] == '.')
        return 0;
    else
        return 1;
}

//+
// Function:	lsFunc
//
// Purpose:	TODO: add description
//  lists out the files and directories in the current directory
// Parameters:
//	TODO: add parameter names and descriptions
//  *args - list of commands inputted by user
//  nargs - number of arguments inputted by user
//
// Returns	void
//-
commandFunc lsFunc(char *args[], int nargs)
{
    struct dirent **namelist;
    int numEnts;

    // No second param passed to "ls"
    if (args[1] == NULL)
        numEnts = scandir(".", &namelist, dotFile, NULL);
    //"-a" passed as second param
    else if (strcmp(args[1], "-a") == 0)
        numEnts = scandir(".", &namelist, NULL, NULL);
    else
        fprintf(stderr, "Invalid argument to ls command.\n");

    // Lists out the directories based on command
    for (int i = 0; i < numEnts; i++)
    {
        printf("%s\n", namelist[i]->d_name);
    }
}

//+
// Function:	cdFunc
//
// Purpose:	TODO: add description
//  Changes the user's current directory
// Parameters:
//	TODO: add parameter names and descriptions
//  *args - list of commands inputted by user
//  nargs - number of arguments inputted by user
//
// Returns	void
//-
commandFunc cdFunc(char *args[], int nargs)
{
    //Checks if second argument is empty
    if (args[1] == NULL)
    {
        struct passwd *pw = getpwuid(getuid()); //Retrieve the user database entry for the given user ID
        if (*pw->pw_dir == NULL)  //If home directory is null
        {
            fprintf(stderr, "No current working directory error.\n");
        }
    }
    else
    {
        if (chdir(args[1]) != 0) //Change directory if second argument is not NULL and if it fails log error message.
        {
            fprintf(stderr, "Change directory could not be completed. Directory not found, oof.\n");
        }
    }
}