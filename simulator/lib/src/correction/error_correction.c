#include "polynomial.h"

char errorCorrection(struct_powerFormPolynomials *targetCodeWord, struct_powerFormPolynomials *errorLocationPowerFormPolynomial)
{
    unsigned int i;

    #ifndef RELEASE
    if(!targetCodeWord)
    {
        errorMes;
        printf("in errorCorrection, struct_powerFormPolynomials *targetCodeWord is NULL.\n");
        return -1;
    }
    if(!errorLocationPowerFormPolynomial)
    {
        errorMes;
        printf("in errorCorrection, struct_powerFormPolynomials *errorLocationPowerFormPolynomial is NULL.\n");
        return -1;
    }
    if(targetCodeWord->usedLength!=errorLocationPowerFormPolynomial->usedLength)
    {
        errorMes;
        printf("in errorCorrection, targetCodeWord->usedLength(%d)!=errorLocationPowerFormPolynomial->usedLength(%d).\n", targetCodeWord->usedLength, errorLocationPowerFormPolynomial->usedLength);
        printf("targetCodeWord->length(%d)!=errorLocationPowerFormPolynomial->length(%d).\n", targetCodeWord->length, errorLocationPowerFormPolynomial->length);
        return -1;
    }
    if(!targetCodeWord->usedLength)
    {
        errorMes;
        printf("in errorCorrection, targetCodeWord->usedLength(%d) and errorLocationPowerFormPolynomial->usedLength(%d) are zero.\n", targetCodeWord->usedLength, errorLocationPowerFormPolynomial->usedLength);
        return -1;
    }
    #endif


    #ifdef USING_OPEN_MP
    #pragma omp parallel for schedule(guided) private(i) shared(targetCodeWord, errorLocationPowerFormPolynomial)
    #endif
    for(i=0; i<targetCodeWord->usedLength; i++)
    {
        if(*(errorLocationPowerFormPolynomial->equation+i)=='1')
        {
            if(*(targetCodeWord->equation+i)=='1')
            {
                *(targetCodeWord->equation+i)='0';
            }
            else
            {
                *(targetCodeWord->equation+i)='1';
            }
        }
    }

            #ifndef RELEASE
            if((global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)==FLAG_MASK_PRINTF_LOG)
            {
                logMes;
                printf("corrected code : ");
                printPowerFormWithEnterPolynomialAndInfoUsingAddress(targetCodeWord);
            }
            #endif

    return 0;
}
