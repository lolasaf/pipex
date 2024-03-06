/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   notes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wel-safa <wel-safa@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/18 21:49:07 by wel-safa          #+#    #+#             */
/*   Updated: 2024/03/06 16:05:10 by wel-safa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
This is for testing and learning purposes
*/
#define _GNU_SOURCE
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <sys/wait.h>
#include <errno.h>

/*******************************fork() function********************************/

/*fork function*/
/*int	main(int argc, char **argv)
{
	int id = fork();
	// from this line onwards everything is going to be executed twice
	// child id is passed as a return to the old process
	// child return id from fork is then set to zero (Actual ID remains)
	//printf("Hello World! from id: %d\n", id);
	if (id == 0)
		printf("Hello World! from child process\n");
	else
		printf("Hello World! from main process\n");
	return (0);
}*/

/*creating 4 processes*/
/*int main(int argc, char **argv)
{
	int id = fork();
	// everything is twice.
	fork();
	// everything below is 4 times.
	printf("Hello World\n");
	return (0);
}*/

/*creating 3 processes*/
/*int main(int argc, char **argv)
{
	int id = fork();
	// everything is twice.
	if (id)
		fork();
	// everything below is 3 times.
	printf("Hello World\n");
	return (0);
}*/

/*******************************wait() function********************************/

/*int	main(void)
{
	int	id = fork();
	int	n;

	if (id == 0) // child process
		n = 1;
	else // main process
		n = 6;

	// Code below might not execute at the order we want i.e. printing from 1 to 10
	// That is where the wait() function comes in.
	// We will tell the main process to wait for a child process and then executes
	if (id) // main process
		wait(0); // #include <sys/wait.h>

	int i;
	for (i = n; i < n + 5; i++)
	{
		printf("%d ", i);
		fflush(stdout);
	}
	if (id)
		printf("\n");
	return (0);
}
*/

/********************************process IDs***********************************/

/*Every process has a process ID. 
getpid() returns current ID and getppid() returns parent ID
child process terminates before parent process.

if the parent process terminates first, it is assigned a new parent ID
for instance using sleep(1) func call on child process to sleep for 1 second
parent process terminates and child process is assigned a new ID and it is
considered a zombie process, if you try to kill it with a sink kill command
it will not actually free the memory
 
In all fork functions, you need to wait for child process to finish before
parent process, or there will be memory leaks
*/
/*int	main(void)
{
	int id = fork();
	// if (id == 0)
	// 	sleep(1);
	printf("current ID: %d, parent ID: %d \n", getpid(), getppid());
	//if (id != 0)
	//	wait(NULL);
	// no need for if function for wait, this works too:
	//wait(NULL);
	// wait checks if there is a child process to wait for
	// if there is it will wait, if there isn't it will return -1
	int res = wait(NULL);
	if (res == -1)
		printf("No children to wait for\n");
	else
		printf("%d finished execution\n", res); // you will get ID you waited for
	return (0);
}
*/

/***************************Calling fork multiple times************************/

/*Process IDs in multiple forks
  _________
 | id1 = x |
 | id2 = z |_________
 |_________|         |
      |              |
  ____|____      ____|____
 | id1 = 0 |    | id1 = x |
x| id2 = y |   z| id2 = 0 |
 |_________|    |_________|
      |
  ____|____
 | id1 = 0 |
y| id2 = 0 |
 |_________|

*/
/*int	main(void)
{
	int id1 = fork();
	int id2 = fork();
	if (id1 == 0)
	{
		if (id2 == 0)
			printf("We are process y\n");
		else
			printf("We are process x\n");
	}
	else
	{
		if (id2 == 0)
			printf("We are process z\n");
		else
			printf("We are the parent process!\n");
	}
	// wait (NULL);
	// wait function waits for only one child process even if there are two
	// in this case the parent process has two child processes
	// so we have to do something else
	while(wait(NULL) != -1 || errno != ECHILD)
		printf("Waited for a child to finish\n"); // we should get this 3 times
	// <errno.h> | this indicates the kind of error because wait could
	// return -1 in other situations too
	// this while loop is the solution
	return (0);
}*/

