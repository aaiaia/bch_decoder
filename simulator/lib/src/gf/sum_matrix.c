#include "sum_matrix.h"

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

/* Functions making 'struct_summationMatrix' for list of elements that are 'struct_galoisFieldPolyForm' type */
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

char copyListOfGaloisField(struct_galoisFieldPolyForm **to, struct_galoisFieldPolyForm **from, unsigned int size)
{
    unsigned int i;

    #ifndef RELEASE
    if(!to)
    {
        errorMes;
        printf("in copyListOfGaloisField, struct_galoisFieldPolyForm **to is NULL.\n");
        return -1;
    }
    if(!from)
    {
        errorMes;
        printf("in copyListOfGaloisField, struct_galoisFieldPolyForm **from is NULL.\n");
        return -1;
    }
    if(!size)
    {
        errorMes;
        printf("in copyListOfGaloisField, unsigned int size is 0.\n");
        return -1;
    }

    if((global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)==FLAG_MASK_PRINTF_LOG)
    {
        logMes;
        printf("in copyListOfGaloisField, addr(to)=0x%lx, addr(from)=0x%lx\n", (unsigned long)to, (unsigned long)from);
    }
    #endif

    #ifdef USING_OPEN_MP
    #pragma omp parallel for schedule(guided) private(i) shared(size, to, from)
    #endif
    for(i=0; i<size; i++)
    {
        *(to+i)=*(from+i);
    }

    return 0;
}

char createGaloisFieldElementsAtList(struct_galoisFieldPolyForm **p, unsigned int lengthOfList, unsigned int lengthOfPolyForm)
{
    unsigned int i;

    #ifndef RELEASE
    if(!p)
    {
        errorMes;
        printf("in createGaloisFieldElementsAtList, struct_galoisFieldPolyForm **p is NULL.\n");
        return -1;
    }
    if(!lengthOfList)
    {
        errorMes;
        printf("in createGaloisFieldElementsAtList,  unsigned int lengthOfList is 0.\n");
        return -1;
    }
    if(!lengthOfPolyForm)
    {
        errorMes;
        printf("in createGaloisFieldElementsAtList, unsigned int lengthOfPolyForm is 0.\n");
        return -1;
    }
    #endif

    #ifdef USING_OPEN_MP
    #pragma omp parallel for schedule(guided) private(i) shared(lengthOfList, p, lengthOfPolyForm)
    #endif
    for(i=0; i<lengthOfList; i++)
    {
        *(p+i)=createPolyForm(lengthOfPolyForm);
    }
    return 0;
}

char closeGaloisFieldElementsAtList(struct_galoisFieldPolyForm ***p, unsigned int lengthOfList)
{
    unsigned int i;

    #ifndef RELEASE
    if(!p)
    {
        errorMes;
        printf("in closeGaloisFieldElementsAtList, struct_galoisFieldPolyForm ***p is NULL.\n");
        return -1;
    }
    if(!(*p))
    {
        warningMes;
        printf("in closeGaloisFieldElementsAtList, struct_galoisFieldPolyForm **p is NULL.\n");
        return -1;
    }
    if(!lengthOfList)
    {
        errorMes;
        printf("in closeGaloisFieldElementsAtList, unsigned int lengthOfList is 0.\n");
        return -1;
    }
    #endif

    #ifdef USING_OPEN_MP
    #pragma omp parallel for schedule(guided) private(i) shared(lengthOfList, p)
    #endif
    for(i=0; i<lengthOfList; i++)
    {
        closePolyForm(&(*((*p)+i)));
    }
    return 0;
}

char recreateGaloisFieldElementsAtList(struct_galoisFieldPolyForm **p, unsigned int lengthOfList, unsigned int lengthOfPolyForm)
{
    if(closeGaloisFieldElementsAtList(&p, (*p)->length))
    {
        warningMes;
        printf("in recreateGaloisFieldElementsAtList, closeGaloisFieldElementsAtList(&p, (*p)->length) is failed.\n");
    }
    createGaloisFieldElementsAtList(p, lengthOfList, lengthOfPolyForm);
    return 0;
}

