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
	int time_start;
	int time_end;
	int num_block;
	vector<int> block_size;
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
void addToMemoryMap(vector<memoryBlock> &, int &, int &, process &);
void printMemoryMap(vector<memoryBlock> &, int &, int &);

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
		my_file >> temp_proc.time_start;
		my_file >> temp_proc.time_end;
		my_file >> temp_proc.num_block;

		// Create vector at run time for block sizes
		vector<int> block_cont(temp_proc.num_block);
		int block;

		// Store values in a vector
		for (int j = 0; j < temp_proc.num_block; ++j)
		{
			my_file >> block;
			block_cont.push_back(block);
		}

		// Swap vector into temporary process
		temp_proc.block_size.swap(block_cont);
		
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
			if (process_list[i].time_start == timeline[j])
			{
				b_found = true;
			}

			if (process_list[i].time_start + process_list[i].time_end == timeline[j])
			{
				e_found = true;
			}
		}

		if (!b_found)
		{
			timeline.push_back(process_list[i].time_start);
		}

		if (!e_found)
		{
			timeline.push_back(process_list[i].time_start + process_list[i].time_end);
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
	vector<memoryBlock> memoryMap;

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
			if (process_list[j].time_start == timeline[i])
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
			else if (process_list[j].time_start + process_list[j].time_end == timeline[i])
			{
				if (!first_line)
				{
					cout << "       ";
				}

				cout << "Process " << process_list[j].num << " completes" << endl;
				first_line = false;

				// Change Memory Map...
			}
		}

		while (input_q.size() != 0)
		{
			// Dequeue and add to MM
			temp = process_list[input_q.front() - 1];
			cout << "       MM moves Process " << temp.num << " to memory" << endl;

			// Add to Memory Map...
			int index = input_q.front();
			temp = process_list[index];
			addToMemoryMap(memoryMap, memSize, pageSize, temp);

			input_q.erase(input_q.begin());
			
			cout << "       Input Queue: [ ";
			v_print(input_q);
			cout << "]" << endl;

			// Display Memory Map...
			printMemoryMap(memoryMap, memSize, pageSize);
		}

		cout << endl;
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

void addToMemoryMap(vector<memoryBlock> &memoryMap, int &memSize, int &pageSize, process &input)
{
	int count = 0;
	int blockCount = 0;

	while (count < (memSize / pageSize))
	{
		if(memoryMap[count].blockFree)
		{
			memoryMap[count].processNum = input.num_block;
			memoryMap[count].pageNum = input.block_size[blockCount];
		}

		if (blockCount == input.block_size.size())
		{
			count++;
		}

		blockCount++;
	}
}

void printMemoryMap(vector<memoryBlock> &memoryMap, int &memSize, int &pageSize)
{
	cout << "       Memory Map: " << endl;
	int count = 0;
	int pageNum = 0;
	int processNum = 0;

	while (!memoryMap[count].blockFree && count < (memSize / pageSize))
	{
		cout << "            " << memoryMap[count].blockStart << "-" << memoryMap[count].blockEnd << ": Process " 
			 << memoryMap[count].processNum << " , Page " << memoryMap[count].pageNum << endl;

		count++;
	}

	if (count < (memSize / pageSize) - 1)
	{
		cout << "            " << count * pageSize << "-" << memSize - 1 << ": Free frame(s)" <<endl;
	}
	
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
		cout << process_list[i].num << " | " << process_list[i].time_start << " | " << process_list[i].time_end << " | " << process_list[i].num_block << " | ";
		for (int j = 0; j < process_list[i].block_size.size(); ++j)
		{
			cout << process_list[i].block_size[j] << " ";
		}

		cout << endl;
	}
	cout << "End Debug...\n";
}
