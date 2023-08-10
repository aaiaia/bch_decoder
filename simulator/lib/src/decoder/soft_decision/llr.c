#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "common/cmd_opt_vars.h"
#include "decoder/soft_decision/llr.h"

/* Constructor and Destructor */
struct_logLikeHoodRatio* createLogLikeHoodRatio(unsigned int length)
{
    struct_logLikeHoodRatio *p;

    if(!length)
    {
        warningMes;
        printf("in createLlrValueUsingBitLength, length is '%d'.\n", length);
        return NULL;
    }
    p=(struct_logLikeHoodRatio*)malloc(sizeof(struct_logLikeHoodRatio));
    memset(p, 0, sizeof(struct_logLikeHoodRatio));

    p->llr=(double_RATIONAL_NUMBER*)malloc(sizeof(double_RATIONAL_NUMBER)*length);
    memset(p->llr, 0, sizeof(sizeof(double_RATIONAL_NUMBER)*length));

    p->squreRootAvrLLR=0.0;

    p->quantizedLLR=(s_int_QUANTIZ_DIGIT*)malloc(sizeof(s_int_QUANTIZ_DIGIT)*length);
    memset(p->quantizedLLR, 0, sizeof(sizeof(s_int_QUANTIZ_DIGIT)*length));
    p->quantizedLLRMask = 0;

    p->magnitude=(s_int_QUANTIZ_DIGIT*)malloc(sizeof(s_int_QUANTIZ_DIGIT)*length);
    memset(p->magnitude, -1, sizeof(sizeof(s_int_QUANTIZ_DIGIT)*length));

    p->length=length;
    p->usedLength=0;

    //p->magnitudeShort=(s_int_QUANTIZ_DIGIT*)malloc(sizeof(s_int_QUANTIZ_DIGIT)*length);
    //memset(p->magnitudeShort, 0, sizeof(sizeof(s_int_QUANTIZ_DIGIT)*length));
    p->magnitudeShort=NULL;
    p->hardDecisionShort=NULL;
    p->locator=NULL;
    p->locatorLength=0;

    p->treeStruct=NULL;

    return p;
}

char closeLogLikeHoodRatio(struct_logLikeHoodRatio **p)
{
    #ifndef RELEASE
    if(!p)
    {
        warningMes;
        printf("in closeLogLikeHoodRatio, struct_logLikeHoodRatio p is NULL.\n");
        return -1;
    }
    if(!*p)
    {
        warningMes;
        printf("in closeLogLikeHoodRatio, struct_logLikeHoodRatio *p is NULL.\n");
        return -1;
    }
    #endif

    if(((*p)->llr))
    {
        free((*p)->llr);
    }
    if(((*p)->quantizedLLR))
    {
        free((*p)->quantizedLLR);
    }
    if(((*p)->magnitude))
    {
        free((*p)->magnitude);
    }
    if(((*p)->magnitudeShort))
    {
        free((*p)->magnitudeShort);
    }
    if(((*p)->hardDecisionShort))
    {
        free((*p)->hardDecisionShort);
    }
    if(((*p)->locatorValidity))
    {
        free((*p)->locatorValidity);
    }
    if(((*p)->locator))
    {
        free((*p)->locator);
    }

    closeTreeStruct(&((*p)->treeStruct));

    free(*p);
    *p=NULL;
    return 0;
}

struct_logLikeHoodRatio* createLogLikeHoodRatioUsingNumberOfMaskBits(unsigned int length, unsigned int numberOfMask)
{
    //unsigned int i;
    struct_logLikeHoodRatio *p=NULL;

    p=createLogLikeHoodRatio(length);
            #ifndef RELEASE
            if(!(p))
            {
                warningMes;
                printf("in createLogLikeHoodRatioUsingNumberOfMaskBits, p=createLogLikeHoodRatio(%d) return NULL.\n", length);
                return NULL;
            }
            #endif

    if(setQuantizedLLRMaskOfLogLikeHoodRatio(p, numberOfMask))
    {
        closeLogLikeHoodRatio(&p);
                #ifndef RELEASE
                errorMes; printf("in createLogLikeHoodRatioUsingNumberOfMaskBits, setQuantizedLLRMaskOfLogLikeHoodRatio(p, numberOfMask) return Non-zero.\n");
                #endif
        return NULL;
    }


    //if(!(p->llr))
    //{
    //    if(!(p->length))
    //    {
    //        p->quantizedLLRMask=0;
    //        for(i=0; i<numberOfMask; i++)
    //        {
    //            p->quantizedLLRMask|=(1<<i);
    //        }
    //        p->magnitudeMask=p->quantizedLLRMask&(~(1<<(i-1)));
    //                printf("Mask test quantizat mask %d\nMask test magnitudeShort mask %d\n",p->quantizedLLRMask, p->magnitudeMask);
    //    }
    //    else
    //    {
    //        p->quantizedLLRMask=0;
    //        warningMes;
    //        printf("in initialLlrValueUsingMask, (*p)->length is zero.\n");
    //        return -1;
    //    }
    //}
    return p;
}

