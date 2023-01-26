/* handling galois field except elements */
struct_galoisFieldElements *createGaloisFieldExceptElements(unsigned int numberOfElement)
{
    struct_galoisFieldElements *p;

    if(!numberOfElement)
    {
                #ifndef RELEASE
                errorMes; printf("in *createGaloisFieldExceptElements, unsigned int numberOfElement is ZERO\n");
                #endif
        return NULL;
    }
    p=(struct_galoisFieldElements*)malloc(sizeof(struct_galoisFieldElements));
    memset(p, 0, sizeof(struct_galoisFieldElements));

    p->length=numberOfElement;
    p->element=createListOfGaloisField(numberOfElement);
    return p;
}

struct_galoisFieldElements *createGaloisFieldExceptElements_VariableLength()
{
    struct_galoisFieldElements *p;


    p=(struct_galoisFieldElements*)malloc(sizeof(struct_galoisFieldElements));
    memset(p, 0, sizeof(struct_galoisFieldElements));

    //p->length=numberOfElement;
    //p->element=createListOfGaloisField(numberOfElement);
    return p;
}

char addLengthOfGaloisFieldExceptElements_VariableLength(struct_galoisFieldElements *p, unsigned int addedLength)
{
    #ifndef RELEASE
    if(!p)
    {
        errorMes;
        printf("in *addedLengthOfGaloisFieldExceptElements, struct_galoisFieldElements *p is NULL\n");
        return -1;
    }
    if(!addedLength)
    {
        errorMes;
        printf("in *addedLengthOfGaloisFieldExceptElements, unsigned int addedLength is ZERO\n");
        return -1;
    }
    #endif


    if(!(p->length))
    {
        if(!(p->element))
        {
            p->element=createListOfGaloisField(addedLength);
        }
        else
        {
            p->element=recreateListOfGaloisField(&(p->element), addedLength);
        }
        p->length=addedLength;
    }
    else
    {
        if(addLengthOfListOfGaloisField_VariableLength(&(p->element), p->length, addedLength))
        {
                    #ifndef RELEASE
                    warningMes; printf("in addLengthOfGaloisFieldExceptElements_VariableLength, addLengthOfListOfGaloisField_VariableLength(p->element, p->length, addedLength) is return to Not ZERO.\n");
                    #endif
            return -1;
        }
        else
        {
            p->length+=addedLength;
        }
    }
    return 0;
}

char closeGaloisFieldExceptElements(struct_galoisFieldElements **p)
{
    #ifndef RELEASE
    if(!p)
    {
        errorMes;
        printf("in closeGaloisFieldExceptElements, struct_galoisFieldElements **p is NULL\n");
        return -1;
    }
    if(!*p)
    {
        warningMes;
        printf("in closeGaloisFieldExceptElements, struct_galoisFieldElements *p is NULL\n");
        return -1;
    }
    #endif

            #ifndef RELEASE
            if((global_flag_cmdOption&FLAG_MASK_PRINTF_LOG))
            {
                infoMes;
                printf("closeGaloisFieldExceptElements, struct_galoisFieldElements p = \'0x%lx\', struct_galoisFieldElements *p = \'0x%lx\'\n", (unsigned long)p, (unsigned long)*p);
            }
            if(global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)
            {
                infoMes;
                printf("in closeGaloisFieldExceptElements, &((*p)->element)) = \'0x%lx\'\n", (unsigned long)&((*p)->element));
                printf("in closeGaloisFieldExceptElements, ((*p)->element)) = \'0x%lx\'\n", (unsigned long)((*p)->element));
            }
            #endif
    if(closeListOfGaloisField(&((*p)->element)))
    {
        #ifndef RELEASE
        errorMes; printf("in closeGaloisFieldExceptElements, closeListOfGaloisField(&((*p)->element)) is fail.\n");
        #endif
    }
    free((*p));
    *p=NULL;
    return 0;
}

