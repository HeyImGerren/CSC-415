#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/stat.h> 
#include <fcntl.h>

#define BUFFERSIZE 67108864
#define NUMBEROFASCIICHARS 128
 
char buffer [ BUFFERSIZE ];
int threadCounts [ NUMBEROFASCIICHARS ]; 

int main ( int argc, char *argv[] ) {
  int fileOpener, fileCloser, asciiCharacter, chTotal = 0; 
  ssize_t fileReader; 

  if ( argc != 2 ) {
    fprintf( stderr, "Incorrect number of command line arguments!" );
    
    return ( 1 ); 
  }
  
  fileOpener = open( argv[ 1 ], O_RDONLY ); 
  if ( fileOpener == -1 ){
    fprintf( stderr, "Failed to open file!" );
		
    return ( 1 );
  }
  
  fileReader = read( fileOpener, buffer, sizeof(buffer) ); 
  if ( fileReader == -1 ) { 
    fprintf ( stderr, "Failed to read from file!" ); 

    return ( 1 ); 
  }
   
  fileCloser = close( fileOpener ); 
  if ( fileCloser != 0 ) {
    fprintf ( stderr, "Failed to close file!" );

    return ( 1 );
  }
  
   for ( int incrementer = 0; incrementer < fileReader; incrementer++ ){
    asciiCharacter = buffer[ incrementer ];  
    threadCounts[ asciiCharacter ]++; 
    }
  
  for ( int ch = 0; ch < 128; ch++ ) {
    chTotal += threadCounts[ ch ];
    printf( "%i occurrences of ", chTotal ); 
    if ( ch <= 32 || ch == 127 ) {
      printf( "0x%02x\n ", ch ); 
    }else {
      printf( "%c\n ", ch );
     } 
     chTotal = 0;
  }

  return ( 0 ); 
}//end main  


