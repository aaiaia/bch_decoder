#ifndef _COMMON_CMD_OPT_VARS_H_
#define _COMMON_CMD_OPT_VARS_H_

#include <stddef.h>

/* Flags for Debugging(Displaying) start */
/* Log definition */
#define errorMes        printf("ERROR ) ")
#define warningMes      printf("/*********************************/\n/********** WARNING !!! **********/\n/*********************************/\nWARNING ) ")
#define warningMesShort printf("WARNING ) ")
#define infoMes         printf("Info ) ")

#define initMes         printf("Initial ) ")

#define resultMes       printf("Result ) ")

/* Flags for log*/
extern unsigned int global_flag_cmdOption;
#define PRINTF_LOG_WORD                         "l" //not used
#define FLAG_MASK_PRINTF_LOG                    (1<<0)
#define logMes                                  printf("LOG ) ")

#define PRINTF_CALCULATE_PROCESS_WORD           "c" //not used
#define FLAG_MASK_PRINTF_CALCULATE_PROCESS      (1<<1)
#define calculateMes                            printf("CALCULATE ) ")

#define PRINTF_MEMORY_CHECK_PROCESS_WORD        "m" //not used
#define FLAG_MASK_PRINTF_MEMORY_CHECK_PROCESS   (1<<2)
#define memoryMes                               printf("MEMORY ) ")

#define IGNORE_WARNING_MES_WORD                 "w" //not used
#define FLAG_MASK_PRINTF_WARNING_MES            (1<<3)
    /* variables to check memory lineary */
    extern unsigned int global_tmp_i_memoryCheck;

/* Sysgem Hold command */
extern unsigned int global_flag_cmdSystemHold;
#define PRINTF_HELP_WORD    "h" //not used
#define FLAG_MASK_HOLD_HELP (1<<0)
#define systemHoldMes       printf("HOLD ) ")

/* forced Sysgem unhold command */
extern unsigned int global_flag_cmdSystemUnhold;
#define SYSTEM_UNHOLD_FORCED_WORD       "f" //not used
#define FLAG_MASK_SYSTEM_UNHOLD_FORCED  (1<<0)
#define systemUnholdMes                 printf("UNHOLD ) ")

    /* Algorithm debugging options start */
    /* BM Algorithm debug option start */
    extern unsigned int global_flag_debug_bmSim;
    #define DEBUG_BCH_BM_ALGORITHM_WORD         "debug-bm"  //not used
    #define FLAG_MASK_DEBUG_BCH_BM_ALGORITHM    (1<<0)
    #define debugBmAlgorithmMes                 printf("debug-BM ) ");

    #define DEBUG_BCH_BM_CAL_DELTA_WORD         "debug-bm-cal-delta"    //not used
    #define FLAG_MASK_DEBUG_BCH_BM_CAL_DELTA    (1<<1)
    #define debugBmAlgorithmCalDeltaMes         printf("debug-BM-cal-delta ) ");
    /* BM Algorithm debug option end */

    /*mSBS Algorithm debug option start*/
    /* below variables could be unnecessary */
    extern unsigned int global_flag_debug_mSBS_Sim;
    #define DEBUG_BCH_mSBS_ALGORITHM_WORD       "debug-mSBS"    //not used
    #define FLAG_MASK_DEBUG_BCH_mSBS_ALGORITHM  (1<<0)
    #define debug_mSBS_AlgorithmMes             printf("debug-mSBS ) ");

    #define DEBUG_BCH_M_SBS_CAL_DELTA_WORD      "debug-mSBS-cal-delta"  //not used
    #define FLAG_MASK_DEBUG_BCH_M_SBS_CAL_DELTA (1<<1)
    #define debug_mSBS_AlgorithmCalDeltaMes     printf("debug-mSBS-cal-delta ) ");
    /*mSBS Algorithm debug option end*/
    /* Algorithm debugging options end */