/****************************communicating with pipes**************************/

/*A pipe is an in-memory file. 
It has a buffer saved in memory and you can write and read to it
If we want to open a pipe, we use function pipe that takes an array of two ints.
it saves in the array two integers.
the two ints are the file discreptors (fd) for the pipe it opened.
fd is a key to access the file. 

We have two fds because a pipe has two ends and each end is stored
fd[0] - read end
fd[1] - write end
*/
/*int	main(int argc, char **argv)
{
	int fd[2];
	if (pipe(fd) == -1)
	{
		printf("an error occurred with opening the pipe\n");
		return (1);
	}
	
	// pipe function will save the fds it opens when it creates the pipe
	// returns -1 on error, and 0 on success

	int id = fork();
	if (id == -1)
		return(4); // error occured with forking - should check
	
	// the fds get copied over here
	// Aside from the fd ints get copied over.
	// the fds get assigned to the new child process, so they get INHERITED
	
	// if you close the fd in a process, it remains open in the other process
	// they are independent of each other
	// but the fds are referring to the same pipe in this case.

	// Now we want to create a program that asks the user for a number
	// and sends it to the other process

	if (id == 0) // CHILD PROCESS
	{
		close(fd[0]); // we don't read inside this process for this pipe
		int x;
		printf("Input a number: ");
		scanf("%d", &x);
		if (write(fd[1], &x, sizeof(int)) == -1)
			return (2);
		close(fd[1]);
	}
	else // PARENT PROCESS
	{
		close(fd[1]); // because here we do not write in this process
		// it is common procedure to close one end in each process
		// whichever is not used
		int y;
		if (read(fd[0], &y, sizeof(int)) == -1)
			return (3);
		close(fd[0]);
		printf("Got from child process %d\n", y);
	}

	// You need to check for errors for the read and write functions
	return (0);
}
*/

/****************************execute another program***************************/

/*execl function
l stands for list*/
/*int main(int argc, char **argv)
{
	// execl will execute a program
	// it takes arguments in the following order
	// (path to program, path to program, argvs, .., NULL)
	
	execl("/usr/bin/ping", "/usr/bin/ping", "google.com", NULL);

	// all exec functions behave in a way where
	// it does not create another process, finishes it, and comes back to main
	// but it takes the new ping process and puts it on top of it
	// overwriting everything it had before
	// we are no longer in this program
	// we are in the ping program and that is it!!
	// for instance the below code will NOT execute
	// unless execl function did not work properly and failed to run
	printf("Ping finished executing\n");
	return (0);
}
*/

/*execlp function
p stands for path
what if I want to just pass ping as arg and not the entire path
this can be done with execlp*/
/*int main(int argc, char **argv)
{
	execlp("ping", "ping", "google.com", NULL);
	return (0);
}
*/

/*execvp function
v stands for vector*/
/*int main(void)
{
	char *arr[] = {
		"ping",
		"google.com",
		NULL
	};
	execvp("ping", arr);
	return (0);
}*/

/*execvpe function
e stands for environment
environment is a set of variables that you can pass with another array*/
/*int main(void)
{
	char *arr[] = {
		"ping",
		"google.com",
		NULL
	};
	char *env[] = {
		"TEST=environment variable",
		NULL
	};
	execvpe("ping", arr, env);

	int err = errno; // in case execvpe fails
	// this will return error code
	// meaning of the code is in errno.h
	return (0);
}*/

/*****************************environment variables****************************/

