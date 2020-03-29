#define SELECT_WRITE_BUFFER(buff) char* BUFF_CURSOR = buff;
#define WRITE(...) BUFF_CURSOR += sprintf (BUFF_CURSOR, ##__VA_ARGS__)

#include <stdlib.h>
#include <stdio.h>

int main () {
	char* buff = malloc(512);
	SELECT_WRITE_BUFFER(buff);
	WRITE("test %d", 5);
	puts(buff);
	return 0;
}