/* Global debug option start */
extern unsigned int global_flag_deBugOption; //not used
#define DEBUG_DEBUG_WORD        "debug" //not used
#define FLAG_MASK_DEBUG_DEBUG   (1<<0)  //used with global_flag_debug_display
#define debugMes                printf("debug ) ");

    /*Display RESULT or Progress or Function name*/
    enum DEBUG_DISPLAY_MODE
    {
        FLAG_MASK_DISPLAY_RESULT    = (1<<0),
        FLAG_MASK_DISPLAY_PROGRESS  = (1<<1),
        FLAG_MASK_DISPLAY_FUNCNAME  = (1<<2),
    };

    /* flags using enum DEBUG_DISPLAY_MODE start */
    extern unsigned int global_flag_debug_display;
    #define debugFuncNameMes    printf("debug-funcname ) ");
    #define debugSequenceMes    printf("debug-progress ) ");

    extern unsigned int global_flag_bch_BM_SimHD_display;
    #define test_BM_HardDisplayMes  printf("test BCH BM HARD DISPLAY ) ")

    extern unsigned int global_flag_bch_BM_SimSD_display;
    #define test_BM_SoftDisplayMes  printf("test SBCH BM SOFT DISPLAY ) ");

    extern unsigned int global_flag_bch_mSBS_SimHD_display;
    #define test_mSBS_HardMes   printf("test BCH mSBS HARD ) ");

    extern unsigned int global_flag_bch_mSBS_SimSD_display;
    #define test_mSBS_SoftMes   printf("test BCH mSBS SOFT ) ");
    #define test_mSBS_SoftDisplayMes    printf("test BCH mSBS  SOFT DISPLAY ) ");

    extern unsigned int global_flag_Modulation_Mode_BPSK_Display;

    extern unsigned int global_flag_case_getBER_method_Display;
    /* flags using enum DEBUG_DISPLAY_MODE end */

/* ber option start */
extern unsigned int global_flag_berOptions;
#define FLAG_MASK_TEST_GET_BER  (1<<0)
#define testGetBerMes           printf("test GET-BER ) ");
/* ber option end */

/* Log LikeHood Ratio debug option start */
extern unsigned int global_flag_debug_awgnLLR;
#define FLAG_MASK_DEBUG_AWGN_LLR_SEQUENCE   (1<<0)
#define debugAwgnLlrSequenceMes             printf("SEQUENCE LLR ) ");

#define FLAG_MASK_DEBUG_AWGN_LLR_LOCATOR    (1<<1)
#define debugAwgnLlrLocatorMes              printf("debug-LLR-Locator ) ");
/* Log LikeHood Ratio debug option end */

/* Display Galois Field(GF) start */
extern unsigned int global_flag_gfCommon_display;
enum FLAG_MASK_DISPLAY_GF_COMMON
{
    FLAG_MASK_DISPLAY_GF_COMMON_GENPOLY     = (1<<0),//if(global_flag_gfCommon_display&FLAG_MASK_DISPLAY_GF_COMMON_GENPOLY)
    FLAG_MASK_DISPLAY_GF_COMMON_CONJUGACIES = (1<<2),//if(global_flag_gfCommon_display&FLAG_MASK_DISPLAY_GF_COMMON_CONJUGACIES)
    FLAG_MASK_DISPLAY_GF_COMMON_FIELD       = (1<<1),//if(global_flag_gfCommon_display&FLAG_MASK_DISPLAY_GF_COMMON_FIELD)
    FLAG_MASK_DISPLAY_GF_COMMON_PRIMEPOLY   = (1<<3),//if(global_flag_gfCommon_display&FLAG_MASK_DISPLAY_GF_COMMON_PRIMEPOLY)
    FLAG_MASK_DISPLAY_GF_COMMON_SUM_MATRIX  = (1<<4),//if(global_flag_gfCommon_display&FLAG_MASK_DISPLAY_GF_COMMON_SUM_MATRIX)
    FLAG_MASK_DISPLAY_GF_COMMON_FILE_IO     = (1<<5),//if(global_flag_gfCommon_display&FLAG_MASK_DISPLAY_GF_COMMON_FILE_IO)
};
/* Display Galois Field(GF) end */
/* Global debug option end */
/* Flags for Debugging(Displaying) end */

