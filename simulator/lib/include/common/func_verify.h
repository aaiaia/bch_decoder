#ifndef _COMMON_FUNC_VERIFY_H_
#define _COMMON_FUNC_VERIFY_H_

#include <stdio.h>

/* Simulation funtion Test(verification) Mode start */
/* Test Mode */
enum ENUM_FLAG_CASE_SIM_TEST_OPTIONS
{
    FLAG_CASE_SIM_TEST_MODE_NONE,
    FLAG_CASE_SIM_TEST_MODE_GET_LLR_MAG_AVR_BELONG_EBN0,
    FLAG_CASE_SIM_TEST_MODE_GET_PROBABILITY_OF_MINIMUM_VAL,
    FLAG_CASE_SIM_TEST_MODE_SHOW_LOOP_CNT,
};

extern unsigned int global_flag_case_sim_testOpt;

/* Test Mode Commonly using variables */
/* Test mode iteration variables */
#define TMP_TEST_MODE_LOOPS             1000000UL
#define TMP_TEST_MODE_ZERO_BIT_RATIO    0.5f

extern unsigned long tmp_testMode_loops;


/* Error Distribution */
extern FILE *fp_testMode_log;
extern FILE *fp_testMode_sum_bit_cnt_arranged_by_bitRetio_csv;
extern FILE *fp_testMode_sum_bit_cnt_classified_arranged_by_bitRatio_csv;
extern FILE *fp_testMode_sum_llr_mag_arranged_by_bitRatio_csv;
extern FILE *fp_testMode_sum_err_bit_cnt_chg_classified_arranged_by_bitRatio_csv;

extern FILE *fp_testMode_all_arranged_by_bitRatio_csv;


extern FILE *fp_testMode_sum_codeword_cnt_arranged_by_errorNums_csv;
extern FILE *fp_testMode_sum_llr_mag_distribution_cnt_arranged_by_errorNums_csv;
extern FILE *fp_testMode_sum_bit_ratio_cnt_arranged_by_errorNums_csv;

extern FILE *fp_testMode_all_arranged_by_errorNums_csv;

enum ENUM_CASE_TESTMODE_ERR_RATE_MSG_GEN_METH
{
    ENUM_CASE_TESTMODE_ERR_RATE_MSG_GEN_METH_NONE,
    ENUM_CASE_TESTMODE_ERR_RATE_MSG_GEN_METH_STATIC,
    ENUM_CASE_TESTMODE_ERR_RATE_MSG_GEN_METH_RAND,
};
extern unsigned int global_flag_case_testMode_msg_gen_method;

enum ENUM_CASE_TESTMODE_ERR_RATE_DECODING_METH
{
    ENUM_CASE_TEST_MODE_ERR_RATE_DECODING_METH_NONE,
    ENUM_CASE_TEST_MODE_ERR_RATE_DECODING_METH_HD,
    ENUM_CASE_TEST_MODE_ERR_RATE_DECODING_METH_SD,
};
extern unsigned int global_flag_case_testMode_decoding_method;

/* encoding parts arranged by bit ratio */
extern unsigned int tmp_testMode_zero_bit_nums_inC;
extern unsigned int tmp_testMode_one_bit_nums_inC;

/*
 * account of tmp_bit_num_inC_ref_zero_OZ_ratio
 * index 0 -> zero bit num in Codeword.
 * index 1 -> one bit num in Codeword.
 */
extern unsigned long** tmp_bit_num_inC_ref_zero_OZ_ratio;

/* decoding parts arranged by bit ratio */
extern unsigned int tmp_testMode_zeroToOne_bit_nums_inR;//received bit change detect
extern unsigned int tmp_testMode_oneToZero_bit_nums_inR;//received bit change detect

extern unsigned int tmp_testMode_zero_is_inR_w;//received bits cnt, which are zero or one and is error
extern unsigned int tmp_testMode_one_is_inR_w;//received bits cnt, which are zero or one and is error

extern unsigned int tmp_testMode_zero_is_inR_c;//received bits cnt, which are zero or one and is correct
extern unsigned int tmp_testMode_one_is_inR_c;//received bits cnt, which are zero or one and is correct

extern unsigned long tmp_testMode_llr_mag_sum_inR_w;//llr sum in received codeword wrong case
extern unsigned long tmp_testMode_llr_mag_sum_inR_c;//llr sum in received codeword good case


