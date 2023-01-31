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

/* Operation */
char calculateConjugacyClasses(struct_galoisFieldElements *galoisFields, struct_setOfGaloisFieldElements *p)
{
    char *elementFlagger;

    unsigned int i, j;
    unsigned int elementFlaggerSecelcted;//k;

    unsigned int exponential;

    struct_galoisFieldPolyForm *calculationBuffer;

    #ifndef RELEASE
    if(!galoisFields)
    {
        errorMes;
        printf("in calculateConjugacyClasses, struct_galoisFieldElements *galoisFields is NULL.\n");
        return -1;
    }
    if(!p)
    {
        errorMes;
        printf("in calculateConjugacyClasses, struct_setOfGaloisFieldElements *p is NULL.\n");
        return -1;
    }
    #endif

    //*(elementFlagger+0) is mean that a^1 is selected already.
    elementFlagger=(char*)malloc(sizeof(char)*((galoisFields->length)-2+1));//index 0 is mean that a^1, and latest index(except NULL) is total number of element in GF -(minus) 2.
    memset(elementFlagger, '0', sizeof(char)*((galoisFields->length)-2));
    *(elementFlagger+((galoisFields->length)-2))=0;//last string have to set NULL.

    elementFlaggerSecelcted=1;//elementFlaggerSecelcted is indicate elementsFlagger location.

    for(i=0; i<p->length; i++)
    {
        *((*(p->conjugateSet+i))->element+0)=*(galoisFields->element+2+(elementFlaggerSecelcted-1));
        elementFlagger[(elementFlaggerSecelcted-1)]='1';

        for(j=1; j<(*(p->conjugateSet+i))->length; j++)
        {
            calculationBuffer=multiElementsInGF_returnAddr(galoisFields, *((*(p->conjugateSet+i))->element-1+j), *((*(p->conjugateSet+i))->element-1+j));
            if(checkValueFromPolyFormUsingGaloisFieldValueUsingIntValue_(*((*(p->conjugateSet+i))->element+0), calculationBuffer))//check, it is same to first value?
            {
                if(j!=(*(p->conjugateSet+i))->length)//length is number and j-1 is offset. So, j-1+1 is number.
                {
                            #ifndef RELEASE
                            warningMes; printf("in calculateConjugacyClasses, j(=\'%d\')!=(*(p->conjugateSet+i))->length(=\'%d\').\n", j, (*(p->conjugateSet+i))->length);
                            #endif
                    (*(p->conjugateSet+i))->length=j;

                    #ifndef RELEASE
                    for(; j<(*(p->conjugateSet+i))->length; j++)
                    {
                        if(*((*(p->conjugateSet+i))->element+j))
                        {
                            warningMes; printf("in calculateConjugacyClasses, *((*(p->conjugateSet+i))->element+j)(Addr = \'0x%lx\') is not NULL.\n", (unsigned long)(*((*(p->conjugateSet+i))->element+j)));
                            warningMesShort; printf("this link information(it is pointer) will be lost.\n");
                        }
                    }
                    #endif
                }
                break;
            }
            else
            {
                exponential=abstractIndexOfPowerFormInElementsOfGaloisFieldUsingIndex(galoisFields, calculationBuffer);

                if((exponential==-1)||(exponential==0)||(exponential==1))
                {
                            #ifndef RELEASE
                            errorMes; printf("in calculateConjugacyClasses, exponential can not be \'%d\'.\n", exponential);
                            #endif
                    return -1;
                }

                elementFlagger[(exponential-2)]='1';
                *((*(p->conjugateSet+i))->element+j)=calculationBuffer;
            }
        }

        //find non selected elements using elementFlagger.
        for(elementFlaggerSecelcted=1; elementFlaggerSecelcted<((galoisFields->length)-2+1); elementFlaggerSecelcted++)
        {
            if((elementFlagger[elementFlaggerSecelcted-1]=='0'))
            {
                break;
            }
        }
        if(elementFlaggerSecelcted==((galoisFields->length)-2+1))
        {
            break;
        }
    }
    free(elementFlagger);
    return 0;
}