struct_galoisFieldElements *recreateGaloisFieldExceptElements(struct_galoisFieldElements **p, unsigned int numberOfElement)
{
    if(*p)
    {
                #ifndef RELEASE
                if((global_flag_cmdOption&FLAG_MASK_PRINTF_LOG))
                {
                    infoMes;
                    printf("in recreateGaloisFieldExceptElements, struct_galoisFieldElements *p have data already.\n");
                    printf("it will be recreated.\n");
                }
                #endif
        closeGaloisFieldExceptElements(p);
    }
    *p=createGaloisFieldExceptElements(numberOfElement);
    return *p;
}

/* handling galois field list and elements */
struct_galoisFieldElements *createGaloisFieldAndElements(unsigned int numberOfElement, unsigned int polyLength)
{
    struct_galoisFieldElements *p;

    #ifndef RELEASE
    if(!numberOfElement)
    {
        errorMes;
        printf("in *createGaloisFieldAndElements, GaloisFielsElements numberOfElement is %d\n", numberOfElement);
        return NULL;
    }
    #endif

    p = (struct_galoisFieldElements*)malloc(sizeof(struct_galoisFieldElements));
    memset(p, 0, sizeof(struct_galoisFieldElements));

    p->length=numberOfElement;

    p->element = createListOfGaloisFieldAndComponents(numberOfElement, polyLength);

            #ifndef RELEASE
            if((global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)==FLAG_MASK_PRINTF_LOG)
            {
                logMes;
                printf("GaloisFielsElements length is %d\n", numberOfElement);
            }
            #endif
    return p;
}

char closeGaloisFielsAndElements(struct_galoisFieldElements **p)
{
    #ifndef RELEASE
    if(!p)
    {
        errorMes;
        printf("in closeGaloisFielsAndElements, **p addr = 0x%lx\n", ((unsigned long)p));
        return -1;
    }
    if(!(*p))
    {
        warningMes;
        printf("in closeGaloisFielsAndElements, *p addr = 0x%lx\n", ((unsigned long)*p));
        return -1;
    }
    #endif

    closeListOfGaloisFieldAndElements(&((*p)->element), (*p)->length);

    free((*p));
    *p=NULL;
    return 0;
}

struct_galoisFieldElements *recreateGaloisFieldAndElements(struct_galoisFieldElements **p, unsigned int numberOfElement, unsigned int polyLength)
{
    if(*p)
    {
                #ifndef RELEASE
                if(global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)
                {
                    infoMes;
                    printf("in recreateGaloisFieldAndElements, struct_galoisFieldElements *p have data already.\n");
                    printf("it will be recreated.\n");
                }
                #endif
        closeGaloisFielsAndElements(p);
    }
    *p=createGaloisFieldAndElements(numberOfElement, polyLength);
    return *p;
}

/* Constructor and Desctructor */
struct_galoisFieldElements *createGaloisField(struct_powerFormPolynomials *primitivePolynomial, unsigned int numberOfElement, unsigned int polyLength)
{
    struct_galoisFieldElements *p=NULL;
    if(!(p=createGaloisFieldAndElements(numberOfElement, polyLength)))
    {
        errorMes; printf("in createGaloisField, createGaloisFieldAndElements return NULL.\r\n");
        return p;
    }

    if(calculateGaloisField(primitivePolynomial, p))
    {
        errorMes; printf("in createGaloisField, calculateGaloisField return not 0.\r\n");
        closeGaloisFielsAndElements(&p);
        return NULL;
    }
    return p;
}

char closeGaloisField(struct_galoisFieldElements **p)
{
    #ifndef RELEASE
    if(!p)      return -1;
    if(!(*p))   return -2;
    #endif

    if(closeGaloisFielsAndElements(p))
    {
        return -3;
    }
    return 0;
}

