#include"stdio.h"
#include"stdint.h"
#include"pthread.h"
 int main(int aincrgc, char const *argv[])
{

	FILE *fp;
	fp = fopen("freq.txt","r");
	char c ;
	while( (c = fgetc(fp)) != EOF)
	{
		printf("%c",c);
	}
	fclose(fp);
	return 0;
}