// Project 3
// By Maira, Tanner, and David

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include <vector>
#include <queue>
using namespace std;

struct process
{
	int num;
	int timeStart;
	int timeEnd;
	int numBlocks;
	vector<int> blockSizes;
};

struct memoryBlock
{
	int blockStart;
	int blockEnd;
	int processNum;
	int pageNum;
	bool blockFree;
};

//Prototypes
void readInFile(string, vector<process> &);
void findTimes(vector<process> &, vector<int> &);
void printOutput(vector<process> &, vector<int> &, int &, int &);
void v_print(vector<int> &);
void debug_print(vector<process> &);
void createMemoryMap(int [], int &, int &);
void createMemoryMap(vector<memoryBlock> &, int &, int &);
bool addToMemoryMap(vector<memoryBlock> &, int &, int &, process &);
int checkIfSpace(vector<memoryBlock> &, int &, int &, int &);
void printMemoryMap(vector<memoryBlock> &, int &, int &);
void removeFromMemoryMap(vector<memoryBlock> &, int &, int &, int &);

int main(int argc, char** argv)
{
	// Check the command line arguments
	int memSize = 0;
	int pageSize= 0;

	if(argc == 3)
	{
		// ./memtest <memSize> <pageSize>
		memSize = atoi(argv[1]);
		pageSize = atoi(argv[2]) * 100;
	}
	else
	{
		cout<< "Memory Size(Kbytes): ";
		cin>> memSize;
		cout<< "Page Size (1:100, 2:200, 3:400): ";
		cin>> pageSize;
		pageSize = pageSize * 100;
	}

	//Variables
	vector<process> process_list;
	vector<int> timeline;

	if (pageSize == 300)
		pageSize += 100;

	//DO SOMETHING...
	readInFile("in1.txt", process_list);
	findTimes(process_list, timeline);
	printOutput(process_list, timeline, memSize, pageSize);

	return 0;
}

void readInFile(string input_file, vector<process> &process_list)
{
	ifstream my_file;
	my_file.open(input_file);

	int num_processes;
	process temp_proc;

	my_file >> num_processes;

	for (int i = 0; i < num_processes; ++i)
	{
		// Grab all atributes
		my_file >> temp_proc.num;
		my_file >> temp_proc.timeStart;
		my_file >> temp_proc.timeEnd;
		my_file >> temp_proc.numBlocks;

		// Create vector at run time for block sizes
		vector<int> tempBlockSizes;
		int block;

		// Store values in a vector
		for (int j = 0; j < temp_proc.numBlocks; ++j)
		{
			my_file >> block;
			tempBlockSizes.push_back(block);
		}

		// Swap vector into temporary process
		temp_proc.blockSizes = tempBlockSizes;
		
		// Push back temporary process into process vector
		process_list.push_back(temp_proc);
	}

	my_file.close();
}

void findTimes(vector<process> &process_list, vector<int> &timeline)
{
	// Variables
	bool b_found;
	bool e_found;

	// Add to Timeline
	for (int i = 0; i < process_list.size(); ++i)
	{
		b_found = false;
		e_found = false;

		for (int j = 0; j < timeline.size(); ++j)
		{
			if (process_list[i].timeStart == timeline[j])
			{
				b_found = true;
			}

			if (process_list[i].timeStart + process_list[i].timeEnd == timeline[j])
			{
				e_found = true;
			}
		}

		if (!b_found)
		{
			timeline.push_back(process_list[i].timeStart);
		}

		if (!e_found)
		{
			timeline.push_back(process_list[i].timeStart + process_list[i].timeEnd);
		}
	}

	sort(timeline.begin(), timeline.end());
}

void printOutput(vector<process> &process_list, vector<int> &timeline, int &memSize, int &pageSize)
{
	//Variables
	vector<int> input_q;
	bool first_line;
	process temp;
	int temp2;
	vector<memoryBlock> memoryMap;
	vector<int> processesToRemove;

	//Create Memory Map
	createMemoryMap(memoryMap, memSize, pageSize);

	// Loop through each process
	for (int i = 0; i < timeline.size(); ++i)
	{
		first_line = true;

		cout << "t = " << timeline[i] << ": ";

		// Complete functions at this time to make room
		// And add new processes!
		for (int j = 0; j < process_list.size(); ++j)
		{
			// Times sync up for start time
			if (process_list[j].timeStart == timeline[i])
			{
				input_q.push_back(process_list[j].num);

				if (!first_line)
				{
					cout << "       ";
				}

				cout << "Process " << process_list[j].num << " arrives" << endl;
				cout << "       Input Queue: [ ";
				v_print(input_q);
				cout << "]" << endl;

				first_line = false;
			}
			else if (process_list[j].timeStart + process_list[j].timeEnd == timeline[i])
			{
				if (!first_line)
				{
					cout << "       ";
				}

				cout << "Process " << process_list[j].num << " completes" << endl;
				removeFromMemoryMap(memoryMap, memSize, pageSize, process_list[j].num);
				printMemoryMap(memoryMap, memSize, pageSize);
				//processesToRemove.push_back(process_list[j].num);
				first_line = false;
			}
		}

		//Remove processes from Memory Map
		/*
		while (processesToRemove.size() != 0)
		{
			temp2 = processesToRemove.front();
			removeFromMemoryMap(memoryMap, memSize, pageSize, temp2);
			printMemoryMap(memoryMap, memSize, pageSize);
			processesToRemove.erase(processesToRemove.begin());
		}
		 */

		//Add processes to Memory Map
		while (input_q.size() != 0)
		{
			// Dequeue and add to MM
			temp = process_list[input_q.front() - 1];

			// Add to Memory Map...
			if ( addToMemoryMap(memoryMap, memSize, pageSize, temp) )
			{
				cout << "       MM moves Process " << temp.num << " to memory" << endl;

				input_q.erase(input_q.begin());
			
				cout << "       Input Queue: [ ";
				v_print(input_q);
				cout << "]" << endl;

				// Display Memory Map...
				printMemoryMap(memoryMap, memSize, pageSize);
			}
			else
			{
				cout << "\nERROR: No free space!\n\n";
				break;
			}
		}
	}
}

