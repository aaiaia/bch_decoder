#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "func_verify.h"

unsigned int* initTestMode_prob_min_counting(unsigned int **p, unsigned int *len, unsigned int max_val)
{
    if(!p)          return NULL;
    if(!max_val)    return NULL;

    (*len) = max_val + 1;
    (*p) = (unsigned int*)malloc(sizeof(unsigned int) * ((*len) + 1));
    memset(*p, 0, sizeof(unsigned int) * ((*len) + 1));
    return (*p);
}

char clearTestMode_prob_min_counting(unsigned int *p, unsigned int len, unsigned int max_val)
{
    if(!p)  return -1;
    memset(p, 0, sizeof(unsigned int) * len);
    return 0;
}
