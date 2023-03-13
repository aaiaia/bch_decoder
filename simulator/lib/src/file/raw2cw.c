#include "raw2cw.h"

char setVariableSetConvertBitStreamToPowerForm(struct_variableSetConvertBitStreamToPowerForm *p, unsigned char *data, unsigned int infoBitLength, unsigned char dataBytes, unsigned long arrayIndex)
{
    #ifndef RELEASE
    if(!p)
    {
        return -1;
    }
    if(!infoBitLength)
    {
        return -1;
    }
    if(!dataBytes)
    {
        return -1;
    }
    if(!arrayIndex)
    {
        return -1;
    }
    if(!data)
    {
        return -1;
    }
    #endif

    p->data=data;

    p->dataBitLength=dataBytes*8;
    p->codeWordPerDataNumber=infoBitLength/(p->dataBitLength);
    if(p->codeWordPerDataNumber)//infoBitLength>(p->dataBitLength)
    {
        p->totalCodeWordNumber=arrayIndex/(p->codeWordPerDataNumber);
        if(arrayIndex%(p->codeWordPerDataNumber))
        {
            p->totalCodeWordNumber++;
        }
    }
    else
    {
        p->totalCodeWordNumber=0;
    }
    p->totalDataNumber=arrayIndex;

    p->countedCodeWordNumber=0;

    p->countedDataNumber=0;

    return 0;
}

struct_variableSetConvertBitStreamToPowerForm *createVariableSetConvertBitStreamToPowerForm(struct_variableSetConvertBitStreamToPowerForm **p)
{
    if(!p)
    {
        return NULL;
    }
    else if(!(*p))
    {
        free(*p);
    }
    (*p)=(struct_variableSetConvertBitStreamToPowerForm*)malloc(sizeof(struct_variableSetConvertBitStreamToPowerForm));
    memset(*p, 0, sizeof(struct_variableSetConvertBitStreamToPowerForm));
    return *p;
}

char closeVariableSetConvertBitStreamToPowerForm(struct_variableSetConvertBitStreamToPowerForm **p)
{
    if(!p)
    {
        return 0;
    }
    else if(!(*p))
    {
        free(*p);
    }
    (*p)=NULL;
    return 0;
}

char loadToCodeWordStartAtHighSide(struct_powerFormPolynomials *p, struct_variableSetConvertBitStreamToPowerForm *variables)
{
    unsigned int i;
    unsigned char shifted;
    unsigned int equationIndex;

    #ifndef RELEASE
    if(!p)
    {
        return -1;
    }
    if(!variables)
    {
        return -1;
    }
    #endif

    /*initial high side condition*/
    shifted=variables->dataBitLength;
    equationIndex=p->usedLength;

    /*load 1 codeword start*/
    for(i=0; i<variables->codeWordPerDataNumber; i++)
    {
        /*load 8 bits start*/
        if(!(variables->countedDataNumber<variables->totalDataNumber))
        {
            break;
        }
        for(shifted=variables->dataBitLength; shifted!=0; shifted--)
        {
            if(((1<<(shifted-1))&(*(variables->data+variables->countedCodeWordNumber))))
            {
                *(p->equation+equationIndex-1)='1';
            }
            else
            {
                *(p->equation+equationIndex-1)='0';
            }
            equationIndex--;
        }
        variables->countedDataNumber++;
        /*load 8 bits end*/
        variables->countedCodeWordNumber++;
    }
    /*load 1 codeword end*/

    for(; equationIndex!=0; equationIndex--)
    {
        *(p->equation+equationIndex-1)='0';
    }

    return 0;
}

char unloadFromCodeWordStartAtHighSide(struct_variableSetConvertBitStreamToPowerForm *variables, struct_powerFormPolynomials *p)
{
    unsigned int i;
    unsigned char shifted;
    unsigned int equationIndex;

    #ifndef RELEASE
    if(!p)
    {
        return -1;
    }
    if(!variables)
    {
        return -1;
    }
    #endif

    /*initial high side condition*/
    shifted=variables->dataBitLength;
    equationIndex=p->usedLength;

    /*load 1 codeword start*/
    for(i=0; i<variables->codeWordPerDataNumber; i++)
    {
        /*load 8 bits start*/
        if(!(variables->countedDataNumber<variables->totalDataNumber))
        {
            break;
        }
        (*(variables->data+variables->countedCodeWordNumber))=0;
        for(shifted=variables->dataBitLength; shifted!=0; shifted--)
        {
            if(*(p->equation+equationIndex-1)=='1')
            {
                (*(variables->data+variables->countedCodeWordNumber))|=(1<<(shifted-1));
            }
            equationIndex--;
        }
        variables->countedDataNumber++;
        /*load 8 bits end*/
        variables->countedCodeWordNumber++;
    }
    /*load 1 codeword end*/

    return 0;
}
