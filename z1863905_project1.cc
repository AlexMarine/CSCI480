/*
*   Programmer: Alex Marine
*   Instructor: Jie Zhou
*   Course:     CSCI 480
*   Due Date:   9/9/21
*
*   Content:
*     This program goes through multiple different '/proc' files on Hopper/Turing and
*     answers questions that can be found within these files. It will read /proc/sys/
*     kernel/ostype, /proc/sys/kernel/hostname, /proc/sys/kernel/oslrease, /proc/sys/
*     kernel/version, /proc/cpuinfo, /proc/uptime, /proc/stat, and /proc/swaps. Then,
*     the program will sort through the data held within each file, store the data that
*     is necessary to answer the questions prompted, then use that stored data to 
*     answer the given question.
*/

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

int main()
{
  std::cout << "A: Questions about turing's OS:" << std::endl;

  std::string ostype; //string to hold text in ostype file
  std::ifstream OSTypeFile("/proc/sys/kernel/ostype"); //open the file that contains the ostype of turing
  if (OSTypeFile.is_open())
  {
    //Get eachline in the file and put it in the string (there's only one)
    while (getline(OSTypeFile, ostype))
    {
      //Output the ostype of turing
      std::cout << "1. The type of turing's OS is " << ostype << "." << std::endl;
    }
  }
  else
  {
    //Could not open file so output error
    std::cerr << "ERROR: Could not open file /proc/sys/kernel/ostype" << std::endl;
  }
  OSTypeFile.close();

  std::string hostname; //string to hold text in hostname file
  std::ifstream HostNameFile("/proc/sys/kernel/hostname"); //open the file that contains the hostname of turing
  if (HostNameFile.is_open())
  {
    //Get eachline in the file and put it in the string (there's only one)
    while (getline(HostNameFile, hostname))
    {
      //Output the hostname of turing
      std::cout << "2. The host name of turing is " << hostname << "." << std::endl;
    }
  }
  else
  {
    //Could not open file so output error
    std::cerr << "ERROR: Could not open file /proc/sys/kernel/hostname" << std::endl;
  }
  HostNameFile.close();

  std::string osrelease; //string to hold the text in osrelease file
  std::ifstream OSReleaseFile("/proc/sys/kernel/osrelease"); //open the file that contains the osrelease of turing
  if (OSReleaseFile.is_open())
  {
    //Get eachline in the file and put it in the string(there's only one)
    while (getline(OSReleaseFile, osrelease))
    {
      //Output the osrelease of turing
      std::cout << "3. The OS release of turing is " << osrelease << "." << std::endl;
    }
  }
  else
  {
    //Could not open file so output error
    std::cerr << "ERROR: Could not open file /proc/sys/kernel/osrelease" << std::endl;
  }
  OSReleaseFile.close();

  std::string version; //string to hold the text in version file
  std::ifstream VersionFile("/proc/sys/kernel/version"); //open the file that contains the version of turing
  if (VersionFile.is_open())
  {
    //Get eachline in the file and put it in the string (there's only one)
    while (getline(VersionFile, version))
    {
      //Outpit the version of turing
      std::cout << "4. This version of turing is " << version << "." << std::endl << std::endl;
    }
  }
  else
  {
    //Could not open file so output error
    std::cerr << "ERROR: Could not open file /proc/sys/kernel/version" << std::endl;
  }
  VersionFile.close();

  std::cout << "B. Questions about turing's processor:" << std::endl;

  std::string cpuinfo1; //string to hold the text in cpuinfo file
  std::ifstream CPUInfoFile1("/proc/cpuinfo"); //open the file that contains the cpuinfo of turing
  int processorCount = 0; //variable used to count processors
  int idCount = 0;
  int hold = -1;
  if (CPUInfoFile1.is_open())
  {
    //Get eachline in the file and put it in the string
    while (getline(CPUInfoFile1, cpuinfo1))
    {
      //If the line is empty, that means there is another processors, so add to the count
      if (cpuinfo1.empty())
      {
        processorCount++;
      }
      
      //If the line contains the physical id
      if (cpuinfo1.find("physical id") != std::string::npos)
      {
	//Cut off everything except the number of the physical id
	cpuinfo1 = cpuinfo1.substr(cpuinfo1.find(":") + 2);
	
	//Convert the string to int so its easier to compare
        int id = std::stoi(cpuinfo1);

	//if the placeholder id doesn't matche the number of the actual id
	if (hold != id)
	{
	  //Add one to the count of physical ids
          idCount++;
	}

	//set the placeholder the the number of the id
	hold = id;
      }
    }

    //Output the number of processors that turing has
    std::cout << "1. Turing has " << processorCount << " processors." << std::endl;

    //Output the number of physical multi-core chips turing has
    std::cout << "2. Turing has " << idCount << " physical multi-core chips." << std::endl;
  }
  else
  {
    //Could not open file so output error
    std::cerr << "ERROR: Could not open file /proc/cpuinfo" << std::endl;
  }
  CPUInfoFile1.close();

  std::string uptime; //string to hold the text in uptime file
  std::ifstream UptimeFile("/proc/uptime"); //open the file that contains the uptime info
  if (UptimeFile.is_open())
  {
    //get eachline in the file and put it in the string (there's only one)
    while (getline(UptimeFile, uptime))
    {
      //Cut out everything after the space as we only need the first number
      std::string strTime = uptime.substr(0, uptime.find(" ", 0));

      //Output the time turing has been up in seconds
      std::cout << "3. Turing has been open for " << strTime << " seconds." << std::endl;

      //convert the string to int so we can perform arithmetic on it
      int uptimeTime =  std::stoi(strTime);

      int uptimeDays = uptimeTime / (24 * 3600); //Convert the seconds to days
      uptimeTime = uptimeTime % (24 * 3600);
      int uptimeHours = uptimeTime / 3600; //Take what's left and convert it to hours
      uptimeTime %= 3600;
      int uptimeMinutes = uptimeTime / 60; //Take what's left and convert it to minutes
      uptimeTime %= 60;
      int uptimeSeconds = uptimeTime; //Everything left is the seconds

      //Output the time turing has been up in days, hours, minutes, and seconds
      std::cout << "4. That time is equivalent to " << uptimeDays << " days, " << uptimeHours << " hours, " << 
	      uptimeMinutes << " minutes, " << uptimeSeconds << " seconds." << std::endl << std::endl;
    }
  }
  else
  {
    //Could not open file so output error
    std::cerr << "ERROR: Could not open file /proc/uptime" << std::endl;
  }
  UptimeFile.close();

  std::cout << "C. Questions about processor 0:" << std::endl;

  std::string cpuinfo2; //string to hold the the text in the cpuinfo file
  std::ifstream CPUInfoFile2("/proc/cpuinfo"); //open the file that contains the cpuinfo

  bool trigger = false; //set to true when we hit processor 0
  std::string processor = "-1"; //the number of the current processor

  if (CPUInfoFile2.is_open())
  {
    //Get eachline in the file and put it in the string
    while (getline(CPUInfoFile2, cpuinfo2))
    {
      //if we are on a line that gives the processor number
      if (cpuinfo2.find("processor") != std::string::npos)
      {
	//set the processor number to 0
        processor = cpuinfo2.substr(cpuinfo2.find(":") + 2);
      }

      //if we hit processor 0, set the trigger to true
      if (processor == "0")
      {
        trigger = true;
      }

      //If the trigger is set to true, enter the if loop
      if (trigger == true)
      {
        //If a line is empty, we've passed processor 0 and can exit the while loop
        if (cpuinfo2.empty())
        {
          break;
        }

        //If a line contains "vendor_id"
        if (cpuinfo2.find("vendor_id") != std::string::npos)
        {
	  //Output who the vendor of processor 0 is (cut out everythign before the ":")
          std::cout << "1. The vendor of processor 0 is" << cpuinfo2.substr(cpuinfo2.find(":") + 1) << "." << std::endl;
        }

        //If a line contains "model name:
        if (cpuinfo2.find("model name") != std::string::npos)
        {
	  //Output what the model name of processor 0 is (cut out everything before the ":")
          std::cout << "2. The model name of processor 0 is" << cpuinfo2.substr(cpuinfo2.find(":") + 1) << "." << std::endl;
        }

        //If a line contains "address sizes"
        if (cpuinfo2.find("address sizes") != std::string::npos)
        {
	  //String to hold everything after the ":" (both the physical and virtual addresses)
          std::string physical = cpuinfo2.substr(cpuinfo2.find(":") + 1);
	  physical = physical.substr(0, physical.find(",", 0)); //Cuts out everything after the "," in the string (the virtual address)

	  //Output the phsycial address size of processor 0
          std::cout << "3. The physical address size of processor 0 is" << physical << "." << std::endl;

	  //Output the virtual address size of processor 0 (cut out everything before the "," (the physical address))
	  std::cout << "4. The virtual address size of processor 0 is" << cpuinfo2.substr(cpuinfo2.find(",") + 1) << "." << std::endl << std::endl;;
        }
      }
    }
  }
  else
  {
    //Could not open file so output error
    std::cerr << "ERROR: Could not open file /proc/cpuinfo" << std::endl;
  }
  CPUInfoFile2.close();

  std::cout << "D. Questions about processor 5:" << std::endl;

  std::string stat; //string to hold the text in the stat file
  std::ifstream StatFile("/proc/stat"); //open the file that contains the stat info
  std::string strUser, strSystem, strIdle; //strings to hold the values from the file
  int intUser, intSystem, intIdle; //integers to hold the converted strings

  if (StatFile.is_open())
  {
    //Get eachline in the file and store it in the string
    while (getline(StatFile, stat))
    {
      //If a line contains "cpu5"
      if (stat.find("cpu5") != std::string::npos)
      {
	std::string arr[10]; //create an array to hold each value in the line
	int i = 0; //counter to store each word in slot in the array
	std::stringstream ssin(stat); //split each value in the line

	//While there are still values in the line
	while (ssin.good() && i < 10)
	{
          //Put each value at a different spot in the array
          ssin >> arr[i];
	  i++;
	}

	strUser = arr[1]; //store the value in the array to the string
        intUser = std::stoi(strUser); //convert the string to an int
	intUser *= .01; //convert the time from miliseconds to seconds

	strSystem = arr[3]; //store the value in the array to the string
	intSystem = std::stoi(strSystem); //convert the string to an int
	intSystem *= .01; //convert the time from miliseconds to seconds

	strIdle = arr[4]; //store the value in the array to the string
	intIdle = std::stoi(strIdle); //convert the string to an int
	intIdle *= .01; //convert the time from miliseconds to seconds

	//Output the time processor 5 has been in user mode
        std::cout << "1. Processor 5 has been in user mode for " << intUser << " seconds." << std::endl;

        //Output the time processor 5 has been in system mode
        std::cout << "2. Processor 5 has been in system mode for " << intSystem << " seconds." << std::endl;

	//Output the time processor 5 has been idle
        std::cout << "3. Processor 5 has been idle for " << intIdle << " seconds." << std::endl;

        int idleDays = intIdle / (24 * 3600); //Convert the seconds to days
        intIdle = intIdle % (24 * 3600);
        int idleHours = intIdle / 3600; //Take what's left and convert it to hours
        intIdle %= 3600;
        int idleMinutes = intIdle / 60; //Take what's left and convert it to minutes
        intIdle %= 60;
        int idleSeconds = intIdle; //Everything left is the seconds

	//Output the time processor 5 has been idle in days, hours, minutes, and seconds
        std::cout << "4. That time is equivalent to " << idleDays << " days, " << idleHours << " hours, " << 
	      idleMinutes << " minutes, " << idleSeconds << " seconds." << std::endl << std::endl;
      }
    }
  }
  else
  {
    //The file could not open so output error
    std::cerr << "ERROR: Could not open file /proc/stat" << std::endl;
  }
  StatFile.close();

  std::string swaps; //string to hold the text in the swaps file
  std::ifstream SwapsFile("/proc/swaps"); //open the file that contains the swaps info

  if (SwapsFile.is_open())
  {
    //Get eachline the file and store it in the string
    while (getline(SwapsFile, swaps))
    {
      //if the line does not contain "Filename"
      if (swaps.find("/dev/md2") != std::string::npos)
      {
        std::string strSize = swaps.substr(swaps.find("partition") + 10); //Start the string at the size of the swap device.
	strSize = strSize.substr(0, strSize.find("\t", 0)); //Cut out everything after the size

	//Convert the string to double so we can perform arithmetic on it
	double size = std::stoi(strSize);
	size *= 0.000976562; //Convert it from kb to mb

	//Output the size of turing;s swap device in mb
        std::cout << "E. The size of turing's swap device in MB is " << size << "." << std::endl;
      }
    }
  }
  else
  {
    //The file could not open so output error
    std::cerr << "ERROR: Could not open file /proc/swaps" << std::endl;
  }
  SwapsFile.close();

  return 0;
}
