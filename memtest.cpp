#include <stdio.h>
#include <stdlib.h>

using namespace std;

int main(int argc, char** argv)
{
	// Check the command line arguments
	if(argc < 2)
	{
		fprintf(stderr, "USAGE: %s <MEMSIZE_KBYTES> <PAGE_SIZE>\n", argv[0]);
		exit(-1);
	}

	//Variables
	int memSize = argv[1];
	int pageSize = argv[2];

	//Start Counting
	auto start = chrono::high_resolution_clock::now();

	//DO SOMETHING...

	//Stop Counting
	auto end = chrono::high_resolution_clock::now();

	int microseconds = chrono::duration_cast<chrono::microseconds>(end - start).count();
	double seconds = microseconds / 1E6;
	cout << "Average Turnaround Time: " << seconds << " seconds" << endl;

	return 0;
}