struct_galoisFieldPolyForm **createListOfGaloisFieldAndComponents(unsigned int lengthOfList, unsigned int polyLength)
{
    //struct_galoisFieldPolyForm **p=(struct_galoisFieldPolyForm**)malloc(sizeof(struct_galoisFieldPolyForm*)*lengthOfList);
    //memset(p, 0, sizeof(struct_galoisFieldPolyForm*)*lengthOfList);
    struct_galoisFieldPolyForm **p=NULL;
    if(!(p=createListOfGaloisField(lengthOfList)))
    {
                #ifndef RELEASE
                errorMes; printf("in createListOfGaloisFieldAndComponents, fail that createListOfGaloisField(lengthOfList).\n");
                errorMes; printf("return value struct_galoisFieldPolyForm **p = 0x%lx\n", (unsigned long)p);
                #endif
        return NULL;
    }
    if(createGaloisFieldElementsAtList(p, lengthOfList, polyLength))
    {
        closeGaloisFieldElementsAtList(&p, lengthOfList);
        closeListOfGaloisField(&p);
                #ifndef RELEASE
                errorMes;    printf("in createListOfGaloisFieldAndComponents, fail that createGaloisFieldElementsAtList(p, lengthOfList, polyLength).\n");
                #endif
        return NULL;
    }

    return p;
}

char closeListOfGaloisFieldAndElements(struct_galoisFieldPolyForm ***p, unsigned int lengthOfList)
{
    if(closeGaloisFieldElementsAtList(p, lengthOfList))
    {
        #ifndef RELEASE
        errorMes; printf("in closeListOfGaloisFieldAndElements, fail close closeGaloisFieldElementsAtList(p, lengthOfList)\n");
        #endif
        return -1;
    }
    if(closeListOfGaloisField(p))
    {
                #ifndef RELEASE
                errorMes; printf("in closeListOfGaloisFieldAndElements, fail close closeListOfGaloisField(p)\n");
                #endif
        return -1;
    }
    *p=NULL;
    return 0;
}

/*
//struct_galoisFieldPolyForm **recreateListOfGaloisFieldAndComponents(struct_galoisFieldPolyForm ***p, unsigned int lengthOfList, unsigned int polyLength)
//{
//    if(*p)
//    {
//        if((global_flag_cmdOption&FLAG_MASK_PRINTF_LOG))
//        {
//            infoMes;
//            printf("in createListOfGaloisFieldAndComponents, struct_galoisFieldPolyForm *p have data already.\n");
//            printf("it will be recreated.\n");
//        }
//        closeListOfGaloisFieldAndComponents((*p)->length);
//    }
//    *p=createListOfGaloisFieldAndComponents(lengthOfList, polyLength);
//    return *p;
//}
*/

char calculateSummationMatrix(struct_galoisFieldElements *galoisFields, struct_summationMatrix *p)
{
    unsigned int i,j;

    unsigned int tmp_row;
    unsigned int tmp_column;

    #ifndef RELEASE
    if(!galoisFields)
    {
        return -1;
    }

    if(!p)
    {
        return -2;
    }

    if(!(p->row))
    {
        return -3;
    }
    if(!(p->column))
    {
        return -4;
    }
    #endif

    tmp_row = p->row;
    tmp_column = p->column;

            #ifndef RELEASE
            infoMes;  printf("calculating a struct_summationMatrix ");
            #endif
    #ifdef USING_OPEN_MP
    #pragma omp parallel for schedule(guided) private(i, j) shared(tmp_row, p, tmp_column, galoisFields)
    #endif
    for(i=0; i<tmp_row; i++)
    {
        for(j=0; j<tmp_column; j++)
        {
            //summationElementsInGaloisFieldUsingGaloisFieldInt_finding is return (*(field->element+i));
            (*((*(p->element+i))+j))=summationElementsInGaloisFieldUsingGaloisFieldInt_finding(galoisFields, *(galoisFields->element+i), *(galoisFields->element+j));
        }
    }
            #ifndef RELEASE
            printf("is done.\n");
            #endif

    return 0;
}