/* Flags for simulation mode start */
    /* global flag associate with selecting test pattern start */
    enum FLAG_CASE_SEL_TP_BEFORE_DEC_METHOD
    {
        FLAG_CASE_SEL_TP_BEFORE_DEC_NONE,
        FLAG_CASE_SEL_TP_BEFORE_DEC_THEO,
        FLAG_CASE_SEL_TP_BEFORE_DEC_ACT,
        FLAG_CASE_SEL_TP_BEFORE_DEC_END,
    }typedef enum_FLAG_CASE_SEL_TP_BEFORE_DEC_METHOD;

    extern const char NAME_FLAG_CASE_SEL_TP_BEFORE_DEC_MTEHOD_FILE_ADDED
                    [FLAG_CASE_SEL_TP_BEFORE_DEC_END][51];
    /* global flag associate with selecting test pattern end */

    extern unsigned int global_flag_case_selTP_before_dec;

    #define FLAG_CASE_DECODING_FAIL_METHOD_DEFAULT  FLAG_CASE_SEL_TP_BEFORE_DEC_THEO

    /* subflags in FLAG_CASE_SEL_TP_BEFORE_DEC_THEO start */
    /*
     * global flag to selecting test pettern.
     * It is working only in
     * FLAG_CASE_SEL_TP_BEFORE_DEC_THEO mode.
     */
    extern unsigned int global_flag_case_choosingCodeWordMethod_inTheo;
    enum FLAG_CASE_CHOOSEN_CODWORD_METHOD
    {
        FLAG_CASE_CODEWORD_CHOOSEN_METHOD_IN_THEO_NONE,
        /* not-support */
        FLAG_CASE_CODEWORD_CHOOSEN_METHOD_IN_THEO_NO_ERR,
        /* support */
        FLAG_CASE_CODEWORD_CHOOSEN_METHOD_IN_THEO_NORMAL,
        FLAG_CASE_CODEWORD_CHOOSEN_METHOD_IN_THEO_MIN_ERR,
        FLAG_CASE_CODEWORD_CHOOSEN_METHOD_IN_THEO_MAX_ERR,

        FLAG_CASE_CODEWORD_CHOOSEN_METHOD_IN_THEO_DEFAULT = FLAG_CASE_CODEWORD_CHOOSEN_METHOD_IN_THEO_NORMAL,
    }typedef FLAG_CASE_CHOOSEN_CODWORD_METHOD;
    /* subflags in FLAG_CASE_SEL_TP_BEFORE_DEC_THEO end */

    /* sub-flags in FLAG_CASE_SEL_TP_BEFORE_DEC_ACT start */
    /*
     * In FLAG_CASE_SEL_TP_BEFORE_DEC_ACT mode(which is non-correctable case),
     * select handling method
     */
    extern unsigned int global_flag_case_dec_fail_handling;

    /* Method that re transfer is not supported yet */
    enum FLAG_CASE_DEC_FAIL_HANDLING
    {
        FLAG_CASE_DEC_FAIL_HANDLING_NONE,
        FLAG_CASE_DEC_FAIL_HANDLING_RE_TRANFER_FAIL_NO_MORE,
        FLAG_CASE_DEC_FAIL_HANDLING_RE_TRANFER_INTERATION,//not support yet
    }typedef enum_FLAG_CASE_DEC_FAIL_HANDLING;
    /* sub-flags in FLAG_CASE_SEL_TP_BEFORE_DEC_ACT end */
    /****** global flag associate with decoding end *****/

