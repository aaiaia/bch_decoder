char *convertPrimeNumberToString(char *string, unsigned int exponential, const unsigned int *primeNumberArray)
{
    int i;

    #ifndef RELEASE
    if(!string)
    {
        return NULL;
    }
    if(!string)
    {
        return NULL;
    }
    if(exponential<2)
    {
        return NULL;
    }
    #endif

    for(i=31; 0<=i; i--)
    {
        if(((*(primeNumberArray+exponential-2))>>i)&1)
        {
            *(string+i+1)=0;
            break;
        }
    }
    for(; 0<=i; i--)
    {
        if(((*(primeNumberArray+exponential-2))>>i)&1)
        {
            printf("%d", ((*(primeNumberArray+exponential-2))>>i)&1);
            *(string+i)='1';
        }
        else
        {
            printf("%d", ((*(primeNumberArray+exponential-2))>>i)&1);
            *(string+i)='0';
        }
    }
    printf("\n");
    return string;
}
