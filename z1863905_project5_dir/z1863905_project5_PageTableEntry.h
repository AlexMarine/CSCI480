/***************************************************************************
 *  Programmer: Alex Marine
 *  Instructor: Jie Zhou
 *  Course:     CSCI480
 *  Due Date:   11/19/21
 *
 *  Content:
 *    Struct that holds the data for each element in the page table.
 **************************************************************************/

//Represents each element in the page table
struct PageTableEntry
{
  //variable declaration
  bool valid; //is the entry in use or not
  int physicalMemoryIndex; //the index of the entry in physical memory
};