char calculateConjugacyClasses_VariableLength(struct_galoisFieldElements *galoisFields, struct_setOfGaloisFieldElements *p)
{
    char *elementFlagger;

    unsigned int i;
    unsigned int selectedConjugacySet;
    unsigned int selectedElementOfConjugacy;



    struct_galoisFieldPolyForm *firstElementOfConjugacySet;
    struct_galoisFieldPolyForm *calculationBuffer;

    #ifndef RELEASE
    if(!galoisFields)
    {
        errorMes;
        printf("in calculateConjugacyClasses, struct_galoisFieldElements *galoisFields is NULL.\n");
        return -1;
    }
    if(!p)
    {
        errorMes;
        printf("in calculateConjugacyClasses, struct_setOfGaloisFieldElements *p is NULL.\n");
        return -1;
    }
    #endif

    /* flagger start a^0, but a^0 one of conjugacy any conjugacy set. */
    elementFlagger=(char*)malloc(sizeof(char)*((galoisFields->length)+1));
    memset(elementFlagger, '0', sizeof(char)*((galoisFields->length)-1));

    elementFlagger[(galoisFields->length)]=0;//last string have to set NULL.
    elementFlagger[0]='1';//a^- already select, because it is not change when doubled itself.
    elementFlagger[1]='1';//a^0 already select, because it is not change when doubled itself.

    selectedConjugacySet=0;//start 0-th conjugacy set

    for(i=0; i<galoisFields->length; i++)
    {
        /* pass selected location of element*/
        if(elementFlagger[i]=='1')
        {
            continue;
        }

        /* if not enough space of conjugacy set to charge conjugacy, added space */
        if(!((selectedConjugacySet)<(p->length)))
        {
            if(addLengthOfConjugacySet_VariableLength(galoisFields, p, 1))
            {
                        #ifndef RELEASE
                        errorMes; printf("in calculateConjugacyClasses_VariableLength, addLengthOfConjugacySet_VariableLength(galoisFields, p, 1) return to not zero.");
                        printf("it is mean that add conjugacy set is fail. conjugacy set will be closed and simulator can not work normally.\n");
                        #endif
                closeConjugacyClasses(&p);
                return -1;
            }

        }

        /* To fill up conjugacy set, initialize conditions */
        firstElementOfConjugacySet=galoisFields->element[i];
        calculationBuffer=firstElementOfConjugacySet;

        if(!(p->conjugateSet[selectedConjugacySet]->length))
        {
            if(p->conjugateSet[selectedConjugacySet]->element)
            {
                p->conjugateSet[selectedConjugacySet]=recreateGaloisFieldExceptElements(&(p->conjugateSet[selectedConjugacySet]), 1);
            }

        }
        /*
        //if(!(p->limitedConjugateSet[selectedConjugacySet]->length))
        //{
        //    if(p->limitedConjugateSet[selectedConjugacySet]->element)
        //    {
        //        p->limitedConjugateSet[selectedConjugacySet]=recreateGaloisFieldExceptElements(&(p->limitedConjugateSet[selectedConjugacySet]), 1);
        //    }
        //}
        */

        selectedElementOfConjugacy=0;
        do
        {
            if(!((selectedElementOfConjugacy)<(p->conjugateSet[selectedConjugacySet]->length)))
            {
                addLengthOfGaloisFieldExceptElements_VariableLength(p->conjugateSet[selectedConjugacySet], 1);
            }
            ((p->conjugateSet[selectedConjugacySet])->element[selectedElementOfConjugacy])=calculationBuffer;

            /*
            //This code is wrong.... 1
            //if(!((selectedElementOfConjugacy)<(p->limitedConjugateSet[selectedConjugacySet]->length)))
            //{
            //    addLengthOfGaloisFieldExceptElements_VariableLength(p->limitedConjugateSet[selectedConjugacySet], 1);
            //    if( !( (abstractIndexOfPowerFormInElementsOfGaloisFieldUsingIndex(galoisFields, calculationBuffer)-1) > p->limitedExponential ) )
            //    {
            //        ((p->limitedConjugateSet[selectedConjugacySet])->element[selectedElementOfConjugacy])=calculationBuffer;
            //    }
            //    else
            //    {
            //        ((p->limitedConjugateSet[selectedConjugacySet])->element[selectedElementOfConjugacy])=galoisFields->element[0];
            //    }
            //}
            */
            /*
            //This code is wrong.... 2
            //if(!((selectedElementOfConjugacy)<(p->limitedConjugateSet[selectedConjugacySet]->length)))
            //{
            //    addLengthOfGaloisFieldExceptElements_VariableLength(p->limitedConjugateSet[selectedConjugacySet], 1);
            //    if( !(selectedElementOfConjugacy%2) )
            //    {
            //        ((p->limitedConjugateSet[selectedConjugacySet])->element[selectedElementOfConjugacy])=calculationBuffer;
            //    }
            //    else
            //    {
            //        ((p->limitedConjugateSet[selectedConjugacySet])->element[selectedElementOfConjugacy])=galoisFields->element[0];
            //    }
            //}
            */

            elementFlagger[abstractIndexOfPowerFormInElementsOfGaloisFieldUsingIndex(galoisFields, calculationBuffer)]='1';//set selected location
            selectedElementOfConjugacy++;
            calculationBuffer=multiElementsInGF_returnAddr(galoisFields, calculationBuffer, calculationBuffer);
        }
        while(firstElementOfConjugacySet!=calculationBuffer);
        /* increase 1, selectedConjugacySet */
        selectedConjugacySet++;
    }

    free(elementFlagger);
    return 0;
}