/*environment variable
examples of Variables PATH, API_KEY, or others set..
You can declare environment variable on terminal e.g.
	>>API_KEY=test
then running
	>>echo $API_KEY
	>>test
we get test, then running
	>>./a.out
The a.out program here is using getenv, to get the env variable and 
then prints it, but here we won't see anything printed on stdout
because the env variable we created was not passed to the child process
i.e. our ./a.out program

Alternatively you could use the following syntax
	>>API_KEY=test ./a.out
This will set a variable API_KEY and send it to ./a.out
but if you then run
	>>echo $API_KEY
you get nothing, because you did not really create this variable in the
terminal process. You just created it for the c program.

To set variable in terminal and have it available in the executable
you can use export (available in bash)
>>export API_KEY=test
*/
/*int main(int argc, char **argv)
{
	//char *env_variable1 = getenv("PATH");
	//printf("The value of our environment variable is %s\n", env_variable1);
	
	char *env_variable2 = getenv("API_KEY");
	if (env_variable2 == 0)
	{
		printf("No environment variable was found\n");
		return (1);
	}
	printf("The value of our environment variable is %s\n", env_variable2);
	
	return (0);
}
*/

/*envp variable in main
it is an array containing all environment variables and is NULL terminated
		envp |__| |__| |__| |__| |__| ... |NULL|
The array element are of type (char *), ending with a null pointer, and 
these strings are formatted as follows
"ENV_VARIABLE_NAME=path", e.g.
"PATH=/usr/lib..."
"JAVA_HOME=/usr/.."
Below function prints ALL environment variables
*/
/*int main(int argc, char **argv, char **envp)
{
	int i = 0;
	char *env_variable = envp[i];
	while (env_variable != NULL)
	{
		printf("%s\n", env_variable);
		i++;
		env_variable = envp[i];
	}
	return (0);
}*/

/*parcing envp
to get a specific venv variable value*/
/*int main(int argc, char **argv, char **envp)
{
	int i = 0;
	char *env_variable = envp[i];
	while (env_variable != NULL)
	{
		//printf("%s\n", env_variable);
		char *key = strtok(env_variable, "=");
		char *value = strtok(NULL, "="); // it uses the same string
		// error checking is important to add
		if (strcmp(key, "API_KEY") == 0)
			printf("API_KEY value is %s\n\n", value);
		i++;
		env_variable = envp[i];
	}
	return (0);
}
*/

/**********************sending an array through a pipe*************************/

/*2 processes
1) Child process generates random numbers and sends them to the parent
2) Parent sums up numbers and prints the result*/
/*int main(void)
{
	int fd[2];
	if (pipe(fd) == -1)
		return (1); // error piping

	int pid = fork();
	if (pid == -1)
		return (2); // error forking
	if (pid == 0) // child processes
	{
		close(fd[0]);
		
		int n, i;
		int arr[10];
		
		srand(time(NULL));
		n = rand() % 10 + 1; 
		// generates random num n = number of randomly generated numbers from 1 to 9
		
		printf("Generated: ");
		for (i = 0; i < n; i++)
		{
			arr[i] = rand() % 11; // generate random numbers from 0 to 10
			printf("%d ", arr[i]);
		}
		printf("\n");
		
		// WE SHOULD SEND NUMBER OF ELEMENTS FIRST BEFORE ARRAY
		if(write(fd[1], &n, sizeof(int)) < 0)
			return (4);
		printf("Sent n = %d\n", n);
		
		if (write(fd[1], arr, sizeof(int) * n) < 0)
			return (3); // error writing
		printf("Sent array\n");
		close(fd[1]);
	}
	else // parent process
	{
		close(fd[1]);
		
		int arr[10];
		int n, i, sum = 0;
		
		if(read(fd[0], &n, sizeof(int)) < 0)
			return (5);
		printf("Received n = %d\n", n);

		if(read(fd[0], arr, sizeof(int) * n ) < 0)
			return (6);
		printf("Received array\n");
		
		close(fd[0]);
	
		for (i = 0; i < n; i++)
			sum += arr[i];
		printf("Result is: %d\n", sum);
		
		wait(NULL); // we should always wait for child process to finish
	}
	return (0);
}
*/

