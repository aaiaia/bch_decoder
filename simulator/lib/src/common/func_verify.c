#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "common/func_verify.h"

/* Simulation funtion Test(verification) Mode start */
/* Test Mode */
unsigned int global_flag_case_sim_testOpt = 0;

/* Test Mode Commonly using variables */
/* Test mode iteration variables */
unsigned long tmp_testMode_loops = TMP_TEST_MODE_LOOPS;


/* Error Distribution */
FILE *fp_testMode_log = NULL;
FILE *fp_testMode_sum_bit_cnt_arranged_by_bitRetio_csv = NULL;
FILE *fp_testMode_sum_bit_cnt_classified_arranged_by_bitRatio_csv = NULL;
FILE *fp_testMode_sum_llr_mag_arranged_by_bitRatio_csv = NULL;
FILE *fp_testMode_sum_err_bit_cnt_chg_classified_arranged_by_bitRatio_csv = NULL;

FILE *fp_testMode_all_arranged_by_bitRatio_csv = NULL;


FILE *fp_testMode_sum_codeword_cnt_arranged_by_errorNums_csv = NULL;
FILE *fp_testMode_sum_llr_mag_distribution_cnt_arranged_by_errorNums_csv = NULL;
FILE *fp_testMode_sum_bit_ratio_cnt_arranged_by_errorNums_csv = NULL;

FILE *fp_testMode_all_arranged_by_errorNums_csv = NULL;

unsigned int global_flag_case_testMode_msg_gen_method =
                ENUM_CASE_TESTMODE_ERR_RATE_MSG_GEN_METH_STATIC;

unsigned int global_flag_case_testMode_decoding_method =
                ENUM_CASE_TEST_MODE_ERR_RATE_DECODING_METH_HD;

/* encoding parts arranged by bit ratio */
unsigned int tmp_testMode_zero_bit_nums_inC = 0;
unsigned int tmp_testMode_one_bit_nums_inC = 0;

/*
 * account of tmp_bit_num_inC_ref_zero_OZ_ratio
 * index 0 -> zero bit num in Codeword.
 * index 1 -> one bit num in Codeword.
 */
unsigned long** tmp_bit_num_inC_ref_zero_OZ_ratio = NULL;

/* decoding parts arranged by bit ratio */
unsigned int tmp_testMode_zeroToOne_bit_nums_inR = 0;//received bit change detect
unsigned int tmp_testMode_oneToZero_bit_nums_inR = 0;//received bit change detect

unsigned int tmp_testMode_zero_is_inR_w = 0;//received bits cnt, which are zero or one and is error
unsigned int tmp_testMode_one_is_inR_w = 0;//received bits cnt, which are zero or one and is error

unsigned int tmp_testMode_zero_is_inR_c = 0;//received bits cnt, which are zero or one and is correct
unsigned int tmp_testMode_one_is_inR_c = 0;//received bits cnt, which are zero or one and is correct

unsigned long tmp_testMode_llr_mag_sum_inR_w = 0UL;//llr sum in received codeword wrong case
unsigned long tmp_testMode_llr_mag_sum_inR_c = 0UL;//llr sum in received codeword good case


/*
 * account of tmp_bit_num_inR_ref_zero_OZ_ratio
 * index 0 -> total zero bit nums are counted.
 * index 1 -> zero bit nums in correct bit
 * index 2 -> zero bit nums in error bit
 */
unsigned long** tmp_bit_num_inR_ref_zero_OZ_ratio = NULL;

/*
 * account of tmp_bit_num_inR_ref_one_OZ_ratio
 * index 0 -> total one bit nums are counted.
 * index 1 -> one bit nums in correct bit
 * index 2 -> one bit nums in error bit
 */
unsigned long** tmp_bit_num_inR_ref_one_OZ_ratio = NULL;