struct_logLikeHoodRatio* createLogLikeHoodRatioUsingLlrMask(unsigned int length, SIGNED_INT quantizedLLRMask)
{
    //unsigned int i;
    struct_logLikeHoodRatio *p=NULL;

    #ifndef RELEASE
    if(!quantizedLLRMask)
    {
        warningMes;
        printf("in createLogLikeHoodRatioUsingLlrMask, quantizedLLRMask is '%d'.\n", quantizedLLRMask);
        return NULL;
    }

    if(!(p=createLogLikeHoodRatio(length)))
    {
        warningMes;
        printf("in createLogLikeHoodRatioUsingLlrMask, p=createLogLikeHoodRatio(%d) return NULL.\n", length);
        return NULL;
    }
    #endif

    //if(!(p->llr))
    //{
    //    if(!(p->length))
    //    {
            p->quantizedLLRMask=quantizedLLRMask;
    //        //for(i=0; (i<(sizeof(s_int_QUANTIZ_DIGIT)*8))&&((1<<i)&quantizedLLRMask); i++);
    //        p->magnitudeMask=(p->quantizedLLRMask&(~(1<<(i-1))));
    //                printf("Mask test quantizat mask %d\nMask test magnitudeShort mask %d\n",p->quantizedLLRMask, p->magnitudeMask);
    //    }
    //    else
    //    {
    //        p->quantizedLLRMask=0;
    //        warningMes;
    //        printf("in initialLlrValueUsingMask, (*p)->length is zero.\n");
    //        return -1;
    //    }
    //}
    return p;
}

/* Initializer */
char initLogLikeHoodRatioLocator(struct_logLikeHoodRatio *p, unsigned int locatorLength)
{
    #ifndef RELEASE
    if(!p)
    {
        errorMes;
        printf("in initLogLikeHoodRatioLocator, struct_logLikeHoodRatio p is NULL.\n");
        return -1;
    }
    if(!locatorLength)
    {
        warningMes;
        printf("in initLogLikeHoodRatioLocator, unsigned int locatorLength is zero.\n");
        return -1;
    }
    if((p->locator))
    {
        warningMes;
        printf("in initLogLikeHoodRatioLocator, p->locator has any values.\n");
        printf("length of p->locator is '%d'\n", p->locatorLength);
        return 1;
    }
    if((p->magnitudeShort))
    {
        warningMes;
        printf("in initLogLikeHoodRatioLocator, p->magnitudeShort has any values.\n");
        printf("length of p->magnitudeShort is '%d'\n", p->locatorLength);
        return 1;
    }
    if((p->hardDecisionShort))
    {
        warningMes;
        printf("in initLogLikeHoodRatioLocator, p->hardDecisionShort has any values.\n");
        printf("length of p->hardDecisionShort is '%d'\n", p->locatorLength);
        return 1;
    }
    #endif

    p->magnitudeShort=(u_int_QUANTIZ_MAGNITUDE_DIGIT*)malloc(sizeof(u_int_QUANTIZ_MAGNITUDE_DIGIT)*locatorLength);
    memset(p->magnitudeShort, -1, sizeof(u_int_QUANTIZ_MAGNITUDE_DIGIT)*locatorLength);

    p->hardDecisionShort=(char*)malloc(sizeof(char)*locatorLength);
    memset(p->hardDecisionShort, -1, sizeof(char)*locatorLength);

    p->locatorValidity=(unsigned char*)malloc(sizeof(unsigned int)*locatorLength);
    memset(p->locatorValidity, 0, sizeof(unsigned char)*locatorLength);

    p->locator=(unsigned int*)malloc(sizeof(unsigned int)*locatorLength);
    memset(p->locator, -1, sizeof(unsigned int)*locatorLength);

    p->locatorLength=locatorLength;
    return 0;
}

/*
 * To find minumum value of magnitude, initializing values associate with tree in struct struct_logLikeHoodRatio.
 * Tree, can use, is define that is in enum_treeTypes.
 * If you needs this list, find a enum_treeTypes.
 */
char initializingVariablesAssociateWithTree(struct_logLikeHoodRatio *p, enum_treeTypes treeType)
{
    #ifndef RELEASE
    if(!p)
    {
        warningMes;
        printf("in initializingVariablesAssociateWithTree, struct_logLikeHoodRatio p is NULL.\n");
        return -1;
    }
    if(!(p->usedLength))
    {
        warningMes;
        printf("in initializingVariablesAssociateWithTree, struct_logLikeHoodRatio p->usedLength is zero.\n");
        warningMes;
        printf("To initializing tree, this value must be setted, first.\n");
        return -1;
    }
    #endif

    if((p->treeStruct))
    {
                #ifndef RELEASE
                warningMes;    printf("in initializingVariablesAssociateWithTree, p->treeStruct is not NULL.\n");
                warningMesShort;    printf("p->treeStruct will be removed.\n");
                #endif

        closeTreeStruct(&(p->treeStruct));
    }

    (p->treeStruct)=createTreeStruct(p->usedLength, treeType);
            #ifndef RELEASE
            if(!(p->treeStruct))//create tree
            {
                warningMes;
                printf("in initializingVariablesAssociateWithTree, can't createTreeStruct.\n");
                return -1;
            }
            #endif

    return 0;
}

/* Configure */
char setUsedLengthOfLogLikeHoodRatio(struct_logLikeHoodRatio *p, unsigned int usedLength)
{
    #ifndef RELEASE
    if(!p)
    {
        errorMes;
        printf("in setUsedLengthOfLogLikeHoodRatio, struct_logLikeHoodRatio p is NULL.\n");
        return -1;
    }
    if(!(p->quantizedLLR))
    {
        errorMes;
        printf("in setUsedLengthOfLogLikeHoodRatio, p->quantizedLLR is NULL.\n");
        return -1;
    }
    if(p->length<usedLength)
    {
        errorMes;
        printf("in setUsedLengthOfLogLikeHoodRatio, satisfing p->length(=%d)<usedLength(=%d).\n", p->length, usedLength);
        return -1;
    }
    #endif

    p->usedLength=usedLength;
    return 0;
}

