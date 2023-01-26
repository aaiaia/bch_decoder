struct_galoisFieldElements *createGaloisFieldExceptElements(unsigned int numberOfElement)
{
    struct_galoisFieldElements *p;

    if(!numberOfElement)
    {
                #ifndef RELEASE
                errorMes; printf("in *createGaloisFieldExceptElements, unsigned int numberOfElement is ZERO\n");
                #endif
        return NULL;
    }
    p=(struct_galoisFieldElements*)malloc(sizeof(struct_galoisFieldElements));
    memset(p, 0, sizeof(struct_galoisFieldElements));

    p->length=numberOfElement;
    p->element=createListOfGaloisField(numberOfElement);
    return p;
}

struct_galoisFieldElements *createGaloisFieldExceptElements_VariableLength()
{
    struct_galoisFieldElements *p;


    p=(struct_galoisFieldElements*)malloc(sizeof(struct_galoisFieldElements));
    memset(p, 0, sizeof(struct_galoisFieldElements));

    //p->length=numberOfElement;
    //p->element=createListOfGaloisField(numberOfElement);
    return p;
}
