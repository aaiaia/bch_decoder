struct struct_logLikeHoodRatio
{
    double_RATIONAL_NUMBER *llr;
    double_RATIONAL_NUMBER squreRootAvrLLR;

    s_int_QUANTIZ_DIGIT *quantizedLLR;
    s_int_QUANTIZ_DIGIT quantizedLLRMask;
    //s_int_QUANTIZ_DIGIT magnitudeMask;
    s_int_QUANTIZ_DIGIT *magnitude;//intently to use -1, defined signed value
    unsigned int length;
    unsigned int usedLength;

    u_int_QUANTIZ_MAGNITUDE_DIGIT *magnitudeShort;//magnitudeShort has length that is locatorLength.
    char* hardDecisionShort;
    unsigned char *locatorValidity;
    unsigned int *locator;//locator has length that is locatorLength.
    unsigned int locatorLength;

    struct struct_treeStructure *treeStruct;
}typedef struct_logLikeHoodRatio;

/* Constructor and Destructor */
struct_logLikeHoodRatio* createLogLikeHoodRatio(unsigned int length);
char closeLogLikeHoodRatio(struct_logLikeHoodRatio **p);

/* Initializer */
char initLogLikeHoodRatioLocator(struct_logLikeHoodRatio *p, unsigned int locatorLength);
/*
 * To find minumum value of magnitude, initializing values associate with tree in struct struct_logLikeHoodRatio.
 * Tree, can use, is define that is in enum treeTypes.
 * If you needs this list, find a enum treeTypes.
 */
char initializingVariablesAssociateWithTree(struct_logLikeHoodRatio *p, enum treeTypes treeType);

/* Configure */
char setUsedLengthOfLogLikeHoodRatio(struct_logLikeHoodRatio *p, unsigned int usedLength);
char setQuantizedLLRMaskOfLogLikeHoodRatio(struct_logLikeHoodRatio *p, unsigned int numberOfMask);

/* Operation */
double_RATIONAL_NUMBER getSqureRootAvrLLR(struct_logLikeHoodRatio *p);

/* Converter */
char convertQuantizedLLRToMagnitude(struct_logLikeHoodRatio *p);

/* Verificator */
char checkMinimumMagnitudeLocatorValidityLogLikeHoodRatio(
    struct_logLikeHoodRatio *p,
    struct_powerFormPolynomials *operandA,
    struct_powerFormPolynomials *operandB)

/* For displying(debugging) */
void printQuatizLLRWithSpaceAndEnter(struct_logLikeHoodRatio *p);
void printTestMagitudeQuatizLLRWithSpaceAndEnter(struct_logLikeHoodRatio *p);
void printLLRWithSpaceAndEnter(struct_logLikeHoodRatio *p);

char printMagnitudeOfLLRWithSpaceAndEnter(struct_logLikeHoodRatio *p);
char printMagnitudeOfLLR_andLocationWithSpaceAndEnter(struct_logLikeHoodRatio *p);

void testPrintLLRWithSpaceAndEnter(struct_logLikeHoodRatio *p);

void testPrintShortMinimumQuantizedLLRAndLocator(struct_logLikeHoodRatio *p);

/* File IO */
char fprintQuatizLLR_toHEX(FILE* fp, struct_logLikeHoodRatio* p, char* str_tail);
char fprintVerilogQuatizLLR_toHEX(FILE* fp, struct_logLikeHoodRatio* p, char* str_tail);
char fprintQuatizLLR_fullDescriptionToHEX(FILE* fp, struct_logLikeHoodRatio* p, char* str_tail);

char fprintMagnitudeOfQuantizedLLR_toHex(FILE *fp, struct_logLikeHoodRatio *p, char *str_tail);
char fprintVerilogMagnitudeOfQuantizedLLR_toHex(FILE *fp, struct_logLikeHoodRatio *p, char *str_tail);
char fprintMagnitudeOfQuantizedLLR_fullDescription_toHex(FILE *fp, struct_logLikeHoodRatio *p, char *str_tail);