/* global flags error correction codes start */
/* selection Code mode start */
extern unsigned int global_flag_simulationCode;
/*Channel Setting, LLR quantization bit setting*/
#define FLAG_MASK_CODE_BCH  (1<<0)
#define mes_bchCode         printf("BCH code) ");
/* selection Code mode start */

    /* sub-flags in FLAG_MASK_CODE_BCH start */
    enum KIND_OF_BCH_DECODING_ALGORITHM
    {
        KIND_OF_BCH_DECODING_BM,
        //KIND_OF_BCH_DECODING_mSBS,
        KIND_OF_BCH_DECODING_iBM,
        KIND_OF_BCH_DECODING_RiBM,
        KIND_OF_BCH_DECODING_SiBM,
        KIND_OF_BCH_DECODING_mSBS,
        //KIND_OF_BCH_DECODING_BM,
        KIND_OF_BCH_DECODING_PETERSON,
        KIND_OF_BCH_DECODING_EUCLIDIAN,
        KIND_OF_BCH_DECODING_END,
    };

    extern const char KIND_OF_BCH_DECODING_ALGORITHM_NAME[KIND_OF_BCH_DECODING_END][21];

    /* bch enable algorithm list */
    extern unsigned int global_flag_bchDecAlgo_Enable;
    /*CODE LENGTH(GALOIS FIELD), symbols(BCH not needs)*/

    enum BCH_DECODING_ALGORITHM
    {
        FLAG_MASK_BCH_DECORDING_ALGORITHM_BM        = (1<<KIND_OF_BCH_DECODING_BM),
        FLAG_MASK_BCH_DECORDING_ALGORITHM_iBM       = (1<<KIND_OF_BCH_DECODING_iBM),
        FLAG_MASK_BCH_DECORDING_ALGORITHM_RiBM      = (1<<KIND_OF_BCH_DECODING_RiBM),
        FLAG_MASK_BCH_DECORDING_ALGORITHM_SiBM      = (1<<KIND_OF_BCH_DECODING_SiBM),
        FLAG_MASK_BCH_DECORDING_ALGORITHM_M_SBS     = (1<<KIND_OF_BCH_DECODING_mSBS),
        FLAG_MASK_BCH_DECORDING_ALGORITHM_PETERSON  = (1<<KIND_OF_BCH_DECODING_PETERSON),
        FLAG_MASK_BCH_DECORDING_ALGORITHM_EUCLIDIAN = (1<<KIND_OF_BCH_DECODING_EUCLIDIAN),
    };

    /* Simulation Working Mode */
    extern unsigned int global_flag_sim_workingMode;   //not used
    enum SIMULATION_WORKING_MODE    //not used
    {
        FLAG_MASK_SIM_DECOING       = (1<<0),
        FLAG_MASK_SIM_ABSTRACT_DATA = (1<<1),//reserved Working mode
        FLAG_MASK_SIM_TEST          = (1<<2),
    };

    /* global_list_flag_bch_sim_decodingMode */
    extern unsigned int global_flag_sim_decodingMode;
    extern unsigned int global_list_flag_bch_sim_decodingMode[KIND_OF_BCH_DECODING_END];
        /*******************************************************************/
        /********** global flag associate wieh bch - BM algorithm **********/
        /*******************************************************************/
        enum BCH_ALGORITHM_COMMON_DECODING_OPTIONS
        {
            FLAG_MASK_CODE_BCH_DEC_AL_TEST_THIS_ALGORITHM   = (1<<0),
            FLAG_MASK_CODE_BCH_DEC_AL_HARD_DECISION         = (1<<1),
            FLAG_MASK_CODE_BCH_DEC_AL_SOFT_DECISION         = (1<<2),

            FLAG_MASK_LRB_LLR_TEST                          = (1<<3),

            FLAG_MASK_CODE_BCH_DEC_AL_SKIP                  = (1<<15),
        };
    /* sub-flags in FLAG_MASK_CODE_BCH end */
/* global flags error correction codes end */
/* Flags for simulation mode end */

