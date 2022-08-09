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
    clock_gettime(CLOCK_REALTIME, &request1);
    while (1)
    {
        clock_gettime(CLOCK_REALTIME, &tp);
            request1.tv_nsec += freq;
            if(request1.tv_nsec > 1000000000)
            {
              request1.tv_sec +=1;
              request1.tv_nsec -= 1000000000;
            }
        clock_nanosleep(CLOCK_REALTIME, TIMER_ABSTIME, &request1, NULL);

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

        pthread_attr_t attr1,attr2,attr3;
    struct sched_param param1, param2,param3;

    pthread_attr_init(&attr1);
    pthread_attr_init(&attr2);
    pthread_attr_init(&attr3);

    param1.sched_priority = 3;
    param2.sched_priority = 4;
    param3.sched_priority = 5;

    pthread_attr_setschedpolicy(&attr1,SCHED_FIFO);
    pthread_attr_setschedpolicy(&attr2,SCHED_FIFO);
    pthread_attr_setschedpolicy(&attr3,SCHED_FIFO);

    pthread_attr_setschedparam(&attr1,&param1);
    pthread_attr_setschedparam(&attr2,&param2);
        pthread_attr_setschedparam(&attr3,&param3);

    i = pthread_create(&INPUT, &attr1, getFreq, NULL);
    s = pthread_create(&SAMPLE, &attr2, getTime, NULL);
    l = pthread_create(&LOGGING, &attr3, save_time, NULL);

    // pthread_attr_setinheritsched(&attr1,PTHREAD_EXPLICIT_SCHED);
    // pthread_attr_setinheritsched(&attr2,PTHREAD_EXPLICIT_SCHED);
    // pthread_attr_setinheritsched(&attr3,PTHREAD_EXPLICIT_SCHED);

    pthread_join(INPUT, NULL);
    pthread_join(SAMPLE, NULL);
    pthread_join(LOGGING, NULL);

    pthread_attr_destroy(&attr1);
    pthread_attr_destroy(&attr2);
    pthread_attr_destroy(&attr3);
}