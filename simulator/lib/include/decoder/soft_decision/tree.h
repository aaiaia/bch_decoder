#ifndef _DECODER_SOFT_DECISION_TREE_H_
#define _DECODER_SOFT_DECISION_TREE_H_

#include "decoder/soft_decision/quantiz.h"

enum treeTypes
{
    TREE_NONE,
    TREE_CHASE
}typedef enum_treeTypes;

struct struct_treeStructure
{
    /*
     * Tree that is triple pointer indicate magnitudeShort.
     * And it is using when find minimum magnitudeShort.
     */
    enum_treeTypes treeType;
    u_int_QUANTIZ_MAGNITUDE_DIGIT*** tree_mag;
    char*** tree_hd;
    unsigned char** direction;
    unsigned int** treeIndex;

    unsigned int* treeLength;
    unsigned int treeDepth;

}typedef struct_treeStructure;

/* Constructor and Destructor */
struct_treeStructure* createTreeStruct(unsigned int zeroStageLength, enum_treeTypes treeType);
char closeTreeStruct(struct_treeStructure **p);

/* For Display(Debugging) */
void testTreeStruct(struct_treeStructure *p);
void testPrintTreeStructIndicateAddr(struct_treeStructure *p);
void testPrintTreeStructValue(struct_treeStructure *p);
void testPrintValueOfTreeStruct(struct_treeStructure *p);

#endif
