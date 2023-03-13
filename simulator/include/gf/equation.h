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
/*
 * Examples of expandRootOfLCM_usingEleOfGF() is below
 * f(x)=(x+a)(x+b)(x+c)(x+d)(x+e) = x^5 + A*x^4 + B*x^3 + C*x^2 + D*x^2 + E*x^1 + F
 */
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
