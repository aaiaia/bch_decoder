/* Constructor and Destructor */
struct_errorComponent *createErrorComponent(unsigned int length)
{
    struct_errorComponent *p;

    #ifndef RELEASE
    if(!length)
    {
        errorMes;
        printf("in createErrorComponent, unsigned int length is zero.\n");
        return NULL;
    }
    #endif

    p=(struct_errorComponent*)malloc(sizeof(struct_errorComponent));
    memset(p, 0, sizeof(struct_errorComponent));

    p->erroredCodeWord=createPowerFormPolynomial(length);
    p->errorLocationVector=createPowerFormPolynomial(length);
    return p;
}

char closeErrorComponent(struct_errorComponent **p)
{
    #ifndef RELEASE
    if(!p)
    {
        warningMes;
        printf("in closeErrorComponent, struct_errorComponent **p is null.\n");
        return -1;
    }
    if(!*p)
    {
        warningMes;
        printf("in closeErrorComponent, struct_errorComponent *p is null.\n");
        return -1;
    }
    #endif

    closePowerFormPolynomial(&((*p)->erroredCodeWord));
    closePowerFormPolynomial(&((*p)->errorLocationVector));
    free(*p);
    (*p)=NULL;
    return 0;
}

struct_errorComponent *recreateErrorComponent(struct_errorComponent **p, unsigned int length)
{
    #ifndef RELEASE
    if(!p)
    {
        errorMes;
        printf("in recreateErrorComponent, struct_errorComponent **p is NULL.\n");
        return NULL;
    }
    #endif
    if(*p)
    {
        #ifndef RELEASE
        if(closeErrorComponent(p))
        {
            warningMes;
            printf("in recreateErrorComponent, closeErrorComponent(p) is fail.\n");
        }
        #else
        closeErrorComponent(p);
        #endif
    }
    *p=createErrorComponent(length);
    return *p;
}

/* Operation */
struct_errorComponent *ifNotExistCreateErrorComponent(struct_errorComponent **p, unsigned int length)
{
    #ifndef RELEASE
    if(!p)
    {
        errorMes;
        printf("in recreateErrorComponent, struct_errorComponent **p is NULL.\n");
        return NULL;
    }
    #endif
    if(*p)
    {
        #ifndef RELEASE
        if(closeErrorComponent(p))
        {
            warningMes;
            printf("in recreateErrorComponent, closeErrorComponent(p) is fail.\n");
        }
        #else
        closeErrorComponent(p);
        #endif
    }
    else
    {
        *p=createErrorComponent(length);
    }
    return *p;
}

void errorOccurationWhichAreRandomPosition(struct_powerFormPolynomials *p, unsigned int numberOfError)
{
    unsigned int i;

    #ifndef RELEASE
    if(!p)
    {
        errorMes;
        printf("in errorOccurationWhichAreRandomPosition, struct_powerFormPolynomials *p is NULL.\n");
    }
    #endif
    /* initial all zero condition */
    memset(p->equation, '0', sizeof(char_POWER_FORM)*(p->usedLength));
    *(p->equation+(p->usedLength))=0;
    while(numberOfError)
    {
        if(*(p->equation+(i=(rand()%p->usedLength)))=='0')
        {
            *(p->equation+i)='1';
            numberOfError--;
        }
    }
}
