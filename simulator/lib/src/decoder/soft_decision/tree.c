#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "common/cmd_opt_vars.h"
#include "decoder/soft_decision/tree.h"

/* Constructor and Destructor */
struct_treeStructure* createTreeStruct(unsigned int zeroStageLength, enum treeTypes treeType)
{
    double tmp_log;
    unsigned int tmp_treeDepth;
    unsigned int tmp_treeLength;

    unsigned int i;

    struct_treeStructure *p;

    if(!zeroStageLength)
    {
        warningMes;
        printf("in createTreeStruct, zeroStageLength is zero.\n");
        return NULL;
    }
    if(treeType == TREE_NONE)
    {
        warningMes;
        printf("in createTreeStruct, enum treeTypes treeType is TREE_NONE.\n");
        warningMes;
        printf("can't create structure of tree.\n");
        return NULL;
    }

    p=(struct_treeStructure*)malloc(sizeof(struct_treeStructure));
    memset(p, 0, sizeof(struct_treeStructure));

    switch(treeType)
    {
        case TREE_CHASE:
            /* calculate depth of tree */
            tmp_log = (log10(zeroStageLength)/log10(2));
            tmp_treeDepth = (unsigned int)tmp_log;

            if((tmp_log-((double)tmp_treeDepth))==0.0)    tmp_treeDepth--;

            tmp_treeDepth++;//To store smallest loc and llr magnitudeShort
            p->treeDepth = tmp_treeDepth;

            p->treeLength = (unsigned int*)malloc(sizeof(unsigned int)*tmp_treeDepth);
            memset(p->treeLength, 0, sizeof(unsigned int)*tmp_treeDepth);

            p->tree_mag = (u_int_QUANTIZ_MAGNITUDE_DIGIT***)malloc(sizeof(u_int_QUANTIZ_MAGNITUDE_DIGIT**)*tmp_treeDepth);
            memset(p->tree_mag, 0, sizeof(u_int_QUANTIZ_MAGNITUDE_DIGIT**)*tmp_treeDepth);
            p->tree_hd= (char***)malloc(sizeof(char**)*tmp_treeDepth);
            memset(p->tree_hd, 0, sizeof(char**)*tmp_treeDepth);

            p->direction=(unsigned char **)malloc(sizeof(char*)*tmp_treeDepth);
            memset(p->direction, 0, sizeof(char*)*tmp_treeDepth);
            p->treeIndex = (unsigned int**)malloc(sizeof(unsigned int*)*tmp_treeDepth);
            memset(p->treeIndex, 0, sizeof(unsigned int*)*tmp_treeDepth);



            tmp_treeLength = zeroStageLength;

            for(i=0; i<tmp_treeDepth; i++)
            {
                if((tmp_treeLength%2)!=0)//having redundant
                {
                    tmp_treeLength=((tmp_treeLength/2)+1);
                }
                else
                {
                    tmp_treeLength/=2;
                }
                *(p->tree_mag+i) = (u_int_QUANTIZ_MAGNITUDE_DIGIT**)malloc(sizeof(u_int_QUANTIZ_MAGNITUDE_DIGIT*)*tmp_treeLength);
                memset(*(p->tree_mag+i), 0, sizeof(u_int_QUANTIZ_MAGNITUDE_DIGIT*)*tmp_treeLength);
                *(p->tree_hd+i) = (char**)malloc(sizeof(char*)*tmp_treeLength);
                memset(*(p->tree_hd+i), 0, sizeof(char*)*tmp_treeLength);
                *(p->direction+i) = (unsigned char*)malloc(sizeof(unsigned char)*tmp_treeLength);
                memset(*(p->direction+i), 0, sizeof(unsigned char)*tmp_treeLength);
                *(p->treeIndex+i) = (unsigned int*)malloc(sizeof(unsigned int)*tmp_treeLength);
                memset(*(p->treeIndex+i), -1, sizeof(unsigned int)*tmp_treeLength);

                *(p->treeLength+i) = tmp_treeLength;//store treeLength
            }
            break;

        case TREE_NONE:
        default:
            free(p);
            return NULL;
    }
    p->treeType=treeType;

    return p;
}

