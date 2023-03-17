#include <stdlib.h>
#include <string.h>

#include "gf/field.h"
#include "gf/field_set.h"
#include "gf/syndrome.h"
#include "gf/equation.h"
#include "decoder/hard_decision/algo_bm.h"

/* local variables */
static char local_buf_KeyBoardInput[255] = {0};

/* Constructor and Destructor */
struct_HD_BM_algorithmComponent *createBmAlgorithmComponent(struct_galoisFieldElements *galoisFields, unsigned int t, char *string, unsigned int stringLength)
{
    struct_HD_BM_algorithmComponent *p;

    //double galoisFieldElementLengthBuffer;
    //unsigned int galoisFieldElementLength;

    #ifndef RELEASE
    if(!t)
    {
        errorMes;
        printf("correctable number(t) is %d\n", t);
    }
    if(!string)
    {
        errorMes;
        printf("to calculate syndrome, string(cordWord) is not inserted\n");
    }

    if(!stringLength)
    {
        warningMesShort; printf("to calculate syndrome, string(cordWord) length is %d\n", stringLength);
        warningMesShort; printf("string : %s\n", string);
        stringLength=strlen(string);
    }
    #endif

            #ifndef RELEASE
            if((global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)==FLAG_MASK_PRINTF_LOG)
            {
                logMes; printf("codeWord is received and set a information to correct error.\n");
            }
            #endif
    p = (struct_HD_BM_algorithmComponent*)malloc(sizeof(struct_HD_BM_algorithmComponent));
    memset(p, 0, sizeof(struct_HD_BM_algorithmComponent));


    p->numberOfCorrection = t;
            #ifndef RELEASE
            if((global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)==FLAG_MASK_PRINTF_LOG)
            {
                logMes; printf("number of correctable error is \'%d\', ", p->numberOfCorrection);
            }
            #endif

    p->vMAX = cal_numsOfDegreeOfSynd(t);/* use for, when variable is 6, it is break. */
            #ifndef RELEASE
            if((global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)==FLAG_MASK_PRINTF_LOG)
            {
                logMes; printf("vMAX is \'%d\'\n", p->vMAX);
            }
            #endif

    p->codeWord = createPowerFormPolynomialUsingString(string, stringLength);
    p->errorLocationVector = createPowerFormPolynomial(stringLength);

            #ifndef RELEASE
            if((global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)==FLAG_MASK_PRINTF_LOG)
            {
                logMes; printf("received codeword is below : \n");
                printPowerFormWithEnterPolynomialAndInfoUsingAddress(p->codeWord);
            }
            #endif

        //galoisFieldElementLengthBuffer=log((double)stringLength)/log(2.0);
        //galoisFieldElementLength=(unsigned int)galoisFieldElementLengthBuffer;

        //while((galoisFieldElementLengthBuffer-((double)galoisFieldElementLength))!=0)
        //{
        //    stringLength++;
        //    galoisFieldElementLengthBuffer=log((double)stringLength)/log(2.0);
        //    galoisFieldElementLength=(unsigned int)galoisFieldElementLengthBuffer;
        //}

    /* create syndrome of received vector */
    p->syndrome = createGaloisFieldExceptElements(p->vMAX+global_bch_Soft_OverSyndLen);
    /* calculate syndrome */
    syndromeArrayCalculatorUsingString(galoisFields, p->syndrome, p->vMAX+global_bch_Soft_OverSyndLen, string);

    /* create syndrome of error location */
    p->errLocSyndrome = createGaloisFieldExceptElements(p->vMAX+global_bch_Soft_OverSyndLen);
    /* create syndrome about sum syndromes of received and error location vector */
    p->sumReceiveAndErrLocSyndrome = createGaloisFieldExceptElements(p->vMAX+global_bch_Soft_OverSyndLen);



    /* BM algorithm initial condition setting. */
    //p->value_r = 0;

    /* Initializing degree of Error Location Poly */
    p->degreeOfErrLocPoly=-1;

    /* clear metricCheck */
    p->metricCheck=0;

    /* create alpha and beta formula and set initial condition when value_r is -1. */
    p->alpha = createGaloisFieldExceptElements(t+1);
    p->beta = createGaloisFieldExceptElements(t+1);
        p->errLocPoly = p->alpha;
    initEquation(galoisFields, p->alpha);
    initEquation(galoisFields, p->beta);

            #ifndef RELEASE
            if(global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)
            {
                infoMes;
                printf(">> alpha");
                printGaloisField2(galoisFields, p->alpha, PRINT_FIELD_EQUATION_TITLE, 0);
                infoMes;
                printf(">> beta");
                printGaloisField2(galoisFields, p->beta, PRINT_FIELD_EQUATION_TITLE, 0);
            }
            #endif

    /* create output of register */
    p->beforeAlpha = createGaloisFieldExceptElements(t+1);
    p->beforeBeta = createGaloisFieldExceptElements(t+1);
    /* synchronize register and wire, pulse input liked */
    copyListOfGaloisField(p->beforeAlpha->element, p->alpha->element, p->alpha->length);
    copyListOfGaloisField(p->beforeBeta->element, p->beta->element, p->alpha->length);


    /* initial condition of length of Stage is 0. */
    p->lengthOfStage=0;

    /* indexing kind of bch decoding algorithm */
    p->KIND_OF_BCH_ALGORITHM = KIND_OF_BCH_DECODING_BM;

    return p;
}

