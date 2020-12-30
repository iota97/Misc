#if defined(_WIN64) || defined(_WIN32)
#define OS "Windows"
#elif defined(__APPLE__)
#define OS "Apple"
#elif defined(__linux__)
#define OS "Linux"
#elif defined(__unix__)
#define OS "Unix"
#else
#error Uknown OS
#endif

#include <stdio.h>
int main() {
	puts(OS);
	return 0;
}
