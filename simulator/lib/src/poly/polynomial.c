#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include "polynomial.h"

/* bits is mean that length of bits */
struct_powerFormPolynomials *createPowerFormPolynomial(unsigned int length)
{
    struct_powerFormPolynomials *p;

    #ifndef RELEASE
    if(!length)
    {
        errorMes;
        printf("length of polynomial is %d.\n",length);
        return NULL;
    }
    #endif

    p=(struct_powerFormPolynomials*)malloc(sizeof(struct_powerFormPolynomials));
    memset(p, 0, sizeof(struct_powerFormPolynomials));

    //p->numberOfelement=&(p->length);
    p->length=length;

    p->usedLength=length;

    p->equation=(char_POWER_FORM*)malloc(sizeof(char_POWER_FORM)*(length+1));//+1 is to null
    memset(p->equation, '0', sizeof(char_POWER_FORM)*(length));
    *(p->equation+length)=0;

    /*quantization setting. default setting is zero.*/
    //p->llrMask=0;
    //p->llr=NULL;

    return p;
}

struct_powerFormPolynomials *createPowerFormPolynomialUsingString(char *string, unsigned int strLength)
{
    unsigned int i;

    struct_powerFormPolynomials *p;

    #ifndef RELEASE
    if(!string)
    {
        errorMes;
        printf("in createPowerFormPolynomialUsingString, string is NULL.\n");
        return NULL;
    }
    #endif

    if(!strLength)
    {
        strLength=strlen(string);
    }

    p = createPowerFormPolynomial(strLength);

    //memcpy(p->equation, string, strLength);
    #ifdef USING_OPEN_MP
    #pragma omp parallel for schedule(guided) private(i) shared(strLength, string, p)
    #endif
    for(i=0; i<strLength; i++)
    {
        if((*(string+i)!='0')&&(*(string+i)!='1'))
        {
            #ifndef RELEASE
            errorMes; printf("in createPowerFormPolynomialUsingString, string have another character(can use only '0' and '1').\niuputString : %d-th '%c'\n", i, *(string+i));
            #endif

            #ifdef USING_OPEN_MP
            if(*(p->equation+i)%2)
            {
                *(p->equation+i)='1';
            }
            else
            {
                *(p->equation+i)='0';
            }
            #else
            if(rand()%2)
            {
                *(p->equation+i)='1';
            }
            else
            {
                *(p->equation+i)='0';
            }
            #endif
        }
        else
        {
            *(p->equation+i)=*(string+i);
        }
    }
    *(p->equation+strLength)=0;

    return p;
}

char closePowerFormPolynomial(struct_powerFormPolynomials **p)
{
    #ifndef RELEASE
    if(!p)
    {
        warningMes;
        printf("in closePowerFormPolynomial, struct_powerFormPolynomials **p has NULL;\n");
        printf("it is already removed.\n");
        return 0;
    }
    if(!(*p))
    {
        warningMes;
        printf("in closePowerFormPolynomial, struct_powerFormPolynomials *p is already removed.\n");
        printf("it is already removed.\n");
        return -1;
    }
    #endif

    if((*p)->equation)
    {
        free((*p)->equation);
    }
    //if((*p)->llr!=NULL)
    //{
    //    free((*p)->llr);
    //}

    free((*p));
    *p=NULL;
    return 0;
}

struct_powerFormPolynomials *recreatePowerFormPolynomial(struct_powerFormPolynomials **p, unsigned int length)
{
    if(*p)
    {
        #ifndef RELEASE
        if((global_flag_cmdOption&FLAG_MASK_PRINTF_LOG))
        {
            printf("in recreatePowerFormPolynomial, struct_powerFormPolynomials *p is already have data.\n");
            printf("*p will be closePowerFormPolynomial(p).\n");
        }
        #endif

        closePowerFormPolynomial(p);
    }
    *p=createPowerFormPolynomial(length);
    return *p;
}

