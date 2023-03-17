#include <stdlib.h>
#include <string.h>

#include "gf/field_set.h"
#include "gf/syndrome.h"
#include "gf/equation.h"
#include "decoder/hard_decision/algo_mSBS.h"

/* Constructor and Desctructor */
/*
 * ignore t, that is mean correctable bits are already constants.
 * maximum correctables are 3.
 */
struct_HD_mSBS_t3_algorithmComponent *create_mSBS_algorithmComponent(struct_galoisFieldElements *galoisFields, unsigned int t, char *string, unsigned int stringLength)
{
    struct_HD_mSBS_t3_algorithmComponent *p;

        //double galoisFieldElementLengthBuffer;
        //unsigned int galoisFieldElementLength;

    #ifndef RELEASE
        //if(!t)
        //{
        //    errorMes;
        //    printf("correctable number(t) is %d\n", t);
        //}

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
    if(t > 3)
    {
        t = 3;
    }
            #ifndef RELEASE
            if((global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)==FLAG_MASK_PRINTF_LOG)
            {
                logMes; printf("codeWord is received and set a information to correct error.\n");
            }
            #endif
    p = (struct_HD_mSBS_t3_algorithmComponent*)malloc(sizeof(struct_HD_mSBS_t3_algorithmComponent));
    memset(p, 0, sizeof(struct_HD_mSBS_t3_algorithmComponent));


    p->numberOfCorrection = t;
            #ifndef RELEASE
            if((global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)==FLAG_MASK_PRINTF_LOG)
            {
                logMes; printf("number of correctable error is \'%d\', ", p->numberOfCorrection);
            }
            #endif

        p->vMAX = cal_numsOfDegreeOfSynd(t);/* use for, when variable is 6, it is break. */
                //#ifndef RELEASE
                //if((global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)==FLAG_MASK_PRINTF_LOG)
                //{
                //    logMes; printf("vMAX is \'%d\'\n", p->vMAX);
                //}
                //#endif

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
    p->syndrome = createGaloisFieldExceptElements(p->vMAX);
    /* calculate syndrome */
    syndromeArrayCalculatorUsingString(galoisFields, p->syndrome, p->vMAX, string);

        ///* create syndrome of error location */
        //p->errLocSyndrome = createGaloisFieldExceptElements(p->vMAX+global_bch_Soft_OverSyndLen);
        ///* create syndrome about sum syndromes of received and error location vector */
        //p->sumReceiveAndErrLocSyndrome = createGaloisFieldExceptElements(p->vMAX+global_bch_Soft_OverSyndLen);



        /* BM algorithm initial condition setting. */
        //p->value_r = 0;

    /* Initializing degree of Error Location Poly */
    p->degreeOfErrLocPoly=-1;

    /* clear metricCheck */
    p->metricCheck=0;

        /* create alpha and beta formula and set initial condition when value_r is -1. */
        //p->alpha = createGaloisFieldExceptElements(t+1);
        //p->beta = createGaloisFieldExceptElements(t+1);
        //    p->errLocPoly = p->alpha;
    p->errLocPoly = createGaloisFieldExceptElements(t+1);
        //initEquation(galoisFields, p->alpha);
        //initEquation(galoisFields, p->beta);
    initEquation(galoisFields, p->errLocPoly);

            #ifndef RELEASE
            if(global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)
            {
                //infoMes;
                //printf(">> alpha");
                //printGaloisField2(galoisFields, p->alpha, PRINT_FIELD_EQUATION_TITLE, 0);
                //infoMes;
                //printf(">> beta");
                //printGaloisField2(galoisFields, p->beta, PRINT_FIELD_EQUATION_TITLE, 0);
                    infoMes;
                    printf(">> errLocPoly");
                    printGaloisField2(galoisFields, p->errLocPoly, PRINT_FIELD_EQUATION_TITLE, 0);
            }
            #endif

        /* create output of register */
        //p->beforeAlpha = createGaloisFieldExceptElements(t+1);
        //p->beforeBeta = createGaloisFieldExceptElements(t+1);
        /* synchronize register and wire, pulse input liked */
        //copyListOfGaloisField(p->beforeAlpha->element, p->alpha->element, p->alpha->length);
        //copyListOfGaloisField(p->beforeBeta->element, p->beta->element, p->alpha->length);


        /* initial condition of length of Stage is 0. */
        //p->lengthOfStage=0;

    /* indexing kind of bch decoding algorithm */
    p->KIND_OF_BCH_ALGORITHM = KIND_OF_BCH_DECODING_mSBS;

    return p;
}