/* file IO */
void print_setOfGaloisFieldElementsSavedForm(struct_setOfGaloisFieldElements *p)
{
    unsigned int tmp_i;
    unsigned int tmp_j;

    #ifndef RELEASE
    if(!p)
    {
        errorMes;
        printf("in print_setOfGaloisFieldElementsSavedForm, struct_setOfGaloisFieldElements *p is NULL\r\n");
        return;
    }
    #endif

    printf(">> Set Of Galois Field File I/O Form <<\n");

    for(tmp_i=0; tmp_i<(p->length); tmp_i++)
    {
        for(tmp_j=0; tmp_j<(*(p->conjugateSet+tmp_i))->length; tmp_j++)
        {
            if((*((*(p->conjugateSet+tmp_i))->element+tmp_j))->index)
            {
                printf("%d(%d)%s",
                    ((*((*(p->conjugateSet+tmp_i))->element+tmp_j))->index - 1),
                    (*((*(p->conjugateSet+tmp_i))->element+tmp_j))->index,
                    (tmp_j<((*(p->conjugateSet+tmp_i))->length-1))?", ":""
                );
            }
            else
            {
                printf("%c(%d)%s",
                    '-',
                    (*((*(p->conjugateSet+tmp_i))->element+tmp_j))->index,
                    (tmp_j<((*(p->conjugateSet+tmp_i))->length-1))?", ":""
                );

            }
        }
        printf("\r\n");
    }
}

