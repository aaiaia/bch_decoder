#include <stdio.h>

#include "util/bit_mask.h"
#include "common/cmd_opt_vars.h"

/* Flags for Debugging(Displaying) start */
/* Log definition */
unsigned int global_flag_cmdOption = 0;
    /* variables to check memory lineary */
    unsigned int global_tmp_i_memoryCheck = 0;

/* Sysgem Hold command */
unsigned int global_flag_cmdSystemHold = 0;

/* forced Sysgem unhold command */
unsigned int global_flag_cmdSystemUnhold = 0;

    /* Algorithm debugging options start */
    /* BM Algorithm debug option start */
    unsigned int global_flag_debug_bmSim = 0;
    /* BM Algorithm debug option end */

    /*mSBS Algorithm debug option start*/
    /* below variables could be unnecessary */
    unsigned int global_flag_debug_mSBS_Sim = 0;
    /*mSBS Algorithm debug option end*/
    /* Algorithm debugging options end */

/* Global debug option start */
unsigned int global_flag_deBugOption = 0; //not used

    /*Display RESULT or Progress or Function name*/

    /* flags using enum DEBUG_DISPLAY_MODE start */
    unsigned int global_flag_debug_display = 0;

    unsigned int global_flag_bch_BM_SimHD_display = 0;

    unsigned int global_flag_bch_BM_SimSD_display = 0;

    unsigned int global_flag_bch_mSBS_SimHD_display = 0;

    unsigned int global_flag_bch_mSBS_SimSD_display = 0;

    unsigned int global_flag_Modulation_Mode_BPSK_Display = 0;

    unsigned int global_flag_case_getBER_method_Display = 0;
    /* flags using enum DEBUG_DISPLAY_MODE end */

/* ber option start */
unsigned int global_flag_berOptions = 0;

/* Log LikeHood Ratio debug option start */
unsigned int global_flag_debug_awgnLLR = 0;
/* Log LikeHood Ratio debug option end */

/* Display Galois Field(GF) start */
unsigned int global_flag_gfCommon_display = 0;
/* Display Galois Field(GF) end */
/* Global debug option end */
/* Flags for Debugging(Displaying) end */

/* Flags for simulation mode start */
    /* global flag associate with selecting test pattern start */
    const char NAME_FLAG_CASE_SEL_TP_BEFORE_DEC_MTEHOD_FILE_ADDED
                    [FLAG_CASE_SEL_TP_BEFORE_DEC_END][51] = \
    {
        "NONE",
        "THEO",
        "ACT"
    };
    /* global flag associate with selecting test pattern end */

    unsigned int global_flag_case_selTP_before_dec = 0;

    /* subflags in FLAG_CASE_SEL_TP_BEFORE_DEC_THEO start */
    /*
     * global flag to selecting test pettern.
     * It is working only in
     * FLAG_CASE_SEL_TP_BEFORE_DEC_THEO mode.
     */
    unsigned int global_flag_case_choosingCodeWordMethod_inTheo = 0;
    /* subflags in FLAG_CASE_SEL_TP_BEFORE_DEC_THEO end */

    /* sub-flags in FLAG_CASE_SEL_TP_BEFORE_DEC_ACT start */
    /*
     * In FLAG_CASE_SEL_TP_BEFORE_DEC_ACT mode(which is non-correctable case),
     * select handling method
     */
    unsigned int global_flag_case_dec_fail_handling = 0;

    /* Method that re transfer is not supported yet */
    /* sub-flags in FLAG_CASE_SEL_TP_BEFORE_DEC_ACT end */
    /****** global flag associate with decoding end *****/

/* global flags error correction codes start */
/* selection Code mode start */
unsigned int global_flag_simulationCode = 0;
/*Channel Setting, LLR quantization bit setting*/
/* selection Code mode start */

    /* sub-flags in FLAG_MASK_CODE_BCH start */

    const char KIND_OF_BCH_DECODING_ALGORITHM_NAME[KIND_OF_BCH_DECODING_END][21] = \
    {
        "BM",
        //"mSBS",
        "iBM",
        "RiBM",
        "SiBM",
        "mSBS",
        //"BM",
        "PETERSON",
        "EUCLIDIAN",
    };

    /* bch enable algorithm list */
    unsigned int global_flag_bchDecAlgo_Enable = 0;
    /*CODE LENGTH(GALOIS FIELD), symbols(BCH not needs)*/

    /* Simulation Working Mode */
    unsigned int global_flag_sim_workingMode = 0;   //not used

    /* global_list_flag_bch_sim_decodingMode */
    unsigned int global_flag_sim_decodingMode = 0;
    unsigned int global_list_flag_bch_sim_decodingMode[KIND_OF_BCH_DECODING_END] = {0};
        /*******************************************************************/
        /********** global flag associate wieh bch - BM algorithm **********/
        /*******************************************************************/
    /* sub-flags in FLAG_MASK_CODE_BCH end */
