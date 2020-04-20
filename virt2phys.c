#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]){
    //Load arguments as pointers
    pgFile_ptr = argv[1];
    int *virtAddr_ptr = argv[2];
    int virtAddr = *virtAddr_ptr;
    //Load page file
    FILE *pgFile = fopen(*pgFile_ptr, "r");
    //Load fields from the heading of the page file
    int virtAddrSize = 0;
    int pageSize = 0;
    fscanf(pgFile, "%d %d", &virtAddrSize, &pageSize);
    //Compute the page offset size and the VPN size
    int pgOffsetSize = log(pageSize) / log(2);
    //Extract the page offset from the virtual address
    /*
    TODO: Extract method
    */
    int *pgOffsetBits = malloc(sizeof(int) * pgOffsetSize);
    int i;
    for(i = 0; i < pgOffsetSize - 1, i++){
        int mask = 1 << i;
        int masked = virtAddr & mask; 
        int addrBit = masked >> i; 
        pgOffsetBits[i] = addrBit; 
    }
    //Extract the VPN from the virtual address
    /*
    TODO: Extract method
    */
    int *vpnBits = malloc(sizeof(int) * (virtAddrSize - pgOffsetSize));
    for(i = pgOffsetSize; i < virtAddrSize - 1; i++){
        int mask = 1 << i;
        int masked = virtAddr & mask; 
        int addrBit = masked >> i; 
        vpnBits[i] = addrBit; 
    }


    return EXIT_SUCCESS;
}