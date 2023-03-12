#include "element.h"

struct_galoisFieldPolyForm *createPolyForm(unsigned int length)
{
    struct_galoisFieldPolyForm *p;

    #ifndef RELEASE
    if(!length)
    {
        errorMes;
        printf("arg[0], poly form length is %d\n", length);
        return NULL;
    }
    #endif

    p = (struct_galoisFieldPolyForm*)malloc(sizeof(struct_galoisFieldPolyForm));
    memset(p, 0, sizeof(struct_galoisFieldPolyForm));

    //p->bits=&(p->length);

    p->length=length;

    p->value = (char_GALOIS_FIELD_VALUE*)malloc((sizeof(char_GALOIS_FIELD_VALUE)*length)+1);//+1 is to null.
    memset(p->value, '0', sizeof(char_GALOIS_FIELD_VALUE)*length);
    *(p->value+length)=0;//insert null.

    p->intValue=0;
    p->index=0;

    return p;
}

char closePolyForm(struct_galoisFieldPolyForm **p)
{
    #ifndef RELEASE
    if(!p)
    {
        warningMes;
        printf("in struct_galoisFieldPolyForm, struct_galoisFieldPolyForm *p = 0x%lx\n", (unsigned long)p);
        return -1;
    }
    if(!*p)
    {
        warningMes;
        printf("in struct_galoisFieldPolyForm, struct_galoisFieldPolyForm **p = 0x%lx\n", (unsigned long)*p);
        return -1;
    }
    #endif

    free((*p)->value);
    free((*p));
    *p=NULL;
    return 0;
}

struct_galoisFieldPolyForm *recreatePolyForm(struct_galoisFieldPolyForm **p, unsigned int length)
{
    if(*p)
    {
        #ifndef RELEASE
        if((global_flag_cmdOption&FLAG_MASK_PRINTF_LOG))
        {
            printf("in recreatePolyForm, struct_galoisFieldPolyForm *p is already have data.\n");
            printf("*p will be closePolyForm(p).\n");
        }
        #endif

        closePolyForm(p);
    }
    *p=createPolyForm(length);
    return *p;
}

/* Setter, Getter */
void inputValueToPolyForm(struct_galoisFieldPolyForm *p, char *string)
{
    unsigned int i;

    #ifndef RELEASE
    if(!p)
    {
        errorMes;
        printf("in inputValueToPolyForm, struct_galoisFieldPolyForm addr is wrong(addr:0x%lx).\n", (unsigned long)p);
    }
    if(p->length!=strlen(string))
    {
        errorMes;
        printf("in inputValueToPolyForm, %s(%ld) and struct_galoisFieldPolyForm->length(%d) are not same.\n", string, strlen(string), p->length);
    }
    #endif

    #ifndef RELEASE
    for(i=0; i<p->length; i++)
    {
        if((*(string+i)=='0')&&(*(string+i)=='1'))
        {
            errorMes;
            printf("%s(%ld) is composed with wrong character.\n", string, strlen(string));
            return;
        }
    }
    #endif

    #ifdef USING_OPEN_MP
    #pragma omp parallel for schedule(guided) private(i) shared(p, string)
    #endif
    for(i=0; i<p->length; i++)
    {
        *(p->value+i)=*(string+i);
    }
}

/* Convert */
unsigned int convertGaloisFieldPolyFormUnsignedInt(struct_galoisFieldPolyForm *p)
{
    unsigned int i;
    unsigned int result=0;

    #ifdef USING_OPEN_MP
    #pragma omp parallel for schedule(guided) private(i) shared(p) reduction(|:result)
    #endif
    for(i=0; i<p->length; i++)
    {
        if(*(p->value+i)=='1')
        {
            result|=(1<<i);
        }
    }
    return result;
}

/* Comparing, Same -> True(1), Diff -> False(0) */
char checkValueFromPolyFormUsingGaloisFieldValue(struct_galoisFieldPolyForm *operandA, struct_galoisFieldPolyForm *operandB)
{
    unsigned int i;

    #ifndef RELEASE
    if(!operandA)
    {
        errorMes;
        printf("struct_galoisFieldPolyForm addr is wrong(addr:0x%lx).\n", (unsigned long)operandA);
        return 0;
    }
    if(!operandB)
    {
        errorMes;
        printf("struct_galoisFieldPolyForm addr is wrong(addr:0x%lx).\n", (unsigned long)operandB);
        return 0;
    }
    if(operandA->length!=operandB->length)
    {
        errorMes;
        printf("galois field polynomial A and B lengths(%d!=%d) are not same each other.\n", operandA->length, operandB->length);
        return 0;
    }
    #endif

    for(i=0; i<operandA->length; i++)
    {
        if((*(operandA->value+i))!=(*(operandB->value+i)))
        {
            return 0;
        }
    }
    /*
    //if((global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)==FLAG_MASK_PRINTF_LOG)
    //{
    //    logMes;
    //    printf("find!\n");
    //}
    */
    return 1;
}

