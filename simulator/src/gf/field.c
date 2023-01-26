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

/* handling galois field except elements */
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
