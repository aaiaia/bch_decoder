struct struct_summationMatrix
{
    unsigned int row;
    unsigned int column;
    struct_galoisFieldPolyForm ***element;
}typedef struct_summationMatrix;

struct_galoisFieldPolyForm **createListOfGaloisField(unsigned int lengthOfList);
char addLengthOfListOfGaloisField_VariableLength(struct_galoisFieldPolyForm *(**p), unsigned int lengthOfList, unsigned int addedLengthOfList);
char closeListOfGaloisField(struct_galoisFieldPolyForm ***p);
struct_galoisFieldPolyForm **recreateListOfGaloisField(struct_galoisFieldPolyForm ***p, unsigned int lengthOfList);