char closeTreeStruct(struct_treeStructure **p)
{
    unsigned int i;

    if(!(*p))
    {
        warningMes;
        printf("in closeTreeStruct, struct_treeStructure (*p) is NULL.\n");
        warningMes;
        printf("Already struct_treeStructure is removed.");
        return -1;
    }
    if(!((*p)->tree_mag))
    {
        warningMes;
        printf("in closeTreeStruct, struct_treeStructure (*p)->tree_mag is NULL.\n");
        return -1;
    }
    if(!((*p)->treeLength))
    {
        warningMes;
        printf("in closeTreeStruct, struct_treeStructure (*p)->treeLength is NULL.\n");
        return -1;
    }
    if(!((*p)->treeDepth))
    {
        warningMes;
        printf("in closeTreeStruct, struct_treeStructure (*p)->treeDepth is NULL.\n");
        return -1;
    }

    switch((*p)->treeType)
    {
        case TREE_CHASE:
            free((*p)->treeLength);

            for(i=0; i<(*p)->treeDepth; i++)
            {
                free(*(((*p)->tree_mag)+i));
                free(*(((*p)->tree_hd)+i));
                free(*(((*p)->direction)+i));
                free(*(((*p)->treeIndex)+i));
            }
            free((*p)->tree_mag);
            free((*p)->tree_hd);
            free((*p)->direction);
            free((*p)->treeIndex);
            break;

        case TREE_NONE:
        default:
            break;
    }
    free((*p));
    (*p)=NULL;

    return 0;
}

/* For Display(Debugging) */
void testTreeStruct(struct_treeStructure *p)
{
    debugMes;
    printf("p->treeType = %d\n", p->treeType);

    if(!p)
    {
        warningMes;
        printf("in testTreeStruct, struct_treeStructure p is NULL.\n");
        warningMes;
        printf("Already struct_treeStructure is removed.");
        return;
    }
    debugMes;
    printf("p = 0x%lx\n", (unsigned long)p);

    if(!(p->tree_mag))
    {
        warningMes;
        printf("in testTreeStruct, struct_treeStructure p->tree_mag is NULL.\n");
        return;
    }
    if(!(p->tree_hd))
    {
        warningMes;
        printf("in testTreeStruct, struct_treeStructure p->tree_hd is NULL.\n");
        return;
    }
    debugMes;
    printf("p->tree_mag = 0x%lx, p->tree_hd = 0x%lx\n", (unsigned long)(p->tree_mag), (unsigned long)(p->tree_hd));

    if(!(p->treeIndex))
    {
        warningMes;
        printf("in testTreeStruct, struct_treeStructure p->treeIndex is NULL.\n");
        return;
    }
    debugMes;
    printf("p->treeIndex = 0x%lx\n", (unsigned long)(p->treeIndex));

    if(!(p->treeLength))
    {
        warningMes;
        printf("in testTreeStruct, struct_treeStructure p->treeLength is NULL.\n");
        return;
    }
    debugMes;
    printf("p->treeLength = 0x%lx\n", (unsigned long)(p->treeLength));

    if(!(p->treeDepth))
    {
        warningMes;
        printf("in testTreeStruct, struct_treeStructure p->treeDepth is NULL.\n");
        return;
    }
    debugMes;
    printf("p->treeDepth = %d\n", p->treeDepth);
}

void testPrintTreeStructIndicateAddr(struct_treeStructure *p)
{
    unsigned int i,j;
    if(!p)
    {
        errorMes;
        printf("in testPrintTreeStructIndicateAddr, p is NULL.\n");
        return;
    }
    if(!(p->treeType))
    {
        errorMes;
        printf("in testPrintTreeStructIndicateAddr, p->treeType is ZERO(TREE_NONE).\n");
        return;
    }
    if(!(p->tree_mag))
    {
        errorMes;
        printf("in testPrintTreeStructIndicateAddr, p->tree_mag is NULL.\n");
        return;
    }
    if(!(p->tree_hd))
    {
        errorMes;
        printf("in testPrintTreeStructIndicateAddr, p->tree_hd is NULL.\n");
        return;
    }
    if(!(p->treeLength))
    {
        errorMes;
        printf("in testPrintTreeStructIndicateAddr, p->treeLength is NULL.\n");
        return;
    }
    if(!(p->treeDepth))
    {
        errorMes;
        printf("in testPrintTreeStructIndicateAddr, p->treeDepth is ZERO.\n");
        return;
    }

    printf("/********** struct_treeStructure->tree printf address test **********/\n");
    debugMes; printf("treeStruct pointer : 0x%lx\n", (unsigned long)p);
    debugMes; printf("root pointer : p->tree_mag = 0x%lx, p->tree_hd = 0x%lx\n",
        (unsigned long)(p->tree_mag), (unsigned long)(p->tree_hd));
    for(i=0; i<p->treeDepth; i++)
    {
        debugMes; printf("%d-stage tree_mag pointer = 0x%lx, tree_hd pointer = 0x%lx\n",
                i, (unsigned long)(*(p->tree_mag+i)), (unsigned long)(*(p->tree_hd+i)));
        debugMes; printf("%d-stage length = %d\n",i, *(p->treeLength+i));
        debugMes; printf("%d-stage tree_mag and tree_hd => ", i);
        for(j=0; j<(*(p->treeLength+i)); j++)
        {
            printf("0x%lx, 0x%lx(%u) ",
                (unsigned long)(*((*(p->tree_mag+i))+j)),
                (unsigned long)(*((*(p->tree_hd+i))+j)),
                *((*(p->treeIndex+i))+j));
        }
        printf("\n");
    }
}

