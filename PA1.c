/* Unix Shell Implementation: Includes implementation for internal commands like "cd", "clr", "dir", "env", "echo", "help", "pause", "exit".
The program also contains implementation of commands supporting process like background, piping, input & output redirection.
Author: Pallavi V. Dhakne
Student Id: W1094415
Course: COEN 283*/

/* All header files required by our program are included. */

#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>

/* Internal shell command functions declaration:- cd, help, exit, clr, env, echo, pause, dir, ls */

int shell_cd(char **args);
int shell_help(char **args);
int shell_exit(char **args);
int shell_clr(char **args);
int shell_env(char **args);
int shell_echo(char **args);
int shell_pause(char **args);		
int shell_dir(char **args);
int shell_mls(char **args);	

/* List of internal shell commands */

char *builtin_str[] = {
    "cd",
    "help",
    "exit",
    "clr",
    "env",
    "echo",
    "pause",
    "dir",
    "mls",
  };

/*  List of internal shell commands functions */

int (*builtin_func[]) (char **) = {
    &shell_cd,
    &shell_help,
    &shell_exit,
    &shell_clr,
    &shell_env,
    &shell_echo,
    &shell_pause,
    &shell_dir,
    &shell_mls,
  };

/* Function returns the number of internal builtin functions */

int shell_num_builtins() 
	{
    	return sizeof(builtin_str) / sizeof(char *);
  }

/* Internal Shell command: change directory (cd). Here we change the current default directory to <directory>. If the <directory> argument is not present, 
report the current directory. If the directory does not exist, an appropriate error is reported. This function also changes the PWD environment variable. */

int shell_cd(char **args)
	{
    	char cwd[1024];
    	if (args[1] == NULL) {
      		//fprintf(stderr, "Argument Expected for command \"cd\"\n");
          return 1;
    	} else {
      		if (chdir(args[1]) != 0) {
        		perror("Error while changing directory");
      		}
      		else {
        		getcwd(cwd, sizeof(cwd));
        		setenv("PWD",cwd,1);
      		}
    	}
    	return 1;
  	}

/* Internal Shell command: help. In this function we display the user manual using the more filter. The user manual data is stored in a file helpFile.txt */

int shell_help(char **args)
  	{
    	char command[50];
    	strcpy(command, "more helpFile" );
    	system(command);

    	return 1;
  	}

/* Internal Shell command: exit. In this function we terminate the shell program. */

int shell_exit(char **args)
  	{
    	return 0;
  	}

/* Internal Shell command: clr. In this function we clear the terminal screen. */

int shell_clr(char **args)
  	{
    	system("clear");    
		  return 1;
  	}

/* Internal Shell command: env. In this function we list all the environment variables on the terminal. */

int shell_env(char **args)
  	{
    	extern char **environ;
  		int i = 1;
 	  	char *s = *environ;

  		for (; s; i++) 
    	{
    	 	printf("%s\n", s);
    	 	s = *(environ+i);
  		}

  		return 1;

  	}

/* Internal Shell command: echo. In this function we display the comment on the terminal followed by a new line. Multiple spaces/tabs are reduced 
to a single space.*/

int shell_echo(char **args)
	{
      int i;
	    for (i = 1; args[i]!= NULL; i++)
           printf(" %s", args[i]);
    	printf("\n");
      return 1;

  	}

/* Internal Shell command: pause. In this function we pause shell operation till enetr key is pressed. */

int shell_pause(char **args)
  	{
      	getpass("Press Enter to continue...");
      	return 1;

  	}
/* Internal Shell command: dir. In this function we list the contents of directory. If the directory is not valid we give an error mssg.*/

int shell_dir(char **args)
  	{
	  	DIR *d;
    	struct dirent *dir;
	  	if(args[1] == NULL)
    		d = opendir(".");
	  	else d = opendir(args[1]);
    	
    	if (d)
    	{
        	while ((dir = readdir(d)) != NULL)
          	{
             	printf("%s\n", dir->d_name);
          	}
        	closedir(d);
    	}
  		else
	  	{
      		perror ("Error: Couldn't open the directory. Please check if the directory is a valid one.");
	  	}
    	return 1;
   	}

/* Shell command ls implementated without using execvp.*/

