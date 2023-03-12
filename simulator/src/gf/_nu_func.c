#include "field.h"
char copyListOfGaloisFieldIngaloisFieldElements(struct_galoisFieldElements *galoisFields, struct_galoisFieldElements *to, struct_galoisFieldElements *from)
{
    unsigned int i;

    #ifndef RELEASE
    if(!to)
    {
        errorMes;
        printf("in copyListOfGaloisFieldIngaloisFieldElements, struct_galoisFieldPolyForm **to is NULL.\n");
        return -1;
    }
    if(!from)
    {
        errorMes;
        printf("in copyListOfGaloisFieldIngaloisFieldElements, struct_galoisFieldPolyForm **from is NULL.\n");
        return -1;
    }
    if((to->length)<(from->length))
    {
        errorMes;
        printf("in copyListOfGaloisFieldIngaloisFieldElements, to->length(=\'%d\') is shorter than from->length(=\'%d\').\n", (to->length), (from->length));
        return -1;
    }
    #endif

    #ifndef RELEASE
    if((global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)==FLAG_MASK_PRINTF_LOG)
    {
        logMes;
        printf("in copyListOfGaloisFieldIngaloisFieldElements, addr(to)=0x%lx, addr(from)=0x%lx\n", (unsigned long)to, (unsigned long)from);
    }
    if((to->length)!=(from->length))
    {
        warningMes;
        printf("in copyListOfGaloisFieldIngaloisFieldElements, to->length(=\'%d\') and from->length(=\'%d\') is not same.\n", (to->length), (from->length));
        printf("exceed order is cleared to all zero.");
    }
    #endif

    if(copyListOfGaloisField(to->element, from->element, from->length))
    {
        return -1;
    }

    #ifdef USING_OPEN_MP
    #pragma omp parallel for schedule(guided) private(i) shared(from, to, galoisFields)
    #endif
    for(i=from->length; i<to->length; i++)
    {
        *(to->element+i)=*(galoisFields->element+0);
    }
    return 0;
}
