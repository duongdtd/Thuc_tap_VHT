#include "stdio.h"
#include "stdint.h"
#include "pthread.h"
#include "time.h"
#include "string.h"
#include "stdbool.h"
#include "stdlib.h"
#include "unistd.h"
struct timespec tp;
struct timespec tmp, request1;
long freq = 100000;


void *getTime(void *args)
{
    clock_gettime(CLOCK_MONOTONIC, &request1);
    while (1)
    {
        clock_gettime(CLOCK_REALTIME, &tp);
            request1.tv_nsec += freq;
            if(request1.tv_nsec > 1000000000)
            {
              request1.tv_sec +=1;
              request1.tv_nsec -= 1000000000;
            }
        clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &request1, NULL);
    }
}
void *getFreq(void *args)

{
        FILE *fp;
        fp = fopen("freq.txt", "r");
        unsigned long new_freq = 0;
        fscanf(fp, "%lu", &new_freq);
    while (1)
    {
            if (new_freq != freq)
        {
            freq = new_freq;
        }
    }
            fclose(fp);
}
void *save_time(void *args)
{
    // save time
            FILE *file;
        file = fopen("result.txt", "a+");
    while (1)
    {
        long diff_nsec;
        if (tmp.tv_nsec != tp.tv_nsec || tmp.tv_sec != tp.tv_sec)
        {
            if (tp.tv_nsec > tmp.tv_nsec)
            {
                diff_nsec = tp.tv_nsec - tmp.tv_nsec;
            }
            else
            {
                diff_nsec = 1000000000 + tp.tv_nsec - tmp.tv_nsec;
            }
            fprintf(file, "\n%ld",diff_nsec);
            tmp.tv_nsec = tp.tv_nsec;
            tmp.tv_sec = tp.tv_sec;
        }
    }
            fclose(file);
}

int main(int argc, char const *argv[])
{

    pthread_t SAMPLE;
    pthread_t INPUT;
    pthread_t LOGGING;
    int s, i, l;
    tmp.tv_sec = 0;
    tmp.tv_nsec = 0;

    i = pthread_create(&INPUT, NULL, getFreq, NULL);
    s = pthread_create(&SAMPLE, NULL, getTime, NULL);
    l = pthread_create(&LOGGING, NULL, save_time, NULL);

    pthread_join(SAMPLE, NULL);
    pthread_join(INPUT, NULL);
    pthread_join(LOGGING, NULL);


}