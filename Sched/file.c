#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include<string.h>
#include<stdlib.h>
void looping(int n)
{
	int i,j,tmp;
	for( i=0;i<200000;i++)
	{
		for( j=0;j<2000000;j++)
		{
			tmp+=i;
		}
	}
}

int main(int argc, char **args)
{
   pid_t child_pid;
   int i;
   int x = atoi(args[1]);

   for(i = 0;i<x;i++)
   {
  child_pid = fork (); // Create a new child process;
   if (child_pid < 0) {
      printf("fork failed");
      return 1;
   } 
   else if(child_pid == 0) {
		looping(x);
		printf("a");
   } 
	}
   return 0;
}