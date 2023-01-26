struct struct_galoisFieldElements
{
    unsigned int length;
    struct_galoisFieldPolyForm **element;

    struct_summationMatrix *summationReferMatrix;
}typedef struct_galoisFieldElements;

struct_galoisFieldElements *createGaloisFieldExceptElements(unsigned int numberOfElement);
struct_galoisFieldElements *createGaloisFieldExceptElements_VariableLength();

/* handling galois field except elements */
char addLengthOfGaloisFieldExceptElements_VariableLength(struct_galoisFieldElements *p, unsigned int addedLength);
char closeGaloisFieldExceptElements(struct_galoisFieldElements **p);
struct_galoisFieldElements *recreateGaloisFieldExceptElements(struct_galoisFieldElements **p, unsigned int numberOfElement);
