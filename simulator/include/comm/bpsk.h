#define BPSK_TRANCEIVE_FLAG_DATA_INIT   0
#define BPSK_TRANCEIVE_FLAG_DATA_LOAD   (1<<0)
#define BPSK_TRANCEIVE_FLAG_DATA_TRANS  (1<<1)
#define BPSK_TRANCEIVE_FLAG_DATA_RECEIV (1<<2)

typedef double double_BPSK_DATA;

struct struct_bpskComponents
{
    unsigned int length;
    unsigned int usedLength;
    double_BPSK_DATA *bpskTransData;
    double_BPSK_DATA *bpskReceivedDataAddedAwgn;
    double_BPSK_DATA squreRootBpskReceivedDataAddedAwgn;

    //double_BPSK_DATA *bpskReceivedDataLLR;
    struct_logLikeHoodRatio *bpskReceivedLLR;
    unsigned char statusFlag;
}typedef struct_bpskComponents;

/* Constructor and Destructor */
struct_bpskComponents *createBpskComponents(unsigned int length);
char closeBpskComponents(struct_bpskComponents **p);
struct_bpskComponents *recreateBpskComponents(struct_bpskComponents **p, unsigned int length);
