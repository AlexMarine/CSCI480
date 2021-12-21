/************************************************************************************************
 *  Programmer: Alex Marine
 *  Instructor: Jie Zhou
 *  Course:     CSCI480
 *  Due Date:   11/19/21
 *
 *  Content:
 *    The PageTable class checks to see if the item that wants to be added to the page table is
 *    valid to join. If it is, the physical memory is accessed and the item is entered. If it is
 *    not valid, a page fault is created and the item is sent to physical memory to be swapped
 *    in. The index where the item is swapped in is set to valid, and now it is accessed in the
 *    physical memory.
 ***********************************************************************************************/

#include <string.h>
#include <vector>
#include <iostream>

class PageTable
{
  public:
    PageTable(PhysicalMemory* pmIn, int tableSize); //initialize all entries to invalid
    void reference(int pageID, std::string item); //reference a logical page, if not in memory, call pageFault()
    int getFaults() const; //return number of faults
    void printTable() const; //print the layout of the page table
    void reset(); //reset the validity flag to false, and numberoffaults to 0;
  private:
    std::vector<PageTableEntry> entryList; //the page table
    int numFaults; //will hold the total number of page faults
    PhysicalMemory* mainPhysicalMemory; //pointer to PhysicalMemory class
    void pageFault(int pageID, std::string item); //increment numFaults. need to swap in the item into physical memory by calling the swapIn() of the PhysicalMemory class
};

/****************************************************************************************
 *  The constructor takes the size of the page table that creates the entries, initialize
 *  the flags in the entries to false, and initialize number of faults to 0. It also
 *  takes a pointer to the PhysicalMemory (for calling some of its methods later).
 ***************************************************************************************/
PageTable::PageTable(PhysicalMemory* pmIn, int tableSize)
{
  //set the vector to the size of the table
  entryList.resize(tableSize);

  //for each entry in the table
  for (int i = 0; i < tableSize; i++)
  {
    //set its validity to false and the index to -1
    entryList[i].valid = false;
    entryList[i].physicalMemoryIndex = -1;
  }

  //set numFaults to 0
  numFaults = 0;

  //set our private variable to the one passed through the constructor
  mainPhysicalMemory = pmIn;
}

/****************************************************************************************
 * Checks if the corresponding entry given pageID is valid. If yes, access the entry in
 * Physical Memory (by calling its access() method), otherwise call the prive method
 * pageFault().
 ***************************************************************************************/
void PageTable::reference(int pageID, std::string item)
{
  //set bool to true if the pageID is valid
  bool pageValid = false;

  //if the pageID is already in physical memory
  if (entryList[pageID].valid == true)
  {
    //the pageID is valid
    pageValid = true;
  }
  
  //if pageID is valid, call access()
  if (pageValid == true)
  {
    mainPhysicalMemory->access(entryList[pageID].physicalMemoryIndex);
  }
  //else, call pageFault()
  else
  {
    pageFault(pageID, item);
    mainPhysicalMemory->access(entryList[pageID].physicalMemoryIndex);
  }
}

/****************************************************************************************
 *  Return the number of faults
 ***************************************************************************************/
int PageTable::getFaults() const
{
  //return numFaults;
  return numFaults;
}

/****************************************************************************************
 *  Print the layout of the page table.
 ***************************************************************************************/
void PageTable::printTable() const
{
  //output the page table
  std::cout << "Page Table Snapshot:" << std::endl;

  //for every element in the entryList
  for(long unsigned int i = 0; i < entryList.size(); i++)
  {
    //output the page index, the index of the physical memory, and whether that frame is valid or not
    std::cout << "Page Index: " << i << " : Physical Frame Index: " << entryList[i].physicalMemoryIndex
	    << " : In Use: " << std::boolalpha << entryList[i].valid << std::endl;
  }

  //print number of faults
  std::cout << "PageTable: Current number of page faults: " << numFaults << std::endl;
}

/****************************************************************************************
 *  Reset the validity flag to false, and the number of faults to 0.
 ***************************************************************************************/
void PageTable::reset()
{
  //for every entry in the table
  for (long unsigned int i = 0; i < entryList.size(); i++)
  {
    //set its validity to false
    entryList[i].valid = false;
  }

  //set numFaults to 0
  numFaults = 0;
}

/****************************************************************************************
 *  Increments the numFaults and calls a method swapIn() of PhysicalMemory to swap in the
 *  page. It then updates the related entries in the page table by making the original
 *  mapping for the frame invalid, and the new mapping valid
 ***************************************************************************************/
void PageTable::pageFault(int pageID, std::string item)
{
  //output that a page fault occured
  std::cout << "PageTable: page fault occurred" << std::endl;

  //increment numFaults
  numFaults++;

  //class the swapIn method and give item
  int index = mainPhysicalMemory->swapIn(item);

  //make the new mapping of the frame valid
  entryList[pageID].valid = true;

  //for every element in the list
  for (long unsigned int i = 0; i < entryList.size(); i++)
  {
    //if the physical memory index is equal to our new index
    if (entryList[i].physicalMemoryIndex == index)
    {
      //set the physical memory index to -1 and make it invalid, then break out of the loop
      entryList[i].physicalMemoryIndex = -1;
      entryList[i].valid = false;
      break;
    }
  }

  //set the pageID to the new index
  entryList[pageID].physicalMemoryIndex = index;
}
