#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#define C 5 // column
#define R 5 // row
int arr[R][C] = {{1, 19, 12, -1, -6},{9, 7, 5, 3,-4 },{7, 2, 4, 9, -3},{ -4, 7, 8, -2, 5 },{2, -5, -4, 12, 7}};
int s[R];
void *f(void *p)
{
 int row = *((int*) p);
int i;
for(i = 0; i < C; i++)
{
s[row] += arr[row][i];
 }
 return NULL;
}
int main(int argc, char *argv[])
{
 int i, j;
 int *p = NULL;
int sum = 0;
pthread_t th[R];
for(i = 0; i < R; i++)
{
p = malloc(sizeof(int));
*p = i;
 int rc = pthread_create(&th[i], NULL, f, p);
 if(rc!=0)
{
 printf(" Thread creation failed");
 exit(-1);
 }
}
for(i = 0; i < R; i++)
 {
pthread_join(th[i], NULL);
}
 for(i = 0; i < R; i++)
{
sum += s[i];
}
printf(" SUM = %d \n", sum);
 return 0;
}