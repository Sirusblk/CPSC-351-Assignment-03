// Project 3
// By Maira, Tanner, and David

#include <stdio.h>
#include <stdlib.h>

struct process
{
	int num;
	int time_start;
	int time_end;
	int num_block;
	int block_size[];
};

int main(int argc, char** argv)
{
	// Check the command line arguments
	if(argc < 2)
	{
		fprintf(stderr, "USAGE: %s <MEMSIZE_KBYTES> <PAGE_SIZE>\n", argv[0]);
		exit(-1);
	}

	//Variables
	int memSize = atoi(argv[1]);
	int pageSize = atoi(argv[2]);

	//DO SOMETHING...


	return 0;
}
