struct struct_encodingComponentInGF
{
    struct_powerFormPolynomials *codeWord;
    //struct_powerFormPolynomials *encodedCodeWord;
    //struct_powerFormPolynomials *receivedCodeWord;
    //struct_powerFormPolynomials *errorCodeWord;
    //struct_powerFormPolynomials *erroredCodeWord;

    unsigned int minimumDistance;
    unsigned int parityBitsLength;//shifted....

    char_POWER_FORM *generationPolynomial;
}typedef struct_encodingComponentInGF;

/* Constructor and Destructor */
struct_encodingComponentInGF *createEncodingComponent(unsigned int length, unsigned int numberOfCorrectableBit, char_POWER_FORM *generationPolynomial);
char closeEncodingComponent(struct_encodingComponentInGF **p);
struct_encodingComponentInGF *recreateEncodingComponent(
        struct_encodingComponentInGF **p,
        unsigned int length,
        unsigned int numberOfCorrectableBit,
        char_POWER_FORM *generationPolynomial
);
struct_encodingComponentInGF *createEncodingComponentAndInputValue(
        unsigned int length,
        unsigned int numberOfCorrectableBit,
        char_POWER_FORM *generationPolynomial,
        char *value
);
struct_encodingComponentInGF *recreateEncodingComponentAndInputValue(
        struct_encodingComponentInGF **p,
        unsigned int length,
        unsigned int numberOfCorrectableBit,
        char_POWER_FORM *generationPolynomial,
        char *value
);