/**********************sending a string through a pipe*************************/

/*int main(void)
{
	int fd[2];
	if (pipe(fd) == -1)
		return (1);
	
	int pid = fork();
	if (pid < 0)
		return (2); // error

	if (pid == 0)
	{
		close(fd[0]);
		// child process
		char str[200];
		printf("Input string\n");
		fgets(str, 200, stdin);
		str[strlen(str) - 1] = '\0'; // fgets puts a new line at end so we null it
		
		int n = strlen(str) + 1;
		if (write(fd[1], &n, sizeof(int)) < 0)
			return (3);
		if (write(fd[1], str, sizeof(char) * n) < 0)
			return (4);
		close(fd[1]);
	}
	else
	{
		// parent process
		close(fd[1]); // we re not writing in parent process
		char str[200];
		int n;
		
		if(read(fd[0], &n, sizeof(int)) < 0)
			return (5);
		if (read(fd[0], str, sizeof(char) * n) < 0)
			return (6);

		printf("Received: %s\n", str);
		close(fd[0]);
		wait(NULL);
	}
	return (0);
}
*/

/**********************simulating the pipe "|" operator************************/

/* "|" is a pipe
*ping -c 5 google.com | grep rtt
ping is writing it's stdout to the pipe
the stdin of grep is set to be on the pipe

ping --stdout--> pipe --stdin--> grep		

main prcoess forks another process that will execute ping
main process forks another process that will execite grep
So we have three processes
the process executing grep and the process executing ping will communicate
via a pipe, which is also created by the main process
  _________
 | main    |   
 | process |_____________
 |_________|   |         |
      |        |         |
      |        |         |
  ____|____    |     ____|____
 |         |___|____|         |
 |  grep   |________|   ping  |
 |_________|  pipe  |_________|
            <------
          flow of info
*/
/*int	main(int argc, char **argv)
{
	int fd[2];
	// fd[0] -- read end of the pipe
	// fd[1] -- write end of the pipe
	if (pipe(fd) == - 1)
		return (1);

	int pid1 = fork(); // creates chid process
	if (pid1 < 0)
		return (2);

	if (pid1 == 0)	
	{
		// child process 1 (ping)
		dup2(fd[1], STDOUT_FILENO); // STDOUT_FILENO = 1
		// dup2 duplicates fd_1 into fd_2, 
		// closing fd_2 and making it open on same file
		// fd[1] -- write end of pipe -- is duplicated to STDOUT.
		// now instead of ping going to STDOUT, it routes to the pipe.

		close(fd[0]); // we do not read from pipe in grep process.
		close(fd[1]); // this remains open after dup2 and is no longer needed.
		
		execlp("ping", "ping", "-c", "5", "google.com", NULL);
		// execlp replaces process by ping process 
		// so code after this line will not continue executing
	}
	
	// main process
	int pid2 = fork(); // creates a second child process
	if (pid2 < 0)
		return (3);

	if (pid2 == 0)
	{
		// child process 2
		dup2(fd[0], STDIN_FILENO); // STDIN_FILENO = 0
		// this duplicates fd[0] into STDIN.
		// now grep instead of taking input from STDIN,
		// it will take it from the pipe
		close(fd[0]);
		close(fd[1]);
		execlp("grep", "grep", "rtt", NULL);
	}
	
	close(fd[0]);
	close(fd[1]);
	// all fds are inhereted into child processes
	// we closed them in the child process
	// but in the main process they remain open
	// grep will not stop executing until all read fds are closed
	// so if we do not close fd[0] grep will be in infinite loop
	// and it is good practise to close all fds anyway so
	// also fd[1] should be closed
	
	waitpid(pid1, NULL, 0); 
	// wait for particular process, second arg is status, then options
	waitpid(pid2, NULL, 0);
	return(0);
}
*/
