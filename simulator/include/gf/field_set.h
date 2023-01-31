struct struct_galoisField_info
{
    //unsigned int *gfBits;
    //unsigned int length;

    struct_powerFormPolynomials *primitivePolynomial;

    struct_galoisFieldElements *nonSorted;
    struct_galoisFieldElements *sorted;

    struct_setOfGaloisFieldElements *conjugacies;
}typedef struct_galoisField_info;

/* Constructor and Desctructor */
struct_galoisField_info *createGaloisField_info_emptySpace(struct_powerFormPolynomials *primitivePolynomial);
//struct_galoisField_info *createGaloisField(struct_powerFormPolynomials *primitivePolynomial);
//struct_galoisField_info *createGaloisField_deleted_201604251820(struct_powerFormPolynomials *primitivePolynomial);
struct_galoisField_info *createGaloisFieldAndSumMatrix(struct_powerFormPolynomials *primitivePolynomial);
