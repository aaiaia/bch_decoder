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
 * Tree, can use, is define that is in enum treeTypes.
 * If you needs this list, find a enum treeTypes.
 */
char initializingVariablesAssociateWithTree(struct_logLikeHoodRatio *p, enum treeTypes treeType)
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

/* For displying(debugging) */
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

/* File IO */
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