void createMemoryMap(vector<memoryBlock> &memoryMap, int &memSize, int &pageSize)
{
	memoryBlock temp;

	for (int i = 0; i < (memSize / pageSize); ++i)
	{
		temp.blockStart = i * pageSize;
		temp.blockEnd = ((i + 1) * pageSize) - 1;
		temp.processNum = -1;
		temp.pageNum = -1;
		temp.blockFree = true;
		memoryMap.push_back(temp);
	}
}

bool addToMemoryMap(vector<memoryBlock> &memoryMap, int &memSize, int &pageSize, process &input)
{
	int count = 0;
	int blockCount = 0;
	int section = 0;
	double blockSize;
	int intBlockSize;

	//DEBUG
	//cout << "DEBUG: Process " << input.num << ": " << input.timeStart << " - " << input.timeEnd << " | " << input.numBlocks
	//	 << ": " << input.blockSizes[0] << endl;

	int startBlock = -1;
	bool wasAdded;
	int numPage = 1;

	//Loop through number of blocks
	for (int i = 0; i < input.numBlocks; ++i)
	{
		blockSize = input.blockSizes[i] / pageSize;

		// Look ahead and see if we can combine multiple blocks together
		for (int j = i; j < input.numBlocks; ++j)
		{
			blockSize += (input.blockSizes[j] / pageSize);
		}

		intBlockSize = (int) blockSize;
		startBlock = checkIfSpace(memoryMap, memSize, pageSize, intBlockSize);

		if (startBlock == -1)
		{
			intBlockSize = (int) blockSize;
			startBlock = checkIfSpace(memoryMap, memSize, pageSize, intBlockSize);
		}
		
		/*
		cout << " == DEBUG ==\n";
		cout << "Process: " << input.num << ":" << i << endl;
		cout << "Start Here: " << startBlock << endl;
		cout << "End Here: " << startBlock + ((input.blockSizes[i] + 99) / pageSize) << endl;
		cout << endl;
		 */

		wasAdded = false;

		if (startBlock > -1)
		{
			wasAdded = true;
			
			//cout << "Space found! " << startBlock << " " << ((input.blockSizes[i] + 99) / pageSize) << endl;
			for (int j = startBlock; j < (startBlock + ((input.blockSizes[i] + 99) / pageSize)); ++j)
			{
				memoryMap[j].processNum = input.num;
				memoryMap[j].pageNum = numPage;
				memoryMap[j].blockFree = false;
				numPage++;
				//cout << "Added!\n";
			}	
		}
	}

	return wasAdded;
}

void removeFromMemoryMap(vector<memoryBlock> &memoryMap, int &memSize, int &pageSize, int &input)
{
	for (int i = 0; i < memoryMap.size(); ++i)
	{
		if (memoryMap[i].processNum == input)
		{
			memoryMap[i].blockFree = true;
		}
	}
}

int checkIfSpace(vector<memoryBlock> &memoryMap, int &memSize, int &pageSize, int &inputBlockSize)
{
	int numFree = 0;

	//cout << "DEBUG: Input Block Size - " << inputBlockSize << endl;

	for (int i = 0; i < memoryMap.size(); ++i)
	{
		if (memoryMap[i].blockFree)
		{
			numFree++;
		}
		else
		{
			numFree = 0;
		}

		//If there is room fill it up
		if (numFree == inputBlockSize)
		{
			//cout << "DEBUG: Number Free - " << numFree << " Input Size - " << inputBlockSize << " Returning - " << (i + 1 - inputBlockSize) << endl;

			return (i + 1 - inputBlockSize);
		}
	}

	return -1;
}

void printMemoryMap(vector<memoryBlock> &memoryMap, int &memSize, int &pageSize)
{
	cout << "       Memory Map: " << endl;
	int count = 0;
	int pageNum = 0;
	int processNum = 0;
	int start = 0;
	int end = 0;

	while (count < (memSize / pageSize))
	{
		if (!memoryMap[count].blockFree)
		{
			if (start != 0)
			{
				end = count - 1;
				cout << "            " << start * pageSize << "-" << ((end + 1) * pageSize) - 1 << ": Free frame(s)" <<endl;
				start = 0;
			}

			cout << "            " << memoryMap[count].blockStart << "-" << memoryMap[count].blockEnd << ": Process " 
				 << memoryMap[count].processNum << " , Page " << memoryMap[count].pageNum << endl;
		}
		else if (start == 0)
		{
			start = count;
		}

		count++;
	}

	if (start != 0)
	{
		end = count - 1;
		cout << "            " << start * pageSize << "-" << ((end + 1) * pageSize) - 1 << ": Free frame(s)" <<endl;
	}

	cout << endl;
}

void v_print(vector<int> &input_v)
{
	for (int i = 0; i < input_v.size(); ++i)
		cout << input_v[i] << " ";
}

void debug_print(vector<process> &process_list)
{
	cout << "Start Debug...\n";
	for (int i = 0; i < process_list.size(); ++i)
	{
		cout << process_list[i].num << " | " << process_list[i].timeStart << " | " << process_list[i].timeEnd << " | " << process_list[i].numBlocks << " | ";
		for (int j = 0; j < process_list[i].blockSizes.size(); ++j)
		{
			cout << process_list[i].blockSizes[j] << " ";
		}

		cout << endl;
	}
	cout << "End Debug...\n";
}
