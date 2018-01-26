#include <stdio.h>
#include <stdlib.h>
#include <pthread.h> 
#include <errno.h>
#include <err.h> 
#include <unistd.h>
#include <string.h>

#define BUFFERSIZE 16

static pthread_mutex_t lock;
static pthread_cond_t ready_to_read, ready_to_write; 

int buffer [ BUFFERSIZE ]; 
int bufferCurrentSize = 0; 

int totalItemsProduced = 0;
int totalItemsConsumed = 0; 
int readyIndex = 0;

typedef struct producerInfo {
  int producer_thread_number; 
  int num_produced; 
} producerInfo; 

typedef struct consumerInfo {
  int consumer_thread_number;
  int num_consumed; 
} consumerInfo; 

void *consumer ( void *param );
void *producer ( void *param );

int main ( int argc, char *argv[] ) { 
  if ( argc != 4 ) {
    fprintf( stderr, "Usage: %s <source> <destination>\n", argv[ 0 ] ); 
    return ( 1 ); 
  }
 
  int numProducers = 1 << atoi( argv[ 1 ] );
  int numConsumers = 1 << atoi( argv[ 2 ] );
  int numItemsProduced = 1 << atoi( argv[ 3 ] );
  int error, createConsumerThread, createProducerThread, joinConsumerThread, joinProducerThread;
 
  struct consumerInfo consumerThreadInfo [ numConsumers ];
  pthread_t consumerThread [ numConsumers ]; 
  
  struct producerInfo producerThreadInfo [ numProducers ];
  pthread_t producerThread [ numProducers ]; 
 
  printf( "Producers: %d, Consumers: %d, ItemsProduced: %d \n", numProducers, numConsumers, numItemsProduced ); 
  printf ( "Each consumer will consume %d items\n\n", ( numProducers * numItemsProduced ) / numConsumers );
  
  error = pthread_mutex_init( &lock, NULL );
  if ( error ) {
    errno = error;
    err( 1, "pthread_mutex_init" );
  }
  
  error = pthread_cond_init( &ready_to_read, NULL );
  if ( error ) {
    errno = error;
    err( 1, "pthread_cond_init(ready_to_read)" );
  }
  
  error = pthread_cond_init( &ready_to_write, NULL );
  if ( error ) {
    errno = error;
    err( 1, "pthread_cond_init(ready_to_write)" );
  }
  
  for ( int producerIndex = 0; producerIndex < numProducers; producerIndex++ ) {
    producerThreadInfo[ producerIndex ].producer_thread_number = producerIndex; 
    producerThreadInfo[ producerIndex ].num_produced = numItemsProduced; 
    
    createProducerThread = pthread_create( &producerThread[ producerIndex ], NULL, producer,  &producerThreadInfo[ producerIndex ] );
    if ( createProducerThread != 0 ) { 
      fprintf ( stderr, "Error Creating Producer Thread!" );
      return ( 1 );
    }  
  }
 
  for ( int consumerIndex = 0; consumerIndex < numConsumers; consumerIndex++ ){
    consumerThreadInfo[ consumerIndex ].consumer_thread_number = consumerIndex;
    consumerThreadInfo[ consumerIndex ].num_consumed = ( numProducers * numItemsProduced ) / numConsumers;
     
    createConsumerThread = pthread_create( &consumerThread[ consumerIndex ], NULL, consumer, &consumerThreadInfo[ consumerIndex ] );
    if ( createConsumerThread != 0 ) { 
      fprintf ( stderr, "Error Creating Consumer Thread!" );
      return ( 1 );
    } 
  }
  
  for ( int prodThread = 0; prodThread < numProducers; prodThread++ ) {
    joinProducerThread = pthread_join( producerThread[ prodThread ], NULL );
    if ( joinProducerThread != 0 ) {
      fprintf ( stderr, "Error Joining Producer Thread!" );
      return ( 1 );
    }
  }
  
  for ( int consThread = 0; consThread < numConsumers; consThread++ ) {
    joinConsumerThread = pthread_join( consumerThread[ consThread ], NULL );
    if ( joinConsumerThread != 0 ) {
      fprintf ( stderr, "Error Joining Consumer Thread!" );
      return ( 1 );
    }
  }
  
  pthread_cond_destroy( &ready_to_read );
  pthread_cond_destroy( &ready_to_write );
  pthread_mutex_destroy( &lock );
  
  printf( "\nThe total number of items produced is %d\n", totalItemsProduced );
  printf( "The total number of items consumed is %d\n", totalItemsConsumed );  
  
  return ( 0 );
}

void *producer ( void *param ) { 
  producerInfo *prThread = ( producerInfo* ) param;
  
  int counter = 0;
  int producerID = prThread->producer_thread_number; 
  int numToProduce = prThread ->num_produced; 
  
  for ( int i = 0; i < numToProduce; i++ ) {
    pthread_mutex_lock( &lock );
    while ( bufferCurrentSize >= BUFFERSIZE ) {
      pthread_cond_wait( &ready_to_write, &lock );
    }
    
    int indexInBuffer = ( readyIndex + bufferCurrentSize ) % BUFFERSIZE; 
    buffer [ indexInBuffer ] = producerID * numToProduce + counter; 
    counter++;
    bufferCurrentSize++;
    
    printf( "producer %d produced %d in index %d of the buffer\n", producerID, buffer[indexInBuffer], indexInBuffer);
    
    pthread_mutex_unlock( &lock );
    pthread_cond_signal( &ready_to_read );
    totalItemsProduced++;
  }
  
  printf( "\n**producer %d is now exiting, it produced %d items**\n\n", producerID, counter ); 
  pthread_exit( NULL );
}


void *consumer ( void *param ) {
  int counter = 0; 
  consumerInfo *coThread = ( consumerInfo* ) param;
  int consumerID = coThread->consumer_thread_number; 
  int numToConsume = coThread->num_consumed; 
  
  for ( int i = 0; i < numToConsume; i++ ) {
    pthread_mutex_lock ( &lock ); 
    while ( bufferCurrentSize <= 0 ) {
      pthread_cond_wait( &ready_to_read, &lock ); 
    }
  
    printf( "consumer %d consumed %d in index %d of the buffer \n", consumerID, buffer[ readyIndex ], readyIndex );
    counter++;
    bufferCurrentSize--; 
    readyIndex = ( readyIndex + 1 ) % BUFFERSIZE; 
  
    pthread_mutex_unlock ( &lock ); 
    pthread_cond_signal ( &ready_to_write ); 
  
    totalItemsConsumed++; 
  }
  
  printf( "\n**consumer %d is now exiting, it consumed %d items**\n\n", consumerID, counter ); 
  pthread_exit( NULL );
}