char setQuantizedLLRMaskOfLogLikeHoodRatio(struct_logLikeHoodRatio *p, unsigned int numberOfMask)
{
    unsigned int i;

    #ifndef RELEASE
    if(!p)
    {
        errorMes;
        printf("in setQuantizedLLRMaskOfLogLikeHoodRatio, struct_logLikeHoodRatio p is NULL.\n");
        return -1;
    }
    if(!numberOfMask)
    {
        warningMes;
        printf("in setQuantizedLLRMaskOfLogLikeHoodRatio, numberOfMask is '%d'.\n", numberOfMask);
        return -1;
    }
    #endif


    p->quantizedLLRMask=0;
    for(i=0; ((i<(sizeof(s_int_QUANTIZ_DIGIT)*8))&&(i<numberOfMask)); i++)
    {
        p->quantizedLLRMask|=(1<<i);
    }
    return 0;
}

/* Operation */
double_RATIONAL_NUMBER getSqureRootAvrLLR(struct_logLikeHoodRatio *p)
{
    unsigned int i;
    double_RATIONAL_NUMBER tmpAvr=0.0;

    for(i=0; i<p->usedLength; i++)
    {
        tmpAvr+=((*(p->llr+i))*(*(p->llr+i)));
    }
    tmpAvr/=((double_RATIONAL_NUMBER)p->usedLength);
    sqrt(tmpAvr);
    p->squreRootAvrLLR=tmpAvr;
    return tmpAvr;
}

/* Converter */
char convertQuantizedLLRToMagnitude(struct_logLikeHoodRatio *p)
{
    unsigned int tmp_llr_mag_mask;

    #ifndef RELEASE
    if(!p)
    {
        errorMes;
        printf("in convertQuantizedLLRToMagnitude, struct_logLikeHoodRatio p is NULL.\n");
        return -1;
    }
    if(!(p->quantizedLLR))
    {
        warningMes;
        printf("in convertQuantizedLLRToMagnitude, p->quantizedLLR is NULL.\n");
        return -1;
    }
    if(!(p->usedLength))
    {
        warningMes;
        printf("in convertQuantizedLLRToMagnitude, p->usedLength is zero.\n");
        return -1;
    }
    if(!(p->locator))
    {
        warningMes;
        printf("in convertQuantizedLLRToMagnitude, p->locator is NULL.\n");
        return -1;
    }
    if(!(p->magnitude))
    {
        warningMes;
        printf("in convertQuantizedLLRToMagnitude, p->magnitudeShort is NULL.\n");
        return -1;
    }
    if(!(p->usedLength))
    {
        warningMes;
        printf("in convertQuantizedLLRToMagnitude, p->magnitude is zero.\n");
        return -1;
    }
    #endif

    tmp_llr_mag_mask = (p->quantizedLLRMask >> 1);

    memset(p->magnitude, tmp_llr_mag_mask, sizeof(u_int_QUANTIZ_MAGNITUDE_DIGIT)*(p->usedLength));

    switch(global_flag_case_llr_numberic_system)
    {
        /* 2'complement */
        default:
        case ENUM_FLAG_CASE_QUANTIZ_NUMBERIC_SYSTEM_TWO_S_COMPLEMENT:
        {
            convertTwosComplementMagitude(
                p->magnitude,
                p->quantizedLLR,
                p->usedLength,
                tmp_llr_mag_mask);
        }
        break;


        /* 1'complement */
        case ENUM_FLAG_CASE_QUANTIZ_NUMBERIC_SYSTEM_ONE_S_COMPLEMENT:
        {
            convertOnesComplementMagnitude(
                    p->magnitude, p->quantizedLLR, p->usedLength);
        }
        break;
    }
    return 0;
}

/* Verificator */
char checkMinimumMagnitudeLocatorValidityLogLikeHoodRatio(
    struct_logLikeHoodRatio *p,
    struct_powerFormPolynomials *operandA,
    struct_powerFormPolynomials *operandB)
{
    unsigned int i;
    #ifndef RELEASE
    if(!p)    return -1;
    if(!(p->locatorLength)) return -1;
    if(!(p->locatorValidity)) return -1;

    if(!operandA) return -1;
    if(!(operandA->length)) return -1;
    if(!(operandA->usedLength)) return -1;
    if(!(operandA->equation)) return -1;

    if(!operandB) return -1;
    if(!(operandB->length)) return -1;
    if(!(operandB->usedLength)) return -1;
    if(!(operandB->equation)) return -1;
    #endif

    for(i=0; i<p->locatorLength; i++)
    {
        /* locator is always smaller than codeword Length */
        if(((*(p->locator+i)) < (operandA->usedLength)) && ((*(p->locator+i)) < (operandB->usedLength)))
        {
            if((*(operandA->equation+(*(p->locator+i)))) != (*(operandB->equation+(*(p->locator+i)))))    *(p->locatorValidity+i) = 1;
            else                                                                                        *(p->locatorValidity+i) = 0;
        }
        /* not indication any location */
        else if((*(p->locator+i)) == -1)
        {
            *(p->locatorValidity+i) = 1;
        }
        else
        {
            errorMes;
            printf("in checkMinimumMagnitudeLocatorValidityLogLikeHoodRatio, (*(p->locator+%d)='%d' exceed (operandA->usedLength)='%d' or (operandB->usedLength)='%d'\n",
                i, (*(p->locator+i)), (operandA->usedLength), (operandB->usedLength) );
            return -2;
        }
    }
    return 0;
}

