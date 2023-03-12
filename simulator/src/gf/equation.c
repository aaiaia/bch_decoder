#include "field.h"

///* Example of making equation *///
/*
//    struct_galoisFieldElements *alpha;
//    struct_galoisFieldElements *beta;
//
//    p->alpha = createGaloisFieldExceptElements(t+1);
//    p->beta = createGaloisFieldExceptElements(t+1);
//
//    initEquation(galoisFields, p->alpha);
//    initEquation(galoisFields, p->beta);
//
//    copyListOfGaloisField(p->beforeAlpha->element, p->alpha->element, p->alpha->length);
*/

/* Constructor and Destructor */
char initEquation(struct_galoisFieldElements *galoisFields, struct_galoisFieldElements *p)
{
    unsigned int i;

    #ifndef RELEASE
    if(!galoisFields)
    {
        errorMes;
        printf("in initEquation, struct_galoisFieldElements *galoisFields is zero\n");
        return -1;
    }
    if(!p)
    {
        errorMes;
        printf("in initEquation, struct_galoisFieldElements *p\n");
        return -1;
    }
    if(!(p->length))
    {
        errorMes;
        printf("in initEquation, p->length is zero\n");
        return -1;
    }
    #endif


    (*(p->element+0)) = (*(galoisFields->element+1));
    #ifdef USING_OPEN_MP
    #pragma omp parallel for schedule(guided) private(i) shared(p, galoisFields)
    #endif
    for(i=1; i<p->length; i++)
    {
        (*(p->element+i)) = (*(galoisFields->element+0));
    }
    return 0;
}

char initEquationFillAllZero(struct_galoisFieldElements *galoisFields, struct_galoisFieldElements *p)
{
    unsigned int i;

    #ifndef RELEASE
    if(!galoisFields)
    {
        errorMes;
        printf("in initEquation, struct_galoisFieldElements *galoisFields is zero\n");
        return -1;
    }
    if(!p)
    {
        errorMes;
        printf("in initEquation, struct_galoisFieldElements *p\n");
        return -1;
    }
    if(!(p->length))
    {
        errorMes;
        printf("in initEquation, p->length is zero\n");
        return -1;
    }
    #endif


    #ifdef USING_OPEN_MP
    #pragma omp parallel for schedule(guided) private(i) shared(p, galoisFields)
    #endif
    for(i=0; i<p->length; i++)
    {
        (*(p->element+i)) = (*(galoisFields->element+0));
    }
    return 0;
}

char initBinEquationUsingStr(struct_galoisFieldElements *galoisFields, struct_galoisFieldElements *p, char *str, unsigned int strlen)
{
    unsigned int i;
    unsigned int tmp_cnt;

    #ifndef RELEASE
    if(!galoisFields)
    {
        errorMes;
        printf("in initBinEquationUsingStr, struct_galoisFieldElements *galoisFields is zero\n");
        return -1;
    }
    if(!p)
    {
        errorMes;
        printf("in initBinEquationUsingStr, struct_galoisFieldElements *p\n");
        return -1;
    }
    if(!(p->length))
    {
        errorMes;
        printf("in initBinEquationUsingStr, p->length is zero\n");
        return -1;
    }
    if(!str)
    {
        errorMes;
        printf("in initBinEquationUsingStr, char *str is NULL\n");
        return -1;
    }
    if(!strlen)
    {
        errorMes;
        printf("in initBinEquationUsingStr, unsigned int strlen is zero\n");
        return -1;
    }
    #endif


    tmp_cnt=0;
    //#ifdef USING_OPEN_MP
    //#pragma omp parallel for schedule(guided) private(i) shared(p, galoisFields)
    //#endif
    for(i=0; i<strlen; i++)
    {
        if(str[i]=='1')
        {
            (*(p->element+i)) = (*(galoisFields->element+1));
        }
        else if(str[i]=='0')
        {
            (*(p->element+i)) = (*(galoisFields->element+0));
        }
        else
        {
            tmp_cnt++;
            (*(p->element+i)) = (*(galoisFields->element+1));
        }
    }

    if(tmp_cnt)
    {
        warningMesShort; printf("in initBinEquationUsingStr, %d characters are not 1 or 0 among %d characters.\n", tmp_cnt, i);
        warningMesShort; printf("in initBinEquationUsingStr, %d characters are alternated to 1.\n", tmp_cnt);
    }

    return 0;
}

