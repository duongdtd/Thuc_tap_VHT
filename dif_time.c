#include"stdio.h"
#include"time.h"
#include"stdint.h"
#include"stdlib.h"
#include"string.h"
int main(int argc, char const *argv[])
{
	/* code */
	// get time from time_and_interval.txt
	FILE *fp;
	fp = fopen("time_and_interval.txt","r");
	char buff[100];
	fgets(buff,sizeof(buff),fp);
	char *t_sec;
	char *t_nsec;
	t_sec = strtok(buff,".");
	t_nsec = strtok(NULL,".");
	fclose(fp);
	// get real time (nanoseconds)
	struct timespec tp;
	clock_gettime(CLOCK_REALTIME,&tp);
	//convert sting to long
	char *eptr;
	long old_nsec;
	long old_sec;
	old_nsec = strtol(t_nsec,&eptr,10);
	old_sec = strtol(t_sec,&eptr,10);
	//save time
	FILE *file;
	file = fopen("time_and_interval.txt","w+");
	long diff_sec = ((long) tp.tv_sec) - old_sec ;
	long diff_nsec;
	if(tp.tv_nsec > old_nsec)
		{
			diff_nsec = tp.tv_nsec - old_nsec;
		}
	else 
		{
			diff_nsec = old_nsec - tp.tv_nsec;
			diff_sec = diff_sec - 1;
		}
	fprintf(file, "%ld.%09ld\n%ld.%09ld",tp.tv_sec,tp.tv_nsec,diff_sec,diff_nsec);	
	fclose(file);
		printf("%09ld\n",diff_nsec);
	return 0;
}