char setPowerFormPolynomialUsingStringAddr(struct_powerFormPolynomials *p, char *string, unsigned int usedLength, unsigned int length)
{
    unsigned int strLength;

    //unsigned int tmp_length=0;
    //unsigned int tmp_usedLength=0;
    char_POWER_FORM *tmp_char_powerForm=NULL;

    #ifndef RELEASE
    if(!p)
    {
        errorMes;
        printf("in setPowerFormPolynomialUsingStringAddr, struct_powerFormPolynomials p is NULL\n");
        return -1;
    }

    if(!string)
    {
        errorMes;
        printf("in setPowerFormPolynomialUsingStringAddr, not exist string.\n");
        return -1;
    }
    #endif

    #ifndef RELEASE
    if(!usedLength)
    {
        strLength=strlen(string);
        usedLength=strLength;
        if(!strLength)
        {
            /*no length infomation at all*/
            errorMes;
            printf("in setPowerFormPolynomialUsingStringAddr, string length(%d) is ZERO.\n", usedLength);
            return -1;
        }
        else if(!length)
        {
            warningMes; printf("in setPowerFormPolynomialUsingStringAddr, unsigned int  length is ZERO, p->length can set to strlen(string).\n");
            warningMesShort; printf("So simulator can be broken, when operate with different length of p->equation each other.\n");
            length=strLength;
        }
    }
    #endif

    #ifndef RELEASE
    if(length<usedLength)
    {
        errorMes;
        printf("in setPowerFormPolynomialUsingStringAddr, length < usedLength.\n");
        return -1;
    }
    #endif

    #ifndef RELEASE
    if(!(p->length))
    {
        warningMes; printf("in setPowerFormPolynomialUsingStringAddr, p->length is ZERO.\n");
        warningMesShort; printf("p->length can be over wroten to arg of length\n");
    }
    else
    {
        if(p->length != length)
        {
            warningMes; printf("in setPowerFormPolynomialUsingStringAddr, p->length != length.\n");
            warningMesShort; printf("p->length is will over wroten to arg of length.\n");
            warningMesShort; printf("Simulation can be broken.\n");
        }
    }
    if(!(p->usedLength))
    {
        warningMes; printf("in setPowerFormPolynomialUsingStringAddr, p->usedLength is ZERO.\n");
        warningMesShort; printf("p->usedLength can be over wroten to arg of usedLength\n");
    }
    else
    {
        if(p->usedLength != usedLength)
        {
            warningMes; printf("in setPowerFormPolynomialUsingStringAddr, p->usedLength != usedLength.\n");
            warningMesShort; printf("p->usedLength is will over wroten to arg of usedLength.\n");
            warningMesShort; printf("Simulation can be broken.\n");
        }
    }
    if(!(p->equation))
    {
        /*dont care case*/
        warningMes; printf("in setPowerFormPolynomialUsingStringAddr, p->equation(0x%lx) is not NULL.\n", (unsigned long)p->equation);
    }
    #endif


    /* backuped */
    //tmp_length=p->length;
    //tmp_usedLength=p->usedLength;
    tmp_char_powerForm=p->equation;

    p->length=length;
    p->usedLength=usedLength;
    p->equation=((char_POWER_FORM*)string);

            #ifndef RELEASE
            if((global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)==FLAG_MASK_PRINTF_LOG)
            {
                logMes;
                printf("in copyPowerFormPolynomialFromString, previous value is...\n");
                printPowerFormWithEnter(tmp_char_powerForm);
                printf("And replaced value is...\n");
                printPowerFormWithEnter(p->equation);
                printf("And buffered Length is \'%d\', used length is \'%d\'\n", p->length, p->usedLength);
                printf("Previous value will be deleted.\n");
            }
            #endif

    if(tmp_char_powerForm)
    {
        free(tmp_char_powerForm);
    }

    return 0;
}

