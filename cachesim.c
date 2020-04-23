#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>

struct block 
{
	int tag;
	int isValid; 
	int orderUsed;
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
	int setUsedCount[numSets];
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
		// Extract the tag from the address
		int targetTag = currAddress >> (blkOffsetSize + indexSize);
		

		// Search for a matching tag in the appropriate set
		int i;
		int isHit = 0;
		int notValidIndex = -1;
		for(i = 0; i < associativity; i++){
			int currTag = cache[index][i].tag;
			int isValid = cache[index][i].isValid;
			if((currTag == targetTag) && (isValid == 0)){
				isHit = 1;
			}
			if((isValid == 0) && (notValidIndex == -1)){
				notValidIndex = i;
			}
		}
		// Handle miss given that a block in the set has valid = 0
		if((notValidIndex != -1) && (isHit == 0)){
			cache[index][notValidIndex].isValid = 1; 
		}
		// Handle miss given that all blocks in the set have valid = 1
		else if(isHit == 0){
			int minUsedCount = INT_MAX;
			int LRUIndex = -1;
			for(i = 0; i < associativity; i++){
				if(cache[index][i].orderUsed < minUsedCount){
					minUsedCount = cache[index][i].orderUsed;
					LRUIndex = i; 
				}
			}
			setUsedCount[index] = setUsedCount[index] + 1;
			cache[index][LRUIndex].tag = targetTag;
			cache[index][LRUIndex].orderUsed = setUsedCount[index]; 
		}

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
