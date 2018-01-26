#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h> 
#include <stdio.h>
#include <errno.h>
#include <string.h>

#define BUFFERSIZE 1024
extern int errno;

int main ( int argc, char *argv[]){
  int bytesTransferred = 0;
  int originalFile; 
  int copiedFile; 
  int closeFile1, closeFile2;
  ssize_t readBytes; 
  ssize_t writeBytes;
  char contentOfFile[ BUFFERSIZE ]; 
 
  if ( (originalFile = open( argv[1], O_RDONLY )) < 0 ){
    fprintf( stderr, "Error opening the first file: %s\n", strerror( errno )); 
    return -1;
  } 
       
  if ( (copiedFile = open( argv[2], O_WRONLY|O_CREAT|O_EXCL, 00700 )) < 0 ){
    fprintf( stderr, "Error opening second file: %s\n", strerror( errno ) );
    return -1;
  }	
  
 
  while ( (readBytes = read( originalFile, contentOfFile, sizeof( contentOfFile )) ) > 0 ) {
    if ( readBytes < 0 ){
      fprintf( stderr, "Error reading from first file: %s\n", strerror( errno ) );
      return -1;
    } else if( (writeBytes = write( copiedFile, contentOfFile, readBytes )) != readBytes ){
      printf("Error: Short write!\n"); 
      return -1; 
    } else {
        bytesTransferred += writeBytes;
      }
  } 

  if( (closeFile1 = close(originalFile)) != 0 ){
    fprintf( stderr, "Error: %s\n", strerror( errno ));
    return -1;
  }
  
  if( (closeFile2 = close(copiedFile)) != 0 ){
    fprintf( stderr, "Error: %s\n", strerror( errno ));
    return -1;
  }
 
  printf ("copied %d bytes \n", bytesTransferred);
  
  
  return 0;
}
