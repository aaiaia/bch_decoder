#include "encode.h"

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

/* Operation */
char calculateParityInGaloisFieldAttachLowSide(struct_powerFormPolynomials *encodedCodeWord, unsigned int numberOfParityBits, char_POWER_FORM *generationPolynomial)
{
    unsigned int i;
    unsigned int j;
    unsigned int genPolyLength=strlen(generationPolynomial);

    char_POWER_FORM *remainder;

            #ifndef RELEASE
            if(global_flag_debug_display&FLAG_MASK_DEBUG_DEBUG)
            {
                infoMes; printf("[start] calculateParityInGaloisFieldAttachLowSide\n");
            }
            #endif

    #ifndef RELEASE
    if(!encodedCodeWord)
    {
        errorMes;
        printf("in calculateParityInGaloisFieldAttachLowSide, struct_powerFormPolynomials *encodedCodeWord is NULL.\n");
        return -1;
    }
    if(!generationPolynomial)
    {
        errorMes;
        printf("in calculateParityInGaloisFieldAttachLowSide, char_POWER_FORM *generationPolynomial is NULL.\n");
        return -1;
    }
    #endif

    //char_POWER_FORM *remainder=(char_POWER_FORM*)malloc(sizeof(char_POWER_FORM)*(encodedCodeWord->length+1));
    //memcpy(remainder, encodedCodeWord->equation, sizeof(char_POWER_FORM)*(encodedCodeWord->length+1));
    remainder=(char_POWER_FORM*)malloc(sizeof(char_POWER_FORM)*(encodedCodeWord->usedLength+1));
    #ifndef USING_OPEN_MP
    memcpy(remainder, encodedCodeWord->equation, sizeof(char_POWER_FORM)*(encodedCodeWord->usedLength));
    #else
    #ifdef USING_OPEN_MP
    #pragma omp parallel for schedule(guided) private(i) shared(encodedCodeWord, remainder)
    #endif
    for(i=0; i<encodedCodeWord->usedLength; i++)
    {
        *(remainder+i)=*(encodedCodeWord->equation+i);
    }
    #endif
    *(remainder+encodedCodeWord->usedLength)=0;


            #ifndef RELEASE
            if(global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)
            {
                logMes; printf("in calculateParityInGaloisFieldAttachLowSide, memcpy result is below...\n\"%s\"\n", remainder);
            }
            #endif


    for(i=0; i<encodedCodeWord->usedLength; i++)
    {
        if((encodedCodeWord->usedLength-genPolyLength)<i)
        {
            break;
        }
        if(*(remainder+encodedCodeWord->usedLength-1-i)=='1')
        {
            #ifdef USING_OPEN_MP
            #pragma omp parallel for schedule(guided) private(j) shared(i, genPolyLength, remainder, encodedCodeWord, generationPolynomial)
            #endif
            for(j=0; j<genPolyLength; j++)
            {
                if(*(remainder+encodedCodeWord->usedLength-1-i-j)!=*(generationPolynomial+genPolyLength-1-j))
                {
                    *(remainder+encodedCodeWord->usedLength-1-i-j)='1';
                }
                else
                {
                    *(remainder+encodedCodeWord->usedLength-1-i-j)='0';
                }
            }
        }
    }

            #ifndef RELEASE
            if((global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)==FLAG_MASK_PRINTF_LOG)
            {
                logMes; printf("encoded(not op done : \n\"%s\"\n", encodedCodeWord->equation);
                logMes; printf("remainder : \n\"%s\"\n", remainder);
            }
            #endif
    #ifndef USING_OPEN_MP
    memcpy(encodedCodeWord->equation, remainder, sizeof(char_POWER_FORM)*numberOfParityBits);
    #else
    #pragma omp parallel for schedule(guided) private(i) shared(numberOfParityBits, encodedCodeWord, remainder)
    for(i=0; i<numberOfParityBits; i++)
    {
        *(encodedCodeWord->equation+i)=*(remainder+i);
    }
    #endif

    free(remainder);

            #ifndef RELEASE
            if(global_flag_debug_display&FLAG_MASK_DEBUG_DEBUG)
            {
                infoMes; printf("[end] calculateParityInGaloisFieldAttachLowSide\n");
            }
            #endif
    return 0;
}

char calculateParityInGaloisFieldAttachHighSide(struct_powerFormPolynomials *encodedCodeWord, unsigned int numberOfParityBits, char_POWER_FORM *generationPolynomial)
{
    unsigned int i;
    unsigned int j;
    unsigned int genPolyLength=strlen(generationPolynomial);

    char_POWER_FORM *remainder;

    #ifndef RELEASE
    if(!encodedCodeWord)
    {
        errorMes;
        printf("in calculateParityInGaloisFieldAttachHighSide, struct_powerFormPolynomials *encodedCodeWord is NULL.\n");
        return -1;
    }
    if(!generationPolynomial)
    {
        errorMes;
        printf("in calculateParityInGaloisFieldAttachHighSide, char_POWER_FORM *generationPolynomial is NULL.\n");
        return -1;
    }
    #endif

    remainder=(char_POWER_FORM*)malloc(sizeof(char_POWER_FORM)*(encodedCodeWord->usedLength+1));
    memcpy(remainder, encodedCodeWord->equation, sizeof(char_POWER_FORM)*(encodedCodeWord->usedLength+1));

            #ifndef RELEASE
            if((global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)==FLAG_MASK_PRINTF_LOG)
            {
                logMes;
                printf("in calculateParityInGaloisFieldAttachHighSide, memcpy result : \"%s\"\n", remainder);
            }
            #endif

    for(i=0; i<encodedCodeWord->usedLength; i++)
    {
        if((encodedCodeWord->usedLength-genPolyLength)<i)
        {
            break;
        }
        if(*(remainder+encodedCodeWord->usedLength-1-i)=='1')
        {
            for(j=0; j<genPolyLength; j++)
            {
                if(*(remainder+encodedCodeWord->usedLength-1-i-j)!=*(generationPolynomial+genPolyLength-1-j))
                {
                    *(remainder+encodedCodeWord->usedLength-1-i-j)='1';
                }
                else
                {
                    *(remainder+encodedCodeWord->usedLength-1-i-j)='0';
                }
            }
        }
    }

    memcpy(encodedCodeWord->equation+encodedCodeWord->usedLength-numberOfParityBits-1, remainder, sizeof(char_POWER_FORM)*numberOfParityBits);

    free(remainder);
    return 0;
}