/* global flags error correction codes end */
/* Flags for simulation mode end */

/* Flags to output test pattern start */
unsigned int global_flag_fileInputOutput = 0;

    unsigned int global_flag_file_io_hd_pattern_output = 0;
    unsigned int global_flag_file_io_hd_pattern_output_log = 0;
    unsigned int global_flag_file_io_hd_pattern_output_verilog = 0;

    unsigned int global_flag_file_io_sd_pattern_output = 0;
    unsigned int global_flag_file_io_sd_pattern_output_log = 0;
    unsigned int global_flag_file_io_sd_pattern_output_verilog = 0;
/* Flags to output test pattern end */

/* Flags about Modulation mode(bpsk, qpsk, qam etc...) start */
unsigned int global_flag_Modulation_Mode = 0;
/* Flags about Modulation mode(bpsk, qpsk, qam etc...) end */

/* Flags about AWGN(channel noise) start */
double global_EbN0_Start_Value = DEFAULT_VALUE_EBN0;

double global_EbN0_End_Value = DEFAULT_VALUE_EBN0_END;

double global_EbN0_Step = DEFAULT_VALUE_EBN0_STEP;


unsigned int global_flag_debug_awgn = 0;
/* Flags about AWGN(channel noise) end */

/* Flags to LLR calculation start */
unsigned int global_Channel_Mode = DEFAULT_CHANNEL_MODE;    //not used

    unsigned int global_flag_Channel_Mode_Func = FLAG_MASK_CHANNEL_MODE_FUNC_NONE;
/* Flags to LLR calculation end */

/* Flags about Log Likehood Ratio(LLR) processong start */
unsigned int global_flag_llr_display = 0;

const char NAME_FLAG_CASE_FINDING_MIN_MTEHOD[FLAG_CASE_FINDING_MIN_LLR_METHOD_END][51] = \
{
    "NONE",
    "MINIMUM",
    "GROUPING_MINIMUM",
    "TREE-FALLOWING_MIN1_PATH",
    "TREE-PROBABILISTIC_CHOOSEN(Not support yet)",
    "TREE-PRIOR_HD_ZERO(Not support yet)",
};
const char NAME_FLAG_CASE_FINDING_MIN_MTEHOD_FILE_ADDED[FLAG_CASE_FINDING_MIN_LLR_METHOD_END][51] = \
{
    "NONE",
    "MIN",
    "GROUP_MIN",
    "TREE-FAL_MIN1_PH",
    "TREE-PROB_CHOOSEN",
    "TREE-PRIOR_HD_ZERO",
};


unsigned int global_flag_case_find_LLR_method = 0;

    unsigned int global_grouping_stream_nums = 1;
    unsigned int global_group_bit_num = 0;
    unsigned int global_group_last_bit_num = 0;

/* pass HD 1 when llr mag is zero start */
const char NAME_FLAG_CASE_PASS_MAG0_HD_1[FLAG_CASE_PASS_MAG0_HD_1_END][71] = \
{
    "No passing llr magnitude when HD 1(finding min value only)",
    "Passing llr magnitude when HD 1(finding min value only)",
};

unsigned int global_flag_case_pass_hd_1 = FLAG_CASE_PASS_MAG0_HD_1_DISABLE;
/*  pass HD 1 when llr mag is zero end  */

const char NAME_FLAG_CASE_SAME_LLR_HANDLING[FLAG_CASE_SAME_LLR_HANDLING_END][31] = \
{
    "same llr mag is none",
    "same llr mag is not selected",
    "same llr mag is prior 0",
    "same llr mag is prior 1",
};
unsigned int global_flag_case_same_llr_handling = FLAG_CASE_SAME_LLR_HANDLING_NONE;

const char NAME_FLAG_CASE_INIT_LLR_MAG_METHOD[FLAG_CASE_INIT_LLR_MAG_METHOD_END][51] = \
{
    "first loc is selected, has no condition",
    "if no minimum llr val than max llr, not select",
};
unsigned int global_flag_case_init_llr_mag_method = FLAG_CASE_INIT_LLR_MAG_METHOD_NONE;

