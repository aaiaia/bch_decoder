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

/* File IO */
char save_struct_galoisFieldElements(char *path, char *primitivePoly, struct_galoisFieldElements *p)
{
    unsigned int tmp_i;
    FILE *fileio;
    if(!path)           return -1;
    if(!primitivePoly)  return -2;
    if(!p)              return -3;

    fileio = fopen(path, "w");

    fprintf(fileio, "primitive polynomials, %s\r\n", primitivePoly);

    fprintf(fileio, "%s\r\n", FILE_IO_DATA_PARSING_KEY_STRING_SAVED_DATA_START);


    fprintf(fileio, "%s\r\n", FILE_IO_DATA_PARSING_KEY_STRING_KIND_OF_DATA_START);
    fprintf(fileio, "Kind Of Data, %d\r\n", FILE_IO_KIND_OF_DATA_GALOIS_FIELDS);
    fprintf(fileio, "%s\r\n", FILE_IO_DATA_PARSING_KEY_STRING_KIND_OF_DATA_END);


    fprintf(fileio, "%s\r\n", FILE_IO_DATA_PARSING_KEY_STRING_ARRAY_TYPE_START);
    fprintf(fileio, "Saved Data Type, %d\r\n", FILD_IO_DATA_ARRAY_TYPE_LINEAR);
    fprintf(fileio, "%s\r\n", FILE_IO_DATA_PARSING_KEY_STRING_ARRAY_TYPE_END);

    fprintf(fileio, "%s\r\n", FILE_IO_DATA_PARSING_KEY_STRING_BIT_WIDTH_START);
    fprintf(fileio, "Bit Width, %d\r\n", (*(p->element+0))->length);
    fprintf(fileio, "%s\r\n", FILE_IO_DATA_PARSING_KEY_STRING_BIT_WIDTH_END);

    fprintf(fileio, "%s\r\n", FILE_IO_DATA_PARSING_KEY_STRING_ROW_DATA_START);
    fprintf(fileio, "galois field row length, %d\r\n", p->length);
    fprintf(fileio, "%s\r\n", FILE_IO_DATA_PARSING_KEY_STRING_ROW_DATA_END);

    fprintf(fileio, "%s\r\n", FILE_IO_DATA_PARSING_KEY_STRING_COL_DATA_START);
    fprintf(fileio, "galois field column length, %d\r\n", 1);
    fprintf(fileio, "%s\r\n", FILE_IO_DATA_PARSING_KEY_STRING_COL_DATA_END);

    fprintf(fileio, "pow(index),string,intValue\r\n");
    fprintf(fileio, "%s\r\n", FILE_IO_DATA_PARSING_KEY_STRING_RAW_DATA_START);
    fprintf(fileio, "%c(%d), %s, %d\r\n", '-', (*(p->element+0))->index, (*(p->element+0))->value, (*(p->element+0))->intValue);
    for(tmp_i=1; tmp_i<p->length; tmp_i++)
    {
        fprintf(fileio, "%d(%d), %s, %d\r\n", ((*(p->element+tmp_i))->index - 1), (*(p->element+tmp_i))->index, (*(p->element+tmp_i))->value, (*(p->element+tmp_i))->intValue);
    }
    fprintf(fileio, "%s\r\n", FILE_IO_DATA_PARSING_KEY_STRING_RAW_DATA_END);


    fprintf(fileio, "%s\r\n", FILE_IO_DATA_PARSING_KEY_STRING_SAVED_DATA_END);

    fclose(fileio);

    return 0;
}

