/* Reserved declare equation type */

char initEquation(struct_galoisFieldElements *galoisFields, struct_galoisFieldElements *p);
char initEquationFillAllZero(struct_galoisFieldElements *galoisFields, struct_galoisFieldElements *p);
char initBinEquationUsingStr(struct_galoisFieldElements *galoisFields, struct_galoisFieldElements *p, char *str, unsigned int strlen);

struct_galoisFieldElements* multipleTwoEqReturnAddr(
    struct_galoisFieldElements *galoisFields,
    struct_galoisFieldElements **result,
    struct_galoisFieldElements *operandEqA,
    struct_galoisFieldElements *operandEqB
);

char abstractOrderOfEquation(struct_galoisFieldElements *equation);