int shell_mls(char **args)
 	{

     	DIR *dp;
     	struct dirent *dirp;
     	if(args[2] == NULL)
     	{
			dp=opendir(".");
     	}
     	else
     	{
			dp= opendir(args[2]);
     	}
     	if (dp == NULL)
         	printf("Can’t open %s", args[2]);
     	else
		  {
      	  while ((dirp = readdir(dp)) != NULL){
          		struct stat fileStat;
          		stat(dirp->d_name,&fileStat);   
          		puts(dirp->d_name);
          		printf("—————————\n");
          		printf("File Size: \t\t%lld bytes\n",fileStat.st_size);
          		printf("Number of Links: \t%d\n",fileStat.st_nlink);
          		printf("File inode: \t\t%llu\n",fileStat.st_ino);
  
          		printf("File Permissions: \t");
          		printf( (S_ISDIR(fileStat.st_mode)) ? "d" : "-");
          		printf( (fileStat.st_mode & S_IRUSR) ? "r" : "-");
          		printf( (fileStat.st_mode & S_IWUSR) ? "w" : "-");
          		printf( (fileStat.st_mode & S_IXUSR) ? "x" : "-");
          		printf( (fileStat.st_mode & S_IRGRP) ? "r" : "-");
          		printf( (fileStat.st_mode & S_IWGRP) ? "w" : "-");
          		printf( (fileStat.st_mode & S_IXGRP) ? "x" : "-");
          		printf( (fileStat.st_mode & S_IROTH) ? "r" : "-");
          		printf( (fileStat.st_mode & S_IWOTH) ? "w" : "-");
          		printf( (fileStat.st_mode & S_IXOTH) ? "x" : "-");
          		printf("\n\n");
  
          		printf("The file %s a symbolic link\n", (S_ISLNK(fileStat.st_mode)) ? "is" : "is not");
 	  		  }     
     	} 		
     	return 1;
 	}

/* Background process ("&") execution function. Here we execute the command and return immediately, not blocking until the command finishes.*/

int shell_bg(char **args)
  	{
    	pid_t pid, wpid;
    	int status;

    	pid = fork();
    	if (pid == 0) {
      		// Child process
      		if (execlp(args[0],args[0],args[1],NULL) == -1) {
        		perror("Error: execvp command execution error.");
      	}

      	exit(EXIT_FAILURE);
    	} else if (pid < 0) {
      		// Error forking
      		perror("Error: forking error.");
    	} else {
      		// do not wait
      
    	}

    	return 1;
  	}  

/* Input redirection ("<").  In this function the input is redirected from a file eg. apple.txt and passed to the commmand. */

int shell_ipredirect(char **args, int argCount)
  	{
    	pid_t pid, wpid;
    	int status;
    	extern int errno ;
    	FILE *inputFilename;
    

    	pid = fork();
    	if (pid == 0) {
      		// Child process
      		inputFilename= freopen(args[argCount-1],"r",stdin);
      		if(inputFilename==NULL)
    		{
      			fprintf(stderr, "Error opening file: %s\n", strerror( errno ));
    		}
      
      		char * argv1[] = { args[0], args[argCount-1], NULL };
      		if (execvp(argv1[0], argv1) == -1) {
        		perror("Error: execvp command execution error.");
      		}
      		fclose(inputFilename);
      		exit(EXIT_FAILURE);
    	} else if (pid < 0) {
      		// Error forking
      		perror("Error: forking error.");
    	} else {
      		// Parent process
      		do {
        		wpid = waitpid(pid, &status, WUNTRACED);
      		} 
      		while (!WIFEXITED(status) && !WIFSIGNALED(status));
    	}
  

    	return 1;
  	}

/* Output redirection (">"). In this function the output from the command is redirected to a file eg. foo.txt*/

int shell_opredirect(char **args, int argCount)
  	{
    	pid_t pid, wpid;
    	int status;
    	extern int errno ;
    	FILE *outputFilename;

    	pid = fork();
    	if (pid == 0) {
      		// Child process
      		outputFilename=freopen(args[argCount-1], "w",stdout);
      		if(outputFilename==NULL)
      		{
        		fprintf(stderr, "Error opening file: %s\n", strerror( errno ));
      		}
      		char * argv1[] = { args[0], args[1], NULL };
      		if (execvp(argv1[0], argv1) == -1) {
        		perror("Error: execvp execution error.");
      		}
      		fclose(outputFilename);
      		exit(EXIT_FAILURE);
    	} else if (pid < 0) {
      		// Error forking
      		perror("Error: forking error.");
    	} else {
      		// Parent process
      		do {
        		wpid = waitpid(pid, &status, WUNTRACED);
      		}
      		while (!WIFEXITED(status) && !WIFSIGNALED(status));
    	}
  

    	return 1;
  	}

/* Pipe implementation ("|"). In this function a command, with or without arguments, whose output is piped to the input of another command.*/

#define READ_END 0
#define WRITE_END 1

int shell_pipe(char **args, int argCount)
    {
      pid_t pid;
      int status;
      int pfds[2];

      pipe(pfds);
      pid=fork();

      if(pid < 0)
      {
          perror("Error: First Child forking error.");
      }
      else if(pid==0)
      {
          // first child
          close(pfds[READ_END]);
          dup2(pfds[WRITE_END], STDOUT_FILENO);
          char * argv1[] = { args[0], args[1], NULL };
          if (execvp(argv1[0], argv1) == -1) 
          {
              perror("Error: First child execvp execution error.");
          }

      }
      else
      {
          
          pid=fork();

          if(pid < 0)
          {
              perror("Error: Second Child forking error.");
          }
          else if(pid==0)
          {
              // second child
              close(pfds[WRITE_END]);
              dup2(pfds[READ_END], STDIN_FILENO);
              char * argv1[] = { args[argCount-1], NULL };
              if (execvp(argv1[0], argv1) == -1) 
              {
                perror("Error: Second child execvp execution error.");
              }
          }
          else
          {
              close(pfds[READ_END]);
              close(pfds[WRITE_END]);
              wait(&status);
          }
      }

      return 1;
    }

