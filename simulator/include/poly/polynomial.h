/* power form poly is mean that bit stream like 0100011000100. */
struct struct_powerFormPolynomials
{
    //unsigned int *numberOfelement;
    unsigned int length;
    unsigned int usedLength;
    char_POWER_FORM *equation;
}typedef struct_powerFormPolynomials;

struct_powerFormPolynomials *createPowerFormPolynomial(unsigned int length);
struct_powerFormPolynomials *createPowerFormPolynomialUsingString(char *string, unsigned int strLength);
char closePowerFormPolynomial(struct_powerFormPolynomials **p);
struct_powerFormPolynomials *recreatePowerFormPolynomial(struct_powerFormPolynomials **p, unsigned int length);
char setPowerFormPolynomialUsingStringAddr(struct_powerFormPolynomials *p, char *string, unsigned int usedLength, unsigned int length);
char setPowerFormPolynomialUsingStringAddr_clearStringPointer(struct_powerFormPolynomials *p, char **string, unsigned int usedLength, unsigned int length);
char copyPowerFormPolynomialFromString(struct_powerFormPolynomials *p, char *string);
//struct_powerFormPolynomials *createPowerFormPolynomialAndInputValue(char *string);
struct_powerFormPolynomials *recreatePowerFormPolynomialUsingString(struct_powerFormPolynomials **p, char *string, unsigned int strLength);

/* About polynomial operation */
void shiftHighSidePowerFormPolynomial(struct_powerFormPolynomials *p, unsigned int length);
void shiftLowSidePowerFormPolynomial(struct_powerFormPolynomials *p, unsigned int length);
unsigned int calculateHammingWeightPowerFormPolynomial(struct_powerFormPolynomials *p);
unsigned int calculateHammingWeightFromDiffentPowerFormPolynomial(struct_powerFormPolynomials *p1, struct_powerFormPolynomials *p2);

/* For Display(Debugging) */
void printPowerFormWithEnterPolynomialAndInfoUsingAddress(struct_powerFormPolynomials *p);
void printPowerFormWithEnterPolynomialWithEnterUsingAddress(struct_powerFormPolynomials *p);
void printPowerFormWithEnterPolynomialWithTapUsingAddress(struct_powerFormPolynomials *p);
void printPowerFormExclusivedUsingAddress(struct_powerFormPolynomials *p1, struct_powerFormPolynomials *p2);

/* file IO */
void fprintPowerFormUsingAddress(FILE *fp, struct_powerFormPolynomials *p, char* str_tail);
void fprintVerilogPowerFormUsingAddress(FILE *fp, struct_powerFormPolynomials *p, char* str_tail);
void fprintPowerFormFullDescriptionUsingAddresss(FILE *fp, struct_powerFormPolynomials *p, char *str_tail);
