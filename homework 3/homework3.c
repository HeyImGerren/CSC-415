#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define BUFFERSIZE 1024
#define UNIXCOMMANDSSIZE 6


int main ( void ){
  while( 1 ) {
    char userCommands [ BUFFERSIZE ];
    char *unixCommands[ UNIXCOMMANDSSIZE ];
    char *tokenizedCommands = NULL;
    int numberOfArgs = 0;
    pid_t pid;

    printf( ">" );

    if( fgets( userCommands, 1024, stdin ) == NULL ){
      exit( 1 );
    }

    userCommands[ strlen( userCommands ) - 1 ] = '\0';
    tokenizedCommands = strtok( userCommands, " " );

    if( tokenizedCommands == NULL ) {
      printf( "Error: Empty Command!\n" );
      continue;
    }

    while( tokenizedCommands != NULL ){
      unixCommands[ numberOfArgs ] = tokenizedCommands;
      tokenizedCommands = strtok( NULL, " " );
      numberOfArgs++;
    }

    if( numberOfArgs > 5 ){
      printf( "Too many arguments!\n" );
      continue;
    }


    unixCommands[ numberOfArgs ] = '\0';


    if( strcmp( unixCommands[ 0 ], "exit" ) == 0 ){
      exit( 1 );
    }

    pid = fork();

    if( pid == 0 ){
      execvp( unixCommands[ 0 ], unixCommands );
    } else {
        wait( NULL );
      }
  }

  return 0;
}


