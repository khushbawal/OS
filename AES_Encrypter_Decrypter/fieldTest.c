/**
  @file fieldTest.c
  @author Khush Bawal
  Unit test program for the field component.
*/

#include <stdlib.h>
#include <stdio.h>

#include "field.h"

/** Number of tests we should have, if they're all turned on. */
#define EXPECTED_TOTAL 10

/** Total number or tests we tried. */
static int totalTests = 0;

/** Number of test cases passed. */
static int passedTests = 0;

/** Macro to check the condition on a test case, keep counts of
    passed/failed tests and report a message if the test fails. */
#define TestCase( conditional ) {\
  totalTests += 1; \
  if ( conditional ) { \
    passedTests += 1; \
  } else { \
    printf( "**** Failed unit test on line %d of %s\n", __LINE__, __FILE__ );    \
  } \
}

int main()
{
  

  ////////////////////////////////////////////////////////////////////////
  // Test fieldAdd()

  {
    byte c = fieldAdd( 0x00, 0x00 );
    TestCase( c == 0x00 );
    
    c = fieldAdd( 0xF0, 0x0F );
    TestCase( c == 0xFF );
  }
  
  ////////////////////////////////////////////////////////////////////////
  // Test fieldSub()

  {
    byte c = fieldSub( 0xC2, 0xBE );
    TestCase( c == 0x7C );
  
    c = fieldSub( 0xA9, 0x47 );
    TestCase( c == 0xEE );
  }
  
  ////////////////////////////////////////////////////////////////////////
  // Test fieldMul()

  {
    byte c = fieldMul( 0x01, 0x01 );
    TestCase( c == 0x01 );
    
    c = fieldMul( 0x80, 0x00 );
    TestCase( c == 0x00 );
    
    c = fieldMul( 0xFF, 0xFF );
    TestCase( c == 0x13 );
    
    c = fieldMul( 0x3B, 0x57 );
    TestCase( c == 0x7E );
    
    c = fieldMul( 0x65, 0xB1 );
    TestCase( c == 0x2B );
    
    c = fieldMul( 0xC9, 0xA6 );
    TestCase( c == 0xF3 );
  }


#ifdef DISABLE_TESTS
#endif

  // Report a message if some tests are still disabled.
  if ( totalTests < EXPECTED_TOTAL )
    printf( "** %d of %d tests currently enabled.\n", totalTests,
            EXPECTED_TOTAL );
    
  // Exit successfully if all tests are enabled and they all pass.
  if ( passedTests != EXPECTED_TOTAL )
    return EXIT_FAILURE;
  else
    return EXIT_SUCCESS;
}
