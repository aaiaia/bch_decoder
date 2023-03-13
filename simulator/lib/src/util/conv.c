#include <stdio.h>
#include <string.h>
#include <math.h>

#include "cmd_opt_vars.h"

double convertStringToDouble(char *string)
{
    double tmp=0.0;
    unsigned int strLengh=0;
    unsigned int i=0;
    unsigned int dotFlag=0;

    unsigned char minusFlag=0;

    if(!string)
    {
        return 0.0;
    }


    strLengh=strlen(string);

    if(*(string+0) == '-')
    {
        minusFlag = 1;
    }
    else
    {
        minusFlag = 0;
    }


    for(i=minusFlag; i<strLengh; i++)
    {
        if(('0'<=*(string+i))&&((*(string+i))<='9'))
        {
            if(!(dotFlag))
            {
                tmp*=10.0;
                tmp+=((double)((*(string+i))-'0'));
            }
            else
            {
                tmp+=((double)((*(string+i))-'0'))*pow(0.1, ((double)(i-dotFlag)));
            }
        }
        else if(*(string+i)=='.')
        {
            dotFlag=i;
        }
        else
        {
            warningMes;
            printf("in convertStringToDouble, ");
            printf("Include Invalied charactor, value : %s.\n", string);
            return 0.0;
        }
    }
    if(minusFlag) tmp*=(-1.0);

    return tmp;
}

unsigned char convertStringToUnsignedChar(char *string)
{
    unsigned char tmp=0;
    unsigned int strLengh=0;
    unsigned int i=0;

    if(!string)
    {
        return 0;
    }

    strLengh=strlen(string);
    for(i=0; i<strLengh; i++)
    {
        if(('0'<=*(string+i))&&((*(string+i))<='9'))
        {
            tmp*=10;
            tmp+=((*(string+i))-'0');
        }
        else
        {
            warningMes;
            printf("in convertStringToUnsignedInt, ");
            printf("Include Invalied charactor, value : %s.\n", string);
            return 0;
        }
    }
    return tmp;
}

unsigned int convertStringToUnsignedInt(char *string)
{
    unsigned int tmp=0;
    unsigned int strLengh=0;
    unsigned int i=0;

    if(!string)
    {
        return 0;
    }

    strLengh=strlen(string);
    for(i=0; i<strLengh; i++)
    {
        if(('0'<=*(string+i))&&((*(string+i))<='9'))
        {
            tmp*=10;
            tmp+=((*(string+i))-'0');
        }
        else
        {
            warningMes;
            printf("in convertStringToUnsignedInt, ");
            printf("Include Invalied charactor, value : %s.\n", string);
            return 0;
        }
    }
    return tmp;
}

unsigned long convertStringToUnsignedLong(char *string)
{
    unsigned long tmp=0;
    unsigned int strLengh=0;
    unsigned int i=0;

    if(!string)
    {
        return 0;
    }

    strLengh=strlen(string);
    for(i=0; i<strLengh; i++)
    {
        if(('0'<=*(string+i))&&((*(string+i))<='9'))
        {
            tmp*=10UL;
            tmp+=((*(string+i))-'0');
        }
        else
        {
            warningMes;
            printf("in convertStringToUnsignedLong, ");
            printf("Include Invalied charactor, value : %s.\n", string);
            return 0;
        }
    }
    return tmp;
}
