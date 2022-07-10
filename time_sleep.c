#include"time.h"
#include"stdint.h"
#include"stdlib.h"
#include"string.h"
#include"stdio.h"
    long old_sec;
int main(int argc, char const *argv[])
{
    if(argc >1)
    {
    char *eptr;

    old_sec = strtol(argv[1],&eptr,10);
    }
    printf("%ld",old_sec);
    return 0;
}