char setPowerFormPolynomialUsingStringAddr_clearStringPointer(struct_powerFormPolynomials *p, char **string, unsigned int usedLength, unsigned int length)
{
    if(!string)
    {
        errorMes; printf("in setPowerFormPolynomialUsingStringAddr_clearStringPointer, char **string is NULL.\n");
        return -1;
    }
    if(setPowerFormPolynomialUsingStringAddr(p, *string, usedLength, length))
    {
        errorMes; printf("in setPowerFormPolynomialUsingStringAddr_clearStringPointer, setPowerFormPolynomialUsingStringAddr return to not ZERO.\n");
        return -1;
    }
    *string = NULL;

    return 0;
}

char copyPowerFormPolynomialFromString(struct_powerFormPolynomials *p, char *string)
{
    unsigned int i, strLength;
    char *backupP;

    #ifndef RELEASE
    if(!string)
    {
        errorMes;
        printf("in copyPowerFormPolynomialFromString, not exist string.\n");
        return -1;
    }
    #endif

    strLength=strlen(string);
    if(strLength>p->length)
    {
        errorMes;
        printf("in copyPowerFormPolynomialFromString, string length(%d) is have to shorter than power form polynomial length(%d).\n", strLength, p->length);
        return -1;
    }
    #ifndef RELEASE
    if(!(p->equation))
    {
        errorMes;
        printf("in copyPowerFormPolynomialFromString, p->equation(0x%lx) is not defined.\n", (unsigned long)p->equation);
        return -1;
    }
    #endif

    backupP = (char*)malloc(sizeof(char)*(p->length+1));
    #ifndef USING_OPEN_MP
    memcpy(backupP, p->equation, sizeof(sizeof(char)*(p->usedLength+1)));
    #else
    #pragma omp parallel for schedule(guided) private(i) shared(p, backupP)
    for(i=0; i<p->usedLength; i++)
    {
        *(backupP+i) = *(p->equation+i);
    }
    #endif
    *(backupP+p->usedLength)=0;

    #ifndef USING_OPEN_MP
    memcpy(p->equation, string, sizeof(char)*(strLength+1));
    #else
    #pragma omp parallel for schedule(guided) private(i) shared(p, string, strLength)
    for(i=0; i<strLength; i++)
    {
        *(p->equation+i) = *(string+i);
    }
    #endif
    *(p->equation+strLength)=0;

            #ifndef RELEASE
            for(i=0; i<strLength; i++)
            {
                if((*(string+i)!='0')&&(*(string+i)!='1'))
                {
                    errorMes;
                    printf("in copyPowerFormPolynomialFromString, string have another character(can use only '0' and '1').\niuputString : %s\n", string);
                    printf("Can't copy input string \"%s\" to struct_powerFormPolynomials *p->equation.\n", string);
                    printf("p->length is \'%d\', strLength is \'%d\', loops num is \'%d\', input character is \'%c(%d)\'.\n", p->length, strLength, i, *(string+i), *(string+i));

                    //if(p->equation)
                    //{
                        free(p->equation);
                    //}
                    p->equation=backupP;
                    return -1;
                }
            }
            #endif
    /*insert NULL*/

    p->usedLength=strLength;
            #ifndef RELEASE
            if((global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)==FLAG_MASK_PRINTF_LOG)
            {
                logMes;
                printf("in copyPowerFormPolynomialFromString, setted power form equation is...\n");
                printPowerFormWithEnter(p->equation);
            }
            #endif

    free(backupP);
    return 0;
}

/*
//struct_powerFormPolynomials *createPowerFormPolynomialAndInputValue(char *string)
//{
//    struct_powerFormPolynomials *p = createPowerFormPolynomial(strlen(string));
//    if((global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)==FLAG_MASK_PRINTF_LOG)
//    {
//        logMes;
//        printf("in createPowerFormPolynomialAndInputValue, CreationPowerFormPolynomial. input String is : \"%s\"\n", string);
//    }
//    if(copyPowerFormPolynomialFromString(p, string))
//    {
//        //closePowerFormPolynomial(p);
//        errorMes;
//        printf("in createPowerFormPolynomialAndInputValue, copyPowerFormPolynomialFromString(p, string).\n");
//        return NULL;
//    }
//    return p;
//}
*/

