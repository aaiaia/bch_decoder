#include "field_set.h"

/* Constructor and Desctructor */
struct_galoisField_info *createGaloisField_info_emptySpace(struct_powerFormPolynomials *primitivePolynomial)
{
    struct_galoisField_info *p;

    #ifndef RELEASE
    if(!primitivePolynomial->length)
    {
        errorMes;
        printf("primitive polynomial length is %d\n", primitivePolynomial->length);
        return NULL;
    }
    if(!primitivePolynomial->usedLength)
    {
        errorMes;
        printf("primitive polynomial usedLength is %d\n", primitivePolynomial->usedLength);
        return NULL;
    }
    #endif


    p=(struct_galoisField_info*)malloc(sizeof(struct_galoisField_info));
    memset(p, 0, sizeof(struct_galoisField_info));

    // p->gfBits=&(p->length);

    p->primitivePolynomial=primitivePolynomial;


    if(!(p->nonSorted=createGaloisFieldAndElements(intPower(2, (p->primitivePolynomial->usedLength)-1), p->primitivePolynomial->usedLength-1)))
    {
        errorMes; printf("in createGaloisField_info_emptySpace, createGaloisFieldAndElements has error.\r\n");
    }

            #ifndef RELEASE
            if((global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)==FLAG_MASK_PRINTF_LOG)
            {
                logMes;
                printf("primitive polynomial length is \'%d\'(used length) and allocated memory length is \'%d\' \n", primitivePolynomial->length, primitivePolynomial->usedLength);
                printf("number of elements of galois field is %d\n", p->nonSorted->length);
            }
            #endif

    if(!(p->nonSorted->summationReferMatrix=createSummationMatrix_emptySpace(p->nonSorted->length, p->nonSorted->length)))
    {
        errorMes; printf("in createGaloisField_info_emptySpace, createSummationMatrix_emptySpace has error.\r\n");
    }

    return p;
}

//struct_galoisField_info *createGaloisField(struct_powerFormPolynomials *primitivePolynomial)
//{
//    #ifndef RELEASE
//    if(!primitivePolynomial)
//    {
//        in
//    }
//    #endif
//    struct_galoisField_info *p;
//}