/*
 * account of tmp_bit_chg_cnt_inR_OZ_ratio
 * index 0 -> total error bit transaction nums actually error bit nums.
 * index 1 -> nums of error bit transacting 0 to 1.
 * index 2 -> nums of error bit transacting 1 to 0.
 */
unsigned long** tmp_bit_chg_cnt_inR_OZ_ratio = NULL;

/*
 * account of tmp_llr_mag_sum_inR_OZ_ratio
 * index 0 -> summation of llr magnitude at all bit, don't care error or not.
 * index 1 -> summation of llr magnitude at corrected bit.
 * index 2 -> summation of llr magnitude at error bit.
 */
unsigned long** tmp_llr_mag_sum_inR_OZ_ratio = NULL;

double** tmp_llr_mag_inR = NULL;


/* data arranged by number of error */
unsigned long* tmp_sum_err_codeword_cnt_arranged_by_err_nums = NULL;//after chk codeword
unsigned long** tmp_err_llr_mag_distribution_in_all_loop_arranged_by_err_nums = NULL;//after chk codeword
unsigned int* tmp_err_llr_mag_distribution_in_one_codeword = NULL;//in codeword chking

unsigned long* tmp_sum_cor_codeword_cnt_arranged_by_err_nums = NULL;//after chk codeword
unsigned long** tmp_cor_llr_mag_distribution_in_all_loop_arranged_by_err_nums = NULL;//after chk codeword
unsigned int* tmp_cor_llr_mag_distribution_in_one_codeword = NULL;//in codeword chking

/* index 0 is sum num of zero and 1 is also sum num of one */
unsigned long** tmp_sum_zero_and_one_bit_cnt_arranged_by_err_nums = NULL;


double tmp_testMode_loops_zero_bit_ratio = 0.5f;
unsigned long tmp_testMode_loops_bit_inversion_val = 0UL;

unsigned long tmp_testMode_loops_cnt = 0UL;

unsigned long tmp_testMode_totalBit = 0UL;

unsigned long tmp_testMode_count_bit_w = 0UL;

unsigned long tmp_testMode_llr_mag_w[LLR_CASE_NUM] = {0UL};

unsigned long tmp_testMode_llr_mag_sum_w = 0UL;

unsigned int tmp_testMode_llr_mag_max_w = 0;
unsigned int tmp_testMode_llr_mag_min_w = 7;

double tmp_testMode_llr_mag_avr_w = 0.0f;


unsigned long tmp_testMode_count_bit_c = 0UL;

unsigned long tmp_testMode_llr_mag_c[LLR_CASE_NUM] = {0UL};

unsigned long tmp_testMode_llr_mag_sum_c = 0UL;

unsigned int tmp_testMode_llr_mag_max_c = 0;
unsigned int tmp_testMode_llr_mag_min_c = 7;

double tmp_testMode_llr_mag_avr_c = 0.0f;


/****************************************************************/
/***** Probability of Min val *****/
unsigned int tmp_testMode_prob_distribution_group_nums = 1;

unsigned int tmp_testMode_prob_min_counting_length = 0;
unsigned int *tmp_testMode_prob_min_counting = NULL;

unsigned int tmp_testMode_prob_min_choosen_politics =
                ENUM_CASE_TEST_MODE_PROB_MIN_CHOOSE_SAME_VAL;

/* Functions */
unsigned int* initTestMode_prob_min_counting(unsigned int **p, unsigned int *len, unsigned int max_val)
{
    if(!p)          return NULL;
    if(!max_val)    return NULL;

    (*len) = max_val + 1;
    (*p) = (unsigned int*)malloc(sizeof(unsigned int) * ((*len) + 1));
    memset(*p, 0, sizeof(unsigned int) * ((*len) + 1));
    return (*p);
}

char clearTestMode_prob_min_counting(unsigned int *p, unsigned int len, unsigned int max_val)
{
    if(!p)  return -1;
    memset(p, 0, sizeof(unsigned int) * len);
    return 0;
}
/* Simulation funtion Test(verification) Mode end */
