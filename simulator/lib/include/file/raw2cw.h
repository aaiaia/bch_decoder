#ifndef _FILE_RAW2CW_H_
#define _FILE_RAW2CW_H_

#include <stdlib.h>
#include <string.h>

#include "poly/polynomial.h"

/* It is load bitstream(raw data like bmp, wave etc) to codeword */
struct struct_variableSetConvertBitStreamToPowerForm
{
    unsigned char *data;

    unsigned char dataBitLength;

    unsigned int codeWordPerDataNumber;

    unsigned long totalCodeWordNumber;
    unsigned long countedCodeWordNumber;

    unsigned long totalDataNumber;
    unsigned long countedDataNumber;

}typedef struct_variableSetConvertBitStreamToPowerForm;

char setVariableSetConvertBitStreamToPowerForm(struct_variableSetConvertBitStreamToPowerForm *p, unsigned char *data, unsigned int infoBitLength, unsigned char dataBytes, unsigned long arrayIndex);
struct_variableSetConvertBitStreamToPowerForm *createVariableSetConvertBitStreamToPowerForm(struct_variableSetConvertBitStreamToPowerForm **p);
char closeVariableSetConvertBitStreamToPowerForm(struct_variableSetConvertBitStreamToPowerForm **p);
char loadToCodeWordStartAtHighSide(struct_powerFormPolynomials *p, struct_variableSetConvertBitStreamToPowerForm *variables);
char unloadFromCodeWordStartAtHighSide(struct_variableSetConvertBitStreamToPowerForm *variables, struct_powerFormPolynomials *p);

#endif
