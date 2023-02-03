/* Constructor and Destructor */
struct_errorComponent *createErrorComponent(unsigned int length)
{
    struct_errorComponent *p;

    #ifndef RELEASE
    if(!length)
    {
        errorMes;
        printf("in createErrorComponent, unsigned int length is zero.\n");
        return NULL;
    }
    #endif

    p=(struct_errorComponent*)malloc(sizeof(struct_errorComponent));
    memset(p, 0, sizeof(struct_errorComponent));

    p->erroredCodeWord=createPowerFormPolynomial(length);
    p->errorLocationVector=createPowerFormPolynomial(length);
    return p;
}

char closeErrorComponent(struct_errorComponent **p)
{
    #ifndef RELEASE
    if(!p)
    {
        warningMes;
        printf("in closeErrorComponent, struct_errorComponent **p is null.\n");
        return -1;
    }
    if(!*p)
    {
        warningMes;
        printf("in closeErrorComponent, struct_errorComponent *p is null.\n");
        return -1;
    }
    #endif

    closePowerFormPolynomial(&((*p)->erroredCodeWord));
    closePowerFormPolynomial(&((*p)->errorLocationVector));
    free(*p);
    (*p)=NULL;
    return 0;
}

struct_errorComponent *recreateErrorComponent(struct_errorComponent **p, unsigned int length)
{
    #ifndef RELEASE
    if(!p)
    {
        errorMes;
        printf("in recreateErrorComponent, struct_errorComponent **p is NULL.\n");
        return NULL;
    }
    #endif
    if(*p)
    {
        #ifndef RELEASE
        if(closeErrorComponent(p))
        {
            warningMes;
            printf("in recreateErrorComponent, closeErrorComponent(p) is fail.\n");
        }
        #else
        closeErrorComponent(p);
        #endif
    }
    *p=createErrorComponent(length);
    return *p;
}

/* Operation */
struct_errorComponent *ifNotExistCreateErrorComponent(struct_errorComponent **p, unsigned int length)
{
    #ifndef RELEASE
    if(!p)
    {
        errorMes;
        printf("in recreateErrorComponent, struct_errorComponent **p is NULL.\n");
        return NULL;
    }
    #endif
    if(*p)
    {
        #ifndef RELEASE
        if(closeErrorComponent(p))
        {
            warningMes;
            printf("in recreateErrorComponent, closeErrorComponent(p) is fail.\n");
        }
        #else
        closeErrorComponent(p);
        #endif
    }
    else
    {
        *p=createErrorComponent(length);
    }
    return *p;
}

char *randdomInfoBitStreamChanger(unsigned int infoBitsLength, unsigned int parityBitsLength, char *p)
{
    unsigned int bitLength=infoBitsLength+parityBitsLength;
    unsigned int i, j;
    double numberOfSameBits;
    double probability;

    if(!p)
    {
        p=(char*)malloc(sizeof(char)*(infoBitsLength+parityBitsLength+1));

        warningMes;
        printf("in randdomInfoBitStreamChanger, memory allocation. size is '%d'+1='%d'\n", infoBitsLength+parityBitsLength,infoBitsLength+parityBitsLength+1);
    }
    *(p+infoBitsLength+parityBitsLength)=0;

            #ifndef RELEASE
            if((global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)==FLAG_MASK_PRINTF_LOG)
            {
                infoMes;
                printf("in randdomInfoBitStreamChanger, code length is \'%d\', info length is \'%d\', parity length is \'%d\'\n", bitLength, infoBitsLength, parityBitsLength);
            }
            if((global_flag_cmdOption&FLAG_MASK_PRINTF_CALCULATE_PROCESS)==FLAG_MASK_PRINTF_CALCULATE_PROCESS)
            {
                calculateMes;
                printf("in randdomInfoBitStreamChanger, Generated info Bits is...\n");
            }
            #endif

    j=0;
    for(i=0; i<bitLength; i++)
    {
        if(i<infoBitsLength)
        {
            if(!j)
            {
                probability=(double)((double)rand() / ((double)RAND_MAX + 1));
                numberOfSameBits=((double)rand() / ((double)RAND_MAX + 1) * 5)+1;
                //probability=(double)((double)rand_r(&tmp_seed) / ((double)RAND_MAX + 1));
                //numberOfSameBits=((double)rand_r(&tmp_seed) / ((double)RAND_MAX + 1) * 5)+1;

                j=(unsigned int)numberOfSameBits;
            }
            if(probability>0.5)
            {
                *(p+i) = '1';
            }
            else
            {
                *(p+i) = '0';
            }
            j--;
        }
        else
        {
            *(p+i) = '0';
        }
                #ifndef RELEASE
                if((global_flag_cmdOption&FLAG_MASK_PRINTF_CALCULATE_PROCESS)==FLAG_MASK_PRINTF_CALCULATE_PROCESS)
                {
                    printf("%c(%d)",*(p+i), i);
                    if((global_flag_cmdOption&FLAG_MASK_PRINTF_CALCULATE_PROCESS)==FLAG_MASK_PRINTF_CALCULATE_PROCESS)
                    {
                        if((i+1)==infoBitsLength)
                        {
                            printf("[infoBitsEnd]");
                        }
                    }
                }
                #endif
    }

            #ifndef RELEASE
            if((global_flag_cmdOption&FLAG_MASK_PRINTF_CALCULATE_PROCESS)==FLAG_MASK_PRINTF_CALCULATE_PROCESS)
            {
                printf("\n");
            }
            if((global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)==FLAG_MASK_PRINTF_LOG)
            {
                infoMes;
                printf("randdomInfoBitStreamChanger is end.\n");
            }
            #endif
    return p;
}

