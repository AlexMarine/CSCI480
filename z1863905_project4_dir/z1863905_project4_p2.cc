/*****************************************************************************************************
 *  Programmer: Alex Marine
 *  Instructor: Jie Zhou
 *  Course:     CSCI480
 *  Due Date:   10/30/2021
 *  
 *  Content:
 *    Assumes that there is many writers and one reader. One reader prints the contents of the string
 *    while the writers write to the same shared string. Only one writer can write at a time, and once
 *    a writer writes, the reader should display the string before the other writes alter it.
 ****************************************************************************************************/

#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <unistd.h>

//global variables shared by all threads
std::string str = "All work and no play makes Jack a dull boy."; //string that will be read and written to
sem_t firstSem; //protect the updating of readcount
sem_t secondSem; //for mutex among writers and the first or last readers that enter the critical section
int waitingWrites = 0; //number of readers minus number of writers to signal the rest of the waiting semaphores in writer

/****************************************************************************************************************
 *  Content:
 *    In a loop until the string is empty, writer performs wait on first semaphore before it writers to shared 
 *    string. After writing, it performs signal on the second semaphore to notify the reader that a writer 
 *    finished.
 *
 *  Parameters:
 *    *param: will hold the thread id of the semaphore
 ***************************************************************************************************************/
void *writer(void *param)
{
  //intialize variables
  long tid = (long)param; //thread id

  //loop until the string is empty
  while (!str.empty())
  {
    //make first semaphore wait
    if (sem_wait(&firstSem) == -1)
    {
      perror("ERROR: sem_wait() failed\n");
      exit(-1);
    }

    //print out a message saying that it is writing
    printf("writer %ld is writing ...\n", tid);

    //if the string is not empty, pop off the last character in the string
    if (!str.empty())
    {
      str.pop_back();
    }

    //signal second semaphore
    if (sem_post(&secondSem) == -1)
    {
      perror("ERROR: sem_post() failed\n");
      exit(-1);
    }
  }

  //exit the writer thread
  printf("writer %ld is exiting ...\n", tid);
  return 0;
}

/****************************************************************************************************************
 *  Content:
 *    Reader performs wait on the second semaphore, reads contents of shared and displays it, then peforms signal
 *    on the first semaphore, so the waiting writers can proceed. Before exiting, signal the rest of the waiting
 *    first semaphores so that they can exit.
 *
 *  Parameters:
 *    *param: will hold the thread id of the semaphore
 ***************************************************************************************************************/
void *reader(void *param)
{
  //initialize variables
  long tid = (long)param; //thread id

  while (!str.empty())
  {
    //make the second semaphore wait
    if (sem_wait(&secondSem) == -1)
    {
      perror("ERROR: sem_wait() failed\n");
      exit(-1);
    }

    //print out the message saying it is reading
    printf("reader %ld is reading ... content : %s\n", tid, str.c_str());

    //signal the first semaphore
    if (sem_post(&firstSem) == -1)
    {
      perror("ERROR: sem_post() failed\n");
      exit(-1);
    }
  }

  printf("There are still %d writers waiting on the semaphore...\n", waitingWrites);

  //signal the rest of the semaphores waiting in write
  for (int i = 0; i < waitingWrites; i++)
  {
    if (sem_post(&firstSem) == -1)
    {
      perror("ERROR: sem_post() failed\n");
      exit(-1);
    }
  }

  //exit the reader thread
  printf("reader %ld is exiting ...\n", tid);
  return 0;
}


/****************************************************************************************************************
 *  Content:
 *    Get the command line arguments entered by the user for the number of readers and writers. Initialize the
 *    semaphores, with the first being set to 1, and the second being set to 0. Create the reader and writer
 *    threads. Wait for the reader threads to finish, then waiting for the writer threads to finish. Cleanup
 *    the threads and semaphores and exit.
 *
 *  Parameters:
 *    argc: The number of arguments enterd
 *    *argv[]: Character array of all arguments entered
 ***************************************************************************************************************/
int main(int argc, char *argv[])
{
  //declare variables
  int numRead = 0; //will hold user entered number of reader threads
  int numWrite = 0; //will hold user entered number of writer threads
  int rc; //return code for threads
  long i; //used as an index

  //output header for program
  printf("*** Reader-Writer Problem Solution ***\n");

  //if there were not 3 character arguments provided, throw an error
  if (argc != 3)
  {
    perror("ERROR: input number of reader and writer threads\n");
    exit(-1);
  }
  //if the second value is less than or equal to 0, throw an error
  if ((numRead = atoi(argv[1])) <= 0)
  {
    perror("ERROR: there must be a postive number of reader threads\n");
    exit(-1);
  }
  //if the third value is less than or equal to 0, throw and error
  if ((numWrite = atoi(argv[2])) <= 0)
  {
    perror("ERROR: there must be a postive number of writer threads\n");
    exit(-1);
  }

  //output the number of reader adn writer threads
  printf("Number of reader threads: %d\n", numRead);
  printf("Number of writer threads: %d\n", numWrite);

  //initializion of semaphores
  if (sem_init(&firstSem, 0, 1) == -1)
  {
    perror("ERROR: sem_init failed\n");
    exit(-1);
  }
  if (sem_init(&secondSem, 0, 0) == -1)
  {
    perror("ERROR: sem_init failed\n");
    exit(-1);
  }

  //declare reader and writer threads
  pthread_t readerThreads[numRead];
  pthread_t writerThreads[numWrite];

  //calculate the number of threads that will be waiting in the writer
  waitingWrites = numWrite - numRead;
  
  //create reader threads
  for (i = 0; i < numRead; i++)
  {
    rc = pthread_create(&readerThreads[i], NULL, &reader, (void *)i);
    if (rc)
    {
      perror("ERROR: pthread_create() failed\n");
      exit(-1);
    }
  }

  //create writer threads
  for (i = 0; i < numWrite; i++)
  {
    rc = pthread_create(&writerThreads[i], NULL, &writer, (void *)i);
    if (rc)
    {
      perror("ERROR: pthread_create() failed\n");
      exit(-1);
    }
  }

  //Wait for reader threads to finish
  for (i = 0; i < numRead; i++)
  {
    rc = pthread_join(readerThreads[i], NULL);
    if (rc)
    {
      perror("ERROR: pthread_join() failed\n");
      exit(-1);
    }
  }

  //Wait for writer threads to finish
  for (i = 0; i < numWrite; i++)
  {
    rc = pthread_join(writerThreads[i], NULL);
    if (rc)
    {
      perror("ERROR: pthread_join() failed\n");
      exit(-1);
    }
  }

  printf("All threads are done.\n");

  //Cleanup and exit
  if (sem_destroy(&firstSem) == -1)
  {
    perror("ERROR: sem_destory() failed\n");
  }
  if (sem_destroy(&secondSem) == -1)
  {
    perror("ERROR: sem_destroy() failed\n");
  }

  //everything is cleaned up, so exit program
  printf("Resources cleaned up.\n");
  pthread_exit(NULL);
}
