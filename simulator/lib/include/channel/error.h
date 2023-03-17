#ifndef _CHANNEL_ERROR_H_
#define _CHANNEL_ERROR_H_

#include "poly/polynomial.h"

struct struct_errorComponent
{
    struct_powerFormPolynomials *erroredCodeWord;
    struct_powerFormPolynomials *errorLocationVector;
}typedef struct_errorComponent;

/* Constructor and Destructor */
struct_errorComponent *createErrorComponent(unsigned int length);
char closeErrorComponent(struct_errorComponent **p);
struct_errorComponent *recreateErrorComponent(struct_errorComponent **p, unsigned int length);

/* Operation */
struct_errorComponent *ifNotExistCreateErrorComponent(struct_errorComponent **p, unsigned int length);
char *randdomInfoBitStreamChanger(unsigned int infoBitsLength, unsigned int parityBitsLength, char *p);
char *randdomInfoBitChanger(unsigned int infoBitsLength, unsigned int parityBitsLength, char *p);
char *allSameInfoBitGenerator(char initChar, unsigned int infoBitsLength, unsigned int parityBitsLength, char **p);
char *randdomInfoBitGenerator(unsigned int infoBitsLength, unsigned int parityBitsLength, char **p);

/* Generate test pattern using LLR locator */
/*
 * Using string type of codeword(binary) and error locator,
 * generate a test pattern(is binary and string)
 */
char *temporaryFunc_generateTestPatternToString(char *string, unsigned int stringLength, unsigned int *locatorArray, unsigned int locatorArrayLength);
char temporaryFunc_generateTestPatterns_using_LLR_Locator
    (
        char *refferString,
        unsigned int refferStringLength,
        unsigned int *locatorArray,
        unsigned int locatorArrayLength,
        char **testPattern,
        unsigned int numsOfTestPattern
    );

#endif
