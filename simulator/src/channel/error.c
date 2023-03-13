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

/* Generate test pattern using LLR locator */
/*
 * Using string type of codeword(binary) and error locator,
 * generate a test pattern(is binary and string)
 */
char *temporaryFunc_generateTestPatternToString(char *string, unsigned int stringLength, unsigned int *locatorArray, unsigned int locatorArrayLength)
{
    unsigned int tmpStringLength=strlen(string);
    unsigned int i;

    char *p;
    #ifdef USING_OPEN_MP
        unsigned int OMP_locatorArrayLength;
        unsigned int *OMP_locatorArray
        char *OMP_p;
    #endif
    //        #pragma omp for private(i) shared(locatorArrayLength, locatorArray, p)
    #ifndef RELEASE
    if(!string)
    {
        errorMes;
        printf("in temporaryFunc_generateTestPatternToString, \n");
        return NULL;
    }
    if(!locatorArray)
    {
        errorMes;
        printf("in temporaryFunc_generateTestPatternToString, \n");
        return NULL;
    }
    if(!locatorArrayLength)
    {
        errorMes;
        printf("in temporaryFunc_generateTestPatternToString, \n");
        return NULL;
    }
    #endif

    if(!stringLength)
    {
        stringLength=tmpStringLength;
    }
    p=(char*)malloc(sizeof(char)*stringLength+1);
    memcpy(p, string, sizeof(char)*tmpStringLength);
    *(p+tmpStringLength)=0;
    //memset(p, '0', sizeof(char)*stringLength);
    //*(p+stringLength)=0;

                        #ifndef RELEASE
                        if(global_flag_debug_awgnLLR&FLAG_MASK_DEBUG_AWGN_LLR_LOCATOR)
                        {
                            debugAwgnLlrLocatorMes;
                            for(i=0; i<locatorArrayLength; i++)
                            {
                                if((*(locatorArray+i)) == -1)
                                {
                                    printf("operation that is if((*(locatorArray+i)) == -1) is true.\n");
                                    printf("If you want continue simulation, input any string : ");
                                    scanf("%s",global_buf_KeyBoardInput);
                                }
                            }
                        }
                        #endif



    #ifdef USING_OPEN_MP
    #pragma omp parallel
    #endif
    {
        #ifdef USING_OPEN_MP
        #pragma omp for private(i) shared(locatorArrayLength, locatorArray, p)
        #endif
        for(i=0; i<locatorArrayLength; i++)
        {
            if((*(locatorArray+i)) == -1) continue;
            if((*(p+(*(locatorArray+i))))=='0')
            {
                (*(p+(*(locatorArray+i))))='1';
            }
            else if((*(p+(*(locatorArray+i))))=='1')
            {
                (*(p+(*(locatorArray+i))))='0';
            }
            else
            {
                #ifdef USING_OPEN_MP
                if((*(p+(*(locatorArray+i))))%2)
                {
                    *(p+(*(locatorArray+i)))='0';
                }
                else
                {
                    *(p+(*(locatorArray+i)))='1';
                }
                #else
                if(rand()%2)
                {
                    *(p+(*(locatorArray+i)))='0';
                }
                else
                {
                    *(p+(*(locatorArray+i)))='1';
                }
                #endif
            }
        }
    }


    return p;
}

/*
 * Using string type of codeword(binary) and error locator,
 * generate a test patterns, are binary and string
 */
