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
char abstractOrderOfEquation(struct_galoisFieldElements *equation);