char closeBmAlgorithmComponent(struct_HD_BM_algorithmComponent **p)
{
    #ifndef RELEASE
    if(!p)
    {
        if(!(global_flag_cmdSystemUnhold&FLAG_MASK_SYSTEM_UNHOLD_FORCED))
        {
            errorMes;
            printf("in closeBmAlgorithmComponent, closeBmAlgorithmComponent &p = 0x%lx\n", (unsigned long)p);
        }
        return -1;
    }
    if(!*p)
    {
        if(!(global_flag_cmdSystemUnhold&FLAG_MASK_SYSTEM_UNHOLD_FORCED))
        {
            warningMes;
            printf("in closeBmAlgorithmComponent, closeBmAlgorithmComponent *p = 0x%lx\n", (unsigned long)(*p));
        }
        return -1;
    }
    #endif

    #ifndef RELEASE
        if(closePowerFormPolynomial(&((*p)->codeWord)))
        {
            warningMes; printf("in closeBmAlgorithmComponent, closePowerFormPolynomial(&((*p)->codeWord)) is fail.\n");
        }
    #else
        closePowerFormPolynomial(&((*p)->codeWord));
    #endif

    #ifndef RELEASE
        if(closePowerFormPolynomial(&((*p)->errorLocationVector)))
        {
            warningMes; printf("in closeBmAlgorithmComponent, closePowerFormPolynomial(&((*p)->codeWord)) is fail.\n");
        }
    #else
        closePowerFormPolynomial(&((*p)->errorLocationVector));
    #endif

    #ifndef RELEASE
        if(closeGaloisFieldExceptElements(&((*p)->syndrome)))
        {
            warningMes;
            printf("in closeBmAlgorithmComponent, closeGaloisFieldExceptElements(&((*p)->syndrome)) is fail.\n");
        }

        if(closeGaloisFieldExceptElements(&((*p)->errLocSyndrome)))
        {
            warningMes;
            printf("in closeBmAlgorithmComponent, closeGaloisFieldExceptElements(&((*p)->errLocSyndrome)) is fail.\n");
        }
        if(closeGaloisFieldExceptElements(&((*p)->sumReceiveAndErrLocSyndrome)))
        {
            warningMes;
            printf("in closeBmAlgorithmComponent, closeGaloisFieldExceptElements(&((*p)->sumReceiveAndErrLocSyndrome)) is fail.\n");
        }
    #else
        closeGaloisFieldExceptElements(&((*p)->syndrome));

        closeGaloisFieldExceptElements(&((*p)->errLocSyndrome));
        closeGaloisFieldExceptElements(&((*p)->sumReceiveAndErrLocSyndrome));
    #endif

    #ifndef RELEASE
        if(closeGaloisFieldExceptElements(&((*p)->alpha)))
        {
            warningMes;
            printf("in closeBmAlgorithmComponent, closeGaloisFieldExceptElements(&((*p)->alpha)) is fail.\n");
        }
    #else
        closeGaloisFieldExceptElements(&((*p)->alpha));
    #endif

    #ifndef RELEASE
        if(closeGaloisFieldExceptElements(&((*p)->beta)))
        {
            warningMes;
            printf("in closeBmAlgorithmComponent, closeGaloisFieldExceptElements(&((*p)->beta)) is fail.\n");
        }
    #else
        closeGaloisFieldExceptElements(&((*p)->beta));
    #endif

    #ifndef RELEASE
        if(closeGaloisFieldExceptElements(&((*p)->beforeAlpha)))
        {
            warningMes;
            printf("in closeBmAlgorithmComponent, closeGaloisFieldExceptElements(&((*p)->beforeAlpha)) is fail.\n");
        }
    #else
        closeGaloisFieldExceptElements(&((*p)->beforeAlpha));
    #endif

    #ifndef RELEASE
        if(closeGaloisFieldExceptElements(&((*p)->beforeBeta)))
        {
            warningMes;
            printf("in closeBmAlgorithmComponent, closeGaloisFieldExceptElements(&((*p)->beforeBeta)) is fail.\n");
        }
    #else
        closeGaloisFieldExceptElements(&((*p)->beforeBeta));
    #endif

    free(*p);
    *p=NULL;
            #ifndef RELEASE
                if(global_flag_cmdOption&FLAG_MASK_PRINTF_MEMORY_CHECK_PROCESS)
                {
                    memoryMes;
                    printf("free(*p), p = \'0x%lx\', *p = \'0x%lx\'\n", (unsigned long)p, (unsigned long)(*p));
                }
            #endif

            #ifndef RELEASE
                if((global_flag_cmdOption&FLAG_MASK_PRINTF_LOG))
                {
                    infoMes;
                    printf("in closeBmAlgorithmComponent, closeBmAlgorithmComponent is done.\n");
                }
            #endif
    return 0;
}