/* For displying(debugging) */
void printQuatizLLRWithSpaceAndEnter(struct_logLikeHoodRatio *p)
{
    unsigned int i;

    if(!p)
    {
        errorMes;
        printf("in printQuatizLLRWithSpaceAndEnter, struct_logLikeHoodRatio p is NULL.\n");
        return;
    }
    if(!(p->quantizedLLR))
    {
        warningMes;
        printf("in printQuatizLLRWithSpaceAndEnter, struct_logLikeHoodRatio p->quantizedLLR is NULL.\n");
        return;
    }
    if(!(p->usedLength))
    {
        warningMes;
        printf("in printQuatizLLRWithSpaceAndEnter, struct_logLikeHoodRatio p->usedLength is zero.\n");
        return;
    }
    if(!(p->quantizedLLRMask))
    {
        warningMes;
        printf("in printQuatizLLRWithSpaceAndEnter, struct_logLikeHoodRatio p->quantizedLLRMask is zero.\n");
        return;
    }
    printf("[Quantiz LLR] : \t\t\t");
    for(i=0; i<p->usedLength; i++)
    {
        if(*(p->quantizedLLR+i)>=0)
        {
            printf("+%d ", *(p->quantizedLLR+i));
        }
        else
        {
            printf("%d ", *(p->quantizedLLR+i));
        }
    }
    printf("[Used Length : %d]\n", p->usedLength);
}

void printTestMagitudeQuatizLLRWithSpaceAndEnter(struct_logLikeHoodRatio *p)
{
    unsigned int i;

    if(!p)
    {
        errorMes;
        printf("in printTestMagitudeQuatizLLRWithSpaceAndEnter, struct_logLikeHoodRatio p is NULL.\n");
        return;
    }
    if(!(p->quantizedLLR))
    {
        warningMes;
        printf("in printTestMagitudeQuatizLLRWithSpaceAndEnter, struct_logLikeHoodRatio p->quantizedLLR is NULL.\n");
        return;
    }
    if(!(p->usedLength))
    {
        warningMes;
        printf("in printTestMagitudeQuatizLLRWithSpaceAndEnter, struct_logLikeHoodRatio p->usedLength is zero.\n");
        return;
    }
    if(!(p->quantizedLLRMask))
    {
        warningMes;
        printf("in printTestMagitudeQuatizLLRWithSpaceAndEnter, struct_logLikeHoodRatio p->quantizedLLRMask is zero.\n");
        return;
    }
    printf("[magnitudeShort Quantiz LLR] : \t");
    for(i=0; i<p->usedLength; i++)
    {
        if(*(p->quantizedLLR+i)>0)    printf("+%d '%c' ", *(p->quantizedLLR+i), *(p->hardDecisionShort+i));
        else                        printf("+%d '%c' ", -1*(*(p->quantizedLLR+i)), *(p->hardDecisionShort+i));
    }
    printf("[Used Length : %d]\n", p->usedLength);
}

void printLLRWithSpaceAndEnter(struct_logLikeHoodRatio *p)
{
    unsigned int i;

    if(!p)
    {
        errorMes;
        printf("in printLLRWithSpaceAndEnter, struct_logLikeHoodRatio p is NULL.\n");
        return;
    }
    if(!(p->llr))
    {
        warningMes;
        printf("in printLLRWithSpaceAndEnter, struct_logLikeHoodRatio p->llr is NULL.\n");
        return;
    }
    if(!(p->usedLength))
    {
        warningMes;
        printf("in printLLRWithSpaceAndEnter, struct_logLikeHoodRatio p->usedLength is zero.\n");
        return;
    }
    //if(!(p->quantizedLLRMask))
    //{
    //    warningMes;
    //    printf("in printLLRWithSpaceAndEnter, struct_logLikeHoodRatio p->quantizedLLRMask is zero.\n");
    //    return;
    //}
    printf("[LLR] : ");
    for(i=0; i<p->usedLength; i++)
    {
        printf("%.2f ", *(p->llr+i));
    }
    printf("[Used Length : %d]\n", p->usedLength);
}

char printMagnitudeOfLLRWithSpaceAndEnter(struct_logLikeHoodRatio *p)
{
    unsigned int i;

    if(!p)
    {
        errorMes;
        printf("in printMagnitudeOfLLRWithSpaceAndEnter, struct_logLikeHoodRatio p is NULL.\n");
        return -1;
    }
    if(!(p->magnitude))
    {
        warningMes;
        printf("in printMagnitudeOfLLRWithSpaceAndEnter, struct_logLikeHoodRatio p->quantizedLLR is NULL.\n");
        return -1;
    }
    if(!(p->usedLength))
    {
        warningMes;
        printf("in printMagnitudeOfLLRWithSpaceAndEnter, struct_logLikeHoodRatio p->usedLength is zero.\n");
        return -1;
    }

    printf("[Magnitude of LLR]\r\n");
    for(i=0; i<p->usedLength; i++)
    {
            printf("%d", *(p->magnitude+i));
    }
    printf("[Used Length : %d]\n", p->usedLength);
    return 0;
}

