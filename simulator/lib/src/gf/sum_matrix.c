#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "common/cmd_opt_vars.h"
#include "gf/element.h"
#include "gf/sum_matrix.h"

/* Constructor and Destructor */
struct_summationMatrix *createSummationMatrix_emptySpace(unsigned int row, unsigned int column)
{
    unsigned int i;
    struct_summationMatrix *p;

    #ifndef RELEASE
    if(!row)
    {
        return NULL;
    }
    if(!column)
    {
        return NULL;
    }
    if(row!=column)
    {
        return NULL;
    }
    #endif


    p=(struct_summationMatrix*)malloc(sizeof(struct_summationMatrix));

    p->element=(struct_galoisFieldPolyForm ***)malloc(sizeof(struct_galoisFieldPolyForm **)*row);
    for(i=0; i<row; i++)
    {
        (*(p->element+i))=(struct_galoisFieldPolyForm **)malloc(sizeof(struct_galoisFieldPolyForm *)*column);
    }

    p->row=row;
    p->column=column;
    return p;
}

char closeSummationMatrix(struct_summationMatrix **p)
{
    unsigned int i;
    if(!p)
    {
        return -1;
    }
    if(!(*p))
    {
        return -1;
    }
    #ifdef USING_OPEN_MP
    #pragma omp parallel for schedule(guided) private(i) shared(p)
    #endif
    for(i=0; i<(*p)->row; i++)
    {
        free(*((*p)->element+i));
    }
    free((*p)->element);

    free(*p);

    *p=NULL;
    return 0;
}

/* Functions making 'struct_summationMatrix' for list of elements that are 'struct_galoisFieldPolyForm' type */
/*
 *  !!! WARNING !!!
 *  Avoid to use functions are related with struct_galoisFieldPolyForm directly.
 *  As possible, use functions of struct_galoisFieldElements.
 */
struct_galoisFieldPolyForm **createListOfGaloisField(unsigned int lengthOfList)
{
    struct_galoisFieldPolyForm **p;

    #ifndef RELEASE
    if(!lengthOfList)
    {
        errorMes;
        printf("createListOfGaloisField lengthOfList is 0\n");
        return NULL;
    }
    #endif

    p=(struct_galoisFieldPolyForm**)malloc(sizeof(struct_galoisFieldPolyForm*)*lengthOfList);
    memset(p, 0, sizeof(struct_galoisFieldPolyForm*)*lengthOfList);

    return p;
}

/*
 * !!! WARNING !!!
 * Avoid to use functions are related with struct_galoisFieldPolyForm directly.
 * As possible, use functions of struct_galoisFieldElements.
 */
char addLengthOfListOfGaloisField_VariableLength(struct_galoisFieldPolyForm *(**p), unsigned int lengthOfList, unsigned int addedLengthOfList)
{

    struct_galoisFieldPolyForm **newGaloisFieldPolyForm=NULL;
    #ifndef RELEASE
    if(!(*p))
    {
        errorMes;
        printf("in addLengthOfListOfGaloisField_VariableLength, struct_galoisFieldPolyForm *(**p) is NULL\n");
        return -1;
    }
    if(!lengthOfList)
    {
        errorMes;
        printf("in addLengthOfListOfGaloisField_VariableLength, unsigned int lengthOfList is 0\n");
        return -1;
    }
    #endif


    #ifndef RELEASE
    if(!addedLengthOfList)
    {
        warningMes;
        printf("in addLengthOfListOfGaloisField_VariableLength, unsigned int addedLengthOfList is 0\n");
        return 0;
    }
    #endif



    /* Sometime realloc has memory leakage. So to increase memory size, using this func is invalid. */
    //p=(struct_galoisFieldPolyForm**)realloc(p, sizeof(struct_galoisFieldPolyForm*)*addedLengthOfList);
    newGaloisFieldPolyForm=(struct_galoisFieldPolyForm**)malloc(sizeof(struct_galoisFieldPolyForm*)*(lengthOfList+addedLengthOfList));
    memset(newGaloisFieldPolyForm+lengthOfList, 0, (sizeof(struct_galoisFieldPolyForm*)*(addedLengthOfList)));
    memcpy(newGaloisFieldPolyForm, (*p), (sizeof(struct_galoisFieldPolyForm*)*(lengthOfList)));
    free((*p));
    (*p)=newGaloisFieldPolyForm;

    return 0;
}