struct_HD_BM_algorithmComponent *recreateBmAlgorithmComponent(struct_HD_BM_algorithmComponent **p, struct_galoisFieldElements *galoisFields, unsigned int t, char *string, unsigned int stringLength)
{
    #ifndef RELEASE
    if(!p)
    {
        errorMes;
        printf("in recreateBmAlgorithmComponent, struct_HD_BM_algorithmComponent **p is NULL.\n");
        return NULL;
    }
    #endif

    if(*p)
    {
            #ifndef RELEASE
            if((global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)==FLAG_MASK_PRINTF_LOG)
            {
                infoMes;
                printf("in recreateBmAlgorithmComponent, closeBmAlgorithmComponent(p); will be excute.\n");
            }
            if(!(global_flag_cmdSystemUnhold&FLAG_MASK_SYSTEM_UNHOLD_FORCED))
            {
                warningMes; printf("in recreateBmAlgorithmComponent, struct_HD_BM_algorithmComponent *p have data already.\n");
                warningMesShort; printf("it will be closed.\n");
            }
            #endif

        closeBmAlgorithmComponent(p);
    }

            #ifndef RELEASE
            if((global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)==FLAG_MASK_PRINTF_LOG)
            {
                infoMes;
                printf("in recreateBmAlgorithmComponent, *p=createBmAlgorithmComponent(galoisFields,t, string); will be excute.\n");
                printf("it will be closed.\n");
            }
            #endif

    if(!stringLength) stringLength=strlen(string);

    *p=createBmAlgorithmComponent(galoisFields,t, string, stringLength);
    return *p;
}

/* Operation */
unsigned char calculateCalculationSelector(struct_galoisFieldPolyForm *delta, unsigned int value_r, unsigned int beforeLengthOfStage)
{
    unsigned int i;

    #ifndef RELEASE
    if(!delta)
    {
        errorMes;
        printf("in calculateCalculationSelector, struct_galoisFieldElements *delta is NULL.\n");
        return -1;
    }
    #endif

    if(value_r<(2*beforeLengthOfStage))
    {
        return 0;
    }

    for(i=0; i<(delta->length); i++)
    {
        if(*(delta->value+i)!='0')
        {
            break;
        }
    }
    if(i==(delta->length))//delta is all zero.
    {
        return 0;
    }

    return 1;
}

