/* Write a string as a ASM function to annoy disassembler */

#include <stdio.h>
#include <stdlib.h>

int write_obfuscated_source(char* msg, char* path)
{
	const char* preamble = 
		"#include <stdio.h>\n\n"
		"__attribute__((naked)) void str()\n"
		"{\n";

	const char* closing =
		"\tasm (\".byte 0x00\");\n"
		"\tasm (\"retq\");\n"
		"}\n\n"
		"int main()\n"
		"{\n"
		"\tprintf(\"%s\", (char*) &str);\n"
		"\treturn 0;\n"
		"}\n";

	FILE* outfile = fopen(path, "w");
	
	if (!outfile)
	{
		puts("Error opening output file");
		return 1;
	}

	fprintf(outfile, "%s", preamble);

	int i = 0;
	while (msg[i] != '\0')
	{	
		fprintf(outfile, "\tasm (\".byte 0x%02x\");\n", msg[i]);
		i++;
	}

	fprintf(outfile, "%s", closing);
	fclose(outfile);
	
	return 0;
}

int main ()
{
	char msg[2048];
	
	puts("Enter the message:");
	fgets(msg, 2048, stdin);
	
	int result = write_obfuscated_source(msg, "obf_msg.c");

	puts("Run \"cc -O2 obf_msg.c -o message\" to compile");
	
	return result;
}
