#ifndef _FILE_AWGN_H_
#define _FILE_AWGN_H_

#include "file/raw2cw.h"

/* Example codes in main functions
//while((loadStream.countedDataNumber<loadStream.totalDataNumber))
//{
//    printf("[%0000006d] : ", loadStream.countedCodeWordNumber);
//    loadToCodeWordStartAtHighSide(main_encodingComponentInGF->codeWord, &loadStream);
//    unloadFromCodeWordStartAtHighSide(&unloadStream, main_encodingComponentInGF->codeWord);
//    printPowerFormWithEnterPolynomialWithTapUsingAddress(main_encodingComponentInGF->codeWord);
//    printf("[S:%d, U:%d, M:%d]\n",strlen(main_encodingComponentInGF->codeWord->equation), main_encodingComponentInGF->codeWord->usedLength, main_encodingComponentInGF->codeWord->length);
//}
//printBmpFileToHexCode2();
//saveBmpFile2(0);
*/
char passingThroughAwgnData(
    double filePass_EbN0,
    unsigned int filePass_parityBitsLength,
    char *filePass_generationPolynomial,
    unsigned int filePass_selectedNumberOfSetOfConjugacy,
    unsigned int filePass_codeLength,
    struct_galoisField_info *filePass_galoisField,
    struct_variableSetConvertBitStreamToPowerForm *loadStream,
    struct_variableSetConvertBitStreamToPowerForm *unloadStream,
    struct_variableSetConvertBitStreamToPowerForm *noisedStream
);

#endif