struct_galoisFieldPolyForm *calculcateDeltaInBmAlgorithm
(
    struct_galoisFieldElements *galoisFields, struct_galoisFieldElements *syndrome,
    struct_galoisFieldElements *beforeStageAlpha, unsigned int lengthOfStage,
    unsigned int value_r
)
{
    unsigned int i;
    struct_galoisFieldPolyForm *buffer=*(galoisFields->element+0);
    struct_galoisFieldPolyForm *buffer2;


    unsigned int debugingTemp;
    struct_galoisFieldPolyForm *debuggingTemp2;

    #ifndef RELEASE
    if(!galoisFields)
    {
        errorMes;
        printf("in struct_galoisFieldPolyForm *calculcateDeltaInBmAlgorithm, struct_galoisFieldElements *galoisFields is NULL.\n");
        return NULL;
    }
    if(!syndrome)
    {
        errorMes;
        printf("in struct_galoisFieldPolyForm *calculcateDeltaInBmAlgorithm, struct_galoisFieldElements *syndrome is NULL.\n");
        return NULL;
    }
    if(!beforeStageAlpha)
    {
        errorMes;
        printf("in struct_galoisFieldPolyForm *calculcateDeltaInBmAlgorithm, struct_galoisFieldElements *beforeStageAlpha is NULL.\n");
        return NULL;
    }
    #endif

            #ifndef RELEASE
            if((global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)==FLAG_MASK_PRINTF_LOG)
            {
                logMes;    printf("in calculcateDeltaInBmAlgorithm, [Delta calculation is start]\n");

                printf("(confirm all zero)clear buffer value of summation of delta is... ");
                printPolyFormWithTapAndEnter(buffer);

                printf("start from : %d, end to : %d\n", 0, lengthOfStage);

                printf("delta(%d) = ", value_r);
            }
            #endif

            #ifndef RELEASE
            if(global_flag_debug_bmSim&FLAG_MASK_DEBUG_BCH_BM_CAL_DELTA)
            {
                debugBmAlgorithmCalDeltaMes;
                printf("for(i=0; i<=lengthOfStage; i++) start, lengthOfStage='%d'\n", lengthOfStage);
            }
            #endif


            #ifndef RELEASE
            if(beforeStageAlpha->length-1 < lengthOfStage)
            {
                if(!(global_flag_cmdSystemUnhold&FLAG_MASK_SYSTEM_UNHOLD_FORCED))
                {
                    warningMes; printf("Case, lengthOfStage is '%d' equal or larger than length of beforeStageAlpha is '%d', and value_r is '%d', MAX(value_r-i) is '%d'\r\n", lengthOfStage, beforeStageAlpha->length, value_r, value_r-lengthOfStage);
                    //warningMesShort; printf("occur. press input any string.\n", lengthOfStage);
                    //systemHoldMes; printf("you can ignore this exception, adding command option '-f' or \"--force\"\r\n");

                    //systemHoldMes; printf("insert any char : "); scanf("%s", local_buf_KeyBoardInput);
                }
            }
            #endif

    for(i=0; i<=lengthOfStage; i++)
    {
                #ifndef RELEASE
                if((global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)==FLAG_MASK_PRINTF_LOG)
                {
                    if(debugingTemp)
                    {
                        printf("(A)a^%d * ",debugingTemp-1);
                    }
                    else
                    {
                        printf("(A)a^- * ");
                    }
                    debugingTemp=abstractIndexOfPowerFormInElementsOfGaloisFieldUsingIndex(galoisFields, *(syndrome->element+(value_r-i)));
                    if(debugingTemp)
                    {
                        printf("(S)a^%d(=",debugingTemp-1);
                    }
                    else
                    {
                        printf("(S)a^-(=");
                    }

                    debuggingTemp2=multiElementsInGF_returnAddr(galoisFields, *(beforeStageAlpha->element+(i)), *(syndrome->element+(value_r-i)));
                    debugingTemp=abstractIndexOfPowerFormInElementsOfGaloisFieldUsingIndex(galoisFields, debuggingTemp2);
                    if(debugingTemp)
                    {
                        printf("a^%d)",debugingTemp-1);
                    }
                    else
                    {
                        printf("a^-)");
                    }
                    if(i!=lengthOfStage)
                    {
                        printf(" +\t");
                    }
                }
                #endif

                #ifndef RELEASE
                if(global_flag_debug_bmSim&FLAG_MASK_DEBUG_BCH_BM_CAL_DELTA)
                {
                    debugBmAlgorithmCalDeltaMes;
                    printf("buffer=sumElementInGF_usingSumMatrixReturnAddr(galoisFields, buffer, multiElementsInGF_returnAddr(galoisFields, *(beforeStageAlpha->element+(i)), *(syndrome->element+(value_r-i))));\n");
                }
                #endif
                #ifndef RELEASE
                if(global_flag_debug_bmSim&FLAG_MASK_DEBUG_BCH_BM_CAL_DELTA)
                {
                    debugBmAlgorithmCalDeltaMes;
                    printf("i = %d, value_r = %d, value_r - i = %d\r\n", i, value_r, value_r-i);
                }
                #endif
        /*
         *  note about " if(beforeStageAlpha->length-1 < i). "
         *  If lengthOfStage is larger or equal than length of beforeStageAlpha,
         *  there is no process elements, so calculate 0
         */
        if(beforeStageAlpha->length-1 < i)
        {
            buffer2=multiElementsInGF_returnAddr(galoisFields, *(galoisFields->element+(0)), *(syndrome->element+(value_r-i)));
        }
        else
        {
            buffer2=multiElementsInGF_returnAddr(galoisFields, *(beforeStageAlpha->element+(i)), *(syndrome->element+(value_r-i)));
        }
                #ifndef RELEASE
                if(global_flag_debug_bmSim&FLAG_MASK_DEBUG_BCH_BM_CAL_DELTA)
                {
                    if(beforeStageAlpha->length<=i)
                    {
                        debugingTemp=abstractIndexOfPowerFormInElementsOfGaloisFieldUsingIndex(galoisFields, *(galoisFields->element+(0)));
                    }
                    else
                    {
                        debugingTemp=abstractIndexOfPowerFormInElementsOfGaloisFieldUsingIndex(galoisFields, *(beforeStageAlpha->element+(i)));
                    }

                    debugBmAlgorithmCalDeltaMes;
                    debugingTemp=abstractIndexOfPowerFormInElementsOfGaloisFieldUsingIndex(galoisFields, buffer);
                    if(debugingTemp)
                    {
                        printf("\t(buffer = )a^%d + ",debugingTemp-1);
                    }
                    else
                    {
                        printf("\t(buffer = )a^- + ");
                    }


                    debugingTemp=abstractIndexOfPowerFormInElementsOfGaloisFieldUsingIndex(galoisFields, buffer2);
                    if(debugingTemp)
                    {
                        printf("\t(buffer2 = )a^%d = ",debugingTemp-1);
                    }
                    else
                    {
                        printf("\t(buffer2 = )a^- = ");
                    }
                }
                #endif

        buffer=sumElementInGF_usingSumMatrixReturnAddr(galoisFields, buffer, buffer2);
                                            if(global_flag_debug_bmSim&FLAG_MASK_DEBUG_BCH_BM_CAL_DELTA)
                                            {
                                                debugingTemp=abstractIndexOfPowerFormInElementsOfGaloisFieldUsingIndex(galoisFields, buffer);
                                                if(debugingTemp)
                                                {
                                                    printf("\t(buffer = )a^%d\n",debugingTemp-1);
                                                }
                                                else
                                                {
                                                    printf("\t(buffer = )a^-\n");
                                                }
                                            }
    }
            #ifndef RELEASE
            if(global_flag_debug_bmSim&FLAG_MASK_DEBUG_BCH_BM_CAL_DELTA)
            {
                debugBmAlgorithmCalDeltaMes;
                printf("for(i=0; i<=lengthOfStage; i++) end\n");
            }
            #endif

            #ifndef RELEASE
            if((global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)==FLAG_MASK_PRINTF_LOG)
            {
                    debugingTemp=abstractIndexOfPowerFormInElementsOfGaloisFieldUsingIndex(galoisFields, buffer);
                    if(debugingTemp)
                    {
                        printf(" = a^%d",debugingTemp-1);
                    }
                    else
                    {
                        printf(" = a^-");
                    }
                printf("\n");
                logMes;    printf("in calculcateDeltaInBmAlgorithm, [Delta calculation is done]\n");
            }
            #endif
    return buffer;
}