char close_mSBS_algorithmComponent(struct_HD_mSBS_t3_algorithmComponent **p)
{
    #ifndef RELEASE
        if(!p)
        {
            if(!(global_flag_cmdSystemUnhold&FLAG_MASK_SYSTEM_UNHOLD_FORCED))
            {
                errorMes;
                printf("in close_mSBS_algorithmComponent, close_mSBS_algorithmComponent &p = 0x%lx\n", (unsigned long)p);
            }
            return -1;
        }
        if(!*p)
        {
            if(!(global_flag_cmdSystemUnhold&FLAG_MASK_SYSTEM_UNHOLD_FORCED))
            {
                warningMes;
                printf("in close_mSBS_algorithmComponent, close_mSBS_algorithmComponent *p = 0x%lx\n", (unsigned long)(*p));
            }
            return -1;
    }
    #endif

    #ifndef RELEASE
        if(closePowerFormPolynomial(&((*p)->codeWord)))
        {
            warningMes; printf("in close_mSBS_algorithmComponent, closePowerFormPolynomial(&((*p)->codeWord)) is fail.\n");
        }
    #else
        closePowerFormPolynomial(&((*p)->codeWord));
    #endif

    #ifndef RELEASE
        if(closePowerFormPolynomial(&((*p)->errorLocationVector)))
        {
            warningMes; printf("in close_mSBS_algorithmComponent, closePowerFormPolynomial(&((*p)->codeWord)) is fail.\n");
        }
    #else
        closePowerFormPolynomial(&((*p)->errorLocationVector));
    #endif

    #ifndef RELEASE
        if(closeGaloisFieldExceptElements(&((*p)->syndrome)))
        {
            warningMes;
            printf("in close_mSBS_algorithmComponent, closeGaloisFieldExceptElements(&((*p)->syndrome)) is fail.\n");
        }
    #else
        closeGaloisFieldExceptElements(&((*p)->syndrome));
    #endif


    #ifndef RELEASE
    if(closeGaloisFieldExceptElements(&((*p)->errLocPoly)))
    {
        warningMes;
        printf("in close_mSBS_algorithmComponent, closeGaloisFieldExceptElements(&((*p)->errLocPoly)) is fail.\n");
    }
    #else
        closeGaloisFieldExceptElements(&((*p)->errLocPoly));
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
                    printf("in close_mSBS_algorithmComponent, close_mSBS_algorithmComponent is done.\n");
                }
            #endif
    return 0;
}

struct_HD_mSBS_t3_algorithmComponent *recreate_mSBS_algorithmComponent(struct_HD_mSBS_t3_algorithmComponent **p, struct_galoisFieldElements *galoisFields, unsigned int t, char *string, unsigned int stringLength)
{
    #ifndef RELEASE
    if(!p)
    {
        errorMes;
        printf("in recreate_mSBS_algorithmComponent, struct_HD_mSBS_t3_algorithmComponent **p is NULL.\n");
        return NULL;
    }
    #endif

    if(*p)
    {
            #ifndef RELEASE
            if((global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)==FLAG_MASK_PRINTF_LOG)
            {
                infoMes;
                printf("in recreate_mSBS_algorithmComponent, closeBmAlgorithmComponent(p); will be excute.\n");
            }
            if(!(global_flag_cmdSystemUnhold&FLAG_MASK_SYSTEM_UNHOLD_FORCED))
            {
                warningMes; printf("in recreate_mSBS_algorithmComponent, struct_HD_mSBS_t3_algorithmComponent *p have data already.\n");
                warningMesShort; printf("it will be closed.\n");
            }
            #endif

        close_mSBS_algorithmComponent(p);
    }

            #ifndef RELEASE
            if((global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)==FLAG_MASK_PRINTF_LOG)
            {
                infoMes;
                printf("in recreate_mSBS_algorithmComponent, *p=create_mSBS_algorithmComponent(galoisFields,t, string); will be excute.\n");
                printf("it will be closed.\n");
            }
            #endif

    if(!stringLength) stringLength=strlen(string);

    return create_mSBS_algorithmComponent(galoisFields,t, string, stringLength);;
}