/* This function is called for any other shell commands apart from the once implemented explicitly. */

int shell_launch(char **args)
  	{
    	pid_t pid, wpid;
    	int status;
		  pid = fork();
    	if (pid == 0) {
      		// Child process
      		if (execvp(args[0], args) == -1) {
        		perror("Error: execvp command execution error.");
      		}
      		exit(EXIT_FAILURE);
    	} else if (pid < 0) {
      		// Error while forking
      		perror("Error: forking error.");
    	} else {
      		// Parent process
      		do  {
        			wpid = waitpid(pid, &status, WUNTRACED);
      			} 
      		while (!WIFEXITED(status) && !WIFSIGNALED(status));
    	}
  

    	return 1;
  	}

/* In this function we have different function calls based on their type: builtin internal command, background process, pipe, 
ip or op redirection etc, other command. */

int shell_execute(char **args, int argCount)
  	{
    	int i;

    	if (args[0] == NULL) {
      		// An empty command was entered.
      		printf("Error: Enter valid command\n");
      		return 1;
    	}
  

    	for (i = 0; i < shell_num_builtins(); i++) {
      		if (strcmp(args[0], builtin_str[i]) == 0) {
        		return (*builtin_func[i])(args);
      		}
    	}

    	if(argCount==1)
    	{
        	return shell_launch(args); // command with no arguments.
    	}
    	else if(*args[argCount-1] =='&') //background process check
    	{
        	return shell_bg(args);
    	}
    	else if(*args[argCount-2] =='<') //input redirection process check
    	{
        	return shell_ipredirect(args,argCount);
    	}
    	else if(*args[argCount-2] =='>') //output redirection process check
    	{
        	return shell_opredirect(args,argCount);
    	}
    	else if(*args[argCount-2] =='|') //pipe process check
    	{
        	return shell_pipe(args,argCount);
    	}

    	return shell_launch(args); // other command check & commands with one or more arguments.
  	}

#define shell_RL_BUFSIZE 1024  //buffer size defined
  
/* In this function we read a line of input from stdin and return the line from stdin. */

char *shell_read_line(void)
  	{
    	int bufsize = shell_RL_BUFSIZE;
    	int position = 0;
    	char *buffer = malloc(sizeof(char) * bufsize);
    	int c;
  
    	if (!buffer) {
      		fprintf(stderr, "Error: allocation error\n");
      		exit(EXIT_FAILURE);
    	}
  

    	while (1) {
      		// Read a character
      		c = getchar();
 
      		// If we hit End Of File, replace it with a null character and return.
      		if (c == EOF || c == '\n') {
        		buffer[position] = '\0';
        		return buffer;
      		} else {
        		buffer[position] = c;
      		}
      		position++;
  
      		// If we have exceeded the buffer, reallocate.
      		if (position >= bufsize) {
        		bufsize += shell_RL_BUFSIZE;
        		buffer = realloc(buffer, bufsize);
        		if (!buffer) {
          			fprintf(stderr, "Error: allocation error\n");
          			exit(EXIT_FAILURE);
        		}
      		}
    	}
  	}

#define shell_TOK_BUFSIZE 64
#define shell_TOK_DELIM " \t\r\n\a"

/* In this function we split a line into tokens. We pass the line as parameter and return array of tokens */

char **shell_split_line(char *line, int *argCount)
  	{
    	int bufsize = shell_TOK_BUFSIZE, position=0; 
    	*argCount = 0;
    	char **tokens = malloc(bufsize * sizeof(char*));
    	char *token;
  

    	if (!tokens) {
      		fprintf(stderr, "Error: allocation error\n");
      		exit(EXIT_FAILURE);
    	}
  
    	token = strtok(line, shell_TOK_DELIM);
    	while (token != NULL) {
      		tokens[position] = token;
      		position++;
  
      		if (position >= bufsize) {
        		bufsize += shell_TOK_BUFSIZE;
        		tokens = realloc(tokens, bufsize * sizeof(char*));
        		if (!tokens) {
          			fprintf(stderr, "Error: allocation error\n");
          			exit(EXIT_FAILURE);
        		}
      		}
  
      		token = strtok(NULL, shell_TOK_DELIM);
    	}
    	tokens[position] = NULL;
    	*argCount=position;
 
    	return tokens;
  	} 

/* Main entry point function. Here argv contains argument vector. We read all the input and parse it pass it to respective
function based on the execute functionality. */

int main(int argc, char **argv,char * envp[])
  	{

    	char *line;
    	char **args;
    	int status;
    	char cwd[1024];
    	printf("Welcome to Pallavi Shell Program.\n");
    	int argCount;

    	do {
      			getcwd(cwd, sizeof(cwd));
      			printf("%s >", cwd);

      			line = shell_read_line();
      			args = shell_split_line(line, &argCount);
      			status = shell_execute(args,argCount);

      			free(line);
      			free(args);
    		} 
    	while (status);
  
    	return EXIT_SUCCESS;
  	}  