char save_struct_setOfGaloisFieldElements(char *path, char *primitivePoly, struct_setOfGaloisFieldElements *p)
{
    //*((*(p->conjugateSet+i))->element+j)=calculationBuffer;
    //(*(galoisFields->element+0));

    unsigned int tmp_i, tmp_j;
    FILE *fileio;
    if(!path)            return -1;
    if(!primitivePoly)    return -2;
    if(!p)                return -3;

    fileio = fopen(path, "w");

    fprintf(fileio, "primitive polynomials, %s\r\n", primitivePoly);

    fprintf(fileio, "%s\r\n", FILE_IO_DATA_PARSING_KEY_STRING_SAVED_DATA_START);

    fprintf(fileio, "%s\r\n", FILE_IO_DATA_PARSING_KEY_STRING_KIND_OF_DATA_START);
    fprintf(fileio, "Kind Of Data, %d\r\n", FILE_IO_KIND_OF_DATA_CONJUGATE_SET);
    fprintf(fileio, "%s\r\n", FILE_IO_DATA_PARSING_KEY_STRING_KIND_OF_DATA_END);

    fprintf(fileio, "%s\r\n", FILE_IO_DATA_PARSING_KEY_STRING_ARRAY_TYPE_START);
    fprintf(fileio, "Saved Data Type, %d\r\n", FILD_IO_DATA_ARRAY_TYPE_NONE_LINEAR);
    fprintf(fileio, "%s\r\n", FILE_IO_DATA_PARSING_KEY_STRING_ARRAY_TYPE_END);

    fprintf(fileio, "%s\r\n", FILE_IO_DATA_PARSING_KEY_STRING_BIT_WIDTH_START);
    fprintf(fileio, "Bit Width, %d\r\n", (*((*(p->conjugateSet+0))->element+0))->length);
    fprintf(fileio, "%s\r\n", FILE_IO_DATA_PARSING_KEY_STRING_BIT_WIDTH_END);

    fprintf(fileio, "%s\r\n", FILE_IO_DATA_PARSING_KEY_STRING_ROW_DATA_START);
    fprintf(fileio, "conjugate set nums, %d\r\n", p->length);
    fprintf(fileio, "%s\r\n", FILE_IO_DATA_PARSING_KEY_STRING_ROW_DATA_END);



    fprintf(fileio, "%s\r\n", FILE_IO_DATA_PARSING_KEY_STRING_COL_DATA_START);
    for(tmp_i=0; tmp_i<(p->length); tmp_i++)
    {
        fprintf(fileio, "%d-th set conjugate elements nums, %d\r\n", tmp_i, (*(p->conjugateSet+tmp_i))->length);
    }
    fprintf(fileio, "%s\r\n", FILE_IO_DATA_PARSING_KEY_STRING_COL_DATA_END);

    fprintf(fileio, "pow(index)\r\n");

    fprintf(fileio, "%s\r\n", FILE_IO_DATA_PARSING_KEY_STRING_RAW_DATA_START);
    //(*(p->conjugateSet+i))->length
    for(tmp_i=0; tmp_i<(p->length); tmp_i++)
    {
        for(tmp_j=0; tmp_j<(*(p->conjugateSet+tmp_i))->length; tmp_j++)
        {
            if((*((*(p->conjugateSet+tmp_i))->element+tmp_j))->index)
            {
                fprintf(fileio, "%d(%d)%s",
                    ((*((*(p->conjugateSet+tmp_i))->element+tmp_j))->index - 1),
                    (*((*(p->conjugateSet+tmp_i))->element+tmp_j))->index,
                    (tmp_j<((*(p->conjugateSet+tmp_i))->length-1))?", ":""
                );
            }
            else
            {
                fprintf(fileio, "%c(%d)%s",
                    '-',
                    (*((*(p->conjugateSet+tmp_i))->element+tmp_j))->index,
                    (tmp_j<((*(p->conjugateSet+tmp_i))->length-1))?", ":""
                );

            }
        }
        fprintf(fileio, "\r\n");
    }
    fprintf(fileio, "%s\r\n", FILE_IO_DATA_PARSING_KEY_STRING_RAW_DATA_END);

    fprintf(fileio, "%s\r\n", FILE_IO_DATA_PARSING_KEY_STRING_SAVED_DATA_END);

    fclose(fileio);

    return 0;
}