/* Flags to output test pattern start */
#define FLAG_MASK_FILE_IO    (1<<0)
extern unsigned int global_flag_fileInputOutput;

    extern unsigned int global_flag_file_io_hd_pattern_output;
    extern unsigned int global_flag_file_io_hd_pattern_output_log;
    extern unsigned int global_flag_file_io_hd_pattern_output_verilog;
    enum HD_PATTERN_OUTPUT
    {
        FLAG_MASK_FILE_IO_HD_MES            = (1<<0),
        FLAG_MASK_FILE_IO_HD_ENCODING_MES   = (1<<1),
        FLAG_MASK_FILE_IO_HD_RECEIVED_MES   = (1<<2),
        FLAG_MASK_FILE_IO_HD_CORRECTED_MES  = (1<<3),
    };

    extern unsigned int global_flag_file_io_sd_pattern_output;
    extern unsigned int global_flag_file_io_sd_pattern_output_log;
    extern unsigned int global_flag_file_io_sd_pattern_output_verilog;
    enum SD_PATTERN_OUTPUT
    {
        FLAG_MASK_FILE_IO_SD_RECEIVED_LLR       = (1<<0),
        FLAG_MASK_FILE_IO_SD_RECEIVED_LLR_mag   = (1<<1),
    };
/* Flags to output test pattern end */

/* Flags about Modulation mode(bpsk, qpsk, qam etc...) start */
extern unsigned int global_flag_Modulation_Mode;
#define FLAG_MASK_MODULATION_MODE_BPSK  (1<<0)                        //if(global_flag_Modulation_Mode&FLAG_MASK_MODULATION_MODE_BPSK)
#define DEFAULT_MODULATION_MODE         FLAG_MASK_MODULATION_MODE_BPSK
/* Flags about Modulation mode(bpsk, qpsk, qam etc...) end */

/* Flags about AWGN(channel noise) start */
#define VALUE_EBN0_WORD         "ebn0"//SET_VALUE
/*
//extern double main_com_EbN0;
*/
#define DEFAULT_VALUE_EBN0      1.0f
extern double global_EbN0_Start_Value;

#define VALUE_EBN0_END_WORD     "ebn0-end"//SET_VALUE
#define DEFAULT_VALUE_EBN0_END  8.0f
extern double global_EbN0_End_Value;

#define VALUE_EBN0_STEP_WORD    "ebn0-step"//SET_VALUE
#define DEFAULT_VALUE_EBN0_STEP 0.25f
extern double global_EbN0_Step;


extern unsigned int global_flag_debug_awgn;
#define FLAG_MASK_DEBUG_AWGN_SEQUENCE   (1<<0)
#define debugAwgnSequenceMes            printf("SEQUENCE AWGN ) ");
/* Flags about AWGN(channel noise) end */

/* Flags to LLR calculation start */
#define FLAG_MASK_CHANNEL_MODE_AWGN    (1<<0)   //not used

#define DEFAULT_CHANNEL_MODE    FLAG_MASK_CHANNEL_MODE_AWGN //not used
extern unsigned int global_Channel_Mode;    //not used

    enum ENUM_GLO_CH_MODE_FUNC
    {
        FLAG_MASK_CHANNEL_MODE_FUNC_NONE                = 0,
        FLAG_MASK_CHANNEL_MODE_FUNC_BITRATE_DISABLE     = (1<<0),//if(global_flag_Channel_Mode_Func&FLAG_MASK_CHANNEL_MODE_FUNC_BITRATE_DISABLE)
        FLAG_MASK_CHANNEL_MODE_FUNC_RANDOM_SEED_DISABLE = (1<<1),//if(global_flag_Channel_Mode_Func&FLAG_MASK_CHANNEL_MODE_FUNC_RANDOM_SEED_DISABLE)
    }typedef ENUM_GLO_CH_MODE_FUNC;

    extern unsigned int global_flag_Channel_Mode_Func;
/* Flags to LLR calculation end */

/* Flags about Log Likehood Ratio(LLR) processong start */
extern unsigned int global_flag_llr_display;
enum FLAG_LLR_DISPLAY
{
    FLAG_LLR_DISPLAY_NONE                   = 0,
    FLAG_LLR_DISPLAY_QUANTIZATION_FILTERING = (1<<0),
}typedef enum_FLAG_LLR_DISPLAY;