void testPrintTreeStructValue(struct_treeStructure *p)
{
    unsigned int i,j;
    if(!p)
    {
        errorMes;
        printf("in testPrintTreeStructIndicateAddr, p is NULL.\n");
        return;
    }
    if(!(p->treeType))
    {
        errorMes;
        printf("in testPrintTreeStructIndicateAddr, p->treeType is ZERO(TREE_NONE).\n");
        return;
    }
    if(!(p->tree_mag))
    {
        errorMes;
        printf("in testPrintTreeStructIndicateAddr, p->tree_mag is NULL.\n");
        return;
    }
    if(!(p->tree_hd))
    {
        errorMes;
        printf("in testPrintTreeStructIndicateAddr, p->tree_hd is NULL.\n");
        return;
    }
    if(!(p->treeLength))
    {
        errorMes;
        printf("in testPrintTreeStructIndicateAddr, p->treeLength is NULL.\n");
        return;
    }
    if(!(p->treeDepth))
    {
        errorMes;
        printf("in testPrintTreeStructIndicateAddr, p->treeDepth is ZERO.\n");
        return;
    }

    printf("/********** struct_treeStructure->tree_mag, tree_hd, index and direction printf value test **********/\n");
    for(i=0; i<p->treeDepth; i++)
    {
        printf("depth:%d\t", i);
        for(j=0; j<(*(p->treeLength+i)); j++)
        {
            printf("%d,'%c',%u,%c ",
                *(*((*(p->tree_mag+i))+j)),
                *(*((*(p->tree_hd+i))+j)),
                *((*(p->treeIndex+i))+j),
                *((*(p->direction+i))+j)?'R':'L');
        }
        printf("\n");
    }
}

void testPrintValueOfTreeStruct(struct_treeStructure *p)
{
    unsigned int i,j;
    if(!p)
    {
        errorMes;
        printf("in testPrintTreeStructIndicateAddr, p is NULL.\n");
        return;
    }
    if(!(p->treeType))
    {
        errorMes;
        printf("in testPrintTreeStructIndicateAddr, p->treeType is ZERO(TREE_NONE).\n");
        return;
    }
    if(!(p->tree_mag))
    {
        errorMes;
        printf("in testPrintTreeStructIndicateAddr, p->tree_mag is NULL.\n");
        return;
    }
    if(!(p->tree_hd))
    {
        errorMes;
        printf("in testPrintTreeStructIndicateAddr, p->tree_hd is NULL.\n");
        return;
    }
    if(!(p->treeLength))
    {
        errorMes;
        printf("in testPrintTreeStructIndicateAddr, p->treeLength is NULL.\n");
        return;
    }
    if(!(p->treeDepth))
    {
        errorMes;
        printf("in testPrintTreeStructIndicateAddr, p->treeDepth is ZERO.\n");
        return;
    }

    printf("/********** printf tree_mag and tree_hd value(tree Type : %d) **********/\n", p->treeType);
    for(i=0; i<p->treeDepth; i++)
    {
        printf("%d-stage\tLength = %d\n", i, *(p->treeLength+i));
        printf("[Values],[HD](index)\t");
        for(j=0; j<(*(p->treeLength+i)); j++)
        {
            printf("%d, '%c'(%d)\t",
                **(*(p->tree_mag+i)+j),
                **(*(p->tree_hd+i)+j),
                *(*(p->treeIndex+i)+j));
        }
        printf("\n");
    }
}
