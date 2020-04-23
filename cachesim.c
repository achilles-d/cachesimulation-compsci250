#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

struct block 
{
	int tag;
	int isValid; 
};

int log2(int n) { 
	int r = 0;
    while (n >>= 1) 
        r++; 
    return r;
}

int main (int argc, char* argv[]) {
	int cacheSize, associativity, blockSize;

    // Buffer to store instruction (i.e. "load" or "store")
	char instruction_buffer[6];

    // Open the trace file in read mode
	FILE* myFile = fopen(argv[1], "r");

    // Read in the command line arguments
	sscanf(argv[2], "%d", &cacheSize);
	sscanf(argv[3], "%d", &associativity);
	sscanf(argv[4], "%d", &blockSize);

	// Initialize cache and allocate space in heap for main memory
	int numSets = (cacheSize << 10) / (blockSize * associativity);
	struct block cache[numSets][associativity]; 
	unsigned char *mem = malloc(1024 * 1024 * 16 * sizeof(char));

    // Keep reading the instruction until end of file
	while(fscanf(myFile,"%s", &instruction_buffer)!=EOF) {
		int currAddress, accessSize;

        // Read the address and access size info
		fscanf(myFile, "%x", &currAddress);
		fscanf(myFile, "%d", &accessSize);

		// Extract the block offset from the address
		int blkOffsetSize = log2(blockSize);
		int blkOffsetMask = (1 << blkOffsetSize) - 1;
		int blkOffset = currAddress & blkOffsetMask; 
		// Extract the index from the address
		int indexSize = log2(numSets);
		int indexMask = (1 << numSets) - 1; 
		int index = (currAddress >> blkOffsetSize) & indexMask;
		//Extract the tag from the address
		int tag = currAddress >> (blkOffsetSize + indexSize);
		
		if(instruction_buffer[0]=='l'){    // If load
            // Print the load line in the same format as trace file
			printf("load 0x%x %d\n", currAddress, accessSize);
		}
        else {                              // Else store
            // Buffer to store data to be stored
            char data_buffer[16];

            // Read the data
            fscanf(myFile, "%s", &data_buffer);

            // Print the store line in the same format as trace file
            printf("store 0x%x %d %s\n", currAddress, accessSize, data_buffer);
		}
	}

	free(mem);

	return EXIT_SUCCESS;
}