enum FLAG_CASE_FINDING_MIN_METHOD
{
    FLAG_CASE_FINDING_MIN_LLR_METHOD_NONE,
    FLAG_CASE_FINDING_MIN_LLR_METHOD_MINIMUM,
    FLAG_CASE_FINDING_MIN_LLR_METHOD_GROUPING,
    FLAG_CASE_FINDING_MIN_LLR_METHOD_TREE_FOLLOWING_MIN1_PATH,
    FLAG_CASE_FINDING_MIN_LLR_METHOD_TREE_PROBABILISIC_CHOOSEN, //NOT SUPPORT YET
    FLAG_CASE_FINDING_MIN_LLR_METHOD_TREE_PRIOR_HD_ZERO,        //NOT SUPPORT YET
    FLAG_CASE_FINDING_MIN_LLR_METHOD_END,
};
extern const char NAME_FLAG_CASE_FINDING_MIN_MTEHOD[FLAG_CASE_FINDING_MIN_LLR_METHOD_END][51];
extern const char NAME_FLAG_CASE_FINDING_MIN_MTEHOD_FILE_ADDED[FLAG_CASE_FINDING_MIN_LLR_METHOD_END][51];


#define FLAG_CASE_FINDING_MIN_LLR_METHOD_DEFAULT    FLAG_CASE_FINDING_MIN_LLR_METHOD_TREE_FOLLOWING_MIN1_PATH
extern unsigned int global_flag_case_find_LLR_method;

    extern unsigned int global_grouping_stream_nums;
    extern unsigned int global_group_bit_num;
    extern unsigned int global_group_last_bit_num;

/* pass HD 1 when llr mag is zero start */
enum FLAG_CASE_PASS_MAG0_HD_1_MEAN_NOT_CHOOSEN_LOW_LLR
{
    FLAG_CASE_PASS_MAG0_HD_1_DISABLE,
    FLAG_CASE_PASS_MAG0_HD_1_ENABLE,
    FLAG_CASE_PASS_MAG0_HD_1_END,
};
extern const char NAME_FLAG_CASE_PASS_MAG0_HD_1[FLAG_CASE_PASS_MAG0_HD_1_END][71];

extern unsigned int global_flag_case_pass_hd_1;
/*  pass HD 1 when llr mag is zero end  */

enum FLAG_CASE_SAME_LLR_HANDLING
{
    FLAG_CASE_SAME_LLR_HANDLING_NONE,
    FLAG_CASE_SAME_LLR_HANDLING_NOT_SEL,
    FLAG_CASE_SAME_LLR_HANDLING_PRIOR_HD_0,
    FLAG_CASE_SAME_LLR_HANDLING_PRIOR_HD_1,
    FLAG_CASE_SAME_LLR_HANDLING_END,
};
extern const char NAME_FLAG_CASE_SAME_LLR_HANDLING[FLAG_CASE_SAME_LLR_HANDLING_END][31];
extern unsigned int global_flag_case_same_llr_handling;

enum FLAG_CASE_INIT_LLR_MAG_METHOD
{
    FLAG_CASE_INIT_LLR_MAG_METHOD_NONE,
    FLAG_CASE_INIT_LLR_MAG_METHOD_NOT_SEL_MAX_LLR,
    FLAG_CASE_INIT_LLR_MAG_METHOD_END,
};
extern const char NAME_FLAG_CASE_INIT_LLR_MAG_METHOD[FLAG_CASE_INIT_LLR_MAG_METHOD_END][51];
extern unsigned int global_flag_case_init_llr_mag_method;

/* LLR QUANTIZATION FILTER options start */
enum ENUM_FLAG_CASE_QUANTIZ_NUMBERIC_SYSTEM
{
    ENUM_FLAG_CASE_QUANTIZ_NUMBERIC_SYSTEM_NONE,
    ENUM_FLAG_CASE_QUANTIZ_NUMBERIC_SYSTEM_TWO_S_COMPLEMENT,
    ENUM_FLAG_CASE_QUANTIZ_NUMBERIC_SYSTEM_ONE_S_COMPLEMENT,
    ENUM_FLAG_CASE_QUANTIZ_NUMBERIC_SYSTEM_END,
};
extern const char NAME_QUANTIZ_NUMBERIC_SYSTEM[ENUM_FLAG_CASE_QUANTIZ_NUMBERIC_SYSTEM_END][51];

