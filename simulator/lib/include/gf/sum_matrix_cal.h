#ifndef _GF_SUM_MATRIX_CAL_H_
#define _GF_SUM_MATRIX_CAL_H_

#include "gf/field.h"

char calculateSummationMatrix(struct_galoisFieldElements *galoisFields, struct_summationMatrix *p);
//struct_summationMatrix *createSummationMatrix(struct_galoisFieldElements *galoisFields, unsigned int row, unsigned int column);
struct_summationMatrix *createSummationMatrix(struct_galoisFieldElements *galoisFields, unsigned int in_row, unsigned int in_column);

/* FILE IO */
char save_struct_summationMatrix(char *path, char *primitivePoly, struct_summationMatrix *p);
char load_struct_summationMatrix(char *path, char *primitivePoly, struct_galoisFieldElements *galoisFields, struct_summationMatrix *p);
void printSummationMatrixSavedForm(struct_summationMatrix *p);

#endif
