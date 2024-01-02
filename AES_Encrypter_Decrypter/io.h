#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "field.h"

/* The number 16. */
#define NUMBER_SIXTEEN 16

extern byte* readBinaryFile( char const *filename, int *size );

extern void writeBinaryFile( char const *filename, byte *data, int size );