struct_powerFormPolynomials *recreatePowerFormPolynomialUsingString(struct_powerFormPolynomials **p, char *string, unsigned int strLength)
{
    #ifndef RELEASE
    if(*p)
    {
        if((global_flag_cmdOption&FLAG_MASK_PRINTF_LOG))
        {
            printf("in recreatePowerFormPolynomialUsingString, struct_powerFormPolynomials *p is already have data.\n");
            printf("it is cleared and recreate.\n");
        }
        closePowerFormPolynomial(p);
    }
    #endif


    if(!strLength) strLength = strlen(string);

    *p=createPowerFormPolynomialUsingString(string, strLength);
    return *p;
}

/* About polynomial operation */
void shiftHighSidePowerFormPolynomial(struct_powerFormPolynomials *p, unsigned int length)
{
    char_POWER_FORM *buffer;

    #ifndef RELEASE
    if(!p)
    {
        errorMes;
        printf("in shiftHighSidePowerFormPolynomial, struct_powerFormPolynomials *p is NULL.\n");
        return;
    }
    if(!length)
    {
        return;
    }
    if(!p->length)
    {
        infoMes;
        printf("in shiftHighSidePowerFormPolynomial, p->length is zero.\n");
        return;
    }
    if(p->length<length)
    {
        infoMes;
        printf("in shiftHighSidePowerFormPolynomial, shift length(%d) is can not exceed p->length(%d).\n", length, p->length);
        return;
    }

    if(p->usedLength<length)
    {
        warningMes;
        printf("in shiftHighSidePowerFormPolynomial, used length(%d) is shortter than shifted length.\n", p->usedLength);
        printf("Result can have incorrected value.\n");
        printf("Because shifted string will be cuf off at \'%d\'-th value\n", p->usedLength+1);
    }
    if((global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)==FLAG_MASK_PRINTF_LOG)
    {
        logMes;
        printf("in shiftHighSidePowerFormPolynomial, Shift length is \'%d\'.\n", length);
        printf("struct_powerFormPolynomials p->length is \'%d\'.\n", p->length);
        printf("struct_powerFormPolynomials p->usedLength is \'%d\'.\n", p->usedLength);
        printf("non-shifted codeWord is\n\"%s\".\n", p->equation);
    }
    #endif

    buffer=(char_POWER_FORM*)malloc(sizeof(char_POWER_FORM)*(p->usedLength-length));
    memcpy(buffer, p->equation, sizeof(char_POWER_FORM)*(p->usedLength-length));
    memset(p->equation, '0', sizeof(char_POWER_FORM)*length);
    memcpy(p->equation+length, buffer, sizeof(char_POWER_FORM)*(p->usedLength-length));
    *(p->equation+p->usedLength)=0;//insert NULL

            #ifndef RELEASE
            if((global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)==FLAG_MASK_PRINTF_LOG)
            {
                logMes;
                printf("in shiftHighSidePowerFormPolynomial, ");
                printf("Shift codeWord\n\"%s\".\n", p->equation);
            }
            #endif
    free(buffer);
}

void shiftLowSidePowerFormPolynomial(struct_powerFormPolynomials *p, unsigned int length)
{
    return;
}

unsigned int calculateHammingWeightPowerFormPolynomial(struct_powerFormPolynomials *p)
{
    unsigned int i;
    unsigned int result=0;
    if(!p) return 0;
    if(!p->equation) return 0;
    if(!p->usedLength) return 0;

    #ifdef USING_OPEN_MP
    #pragma omp parallel for schedule(guided) private(i) shared(p) reduction(+:result)
    #endif
    for(i=0; i<(p->usedLength); i++)
    {
        if(*(p->equation+i)=='1') result++;
    }
    return result;
}

