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
