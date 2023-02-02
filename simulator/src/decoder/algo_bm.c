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