char calculateAlphaInBmAlgorithm
(
    struct_galoisFieldElements *beforeAlpha, struct_galoisFieldElements *beforeBeta,
    struct_galoisFieldPolyForm *delta, struct_galoisFieldElements *galoisFields,
    struct_galoisFieldElements *alpha
)
{
    unsigned int i;

    #ifndef RELEASE
    if(!beforeAlpha)
    {
        errorMes;
        printf("in calculateAlphaInBmAlgorithm, struct_galoisFieldElements *beforeAlpha is NULL\n");
        return -1;
    }
    if(!beforeBeta)
    {
        errorMes;
        printf("in calculateAlphaInBmAlgorithm, struct_galoisFieldElements *beforeBeta is NULL\n");
        return -1;
    }
    if(!delta)
    {
        errorMes;
        printf("in calculateAlphaInBmAlgorithm, struct_galoisFieldPolyForm *delta is NULL\n");
        return -1;
    }
    if(!galoisFields)
    {
        errorMes;
        printf("in calculateAlphaInBmAlgorithm, struct_galoisFieldElements *galoisFields is NULL\n");
        return -1;
    }
    if(!alpha)
    {
        errorMes;
        printf("in calculateAlphaInBmAlgorithm, struct_galoisFieldElements *alpha is NULL\n");
        return -1;
    }
    #endif

    for(i=1; i<alpha->length; i++)
    {
        *(alpha->element+i)=sumElementInGF_usingSumMatrixReturnAddr(galoisFields, *(beforeAlpha->element+i), multiElementsInGF_returnAddr(galoisFields, delta, *(beforeBeta->element+i-1)));
    }
    return 0;
}

