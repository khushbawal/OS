/**
    @file field.c
    @author Khush Bawal
*/

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "field.h"

byte fieldAdd( byte a, byte b ) {
    return a ^ b;
}

byte fieldSub( byte a, byte b ) {
    return a ^ b;
}

byte fieldMul( byte a, byte b ) {
    byte x = 0x01;
    int count = 0;
    unsigned short bitArray[BBITS];
    while (count < BBITS){
        if ((b & x) != 0x00) {
            bitArray[count] = (unsigned short)a << count;
        }
        else {
            bitArray[count] = 0;
        }
        x = x << 1;
        count++;
    }
    count = 0;
    unsigned short y;
    while (count < BBITS - 1) {
        y = bitArray[count] ^ bitArray[count + 1];
        bitArray[count + 1] = y;
        count++;
    }
    // printf("%x\n", bitArray[count]);
    unsigned short reducer = REDUCER;
    int rightShiftCount = 0;
    unsigned short bigVal = BITMASK;
    while ((bigVal & bitArray[count]) == 0) {
        rightShiftCount++;
        bigVal = bigVal >> 1;
        // printf("RightShilf LOOP- %x %x\n", bitArray[count], bigVal);
        if (rightShiftCount > BBITS) {
            return (byte) bitArray[count];
        }
    }
    rightShiftCount++;
    int leftShiftVal = BBITS - rightShiftCount;
    // printf("LeftShilfValue - %d\n", leftShiftVal);
    reducer =  reducer << (leftShiftVal);
    y = bitArray[count];
    leftShiftVal++;
    // printf("### - %x %x %x\n", y, reducer, bigVal);
    while (leftShiftVal != 0){
        if ((y & bigVal) != 0) {
            y = y ^ reducer;
        }
        bigVal = bigVal >> 1;
        reducer = reducer >> 1;
        leftShiftVal--;
        // printf("##%x %x %x\n", y, reducer, bigVal);
    }
    // printf("Final - %x\n", y);
    byte returningByte = (byte) y;
    return returningByte;
}
