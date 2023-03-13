#ifndef _CHANNEL_BPSK_H_
#define _CHANNEL_BPSK_H_

#include "decoder/soft_decision/llr.h"

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

/* Operation */
double_BPSK_DATA getSqureRootAvrBpskReceivedDataAddedAwgn(struct_bpskComponents *p);

/* Mod/Demod */
void BPSK_Mod(double_BPSK_DATA *Transmitted_MSG, char *Codeword_MSG, unsigned int WordLength);
void BPSK_Demod(char *Codeword_MSG, double_BPSK_DATA *Received_MSG, unsigned WordLength);
void ADD_AWGN(double_BPSK_DATA *transmitted_msg, double_BPSK_DATA *received_msg, double bitrate, unsigned int number_of_loop, double main_com_EbN0);
/* ADD_AWGN_CAL_LLR inscluds demodulation to LLR */
double_BPSK_DATA LLR_MAX=0;
double_BPSK_DATA LLR_MIN=0;
void ADD_AWGN_CAL_LLR(double_BPSK_DATA *transmitted_msg, double_BPSK_DATA *received_msg, double_BPSK_DATA *received_LLR, double bitrate, unsigned int number_of_loop, double main_com_EbN0); //need to add bitrate

/* For Debugging(Displaying */
void printBpskModulation(unsigned int printLength, double_BPSK_DATA *p);
void printBpskAWGN(unsigned int printLength, double_BPSK_DATA *operandA, double_BPSK_DATA *operandB);
void printLLR(unsigned int printLength, double_BPSK_DATA *p);

#endif
