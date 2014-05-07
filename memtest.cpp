// Project 3
// By Maira, Tanner, and David

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <list>
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
void debug_print(vector<process> &);
void printOutput(vector<process> &);

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

	if (pageSize == 300)
		pageSize += 100;

	//DO SOMETHING...
	readInFile("in1.txt", process_list);
	debug_print(process_list);

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

void printOutput(vector<process>)
{

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