unsigned int calculateHammingWeightFromDiffentPowerFormPolynomial(struct_powerFormPolynomials *p1, struct_powerFormPolynomials *p2)
{
    unsigned int i;
    unsigned int result=0;

    #ifndef RELEASE
    if(!p1)
    {
        errorMes;
        printf("in calculateHammingWeightFromDiffentPowerFormPolynomial, struct_powerFormPolynomials p1.\n");
        return -1;
    }
    if(!p2)
    {
        errorMes;
        printf("in calculateHammingWeightFromDiffentPowerFormPolynomial, struct_powerFormPolynomials p2.\n");
        return -1;
    }
    if(p1->usedLength!=p2->usedLength)
    {
        errorMes;
        printf("in calculateHammingWeightFromDiffentPowerFormPolynomial, p1->usedLength!=p2->usedLength, can't comparing.\n");
        return -1;
    }
    #endif

    #ifdef USING_OPEN_MP
    #pragma omp parallel for schedule(guided) private(i) shared(p1, p2) reduction(+:result)
    #endif
    for(i=0; i<p1->usedLength; i++)
    {
        //if(((*(p1->equation+i))=='0'))
        //{
        //    if(((*(p2->equation+i))!='0')) result++;
        //}
        //else if(((*(p1->equation+i))=='1'))
        //{
        //    if(((*(p2->equation+i))!='1')) result++;
        //}
        if((*(p1->equation+i))!=(*(p2->equation+i))) result++;
    }
    return result;
}

/* for make error */
void errorOccurationWhichAreRandomPosition(struct_powerFormPolynomials *p, unsigned int numberOfError)
{
    unsigned int i;

    #ifndef RELEASE
    if(!p)
    {
        errorMes;
        printf("in errorOccurationWhichAreRandomPosition, struct_powerFormPolynomials *p is NULL.\n");
    }
    #endif
    /* initial all zero condition */
    memset(p->equation, '0', sizeof(char_POWER_FORM)*(p->usedLength));
    *(p->equation+(p->usedLength))=0;
    while(numberOfError)
    {
        if(*(p->equation+(i=(rand()%p->usedLength)))=='0')
        {
            *(p->equation+i)='1';
            numberOfError--;
        }
    }
}

/* summation */
void summationError(struct_powerFormPolynomials *codeWord, struct_powerFormPolynomials *error)
{
    unsigned int i;


    #ifndef RELEASE
    if(!codeWord)
    {
        errorMes;
        printf("in summationError, struct_powerFormPolynomials *codeWord is NULL.\n");
    }
    if(!error)
    {
        errorMes;
        printf("in summationError, struct_powerFormPolynomials *error is NULL.\n");
    }

    if(codeWord->usedLength!=error->usedLength)
    {
        errorMes;
        printf("in summationError, codeWord->length!=error->length.\n");
    }
    #endif

    for(i=0; i<codeWord->usedLength; i++)
    {
        if(*(error->equation+i)=='1')
        {
            if(*(codeWord->equation+i)=='0')
            {
                *(codeWord->equation+i)='1';
            }
            else
            {
                *(codeWord->equation+i)='0';
            }
        }
    }
}

void addErrorCodeWordToUnErrorCodeWord(struct_powerFormPolynomials *result, struct_powerFormPolynomials *codeWord, struct_powerFormPolynomials *error)
{
    unsigned int i;

    #ifndef RELEASE
    if(!result)
    {
        errorMes;
        printf("in addErrorCodeWordToUnErrorCodeWord, struct_powerFormPolynomials *result is NULL.\n");
    }
    if(!codeWord)
    {
        errorMes;
        printf("in addErrorCodeWordToUnErrorCodeWord, struct_powerFormPolynomials *codeWord is NULL.\n");
    }
    if(!error)
    {
        errorMes;
        printf("in addErrorCodeWordToUnErrorCodeWord, struct_powerFormPolynomials *error is NULL.\n");
    }

    if(result->usedLength!=codeWord->usedLength)
    {
        errorMes;
        printf("in addErrorCodeWordToUnErrorCodeWord, result->usedLength!=codeWord->usedLength.\n");
    }
    if(codeWord->usedLength!=error->usedLength)
    {
        errorMes;
        printf("in addErrorCodeWordToUnErrorCodeWord, codeWord->usedLength!=error->usedLength.\n");
    }
    #endif

    #ifdef USING_OPEN_MP
    #pragma omp parallel for schedule(guided) private(i) shared(codeWord, error, result)
    #endif
    for(i=0; i<codeWord->usedLength; i++)
    {

        if(*(error->equation+i)=='1')
        {
            if(*(codeWord->equation+i)=='0')
            {
                *(result->equation+i)='1';
            }
            else
            {
                *(result->equation+i)='0';
            }
        }
        else
        {
            *(result->equation+i)=*(codeWord->equation+i);
        }
    }

    *(result->equation+i)=0;
}

