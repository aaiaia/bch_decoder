#ifndef _GF_ELEMENT_H_
#define _GF_ELEMENT_H_

#include "gf/type.h"
/*
 * Elements of galois field have finite values are represented like 0110.
 * And if polynomial represented by power form have 15 bits length, length of elements of galois field is represented (15+1)/2.
 */
struct struct_galoisFieldPolyForm
{
    //unsigned int *bits;
    unsigned int length;
    char_GALOIS_FIELD_VALUE *value;
    u_int_GALOIS_FIELD_INTEGER intValue;
    u_int_GALOIS_FIELD_INTEGER index;
}typedef struct_galoisFieldPolyForm;

struct_galoisFieldPolyForm *createPolyForm(unsigned int length);
char closePolyForm(struct_galoisFieldPolyForm **p);
struct_galoisFieldPolyForm *recreatePolyForm(struct_galoisFieldPolyForm **p, unsigned int length);

/* Setter, Getter */
void inputValueToPolyForm(struct_galoisFieldPolyForm *p, char *string);

/* Convert */
unsigned int convertGaloisFieldPolyFormUnsignedInt(struct_galoisFieldPolyForm *p);

/* Comparing, Same -> True(1), Diff -> False(0) */
char checkValueFromPolyFormUsingGaloisFieldValue(struct_galoisFieldPolyForm *operandA, struct_galoisFieldPolyForm *operandB);
char checkValueFromPolyFormUsingGaloisFieldValueUsingIntValue_(struct_galoisFieldPolyForm *operandA, struct_galoisFieldPolyForm *operandB);
char checkAllValueUsingCharacterFromPolyForm(struct_galoisFieldPolyForm *p, char c);
char checkValueUsingStringFromPolyForm(struct_galoisFieldPolyForm *p, char *string);

/* For Display(Debugging) */
void printPolyFormWithEnter(struct_galoisFieldPolyForm *p);
void printPolyFormPure(struct_galoisFieldPolyForm *p);
void printPolyForm(struct_galoisFieldPolyForm *p);
void printPolyFormWithTapAndEnter(struct_galoisFieldPolyForm *p);
void printPolyFormWithTap(struct_galoisFieldPolyForm *p);

#endif
