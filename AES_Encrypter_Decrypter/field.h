#ifndef _FIELD_H_
#define _FIELD_H_

/** Type used for our field, an unsigned byte. */
typedef unsigned char byte;

/** Number of bits in a byte. */
#define BBITS 8

/** Reducer. */
#define REDUCER 0x11B

/** 16 BitMask. */
#define BITMASK 0x8000

#endif

extern byte fieldAdd( byte a, byte b );

extern byte fieldSub( byte a, byte b );

extern byte fieldMul( byte a, byte b );
