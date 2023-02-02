/* Operation */
unsigned int cal_numsOfDegreeOfSynd(unsigned int t);
char syndromeArrayCalculatorUsingString(struct_galoisFieldElements *galoisFields, struct_galoisFieldElements *syndrome, unsigned int lengthOfSyndromeArray, char *string);
char summationSyndromeArray
(
    struct_galoisFieldElements *galoisFields,
    struct_galoisFieldElements *resultSyndrome,
    struct_galoisFieldElements *operandA,
    struct_galoisFieldElements *operandB
);
char allSyndromeIsZero(struct_galoisFieldElements *galoisFields, struct_galoisFieldElements *targetSyndrome);
