struct struct_generalForm_algorithmComponent{
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
}typedef struct_generalForm_algorithmComponent;

/* Constructor and Destructor */
struct_generalForm_algorithmComponent* recreateAlgoriehmComponent(
        struct_generalForm_algorithmComponent **p,
        struct_galoisFieldElements *galoisFields,
        unsigned int t, char *string,
        unsigned int stringLength,
        enum KIND_OF_BCH_DECODING_ALGORITHM KIND_OF_BCH_ALGORITHM
);
char closeAlgoriehmComponent(struct_generalForm_algorithmComponent **p, enum KIND_OF_BCH_DECODING_ALGORITHM KIND_OF_BCH_ALGORITHM);

/* Operation */
char calculateBCH_decodingAlgorithm(struct_galoisFieldElements *galoisFields, struct_generalForm_algorithmComponent *p);
