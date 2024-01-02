#ifndef _AES_H_
#define _AES_H_

#include "field.h"

/** Number of bytes in an AES key or an AES block. */
#define BLOCK_SIZE 16

/** Numer of rows when a data block is arranged in a square. */
#define BLOCK_ROWS 4

/** Numer of columns when a data block is arranged in a square. */
#define BLOCK_COLS 4

/** Number of bytes in a word. */
#define WORD_SIZE 4

/** Number 2 */
#define NUMBER_TWO 2

/** Number of roudns for 128-bit AES. */
#define ROUNDS 10

#endif

extern void gFunction( byte dest[ WORD_SIZE ], byte const src[ WORD_SIZE ], int r );

extern void generateSubkeys( byte subkey[ ROUNDS + 1 ][ BLOCK_SIZE ], byte const key[ BLOCK_SIZE ] );

extern void addSubkey( byte data[ BLOCK_SIZE ], byte const key[ BLOCK_SIZE ] );

extern void blockToSquare( byte square[ BLOCK_ROWS ][ BLOCK_COLS ], byte const data[ BLOCK_SIZE ] );

extern void squareToBlock( byte data[ BLOCK_SIZE ], byte const square[ BLOCK_ROWS ][ BLOCK_COLS ] );

extern void shiftRows( byte square[ BLOCK_ROWS ][ BLOCK_COLS ] );

extern void unShiftRows( byte square[ BLOCK_ROWS ][ BLOCK_COLS ] );

extern void mixColumns( byte square[ BLOCK_ROWS ][ BLOCK_COLS ] );

extern void unMixColumns( byte square[ BLOCK_ROWS ][ BLOCK_COLS ] );

extern void encryptBlock( byte data[ BLOCK_SIZE ], byte key[ BLOCK_SIZE ] );

extern void decryptBlock( byte data[ BLOCK_SIZE ], byte key[ BLOCK_SIZE ] );
