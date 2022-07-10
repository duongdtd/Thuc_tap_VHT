#include"time.h"
#include"stdio.h"
#include"stdint.h"
int main(int argc, char const *argv[])
{
	struct timespec tp;
	clock_gettime(CLOCK_REALTIME,&tp);
	    char buff[100];
    strftime(buff, sizeof buff, "%D %T", gmtime(&tp.tv_sec));
    printf("Current time: %s.%09ld UTC\n", buff, tp.tv_nsec);
	printf("%09ld",tp.tv_sec);


	FILE *fp;
	fp = fopen("time_and_interval.txt","w+");
	fprintf(fp, "%ld.%09ld",tp.tv_sec,tp.tv_nsec);
	fclose(fp);
	return 0;
}