/*
 * !!! WARNING !!!
 * Avoid to use functions are related with struct_galoisFieldPolyForm directly.
 * As possible, use functions of struct_galoisFieldElements.
 */
char closeListOfGaloisField(struct_galoisFieldPolyForm ***p)
{
    #ifndef RELEASE
    if(!p)
    {
        errorMes;
        printf("closeListOfGaloisField ***p is NULL\n");
        return -1;
    }
    if(!(*p))
    {
        warningMes;
        printf("closeListOfGaloisField **p is NULL\n");
        return -1;
    }

    if(global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)
    {
        printf("in closeListOfGaloisField, struct_galoisFieldPolyForm p = \'0x%lx\'\n", (unsigned long)p);
        printf("in closeListOfGaloisField, struct_galoisFieldPolyForm *p = \'0x%lx\'\n", (unsigned long)*p);
    }
    #endif

    free (*p);
    *p=NULL;
    return 0;
}

/*
 * !!! WARNING !!!
 * Avoid to use functions are related with struct_galoisFieldPolyForm directly.
 * As possible, use functions of struct_galoisFieldElements.
 */
struct_galoisFieldPolyForm **recreateListOfGaloisField(struct_galoisFieldPolyForm ***p, unsigned int lengthOfList)
{
    #ifndef RELEASE
    if(p)
    {
        errorMes;
        printf("in recreateListOfGaloisField, struct_galoisFieldPolyForm ***p is NULL.\n");
        return NULL;
    }
    #endif
    if(*p)
    {
                #ifndef RELEASE
                if((global_flag_cmdOption&FLAG_MASK_PRINTF_LOG))
                {
                    printf("in createListOfGaloisField, struct_galoisFieldPolyForm **p is already have data.\n");
                    printf("**p will be closePowerFormPolynomial(p).\n");
                }
                #endif
        closeListOfGaloisField(p);
    }
    *p=createListOfGaloisField(lengthOfList);
    return *p;
}

char copyListOfGaloisField(struct_galoisFieldPolyForm **to, struct_galoisFieldPolyForm **from, unsigned int size)
{
    unsigned int i;

    #ifndef RELEASE
    if(!to)
    {
        errorMes;
        printf("in copyListOfGaloisField, struct_galoisFieldPolyForm **to is NULL.\n");
        return -1;
    }
    if(!from)
    {
        errorMes;
        printf("in copyListOfGaloisField, struct_galoisFieldPolyForm **from is NULL.\n");
        return -1;
    }
    if(!size)
    {
        errorMes;
        printf("in copyListOfGaloisField, unsigned int size is 0.\n");
        return -1;
    }

    if((global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)==FLAG_MASK_PRINTF_LOG)
    {
        logMes;
        printf("in copyListOfGaloisField, addr(to)=0x%lx, addr(from)=0x%lx\n", (unsigned long)to, (unsigned long)from);
    }
    #endif

    #ifdef USING_OPEN_MP
    #pragma omp parallel for schedule(guided) private(i) shared(size, to, from)
    #endif
    for(i=0; i<size; i++)
    {
        *(to+i)=*(from+i);
    }

    return 0;
}

char createGaloisFieldElementsAtList(struct_galoisFieldPolyForm **p, unsigned int lengthOfList, unsigned int lengthOfPolyForm)
{
    unsigned int i;

    #ifndef RELEASE
    if(!p)
    {
        errorMes;
        printf("in createGaloisFieldElementsAtList, struct_galoisFieldPolyForm **p is NULL.\n");
        return -1;
    }
    if(!lengthOfList)
    {
        errorMes;
        printf("in createGaloisFieldElementsAtList,  unsigned int lengthOfList is 0.\n");
        return -1;
    }
    if(!lengthOfPolyForm)
    {
        errorMes;
        printf("in createGaloisFieldElementsAtList, unsigned int lengthOfPolyForm is 0.\n");
        return -1;
    }
    #endif

    #ifdef USING_OPEN_MP
    #pragma omp parallel for schedule(guided) private(i) shared(lengthOfList, p, lengthOfPolyForm)
    #endif
    for(i=0; i<lengthOfList; i++)
    {
        *(p+i)=createPolyForm(lengthOfPolyForm);
    }
    return 0;
}

