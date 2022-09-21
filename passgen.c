/*******************************************************************************
 * FILENAME: passgen.c                                                         *
 *                                                                             *
 * DESCRIPTION:                                                                *
 *       Generic password generator based on random numbers                    *
 *                                                                             *
 * AUTHOR: Fabiano Furtado          START DATE: 2022-09-20                     *
 *                                                                             *
 * COMPILING: make                                                             *
 *                                                                             *
 * CHANGES:                                                                    *
 *                                                                             *
 * REF NO  VERSION DATE   WHO    DETAIL                                        *
 *                                                                             *
 *                                                                             *
 ******************************************************************************/
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include <unistd.h>


#define VALID_CHARS "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ\
0123456789.,-_@$%^*#?!<>=+;:~'|\"/\\[]{}()"


int
main( int argc, char *argv[] )
{
  const char * valid_chars = VALID_CHARS;
  char *str, *endstr;
  uint64_t i, j, pass_num, pass_size;

  if ( argc != 3 ) {
    fprintf( stderr,"Error! Argument not found!\n\n  "
             "Usage: %s <passwd size> <number of passwd>\n\n", argv[0]);
    return EXIT_FAILURE;
  }


  str = argv[1]; // password size

  errno = 0;    /* To distinguish success/failure after call */
  pass_size = strtoul( str, &endstr, 10 );

  /* Check for various possible errors. */
  if ( errno != 0 ) {
      fprintf( stderr, "strtoul()\n" );
      return EXIT_FAILURE;
  }

  if ( endstr == str ) {
      fprintf( stderr, "No digits were found.\n" );
      return EXIT_FAILURE;
  }

  if ( *endstr != '\0' )        /* Not necessarily an error... */
      fprintf( stdout, "Further characters after number: \"%s\"\n", endstr );

  if ( pass_size < 8 ) {
    fprintf( stderr, "Error! Cannot generate passsords shorter than "
                     "8 characters!\n" );
    return EXIT_FAILURE;
  }

  str = argv[2]; // number os passwords

  errno = 0;    /* To distinguish success/failure after call */
  pass_num = strtoul( str, &endstr, 10 );

  /* Check for various possible errors. */
  if ( errno != 0 ) {
      fprintf( stderr, "strtoul()\n" );
      return EXIT_FAILURE;
  }

  if ( endstr == str ) {
      fprintf( stderr, "No digits were found.\n" );
      return EXIT_FAILURE;
  }

  if ( *endstr != '\0' )        /* Not necessarily an error... */
      fprintf( stdout, "Further characters after number: \"%s\"\n", endstr );

  if ( pass_num == 0 ) {
    fprintf( stderr, "Error! Number of passwords cannot be 0!\n" ); 
    return EXIT_FAILURE;
  }

  char passwd[ pass_size + 1 ];
  passwd[pass_size] = '\0';

  fprintf( stdout,
           "Wait... generating %lu passwords with %lu digits:\n",
           pass_num, pass_size );

  srand( (unsigned int)time(NULL) + (unsigned int)getpid() );

  for ( i=0; i < pass_num; i++ ) {
    for ( j=0; j < pass_size; j++ ) {
      passwd[j] = valid_chars[ ( (uint64_t)rand() + i + j ) %
                               (uint64_t)( sizeof( VALID_CHARS )-1 ) ];
    }
    puts( passwd );
  }
  puts( "\nDone!" );

  return EXIT_SUCCESS;
}