/* LLR QUANTIZATION FILTER options start */
const char NAME_QUANTIZ_NUMBERIC_SYSTEM[ENUM_FLAG_CASE_QUANTIZ_NUMBERIC_SYSTEM_END][51] = \
{
    "numberic system is not identified",
    "2's compliment",
    "1's compliment",
};

unsigned int global_flag_case_llr_numberic_system = \
    ENUM_FLAG_CASE_QUANTIZ_NUMBERIC_SYSTEM_TWO_S_COMPLEMENT;


const char NAME_QUANTIZ_ONE_S_COMPLEMENT_ZERO_HANDLING \
    [ENUM_FLAG_CASE_QUANTIZ_NUMBERIC_SYS_ONE_S_COM_END][21] = \
{
    "none",
    "overlaping",
    "duplicated",
};

unsigned int global_flag_case_llr_mag_ones_complement_zero_handling = \
    ENUM_FLAG_CASE_QUANTIZ_NUMBERIC_SYS_ONE_S_COM_NONE;

/* LLR QUANTIZATION FILTER VARIABLES start */
double global_LLR_quantizFilter_offset = -0.5;
double global_LLR_quantizFilter_step = 1.0;
unsigned char global_LLR_quantizFilter_bitWidth = 4;
/* LLR QUANTIZATION FILTER VARIABLES end */
/* LLR QUANTIZATION FILTER options end */
/* Flags about Log Likehood Ratio(LLR) processong end */

/* flags for simulation main config start */
unsigned long global_minimum_loop = 0;
unsigned long global_base_of_loop = 0;
unsigned int global_rate_of_loop = 0;

unsigned int global_ErrorNum = 0;
/* flags for simulation main config end */

/* flags for generating galois field(GF) start */
/* Make Galois Field options start */
unsigned int global_flag_gfCommon = 0;
/* Make Galois Field options end */


/* Code Specification start */
unsigned int global_GaloisFieldExponential = DEFAULT_VALUE_GF_EXP;

unsigned int global_MaximumCodeLength = 0;
unsigned int global_CodeLength = DEFAULT_VALUE_CODE_LENGTH;
unsigned int global_LimitCodeLength = 0;

/* added LCM expantion */
char *global_addedRootToLCM_string = NULL;// to adding low common multiple(LCM)
unsigned int global_degAddedRootToLCM = 0;
/* Code Specification end */

/* Error correction specification start */
unsigned int global_bch_hardCorrectable = DEFAULT_VALUE_BCH_HARD_CORRECTABLE;

unsigned int global_bch_SoftCorrectable = DEFAULT_VALUE_BCH_SOFT_CORRECTABLE;
/* Error correction specification end */

unsigned int global_bch_Soft_OverSyndLen = DEFAULT_VALUE_BCH_SOFT_OVER_SYND;
/* flags for generating galois field(GF) end */

/* non-categorized flags start */
char *global_AddedRoot = NULL;  //notused
/* non-categorized flags end */

