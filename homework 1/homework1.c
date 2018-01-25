/*
Name: homework1
Author: Gerren Penaloza
Date: January 30th, 2017
Description: The program uses snprintf() which passes the string "Hello 415, I am " along with a defined constant
which holds any name that you desire to a buffer. The buffer is then passed to the write() system call where we input
the integer 1 for standard output. The output for the program is "Hello 415, I am <NAME>!" where NAME is any name you
would like. 
*/ 

//you need to include <unistd.h> for the write() system call
//you need to include <stdio.h> for the standard input/output stuff like snprintf()
#include <unistd.h>
#include <stdio.h> 

/*this is the defined variable for the name that is being outputted in the sentence.
you can change the program and make it output a different name by changing the characters
inside the " ".*/ 
#define NAME "Gerren Penaloza" 

int main(void)
{
//this is the buffer that we use to pass the entire sentence into. I made it size 100 just to be safe.

char nameBuffer[100];

/*
snprintf() takes in multiple arguments. The first is the buffer you will pass the string into. The second
is the maximum number of bytes that will be written to the buffer. The third argument is the conversion specifier.
The fourth argument is the entire string being passed into the buffer. 
*/ 

snprintf(nameBuffer, 100, "%s", "Hello 415, I am " NAME "!\n");

/*
this is the write() system call. The first argument can be 0(standard input), 1(standard output), 2(standard error). 
The second argument is the buffer which contains the string that will be printed out (since we chose to write 1). 
the third argument is the max number of bytes being printed from that buffer(be sure to include the null character at the end.
*/

write(1, nameBuffer, 34);

return 0;

}