char load_struct_galoisFieldElements(char *path, char *primitivePoly, struct_galoisFieldElements *p)
{
    unsigned int tmp_i;
    FILE *load_fileio_GF;

    char load_primitivePoly[1025];

    int load_kindOfData=0;
    int load_saveOfData=0;

    unsigned int load_bitWidth;
    unsigned int load_row;
    unsigned int load_column;

    unsigned int load_intVal;
    unsigned int load_index;
    char load_exp_char=0;
    unsigned int load_exp_integer=0;

    char load_stringBuf[1025]={0};


    if(!path)            return -1;
    if(!primitivePoly)    return -2;
    if(!p)                return -3;

    load_fileio_GF = fopen(path, "r");

            printf("[load_struct_galoisFieldElements START]\r\n");

    fscanf(load_fileio_GF, "primitive polynomials, %s\r\n", load_primitivePoly);
            //printf("primitive polynomials, %s\r\n", load_primitivePoly);

    fscanf(load_fileio_GF, "%s\r\n", load_stringBuf);
            //printf("%s\r\n", load_stringBuf);


    fscanf(load_fileio_GF, "%s\r\n", load_stringBuf);
            //printf("%s\r\n", load_stringBuf);
    fscanf(load_fileio_GF, "Kind Of Data, %d\r\n", &load_kindOfData);
    fscanf(load_fileio_GF, "%s\r\n", load_stringBuf);
            //printf("%s\r\n", load_stringBuf);


    fscanf(load_fileio_GF, "%s\r\n", load_stringBuf);
            //printf("%s\r\n", load_stringBuf);
    fscanf(load_fileio_GF, "Saved Data Type, %d\r\n", &load_saveOfData);
    fscanf(load_fileio_GF, "%s\r\n", load_stringBuf);
            //printf("%s\r\n", load_stringBuf);

    fscanf(load_fileio_GF, "%s\r\n", load_stringBuf);
            //printf("%s\r\n", load_stringBuf);
    fscanf(load_fileio_GF, "Bit Width, %d\r\n", &load_bitWidth);
        //(*(p->element+0))->length
    fscanf(load_fileio_GF, "%s\r\n", load_stringBuf);
            //printf("%s\r\n", load_stringBuf);

    fscanf(load_fileio_GF, "%s\r\n", load_stringBuf);
            //printf("%s\r\n", load_stringBuf);
    fscanf(load_fileio_GF, "galois field row length, %d\r\n", &load_row);
        //p->length
    fscanf(load_fileio_GF, "%s\r\n", load_stringBuf);
            //printf("%s\r\n", load_stringBuf);

    fscanf(load_fileio_GF, "%s\r\n", load_stringBuf);
            //printf("%s\r\n", load_stringBuf);
    fscanf(load_fileio_GF, "galois field column length, %d\r\n", &load_column);

    fscanf(load_fileio_GF, "%s\r\n", load_stringBuf);
            //printf("%s\r\n", load_stringBuf);

    fscanf(load_fileio_GF, "%s\r\n", load_stringBuf);
            //printf("%s\r\n", load_stringBuf);

    fscanf(load_fileio_GF, "%s\r\n", load_stringBuf);
            //printf("%s\r\n", load_stringBuf);
    /*
    //fscanf(load_fileio_GF, "%c(%d), %s, %d\r\n", '-', (*(p->element+0))->index, (*(p->element+0))->value, (*(p->element+0))->intValue);
    //for(tmp_i=1; tmp_i<p->length; tmp_i++)
    //{
    //    fscanf(load_fileio_GF, "%d(%d), %s, %d\r\n", ((*(p->element+tmp_i))->index - 1), (*(p->element+tmp_i))->index, (*(p->element+tmp_i))->value, (*(p->element+tmp_i))->intValue);
    //}
    //fscanf(load_fileio_GF, "%c(%d), %s, %d\r\n", &load_exp_char, &load_index, load_stringBuf, &load_intVal);
    */
        fscanf(load_fileio_GF, "%c(%d), ", &load_exp_char, &load_index);
        fgets(load_stringBuf, load_bitWidth+1, load_fileio_GF);
        load_stringBuf[load_bitWidth] = 0;
        fscanf(load_fileio_GF, ", %d\r\n", &load_intVal);

                //printf("%c(%d), %s, %d\r\n", load_exp_char, load_index, load_stringBuf, load_intVal);

    strcpy((*(p->element+0))->value, load_stringBuf);
    (*(p->element+0))->intValue = load_intVal;
    (*(p->element+0))->index = load_index;
    for(tmp_i=1; tmp_i<p->length; tmp_i++)
    {
        fscanf(load_fileio_GF, "%d(%d), ", &load_exp_integer, &load_index);
        fgets(load_stringBuf, load_bitWidth+1, load_fileio_GF);
        load_stringBuf[load_bitWidth] = 0;
        fscanf(load_fileio_GF, ", %d\r\n", &load_intVal);
        strcpy((*(p->element+tmp_i))->value, load_stringBuf);
        (*(p->element+tmp_i))->intValue = load_intVal;
        (*(p->element+load_index))->index = load_index;
                //printf("%d(%d), %s, %d\r\n", load_exp_integer, load_index, load_stringBuf, load_intVal);
    }
    fscanf(load_fileio_GF, "%s\r\n", load_stringBuf);
                //printf("%s\r\n", load_stringBuf);


    fscanf(load_fileio_GF, "%s\r\n", load_stringBuf);
                //printf("%s\r\n", load_stringBuf);

    fclose(load_fileio_GF);

                printf("[load_struct_galoisFieldElements END]\r\n");

            #ifndef RELEASE
                if(global_flag_gfCommon_display&FLAG_MASK_DISPLAY_GF_COMMON_FILE_IO)
                {
                    printf(">> Loaded Galois Fields <<\r\n");
                    printGaloisFieldSavedForm(p);
                }
            #endif

    return 0;
}

