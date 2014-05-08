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

//Prototypes
void readInFile(string, vector<process> &);
void findTimes(vector<process> &, vector<int> &);
void printOutput(vector<process> &, int &, int &);
void v_print(vector<int> &);
void debug_print(vector<process> &);

int main(int argc, char** argv)
{
	// Check the command line arguments
	if(argc < 2)
	{
		fprintf(stderr, "USAGE: %s <MEMSIZE_KBYTES> <PAGE_SIZE_OPTION>\n", argv[0]);
		exit(-1);
	}

	//Variables
	int memSize = atoi(argv[1]);
	int pageSize = atoi(argv[2]) * 100;
	vector<process> process_list;
	vector<int> timeline;

	if (pageSize == 300)
		pageSize += 100;

	//DO SOMETHING...
	readInFile("in1.txt", process_list);
	findTimes(process_list, timeline);
	//debug_print(process_list);
	printOutput(process_list, memSize, pageSize);

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

void printOutput(vector<process> &process_list, int &memSize, int &pageSize)
{
	//Variables
	vector<int> timeline;
	int time_index;

	// Loop through each process
	for (int i = 0; i < process_list.size(); ++i)
	{
		if (timeline.empty() || (process_list[i].time_start > time_index))
		{
			time_index = process_list[i].time_start;
			timeline.push_back(process_list[i].num);         // Add to timeline

			if (process_list[i].time_end < end_index)
			{
				end_index = process_list[i].time_end;
				//end =
			}

			cout << "t = " << time_index << ": Process " << process_list[i].num << " arrives" << endl;
			cout << "       Input Queue: [ ";
			v_print(timeline);
			cout << "]" << endl;
		}
		else if (process_list[i].time_start == time_index)
		{
			timeline.push_back(process_list[i].num);         // Add to timeline

			cout << "       Process " << process_list[i].num << " arrives" <<endl;
			cout << "       Input Queue: [ ";
			v_print(timeline);
			cout << "]" << endl;
		}
		//Also check for process end

		//Print Memory Map
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
