#include "field.h"

/* Operation */
unsigned int cal_numsOfDegreeOfSynd(unsigned int t)
{
    return 2*t;
}

char syndromeArrayCalculatorUsingString(struct_galoisFieldElements *galoisFields, struct_galoisFieldElements *syndrome, unsigned int lengthOfSyndromeArray, char *string)
{
    unsigned int i, j;
    struct_galoisFieldPolyForm **calculationBuffer;

    unsigned int strLength = strlen(string);


            #ifndef RELEASE
            if((strLength+1)>galoisFields->length)
            {
                errorMes;
                printf("strLength+1(%d) > galoisFields->length(%d)\n", (strLength+1), galoisFields->length);
                printf("Sring is \"%s\"\n", string);
                return -1;
            }
            #endif

    calculationBuffer = createListOfGaloisField(lengthOfSyndromeArray);

    #ifdef USING_OPEN_MP
    #pragma omp parallel for schedule(guided) private(i) shared(lengthOfSyndromeArray, syndrome, calculationBuffer, galoisFields)
    #endif
    /* to calculate syndrome, set initial condition. */
    for(i=0; i<lengthOfSyndromeArray; i++)
    {
        (*(syndrome->element+i))=(*(galoisFields->element+0));
        (*(calculationBuffer+i))=(*(galoisFields->element+0));
    }


    for(i=0; i<strLength; i++)
    {
        if(*(string+i)=='1')
        {

            *(calculationBuffer+0)=(*(galoisFields->element+1+i));//a^0 is decimalic '1'. so it is 1st element of galoisFields(start 0th).
            (*(syndrome->element+0))=sumElementInGF_usingSumMatrixReturnAddr(galoisFields, (*(syndrome->element+0)), *(calculationBuffer+0));

            *(calculationBuffer+1)=multiElementsInGF_returnAddr(galoisFields, *(calculationBuffer+0), *(calculationBuffer+0));
            (*(syndrome->element+1))=sumElementInGF_usingSumMatrixReturnAddr(galoisFields, (*(syndrome->element+1)), *(calculationBuffer+1));

            for(j=2; j<lengthOfSyndromeArray; j++)
            {
                *(calculationBuffer+j)=multiElementsInGF_returnAddr(galoisFields, *(calculationBuffer+j-1), *(calculationBuffer+0));
                (*(syndrome->element+j))=sumElementInGF_usingSumMatrixReturnAddr(galoisFields, (*(syndrome->element+j)), *(calculationBuffer+j));
            }
        }
    }

    closeListOfGaloisField(&calculationBuffer);

            #ifndef RELEASE
            if((global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)==FLAG_MASK_PRINTF_LOG)
            {
                logMes;
                printf("result of syndrome calculation\n");
                printGaloisField2(galoisFields, syndrome, PRINT_FIELD_SYNDROME_TITLE, 0);
            }
            #endif
    return 0;
}

char summationSyndromeArray
(
    struct_galoisFieldElements *galoisFields,
    struct_galoisFieldElements *resultSyndrome,
    struct_galoisFieldElements *operandA,
    struct_galoisFieldElements *operandB
)
{
    unsigned int i;

    if(!galoisFields) return -1;
    if(!resultSyndrome) return -1;
    if(!operandA) return -1;
    if(!operandB) return -1;

    if(!resultSyndrome->length) return -2;
    if(resultSyndrome->length!=operandA->length) return -3;
    if(operandA->length!=operandB->length) return -3;


    for(i=0; i<resultSyndrome->length; i++)
    {
        (*(resultSyndrome->element+i))=sumElementInGF_usingSumMatrixReturnAddr(galoisFields, (*(operandA->element+i)), (*(operandB->element+i)));
    }

    return 0;
}

char allSyndromeIsZero(struct_galoisFieldElements *galoisFields, struct_galoisFieldElements *targetSyndrome)
{
    unsigned int i;

    #ifndef RELEASE
    if(!galoisFields)
    {
        printf("in allSyndromeIsZero, if(!galoisFields) is NULL\r\n");
        return -1;
    }
    if(!targetSyndrome)
    {
        printf("in allSyndromeIsZero, if(!targetSyndrome) is NULL\r\n");
        return -1;
    }
    if(!targetSyndrome->length)
    {
        printf("in allSyndromeIsZero, if(!targetSyndrome->length) is zero\r\n");
        return -2;
    }
    #endif

    for(i=0; i<targetSyndrome->length; i++)
    {
        if((*(galoisFields->element+0))!=(*(targetSyndrome->element+i)))
        {
            return 0;
        }
    }
    return 1;
}

/* For Dubugging(Displaying) */
void printSyndromeArray(struct_galoisFieldElements *p, struct_galoisFieldElements *galoisFields)
{
    unsigned int i;

    unsigned int exponential;

    if(!p)
    {
        printf("in printSyndromeArray, struct_galoisFieldElements *p is NULL.\r\n");
        return;
    }


    printf("------------------ Syndrome Array ------------------\n");

    printf("syndrome length is \'%d\'\n", p->length);

    printf("index\tSn\tpow\t");
    for(i=0; i<(*(p->element))->length; i++)
    {
        printf("a^%d\t", i);
    }
    printf("\n");

    for(i=0; i<p->length; i++)
    {
        printf("%d :\tS_%d : ", i, i+1);
        exponential=abstractIndexOfPowerFormInElementsOfGaloisFieldUsingIndex(galoisFields, *(p->element+i));
        if(exponential==0)
        {
            printf("a^-\t");
        }
        else
        {
            printf("a^%d\t", exponential-1);
        }
        printPolyFormWithTapAndEnter(*(p->element+i));
    }
    printf("----------------------- END -----------------------\n");
}

void printSyndromeSeriesWithTap(struct_galoisFieldElements *p, struct_galoisFieldElements *galoisFields)
{
    unsigned int i;

    unsigned int exponential;

    for(i=0; i<p->length; i++)
    {
        printf("S_%d : ", i);
        exponential=abstractIndexOfPowerFormInElementsOfGaloisFieldUsingIndex(galoisFields, *(p->element+i));
        printf("a^");
        if(exponential==0)
        {
            printf("-\t");
        }
        else
        {
            printf("%d\t", exponential-1);
        }
    }
}

void printSyndromeSeriesWithTapAndEnter(struct_galoisFieldElements *p, struct_galoisFieldElements *galoisFields)
{
    unsigned int i;

    unsigned int exponential;

    for(i=0; i<p->length; i++)
    {
        printf("S_%d : ", i);
        exponential=abstractIndexOfPowerFormInElementsOfGaloisFieldUsingIndex(galoisFields, *(p->element+i));
        printf("a^");
        if(exponential==0)
        {
            printf("-\t");
        }
        else
        {
            printf("%d\t", exponential-1);
        }
    }
    printf("\n");
}