char printMagnitudeOfLLR_andLocationWithSpaceAndEnter(struct_logLikeHoodRatio *p)
{
    unsigned int i;

    if(!p)
    {
        errorMes;
        printf("in printMagnitudeOfLLR_andLocationWithSpaceAndEnter, struct_logLikeHoodRatio p is NULL.\n");
        return -1;
    }
    if(!(p->magnitude))
    {
        warningMes;
        printf("in printMagnitudeOfLLR_andLocationWithSpaceAndEnter, struct_logLikeHoodRatio p->quantizedLLR is NULL.\n");
        return -1;
    }
    if(!(p->usedLength))
    {
        warningMes;
        printf("in printMagnitudeOfLLR_andLocationWithSpaceAndEnter, struct_logLikeHoodRatio p->usedLength is zero.\n");
        return -1;
    }

    printf("[Magnitude of LLR] : \t");
    for(i=0; i<p->usedLength; i++)
    {
            printf("%d(%d) ", *(p->magnitude+i), i);
    }
    printf("[Used Length : %d]\n", p->usedLength);
    return 0;
}

void testPrintLLRWithSpaceAndEnter(struct_logLikeHoodRatio *p)
{
    unsigned int i;

    if(!p)
    {
        errorMes;
        printf("in testPrintLLRWithSpaceAndEnter, struct_logLikeHoodRatio p is NULL.\n");
        return;
    }
    if(!(p->llr))
    {
        warningMes;
        printf("in testPrintLLRWithSpaceAndEnter, struct_logLikeHoodRatio p->llr is NULL.\n");
        return;
    }
    if(!(p->usedLength))
    {
        warningMes;
        printf("in testPrintLLRWithSpaceAndEnter, struct_logLikeHoodRatio p->usedLength is zero.\n");
        return;
    }

    printf("[LLR and Qantized LLR]\n");
    for(i=0; i<p->usedLength; i++)
    {
        if(*(p->llr+i)>=0)  printf("+%g\t", *(p->llr+i));
        else                printf("%g\t", *(p->llr+i));
        if(*(p->quantizedLLR+i)>=0) printf("+%d\n", *(p->quantizedLLR+i));
        else                        printf("%d\n", *(p->quantizedLLR+i));
    }
    printf("[Used Length : %d]\n", p->usedLength);
}

void testPrintShortMinimumQuantizedLLRAndLocator(struct_logLikeHoodRatio *p)
{
    unsigned int i;

    if(!p)
    {
        errorMes;
        printf("in testPrintShortMinimumQuantizedLLRAndLocator, !p\n");
        return;
    }
    if(!(p->locatorLength))
    {
        errorMes;
        printf("in testPrintShortMinimumQuantizedLLRAndLocator, !(p->locatorLength)\n");
        return;
    }
    if(!(p->magnitudeShort))
    {
        errorMes;
        printf("in testPrintShortMinimumQuantizedLLRAndLocator, !(p->magnitudeShort)\n");
        return;
    }
    if(!(p->hardDecisionShort))
    {
        errorMes;
        printf("in testPrintShortMinimumQuantizedLLRAndLocator, !(p->hardDecisionShort)\n");
        return;
    }
    if(!(p->locator))
    {
        errorMes;
        printf("in testPrintShortMinimumQuantizedLLRAndLocator, !(p->locator).\n");
        return;
    }
    if(!(p->locatorValidity))
    {
        errorMes;
        printf("in testPrintShortMinimumQuantizedLLRAndLocator, !(p->locatorValidity).\n");
        return;
    }
    printf("[magnitudeShort, locator and validity]\n");
    for(i=0; i<p->locatorLength; i++)
    {
        printf("[magnitudeShort]\t%d\t[HD]\t'%c'\t[Location]\t%d\t[Validity]\t%d\n",
                *(p->magnitudeShort+i), *(p->hardDecisionShort+i), *(p->locator+i), *(p->locatorValidity+i) );
    }
}

/* File IO */
char fprintQuatizLLR_toHEX(FILE* fp, struct_logLikeHoodRatio* p, char* str_tail)
{
    unsigned int i;
    unsigned int j;
    unsigned int bitLength;
    unsigned int hexLength;
    unsigned int hexRedundantBitLength;
    unsigned int hexRedundantBitMask;
    unsigned int uint_tmp;
    const int HEX_BIT_LENGTH = 4;

    #ifndef RELEASE
    if(!fp)
    {
        errorMes;
        printf("in fprintQuatizLLR_toHEX, FILE* fp is NULL.\n");
        return -1;
    }
    if(!p)
    {
        errorMes;
        printf("in fprintQuatizLLR_toHEX, struct_logLikeHoodRatio p is NULL.\n");
        return -1;
    }
    if(!(p->quantizedLLR))
    {
        warningMes;
        printf("in fprintQuatizLLR_toHEX, struct_logLikeHoodRatio p->quantizedLLR is NULL.\n");
        return -1;
    }
    if(!(p->usedLength))
    {
        warningMes;
        printf("in fprintQuatizLLR_toHEX, struct_logLikeHoodRatio p->usedLength is zero.\n");
        return -1;
    }
    if(!(p->quantizedLLRMask))
    {
        warningMes;
        printf("in fprintQuatizLLR_toHEX, struct_logLikeHoodRatio p->quantizedLLRMask is zero.\n");
        return -1;
    }
    #endif
    /* Check up LLR bit mask */
    bitLength = 0;
    uint_tmp = 0;
    uint_tmp |= p->quantizedLLRMask;
    for(i=0; uint_tmp&1; i++)
    {
        bitLength++;
        uint_tmp = (uint_tmp >> 1);
    }
    hexLength = (bitLength) / HEX_BIT_LENGTH;
    hexRedundantBitLength = (bitLength) % HEX_BIT_LENGTH;

    hexRedundantBitMask = 0;
    if(hexRedundantBitLength)
    {
        hexLength++;
        for(i = 0; i < hexRedundantBitLength; i++)
        {
            hexRedundantBitMask |= (1<<i);
        }
    }

    for(i=0; i<p->usedLength; i++)
    {
        for(j = hexLength; j != 0; j--)
        {
            if(j == hexLength)
            {
                if(hexRedundantBitLength)
                {
                    fprintf(fp, "%01x", (((*(p->quantizedLLR+i)) >> ((j-1)*4)) & hexRedundantBitMask));
                }
                else
                {
                    fprintf(fp, "%01x", (((*(p->quantizedLLR+i)) >> ((j-1)*4)) & 0xF));
                }
            }
            else
            {
                fprintf(fp, "%01x", (((*(p->quantizedLLR+i)) >> ((j-1)*4)) & 0xF));
            }
        }
    }

    if(str_tail) fprintf(fp, str_tail);

    return 0;
}