/*
 * account of tmp_bit_num_inR_ref_zero_OZ_ratio
 * index 0 -> total zero bit nums are counted.
 * index 1 -> zero bit nums in correct bit
 * index 2 -> zero bit nums in error bit
 */
extern unsigned long** tmp_bit_num_inR_ref_zero_OZ_ratio;

/*
 * account of tmp_bit_num_inR_ref_one_OZ_ratio
 * index 0 -> total one bit nums are counted.
 * index 1 -> one bit nums in correct bit
 * index 2 -> one bit nums in error bit
 */
extern unsigned long** tmp_bit_num_inR_ref_one_OZ_ratio;

/*
 * account of tmp_bit_chg_cnt_inR_OZ_ratio
 * index 0 -> total error bit transaction nums actually error bit nums.
 * index 1 -> nums of error bit transacting 0 to 1.
 * index 2 -> nums of error bit transacting 1 to 0.
 */
extern unsigned long** tmp_bit_chg_cnt_inR_OZ_ratio;

/*
 * account of tmp_llr_mag_sum_inR_OZ_ratio
 * index 0 -> summation of llr magnitude at all bit, don't care error or not.
 * index 1 -> summation of llr magnitude at corrected bit.
 * index 2 -> summation of llr magnitude at error bit.
 */
extern unsigned long** tmp_llr_mag_sum_inR_OZ_ratio;

extern double** tmp_llr_mag_inR;


/* data arranged by number of error */
extern unsigned long* tmp_sum_err_codeword_cnt_arranged_by_err_nums;//after chk codeword
extern unsigned long** tmp_err_llr_mag_distribution_in_all_loop_arranged_by_err_nums;//after chk codeword
extern unsigned int* tmp_err_llr_mag_distribution_in_one_codeword;//in codeword chking

extern unsigned long* tmp_sum_cor_codeword_cnt_arranged_by_err_nums;//after chk codeword
extern unsigned long** tmp_cor_llr_mag_distribution_in_all_loop_arranged_by_err_nums;//after chk codeword
extern unsigned int* tmp_cor_llr_mag_distribution_in_one_codeword;//in codeword chking

/* index 0 is sum num of zero and 1 is also sum num of one */
extern unsigned long** tmp_sum_zero_and_one_bit_cnt_arranged_by_err_nums;


extern double tmp_testMode_loops_zero_bit_ratio;
extern unsigned long tmp_testMode_loops_bit_inversion_val;

extern unsigned long tmp_testMode_loops_cnt;

extern unsigned long tmp_testMode_totalBit;

#define LLR_CASE_NUM    8

extern unsigned long tmp_testMode_count_bit_w;

extern unsigned long tmp_testMode_llr_mag_w[LLR_CASE_NUM];

extern unsigned long tmp_testMode_llr_mag_sum_w;

extern unsigned int tmp_testMode_llr_mag_max_w;
extern unsigned int tmp_testMode_llr_mag_min_w;

extern double tmp_testMode_llr_mag_avr_w;


extern unsigned long tmp_testMode_count_bit_c;

extern unsigned long tmp_testMode_llr_mag_c[LLR_CASE_NUM];

extern unsigned long tmp_testMode_llr_mag_sum_c;

extern unsigned int tmp_testMode_llr_mag_max_c;
extern unsigned int tmp_testMode_llr_mag_min_c;

extern double tmp_testMode_llr_mag_avr_c;


/****************************************************************/
/***** Probability of Min val *****/
extern unsigned int tmp_testMode_prob_distribution_group_nums;

extern unsigned int tmp_testMode_prob_min_counting_length;
extern unsigned int *tmp_testMode_prob_min_counting;

enum ENUM_CASE_TESTMODE_PROB_MIN_CHOOSEN_POLITICS
{
    ENUM_CASE_TEST_MODE_PROB_MIN_CHOOSE_SAME_VAL,
    ENUM_CASE_TEST_MODE_PROB_MIN_CHOOSE_LOW_VAL,
};
extern unsigned int tmp_testMode_prob_min_choosen_politics;

/* Functions */
unsigned int* initTestMode_prob_min_counting(unsigned int **p, unsigned int *len, unsigned int max_val);
char clearTestMode_prob_min_counting(unsigned int *p, unsigned int len, unsigned int max_val);
/* Simulation funtion Test(verification) Mode end */
#endif