/* Operation */
char calculate_mSBS_algorithm(struct_galoisFieldElements *galoisFields, struct_HD_mSBS_t3_algorithmComponent *p)
{
    struct_galoisFieldPolyForm *tmp_S_1_pow_2;
    struct_galoisFieldPolyForm *tmp_S_1_pow_3;
    struct_galoisFieldPolyForm *tmp_S_1_pow_4;
    struct_galoisFieldPolyForm *tmp_S_1_pow_6;

    struct_galoisFieldPolyForm *tmp_S_3_pow_2;

    struct_galoisFieldPolyForm *tmp_ref_coefficient;//it is same with S_1^3 + S_3 is tmp_C_t3.

    struct_galoisFieldPolyForm *tmp_C_t3;
    struct_galoisFieldPolyForm *tmp_B_t3;
    struct_galoisFieldPolyForm *tmp_A_t3;
    struct_galoisFieldPolyForm *tmp_R_t3;


            #ifndef RELEASE
            if(global_flag_debug_bmSim&FLAG_MASK_DEBUG_BCH_BM_ALGORITHM)
            {
                debugBmAlgorithmMes;
                printf("calculate_mSBS_algorithm\n");
            }
            #endif

    #ifndef RELEASE
    if(!galoisFields)
    {
        errorMes;
        printf("in calculate_mSBS_algorithm, struct_galoisFieldElements *galoisFields is NULL.\n");
        return -1;
    }
    if(!p)
    {
        errorMes;
        printf("in calculate_mSBS_algorithm, struct_HD_BM_algorithmComponent *p is NULL.\n");
        return -1;
    }
    #endif

    /*
     * if correctables is set up to larger than 3, it is no make any sense.
     * And then, to block changing any received, So make error location polynomial 0 = 1 + 0 * a^1 + 0 * a^2 + 0 * a^3 ...
     * So the err loc poly can not change any bits.
     */
    //if(p->numberOfCorrection == 3)
    //{
    //    #ifndef RELEASE
    //    warningMesShort; printf("in calculate_mSBS_algorithm, if(!(p->numberOfCorrection < p->syndrome->length)).\n");
    //    #endif
    //
    //    *(p->errLocPoly->element+3) = (*(galoisFields->element+0));
    //    *(p->errLocPoly->element+2) = (*(galoisFields->element+0));
    //    *(p->errLocPoly->element+1) = (*(galoisFields->element+0));
    //    *(p->errLocPoly->element+0) = (*(galoisFields->element+1));
    //
    //    return -2;
    //}

            #ifndef RELEASE
            if(((global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)==FLAG_MASK_PRINTF_LOG)||(global_flag_debug_bmSim&FLAG_MASK_DEBUG_BCH_BM_CAL_DELTA))
            {
                logMes;
                printf("Error correction formula(mSBS t=3) calculation is started.\n");
                printf("initial condition is...\n");
                printf(">> errLocPoly");
                printGaloisField2(galoisFields, p->errLocPoly, PRINT_FIELD_EQUATION_TITLE, PRINT_FIELD_PRINT_OPTION_NUMBERING);
            }
            #endif

    //struct_galoisFieldPolyForm *sumElementInGF_usingSumMatrixReturnAddr(struct_galoisFieldElements *field, struct_galoisFieldPolyForm *operandA, struct_galoisFieldPolyForm *operandB)
    //struct_galoisFieldPolyForm *multiElementsInGF_returnAddr(struct_galoisFieldElements *field, struct_galoisFieldPolyForm *operandA, struct_galoisFieldPolyForm *operandB)
    /*
     * case t >= 3
     * C_t3 = S_1^3 + S_3
     * B_t3 = S_1^4 + ( S_1 * S_3 )
     * A_t3 = S_5 + ( S_1^2 * S_3 )
     * R_t3 = S_1^6 + S_3^2 + ( S_1^3 * S_3 ) + ( S_1 * S_5 )
     *
     * case t <= 2
     * C_t2 = 0
     * B_t2 = S_1
     * A_t2 = S_1^2
     * R_t2 = S_1^3 + S_3
     *
     * S_1 is *(syndrome->element+0)
     * S_3 is *(syndrome->element+2)
     * S_5 is *(syndrome->element+4)
     */
            #ifndef RELEASE
            if(((global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)==FLAG_MASK_PRINTF_LOG)||(global_flag_debug_bmSim&FLAG_MASK_DEBUG_BCH_BM_CAL_DELTA))
            {
                logMes;    printf("[calculate syndrome factors start]\r\n");
            }
            #endif
    tmp_S_1_pow_2 = multiElementsInGF_returnAddr(galoisFields, *(p->syndrome->element+0), *(p->syndrome->element+0));
            #ifndef RELEASE
            if(((global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)==FLAG_MASK_PRINTF_LOG)||(global_flag_debug_bmSim&FLAG_MASK_DEBUG_BCH_BM_CAL_DELTA))
            {
                logMes;    printf("[tmp_S_1_pow_2 is done]\r\n");
            }
            #endif

    tmp_S_1_pow_3 = multiElementsInGF_returnAddr(galoisFields, tmp_S_1_pow_2, *(p->syndrome->element+0));
            #ifndef RELEASE
            if(((global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)==FLAG_MASK_PRINTF_LOG)||(global_flag_debug_bmSim&FLAG_MASK_DEBUG_BCH_BM_CAL_DELTA))
            {
                logMes;    printf("[tmp_S_1_pow_3 is done]\r\n");
            }
            #endif

    tmp_S_1_pow_4 = multiElementsInGF_returnAddr(galoisFields, tmp_S_1_pow_2, tmp_S_1_pow_2);
            #ifndef RELEASE
            if(((global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)==FLAG_MASK_PRINTF_LOG)||(global_flag_debug_bmSim&FLAG_MASK_DEBUG_BCH_BM_CAL_DELTA))
            {
                logMes;    printf("[tmp_S_1_pow_4 is done]\r\n");
            }
            #endif

    tmp_S_1_pow_6 = multiElementsInGF_returnAddr(galoisFields, tmp_S_1_pow_3, tmp_S_1_pow_3);
            #ifndef RELEASE
            if(((global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)==FLAG_MASK_PRINTF_LOG)||(global_flag_debug_bmSim&FLAG_MASK_DEBUG_BCH_BM_CAL_DELTA))
            {
                logMes;    printf("[tmp_S_1_pow_6 is done]\r\n");
            }
            #endif

    if(p->numberOfCorrection > 1)
    {
        tmp_S_3_pow_2 = multiElementsInGF_returnAddr(galoisFields, *(p->syndrome->element+2), *(p->syndrome->element+2));
                #ifndef RELEASE
                if(((global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)==FLAG_MASK_PRINTF_LOG)||(global_flag_debug_bmSim&FLAG_MASK_DEBUG_BCH_BM_CAL_DELTA))
                {
                    logMes;    printf("[tmp_S_3_pow_2 is done]\r\n");
                }
                #endif
    }


    if(p->numberOfCorrection > 1)
    {
        tmp_C_t3 = sumElementInGF_usingSumMatrixReturnAddr(galoisFields, tmp_S_1_pow_3, *(p->syndrome->element+2));
                #ifndef RELEASE
                if(((global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)==FLAG_MASK_PRINTF_LOG)||(global_flag_debug_bmSim&FLAG_MASK_DEBUG_BCH_BM_CAL_DELTA))
                {
                    logMes;    printf("[tmp_C_t3 is done]\r\n");
                }
                #endif

        tmp_B_t3 = sumElementInGF_usingSumMatrixReturnAddr(galoisFields, tmp_S_1_pow_4, multiElementsInGF_returnAddr(galoisFields, *(p->syndrome->element+0), *(p->syndrome->element+2)));
                #ifndef RELEASE
                if(((global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)==FLAG_MASK_PRINTF_LOG)||(global_flag_debug_bmSim&FLAG_MASK_DEBUG_BCH_BM_CAL_DELTA))
                {
                    logMes;    printf("[tmp_B_t3 is done]\r\n");
                }
                #endif
    }


    if(p->numberOfCorrection > 2)
    {
        tmp_A_t3 = sumElementInGF_usingSumMatrixReturnAddr(galoisFields, *(p->syndrome->element+4), multiElementsInGF_returnAddr(galoisFields, tmp_S_1_pow_2, *(p->syndrome->element+2)));
                #ifndef RELEASE
                if(((global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)==FLAG_MASK_PRINTF_LOG)||(global_flag_debug_bmSim&FLAG_MASK_DEBUG_BCH_BM_CAL_DELTA))
                {
                    logMes;    printf("[tmp_A_t3 is done]\r\n");
                }
                #endif
        /*
        //tmp_R_t3 = sumElementInGF_usingSumMatrixReturnAddr(galoisFields,
        //            sumElementInGF_usingSumMatrixReturnAddr(galoisFields, tmp_S_1_pow_6, multiElementsInGF_returnAddr(galoisFields, tmp_S_1_pow_3, *(p->syndrome->element+2))),
        //            sumElementInGF_usingSumMatrixReturnAddr(galoisFields, tmp_S_3_pow_2, multiElementsInGF_returnAddr(galoisFields, *(p->syndrome->element+0), *(p->syndrome->element+4)))
        //            );
        */
        tmp_R_t3 = sumElementInGF_usingSumMatrixReturnAddr(galoisFields,
                    multiElementsInGF_returnAddr(galoisFields, tmp_C_t3, tmp_C_t3),
                    multiElementsInGF_returnAddr(galoisFields, *(p->syndrome->element+0), tmp_A_t3)
                    );

                #ifndef RELEASE
                if(((global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)==FLAG_MASK_PRINTF_LOG)||(global_flag_debug_bmSim&FLAG_MASK_DEBUG_BCH_BM_CAL_DELTA))
                {
                    logMes;    printf("[tmp_R_t3 is done]\r\n");
                }
                #endif
    }






    if(p->numberOfCorrection > 2)
    {
        tmp_ref_coefficient = tmp_C_t3;
                #ifndef RELEASE
                if(((global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)==FLAG_MASK_PRINTF_LOG)||(global_flag_debug_bmSim&FLAG_MASK_DEBUG_BCH_BM_CAL_DELTA))
                {
                    logMes;    printf("[calculate syndrome factors end]\r\n");
                }
                #endif

                #ifndef RELEASE
                if(((global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)==FLAG_MASK_PRINTF_LOG)||(global_flag_debug_bmSim&FLAG_MASK_DEBUG_BCH_BM_CAL_DELTA))
                {
                    logMes;    printf("[saving errLocPoly start]\r\n");
                }
                #endif
        if( tmp_ref_coefficient != (*(galoisFields->element+0)) )//check refference coefficient is zero?
        //NO -> degree of errLoc poly is 3.
        {
                    #ifndef RELEASE
                    if(((global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)==FLAG_MASK_PRINTF_LOG)||(global_flag_debug_bmSim&FLAG_MASK_DEBUG_BCH_BM_CAL_DELTA))
                    {
                        logMes;    printf("degree of errLoc poly is 3\r\n");
                    }
                    #endif
            *(p->errLocPoly->element+3) = tmp_C_t3;
            *(p->errLocPoly->element+2) = tmp_B_t3;
            *(p->errLocPoly->element+1) = tmp_A_t3;
            *(p->errLocPoly->element+0) = tmp_R_t3;

            //p->degreeOfErrLocPoly=3;

        }
        else
        //YES -> degree of errLoc poly is 2.
        {
                    #ifndef RELEASE
                    if(((global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)==FLAG_MASK_PRINTF_LOG)||(global_flag_debug_bmSim&FLAG_MASK_DEBUG_BCH_BM_CAL_DELTA))
                    {
                        logMes;    printf("degree of errLoc poly is 2\r\n");
                    }
                    #endif
            *(p->errLocPoly->element+3) = (*(galoisFields->element+0));
            *(p->errLocPoly->element+2) = *(p->syndrome->element+0);
            *(p->errLocPoly->element+1) = tmp_S_1_pow_2;
            *(p->errLocPoly->element+0) = tmp_C_t3;

            // p->degreeOfErrLocPoly=2;

        }

                #ifndef RELEASE
                if(((global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)==FLAG_MASK_PRINTF_LOG)||(global_flag_debug_bmSim&FLAG_MASK_DEBUG_BCH_BM_CAL_DELTA))
                {
                    logMes;    printf("[saving errLocPoly end]\r\n");
                }
                #endif
        p->coefficient_of_alpha_three = *(p->errLocPoly->element+3);
        p->coefficient_of_alpha_two = *(p->errLocPoly->element+2);
        p->coefficient_of_alpha_one = *(p->errLocPoly->element+1);
        p->coefficient_of_alpha_zero = *(p->errLocPoly->element+0);

    }
    else if(p->numberOfCorrection > 1)
    {
        {
                    #ifndef RELEASE
                    if(((global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)==FLAG_MASK_PRINTF_LOG)||(global_flag_debug_bmSim&FLAG_MASK_DEBUG_BCH_BM_CAL_DELTA))
                    {
                        logMes;    printf("degree of errLoc poly is 3\r\n");
                    }
                    #endif
            *(p->errLocPoly->element+2) = *(p->syndrome->element+0);
            *(p->errLocPoly->element+1) = tmp_S_1_pow_2;
            *(p->errLocPoly->element+0) = tmp_C_t3;

            //if( tmp_C_t3 != (*(galoisFields->element+0)) )
            //{
            //    p->degreeOfErrLocPoly=2;
            //}
            //else
            //{
            //    p->degreeOfErrLocPoly=1;
            //}

        }
    }


    p->degreeOfErrLocPoly = checkDegreePolynomials(galoisFields, p->errLocPoly);

    return 0;
}