/* For Display(Debugging) */
void printPowerFormWithEnterPolynomialAndInfoUsingAddress(struct_powerFormPolynomials *p)
{
    if(!p)
    {
        errorMes;
        printf("in printPowerFormWithEnterPolynomialAndInfoUsingAddress, struct_powerFormPolynomials *p\n");
        return;
    }

    printf("Stored length is \'%d\' and used length of struct_powerFormPolynomials is \'%d\'\n", p->length, p->usedLength);
    printf("value : \"");
    printf("%s", p->equation);
    printf("\"\n");
}

void printPowerFormWithEnterPolynomialWithEnterUsingAddress(struct_powerFormPolynomials *p)
{
    unsigned int i;

    unsigned int even_bit;
    unsigned int odd_bit;
    unsigned int all_bit;
    if(!p)
    {
        errorMes;
        printf("in printPowerFormWithEnterPolynomialWithEnterUsingAddress, struct_powerFormPolynomials *p\n");
        return;
    }

    //printf("%s", p->equation);
    even_bit = 0;
    odd_bit = 0;
    all_bit = 0;
    for(i=0; i<p->usedLength; i++)
    {
        printf("%c", *(p->equation+i));
        if(i%2)//odd
        {
            if(*(p->equation+i) == '0')
            {
                odd_bit ^= 0;
                all_bit ^= 0;
            }
            else if(*(p->equation+i) == '1')
            {
                odd_bit ^= 1;
                all_bit ^= 1;
            }
            else
            {
                errorMes;
                printf("character is not '0' and '1'\n");
            }
        }
        else//even
        {
            if(*(p->equation+i) == '0')
            {
                even_bit ^= 0;
                all_bit ^= 0;
            }
            else if(*(p->equation+i) == '1')
            {
                even_bit ^= 1;
                all_bit ^= 1;
            }
            else
            {
                errorMes;
                printf("character is not '0' and '1'\n");
            }
        }

    }
    printf("[even]:%d",even_bit);
    printf("[odd]:%d",odd_bit);
    printf("[all]:%d",all_bit);
    printf("[used]:%d", p->usedLength);
    printf("[strLen]:%ld", strlen(p->equation));
    printf("[full]:%d\n", p->length);
}

void printPowerFormWithEnterPolynomialWithTapUsingAddress(struct_powerFormPolynomials *p)
{
    if(!p)
    {
        errorMes;
        printf("in printPowerFormWithEnterPolynomialWithTapUsingAddress, struct_powerFormPolynomials *p\n");
        return;
    }

    printf("%s\t", p->equation);
}

void printPowerFormExclusivedUsingAddress(struct_powerFormPolynomials *p1, struct_powerFormPolynomials *p2)
{
    unsigned int i;

    if(!p1)
    {
        errorMes;
        printf("in printPowerFormExclusivedUsingAddress, struct_powerFormPolynomials p1\n");
        return;
    }
    if(!p2)
    {
        errorMes;
        printf("in printPowerFormExclusivedUsingAddress, struct_powerFormPolynomials p2\n");
        return;
    }
    if(p1->usedLength!=p2->usedLength)
    {
        errorMes;
        printf("in printPowerFormExclusivedUsingAddress, p1->usedLength!=p2->usedLength\n");
        return;
    }


    for(i=0; i<p1->usedLength; i++)
    {
        if(((*(p1->equation+i))=='0'))
        {
            if(((*(p2->equation+i))=='0'))
            {
                printf("0");
            }
            else if(((*(p2->equation+i))=='1'))
            {
                printf("1");
            }
            else
            {
                printf("X");
            }
        }
        else if(((*(p1->equation+i))=='1'))
        {
            if(((*(p2->equation+i))=='0'))
            {
                printf("1");
            }
            else if(((*(p2->equation+i))=='1'))
            {
                printf("0");
            }
            else
            {
                printf("X");
            }
        }
        else
        {
            printf("X");
        }

    }
    printf("[used]:%d", p1->usedLength);
    printf("[full]:%d,%d\n", p1->length, p2->length);
    //printf("\n");
}

