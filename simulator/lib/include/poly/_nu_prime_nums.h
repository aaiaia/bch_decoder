#ifndef _POLY_PRIME_NUMS_H_
#define _POLY_PRIME_NUMS_H_

/*to generate elements of galois field 2, primitive polynomials*/
const unsigned int primmeNumber[]={07, 013, 023, 045, 0103, 0211, 0435, 01021, 02011};

char primitivePringBuffer[33] = {0};


char *convertPrimeNumberToString(char *string, unsigned int exponential, const unsigned int *primeNumberArray);

#endif
