/* Constructor and Destructor */
struct_encodingComponentInGF *createEncodingComponent(unsigned int length, unsigned int numberOfCorrectableBit, char_POWER_FORM *generationPolynomial)
{
    struct_encodingComponentInGF *p;

    #ifndef RELEASE
    if(!length)
    {
        errorMes;
        printf("in createEncodingComponent, unsigned int length is zero.\n");
        return NULL;
    }
    if(!numberOfCorrectableBit)
    {
        errorMes;
        printf("in createEncodingComponent, unsigned int numberOfCorrectableBit is zero.\n");
        return NULL;
    }
    if(!generationPolynomial)
    {
        errorMes;
        printf("in createEncodingComponent, char_POWER_FORM *generationPolynomial is NULL.\n");
        return NULL;
    }
    #endif

    p=(struct_encodingComponentInGF*)malloc(sizeof(struct_encodingComponentInGF));
    memset(p, 0, sizeof(struct_encodingComponentInGF));

    p->minimumDistance=2*numberOfCorrectableBit+1;

    p->parityBitsLength=strlen(generationPolynomial)-1;

    p->codeWord=createPowerFormPolynomial(length);

    p->generationPolynomial=generationPolynomial;
    return p;
}

char closeEncodingComponent(struct_encodingComponentInGF **p)
{
    #ifndef RELEASE
    if(!p)
    {
        errorMes;
        printf("in closeEncodingComponent, struct_encodingComponentInGF **p is NULL.\n");
        return -1;
    }
    if(!*p)
    {
        errorMes;
        printf("in closeEncodingComponent, struct_encodingComponentInGF *p is NULL.\n");
        return -1;
    }
    #endif

    closePowerFormPolynomial(&((*p)->codeWord));

    free(*p);
    *p=NULL;
    return 0;
}

struct_encodingComponentInGF *recreateEncodingComponent(struct_encodingComponentInGF **p, unsigned int length, unsigned int numberOfCorrectableBit, char_POWER_FORM *generationPolynomial)
{
    if(*p)
    {
                #ifndef RELEASE
                if(global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)
                {
                    infoMes;
                    printf("in recreateEncodingComponent, struct_encodingComponentInGF *p have data already.\n");
                    printf("it will be deleted.\n");
                }
                #endif
        closeEncodingComponent(p);
    }
    *p=createEncodingComponent(length, numberOfCorrectableBit, generationPolynomial);
    return *p;
}

struct_encodingComponentInGF *createEncodingComponentAndInputValue(unsigned int length, unsigned int numberOfCorrectableBit, char_POWER_FORM *generationPolynomial, char *value)
{
    struct_encodingComponentInGF *p;

    p=createEncodingComponent(length, numberOfCorrectableBit, generationPolynomial);
    #ifndef RELEASE
    if(!p)
    {
        errorMes;
        closeEncodingComponent(&p);
        printf("in createEncodingComponentAndInputValue, (p=createEncodingComponent(length, numberOfCorrectableBit, generationPolynomial)) return NULL.\n");
        return NULL;
    }
    #endif
    if(copyPowerFormPolynomialFromString(p->codeWord, value))
    {
                #ifndef RELEASE
                errorMes; printf("in createEncodingComponentAndInputValue, copyPowerFormPolynomialFromString(p->codeWord, value) return not 0.\n");
                #endif

        closeEncodingComponent(&p);
        return NULL;
    }
    return p;
}

struct_encodingComponentInGF *recreateEncodingComponentAndInputValue(struct_encodingComponentInGF **p, unsigned int length, unsigned int numberOfCorrectableBit, char_POWER_FORM *generationPolynomial, char *value)
{
    if(*p)
    {
                #ifndef RELEASE
                if(global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)
                {
                    infoMes;
                    printf("in recreateEncodingComponentAndInputValue, struct_encodingComponentInGF *p have data already.\n");
                    printf("it will be deleted.\n");
                }
                #endif

        closeEncodingComponent(p);
    }
    *p=createEncodingComponentAndInputValue(length, numberOfCorrectableBit, generationPolynomial, value);
    return *p;
}
