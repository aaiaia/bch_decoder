#include "gf/field_set.h"
#include "channel/bpsk.h"
#include "channel/error.h"
#include "encoder/encode.h"
#include "decoder/hard_decision/algo_bm.h"
#include "decoder/hard_decision/chien_search.h"
#include "correction/error_correction.h"
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
)
{
    struct_encodingComponentInGF *filePass_encodingComponentInGaloisField=NULL;
    struct_errorComponent *filePass_errorComponents=NULL;
    struct_bpskComponents *filePass_bpskComponents=NULL;
    struct_HD_BM_algorithmComponent *filePass_decordingComponents=NULL;

    unsigned int errorCount=0;

    unsigned int i;

    printf(">> data passing AWGN channel\n");
    infoMes;
    printf("EbN0 is '%f'.\r\n", filePass_EbN0);
    infoMes;
    printf("Parity length is '%d'.\r\n", filePass_parityBitsLength);
    infoMes;
    printf("Generator polynomial is '%s'.\r\n", filePass_generationPolynomial);
    infoMes;
    printf("Set of conjugacy is '%d'.\r\n", filePass_selectedNumberOfSetOfConjugacy);
    infoMes;
    printf("Code length is '%d'.\r\n", filePass_codeLength);
    infoMes;
    printf("Field length is '%d'.\r\n", filePass_galoisField->nonSorted->length);

    if(filePass_EbN0 < 4.0)
    {
        warningMes;
        printf("EbN0 is low.\n");
    }

    filePass_encodingComponentInGaloisField = recreateEncodingComponent(&filePass_encodingComponentInGaloisField, filePass_codeLength, filePass_selectedNumberOfSetOfConjugacy, (char_POWER_FORM*)filePass_generationPolynomial);
    filePass_bpskComponents=createBpskComponents(filePass_codeLength);
    filePass_errorComponents=recreateErrorComponent(&filePass_errorComponents, filePass_codeLength);


    while((loadStream->countedDataNumber<loadStream->totalDataNumber))
    {
        loadToCodeWordStartAtHighSide(filePass_encodingComponentInGaloisField->codeWord, loadStream);
        calculateParityInGaloisFieldAttachLowSide(filePass_encodingComponentInGaloisField->codeWord, filePass_parityBitsLength, filePass_encodingComponentInGaloisField->generationPolynomial);

        do
        {
            filePass_errorComponents=recreateErrorComponent(&filePass_errorComponents, filePass_codeLength);
            BPSK_Mod(filePass_bpskComponents->bpskTransData, filePass_encodingComponentInGaloisField->codeWord->equation, filePass_encodingComponentInGaloisField->codeWord->length);

            ADD_AWGN_CAL_LLR(filePass_bpskComponents->bpskTransData, filePass_bpskComponents->bpskReceivedDataAddedAwgn, filePass_bpskComponents->bpskReceivedLLR->llr, 1.0, filePass_codeLength, filePass_EbN0); //need to add bitrate

            BPSK_Demod(filePass_errorComponents->erroredCodeWord->equation, filePass_bpskComponents->bpskReceivedDataAddedAwgn, filePass_codeLength);

            errorCount=0;
            for(i=0; i<filePass_codeLength; i++)        //채널 통과후
            {
                if( *(filePass_errorComponents->erroredCodeWord->equation+i) != *(filePass_encodingComponentInGaloisField->codeWord->equation+i) )
                {
                    errorCount+=1;
                }
            }
        }
        while(errorCount>filePass_selectedNumberOfSetOfConjugacy);


        filePass_decordingComponents=createBmAlgorithmComponent(filePass_galoisField->nonSorted, filePass_selectedNumberOfSetOfConjugacy, (char*)filePass_errorComponents->erroredCodeWord->equation, filePass_errorComponents->erroredCodeWord->usedLength);
        calculateBmAlgorithm(filePass_galoisField->nonSorted, filePass_decordingComponents);
        chienSearch_static(filePass_galoisField->nonSorted, filePass_decordingComponents->alpha, filePass_decordingComponents->errorLocationVector, &(filePass_decordingComponents->metricCheck) );
        errorCorrection(filePass_decordingComponents->codeWord, filePass_decordingComponents->errorLocationVector);
        unloadFromCodeWordStartAtHighSide(unloadStream, filePass_decordingComponents->codeWord);
        unloadFromCodeWordStartAtHighSide(noisedStream, filePass_errorComponents->erroredCodeWord);

        closeBmAlgorithmComponent(&filePass_decordingComponents);
    }

    closeEncodingComponent(&filePass_encodingComponentInGaloisField);
    closeBpskComponents(&filePass_bpskComponents);
    closeBmAlgorithmComponent(&filePass_decordingComponents);
    closeErrorComponent(&filePass_errorComponents);

    return 0;
}
