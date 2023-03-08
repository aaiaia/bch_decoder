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

/* Configure */
char setUsedLengthOfLogLikeHoodRatio(struct_logLikeHoodRatio *p, unsigned int usedLength);
char setQuantizedLLRMaskOfLogLikeHoodRatio(struct_logLikeHoodRatio *p, unsigned int numberOfMask);

/* Operation */
double_RATIONAL_NUMBER getSqureRootAvrLLR(struct_logLikeHoodRatio *p);
