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

/* For Dubugging(Displaying) */
void printSyndromeArray(struct_galoisFieldElements *p, struct_galoisFieldElements *galoisFields);
void printSyndromeSeriesWithTap(struct_galoisFieldElements *p, struct_galoisFieldElements *galoisFields);
void printSyndromeSeriesWithTapAndEnter(struct_galoisFieldElements *p, struct_galoisFieldElements *galoisFields);
