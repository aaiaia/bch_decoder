/* Reserved declare equation type */

/* Constructor and Destructor */
char initEquation(struct_galoisFieldElements *galoisFields, struct_galoisFieldElements *p);
char initEquationFillAllZero(struct_galoisFieldElements *galoisFields, struct_galoisFieldElements *p);
char initBinEquationUsingStr(struct_galoisFieldElements *galoisFields, struct_galoisFieldElements *p, char *str, unsigned int strlen);

/* Operation */
struct_galoisFieldElements* multipleTwoEqReturnAddr(
    struct_galoisFieldElements *galoisFields,
    struct_galoisFieldElements **result,
    struct_galoisFieldElements *operandEqA,
    struct_galoisFieldElements *operandEqB
);

/* etc */
unsigned int checkDegreePolynomials(struct_galoisFieldElements *galoisFields, struct_galoisFieldElements *targetPolynomial);

/* Least Common Multiple(LCM) expantion */
char abstractOrderOfEquation(struct_galoisFieldElements *equation);
struct_galoisFieldElements *expandRootOfLCM_usingEleOfGF
(

    struct_galoisFieldElements *galoisFields,
    struct_galoisFieldElements *equation,
    struct_setOfGaloisFieldElements *multipledRootSet,
    unsigned int numberOfSelectedSet//numberOfSelectedSet is same that number of correctable bit
);
struct_galoisFieldElements *test_expandLimitedLeastCommonMultipleUsingElementOfGaloisFieldToRoot
(
    struct_galoisFieldElements *galoisFields,
    struct_galoisFieldElements *equation,
    struct_setOfGaloisFieldElements *multipledRootSet,
    unsigned int numberOfSelectedSet//numberOfSelectedSet is same that number of correctable bit
);