/* Operation */
void GLOBAL_flag_proc()
{
    unsigned int G_FP_tmp_i;
    unsigned int G_FP_tmp_cnt;

    if(!(global_flag_sim_decodingMode&FLAG_MASK_CODE_BCH_DEC_AL_TEST_THIS_ALGORITHM))
    {
        infoMes; printf("Global algorithm verification mode is not enabled.\r\n");
        infoMes; printf("Start checking some decoding algorithm verification mode enabled.\r\n");

        G_FP_tmp_cnt=0;
        for(G_FP_tmp_i=0; G_FP_tmp_i<KIND_OF_BCH_DECODING_END; G_FP_tmp_i++)
        {

            if(global_list_flag_bch_sim_decodingMode[G_FP_tmp_i]&FLAG_MASK_CODE_BCH_DEC_AL_TEST_THIS_ALGORITHM)
            {
                G_FP_tmp_cnt++;
                global_flag_bchDecAlgo_Enable |= uint32_bitMask[G_FP_tmp_i];
                global_list_flag_bch_sim_decodingMode[G_FP_tmp_i] &= ~FLAG_MASK_CODE_BCH_DEC_AL_HARD_DECISION;
                global_list_flag_bch_sim_decodingMode[G_FP_tmp_i] &= ~FLAG_MASK_CODE_BCH_DEC_AL_SOFT_DECISION;
                infoMes; printf("%s algorithm verification mode enabled.\r\n", KIND_OF_BCH_DECODING_ALGORITHM_NAME[G_FP_tmp_i]);
                infoMes; printf("Even if [%s] algorithm do simulation using soft and hard decision mode are enabled, they are ignored.\r\n",
                KIND_OF_BCH_DECODING_ALGORITHM_NAME[G_FP_tmp_i]);
            }
        }

        if(G_FP_tmp_cnt)
        {
            global_flag_sim_decodingMode |= FLAG_MASK_CODE_BCH_DEC_AL_TEST_THIS_ALGORITHM;
            global_flag_sim_decodingMode &= ~FLAG_MASK_CODE_BCH_DEC_AL_SOFT_DECISION;
            global_flag_sim_decodingMode &= ~FLAG_MASK_CODE_BCH_DEC_AL_HARD_DECISION;
            infoMes; printf("Global algorithm verification mode is enabled.\r\n");

        }
        else
        {
            infoMes; printf("Any algorithms are not enabled to algorithm verification mode.\r\n");
            infoMes; printf("Global verification mode is disabled.\r\n");
        }
    }
    else
    {
        infoMes; printf("Global algorithm verification mode is enabled.\r\n");
        infoMes; printf("If some algorithm is enabled to hard or soft decision or decording algorithm is enabled, they would be change a mode to verification mode.\r\n");
        for(G_FP_tmp_i=0; G_FP_tmp_i<KIND_OF_BCH_DECODING_END; G_FP_tmp_i++)
        {
            if(
                global_flag_bchDecAlgo_Enable & uint32_bitMask[G_FP_tmp_i] ||
                global_list_flag_bch_sim_decodingMode[G_FP_tmp_i] & FLAG_MASK_CODE_BCH_DEC_AL_TEST_THIS_ALGORITHM ||
                global_list_flag_bch_sim_decodingMode[G_FP_tmp_i] & FLAG_MASK_CODE_BCH_DEC_AL_HARD_DECISION ||
                global_list_flag_bch_sim_decodingMode[G_FP_tmp_i] & FLAG_MASK_CODE_BCH_DEC_AL_SOFT_DECISION
            )
            {
                global_flag_bchDecAlgo_Enable |= uint32_bitMask[G_FP_tmp_i];
                global_list_flag_bch_sim_decodingMode[G_FP_tmp_i] |= FLAG_MASK_CODE_BCH_DEC_AL_TEST_THIS_ALGORITHM;
                global_list_flag_bch_sim_decodingMode[G_FP_tmp_i] &= ~FLAG_MASK_CODE_BCH_DEC_AL_HARD_DECISION;
                global_list_flag_bch_sim_decodingMode[G_FP_tmp_i] &= ~FLAG_MASK_CODE_BCH_DEC_AL_SOFT_DECISION;

                infoMes; printf("%s algorithm verification mode enabled.", KIND_OF_BCH_DECODING_ALGORITHM_NAME[G_FP_tmp_i]);
            }
        }
        global_flag_sim_decodingMode &= ~FLAG_MASK_CODE_BCH_DEC_AL_SOFT_DECISION;
        global_flag_sim_decodingMode &= ~FLAG_MASK_CODE_BCH_DEC_AL_HARD_DECISION;
        infoMes; printf("Global Soft and Hard Decision mode modes are disabled..\r\n");
    }
    printf("\r\n");



    if(!(global_flag_sim_decodingMode&FLAG_MASK_CODE_BCH_DEC_AL_TEST_THIS_ALGORITHM))
    {
        if(!(global_flag_sim_decodingMode&FLAG_MASK_CODE_BCH_DEC_AL_SOFT_DECISION))
        {
            infoMes; printf("Global Soft Decision mode is not enabled.\r\n");
            infoMes; printf("Start checking some decoding algorithm Soft Decision mode enabled.\r\n");

            G_FP_tmp_cnt=0;
            for(G_FP_tmp_i=0; G_FP_tmp_i<KIND_OF_BCH_DECODING_END; G_FP_tmp_i++)
            {

                if(global_list_flag_bch_sim_decodingMode[G_FP_tmp_i]&FLAG_MASK_CODE_BCH_DEC_AL_SOFT_DECISION)
                {
                    G_FP_tmp_cnt++;
                    global_flag_bchDecAlgo_Enable |= uint32_bitMask[G_FP_tmp_i];
                    global_list_flag_bch_sim_decodingMode[G_FP_tmp_i] |= FLAG_MASK_CODE_BCH_DEC_AL_HARD_DECISION;
                    global_list_flag_bch_sim_decodingMode[G_FP_tmp_i] |= FLAG_MASK_CODE_BCH_DEC_AL_SOFT_DECISION;

                    infoMes; printf("%s algorithm Soft Decision mode enabled.", KIND_OF_BCH_DECODING_ALGORITHM_NAME[G_FP_tmp_i]);
                }
            }

            if(G_FP_tmp_cnt)
            {
                global_flag_sim_decodingMode |= FLAG_MASK_CODE_BCH_DEC_AL_SOFT_DECISION;
                global_flag_sim_decodingMode |= FLAG_MASK_CODE_BCH_DEC_AL_HARD_DECISION;

                infoMes; printf("Global Soft Decision mode is enabled.\r\n");
            }
            else
            {
                infoMes; printf("Any algorithms are not enabled to Soft Decision mode.\r\n");
                infoMes; printf("Global Soft Decision mode is disabled.\r\n");
            }
        }
        else
        {
            infoMes; printf("Global Soft Decision mode is enabled.\r\n");
            infoMes; printf("If some algorithm is enabled to hard or soft decision or decording algorithm is enabled, \r\n\tthey would be change a mode to Soft Decision mode.\r\n");

            for(G_FP_tmp_i=0; G_FP_tmp_i<KIND_OF_BCH_DECODING_END; G_FP_tmp_i++)
            {

                if(
                    global_flag_bchDecAlgo_Enable & uint32_bitMask[G_FP_tmp_i] ||
                    global_list_flag_bch_sim_decodingMode[G_FP_tmp_i] & FLAG_MASK_CODE_BCH_DEC_AL_HARD_DECISION ||
                    global_list_flag_bch_sim_decodingMode[G_FP_tmp_i] & FLAG_MASK_CODE_BCH_DEC_AL_SOFT_DECISION
                )
                {
                    global_flag_bchDecAlgo_Enable |= uint32_bitMask[G_FP_tmp_i];
                    global_list_flag_bch_sim_decodingMode[G_FP_tmp_i] |= FLAG_MASK_CODE_BCH_DEC_AL_HARD_DECISION;
                    global_list_flag_bch_sim_decodingMode[G_FP_tmp_i] |= FLAG_MASK_CODE_BCH_DEC_AL_SOFT_DECISION;

                    infoMes; printf("%s algorithm Soft Decision mode enabled.\r\n", KIND_OF_BCH_DECODING_ALGORITHM_NAME[G_FP_tmp_i]);

                }
            }

            global_flag_sim_decodingMode |= FLAG_MASK_CODE_BCH_DEC_AL_SOFT_DECISION;
            global_flag_sim_decodingMode |= FLAG_MASK_CODE_BCH_DEC_AL_HARD_DECISION;

            infoMes; printf("Global Soft and Hard Decision modes are enabled.\r\n");
        }
    }
    printf("\r\n");


    if(!(global_flag_sim_decodingMode&FLAG_MASK_CODE_BCH_DEC_AL_SOFT_DECISION))
    {
        if(!(global_flag_sim_decodingMode&FLAG_MASK_CODE_BCH_DEC_AL_HARD_DECISION))
        {
            infoMes; printf("Global Hard Decision mode is not enabled.\r\n");
            infoMes; printf("Start checking some decoding algorithm Hard Decision mode enabled.\r\n");

            G_FP_tmp_cnt=0;
            for(G_FP_tmp_i=0; G_FP_tmp_i<KIND_OF_BCH_DECODING_END; G_FP_tmp_i++)
            {
                if(global_list_flag_bch_sim_decodingMode[G_FP_tmp_i]&FLAG_MASK_CODE_BCH_DEC_AL_HARD_DECISION)
                {
                    G_FP_tmp_cnt++;
                    global_flag_bchDecAlgo_Enable |= uint32_bitMask[G_FP_tmp_i];
                    global_list_flag_bch_sim_decodingMode[G_FP_tmp_i] |= FLAG_MASK_CODE_BCH_DEC_AL_HARD_DECISION;

                    infoMes; printf("%s algorithm Hard Decision mode enabled.", KIND_OF_BCH_DECODING_ALGORITHM_NAME[G_FP_tmp_i]);
                    infoMes; printf("[%s] algorithm do simulation using hard decision mode are enabled.",
                        KIND_OF_BCH_DECODING_ALGORITHM_NAME[G_FP_tmp_i]);

                }
            }

            if(G_FP_tmp_cnt)
            {
                global_flag_sim_decodingMode |= FLAG_MASK_CODE_BCH_DEC_AL_HARD_DECISION;

                infoMes; printf("Global Hard Decision mode is enabled.\r\n");
            }
            else
            {
                infoMes; printf("Any algorithms are not enabled to Hard Decision mode.\r\n");
                infoMes; printf("Global Hard Decision mode is disabled.\r\n");
            }
        }
        else
        {
            for(G_FP_tmp_i=0; G_FP_tmp_i<KIND_OF_BCH_DECODING_END; G_FP_tmp_i++)
            {
                infoMes; printf("Global Hard Decision mode is enabled.\r\n");
                infoMes; printf("If some algorithm is enabled to hard decision or decording algorithm is enabled, they would be change a mode to Soft Decision mode.\r\n");

                if(
                    global_flag_bchDecAlgo_Enable & uint32_bitMask[G_FP_tmp_i] ||
                    global_list_flag_bch_sim_decodingMode[G_FP_tmp_i] & FLAG_MASK_CODE_BCH_DEC_AL_HARD_DECISION
                )
                {
                    global_flag_bchDecAlgo_Enable |= uint32_bitMask[G_FP_tmp_i];
                    global_list_flag_bch_sim_decodingMode[G_FP_tmp_i] |= FLAG_MASK_CODE_BCH_DEC_AL_HARD_DECISION;

                    infoMes; printf("%s algorithm Hard Decision mode enabled.", KIND_OF_BCH_DECODING_ALGORITHM_NAME[G_FP_tmp_i]);
                }
            }

            global_flag_sim_decodingMode |= FLAG_MASK_CODE_BCH_DEC_AL_HARD_DECISION;

            infoMes; printf("Global Hard Decision mode modes are enabled.\r\n");
        }
    }
    printf("\r\n");

    /*
    //        global_flag_sim_decodingMode |= FLAG_MASK_CODE_BCH_DEC_AL_TEST_THIS_ALGORITHM;
    //        global_flag_sim_decodingMode &= ~FLAG_MASK_CODE_BCH_DEC_AL_SOFT_DECISION;
    //        global_flag_sim_decodingMode &= ~FLAG_MASK_CODE_BCH_DEC_AL_HARD_DECISION;
    */
    printf("/*****************************************************************/\r\n");

    infoMes; printf("Simulator Mode : %s\r\n",
    global_flag_sim_decodingMode&FLAG_MASK_CODE_BCH_DEC_AL_TEST_THIS_ALGORITHM?"Algorithm Verification Mode":
    global_flag_sim_decodingMode&FLAG_MASK_CODE_BCH_DEC_AL_SOFT_DECISION?"Soft Decision Mode":
    global_flag_sim_decodingMode&FLAG_MASK_CODE_BCH_DEC_AL_HARD_DECISION?"hard Decision Mode":"Unknown"
    );

    for(G_FP_tmp_i=0; G_FP_tmp_i<KIND_OF_BCH_DECODING_END; G_FP_tmp_i++)
    {
        if(global_flag_bchDecAlgo_Enable & uint32_bitMask[G_FP_tmp_i])
        {
            infoMes; printf("%s decoding algorithm [Enabled]\r\n", KIND_OF_BCH_DECODING_ALGORITHM_NAME[G_FP_tmp_i]);
        }

        if(global_list_flag_bch_sim_decodingMode[G_FP_tmp_i] & FLAG_MASK_CODE_BCH_DEC_AL_TEST_THIS_ALGORITHM)
        {
            infoMes; printf("%s decoding algorithm [Test Algorithm Mode Enabled]\r\n", KIND_OF_BCH_DECODING_ALGORITHM_NAME[G_FP_tmp_i]);
        }

        if(global_list_flag_bch_sim_decodingMode[G_FP_tmp_i] & FLAG_MASK_CODE_BCH_DEC_AL_SOFT_DECISION)
        {
            infoMes; printf("%s decoding algorithm [Soft Decision Mode Enabled]\r\n", KIND_OF_BCH_DECODING_ALGORITHM_NAME[G_FP_tmp_i]);
        }

        if(global_list_flag_bch_sim_decodingMode[G_FP_tmp_i] & FLAG_MASK_CODE_BCH_DEC_AL_HARD_DECISION)
        {
            infoMes; printf("%s decoding algorithm [Hard Decision Mode Enabled]\r\n", KIND_OF_BCH_DECODING_ALGORITHM_NAME[G_FP_tmp_i]);
        }
    }
    printf("/*****************************************************************/\r\n");
}