char *randdomInfoBitChanger(unsigned int infoBitsLength, unsigned int parityBitsLength, char *p)
{
    unsigned int bitLength=infoBitsLength+parityBitsLength;
    unsigned int i;

    if(!p)
    {
        p=(char*)malloc(sizeof(char)*(infoBitsLength+parityBitsLength+1));

        warningMes;
        printf("in randdomInfoBitChanger, memory allocation. size is '%d'+1='%d'\n", infoBitsLength+parityBitsLength,infoBitsLength+parityBitsLength+1);
    }
    *(p+infoBitsLength+parityBitsLength)=0;

            #ifndef RELEASE
            if((global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)==FLAG_MASK_PRINTF_LOG)
            {
                infoMes;
                printf("in randdomInfoBitChanger, code length is \'%d\', info length is \'%d\', parity length is \'%d\'\n", bitLength, infoBitsLength, parityBitsLength);
            }
            if((global_flag_cmdOption&FLAG_MASK_PRINTF_CALCULATE_PROCESS)==FLAG_MASK_PRINTF_CALCULATE_PROCESS)
            {
                calculateMes;
                printf("in randdomInfoBitChanger, Generated info Bits is...\n");
            }
            #endif

    for(i=0; i<bitLength; i++)
    {
        if(rand()%2)
        {
            *(p+i) = '1';
        }
        else
        {
            *(p+i) = '0';
        }
    }

            #ifndef RELEASE
            if((global_flag_cmdOption&FLAG_MASK_PRINTF_CALCULATE_PROCESS)==FLAG_MASK_PRINTF_CALCULATE_PROCESS)
            {
                printf("\n");
            }
            if((global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)==FLAG_MASK_PRINTF_LOG)
            {
                infoMes;
                printf("randdomInfoBitChanger is end.\n");
            }
            #endif
    return p;
}

char *allSameInfoBitGenerator(char initChar, unsigned int infoBitsLength, unsigned int parityBitsLength, char **p)
{
    if(!(*p))
    {
        (*p)=(char*)malloc(sizeof(char)*(infoBitsLength+parityBitsLength+1));
        memset((*p), initChar, sizeof(char)*(infoBitsLength+parityBitsLength));
        *((*p)+infoBitsLength+parityBitsLength) = 0;
    }
    return (*p);
}

char *randdomInfoBitGenerator(unsigned int infoBitsLength, unsigned int parityBitsLength, char **p)
{
    if(!(*p))
    {
        (*p)=(char*)malloc(sizeof(char)*(infoBitsLength+parityBitsLength+1));
    }

    return randdomInfoBitChanger(infoBitsLength, parityBitsLength, *p);
    //return randdomInfoBitStreamChanger(infoBitsLength, parityBitsLength, *p);
}
