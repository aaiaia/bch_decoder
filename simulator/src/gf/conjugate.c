/* Constructor and Destructor */
struct_setOfGaloisFieldElements *createConjugacyClasses(struct_galoisFieldElements *galoisFields)
{
    struct_setOfGaloisFieldElements *p;
    unsigned int numberOfConjugatesSet;
    unsigned int restSetOfConjugates;

    unsigned int i;

    #ifndef RELEASE
    if(!galoisFields)
    {
        errorMes;
        printf("in createConjugacyClasses, struct_galoisFieldElements *galoisFields is NULL.\n");
        return NULL;
    }
    #endif

    p=(struct_setOfGaloisFieldElements*)malloc(sizeof(struct_setOfGaloisFieldElements));
    memset(p, 0, sizeof(struct_setOfGaloisFieldElements));

    numberOfConjugatesSet=(galoisFields->length);


    restSetOfConjugates=numberOfConjugatesSet%((*(galoisFields->element+0))->length);
    numberOfConjugatesSet/=((*(galoisFields->element+0))->length);



    ////setup number of conjugate set
    p->length=numberOfConjugatesSet;

            #ifndef RELEASE
            if((global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)==FLAG_MASK_PRINTF_LOG)
            {
                logMes;
                printf("in createConjugacyClasses,\n");
                printf("p->length(is that mean number of conjugates set) is \'%d\'\n", p->length);
                if(restSetOfConjugates)
                {
                    printf("all conjugates set have \'%d\' elements.\n", ((*(galoisFields->element+0))->length));
                }
                else
                {
                    printf("except lastest, conjugates set have \'%d\' elements.\n", ((*(galoisFields->element+0))->length));
                    printf("lastest conjugates set have \'%d\'\n", restSetOfConjugates);
                }
            }
            #endif



    ////making conjugacy set
    p->conjugateSet=(struct_galoisFieldElements**)malloc(sizeof(struct_galoisFieldElements*)*p->length);
    memset(p->conjugateSet, 0, sizeof(struct_galoisFieldElements*)*p->length);
    p->limitedConjugateSet=(struct_galoisFieldElements**)malloc(sizeof(struct_galoisFieldElements*)*p->length);
    memset(p->limitedConjugateSet, 0, sizeof(struct_galoisFieldElements*)*p->length);
    #ifdef USING_OPEN_MP
    #pragma omp parallel for schedule(guided) private(i) shared(p, galoisFields)
    #endif
    for(i=0;i<p->length; i++)//to except lastes one, looping numberOfConjugatesSet-1;
    {
        *(p->conjugateSet+i)=createGaloisFieldExceptElements(((*(galoisFields->element+0))->length));
        *(p->limitedConjugateSet+i)=createGaloisFieldExceptElements(((*(galoisFields->element+0))->length));
    }

    return p;
}

struct_setOfGaloisFieldElements *createConjugacyClasses_staticSetNums(unsigned int setLength, unsigned int *setElementLength)
{
    struct_setOfGaloisFieldElements *p;

    unsigned int i;

    p=(struct_setOfGaloisFieldElements*)malloc(sizeof(struct_setOfGaloisFieldElements));
    memset(p, 0, sizeof(struct_setOfGaloisFieldElements));

    p->length=setLength;

    ////making conjugacy set
    p->conjugateSet=(struct_galoisFieldElements**)malloc(sizeof(struct_galoisFieldElements*)*setLength);
    memset(p->conjugateSet, 0, sizeof(struct_galoisFieldElements*)*setLength);
    p->limitedConjugateSet=(struct_galoisFieldElements**)malloc(sizeof(struct_galoisFieldElements*)*setLength);
    memset(p->limitedConjugateSet, 0, sizeof(struct_galoisFieldElements*)*setLength);
    #ifdef USING_OPEN_MP
    #pragma omp parallel for schedule(guided) private(i) shared(p)
    #endif
    for(i=0;i<setLength; i++)//to except lastes one, looping numberOfConjugatesSet-1;
    {
        *(p->conjugateSet+i)=createGaloisFieldExceptElements(setElementLength[i]);
        *(p->limitedConjugateSet+i)=createGaloisFieldExceptElements(setElementLength[i]);
    }

    return p;
}

struct_setOfGaloisFieldElements *createConjugacyClasses_VariableLength()
{
    struct_setOfGaloisFieldElements *p;

    p=(struct_setOfGaloisFieldElements*)malloc(sizeof(struct_setOfGaloisFieldElements));
    memset(p, 0, sizeof(struct_setOfGaloisFieldElements));
    //p->length=0;//it is already reset.

    ////making conjugacy set
    //p->conjugateSet=NULL;//it is already reset.

    return p;
}