char fprintVerilogQuatizLLR_toHEX(FILE* fp, struct_logLikeHoodRatio* p, char* str_tail)
{
    unsigned int i;
    unsigned int j;
    unsigned int bitLength;
    unsigned int hexLength;
    unsigned int hexRedundantBitLength;
    unsigned int hexRedundantBitMask;
    unsigned int uint_tmp;
    const int HEX_BIT_LENGTH = 4;

    #ifndef RELEASE
    if(!fp)
    {
        errorMes;
        printf("in fprintQuatizLLR_toHEX, FILE* fp is NULL.\n");
        return -1;
    }
    if(!p)
    {
        errorMes;
        printf("in fprintQuatizLLR_toHEX, struct_logLikeHoodRatio p is NULL.\n");
        return -1;
    }
    if(!(p->quantizedLLR))
    {
        warningMes;
        printf("in fprintQuatizLLR_toHEX, struct_logLikeHoodRatio p->quantizedLLR is NULL.\n");
        return -1;
    }
    if(!(p->usedLength))
    {
        warningMes;
        printf("in fprintQuatizLLR_toHEX, struct_logLikeHoodRatio p->usedLength is zero.\n");
        return -1;
    }
    if(!(p->quantizedLLRMask))
    {
        warningMes;
        printf("in fprintQuatizLLR_toHEX, struct_logLikeHoodRatio p->quantizedLLRMask is zero.\n");
        return -1;
    }
    #endif
    /* Check up LLR bit mask */
    bitLength = 0;
    uint_tmp = 0;
    uint_tmp |= p->quantizedLLRMask;
    for(i=0; uint_tmp&1; i++)
    {
        bitLength++;
        uint_tmp = (uint_tmp >> 1);
    }
    hexLength = (bitLength) / HEX_BIT_LENGTH;
    hexRedundantBitLength = (bitLength) % HEX_BIT_LENGTH;

    hexRedundantBitMask = 0;
    if(hexRedundantBitLength)
    {
        hexLength++;
        for(i = 0; i < hexRedundantBitLength; i++)
        {
            hexRedundantBitMask |= (1<<i);
        }
    }

    for(i=0; i<p->usedLength; i++)
    {
        for(j = hexLength; j != 0; j--)
        {
            if(j == hexLength)
            {
                if(hexRedundantBitLength)
                {
                    fprintf(fp, "%01x\r\n", (((*(p->quantizedLLR+i)) >> ((j-1)*4)) & hexRedundantBitMask));
                }
                else
                {
                    fprintf(fp, "%01x\r\n", (((*(p->quantizedLLR+i)) >> ((j-1)*4)) & 0xF));
                }
            }
            else
            {
                fprintf(fp, "%01x\r\n", (((*(p->quantizedLLR+i)) >> ((j-1)*4)) & 0xF));
            }
        }
    }

    if(str_tail) fprintf(fp, str_tail);

    return 0;
}

