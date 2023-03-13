#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "common/cmd_opt_vars.h"
#include "poly/primitive.h"

void initPrimitivePolynomial(struct_primitivePolynomialSet *p)
{
    p->PRIMITIVE_POLYNOMIAL_NUMBER=MAXUMUM_PRIMITIVE_POLYNOMIAL_NUMBER;
    p->PRIMITIVE_POLYNOMIAL=(char_POWER_FORM**)malloc(sizeof(const char_POWER_FORM*)*(p->PRIMITIVE_POLYNOMIAL_NUMBER));

    p->PRIMITIVE_POLYNOMIAL[0]=PRIMITIVE_POLYNOMIAL_1_INVERSE_INDEX;
    p->PRIMITIVE_POLYNOMIAL[1]=PRIMITIVE_POLYNOMIAL_2_INVERSE_INDEX;
    p->PRIMITIVE_POLYNOMIAL[2]=PRIMITIVE_POLYNOMIAL_4_INVERSE_INDEX;
    p->PRIMITIVE_POLYNOMIAL[3]=PRIMITIVE_POLYNOMIAL_8_INVERSE_INDEX;
    p->PRIMITIVE_POLYNOMIAL[4]=PRIMITIVE_POLYNOMIAL_16_INVERSE_INDEX;
    p->PRIMITIVE_POLYNOMIAL[5]=PRIMITIVE_POLYNOMIAL_32_INVERSE_INDEX;
    p->PRIMITIVE_POLYNOMIAL[6]=PRIMITIVE_POLYNOMIAL_64_INVERSE_INDEX;
    p->PRIMITIVE_POLYNOMIAL[7]=PRIMITIVE_POLYNOMIAL_128_INVERSE_INDEX;
    p->PRIMITIVE_POLYNOMIAL[8]=PRIMITIVE_POLYNOMIAL_256_INVERSE_INDEX;
    p->PRIMITIVE_POLYNOMIAL[9]=PRIMITIVE_POLYNOMIAL_512_INVERSE_INDEX;

    p->PRIMITIVE_POLYNOMIAL[10]=PRIMITIVE_POLYNOMIAL_1024_INVERSE_INDEX;
    p->PRIMITIVE_POLYNOMIAL[11]=PRIMITIVE_POLYNOMIAL_2048_INVERSE_INDEX;

    p->PRIMITIVE_POLYNOMIAL[12]=PRIMITIVE_POLYNOMIAL_4096_INVERSE_INDEX;
    p->PRIMITIVE_POLYNOMIAL[13]=PRIMITIVE_POLYNOMIAL_8192_INVERSE_INDEX;
    p->PRIMITIVE_POLYNOMIAL[14]=PRIMITIVE_POLYNOMIAL_16384_INVERSE_INDEX;
    p->PRIMITIVE_POLYNOMIAL[15]=PRIMITIVE_POLYNOMIAL_32768_INVERSE_INDEX;
    p->PRIMITIVE_POLYNOMIAL[16]=PRIMITIVE_POLYNOMIAL_65536_INVERSE_INDEX;
}

void initPrimitivePolynomial_inversed(struct_primitivePolynomialSet *inversed, struct_primitivePolynomialSet *non_inversed)
{
    unsigned int i;
    int j;
    unsigned int bitLoc;
    unsigned int polyLength;

    inversed->PRIMITIVE_POLYNOMIAL=(char_POWER_FORM**)malloc(sizeof(char_POWER_FORM*)*(non_inversed->PRIMITIVE_POLYNOMIAL_NUMBER));
    for(i=0; i<non_inversed->PRIMITIVE_POLYNOMIAL_NUMBER; i++)
    {
        polyLength=strlen(non_inversed->PRIMITIVE_POLYNOMIAL[i]);
        inversed->PRIMITIVE_POLYNOMIAL[i]=(char_POWER_FORM*)malloc((sizeof(char_POWER_FORM)*polyLength)+1);
        *(((char_POWER_FORM*)inversed->PRIMITIVE_POLYNOMIAL[i])+polyLength)=0;

        bitLoc=0;
        for(j=(polyLength-1); j>-1; j--)
        {
            if(*(non_inversed->PRIMITIVE_POLYNOMIAL[i]+j)=='1') *(((char_POWER_FORM*)inversed->PRIMITIVE_POLYNOMIAL[i])+bitLoc)='1';
            else if(*(non_inversed->PRIMITIVE_POLYNOMIAL[i]+j)=='0') *(((char_POWER_FORM*)inversed->PRIMITIVE_POLYNOMIAL[i])+bitLoc)='0';
            else{ errorMes; printf("detected!!! ilegal word, 0x%02x, %c", *(non_inversed->PRIMITIVE_POLYNOMIAL[i]+j), *(non_inversed->PRIMITIVE_POLYNOMIAL[i]+j)); return; }
            bitLoc++;
        }

    }

    inversed->PRIMITIVE_POLYNOMIAL_NUMBER=non_inversed->PRIMITIVE_POLYNOMIAL_NUMBER;
}