/* 20160425_18:07 deleted */
/*
//struct_summationMatrix *createSummationMatrix(struct_galoisFieldElements *galoisFields, unsigned int row, unsigned int column)
//{
//    unsigned int i,j;
//    struct_summationMatrix *p;
//
//    unsigned int count;
//
//    #ifndef RELEASE
//    if(!galoisFields)
//    {
//        return NULL;
//    }
//    if(!row)
//    {
//        return NULL;
//    }
//    if(!column)
//    {
//        return NULL;
//    }
//    if(row!=column)
//    {
//        return NULL;
//    }
//    #endif
//
//
//    p=(struct_summationMatrix*)malloc(sizeof(struct_summationMatrix));
//
//    p->element=(struct_galoisFieldPolyForm ***)malloc(sizeof(struct_galoisFieldPolyForm **)*row);
//
//            #ifndef RELEASE
//            infoMes;  printf("calculating a struct_summationMatrix ");
//            #endif
//
//    #ifdef USING_OPEN_MP
//    #pragma omp parallel for schedule(guided) private(i, j) shared(row, p, column, galoisFields)
//    #endif
//    for(i=0; i<row; i++)
//    {
//        (*(p->element+i))=(struct_galoisFieldPolyForm **)malloc(sizeof(struct_galoisFieldPolyForm *)*column);
//        for(j=0; j<column; j++)
//        {
//            (*((*(p->element+i))+j))=summationElementsInGaloisFieldUsingGaloisFieldInt_finding(galoisFields, *(galoisFields->element+i), *(galoisFields->element+j));
//        }
//    }
//            #ifndef RELEASE
//            printf("is done.\n");
//            #endif
//
//    p->row=row;
//    p->column=column;
//    return p;
//}
*/

struct_summationMatrix *createSummationMatrix(struct_galoisFieldElements *galoisFields, unsigned int in_row, unsigned int in_column)
{
    struct_summationMatrix *p;

    p=createSummationMatrix_emptySpace(in_row, in_column);
    if(calculateSummationMatrix(galoisFields, p))
    {
        errorMes; printf("in createSummationMatrix, calculateSummationMatrix has error(close).\r\n");
        closeSummationMatrix(&p);
    }
    return p;
}

/* For displaying(debugging) */

