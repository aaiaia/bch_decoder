#ifndef _DECODER_HARD_DECISION_ALGO_BM_H_
#define _DECODER_HARD_DECISION_ALGO_BM_H_

#include "common/cmd_opt_vars.h"
#include "gf/field.h"
#include "poly/polynomial.h"

struct struct_HD_BM_algorithmComponent{
    enum KIND_OF_BCH_DECODING_ALGORITHM KIND_OF_BCH_ALGORITHM;

    struct_powerFormPolynomials *codeWord;
    struct_powerFormPolynomials *errorLocationVector;

    struct_galoisFieldElements *syndrome;

    struct_galoisFieldElements *errLocPoly;//it is same with struct_galoisFieldElements *alpha;

    //struct_powerFormPolynomials *errorCorectableCodeWord;
    //struct_powerFormPolynomials *correctedCodeWord;


    unsigned int numberOfCorrection;//correction number t;

    unsigned int vMAX;

    int degreeOfErrLocPoly;

    unsigned int metricCheck;

    unsigned int value_r;

    unsigned char calculationSelector;

    struct_galoisFieldPolyForm *delta;//just pointer, not create a anything.

    struct_galoisFieldElements *alpha;
    struct_galoisFieldElements *beforeAlpha;
    struct_galoisFieldElements *beta;
    struct_galoisFieldElements *beforeBeta;

    unsigned int lengthOfStage;

    /* to test ,soft decision */
    struct_galoisFieldElements *errLocSyndrome;
    struct_galoisFieldElements *sumReceiveAndErrLocSyndrome;

}typedef struct_HD_BM_algorithmComponent;

/* Constructor and Destructor */
struct_HD_BM_algorithmComponent *createBmAlgorithmComponent(struct_galoisFieldElements *galoisFields, unsigned int t, char *string, unsigned int stringLength);
char closeBmAlgorithmComponent(struct_HD_BM_algorithmComponent **p);
struct_HD_BM_algorithmComponent *recreateBmAlgorithmComponent(struct_HD_BM_algorithmComponent **p, struct_galoisFieldElements *galoisFields, unsigned int t, char *string, unsigned int stringLength);

/* Operation */
unsigned char calculateCalculationSelector(struct_galoisFieldPolyForm *delta, unsigned int value_r, unsigned int beforeLengthOfStage);
struct_galoisFieldPolyForm *calculcateDeltaInBmAlgorithm
(
    struct_galoisFieldElements *galoisFields, struct_galoisFieldElements *syndrome,
    struct_galoisFieldElements *beforeStageAlpha, unsigned int lengthOfStage,
    unsigned int value_r
);
char calculateAlphaInBmAlgorithm
(
    struct_galoisFieldElements *beforeAlpha, struct_galoisFieldElements *beforeBeta,
    struct_galoisFieldPolyForm *delta, struct_galoisFieldElements *galoisFields,
    struct_galoisFieldElements *alpha
);
char calculateBetaInBmAlgorithm
(
    struct_galoisFieldElements *beforeAlpha, struct_galoisFieldElements *beforeBeta,
    struct_galoisFieldPolyForm *delta, struct_galoisFieldElements *galoisFields,
    unsigned char calculationSelector, struct_galoisFieldElements *beta
);
unsigned int calculateBmAlgorithmLengthOfStage(unsigned int value_r, unsigned int lengthOfStage, unsigned char calculationSelector);
char calculateBmAlgorithm(struct_galoisFieldElements *galoisFields, struct_HD_BM_algorithmComponent *p);

#endif