char closeConjugacyClasses(struct_setOfGaloisFieldElements **p)
{
    unsigned int i;

    #ifndef RELEASE
    if(!p)
    {
        errorMes;
        printf("in closeConjugacyClasses, struct_setOfGaloisFieldElements **p is NULL.\n");
        return -1;
    }

    {
        warningMes;
        printf("in closeConjugacyClasses, struct_setOfGaloisFieldElements *p is NULL.\n");
        return -1;
    }
    #endif

    #ifdef USING_OPEN_MP
    #pragma omp parallel for schedule(guided) private(i) shared(p)
    #endif
    for(i=0; i<(*p)->length; i++)
    {
        #ifdef RELEASE
        closeGaloisFieldExceptElements(((*p)->conjugateSet+i));
        closeGaloisFieldExceptElements(((*p)->limitedConjugateSet+i));

        #else
        if(closeGaloisFieldExceptElements(((*p)->conjugateSet+i)))//if(closeGaloisFieldExceptElements(&(*((*p)->conjugateSet+i))))
        {
            errorMes;
            printf("in closeConjugacyClasses, closeGaloisFieldExceptElements(*(p->conjugateSet+%d)) return any value.\n", i);
        }
        if(closeGaloisFieldExceptElements(((*p)->limitedConjugateSet+i)))//if(closeGaloisFieldExceptElements(&(*((*p)->conjugateSet+i))))
        {
            errorMes;
            printf("in closeConjugacyClasses, closeGaloisFieldExceptElements(((*p)->limitedConjugateSet+%d)) return any value.\n", i);
        }
        #endif

    }

            #ifndef RELEASE
            if((global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)==FLAG_MASK_PRINTF_LOG)
            {
                logMes;
                printf("in closeConjugacyClasses, before process free(p->conjugateSet);\n");
            }
            #endif

    free((*p)->conjugateSet);
    free((*p)->limitedConjugateSet);

            #ifndef RELEASE
            if((global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)==FLAG_MASK_PRINTF_LOG)
            {
                logMes;
                printf("in closeConjugacyClasses, before process free(p);\n");
            }
            #endif
    free(*p);
    *p=NULL;
    return 0;
}

struct_setOfGaloisFieldElements *recreateConjugacyClasses(struct_setOfGaloisFieldElements **p, struct_galoisFieldElements *galoisFields)
{
    if(*p)
    {
                #ifndef RELEASE
                if((global_flag_cmdOption&FLAG_MASK_PRINTF_LOG))
                {
                    printf("in recreateConjugacyClasses, struct_setOfGaloisFieldElements *p is already have data.\n");
                    printf("it will be removed.\n");
                }
                #endif
        closeConjugacyClasses(p);
    }
    *p=createConjugacyClasses(galoisFields);
    return *p;
}

/* Configure */
char setLengthOfLimittedLengthOfConjugacyClasse(struct_setOfGaloisFieldElements *p, unsigned int limitedExponential)
{
    #ifndef RELEASE
    if(!p)
    {
        errorMes;
        printf("in setLengthOfLimittedLengthOfConjugacyClasse, struct_setOfGaloisFieldElements *p is NULL\n");
        return -1;
    }
    #endif

    p->limitedExponential = limitedExponential;
    return 0;
}

char addLengthOfConjugacySet_VariableLength(struct_galoisFieldElements *galoisFields, struct_setOfGaloisFieldElements *p, unsigned int addedLength)
{
    unsigned int i;
    struct_galoisFieldElements **newConjugateSet=NULL;
    struct_galoisFieldElements **newLimitedConjugateSet=NULL;


    #ifndef RELEASE
    if(!p)
    {
        errorMes;
        printf("in addConjugacyClassesVariableLength, struct_setOfGaloisFieldElements *p is NULL.\n");
        return -1;
    }
    if(!addedLength)
    {
        warningMes;
        printf("in addConjugacyClassesVariableLength, unsigned int addedLength is 0.\n");
        return 0;
    }
    #endif

    ////making conjugacy set array and initializing
    if(!(p->length))
    {
        p->conjugateSet=(struct_galoisFieldElements**)malloc(sizeof(struct_galoisFieldElements*)*addedLength);//making conjugacy set array

        p->limitedConjugateSet=(struct_galoisFieldElements**)malloc(sizeof(struct_galoisFieldElements*)*addedLength);//making conjugacy set array
        //memset(p->conjugateSet, 0, sizeof(struct_galoisFieldElements*)addedLength);

        #ifdef USING_OPEN_MP
        #pragma omp parallel for schedule(guided) private(i) shared(addedLength, p)
        #endif
        for(i=0; i<addedLength; i++)
        {
            *(p->conjugateSet+i)=createGaloisFieldExceptElements_VariableLength();

            *(p->limitedConjugateSet+i)=createGaloisFieldExceptElements_VariableLength();
        }
        p->length=addedLength;
    }
    else
    {
        /* Sometime realloc has memory leakage. So to increase memory size, using this func is invalid. */
        //p->conjugateSet=(struct_galoisFieldElements**)realloc(p->conjugateSet, sizeof(struct_galoisFieldElements*)*addedLength);//increasing conjugacy set array
        newConjugateSet=(struct_galoisFieldElements**)malloc((sizeof(struct_galoisFieldElements*)*((p->length)+addedLength)));
        newLimitedConjugateSet=(struct_galoisFieldElements**)malloc((sizeof(struct_galoisFieldElements*)*((p->length)+addedLength)));

        memset(newConjugateSet , 0, (sizeof(struct_galoisFieldElements*)*(p->length+addedLength)));
        memset(newLimitedConjugateSet , 0, (sizeof(struct_galoisFieldElements*)*(p->length+addedLength)));

        memcpy(newConjugateSet, p->conjugateSet, (sizeof(struct_galoisFieldElements*)*(p->length)));
        memcpy(newLimitedConjugateSet, p->limitedConjugateSet, (sizeof(struct_galoisFieldElements*)*(p->length)));

        free(p->conjugateSet);
        free(p->limitedConjugateSet);

        p->conjugateSet=newConjugateSet;
        p->limitedConjugateSet=newLimitedConjugateSet;

        //clear added conjugacy set
        #ifdef USING_OPEN_MP
        #pragma omp parallel for schedule(guided) private(i) shared(addedLength, p)
        #endif
        for(i=p->length; i<((p->length)+addedLength); i++)
        {
            *(p->conjugateSet+i)=createGaloisFieldExceptElements_VariableLength();

            *(p->limitedConjugateSet+i)=createGaloisFieldExceptElements_VariableLength();
        }
        p->length+=addedLength;
    }

    return 0;
}
