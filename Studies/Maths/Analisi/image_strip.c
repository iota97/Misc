/*  

./image_strip [page_count] [in_path] [out_path]

./image_strip 23 img/ img_out/

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
	p+=sprintf(p, "-strip %s%%d.png\n", argv[3]);

	system((char*) buffer);

	return 0;
}
