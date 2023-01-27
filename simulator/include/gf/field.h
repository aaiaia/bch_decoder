struct struct_galoisFieldElements
{
    unsigned int length;
    struct_galoisFieldPolyForm **element;

    struct_summationMatrix *summationReferMatrix;
}typedef struct_galoisFieldElements;

/* handling galois field except elements */
struct_galoisFieldElements *createGaloisFieldExceptElements(unsigned int numberOfElement);
struct_galoisFieldElements *createGaloisFieldExceptElements_VariableLength();
char addLengthOfGaloisFieldExceptElements_VariableLength(struct_galoisFieldElements *p, unsigned int addedLength);
char closeGaloisFieldExceptElements(struct_galoisFieldElements **p);
struct_galoisFieldElements *recreateGaloisFieldExceptElements(struct_galoisFieldElements **p, unsigned int numberOfElement);
/* handling galois field list and elements */
struct_galoisFieldElements *createGaloisFieldAndElements(unsigned int numberOfElement, unsigned int polyLength);
char closeGaloisFielsAndElements(struct_galoisFieldElements **p);
struct_galoisFieldElements *recreateGaloisFieldAndElements(struct_galoisFieldElements **p, unsigned int numberOfElement, unsigned int polyLength);
/* Constructor and Desctructor */
struct_galoisFieldElements *createGaloisField(struct_powerFormPolynomials *primitivePolynomial, unsigned int numberOfElement, unsigned int polyLength);
char closeGaloisField(struct_galoisFieldElements **p);
char calculateGaloisField(struct_powerFormPolynomials *primitivePolynomial, struct_galoisFieldElements *p);

/* File IO */
char save_struct_galoisFieldElements(char *path, char *primitivePoly, struct_galoisFieldElements *p);
char load_struct_galoisFieldElements(char *path, char *primitivePoly, struct_galoisFieldElements *p);

/* Converting */
/*
 * About. convertGaloisFielsAndElementsToStringOnlyZeroOrOne(...)
 * struct_galoisFieldElements type polynomial to binary(string)
 * The polynomial has only two coefficients, are a^0 or a^1, like f(x)=a^1*x^2+a^0*x^1+a^1*x^0
 * f(x)=a^1*x^2+a^1*x^1+a^0*x^0 => [MSB]110[LSB]
 */
char *convertGaloisFielsAndElementsToStringOnlyZeroOrOne(struct_galoisFieldElements *field, struct_galoisFieldElements *convertedPolynomial);

/* Operations */
struct_galoisFieldPolyForm *invertValueElementOfGaloisField(struct_galoisFieldElements *field, struct_galoisFieldPolyForm *p);//minus
unsigned int abstractIndexOfPowerFormInElementsOfGaloisField_finding(struct_galoisFieldElements *field, struct_galoisFieldPolyForm *p);
unsigned int abstractIndexOfPowerFormInElementsOfGaloisFieldUsingIndex(struct_galoisFieldElements *field, struct_galoisFieldPolyForm *p);
unsigned int abstractIndexOfPowerFormInElementsOfGaloisFieldUsingIntValue(struct_galoisFieldElements *field, struct_galoisFieldPolyForm *p);
struct_galoisFieldPolyForm *invertExponentialElementInGaloisField(struct_galoisFieldElements *field, struct_galoisFieldPolyForm *operand);
/*
 * About. summationElementsInGaloisFieldUsingGaloisFieldValue(...)
 * Use character, but it is slow, so this function is alternated.
 */
struct_galoisFieldPolyForm *summationElementsInGaloisFieldUsingGaloisFieldValue(struct_galoisFieldElements *field, struct_galoisFieldPolyForm *operandA, struct_galoisFieldPolyForm *operandB);
struct_galoisFieldPolyForm *summationElementsInGaloisFieldUsingGaloisFieldInt_finding(struct_galoisFieldElements *field, struct_galoisFieldPolyForm *operandA, struct_galoisFieldPolyForm *operandB);
struct_galoisFieldPolyForm *sumElementInGF_usingSumMatrixReturnAddr(struct_galoisFieldElements *field, struct_galoisFieldPolyForm *operandA, struct_galoisFieldPolyForm *operandB);
//unsigned int multiplicationWithExponential(struct_galoisFieldElements *field, unsigned int exponentialA, unsigned int exponentialB);
struct_galoisFieldPolyForm *multiElementsInGF_returnAddr(struct_galoisFieldElements *field, struct_galoisFieldPolyForm *operandA, struct_galoisFieldPolyForm *operandB);

/* Find Same elements */
struct_galoisFieldPolyForm *findSameElementOfGaloisField(struct_galoisFieldElements *field, struct_galoisFieldPolyForm *p);
struct_galoisFieldPolyForm *findSameElementOfGaloisFieldUsingString(struct_galoisFieldElements *field, char *string);

/* For displying(debugging) */
void printGaloisFieldSavedForm(struct_galoisFieldElements *p);