char closeGaloisFieldElementsAtList(struct_galoisFieldPolyForm ***p, unsigned int lengthOfList)
{
    unsigned int i;

    #ifndef RELEASE
    if(!p)
    {
        errorMes;
        printf("in closeGaloisFieldElementsAtList, struct_galoisFieldPolyForm ***p is NULL.\n");
        return -1;
    }
    if(!(*p))
    {
        warningMes;
        printf("in closeGaloisFieldElementsAtList, struct_galoisFieldPolyForm **p is NULL.\n");
        return -1;
    }
    if(!lengthOfList)
    {
        errorMes;
        printf("in closeGaloisFieldElementsAtList, unsigned int lengthOfList is 0.\n");
        return -1;
    }
    #endif

    #ifdef USING_OPEN_MP
    #pragma omp parallel for schedule(guided) private(i) shared(lengthOfList, p)
    #endif
    for(i=0; i<lengthOfList; i++)
    {
        closePolyForm(&(*((*p)+i)));
    }
    return 0;
}

char recreateGaloisFieldElementsAtList(struct_galoisFieldPolyForm **p, unsigned int lengthOfList, unsigned int lengthOfPolyForm)
{
    if(closeGaloisFieldElementsAtList(&p, (*p)->length))
    {
        warningMes;
        printf("in recreateGaloisFieldElementsAtList, closeGaloisFieldElementsAtList(&p, (*p)->length) is failed.\n");
    }
    createGaloisFieldElementsAtList(p, lengthOfList, lengthOfPolyForm);
    return 0;
}

struct_galoisFieldPolyForm **createListOfGaloisFieldAndComponents(unsigned int lengthOfList, unsigned int polyLength)
{
    //struct_galoisFieldPolyForm **p=(struct_galoisFieldPolyForm**)malloc(sizeof(struct_galoisFieldPolyForm*)*lengthOfList);
    //memset(p, 0, sizeof(struct_galoisFieldPolyForm*)*lengthOfList);
    struct_galoisFieldPolyForm **p=NULL;
    if(!(p=createListOfGaloisField(lengthOfList)))
    {
                #ifndef RELEASE
                errorMes; printf("in createListOfGaloisFieldAndComponents, fail that createListOfGaloisField(lengthOfList).\n");
                errorMes; printf("return value struct_galoisFieldPolyForm **p = 0x%lx\n", (unsigned long)p);
                #endif
        return NULL;
    }
    if(createGaloisFieldElementsAtList(p, lengthOfList, polyLength))
    {
        closeGaloisFieldElementsAtList(&p, lengthOfList);
        closeListOfGaloisField(&p);
                #ifndef RELEASE
                errorMes;    printf("in createListOfGaloisFieldAndComponents, fail that createGaloisFieldElementsAtList(p, lengthOfList, polyLength).\n");
                #endif
        return NULL;
    }

    return p;
}

char closeListOfGaloisFieldAndElements(struct_galoisFieldPolyForm ***p, unsigned int lengthOfList)
{
    if(closeGaloisFieldElementsAtList(p, lengthOfList))
    {
        #ifndef RELEASE
        errorMes; printf("in closeListOfGaloisFieldAndElements, fail close closeGaloisFieldElementsAtList(p, lengthOfList)\n");
        #endif
        return -1;
    }
    if(closeListOfGaloisField(p))
    {
                #ifndef RELEASE
                errorMes; printf("in closeListOfGaloisFieldAndElements, fail close closeListOfGaloisField(p)\n");
                #endif
        return -1;
    }
    *p=NULL;
    return 0;
}

/*
//struct_galoisFieldPolyForm **recreateListOfGaloisFieldAndComponents(struct_galoisFieldPolyForm ***p, unsigned int lengthOfList, unsigned int polyLength)
//{
//    if(*p)
//    {
//        if((global_flag_cmdOption&FLAG_MASK_PRINTF_LOG))
//        {
//            infoMes;
//            printf("in createListOfGaloisFieldAndComponents, struct_galoisFieldPolyForm *p have data already.\n");
//            printf("it will be recreated.\n");
//        }
//        closeListOfGaloisFieldAndComponents((*p)->length);
//    }
//    *p=createListOfGaloisFieldAndComponents(lengthOfList, polyLength);
//    return *p;
//}
*/


/* For displaying(debugging) */