char fprintQuatizLLR_fullDescriptionToHEX(FILE* fp, struct_logLikeHoodRatio* p, char* str_tail)
{
    unsigned int i;
    unsigned int j;
    unsigned int bitLength;
    unsigned int hexLength;
    unsigned int hexRedundantBitLength;
    unsigned int hexRedundantBitMask;
    unsigned int uint_tmp;
    const int HEX_BIT_LENGTH = 4;

    #ifndef RELEASE
    if(!fp)
    {
        errorMes;
        printf("in fprintQuatizLLR_fullDescriptionToHEX, FILE* fp is NULL.\n");
        return -1;
    }
    if(!p)
    {
        errorMes;
        printf("in fprintQuatizLLR_fullDescriptionToHEX, struct_logLikeHoodRatio p is NULL.\n");
        return -1;
    }
    if(!(p->quantizedLLR))
    {
        warningMes;
        printf("in fprintQuatizLLR_fullDescriptionToHEX, struct_logLikeHoodRatio p->quantizedLLR is NULL.\n");
        return -1;
    }
    if(!(p->usedLength))
    {
        warningMes;
        printf("in fprintQuatizLLR_fullDescriptionToHEX, struct_logLikeHoodRatio p->usedLength is zero.\n");
        return -1;
    }
    if(!(p->quantizedLLRMask))
    {
        warningMes;
        printf("in fprintQuatizLLR_fullDescriptionToHEX, struct_logLikeHoodRatio p->quantizedLLRMask is zero.\n");
        return -1;
    }
    #endif
    /* Check up LLR bit mask */
    bitLength = 0;
    uint_tmp = 0;
    uint_tmp |= p->quantizedLLRMask;
    for(i=0; uint_tmp&1; i++)
    {
        bitLength++;
        uint_tmp = (uint_tmp >> 1);
    }
    hexLength = (bitLength) / HEX_BIT_LENGTH;
    hexRedundantBitLength = (bitLength) % HEX_BIT_LENGTH;

    hexRedundantBitMask = 0;
    if(hexRedundantBitLength)
    {
        hexLength++;
        for(i = 0; i < hexRedundantBitLength; i++)
        {
            hexRedundantBitMask |= (1<<i);
        }
    }

    for(i=0; i<p->usedLength; i++)
    {
        if(*(p->quantizedLLR+i)>=0)
        {
            fprintf(fp, "+%d(", *(p->quantizedLLR+i));
        }
        else
        {
            fprintf(fp, "%d(", *(p->quantizedLLR+i));
        }

        for(j = hexLength; j!= 0; j--)
        {
            if(j == hexLength)
            {
                if(hexRedundantBitLength)
                {
                    fprintf(fp, "%01x", (((*(p->quantizedLLR+i)) >> ((j-1)*4)) & hexRedundantBitMask));
                }
                else
                {
                    fprintf(fp, "%01x", (((*(p->quantizedLLR+i)) >> ((j-1)*4)) & 0xF));
                }
            }
            else
            {
                fprintf(fp, "%01x", (((*(p->quantizedLLR+i)) >> ((j-1)*4)) & 0xF));
            }
        }
        fprintf(fp, ") ");

    }
    fprintf(fp, "[Used Length : %d]", p->usedLength);
    if(str_tail) fprintf(fp, str_tail);

    return 0;
}

char fprintMagnitudeOfQuantizedLLR_toHex(FILE *fp, struct_logLikeHoodRatio *p, char *str_tail)
{
    unsigned int i;
    unsigned int j;
    unsigned int bitLength;
    unsigned int hexLength;
    unsigned int hexRedundantBitLength;
    unsigned int hexRedundantBitMask;
    unsigned int uint_tmp;
    const int HEX_BIT_LENGTH = 4;
    #ifndef RELEASE
    if(!fp)
    {
        errorMes;
        printf("in fprintMagnitudeOfLLR, FILE* fp is NULL.\n");
        return -1;
    }

    if(!p)
    {
        errorMes;
        printf("in fprintMagnitudeOfLLR, struct_logLikeHoodRatio p is NULL.\n");
        return -1;
    }
    if(!(p->magnitude))
    {
        warningMes;
        printf("in fprintMagnitudeOfLLR, struct_logLikeHoodRatio p->quantizedLLR is NULL.\n");
        return -1;
    }
    if(!(p->usedLength))
    {
        warningMes;
        printf("in fprintMagnitudeOfLLR, struct_logLikeHoodRatio p->usedLength is zero.\n");
        return -1;
    }
    #endif
    /* Check up LLR bit mask */
    bitLength = 0;
    uint_tmp = 0;
    uint_tmp |= p->quantizedLLRMask;
    for(i=0; uint_tmp&1; i++)
    {
        bitLength++;
        uint_tmp = (uint_tmp >> 1);
    }
    hexLength = (bitLength - 1) / HEX_BIT_LENGTH;
    hexRedundantBitLength = (bitLength - 1) % HEX_BIT_LENGTH;

    hexRedundantBitMask = 0;
    if(hexRedundantBitLength)
    {
        hexLength++;
        for(i = 0; i < hexRedundantBitLength; i++)
        {
            hexRedundantBitMask |= (1<<i);
        }
    }

    for(i = 0; i < p->usedLength; i++)
    {
        for(j = hexLength; j!= 0; j--)
        {
            if(j == hexLength)
            {
                if(hexRedundantBitLength)
                {
                    fprintf(fp, "%01x", (((*(p->magnitude+i)) >> ((j-1)*4)) & hexRedundantBitMask));
                }
                else
                {
                    fprintf(fp, "%01x", (((*(p->magnitude+i)) >> ((j-1)*4)) & 0xF));
                }
            }
            else
            {
                fprintf(fp, "%01x", (((*(p->magnitude+i)) >> ((j-1)*4)) & 0xF));
            }
        }
    }

    if(str_tail) fprintf(fp, str_tail);
    return 0;
}