//struct_galoisField_info *createGaloisField_deleted_201604251820(struct_powerFormPolynomials *primitivePolynomial)
//{
//    unsigned int i,j;
//
//    struct_galoisField_info *p;
//
//    char_GALOIS_FIELD_VALUE shiftBuffer;
//
//    #ifndef RELEASE
//    if(!primitivePolynomial->length)
//    {
//        errorMes;
//        printf("primitive polynomial length is %d\n", primitivePolynomial->length);
//        return NULL;
//    }
//    if(!primitivePolynomial->usedLength)
//    {
//        errorMes;
//        printf("primitive polynomial usedLength is %d\n", primitivePolynomial->usedLength);
//        return NULL;
//    }
//    #endif
//
//
//    p=(struct_galoisField_info*)malloc(sizeof(struct_galoisField_info));
//    memset(p, 0, sizeof(struct_galoisField_info));
//
//    //p->gfBits=&(p->length);
//
//    p->primitivePolynomial=primitivePolynomial;
//
//
//    p->nonSorted=createGaloisFieldAndElements(intPower(2, (p->primitivePolynomial->usedLength)-1), p->primitivePolynomial->usedLength-1);
//
//            #ifndef RELEASE
//            if((global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)==FLAG_MASK_PRINTF_LOG)
//            {
//                logMes;
//                printf("primitive polynomial length is \'%d\'(used length) and allocated memory length is \'%d\' \n", primitivePolynomial->length, primitivePolynomial->usedLength);
//                printf("number of elements of galois field is %d\n", p->nonSorted->length);
//            }
//            #endif
//
//    *((*(p->nonSorted->element+1))->value+0)='1';
//    //printf("((*(p->nonSorted->element+1))->length) %d <= (sizeof(u_int_GALOIS_FIELD_INTEGER)*8) %d\n", ((*(p->nonSorted->element+1))->length), (sizeof(uint32_t)*8));
//    if(((*(p->nonSorted->element+1))->length)<=(sizeof(u_int_GALOIS_FIELD_INTEGER)*8))
//    {
//        (*(p->nonSorted->element+1))->intValue=convertGaloisFieldPolyFormUnsignedInt(*(p->nonSorted->element+1));
//    }
//    (*(p->nonSorted->element+1))->index=1;
//
//            #ifndef RELEASE
//            if((global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)==FLAG_MASK_PRINTF_LOG)
//            {
//                logMes;
//                printf("(p->nonSorted->length) = %d\n", (p->nonSorted->length));
//                printf("(*(p->nonSorted->element+0))->length = %d\n", (*(p->nonSorted->element+0))->length);
//            }
//            #endif
//
//
//    //printf("[%05d]%s\n", 0,(*(p->nonSorted->element+0))->value);
//    //printf("[%05d]%s\n", 1,(*(p->nonSorted->element+1))->value);
//    infoMes; printf("[START][Generating GaloisField Elements]\n");
//    for(i=2; i<(p->nonSorted->length); i++)
//    {
//        shiftBuffer=(*((*(p->nonSorted->element+i-1))->value+((*(p->nonSorted->element+i))->length-1)));
//        for(j=(*(p->nonSorted->element+i))->length-1; j!=0; j--)
//        {
//            (*((*(p->nonSorted->element+i))->value+j))=(*((*(p->nonSorted->element+i-1))->value+j-1));
//        }
//        (*((*(p->nonSorted->element+i))->value+j))='0';
//        (*((*(p->nonSorted->element+i))->value+(*(p->nonSorted->element+i))->length))=0;
//        (*((*(p->nonSorted->element+i-1))->value+((*(p->nonSorted->element+i))->length-1)))=0;
//
//        if(shiftBuffer=='1')
//        {
//            for(j=0; j<(*(p->nonSorted->element+i))->length; j++)
//            {
//                if((*(primitivePolynomial->equation+j))!=(*((*(p->nonSorted->element+i))->value+j)))
//                {
//                    (*((*(p->nonSorted->element+i))->value+j))='1';
//                }
//                else
//                {
//                    (*((*(p->nonSorted->element+i))->value+j))='0';
//                }
//            }
//            shiftBuffer='0';
//        }
//
//
//        if(((*(p->nonSorted->element+i))->length)<=(sizeof(u_int_GALOIS_FIELD_INTEGER)*8))
//        {
//            (*(p->nonSorted->element+i))->intValue=convertGaloisFieldPolyFormUnsignedInt(*(p->nonSorted->element+i));
//        }
//        (*(p->nonSorted->element+i))->index=i;
//
//        printf("[%05d]%s\n", i,(*(p->nonSorted->element+i))->value);
//    }
//    infoMes; printf("[END][Generating GaloisField Elements]\n");
//
//
//
//
//    /* create struct_summationMatrix */
//    infoMes; printf("[START][Generating summation matrix]\n");
//    p->nonSorted->summationReferMatrix=createSummationMatrix(p->nonSorted, p->nonSorted->length, p->nonSorted->length);
//    infoMes; printf("[END][Generating summation matrix]\n");
//
//    return p;
//}

struct_galoisField_info *createGaloisFieldAndSumMatrix(struct_powerFormPolynomials *primitivePolynomial)
{
    struct_galoisField_info *p;

    #ifndef RELEASE
    if(!primitivePolynomial->length)
    {
        errorMes;
        printf("primitive polynomial length is %d\n", primitivePolynomial->length);
        return NULL;
    }
    if(!primitivePolynomial->usedLength)
    {
        errorMes;
        printf("primitive polynomial usedLength is %d\n", primitivePolynomial->usedLength);
        return NULL;
    }
    #endif

    if(!(p=createGaloisField_info_emptySpace(primitivePolynomial)))
    {
        errorMes; printf("in createGaloisFieldAndSumMatrix, createGaloisField_info_emptySpace has error.\r\n");
    }
    else
    {
        if(calculateGaloisField(primitivePolynomial, p->nonSorted)<0)
        {
            errorMes; printf("in createGaloisFieldAndSumMatrix, calculateGaloisField has error.\r\n");
        }

        /*create struct_summationMatrix*/
        //p->nonSorted->summationReferMatrix=createSummationMatrix(p->nonSorted, p->nonSorted->length, p->nonSorted->length);\
        //printSummationMatrixSavedForm(p->nonSorted->summationReferMatrix);
        if(calculateSummationMatrix(p->nonSorted, p->nonSorted->summationReferMatrix))
        {
            errorMes; printf("in createGaloisFieldAndSumMatrix, calculateSummationMatrix has error.\r\n");
        }
    }
    return p;
}