extern unsigned int global_flag_case_llr_numberic_system;

enum ENUM_FLAG_CASE_QUANTIZ_ONE_S_COMPLEMENT_ZERO_HANDLING
{
    ENUM_FLAG_CASE_QUANTIZ_NUMBERIC_SYS_ONE_S_COM_NONE,
    ENUM_FLAG_CASE_QUANTIZ_NUMBERIC_SYS_ONE_S_COM_ZERO_SYMMETRY,
    ENUM_FLAG_CASE_QUANTIZ_NUMBERIC_SYS_ONE_S_COM_ZERO_DUPLICATED,
    ENUM_FLAG_CASE_QUANTIZ_NUMBERIC_SYS_ONE_S_COM_END,

};

extern const char NAME_QUANTIZ_ONE_S_COMPLEMENT_ZERO_HANDLING \
    [ENUM_FLAG_CASE_QUANTIZ_NUMBERIC_SYS_ONE_S_COM_END][21];

extern unsigned int global_flag_case_llr_mag_ones_complement_zero_handling;

/* LLR QUANTIZATION FILTER VARIABLES start */
extern double global_LLR_quantizFilter_offset;
extern double global_LLR_quantizFilter_step;
extern unsigned char global_LLR_quantizFilter_bitWidth;
/* LLR QUANTIZATION FILTER VARIABLES end */
/* LLR QUANTIZATION FILTER options end */
/* Flags about Log Likehood Ratio(LLR) processong end */

/* flags for simulation main config start */
#define DEFAULT_MINIMUM_LOOP    10000
extern unsigned long global_minimum_loop;
extern unsigned long global_base_of_loop;
extern unsigned int global_rate_of_loop;

#define VALUE_ERROR_NUM_WORD    "error-number"//SET_VALUE
extern unsigned int global_ErrorNum;
/* flags for simulation main config end */

/* flags for generating galois field(GF) start */
/* Make Galois Field options start */
extern unsigned int global_flag_gfCommon;
#define FLAG_MASK_GF_COMMON_MAKE_GF_ALL (1<<0)//if(global_flag_gfCommon&FLAG_MASK_GF_COMMON_MAKE_GF_ALL)
/* Make Galois Field options end */


/* Code Specification start */
#define VALUE_GF_EXP_WORD       "gf-exp"//SET_VALUE
#define DEFAULT_VALUE_GF_EXP    10
extern unsigned int global_GaloisFieldExponential;

#define VALUE_CODE_LENGTH_WORD      "codelength"//SET_VALUE
#define DEFAULT_VALUE_CODE_LENGTH   1023
extern unsigned int global_MaximumCodeLength;
extern unsigned int global_CodeLength;
extern unsigned int global_LimitCodeLength;

/* added LCM expantion */
extern char *global_addedRootToLCM_string;// to adding low common multiple(LCM)
extern unsigned int global_degAddedRootToLCM;
/* Code Specification end */

/* Error correction specification start */
#define VALUE_HARD_CORRECTABLE_WORD         "hard-correctable"//SET_VALUE
#define DEFAULT_VALUE_BCH_HARD_CORRECTABLE  4
extern unsigned int global_bch_hardCorrectable;

#define VALUE_SOFT_CORRECTABLE_WORD         "soft-correctable"//SET_VALUE
#define DEFAULT_VALUE_BCH_SOFT_CORRECTABLE  2
extern unsigned int global_bch_SoftCorrectable;
/* Error correction specification end */

#define DEFAULT_VALUE_BCH_SOFT_OVER_SYND    0
extern unsigned int global_bch_Soft_OverSyndLen;
/* flags for generating galois field(GF) end */

/* non-categorized flags start */
extern char *global_AddedRoot;  //notused
/* non-categorized flags end */

/* Operation */
void GLOBAL_flag_proc();

#endif
