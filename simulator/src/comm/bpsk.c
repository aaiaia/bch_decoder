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

/* Operation */
double_BPSK_DATA getSqureRootAvrBpskReceivedDataAddedAwgn(struct_bpskComponents *p)
{
    unsigned int i;
    double_BPSK_DATA tmpAvr=0.0;
    for(i=0; i<p->usedLength; i++)
    {
        tmpAvr+=(*(p->bpskReceivedDataAddedAwgn+i))*(*(p->bpskReceivedDataAddedAwgn+i));
    }
    tmpAvr/=((double_BPSK_DATA)p->usedLength);
    sqrt(tmpAvr);
    p->squreRootBpskReceivedDataAddedAwgn=tmpAvr;
    return tmpAvr;
}

/* Mod/Demod */
void BPSK_Mod(double_BPSK_DATA *Transmitted_MSG, char *Codeword_MSG, unsigned int WordLength)
{
    unsigned int i;

        #ifndef RELEASE
        if(!Transmitted_MSG)
        {
            debugFuncNameMes; printf("in BPSK_Mod, if(!Transmitted_MSG)\n");
        }
        if(!Codeword_MSG)
        {
            debugFuncNameMes; printf("in BPSK_Mod, if(!Codeword_MSG)\n");
        }
        if(!WordLength)
        {
            debugFuncNameMes; printf("in BPSK_Mod, if(!WordLength)\n");
        }
        #endif

    //for(i=0; i<WordLength; i++)
    //{
    //    printf("%c ", Codeword_MSG[i]);
    //}
    //for(i=0; i<WordLength; i++)
    //{
    //    printf("%f ", Transmitted_MSG[i]);
    //}

    #ifdef USING_OPEN_MP
    #pragma omp parallel for schedule(guided) private(i) shared(WordLength, Codeword_MSG, Transmitted_MSG)
    #endif
    for(i=0; i<WordLength; i++)
    {
        if(Codeword_MSG[i]=='1')
        {
            Transmitted_MSG[i] = 1;
        }
        else
        {
            Transmitted_MSG[i] = -1;
        }
    }
    return;
}

void BPSK_Demod(char *Codeword_MSG, double_BPSK_DATA *Received_MSG, unsigned WordLength)
{
    unsigned int i;

    #ifdef USING_OPEN_MP
    #pragma omp parallel for schedule(guided) private(i) shared(WordLength, Received_MSG, Codeword_MSG)
    #endif
    for(i=0;i<WordLength;i++)
    {
        if(Received_MSG[i]>0)
        {
            Codeword_MSG[i] = '1';
        }
        else if(Received_MSG[i]<0)
        {
            Codeword_MSG[i] = '0';
        }
        else
        {
            #ifndef USING_OPEN_MP
            if(rand()%2)
            {
                Codeword_MSG[i]='1';
            }
            else
            {
                Codeword_MSG[i]='0';
            }
            #else
            if(Codeword_MSG[i]%2)
            {
                Codeword_MSG[i]='1';
            }
            else
            {
                Codeword_MSG[i]='0';
            }
            #endif
        }
    }
    return;
}

void ADD_AWGN(double_BPSK_DATA *transmitted_msg, double_BPSK_DATA *received_msg, double bitrate, unsigned int number_of_loop, double main_com_EbN0) //need to add bitrate
{

    double *channel_noise = (double*)malloc(sizeof(double)*number_of_loop);
    double Sum_Pow = 0;
    double spow, npow, attn;

    unsigned int i;

    memset(channel_noise, 0, sizeof(double)*number_of_loop);



    for(i=0;i<number_of_loop;i++)
    {
        channel_noise[i] = gaussian(0, 1);
    }
    for(i=0;i<number_of_loop;i++)
    {
        Sum_Pow += pow(transmitted_msg[i], 2);
    }

    spow = Sum_Pow / number_of_loop;
    npow = spow / bitrate / pow(10, (main_com_EbN0/10));
    attn = sqrt((0.5 * npow));

    for(i=0;i<number_of_loop;i++)
    {
        received_msg[i] = transmitted_msg[i] + (channel_noise[i] * attn);
    }
    free(channel_noise);
    return;
}
