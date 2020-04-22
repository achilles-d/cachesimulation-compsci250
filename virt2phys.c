#include <stdio.h>
#include <stdlib.h>

int log2(int n) { int r=0;
    while (n >>= 1) 
        r++; 
    return r;
}

void decToBinary(int n) 
{ 
    // array to store binary number 
    int binaryNum[32]; 
  
    // counter for binary array 
    int i = 0; 
    while (n > 0) { 
  
        // storing remainder in binary array 
        binaryNum[i] = n % 2; 
        n = n / 2; 
        i++; 
    } 
  
    // printing binary array in reverse order 
    for (int j = i - 1; j >= 0; j--) 
        printf("%d", binaryNum[j]); 

    printf("\n");
} 

int main(int argc, char *argv[]){
    //Load virtual address
    int virtAddr; 
    sscanf(argv[2], "%x", &virtAddr);
    //Load page file
    FILE *pgFile = fopen(argv[1], "r");
    //Load fields from the heading of the page file
    int virtAddrSize = 0;
    int pageSize = 0;
    fscanf(pgFile, "%d %d", &virtAddrSize, &pageSize);
    fscanf(pgFile, "%*[^\n]\n");
    /*
    printf("Page size:\n");
    printf("%d\n", log2(pageSize));
    */
    //Compute the page offset size and the VPN size
    int pgOffsetSize = log2(pageSize);
    //Create and use bit masks to extract page offset and VPN 
    int pgOffsetMask = ((1 << pgOffsetSize) - 1);
    
    int pgOffsetBits = virtAddr & pgOffsetMask; 
    int vpnBits = virtAddr >> pgOffsetSize;
    /*
    printf("Virtual address:\n");
    decToBinary(virtAddr);
    printf("Page offset bits:\n");
    decToBinary(pgOffsetBits);
    printf("VPN:\n");
    decToBinary(vpnBits);
    */
    int ppn = 0; 
    //Loop to correct location and read PPN
    int i; 
    for(i = 0; i < vpnBits + 1; i++){
        if(i == (vpnBits)){
            fscanf(pgFile, "%d", &ppn);
        }
        else{
            fscanf(pgFile, "%*[^\n]\n");
        }
    }
    fclose(pgFile);
    //Print the result
    if(ppn == -1)
        printf("PAGEFAULT\n");
    else{
        //"Concatenate" the PPN and page offset 
        ppn = ppn << pgOffsetSize; 
        ppn = ppn | pgOffsetBits;
        printf("%x\n", ppn);
    }

    return EXIT_SUCCESS;
}