char temporaryFunc_generateTestPatterns_using_LLR_Locator
    (
        char *refferString,
        unsigned int refferStringLength,
        unsigned int *locatorArray,
        unsigned int locatorArrayLength,
        char **testPattern,
        unsigned int numsOfTestPattern
    )
{
    unsigned int tmpStringLength=0;

    unsigned int i=0, j=0;
    //unsigned char usingBitNumber=sizeof(i)*8;

    //unsigned char usingBitNumber = (unsigned char)(log((double)locatorArrayLength)/log(2.0));
    unsigned char usingBitNumber = locatorArrayLength;
    //for(i=0; i<sizeof(usingBitNumber)*8; i++)
    //{
    //    if(locatorArrayLength&(1<<i)) break;
    //}
    //usingBitNumber=i;

    #ifndef RELEASE
    if(!testPattern)
    {
        errorMes;
        printf("in temporaryFunc_generateTestPatterns_using_LLR_Locator, testPattern is NULL.\n");
        return -1;
    }
    //if(!*testPattern)
    //{
    //    errorMes;
    //    printf("in temporaryFunc_generateTestPatterns_using_LLR_Locator, *testPattern is NULL.\n");
    //    return -1;
    //}
    #endif

    if(!refferStringLength)
    {
        tmpStringLength=strlen(refferString);
        refferStringLength=tmpStringLength;
    }

    testPattern[0]=refferString;//printf("\n");

    //#ifdef USING_OPEN_MP
    //    #pragma omp parallel for schedule(guided) private(i,j) shared(numsOfTestPattern, usingBitNumber, testPattern, locatorArray, refferString, refferStringLength)
    //#endif
    for(i=1; i<numsOfTestPattern; i++)//To generate Test pattern(is same with combination), index i is used to Mask. if we can select 4-case(00 01 10 11).
    {
                                        //printf("for(i=1; i<locatorArrayLength; i++)  i=%d\n", i);
            if((*(testPattern+i)))
            {
                free(*(testPattern+i));
                #ifndef RELEASE
                warningMes;    printf("free(*(testPattern+%d));\n", i);
                #endif
            }

        *(testPattern+i)=(char*)malloc(sizeof(char)*refferStringLength+1);
                                        //printf("*(testPattern+i)=(char*)malloc(sizeof(char)*refferStringLength+1);\n");
        #ifndef USING_OPEN_MP
        memcpy(*(testPattern+i), refferString, sizeof(char)*refferStringLength);
        #else
        #pragma omp parallel for schedule(guided) private(j) shared(refferStringLength, testPattern, i, refferString)
        for(j=0; j<refferStringLength; j++)
        {
            *((*(testPattern+i))+j) = *(refferString+j);
        }
        #endif
                                        //printf("memcpy(*(testPattern+i), refferString, sizeof(char)*refferStringLength);\n");
        *((*(testPattern+i))+refferStringLength)=0;
                                        //printf("*((*(testPattern+i))+refferStringLength)=0;\n");

        //use non-binary code calculate combination of bit position, j is mean that offset locator pointer
                                        //printf("for(j=0; j<usingBitNumber; j++)//j is mask\n");
        //#ifdef USING_OPEN_MP
        //#pragma omp parallel for schedule(guided) private(j) shared(usingBitNumber, locatorArray, testPattern)
        //#endif
        for(j=0; j<usingBitNumber; j++)//j is mask
        {
            if(i&(1<<j))//j is index of locatorArray.
            {
                //(*(locatorArray+j)) is location of codeword
                if((*(locatorArray+j))!=-1)
                {
                    if(*((*(testPattern+i))+(*(locatorArray+j)))=='0')
                    {
                        *((*(testPattern+i))+(*(locatorArray+j)))='1';
                    }
                    else if(*((*(testPattern+i))+(*(locatorArray+j)))=='1')
                    {
                        *((*(testPattern+i))+(*(locatorArray+j)))='0';
                    }
                    else
                    {
                        warningMesShort;    printf("character is not '0' or '1'.\r\n");
                        #ifdef USING_OPEN_MP
                        if((*((*(testPattern+i))+(*(locatorArray+j))))%2)
                        {
                            *((*(testPattern+i))+(*(locatorArray+j)))='0';
                        }
                        else
                        {
                            *((*(testPattern+i))+(*(locatorArray+j)))='1';
                        }
                        #else
                        if(rand()%2)
                        {
                            *((*(testPattern+i))+(*(locatorArray+j)))='0';
                        }
                        else
                        {
                            *((*(testPattern+i))+(*(locatorArray+j)))='1';
                        }
                        #endif
                    }
                }
            }
        }
    }
                        #ifndef RELEASE
                        if(global_flag_bch_BM_SimSD_display&FLAG_MASK_DISPLAY_PROGRESS)
                        {
                            test_BM_SoftDisplayMes;
                            printf("[Test Pattern Generation]\n");
                            #ifdef USING_OPEN_MP
                            #pragma omp parallel for schedule(guided) private(i) shared(numsOfTestPattern, testPattern)
                            #endif
                            for(i=0; i<numsOfTestPattern; i++)
                            {
                                test_BM_SoftDisplayMes; printf("[Test Pattern Generation %d]\n", i);
                                //printf("%s\n",((*(testPattern+i))+j));
                                    printf("%s\n",((*(testPattern+i))));

                            }
                        }
                        #endif
    return 0;
}
