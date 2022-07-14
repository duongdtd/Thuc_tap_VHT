#include"stdio.h"
#include"stdint.h"
#include"pthread.h"
#include"time.h"
#include"string.h"
#include"stdbool.h"
#include"stdlib.h"
#include "unistd.h"
struct timespec tp;
struct timespec tmp,request1;
uint8_t check_loop = 1;
long freq = 1000000;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;  
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;   
 void *getTime(void *args )
{   

  while(check_loop == 1) 
  {      
      clock_gettime(CLOCK_REALTIME,&tp);  
      printf("\n%ld.%09ld",tp.tv_sec,tp.tv_nsec); 
      request1.tv_nsec=tp.tv_nsec;
      request1.tv_sec=tp.tv_sec; 
      long temp;
        if(request1.tv_nsec + freq > 1000000000)
        {
          temp = request1.tv_nsec;
          request1.tv_nsec =0;
          request1.tv_sec +=1;
            if(clock_nanosleep(CLOCK_REALTIME,TIMER_ABSTIME, &request1,NULL) != 0)
              {
                  check_loop = 0;
              }
              else
              {
                request1.tv_nsec+=temp-1000000000 + freq;
                 if(clock_nanosleep(CLOCK_REALTIME,TIMER_ABSTIME, &request1,NULL) != 0)
                  {
                      check_loop = 0;
                  }
                  else
                  {
                      check_loop = 1;
                  }
              }
        }
        else{
            request1.tv_nsec +=freq;
            if(clock_nanosleep(CLOCK_REALTIME,TIMER_ABSTIME, &request1,NULL) != 0)
              {
                  check_loop = 0;
              }
            else
              {
                  check_loop = 1;
              }
        }
        
  }

}
void *getFreq(void *args)
{
  while(1)
  {

  pthread_mutex_lock(&mutex);
  FILE *fp;
  fp = fopen("freq.txt","r");
   unsigned long new_freq = 0;
  fscanf(fp,"%lu",&new_freq);
  fclose(fp); 
  if(new_freq == freq)
  {  pthread_mutex_unlock(&mutex);
  }
    else
  {
    freq = new_freq;
    pthread_mutex_unlock(&mutex);
  } 
  }

}
void *save_time(void *args)
{
  //save time
  while(1)
  {
    FILE *file;
    file = fopen("freq_1000000.txt","a+");
    long diff_sec = ((long) tp.tv_sec) - tmp.tv_sec ;
    long diff_nsec;
    if(tmp.tv_nsec != tp.tv_nsec || tmp.tv_sec != tp.tv_sec)
    {
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
       tmp.tv_nsec =tp.tv_nsec;
       tmp.tv_sec = tp.tv_sec;

    } 
    fclose(file);
  } 
}

int main(int argc, char const *argv[])
{
    
        pthread_t SAMPLE;
        pthread_t INPUT;
        pthread_t LOGGING;
        int s,i,l;
      tmp.tv_sec = 0;
      tmp.tv_nsec = 0;   
    pthread_mutex_init(&mutex, NULL); 
       i = pthread_create(&INPUT,NULL,getFreq,NULL);
        s = pthread_create(&SAMPLE, NULL, getTime,NULL);
       l = pthread_create(&LOGGING,NULL,save_time,NULL);      
       pthread_join(INPUT,NULL);
        pthread_join(SAMPLE,NULL);
       pthread_join(LOGGING,NULL);
           pthread_mutex_destroy(&mutex);
    
}