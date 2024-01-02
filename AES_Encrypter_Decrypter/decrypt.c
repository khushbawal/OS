/**
    @file decrypt.c
    @author Khush Bawal
*/

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "io.h"
#include "field.h"
#include "aes.h"

int main (int argc, char* argv[]) {
    if (argc != WORD_SIZE) {
        fprintf(stderr, "usage: decrypt <key-file> <input-file> <output-file>\n");
        exit(1);
    }
    for (int i = 0; i < WORD_SIZE - 1; i++) {
        FILE* inputStream = fopen(argv[i], "rb");
        if (!inputStream) {
            fprintf(stderr, "Can't open file: %s\n", argv[i]);
            exit(1);
        }
        fclose(inputStream);
    }

    // Read Key.
    int* keySize = (int*) malloc (sizeof(int));
    *keySize = 0;
    byte* keyRead = readBinaryFile(argv[1], keySize);
    // printf("%s\n%d\n", keyRead, *keySize);
    if ((*keySize) != BLOCK_SIZE) {
        fprintf(stderr, "Bad key file: %s\n", argv[1]);
        exit(1);
    }
    byte key[BLOCK_SIZE] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    for (int i = 0; i < BLOCK_SIZE; i++) {
        key[i] = *(keyRead + i);
    }

    //Read Data.
    int* size = (int*) malloc (sizeof(int));
    *size = 0;
    byte* dataRead = readBinaryFile(argv[NUMBER_TWO], size);
    if ((*size) % NUMBER_SIXTEEN != 0) {
        fprintf(stderr, "Bad ciphertext file length: %s\n", argv[NUMBER_TWO]);
        exit(1);
    }
    byte* data = (byte*) malloc ((*size) * sizeof(byte));
    for (int i = 0; i < *size; i++) {
        // *(data + i) = *(dataRead + i);
        data[i] = dataRead[i];
    }
    int timeSize = (*size) / BLOCK_SIZE;
    for (int j = 0; j < timeSize; j++) {
        //Encrypt Block only affects the starter 16 and doesn't affect 17..
        decryptBlock(&(data[j * BLOCK_SIZE]), key);
    }
    byte* dataWrite = data;
    writeBinaryFile(argv[WORD_SIZE - 1], dataWrite, *size);

    return EXIT_SUCCESS;
}
