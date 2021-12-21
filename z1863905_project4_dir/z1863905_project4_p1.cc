/*************************************************************************************************************
 *    Programmer: Alex Marine
 *    Instructor: Jie Zhou
 *    Course:     CSCI480
 *    Due Date:   10/30/2021
 *
 *    Content:
 *      Takes in user entered number of reader and writer threads. The readers print the contents of the string
 *      while the writers write to the same shared string. Only one writer can write at a time, but readers can
 *      access concurrently. Any given writer needed mutual exclusion with other writers and all readers.
 *************************************************************************************************************/

#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <unistd.h>

//global variables shared by all threads
std::string str = "All work and no play makes Jack a dull boy."; //string that will be read and written to
sem_t mutex; //protect the updating of readcount
sem_t wrt; //for mutex among writers and the first or last readers that enter the critical section
int readcount = 0; //how many readers are currently reading the object

/****************************************************************************************************************
 *  Content:
 *    In a loop until the string is empty, the writer calls wait on wrt semaphore, then writes to the string. 
 *    Next, it signals the wrt semaphore and sleeps for a second before exiting.
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
    //print out a message saying that it is writing
    printf("writer %ld is writing ...\n", tid);

    //make wrt semaphore wait
    if (sem_wait(&wrt) == -1)
    {
      perror("ERROR: sem_wait() failed\n");
      exit(-1);
    }

    //if the string is not empty, pop off the last character in the string
    if (!str.empty())
    {
      str.pop_back();
    }

    //signal wrt semaphore
    if (sem_post(&wrt) == -1)
    {
      perror("ERROR: sem_post() failed\n");
      exit(-1);
    }

    //sleep for 1 second
    sleep(1);
  }

  //exit the writer thread
  printf("writer %ld is exiting ...\n", tid);
  return 0;
}

/****************************************************************************************************************
 *  Content:
 *    In a loop until the string is empty, reader calls wait on the mutex semaphore, then increments the
 *    readcount. If readcount is 1, it will call wait on the wrt semaphore. The reader will then exit the
 *    critical section by calling signal on mutex. After doing reading, enter the critical section again by
 *    calling wait on mutex. Decrement mutex, and if it is 0, signal wrt. Exit the critical section and sleep
 *    for one second.
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
    //enter the critical section
    if (sem_wait(&mutex) == -1)
    {
      perror("ERROR: sem_wait() failed\n");
      exit(-1);
    }

    //increment readcount
    readcount++;
    printf("read_count increments to: %d\n", readcount);

    //if readcount is 1, make the wrt semaphore wait
    if (readcount == 1)
    {
      if (sem_wait(&wrt) == -1)
      {
        perror("ERROR: sem_wait() failed\n");
	exit(-1);
      }
    }

    //exit the critical section
    if (sem_post(&mutex) == -1)
    {
      perror("ERROR: sem_post() failed\n");
      exit(-1);
    }

    //print out the message saying it is reading
    printf("reader %ld is reading ... content : %s\n", tid, str.c_str());

    //enter the critical section
    if (sem_wait(&mutex) == -1)
    {
      perror("ERROR: sem_wait() failed\n");
      exit(-1);
    }

    //decrement readcount
    readcount--;
    printf("read_count decrements to: %d\n", readcount);

    //if readcount is 0, signal the wrt semaphore
    if (readcount == 0)
    {
      if (sem_post(&wrt) == -1)
      {
        perror("ERROR: sem_post() failed\n");
	exit(-1);
      }
    }

    //exit the critical section
    if (sem_post(&mutex) == -1)
    {
      perror("ERROR: sem_post() failed\n");
      exit(-1);
    }

    //sleep for one second
    sleep(1);
  }

   //exit the reader thread
  printf("reader %ld is exiting ...\n", tid);
  return 0;
}

/****************************************************************************************************************
 *  Content:
 *    Get the command line arguments entered by the user for the number of readers and writers. Initialize the
 *    semaphores, with both being set to 0. Create the reader and writer threads. Wait for the reader threads to 
 *    finish, then waiting for the writer threads to finish. Cleanup the threads and semaphores and exit.
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
  if (sem_init(&mutex, 0, 1) == -1)
  {
    perror("ERROR: sem_init failed\n");
    exit(-1);
  }
  if (sem_init(&wrt, 0, 1) == -1)
  {
    perror("ERROR: sem_init failed\n");
    exit(-1);
  }

  //declare reader and writer threads
  pthread_t readerThreads[numRead];
  pthread_t writerThreads[numWrite];
  
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
  if (sem_destroy(&mutex) == -1)
  {
    perror("ERROR: sem_destory() failed\n");
  }
  if (sem_destroy(&wrt) == -1)
  {
    perror("ERROR: sem_destroy() failed\n");
  }

  //everything is cleaned up, so end program
  printf("Resources cleaned up.\n");
  pthread_exit(NULL);
}
