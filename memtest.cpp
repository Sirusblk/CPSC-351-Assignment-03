// Project 3
// By Maira, Tanner, and David

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
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
void readInFile(string);

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

	if (pageSize == 300)
		pageSize += 100;

	//DO SOMETHING...
	readInFile("in1.txt");

	return 0;
}

void readInFile(string input_file)
{
	ifstream my_file;
	my_file.open(input_file);

	int num_processes;
	int temp;

	my_file >> num_processes;
	process * my_processes = new process[num_processes];

	for (int i = 0; i < num_processes; ++i)
	{
		my_file >> my_processes[i].num;
		my_file >> my_processes[i].time_start;
		my_file >> my_processes[i].time_end;
		my_file >> my_processes[i].num_block;

		for (int j = 0; j < my_processes[i].num_block; ++j)
		{
			my_file >> temp;
			my_processes[i].block_size.push_back(temp);
		}
	}

	my_file.close();

	//DEBUG
	/*
	cout << "Start Debug...\n";
	for (int i = 0; i < num_processes; ++i)
	{
		cout << my_processes[i].num << " | " << my_processes[i].time_start << " | " << my_processes[i].time_end << " | " << my_processes[i].num_block << endl;
	}
	cout << "End Debug...\n";
	 */	
}