char calculateBetaInBmAlgorithm
(
    struct_galoisFieldElements *beforeAlpha, struct_galoisFieldElements *beforeBeta,
    struct_galoisFieldPolyForm *delta, struct_galoisFieldElements *galoisFields,
    unsigned char calculationSelector, struct_galoisFieldElements *beta
)
{
    unsigned int i;

    #ifndef RELEASE
    if(!beforeAlpha)
    {
        errorMes;
        printf("in calculateBetaInBmAlgorithm, struct_galoisFieldElements *beforeAlpha is NULL\n");
        return -1;
    }
    if(!beforeBeta)
    {
        errorMes;
        printf("in calculateBetaInBmAlgorithm, struct_galoisFieldElements *beforeBeta is NULL\n");
        return -1;
    }
    if(!delta)
    {
        errorMes;
        printf("in calculateBetaInBmAlgorithm, struct_galoisFieldPolyForm *delta is NULL\n");
        return -1;
    }
    if(!galoisFields)
    {
        errorMes;
        printf("in calculateBetaInBmAlgorithm, struct_galoisFieldElements *galoisFields is NULL\n");
        return -1;
    }
    if(!beta)
    {
        errorMes;
        printf("in calculateBetaInBmAlgorithm, struct_galoisFieldElements *beta is NULL\n");
        return -1;
    }
    #endif

    /* calculationSelect is '1'.i */
    if(calculationSelector)
    {
        for(i=0; i<beta->length; i++)
        {
            *(beta->element+i)=multiElementsInGF_returnAddr(galoisFields, *(beforeAlpha->element+i), invertExponentialElementInGaloisField(galoisFields, delta));
        }
    }
    /* calculationSelect is '0'. Formula beta is shifted. */
    else
    {
        for(i=1; i<beta->length; i++)
        {
            *(beta->element+i)=*(beforeBeta->element+i-1);

            *(beta->element+0)=*(galoisFields->element+0);
        }
    }

    return 0;
}

unsigned int calculateBmAlgorithmLengthOfStage(unsigned int value_r, unsigned int lengthOfStage, unsigned char calculationSelector)
{
    /* value_r is even number. */
    if(calculationSelector)
    {
        return (value_r+1-lengthOfStage);
    }
    /* value_r is odd number. */
    else
    {
        return lengthOfStage;
    }
}

