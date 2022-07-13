#include"stdio.h"
#include"stdint.h"
#include"pthread.h"
#include"time.h"
#include"string.h"
#include"stdlib.h"
#include "unistd.h"
struct timespec tp;
struct timespec tmp;

long freq = 1;
struct timespec time1, time2;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;   
 void *getTime(void *args )
{   
    clock_gettime(CLOCK_REALTIME,&tp);
    return NULL;

}
void *getFreq(void *args)
{
    pthread_mutex_lock(&mutex);
  FILE *fp;
  fp = fopen("freq.txt","r");
   unsigned long new_freq = 0;
  fscanf(fp,"%lu",&new_freq);
  fclose(fp); 
  if(new_freq == freq)
  {  pthread_mutex_unlock(&mutex);
    return NULL;
  }
    else
    {
    freq = new_freq;
    time1.tv_nsec = new_freq;
    pthread_mutex_unlock(&mutex);
    return NULL;
    } 

}
void *save_time(void *args)
{
  //save time
  if(tmp.tv_nsec != tp.tv_nsec || tmp.tv_sec != tp.tv_sec)
  {
    FILE *file;
    file = fopen("freq_100000.txt","a");
    long diff_sec = ((long) tp.tv_sec) - tmp.tv_sec ;
    long diff_nsec;

    if(tp.tv_nsec > tmp.tv_nsec)
      {
        diff_nsec = tp.tv_nsec - tmp.tv_nsec;
      }
    else 
      {
        diff_nsec = 1000000000 + tp.tv_nsec - tmp.tv_nsec ;
        diff_sec = diff_sec - 1;
      }
     fprintf(file,"\n%ld.%09ld",diff_sec,diff_nsec);  
    fclose(file);
    tmp.tv_sec = tp.tv_sec;
    tmp.tv_nsec = tp.tv_nsec;
    return NULL;
  }
  else
  {  
    return NULL;
  }
}

int main(int argc, char const *argv[])
{
    
        pthread_t SAMPLE;
        pthread_t INPUT;
        pthread_t LOGGING;
        int s,i,l;
      time1.tv_sec = 0;
      time1.tv_nsec = freq;

      tmp.tv_sec = 0;
      tmp.tv_nsec = 0;   
    pthread_mutex_init(&mutex, NULL); 
      while(1)
        {

        if(nanosleep(&time1 , &time2) < 0 )   
            {
        return -1;
      }
        else
          { 
        i = pthread_create(&INPUT,NULL,getFreq,NULL);
        s = pthread_create(&SAMPLE, NULL, getTime,NULL);
        l = pthread_create(&LOGGING,NULL,save_time,NULL);      
        pthread_join(INPUT,NULL);
        pthread_join(SAMPLE,NULL);
        pthread_join(LOGGING,NULL);
           pthread_mutex_destroy(&mutex);
      }
        } 
}