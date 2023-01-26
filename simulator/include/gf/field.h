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