char load_struct_setOfGaloisFieldElements(char *path, char *primitivePoly, struct_galoisFieldElements *galoisFields, struct_setOfGaloisFieldElements **p)
{
    //*((*(p->conjugateSet+i))->element+j)=calculationBuffer;
    //(*(galoisFields->element+0));

    unsigned int tmp_i, tmp_j;

    FILE *fileio_load_setOfGaloisField;

    char load_primitivePoly[1025];

    int load_kindOfData=0;
    int load_saveOfData=0;

    unsigned int load_bitWidth=0;
    unsigned int load_setOfGF_length=0;
    unsigned int *load_column=NULL;

    unsigned int load_index;
    unsigned int load_exp_integer=0;

    unsigned int load_integer=0;

    char load_stringBuf[1025]={0};

    if(!path)            return -1;
    if(!primitivePoly)    return -2;
    if(!p)                return -3;

    fileio_load_setOfGaloisField = fopen(path, "r");

            printf("[load_struct_setOfGaloisFieldElements START]\r\n");

    fscanf(fileio_load_setOfGaloisField, "primitive polynomials, %s\r\n", load_primitivePoly);
            //printf("primitive polynomials, %s\r\n", load_primitivePoly);

    fscanf(fileio_load_setOfGaloisField, "%s\r\n", load_stringBuf);
            //printf("%s\r\n", load_stringBuf);

    fscanf(fileio_load_setOfGaloisField, "%s\r\n", load_stringBuf);
            //printf("%s\r\n", load_stringBuf);
    fscanf(fileio_load_setOfGaloisField, "Kind Of Data, %d\r\n", &load_kindOfData);
            //printf("Kind Of Data, %d\r\n", load_kindOfData);
    fscanf(fileio_load_setOfGaloisField, "%s\r\n", load_stringBuf);
            //printf("%s\r\n", load_stringBuf);

    fscanf(fileio_load_setOfGaloisField, "%s\r\n", load_stringBuf);
            //printf("%s\r\n", load_stringBuf);
    fscanf(fileio_load_setOfGaloisField, "Saved Data Type, %d\r\n", &load_saveOfData);
            //printf("Saved Data Type, %d\r\n", load_saveOfData);
    fscanf(fileio_load_setOfGaloisField, "%s\r\n", load_stringBuf);
            //printf("%s\r\n", load_stringBuf);

    fscanf(fileio_load_setOfGaloisField, "%s\r\n", load_stringBuf);
            //printf("%s\r\n", load_stringBuf);
    fscanf(fileio_load_setOfGaloisField, "Bit Width, %d\r\n", &load_bitWidth);
            //printf("Bit Width, %d\r\n", load_bitWidth);
    fscanf(fileio_load_setOfGaloisField, "%s\r\n", load_stringBuf);
            //printf("%s\r\n", load_stringBuf);

    fscanf(fileio_load_setOfGaloisField, "%s\r\n", load_stringBuf);
            //printf("%s\r\n", load_stringBuf);
    fscanf(fileio_load_setOfGaloisField, "conjugate set nums, %d\r\n", &load_setOfGF_length);
            //printf("conjugate set nums, %d\r\n", load_setOfGF_length);
    fscanf(fileio_load_setOfGaloisField, "%s\r\n", load_stringBuf);
            //printf("%s\r\n", load_stringBuf);
    load_column=(unsigned int*)malloc(sizeof(unsigned int)*load_setOfGF_length);
    memset(load_column, 0, sizeof(unsigned int)*load_setOfGF_length);

    fscanf(fileio_load_setOfGaloisField, "%s\r\n", load_stringBuf);
            //printf("%s\r\n", load_stringBuf);
    for(tmp_i=0; tmp_i<load_setOfGF_length; tmp_i++)
    {
        fscanf(fileio_load_setOfGaloisField, "%d-th set conjugate elements nums, %d\r\n", &load_integer, load_column+tmp_i);
                //printf("%d-th set conjugate elements nums, %d\r\n", load_integer, *(load_column+tmp_i));
    }
    fscanf(fileio_load_setOfGaloisField, "%s\r\n", load_stringBuf);
            //printf("%s\r\n", load_stringBuf);


            //printf("[createConjugacyClasses_staticSetNums START]\r\n");
    *p = createConjugacyClasses_staticSetNums(load_setOfGF_length, load_column);
            //printf("[createConjugacyClasses_staticSetNums END]\r\n");


    fscanf(fileio_load_setOfGaloisField, "%s\r\n", load_stringBuf);
            //printf("%s\r\n", load_stringBuf);

    fscanf(fileio_load_setOfGaloisField, "%s\r\n", load_stringBuf);
    for(tmp_i=0; tmp_i<load_setOfGF_length; tmp_i++)
    {
        for(tmp_j=0; tmp_j<load_column[tmp_i]; tmp_j++)
        {
            if(tmp_j<(load_column[tmp_i]-1))
            {
                fscanf(fileio_load_setOfGaloisField, "%d(%d), ",
                    &load_exp_integer,
                    &load_index
                );
                        //printf("%d(%d), ", load_exp_integer, load_index);
            }
            else
            {
                fscanf(fileio_load_setOfGaloisField, "%d(%d)",
                    &load_exp_integer,
                    &load_index
                );
                        //printf("%d(%d)", load_exp_integer, load_index);
            }
            (*((*((*p)->conjugateSet+tmp_i))->element+tmp_j)) = (*(galoisFields->element+load_index));
            (*((*((*p)->limitedConjugateSet+tmp_i))->element+tmp_j)) = (*(galoisFields->element+load_index));
        }
        fscanf(fileio_load_setOfGaloisField, "\r\n");
                //printf("\r\n");
    }
    fscanf(fileio_load_setOfGaloisField, "%s\r\n", load_stringBuf);
            //printf("%s\r\n", load_stringBuf);
    fscanf(fileio_load_setOfGaloisField, "%s\r\n", load_stringBuf);
            //printf("%s\r\n", load_stringBuf);

    fclose(fileio_load_setOfGaloisField);

    free(load_column);

            #ifndef RELEASE
            if(global_flag_gfCommon_display&FLAG_MASK_DISPLAY_GF_COMMON_FILE_IO)
            {
                printf(">> Set of Condjugacies <<\r\n");
                print_setOfGaloisFieldElementsSavedForm(*p);
            }
            #endif


            printf("[load_struct_setOfGaloisFieldElements END]\r\n");

    return 0;
}