/* For Debugging(Displaying) */
void printGaloisField(struct_galoisFieldElements *p)
{
    unsigned int i;

    unsigned int exponential;

    printf("\n\n------------------ Galois Field ------------------\n");
    infoMes;
    printf("number of element is \'%d\', GF(%d)\n", p->length, p->length);

    printf("pow\t");
    for(i=0; i<(*(p->element))->length; i++)
    {
        printf("a^%d\t", i);
    }
    printf("decimal\tindex");
    printf("\n");

    for(i=0; i<p->length; i++)
    {
        exponential=abstractIndexOfPowerFormInElementsOfGaloisFieldUsingIndex(p, *(p->element+i));
        if(exponential==0)
        {
            printf("a^-\t");
        }
        else
        {
            printf("a^%d\t", exponential-1);
        }
        printPolyFormWithTap(*(p->element+i));
        printf("%d\t", (*(p->element+i))->intValue);
        //intValue
                //printf("%d", convertGaloisFieldPolyFormUnsignedInt(*(p->element+i)));
        printf("%d", (*(p->element+i))->index);
            if((global_flag_cmdOption&FLAG_MASK_PRINTF_MEMORY_CHECK_PROCESS)==FLAG_MASK_PRINTF_MEMORY_CHECK_PROCESS)
            {
                printf("ADDR : 0x%lx\n", (unsigned long)(*(p->element+i)));
            }
        printf("\n");
    }
    printf("---------------------- END ----------------------\n");
}

void printGaloisFieldToCsv(struct_galoisFieldElements *p)
{
    unsigned int i;

    unsigned int exponential;

    printf("------------------ Galois Field ------------------\n");
    infoMes;
    printf("number of element is \'%d\' GF(%d)\n", p->length, p->length);

    printf("pow\t");
    for(i=0; i<(*(p->element))->length; i++)
    {
        printf("a^%d\t", i);
    }
    printf("dec(abs)");
    printf("\n");

    for(i=0; i<p->length; i++)
    {
        exponential=abstractIndexOfPowerFormInElementsOfGaloisFieldUsingIndex(p, *(p->element+i));
        if(exponential==0)
        {
            printf("a^-, ");
        }
        else
        {
            printf("a^%d, ", exponential-1);
        }
        printf("/");
        printPolyFormPure(*(p->element+i));
        printf(", ");

        printf("%d\n", convertGaloisFieldPolyFormUnsignedInt(*(p->element+i)));
            if((global_flag_cmdOption&FLAG_MASK_PRINTF_MEMORY_CHECK_PROCESS)==FLAG_MASK_PRINTF_MEMORY_CHECK_PROCESS)
            {
                printf("ADDR : 0x%lx\n", (unsigned long)(*(p->element+i)));
            }
    }
    printf("---------------------- END ----------------------\n");
}

void printGaloisField2(struct_galoisFieldElements *baseGaloisField, struct_galoisFieldElements *printedField, unsigned int title, unsigned int options)
{
    unsigned int i;

    unsigned int exponential;

    if(!baseGaloisField)
    {
        errorMes;
        printf("in printGaloisField2, struct_galoisFieldElements *baseGaloisField is NULL.\n");
        return;
    }
    if(!printedField)
    {
        errorMes;
        printf("in printGaloisField2, struct_galoisFieldElements *printedField is NULL.\n");
        return;
    }

    printf("\n------------------ ");
    if(title==PRINT_FIELD_GALOIS_FIELD_TITLE)
    {
        printf("Galois Field");
    }
    else if(title==PRINT_FIELD_SYNDROME_TITLE)
    {
        printf("Syndrome");
    }
    else if(title==PRINT_FIELD_EQUATION_TITLE)
    {
        printf("Equation");
    }
    else
    {
        printf("Undefined kind");
    }
    printf(" ------------------\n\n");

    printf(">> numberOfElement = %d\n", printedField->length);
    if((options&PRINT_FIELD_PRINT_OPTION_NUMBERING)==PRINT_FIELD_PRINT_OPTION_NUMBERING)
    {
        printf("index\t");
    }
    printf("pow\t");
    for(i=0; i<(*(printedField->element))->length; i++)
    {
        printf("a^%d\t", i);
    }
    printf("decimal\tdec[abstract]");
    printf("\n");

    for(i=0; i<printedField->length; i++)
    {
        if((options&PRINT_FIELD_PRINT_OPTION_NUMBERING)==PRINT_FIELD_PRINT_OPTION_NUMBERING)
        {
            printf("%d : \t", i);
        }
        exponential=abstractIndexOfPowerFormInElementsOfGaloisFieldUsingIndex(baseGaloisField, *(printedField->element+i));
        if(exponential==0)
        {
            printf("a^-\t");
        }
        else
        {
            printf("a^%d\t", exponential-1);
        }
        printPolyFormWithTap(*(printedField->element+i));
        printf("%d\t%d", (*(printedField->element+i))->intValue, convertGaloisFieldPolyFormUnsignedInt(*(printedField->element+i)));

        if((global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)==FLAG_MASK_PRINTF_LOG)
        {
            logMes;
            printf("\t\tpolyForm length=%d", (*(printedField->element+i))->length);
        }
        printf("\n");

    }
    printf("---------------------- END ----------------------\n");
}
