#ifndef _GF_FIELD_SET_H_
#define _GF_FIELD_SET_H_

#include "gf/conjugate.h"
#include "gf/field.h"
#include "poly/polynomial.h"

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

/* For Debugging(Displaying) */
#define PRINT_FIELD_GALOIS_FIELD_TITLE      1
#define PRINT_FIELD_SYNDROME_TITLE          2
#define PRINT_FIELD_EQUATION_TITLE          3
#define PRINT_FIELD_PRINT_OPTION_NUMBERING  (1<<1)

void printGaloisField(struct_galoisFieldElements *p);
void printGaloisFieldToCsv(struct_galoisFieldElements *p);
void printGaloisField2(struct_galoisFieldElements *baseGaloisField, struct_galoisFieldElements *printedField, unsigned int title, unsigned int options);

#endif
