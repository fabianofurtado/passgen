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
 ******************************************************************************/
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>


#define VALID_CHARS "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ\
0123456789.,-_@$%^*#?!<>=+;:~'|\"/\\[]{}()"

#define PASSWD_MAX_SIZE  255


static bool
gen_passwd( char * passwd, uint8_t pass_size )
{
  uint8_t byte;
  FILE * f;
  uint8_t i;

  if ( NULL == ( f = fopen( "/dev/urandom", "rb" ) ) ) {
    perror("Erro ao abrir /dev/urandom");
    return false;
  }

  for ( i = 0 ; i < pass_size ; i++ ) {
    if ( fread( &byte, 1, 1, f ) != 1 ) {
      perror("Erro ao ler de \"/dev/urandom\"");
      fclose( f );
      return false;
    }
    passwd[i] = VALID_CHARS[ byte % ( sizeof( VALID_CHARS )-1 ) ];
  }

  fclose( f );

  return true;
}


int
main( int argc, char *argv[] )
{
  char     *str,
           *endstr;
  uint8_t   pass_size,
            ps_count;
  uint64_t  pass_num,
            pn_count;
  char passwd[ PASSWD_MAX_SIZE ];

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
      fprintf( stderr, "Password size is not a valid number!\n" );
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

  if ( pass_size > PASSWD_MAX_SIZE ) {
    fprintf( stderr, "Error! Cannot generate passsords greater than "
                     "%d characters!\n", PASSWD_MAX_SIZE );
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

  fprintf( stdout,
           "# Wait... generating %lu passwords with %u digits:\n",
           pass_num, pass_size );

  for ( pn_count=0 ; pn_count < pass_num ; pn_count++ ) {
    for ( ps_count=0 ; ps_count < pass_size ; ps_count++ ) {
      if ( false == gen_passwd( passwd, pass_size ) )
        return EXIT_FAILURE;
    }
    printf("%.*s\n", PASSWD_MAX_SIZE, passwd);
  }

  return EXIT_SUCCESS;
}
