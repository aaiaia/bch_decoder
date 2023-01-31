/*
 * note about struct_setOfGaloisFieldElements.
 * test. 2016/02/22/14:28, limitedExponential is mean that, when getting conjugacies class, limit exponential of galois fields.
 */
struct struct_setOfGaloisFieldElements
{
    unsigned int length;
    struct_galoisFieldElements **conjugateSet;

    unsigned int limitedExponential;

    struct_galoisFieldElements **limitedConjugateSet;
}typedef struct_setOfGaloisFieldElements;

/* Constructor and Destructor */
struct_setOfGaloisFieldElements *createConjugacyClasses(struct_galoisFieldElements *galoisFields);
struct_setOfGaloisFieldElements *createConjugacyClasses_staticSetNums(unsigned int setLength, unsigned int *setElementLength);
struct_setOfGaloisFieldElements *createConjugacyClasses_VariableLength();
char closeConjugacyClasses(struct_setOfGaloisFieldElements **p);
struct_setOfGaloisFieldElements *recreateConjugacyClasses(struct_setOfGaloisFieldElements **p, struct_galoisFieldElements *galoisFields);

/* Configure */
char setLengthOfLimittedLengthOfConjugacyClasse(struct_setOfGaloisFieldElements *p, unsigned int limitedExponential);
char addLengthOfConjugacySet_VariableLength(struct_galoisFieldElements *galoisFields, struct_setOfGaloisFieldElements *p, unsigned int addedLength);

/* Operation */
char calculateConjugacyClasses(struct_galoisFieldElements *galoisFields, struct_setOfGaloisFieldElements *p);
char calculateConjugacyClasses_VariableLength(struct_galoisFieldElements *galoisFields, struct_setOfGaloisFieldElements *p);

/* Initalizer */
char init_ConjugacyClasses(struct_galoisFieldElements *galoisFields, struct_setOfGaloisFieldElements **p, unsigned int limitedExponential);

/* file IO */
char save_struct_setOfGaloisFieldElements(char *path, char *primitivePoly, struct_setOfGaloisFieldElements *p);
char load_struct_setOfGaloisFieldElements(char *path, char *primitivePoly, struct_galoisFieldElements *galoisFields, struct_setOfGaloisFieldElements **p);
void print_setOfGaloisFieldElementsSavedForm(struct_setOfGaloisFieldElements *p);

/* for Debugging(displaying) */
void printConjugacyClasses(struct_galoisFieldElements *galoisFields, struct_setOfGaloisFieldElements *p);
