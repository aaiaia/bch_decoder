/* Constructor and Destructor */
struct_summationMatrix *createSummationMatrix_emptySpace(unsigned int row, unsigned int column)
{
    unsigned int i;
    struct_summationMatrix *p;

    #ifndef RELEASE
    if(!row)
    {
        return NULL;
    }
    if(!column)
    {
        return NULL;
    }
    if(row!=column)
    {
        return NULL;
    }
    #endif


    p=(struct_summationMatrix*)malloc(sizeof(struct_summationMatrix));

    p->element=(struct_galoisFieldPolyForm ***)malloc(sizeof(struct_galoisFieldPolyForm **)*row);
    for(i=0; i<row; i++)
    {
        (*(p->element+i))=(struct_galoisFieldPolyForm **)malloc(sizeof(struct_galoisFieldPolyForm *)*column);
    }

    p->row=row;
    p->column=column;
    return p;
}

char closeSummationMatrix(struct_summationMatrix **p)
{
    unsigned int i;
    if(!p)
    {
        return -1;
    }
    if(!(*p))
    {
        return -1;
    }
    #ifdef USING_OPEN_MP
    #pragma omp parallel for schedule(guided) private(i) shared(p)
    #endif
    for(i=0; i<(*p)->row; i++)
    {
        free(*((*p)->element+i));
    }
    free((*p)->element);

    free(*p);

    *p=NULL;
    return 0;
}

/*
 *  !!! WARNING !!!
 *  Avoid to use functions are related with struct_galoisFieldPolyForm directly.
 *  As possible, use functions of struct_galoisFieldElements.
 */
struct_galoisFieldPolyForm **createListOfGaloisField(unsigned int lengthOfList)
{
    struct_galoisFieldPolyForm **p;

    #ifndef RELEASE
    if(!lengthOfList)
    {
        errorMes;
        printf("createListOfGaloisField lengthOfList is 0\n");
        return NULL;
    }
    #endif

    p=(struct_galoisFieldPolyForm**)malloc(sizeof(struct_galoisFieldPolyForm*)*lengthOfList);
    memset(p, 0, sizeof(struct_galoisFieldPolyForm*)*lengthOfList);

    return p;
}

/*
 * !!! WARNING !!!
 * Avoid to use functions are related with struct_galoisFieldPolyForm directly.
 * As possible, use functions of struct_galoisFieldElements.
 */
char addLengthOfListOfGaloisField_VariableLength(struct_galoisFieldPolyForm *(**p), unsigned int lengthOfList, unsigned int addedLengthOfList)
{

    struct_galoisFieldPolyForm **newGaloisFieldPolyForm=NULL;
    #ifndef RELEASE
    if(!(*p))
    {
        errorMes;
        printf("in addLengthOfListOfGaloisField_VariableLength, struct_galoisFieldPolyForm *(**p) is NULL\n");
        return -1;
    }
    if(!lengthOfList)
    {
        errorMes;
        printf("in addLengthOfListOfGaloisField_VariableLength, unsigned int lengthOfList is 0\n");
        return -1;
    }
    #endif


    #ifndef RELEASE
    if(!addedLengthOfList)
    {
        warningMes;
        printf("in addLengthOfListOfGaloisField_VariableLength, unsigned int addedLengthOfList is 0\n");
        return 0;
    }
    #endif



    /* Sometime realloc has memory leakage. So to increase memory size, using this func is invalid. */
    //p=(struct_galoisFieldPolyForm**)realloc(p, sizeof(struct_galoisFieldPolyForm*)*addedLengthOfList);
    newGaloisFieldPolyForm=(struct_galoisFieldPolyForm**)malloc(sizeof(struct_galoisFieldPolyForm*)*(lengthOfList+addedLengthOfList));
    memset(newGaloisFieldPolyForm+lengthOfList, 0, (sizeof(struct_galoisFieldPolyForm*)*(addedLengthOfList)));
    memcpy(newGaloisFieldPolyForm, (*p), (sizeof(struct_galoisFieldPolyForm*)*(lengthOfList)));
    free((*p));
    (*p)=newGaloisFieldPolyForm;

    return 0;
}


/*
 * !!! WARNING !!!
 * Avoid to use functions are related with struct_galoisFieldPolyForm directly.
 * As possible, use functions of struct_galoisFieldElements.
 */
char closeListOfGaloisField(struct_galoisFieldPolyForm ***p)
{
    #ifndef RELEASE
    if(!p)
    {
        errorMes;
        printf("closeListOfGaloisField ***p is NULL\n");
        return -1;
    }
    if(!(*p))
    {
        warningMes;
        printf("closeListOfGaloisField **p is NULL\n");
        return -1;
    }

    if(global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)
    {
        printf("in closeListOfGaloisField, struct_galoisFieldPolyForm p = \'0x%lx\'\n", (unsigned long)p);
        printf("in closeListOfGaloisField, struct_galoisFieldPolyForm *p = \'0x%lx\'\n", (unsigned long)*p);
    }
    #endif

    free (*p);
    *p=NULL;
    return 0;
}

/*
 * !!! WARNING !!!
 * Avoid to use functions are related with struct_galoisFieldPolyForm directly.
 * As possible, use functions of struct_galoisFieldElements.
 */
struct_galoisFieldPolyForm **recreateListOfGaloisField(struct_galoisFieldPolyForm ***p, unsigned int lengthOfList)
{
    #ifndef RELEASE
    if(p)
    {
        errorMes;
        printf("in recreateListOfGaloisField, struct_galoisFieldPolyForm ***p is NULL.\n");
        return NULL;
    }
    #endif
    if(*p)
    {
                #ifndef RELEASE
                if((global_flag_cmdOption&FLAG_MASK_PRINTF_LOG))
                {
                    printf("in createListOfGaloisField, struct_galoisFieldPolyForm **p is already have data.\n");
                    printf("**p will be closePowerFormPolynomial(p).\n");
                }
                #endif
        closeListOfGaloisField(p);
    }
    *p=createListOfGaloisField(lengthOfList);
    return *p;
}