char fprintVerilogMagnitudeOfQuantizedLLR_toHex(FILE *fp, struct_logLikeHoodRatio *p, char *str_tail)
{
    unsigned int i;
    unsigned int j;
    unsigned int bitLength;
    unsigned int hexLength;
    unsigned int hexRedundantBitLength;
    unsigned int hexRedundantBitMask;
    unsigned int uint_tmp;
    const int HEX_BIT_LENGTH = 4;
    #ifndef RELEASE
    if(!fp)
    {
        errorMes;
        printf("in fprintMagnitudeOfLLR, FILE* fp is NULL.\n");
        return -1;
    }

    if(!p)
    {
        errorMes;
        printf("in fprintMagnitudeOfLLR, struct_logLikeHoodRatio p is NULL.\n");
        return -1;
    }
    if(!(p->magnitude))
    {
        warningMes;
        printf("in fprintMagnitudeOfLLR, struct_logLikeHoodRatio p->quantizedLLR is NULL.\n");
        return -1;
    }
    if(!(p->usedLength))
    {
        warningMes;
        printf("in fprintMagnitudeOfLLR, struct_logLikeHoodRatio p->usedLength is zero.\n");
        return -1;
    }
    #endif
    /* Check up LLR bit mask */
    bitLength = 0;
    uint_tmp = 0;
    uint_tmp |= p->quantizedLLRMask;
    for(i=0; uint_tmp&1; i++)
    {
        bitLength++;
        uint_tmp = (uint_tmp >> 1);
    }
    hexLength = (bitLength - 1) / HEX_BIT_LENGTH;
    hexRedundantBitLength = (bitLength - 1) % HEX_BIT_LENGTH;

    hexRedundantBitMask = 0;
    if(hexRedundantBitLength)
    {
        hexLength++;
        for(i = 0; i < hexRedundantBitLength; i++)
        {
            hexRedundantBitMask |= (1<<i);
        }
    }

    for(i = 0; i < p->usedLength; i++)
    {
        for(j = hexLength; j!= 0; j--)
        {
            if(j == hexLength)
            {
                if(hexRedundantBitLength)
                {
                    fprintf(fp, "%01x\r\n", (((*(p->magnitude+i)) >> ((j-1)*4)) & hexRedundantBitMask));
                }
                else
                {
                    fprintf(fp, "%01x\r\n", (((*(p->magnitude+i)) >> ((j-1)*4)) & 0xF));
                }
            }
            else
            {
                fprintf(fp, "%01x\r\n", (((*(p->magnitude+i)) >> ((j-1)*4)) & 0xF));
            }
        }
    }

    if(str_tail) fprintf(fp, str_tail);
    return 0;
}

char fprintMagnitudeOfQuantizedLLR_fullDescription_toHex(FILE *fp, struct_logLikeHoodRatio *p, char *str_tail)
{
    unsigned int i;
    unsigned int j;
    unsigned int bitLength;
    unsigned int hexLength;
    unsigned int hexRedundantBitLength;
    unsigned int hexRedundantBitMask;
    unsigned int uint_tmp;
    const int HEX_BIT_LENGTH = 4;

    #ifndef RELEASE
    if(!fp)
    {
        errorMes;
        printf("in fprintMagnitudeOfQuantizedLLR_fullDescription_toHex, FILE* fp is NULL.\n");
        return -1;
    }
    if(!p)
    {
        errorMes;
        printf("in fprintMagnitudeOfQuantizedLLR_fullDescription_toHex, struct_logLikeHoodRatio p is NULL.\n");
        return -1;
    }
    if(!(p->quantizedLLR))
    {
        warningMes;
        printf("in fprintMagnitudeOfQuantizedLLR_fullDescription_toHex, struct_logLikeHoodRatio p->quantizedLLR is NULL.\n");
        return -1;
    }
    if(!(p->usedLength))
    {
        warningMes;
        printf("in fprintMagnitudeOfQuantizedLLR_fullDescription_toHex, struct_logLikeHoodRatio p->usedLength is zero.\n");
        return -1;
    }
    if(!(p->quantizedLLRMask))
    {
        warningMes;
        printf("in fprintMagnitudeOfQuantizedLLR_fullDescription_toHex, struct_logLikeHoodRatio p->quantizedLLRMask is zero.\n");
        return -1;
    }
    #endif

    if(!(p->quantizedLLRMask))
    {
        warningMes;
        printf("in fprintMagnitudeOfQuantizedLLR_fullDescription_toHex, quantization bit mask not exist.\r\b");
        return 1;
    }

    /* Check up LLR bit mask */
    bitLength = 0;
    uint_tmp = 0;
    uint_tmp |= p->quantizedLLRMask;
    for(i=0; uint_tmp&1; i++)
    {
        bitLength++;
        uint_tmp = (uint_tmp >> 1);
    }
    hexLength = (bitLength - 1) / HEX_BIT_LENGTH;
    hexRedundantBitLength = (bitLength - 1) % HEX_BIT_LENGTH;

    hexRedundantBitMask = 0;
    if(hexRedundantBitLength)
    {
        hexLength++;
        for(i = 0; i < hexRedundantBitLength; i++)
        {
            hexRedundantBitMask |= (1<<i);
        }
    }


    for(i=0; i<p->usedLength; i++)
    {
        if(*(p->quantizedLLR+i)>0)    fprintf(fp, "+%d(", *(p->quantizedLLR+i));
        else                        fprintf(fp, "+%d(", -1*(*(p->quantizedLLR+i)));

        for(j = hexLength; j!= 0; j--)
        {
            if(j == hexLength)
            {
                if(hexRedundantBitLength)
                {
                    fprintf(fp, "%01x", (((*(p->magnitude+i)) >> ((j-1)*4)) & hexRedundantBitMask));
                }
                else
                {
                    fprintf(fp, "%01x", (((*(p->magnitude+i)) >> ((j-1)*4)) & 0xF));
                }
            }
            else
            {
                fprintf(fp, "%01x", (((*(p->magnitude+i)) >> ((j-1)*4)) & 0xF));
            }
        }
        fprintf(fp, ") ");
    }

    fprintf(fp, "[Used Length : %d]", p->usedLength);

    if(str_tail) fprintf(fp, str_tail);

    return 0;
}