char calculateGaloisField(struct_powerFormPolynomials *primitivePolynomial, struct_galoisFieldElements *p)
{
    unsigned int i,j;

    char_GALOIS_FIELD_VALUE shiftBuffer;

    #ifndef RELEASE
    if(!p)
    {
        return -1;
    }

    if(!primitivePolynomial)
    {
        return -3;
    }
    #endif


    *((*(p->element+1))->value+0)='1';
    //printf("((*(p->element+1))->length) %d <= (sizeof(u_int_GALOIS_FIELD_INTEGER)*8) %d\n", ((*(p->element+1))->length), (sizeof(uint32_t)*8));
    if(((*(p->element+1))->length)<=(sizeof(u_int_GALOIS_FIELD_INTEGER)*8))
    {
        (*(p->element+1))->intValue=convertGaloisFieldPolyFormUnsignedInt(*(p->element+1));
    }
    (*(p->element+1))->index=1;

            #ifndef RELEASE
            if((global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)==FLAG_MASK_PRINTF_LOG)
            {
                logMes;
                printf("(p->length) = %d\n", (p->length));
                printf("(*(p->element+0))->length = %d\n", (*(p->element+0))->length);
            }
            #endif


    //printf("[%05d]%s\n", 0,(*(p->element+0))->value);
    //printf("[%05d]%s\n", 1,(*(p->element+1))->value);
    infoMes; printf("[START][Generating GaloisField Elements]\n");
    for(i=2; i<(p->length); i++)
    {
        shiftBuffer=(*((*(p->element+i-1))->value+((*(p->element+i))->length-1)));
        for(j=(*(p->element+i))->length-1; j!=0; j--)
        {
            (*((*(p->element+i))->value+j))=(*((*(p->element+i-1))->value+j-1));
        }
        (*((*(p->element+i))->value+j))='0';
        (*((*(p->element+i))->value+(*(p->element+i))->length))=0;
        //(*((*(p->element+i-1))->value+((*(p->element+i))->length-1)))=0;

        if(shiftBuffer=='1')
        {
            for(j=0; j<(*(p->element+i))->length; j++)
            {
                if((*(primitivePolynomial->equation+j))!=(*((*(p->element+i))->value+j)))
                {
                    (*((*(p->element+i))->value+j))='1';
                }
                else
                {
                    (*((*(p->element+i))->value+j))='0';
                }
            }
            shiftBuffer='0';
        }


        if(((*(p->element+i))->length)<=(sizeof(u_int_GALOIS_FIELD_INTEGER)*8))
        {
            (*(p->element+i))->intValue=convertGaloisFieldPolyFormUnsignedInt(*(p->element+i));
        }
        (*(p->element+i))->index=i;

        //printf("[%05d]%s\n", i,(*(p->element+i))->value);
    }
    infoMes; printf("[END][Generating GaloisField Elements]\n");

    /*
    //#ifndef RELEASE
    //if(global_flag_gfCommon_display&FLAG_MASK_DISPLAY_GF_COMMON_FIELD)
    //{
    //    initMes; printGaloisField(p);
    //}
    //#endif
    */

        /*
         * Update. 2016/02/22
         * seperated this parts, creating parts of conjugacy class is moved init_ConjugacyClasses
         */
        /* create conjugacy Class (static) */
        //p->conjugacies=createConjugacyClasses(p);
        //calculateConjugacyClasses(p, p->conjugacies);

        /* create conjugacy Class (variable) */
        //p->conjugacies=createConjugacyClasses_VariableLength();
        //
        //infoMes; printf("[START][Generating conjugacy set]\n");
        //calculateConjugacyClasses_VariableLength(p, p->conjugacies);
        //infoMes; printf("[END][Generating conjugacy set]\n");
        //
        //#ifndef RELEASE
        //if(global_flag_gfCommon_display&FLAG_MASK_DISPLAY_GF_COMMON_CONJUGACIES)
        //{
        //    initMes;
        //    printConjugacyClasses(p, p->conjugacies);
        //}
        //#endif

    return 0;
}
