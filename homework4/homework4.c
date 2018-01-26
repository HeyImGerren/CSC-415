#include <stdio.h>
#include <stdlib.h>
#include <pthread.h> 
#include <errno.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/stat.h> 
#include <fcntl.h>

#define BUFFERSIZE 67108864
#define NUMBEROFTHREADS 4 
#define NUMBEROFASCIICHARS 128
 
char buffer [ BUFFERSIZE ];
int threadCounts [ NUMBEROFTHREADS ][ NUMBEROFASCIICHARS ]; 

typedef struct threadInfo {
  int threadIndex;
  int threadSize; 
}threadInfo; 

void *characterCounter ( void *param ); 

int main ( int argc, char *argv[] ) {
  int fileOpener, fileCloser, boundaryOfPartition, createThreads, joinThreads, chTotal = 0; 
  ssize_t fileReader; 
  
  pthread_t thread[ NUMBEROFTHREADS ]; 
  
  struct threadInfo t_info[ NUMBEROFTHREADS ];

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
  
  boundaryOfPartition =  fileReader / NUMBEROFTHREADS; 
    
  for ( int t_index = 0; t_index < NUMBEROFTHREADS; t_index++ ) {
    t_info[ t_index ].threadIndex = t_index; 
    t_info[ t_index ].threadSize = boundaryOfPartition; 
    
    createThreads = pthread_create( &thread[ t_index ], NULL, characterCounter, &t_info[ t_index ] ); 
    if ( createThreads != 0 ) { 
      fprintf ( stderr, "Error Creating Thread!" );
      
      return ( 1 ); 
    } else { 
        joinThreads = pthread_join( thread[ t_index ], NULL );  
        if ( joinThreads != 0 ) {
          fprintf ( stderr, "Error Joining Thread!" );
      
          return ( 1 );
        }
    }
  }  
   
  for ( int ch = 0; ch < 128; ch++ ) {
    for ( int thIndex = 0; thIndex < NUMBEROFTHREADS; thIndex++ ) {
      chTotal += threadCounts [ thIndex ][ ch ];
    }
    
    printf( "%i occurrences of ", chTotal ); 
    if ( ch <= 32 || ch == 127 ) {
      printf( "0x%02x\n ", ch ); 
 
    }else {
      printf( "%c\n ", ch );
  
    } 

    chTotal = 0;

  }

  return ( 0 ); 
} 

void *characterCounter ( void *param ) {
  threadInfo *threadCt = (threadInfo*)param;
  int startingPoint = threadCt->threadIndex * threadCt->threadSize; 
  int asciiCharacter = buffer[ startingPoint ];  
  
  for ( int incrementer = 0; incrementer < threadCt->threadSize; incrementer++ ){
    threadCounts[ threadCt->threadIndex ][ asciiCharacter ]++; 
    startingPoint++; 
    asciiCharacter = buffer[ startingPoint ];  
  }

  pthread_exit( NULL ); 
    
}