/* FILE IO */
char save_struct_summationMatrix(char *path, char *primitivePoly, struct_summationMatrix *p)
{
    unsigned int tmp_i;
    unsigned int tmp_j;
    FILE *fileio;
    if(!path)            return -1;
    if(!primitivePoly)    return -2;
    if(!p)                return -3;

    fileio = fopen(path, "w");

    fprintf(fileio, "primitive polynomials, %s\r\n", primitivePoly);


    fprintf(fileio, "%s\r\n", FILE_IO_DATA_PARSING_KEY_STRING_SAVED_DATA_START);

    fprintf(fileio, "%s\r\n", FILE_IO_DATA_PARSING_KEY_STRING_KIND_OF_DATA_START);
    fprintf(fileio, "Kind Of Data, %d\r\n", FILE_IO_KIND_OF_DATA_SUMATION_MATRIX);
    fprintf(fileio, "%s\r\n", FILE_IO_DATA_PARSING_KEY_STRING_KIND_OF_DATA_END);


    fprintf(fileio, "%s\r\n", FILE_IO_DATA_PARSING_KEY_STRING_ARRAY_TYPE_START);
    fprintf(fileio, "Saved Data Type, %d\r\n", FILD_IO_DATA_ARRAY_TYPE_LINEAR);
    fprintf(fileio, "%s\r\n", FILE_IO_DATA_PARSING_KEY_STRING_ARRAY_TYPE_END);

    fprintf(fileio, "%s\r\n", FILE_IO_DATA_PARSING_KEY_STRING_BIT_WIDTH_START);
    fprintf(fileio, "Bit Width, %d\r\n", (*((*(p->element+0))+0))->length);
    fprintf(fileio, "%s\r\n", FILE_IO_DATA_PARSING_KEY_STRING_BIT_WIDTH_END);


    fprintf(fileio, "%s\r\n", FILE_IO_DATA_PARSING_KEY_STRING_ROW_DATA_START);
    fprintf(fileio, "Matrix row length, %d\r\n", p->row);
    fprintf(fileio, "%s\r\n", FILE_IO_DATA_PARSING_KEY_STRING_ROW_DATA_END);


    fprintf(fileio, "%s\r\n", FILE_IO_DATA_PARSING_KEY_STRING_COL_DATA_START);
    fprintf(fileio, "Matrix column length, %d\r\n", p->column);
    fprintf(fileio, "%s\r\n", FILE_IO_DATA_PARSING_KEY_STRING_COL_DATA_END);

    fprintf(fileio, "pow(index)\r\n");

    fprintf(fileio, "%s\r\n", FILE_IO_DATA_PARSING_KEY_STRING_RAW_DATA_START);
    for(tmp_i=0; tmp_i<p->row; tmp_i++)
    {
        for(tmp_j=0; tmp_j<p->column; tmp_j++)
        {
            if((*((*(p->element+tmp_i))+tmp_j))->index)
            {
                fprintf(fileio, "%d(%d)%s",
                    ((*((*(p->element+tmp_i))+tmp_j))->index - 1),
                    (*((*(p->element+tmp_i))+tmp_j))->index,
                    ((tmp_j<(p->column-1))?", ":"")
                );
            }
            else
            {
                fprintf(fileio, "%c(%d)%s",
                    '-',
                    (*((*(p->element+tmp_i))+tmp_j))->index,
                    ((tmp_j<(p->column-1))?", ":"")
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

char load_struct_summationMatrix(char *path, char *primitivePoly, struct_galoisFieldElements *galoisFields, struct_summationMatrix *p)
{
    unsigned int tmp_i=0;
    unsigned int tmp_j=0;
    FILE *fileio_load_sumMatrix;

    unsigned int primitivePoly_length=0;
    char load_primitivePoly[1025];

    int load_kindOfData=0;
    int load_saveOfData=0;

    unsigned int load_bitWidth;
    unsigned int load_row;
    unsigned int load_column;

    unsigned int load_index;
    char load_exp_char=0;
    unsigned int load_exp_integer=0;

    char load_stringBuf[1025]={0};

    unsigned int chk_incorrectFormCnt=0;
    unsigned int chk_incorrectDataCnt=0;

    if(!path)            return -1;
    if(!primitivePoly)    return -2;
    if(!p)                return -3;

    /* condition chk */
    if(!(primitivePoly_length=strlen(primitivePoly)))
    {
        return -4;
    }


    /* buffer set up to varify */


    fileio_load_sumMatrix = fopen(path, "r");

                printf("[load_struct_summationMatrix START]\r\n");

    if(fscanf(fileio_load_sumMatrix, "primitive polynomials, %s\r\n", load_primitivePoly)<0)            chk_incorrectFormCnt++;
            //printf("primitive polynomials, %s\r\n", load_primitivePoly);
        //if(strcmp(load_primitivePoly, primitivePoly))
        //{
        //    chk_incorrectDataCnt++;
        //}

    if(fscanf(fileio_load_sumMatrix, "%s\r\n", load_stringBuf)<0)        chk_incorrectFormCnt++;
            //printf("%s\r\n", load_stringBuf);//FILE_IO_DATA_PARSING_KEY_STRING_SAVED_DATA_START

    if(fscanf(fileio_load_sumMatrix, "%s\r\n", load_stringBuf)<0)    chk_incorrectFormCnt++;
            //printf("%s\r\n", load_stringBuf);//FILE_IO_DATA_PARSING_KEY_STRING_KIND_OF_DATA_START
    if(fscanf(fileio_load_sumMatrix, "Kind Of Data, %d\r\n", &load_kindOfData)<0)    chk_incorrectFormCnt++;
            //printf("Kind Of Data, %d\r\n", load_kindOfData);//FILE_IO_KIND_OF_DATA_SUMATION_MATRIX
    if(fscanf(fileio_load_sumMatrix, "%s\r\n", load_stringBuf)<0)        chk_incorrectFormCnt++;
            //printf("%s\r\n", load_stringBuf);//FILE_IO_DATA_PARSING_KEY_STRING_KIND_OF_DATA_END


    if(fscanf(fileio_load_sumMatrix, "%s\r\n", load_stringBuf)<0)        chk_incorrectFormCnt++;
            //printf("%s\r\n", load_stringBuf);//FILE_IO_DATA_PARSING_KEY_STRING_ARRAY_TYPE_START
    if(fscanf(fileio_load_sumMatrix, "Saved Data Type, %d\r\n", &load_saveOfData)<0)        chk_incorrectFormCnt++;
            //printf("Saved Data Type, %d\r\n", load_saveOfData);//FILD_IO_DATA_ARRAY_TYPE_LINEAR
    if(fscanf(fileio_load_sumMatrix, "%s\r\n", load_stringBuf)<0)        chk_incorrectFormCnt++;
            //printf("%s\r\n", load_stringBuf);//FILE_IO_DATA_PARSING_KEY_STRING_ARRAY_TYPE_END

    if(fscanf(fileio_load_sumMatrix, "%s\r\n", load_stringBuf)<0)        chk_incorrectFormCnt++;
            //printf("%s\r\n", load_stringBuf);//FILE_IO_DATA_PARSING_KEY_STRING_BIT_WIDTH_START
    if(fscanf(fileio_load_sumMatrix, "Bit Width, %d\r\n", &load_bitWidth)<0)                                chk_incorrectFormCnt++;
            //printf("Bit Width, %d\r\n", load_bitWidth);
        //if((*((*(p->element+0))+0))->length != load_bitWidth)
        //{
        //    chk_incorrectDataCnt++;
        //}
    if(fscanf(fileio_load_sumMatrix, "%s\r\n", load_stringBuf)<0)        chk_incorrectFormCnt++;
            //printf("%s\r\n", load_stringBuf);//FILE_IO_DATA_PARSING_KEY_STRING_BIT_WIDTH_END


    if(fscanf(fileio_load_sumMatrix, "%s\r\n", load_stringBuf)<0)        chk_incorrectFormCnt++;
            //printf("%s\r\n", load_stringBuf);//FILE_IO_DATA_PARSING_KEY_STRING_ROW_DATA_START
    if(fscanf(fileio_load_sumMatrix, "Matrix row length, %d\r\n", &load_row)<0)                            chk_incorrectFormCnt++;
            //printf("Matrix row length, %d\r\n", load_row);
        //if(p->row != load_row)
        //{
        //    chk_incorrectDataCnt++;
        //}

    if(fscanf(fileio_load_sumMatrix, "%s\r\n", load_stringBuf)<0)            chk_incorrectFormCnt++;
            //printf("%s\r\n", load_stringBuf);//FILE_IO_DATA_PARSING_KEY_STRING_ROW_DATA_END


    if(fscanf(fileio_load_sumMatrix, "%s\r\n", load_stringBuf)<0)        chk_incorrectFormCnt++;
            //printf("%s\r\n", load_stringBuf);//FILE_IO_DATA_PARSING_KEY_STRING_COL_DATA_START
    if(fscanf(fileio_load_sumMatrix, "Matrix column length, %d\r\n", &load_column)<0)                    chk_incorrectFormCnt++;
            //printf("Matrix column length, %d\r\n", load_column);
        //if(p->column != load_column)
        //{
        //    chk_incorrectDataCnt++;
        //}
    if(fscanf(fileio_load_sumMatrix, "%s\r\n", load_stringBuf)<0)            chk_incorrectFormCnt++;
            //printf("%s\r\n", load_stringBuf);//FILE_IO_DATA_PARSING_KEY_STRING_COL_DATA_END

    if(fscanf(fileio_load_sumMatrix, "%s\r\n", load_stringBuf)<0)                                                chk_incorrectFormCnt++;
            //printf("%s\r\n", load_stringBuf);

    if(fscanf(fileio_load_sumMatrix, "%s\r\n", load_stringBuf)<0)        chk_incorrectFormCnt++;
            //printf("%s\r\n", load_stringBuf);//FILE_IO_DATA_PARSING_KEY_STRING_RAW_DATA_START
    for(tmp_i=0; tmp_i<load_row; tmp_i++)
    {
        for(tmp_j=0; tmp_j<load_column; tmp_j++)
        {
            if(tmp_i != tmp_j)
            {
                if(tmp_j<(load_column-1))
                {
                    if(fscanf(fileio_load_sumMatrix, "%d(%d), ",
                        &load_exp_integer,
                        &load_index
                    )<0)    chk_incorrectFormCnt++;
                }
                else
                {
                    if(fscanf(fileio_load_sumMatrix, "%d(%d)",
                        &load_exp_integer,
                        &load_index
                    )<0)    chk_incorrectFormCnt++;
                }
                        //printf("%d(%d)%s", load_exp_integer, load_index, ((tmp_j<(load_column-1))?", ":""));
                (*((*(p->element+tmp_i))+tmp_j)) = (*(galoisFields->element+load_index));
            }
            else
            {
                if(tmp_j<(load_column-1))
                {
                    if(fscanf(fileio_load_sumMatrix, "%c(%d), ",
                        &load_exp_char,
                        &load_index
                    )<0)    chk_incorrectFormCnt++;
                }
                else
                {
                    if(fscanf(fileio_load_sumMatrix, "%c(%d)",
                        &load_exp_char,
                        &load_index
                    )<0)    chk_incorrectFormCnt++;
                }
                        //printf("%c(%d)%s", load_exp_char, load_index, ((tmp_j<(load_column-1))?", ":""));
                (*((*(p->element+tmp_i))+tmp_j)) = (*(galoisFields->element+load_index));
            }
        }
        if(fscanf(fileio_load_sumMatrix, "\r\n")<0)    chk_incorrectFormCnt++;
                //printf("\r\n");
    }
    if(fscanf(fileio_load_sumMatrix, "%s\r\n", load_stringBuf)<0)            chk_incorrectFormCnt++;
            //printf("%s\r\n", load_stringBuf);//FILE_IO_DATA_PARSING_KEY_STRING_RAW_DATA_END
    if(fscanf(fileio_load_sumMatrix, "%s\r\n", load_stringBuf)<0)        chk_incorrectFormCnt++;
            //printf("%s\r\n", load_stringBuf);//FILE_IO_DATA_PARSING_KEY_STRING_SAVED_DATA_END

    fclose(fileio_load_sumMatrix);

                printf("[load_struct_summationMatrix END]\r\n");

            #ifndef RELEASE
            if(global_flag_gfCommon_display&FLAG_MASK_DISPLAY_GF_COMMON_FILE_IO)
            {
                printf(">> Loaded Summation Matrix <<\r\n");
                printSummationMatrixSavedForm(p);
            }
            #endif



    if(chk_incorrectFormCnt || chk_incorrectDataCnt)    return 1;
    else                                                return 0;
}

void printSummationMatrixSavedForm(struct_summationMatrix *p)
{
    unsigned int tmp_i,tmp_j;

    #ifndef RELEASE
    if(!p)
    {
        errorMes;
        printf("in printSummationMatrixSavedForm, struct_summationMatrix *p is NULL\r\n");
        return;
    }
    #endif

    printf(">> Summation Matrix File I/O Form <<\n");

    for(tmp_i=0; tmp_i<p->row; tmp_i++)
    {
        for(tmp_j=0; tmp_j<p->column; tmp_j++)
        {
            if((*((*(p->element+tmp_i))+tmp_j))->index)
            {
                printf("%d(%d)%s",
                    ((*((*(p->element+tmp_i))+tmp_j))->index - 1),
                    (*((*(p->element+tmp_i))+tmp_j))->index,
                    ((tmp_j<(p->column-1))?", ":"")
                );
            }
            else
            {
                printf("%c(%d)%s",
                    '-',
                    (*((*(p->element+tmp_i))+tmp_j))->index,
                    ((tmp_j<(p->column-1))?", ":"")
                );

            }

        }
        printf("\r\n");
    }
}
