/************************************************************************************************************
 *  Programmer: Alex Marine
 *  Instructor: Jie Zhou
 *  Course:     CSCI480
 *  Due Date:   11/19/21
 *
 *  Content:
 *    The PhysicalMemory class allows us to swap in a new entry to the frame if there is an open spot, of if
 *    a spot needs to be created by replacement. It prints out the frame and what is inside each frame.
 ***********************************************************************************************************/

#include <string.h>
#include <vector>
#include <iostream>

class PhysicalMemory
{
  public:
    PhysicalMemory(int memorySize, int algorithmIn); //initialize all the data and data structures in the class (e.g. set clock to 0). 
                                                     //the second argument is for testing different page replacement algorithms if you will implement the bonus credits for LRU.
    void access(int frameID); //access the frame. if LRU update the time stamp and time list
    void printMemory() const; //print out the physical memory layout
    int swapIn(std::string item); //returns the frame ID of the item just swapped in. if FIFO uodate the timestamp and time list
  private:
    std::vector<std::string> memoryList; //the physical memory
    std::vector<int> timeList; //data structure for the replacement algorithm
    int currentTimeIndex; //current clock for timestamp
    int algorithmFlag; //indicate if using FIFO or LRU
    int getNextAvailableFrame(); //get a frame, either available or via replacement
    bool isFull(); //check if the memory is full
};

/****************************************************************************************
 *  Initialize all the data and data structures in the class (e.g. set clock to 0). The
 *  second argument is for testing different page replacement algorithms if you will
 *  implement the bonus credit for LRU.
 ***************************************************************************************/
PhysicalMemory::PhysicalMemory(int memorySize, int algorithmIn)
{
  //set currentTimeIndex to 0 and the alforithmFlag to whatever was sent in
  currentTimeIndex = 0;
  algorithmFlag = algorithmIn;

  //resize the vectors to the size of memory
  memoryList.resize(memorySize);
  timeList.resize(memorySize);

  //for every entry in the vectors
  for(int i = 0; i < memorySize; i++)
  {
    //set memoryList to empty and timeList to 0
    memoryList[i] = "";
    timeList[i] = 0;
  }
}

/****************************************************************************************
 *  Access the frame. If LRU, update the time stamp and time list
 ***************************************************************************************/
void PhysicalMemory::access(int frameID)
{
  //output the frameID and what is at that memory location
  std::cout << "Physical: Accessed frameID: " << frameID << " contains: " << memoryList[frameID]  << std::endl << std::endl;
}

/****************************************************************************************
 * Print out the physical memory layout.
 ***************************************************************************************/
void PhysicalMemory::printMemory() const
{
  //output the physical memory layout
  std::cout << "Physical Memory Layout:" << std::endl;

  //for every element in the memoryList
  for(long unsigned int i = 0; i < memoryList.size(); i++)
  {
    //output the frame and what is at that memory location
    std::cout << "Frame: " << i << " contains: " << memoryList[i] << std::endl;
  }

  //line break
  std::cout << std::endl;
}

/****************************************************************************************
 *  Needs to get the next available frame by calling getNextAvailableFrame(), and update
 *  the memory list. For FIFO, it also updates the time stamp and the time list (because
 *  that's how the program keeps track of when the content of the frame is in). Return
 *  the frame ID.
 ***************************************************************************************/
int PhysicalMemory::swapIn(std::string item)
{
  //output what is swapped in
  std::cout << "Physical: Swap In: " << item << std::endl;

  //call method to get the next frame and store the index that we will use
  int index = getNextAvailableFrame();

  //update the memory list
  memoryList[index] = item;

  //increment the current time index
  currentTimeIndex++;

  //update the time list
  timeList[index] = currentTimeIndex;

  //return the frameID
  return index;
}

/****************************************************************************************
 *  Returns the frame that will be used by PageTable to store info when there is a page
 *  fault. It can be a currently empty frame, or a victim as the result of replacement.
 *  If there is empty frame, then return the available frame in a sequence starting from
 *  0. Otherwise, uses FIFO (or LRU for bonus credit).
 ***************************************************************************************/
int PhysicalMemory::getNextAvailableFrame()
{
  //initialize variables
  int index = -1; //used to find the index of the next available frame
  int hold = -1;
  bool full = isFull(); //true if memory is full, false if not

  //if memory is not full, find first open space in memory
  if (full == false)
  {
    //for every entry in the timeList
    for(long unsigned int i = 0; i < timeList.size(); i++)
    {
      //if the entry is empty, there is an open spot in memory
      if (timeList[i] == 0)
      {
        //set our index to the index of the open space in memory
        index = i;
        //since we found an open spot, we can break out of the for loop
        break;
      }
    }
  }
  //else, memory is full so replace whichever one got in first
  else
  {
    //set to the first entry in timeList, will compare to other entries to find the oldest one
    hold = timeList[0];

    //for every entry in the timeList
    for(long unsigned int i = 0; i < timeList.size(); i++)
    {
      //find the smalled entry in the timeList
      if (hold >= timeList[i])
      {
	//set that new smallest entry to our index
        index = i;
	hold = timeList[i];
      }
    }
  }

  //return the index
  return index;
}

/****************************************************************************************
 *  Can be implemented using the initial value of timeList or memoryList. For example, 
 *  the timeList can be initialized to 0. So if a time list has a status of 0, it means
 *  that physical frame is empty (initial status).
 ***************************************************************************************/
bool PhysicalMemory::isFull()
{
  //assume that memory is full
  bool full = true;

  //for every entry in the timeList
  for(long unsigned int i = 0; i < timeList.size(); i++)
  {
    //if the entry is empty, there is an open spot in memory
    if (timeList[i] == 0)
    {
      //set full to false since the memory is not yet full
      full = false;
    }
  }

  //return whether the frame is full or not
  return full;
}
