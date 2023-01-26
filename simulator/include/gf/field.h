struct struct_galoisFieldElements
{
    unsigned int length;
    struct_galoisFieldPolyForm **element;

    struct_summationMatrix *summationReferMatrix;
}typedef struct_galoisFieldElements;

struct_galoisFieldElements *createGaloisFieldExceptElements(unsigned int numberOfElement);
struct_galoisFieldElements *createGaloisFieldExceptElements_VariableLength();
