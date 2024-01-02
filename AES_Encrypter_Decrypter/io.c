/**
    @file io.c
    @author Khush Bawal
*/

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "io.h"
#include "field.h"

byte* readBinaryFile(char const *filename, int *size){

    FILE* inputStream = fopen(filename, "rb");
    byte* st = (byte*) malloc (NUMBER_SIXTEEN * sizeof(byte));
    int capacity = NUMBER_SIXTEEN;
    // while(fread(st + (*size), NUMBER_SIXTEEN, 1, inputStream) == 1){
    //     // printf("%d\n", *size);
    //     (*size) += NUMBER_SIXTEEN;
    //     if ((*size) > 0) {
    //         st = (byte*) realloc (st, ((*size) + NUMBER_SIXTEEN) * sizeof(byte));
    //     }
    // }
    while(fread(st + (*size), 1, 1, inputStream) == 1){
        // printf("%d\n", *size);
        (*size) += 1;
        if ((*size) > capacity) {
            capacity += NUMBER_SIXTEEN;
            st = (byte*) realloc (st, (capacity) * sizeof(byte));
        }
    }
    fclose(inputStream);
    return st;
}

void writeBinaryFile(char const *filename, byte* data, int size){

    FILE* outputStream = fopen(filename, "wb");
    fwrite(data, sizeof(byte), size, outputStream);
    fclose(outputStream);
}
