/*  

./pdf_maker [page_count] [img_path]

./pdf_maker 23 img/

*/

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv)
{
	char buffer[2048];
	char* p = (char*)buffer;

	p+=sprintf(p, "convert ");
	for(int i = 0; i <= atoi(argv[1]); i++)
	{
		p+=sprintf(p, "%s%d.png ", argv[2], i);
	} 
	p+=sprintf(p, "out.pdf\n");

	system((char*) buffer);

	return 0;
}
