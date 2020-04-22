#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(int argc, char *argv[]){
    //Load virtual address
    char * ptr;
    int virtAddr = strtol(arg[2], &ptr, 16);
    //Load page file
    FILE *pgFile = fopen(argv[1], "r");
    //Load fields from the heading of the page file
    int virtAddrSize = 0;
    int pageSize = 0;
    fscanf(pgFile, "%d %d", &virtAddrSize, &pageSize);
    fscanf(pgFile, "%*[^\n]\n");
    //Compute the page offset size and the VPN size
    int pgOffsetSize = log(pageSize) / log(2);
    //Create and use bit masks to extract page offset and VPN 
    int pgOffsetMask = ~((1 << pgOffsetSize) - 1);
    pgOffsetMask = (pgOffsetMask ^ (1 << (pgOffsetSize - 1)));
    int pgOffsetBits = pgOffsetMask & virtAddr; 
    int vpnMask = virtAddr >> (virtAddrSize - pgOffsetSize - 1);
    int vpnBits = virtAddr & vpnMask; 
    int ppn = -1; 
    for(int i = 0; i < vpnBits + 1; i++){
        fscanf(pgFile, "%d", &ppn);
    }
    //"Concatenate" the PPN and page offset 
    ppn = ppn << pgOffsetSize; 
    ppn = ppn | pgOffsetBits; 
    //Print the result
    if(ppn == -1){
        printf("PAGEFAULT/n");
    }
    else{
        printf("%.4x", ppn);
        printf("\n");
    }

    return EXIT_SUCCESS;
}