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
	unsigned char *mainMem = (unsigned char *) malloc(1 << 24);

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
	int i, j;
	for(i = 0; i < numSets; i++){
		for(j = 0; j < associativity; j++){
			struct block blk = {0, 0, 0};
			cache[i][j] = blk; 
		}
	}
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
		int indexMask = (1 << indexSize) - 1; 
		int index = (currAddress >> blkOffsetSize) & indexMask;
		// Extract the tag from the address
		int targetTag = currAddress >> (blkOffsetSize + indexSize);
		

		// Search for a matching tag in the appropriate set
		int i;
		int j = 0;
		int isHit = 0;
		int notValidIndex = -1;
		for(i = 0; i < associativity; i++){
			int currTag = cache[index][i].tag;
			int validity = cache[index][i].isValid;
			if((currTag == targetTag) && (validity == 1)){
				isHit = 1;
				
				for(j = 0; j < associativity; j++){
					if(j != i){
						cache[index][j].orderUsed = cache[index][j].orderUsed + 1; 
					}
				}

			}
			if((validity == 0) && (notValidIndex == -1)){
				notValidIndex = i;
			}
		}
		// Handle miss given that a block in the set has valid = 0
		if((notValidIndex != -1) && (isHit == 0)){
			cache[index][notValidIndex].isValid = 1; 
			cache[index][notValidIndex].tag = targetTag; 

			for(i = 0; i < associativity; i++){
				if(i != notValidIndex){
					cache[index][i].orderUsed = cache[index][i].orderUsed + 1;
				} 
			}
		} // Handle miss given that all blocks in the set have valid = 1
		else if(isHit == 0){
			int minUsedCount = INT_MAX;
			int LRUIndex = 0;
			for(i = 0; i < associativity; i++){
				if(cache[index][i].orderUsed < minUsedCount){
					minUsedCount = cache[index][i].orderUsed;
					LRUIndex = i; 
				}
			}

			cache[index][LRUIndex].tag = targetTag;
			
			for(j = 0; j < associativity; j++){
				if(j != LRUIndex){
					cache[index][j].orderUsed = cache[index][j].orderUsed + 1; 
				}
			}
		}

		if(instruction_buffer[0]=='l'){    // If load
            // Print the load line in the same format as trace file
			if(isHit == 0){
				printf("load 0x%x miss %d ", currAddress, accessSize);
			}
			else{
				printf("load 0x%x hit %d ", currAddress, accessSize);
			}
			// Print memory data
			for(i = 0; i < accessSize; i++){
				printf("%x", mainMem[currAddress + i]);
			}
			printf("\n");
			
		}
        else {                              // Else store
            // Buffer to store data to be stored
            char data_buffer[16];

            // Read the data
            fscanf(myFile, "%s", &data_buffer);
			// Store in memory
			for(int i = 0; i < accessSize; i++){
				sscanf(data_buffer + 2 * i, "%2hhx", mainMem + currAddress + i);
			}

            // Print the store line in the same format as trace file
			if(isHit == 0){
				printf("store 0x%x miss\n", currAddress);
			}
			else{
				printf("store 0x%x hit\n", currAddress);
			}
		}

		
	}
	free(mainMem);
	return EXIT_SUCCESS;
}
