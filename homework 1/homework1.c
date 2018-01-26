#include <stdio.h>

#define NAME "Gerren Penaloza"

int main(void)
{
	char nameBuffer[100];

	snprintf(nameBuffer, 100, "%s", "Hello 415, I am " NAME "!\n");

	write(1, nameBuffer, 34);

	return 0;
}
