#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]){
    //Load arguments 
    pgFile_ptr = argv[0];
    virtAdd_ptr = argv[1];
    //Load page file
    FILE * pgFile = fopen(*pgFile_ptr, "r+");
    //Load page file heading fields
    int addBits = 0;
    int pageSize = 0;
    fscanf(pgFile, "%d %d", addBits, pageSize);

}