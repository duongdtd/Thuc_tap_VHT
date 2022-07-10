#include"stdio.h"
#include"stdint.h"
#include"pthread.h"
#include"time.h"
#include"string.h"
#include"stdlib.h"
#include "unistd.h"
struct timespec tp;
long freq;
long check_freq;
struct timespec time1, time2;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;   
pthread_cond_t cond = PTHREAD_COND_INITIALIZER; 
 long get_freq()
{
 	long data;
 	FILE *fp;
	fp = fopen("freq.txt","r");
	char buff[100];
	fgets(buff,sizeof(buff),fp);
	char *eptr;
	data = strtol(buff,&eptr,10);
	fclose(fp);
	return data;
 }
 void *getTime(void *args )
{		long x = *((long*)args);

		clock_gettime(CLOCK_REALTIME,&tp);	
       	char buff[100];
    	strftime(buff, sizeof buff, "%D %T", gmtime(&tp.tv_sec));
    	printf("Current time: %s.%09ld UTC\n", buff, tp.tv_nsec);
		printf("%09ld\n",tp.tv_sec);
		return NULL;
}
void *getFreq(void *args)
{
	long x = *((long*)args);
	long old_freq = get_freq();
	if(old_freq == x)
	{
		return NULL;
	}
   	else
   	{
		FILE *fp;
		fp = fopen("freq.txt","w");
		fprintf(fp,"%ld",x);
		fclose(fp);
		freq = x;
		time1.tv_sec = 0;
	   	time1.tv_nsec = freq;
   	} 
   		return NULL;

}
void *save_time(void *args)
{
	struct timespec *tpx = (struct timespec *)args;

	//get time from time_and_interval;
	FILE *fp;
	fp = fopen("time_and_interval.txt","r");
	char buff[100];
	fgets(buff,sizeof(buff),fp);
	char *t_sec;
	char *t_nsec;
	t_sec = strtok(buff,".");
	t_nsec = strtok(NULL,".");
	fclose(fp);
	char *eptr;
	long old_nsec;
	long old_sec;
	old_nsec = strtol(t_nsec,&eptr,10);
	old_sec = strtol(t_sec,&eptr,10);

	//save time
	FILE *file;
	file = fopen("time_and_interval.txt","w+");
	long diff_sec = ((long) tpx->tv_sec) - old_sec ;
	long diff_nsec;
	if(tpx->tv_nsec > old_nsec)
		{
			diff_nsec = tpx->tv_nsec - old_nsec;
		}
	else 
		{
			diff_nsec = old_nsec - tpx->tv_nsec;
			diff_sec = diff_sec - 1;
		}
	fprintf(file, "%ld.%09ld\n%ld.%09ld",tpx->tv_sec,tpx->tv_nsec,diff_sec,diff_nsec);	
	fclose(file);
}

int main(int argc, char const *argv[])
{
	if(argc >1)
    {
    char *eptr;
    check_freq = strtol(argv[1],&eptr,10);
    }
		FILE *fp;
		freq = get_freq();
        pthread_t SAMPLE;
        pthread_t INPUT;
        pthread_t LOGGING;
        int s,i,l;
   		time1.tv_sec = 0;
   		time1.tv_nsec = freq;        
   		while(1)
        {

        if(nanosleep(&time1 , &time2) < 0 )   
			{
			 	printf("Nano sleep system call failed \n");
			 	return -1;
			}
		else
        	{	
        		i = pthread_create(&INPUT,NULL,getFreq,&check_freq);
        		s = pthread_create(&SAMPLE, NULL, getTime,&freq);
        		l = pthread_create(&LOGGING,NULL,save_time,&tp);
        		pthread_join(INPUT,NULL);
        		pthread_join(SAMPLE,NULL);
				pthread_join(LOGGING,NULL);
			}
        }	return 0;
}