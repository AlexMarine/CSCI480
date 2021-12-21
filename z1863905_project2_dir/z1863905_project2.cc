/*
*   Programmer: Alex Marine
*   Instructor: Jie Zhou
*   Course:     CSCI480
*   Due Date:   9/27/2021
*
*   Content:
*     Create a microshell that allows the user to execute a command such as
*     'ls', 'ps', 'ls -l', etc, enter 'q' or 'quit' to exit, repeat the prompt
*     if a return was enetered, perform output redirection with option whitespace
*     between the '>' chracter, and perform FCFS CPU scheduling, whcih defaults to
*     5 CPUs, but allows any number to be entered.
*/

#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <fcntl.h>

int main(void)
{
  char buf[1024]; //buffer to hold the user input
  pid_t pid; //process id created from fork()
  int status; //status of the process
  int cpuBurst[5]; //array to hold the random int for each CPU
  bool fcfs, outputRedirection = false; //if fcfs or '>' was entered, set to true
  int averageWaitTime = 0; //average wait time for FCFS CPU scheduling
  int numBursts = 5; //Number of CPUS for FCFS CPU scheduling
  char *greaterThan; //used to hold the value of '>' if we find it
  int holder = 0; //used to remember where in the array the '>', then add one to get everything after it

  //print the prompt
  std::cout << "myshell>";

  //get the line entered by the user and store in buf
  while (fgets(buf, 1024, stdin) != NULL)
  {
    buf[strlen(buf) - 1] = 0; //set the value in the buffer to 0
    char * comargs[1024]; //holds each token of the user input
    char * ptr; //pointer to point to value in array
    int number = 0; //counter

    //Set everything in the array to NULL so it gets reset after each user input
    for (int i = 0; i < 1024; i++)
    {
      comargs[i] = NULL;
    }

    //tokenize the user input
    comargs[0] = {strtok(buf, " ")};

    //store each token into a slot in the array
    while ((ptr = strtok(NULL, " ")) != NULL)
    {
      number++;
      comargs[number] = ptr;
    }

    //if fork returns less than 0, there was an error
    if ((pid = fork()) < 0)
    {
      std::cerr << "ERROR: fork failed" << std::endl;
      exit(1);
    }
    else if (pid == 0) //child
    {
      //if the quit statement was entered, break us out of the if statement
      if (strcmp(buf, "quit") == 0 || strcmp(buf, "q") == 0)
      {
	break;
      }

      //go through each element in comargs
      for (int i = 0; i < number; i++)
      {
	//see if any element contains a greater than symbol for output redirection
        greaterThan = strstr(comargs[i], ">");

	//if a '>' was found
        if (greaterThan != NULL)
        {
	  std::cout << "sdfasdadfgadfvsdfv" << std::endl;
          outputRedirection = true; //Set to true so we don't get an error message later
	  holder = i + 1; //the next value in the array after the '>'
        }
      }

      //if we found a '>' in the array 
      if (outputRedirection == true)
      {
	//open the file we want to write to and save the file descriptor
        int fd1 = open(comargs[holder], O_CREAT|O_WRONLY|O_TRUNC, 0644); //O_TRUNC deletes everything in a file, if it already exists

        //if fd1 is negative, there was an error
        if (fd1 < 0)
        {
          //error occured to end program
          std::cerr << "ERROR: Could not open file" << std::endl;
	  exit(1);
	}

        //creates a copy of fd1 using standard ouput file descriptor to achieve the redirection
        if (dup2(fd1, STDOUT_FILENO) == -1)
        {
	  //error occured so end program
          std::cerr << "ERROR: dup2 failed" << std::endl;
	  exit(1);
        }

	//close the original file descriptor
	if (close(fd1) == -1)
	{
	  //error occured to end program
	  std::cerr << "ERROR: close failed" << std::endl;
	  exit(1);
	}

	comargs[holder - 1] = NULL; //set the '>' in the array to NULL so execvp works
	execvp(comargs[0], comargs); //execute everything in the array before '>'
      }

      //if 'fcfs' was entered by the user
      if (strcmp(comargs[0], "fcfs") == 0)
      {
        fcfs = true;
	int totalWaitTime = 0;

	//if a number was entered after 'fcfs'
	if (comargs[1] != NULL)
	{
          //Convert the char to int
          numBursts = atoi(comargs[1]);
	}
	
	//if numBursts was entered as 0, default to 5
	if (numBursts == 0)
	{
          numBursts = 5;
	}

        std::cout << "FCFS CPU scheduling simulation with " << numBursts << " processes." << std::endl;

	//set the seed to 10
	srand(10);

	//for each number of bursts entered or default to 5
	for (int i = 0; i < numBursts; i++)
	{
          cpuBurst[i] = rand() % 100 + 1; //random value between 1 and 100
	  std::cout << "CPU burst: " << std::dec << cpuBurst[i] << " ms" << std::endl;
	}
        
	//calculate the total wait time
	int temp = 0; 
	for (int i = 1; i < numBursts; i++)
	{
          temp = i;

	  while (temp != 0)
	  {
	    //Add every value together from the highest cpu we are at down to the first one
            temp--;
            totalWaitTime += cpuBurst[temp];
	    //continue this for each cpu until we reach the last one, which is not added
	  }
	}

	//calculate the average wait time
	averageWaitTime = totalWaitTime / numBursts;
	
	std::cout << "Total waiting time in the ready queue: " << totalWaitTime << " ms" << std::endl;
        std::cout << "Average waiting time in the ready queue: " << averageWaitTime << " ms" << std::endl;
      }

      //if fcfs was entered, make it so we don't get the can't execute error
      if (fcfs == true)
      {
        break;
      }

      //execute the command that was entered
      if (outputRedirection == false)
      {
        execvp(comargs[0], comargs);
      }

      //print error if value entered was invalid
      std::cerr << "Couldn't execute: " << buf << std::endl;
      exit(127);
    }

    //parent
    if ((pid = waitpid(pid, &status, 0)) < 0)
    {
      std::cerr << "ERROR: waitpid";
      exit(1);
    }

    //if the quit statement was entered, end the program
    if (strcmp(buf, "quit") == 0 || strcmp(buf, "q") == 0)
    {
      return 0;
    }

    //print the prompt
    std::cout << "myshell>";
  }

  exit(0);
}