/*
 * Converting struct_galoisFieldElements type polynomial to binary(string)
 * The polynomial has only two coefficients, are a^0 or a^1, like f(x)=a^1*x^2+a^0*x^1+a^1*x^0
 * f(x)=a^1*x^2+a^1*x^1+a^0*x^0 => [MSB]110[LSB]
 */
char *convertGaloisFielsAndElementsToStringOnlyZeroOrOne(struct_galoisFieldElements *field, struct_galoisFieldElements *convertedPolynomial)
{
    unsigned int i;

    char *p;

    #ifndef RELEASE
    if(!field)
    {
        errorMes;
        printf("in convertGaloisFielsAndElementsToStringOnlyZeroOrOne, struct_galoisFieldElements *field is NULL.\n");
        return NULL;
    }
    if(!convertedPolynomial)
    {
        errorMes;
        printf("in convertGaloisFielsAndElementsToStringOnlyZeroOrOne, struct_galoisFieldElements *p is NULL.\n");
        return NULL;
    }
    #endif

    p=(char*)malloc(sizeof(char)*(convertedPolynomial->length+1));
    *(p+convertedPolynomial->length)=0;

    for(i=0; i<convertedPolynomial->length; i++)
    {
        if(checkValueFromPolyFormUsingGaloisFieldValueUsingIntValue_((*(convertedPolynomial->element+i)), (*(field->element+0))))
        {
            *(p+i)='0';
        }
        else if(checkValueFromPolyFormUsingGaloisFieldValueUsingIntValue_((*(convertedPolynomial->element+i)), (*(field->element+1))))
        {
            *(p+i)='1';
        }
        else
        {
                    #ifndef RELEASE
                    errorMes; printf("in convertGaloisFielsAndElementsToStringOnlyZeroOrOne, (*(convertedPolynomial->element+i)) have a ilegal valie.\n");
                    #endif
            free(p);
            return NULL;
        }
    }
    return p;
}

/* Find Same elements */
struct_galoisFieldPolyForm *findSameElementOfGaloisField(struct_galoisFieldElements *field, struct_galoisFieldPolyForm *p)
{
    unsigned int i;
    struct_galoisFieldPolyForm *result=NULL;
    #ifndef RELEASE
    if((*(field->element))->length!=p->length)
    {
        errorMes;
        printf("in *findSameElementOfGaloisField, field poly length: %d, target field poly length: %d\n", (*(field->element))->length, p->length);
    }
    #endif

    for(i=0; i<field->length; i++)
    {
        if(checkValueFromPolyFormUsingGaloisFieldValueUsingIntValue_(*(field->element+i), p))
        {
            result=(*(field->element+i));
        }
    }
    return result;
}

struct_galoisFieldPolyForm *findSameElementOfGaloisFieldUsingString(struct_galoisFieldElements *field, char *string)
{
    unsigned int strLength = strlen(string);

    unsigned int i;

    #ifndef RELEASE
    if((*(field->element))->length!=strLength)
    {
        errorMes;
        printf("in *findSameElementOfGaloisFieldUsingString, field poly length: %d, string length: %d\n", (*(field->element))->length, strLength);
        printf("string : \"%s\"\n", string);
    }
    #endif

    for(i=0; i<field->length; i++)
    {
        if(checkValueUsingStringFromPolyForm(*(field->element+i), string)) return (*(field->element+i));
    }
    return NULL;
}

/* For displying(debugging) */
void printGaloisFieldSavedForm(struct_galoisFieldElements *p)
{
    unsigned int tmp_i=0;

    #ifndef RELEASE
    if(!p) return;
    #endif

    printf(">> Galois Field File I/O Form <<\n");

    printf("pow(index), string, int value\r\n");
    printf("%c(%d), %s, %d\r\n", '-', (*(p->element+0))->index, (*(p->element+0))->value, (*(p->element+0))->intValue);
    for(tmp_i=1; tmp_i<p->length; tmp_i++)
    {
        printf("%d(%d), %s, %d\r\n", ((*(p->element+tmp_i))->index - 1), (*(p->element+tmp_i))->index, (*(p->element+tmp_i))->value, (*(p->element+tmp_i))->intValue);
    }
}