char calculateBmAlgorithm(struct_galoisFieldElements *galoisFields, struct_HD_BM_algorithmComponent *p)
{
    unsigned int bebuggingTemp;

            #ifndef RELEASE
            if(global_flag_debug_bmSim&FLAG_MASK_DEBUG_BCH_BM_ALGORITHM)
            {
                debugBmAlgorithmMes;
                printf("calculateBmAlgorithm\n");
            }
            #endif

    #ifndef RELEASE
    if(!galoisFields)
    {
        errorMes;
        printf("in calculateBmAlgorithm, struct_galoisFieldElements *galoisFields is NULL.\n");
        return -1;
    }
    if(!p)
    {
        errorMes;
        printf("in calculateBmAlgorithm, struct_HD_BM_algorithmComponent *p is NULL.\n");
        return -1;
    }
    #endif

            #ifndef RELEASE
            if(((global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)==FLAG_MASK_PRINTF_LOG)||(global_flag_debug_bmSim&FLAG_MASK_DEBUG_BCH_BM_CAL_DELTA))
            {
                logMes;
                printf("Error correction formula calculation is started.\n");
                printf("initial condition is...\n");
                printf(">> lengthOfStage is %d\n", p->lengthOfStage);
                printf(">> alpha");
                printGaloisField2(galoisFields, p->alpha, PRINT_FIELD_EQUATION_TITLE, PRINT_FIELD_PRINT_OPTION_NUMBERING);
                printf(">> beta");
                printGaloisField2(galoisFields, p->beta, PRINT_FIELD_EQUATION_TITLE, PRINT_FIELD_PRINT_OPTION_NUMBERING);
            }
            #endif

            #ifndef RELEASE
            if(global_flag_debug_bmSim&FLAG_MASK_DEBUG_BCH_BM_ALGORITHM)
            {
                debugBmAlgorithmMes;
                printf("for(p->value_r=0; p->value_r<p->vMAX; p->value_r++)\n");
            }
            #endif
    for(p->value_r=0; p->value_r<p->vMAX; p->value_r++)
    {
                #ifndef RELEASE
                if(((global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)==FLAG_MASK_PRINTF_LOG)||(global_flag_debug_bmSim&FLAG_MASK_DEBUG_BCH_BM_CAL_DELTA))
                {
                    logMes;
                    printf("Error correction formula calculation %d-th stage\n", p->value_r);
                }
                #endif

                    #ifndef RELEASE
                    if(global_flag_debug_bmSim&FLAG_MASK_DEBUG_BCH_BM_ALGORITHM)
                    {
                        debugBmAlgorithmMes;
                        printf("p->delta=calculcateDeltaInBmAlgorithm(galoisFields, p->syndrome, p->beforeAlpha, p->lengthOfStage, p->value_r);\n");
                    }
                    #endif
        p->delta=calculcateDeltaInBmAlgorithm(galoisFields, p->syndrome, p->beforeAlpha, p->lengthOfStage, p->value_r);
                #ifndef RELEASE
                if(((global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)==FLAG_MASK_PRINTF_LOG)||(global_flag_debug_bmSim&FLAG_MASK_DEBUG_BCH_BM_CAL_DELTA))
                {

                    logMes;
                    printf("delta is...\n");
                    bebuggingTemp=abstractIndexOfPowerFormInElementsOfGaloisFieldUsingIndex(galoisFields, p->delta);

                    if(bebuggingTemp)
                    {
                        printf("a^%d\t=\t", bebuggingTemp-1);
                    }
                    else
                    {
                        printf("a^-\t=\t");
                    }
                    printPolyFormWithEnter(p->delta);
                }
                #endif

        /* calculate calculationSelector */
                                    #ifndef RELEASE
                                    if(global_flag_debug_bmSim&FLAG_MASK_DEBUG_BCH_BM_ALGORITHM)
                                    {
                                        debugBmAlgorithmMes;
                                        printf("p->calculationSelector=calculateCalculationSelector(p->delta, p->value_r, p->lengthOfStage);\n");
                                    }
                                    #endif
        p->calculationSelector=calculateCalculationSelector(p->delta, p->value_r, p->lengthOfStage);

                                    #ifndef RELEASE
                                    if(global_flag_debug_bmSim&FLAG_MASK_DEBUG_BCH_BM_ALGORITHM)
                                    {
                                        debugBmAlgorithmMes;
                                        printf("calculateAlphaInBmAlgorithm(p->beforeAlpha, p->beforeBeta, p->delta, galoisFields, p->alpha);\n");
                                    }
                                    #endif
        calculateAlphaInBmAlgorithm(p->beforeAlpha, p->beforeBeta, p->delta, galoisFields, p->alpha);
                                    #ifndef RELEASE
                                    if(global_flag_debug_bmSim&FLAG_MASK_DEBUG_BCH_BM_ALGORITHM)
                                    {
                                        debugBmAlgorithmMes;
                                        printf("calculateBetaInBmAlgorithm(p->beforeAlpha, p->beforeBeta, p->delta, galoisFields, p->calculationSelector, p->beta);\n");
                                    }
                                    #endif
        calculateBetaInBmAlgorithm(p->beforeAlpha, p->beforeBeta, p->delta, galoisFields, p->calculationSelector, p->beta);
                #ifndef RELEASE
                if(((global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)==FLAG_MASK_PRINTF_LOG)||(global_flag_debug_bmSim&FLAG_MASK_DEBUG_BCH_BM_CAL_DELTA))
                {
                    logMes; printf("Small delta(p->calculationSelector) ls '%d'\n", p->calculationSelector);
                    logMes;    printf("check >> alpha");
                    printGaloisField2(galoisFields, p->alpha, PRINT_FIELD_EQUATION_TITLE, PRINT_FIELD_PRINT_OPTION_NUMBERING);

                    logMes;    printf("check >> beforeAlpha");
                    printGaloisField2(galoisFields, p->beforeAlpha, PRINT_FIELD_EQUATION_TITLE, PRINT_FIELD_PRINT_OPTION_NUMBERING);


                    logMes;    printf("check >> beta");
                    printGaloisField2(galoisFields, p->beta, PRINT_FIELD_EQUATION_TITLE, PRINT_FIELD_PRINT_OPTION_NUMBERING);


                    logMes;    printf("check >> beforeBeta");
                    printGaloisField2(galoisFields, p->beforeBeta, PRINT_FIELD_EQUATION_TITLE, PRINT_FIELD_PRINT_OPTION_NUMBERING);
                }
                #endif


                                    #ifndef RELEASE
                                    if(global_flag_debug_bmSim&FLAG_MASK_DEBUG_BCH_BM_ALGORITHM)
                                    {
                                        debugBmAlgorithmMes;
                                        printf("p->lengthOfStage=calculateBmAlgorithmLengthOfStage(p->value_r, p->lengthOfStage, p->calculationSelector);\n");
                                    }
                                    #endif
        p->lengthOfStage=calculateBmAlgorithmLengthOfStage(p->value_r, p->lengthOfStage, p->calculationSelector);

                #ifndef RELEASE
                if(((global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)==FLAG_MASK_PRINTF_LOG)||(global_flag_debug_bmSim&FLAG_MASK_DEBUG_BCH_BM_CAL_DELTA))
                {
                    logMes;
                    printf("L(%d) is %d.\n", p->value_r, p->lengthOfStage);
                }
                #endif

                #ifndef RELEASE
                /* synchronize register and wire, pulse input liked */
                if(((global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)==FLAG_MASK_PRINTF_LOG)||(global_flag_debug_bmSim&FLAG_MASK_DEBUG_BCH_BM_CAL_DELTA))
                {
                    logMes; printf("before alpha, beta are synchronized.\n");
                }
                #endif

                                    #ifndef RELEASE
                                    if(global_flag_debug_bmSim&FLAG_MASK_DEBUG_BCH_BM_ALGORITHM)
                                    {
                                        debugBmAlgorithmMes;
                                        printf("copyListOfGaloisField(p->beforeAlpha->element, p->alpha->element, p->alpha->length);\n");
                                    }
                                    #endif
        copyListOfGaloisField(p->beforeAlpha->element, p->alpha->element, p->alpha->length);

                                    #ifndef RELEASE
                                    if(global_flag_debug_bmSim&FLAG_MASK_DEBUG_BCH_BM_ALGORITHM)
                                    {
                                        debugBmAlgorithmMes;
                                        printf("copyListOfGaloisField(p->beforeBeta->element, p->beta->element, p->alpha->length);\n");
                                    }
                                    #endif
        copyListOfGaloisField(p->beforeBeta->element, p->beta->element, p->alpha->length);
    }

    p->degreeOfErrLocPoly = checkDegreePolynomials(galoisFields, p->alpha);
                                    #ifndef RELEASE
                                    if(global_flag_debug_bmSim&FLAG_MASK_DEBUG_BCH_BM_ALGORITHM)
                                    {
                                        debugBmAlgorithmMes; printf("Degree of Polynomials : %d\r\n",p->degreeOfErrLocPoly);
                                        systemHoldMes;    printf("input any charactor : ");
                                        scanf("%s", local_buf_KeyBoardInput);
                                    }
                                    #endif

    return 0;
}
