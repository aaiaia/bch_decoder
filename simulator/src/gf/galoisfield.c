struct_galoisFieldPolyForm *createPolyForm(unsigned int length)
{
    struct_galoisFieldPolyForm *p;

    #ifndef RELEASE
    if(!length)
    {
        errorMes;
        printf("arg[0], poly form length is %d\n", length);
        return NULL;
    }
    #endif

    p = (struct_galoisFieldPolyForm*)malloc(sizeof(struct_galoisFieldPolyForm));
    memset(p, 0, sizeof(struct_galoisFieldPolyForm));

    //p->bits=&(p->length);

    p->length=length;

    p->value = (char_GALOIS_FIELD_VALUE*)malloc((sizeof(char_GALOIS_FIELD_VALUE)*length)+1);//+1 is to null.
    memset(p->value, '0', sizeof(char_GALOIS_FIELD_VALUE)*length);
    *(p->value+length)=0;//insert null.

    p->intValue=0;
    p->index=0;

    return p;
}

char closePolyForm(struct_galoisFieldPolyForm **p)
{
    #ifndef RELEASE
    if(!p)
    {
        warningMes;
        printf("in struct_galoisFieldPolyForm, struct_galoisFieldPolyForm *p = 0x%lx\n", (unsigned long)p);
        return -1;
    }
    if(!*p)
    {
        warningMes;
        printf("in struct_galoisFieldPolyForm, struct_galoisFieldPolyForm **p = 0x%lx\n", (unsigned long)*p);
        return -1;
    }
    #endif

    free((*p)->value);
    free((*p));
    *p=NULL;
    return 0;
}

struct_galoisFieldPolyForm *recreatePolyForm(struct_galoisFieldPolyForm **p, unsigned int length)
{
    if(*p)
    {
        #ifndef RELEASE
        if((global_flag_cmdOption&FLAG_MASK_PRINTF_LOG))
        {
            printf("in recreatePolyForm, struct_galoisFieldPolyForm *p is already have data.\n");
            printf("*p will be closePolyForm(p).\n");
        }
        #endif

        closePolyForm(p);
    }
    *p=createPolyForm(length);
    return *p;
}

/* Convert */
unsigned int convertGaloisFieldPolyFormUnsignedInt(struct_galoisFieldPolyForm *p)
{
    unsigned int i;
    unsigned int result=0;

    #ifdef USING_OPEN_MP
    #pragma omp parallel for schedule(guided) private(i) shared(p) reduction(|:result)
    #endif
    for(i=0; i<p->length; i++)
    {
        if(*(p->value+i)=='1')
        {
            result|=(1<<i);
        }
    }
    return result;
}

/* For Display(Debugging) */
void printPolyFormWithEnter(struct_galoisFieldPolyForm *p)
{
    unsigned int i;

    if(!p)
    {
        errorMes;
        printf("struct_galoisFieldPolyForm addr is wrong(addr:0x%lx).\n", (unsigned long)p);
    }

    printf("\"");
    for(i=0; i<p->length; i++)
    {
        printf("%c", *(p->value+i));
    }
    printf("\"");
    printf("\n");
}

void printPolyFormPure(struct_galoisFieldPolyForm *p)
{
    unsigned int i;

    if(!p)
    {
        errorMes;
        printf("struct_galoisFieldPolyForm addr is wrong(addr:0x%lx).\n", (unsigned long)p);
    }

    for(i=0; i<p->length; i++)
    {
        printf("%c", *(p->value+i));
    }
}

void printPolyForm(struct_galoisFieldPolyForm *p)
{
    unsigned int i;

    if(!p)
    {
        errorMes;
        printf("struct_galoisFieldPolyForm addr is wrong(addr:0x%lx).\n", (unsigned long)p);
    }

    printf("\"");
    for(i=0; i<p->length; i++)
    {
        printf("%c", *(p->value+i));
    }
    printf("\"");
}

void printPolyFormWithTapAndEnter(struct_galoisFieldPolyForm *p)
{
    unsigned int i;

    if(!p)
    {
        errorMes;
        printf("struct_galoisFieldPolyForm addr is wrong(addr:0x%lx).\n", (unsigned long)p);
    }

    for(i=0; i<p->length; i++)
    {
        printf("%c\t", *(p->value+i));
    }
    printf("\n");
}

void printPolyFormWithTap(struct_galoisFieldPolyForm *p)
{
    unsigned int i;

    if(!p)
    {
        errorMes;
        printf("struct_galoisFieldPolyForm addr is wrong(addr:0x%lx).\n", (unsigned long)p);
    }

    for(i=0; i<p->length; i++)
    {
        printf("%c\t", *(p->value+i));
    }
}
