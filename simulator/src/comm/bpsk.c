/* Constructor and Destructor */
struct_bpskComponents *createBpskComponents(unsigned int length)
{
    struct_bpskComponents *p=(struct_bpskComponents*)malloc(sizeof(struct_bpskComponents));
    memset(p, 0, sizeof(struct_bpskComponents));

    p->bpskTransData=(double_BPSK_DATA*)malloc(sizeof(double_BPSK_DATA)*(length));
    memset(p->bpskTransData, 0, sizeof(double_BPSK_DATA)*length);
            debugMes; printf("in createBpskComponents, p->bpskTransData=(double_BPSK_DATA*)malloc(sizeof(double_BPSK_DATA)*(length));\n");
            debugMes; printf("in createBpskComponents, p->bpskTransData=0x%lx\n", (unsigned long)(p->bpskTransData));

    p->bpskReceivedDataAddedAwgn=(double_BPSK_DATA*)malloc(sizeof(double_BPSK_DATA)*length);
    memset(p->bpskReceivedDataAddedAwgn, 0, sizeof(double_BPSK_DATA)*length);

    //p->bpskReceivedDataLLR=(double_BPSK_DATA*)malloc(sizeof(double_BPSK_DATA)*length);
    //memset(p->bpskReceivedDataLLR, 0, sizeof(double_BPSK_DATA)*length);

    p->bpskReceivedLLR=createLogLikeHoodRatio(length);
            #ifndef RELEASE
            if(!(p->bpskReceivedLLR))
            {
                warningMes;
                printf("in createBpskComponents, !(p->bpskReceivedLLR=createLogLikeHoodRatio(unsigned int length))\n");
            }
            #endif

    p->length=length;
    p->usedLength=length;

    p->statusFlag=BPSK_TRANCEIVE_FLAG_DATA_INIT;

    return p;
}

char closeBpskComponents(struct_bpskComponents **p)
{
    #ifndef RELEASE
    if(p)
    {
        warningMes;;
        printf("in closeBpskComponents, struct_bpskComponents **p is NULL\n");
        return -1;
    }
    if(*p)
    {
        warningMes;
        printf("in closeBpskComponents, struct_bpskComponents *p is NULL\n");
        return -1;
    }
    #endif

    closeLogLikeHoodRatio(&((*p)->bpskReceivedLLR));

    free((*p)->bpskTransData);
    free((*p)->bpskReceivedDataAddedAwgn);
    free(*p);
    *p=NULL;
    return 0;
}

struct_bpskComponents *recreateBpskComponents(struct_bpskComponents **p, unsigned int length)
{
    #ifndef RELEASE
    if(!p)
    {
        errorMes;
        printf("in createBpskComponents, struct_bpskComponents **p is NULL.\n");
        return NULL;
    }
    #endif

    if(*p)
    {
                #ifndef RELEASE
                warningMes; printf("in createBpskComponents, struct_bpskComponents *p is will be deleted.\n");
                #endif
        closeBpskComponents(p);
    }
    (*p)=createBpskComponents(length);
    return *p;
}