/* file IO */
void fprintPowerFormUsingAddress(FILE *fp, struct_powerFormPolynomials *p, char* str_tail)
{
    #ifndef RELEASE
        if(!fp)
        {
            errorMes;
            printf("in fprintPowerFormWithEnterPolynomialWithTapUsingAddress, FILE *fp\n");
            return;
        }
        if(!p)
        {
            errorMes;
            printf("in fprintPowerFormWithEnterPolynomialWithTapUsingAddress, struct_powerFormPolynomials *p\n");
            return;
        }
    #endif

    fprintf(fp, "%s", p->equation);
    if(str_tail) fprintf(fp, str_tail);
}

void fprintVerilogPowerFormUsingAddress(FILE *fp, struct_powerFormPolynomials *p, char* str_tail)
{
    unsigned int i;
    #ifndef RELEASE
        if(!fp)
        {
            errorMes;
            printf("in fprintPowerFormWithEnterPolynomialWithTapUsingAddress, FILE *fp\n");
            return;
        }
        if(!p)
        {
            errorMes;
            printf("in fprintPowerFormWithEnterPolynomialWithTapUsingAddress, struct_powerFormPolynomials *p\n");
            return;
        }
    #endif

    for(i=0; i<p->usedLength; i++)
    {
        fprintf(fp, "%c\r\n", p->equation[i]);
    }
    if(str_tail) fprintf(fp, str_tail);
}

void fprintPowerFormFullDescriptionUsingAddresss(FILE *fp, struct_powerFormPolynomials *p, char *str_tail)
{
    unsigned int i;

    unsigned int even_bit;
    unsigned int odd_bit;
    unsigned int all_bit;
    unsigned int illegal_bit;

    #ifndef RELEASE
    if(!p)
    {
        errorMes;
        printf("in printPowerFormWithEnterPolynomialWithEnterUsingAddress, struct_powerFormPolynomials *p\n");
        return;
    }

    if(!fp)
    {
        errorMes;
        printf("in printPowerFormWithEnterPolynomialWithEnterUsingAddress, FILE *fp\n");
        return;
    }
    #endif

    //printf("%s", p->equation);
    even_bit = 0;
    odd_bit = 0;
    all_bit = 0;
    illegal_bit = 0;
    for(i=0; i<p->usedLength; i++)
    {
        fprintf(fp, "%c", *(p->equation+i));
        if(i%2)//odd
        {
            if(*(p->equation+i) == '0')
            {
                odd_bit ^= 0;
                all_bit ^= 0;
            }
            else if(*(p->equation+i) == '1')
            {
                odd_bit ^= 1;
                all_bit ^= 1;
            }
            else
            {
                illegal_bit++;
            }
        }
        else//even
        {
            if(*(p->equation+i) == '0')
            {
                even_bit ^= 0;
                all_bit ^= 0;
            }
            else if(*(p->equation+i) == '1')
            {
                even_bit ^= 1;
                all_bit ^= 1;
            }
            else
            {
                illegal_bit++;
            }
        }

    }
    fprintf(fp, "[even]:%d",even_bit);
    fprintf(fp, "[odd]:%d",odd_bit);
    fprintf(fp, "[all]:%d",all_bit);
    fprintf(fp, "[used]:%d", p->usedLength);
    fprintf(fp, "[strLen]:%ld", strlen(p->equation));
    fprintf(fp, "[full]:%d", p->length);
    fprintf(fp, "[illegal]:%d", illegal_bit);

    if(str_tail) fprintf(fp, str_tail);
}
