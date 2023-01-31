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

