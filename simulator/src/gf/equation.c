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

/* Least Common Multiple(LCM) expantion */
char abstractOrderOfEquation(struct_galoisFieldElements *equation)
{
    unsigned int i;

    #ifndef RELEASE
    if(!equation)
    {
        errorMes;
        printf("in abstractOrderOfEquation, struct_galoisFieldElements *equation is NULL.\n");
        return 0;
    }
    if(!(equation->length))
    {
        errorMes;
        printf("in abstractOrderOfEquation, (equation->length) is ZERO.\n");
        return 0;
    }
    #endif

    for(i=equation->length-1; i!=0; i--)//can't check when i==0.
    {
        if(!checkAllValueUsingCharacterFromPolyForm(*(equation->element+i), '0'))
        {
            break;
        }
    }
    if(!i)//check when i==0;
    {
        return i;
    }
    else
    {
        if(!checkAllValueUsingCharacterFromPolyForm(*(equation->element+i), '0'))
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
}

//////////////////////////////////////////////////////////////////////////////////
//f(x)=(x+a)(x+b)(x+c)(x+d)(x+e) = x^5 + A*x^4 + B*x^3 + C*x^2 + D*x^2 + E*x^1 + F
struct_galoisFieldElements *expandRootOfLCM_usingEleOfGF
(

    struct_galoisFieldElements *galoisFields,
    struct_galoisFieldElements *equation,
    struct_setOfGaloisFieldElements *multipledRootSet,
    unsigned int numberOfSelectedSet//numberOfSelectedSet is same that number of correctable bit
)
{
    unsigned int i, j, k;//i is indicator of multipled Root Set, j indicate element of conjugacy class, k is indicator of equation, .
    unsigned int totalLcmOrder=0;

    unsigned int multipledOrder;

    struct_galoisFieldElements *beforeEquation;


    #ifndef RELEASE
    if(!equation)
    {
        errorMes;
        printf("in calculateLeastCommonMultipleUsingElementsOfGaloisField, struct_galoisFieldElements *equation is NULL.\n");
        return NULL;
    }
    if(!multipledRootSet)
    {
        errorMes;
        printf("in calculateLeastCommonMultipleUsingElementsOfGaloisField, struct_galoisFieldElements *multipledRootSet is NULL.\n");
        return NULL;
    }
    #endif


    if((multipledRootSet->length)<numberOfSelectedSet)
    {
                #ifndef RELEASE
                warningMes;    printf("in calculateLeastCommonMultipleUsingElementsOfGaloisField, ");
                warningMesShort; printf("number of selected set(=\'%d\') can not exceed to number of conjugacy set(=\'%d\').\n", numberOfSelectedSet, multipledRootSet->length);
                #endif

        numberOfSelectedSet=(multipledRootSet->length);
                #ifndef RELEASE
                warningMes; printf("number of conjugacy set that %d, which it is maximum length in this condition.\n", numberOfSelectedSet);
                #endif
    }

    for(i=0; i<numberOfSelectedSet; i++)
    {
        totalLcmOrder+=(*(multipledRootSet->conjugateSet+i))->length;
    }

    /* if order is 0, length is 1, if order is n-th, length is n+1. */
    multipledOrder = abstractOrderOfEquation(equation)+totalLcmOrder;

            #ifndef RELEASE
            if(equation->length<multipledOrder+1)
            {
                errorMes; printf("in calculateLeastCommonMultipleUsingElementsOfGaloisField, ");
                printf("not enough space to multiply polynomials(equation order is ");
                printf("\'%d\', totalLcmOrder order is \'%d\').\n", abstractOrderOfEquation(equation), totalLcmOrder);

                return NULL;
            }
            #endif


    beforeEquation = createGaloisFieldExceptElements(equation->length);
                    #ifndef RELEASE
                    if((global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)==FLAG_MASK_PRINTF_LOG)
                    {
                        logMes; printf("in expandRootOfLCM_usingEleOfGF, struct_galoisFieldElements *beforeEquation is made, length is \'%d\'.\n", equation->length);
                    }
                    if((global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)==FLAG_MASK_PRINTF_LOG)
                    {
                        logMes; printf("in expandRootOfLCM_usingEleOfGF, i is indicator of multipled Root Set, j indicate element of conjugacy class, k is indicator of equation.\n");
                    }
                    #endif
    for(i=0; i<numberOfSelectedSet; i++)
    {
        for(j=0; j<(*(multipledRootSet->conjugateSet+i))->length; j++)
        {
            //synchronize...
            if(copyListOfGaloisField(beforeEquation->element, equation->element, equation->length))
            {
                #ifndef RELEASE
                warningMes; printf("in expandRootOfLCM_usingEleOfGF, copyListOfGaloisField(beforeEquation->element, equation->element, equation->length) is not complete.\n");
                #endif
            }

                    #ifndef RELEASE
                    if((global_flag_cmdOption&FLAG_MASK_PRINTF_MEMORY_CHECK_PROCESS)==FLAG_MASK_PRINTF_MEMORY_CHECK_PROCESS)
                    {
                        memoryMes;
                        printf("1) in expandRootOfLCM_usingEleOfGF, memory check *(beforeEquation->element+global_tmp_i_memoryCheck '0' to \'%d\', equation->length is \'%d\'.\n", beforeEquation->length, equation->length);
                        printf("i=\'%d\', j=\'%d\', k=\'%d\'\n", i,j,k);
                        for(global_tmp_i_memoryCheck=0; global_tmp_i_memoryCheck<beforeEquation->length; global_tmp_i_memoryCheck++)
                        {
                            printf("*(beforeEquation->element+global_tmp_i_memoryCheck) = 0x%lx.\n", (unsigned long)(*(beforeEquation->element+global_tmp_i_memoryCheck)));
                        }
                    }
                    #endif

            for(k=0; k<equation->length; k++)
            {
                //calculate 0-th order, just multiple 0-th order and equation.
                        #ifndef RELEASE
                        if((global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)==FLAG_MASK_PRINTF_LOG)
                        {
                            logMes;
                            printf("in expandRootOfLCM_usingEleOfGF, \n");
                            printf("i=\'%d\', j=\'%d\', k=\'%d\'\n", i, j, k);
                            printf("*((*(multipledRootSet->conjugateSet+%d))->element+%d)\t{=>>}(ADDR:\'0x%lx\') == \t", i, j, (unsigned long)(*((*(multipledRootSet->conjugateSet+i))->element+j)));
                            printPolyFormWithEnter(*((*(multipledRootSet->conjugateSet+i))->element+j));
                            printf("\t\t\t*(beforeEquation->element+%d)\t{=>>}(ADDR:\'0x%lx\') == \t", k, (unsigned long)(*(beforeEquation->element+k)));
                            printPolyFormWithEnter(*(beforeEquation->element+k));
                        }
                        #endif
                *(equation->element+k)=multiElementsInGF_returnAddr(galoisFields, *((*(multipledRootSet->conjugateSet+i))->element+j), *(beforeEquation->element+k));
            }

                    #ifndef RELEASE
                    if((global_flag_cmdOption&FLAG_MASK_PRINTF_MEMORY_CHECK_PROCESS)==FLAG_MASK_PRINTF_MEMORY_CHECK_PROCESS)
                    {
                        memoryMes;
                        printf("2) in expandRootOfLCM_usingEleOfGF, memory check *(equation->element+global_tmp_i_memoryCheck) '0' to \'%d\', equation->length is \'%d\'.\n", equation->length, equation->length);
                        printf("i=\'%d\', j=\'%d\', k=\'%d\'\n", i,j,k);
                        for(global_tmp_i_memoryCheck=0; global_tmp_i_memoryCheck<equation->length; global_tmp_i_memoryCheck++)
                        {
                            printf("*(equation->element+global_tmp_i_memoryCheck) = 0x%lx.\n", (unsigned long)(*(equation->element+global_tmp_i_memoryCheck)));
                        }
                    }
                    #endif

            for(k=0; k<equation->length-1; k++)
            {
                //shifted orders
                *(equation->element+k+1)=sumElementInGF_usingSumMatrixReturnAddr(galoisFields, *(beforeEquation->element+k), *(equation->element+k+1));
            }
                    #ifndef RELEASE
                    if((global_flag_cmdOption&FLAG_MASK_PRINTF_MEMORY_CHECK_PROCESS)==FLAG_MASK_PRINTF_MEMORY_CHECK_PROCESS)
                    {
                        memoryMes;
                        printf("3) in expandRootOfLCM_usingEleOfGF, memory check *(equation->element+global_tmp_i_memoryCheck) '0' to \'%d\', equation->length is \'%d\'.\n", equation->length, equation->length);
                        printf("i=\'%d\', j=\'%d\', k=\'%d\'\n", i,j,k);
                        for(global_tmp_i_memoryCheck=0; global_tmp_i_memoryCheck<equation->length; global_tmp_i_memoryCheck++)
                        {
                            printf("*(equation->element+global_tmp_i_memoryCheck) = 0x%lx.\n", (unsigned long)(*(equation->element+global_tmp_i_memoryCheck)));
                        }
                    }
                    #endif
        }
    }

    closeGaloisFieldExceptElements(&beforeEquation);

    return equation;
}
//////////////////////////////////////////////////////////////////////////////////
struct_galoisFieldElements *test_expandLimitedLeastCommonMultipleUsingElementOfGaloisFieldToRoot
(
    struct_galoisFieldElements *galoisFields,
    struct_galoisFieldElements *equation,
    struct_setOfGaloisFieldElements *multipledRootSet,
    unsigned int numberOfSelectedSet//numberOfSelectedSet is same that number of correctable bit
)
{
    unsigned int i, j, k;//i is indicator of multipled Root Set, j indicate element of conjugacy class, k is indicator of equation, .
    unsigned int totalLcmOrder=0;

    unsigned int multipledOrder;

    struct_galoisFieldElements *beforeEquation;


    #ifndef RELEASE
    if(!equation)
    {
        errorMes;
        printf("in test_expandLimitedLeastCommonMultipleUsingElementOfGaloisFieldToRoot, struct_galoisFieldElements *equation is NULL.\n");
        return NULL;
    }
    if(!multipledRootSet)
    {
        errorMes;
        printf("in test_expandLimitedLeastCommonMultipleUsingElementOfGaloisFieldToRoot, struct_galoisFieldElements *multipledRootSet is NULL.\n");
        return NULL;
    }
    #endif


    if((multipledRootSet->length)<numberOfSelectedSet)
    {
                #ifndef RELEASE
                warningMes;    printf("in test_expandLimitedLeastCommonMultipleUsingElementOfGaloisFieldToRoot, ");
                warningMesShort; printf("number of selected set(=\'%d\') can not exceed to number of conjugacy set(=\'%d\').\n", numberOfSelectedSet, multipledRootSet->length);
                #endif

        numberOfSelectedSet=(multipledRootSet->length);
                #ifndef RELEASE
                warningMes; printf("number of conjugacy set that %d, which it is maximum length in this condition.\n", numberOfSelectedSet);
                #endif
    }

    for(i=0; i<numberOfSelectedSet; i++)
    {
        totalLcmOrder+=(*(multipledRootSet->limitedConjugateSet+i))->length;
    }

    /* if order is 0, length is 1, if order is n-th, length is n+1. */
    multipledOrder = abstractOrderOfEquation(equation)+totalLcmOrder;

            #ifndef RELEASE
            if(equation->length<multipledOrder+1)
            {
                errorMes; printf("in test_expandLimitedLeastCommonMultipleUsingElementOfGaloisFieldToRoot, ");
                printf("not enough space to multiply polynomials(equation order is ");
                printf("\'%d\', totalLcmOrder order is \'%d\').\n", abstractOrderOfEquation(equation), totalLcmOrder);

                return NULL;
            }
            #endif


    beforeEquation = createGaloisFieldExceptElements(equation->length);
                    #ifndef RELEASE
                    if((global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)==FLAG_MASK_PRINTF_LOG)
                    {
                        logMes; printf("in test_expandLimitedLeastCommonMultipleUsingElementOfGaloisFieldToRoot, struct_galoisFieldElements *beforeEquation is made, length is \'%d\'.\n", equation->length);
                    }
                    if((global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)==FLAG_MASK_PRINTF_LOG)
                    {
                        logMes; printf("in test_expandLimitedLeastCommonMultipleUsingElementOfGaloisFieldToRoot, i is indicator of multipled Root Set, j indicate element of conjugacy class, k is indicator of equation.\n");
                    }
                    #endif
    for(i=0; i<numberOfSelectedSet; i++)
    {
        for(j=0; j<(*(multipledRootSet->limitedConjugateSet+i))->length; j++)
        {
            //synchronize...
            if(copyListOfGaloisField(beforeEquation->element, equation->element, equation->length))
            {
                #ifndef RELEASE
                warningMes; printf("in test_expandLimitedLeastCommonMultipleUsingElementOfGaloisFieldToRoot, copyListOfGaloisField(beforeEquation->element, equation->element, equation->length) is not complete.\n");
                #endif
            }

                    #ifndef RELEASE
                    if((global_flag_cmdOption&FLAG_MASK_PRINTF_MEMORY_CHECK_PROCESS)==FLAG_MASK_PRINTF_MEMORY_CHECK_PROCESS)
                    {
                        memoryMes;
                        printf("1) in test_expandLimitedLeastCommonMultipleUsingElementOfGaloisFieldToRoot, memory check *(beforeEquation->element+global_tmp_i_memoryCheck '0' to \'%d\', equation->length is \'%d\'.\n", beforeEquation->length, equation->length);
                        printf("i=\'%d\', j=\'%d\', k=\'%d\'\n", i,j,k);
                        for(global_tmp_i_memoryCheck=0; global_tmp_i_memoryCheck<beforeEquation->length; global_tmp_i_memoryCheck++)
                        {
                            printf("*(beforeEquation->element+global_tmp_i_memoryCheck) = 0x%lx.\n", (unsigned long)(*(beforeEquation->element+global_tmp_i_memoryCheck)));
                        }
                    }
                    #endif

            for(k=0; k<equation->length; k++)
            {
                //calculate 0-th order, just multiple 0-th order and equation.
                        #ifndef RELEASE
                        if((global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)==FLAG_MASK_PRINTF_LOG)
                        {
                            logMes;
                            printf("in test_expandLimitedLeastCommonMultipleUsingElementOfGaloisFieldToRoot, \n");
                            printf("i=\'%d\', j=\'%d\', k=\'%d\'\n", i, j, k);
                            printf("*((*(multipledRootSet->limitedConjugateSet+%d))->element+%d)\t{=>>}(ADDR:\'0x%lx\') == \t", i, j, (unsigned long)(*((*(multipledRootSet->limitedConjugateSet+i))->element+j)));
                            printPolyFormWithEnter(*((*(multipledRootSet->limitedConjugateSet+i))->element+j));
                            printf("\t\t\t*(beforeEquation->element+%d)\t{=>>}(ADDR:\'0x%lx\') == \t", k, (unsigned long)(*(beforeEquation->element+k)));
                            printPolyFormWithEnter(*(beforeEquation->element+k));
                        }
                        #endif
                *(equation->element+k)=multiElementsInGF_returnAddr(galoisFields, *((*(multipledRootSet->limitedConjugateSet+i))->element+j), *(beforeEquation->element+k));
            }

                    #ifndef RELEASE
                    if((global_flag_cmdOption&FLAG_MASK_PRINTF_MEMORY_CHECK_PROCESS)==FLAG_MASK_PRINTF_MEMORY_CHECK_PROCESS)
                    {
                        memoryMes;
                        printf("2) in test_expandLimitedLeastCommonMultipleUsingElementOfGaloisFieldToRoot, memory check *(equation->element+global_tmp_i_memoryCheck) '0' to \'%d\', equation->length is \'%d\'.\n", equation->length, equation->length);
                        printf("i=\'%d\', j=\'%d\', k=\'%d\'\n", i,j,k);
                        for(global_tmp_i_memoryCheck=0; global_tmp_i_memoryCheck<equation->length; global_tmp_i_memoryCheck++)
                        {
                            printf("*(equation->element+global_tmp_i_memoryCheck) = 0x%lx.\n", (unsigned long)(*(equation->element+global_tmp_i_memoryCheck)));
                        }
                    }
                    #endif

            for(k=0; k<equation->length-1; k++)
            {
                //shifted orders
                *(equation->element+k+1)=sumElementInGF_usingSumMatrixReturnAddr(galoisFields, *(beforeEquation->element+k), *(equation->element+k+1));
            }
                    #ifndef RELEASE
                    if((global_flag_cmdOption&FLAG_MASK_PRINTF_MEMORY_CHECK_PROCESS)==FLAG_MASK_PRINTF_MEMORY_CHECK_PROCESS)
                    {
                        memoryMes;
                        printf("3) in test_expandLimitedLeastCommonMultipleUsingElementOfGaloisFieldToRoot, memory check *(equation->element+global_tmp_i_memoryCheck) '0' to \'%d\', equation->length is \'%d\'.\n", equation->length, equation->length);
                        printf("i=\'%d\', j=\'%d\', k=\'%d\'\n", i,j,k);
                        for(global_tmp_i_memoryCheck=0; global_tmp_i_memoryCheck<equation->length; global_tmp_i_memoryCheck++)
                        {
                            printf("*(equation->element+global_tmp_i_memoryCheck) = 0x%lx.\n", (unsigned long)(*(equation->element+global_tmp_i_memoryCheck)));
                        }
                    }
                    #endif
        }
    }

    closeGaloisFieldExceptElements(&beforeEquation);

    return equation;
}
