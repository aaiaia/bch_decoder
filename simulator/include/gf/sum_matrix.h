struct struct_summationMatrix
{
    unsigned int row;
    unsigned int column;
    struct_galoisFieldPolyForm ***element;
}typedef struct_summationMatrix;

/* Constructor and Destructor */
struct_summationMatrix *createSummationMatrix_emptySpace(unsigned int row, unsigned int column)
char closeSummationMatrix(struct_summationMatrix **p)

/* Functions making 'struct_summationMatrix' for list of elements that are 'struct_galoisFieldPolyForm' type */
struct_galoisFieldPolyForm **createListOfGaloisField(unsigned int lengthOfList);
char addLengthOfListOfGaloisField_VariableLength(struct_galoisFieldPolyForm *(**p), unsigned int lengthOfList, unsigned int addedLengthOfList);
char closeListOfGaloisField(struct_galoisFieldPolyForm ***p);
struct_galoisFieldPolyForm **recreateListOfGaloisField(struct_galoisFieldPolyForm ***p, unsigned int lengthOfList);