/* Operation */
struct_galoisFieldElements* multipleTwoEqReturnAddr(
    struct_galoisFieldElements *galoisFields,
    struct_galoisFieldElements **result,
    struct_galoisFieldElements *operandEqA,
    struct_galoisFieldElements *operandEqB
)
{
    unsigned int tmp_order=0;
    struct_galoisFieldPolyForm *tmp_calBuf=NULL;

    unsigned int i;
    unsigned int j;
    #ifndef RELEASE
    if(!galoisFields)
    {
        errorMes;
        printf("in multipleTwoEqReturnAddr, struct_galoisFieldElements *galoisFields is null\n");
        return NULL;
    }
    if(!result)
    {
        errorMes;
        printf("in multipleTwoEqReturnAddr, struct_galoisFieldElements **result is null\n");
        return NULL;
    }
    if(!operandEqA)
    {
        errorMes;
        printf("in multipleTwoEqReturnAddr, struct_galoisFieldElements *p\n");
        return NULL;
    }
    if(!(operandEqA->length))
    {
        errorMes;
        printf("in multipleTwoEqReturnAddr, p->length is zero\n");
        return NULL;
    }
    if(!operandEqB)
    {
        errorMes;
        printf("in multipleTwoEqReturnAddr, struct_galoisFieldElements *p\n");
        return NULL;
    }
    if(!(operandEqB->length))
    {
        errorMes;
        printf("in multipleTwoEqReturnAddr, p->length is zero\n");
        return NULL;
    }
    #endif

    tmp_order=((operandEqA->length)+(operandEqB->length)-2);
    recreateGaloisFieldExceptElements(result, tmp_order+1);
    initEquationFillAllZero(galoisFields, *result);

    //*(equation->element+k+1)=sumElementInGF_usingSumMatrixReturnAddr(galoisFields, *(beforeEquation->element+k), *(equation->element+k+1));
    //*(equation->element+k)=multiElementsInGF_returnAddr(galoisFields, *((*(multipledRootSet->conjugateSet+i))->element+j), *(beforeEquation->element+k));
    for(i=0; i<operandEqA->length; i++)
    {
        for(j=0; j<operandEqB->length; j++)
        {
            tmp_calBuf=multiElementsInGF_returnAddr(galoisFields, (*(operandEqA->element+i)), (*(operandEqB->element+j)));
            (*((*result)->element+i+j))=sumElementInGF_usingSumMatrixReturnAddr(galoisFields, (*((*result)->element+i+j)), tmp_calBuf);
        }
    }
    return *result;
}

/* etc */
unsigned int checkDegreePolynomials(struct_galoisFieldElements *galoisFields, struct_galoisFieldElements *targetPolynomial)
{
    unsigned int i,j;
        //#ifndef RELEASE
        //    unsigned int bebuggingTemp;
        //    unsigned int bebuggingTemp2;
        //#endif
    if(!targetPolynomial) return -1;
    if(!galoisFields) return -1;

    for(i=targetPolynomial->length; i!=0; i--)
    {
        //#ifndef RELEASE
        //    bebuggingTemp = abstractIndexOfPowerFormInElementsOfGaloisFieldUsingIndex(galoisFields, *(galoisFields->element+(0)));
        //    bebuggingTemp2 = abstractIndexOfPowerFormInElementsOfGaloisFieldUsingIndex(galoisFields, *(targetPolynomial->element+(i-1)));
        //    debugMes; printf("index %d == index %d ??\r\n", bebuggingTemp, bebuggingTemp2);
        //#endif

        if(*(targetPolynomial->element+(i-1)) != *(galoisFields->element+(0))) break;
    }
    for(j=0; j<targetPolynomial->length; j++)
    {
        if(*(targetPolynomial->element+(j)) != *(galoisFields->element+(0)))    return i-1;
        else i--;
    }

    return 0;
}
