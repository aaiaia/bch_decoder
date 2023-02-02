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