char checkValueFromPolyFormUsingGaloisFieldValueUsingIntValue_(struct_galoisFieldPolyForm *operandA, struct_galoisFieldPolyForm *operandB)
{
    #ifndef RELEASE
    if(!operandA)
    {
        errorMes;
        printf("struct_galoisFieldPolyForm addr is wrong(addr:0x%lx).\n", (unsigned long)operandA);
        return 0;
    }
    if(!operandB)
    {
        errorMes;
        printf("struct_galoisFieldPolyForm addr is wrong(addr:0x%lx).\n", (unsigned long)operandB);
        return 0;
    }
    if(operandA->length!=operandB->length)
    {
        errorMes;
        printf("galois field polynomial A and B lengths(%d!=%d) are not same each other.\n", operandA->length, operandB->length);
        return 0;
    }
    #endif


    if(!((operandA->intValue) ^ (operandB->intValue)))
    {
        return 1;
    }

    return 0;
}

char checkAllValueUsingCharacterFromPolyForm(struct_galoisFieldPolyForm *p, char c)
{
    unsigned int i;

    #ifndef RELEASE
    if(!p)
    {
        errorMes;
        printf("in checkAllValueUsingCharacterFromPolyForm, struct_galoisFieldPolyForm addr is wrong(addr:0x%lx).\n", (unsigned long)p);
        return 0;
    }
    if((c!='0')&&(c!='0'))
    {
        errorMes;
        printf("in checkAllValueUsingCharacterFromPolyForm, \'%c\'(0x%lx) is not \'0\' or \'1\'.\n", c, (unsigned long)&c);
        return 0;
    }
    #endif

    for(i=0; i<p->length; i++)
    {
        if((*(p->value+i))!=c)
        {
            return 0;
        }
    }
    return 1;
}

char checkValueUsingStringFromPolyForm(struct_galoisFieldPolyForm *p, char *string)
{
    unsigned int i;

    #ifndef RELEASE
    if(!p)
    {
        errorMes;
        printf("in checkValueUsingStringFromPolyForm, struct_galoisFieldPolyForm addr is wrong(addr:0x%lx).\n", (unsigned long)p);
        return 0;
    }
    if(p->length!=strlen(string))
    {
        errorMes;
        printf("in checkValueUsingStringFromPolyForm, \"%s\"(%ld) and struct_galoisFieldPolyForm->length(%d) is not same.\n", string, strlen(string), p->length);
        return 0;
    }
    #endif

    for(i=0; i<p->length; i++)
    {
        if((*(string+i)!='0')&&(*(string+i)!='1'))
        {
            #ifndef RELEASE
                errorMes;
                printf("in checkValueUsingStringFromPolyForm, \"%s\"(%ld) is composed with wrong character.\n", string, strlen(string));
            #endif
            return 0;
        }
        else if((*(p->value+i))!=(*(string+i)))
        {
            return 0;
        }
    }
    return 1;
}

/* For Display(Debugging) */
void printPolyFormWithEnter(struct_galoisFieldPolyForm *p)
{
    unsigned int i;

    if(!p)
    {
        errorMes;
        printf("struct_galoisFieldPolyForm addr is wrong(addr:0x%lx).\n", (unsigned long)p);
    }

    printf("\"");
    for(i=0; i<p->length; i++)
    {
        printf("%c", *(p->value+i));
    }
    printf("\"");
    printf("\n");
}

void printPolyFormPure(struct_galoisFieldPolyForm *p)
{
    unsigned int i;

    if(!p)
    {
        errorMes;
        printf("struct_galoisFieldPolyForm addr is wrong(addr:0x%lx).\n", (unsigned long)p);
    }

    for(i=0; i<p->length; i++)
    {
        printf("%c", *(p->value+i));
    }
}

void printPolyForm(struct_galoisFieldPolyForm *p)
{
    unsigned int i;

    if(!p)
    {
        errorMes;
        printf("struct_galoisFieldPolyForm addr is wrong(addr:0x%lx).\n", (unsigned long)p);
    }

    printf("\"");
    for(i=0; i<p->length; i++)
    {
        printf("%c", *(p->value+i));
    }
    printf("\"");
}

void printPolyFormWithTapAndEnter(struct_galoisFieldPolyForm *p)
{
    unsigned int i;

    if(!p)
    {
        errorMes;
        printf("struct_galoisFieldPolyForm addr is wrong(addr:0x%lx).\n", (unsigned long)p);
    }

    for(i=0; i<p->length; i++)
    {
        printf("%c\t", *(p->value+i));
    }
    printf("\n");
}

void printPolyFormWithTap(struct_galoisFieldPolyForm *p)
{
    unsigned int i;

    if(!p)
    {
        errorMes;
        printf("struct_galoisFieldPolyForm addr is wrong(addr:0x%lx).\n", (unsigned long)p);
    }

    for(i=0; i<p->length; i++)
    {
        printf("%c\t", *(p->value+i));
    }
}
