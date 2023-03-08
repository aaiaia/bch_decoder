#include <stdio.h>
#include <unistd.h>
#include <dirent.h> //directory lib
#include <errno.h>  //error number
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <malloc.h>
#include <memory.h>
#include <time.h>   //time()
#include <unistd.h> //getpid()

#include <errno.h>  //errno

#include <sys/time.h>

    #ifdef USING_OPEN_MP
    #include<omp.h>

    #ifndef THREAD_NUMS
        #define THREAD_NUMS 4
    #endif

    #endif

// #include "./bmp_file_io.h"


unsigned int global_seed;


struct tm *date_simulation_date;
//date_simulation_date=localtime(&time_program_init.tv_sec);
//date_simulation_date->tm_year+1900
//date_simulation_date->tm_mon+1
//date_simulation_date->tm_mday
//date_simulation_date->tm_hour
//date_simulation_date->tm_min
//date_simulation_date->tm_sec
//gettimeofday(&time_proc_start, NULL);
//printExcutingTime(&time_proc_start);
#define timeMes printf("[TIME] ) ")
struct timeval time_program_init;

struct timeval time_EbN0_start;
struct timeval time_encoding_start;
struct timeval time_decoding_start;

struct timeval time_bm_algorithm_start;
struct timeval time_chien_search_start;


struct timeval time_proc_start;



//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
void printExcutingTime(struct timeval *start)
{
    struct timeval end;
    struct timeval diff;
    gettimeofday( &end, NULL );

    diff.tv_sec = end.tv_sec - start->tv_sec;
    diff.tv_usec = end.tv_usec - start->tv_usec;
    if(diff.tv_usec<0)
    {
        diff.tv_usec+=1000000;
        diff.tv_sec-=1;
        timeMes;    printf("%ld [sec], %ld [usec]\n", diff.tv_sec, diff.tv_usec);
    }
    else
    {
        timeMes;    printf("%ld [sec], %ld [usec]\n", diff.tv_sec, diff.tv_usec);
    }
}




char global_buf_KeyBoardInput[255]={0};


#define errorMes    printf("ERROR ) ")
#define warningMes    printf("/*********************************/\n/********** WARNING !!! **********/\n/*********************************/\nWARNING ) ")
#define warningMesShort    printf("WARNING ) ")
#define infoMes        printf("Info ) ")

#define initMes        printf("Initial ) ")

#define resultMes    printf("Result ) ")


#define OPTION_WORD    '-'


unsigned int global_flag_cmdOption=0;
#define PRINTF_LOG_WORD                         "l"
#define FLAG_MASK_PRINTF_LOG                    (1<<0)
#define logMes                                  printf("LOG ) ")

#define PRINTF_CALCULATE_PROCESS_WORD           "c"
#define FLAG_MASK_PRINTF_CALCULATE_PROCESS      (1<<1)
#define calculateMes                            printf("CALCULATE ) ")

#define PRINTF_MEMORY_CHECK_PROCESS_WORD        "m"
#define FLAG_MASK_PRINTF_MEMORY_CHECK_PROCESS   (1<<2)
#define memoryMes                               printf("MEMORY ) ")

#define IGNORE_WARNING_MES_WORD                 "w"
#define FLAG_MASK_PRINTF_WARNING_MES            (1<<3)
    /* variables to check memory lineary */
    unsigned int global_tmp_i_memoryCheck=0;


/* Sysgem Hold command */
unsigned int global_flag_cmdSystemHold=0;
#define PRINTF_HELP_WORD    "h"
#define FLAG_MASK_HOLD_HELP (1<<0)
#define systemHoldMes       printf("HOLD ) ")



/* forced Sysgem unhold command */
unsigned int global_flag_cmdSystemUnhold=0;
#define SYSTEM_UNHOLD_FORCED_WORD       "f"
#define FLAG_MASK_SYSTEM_UNHOLD_FORCED  (1<<0)//if(global_flag_cmdSystemUnhold&FLAG_MASK_SYSTEM_UNHOLD_FORCED)
#define systemUnholdMes                 printf("UNHOLD ) ")




#define ADDED_OPTION_WORD       '-'



    /*Algorithm Functional Test*/

    #define ALGORITHM_FUNC          (1<<0)

    #define ALGORITHM_CHANNEL       (1<<1)

    /*Display RESULT or Progress or Function name*/
    enum DEBUG_DISPLAY_MODE
    {
        FLAG_MASK_DISPLAY_RESULT    = (1<<0),
        FLAG_MASK_DISPLAY_PROGRESS  = (1<<1),
        FLAG_MASK_DISPLAY_FUNCNAME  = (1<<2),
    };



/*Global debug option start*/
unsigned int global_flag_deBugOption=0;
#define DEBUG_DEBUG_WORD        "debug"//FLAG_MASK_DISPLAY_PROGRESS
#define FLAG_MASK_DEBUG_DEBUG   (1<<0)
#define debugMes                printf("debug ) ");

    unsigned int global_flag_debug_display=0;
    /*
    //#define FLAG_MASK_DISPLAY_RESULT    (1<<0)
    //#define FLAG_MASK_DISPLAY_PROGRESS  (1<<1)
    //#define FLAG_MASK_DISPLAY_FUNCNAME  (1<<2)
    */
    #define debugFuncNameMes    printf("debug-funcname ) ");
    #define debugSequenceMes    printf("debug-progress ) ");
/*Global debug option end*/



    /*BM Algorithm debug option start*/
    unsigned int global_flag_debug_bmSim=0;
    #define DEBUG_BCH_BM_ALGORITHM_WORD         "debug-bm"//OPT_DISPLAY_PROGRESS
    #define FLAG_MASK_DEBUG_BCH_BM_ALGORITHM    (1<<0)
    #define debugBmAlgorithmMes                 printf("debug-BM ) ");

    #define DEBUG_BCH_BM_CAL_DELTA_WORD         "debug-bm-cal-delta"//OPT_DISPLAY_PROGRESS
    #define FLAG_MASK_DEBUG_BCH_BM_CAL_DELTA    (1<<1)
    #define debugBmAlgorithmCalDeltaMes         printf("debug-BM-cal-delta ) ");
    /*BM Algorithm debug option end*/

    /*mSBS Algorithm debug option start*/
    /* below variables could be unnecessary */
    unsigned int global_flag_debug_mSBS_Sim=0;
    #define DEBUG_BCH_mSBS_ALGORITHM_WORD       "debug-mSBS"//OPT_DISPLAY_PROGRESS
    #define FLAG_MASK_DEBUG_BCH_mSBS_ALGORITHM  (1<<0)
    #define debug_mSBS_AlgorithmMes             printf("debug-mSBS ) ");

    #define DEBUG_BCH_M_SBS_CAL_DELTA_WORD      "debug-mSBS-cal-delta"//OPT_DISPLAY_PROGRESS
    #define FLAG_MASK_DEBUG_BCH_M_SBS_CAL_DELTA (1<<1)
    #define debug_mSBS_AlgorithmCalDeltaMes     printf("debug-mSBS-cal-delta ) ");
    /*mSBS Algorithm debug option end*/



unsigned int global_flag_berOptions=0;
/*
#define TEST_GET_BER_WORD       "get-ber"//SETVALUE
*/
#define FLAG_MASK_TEST_GET_BER  (1<<0)
#define testGetBerMes           printf("test GET-BER ) ");
    /****************************************************/
    /********** global flag associate wieh ber **********/
    /****************************************************/
    enum FLAG_CASE_SEL_TP_BEFORE_DEC_METHOD
    {
        FLAG_CASE_SEL_TP_BEFORE_DEC_NONE,
        FLAG_CASE_SEL_TP_BEFORE_DEC_THEO,
        FLAG_CASE_SEL_TP_BEFORE_DEC_ACT,
        FLAG_CASE_SEL_TP_BEFORE_DEC_END,
    }typedef enum_FLAG_CASE_SEL_TP_BEFORE_DEC_METHOD;

    const char NAME_FLAG_CASE_SEL_TP_BEFORE_DEC_MTEHOD_FILE_ADDED
                    [FLAG_CASE_SEL_TP_BEFORE_DEC_END][51] =
    {
        "NONE",
        "THEO",
        "ACT"
    };

    unsigned int global_flag_case_selTP_before_dec = 0;

    #define FLAG_CASE_DECODING_FAIL_METHOD_DEFAULT  FLAG_CASE_SEL_TP_BEFORE_DEC_THEO


        /****************************************************/
        /********** global flag associate wieh ber **********/
        /****************************************************/
        unsigned int global_flag_case_choosingCodeWordMethod_inTheo=0;
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


            unsigned int global_flag_case_getBER_method_Display=0;
            //if( (global_flag_case_getBER_method_Display&FLAG_MASK_DISPLAY_PROGRESS) )
            /*
            //#define FLAG_MASK_DISPLAY_RESULT    (1<<0)
            //#define FLAG_MASK_DISPLAY_PROGRESS  (1<<1)
            //#define FLAG_MASK_DISPLAY_FUNCNAME  (1<<2)
            */

    unsigned int global_flag_case_dec_fail_handling = 0;

    /* Method that re transfer is not supported yet */
    enum FLAG_CASE_DEC_FAIL_HANDLING
    {
        FLAG_CASE_DEC_FAIL_HANDLING_NONE,
        FLAG_CASE_DEC_FAIL_HANDLING_RE_TRANFER_FAIL_NO_MORE,
        FLAG_CASE_DEC_FAIL_HANDLING_RE_TRANFER_INTERATION,//not support yet
    }typedef enum_FLAG_CASE_DEC_FAIL_HANDLING;








unsigned int global_flag_simulationCode=0;
/*Channel Setting, LLR quantization bit setting*/
#define FLAG_MASK_CODE_BCH  (1<<0)
#define mes_bchCode         printf("BCH code) ");

    //////////////////////////////////////////////////////////////////////////////////
    //////////////////////* Common BCH Codes Calculations start */////////////////////
    //////////////////////////////////////////////////////////////////////////////////
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
    //////////////////////////////////////////////////////////////////////////////////
    ///////////////////////* Common BCH Codes Calculations end *//////////////////////
    //////////////////////////////////////////////////////////////////////////////////



    /****************************************************/
    /********** global flag associate wieh bch **********/
    /****************************************************/
    unsigned int global_flag_bchDecAlgo_Enable=0;
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
    unsigned int global_flag_sim_workingMode = 0;
    enum SIMULATION_WORKING_MODE
    {
        FLAG_MASK_SIM_DECOING       = (1<<0),
        FLAG_MASK_SIM_ABSTRACT_DATA = (1<<1),//reserved Working mode
        FLAG_MASK_SIM_TEST          = (1<<2),
    };

    /* global_list_flag_bch_sim_decodingMode */
    unsigned int global_flag_sim_decodingMode = 0;
    unsigned int global_list_flag_bch_sim_decodingMode[KIND_OF_BCH_DECODING_END] = {0};
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
        /*
        //unsigned int global_flag_bchBM_simulationMode=0;
        //enum BCH_BM_SIMULATION_MODE
        //{
        //    FLAG_MASK_TEST_BM_ALGORITHM         = (1<<0),
        //    FLAG_MASK_CODE_BCH_BM_HARD_DECISION = (1<<1),
        //    FLAG_MASK_CODE_BCH_BM_SOFT_DECISION = (1<<2),
        //
        //    FLAG_MASK_CODE_BCH_BM_DECODING_SKIP = (1<<15),
        //};
        */

        /*Correctable bits length, set global_flag_bchBM_simulationMode|=FLAG_MASK_CODE_BCH_DEC_AL_HARD_DECISION*/
        /*
        //#define FLAG_MASK_DISPLAY_RESULT    (1<<0)
        //#define FLAG_MASK_DISPLAY_PROGRESS  (1<<1)
        //#define FLAG_MASK_DISPLAY_FUNCNAME  (1<<2)
        */
            /*
            //#define infoMes printf("test BCH BM HARD ) ")
            */
            unsigned int global_flag_bch_BM_SimHD_display=0;
        /*LLR Quantization value Set*/
            /*
            //#define infoMes printf("test BCH BM SOFT ) ")
            */
            unsigned int global_flag_bch_BM_SimSD_display=0;//make common thing.
            #define test_BM_SoftDisplayMes  printf("test SBCH BM SOFT DISPLAY ) ");


        /*******************************************************************/
        /********** global flag associate wieh bch - mSBS algorithm **********/
        /*******************************************************************/
        /*
        //unsigned int global_flag_bch_mSBS_simulationMode = 0;
        //enum BCH_M_SBS_SIMULATION_MODE
        //{
        //    FLAG_MASK_CODE_BCH_DEC_AL_TEST_THIS_ALGORITHM   = (1<<0),
        //    FLAG_MASK_CODE_BCH_DEC_AL_HARD_DECISION         = (1<<1),
        //    FLAG_MASK_CODE_BCH_DEC_AL_SOFT_DECISION         = (1<<2),
        //
        //    FLAG_MASK_CODE_BCH_DEC_AL_SKIP                  = (1<<15),
        //};
        */

        /*Correctable bits length, set global_flag_bchBM_simulationMode|=FLAG_MASK_CODE_BCH_DEC_AL_HARD_DECISION*/
        /*
        //#define FLAG_MASK_DISPLAY_RESULT    (1<<0)
        //#define FLAG_MASK_DISPLAY_PROGRESS  (1<<1)
        //#define DISPLAY_FUNCNAME_WORD       "funcname"
        //#define FLAG_MASK_DISPLAY_FUNCNAME  (1<<2)
        */
        #define test_mSBS_HardMes   printf("test BCH mSBS HARD ) ");
            unsigned int global_flag_bch_mSBS_SimHD_display=0;
        /*LLR Quantization value Set*/
        #define test_mSBS_SoftMes   printf("test BCH mSBS SOFT ) ");
            unsigned int global_flag_bch_mSBS_SimSD_display=0;
            #define test_mSBS_SoftDisplayMes    printf("test BCH mSBS  SOFT DISPLAY ) ");


//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
#define FLAG_MASK_FILE_IO    (1<<0)
unsigned int global_flag_fileInputOutput = 0;

    unsigned int global_flag_file_io_hd_pattern_output = 0;
    unsigned int global_flag_file_io_hd_pattern_output_log = 0;
    unsigned int global_flag_file_io_hd_pattern_output_verilog = 0;
    enum HD_PATTERN_OUTPUT
    {
        FLAG_MASK_FILE_IO_HD_MES            = (1<<0),
        FLAG_MASK_FILE_IO_HD_ENCODING_MES   = (1<<1),
        FLAG_MASK_FILE_IO_HD_RECEIVED_MES   = (1<<2),
        FLAG_MASK_FILE_IO_HD_CORRECTED_MES  = (1<<3),
    };

    unsigned int global_flag_file_io_sd_pattern_output = 0;
    unsigned int global_flag_file_io_sd_pattern_output_log = 0;
    unsigned int global_flag_file_io_sd_pattern_output_verilog = 0;
    enum SD_PATTERN_OUTPUT
    {
        FLAG_MASK_FILE_IO_SD_RECEIVED_LLR       = (1<<0),
        FLAG_MASK_FILE_IO_SD_RECEIVED_LLR_mag   = (1<<1),
    };



//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
                unsigned int testOption=0;
                #define FLAG_MASK_TEST_RESULT   (1<<5)

                unsigned int enviromentsOption=0;


/*****************************************/
/********** Associate with AWGN **********/
/*****************************************/
#define VALUE_EBN0_WORD         "ebn0"//SET_VALUE
/*
//double main_com_EbN0 = 1.0;
*/
#define DEFAULT_VALUE_EBN0      1.0f
double global_EbN0_Start_Value = DEFAULT_VALUE_EBN0;

#define VALUE_EBN0_END_WORD     "ebn0-end"//SET_VALUE
#define DEFAULT_VALUE_EBN0_END  8.0f
double global_EbN0_End_Value = DEFAULT_VALUE_EBN0_END;

#define VALUE_EBN0_STEP_WORD    "ebn0-step"//SET_VALUE
#define DEFAULT_VALUE_EBN0_STEP 0.25f
double global_EbN0_Step = DEFAULT_VALUE_EBN0_STEP;


unsigned int global_flag_debug_awgn=0;
#define FLAG_MASK_DEBUG_AWGN_SEQUENCE   (1<<0)
#define debugAwgnSequenceMes            printf("SEQUENCE AWGN ) ");



/********************************************************/
/********** Associate with Log Like Hood Ratio **********/
/********************************************************/
unsigned int global_flag_llr_display=0;
enum FLAG_LLR_DISPLAY
{
    FLAG_LLR_DISPLAY_NONE                   = 0,//if(global_flag_llr_display&FLAG_LLR_DISPLAY_QUANTIZATION_FILTERING)
    FLAG_LLR_DISPLAY_QUANTIZATION_FILTERING = (1<<0),
}typedef enum_FLAG_LLR_DISPLAY;

/*
//#define FLAG_CASE_FINDING_MIN_LLR_METHOD_NONE                       0
//#define FLAG_CASE_FINDING_MIN_LLR_METHOD_MINIMUM                    1
//#define FLAG_CASE_FINDING_MIN_LLR_METHOD_TREE_FOLLOWING_MIN1_PATH   2
//#define FLAG_CASE_FINDING_MIN_LLR_METHOD_TREE_PROBABILISIC_CHOOSEN  3//NOT SUPPORT YET
//#define FLAG_CASE_FINDING_MIN_LLR_METHOD_TREE_PRIOR_HD_ZERO         4//NOT SUPPORT YET
*/
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
const char NAME_FLAG_CASE_FINDING_MIN_MTEHOD[FLAG_CASE_FINDING_MIN_LLR_METHOD_END][51] =
{
    "NONE",
    "MINIMUM",
    "GROUPING_MINIMUM",
    "TREE-FALLOWING_MIN1_PATH",
    "TREE-PROBABILISTIC_CHOOSEN(Not support yet)",
    "TREE-PRIOR_HD_ZERO(Not support yet)",
};

const char NAME_FLAG_CASE_FINDING_MIN_MTEHOD_FILE_ADDED[FLAG_CASE_FINDING_MIN_LLR_METHOD_END][51] =
{
    "NONE",
    "MIN",
    "GROUP_MIN",
    "TREE-FAL_MIN1_PH",
    "TREE-PROB_CHOOSEN",
    "TREE-PRIOR_HD_ZERO",
};


#define FLAG_CASE_FINDING_MIN_LLR_METHOD_DEFAULT    FLAG_CASE_FINDING_MIN_LLR_METHOD_TREE_FOLLOWING_MIN1_PATH
unsigned int global_flag_case_find_LLR_method = 0;

    unsigned int global_grouping_stream_nums = 1;
    unsigned int global_group_bit_num = 0;
    unsigned int global_group_last_bit_num = 0;

//////////////////////////////////////////
/* pass HD 1 when llr mag is zero start */
enum FLAG_CASE_PASS_MAG0_HD_1_MEAN_NOT_CHOOSEN_LOW_LLR
{
    FLAG_CASE_PASS_MAG0_HD_1_DISABLE,
    FLAG_CASE_PASS_MAG0_HD_1_ENABLE,
    FLAG_CASE_PASS_MAG0_HD_1_END,
};
const char NAME_FLAG_CASE_PASS_MAG0_HD_1[FLAG_CASE_PASS_MAG0_HD_1_END][71] =
{
    "No passing llr magnitude when HD 1(finding min value only)",
    "Passing llr magnitude when HD 1(finding min value only)",
};

unsigned int global_flag_case_pass_hd_1 = FLAG_CASE_PASS_MAG0_HD_1_DISABLE;
/*  pass HD 1 when llr mag is zero end  */

enum FLAG_CASE_SAME_LLR_HANDLING
{
    FLAG_CASE_SAME_LLR_HANDLING_NONE,
    FLAG_CASE_SAME_LLR_HANDLING_NOT_SEL,
    FLAG_CASE_SAME_LLR_HANDLING_PRIOR_HD_0,
    FLAG_CASE_SAME_LLR_HANDLING_PRIOR_HD_1,
    FLAG_CASE_SAME_LLR_HANDLING_END,
};
const char NAME_FLAG_CASE_SAME_LLR_HANDLING[FLAG_CASE_SAME_LLR_HANDLING_END][31] =
{
    "same llr mag is none",
    "same llr mag is not selected",
    "same llr mag is prior 0",
    "same llr mag is prior 1",
};
unsigned int global_flag_case_same_llr_handling = FLAG_CASE_SAME_LLR_HANDLING_NONE;
//////////////////////////////////////////
enum FLAG_CASE_INIT_LLR_MAG_METHOD
{
    FLAG_CASE_INIT_LLR_MAG_METHOD_NONE,
    FLAG_CASE_INIT_LLR_MAG_METHOD_NOT_SEL_MAX_LLR,
    FLAG_CASE_INIT_LLR_MAG_METHOD_END,
};
const char NAME_FLAG_CASE_INIT_LLR_MAG_METHOD[FLAG_CASE_INIT_LLR_MAG_METHOD_END][51] =
{
    "first loc is selected, has no condition",
    "if no minimum llr val than max llr, not select",
};
unsigned int global_flag_case_init_llr_mag_method = FLAG_CASE_INIT_LLR_MAG_METHOD_NONE;
//////////////////////////////////////////

/*Log LikeHood Ratio debug option start*/
unsigned int global_flag_debug_awgnLLR=0;
#define FLAG_MASK_DEBUG_AWGN_LLR_SEQUENCE   (1<<0)
#define debugAwgnLlrSequenceMes             printf("SEQUENCE LLR ) ");

#define FLAG_MASK_DEBUG_AWGN_LLR_LOCATOR    (1<<1)
#define debugAwgnLlrLocatorMes              printf("debug-LLR-Locator ) ");
/*Log LikeHood Ratio debug option end*/

/* LLR QUANTIZATION FILTER options start */
enum ENUM_FLAG_CASE_QUANTIZ_NUMBERIC_SYSTEM
{
    ENUM_FLAG_CASE_QUANTIZ_NUMBERIC_SYSTEM_NONE,
    ENUM_FLAG_CASE_QUANTIZ_NUMBERIC_SYSTEM_TWO_S_COMPLEMENT,
    ENUM_FLAG_CASE_QUANTIZ_NUMBERIC_SYSTEM_ONE_S_COMPLEMENT,
    ENUM_FLAG_CASE_QUANTIZ_NUMBERIC_SYSTEM_END,
};
const char NAME_QUANTIZ_NUMBERIC_SYSTEM[ENUM_FLAG_CASE_QUANTIZ_NUMBERIC_SYSTEM_END][51]=
{
    "numberic system is not identified",
    "2's compliment",
    "1's compliment",
};

unsigned int global_flag_case_llr_numberic_system = \
    ENUM_FLAG_CASE_QUANTIZ_NUMBERIC_SYSTEM_TWO_S_COMPLEMENT;



enum ENUM_FLAG_CASE_QUANTIZ_ONE_S_COMPLEMENT_ZERO_HANDLING
{
    ENUM_FLAG_CASE_QUANTIZ_NUMBERIC_SYS_ONE_S_COM_NONE,
    ENUM_FLAG_CASE_QUANTIZ_NUMBERIC_SYS_ONE_S_COM_ZERO_SYMMETRY,
    ENUM_FLAG_CASE_QUANTIZ_NUMBERIC_SYS_ONE_S_COM_ZERO_DUPLICATED,
    ENUM_FLAG_CASE_QUANTIZ_NUMBERIC_SYS_ONE_S_COM_END,

};

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













#define DEFAULT_MINIMUM_LOOP    10000
unsigned long global_minimum_loop = 0;
unsigned long global_base_of_loop = 0;
unsigned int global_rate_of_loop = 0;


#define VALUE_ERROR_NUM_WORD    "error-number"//SET_VALUE
unsigned int global_ErrorNum = 0;




unsigned int global_flag_gfCommon_display = 0;
enum FLAG_MASK_DISPLAY_GF_COMMON
{
    FLAG_MASK_DISPLAY_GF_COMMON_GENPOLY     = (1<<0),//if(global_flag_gfCommon_display&FLAG_MASK_DISPLAY_GF_COMMON_GENPOLY)
    FLAG_MASK_DISPLAY_GF_COMMON_CONJUGACIES = (1<<2),//if(global_flag_gfCommon_display&FLAG_MASK_DISPLAY_GF_COMMON_CONJUGACIES)
    FLAG_MASK_DISPLAY_GF_COMMON_FIELD       = (1<<1),//if(global_flag_gfCommon_display&FLAG_MASK_DISPLAY_GF_COMMON_FIELD)
    FLAG_MASK_DISPLAY_GF_COMMON_PRIMEPOLY   = (1<<3),//if(global_flag_gfCommon_display&FLAG_MASK_DISPLAY_GF_COMMON_PRIMEPOLY)
    FLAG_MASK_DISPLAY_GF_COMMON_SUM_MATRIX  = (1<<4),//if(global_flag_gfCommon_display&FLAG_MASK_DISPLAY_GF_COMMON_SUM_MATRIX)
    FLAG_MASK_DISPLAY_GF_COMMON_FILE_IO     = (1<<5),//if(global_flag_gfCommon_display&FLAG_MASK_DISPLAY_GF_COMMON_FILE_IO)
};


    unsigned int global_flag_gfCommon = 0;
    #define FLAG_MASK_GF_COMMON_MAKE_GF_ALL (1<<0)//if(global_flag_gfCommon&FLAG_MASK_GF_COMMON_MAKE_GF_ALL)


/* Code Specification start */
                #define VALUE_GF_EXP_WORD       "gf-exp"//SET_VALUE
                #define DEFAULT_VALUE_GF_EXP    10
                unsigned int global_GaloisFieldExponential = DEFAULT_VALUE_GF_EXP;

#define VALUE_CODE_LENGTH_WORD      "codelength"//SET_VALUE
#define DEFAULT_VALUE_CODE_LENGTH   1023
unsigned int global_MaximumCodeLength = 0;
unsigned int global_CodeLength = DEFAULT_VALUE_CODE_LENGTH;
unsigned int global_LimitCodeLength = 0;
/* Code Specification end */

                /* Error correction specification start */
                /*
                //#define VALUE_INFO_LENGTH_WORD              "infolength"
                //#define DEFAULT_VALUE_INFO_LENGTH           1024
                //unsigned int global_InfoLength = DEFAULT_VALUE_INFO_LENGTH;
                */
                #define VALUE_HARD_CORRECTABLE_WORD         "hard-correctable"//SET_VALUE
                #define DEFAULT_VALUE_BCH_HARD_CORRECTABLE  4
                unsigned int global_bch_hardCorrectable = DEFAULT_VALUE_BCH_HARD_CORRECTABLE;

                #define VALUE_SOFT_CORRECTABLE_WORD         "soft-correctable"//SET_VALUE
                #define DEFAULT_VALUE_BCH_SOFT_CORRECTABLE  2
                unsigned int global_bch_SoftCorrectable = DEFAULT_VALUE_BCH_SOFT_CORRECTABLE;
                /* Error correction specification end */

                #define DEFAULT_VALUE_BCH_SOFT_OVER_SYND    0
                unsigned int global_bch_Soft_OverSyndLen = DEFAULT_VALUE_BCH_SOFT_OVER_SYND;

                //unsigned int global_correctables[2] = {&global_bch_hardCorrectable, &global_bch_SoftCorrectable};

char *global_AddedRoot = NULL;


#define FLAG_MASK_CHANNEL_MODE_AWGN    (1<<0)

#define DEFAULT_CHANNEL_MODE    FLAG_MASK_CHANNEL_MODE_AWGN

unsigned int global_Channel_Mode = DEFAULT_CHANNEL_MODE;

    enum ENUM_GLO_CH_MODE_FUNC
    {
        FLAG_MASK_CHANNEL_MODE_FUNC_NONE                = 0,
        FLAG_MASK_CHANNEL_MODE_FUNC_BITRATE_DISABLE     = (1<<0),//if(global_flag_Channel_Mode_Func&FLAG_MASK_CHANNEL_MODE_FUNC_BITRATE_DISABLE)
        FLAG_MASK_CHANNEL_MODE_FUNC_RANDOM_SEED_DISABLE = (1<<1),//if(global_flag_Channel_Mode_Func&FLAG_MASK_CHANNEL_MODE_FUNC_RANDOM_SEED_DISABLE)
    }typedef ENUM_GLO_CH_MODE_FUNC;

    unsigned int global_flag_Channel_Mode_Func = FLAG_MASK_CHANNEL_MODE_FUNC_NONE;



/***************************************************************/
unsigned int global_flag_Modulation_Mode = 0;
#define FLAG_MASK_MODULATION_MODE_BPSK  (1<<0)                        //if(global_flag_Modulation_Mode&FLAG_MASK_MODULATION_MODE_BPSK)
#define DEFAULT_MODULATION_MODE         FLAG_MASK_MODULATION_MODE_BPSK
    unsigned int global_flag_Modulation_Mode_BPSK_Display = 0;
    /*
    //#define FLAG_MASK_DISPLAY_RESULT    (1<<0)
    //#define FLAG_MASK_DISPLAY_PROGRESS  (1<<1)
    //#define FLAG_MASK_DISPLAY_FUNCNAME  (1<<2)
    //#define DISPLAY_DEBUG_WORD          "debug"
    //#define DISPLAY_DEBUG               (1<<3)
    */

//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
/********** Simulation Test Mode **********/

    /****************************************************************/
    /* Test Mode */
    enum ENUM_FLAG_CASE_SIM_TEST_OPTIONS
    {
        FLAG_CASE_SIM_TEST_MODE_NONE,
        FLAG_CASE_SIM_TEST_MODE_GET_LLR_MAG_AVR_BELONG_EBN0,
        FLAG_CASE_SIM_TEST_MODE_GET_PROBABILITY_OF_MINIMUM_VAL,
        FLAG_CASE_SIM_TEST_MODE_SHOW_LOOP_CNT,
    };

    unsigned int global_flag_case_sim_testOpt = 0;



    /* Test Mode Commonly using variables */
    /* Test mode iteration variables */
    #define TMP_TEST_MODE_LOOPS             1000000UL
    #define TMP_TEST_MODE_ZERO_BIT_RATIO    0.5f

    unsigned long tmp_testMode_loops = TMP_TEST_MODE_LOOPS;


    /****************************************************************/
    /* Error Distribution */
    FILE *fp_testMode_log = NULL;
    FILE *fp_testMode_sum_bit_cnt_arranged_by_bitRetio_csv= NULL;
    FILE *fp_testMode_sum_bit_cnt_classified_arranged_by_bitRatio_csv = NULL;
    FILE *fp_testMode_sum_llr_mag_arranged_by_bitRatio_csv = NULL;
    FILE *fp_testMode_sum_err_bit_cnt_chg_classified_arranged_by_bitRatio_csv = NULL;

    FILE *fp_testMode_all_arranged_by_bitRatio_csv = NULL;


    FILE *fp_testMode_sum_codeword_cnt_arranged_by_errorNums_csv = NULL;
    FILE *fp_testMode_sum_llr_mag_distribution_cnt_arranged_by_errorNums_csv = NULL;
    FILE *fp_testMode_sum_bit_ratio_cnt_arranged_by_errorNums_csv = NULL;

    FILE *fp_testMode_all_arranged_by_errorNums_csv = NULL;

    enum ENUM_CASE_TESTMODE_ERR_RATE_MSG_GEN_METH
    {
        ENUM_CASE_TESTMODE_ERR_RATE_MSG_GEN_METH_NONE,
        ENUM_CASE_TESTMODE_ERR_RATE_MSG_GEN_METH_STATIC,
        ENUM_CASE_TESTMODE_ERR_RATE_MSG_GEN_METH_RAND,
    };
    unsigned int global_flag_case_testMode_msg_gen_method =
                    ENUM_CASE_TESTMODE_ERR_RATE_MSG_GEN_METH_STATIC;

    enum ENUM_CASE_TESTMODE_ERR_RATE_DECODING_METH
    {
        ENUM_CASE_TEST_MODE_ERR_RATE_DECODING_METH_NONE,
        ENUM_CASE_TEST_MODE_ERR_RATE_DECODING_METH_HD,
        ENUM_CASE_TEST_MODE_ERR_RATE_DECODING_METH_SD,
    };
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

    unsigned long tmp_testMode_llr_mag_sum_inR_w =0UL;//llr sum in received codeword wrong case
    unsigned long tmp_testMode_llr_mag_sum_inR_c =0UL;//llr sum in received codeword good case


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
    unsigned long** tmp_llr_mag_sum_inR_OZ_ratio=NULL;

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

    #define LLR_CASE_NUM    8

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

    enum ENUM_CASE_TESTMODE_PROB_MIN_CHOOSEN_POLITICS
    {
        ENUM_CASE_TEST_MODE_PROB_MIN_CHOOSE_SAME_VAL,
        ENUM_CASE_TEST_MODE_PROB_MIN_CHOOSE_LOW_VAL,
    };
    unsigned int tmp_testMode_prob_min_choosen_politics =
                    ENUM_CASE_TEST_MODE_PROB_MIN_CHOOSE_SAME_VAL;

    unsigned int* initTestMode_prob_min_counting
            (unsigned int **p, unsigned int *len, unsigned int max_val)
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



//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
char *temporaryFunc_generateTestPatternToString(char *string, unsigned int stringLength, unsigned int *locatorArray, unsigned int locatorArrayLength)
{
    unsigned int tmpStringLength=strlen(string);
    unsigned int i;

    char *p;
    #ifdef USING_OPEN_MP
        unsigned int OMP_locatorArrayLength;
        unsigned int *OMP_locatorArray
        char *OMP_p;
    #endif
    //        #pragma omp for private(i) shared(locatorArrayLength, locatorArray, p)
    #ifndef RELEASE
    if(!string)
    {
        errorMes;
        printf("in temporaryFunc_generateTestPatternToString, \n");
        return NULL;
    }
    if(!locatorArray)
    {
        errorMes;
        printf("in temporaryFunc_generateTestPatternToString, \n");
        return NULL;
    }
    if(!locatorArrayLength)
    {
        errorMes;
        printf("in temporaryFunc_generateTestPatternToString, \n");
        return NULL;
    }
    #endif

    if(!stringLength)
    {
        stringLength=tmpStringLength;
    }
    p=(char*)malloc(sizeof(char)*stringLength+1);
    memcpy(p, string, sizeof(char)*tmpStringLength);
    *(p+tmpStringLength)=0;
    //memset(p, '0', sizeof(char)*stringLength);
    //*(p+stringLength)=0;

                        #ifndef RELEASE
                        if(global_flag_debug_awgnLLR&FLAG_MASK_DEBUG_AWGN_LLR_LOCATOR)
                        {
                            debugAwgnLlrLocatorMes;
                            for(i=0; i<locatorArrayLength; i++)
                            {
                                if((*(locatorArray+i)) == -1)
                                {
                                    printf("operation that is if((*(locatorArray+i)) == -1) is true.\n");
                                    printf("If you want continue simulation, input any string : ");
                                    scanf("%s",global_buf_KeyBoardInput);
                                }
                            }
                        }
                        #endif



    #ifdef USING_OPEN_MP
    #pragma omp parallel
    #endif
    {
        #ifdef USING_OPEN_MP
        #pragma omp for private(i) shared(locatorArrayLength, locatorArray, p)
        #endif
        for(i=0; i<locatorArrayLength; i++)
        {
            if((*(locatorArray+i)) == -1) continue;
            if((*(p+(*(locatorArray+i))))=='0')
            {
                (*(p+(*(locatorArray+i))))='1';
            }
            else if((*(p+(*(locatorArray+i))))=='1')
            {
                (*(p+(*(locatorArray+i))))='0';
            }
            else
            {
                #ifdef USING_OPEN_MP
                if((*(p+(*(locatorArray+i))))%2)
                {
                    *(p+(*(locatorArray+i)))='0';
                }
                else
                {
                    *(p+(*(locatorArray+i)))='1';
                }
                #else
                if(rand()%2)
                {
                    *(p+(*(locatorArray+i)))='0';
                }
                else
                {
                    *(p+(*(locatorArray+i)))='1';
                }
                #endif
            }
        }
    }


    return p;
}
//////////////////////////////////////////////////////////////////////////////////
//char *temporaryFunc_generateTestPatternToString(char *string, unsigned int stringLength, unsigned int *locatorArray, unsigned int locatorArrayLength)
char temporaryFunc_generateTestPatterns_using_LLR_Locator
    (
        char *refferString,
        unsigned int refferStringLength,
        unsigned int *locatorArray,
        unsigned int locatorArrayLength,
        char **testPattern,
        unsigned int numsOfTestPattern
    )
{
    unsigned int tmpStringLength=0;

    unsigned int i=0, j=0;
    //unsigned char usingBitNumber=sizeof(i)*8;

    //unsigned char usingBitNumber = (unsigned char)(log((double)locatorArrayLength)/log(2.0));
    unsigned char usingBitNumber = locatorArrayLength;
    //for(i=0; i<sizeof(usingBitNumber)*8; i++)
    //{
    //    if(locatorArrayLength&(1<<i)) break;
    //}
    //usingBitNumber=i;

    #ifndef RELEASE
    if(!testPattern)
    {
        errorMes;
        printf("in temporaryFunc_generateTestPatterns_using_LLR_Locator, testPattern is NULL.\n");
        return -1;
    }
    //if(!*testPattern)
    //{
    //    errorMes;
    //    printf("in temporaryFunc_generateTestPatterns_using_LLR_Locator, *testPattern is NULL.\n");
    //    return -1;
    //}
    #endif

    if(!refferStringLength)
    {
        tmpStringLength=strlen(refferString);
        refferStringLength=tmpStringLength;
    }

    testPattern[0]=refferString;//printf("\n");

    //#ifdef USING_OPEN_MP
    //    #pragma omp parallel for schedule(guided) private(i,j) shared(numsOfTestPattern, usingBitNumber, testPattern, locatorArray, refferString, refferStringLength)
    //#endif
    for(i=1; i<numsOfTestPattern; i++)//To generate Test pattern(is same with combination), index i is used to Mask. if we can select 4-case(00 01 10 11).
    {
                                        //printf("for(i=1; i<locatorArrayLength; i++)  i=%d\n", i);
            if((*(testPattern+i)))
            {
                free(*(testPattern+i));
                #ifndef RELEASE
                warningMes;    printf("free(*(testPattern+%d));\n", i);
                #endif
            }

        *(testPattern+i)=(char*)malloc(sizeof(char)*refferStringLength+1);
                                        //printf("*(testPattern+i)=(char*)malloc(sizeof(char)*refferStringLength+1);\n");
        #ifndef USING_OPEN_MP
        memcpy(*(testPattern+i), refferString, sizeof(char)*refferStringLength);
        #else
        #pragma omp parallel for schedule(guided) private(j) shared(refferStringLength, testPattern, i, refferString)
        for(j=0; j<refferStringLength; j++)
        {
            *((*(testPattern+i))+j) = *(refferString+j);
        }
        #endif
                                        //printf("memcpy(*(testPattern+i), refferString, sizeof(char)*refferStringLength);\n");
        *((*(testPattern+i))+refferStringLength)=0;
                                        //printf("*((*(testPattern+i))+refferStringLength)=0;\n");

        //use non-binary code calculate combination of bit position, j is mean that offset locator pointer
                                        //printf("for(j=0; j<usingBitNumber; j++)//j is mask\n");
        //#ifdef USING_OPEN_MP
        //#pragma omp parallel for schedule(guided) private(j) shared(usingBitNumber, locatorArray, testPattern)
        //#endif
        for(j=0; j<usingBitNumber; j++)//j is mask
        {
            if(i&(1<<j))//j is index of locatorArray.
            {
                //(*(locatorArray+j)) is location of codeword
                if((*(locatorArray+j))!=-1)
                {
                    if(*((*(testPattern+i))+(*(locatorArray+j)))=='0')
                    {
                        *((*(testPattern+i))+(*(locatorArray+j)))='1';
                    }
                    else if(*((*(testPattern+i))+(*(locatorArray+j)))=='1')
                    {
                        *((*(testPattern+i))+(*(locatorArray+j)))='0';
                    }
                    else
                    {
                        warningMesShort;    printf("character is not '0' or '1'.\r\n");
                        #ifdef USING_OPEN_MP
                        if((*((*(testPattern+i))+(*(locatorArray+j))))%2)
                        {
                            *((*(testPattern+i))+(*(locatorArray+j)))='0';
                        }
                        else
                        {
                            *((*(testPattern+i))+(*(locatorArray+j)))='1';
                        }
                        #else
                        if(rand()%2)
                        {
                            *((*(testPattern+i))+(*(locatorArray+j)))='0';
                        }
                        else
                        {
                            *((*(testPattern+i))+(*(locatorArray+j)))='1';
                        }
                        #endif
                    }
                }
            }
        }
    }
                        #ifndef RELEASE
                        if(global_flag_bch_BM_SimSD_display&FLAG_MASK_DISPLAY_PROGRESS)
                        {
                            test_BM_SoftDisplayMes;
                            printf("[Test Pattern Generation]\n");
                            #ifdef USING_OPEN_MP
                            #pragma omp parallel for schedule(guided) private(i) shared(numsOfTestPattern, testPattern)
                            #endif
                            for(i=0; i<numsOfTestPattern; i++)
                            {
                                test_BM_SoftDisplayMes; printf("[Test Pattern Generation %d]\n", i);
                                //printf("%s\n",((*(testPattern+i))+j));
                                    printf("%s\n",((*(testPattern+i))));

                            }
                        }
                        #endif
    return 0;
}
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
char *global_addedRootToLCM_string=NULL;// to adding low common multiple(LCM)
unsigned int global_degAddedRootToLCM=0;
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
/********** FILE IO STRUCTURE **********/
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
char abstractOrderOfEquation(struct_galoisFieldElements *equation)
{
    unsigned int i;

    #ifndef RELEASE
    if(!equation)
    {
        errorMes;
        printf("in abstractOrderOfEquation, struct_galoisFieldElements *equation is NULL.\n");
        return 0;
    }
    if(!(equation->length))
    {
        errorMes;
        printf("in abstractOrderOfEquation, (equation->length) is ZERO.\n");
        return 0;
    }
    #endif

    for(i=equation->length-1; i!=0; i--)//can't check when i==0.
    {
        if(!checkAllValueUsingCharacterFromPolyForm(*(equation->element+i), '0'))
        {
            break;
        }
    }
    if(!i)//check when i==0;
    {
        return i;
    }
    else
    {
        if(!checkAllValueUsingCharacterFromPolyForm(*(equation->element+i), '0'))
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
}

//////////////////////////////////////////////////////////////////////////////////
//f(x)=(x+a)(x+b)(x+c)(x+d)(x+e) = x^5 + A*x^4 + B*x^3 + C*x^2 + D*x^2 + E*x^1 + F
struct_galoisFieldElements *expandRootOfLCM_usingEleOfGF
(

    struct_galoisFieldElements *galoisFields,
    struct_galoisFieldElements *equation,
    struct_setOfGaloisFieldElements *multipledRootSet,
    unsigned int numberOfSelectedSet//numberOfSelectedSet is same that number of correctable bit
)
{
    unsigned int i, j, k;//i is indicator of multipled Root Set, j indicate element of conjugacy class, k is indicator of equation, .
    unsigned int totalLcmOrder=0;

    unsigned int multipledOrder;

    struct_galoisFieldElements *beforeEquation;


    #ifndef RELEASE
    if(!equation)
    {
        errorMes;
        printf("in calculateLeastCommonMultipleUsingElementsOfGaloisField, struct_galoisFieldElements *equation is NULL.\n");
        return NULL;
    }
    if(!multipledRootSet)
    {
        errorMes;
        printf("in calculateLeastCommonMultipleUsingElementsOfGaloisField, struct_galoisFieldElements *multipledRootSet is NULL.\n");
        return NULL;
    }
    #endif


    if((multipledRootSet->length)<numberOfSelectedSet)
    {
                #ifndef RELEASE
                warningMes;    printf("in calculateLeastCommonMultipleUsingElementsOfGaloisField, ");
                warningMesShort; printf("number of selected set(=\'%d\') can not exceed to number of conjugacy set(=\'%d\').\n", numberOfSelectedSet, multipledRootSet->length);
                #endif

        numberOfSelectedSet=(multipledRootSet->length);
                #ifndef RELEASE
                warningMes; printf("number of conjugacy set that %d, which it is maximum length in this condition.\n", numberOfSelectedSet);
                #endif
    }

    for(i=0; i<numberOfSelectedSet; i++)
    {
        totalLcmOrder+=(*(multipledRootSet->conjugateSet+i))->length;
    }

    /* if order is 0, length is 1, if order is n-th, length is n+1. */
    multipledOrder = abstractOrderOfEquation(equation)+totalLcmOrder;

            #ifndef RELEASE
            if(equation->length<multipledOrder+1)
            {
                errorMes; printf("in calculateLeastCommonMultipleUsingElementsOfGaloisField, ");
                printf("not enough space to multiply polynomials(equation order is ");
                printf("\'%d\', totalLcmOrder order is \'%d\').\n", abstractOrderOfEquation(equation), totalLcmOrder);

                return NULL;
            }
            #endif


    beforeEquation = createGaloisFieldExceptElements(equation->length);
                    #ifndef RELEASE
                    if((global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)==FLAG_MASK_PRINTF_LOG)
                    {
                        logMes; printf("in expandRootOfLCM_usingEleOfGF, struct_galoisFieldElements *beforeEquation is made, length is \'%d\'.\n", equation->length);
                    }
                    if((global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)==FLAG_MASK_PRINTF_LOG)
                    {
                        logMes; printf("in expandRootOfLCM_usingEleOfGF, i is indicator of multipled Root Set, j indicate element of conjugacy class, k is indicator of equation.\n");
                    }
                    #endif
    for(i=0; i<numberOfSelectedSet; i++)
    {
        for(j=0; j<(*(multipledRootSet->conjugateSet+i))->length; j++)
        {
            //synchronize...
            if(copyListOfGaloisField(beforeEquation->element, equation->element, equation->length))
            {
                #ifndef RELEASE
                warningMes; printf("in expandRootOfLCM_usingEleOfGF, copyListOfGaloisField(beforeEquation->element, equation->element, equation->length) is not complete.\n");
                #endif
            }

                    #ifndef RELEASE
                    if((global_flag_cmdOption&FLAG_MASK_PRINTF_MEMORY_CHECK_PROCESS)==FLAG_MASK_PRINTF_MEMORY_CHECK_PROCESS)
                    {
                        memoryMes;
                        printf("1) in expandRootOfLCM_usingEleOfGF, memory check *(beforeEquation->element+global_tmp_i_memoryCheck '0' to \'%d\', equation->length is \'%d\'.\n", beforeEquation->length, equation->length);
                        printf("i=\'%d\', j=\'%d\', k=\'%d\'\n", i,j,k);
                        for(global_tmp_i_memoryCheck=0; global_tmp_i_memoryCheck<beforeEquation->length; global_tmp_i_memoryCheck++)
                        {
                            printf("*(beforeEquation->element+global_tmp_i_memoryCheck) = 0x%lx.\n", (unsigned long)(*(beforeEquation->element+global_tmp_i_memoryCheck)));
                        }
                    }
                    #endif

            for(k=0; k<equation->length; k++)
            {
                //calculate 0-th order, just multiple 0-th order and equation.
                        #ifndef RELEASE
                        if((global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)==FLAG_MASK_PRINTF_LOG)
                        {
                            logMes;
                            printf("in expandRootOfLCM_usingEleOfGF, \n");
                            printf("i=\'%d\', j=\'%d\', k=\'%d\'\n", i, j, k);
                            printf("*((*(multipledRootSet->conjugateSet+%d))->element+%d)\t{=>>}(ADDR:\'0x%lx\') == \t", i, j, (unsigned long)(*((*(multipledRootSet->conjugateSet+i))->element+j)));
                            printPolyFormWithEnter(*((*(multipledRootSet->conjugateSet+i))->element+j));
                            printf("\t\t\t*(beforeEquation->element+%d)\t{=>>}(ADDR:\'0x%lx\') == \t", k, (unsigned long)(*(beforeEquation->element+k)));
                            printPolyFormWithEnter(*(beforeEquation->element+k));
                        }
                        #endif
                *(equation->element+k)=multiElementsInGF_returnAddr(galoisFields, *((*(multipledRootSet->conjugateSet+i))->element+j), *(beforeEquation->element+k));
            }

                    #ifndef RELEASE
                    if((global_flag_cmdOption&FLAG_MASK_PRINTF_MEMORY_CHECK_PROCESS)==FLAG_MASK_PRINTF_MEMORY_CHECK_PROCESS)
                    {
                        memoryMes;
                        printf("2) in expandRootOfLCM_usingEleOfGF, memory check *(equation->element+global_tmp_i_memoryCheck) '0' to \'%d\', equation->length is \'%d\'.\n", equation->length, equation->length);
                        printf("i=\'%d\', j=\'%d\', k=\'%d\'\n", i,j,k);
                        for(global_tmp_i_memoryCheck=0; global_tmp_i_memoryCheck<equation->length; global_tmp_i_memoryCheck++)
                        {
                            printf("*(equation->element+global_tmp_i_memoryCheck) = 0x%lx.\n", (unsigned long)(*(equation->element+global_tmp_i_memoryCheck)));
                        }
                    }
                    #endif

            for(k=0; k<equation->length-1; k++)
            {
                //shifted orders
                *(equation->element+k+1)=sumElementInGF_usingSumMatrixReturnAddr(galoisFields, *(beforeEquation->element+k), *(equation->element+k+1));
            }
                    #ifndef RELEASE
                    if((global_flag_cmdOption&FLAG_MASK_PRINTF_MEMORY_CHECK_PROCESS)==FLAG_MASK_PRINTF_MEMORY_CHECK_PROCESS)
                    {
                        memoryMes;
                        printf("3) in expandRootOfLCM_usingEleOfGF, memory check *(equation->element+global_tmp_i_memoryCheck) '0' to \'%d\', equation->length is \'%d\'.\n", equation->length, equation->length);
                        printf("i=\'%d\', j=\'%d\', k=\'%d\'\n", i,j,k);
                        for(global_tmp_i_memoryCheck=0; global_tmp_i_memoryCheck<equation->length; global_tmp_i_memoryCheck++)
                        {
                            printf("*(equation->element+global_tmp_i_memoryCheck) = 0x%lx.\n", (unsigned long)(*(equation->element+global_tmp_i_memoryCheck)));
                        }
                    }
                    #endif
        }
    }

    closeGaloisFieldExceptElements(&beforeEquation);

    return equation;
}
//////////////////////////////////////////////////////////////////////////////////
struct_galoisFieldElements *test_expandLimitedLeastCommonMultipleUsingElementOfGaloisFieldToRoot
(
    struct_galoisFieldElements *galoisFields,
    struct_galoisFieldElements *equation,
    struct_setOfGaloisFieldElements *multipledRootSet,
    unsigned int numberOfSelectedSet//numberOfSelectedSet is same that number of correctable bit
)
{
    unsigned int i, j, k;//i is indicator of multipled Root Set, j indicate element of conjugacy class, k is indicator of equation, .
    unsigned int totalLcmOrder=0;

    unsigned int multipledOrder;

    struct_galoisFieldElements *beforeEquation;


    #ifndef RELEASE
    if(!equation)
    {
        errorMes;
        printf("in test_expandLimitedLeastCommonMultipleUsingElementOfGaloisFieldToRoot, struct_galoisFieldElements *equation is NULL.\n");
        return NULL;
    }
    if(!multipledRootSet)
    {
        errorMes;
        printf("in test_expandLimitedLeastCommonMultipleUsingElementOfGaloisFieldToRoot, struct_galoisFieldElements *multipledRootSet is NULL.\n");
        return NULL;
    }
    #endif


    if((multipledRootSet->length)<numberOfSelectedSet)
    {
                #ifndef RELEASE
                warningMes;    printf("in test_expandLimitedLeastCommonMultipleUsingElementOfGaloisFieldToRoot, ");
                warningMesShort; printf("number of selected set(=\'%d\') can not exceed to number of conjugacy set(=\'%d\').\n", numberOfSelectedSet, multipledRootSet->length);
                #endif

        numberOfSelectedSet=(multipledRootSet->length);
                #ifndef RELEASE
                warningMes; printf("number of conjugacy set that %d, which it is maximum length in this condition.\n", numberOfSelectedSet);
                #endif
    }

    for(i=0; i<numberOfSelectedSet; i++)
    {
        totalLcmOrder+=(*(multipledRootSet->limitedConjugateSet+i))->length;
    }

    /* if order is 0, length is 1, if order is n-th, length is n+1. */
    multipledOrder = abstractOrderOfEquation(equation)+totalLcmOrder;

            #ifndef RELEASE
            if(equation->length<multipledOrder+1)
            {
                errorMes; printf("in test_expandLimitedLeastCommonMultipleUsingElementOfGaloisFieldToRoot, ");
                printf("not enough space to multiply polynomials(equation order is ");
                printf("\'%d\', totalLcmOrder order is \'%d\').\n", abstractOrderOfEquation(equation), totalLcmOrder);

                return NULL;
            }
            #endif


    beforeEquation = createGaloisFieldExceptElements(equation->length);
                    #ifndef RELEASE
                    if((global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)==FLAG_MASK_PRINTF_LOG)
                    {
                        logMes; printf("in test_expandLimitedLeastCommonMultipleUsingElementOfGaloisFieldToRoot, struct_galoisFieldElements *beforeEquation is made, length is \'%d\'.\n", equation->length);
                    }
                    if((global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)==FLAG_MASK_PRINTF_LOG)
                    {
                        logMes; printf("in test_expandLimitedLeastCommonMultipleUsingElementOfGaloisFieldToRoot, i is indicator of multipled Root Set, j indicate element of conjugacy class, k is indicator of equation.\n");
                    }
                    #endif
    for(i=0; i<numberOfSelectedSet; i++)
    {
        for(j=0; j<(*(multipledRootSet->limitedConjugateSet+i))->length; j++)
        {
            //synchronize...
            if(copyListOfGaloisField(beforeEquation->element, equation->element, equation->length))
            {
                #ifndef RELEASE
                warningMes; printf("in test_expandLimitedLeastCommonMultipleUsingElementOfGaloisFieldToRoot, copyListOfGaloisField(beforeEquation->element, equation->element, equation->length) is not complete.\n");
                #endif
            }

                    #ifndef RELEASE
                    if((global_flag_cmdOption&FLAG_MASK_PRINTF_MEMORY_CHECK_PROCESS)==FLAG_MASK_PRINTF_MEMORY_CHECK_PROCESS)
                    {
                        memoryMes;
                        printf("1) in test_expandLimitedLeastCommonMultipleUsingElementOfGaloisFieldToRoot, memory check *(beforeEquation->element+global_tmp_i_memoryCheck '0' to \'%d\', equation->length is \'%d\'.\n", beforeEquation->length, equation->length);
                        printf("i=\'%d\', j=\'%d\', k=\'%d\'\n", i,j,k);
                        for(global_tmp_i_memoryCheck=0; global_tmp_i_memoryCheck<beforeEquation->length; global_tmp_i_memoryCheck++)
                        {
                            printf("*(beforeEquation->element+global_tmp_i_memoryCheck) = 0x%lx.\n", (unsigned long)(*(beforeEquation->element+global_tmp_i_memoryCheck)));
                        }
                    }
                    #endif

            for(k=0; k<equation->length; k++)
            {
                //calculate 0-th order, just multiple 0-th order and equation.
                        #ifndef RELEASE
                        if((global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)==FLAG_MASK_PRINTF_LOG)
                        {
                            logMes;
                            printf("in test_expandLimitedLeastCommonMultipleUsingElementOfGaloisFieldToRoot, \n");
                            printf("i=\'%d\', j=\'%d\', k=\'%d\'\n", i, j, k);
                            printf("*((*(multipledRootSet->limitedConjugateSet+%d))->element+%d)\t{=>>}(ADDR:\'0x%lx\') == \t", i, j, (unsigned long)(*((*(multipledRootSet->limitedConjugateSet+i))->element+j)));
                            printPolyFormWithEnter(*((*(multipledRootSet->limitedConjugateSet+i))->element+j));
                            printf("\t\t\t*(beforeEquation->element+%d)\t{=>>}(ADDR:\'0x%lx\') == \t", k, (unsigned long)(*(beforeEquation->element+k)));
                            printPolyFormWithEnter(*(beforeEquation->element+k));
                        }
                        #endif
                *(equation->element+k)=multiElementsInGF_returnAddr(galoisFields, *((*(multipledRootSet->limitedConjugateSet+i))->element+j), *(beforeEquation->element+k));
            }

                    #ifndef RELEASE
                    if((global_flag_cmdOption&FLAG_MASK_PRINTF_MEMORY_CHECK_PROCESS)==FLAG_MASK_PRINTF_MEMORY_CHECK_PROCESS)
                    {
                        memoryMes;
                        printf("2) in test_expandLimitedLeastCommonMultipleUsingElementOfGaloisFieldToRoot, memory check *(equation->element+global_tmp_i_memoryCheck) '0' to \'%d\', equation->length is \'%d\'.\n", equation->length, equation->length);
                        printf("i=\'%d\', j=\'%d\', k=\'%d\'\n", i,j,k);
                        for(global_tmp_i_memoryCheck=0; global_tmp_i_memoryCheck<equation->length; global_tmp_i_memoryCheck++)
                        {
                            printf("*(equation->element+global_tmp_i_memoryCheck) = 0x%lx.\n", (unsigned long)(*(equation->element+global_tmp_i_memoryCheck)));
                        }
                    }
                    #endif

            for(k=0; k<equation->length-1; k++)
            {
                //shifted orders
                *(equation->element+k+1)=sumElementInGF_usingSumMatrixReturnAddr(galoisFields, *(beforeEquation->element+k), *(equation->element+k+1));
            }
                    #ifndef RELEASE
                    if((global_flag_cmdOption&FLAG_MASK_PRINTF_MEMORY_CHECK_PROCESS)==FLAG_MASK_PRINTF_MEMORY_CHECK_PROCESS)
                    {
                        memoryMes;
                        printf("3) in test_expandLimitedLeastCommonMultipleUsingElementOfGaloisFieldToRoot, memory check *(equation->element+global_tmp_i_memoryCheck) '0' to \'%d\', equation->length is \'%d\'.\n", equation->length, equation->length);
                        printf("i=\'%d\', j=\'%d\', k=\'%d\'\n", i,j,k);
                        for(global_tmp_i_memoryCheck=0; global_tmp_i_memoryCheck<equation->length; global_tmp_i_memoryCheck++)
                        {
                            printf("*(equation->element+global_tmp_i_memoryCheck) = 0x%lx.\n", (unsigned long)(*(equation->element+global_tmp_i_memoryCheck)));
                        }
                    }
                    #endif
        }
    }

    closeGaloisFieldExceptElements(&beforeEquation);

    return equation;
}
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
void printSummationMatrixGFabstraction(struct_galoisFieldElements *p)
{
    unsigned int i,j;
    unsigned int exponentialTemp;

    if(!p)
    {
        errorMes;
        printf("struct_galoisFieldElements address is wrong(Addr:0x%lx).", (unsigned long)p);
    }

    printf(">> Summation Matrix\n");

    printf("+\t|\t");

    for(i=0; i<p->length; i++)
    {
        exponentialTemp=abstractIndexOfPowerFormInElementsOfGaloisField_finding(p, *(p->element+i));
        if(exponentialTemp)    printf("a^%d\t", exponentialTemp-1);
        else                printf("a^%c\t", '-');
    }
    printf("\n\t|\n");
    for(i=0; i<p->length*9; i++)
    {
        printf("-");
    }
    printf("\t|\n\t|\n");

    for(i=0; i<p->length; i++)
    {
        exponentialTemp=abstractIndexOfPowerFormInElementsOfGaloisField_finding(p, *(p->element+i));
        if(exponentialTemp)    printf("a^%d\t|\t", exponentialTemp-1);
        else                printf("a^%c\t|\t", '-');

        for(j=0; j<p->length; j++)
        {
            exponentialTemp=abstractIndexOfPowerFormInElementsOfGaloisField_finding(p, summationElementsInGaloisFieldUsingGaloisFieldInt_finding(p, *(p->element+i), *(p->element+j)));
            if(exponentialTemp) printf("a^%d\t", exponentialTemp-1);
            else printf("a^%c\t", '-');
        }
        printf("\n\t|\n\t|\n\t|\n");
    }
}
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
void printMultiplicationMatrix(struct_galoisFieldElements *p)
{
    unsigned int i,j;
    unsigned int exponentialTemp;

    if(!p)
    {
        errorMes;
        printf("struct_galoisFieldElements address is wrong(Addr:0x%lx).", (unsigned long)p);
    }

    printf(">> Multiplication Matrix\n");

    printf("*\t|\t");

    for(i=0; i<p->length; i++)
    {
        exponentialTemp=abstractIndexOfPowerFormInElementsOfGaloisField_finding(p, *(p->element+i));
        if(exponentialTemp)    printf("a^%d\t", exponentialTemp-1);
        else                printf("a^%c\t", '-');
    }
    printf("\n\t|\n");
    for(i=0; i<p->length*9; i++)
    {
        printf("-");
    }
    printf("\t|\n\t|\n");

    for(i=0; i<p->length; i++)
    {
        exponentialTemp=abstractIndexOfPowerFormInElementsOfGaloisField_finding(p, *(p->element+i));
        if(exponentialTemp)    printf("a^%d\t|\t", exponentialTemp-1);
        else                printf("a^%c\t|\t", '-');

        for(j=0; j<p->length; j++)
        {
            exponentialTemp=abstractIndexOfPowerFormInElementsOfGaloisField_finding(p, multiElementsInGF_returnAddr(p, *(p->element+i), *(p->element+j)));
            if(exponentialTemp) printf("a^%d\t", exponentialTemp-1);
            else printf("a^%c\t", '-');
        }
        printf("\n\t|\n\t|\n\t|\n");
    }
}

//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//////////////////////* Common BCH Codes Calculations start */////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
///////////////////////////////* chein search start */////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
///////////////////////* Common BCH Codes Calculations end *//////////////////////
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
/////////////////////////* BM algorithm components start *////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////* BM algorithm components end */////////////////////////
//////////////////////////////////////////////////////////////////////////////////
///////////////////////* mSBS algorithm components start *////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
////////////////////////* mSBS algorithm components end */////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
////////////////////////* mSBS algorithm components end */////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////enum treeTypes
//////////////////////////////////////////////////////////////////////////////////
struct_treeStructure* createTreeStruct(unsigned int zeroStageLength, enum treeTypes treeType)
{
    double tmp_log;
    unsigned int tmp_treeDepth;
    unsigned int tmp_treeLength;

    unsigned int i;

    struct_treeStructure *p;

    if(!zeroStageLength)
    {
        warningMes;
        printf("in createTreeStruct, zeroStageLength is zero.\n");
        return NULL;
    }
    if(treeType == TREE_NONE)
    {
        warningMes;
        printf("in createTreeStruct, enum treeTypes treeType is TREE_NONE.\n");
        warningMes;
        printf("can't create structure of tree.\n");
        return NULL;
    }

    p=(struct_treeStructure*)malloc(sizeof(struct_treeStructure));
    memset(p, 0, sizeof(struct_treeStructure));

    switch(treeType)
    {
        case TREE_CHASE:
            /* calculate depth of tree */
            tmp_log = (log10(zeroStageLength)/log10(2));
            tmp_treeDepth = (unsigned int)tmp_log;

            if((tmp_log-((double)tmp_treeDepth))==0.0)    tmp_treeDepth--;

            tmp_treeDepth++;//To store smallest loc and llr magnitudeShort
            p->treeDepth = tmp_treeDepth;

            p->treeLength = (unsigned int*)malloc(sizeof(unsigned int)*tmp_treeDepth);
            memset(p->treeLength, 0, sizeof(unsigned int)*tmp_treeDepth);

            p->tree_mag = (u_int_QUANTIZ_MAGNITUDE_DIGIT***)malloc(sizeof(u_int_QUANTIZ_MAGNITUDE_DIGIT**)*tmp_treeDepth);
            memset(p->tree_mag, 0, sizeof(u_int_QUANTIZ_MAGNITUDE_DIGIT**)*tmp_treeDepth);
            p->tree_hd= (char***)malloc(sizeof(char**)*tmp_treeDepth);
            memset(p->tree_hd, 0, sizeof(char**)*tmp_treeDepth);

            p->direction=(unsigned char **)malloc(sizeof(char*)*tmp_treeDepth);
            memset(p->direction, 0, sizeof(char*)*tmp_treeDepth);
            p->treeIndex = (unsigned int**)malloc(sizeof(unsigned int*)*tmp_treeDepth);
            memset(p->treeIndex, 0, sizeof(unsigned int*)*tmp_treeDepth);



            tmp_treeLength = zeroStageLength;

            for(i=0; i<tmp_treeDepth; i++)
            {
                if((tmp_treeLength%2)!=0)//having redundant
                {
                    tmp_treeLength=((tmp_treeLength/2)+1);
                }
                else
                {
                    tmp_treeLength/=2;
                }
                *(p->tree_mag+i) = (u_int_QUANTIZ_MAGNITUDE_DIGIT**)malloc(sizeof(u_int_QUANTIZ_MAGNITUDE_DIGIT*)*tmp_treeLength);
                memset(*(p->tree_mag+i), 0, sizeof(u_int_QUANTIZ_MAGNITUDE_DIGIT*)*tmp_treeLength);
                *(p->tree_hd+i) = (char**)malloc(sizeof(char*)*tmp_treeLength);
                memset(*(p->tree_hd+i), 0, sizeof(char*)*tmp_treeLength);
                *(p->direction+i) = (unsigned char*)malloc(sizeof(unsigned char)*tmp_treeLength);
                memset(*(p->direction+i), 0, sizeof(unsigned char)*tmp_treeLength);
                *(p->treeIndex+i) = (unsigned int*)malloc(sizeof(unsigned int)*tmp_treeLength);
                memset(*(p->treeIndex+i), -1, sizeof(unsigned int)*tmp_treeLength);

                *(p->treeLength+i) = tmp_treeLength;//store treeLength
            }
            break;

        case TREE_NONE:
        default:
            free(p);
            return NULL;
    }
    p->treeType=treeType;

    return p;
}
//////////////////////////////////////////////////////////////////////////////////
char closeTreeStruct(struct_treeStructure **p)
{
    unsigned int i;

    if(!(*p))
    {
        warningMes;
        printf("in closeTreeStruct, struct_treeStructure (*p) is NULL.\n");
        warningMes;
        printf("Already struct_treeStructure is removed.");
        return -1;
    }
    if(!((*p)->tree_mag))
    {
        warningMes;
        printf("in closeTreeStruct, struct_treeStructure (*p)->tree_mag is NULL.\n");
        return -1;
    }
    if(!((*p)->treeLength))
    {
        warningMes;
        printf("in closeTreeStruct, struct_treeStructure (*p)->treeLength is NULL.\n");
        return -1;
    }
    if(!((*p)->treeDepth))
    {
        warningMes;
        printf("in closeTreeStruct, struct_treeStructure (*p)->treeDepth is NULL.\n");
        return -1;
    }

    switch((*p)->treeType)
    {
        case TREE_CHASE:
            free((*p)->treeLength);

            for(i=0; i<(*p)->treeDepth; i++)
            {
                free(*(((*p)->tree_mag)+i));
                free(*(((*p)->tree_hd)+i));
                free(*(((*p)->direction)+i));
                free(*(((*p)->treeIndex)+i));
            }
            free((*p)->tree_mag);
            free((*p)->tree_hd);
            free((*p)->direction);
            free((*p)->treeIndex);
            break;

        case TREE_NONE:
        default:
            break;
    }
    free((*p));
    (*p)=NULL;

    return 0;
}
//////////////////////////////////////////////////////////////////////////////////
void testTreeStruct(struct_treeStructure *p)
{
    debugMes;
    printf("p->treeType = %d\n", p->treeType);

    if(!p)
    {
        warningMes;
        printf("in testTreeStruct, struct_treeStructure p is NULL.\n");
        warningMes;
        printf("Already struct_treeStructure is removed.");
        return;
    }
    debugMes;
    printf("p = 0x%lx\n", (unsigned long)p);

    if(!(p->tree_mag))
    {
        warningMes;
        printf("in testTreeStruct, struct_treeStructure p->tree_mag is NULL.\n");
        return;
    }
    if(!(p->tree_hd))
    {
        warningMes;
        printf("in testTreeStruct, struct_treeStructure p->tree_hd is NULL.\n");
        return;
    }
    debugMes;
    printf("p->tree_mag = 0x%lx, p->tree_hd = 0x%lx\n", (unsigned long)(p->tree_mag), (unsigned long)(p->tree_hd));

    if(!(p->treeIndex))
    {
        warningMes;
        printf("in testTreeStruct, struct_treeStructure p->treeIndex is NULL.\n");
        return;
    }
    debugMes;
    printf("p->treeIndex = 0x%lx\n", (unsigned long)(p->treeIndex));

    if(!(p->treeLength))
    {
        warningMes;
        printf("in testTreeStruct, struct_treeStructure p->treeLength is NULL.\n");
        return;
    }
    debugMes;
    printf("p->treeLength = 0x%lx\n", (unsigned long)(p->treeLength));

    if(!(p->treeDepth))
    {
        warningMes;
        printf("in testTreeStruct, struct_treeStructure p->treeDepth is NULL.\n");
        return;
    }
    debugMes;
    printf("p->treeDepth = %d\n", p->treeDepth);
}
void testPrintTreeStructIndicateAddr(struct_treeStructure *p)
{
    unsigned int i,j;
    if(!p)
    {
        errorMes;
        printf("in testPrintTreeStructIndicateAddr, p is NULL.\n");
        return;
    }
    if(!(p->treeType))
    {
        errorMes;
        printf("in testPrintTreeStructIndicateAddr, p->treeType is ZERO(TREE_NONE).\n");
        return;
    }
    if(!(p->tree_mag))
    {
        errorMes;
        printf("in testPrintTreeStructIndicateAddr, p->tree_mag is NULL.\n");
        return;
    }
    if(!(p->tree_hd))
    {
        errorMes;
        printf("in testPrintTreeStructIndicateAddr, p->tree_hd is NULL.\n");
        return;
    }
    if(!(p->treeLength))
    {
        errorMes;
        printf("in testPrintTreeStructIndicateAddr, p->treeLength is NULL.\n");
        return;
    }
    if(!(p->treeDepth))
    {
        errorMes;
        printf("in testPrintTreeStructIndicateAddr, p->treeDepth is ZERO.\n");
        return;
    }

    printf("/********** struct_treeStructure->tree printf address test **********/\n");
    debugMes; printf("treeStruct pointer : 0x%lx\n", (unsigned long)p);
    debugMes; printf("root pointer : p->tree_mag = 0x%lx, p->tree_hd = 0x%lx\n",
        (unsigned long)(p->tree_mag), (unsigned long)(p->tree_hd));
    for(i=0; i<p->treeDepth; i++)
    {
        debugMes; printf("%d-stage tree_mag pointer = 0x%lx, tree_hd pointer = 0x%lx\n",
                i, (unsigned long)(*(p->tree_mag+i)), (unsigned long)(*(p->tree_hd+i)));
        debugMes; printf("%d-stage length = %d\n",i, *(p->treeLength+i));
        debugMes; printf("%d-stage tree_mag and tree_hd => ", i);
        for(j=0; j<(*(p->treeLength+i)); j++)
        {
            printf("0x%lx, 0x%lx(%u) ",
                (unsigned long)(*((*(p->tree_mag+i))+j)),
                (unsigned long)(*((*(p->tree_hd+i))+j)),
                *((*(p->treeIndex+i))+j));
        }
        printf("\n");
    }
}
void testPrintTreeStructValue(struct_treeStructure *p)
{
    unsigned int i,j;
    if(!p)
    {
        errorMes;
        printf("in testPrintTreeStructIndicateAddr, p is NULL.\n");
        return;
    }
    if(!(p->treeType))
    {
        errorMes;
        printf("in testPrintTreeStructIndicateAddr, p->treeType is ZERO(TREE_NONE).\n");
        return;
    }
    if(!(p->tree_mag))
    {
        errorMes;
        printf("in testPrintTreeStructIndicateAddr, p->tree_mag is NULL.\n");
        return;
    }
    if(!(p->tree_hd))
    {
        errorMes;
        printf("in testPrintTreeStructIndicateAddr, p->tree_hd is NULL.\n");
        return;
    }
    if(!(p->treeLength))
    {
        errorMes;
        printf("in testPrintTreeStructIndicateAddr, p->treeLength is NULL.\n");
        return;
    }
    if(!(p->treeDepth))
    {
        errorMes;
        printf("in testPrintTreeStructIndicateAddr, p->treeDepth is ZERO.\n");
        return;
    }

    printf("/********** struct_treeStructure->tree_mag, tree_hd, index and direction printf value test **********/\n");
    for(i=0; i<p->treeDepth; i++)
    {
        printf("depth:%d\t", i);
        for(j=0; j<(*(p->treeLength+i)); j++)
        {
            printf("%d,'%c',%u,%c ",
                *(*((*(p->tree_mag+i))+j)),
                *(*((*(p->tree_hd+i))+j)),
                *((*(p->treeIndex+i))+j),
                *((*(p->direction+i))+j)?'R':'L');
        }
        printf("\n");
    }
}
void testPrintValueOfTreeStruct(struct_treeStructure *p)
{
    unsigned int i,j;
    if(!p)
    {
        errorMes;
        printf("in testPrintTreeStructIndicateAddr, p is NULL.\n");
        return;
    }
    if(!(p->treeType))
    {
        errorMes;
        printf("in testPrintTreeStructIndicateAddr, p->treeType is ZERO(TREE_NONE).\n");
        return;
    }
    if(!(p->tree_mag))
    {
        errorMes;
        printf("in testPrintTreeStructIndicateAddr, p->tree_mag is NULL.\n");
        return;
    }
    if(!(p->tree_hd))
    {
        errorMes;
        printf("in testPrintTreeStructIndicateAddr, p->tree_hd is NULL.\n");
        return;
    }
    if(!(p->treeLength))
    {
        errorMes;
        printf("in testPrintTreeStructIndicateAddr, p->treeLength is NULL.\n");
        return;
    }
    if(!(p->treeDepth))
    {
        errorMes;
        printf("in testPrintTreeStructIndicateAddr, p->treeDepth is ZERO.\n");
        return;
    }

    printf("/********** printf tree_mag and tree_hd value(tree Type : %d) **********/\n", p->treeType);
    for(i=0; i<p->treeDepth; i++)
    {
        printf("%d-stage\tLength = %d\n", i, *(p->treeLength+i));
        printf("[Values],[HD](index)\t");
        for(j=0; j<(*(p->treeLength+i)); j++)
        {
            printf("%d, '%c'(%d)\t",
                **(*(p->tree_mag+i)+j),
                **(*(p->tree_hd+i)+j),
                *(*(p->treeIndex+i)+j));
        }
        printf("\n");
    }
}
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
struct struct_logLikeHoodRatio
{
    double_RATIONAL_NUMBER *llr;
    double_RATIONAL_NUMBER squreRootAvrLLR;

    s_int_QUANTIZ_DIGIT *quantizedLLR;
    s_int_QUANTIZ_DIGIT quantizedLLRMask;
    //s_int_QUANTIZ_DIGIT magnitudeMask;
    s_int_QUANTIZ_DIGIT *magnitude;//intently to use -1, defined signed value
    unsigned int length;
    unsigned int usedLength;

    u_int_QUANTIZ_MAGNITUDE_DIGIT *magnitudeShort;//magnitudeShort has length that is locatorLength.
    char* hardDecisionShort;
    unsigned char *locatorValidity;
    unsigned int *locator;//locator has length that is locatorLength.
    unsigned int locatorLength;

    struct struct_treeStructure *treeStruct;
}typedef struct_logLikeHoodRatio;
//////////////////////////////////////////////////////////////////////////////////
struct_logLikeHoodRatio* createLogLikeHoodRatio(unsigned int length)
{
    struct_logLikeHoodRatio *p;

    if(!length)
    {
        warningMes;
        printf("in createLlrValueUsingBitLength, length is '%d'.\n", length);
        return NULL;
    }
    p=(struct_logLikeHoodRatio*)malloc(sizeof(struct_logLikeHoodRatio));
    memset(p, 0, sizeof(struct_logLikeHoodRatio));

    p->llr=(double_RATIONAL_NUMBER*)malloc(sizeof(double_RATIONAL_NUMBER)*length);
    memset(p->llr, 0, sizeof(sizeof(double_RATIONAL_NUMBER)*length));

    p->squreRootAvrLLR=0.0;

    p->quantizedLLR=(s_int_QUANTIZ_DIGIT*)malloc(sizeof(s_int_QUANTIZ_DIGIT)*length);
    memset(p->quantizedLLR, 0, sizeof(sizeof(s_int_QUANTIZ_DIGIT)*length));
    p->quantizedLLRMask = 0;

    p->magnitude=(s_int_QUANTIZ_DIGIT*)malloc(sizeof(s_int_QUANTIZ_DIGIT)*length);
    memset(p->magnitude, -1, sizeof(sizeof(s_int_QUANTIZ_DIGIT)*length));

    p->length=length;
    p->usedLength=0;

    //p->magnitudeShort=(s_int_QUANTIZ_DIGIT*)malloc(sizeof(s_int_QUANTIZ_DIGIT)*length);
    //memset(p->magnitudeShort, 0, sizeof(sizeof(s_int_QUANTIZ_DIGIT)*length));
    p->magnitudeShort=NULL;
    p->hardDecisionShort=NULL;
    p->locator=NULL;
    p->locatorLength=0;

    p->treeStruct=NULL;

    return p;
}
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
char closeLogLikeHoodRatio(struct_logLikeHoodRatio **p)
{
    #ifndef RELEASE
    if(!p)
    {
        warningMes;
        printf("in closeLogLikeHoodRatio, struct_logLikeHoodRatio p is NULL.\n");
        return -1;
    }
    if(!*p)
    {
        warningMes;
        printf("in closeLogLikeHoodRatio, struct_logLikeHoodRatio *p is NULL.\n");
        return -1;
    }
    #endif

    if(((*p)->llr))
    {
        free((*p)->llr);
    }
    if(((*p)->quantizedLLR))
    {
        free((*p)->quantizedLLR);
    }
    if(((*p)->magnitude))
    {
        free((*p)->magnitude);
    }
    if(((*p)->magnitudeShort))
    {
        free((*p)->magnitudeShort);
    }
    if(((*p)->hardDecisionShort))
    {
        free((*p)->hardDecisionShort);
    }
    if(((*p)->locatorValidity))
    {
        free((*p)->locatorValidity);
    }
    if(((*p)->locator))
    {
        free((*p)->locator);
    }

    closeTreeStruct(&((*p)->treeStruct));

    free(*p);
    *p=NULL;
    return 0;
}
//////////////////////////////////////////////////////////////////////////////////
char initLogLikeHoodRatioLocator(struct_logLikeHoodRatio *p, unsigned int locatorLength)
{
    #ifndef RELEASE
    if(!p)
    {
        errorMes;
        printf("in initLogLikeHoodRatioLocator, struct_logLikeHoodRatio p is NULL.\n");
        return -1;
    }
    if(!locatorLength)
    {
        warningMes;
        printf("in initLogLikeHoodRatioLocator, unsigned int locatorLength is zero.\n");
        return -1;
    }
    if((p->locator))
    {
        warningMes;
        printf("in initLogLikeHoodRatioLocator, p->locator has any values.\n");
        printf("length of p->locator is '%d'\n", p->locatorLength);
        return 1;
    }
    if((p->magnitudeShort))
    {
        warningMes;
        printf("in initLogLikeHoodRatioLocator, p->magnitudeShort has any values.\n");
        printf("length of p->magnitudeShort is '%d'\n", p->locatorLength);
        return 1;
    }
    if((p->hardDecisionShort))
    {
        warningMes;
        printf("in initLogLikeHoodRatioLocator, p->hardDecisionShort has any values.\n");
        printf("length of p->hardDecisionShort is '%d'\n", p->locatorLength);
        return 1;
    }
    #endif

    p->magnitudeShort=(u_int_QUANTIZ_MAGNITUDE_DIGIT*)malloc(sizeof(u_int_QUANTIZ_MAGNITUDE_DIGIT)*locatorLength);
    memset(p->magnitudeShort, -1, sizeof(u_int_QUANTIZ_MAGNITUDE_DIGIT)*locatorLength);

    p->hardDecisionShort=(char*)malloc(sizeof(char)*locatorLength);
    memset(p->hardDecisionShort, -1, sizeof(char)*locatorLength);

    p->locatorValidity=(unsigned char*)malloc(sizeof(unsigned int)*locatorLength);
    memset(p->locatorValidity, 0, sizeof(unsigned char)*locatorLength);

    p->locator=(unsigned int*)malloc(sizeof(unsigned int)*locatorLength);
    memset(p->locator, -1, sizeof(unsigned int)*locatorLength);

    p->locatorLength=locatorLength;
    return 0;
}
char setUsedLengthOfLogLikeHoodRatio(struct_logLikeHoodRatio *p, unsigned int usedLength)
{
    #ifndef RELEASE
    if(!p)
    {
        errorMes;
        printf("in setUsedLengthOfLogLikeHoodRatio, struct_logLikeHoodRatio p is NULL.\n");
        return -1;
    }
    if(!(p->quantizedLLR))
    {
        errorMes;
        printf("in setUsedLengthOfLogLikeHoodRatio, p->quantizedLLR is NULL.\n");
        return -1;
    }
    if(p->length<usedLength)
    {
        errorMes;
        printf("in setUsedLengthOfLogLikeHoodRatio, satisfing p->length(=%d)<usedLength(=%d).\n", p->length, usedLength);
        return -1;
    }
    #endif

    p->usedLength=usedLength;
    return 0;
}
double_RATIONAL_NUMBER getSqureRootAvrLLR(struct_logLikeHoodRatio *p)
{
    unsigned int i;
    double_RATIONAL_NUMBER tmpAvr=0.0;

    for(i=0; i<p->usedLength; i++)
    {
        tmpAvr+=((*(p->llr+i))*(*(p->llr+i)));
    }
    tmpAvr/=((double_RATIONAL_NUMBER)p->usedLength);
    sqrt(tmpAvr);
    p->squreRootAvrLLR=tmpAvr;
    return tmpAvr;
}
char setQuantizedLLRMaskOfLogLikeHoodRatio(struct_logLikeHoodRatio *p, unsigned int numberOfMask)
{
    unsigned int i;

    #ifndef RELEASE
    if(!p)
    {
        errorMes;
        printf("in setQuantizedLLRMaskOfLogLikeHoodRatio, struct_logLikeHoodRatio p is NULL.\n");
        return -1;
    }
    if(!numberOfMask)
    {
        warningMes;
        printf("in setQuantizedLLRMaskOfLogLikeHoodRatio, numberOfMask is '%d'.\n", numberOfMask);
        return -1;
    }
    #endif


    p->quantizedLLRMask=0;
    for(i=0; ((i<(sizeof(s_int_QUANTIZ_DIGIT)*8))&&(i<numberOfMask)); i++)
    {
        p->quantizedLLRMask|=(1<<i);
    }
    return 0;
}
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
/******************************************************************/
/*
 * findMinimumMagnitudeFindLocPushAway function is a convert quantized LLR to magnitudeShort and also compare each magnitudeShort so sort minimum magnitudeShort.
 * For example, let's assume that quantization bit length is 4(include sign bit), LLR values are {-1, 0, 2, 7, 8, -5, 1, 3}.
 * So LLR values are                    {1111,    0000,    0010,    0111,    0111,    1011,    0001,    0011}.
 * magnitudeShort values are            {0001,    0000,    0010,    0111,    0111,    0101,    0001,    0011}.
 * Location indexes are                 {0,       1,       2,       3,       4,       5,       6,       7   }.
 *
 * Sorted minimum magnitudes are        {0000,    0001,    0001,    0010,    0011,    0101,    0111,    0111}.
 * Sorted magnitudeShort locations are  {1,       0,       6,       2,       7,       5,       3,       4   }.
 *
 * Results of this function are(caution that, result length belong to length of locator)
 * Sorted minimum magnitudeShort are    {0000,    0001,    0001,    0010,    0011,    0101,    0111,    0111}.
 * Sorted magnitudeShort location are   {1,       0,       6,       2,       7,       5,       3,       4   }.
 */

/******************************************************************/
char findMinimumMagnitudeFindLocPushAway
(
    struct_logLikeHoodRatio *p,
    struct_powerFormPolynomials* hd_codeword,
    unsigned int zero_magnitude_sel_policy,
    unsigned int same_magnitude_sel_policy,
    unsigned int init_magnitude_policy
)
{
    u_int_QUANTIZ_MAGNITUDE_DIGIT tmp_comapared_mag = 0;

    u_int_QUANTIZ_MAGNITUDE_DIGIT tmp_criteria_mag = -1;
    unsigned int tmp_criteria_loc = -1;
    char tmp_criteria_HD;

    /* Array */
    unsigned int i,j,k;


    #ifndef RELEASE
    if(!p)
    {
        errorMes;
        printf("in findMinimumMagnitudeFindLocPushAway, struct_logLikeHoodRatio p is NULL.\n");
        return -1;
    }
    if(!(p->quantizedLLR))
    {
        warningMes;
        printf("in findMinimumMagnitudeFindLocPushAway, p->quantizedLLR is NULL.\n");
        return -1;
    }
    if(!(p->usedLength))
    {
        warningMes;
        printf("in findMinimumMagnitudeFindLocPushAway, p->usedLength is zero.\n");
        return -1;
    }
    if(!(p->locator))
    {
        warningMes;
        printf("in findMinimumMagnitudeFindLocPushAway, p->locator is NULL.\n");
        return -1;
    }
    if(!(p->magnitudeShort))
    {
        warningMes;
        printf("in findMinimumMagnitudeFindLocPushAway, p->magnitudeShort is NULL.\n");
        return -1;
    }
    if(!(p->hardDecisionShort))
    {
        warningMes;
        printf("in findMinimumMagnitudeFindLocPushAway, p->hardDecisionShort is NULL.\n");
        return -1;
    }
    if(!(p->locatorLength))
    {
        warningMes;
        printf("in findMinimumMagnitudeFindLocPushAway, p->locatorLength is zero.\n");
        return -1;
    }

    if(!hd_codeword)
    {
        errorMes;
        printf("in findMinimumMagnitudeFindLocPushAway, hd_codeword is NULL.\n");
        return -1;
    }
    if(!(hd_codeword->length))
    {
        warningMes;
        printf("in findMinimumMagnitudeFindLocPushAway, hd_codeword->length is zero.\n");
        return -1;
    }
    if(!(hd_codeword->usedLength))
    {
        warningMes;
        printf("in findMinimumMagnitudeFindLocPushAway, hd_codeword->usedLength is zero.\n");
        return -1;
    }
    if(!(hd_codeword->equation))
    {
        warningMes;
        printf("in findMinimumMagnitudeFindLocPushAway, hd_codeword->equation is NULL.\n");
        return -1;
    }
    #endif

    if(same_magnitude_sel_policy == FLAG_CASE_SAME_LLR_HANDLING_PRIOR_HD_0) tmp_criteria_HD = '1';
    else if(same_magnitude_sel_policy == FLAG_CASE_SAME_LLR_HANDLING_PRIOR_HD_1) tmp_criteria_HD = '0';


    memset(p->magnitudeShort, -1, sizeof(u_int_QUANTIZ_MAGNITUDE_DIGIT)*(p->locatorLength));
    if(same_magnitude_sel_policy == FLAG_CASE_SAME_LLR_HANDLING_PRIOR_HD_0)
    {
        memset(p->hardDecisionShort, '1', sizeof(char)*(p->locatorLength));
    }
    else if(same_magnitude_sel_policy == FLAG_CASE_SAME_LLR_HANDLING_PRIOR_HD_1)
    {
        memset(p->hardDecisionShort, '0', sizeof(char)*(p->locatorLength));
    }
    memset(p->locator, -1, sizeof(unsigned int)*(p->locatorLength));

    for(i=0; i<(p->usedLength); i++)
    {

        //if((*(p->quantizedLLR+i)) >= 0)
        //{
        //    tmp_comapared_mag = (*(p->quantizedLLR+i));
        //}
        //else
        //{
        //    tmp_comapared_mag = (-1)*(*(p->quantizedLLR+i));
        //}
        tmp_comapared_mag = *(p->magnitude + i);

        /* in no symmetry adc step, 0->1 error not have mag 0 */
        if(tmp_criteria_mag >= (tmp_comapared_mag))
        {
            if(zero_magnitude_sel_policy == FLAG_CASE_PASS_MAG0_HD_1_ENABLE)
            {
                if(*(hd_codeword->equation+i) == '1')
                {
                    if((tmp_comapared_mag) == 0)
                    {
                        continue;
                    }
                }
            }
            switch(same_magnitude_sel_policy)
            {
                case FLAG_CASE_SAME_LLR_HANDLING_NOT_SEL:
                {
                    if(tmp_criteria_mag==(tmp_comapared_mag)) continue;
                }
                break;

                case FLAG_CASE_SAME_LLR_HANDLING_PRIOR_HD_0:
                {
                    if(tmp_criteria_mag==(tmp_comapared_mag))
                    {
                        //tmp_criteria_HD is not '1' that is tmp_criteria_HD has '0'.
                        if(tmp_criteria_HD != '1')
                        {
                            //needs to compare with the others location value
                            if((*(hd_codeword->equation+i))!='0')
                            {
                                //not need to update minimum val
                                for(j=0; j<p->locatorLength; j++)
                                {
                                    /* find same or larger value */
                                    if((tmp_comapared_mag) < (*(p->magnitudeShort+j)))
                                    {
                                        /*
                                         * if find location to insert new value,
                                         * put away previous value.
                                         */
                                        if((p->locatorLength)>1)
                                        {
                                            for(k=p->locatorLength; k!=j; k--)
                                            {
                                                *(p->magnitudeShort+k-1)=*(p->magnitudeShort+k-2);
                                                *(p->locator+k-1)=*(p->locator+k-2);
                                                *(p->hardDecisionShort+k-1)=*(p->hardDecisionShort+k-2);
                                            }
                                        }
                                        /* put away is done */

                                        /* Update value */
                                        *(p->magnitudeShort+j)=(tmp_comapared_mag);
                                        *(p->locator+j)=i;
                                        *(p->hardDecisionShort+j)=(*(hd_codeword->equation+i));
                                        break;
                                    }
                                }
                                continue;
                            }
                        }
                    }
                }
                break;

                case FLAG_CASE_SAME_LLR_HANDLING_PRIOR_HD_1:
                {
                    if(tmp_criteria_mag==(tmp_comapared_mag))
                    {
                        //tmp_criteria_HD is not '0' that is tmp_criteria_HD has '1'.
                        if(tmp_criteria_HD != '0')
                        {
                            //needs to compare with the others location value
                            if((*(hd_codeword->equation+i))!='1')
                            {
                                //not need to update minimum val
                                for(j=0; j<p->locatorLength; j++)
                                {
                                    /* find same or larger value */
                                    if((tmp_comapared_mag) < (*(p->magnitudeShort+j)))
                                    {
                                        /*
                                         * if find location to insert new value,
                                         * put away previous value.
                                         */
                                        if((p->locatorLength)>1)
                                        {
                                            for(k=p->locatorLength; k!=j; k--)
                                            {
                                                *(p->magnitudeShort+k-1)=*(p->magnitudeShort+k-2);
                                                *(p->locator+k-1)=*(p->locator+k-2);
                                                *(p->hardDecisionShort+k-1)=*(p->hardDecisionShort+k-2);
                                            }
                                        }
                                        /* put away is done */

                                        /* Update value */
                                        *(p->magnitudeShort+j)=(tmp_comapared_mag);
                                        *(p->locator+j)=i;
                                        *(p->hardDecisionShort+j)=(*(hd_codeword->equation+i));
                                        break;
                                    }
                                }
                                continue;
                            }
                        }
                    }
                }
                break;

                default:
                break;
            }

            tmp_criteria_mag = (tmp_comapared_mag);
            tmp_criteria_loc = i;
            tmp_criteria_HD = *(hd_codeword->equation+i);

            for(j=0; j<p->locatorLength; j++)
            {
                /* find same or larger value */
                if(tmp_criteria_mag<=*(p->magnitudeShort+j))
                {
                    /*
                     * if find location to insert new value,
                     * put away previous value.
                     */
                    if((p->locatorLength)>1)
                    {
                        for(k=p->locatorLength; k!=j; k--)
                        {
                            *(p->magnitudeShort+k-1)=*(p->magnitudeShort+k-2);
                            *(p->locator+k-1)=*(p->locator+k-2);
                            *(p->hardDecisionShort+k-1)=*(p->hardDecisionShort+k-2);
                        }
                    }
                    /* put away is done */

                    /* Update value */
                    *(p->magnitudeShort+j)=tmp_criteria_mag;
                    *(p->locator+j)=tmp_criteria_loc;
                    *(p->hardDecisionShort+j)=tmp_criteria_HD;
                    break;
                }
            }
        }
    }
    return 0;
}
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
char findMinimumMagnitudeGroupingPushAway
(
    struct_logLikeHoodRatio *p,
    struct_powerFormPolynomials* hd_codeword,
    unsigned int zero_magnitude_sel_policy,
    unsigned int same_magnitude_sel_policy,
    unsigned int init_magnitude_policy,

    unsigned int group_nums,
    unsigned int group_bit_nums,
    unsigned int last_group_bit_nums
)
{
    u_int_QUANTIZ_MAGNITUDE_DIGIT tmp_comapared_mag;

    u_int_QUANTIZ_MAGNITUDE_DIGIT tmp_criteria_mag;
    unsigned int    tmp_criteria_loc;
    char            tmp_criteria_HD;

    /* Prior HD */
    char            tmp_priorHD;

    /* Grouping */
    unsigned int    tmp_indicate_last_group = 0;
    unsigned int    tmp_proc_group = 0;
    unsigned int    tmp_group_has_err = 0;

    /* Array */
    unsigned int i;//fallow bit loc
    unsigned int j;//find same val
    unsigned int k;//push away


    #ifndef RELEASE
    if(!p)
    {
        errorMes;
        printf("in findMinimumMagnitudeGroupingPushAwayShortMagnitude, struct_logLikeHoodRatio p is NULL.\n");
        return -1;
    }
    if(!(p->magnitude))
    {
        warningMes;
        printf("in findMinimumMagnitudeGroupingPushAwayShortMagnitude, p->magnitude is NULL.\n");
        return -1;
    }
    if(!(p->usedLength))
    {
        warningMes;
        printf("in findMinimumMagnitudeGroupingPushAwayShortMagnitude, p->usedLength is zero.\n");
        return -1;
    }
    if(!(p->locator))
    {
        warningMes;
        printf("in findMinimumMagnitudeGroupingPushAwayShortMagnitude, p->locator is NULL.\n");
        return -1;
    }
    if(!(p->magnitudeShort))
    {
        warningMes;
        printf("in findMinimumMagnitudeGroupingPushAwayShortMagnitude, p->magnitudeShort is NULL.\n");
        return -1;
    }
    if(!(p->hardDecisionShort))
    {
        warningMes;
        printf("in findMinimumMagnitudeGroupingPushAwayShortMagnitude, p->hardDecisionShort is NULL.\n");
        return -1;
    }
    if(!(p->locatorLength))
    {
        warningMes;
        printf("in findMinimumMagnitudeGroupingPushAwayShortMagnitude, p->locatorLength is zero.\n");
        return -1;
    }
    if(!group_nums)
    {
        warningMes;
        printf("in findMinimumMagnitudeGroupingPushAwayShortMagnitude, grouping is enabled but group num is 0, Forcely set group num is one.\n");
        group_nums = 1;
    }

    if(!hd_codeword)
    {
        errorMes;
        printf("in findMinimumMagnitudeGroupingPushAwayShortMagnitude, hd_codeword is NULL.\n");
        return -1;
    }
    if(!(hd_codeword->length))
    {
        warningMes;
        printf("in findMinimumMagnitudeGroupingPushAwayShortMagnitude, hd_codeword->length is zero.\n");
        return -1;
    }
    if(!(hd_codeword->usedLength))
    {
        warningMes;
        printf("in findMinimumMagnitudeGroupingPushAwayShortMagnitude, hd_codeword->usedLength is zero.\n");
        return -1;
    }
    if(!(hd_codeword->equation))
    {
        warningMes;
        printf("in findMinimumMagnitudeGroupingPushAwayShortMagnitude, hd_codeword->equation is NULL.\n");
        return -1;
    }
    #endif

    switch(init_magnitude_policy)
    {
        case FLAG_CASE_INIT_LLR_MAG_METHOD_NONE:
        default:
            tmp_criteria_mag = -1;
            tmp_criteria_loc = 0;
        break;

        case FLAG_CASE_INIT_LLR_MAG_METHOD_NOT_SEL_MAX_LLR:
            tmp_criteria_mag = -1;
            tmp_criteria_loc = -1;
        break;
    }


    if(same_magnitude_sel_policy == FLAG_CASE_SAME_LLR_HANDLING_PRIOR_HD_0)
    {
        tmp_criteria_HD = '1';
        tmp_priorHD = '0';
    }
    else if(same_magnitude_sel_policy == FLAG_CASE_SAME_LLR_HANDLING_PRIOR_HD_1)
    {
        tmp_criteria_HD = '0';
        tmp_priorHD = '1';
    }
    else
    {
        tmp_criteria_HD = 'x';
        tmp_priorHD = 'x';
    }


    memset(p->magnitudeShort, -1, sizeof(u_int_QUANTIZ_MAGNITUDE_DIGIT)*(p->locatorLength));
    if(same_magnitude_sel_policy == FLAG_CASE_SAME_LLR_HANDLING_PRIOR_HD_0)
    {
        memset(p->hardDecisionShort, '1', sizeof(char)*(p->locatorLength));
    }
    else if(same_magnitude_sel_policy == FLAG_CASE_SAME_LLR_HANDLING_PRIOR_HD_1)
    {
        memset(p->hardDecisionShort, '0', sizeof(char)*(p->locatorLength));
    }
    else
    {
        memset(p->hardDecisionShort, 'x', sizeof(char)*(p->locatorLength));
    }

    memset(p->locator, -1, sizeof(unsigned int)*(p->locatorLength));

    /* Grouping variable setting start */
    tmp_group_has_err = p->usedLength / group_nums;
    if(p->usedLength % tmp_group_has_err)
    {
        tmp_group_has_err ++;
    }
    tmp_indicate_last_group = group_nums - 1;
    /* Grouping variable setting end */


    for(i=0; i<(p->usedLength); i++)
    {

        /* n-th Group cal */
        tmp_proc_group = i / group_bit_nums;
        if(!(i % group_bit_nums))
        {
            tmp_criteria_mag = -1;
            tmp_criteria_loc = -1;
        }

        j = tmp_proc_group;

        tmp_comapared_mag = (*(p->magnitude + i));


        if(zero_magnitude_sel_policy == FLAG_CASE_PASS_MAG0_HD_1_ENABLE)
        {
            if((tmp_comapared_mag) == 0)
            {
                if(*(hd_codeword->equation+i) == '1')
                {
                    continue;
                }
            }
        }
        /* in no symmetry adc step, 0->1 error not have mag 0 */
        /* previous min llr is larger or same than compared_mag => proccessing */
        if(tmp_criteria_mag > tmp_comapared_mag)
        {
        }
        else if(tmp_criteria_mag < tmp_comapared_mag)
        {
            continue;
        }
        else//if(tmp_criteria_mag == tmp_comapared_mag)
        {
            switch(same_magnitude_sel_policy)
            {
                case FLAG_CASE_SAME_LLR_HANDLING_NOT_SEL:
                {
                    continue;
                }
                break;

                case FLAG_CASE_SAME_LLR_HANDLING_PRIOR_HD_0:
                case FLAG_CASE_SAME_LLR_HANDLING_PRIOR_HD_1:
                {
                        /* tmp_criteria_HD is not '1' that is tmp_criteria_HD has '0'. */
                        if(tmp_criteria_HD == tmp_priorHD)
                        /* criteria has prior HD. */
                        {
                            /*
                             * needs to compare with the others location value
                             * find non prior HD
                             * or
                             * lower llr mag
                             */
                            if((*(hd_codeword->equation+i)) != tmp_priorHD)
                            {
                                /* find insert loc */
                                for(
                                    j = tmp_proc_group;
                                    j < p->locatorLength;
                                    j += group_nums
                                )
                                {
                                    if((*(p->magnitudeShort + j)) > tmp_comapared_mag)
                                    {
                                        break;
                                    }
                                    if(*(p->hardDecisionShort + j) != tmp_priorHD)
                                    {
                                        break;
                                    }
                                }
                                /* not found case */
                                if(!(j < p->locatorLength))    j = tmp_proc_group;
                            }
                        }
                }
                break;

                default:
                    j = tmp_proc_group;
                break;
            }
        }

        /* push away */
        //start k = tmp_proc_group + (group_nums * (tmp_group_has_err - 1));
        for
        (
            k = tmp_proc_group + (tmp_group_has_err - 1) * group_nums;
            k != j;
            k -= group_nums
        )
        {
            if(!(k < p->locatorLength)) continue;
            else
            {
                *(p->magnitudeShort + k) = *(p->magnitudeShort + (k - group_nums));
                *(p->locator + k) = *(p->locator + (k - group_nums));
                *(p->hardDecisionShort + k) = *(p->hardDecisionShort + (k - group_nums));
            }
        }
        /* Insert value */
        *(p->magnitudeShort + j) = (tmp_comapared_mag);
        *(p->locator + j) = i;
        *(p->hardDecisionShort + j) = (*(hd_codeword->equation + i));


        /* Update criteria */
        tmp_criteria_mag = (tmp_comapared_mag);
        tmp_criteria_loc = i;
        tmp_criteria_HD = *(hd_codeword->equation + i);


    }

    /* varifycation */
    //printf("/* varify start */\r\n");
    //        printf("Short location : ");
    //        for(
    //            j = 0;
    //            j < p->locatorLength;
    //            j++
    //        )
    //        {
    //            printf("%d(%c,%d) ",
    //                    *(p->magnitudeShort + j),
    //                    *(p->hardDecisionShort + j),
    //                    *(p->locator + j)
    //            );
    //        }
    //        printf("\r\n");
    //for(i = 0; i < p->usedLength; i++)
    //{
    //    tmp_proc_group = i / group_bit_nums;
    //    if(!(i % group_bit_nums))
    //    {
    //        tmp_criteria_mag = -1;
    //    }

    //    /* set push end loc */
    //    j = tmp_proc_group;

    //    if(!(i % group_bit_nums))
    //    {
    //        printf("\r\nS loc : ");
    //        for(
    //            j = tmp_proc_group;
    //            j < p->locatorLength;
    //            j += group_nums
    //        )
    //        {
    //            printf("%d(%c,%d) ",
    //                    *(p->magnitudeShort + j),
    //                    *(p->hardDecisionShort + j),
    //                    *(p->locator + j)
    //            );
    //        }
    //        printf("\r\n");
    //    }
    //    printf("%d(%c,%d) ", (*(p->magnitude + i)), *(hd_codeword->equation+i), i);
    //}
    //printf("\r\n");
    //printf("/* varify end */\r\n");
    return 0;
}
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
void convertTwosComplementMagitude(s_int_QUANTIZ_DIGIT* to, s_int_QUANTIZ_DIGIT* from, unsigned int length, unsigned int max_val)
{
    unsigned int i;

     for(i = 0; i < length; i++)
     {
        if((*(from + i)) >= 0)//quantized LLR is positive, Hard decision is 0.
        {
            *(to + i) = (*(from + i));
        }
        else                        //quantized LLR is negative, Hard decision is 1.
        {
            *(to + i) = ((-1) * (*(from + i)));
            if(*(to + i) > max_val) *(to + i) = max_val;
        }
        //printf("Q:%d M:%d i:%d\r\n", (*(p->quantizedLLR + i)), *(p->magnitude + i), i);
     }
};
//////////////////////////////////////////////////////////////////////////////////
void convertOnesComplementMagnitude(s_int_QUANTIZ_DIGIT* to, s_int_QUANTIZ_DIGIT* from, unsigned int length)
{
    unsigned int i;

    for(i = 0; i < length; i++)
    {
        if((*(from + i)) >= 0)//quantized LLR is positive, Hard decision is 0.
        {
            *(to + i) = (*(from + i));
        }
        else                        //quantized LLR is negative, Hard decision is 1.
        {
            *(to + i) = (~(*(from + i)));
        }
        //printf("Q:%d M:%d i:%d\r\n", (*(p->quantizedLLR + i)), *(p->magnitude + i), i);
    }
};
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
char convertQuantizedLLRToMagnitude(struct_logLikeHoodRatio *p)
{
    unsigned int tmp_llr_mag_mask;

    #ifndef RELEASE
    if(!p)
    {
        errorMes;
        printf("in convertQuantizedLLRToMagnitude, struct_logLikeHoodRatio p is NULL.\n");
        return -1;
    }
    if(!(p->quantizedLLR))
    {
        warningMes;
        printf("in convertQuantizedLLRToMagnitude, p->quantizedLLR is NULL.\n");
        return -1;
    }
    if(!(p->usedLength))
    {
        warningMes;
        printf("in convertQuantizedLLRToMagnitude, p->usedLength is zero.\n");
        return -1;
    }
    if(!(p->locator))
    {
        warningMes;
        printf("in convertQuantizedLLRToMagnitude, p->locator is NULL.\n");
        return -1;
    }
    if(!(p->magnitude))
    {
        warningMes;
        printf("in convertQuantizedLLRToMagnitude, p->magnitudeShort is NULL.\n");
        return -1;
    }
    if(!(p->usedLength))
    {
        warningMes;
        printf("in convertQuantizedLLRToMagnitude, p->magnitude is zero.\n");
        return -1;
    }
    #endif

    tmp_llr_mag_mask = (p->quantizedLLRMask >> 1);

    memset(p->magnitude, tmp_llr_mag_mask, sizeof(u_int_QUANTIZ_MAGNITUDE_DIGIT)*(p->usedLength));

    switch(global_flag_case_llr_numberic_system)
    {
        /* 2'complement */
        default:
        case ENUM_FLAG_CASE_QUANTIZ_NUMBERIC_SYSTEM_TWO_S_COMPLEMENT:
        {
            convertTwosComplementMagitude(
                p->magnitude,
                p->quantizedLLR,
                p->usedLength,
                tmp_llr_mag_mask);
        }
        break;


        /* 1'complement */
        case ENUM_FLAG_CASE_QUANTIZ_NUMBERIC_SYSTEM_ONE_S_COMPLEMENT:
        {
            convertOnesComplementMagnitude(
                    p->magnitude, p->quantizedLLR, p->usedLength);
        }
        break;
    }
    return 0;
}
//////////////////////////////////////////////////////////////////////////////////
char printMagnitudeOfLLRWithSpaceAndEnter(struct_logLikeHoodRatio *p)
{
    unsigned int i;

    if(!p)
    {
        errorMes;
        printf("in printMagnitudeOfLLRWithSpaceAndEnter, struct_logLikeHoodRatio p is NULL.\n");
        return -1;
    }
    if(!(p->magnitude))
    {
        warningMes;
        printf("in printMagnitudeOfLLRWithSpaceAndEnter, struct_logLikeHoodRatio p->quantizedLLR is NULL.\n");
        return -1;
    }
    if(!(p->usedLength))
    {
        warningMes;
        printf("in printMagnitudeOfLLRWithSpaceAndEnter, struct_logLikeHoodRatio p->usedLength is zero.\n");
        return -1;
    }

    printf("[Magnitude of LLR]\r\n");
    for(i=0; i<p->usedLength; i++)
    {
            printf("%d", *(p->magnitude+i));
    }
    printf("[Used Length : %d]\n", p->usedLength);
    return 0;
}
//////////////////////////////////////////////////////////////////////////////////
char printMagnitudeOfLLR_andLocationWithSpaceAndEnter(struct_logLikeHoodRatio *p)
{
    unsigned int i;

    if(!p)
    {
        errorMes;
        printf("in printMagnitudeOfLLR_andLocationWithSpaceAndEnter, struct_logLikeHoodRatio p is NULL.\n");
        return -1;
    }
    if(!(p->magnitude))
    {
        warningMes;
        printf("in printMagnitudeOfLLR_andLocationWithSpaceAndEnter, struct_logLikeHoodRatio p->quantizedLLR is NULL.\n");
        return -1;
    }
    if(!(p->usedLength))
    {
        warningMes;
        printf("in printMagnitudeOfLLR_andLocationWithSpaceAndEnter, struct_logLikeHoodRatio p->usedLength is zero.\n");
        return -1;
    }

    printf("[Magnitude of LLR] : \t");
    for(i=0; i<p->usedLength; i++)
    {
            printf("%d(%d) ", *(p->magnitude+i), i);
    }
    printf("[Used Length : %d]\n", p->usedLength);
    return 0;
}
//////////////////////////////////////////////////////////////////////////////////
char fprintMagnitudeOfQuantizedLLR_toHex(FILE *fp, struct_logLikeHoodRatio *p, char *str_tail)
{
    unsigned int i;
    unsigned int j;
    unsigned int bitLength;
    unsigned int hexLength;
    unsigned int hexRedundantBitLength;
    unsigned int hexRedundantBitMask;
    unsigned int uint_tmp;
    const int HEX_BIT_LENGTH = 4;
    #ifndef RELEASE
    if(!fp)
    {
        errorMes;
        printf("in fprintMagnitudeOfLLR, FILE* fp is NULL.\n");
        return -1;
    }

    if(!p)
    {
        errorMes;
        printf("in fprintMagnitudeOfLLR, struct_logLikeHoodRatio p is NULL.\n");
        return -1;
    }
    if(!(p->magnitude))
    {
        warningMes;
        printf("in fprintMagnitudeOfLLR, struct_logLikeHoodRatio p->quantizedLLR is NULL.\n");
        return -1;
    }
    if(!(p->usedLength))
    {
        warningMes;
        printf("in fprintMagnitudeOfLLR, struct_logLikeHoodRatio p->usedLength is zero.\n");
        return -1;
    }
    #endif
    /* Check up LLR bit mask */
    bitLength = 0;
    uint_tmp = 0;
    uint_tmp |= p->quantizedLLRMask;
    for(i=0; uint_tmp&1; i++)
    {
        bitLength++;
        uint_tmp = (uint_tmp >> 1);
    }
    hexLength = (bitLength - 1) / HEX_BIT_LENGTH;
    hexRedundantBitLength = (bitLength - 1) % HEX_BIT_LENGTH;

    hexRedundantBitMask = 0;
    if(hexRedundantBitLength)
    {
        hexLength++;
        for(i = 0; i < hexRedundantBitLength; i++)
        {
            hexRedundantBitMask |= (1<<i);
        }
    }

    for(i = 0; i < p->usedLength; i++)
    {
        for(j = hexLength; j!= 0; j--)
        {
            if(j == hexLength)
            {
                if(hexRedundantBitLength)
                {
                    fprintf(fp, "%01x", (((*(p->magnitude+i)) >> ((j-1)*4)) & hexRedundantBitMask));
                }
                else
                {
                    fprintf(fp, "%01x", (((*(p->magnitude+i)) >> ((j-1)*4)) & 0xF));
                }
            }
            else
            {
                fprintf(fp, "%01x", (((*(p->magnitude+i)) >> ((j-1)*4)) & 0xF));
            }
        }
    }

    if(str_tail) fprintf(fp, str_tail);
    return 0;
}
//////////////////////////////////////////////////////////////////////////////////
char fprintVerilogMagnitudeOfQuantizedLLR_toHex(FILE *fp, struct_logLikeHoodRatio *p, char *str_tail)
{
    unsigned int i;
    unsigned int j;
    unsigned int bitLength;
    unsigned int hexLength;
    unsigned int hexRedundantBitLength;
    unsigned int hexRedundantBitMask;
    unsigned int uint_tmp;
    const int HEX_BIT_LENGTH = 4;
    #ifndef RELEASE
    if(!fp)
    {
        errorMes;
        printf("in fprintMagnitudeOfLLR, FILE* fp is NULL.\n");
        return -1;
    }

    if(!p)
    {
        errorMes;
        printf("in fprintMagnitudeOfLLR, struct_logLikeHoodRatio p is NULL.\n");
        return -1;
    }
    if(!(p->magnitude))
    {
        warningMes;
        printf("in fprintMagnitudeOfLLR, struct_logLikeHoodRatio p->quantizedLLR is NULL.\n");
        return -1;
    }
    if(!(p->usedLength))
    {
        warningMes;
        printf("in fprintMagnitudeOfLLR, struct_logLikeHoodRatio p->usedLength is zero.\n");
        return -1;
    }
    #endif
    /* Check up LLR bit mask */
    bitLength = 0;
    uint_tmp = 0;
    uint_tmp |= p->quantizedLLRMask;
    for(i=0; uint_tmp&1; i++)
    {
        bitLength++;
        uint_tmp = (uint_tmp >> 1);
    }
    hexLength = (bitLength - 1) / HEX_BIT_LENGTH;
    hexRedundantBitLength = (bitLength - 1) % HEX_BIT_LENGTH;

    hexRedundantBitMask = 0;
    if(hexRedundantBitLength)
    {
        hexLength++;
        for(i = 0; i < hexRedundantBitLength; i++)
        {
            hexRedundantBitMask |= (1<<i);
        }
    }

    for(i = 0; i < p->usedLength; i++)
    {
        for(j = hexLength; j!= 0; j--)
        {
            if(j == hexLength)
            {
                if(hexRedundantBitLength)
                {
                    fprintf(fp, "%01x\r\n", (((*(p->magnitude+i)) >> ((j-1)*4)) & hexRedundantBitMask));
                }
                else
                {
                    fprintf(fp, "%01x\r\n", (((*(p->magnitude+i)) >> ((j-1)*4)) & 0xF));
                }
            }
            else
            {
                fprintf(fp, "%01x\r\n", (((*(p->magnitude+i)) >> ((j-1)*4)) & 0xF));
            }
        }
    }

    if(str_tail) fprintf(fp, str_tail);
    return 0;
}
//////////////////////////////////////////////////////////////////////////////////
char fprintMagnitudeOfQuantizedLLR_fullDescription_toHex(FILE *fp, struct_logLikeHoodRatio *p, char *str_tail)
{
    unsigned int i;
    unsigned int j;
    unsigned int bitLength;
    unsigned int hexLength;
    unsigned int hexRedundantBitLength;
    unsigned int hexRedundantBitMask;
    unsigned int uint_tmp;
    const int HEX_BIT_LENGTH = 4;

    #ifndef RELEASE
    if(!fp)
    {
        errorMes;
        printf("in fprintMagnitudeOfQuantizedLLR_fullDescription_toHex, FILE* fp is NULL.\n");
        return -1;
    }
    if(!p)
    {
        errorMes;
        printf("in fprintMagnitudeOfQuantizedLLR_fullDescription_toHex, struct_logLikeHoodRatio p is NULL.\n");
        return -1;
    }
    if(!(p->quantizedLLR))
    {
        warningMes;
        printf("in fprintMagnitudeOfQuantizedLLR_fullDescription_toHex, struct_logLikeHoodRatio p->quantizedLLR is NULL.\n");
        return -1;
    }
    if(!(p->usedLength))
    {
        warningMes;
        printf("in fprintMagnitudeOfQuantizedLLR_fullDescription_toHex, struct_logLikeHoodRatio p->usedLength is zero.\n");
        return -1;
    }
    if(!(p->quantizedLLRMask))
    {
        warningMes;
        printf("in fprintMagnitudeOfQuantizedLLR_fullDescription_toHex, struct_logLikeHoodRatio p->quantizedLLRMask is zero.\n");
        return -1;
    }
    #endif

    if(!(p->quantizedLLRMask))
    {
        warningMes;
        printf("in fprintMagnitudeOfQuantizedLLR_fullDescription_toHex, quantization bit mask not exist.\r\b");
        return 1;
    }

    /* Check up LLR bit mask */
    bitLength = 0;
    uint_tmp = 0;
    uint_tmp |= p->quantizedLLRMask;
    for(i=0; uint_tmp&1; i++)
    {
        bitLength++;
        uint_tmp = (uint_tmp >> 1);
    }
    hexLength = (bitLength - 1) / HEX_BIT_LENGTH;
    hexRedundantBitLength = (bitLength - 1) % HEX_BIT_LENGTH;

    hexRedundantBitMask = 0;
    if(hexRedundantBitLength)
    {
        hexLength++;
        for(i = 0; i < hexRedundantBitLength; i++)
        {
            hexRedundantBitMask |= (1<<i);
        }
    }


    for(i=0; i<p->usedLength; i++)
    {
        if(*(p->quantizedLLR+i)>0)    fprintf(fp, "+%d(", *(p->quantizedLLR+i));
        else                        fprintf(fp, "+%d(", -1*(*(p->quantizedLLR+i)));

        for(j = hexLength; j!= 0; j--)
        {
            if(j == hexLength)
            {
                if(hexRedundantBitLength)
                {
                    fprintf(fp, "%01x", (((*(p->magnitude+i)) >> ((j-1)*4)) & hexRedundantBitMask));
                }
                else
                {
                    fprintf(fp, "%01x", (((*(p->magnitude+i)) >> ((j-1)*4)) & 0xF));
                }
            }
            else
            {
                fprintf(fp, "%01x", (((*(p->magnitude+i)) >> ((j-1)*4)) & 0xF));
            }
        }
        fprintf(fp, ") ");
    }

    fprintf(fp, "[Used Length : %d]", p->usedLength);

    if(str_tail) fprintf(fp, str_tail);

    return 0;
}
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
/*
 * To find minumum value of magnitude, initializing values associate with tree in struct struct_logLikeHoodRatio.
 * Tree, can use, is define that is in enum treeTypes.
 * If you needs this list, find a enum treeTypes.
 */
#define DIRECTION_LEFT    0
#define DIRECTION_RIGHT    1
char initializingVariablesAssociateWithTree(struct_logLikeHoodRatio *p, enum treeTypes treeType)
{
    #ifndef RELEASE
    if(!p)
    {
        warningMes;
        printf("in initializingVariablesAssociateWithTree, struct_logLikeHoodRatio p is NULL.\n");
        return -1;
    }
    if(!(p->usedLength))
    {
        warningMes;
        printf("in initializingVariablesAssociateWithTree, struct_logLikeHoodRatio p->usedLength is zero.\n");
        warningMes;
        printf("To initializing tree, this value must be setted, first.\n");
        return -1;
    }
    #endif

    if((p->treeStruct))
    {
                #ifndef RELEASE
                warningMes;    printf("in initializingVariablesAssociateWithTree, p->treeStruct is not NULL.\n");
                warningMesShort;    printf("p->treeStruct will be removed.\n");
                #endif

        closeTreeStruct(&(p->treeStruct));
    }

    (p->treeStruct)=createTreeStruct(p->usedLength, treeType);
            #ifndef RELEASE
            if(!(p->treeStruct))//create tree
            {
                warningMes;
                printf("in initializingVariablesAssociateWithTree, can't createTreeStruct.\n");
                return -1;
            }
            #endif

    return 0;
}
//////////////////////////////////////////////////////////////////////////////////

char findMinimumMagnitudeLogLikeHoodRatio_chaseAlgorithm_followMinPath(
    struct_logLikeHoodRatio *p)
{
    unsigned int i;
    int tmp_depth;
    unsigned int tmp_row_loc;
    unsigned char tmp_direction;
    #ifndef RELEASE
    if(!p)
    {
        errorMes;
        printf("in findMinimumMagnitudeLogLikeHoodRatio_chaseAlgorithm_followMinPath, struct_logLikeHoodRatio p is NULL.\n");
        return -1;
    }
    if(!(p->quantizedLLR))
    {
        warningMes;
        printf("in findMinimumMagnitudeLogLikeHoodRatio_chaseAlgorithm_followMinPath, p->quantizedLLR is NULL.\n");
        return -1;
    }
    if(!(p->usedLength))
    {
        warningMes;
        printf("in findMinimumMagnitudeLogLikeHoodRatio_chaseAlgorithm_followMinPath, p->usedLength is zero.\n");
        return -1;
    }
    if(!(p->locator))
    {
        warningMes;
        printf("in findMinimumMagnitudeLogLikeHoodRatio_chaseAlgorithm_followMinPath, p->locator is NULL.\n");
        return -1;
    }
    if(!(p->magnitude))
    {
        warningMes;
        printf("in findMinimumMagnitudeLogLikeHoodRatio_chaseAlgorithm_followMinPath, p->magnitude is NULL.\n");
        return -1;
    }
    if(!(p->magnitudeShort))
    {
        warningMes;
        printf("in findMinimumMagnitudeLogLikeHoodRatio_chaseAlgorithm_followMinPath, p->magnitudeShort is NULL.\n");
        return -1;
    }
    if(!(p->hardDecisionShort))
    {
        warningMes;
        printf("in findMinimumMagnitudeLogLikeHoodRatio_chaseAlgorithm_followMinPath, p->hardDecisionShort is NULL.\n");
        return -1;
    }
    if(!(p->locatorLength))
    {
        warningMes;
        printf("in findMinimumMagnitudeLogLikeHoodRatio_chaseAlgorithm_followMinPath, p->locatorLength is zero.\n");
        return -1;
    }
    if(!(p->treeStruct))
    {
        warningMes;
        printf("in findMinimumMagnitudeLogLikeHoodRatio_chaseAlgorithm_followMinPath, p->treeStruct is NULL.\n");
        return -1;
    }
    if(!(p->treeStruct->tree_mag))
    {
        warningMes;
        printf("in findMinimumMagnitudeLogLikeHoodRatio_chaseAlgorithm_followMinPath, p->treeStruct->tree_mag is NULL.\n");
        return -1;
    }
    #endif

    tmp_depth=p->treeStruct->treeDepth;
    tmp_row_loc=0;
    tmp_direction=0;

    /* Initializing minimum LLR value start */
    /* Find 2nd, 3rd minimum LLR using 1st smallest LLR */
    *(p->locator+0) = *((*(p->treeStruct->treeIndex+(tmp_depth-1)))+tmp_row_loc);
    *(p->magnitude+0) = **(*(p->treeStruct->tree_mag+(tmp_depth-1))+tmp_row_loc);
    tmp_direction = *(*(p->treeStruct->direction+tmp_depth-1)+tmp_row_loc);
    tmp_row_loc += tmp_direction;
    tmp_depth--;
    /* Initializing minimum LLR value end */

    /* 1st locator is set above codes */
    /* row location, also, is set */
    /* until follow 1st minimum value, which is located on top, finds min values. */
    for(i=1; i<p->locatorLength; i++)
    {
        if(tmp_direction)//right
        {
            *(p->locator+i) = *((*(p->treeStruct->treeIndex+(tmp_depth-1)))+tmp_row_loc-1);
            *(p->magnitude+i) = **(*(p->treeStruct->tree_mag+(tmp_depth-1))+tmp_row_loc-1);
        }
        else//left
        {
            *(p->locator+i) = *((*(p->treeStruct->treeIndex+(tmp_depth-1)))+tmp_row_loc+1);
            *(p->magnitude+i) = **(*(p->treeStruct->tree_mag+(tmp_depth-1))+tmp_row_loc+1);
        }
        tmp_direction = *(*(p->treeStruct->direction+tmp_depth-1)+tmp_row_loc);
        if(tmp_direction)//right
        {
            tmp_row_loc+=tmp_row_loc+DIRECTION_RIGHT;
        }
        else//left
        {
            tmp_row_loc+=tmp_row_loc+DIRECTION_LEFT;
        }
        tmp_depth--;
        //if(!tmp_depth) printf("if(!tmp_depth) i[%d]\n", i);
    }

    //for(i=0; i<p->locatorLength; i++)
    //{
    //    printf("V[%d]L[%d] ", *(p->magnitude+i), *(p->locator+i));
    //}
    //printf("\n");
    //printf("test) value[%d],loc[%d]", **(*(p->treeStruct->tree_mag+(p->treeStruct->treeDepth-1))+0), *((*(p->treeStruct->treeIndex+(p->treeStruct->treeDepth-1)))+0));

    return 0;
}
/*
 * char sortMinimumMagnitudeLogLikeHoodRatio_chaseAlgorithm(struct_logLikeHoodRatio *p)
 *
 * m_(0)   \
 *         comp -> t(0,0), val:m_(0), direct:left, index:0
 * m_(1)   /       |
 *              comp -> t(1,0), val:t(0,0), direct:left, index:0   \
 * m_(2)   \       |                                               |
 *         comp -> t(0,1), val:m_(3), direct:right, index:3        |
 * m_(3)   /                                                       |
 *                                                             comp -> t(2,0), val:t(1,1), direct:right, index:6
 * m_(4)   \                                                       |
 *         comp -> t(0,2), val:m_(5), direct:right, index:5        |
 * m_(5)   /       |                                               |
 *             comp -> t(1,1), val:t(0,3), direct right. index:6   /
 * m_(6)   \       |
 *         comp -> t(0,3), val:m_(6), direct:left, index:6
 * m_(7)   /
 *
 * .
 *
 * .
 *                                                                 \
 * .                                                               |
 *                                                                 |
 * .                                                               |
 *                                                             comp -> t( ), val:t(2,(n-1)/2), direct:right, index:n-2
 * m_(n-4) \                                                       |
 *         comp -> t(0,(n-1/2)-1), m_(n-4), left, index:(n-4)      |
 * m_(n-3) /       |                                               |
 *             comp -> t(2,(n-1)/2), right, index:(n-2)            /
 * m_(n-2) \       |
 *         comp -> t(0,(n-1)/2), m_(n-2), left, index:(n-2)
 * m_(n-1) /
 */
char sortMinimumMagnitudeLogLikeHoodRatio_chaseAlgorithm(
    struct_logLikeHoodRatio *p,
    struct_powerFormPolynomials *hd_codeword,
    unsigned int zero_magnitude_sel_policy,
    unsigned int same_magnitude_sel_policy)
{
    unsigned int depth;
    unsigned int i;

    #ifndef RELEASE
    if(!p)
    {
        errorMes;
        printf("in sortMinimumMagnitudeLogLikeHoodRatio_chaseAlgorithm, struct_logLikeHoodRatio p is NULL.\n");
        return -1;
    }
    if(!(p->quantizedLLR))
    {
        warningMes;
        printf("in sortMinimumMagnitudeLogLikeHoodRatio_chaseAlgorithm, p->quantizedLLR is NULL.\n");
        return -1;
    }
    if(!(p->usedLength))
    {
        warningMes;
        printf("in sortMinimumMagnitudeLogLikeHoodRatio_chaseAlgorithm, p->usedLength is zero.\n");
        return -1;
    }
    if(!(p->locator))
    {
        warningMes;
        printf("in sortMinimumMagnitudeLogLikeHoodRatio_chaseAlgorithm, p->locator is NULL.\n");
        return -1;
    }
    if(!(p->magnitude))
    {
        warningMes;
        printf("in sortMinimumMagnitudeLogLikeHoodRatio_chaseAlgorithm, p->magnitude is NULL.\n");
        return -1;
    }
    if(!(p->treeStruct))
    {
        warningMes;
        printf("in sortMinimumMagnitudeLogLikeHoodRatio_chaseAlgorithm, p->treeStruct is NULL.\n");
        return -1;
    }
    if(!(p->treeStruct->tree_mag))
    {
        warningMes;
        printf("in sortMinimumMagnitudeLogLikeHoodRatio_chaseAlgorithm, p->treeStruct->tree_mag is NULL.\n");
        return -1;
    }
    if(!(p->treeStruct->tree_hd))
    {
        warningMes;
        printf("in sortMinimumMagnitudeLogLikeHoodRatio_chaseAlgorithm, p->treeStruct->tree_hd is NULL.\n");
        return -1;
    }

    if(!hd_codeword)
    {
        errorMes;
        printf("in sortMinimumMagnitudeLogLikeHoodRatio_chaseAlgorithm, hd_codeword is NULL.\n");
        return -1;
    }
    if(!(hd_codeword->length))
    {
        warningMes;
        printf("in sortMinimumMagnitudeLogLikeHoodRatio_chaseAlgorithm, hd_codeword->length is zero.\n");
        return -1;
    }
    if(!(hd_codeword->usedLength))
    {
        warningMes;
        printf("in sortMinimumMagnitudeLogLikeHoodRatio_chaseAlgorithm, hd_codeword->usedLength is zero.\n");
        return -1;
    }
    if(!(hd_codeword->equation))
    {
        warningMes;
        printf("in sortMinimumMagnitudeLogLikeHoodRatio_chaseAlgorithm, hd_codeword->equation is NULL.\n");
        return -1;
    }
    #endif

    if(global_bch_SoftCorrectable>p->treeStruct->treeDepth)
    {
        errorMes;
        printf("Finding minimum magnitude of LLR using chaseAlgorithm, is not support this condition(p(%d) larger than depth(%d)), yet.\n", global_bch_SoftCorrectable, p->treeStruct->treeDepth);
    }

    /* 0-th stage of tree */
            //debugMes; printf(">>> 0-th stage, tot length(%d)\n", p->usedLength);
    for(i = 0; i < p->usedLength; i += 2)
    {
        /* tree element is one? */
        if(p->usedLength - i != 1)
        {
            /* tree elements are 2 */
            if(*(p->magnitude + i + DIRECTION_LEFT) <= *(p->magnitude + i + DIRECTION_RIGHT))
            {
                /* update left one */
                if(zero_magnitude_sel_policy == FLAG_CASE_PASS_MAG0_HD_1_ENABLE)
                {
                    if(*((hd_codeword->equation + i + DIRECTION_LEFT)) == '1')
                    {
                        if(*(p->magnitude + i + DIRECTION_LEFT) == 0)
                        {
                            *(*(p->treeStruct->tree_mag + 0) + i / 2) = \
                                (u_int_QUANTIZ_MAGNITUDE_DIGIT*)(p->magnitude + i + DIRECTION_RIGHT);
                            *(*(p->treeStruct->tree_hd + 0) + i / 2) = \
                                (hd_codeword->equation + i + DIRECTION_RIGHT);
                            *(*(p->treeStruct->direction + 0) + i / 2) = DIRECTION_RIGHT;
                            *(*(p->treeStruct->treeIndex + 0) + i / 2) = i + DIRECTION_RIGHT;
                            continue;
                        }
                    }
                }

                /* Both left and right have same maginude of LLR. */
                if(*(p->magnitude + i + DIRECTION_LEFT) == *(p->magnitude + i + DIRECTION_RIGHT))
                {
                    switch(same_magnitude_sel_policy)
                    {
                        case FLAG_CASE_SAME_LLR_HANDLING_NOT_SEL:
                        {}
                        break;

                        case FLAG_CASE_SAME_LLR_HANDLING_PRIOR_HD_0:
                        {
                            /* a priority has left one. */
                            if(*((hd_codeword->equation + i + DIRECTION_LEFT)) == '0')
                            {
                                *(*(p->treeStruct->tree_mag + 0) + i / 2) = \
                                    (u_int_QUANTIZ_MAGNITUDE_DIGIT*)(p->magnitude + i + DIRECTION_LEFT);
                                *(*(p->treeStruct->tree_hd+ 0) + i / 2) = \
                                    (hd_codeword->equation + i + DIRECTION_LEFT);
                                *(*(p->treeStruct->direction + 0) + i / 2) = DIRECTION_LEFT;
                                *(*(p->treeStruct->treeIndex + 0) + i / 2) = i + DIRECTION_LEFT;
                                continue;
                            }
                            /* a second priority has right. */
                            else if(*((hd_codeword->equation + i + DIRECTION_RIGHT)) == '0')
                            {
                                *(*(p->treeStruct->tree_mag + 0) + i / 2) = \
                                    (u_int_QUANTIZ_MAGNITUDE_DIGIT*)(p->magnitude + i + DIRECTION_RIGHT);
                                *(*(p->treeStruct->tree_hd+ 0) + i / 2) = \
                                    (hd_codeword->equation + i + DIRECTION_RIGHT);
                                *(*(p->treeStruct->direction + 0) + i / 2) = DIRECTION_RIGHT;
                                *(*(p->treeStruct->treeIndex + 0) + i / 2) = i + DIRECTION_RIGHT;
                                continue;
                            }
                        }
                        break;

                        case FLAG_CASE_SAME_LLR_HANDLING_PRIOR_HD_1:
                        {
                            /* a priority has left one. */
                            if(*((hd_codeword->equation + i + DIRECTION_LEFT)) == '1')
                            {
                                *(*(p->treeStruct->tree_mag + 0) + i / 2) = \
                                    (u_int_QUANTIZ_MAGNITUDE_DIGIT*)(p->magnitude + i + DIRECTION_LEFT);
                                *(*(p->treeStruct->tree_hd+ 0) + i / 2) = \
                                    (hd_codeword->equation + i + DIRECTION_LEFT);
                                *(*(p->treeStruct->direction + 0) + i / 2) = DIRECTION_LEFT;
                                *(*(p->treeStruct->treeIndex + 0) + i / 2) = i + DIRECTION_LEFT;
                                continue;
                            }
                            /* a second priority has right. */
                            else if(*((hd_codeword->equation + i + DIRECTION_RIGHT)) == '1')
                            {
                                *(*(p->treeStruct->tree_mag + 0) + i / 2) = \
                                    (u_int_QUANTIZ_MAGNITUDE_DIGIT*)(p->magnitude + i + DIRECTION_RIGHT);
                                *(*(p->treeStruct->tree_hd+ 0) + i / 2) = \
                                    (hd_codeword->equation + i + DIRECTION_RIGHT);
                                *(*(p->treeStruct->direction + 0) + i / 2) = DIRECTION_RIGHT;
                                *(*(p->treeStruct->treeIndex + 0) + i / 2) = i + DIRECTION_RIGHT;
                                continue;
                            }
                        }
                        break;

                        default:
                        break;
                    }

                }

                *(*(p->treeStruct->tree_mag + 0) + i / 2) = \
                    (u_int_QUANTIZ_MAGNITUDE_DIGIT*)(p->magnitude + i + DIRECTION_LEFT);
                *(*(p->treeStruct->tree_hd+ 0) + i / 2) = \
                    (hd_codeword->equation + i + DIRECTION_LEFT);
                *(*(p->treeStruct->direction + 0) + i / 2) = DIRECTION_LEFT;
                *(*(p->treeStruct->treeIndex + 0) + i / 2) = i + DIRECTION_LEFT;

            }
            else
            {
                /* update right one */
                if(zero_magnitude_sel_policy == FLAG_CASE_PASS_MAG0_HD_1_ENABLE)
                {
                    if(*((hd_codeword->equation + i + DIRECTION_LEFT)) == '1')
                    {
                        if(*(p->magnitude + i + DIRECTION_LEFT) == 0)
                        {
                            *(*(p->treeStruct->tree_mag + 0) + i / 2) = \
                                (u_int_QUANTIZ_MAGNITUDE_DIGIT*)(p->magnitude + i + DIRECTION_LEFT);
                            *(*(p->treeStruct->tree_hd + 0) + i / 2) = \
                                (hd_codeword->equation + i + DIRECTION_LEFT);
                            *(*(p->treeStruct->direction + 0) + i / 2) = DIRECTION_LEFT;
                            *(*(p->treeStruct->treeIndex + 0) + i / 2) = i + DIRECTION_LEFT;
                            continue;
                        }
                    }
                }
                *(*(p->treeStruct->tree_mag + 0) + i / 2) = \
                    (u_int_QUANTIZ_MAGNITUDE_DIGIT*)(p->magnitude + i + DIRECTION_RIGHT);
                *(*(p->treeStruct->tree_hd + 0) + i / 2) = \
                    (hd_codeword->equation + i + DIRECTION_RIGHT);
                *(*(p->treeStruct->direction + 0) + i / 2) = DIRECTION_RIGHT;
                *(*(p->treeStruct->treeIndex + 0) + i / 2) = i + DIRECTION_RIGHT;
            }
                    //printf("depth(0),length(%d), %d(%d) <= %d(%d) --->>> ", i,  *(p->magnitude+i), i, *(p->magnitude+i+1), i+1);
                    //printf("%d(%d)\n", **(*(p->treeStruct->tree_mag+0)+i/2), *(*(p->treeStruct->treeIndex+0)+i/2));
        }
        else
        {
            /* tree element is only one. */

            /* store redundant, no comparing, directly update to 0th stage tree. */
            /* update left one, because right one is not exist */
            *((*(p->treeStruct->tree_mag) + 0) + i / 2) = (u_int_QUANTIZ_MAGNITUDE_DIGIT*)(p->magnitude + i - 1);
            *(*(p->treeStruct->tree_hd + 0) + i / 2) = \
                (hd_codeword->equation + i + DIRECTION_LEFT);
            *(*(p->treeStruct->direction + 0) + i / 2) = DIRECTION_LEFT;
            *(*(p->treeStruct->treeIndex + 0) + i / 2) = i - 1;
                    //printf("EXTRA)) depth(0),length(%d), %d(%d) --->>> ", i, *(p->magnitude+i-1), i-1);
                    //printf("%d(%d)\n", **(*(p->treeStruct->tree_mag+0)+i/2), *(*(p->treeStruct->treeIndex+0)+i/2));
        }
    }

    /* 1st and 2nd, 3rd .... stage(depth) tree */
    for(depth = 1; depth < p->treeStruct->treeDepth; depth++)
    {
                //debugMes; printf(">>> %d-th stage, tot Length(%d)\n", depth, *(p->treeStruct->treeLength+depth-1));
        /* check element of stage(depth) */
        for(i = 0; i < *(p->treeStruct->treeLength + depth - 1); i += 2)
        {
            /* if can comparing */
            if(*(p->treeStruct->treeLength+depth - 1) - i != 1)
            {
                        //printf("depth(%d), length(%d), %d(%d) <= %d(%d) --->>> ", depth, i, **(*(p->treeStruct->tree_mag+depth-1)+i), *(*(p->treeStruct->treeIndex+depth-1)+i), **(*(p->treeStruct->tree_mag+depth-1)+i+1), *(*(p->treeStruct->treeIndex+depth-1)+i+1));
                /* left(lower index) smaller or same than right(higher index) */
                if(
                        **(*(p->treeStruct->tree_mag + depth - 1) + i + DIRECTION_LEFT)
                        <=
                        **(*(p->treeStruct->tree_mag + depth - 1) + i + DIRECTION_RIGHT))
                {
                    if(zero_magnitude_sel_policy == FLAG_CASE_PASS_MAG0_HD_1_ENABLE)
                    {
                        if(**(*(p->treeStruct->tree_hd + depth - 1) + i + DIRECTION_LEFT) == '1')
                        {
                            if(**(*(p->treeStruct->tree_mag + depth - 1) + i + DIRECTION_LEFT) == 0)
                            {
                                *(*(p->treeStruct->tree_mag + depth) + i / 2) = \
                                    *(*(p->treeStruct->tree_mag + depth - 1) + i + DIRECTION_RIGHT);
                                *(*(p->treeStruct->tree_hd + depth) + i / 2) = \
                                    *(*(p->treeStruct->tree_hd + depth - 1) + i + DIRECTION_RIGHT);
                                *(*(p->treeStruct->direction+depth) + i / 2) = DIRECTION_RIGHT;
                                *(*(p->treeStruct->treeIndex+depth) + i / 2) = \
                                    *(*(p->treeStruct->treeIndex + depth - 1) + i + DIRECTION_RIGHT);
                                continue;
                            }
                        }
                    }


                    /* Both left and right have same maginude of LLR. */
                    if(*(p->magnitude + i + DIRECTION_LEFT) == *(p->magnitude + i + DIRECTION_RIGHT))
                    {
                        switch(same_magnitude_sel_policy)
                        {
                            /* This function is working on full-parallel so not to do */
                            case FLAG_CASE_SAME_LLR_HANDLING_NOT_SEL:
                            {
                            }
                            break;

                            case FLAG_CASE_SAME_LLR_HANDLING_PRIOR_HD_0:
                            {
                                if(**(*(p->treeStruct->tree_hd + depth - 1) \
                                    + i + DIRECTION_LEFT) == '0')
                                {
                                    *(*(p->treeStruct->tree_mag + depth) + i / 2) = \
                                        *(*(p->treeStruct->tree_mag + depth - 1) + i + DIRECTION_LEFT);
                                    *(*(p->treeStruct->tree_hd + depth) + i / 2) = \
                                        *(*(p->treeStruct->tree_hd + depth - 1) + i + DIRECTION_LEFT);
                                    *(*(p->treeStruct->direction + depth) + i / 2) = DIRECTION_LEFT;
                                    *(*(p->treeStruct->treeIndex + depth) + i / 2) = \
                                    *(*(p->treeStruct->treeIndex + depth - 1) + i + DIRECTION_LEFT);
                                    continue;
                                }
                                else if(**(*(p->treeStruct->tree_hd + depth - 1) \
                                + i + DIRECTION_RIGHT) == '0')
                                {
                                    *(*(p->treeStruct->tree_mag + depth) + i / 2) = \
                                        *(*(p->treeStruct->tree_mag + depth - 1) + i + DIRECTION_RIGHT);
                                    *(*(p->treeStruct->tree_hd + depth) + i / 2) = \
                                        *(*(p->treeStruct->tree_hd + depth - 1) + i + DIRECTION_RIGHT);
                                    *(*(p->treeStruct->direction + depth) + i / 2) = DIRECTION_RIGHT;
                                    *(*(p->treeStruct->treeIndex + depth) + i / 2) = \
                                    *(*(p->treeStruct->treeIndex + depth - 1) + i + DIRECTION_RIGHT);
                                    continue;
                                }
                            }
                            break;

                            case FLAG_CASE_SAME_LLR_HANDLING_PRIOR_HD_1:
                            {
                                if(**(*(p->treeStruct->tree_hd + depth - 1) \
                                    + i + DIRECTION_LEFT) == '1')
                                {
                                    *(*(p->treeStruct->tree_mag + depth) + i / 2) = \
                                        *(*(p->treeStruct->tree_mag + depth - 1) + i + DIRECTION_LEFT);
                                    *(*(p->treeStruct->tree_hd + depth) + i / 2) = \
                                        *(*(p->treeStruct->tree_hd + depth - 1) + i + DIRECTION_LEFT);
                                    *(*(p->treeStruct->direction + depth) + i / 2) = DIRECTION_LEFT;
                                    *(*(p->treeStruct->treeIndex + depth) + i / 2) = \
                                    *(*(p->treeStruct->treeIndex + depth - 1) + i + DIRECTION_LEFT);
                                    continue;
                                }
                                else if(**(*(p->treeStruct->tree_hd + depth - 1) \
                                + i + DIRECTION_RIGHT) == '1')
                                {
                                    *(*(p->treeStruct->tree_mag + depth) + i / 2) = \
                                        *(*(p->treeStruct->tree_mag + depth - 1) + i + DIRECTION_RIGHT);
                                    *(*(p->treeStruct->tree_hd + depth) + i / 2) = \
                                        *(*(p->treeStruct->tree_hd + depth - 1) + i + DIRECTION_RIGHT);
                                    *(*(p->treeStruct->direction + depth) + i / 2) = DIRECTION_RIGHT;
                                    *(*(p->treeStruct->treeIndex + depth) + i / 2) = \
                                    *(*(p->treeStruct->treeIndex + depth - 1) + i + DIRECTION_RIGHT);
                                    continue;
                                }
                            }
                            break;

                            default:
                            break;
                        }
                    }

                    *(*(p->treeStruct->tree_mag + depth) + i / 2) = \
                        *(*(p->treeStruct->tree_mag + depth - 1) + i + DIRECTION_LEFT);
                    *(*(p->treeStruct->tree_hd + depth) + i / 2) = \
                        *(*(p->treeStruct->tree_hd + depth - 1) + i + DIRECTION_LEFT);
                    *(*(p->treeStruct->direction + depth) + i / 2) = DIRECTION_LEFT;
                    *(*(p->treeStruct->treeIndex + depth) + i / 2) = \
                    *(*(p->treeStruct->treeIndex + depth - 1) + i + DIRECTION_LEFT);
                }
                /* left(lower index) larger than right(higher index) */
                else
                {
                    if(zero_magnitude_sel_policy == FLAG_CASE_PASS_MAG0_HD_1_ENABLE)
                    {
                        if(**(*(p->treeStruct->tree_hd + depth - 1) + i + DIRECTION_RIGHT) == '1')
                        {
                            if(**(*(p->treeStruct->tree_mag + depth - 1) + i + DIRECTION_RIGHT) == 0)
                            {
                                *(*(p->treeStruct->tree_mag + depth) + i / 2) = \
                                    *(*(p->treeStruct->tree_mag + depth - 1) + i + DIRECTION_LEFT);
                                *(*(p->treeStruct->tree_hd + depth) + i / 2) = \
                                    *(*(p->treeStruct->tree_hd + depth - 1) + i + DIRECTION_LEFT);
                                *(*(p->treeStruct->direction + depth) + i / 2) = DIRECTION_LEFT;
                                *(*(p->treeStruct->treeIndex + depth) + i / 2) = \
                                *(*(p->treeStruct->treeIndex + depth - 1) + i + DIRECTION_LEFT);
                                continue;
                            }
                        }

                    }
                    *(*(p->treeStruct->tree_mag + depth) + i / 2) = \
                        *(*(p->treeStruct->tree_mag + depth - 1) + i + DIRECTION_RIGHT);
                    *(*(p->treeStruct->tree_hd + depth) + i / 2) = \
                        *(*(p->treeStruct->tree_hd + depth - 1) + i + DIRECTION_RIGHT);
                    *(*(p->treeStruct->direction+depth) + i / 2) = DIRECTION_RIGHT;
                    *(*(p->treeStruct->treeIndex+depth) + i / 2) = \
                        *(*(p->treeStruct->treeIndex + depth - 1) + i + DIRECTION_RIGHT);
                }
                        //printf("%d(%d)\n", **(*(p->treeStruct->tree_mag+depth)+i/2), *(*(p->treeStruct->treeIndex+depth)+i/2));
            }
            /* if can not comparing, just update to next stage */
            else
            {
                /* store redundant, no comparing, directly update to 0th stage tree. */
                *(*(p->treeStruct->tree_mag + depth) + i / 2) = \
                    *(*(p->treeStruct->tree_mag + depth - 1) + i - 1);
                *(*(p->treeStruct->tree_hd + depth) + i / 2) = \
                    *(*(p->treeStruct->tree_hd + depth - 1) + i - 1);
                *(*(p->treeStruct->direction + depth) + i / 2) = DIRECTION_LEFT;
                *(*(p->treeStruct->treeIndex + depth) + i / 2) = \
                    *(*(p->treeStruct->treeIndex + depth - 1) + i - 1);
                        //printf("EXTRA)) %d(%d) --->>> ", **(*(p->treeStruct->tree_mag+depth-1)+i-1), *(*(p->treeStruct->treeIndex+depth-1)+i-1));
                        //printf("%d(%d)\n", **(*(p->treeStruct->tree_mag+depth)+i/2), *(*(p->treeStruct->treeIndex+depth)+i/2));
            }
        }

    }
    //testPrintTreeStructValue(p->treeStruct);
    //findMinimumMagnitudeLogLikeHoodRatio_chaseAlgorithm_followMinPath(p);
    return 0;
}
//////////////////////////////////////////////////////////////////////////////////
char checkMinimumMagnitudeLocatorValidityLogLikeHoodRatio(
    struct_logLikeHoodRatio *p,
    struct_powerFormPolynomials *operandA,
    struct_powerFormPolynomials *operandB)
{
    unsigned int i;
    #ifndef RELEASE
    if(!p)    return -1;
    if(!(p->locatorLength)) return -1;
    if(!(p->locatorValidity)) return -1;

    if(!operandA) return -1;
    if(!(operandA->length)) return -1;
    if(!(operandA->usedLength)) return -1;
    if(!(operandA->equation)) return -1;

    if(!operandB) return -1;
    if(!(operandB->length)) return -1;
    if(!(operandB->usedLength)) return -1;
    if(!(operandB->equation)) return -1;
    #endif

    for(i=0; i<p->locatorLength; i++)
    {
        /* locator is always smaller than codeword Length */
        if(((*(p->locator+i)) < (operandA->usedLength)) && ((*(p->locator+i)) < (operandB->usedLength)))
        {
            if((*(operandA->equation+(*(p->locator+i)))) != (*(operandB->equation+(*(p->locator+i)))))    *(p->locatorValidity+i) = 1;
            else                                                                                        *(p->locatorValidity+i) = 0;
        }
        /* not indication any location */
        else if((*(p->locator+i)) == -1)
        {
            *(p->locatorValidity+i) = 1;
        }
        else
        {
            errorMes;
            printf("in checkMinimumMagnitudeLocatorValidityLogLikeHoodRatio, (*(p->locator+%d)='%d' exceed (operandA->usedLength)='%d' or (operandB->usedLength)='%d'\n",
                i, (*(p->locator+i)), (operandA->usedLength), (operandB->usedLength) );
            return -2;
        }
    }
    return 0;
}
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
void printQuatizLLRWithSpaceAndEnter(struct_logLikeHoodRatio *p)
{
    unsigned int i;

    if(!p)
    {
        errorMes;
        printf("in printQuatizLLRWithSpaceAndEnter, struct_logLikeHoodRatio p is NULL.\n");
        return;
    }
    if(!(p->quantizedLLR))
    {
        warningMes;
        printf("in printQuatizLLRWithSpaceAndEnter, struct_logLikeHoodRatio p->quantizedLLR is NULL.\n");
        return;
    }
    if(!(p->usedLength))
    {
        warningMes;
        printf("in printQuatizLLRWithSpaceAndEnter, struct_logLikeHoodRatio p->usedLength is zero.\n");
        return;
    }
    if(!(p->quantizedLLRMask))
    {
        warningMes;
        printf("in printQuatizLLRWithSpaceAndEnter, struct_logLikeHoodRatio p->quantizedLLRMask is zero.\n");
        return;
    }
    printf("[Quantiz LLR] : \t\t\t");
    for(i=0; i<p->usedLength; i++)
    {
        if(*(p->quantizedLLR+i)>=0)
        {
            printf("+%d ", *(p->quantizedLLR+i));
        }
        else
        {
            printf("%d ", *(p->quantizedLLR+i));
        }
    }
    printf("[Used Length : %d]\n", p->usedLength);
}
//////////////////////////////////////////////////////////////////////////////////
void printTestMagitudeQuatizLLRWithSpaceAndEnter(struct_logLikeHoodRatio *p)
{
    unsigned int i;

    if(!p)
    {
        errorMes;
        printf("in printTestMagitudeQuatizLLRWithSpaceAndEnter, struct_logLikeHoodRatio p is NULL.\n");
        return;
    }
    if(!(p->quantizedLLR))
    {
        warningMes;
        printf("in printTestMagitudeQuatizLLRWithSpaceAndEnter, struct_logLikeHoodRatio p->quantizedLLR is NULL.\n");
        return;
    }
    if(!(p->usedLength))
    {
        warningMes;
        printf("in printTestMagitudeQuatizLLRWithSpaceAndEnter, struct_logLikeHoodRatio p->usedLength is zero.\n");
        return;
    }
    if(!(p->quantizedLLRMask))
    {
        warningMes;
        printf("in printTestMagitudeQuatizLLRWithSpaceAndEnter, struct_logLikeHoodRatio p->quantizedLLRMask is zero.\n");
        return;
    }
    printf("[magnitudeShort Quantiz LLR] : \t");
    for(i=0; i<p->usedLength; i++)
    {
        if(*(p->quantizedLLR+i)>0)    printf("+%d '%c' ", *(p->quantizedLLR+i), *(p->hardDecisionShort+i));
        else                        printf("+%d '%c' ", -1*(*(p->quantizedLLR+i)), *(p->hardDecisionShort+i));
    }
    printf("[Used Length : %d]\n", p->usedLength);
}
//////////////////////////////////////////////////////////////////////////////////
void printLLRWithSpaceAndEnter(struct_logLikeHoodRatio *p)
{
    unsigned int i;

    if(!p)
    {
        errorMes;
        printf("in printLLRWithSpaceAndEnter, struct_logLikeHoodRatio p is NULL.\n");
        return;
    }
    if(!(p->llr))
    {
        warningMes;
        printf("in printLLRWithSpaceAndEnter, struct_logLikeHoodRatio p->llr is NULL.\n");
        return;
    }
    if(!(p->usedLength))
    {
        warningMes;
        printf("in printLLRWithSpaceAndEnter, struct_logLikeHoodRatio p->usedLength is zero.\n");
        return;
    }
    //if(!(p->quantizedLLRMask))
    //{
    //    warningMes;
    //    printf("in printLLRWithSpaceAndEnter, struct_logLikeHoodRatio p->quantizedLLRMask is zero.\n");
    //    return;
    //}
    printf("[LLR] : ");
    for(i=0; i<p->usedLength; i++)
    {
        printf("%.2f ", *(p->llr+i));
    }
    printf("[Used Length : %d]\n", p->usedLength);
}
//////////////////////////////////////////////////////////////////////////////////
char fprintQuatizLLR_toHEX(FILE* fp, struct_logLikeHoodRatio* p, char* str_tail)
{
    unsigned int i;
    unsigned int j;
    unsigned int bitLength;
    unsigned int hexLength;
    unsigned int hexRedundantBitLength;
    unsigned int hexRedundantBitMask;
    unsigned int uint_tmp;
    const int HEX_BIT_LENGTH = 4;

    #ifndef RELEASE
    if(!fp)
    {
        errorMes;
        printf("in fprintQuatizLLR_toHEX, FILE* fp is NULL.\n");
        return -1;
    }
    if(!p)
    {
        errorMes;
        printf("in fprintQuatizLLR_toHEX, struct_logLikeHoodRatio p is NULL.\n");
        return -1;
    }
    if(!(p->quantizedLLR))
    {
        warningMes;
        printf("in fprintQuatizLLR_toHEX, struct_logLikeHoodRatio p->quantizedLLR is NULL.\n");
        return -1;
    }
    if(!(p->usedLength))
    {
        warningMes;
        printf("in fprintQuatizLLR_toHEX, struct_logLikeHoodRatio p->usedLength is zero.\n");
        return -1;
    }
    if(!(p->quantizedLLRMask))
    {
        warningMes;
        printf("in fprintQuatizLLR_toHEX, struct_logLikeHoodRatio p->quantizedLLRMask is zero.\n");
        return -1;
    }
    #endif
    /* Check up LLR bit mask */
    bitLength = 0;
    uint_tmp = 0;
    uint_tmp |= p->quantizedLLRMask;
    for(i=0; uint_tmp&1; i++)
    {
        bitLength++;
        uint_tmp = (uint_tmp >> 1);
    }
    hexLength = (bitLength) / HEX_BIT_LENGTH;
    hexRedundantBitLength = (bitLength) % HEX_BIT_LENGTH;

    hexRedundantBitMask = 0;
    if(hexRedundantBitLength)
    {
        hexLength++;
        for(i = 0; i < hexRedundantBitLength; i++)
        {
            hexRedundantBitMask |= (1<<i);
        }
    }

    for(i=0; i<p->usedLength; i++)
    {
        for(j = hexLength; j != 0; j--)
        {
            if(j == hexLength)
            {
                if(hexRedundantBitLength)
                {
                    fprintf(fp, "%01x", (((*(p->quantizedLLR+i)) >> ((j-1)*4)) & hexRedundantBitMask));
                }
                else
                {
                    fprintf(fp, "%01x", (((*(p->quantizedLLR+i)) >> ((j-1)*4)) & 0xF));
                }
            }
            else
            {
                fprintf(fp, "%01x", (((*(p->quantizedLLR+i)) >> ((j-1)*4)) & 0xF));
            }
        }
    }

    if(str_tail) fprintf(fp, str_tail);

    return 0;
}
//////////////////////////////////////////////////////////////////////////////////
char fprintVerilogQuatizLLR_toHEX(FILE* fp, struct_logLikeHoodRatio* p, char* str_tail)
{
    unsigned int i;
    unsigned int j;
    unsigned int bitLength;
    unsigned int hexLength;
    unsigned int hexRedundantBitLength;
    unsigned int hexRedundantBitMask;
    unsigned int uint_tmp;
    const int HEX_BIT_LENGTH = 4;

    #ifndef RELEASE
    if(!fp)
    {
        errorMes;
        printf("in fprintQuatizLLR_toHEX, FILE* fp is NULL.\n");
        return -1;
    }
    if(!p)
    {
        errorMes;
        printf("in fprintQuatizLLR_toHEX, struct_logLikeHoodRatio p is NULL.\n");
        return -1;
    }
    if(!(p->quantizedLLR))
    {
        warningMes;
        printf("in fprintQuatizLLR_toHEX, struct_logLikeHoodRatio p->quantizedLLR is NULL.\n");
        return -1;
    }
    if(!(p->usedLength))
    {
        warningMes;
        printf("in fprintQuatizLLR_toHEX, struct_logLikeHoodRatio p->usedLength is zero.\n");
        return -1;
    }
    if(!(p->quantizedLLRMask))
    {
        warningMes;
        printf("in fprintQuatizLLR_toHEX, struct_logLikeHoodRatio p->quantizedLLRMask is zero.\n");
        return -1;
    }
    #endif
    /* Check up LLR bit mask */
    bitLength = 0;
    uint_tmp = 0;
    uint_tmp |= p->quantizedLLRMask;
    for(i=0; uint_tmp&1; i++)
    {
        bitLength++;
        uint_tmp = (uint_tmp >> 1);
    }
    hexLength = (bitLength) / HEX_BIT_LENGTH;
    hexRedundantBitLength = (bitLength) % HEX_BIT_LENGTH;

    hexRedundantBitMask = 0;
    if(hexRedundantBitLength)
    {
        hexLength++;
        for(i = 0; i < hexRedundantBitLength; i++)
        {
            hexRedundantBitMask |= (1<<i);
        }
    }

    for(i=0; i<p->usedLength; i++)
    {
        for(j = hexLength; j != 0; j--)
        {
            if(j == hexLength)
            {
                if(hexRedundantBitLength)
                {
                    fprintf(fp, "%01x\r\n", (((*(p->quantizedLLR+i)) >> ((j-1)*4)) & hexRedundantBitMask));
                }
                else
                {
                    fprintf(fp, "%01x\r\n", (((*(p->quantizedLLR+i)) >> ((j-1)*4)) & 0xF));
                }
            }
            else
            {
                fprintf(fp, "%01x\r\n", (((*(p->quantizedLLR+i)) >> ((j-1)*4)) & 0xF));
            }
        }
    }

    if(str_tail) fprintf(fp, str_tail);

    return 0;
}
//////////////////////////////////////////////////////////////////////////////////
char fprintQuatizLLR_fullDescriptionToHEX(FILE* fp, struct_logLikeHoodRatio* p, char* str_tail)
{
    unsigned int i;
    unsigned int j;
    unsigned int bitLength;
    unsigned int hexLength;
    unsigned int hexRedundantBitLength;
    unsigned int hexRedundantBitMask;
    unsigned int uint_tmp;
    const int HEX_BIT_LENGTH = 4;

    #ifndef RELEASE
    if(!fp)
    {
        errorMes;
        printf("in fprintQuatizLLR_fullDescriptionToHEX, FILE* fp is NULL.\n");
        return -1;
    }
    if(!p)
    {
        errorMes;
        printf("in fprintQuatizLLR_fullDescriptionToHEX, struct_logLikeHoodRatio p is NULL.\n");
        return -1;
    }
    if(!(p->quantizedLLR))
    {
        warningMes;
        printf("in fprintQuatizLLR_fullDescriptionToHEX, struct_logLikeHoodRatio p->quantizedLLR is NULL.\n");
        return -1;
    }
    if(!(p->usedLength))
    {
        warningMes;
        printf("in fprintQuatizLLR_fullDescriptionToHEX, struct_logLikeHoodRatio p->usedLength is zero.\n");
        return -1;
    }
    if(!(p->quantizedLLRMask))
    {
        warningMes;
        printf("in fprintQuatizLLR_fullDescriptionToHEX, struct_logLikeHoodRatio p->quantizedLLRMask is zero.\n");
        return -1;
    }
    #endif
    /* Check up LLR bit mask */
    bitLength = 0;
    uint_tmp = 0;
    uint_tmp |= p->quantizedLLRMask;
    for(i=0; uint_tmp&1; i++)
    {
        bitLength++;
        uint_tmp = (uint_tmp >> 1);
    }
    hexLength = (bitLength) / HEX_BIT_LENGTH;
    hexRedundantBitLength = (bitLength) % HEX_BIT_LENGTH;

    hexRedundantBitMask = 0;
    if(hexRedundantBitLength)
    {
        hexLength++;
        for(i = 0; i < hexRedundantBitLength; i++)
        {
            hexRedundantBitMask |= (1<<i);
        }
    }

    for(i=0; i<p->usedLength; i++)
    {
        if(*(p->quantizedLLR+i)>=0)
        {
            fprintf(fp, "+%d(", *(p->quantizedLLR+i));
        }
        else
        {
            fprintf(fp, "%d(", *(p->quantizedLLR+i));
        }

        for(j = hexLength; j!= 0; j--)
        {
            if(j == hexLength)
            {
                if(hexRedundantBitLength)
                {
                    fprintf(fp, "%01x", (((*(p->quantizedLLR+i)) >> ((j-1)*4)) & hexRedundantBitMask));
                }
                else
                {
                    fprintf(fp, "%01x", (((*(p->quantizedLLR+i)) >> ((j-1)*4)) & 0xF));
                }
            }
            else
            {
                fprintf(fp, "%01x", (((*(p->quantizedLLR+i)) >> ((j-1)*4)) & 0xF));
            }
        }
        fprintf(fp, ") ");

    }
    fprintf(fp, "[Used Length : %d]", p->usedLength);
    if(str_tail) fprintf(fp, str_tail);

    return 0;
}
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
void testPrintLLRWithSpaceAndEnter(struct_logLikeHoodRatio *p)
{
    unsigned int i;

    if(!p)
    {
        errorMes;
        printf("in testPrintLLRWithSpaceAndEnter, struct_logLikeHoodRatio p is NULL.\n");
        return;
    }
    if(!(p->llr))
    {
        warningMes;
        printf("in testPrintLLRWithSpaceAndEnter, struct_logLikeHoodRatio p->llr is NULL.\n");
        return;
    }
    if(!(p->usedLength))
    {
        warningMes;
        printf("in testPrintLLRWithSpaceAndEnter, struct_logLikeHoodRatio p->usedLength is zero.\n");
        return;
    }

    printf("[LLR and Qantized LLR]\n");
    for(i=0; i<p->usedLength; i++)
    {
        if(*(p->llr+i)>=0)  printf("+%g\t", *(p->llr+i));
        else                printf("%g\t", *(p->llr+i));
        if(*(p->quantizedLLR+i)>=0) printf("+%d\n", *(p->quantizedLLR+i));
        else                        printf("%d\n", *(p->quantizedLLR+i));
    }
    printf("[Used Length : %d]\n", p->usedLength);
}
//////////////////////////////////////////////////////////////////////////////////
void testPrintShortMinimumQuantizedLLRAndLocator(struct_logLikeHoodRatio *p)
{
    unsigned int i;

    if(!p)
    {
        errorMes;
        printf("in testPrintShortMinimumQuantizedLLRAndLocator, !p\n");
        return;
    }
    if(!(p->locatorLength))
    {
        errorMes;
        printf("in testPrintShortMinimumQuantizedLLRAndLocator, !(p->locatorLength)\n");
        return;
    }
    if(!(p->magnitudeShort))
    {
        errorMes;
        printf("in testPrintShortMinimumQuantizedLLRAndLocator, !(p->magnitudeShort)\n");
        return;
    }
    if(!(p->hardDecisionShort))
    {
        errorMes;
        printf("in testPrintShortMinimumQuantizedLLRAndLocator, !(p->hardDecisionShort)\n");
        return;
    }
    if(!(p->locator))
    {
        errorMes;
        printf("in testPrintShortMinimumQuantizedLLRAndLocator, !(p->locator).\n");
        return;
    }
    if(!(p->locatorValidity))
    {
        errorMes;
        printf("in testPrintShortMinimumQuantizedLLRAndLocator, !(p->locatorValidity).\n");
        return;
    }
    printf("[magnitudeShort, locator and validity]\n");
    for(i=0; i<p->locatorLength; i++)
    {
        printf("[magnitudeShort]\t%d\t[HD]\t'%c'\t[Location]\t%d\t[Validity]\t%d\n",
                *(p->magnitudeShort+i), *(p->hardDecisionShort+i), *(p->locator+i), *(p->locatorValidity+i) );
    }
}
//////////////////////////////////////////////////////////////////////////////////
struct_logLikeHoodRatio* createLogLikeHoodRatioUsingNumberOfMaskBits(unsigned int length, unsigned int numberOfMask)
{
    //unsigned int i;
    struct_logLikeHoodRatio *p=NULL;

    p=createLogLikeHoodRatio(length);
            #ifndef RELEASE
            if(!(p))
            {
                warningMes;
                printf("in createLogLikeHoodRatioUsingNumberOfMaskBits, p=createLogLikeHoodRatio(%d) return NULL.\n", length);
                return NULL;
            }
            #endif

    if(setQuantizedLLRMaskOfLogLikeHoodRatio(p, numberOfMask))
    {
        closeLogLikeHoodRatio(&p);
                #ifndef RELEASE
                errorMes; printf("in createLogLikeHoodRatioUsingNumberOfMaskBits, setQuantizedLLRMaskOfLogLikeHoodRatio(p, numberOfMask) return Non-zero.\n");
                #endif
        return NULL;
    }


    //if(!(p->llr))
    //{
    //    if(!(p->length))
    //    {
    //        p->quantizedLLRMask=0;
    //        for(i=0; i<numberOfMask; i++)
    //        {
    //            p->quantizedLLRMask|=(1<<i);
    //        }
    //        p->magnitudeMask=p->quantizedLLRMask&(~(1<<(i-1)));
    //                printf("Mask test quantizat mask %d\nMask test magnitudeShort mask %d\n",p->quantizedLLRMask, p->magnitudeMask);
    //    }
    //    else
    //    {
    //        p->quantizedLLRMask=0;
    //        warningMes;
    //        printf("in initialLlrValueUsingMask, (*p)->length is zero.\n");
    //        return -1;
    //    }
    //}
    return p;
}
//////////////////////////////////////////////////////////////////////////////////
struct_logLikeHoodRatio* createLogLikeHoodRatioUsingLlrMask(unsigned int length, SIGNED_INT quantizedLLRMask)
{
    //unsigned int i;
    struct_logLikeHoodRatio *p=NULL;

    #ifndef RELEASE
    if(!quantizedLLRMask)
    {
        warningMes;
        printf("in createLogLikeHoodRatioUsingLlrMask, quantizedLLRMask is '%d'.\n", quantizedLLRMask);
        return NULL;
    }

    if(!(p=createLogLikeHoodRatio(length)))
    {
        warningMes;
        printf("in createLogLikeHoodRatioUsingLlrMask, p=createLogLikeHoodRatio(%d) return NULL.\n", length);
        return NULL;
    }
    #endif

    //if(!(p->llr))
    //{
    //    if(!(p->length))
    //    {
            p->quantizedLLRMask=quantizedLLRMask;
    //        //for(i=0; (i<(sizeof(s_int_QUANTIZ_DIGIT)*8))&&((1<<i)&quantizedLLRMask); i++);
    //        p->magnitudeMask=(p->quantizedLLRMask&(~(1<<(i-1))));
    //                printf("Mask test quantizat mask %d\nMask test magnitudeShort mask %d\n",p->quantizedLLRMask, p->magnitudeMask);
    //    }
    //    else
    //    {
    //        p->quantizedLLRMask=0;
    //        warningMes;
    //        printf("in initialLlrValueUsingMask, (*p)->length is zero.\n");
    //        return -1;
    //    }
    //}
    return p;
}
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
char set_unSelTp_bofore_decoding
(
    unsigned int *W_selectedTP,
    unsigned int *W_selTP_errCnt,

    unsigned int *W_list_errCntOfTP_afterDec_theo,

    struct_powerFormPolynomials **W_sel_TP_CW_pwrFormPoly,

    unsigned int INFO_testPatternNum,

    struct_powerFormPolynomials **INFO_list_TP_CW_pwrFormPoly,

    unsigned int *INFO_list_errCntOfTP_beforeDec_theo,
    const char *INFO_ALGORITHM_NAME
)
{
    unsigned int tmp_i;

    #ifndef RELEASE
    if(!W_selectedTP)
    {
        errorMes; printf("in set_unSelTp_bofore_decoding, unsigned int tmp_sel_TP is NULL.\r\n");
        return -1;
    }

    if(!W_selTP_errCnt)
    {
        errorMes; printf("in set_unSelTp_bofore_decoding, unsigned int tmp_selTP_errCnt is NULL.\r\n");
        return -2;
    }

    if(!W_list_errCntOfTP_afterDec_theo)
    {
        errorMes; printf("in set_unSelTp_bofore_decoding, unsigned int *W_list_errCntOfTP_afterDec_theo is NULL.\r\n");
        return -2;
    }

    if(!W_sel_TP_CW_pwrFormPoly)
    {
        errorMes; printf("in set_unSelTp_bofore_decoding, struct_powerFormPolynomials *W_sel_TP_CW_pwrFormPoly is NULL.\r\n");
        return -3;
    }

    if(!INFO_list_TP_CW_pwrFormPoly)
    {
        errorMes; printf("in set_unSelTp_bofore_decoding, struct_powerFormPolynomials **INFO_list_TP_CW_pwrFormPoly is NULL.\r\n");
        return -6;
    }

    if(!INFO_list_TP_CW_pwrFormPoly[0])
    {
        errorMes; printf("in set_unSelTp_bofore_decoding, struct_powerFormPolynomials *(INFO_list_TP_CW_pwrFormPoly+%d) is NULL.\r\n", 0);
        return -7;
    }

    if(!INFO_list_errCntOfTP_beforeDec_theo)
    {
        errorMes; printf("in set_unSelTp_bofore_decoding, unsigned int *INFO_list_errCntOfTP_beforeDec_theoy is NULL.\r\n");
        return -6;
    }
    #endif

    for(tmp_i=0; tmp_i<INFO_testPatternNum; tmp_i++)
    {
        W_list_errCntOfTP_afterDec_theo[tmp_i]=0;
    }

    *W_selectedTP = INFO_testPatternNum;
    *W_selTP_errCnt = INFO_list_errCntOfTP_beforeDec_theo[0];
    *W_sel_TP_CW_pwrFormPoly = INFO_list_TP_CW_pwrFormPoly[0];

    return 0;
}
//////////////////////////////////////////////////////////////////////////////////
char set_selTP_before_decording_skipDecOnly
(
    unsigned int *W_HD_errCnt,

    unsigned int *W_selectedTP,
    unsigned int *W_selTP_errCnt,

    unsigned int *W_list_errCntOfTP_afterDec_theo,

    struct_powerFormPolynomials **W_sel_TP_CW_pwrFormPoly,

    unsigned int INFO_hd_correctability,

    unsigned int INFO_testPatternNum,

    unsigned int INFO_CH_errCnt,

    struct_powerFormPolynomials **INFO_list_TP_CW_pwrFormPoly,
    unsigned int *INFO_list_errCntOfTP_beforeDec_theo,
    const char *INFO_ALGORITHM_NAME
)
{
    unsigned int tmp_i;

    unsigned int tmp_sel_TP;
    unsigned int tmp_selTP_errCnt;


    #ifndef RELEASE
    if(!W_selectedTP)
    {
        errorMes; printf("in set_selTP_before_decording_skipDecOnly, unsigned int tmp_sel_TP is NULL.\r\n");
        return -1;
    }

    if(!W_selTP_errCnt)
    {
        errorMes; printf("in set_selTP_before_decording_skipDecOnly, unsigned int tmp_selTP_errCnt is NULL.\r\n");
        return -2;
    }

    if(!W_list_errCntOfTP_afterDec_theo)
    {
        errorMes; printf("in set_selTP_before_decording_skipDecOnly, unsigned int *W_list_errCntOfTP_afterDec_theo is NULL.\r\n");
        return -2;
    }

    if(!W_sel_TP_CW_pwrFormPoly)
    {
        errorMes; printf("in set_selTP_before_decording_skipDecOnly, struct_powerFormPolynomials *W_sel_TP_CW_pwrFormPoly is NULL.\r\n");
        return -3;
    }

    if(!INFO_hd_correctability)
    {
        errorMes; printf("in set_selTP_before_decording_skipDecOnly, unsigned int INFO_hd_correctability is zero.\r\n");
        return -4;
    }

    if(!INFO_testPatternNum)
    {
        errorMes; printf("in set_selTP_before_decording_skipDecOnly, unsigned int INFO_testPatternNum is zero.\r\n");
        return -5;
    }
    if(!INFO_list_TP_CW_pwrFormPoly)
    {
        errorMes; printf("in set_selTP_before_decording_skipDecOnly, struct_powerFormPolynomials **INFO_list_TP_CW_pwrFormPoly is NULL.\r\n");
        return -6;
    }

    for(tmp_i=0; tmp_i<INFO_testPatternNum; tmp_i++)
    {
        if(!INFO_list_TP_CW_pwrFormPoly[tmp_i])
        {
            errorMes; printf("in set_selTP_before_decording_skipDecOnly, struct_powerFormPolynomials *(INFO_list_TP_CW_pwrFormPoly+%d) is NULL.\r\n", tmp_i);
            return -7;
        }
    }

    if(!INFO_list_errCntOfTP_beforeDec_theo)
    {
        errorMes; printf("in set_selTP_before_decording_skipDecOnly, unsigned int *INFO_list_errCntOfTP_beforeDec_theo is NULL.\r\n");
        return -8;
    }
    #endif

    tmp_selTP_errCnt = INFO_CH_errCnt;

    switch(global_flag_case_selTP_before_dec)
    {
        /********************************************************************/
        /************ get BER mode is theoretical mode, that is *************/
        /*************** BER is theoretically best case START ***************/
        /********************************************************************/
        /*
         * note. Theoretical error count.
         * Theoretic is mean, simulator known that channel enviroments,
         * that is mean simulator recognize error number corrrectly and already.
         * So in below code and theoretical BER mode,
         * Simulator identify best test-pattern and,
         * choose best test pattern has minimum error number.
         */

        case FLAG_CASE_SEL_TP_BEFORE_DEC_THEO:
        {
        /* initializing... */
        //(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))

            tmp_sel_TP = INFO_testPatternNum;//debug mode, to accur core dump
            /********** selecting which error count number of test patterns **********/
            /*
             * note. belong to which chosen, BER result is effected.
             */
            switch(global_flag_case_choosingCodeWordMethod_inTheo)
            {
                default:
                    warningMes; printf("Method of gettig BER is not selected, So selecting method is pixed to normal(default).\r\n");

                /*
                 * note. if no errors in all test patterns, select this.
                 * if not, select hard-decision error number.
                 * !!! WARNING !!!
                 * not support FLAG_CASE_CODEWORD_CHOOSEN_METHOD_IN_THEO_NO_ERR.
                 */
                case FLAG_CASE_CODEWORD_CHOOSEN_METHOD_IN_THEO_NO_ERR:
                    //printf("[Z]");
                    for(tmp_i=0; tmp_i<INFO_testPatternNum; tmp_i++)
                    {
                        if(!INFO_list_errCntOfTP_beforeDec_theo[tmp_i])//when INFO_list_errCntOfTP_beforeDec_theo[tmp_i] is zero.
                        {
                            W_list_errCntOfTP_afterDec_theo[tmp_i] = 0;
                            tmp_selTP_errCnt = 0;
                            tmp_sel_TP = tmp_i;
                        }
                        else if(INFO_list_errCntOfTP_beforeDec_theo[tmp_i] <= INFO_hd_correctability)
                        {
                            W_list_errCntOfTP_afterDec_theo[tmp_i] = 0;
                        }
                        else
                        {
                            W_list_errCntOfTP_afterDec_theo[tmp_i] = INFO_list_errCntOfTP_beforeDec_theo[tmp_i];
                        }
                    }
                    /*
                     * note. if no error bit count, error count to fix hard-decision.
                     */
                    if(tmp_sel_TP == INFO_testPatternNum)
                    {
                        tmp_selTP_errCnt = INFO_list_errCntOfTP_beforeDec_theo[0];
                        tmp_sel_TP = 0;
                    }
                break;

                /*
                 * note. selecting error bits belong to correctable test pattern's error number.
                 * if can't correct, select hard-decision error number.
                 * in normal(this) case, designer have to think about how to select correctable codeword.
                 */
                case FLAG_CASE_CODEWORD_CHOOSEN_METHOD_IN_THEO_NORMAL:
                {
                    for(tmp_i=0; tmp_i<INFO_testPatternNum; tmp_i++)
                    {
                        if(INFO_list_errCntOfTP_beforeDec_theo[tmp_i] <= INFO_hd_correctability)
                        {
                            W_list_errCntOfTP_afterDec_theo[tmp_i] = 0;
                            tmp_selTP_errCnt = 0;
                            tmp_sel_TP = tmp_i;
                        }
                        else
                        {
                            W_list_errCntOfTP_afterDec_theo[tmp_i] = INFO_list_errCntOfTP_beforeDec_theo[tmp_i];
                        }
                    }

                    if(tmp_sel_TP == INFO_testPatternNum)
                    {
                        tmp_selTP_errCnt = INFO_list_errCntOfTP_beforeDec_theo[0];
                        tmp_sel_TP = 0;
                    }
                }
                break;

                /*
                 * note. selecting minimum error bits comparing each error bits number, patterns have.
                 * In best(this) case, not guarantee decording.
                 */
                case FLAG_CASE_CODEWORD_CHOOSEN_METHOD_IN_THEO_MIN_ERR:
                    //printf("[B]");
                    for(tmp_i=0; tmp_i<INFO_testPatternNum; tmp_i++)
                    {
                        if(INFO_list_errCntOfTP_beforeDec_theo[tmp_i] <= INFO_hd_correctability)
                        {
                            W_list_errCntOfTP_afterDec_theo[tmp_i] = 0;
                        }
                        else
                        {
                            W_list_errCntOfTP_afterDec_theo[tmp_i] = INFO_list_errCntOfTP_beforeDec_theo[tmp_i];
                        }

                        if(tmp_selTP_errCnt > INFO_list_errCntOfTP_beforeDec_theo[tmp_i])
                        {
                            tmp_selTP_errCnt = W_list_errCntOfTP_afterDec_theo[tmp_i];
                            tmp_sel_TP = tmp_i;
                        }
                    }

                    if(tmp_sel_TP == INFO_testPatternNum)
                    {
                        tmp_selTP_errCnt = INFO_list_errCntOfTP_beforeDec_theo[0];
                        tmp_sel_TP = 0;
                    }
                break;

                /*
                 * note. selecting maximum error bits comparing each error bits number, patterns have.
                 * In worst(this) case, not guarantee decording.
                 */
                case FLAG_CASE_CODEWORD_CHOOSEN_METHOD_IN_THEO_MAX_ERR:
                    //for(main_tmp_sel_decAlgo_i=0; main_tmp_sel_decAlgo_i<processingUseThisAlgorithm->length; main_tmp_sel_decAlgo_i++)
                    //{
                    //    tmp_sel_TP = 0;//debug mode, to accur core dump
                    //}
                    //printf("[W]");
                    /* note. to fine maximum error count, tmp_selTP_errCnt is clrear to zero. */
                    for(tmp_i=0; tmp_i<INFO_testPatternNum; tmp_i++)
                    {
                        if(INFO_list_errCntOfTP_beforeDec_theo[tmp_i] <= INFO_hd_correctability)
                        {
                            W_list_errCntOfTP_afterDec_theo[tmp_i] = 0;
                        }
                        else
                        {
                            W_list_errCntOfTP_afterDec_theo[tmp_i] = INFO_list_errCntOfTP_beforeDec_theo[tmp_i];
                        }

                        if(tmp_selTP_errCnt < INFO_list_errCntOfTP_beforeDec_theo[tmp_i])
                        {
                            tmp_selTP_errCnt = W_list_errCntOfTP_afterDec_theo[tmp_i];
                            tmp_sel_TP = tmp_i;
                        }
                    }

                    if(tmp_sel_TP == INFO_testPatternNum)
                    {
                        tmp_selTP_errCnt = INFO_list_errCntOfTP_beforeDec_theo[0];
                        tmp_sel_TP = 0;
                    }
                break;
            }
            *W_selectedTP = tmp_sel_TP;
            *W_selTP_errCnt = tmp_selTP_errCnt;
            /********** selecting target codeword, main_indi_sel_decoding_CW_pwrFormPoly[KIND_OF_BCH_DECODING_BM] **********/
            /*
             * note. main_indi_sel_decoding_CW_pwrFormPoly[KIND_OF_BCH_DECODING_BM] have a memory address of decorded codewoard, which type is struct_powerFormPolynomials*.
             * In soft-dedision case, it has same function.
             */
            *W_sel_TP_CW_pwrFormPoly = INFO_list_TP_CW_pwrFormPoly[tmp_sel_TP];

                    #ifndef RELEASE
                    /********** display parts **********/
                    if(
                        (global_flag_case_getBER_method_Display&FLAG_MASK_DISPLAY_PROGRESS)||
                        (global_flag_debug_display&FLAG_MASK_DISPLAY_PROGRESS)||
                        (global_flag_bch_mSBS_SimSD_display&FLAG_MASK_DISPLAY_PROGRESS)||
                        (global_flag_bch_BM_SimSD_display&FLAG_MASK_DISPLAY_PROGRESS)||
                        (global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)
                    )
                    {
                        infoMes;
                        printf("Selecting test pattern method is ");
                        switch(global_flag_case_choosingCodeWordMethod_inTheo)
                        {
                            default:
                                infoMes; printf("not selecting->apply basic(default).\n");
                            break;

                            /* not-support */
                            case FLAG_CASE_CODEWORD_CHOOSEN_METHOD_IN_THEO_NO_ERR:
                                infoMes; printf("basic.\n");
                            break;

                            case FLAG_CASE_CODEWORD_CHOOSEN_METHOD_IN_THEO_NORMAL:
                                infoMes; printf("normal.\n");
                            break;

                            case FLAG_CASE_CODEWORD_CHOOSEN_METHOD_IN_THEO_MIN_ERR:
                                infoMes; printf("best.\n");
                            break;

                            case FLAG_CASE_CODEWORD_CHOOSEN_METHOD_IN_THEO_MAX_ERR:
                                infoMes; printf("worst.\n");
                            break;
                        }

                        infoMes;
                        printf("INFO_list_errCntOfTP_beforeDec_theo : ");
                        for(tmp_i=0; tmp_i<INFO_testPatternNum; tmp_i++)
                        {
                            printf("(#%d)%d\t", tmp_i, INFO_list_errCntOfTP_beforeDec_theo[tmp_i]);
                        }
                        printf("\r\n");


                        infoMes;
                        printf("[%s] Sel TP[%d], TP[%d] err cnt = %d, Sel TP err cnt %d\r\n",
                            INFO_ALGORITHM_NAME,
                            tmp_sel_TP,
                            *W_selectedTP,
                            INFO_list_errCntOfTP_beforeDec_theo[tmp_sel_TP],
                            *W_selTP_errCnt
                        );
                    }
                    #endif
        }
        break;
        /********************************************************************/
        /************ get BER mode is theoretical mode, that is *************/
        /**************** BER is theoretically best case end ****************/
        /********************************************************************/


        /********************************************************************/
        /************** get BER mode is actual mode, that is  ***************/
        /************** BER can be different with theoretical ***************/
        /************** case start                            ***************/
        /********************************************************************/
        /*
         * note. Actual error count.
         * Actual is mean, simulator does not known that channel enviroments,
         * that is mean simulator can not recognize error number corrrectly.
         * So in below code and actual BER mode,
         * simulator can not choose best test pattern has minimum error number.
         */
        case FLAG_CASE_SEL_TP_BEFORE_DEC_ACT:
        {
            //DO NOTHING
            //DO NOTHING
            //DO NOTHING
        }
        break;
        /********************************************************************/
        /************** get BER mode is actual mode, that is  ***************/
        /************** BER can be different with theoretical ***************/
        /************** case end                              ***************/
        /********************************************************************/



        default:
            errorMes;
            printf("Get BER mode is not selected so, can't procedure program.\r\n");
            printf("Program is closed forcibly.\r\n");
            return -1;
            break;
    }

    /********************************************************************/
    /************** Error counting, to calculate BER and ****************/
    /************** selecting Codeword to decoding end   ****************/
    /************** This sequence only valid in Theoret- ****************/
    /************** ically Fail conclusion Method.       ****************/
    /********************************************************************/
    return 0;
}
//////////////////////////////////////////////////////////////////////////////////
char set_flag_theo_suc_list_TP(unsigned char *correctableFlag, unsigned int TP_length, unsigned int *codeWordErrCnt, unsigned int hardDecisionCorrectability)
{
    #ifndef RELEASE
    if(!correctableFlag)
    {
        errorMes; printf("in set_flag_theo_suc_list_TP, unsigned char *correctableFlag is null.\r\n");
        return -1;
    }

    if(!TP_length)
    {
        errorMes; printf("in set_flag_theo_suc_list_TP, unsigned int TP_length is zero.\r\n");
        return -1;
    }
    #endif
    unsigned int tmp_i;

    for(tmp_i=0; tmp_i<TP_length; tmp_i++)
    {
        if( (codeWordErrCnt[tmp_i] <= hardDecisionCorrectability) )
        {
            correctableFlag[tmp_i] = 1;

        }
        else
        {
            correctableFlag[tmp_i] = 0;
        }
    }
    return 0;
}
//////////////////////////////////////////////////////////////////////////////////
char set_flag_act_fail_list_TP
(
    unsigned int *W_setlectedTP,//write
    unsigned char *W_list_flag_decodingFail,
    unsigned char *W_flag_allDecFail,

    unsigned char *R_list_flag_beforeDec_synd_zero,
    unsigned char *R_list_flag_degErrPolyNotMax,
    unsigned char *R_list_flag_metricChkAndDegErrPolySame,

    unsigned int INFO_TP_Nums,
    const char *INFO_ALGORITHM_NAME
)
{
    unsigned int tmp_i;


    #ifndef RELEASE
    if(!W_setlectedTP)
    {
        errorMes; printf("in setFlagDecodingFailm, unsigned int *W_setlectedTP, is null\r\n");
        return -1;
    }

    if(!W_list_flag_decodingFail)
    {
        errorMes; printf("in setFlagDecodingFailm, unsigned char *W_list_flag_decodingFail is null\r\n");
        return -1;
    }


    if(!R_list_flag_beforeDec_synd_zero)
    {
        errorMes; printf("in setFlagDecodingFailm, unsigned char *R_list_flag_beforeDec_synd_zero is null\r\n");
        return -1;
    }
    if(!R_list_flag_degErrPolyNotMax)
    {
        errorMes; printf("in setFlagDecodingFailm, unsigned char *R_list_flag_degErrPolyNotMax is null\r\n");
        return -1;
    }
    if(!R_list_flag_metricChkAndDegErrPolySame)
    {
        errorMes; printf("in setFlagDecodingFailm, unsigned char *R_list_flag_metricChkAndDegErrPolySame is null\r\n");
        return -1;
    }

    if(!INFO_TP_Nums)
    {
        errorMes; printf("in setFlagDecodingFailm, unsigned char INFO_TP_Nums is zero");
        return -1;
    }
    #endif


    (*W_setlectedTP) = INFO_TP_Nums;
    for(tmp_i = 0; tmp_i < INFO_TP_Nums; tmp_i++)
    {
        if(R_list_flag_beforeDec_synd_zero[tmp_i])
        {
            if(*W_setlectedTP == INFO_TP_Nums) *W_setlectedTP = tmp_i;
            W_list_flag_decodingFail[tmp_i] = 0;


            #ifndef RELEASE
            if(
                (global_flag_case_getBER_method_Display&FLAG_MASK_DISPLAY_PROGRESS)||
                (global_flag_debug_display&FLAG_MASK_DISPLAY_PROGRESS)||
                (global_flag_bch_BM_SimSD_display&FLAG_MASK_DISPLAY_PROGRESS)||
                (global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)||
                (global_flag_debug_display&FLAG_MASK_DISPLAY_FUNCNAME)
            )
            {
                infoMes; printf("[ACT] [%s] TP[%d] received syndrome is all zero. Select TP is #%d\r\n",
                    INFO_ALGORITHM_NAME,
                    tmp_i,
                    *W_setlectedTP
                );
            }
            #endif
        }
        else
        {
            W_list_flag_decodingFail[tmp_i] = 1;
        }
    }

    for(tmp_i = 0; tmp_i < INFO_TP_Nums; tmp_i++)
    {
        if(
        (R_list_flag_degErrPolyNotMax[tmp_i]&&R_list_flag_metricChkAndDegErrPolySame[tmp_i])&&(!R_list_flag_beforeDec_synd_zero[tmp_i])
        )
        {
            if(*W_setlectedTP == INFO_TP_Nums)
            {
                *W_setlectedTP = tmp_i;
            }
            W_list_flag_decodingFail[tmp_i] = 0;

            #ifndef RELEASE
            if(
                (global_flag_case_getBER_method_Display&FLAG_MASK_DISPLAY_PROGRESS)||
                (global_flag_debug_display&FLAG_MASK_DISPLAY_PROGRESS)||
                (global_flag_bch_BM_SimSD_display&FLAG_MASK_DISPLAY_PROGRESS)||
                (global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)||
                (global_flag_debug_display&FLAG_MASK_DISPLAY_FUNCNAME)
            )
            {
                infoMes; printf("[ACT] [%s] TP[%d] Err Loc poly is less than maximum degree and it is same with metric chk. Select TP is #%d\r\n",
                    INFO_ALGORITHM_NAME,
                    tmp_i,
                    *W_setlectedTP
                );
            }
            #endif
        }
        else if(R_list_flag_metricChkAndDegErrPolySame[tmp_i]&&(!R_list_flag_beforeDec_synd_zero[tmp_i]))
        {
            if(*W_setlectedTP == INFO_TP_Nums)
            {
                *W_setlectedTP = tmp_i;
            }
            W_list_flag_decodingFail[tmp_i] = 0;

            #ifndef RELEASE
            if(
                (global_flag_case_getBER_method_Display&FLAG_MASK_DISPLAY_PROGRESS)||
                (global_flag_debug_display&FLAG_MASK_DISPLAY_PROGRESS)||
                (global_flag_bch_BM_SimSD_display&FLAG_MASK_DISPLAY_PROGRESS)||
                (global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)||
                (global_flag_debug_display&FLAG_MASK_DISPLAY_FUNCNAME)
            )
            {
                infoMes; printf("[ACT] [%s] TP[%d] Decoding Fail. Select TP is #%d\r\n",
                    INFO_ALGORITHM_NAME,
                    tmp_i,
                    *W_setlectedTP
                );

            }
            #endif
        }
        else
        {
            W_list_flag_decodingFail[tmp_i] = 1;

            #ifndef RELEASE
            if(
                (global_flag_case_getBER_method_Display&FLAG_MASK_DISPLAY_PROGRESS)||
                (global_flag_debug_display&FLAG_MASK_DISPLAY_PROGRESS)||
                (global_flag_bch_BM_SimSD_display&FLAG_MASK_DISPLAY_PROGRESS)||
                (global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)||
                (global_flag_debug_display&FLAG_MASK_DISPLAY_FUNCNAME)
            )
            {
                infoMes; printf("[ACT] [%s] TP[%d] Decoding Fail. Select TP is #%d\r\n",
                    INFO_ALGORITHM_NAME,
                    tmp_i,
                    *W_setlectedTP
                );

            }
            #endif
        }
    }


    //for(tmp_i = 0; tmp_i < INFO_TP_Nums; tmp_i++)
    //{
    //    printf("test)) W_list_flag_decodingFail[%d] = %s, R_list_flag_beforeDec_synd_zero[%d] = %s, selected TP = '%d', ADDR : 0x%x\r\n",
    //        tmp_i,
    //        W_list_flag_decodingFail[tmp_i]?"[ON]":"[OFF]",
    //        tmp_i,
    //        R_list_flag_beforeDec_synd_zero[tmp_i]?"[ON]":"[OFF]",
    //        *W_setlectedTP,
    //        W_setlectedTP
    //    );
    //}

    for(tmp_i = 0; tmp_i < INFO_TP_Nums; tmp_i++)
    {
        // printf("W_list_flag_decodingFail[%d] = %s\r\n", tmp_i, W_list_flag_decodingFail[tmp_i]?"[ON]":"[OFF]");
        if(!(W_list_flag_decodingFail[tmp_i])) break;
    }

    if(tmp_i == INFO_TP_Nums)
    {
        *W_setlectedTP = 0;
        *W_flag_allDecFail = 1;

        #ifndef RELEASE
        if(
            (global_flag_case_getBER_method_Display&FLAG_MASK_DISPLAY_PROGRESS)||
            (global_flag_debug_display&FLAG_MASK_DISPLAY_PROGRESS)||
            (global_flag_bch_BM_SimSD_display&FLAG_MASK_DISPLAY_PROGRESS)||
            (global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)||
            (global_flag_debug_display&FLAG_MASK_DISPLAY_FUNCNAME)
        )
        {
            infoMes; printf("[ACT] [%s] TP[%d] All Test pattern is Decoding Fail. Select TP is #%d\r\n",
                INFO_ALGORITHM_NAME,
                tmp_i,
                *W_setlectedTP
            );
        }
        #endif
    }
    else
    {
        *W_flag_allDecFail = 0;
    }
    return 0;
}
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
    /* Example codes in main functions
    //while((loadStream.countedDataNumber<loadStream.totalDataNumber))
    //{
    //    printf("[%0000006d] : ", loadStream.countedCodeWordNumber);
    //    loadToCodeWordStartAtHighSide(main_encodingComponentInGF->codeWord, &loadStream);
    //    unloadFromCodeWordStartAtHighSide(&unloadStream, main_encodingComponentInGF->codeWord);
    //    printPowerFormWithEnterPolynomialWithTapUsingAddress(main_encodingComponentInGF->codeWord);
    //    printf("[S:%d, U:%d, M:%d]\n",strlen(main_encodingComponentInGF->codeWord->equation), main_encodingComponentInGF->codeWord->usedLength, main_encodingComponentInGF->codeWord->length);
    //}
    //printBmpFileToHexCode2();
    //saveBmpFile2(0);
    */
char passingThroughAwgnData
    (
        double filePass_EbN0,
        unsigned int filePass_parityBitsLength,
        char *filePass_generationPolynomial,
        unsigned int filePass_selectedNumberOfSetOfConjugacy,
        unsigned int filePass_codeLength,
        struct_galoisField_info *filePass_galoisField,
        struct_variableSetConvertBitStreamToPowerForm *loadStream,
        struct_variableSetConvertBitStreamToPowerForm *unloadStream,
        struct_variableSetConvertBitStreamToPowerForm *noisedStream
    )
{
    struct_encodingComponentInGF *filePass_encodingComponentInGaloisField=NULL;
    struct_errorComponent *filePass_errorComponents=NULL;
    struct_bpskComponents *filePass_bpskComponents=NULL;
    struct_HD_BM_algorithmComponent *filePass_decordingComponents=NULL;

    unsigned int errorCount=0;

    unsigned int i;

    printf(">> data passing AWGN channel\n");
    infoMes;
    printf("EbN0 is '%f'.\r\n", filePass_EbN0);
    infoMes;
    printf("Parity length is '%d'.\r\n", filePass_parityBitsLength);
    infoMes;
    printf("Generator polynomial is '%s'.\r\n", filePass_generationPolynomial);
    infoMes;
    printf("Set of conjugacy is '%d'.\r\n", filePass_selectedNumberOfSetOfConjugacy);
    infoMes;
    printf("Code length is '%d'.\r\n", filePass_codeLength);
    infoMes;
    printf("Field length is '%d'.\r\n", filePass_galoisField->nonSorted->length);

    if(filePass_EbN0 < 4.0)
    {
        warningMes;
        printf("EbN0 is low.\n");
    }

    filePass_encodingComponentInGaloisField = recreateEncodingComponent(&filePass_encodingComponentInGaloisField, filePass_codeLength, filePass_selectedNumberOfSetOfConjugacy, (char_POWER_FORM*)filePass_generationPolynomial);
    filePass_bpskComponents=createBpskComponents(filePass_codeLength);
    filePass_errorComponents=recreateErrorComponent(&filePass_errorComponents, filePass_codeLength);


    while((loadStream->countedDataNumber<loadStream->totalDataNumber))
    {
        loadToCodeWordStartAtHighSide(filePass_encodingComponentInGaloisField->codeWord, loadStream);
        calculateParityInGaloisFieldAttachLowSide(filePass_encodingComponentInGaloisField->codeWord, filePass_parityBitsLength, filePass_encodingComponentInGaloisField->generationPolynomial);

        do
        {
            filePass_errorComponents=recreateErrorComponent(&filePass_errorComponents, filePass_codeLength);
            BPSK_Mod(filePass_bpskComponents->bpskTransData, filePass_encodingComponentInGaloisField->codeWord->equation, filePass_encodingComponentInGaloisField->codeWord->length);

            ADD_AWGN_CAL_LLR(filePass_bpskComponents->bpskTransData, filePass_bpskComponents->bpskReceivedDataAddedAwgn, filePass_bpskComponents->bpskReceivedLLR->llr, 1.0, filePass_codeLength, filePass_EbN0); //need to add bitrate

            BPSK_Demod(filePass_errorComponents->erroredCodeWord->equation, filePass_bpskComponents->bpskReceivedDataAddedAwgn, filePass_codeLength);

            errorCount=0;
            for(i=0; i<filePass_codeLength; i++)        //채널 통과후
            {
                if( *(filePass_errorComponents->erroredCodeWord->equation+i) != *(filePass_encodingComponentInGaloisField->codeWord->equation+i) )
                {
                    errorCount+=1;
                }
            }
        }
        while(errorCount>filePass_selectedNumberOfSetOfConjugacy);


        filePass_decordingComponents=createBmAlgorithmComponent(filePass_galoisField->nonSorted, filePass_selectedNumberOfSetOfConjugacy, (char*)filePass_errorComponents->erroredCodeWord->equation, filePass_errorComponents->erroredCodeWord->usedLength);
        calculateBmAlgorithm(filePass_galoisField->nonSorted, filePass_decordingComponents);
        chienSearch_static(filePass_galoisField->nonSorted, filePass_decordingComponents->alpha, filePass_decordingComponents->errorLocationVector, &(filePass_decordingComponents->metricCheck) );
        errorCorrection(filePass_decordingComponents->codeWord, filePass_decordingComponents->errorLocationVector);
        unloadFromCodeWordStartAtHighSide(unloadStream, filePass_decordingComponents->codeWord);
        unloadFromCodeWordStartAtHighSide(noisedStream, filePass_errorComponents->erroredCodeWord);

        closeBmAlgorithmComponent(&filePass_decordingComponents);
    }

    closeEncodingComponent(&filePass_encodingComponentInGaloisField);
    closeBpskComponents(&filePass_bpskComponents);
    closeBmAlgorithmComponent(&filePass_decordingComponents);
    closeErrorComponent(&filePass_errorComponents);

    return 0;
}
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
const char cmdOtionKeyWords[] = "-{},:=";

enum cmdLineOptionInstructionType
{
    OPT_NOTHING,
    OPT_FLAG,
    OPT_FLAG_CASE,


    OPT_DEBUG,

    OPT_SET_VALUE,

    OPT_SET_VALUES,
    OPT_SET_VALUES_SERIES,//example) {-settingCategory=[value0],[value1],[value2],[value3]}

    OPT_SET_LANGE,//set lange, example) {-settingCategory=[start]:[step]:[end]}
    OPT_SET_LANGE_SERIES,

    OPT_STRING,
    OPT_COMPONENT,

    OPT_MANUAL,

    STRUCT_END
};

enum enum_list_valueType
{
    VALUE_TYPE_NONE,

    VALUE_TYPE_VOID,

    VALUE_TYPE_CHAR,
    VALUE_TYPE_UNSIGNED_CHAR,

    VALUE_TYPE_INT,
    VALUE_TYPE_UNSIGNED_INT,

    VALUE_TYPE_LONG,
    VALUE_TYPE_UNSIGNED_LONG,

    VALUE_TYPE_FLOAT,
    VALUE_TYPE_FLOAT_RATIO,
    VALUE_TYPE_FLOAT_RATIO_LIMIT,

    VALUE_TYPE_DOUBLE,
    VALUE_TYPE_DOUBLE_RATIO,
    VALUE_TYPE_DOUBLE_RATIO_LIMIT,

    VALUE_TYPE_STRING,
}typedef enum_list_valueType;

struct struct_cmdLineOption
{
    const char *longName;
    char shortName;

    enum cmdLineOptionInstructionType instType;
    unsigned char argument;
    unsigned char argumentNumber;

    void *value;
    unsigned int valueMask;

    enum enum_list_valueType valueType;

    struct struct_cmdLineOption *cmdAddedOption;
};

struct struct_scheduling_FIFO
{
    enum_list_valueType valueType;
    void *list;
    uint32_t length;
}typedef struct_scheduling_FIFO;




////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* BCH Command Tree start */
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/************************************ bch_algorithm 4 t h - S t a r t ************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/***** static struct struct_cmdLineOption    bch_BM_algorithm_HD_option[] start *****/
    #ifndef RELEASE
    static struct struct_cmdLineOption    bch_BM_algorithm_HD_display_option[] =
    {
        {"result",  0, OPT_FLAG, 0, 0, &global_flag_bch_BM_SimHD_display, FLAG_MASK_DISPLAY_RESULT,  VALUE_TYPE_UNSIGNED_INT, NULL},
        {"progress", 0, OPT_FLAG, 0, 0, &global_flag_bch_BM_SimHD_display, FLAG_MASK_DISPLAY_PROGRESS, VALUE_TYPE_UNSIGNED_INT, NULL},
        {"funcname", 0, OPT_FLAG, 0, 0, &global_flag_bch_BM_SimHD_display, FLAG_MASK_DISPLAY_FUNCNAME, VALUE_TYPE_UNSIGNED_INT, NULL},
        {NULL,0,STRUCT_END,0,0,NULL,0,VALUE_TYPE_NONE,NULL}
    };
    #endif

    #ifndef RELEASE
    static struct struct_cmdLineOption    bch_BM_algorithm_SD_display_option[] =
    {
        {"result",  0, OPT_FLAG, 0, 0, &global_flag_bch_BM_SimSD_display, FLAG_MASK_DISPLAY_RESULT, VALUE_TYPE_UNSIGNED_INT, NULL},
        {"progress", 0, OPT_FLAG, 0, 0, &global_flag_bch_BM_SimSD_display, FLAG_MASK_DISPLAY_PROGRESS, VALUE_TYPE_UNSIGNED_INT, NULL},
        {"funcname", 0, OPT_FLAG, 0, 0, &global_flag_bch_BM_SimSD_display, FLAG_MASK_DISPLAY_FUNCNAME, VALUE_TYPE_UNSIGNED_INT, NULL},
        {NULL,0,STRUCT_END,0,0,NULL,0,VALUE_TYPE_NONE,NULL}
    };
    #endif
/***** static struct struct_cmdLineOption    bch_BM_algorithm_HD_option[] end *****/
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/************************************ bch_algorithm 4 t h - E n d ************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/************************************ bch_algorithm 3 r d - S t a r t ************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/***** static struct struct_cmdLineOption bch_BM_algorithm_option[] start *****/
static struct struct_cmdLineOption    bch_BM_algorithm_test_option[] =
{
    {"used", 0, OPT_FLAG, 0, 0, &global_list_flag_bch_sim_decodingMode[FLAG_MASK_BCH_DECORDING_ALGORITHM_BM], FLAG_MASK_CODE_BCH_DEC_AL_TEST_THIS_ALGORITHM, VALUE_TYPE_UNSIGNED_INT, NULL},//if(global_flag_bchBM_simulationMode&FLAG_MASK_CODE_BCH_DEC_AL_TEST_THIS_ALGORITHM)

    {NULL,0,STRUCT_END,0,0,NULL,0,VALUE_TYPE_NONE,NULL}
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static struct struct_cmdLineOption    bch_BM_algorithm_decoding_option[] =
{
    {"skip", 0, OPT_FLAG, 0, 0, &global_list_flag_bch_sim_decodingMode[KIND_OF_BCH_DECODING_BM], FLAG_MASK_CODE_BCH_DEC_AL_SKIP, VALUE_TYPE_UNSIGNED_INT, NULL},

    {NULL,0,STRUCT_END,0,0,NULL,0,VALUE_TYPE_NONE,NULL}
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static struct struct_cmdLineOption    bch_BM_algorithm_debug_calculate_option[] =
{
    {"delta", 0, OPT_FLAG, 0, 0, &global_flag_debug_bmSim, FLAG_MASK_DEBUG_BCH_BM_CAL_DELTA, VALUE_TYPE_UNSIGNED_INT, NULL},

    {NULL,0,STRUCT_END,0,0,NULL,0,VALUE_TYPE_NONE,NULL}
};

    #ifndef RELEASE
    static struct struct_cmdLineOption    bch_BM_algorithm_debug_option[] =
    {
        {"sequence", 0, OPT_FLAG, 0, 0, &global_flag_debug_bmSim, FLAG_MASK_DEBUG_BCH_BM_ALGORITHM, VALUE_TYPE_UNSIGNED_INT, NULL},
        {"cal", 0, OPT_COMPONENT, 0, 0, NULL, 0, VALUE_TYPE_NONE, bch_BM_algorithm_debug_calculate_option},

        {NULL,0,STRUCT_END,0,0,NULL,0,VALUE_TYPE_NONE,NULL}
    };
    #endif
/***** static struct struct_cmdLineOption bch_BM_algorithm_option[] end *****/
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/************************************ bch_algorithm 3 r d - E n d ************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/************************************ bch_algorithm 2 n d - S t a r t ************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/***** static struct struct_cmdLineOption bch_algorithm[] start *****/
static struct struct_cmdLineOption bch_BM_algorithm_option[] =
{
    {"used", 0, OPT_FLAG, 0, 0, &global_flag_bchDecAlgo_Enable, FLAG_MASK_BCH_DECORDING_ALGORITHM_BM, VALUE_TYPE_UNSIGNED_INT, NULL},

    {"algorithm", 0, OPT_COMPONENT, 0, 0, &global_list_flag_bch_sim_decodingMode[KIND_OF_BCH_DECODING_BM], FLAG_MASK_CODE_BCH_DEC_AL_TEST_THIS_ALGORITHM, VALUE_TYPE_UNSIGNED_INT, bch_BM_algorithm_test_option},
        #ifndef RELEASE
        {"display_HD", 0, OPT_COMPONENT, 0, 0, NULL, 0, VALUE_TYPE_NONE, bch_BM_algorithm_HD_display_option},
        {"display_SD", 0, OPT_COMPONENT, 0, 0, NULL, 0, VALUE_TYPE_NONE, bch_BM_algorithm_SD_display_option},
        #endif

    {"hard_decision", 0, OPT_FLAG, 0, 0, &global_list_flag_bch_sim_decodingMode[KIND_OF_BCH_DECODING_BM], FLAG_MASK_CODE_BCH_DEC_AL_HARD_DECISION, VALUE_TYPE_UNSIGNED_INT, NULL},//if(global_list_flag_bch_sim_decodingMode[KIND_OF_BCH_DECODING_BM]&FLAG_MASK_CODE_BCH_DEC_AL_HARD_DECISION)
    {"soft_decision", 0, OPT_FLAG, 0, 0, &global_list_flag_bch_sim_decodingMode[KIND_OF_BCH_DECODING_BM], FLAG_MASK_CODE_BCH_DEC_AL_SOFT_DECISION, VALUE_TYPE_UNSIGNED_INT, NULL},//if(global_list_flag_bch_sim_decodingMode[KIND_OF_BCH_DECODING_BM]&FLAG_MASK_CODE_BCH_DEC_AL_SOFT_DECISION)


    {"decoding", 0, OPT_COMPONENT, 0, 0, NULL, 0, VALUE_TYPE_NONE, bch_BM_algorithm_decoding_option},

        #ifndef RELEASE
        {"debug", 0, OPT_DEBUG, 0, 0, NULL, 0, VALUE_TYPE_NONE, bch_BM_algorithm_debug_option},
        #endif

    {NULL,0,STRUCT_END,0,0,NULL,0,VALUE_TYPE_NONE,NULL}
};
/***** static struct struct_cmdLineOption bch_algorithm[] end *****/
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/************************************ bch_algorithm 2 n d - E n d ************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    #ifndef RELEASE
    static struct struct_cmdLineOption    bch_mSBS_algorithm_HD_display_option[] =
    {
        {"result", 0, OPT_FLAG, 0, 0, &global_flag_bch_mSBS_SimHD_display, FLAG_MASK_DISPLAY_RESULT, VALUE_TYPE_UNSIGNED_INT, NULL},
        {"progress", 0, OPT_FLAG, 0, 0, &global_flag_bch_mSBS_SimHD_display, FLAG_MASK_DISPLAY_PROGRESS, VALUE_TYPE_UNSIGNED_INT, NULL},
        {"funcname", 0, OPT_FLAG, 0, 0, &global_flag_bch_mSBS_SimHD_display, FLAG_MASK_DISPLAY_FUNCNAME, VALUE_TYPE_UNSIGNED_INT, NULL},
        {NULL,0,STRUCT_END,0,0,NULL,0,VALUE_TYPE_NONE,NULL}
    };
    #endif

    #ifndef RELEASE
    static struct struct_cmdLineOption    bch_mSBS_algorithm_SD_display_option[] =
    {
        {"result", 0, OPT_FLAG, 0, 0, &global_flag_bch_mSBS_SimSD_display, FLAG_MASK_DISPLAY_RESULT, VALUE_TYPE_UNSIGNED_INT, NULL},
        {"progress", 0, OPT_FLAG, 0, 0, &global_flag_bch_mSBS_SimSD_display, FLAG_MASK_DISPLAY_PROGRESS, VALUE_TYPE_UNSIGNED_INT, NULL},
        {"funcname", 0, OPT_FLAG, 0, 0, &global_flag_bch_mSBS_SimSD_display, FLAG_MASK_DISPLAY_FUNCNAME, VALUE_TYPE_UNSIGNED_INT, NULL},
        {NULL,0,STRUCT_END,0,0,NULL,0,VALUE_TYPE_NONE,NULL}
    };
    #endif
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static struct struct_cmdLineOption    bch_mSBS_algorithm_test_option[] =
{
    {"used", 0, OPT_FLAG, 0, 0, &global_list_flag_bch_sim_decodingMode[KIND_OF_BCH_DECODING_mSBS], FLAG_MASK_CODE_BCH_DEC_AL_TEST_THIS_ALGORITHM, VALUE_TYPE_UNSIGNED_INT, NULL},//if(global_list_flag_bch_sim_decodingMode[KIND_OF_BCH_DECODING_mSBS]&FLAG_MASK_CODE_BCH_DEC_AL_TEST_THIS_ALGORITHM)

    {NULL,0,STRUCT_END,0,0,NULL,0,VALUE_TYPE_NONE,NULL}
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static struct struct_cmdLineOption    bch_mSBS_algorithm_decoding_option[] =
{
    {"skip", 0, OPT_FLAG, 0, 0, &global_list_flag_bch_sim_decodingMode[KIND_OF_BCH_DECODING_mSBS], FLAG_MASK_CODE_BCH_DEC_AL_SKIP, VALUE_TYPE_UNSIGNED_INT, NULL},

    {NULL,0,STRUCT_END,0,0,NULL,0,VALUE_TYPE_NONE,NULL}
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static struct struct_cmdLineOption    bch_mSBS_algorithm_debug_calculate_option[] =
{
    {"delta", 0, OPT_FLAG, 0, 0, &global_flag_debug_mSBS_Sim, FLAG_MASK_DEBUG_BCH_M_SBS_CAL_DELTA, VALUE_TYPE_UNSIGNED_INT, NULL},

    {NULL,0,STRUCT_END,0,0,NULL,0,VALUE_TYPE_NONE,NULL}
};

    #ifndef RELEASE
    static struct struct_cmdLineOption    bch_mSBS_algorithm_debug_option[] =
    {
        {"sequence", 0, OPT_FLAG, 0, 0, &global_flag_debug_mSBS_Sim, FLAG_MASK_DEBUG_BCH_mSBS_ALGORITHM, VALUE_TYPE_UNSIGNED_INT, NULL},
        {"cal", 0, OPT_COMPONENT, 0, 0, NULL, 0, VALUE_TYPE_NONE, bch_mSBS_algorithm_debug_calculate_option},

        {NULL,0,STRUCT_END,0,0,NULL,0,VALUE_TYPE_NONE,NULL}
    };
    #endif
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static struct struct_cmdLineOption bch_mSBS_algorithm_option[] =
{
    {"used", 0, OPT_FLAG, 0, 0, &global_flag_bchDecAlgo_Enable, FLAG_MASK_BCH_DECORDING_ALGORITHM_M_SBS, VALUE_TYPE_UNSIGNED_INT, NULL},

    {"algorithm", 0, OPT_COMPONENT, 0, 0, &global_list_flag_bch_sim_decodingMode[KIND_OF_BCH_DECODING_mSBS], FLAG_MASK_CODE_BCH_DEC_AL_TEST_THIS_ALGORITHM, VALUE_TYPE_UNSIGNED_INT, bch_mSBS_algorithm_test_option},
        #ifndef RELEASE
        {"display_HD", 0, OPT_COMPONENT, 0, 0, NULL, 0, VALUE_TYPE_NONE, bch_mSBS_algorithm_HD_display_option},
        {"display_SD", 0, OPT_COMPONENT, 0, 0, NULL, 0, VALUE_TYPE_NONE, bch_mSBS_algorithm_SD_display_option},
        #endif

    {"hard_decision", 0, OPT_FLAG, 0, 0, &global_list_flag_bch_sim_decodingMode[KIND_OF_BCH_DECODING_mSBS], FLAG_MASK_CODE_BCH_DEC_AL_HARD_DECISION, VALUE_TYPE_UNSIGNED_INT, NULL},//if(global_list_flag_bch_sim_decodingMode[KIND_OF_BCH_DECODING_BM]&FLAG_MASK_CODE_BCH_DEC_AL_HARD_DECISION)
    {"soft_decision", 0, OPT_FLAG, 0, 0, &global_list_flag_bch_sim_decodingMode[KIND_OF_BCH_DECODING_mSBS], FLAG_MASK_CODE_BCH_DEC_AL_SOFT_DECISION, VALUE_TYPE_UNSIGNED_INT, NULL},//if(global_list_flag_bch_sim_decodingMode[KIND_OF_BCH_DECODING_BM]&FLAG_MASK_CODE_BCH_DEC_AL_SOFT_DECISION)

    {"decoding", 0, OPT_COMPONENT, 0, 0, NULL, 0, VALUE_TYPE_NONE, bch_mSBS_algorithm_decoding_option},

        #ifndef RELEASE
        {"debug", 0, OPT_DEBUG, 0, 0, NULL, 0, VALUE_TYPE_NONE, bch_mSBS_algorithm_debug_option},
        #endif

    {NULL,0,STRUCT_END,0,0,NULL,0,VALUE_TYPE_NONE,NULL}
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/************************************ bch_algorithm 1 s t - s t a r t ************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/***** static struct struct_cmdLineOption struct_cmdLineOption[] start *****/
static struct struct_cmdLineOption bch_common[] =
{
    {"oversynd", 0, OPT_SET_VALUE, 0, 0, &global_bch_Soft_OverSyndLen, 0, VALUE_TYPE_UNSIGNED_INT, NULL},
    {"addlcm", 0, OPT_SET_VALUE, 0, 0, &global_addedRootToLCM_string, 0, VALUE_TYPE_STRING, NULL},

    {NULL,0,STRUCT_END,0,0,NULL,0,VALUE_TYPE_NONE,NULL}
};

static struct struct_cmdLineOption bch_algorithm[] =
{
    {"used", 0, OPT_FLAG, 0, 0, &global_flag_simulationCode, FLAG_MASK_CODE_BCH, VALUE_TYPE_UNSIGNED_INT, NULL},
    {"common", 0, OPT_COMPONENT, 0, 0, NULL, 0, VALUE_TYPE_NONE, bch_common},

    {"bm", 0, OPT_COMPONENT, 0, 0, NULL, 0, VALUE_TYPE_NONE, bch_BM_algorithm_option},
    {"msbs_t3", 0, OPT_COMPONENT, 0, 0, NULL, 0, VALUE_TYPE_NONE, bch_mSBS_algorithm_option},
    {NULL,0,STRUCT_END,0,0,NULL,0,VALUE_TYPE_NONE,NULL}
};
/***** static struct struct_cmdLineOption struct_cmdLineOption[] end *****/
/************************************ bch_algorithm 1 s t - e n d ************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    #ifndef RELEASE
    static struct struct_cmdLineOption    usable_code_GaloisField_display_option[] =
    {
    {"primepoly", 0, OPT_FLAG, 0, 0, &global_flag_gfCommon_display, FLAG_MASK_DISPLAY_GF_COMMON_PRIMEPOLY, VALUE_TYPE_UNSIGNED_INT, NULL},
    {"field", 0, OPT_FLAG, 0, 0, &global_flag_gfCommon_display, FLAG_MASK_DISPLAY_GF_COMMON_FIELD, VALUE_TYPE_UNSIGNED_INT, NULL},
    {"conjugacies", 0, OPT_FLAG, 0, 0, &global_flag_gfCommon_display, FLAG_MASK_DISPLAY_GF_COMMON_CONJUGACIES, VALUE_TYPE_UNSIGNED_INT, NULL},
    {"genpoly", 0, OPT_FLAG, 0, 0, &global_flag_gfCommon_display, FLAG_MASK_DISPLAY_GF_COMMON_GENPOLY, VALUE_TYPE_UNSIGNED_INT, NULL},
    {"sum_matrix", 0, OPT_FLAG, 0, 0, &global_flag_gfCommon_display, FLAG_MASK_DISPLAY_GF_COMMON_SUM_MATRIX, VALUE_TYPE_UNSIGNED_INT, NULL},

    {"fileio", 0, OPT_FLAG, 0, 0, &global_flag_gfCommon_display, FLAG_MASK_DISPLAY_GF_COMMON_FILE_IO, VALUE_TYPE_UNSIGNED_INT, NULL},
        {NULL,0,STRUCT_END,0,0,NULL,0,VALUE_TYPE_NONE,NULL}
    };
    #endif

static struct struct_cmdLineOption usable_code_GaloisField[] =
{
    {"exp", 0, OPT_SET_VALUE, 0, 0, &global_GaloisFieldExponential, 0, VALUE_TYPE_UNSIGNED_INT, NULL},
    {"codelength", 0, OPT_SET_VALUE, 0, 0, &global_CodeLength, 0, VALUE_TYPE_UNSIGNED_INT, NULL},
    {"limitcodelength", 0, OPT_SET_VALUE, 0, 0, &global_LimitCodeLength, 0, VALUE_TYPE_UNSIGNED_INT, NULL},

    {"makeall", 0, OPT_FLAG, 0, 0, &global_flag_gfCommon, FLAG_MASK_GF_COMMON_MAKE_GF_ALL, VALUE_TYPE_UNSIGNED_INT, NULL},
        #ifndef RELEASE
        {"display", 0, OPT_COMPONENT, 0, 0, NULL, 0, VALUE_TYPE_NONE, usable_code_GaloisField_display_option},
        #endif
    {NULL,0,STRUCT_END,0,0,NULL,0,VALUE_TYPE_NONE,NULL}
};



static struct struct_cmdLineOption usaable_code_com_algorithm_option[] =
{
    {"errnum", 0, OPT_SET_VALUE, 0, 0, &global_ErrorNum, 0, VALUE_TYPE_UNSIGNED_INT, NULL},

    {NULL,0,STRUCT_END,0,0,NULL,0,VALUE_TYPE_NONE,NULL}
};

static struct struct_cmdLineOption    usaable_code_com_HD_option[] =
{
    {"correctable", 0, OPT_SET_VALUE, 0, 0, &global_bch_hardCorrectable, 0, VALUE_TYPE_UNSIGNED_INT, NULL},
        #ifndef RELEASE
        {"display", 0, OPT_COMPONENT, 0, 0, NULL, 0, VALUE_TYPE_NONE, bch_BM_algorithm_HD_display_option},
        #endif
    {NULL,0,STRUCT_END,0,0,NULL,0,VALUE_TYPE_NONE,NULL}
};

static struct struct_cmdLineOption usaable_code_com_HD_SD_option [] =
{
    {"hard", 0, OPT_SET_VALUES_SERIES, 0, 0, &global_bch_hardCorrectable, 0, VALUE_TYPE_UNSIGNED_INT, NULL},
    {"soft", 0, OPT_SET_VALUES_SERIES, 0, 0, &global_bch_SoftCorrectable, 0, VALUE_TYPE_UNSIGNED_INT, NULL},
    {NULL,0,STRUCT_END,0,0,NULL,0,VALUE_TYPE_NONE,NULL}
};


static struct struct_cmdLineOption    usaable_code_com_SD_option[] =
{
    {"correctable", 0, OPT_SET_VALUE, 0, 0, &global_bch_SoftCorrectable, 0, VALUE_TYPE_UNSIGNED_INT, NULL},
    {"correctables", 0, OPT_SET_VALUES, 0, 0, NULL, 0, VALUE_TYPE_NONE, usaable_code_com_HD_SD_option},

        #ifndef RELEASE
        {"display", 0, OPT_COMPONENT, 0, 0, NULL, 0, VALUE_TYPE_NONE, bch_BM_algorithm_SD_display_option},
        #endif
    {NULL,0,STRUCT_END,0,0,NULL,0,VALUE_TYPE_NONE,NULL}
};

static struct struct_cmdLineOption usable_code_common[] =
{
    {"gf", 0, OPT_COMPONENT, 0, 0, NULL, 0, VALUE_TYPE_NONE, usable_code_GaloisField},

    //added correctables
    {"algorithm", 0, OPT_COMPONENT, 0, 0, &global_flag_sim_decodingMode, FLAG_MASK_CODE_BCH_DEC_AL_TEST_THIS_ALGORITHM, VALUE_TYPE_UNSIGNED_INT, usaable_code_com_algorithm_option},

    {"hard", 0, OPT_COMPONENT, 0, 0, &global_flag_sim_decodingMode, FLAG_MASK_CODE_BCH_DEC_AL_HARD_DECISION, VALUE_TYPE_UNSIGNED_INT, usaable_code_com_HD_option},//if(global_list_flag_bch_sim_decodingMode[KIND_OF_BCH_DECODING_BM]&FLAG_MASK_CODE_BCH_DEC_AL_HARD_DECISION)
    {"soft", 0, OPT_COMPONENT, 0, 0, &global_flag_sim_decodingMode, FLAG_MASK_CODE_BCH_DEC_AL_SOFT_DECISION, VALUE_TYPE_UNSIGNED_INT, usaable_code_com_SD_option},//if(global_list_flag_bch_sim_decodingMode[KIND_OF_BCH_DECODING_BM]&FLAG_MASK_CODE_BCH_DEC_AL_SOFT_DECISION)

    {NULL,0,STRUCT_END,0,0,NULL,0,VALUE_TYPE_NONE,NULL}
};

static struct struct_cmdLineOption cmdOpt_usable_code[] =
{
    {"bch", 0, OPT_COMPONENT, 0, 0, &global_flag_simulationCode, FLAG_MASK_CODE_BCH, VALUE_TYPE_UNSIGNED_INT, bch_algorithm},

    {"common", 0, OPT_COMPONENT, 0, 0, NULL, 0, VALUE_TYPE_NONE, usable_code_common},

    {NULL,0,STRUCT_END,0,0,NULL,0,VALUE_TYPE_NONE,NULL}
};

/* BCH Command Tree end */
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static struct struct_cmdLineOption comm_channel_awgn_ebn0_range[] =
{
    {"start", 0, OPT_SET_LANGE_SERIES, 0, 0, &global_EbN0_Start_Value, 0, VALUE_TYPE_DOUBLE, NULL},
    {"end", 0, OPT_SET_LANGE_SERIES, 0, 0, &global_EbN0_End_Value, 0, VALUE_TYPE_DOUBLE, NULL},
    {"step", 0, OPT_SET_LANGE_SERIES, 0, 0, &global_EbN0_Step, 0, VALUE_TYPE_DOUBLE, NULL},
    {"(not complete analysys argument of range, in partial processing)", 0, OPT_NOTHING, 0, 0, NULL, 0, VALUE_TYPE_NONE, NULL},
    {NULL,0,STRUCT_END,0,0,NULL,0,VALUE_TYPE_NONE,NULL}
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static struct struct_cmdLineOption comm_channel_awgn_ebn0[] =
{
    {"range", 0, OPT_SET_LANGE, 0, 0, NULL, 0, VALUE_TYPE_NONE, comm_channel_awgn_ebn0_range},

    {"start", 0, OPT_SET_VALUE, 0, 0, &global_EbN0_Start_Value, 0, VALUE_TYPE_DOUBLE, NULL},
    {"end", 0, OPT_SET_VALUE, 0, 0, &global_EbN0_End_Value, 0, VALUE_TYPE_DOUBLE, NULL},
    {"step", 0, OPT_SET_VALUE, 0, 0, &global_EbN0_Step, 0, VALUE_TYPE_DOUBLE, NULL},
    {NULL,0,STRUCT_END,0,0,NULL,0,VALUE_TYPE_NONE,NULL}
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    #ifndef RELEASE
    static struct struct_cmdLineOption comm_channel_awgn_llr_debug[] =
    {
        {"sequence", 0, OPT_FLAG, 0, 0, &global_flag_debug_awgnLLR, FLAG_MASK_DEBUG_AWGN_LLR_SEQUENCE, VALUE_TYPE_UNSIGNED_INT, NULL},
        {"locator", 0, OPT_FLAG, 0, 0, &global_flag_debug_awgnLLR, FLAG_MASK_DEBUG_AWGN_LLR_LOCATOR, VALUE_TYPE_UNSIGNED_INT, NULL},
        {NULL,0,STRUCT_END,0,0,NULL,0,VALUE_TYPE_NONE,NULL}
    };
    #endif
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static struct struct_cmdLineOption comm_ch_awgn_same_llr_mag_handling[] =
{
    {"none", 0, OPT_FLAG_CASE, 0, 0, &global_flag_case_same_llr_handling, FLAG_CASE_SAME_LLR_HANDLING_NONE, VALUE_TYPE_UNSIGNED_INT, NULL},
    {"not_sel", 0, OPT_FLAG_CASE, 0, 0, &global_flag_case_same_llr_handling, FLAG_CASE_SAME_LLR_HANDLING_NOT_SEL, VALUE_TYPE_UNSIGNED_INT, NULL},
    {"prior_0", 0, OPT_FLAG_CASE, 0, 0, &global_flag_case_same_llr_handling, FLAG_CASE_SAME_LLR_HANDLING_PRIOR_HD_0, VALUE_TYPE_UNSIGNED_INT, NULL},
    {"prior_1", 0, OPT_FLAG_CASE, 0, 0, &global_flag_case_same_llr_handling, FLAG_CASE_SAME_LLR_HANDLING_PRIOR_HD_1, VALUE_TYPE_UNSIGNED_INT, NULL},

    {"(not sel option is not work on tree)", 0, OPT_NOTHING, 0, 0, NULL, 0, VALUE_TYPE_NONE, NULL},
    {NULL,0,STRUCT_END,0,0,NULL,0,VALUE_TYPE_NONE,NULL}
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static struct struct_cmdLineOption comm_ch_awgn_init_llr_mag_method[] =
{
    {"none", 0, OPT_FLAG_CASE, 0, 0, &global_flag_case_init_llr_mag_method, FLAG_CASE_INIT_LLR_MAG_METHOD_NONE, VALUE_TYPE_UNSIGNED_INT, NULL},
    {"not_sel_max_llr", 0, OPT_FLAG_CASE, 0, 0, &global_flag_case_init_llr_mag_method, FLAG_CASE_INIT_LLR_MAG_METHOD_NOT_SEL_MAX_LLR, VALUE_TYPE_UNSIGNED_INT, NULL},

    {"(not sel max option is not work on tree)", 0, OPT_NOTHING, 0, 0, NULL, 0, VALUE_TYPE_NONE, NULL},
    {NULL,0,STRUCT_END,0,0,NULL,0,VALUE_TYPE_NONE,NULL}
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static struct struct_cmdLineOption comm_ch_awgn_llr_mag_find_meth_grouping[] =
{
    {"enable", 0, OPT_FLAG_CASE, 0, 0, &global_flag_case_find_LLR_method, FLAG_CASE_FINDING_MIN_LLR_METHOD_GROUPING, VALUE_TYPE_UNSIGNED_INT, NULL},
    {"group_num", 0, OPT_SET_VALUE, 0, 0, &global_grouping_stream_nums, 0, VALUE_TYPE_UNSIGNED_INT, NULL},
    {NULL,0,STRUCT_END,0,0,NULL,0,VALUE_TYPE_NONE,NULL}
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static struct struct_cmdLineOption comm_ch_awgn_llr_mag_find_meth_tree[] =
{
    {"follow_min_path", 0, OPT_FLAG_CASE, 0, 0, &global_flag_case_find_LLR_method, FLAG_CASE_FINDING_MIN_LLR_METHOD_TREE_FOLLOWING_MIN1_PATH, VALUE_TYPE_UNSIGNED_INT, NULL},
    {"probabilistic_choosen", 0, OPT_FLAG_CASE, 0, 0, &global_flag_case_find_LLR_method, FLAG_CASE_FINDING_MIN_LLR_METHOD_TREE_PROBABILISIC_CHOOSEN, VALUE_TYPE_UNSIGNED_INT, NULL},
    //{"prior_HD_zero", 0, OPT_FLAG_CASE, 0, 0, &global_flag_case_find_LLR_method, FLAG_CASE_FINDING_MIN_LLR_METHOD_TREE_PRIOR_HD_ZERO, VALUE_TYPE_UNSIGNED_INT, NULL},
    {"(not complete a option of probabilistic_choosen)", 0, OPT_NOTHING, 0, 0, NULL, 0, VALUE_TYPE_NONE, NULL},
    {NULL,0,STRUCT_END,0,0,NULL,0,VALUE_TYPE_NONE,NULL}
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static struct struct_cmdLineOption comm_channel_awgn_llr_mag_finding_meth[] =
{
    {"pass_hd_1_dis", 0, OPT_FLAG_CASE, 0, 0, &global_flag_case_pass_hd_1, FLAG_CASE_PASS_MAG0_HD_1_DISABLE, VALUE_TYPE_UNSIGNED_INT, NULL},
    {"pass_hd_1_en", 0, OPT_FLAG_CASE, 0, 0, &global_flag_case_pass_hd_1, FLAG_CASE_PASS_MAG0_HD_1_ENABLE, VALUE_TYPE_UNSIGNED_INT, NULL},

    {"pri_same_LLR_mag", 0, OPT_COMPONENT, 0, 0, NULL, 0, VALUE_TYPE_NONE, comm_ch_awgn_same_llr_mag_handling},
    {"init_LLR_loc_val", 0, OPT_COMPONENT, 0, 0, NULL, 0, VALUE_TYPE_NONE, comm_ch_awgn_init_llr_mag_method},

    {"minimum", 0, OPT_FLAG_CASE, 0, 0, &global_flag_case_find_LLR_method, FLAG_CASE_FINDING_MIN_LLR_METHOD_MINIMUM, VALUE_TYPE_UNSIGNED_INT, NULL},
    {"group_min_push_away", 0, OPT_COMPONENT, 0, 0, &global_flag_case_find_LLR_method, FLAG_CASE_FINDING_MIN_LLR_METHOD_GROUPING, VALUE_TYPE_NONE, comm_ch_awgn_llr_mag_find_meth_grouping},
    {"tree", 0, OPT_COMPONENT, 0, 0, NULL, 0, VALUE_TYPE_NONE, comm_ch_awgn_llr_mag_find_meth_tree},

    {"notice)pass_hd_1_is_not_tested_in_tree_structure",0,OPT_NOTHING,0,0,NULL,0,VALUE_TYPE_NONE,NULL},
    {NULL,0,STRUCT_END,0,0,NULL,0,VALUE_TYPE_NONE,NULL}
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static struct struct_cmdLineOption comm_channel_llr_quantiz_filter_set[] =
{
    {"print_filtering_range", 0, OPT_FLAG, 0, 0, &global_flag_llr_display, FLAG_LLR_DISPLAY_QUANTIZATION_FILTERING, VALUE_TYPE_UNSIGNED_INT, NULL},

    {"offset", 0, OPT_SET_VALUE, 0, 0, &global_LLR_quantizFilter_offset, 0, VALUE_TYPE_DOUBLE, NULL},
    {"step", 0, OPT_SET_VALUE, 0, 0, &global_LLR_quantizFilter_step, 0, VALUE_TYPE_DOUBLE, NULL},
    {"bitwidth", 0, OPT_SET_VALUE, 0, 0, &global_LLR_quantizFilter_bitWidth, 0, VALUE_TYPE_UNSIGNED_CHAR, NULL},
        #ifndef RELEASE
        {"debug", 0, OPT_COMPONENT, 0, 0, NULL, 0, VALUE_TYPE_NONE, comm_channel_awgn_llr_debug},
        #endif
    {NULL,0,STRUCT_END,0,0,NULL,0,VALUE_TYPE_NONE,NULL}
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static struct struct_cmdLineOption comm_channel_llr_quantiz_numberic_system[] =
{
    {"twos_complement", 0, OPT_FLAG_CASE, 0, 0, &global_flag_case_llr_numberic_system, ENUM_FLAG_CASE_QUANTIZ_NUMBERIC_SYSTEM_TWO_S_COMPLEMENT, VALUE_TYPE_UNSIGNED_INT, NULL},
    {"ones_complement", 0, OPT_FLAG_CASE, 0, 0, &global_flag_case_llr_numberic_system, ENUM_FLAG_CASE_QUANTIZ_NUMBERIC_SYSTEM_ONE_S_COMPLEMENT, VALUE_TYPE_UNSIGNED_INT, NULL},

    {NULL,0,STRUCT_END,0,0,NULL,0,VALUE_TYPE_NONE,NULL}
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static struct struct_cmdLineOption comm_channel_llr_quantiz_zero_symmetry[] =
{
    //{"disable", 0, OPT_FLAG_CASE, 0, 0, &global_flag_case_llr_mag_ones_complement_zero_handling, ENUM_FLAG_CASE_QUANTIZ_NUMBERIC_SYS_ONE_S_COM_NONE, VALUE_TYPE_UNSIGNED_INT, NULL},
    {"enbale", 0, OPT_FLAG_CASE, 0, 0, &global_flag_case_llr_mag_ones_complement_zero_handling, ENUM_FLAG_CASE_QUANTIZ_NUMBERIC_SYS_ONE_S_COM_ZERO_SYMMETRY, VALUE_TYPE_UNSIGNED_INT, NULL},

    {NULL,0,STRUCT_END,0,0,NULL,0,VALUE_TYPE_NONE,NULL}
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static struct struct_cmdLineOption comm_channel_llr_quantiz_zero_duplicated[] =
{
    //{"disable", 0, OPT_FLAG_CASE, 0, 0, &global_flag_case_llr_mag_ones_complement_zero_handling, ENUM_FLAG_CASE_QUANTIZ_NUMBERIC_SYS_ONE_S_COM_NONE, VALUE_TYPE_UNSIGNED_INT, NULL},
    {"enbale", 0, OPT_FLAG_CASE, 0, 0, &global_flag_case_llr_mag_ones_complement_zero_handling, ENUM_FLAG_CASE_QUANTIZ_NUMBERIC_SYS_ONE_S_COM_ZERO_DUPLICATED, VALUE_TYPE_UNSIGNED_INT, NULL},

    {NULL,0,STRUCT_END,0,0,NULL,0,VALUE_TYPE_NONE,NULL}
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static struct struct_cmdLineOption comm_channel_llr_quantization[] =
{
    {"filter_set", 0, OPT_COMPONENT, 0, 0, NULL, 0, VALUE_TYPE_NONE, comm_channel_llr_quantiz_filter_set},

    {"numberic_system", 0, OPT_COMPONENT, 0, 0, NULL, 0, VALUE_TYPE_NONE, comm_channel_llr_quantiz_numberic_system},
    {"zero_symmetry", 0, OPT_COMPONENT, 0, 0, NULL, 0, VALUE_TYPE_NONE, comm_channel_llr_quantiz_zero_symmetry},
    {"zero_duplicated", 0, OPT_COMPONENT, 0, 0, NULL, 0, VALUE_TYPE_NONE, comm_channel_llr_quantiz_zero_duplicated},

    {NULL,0,STRUCT_END,0,0,NULL,0,VALUE_TYPE_NONE,NULL}
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static struct struct_cmdLineOption comm_channel_awgn_llr[] =
{
    {"quantization", 0, OPT_COMPONENT, 0, 0, NULL, 0, VALUE_TYPE_NONE, comm_channel_llr_quantization},

    {"min_val_finding_meth", 0, OPT_COMPONENT, 0, 0, NULL, 0, VALUE_TYPE_NONE, comm_channel_awgn_llr_mag_finding_meth},
    {NULL,0,STRUCT_END,0,0,NULL,0,VALUE_TYPE_NONE,NULL}
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    #ifndef RELEASE
    static struct struct_cmdLineOption comm_channel_awgn_debug[] =
    {
        {"sequence", 0, OPT_FLAG, 0, 0, &global_flag_debug_awgn, FLAG_MASK_DEBUG_AWGN_SEQUENCE, VALUE_TYPE_UNSIGNED_INT, NULL},
        {NULL,0,STRUCT_END,0,0,NULL,0,VALUE_TYPE_NONE,NULL}
    };
    #endif
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static struct struct_cmdLineOption comm_channel_awgn[] =
{
    {"used", 0, OPT_FLAG, 0, 0, &global_Channel_Mode, FLAG_MASK_CHANNEL_MODE_AWGN, VALUE_TYPE_UNSIGNED_INT, NULL},

    {"random_seed_disable", 0, OPT_FLAG, 0, 0, &global_flag_Channel_Mode_Func, FLAG_MASK_CHANNEL_MODE_FUNC_RANDOM_SEED_DISABLE, VALUE_TYPE_UNSIGNED_INT, NULL},
    {"bitrate_disable", 0, OPT_FLAG, 0, 0, &global_flag_Channel_Mode_Func, FLAG_MASK_CHANNEL_MODE_FUNC_BITRATE_DISABLE, VALUE_TYPE_UNSIGNED_INT, NULL},

    {"ebn0", 0, OPT_COMPONENT, 0, 0, NULL, 0, VALUE_TYPE_NONE, comm_channel_awgn_ebn0},
    {"llr", 0, OPT_COMPONENT, 0, 0, NULL, 0, VALUE_TYPE_NONE, comm_channel_awgn_llr},

        #ifndef RELEASE
        {"debug", 0, OPT_COMPONENT, 0, 0, NULL, 0, 0, comm_channel_awgn_debug},
        #endif
    {NULL,0,STRUCT_END,0,0,NULL,0,VALUE_TYPE_NONE,NULL}
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/************************************ cmdOpt_comm_channel 1 s t - s t a r t ************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static struct struct_cmdLineOption cmdOpt_comm_channel[] =
{
    {"awgn", 0, OPT_COMPONENT, 0, 0, &global_Channel_Mode, FLAG_MASK_CHANNEL_MODE_AWGN, VALUE_TYPE_UNSIGNED_INT, comm_channel_awgn},
    {NULL,0,STRUCT_END,0,0,NULL,0,VALUE_TYPE_NONE,NULL}
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/************************************ cmdOpt_comm_channel 1 s t - e n d ************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    #ifndef RELEASE
    static struct struct_cmdLineOption bpsk_modulation_display[] =
    {
        {"result", 0, OPT_FLAG, 0, 0, &global_flag_Modulation_Mode_BPSK_Display, FLAG_MASK_DISPLAY_RESULT, VALUE_TYPE_UNSIGNED_INT, NULL},
        {"progress", 0, OPT_FLAG, 0, 0, &global_flag_Modulation_Mode_BPSK_Display, FLAG_MASK_DISPLAY_PROGRESS, VALUE_TYPE_UNSIGNED_INT, NULL},
        {"funcname", 0, OPT_FLAG, 0, 0, &global_flag_Modulation_Mode_BPSK_Display, FLAG_MASK_DISPLAY_FUNCNAME, VALUE_TYPE_UNSIGNED_INT, NULL},

        {NULL,0,STRUCT_END,0,0,NULL,0,VALUE_TYPE_NONE,NULL}
    };
    #endif

static struct struct_cmdLineOption bpsk_modulation[] =
{
    {"used", 0, OPT_FLAG, 0, 0, &global_flag_Modulation_Mode, DEFAULT_MODULATION_MODE, VALUE_TYPE_UNSIGNED_INT, NULL},

        #ifndef RELEASE
        {"display", 0, OPT_COMPONENT, 0, 0, NULL, 0, VALUE_TYPE_NONE, bpsk_modulation_display},
        #endif
    {NULL,0,STRUCT_END,0,0,NULL,0,VALUE_TYPE_NONE,NULL}
};
static struct struct_cmdLineOption cmdOpt_comm_modulation[] =
{
    {"bpsk", 0, OPT_COMPONENT, 0, 0, NULL, 0, VALUE_TYPE_NONE, bpsk_modulation},
    {NULL,0,STRUCT_END,0,0,NULL,0,VALUE_TYPE_NONE,NULL}
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static struct struct_cmdLineOption cmdOpt_sim_loop[] =
{
    //{"start", 0, OPT_SET_VALUE, 0, 0, &global_EbN0_Start_Value, 0, VALUE_TYPE_DOUBLE, NULL},

    {"min", 0, OPT_SET_VALUE, 0, 0, &global_minimum_loop, 0, VALUE_TYPE_UNSIGNED_LONG, NULL},
    {"base", 0, OPT_SET_VALUE, 0, 0, &global_base_of_loop, 0, VALUE_TYPE_UNSIGNED_LONG, NULL},
    {"rate", 0, OPT_SET_VALUE, 0, 0, &global_rate_of_loop, 0, VALUE_TYPE_UNSIGNED_INT, NULL},

    {NULL,0,STRUCT_END,0,0,NULL,0,VALUE_TYPE_NONE,NULL}
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static struct struct_cmdLineOption cmdOpt_simulation[] =
{
    //{"start", 0, OPT_SET_VALUE, 0, 0, &global_EbN0_Start_Value, 0, VALUE_TYPE_DOUBLE, NULL},

    {"loop", 0, OPT_COMPONENT, 0, 0, NULL, 0, VALUE_TYPE_NONE, cmdOpt_sim_loop},

    {NULL,0,STRUCT_END,0,0,NULL,0,VALUE_TYPE_NONE,NULL}
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    #ifndef RELEASE
    static struct struct_cmdLineOption ber_actual_display[] =
    {
        {"result", 0, OPT_FLAG, 0, 0, &global_flag_case_getBER_method_Display, FLAG_MASK_DISPLAY_RESULT, VALUE_TYPE_UNSIGNED_INT, NULL},
        {"progress", 0, OPT_FLAG, 0, 0, &global_flag_case_getBER_method_Display, FLAG_MASK_DISPLAY_PROGRESS, VALUE_TYPE_UNSIGNED_INT, NULL},
        {"funcname", 0, OPT_FLAG, 0, 0, &global_flag_case_getBER_method_Display, FLAG_MASK_DISPLAY_FUNCNAME, VALUE_TYPE_UNSIGNED_INT, NULL},

        {NULL,0,STRUCT_END,0,0,NULL,0,VALUE_TYPE_NONE,NULL}
    };
    #endif

static struct struct_cmdLineOption ber_theo_decision[] =
{
    //{"(not-support)", 0, OPT_NOTHING, 0, 0, NULL, 0, VALUE_TYPE_NONE, NULL},
    {"allzero", 0, OPT_FLAG_CASE, 0, 0, &global_flag_case_choosingCodeWordMethod_inTheo, FLAG_CASE_CODEWORD_CHOOSEN_METHOD_IN_THEO_NO_ERR, VALUE_TYPE_UNSIGNED_INT, NULL},
    {"normal", 0, OPT_FLAG_CASE, 0, 0, &global_flag_case_choosingCodeWordMethod_inTheo, FLAG_CASE_CODEWORD_CHOOSEN_METHOD_IN_THEO_NORMAL, VALUE_TYPE_UNSIGNED_INT, NULL},
    {"best", 0, OPT_FLAG_CASE, 0, 0, &global_flag_case_choosingCodeWordMethod_inTheo, FLAG_CASE_CODEWORD_CHOOSEN_METHOD_IN_THEO_MIN_ERR, VALUE_TYPE_UNSIGNED_INT, NULL},
    {"worst", 0, OPT_FLAG_CASE, 0, 0, &global_flag_case_choosingCodeWordMethod_inTheo, FLAG_CASE_CODEWORD_CHOOSEN_METHOD_IN_THEO_MAX_ERR, VALUE_TYPE_UNSIGNED_INT, NULL},

    {NULL,0,STRUCT_END,0,0,NULL,0,VALUE_TYPE_NONE,NULL}
};



static struct struct_cmdLineOption ber_dec_decision_handing[] =
{
    {"none", 0, OPT_FLAG_CASE, 0, 0, &global_flag_case_dec_fail_handling, FLAG_CASE_DEC_FAIL_HANDLING_NONE, VALUE_TYPE_UNSIGNED_INT, NULL},
    {"retrans_no_err", 0, OPT_FLAG_CASE, 0, 0, &global_flag_case_dec_fail_handling, FLAG_CASE_DEC_FAIL_HANDLING_RE_TRANFER_FAIL_NO_MORE, VALUE_TYPE_UNSIGNED_INT, NULL},
    {"retrans", 0, OPT_FLAG_CASE, 0, 0, &global_flag_case_dec_fail_handling, FLAG_CASE_DEC_FAIL_HANDLING_RE_TRANFER_INTERATION, VALUE_TYPE_UNSIGNED_INT, NULL},

    {NULL,0,STRUCT_END,0,0,NULL,0,VALUE_TYPE_NONE,NULL}
};

static struct struct_cmdLineOption ber_dec_decision[] =
{
    //{"(not-support)", 0, OPT_NOTHING, 0, 0, NULL, 0, VALUE_TYPE_NONE, NULL},
    {"used", 0, OPT_FLAG_CASE, 0, 0, &global_flag_case_selTP_before_dec, FLAG_CASE_SEL_TP_BEFORE_DEC_ACT, VALUE_TYPE_UNSIGNED_INT, NULL},

    {"handling", 0, OPT_COMPONENT, 0, 0, &global_flag_case_selTP_before_dec, FLAG_CASE_SEL_TP_BEFORE_DEC_ACT, VALUE_TYPE_UNSIGNED_INT, ber_dec_decision_handing},

        #ifndef RELEASE
        {"display", 0, OPT_COMPONENT, 0, 0, NULL, 0, VALUE_TYPE_NONE, ber_actual_display},
        #endif

    {NULL,0,STRUCT_END,0,0,NULL,0,VALUE_TYPE_NONE,NULL}
};

static struct struct_cmdLineOption ber_options[] =
{
    //{"(not-support)", 0, OPT_NOTHING, 0, 0, NULL, 0, VALUE_TYPE_NONE, NULL},
    {"theoretical_decision", 0, OPT_COMPONENT, 0, 0, NULL, 0, VALUE_TYPE_NONE, ber_theo_decision},

    {"decoder_decision", 0, OPT_COMPONENT, 0, 0, &global_flag_case_selTP_before_dec, FLAG_CASE_SEL_TP_BEFORE_DEC_ACT, VALUE_TYPE_UNSIGNED_INT, ber_dec_decision},
    {NULL,0,STRUCT_END,0,0,NULL,0,VALUE_TYPE_NONE,NULL}
};
static struct struct_cmdLineOption cmdOpt_get_thing[] =
{
    //{"(not-support)", 0, OPT_NOTHING, 0, 0, NULL, 0, VALUE_TYPE_NONE, NULL},

    //{"ber", 0, OPT_FLAG, 0, 0, &global_flag_berOptions, FLAG_MASK_TEST_GET_BER, VALUE_TYPE_UNSIGNED_INT, NULL},
    {"ber", 0, OPT_COMPONENT, 0, 0, &global_flag_berOptions, FLAG_MASK_TEST_GET_BER, VALUE_TYPE_UNSIGNED_INT, ber_options},
    {NULL,0,STRUCT_END,0,0,NULL,0,VALUE_TYPE_NONE,NULL}
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static struct struct_cmdLineOption pattern_io[] =
{
    {"infomation", 0, OPT_FLAG, 0, 0, &global_flag_file_io_hd_pattern_output, FLAG_MASK_FILE_IO_HD_MES, VALUE_TYPE_UNSIGNED_INT, NULL},
    {"encoding", 0, OPT_FLAG, 0, 0, &global_flag_file_io_hd_pattern_output, FLAG_MASK_FILE_IO_HD_ENCODING_MES, VALUE_TYPE_UNSIGNED_INT, NULL},
    {"received", 0, OPT_FLAG, 0, 0, &global_flag_file_io_hd_pattern_output, FLAG_MASK_FILE_IO_HD_RECEIVED_MES, VALUE_TYPE_UNSIGNED_INT, NULL},
    {"corrected", 0, OPT_FLAG, 0, 0, &global_flag_file_io_hd_pattern_output, FLAG_MASK_FILE_IO_HD_CORRECTED_MES, VALUE_TYPE_UNSIGNED_INT, NULL},

    {"quantized_LLR", 0, OPT_FLAG, 0, 0, &global_flag_file_io_sd_pattern_output, FLAG_MASK_FILE_IO_SD_RECEIVED_LLR, VALUE_TYPE_UNSIGNED_INT, NULL},
    {"quantized_LLR_magnitude", 0, OPT_FLAG, 0, 0, &global_flag_file_io_sd_pattern_output, FLAG_MASK_FILE_IO_SD_RECEIVED_LLR_mag, VALUE_TYPE_UNSIGNED_INT, NULL},

    {"all", 0, OPT_FLAG, 0, 0, &global_flag_file_io_sd_pattern_output, -1, VALUE_TYPE_UNSIGNED_INT, NULL},
    {NULL,0,STRUCT_END,0,0,NULL,0,VALUE_TYPE_NONE,NULL}
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static struct struct_cmdLineOption pattern_io_log[] =
{
    {"infomation", 0, OPT_FLAG, 0, 0, &global_flag_file_io_hd_pattern_output_log, FLAG_MASK_FILE_IO_HD_MES, VALUE_TYPE_UNSIGNED_INT, NULL},
    {"encoding", 0, OPT_FLAG, 0, 0, &global_flag_file_io_hd_pattern_output_log, FLAG_MASK_FILE_IO_HD_ENCODING_MES, VALUE_TYPE_UNSIGNED_INT, NULL},
    {"received", 0, OPT_FLAG, 0, 0, &global_flag_file_io_hd_pattern_output_log, FLAG_MASK_FILE_IO_HD_RECEIVED_MES, VALUE_TYPE_UNSIGNED_INT, NULL},
    {"corrected", 0, OPT_FLAG, 0, 0, &global_flag_file_io_hd_pattern_output_log, FLAG_MASK_FILE_IO_HD_CORRECTED_MES, VALUE_TYPE_UNSIGNED_INT, NULL},

    {"quantized_LLR", 0, OPT_FLAG, 0, 0, &global_flag_file_io_sd_pattern_output_log, FLAG_MASK_FILE_IO_SD_RECEIVED_LLR, VALUE_TYPE_UNSIGNED_INT, NULL},
    {"quantized_LLR_magnitude", 0, OPT_FLAG, 0, 0, &global_flag_file_io_sd_pattern_output_log, FLAG_MASK_FILE_IO_SD_RECEIVED_LLR_mag, VALUE_TYPE_UNSIGNED_INT, NULL},

    {"all", 0, OPT_FLAG, 0, 0, &global_flag_file_io_sd_pattern_output_log, -1, VALUE_TYPE_UNSIGNED_INT, NULL},
    {NULL,0,STRUCT_END,0,0,NULL,0,VALUE_TYPE_NONE,NULL}
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static struct struct_cmdLineOption pattern_io_verilog[] =
{
    {"infomation", 0, OPT_FLAG, 0, 0, &global_flag_file_io_hd_pattern_output_verilog, FLAG_MASK_FILE_IO_HD_MES, VALUE_TYPE_UNSIGNED_INT, NULL},
    {"encoding", 0, OPT_FLAG, 0, 0, &global_flag_file_io_hd_pattern_output_verilog, FLAG_MASK_FILE_IO_HD_ENCODING_MES, VALUE_TYPE_UNSIGNED_INT, NULL},
    {"received", 0, OPT_FLAG, 0, 0, &global_flag_file_io_hd_pattern_output_verilog, FLAG_MASK_FILE_IO_HD_RECEIVED_MES, VALUE_TYPE_UNSIGNED_INT, NULL},
    {"corrected", 0, OPT_FLAG, 0, 0, &global_flag_file_io_hd_pattern_output_verilog, FLAG_MASK_FILE_IO_HD_CORRECTED_MES, VALUE_TYPE_UNSIGNED_INT, NULL},

    {"quantized_LLR", 0, OPT_FLAG, 0, 0, &global_flag_file_io_sd_pattern_output_verilog, FLAG_MASK_FILE_IO_SD_RECEIVED_LLR, VALUE_TYPE_UNSIGNED_INT, NULL},
    {"quantized_LLR_magnitude", 0, OPT_FLAG, 0, 0, &global_flag_file_io_sd_pattern_output_verilog, FLAG_MASK_FILE_IO_SD_RECEIVED_LLR_mag, VALUE_TYPE_UNSIGNED_INT, NULL},

    {"all", 0, OPT_FLAG, 0, 0, &global_flag_file_io_sd_pattern_output_verilog, -1, VALUE_TYPE_UNSIGNED_INT, NULL},
    {NULL,0,STRUCT_END,0,0,NULL,0,VALUE_TYPE_NONE,NULL}
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static struct struct_cmdLineOption cmdOpt_fileIO[] =
{
    {"testFlag", 0, OPT_FLAG, 0, 0, &global_flag_fileInputOutput, FLAG_MASK_FILE_IO, VALUE_TYPE_UNSIGNED_INT, NULL},
    {"pattern", 0, OPT_COMPONENT, 0, 0, NULL, 0, VALUE_TYPE_NONE, pattern_io},
    {"pattern_log", 0, OPT_COMPONENT, 0, 0, NULL, 0, VALUE_TYPE_NONE, pattern_io_log},
    {"pattern_verilog", 0, OPT_COMPONENT, 0, 0, NULL, 0, VALUE_TYPE_NONE, pattern_io_verilog},
    {NULL,0,STRUCT_END,0,0,NULL,0,VALUE_TYPE_NONE,NULL}
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static struct struct_cmdLineOption cmdOpt_testMode_err_distri_llr_static_msg[] =
{
    {"zero_stream_ratio", 0, OPT_SET_VALUE, 0, 0, &tmp_testMode_loops_zero_bit_ratio, 0, VALUE_TYPE_DOUBLE_RATIO_LIMIT, NULL},

    {NULL,0,STRUCT_END,0,0,NULL,0,VALUE_TYPE_NONE,NULL}
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static struct struct_cmdLineOption cmdOpt_testMode_err_distri_llr_msg_gen_meth[] =
{
    {"static", 0, OPT_FLAG_CASE, 0, 0, &global_flag_case_testMode_msg_gen_method, ENUM_CASE_TESTMODE_ERR_RATE_MSG_GEN_METH_STATIC, VALUE_TYPE_UNSIGNED_INT, cmdOpt_testMode_err_distri_llr_static_msg},
    {"rand", 0, OPT_FLAG_CASE, 0, 0, &global_flag_case_testMode_msg_gen_method, ENUM_CASE_TESTMODE_ERR_RATE_MSG_GEN_METH_RAND, VALUE_TYPE_UNSIGNED_INT, NULL},
    {NULL,0,STRUCT_END,0,0,NULL,0,VALUE_TYPE_NONE,NULL}
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static struct struct_cmdLineOption cmdOpt_testMode_err_distri_decoding_meth[] =
{
    {"hard", 0, OPT_FLAG_CASE, 0, 0, &global_flag_case_testMode_decoding_method, ENUM_CASE_TEST_MODE_ERR_RATE_DECODING_METH_HD, VALUE_TYPE_UNSIGNED_INT, NULL},
    {"soft", 0, OPT_FLAG_CASE, 0, 0, &global_flag_case_testMode_decoding_method, ENUM_CASE_TEST_MODE_ERR_RATE_DECODING_METH_SD, VALUE_TYPE_UNSIGNED_INT, NULL},
    {NULL,0,STRUCT_END,0,0,NULL,0,VALUE_TYPE_NONE,NULL}
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static struct struct_cmdLineOption cmdOpt_testMode_err_distribution[] =
{

    {"msg_gen_method", 0, OPT_COMPONENT, 0, 0, NULL, 0, VALUE_TYPE_NONE, cmdOpt_testMode_err_distri_llr_msg_gen_meth},
    {"decoding_method", 0, OPT_COMPONENT, 0, 0, NULL, 0, VALUE_TYPE_NONE, cmdOpt_testMode_err_distri_decoding_meth},
    {"tot_loops", 0, OPT_SET_VALUE, 0, 0, &tmp_testMode_loops, 0, VALUE_TYPE_UNSIGNED_LONG, NULL},
    {NULL,0,STRUCT_END,0,0,NULL,0,VALUE_TYPE_NONE,NULL}
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static struct struct_cmdLineOption cmdOpt_testMode_prob_distribution[] =
{

    {"group_num", 0, OPT_SET_VALUE, 0, 0, &tmp_testMode_prob_distribution_group_nums, 0, VALUE_TYPE_UNSIGNED_LONG, NULL},

    {"politics_same_val", 0, OPT_FLAG_CASE, 0, 0, &tmp_testMode_prob_min_choosen_politics, ENUM_CASE_TEST_MODE_PROB_MIN_CHOOSE_SAME_VAL, VALUE_TYPE_UNSIGNED_INT, NULL},
    {"politics_low_val", 0, OPT_FLAG_CASE, 0, 0, &tmp_testMode_prob_min_choosen_politics, ENUM_CASE_TEST_MODE_PROB_MIN_CHOOSE_LOW_VAL, VALUE_TYPE_UNSIGNED_INT, NULL},

    {"tot_loops", 0, OPT_SET_VALUE, 0, 0, &tmp_testMode_loops, 0, VALUE_TYPE_UNSIGNED_LONG, NULL},
    {NULL,0,STRUCT_END,0,0,NULL,0,VALUE_TYPE_NONE,NULL}
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static struct struct_cmdLineOption cmdOpt_testMode[] =
{
    {"errDistribution_EbN0", 0, OPT_FLAG_CASE, 0, 0, &global_flag_case_sim_testOpt, FLAG_CASE_SIM_TEST_MODE_GET_LLR_MAG_AVR_BELONG_EBN0, VALUE_TYPE_UNSIGNED_INT, cmdOpt_testMode_err_distribution},

    {"prob_distribution", 0, OPT_FLAG_CASE, 0, 0, &global_flag_case_sim_testOpt, FLAG_CASE_SIM_TEST_MODE_GET_PROBABILITY_OF_MINIMUM_VAL, VALUE_TYPE_UNSIGNED_INT, cmdOpt_testMode_prob_distribution},

    {"show_loop_cnt", 0, OPT_FLAG_CASE, 0, 0, &global_flag_case_sim_testOpt, FLAG_CASE_SIM_TEST_MODE_SHOW_LOOP_CNT, VALUE_TYPE_UNSIGNED_INT, NULL},

    {NULL,0,STRUCT_END,0,0,NULL,0,VALUE_TYPE_NONE,NULL}
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/************************************ R o o t s ************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static struct struct_cmdLineOption struct_cmdLineOption[] =
{
    //General cmd options
        #ifndef RELEASE
        {"log", 'l', OPT_FLAG, 0, 0, &global_flag_cmdOption, FLAG_MASK_PRINTF_LOG, VALUE_TYPE_UNSIGNED_INT, NULL},
        {"calc", 'c', OPT_FLAG, 0, 0, &global_flag_cmdOption, FLAG_MASK_PRINTF_CALCULATE_PROCESS, VALUE_TYPE_UNSIGNED_INT, NULL},
        {"memchk", 'm', OPT_FLAG, 0, 0, &global_flag_cmdOption, FLAG_MASK_PRINTF_MEMORY_CHECK_PROCESS, VALUE_TYPE_UNSIGNED_INT, NULL},
        {"warning", 'w', OPT_FLAG, 0, 0, &global_flag_cmdOption, FLAG_MASK_PRINTF_WARNING_MES, VALUE_TYPE_UNSIGNED_INT, NULL},
        #endif

        #ifndef RELEASE
        {"debug", 'd', OPT_FLAG, 0, 0, &global_flag_debug_display, FLAG_MASK_DEBUG_DEBUG, VALUE_TYPE_UNSIGNED_INT, NULL},
        {"debug_d_func", 0, OPT_FLAG, 0, 0, &global_flag_debug_display, FLAG_MASK_DISPLAY_FUNCNAME, VALUE_TYPE_UNSIGNED_INT, NULL},
        #endif

    {"help", 'h', OPT_FLAG, 0, 0, &global_flag_cmdSystemHold, FLAG_MASK_HOLD_HELP, VALUE_TYPE_UNSIGNED_INT, NULL},

    {"force", 'f', OPT_FLAG, 0, 0, &global_flag_cmdSystemUnhold, FLAG_MASK_SYSTEM_UNHOLD_FORCED, VALUE_TYPE_UNSIGNED_INT, NULL},

    {"code", 0, OPT_COMPONENT, 0, 0, &global_flag_sim_workingMode, FLAG_MASK_SIM_DECOING, VALUE_TYPE_UNSIGNED_INT, cmdOpt_usable_code},

    {"modulation", 0, OPT_COMPONENT, 0, 0, NULL, 0, VALUE_TYPE_NONE, cmdOpt_comm_modulation},
    {"channel", 0, OPT_COMPONENT, 0, 0, NULL, 0, VALUE_TYPE_NONE, cmdOpt_comm_channel},
    {"simulation", 0, OPT_COMPONENT, 0, 0, NULL, 0, VALUE_TYPE_NONE, cmdOpt_simulation},


    {"get", 0, OPT_COMPONENT, 0, 0, NULL, 0, VALUE_TYPE_NONE, cmdOpt_get_thing},

    {"file", 0, OPT_COMPONENT, 0, 0, NULL, 0, VALUE_TYPE_NONE, cmdOpt_fileIO},

    {"test_mode", 0, OPT_COMPONENT, 0, 0, &global_flag_sim_workingMode, FLAG_MASK_SIM_TEST, VALUE_TYPE_UNSIGNED_INT, cmdOpt_testMode},

    {NULL,0,STRUCT_END,0,0,NULL,0,VALUE_TYPE_NONE,NULL}
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void printInstList(struct struct_cmdLineOption *p, unsigned int recursive, char *passedStrng)
{
    unsigned int i;
    struct struct_cmdLineOption *temp;

    char *passingString = NULL;

    if(((p+0)->instType==OPT_SET_VALUES_SERIES)||((p+0)->instType==OPT_SET_LANGE_SERIES))
    {
        printf("=");
        for(i = 0; (temp = (p + i)); i++)
        {
            printf("[%s]", temp->longName);
            if((p + i + 1)->instType == OPT_SET_VALUES_SERIES) printf(",");
            else if((p + i + 1)->instType == OPT_SET_LANGE_SERIES) printf(":");
            else if((p + i + 1)->instType == STRUCT_END) break;
        }
        printf("}\n");
    }
    else
    {
        for(i = 0; (temp = (p + i))->instType != STRUCT_END; i++)
        {
            printf("\t");

            if(temp->longName)
            {
                if(recursive)
                {
                    printf("%s", passedStrng);
                    if((temp->instType==OPT_SET_VALUES)||(temp->instType==OPT_SET_LANGE))    printf("{");
                    printf("-%s%c", temp->longName,((temp->instType==OPT_SET_VALUES)||(temp->instType==OPT_SET_LANGE)?'\0':((temp->instType==OPT_SET_VALUE)?'=':'\t')));
                }
                else
                {
                    printf("--%s%c", temp->longName,((temp->instType==OPT_SET_VALUES)||(temp->instType==OPT_SET_LANGE)?'\0':((temp->instType==OPT_SET_VALUE)?'=':'\t')));
                }
            }

            if(temp->shortName)        printf("-%c\t", temp->shortName);
            //printf("cmd typd : %d", temp->instType);

            if(temp->argument) printf("arg : %d\t", temp->argument);
            if(temp->argumentNumber) printf("argNums : %d\t", temp->argumentNumber);

            if((temp->instType==OPT_SET_VALUE))    printf("[value]");

            if((temp->instType!=OPT_SET_VALUES)&&(temp->instType!=OPT_SET_LANGE))    printf("\n");

            if(temp->cmdAddedOption)
            {
                if(!passingString)
                {
                    if(!recursive)
                    {
                        passingString=(char *)malloc(sizeof(char)*(2+strlen(temp->longName)+1));
                        sprintf(passingString, "--%s", temp->longName);
                    }
                    else
                    {
                        passingString=(char *)malloc(sizeof(char)*(1+strlen(passedStrng)+strlen(temp->longName)+1));
                        sprintf(passingString, "%s-%s", passedStrng, temp->longName);
                    }
                }
                printInstList(temp->cmdAddedOption, recursive + 1, passingString);
            }

            if(passingString)
            {
                free(passingString);
                passingString = NULL;
            }
        }
    }

    if(passingString) free(passingString);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
enum INST_SET_FLAG_NONE_STATE
{
    NONE,
    OVER_WRITED
};
#define INST_SET_FLAG_NONE    0;
int instSetFlag(struct struct_cmdLineOption *p)
{
    enum INST_SET_FLAG_NONE_STATE processingFlag=NONE;

    if(p->value)
    {
        if(p->valueMask)
        {
            switch(p->valueType)
            {
                case VALUE_TYPE_UNSIGNED_CHAR:
                    if(p->instType == OPT_FLAG) (*((unsigned char*)p->value))|=(p->valueMask);
                    else if(p->instType == OPT_FLAG_CASE)
                    {
                        if((*((unsigned char*)p->value))) processingFlag = OVER_WRITED;
                        (*((unsigned char*)p->value))=(p->valueMask);
                    }
                    else (*((unsigned char*)p->value))|=(p->valueMask);
                    break;

                case VALUE_TYPE_UNSIGNED_INT:
                    if(p->instType == OPT_FLAG) (*((unsigned int*)p->value))|=(p->valueMask);
                    else if(p->instType == OPT_FLAG_CASE)
                    {
                        if((*((unsigned char*)p->value))) processingFlag = OVER_WRITED;
                        (*((unsigned int*)p->value))=(p->valueMask);
                    }
                    else (*((unsigned char*)p->value))|=(p->valueMask);
                    break;

                case VALUE_TYPE_UNSIGNED_LONG:
                    if(p->instType == OPT_FLAG) (*((unsigned long*)p->value))|=(p->valueMask);
                    else if(p->instType == OPT_FLAG_CASE)
                    {
                        if((*((unsigned char*)p->value))) processingFlag = OVER_WRITED;
                        (*((unsigned long*)p->value))|=(p->valueMask);
                    }
                    else (*((unsigned char*)p->value))|=(p->valueMask);
                    break;

                default:
                errorMes;
                printf("flagger is not signed number or float, double, void, none value type.\n");
                return -1;
            }

            if(processingFlag != NONE)
            {
                warningMesShort; printf("flag : flag \"%s(%c)\" is over-written.\n", (p->longName?p->longName:"[unknown]"), (p->shortName?p->shortName:'?'));
            }
            else
            {
                infoMes; printf("flag : \"%s(%c)\" is flagged.\n", (p->longName?p->longName:"[unknown]"), (p->shortName?p->shortName:'?'));
            }

            return 0;
        }
        else
        {
            errorMes;
            printf("Instruction %s(%c) not have a mask value of flag.\n", (p->longName?p->longName:"[unknown]"), (p->shortName?p->shortName:'?'));
        }
    }
    else if(p->instType == OPT_COMPONENT) return 0;

    errorMes;
    if(p->longName)
    {
        printf("Instruction \"%s\" not have a flag pointer.\n", p->longName);
    }
    else if(p->shortName)
    {
        printf("Instruction '%c' not have a flag pointer.\n", p->shortName);
    }
    else
    {
        printf("Instruction(unknown) not have a flag pointer.\n");
    }

    return -1;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//void *instructionCreateBufferValue(enum enum_list_valueType valueType)
//{
//    switch(valueType)
//    {
//        case VALUE_TYPE_CHAR:
//            return malloc(sizeof(char));
//        case VALUE_TYPE_UNSIGNED_CHAR:
//            return malloc(sizeof(unsigned char));
//
//        case VALUE_TYPE_INT:
//            return malloc(sizeof(int));
//        case VALUE_TYPE_UNSIGNED_INT:
//            return malloc(sizeof(unsigned int));
//
//        case VALUE_TYPE_LONG:
//            return malloc(sizeof(long));
//        case VALUE_TYPE_UNSIGNED_LONG:
//            return malloc(sizeof(unsigned long));
//
//        case VALUE_TYPE_FLOAT:
//            return malloc(sizeof(float));
//        case VALUE_TYPE_DOUBLE:
//            return malloc(sizeof(double));
//    }
//}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
char instSetValue(struct struct_cmdLineOption *p, char *valueString)
{
    unsigned int tmp;
    if(p->value)
    {
        switch(p->valueType)
        {
            case VALUE_TYPE_NONE:
                errorMes;
                printf("To input value, value type, which set to none , is wrong.\n");
                return -1;
                break;

            case VALUE_TYPE_VOID:
                errorMes;
                printf("To input value, value type, which set to void , is wrong.\n");
                return -1;
                break;

            case VALUE_TYPE_CHAR:
                warningMes;
                printf("VALUE_TYPE_CHAR not support yet.\n");
                (*((char*)p->value))=0;
                break;

            case VALUE_TYPE_UNSIGNED_CHAR:
                //warningMes;
                //printf("VALUE_TYPE_UNSIGNED_CHAR not support yet.\n");
                //(*((unsigned char*)p->value))=0;
                (*((unsigned char*)p->value))=convertStringToUnsignedChar(valueString);
                break;

            case VALUE_TYPE_INT:
                warningMes;
                printf("VALUE_TYPE_INT not support yet.\n");
                (*((int*)p->value))=0;
                break;

            case VALUE_TYPE_UNSIGNED_INT:
                (*((unsigned int*)p->value))=convertStringToUnsignedInt(valueString);
                break;

            case VALUE_TYPE_LONG:
                warningMes;
                printf("VALUE_TYPE_LONG not support yet.\n");
                (*((long*)p->value))=0;
                break;

            case VALUE_TYPE_UNSIGNED_LONG:
                warningMes;
                //printf("VALUE_TYPE_UNSIGNED_LONG not support yet.\n");
                (*((unsigned long*)p->value))=convertStringToUnsignedLong(valueString);
                break;

            case VALUE_TYPE_FLOAT:
                warningMes;
                printf("VALUE_TYPE_FLOAT not support yet.\n");
                (*((float*)p->value))=0;
                break;

            case VALUE_TYPE_FLOAT_RATIO:
                warningMes;
                printf("VALUE_TYPE_FLOAT not support yet.\n");
                (*((float*)p->value))=0;

                if( (*((float*)p->value)) < 0.0 )
                {
                    warningMes;
                    printf("Ratio value is smaller than zero.\r\n");
                }
                if( 1.0 < (*((float*)p->value)) )
                {
                    warningMes;
                    printf("Ratio value is bigger than zero.\r\n");
                }
                break;

            case VALUE_TYPE_FLOAT_RATIO_LIMIT:
                warningMes;
                printf("VALUE_TYPE_FLOAT not support yet.\n");
                (*((float*)p->value))=0;

                if( (*((float*)p->value)) < 0.0 )
                {
                    warningMes;
                    printf("Ratio value is smaller than zero.\r\n");
                    printf("%f -> %f",(*((float*)p->value)), (-1.0)*(*((float*)p->value)));
                    (*((float*)p->value)) = (-1.0)*(*((float*)p->value));
                }
                if( 1.0 < (*((float*)p->value)) )
                {
                    warningMes;
                    printf("Ratio value is bigger than zero.\r\n");
                    printf("%f -> %f",(*((float*)p->value)), 1.0);
                    (*((float*)p->value)) = 1.0f;
                }
                break;

            case VALUE_TYPE_DOUBLE:
                (*((double*)p->value))=convertStringToDouble(valueString);
                break;

            case VALUE_TYPE_DOUBLE_RATIO:
                (*((double*)p->value))=convertStringToDouble(valueString);
                if( (*((double*)p->value)) < 0.0 )
                {
                    warningMes;
                    printf("Ratio value is smaller than zero.\r\n");
                }
                if( 1.0 < (*((double*)p->value)) )
                {
                    warningMes;
                    printf("Ratio value is bigger than zero.\r\n");
                }
                break;

            case VALUE_TYPE_DOUBLE_RATIO_LIMIT:
                (*((double*)p->value))=convertStringToDouble(valueString);
                if( (*((double*)p->value)) < 0.0 )
                {
                    warningMes;
                    printf("Ratio value is smaller than zero.\r\n");
                    printf("%f -> %f",(*((double*)p->value)), (-1.0)*(*((double*)p->value)));
                    (*((double*)p->value)) = (-1.0)*(*((double*)p->value));
                }
                if( 1.0 < (*((double*)p->value)) )
                {
                    warningMes;
                    printf("Ratio value is bigger than zero.\r\n");
                    printf("%f -> %f",(*((double*)p->value)), 1.0);
                    (*((double*)p->value)) = 1.0f;
                }
                break;

            case VALUE_TYPE_STRING:
                tmp=strlen(valueString);
                (*((char**)p->value))=(char*)malloc(sizeof(char)*(tmp+1));
                memset((*((char**)p->value)), 0, (tmp+1));
                strcpy((*((char**)p->value)), valueString);
                break;

            default:
                errorMes;
                printf("flagger is not signed number or float, double, void, none value type.\n");
                return -1;
        }
        return 0;
    }
    else
    {
            errorMes;
            printf("Instruction %s(%c) not have a flag pointer.\n", (p->longName?p->longName:"[unknown]"), (p->shortName?p->shortName:'?'));
    }
    return -1;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
char *instSetValues(struct struct_cmdLineOption *p, char *str)
{
    char argBuf[51] = {0};
    char *arg = str;
    unsigned int argBufLength;
    struct struct_cmdLineOption *temp;

    for(temp = p; temp->instType != STRUCT_END; temp++)
    {
        if(temp->instType == OPT_SET_VALUES_SERIES)
        {
            /* abstract number from str */
            strncpy(argBuf, arg, (argBufLength=strcspn(arg, cmdOtionKeyWords)));
            arg+=argBufLength;

            (*(argBuf+argBufLength)) = 0;

            /* set value */
                                    //printf("\t\t\tinstSetValues) long name is \"%s\" argBuf = %s\n", (temp->longName?temp->longName:"[unknown long name]"), argBuf);
            if(instSetValue(temp, argBuf))
            {
                return str;
            }

            if(!(*arg))
            {
                if((temp+1)->instType != OPT_SET_VALUES_SERIES)
                {
                    errorMes;
                    printf("input values seriesly is wrong. To input, needs number of value and inserted values are not matched.\n");
                    return str;
                }
                else return arg;
            }

            if(*arg == '}')
            {
                arg++;
                break;
            }

            if((*arg)!=',')
            {
                return str;
            }
            else
            {
                if((temp+1)->instType != OPT_SET_VALUES_SERIES)
                {
                    errorMes;
                    printf("input values seriesly is wrong. To input, needs number of value and inserted values are not matched.\n");
                    return str;
                }
                arg++;
            }
        }
        else
        {
            break;
        }
    }

    return arg;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
char instrocessInstElementWhenDetected(struct struct_cmdLineOption *p)
{
struct struct_cmdLineOption *temp = p;

    switch(temp->instType)
    {
        case OPT_NOTHING:
                                                //printf("instType: OPT_NOTHING[%d], name: \"%s\"\n", temp->instType, temp->longName?temp->longName:"[unknown]");
            break;
        case OPT_FLAG:
        case OPT_FLAG_CASE:
            if(instSetFlag(temp))
            {
                errorMes;
                printf("\"%s\" instruction set wrong.\n", temp->longName?temp->longName:"[empty long name]");
                return -1;
            }
                                                //printf("instType: OPT_FLAG[%d], name: \"%s\"\n", temp->instType, temp->longName?temp->longName:"[unknown]");
            break;

        case OPT_DEBUG:
                                                //printf("instType: OPT_DEBUG[%d], name: \"%s\"\n", temp->instType, temp->longName?temp->longName:"[unknown]");
            break;

        case OPT_SET_VALUE:
                                                //printf("instType: OPT_SET_VALUE[%d], name: \"%s\"\n", temp->instType, temp->longName?temp->longName:"[unknown]");
            break;

        case OPT_SET_VALUES:
                                                //printf("instType: OPT_SET_VALUES[%d], name: \"%s\"\n", temp->instType, temp->longName?temp->longName:"[unknown]");
            break;
        case OPT_SET_VALUES_SERIES: //example) {-settingCategory=[value0],[value1],[value2],[value3]}
                                                //printf("instType: OPT_SET_VALUES_SERIES[%d], name: \"%s\"\n", temp->instType, temp->longName?temp->longName:"[unknown]");
            break;

        case OPT_SET_LANGE: //set lange, example) {-settingCategory=[start]:[step]:[end]}
                                                //printf("instType: OPT_SET_LANGE[%d], name: \"%s\"\n", temp->instType, temp->longName?temp->longName:"[unknown]");
            break;
        case OPT_SET_LANGE_SERIES:
                                                //printf("instType: OPT_SET_LANGE_SERIES[%d], name: \"%s\"\n", temp->instType, temp->longName?temp->longName:"[unknown]");
            break;

        case OPT_STRING:
                                                //printf("instType: OPT_STRING[%d], name: \"%s\"\n", temp->instType, temp->longName?temp->longName:"[unknown]");
            break;
        case OPT_COMPONENT:
            if(instSetFlag(temp))
            {
                errorMes;
                printf("\"%s\" instruction set wrong.\n", temp->longName?temp->longName:"[empty long name]");
                return -1;
            }
                                                //printf("instType: OPT_COMPONENT[%d], name: \"%s\"\n", temp->instType, temp->longName?temp->longName:"[unknown]");
            break;

        case STRUCT_END:
                                                //printf("instType: STRUCT_END[%d], name: \"%s\"\n", temp->instType, temp->longName?temp->longName:"[unknown]");
            break;

        default:
            errorMes;
            printf("instruction type is UNKNOWN.\n");
            return -1;
    }
    return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct struct_cmdLineOption *instFindSameLongNameInInstSet(struct struct_cmdLineOption *p, char *targetString)
{
    struct struct_cmdLineOption *temp;

    for(temp = p; temp->instType != STRUCT_END; temp++)
    {
        if(temp->longName)
        {
            if(!strcmp(temp->longName, targetString))
            {
                                            //printf("A option \"%s\" apply.\n", temp->longName);
                return temp;
            }
        }
    }

    return NULL;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
    If cmd options are wrong, return to input str address.
    Else cmd options process normally, return moved str address, which is same to char *arg.
*/
char *instProcessLongNameCmdOption(struct struct_cmdLineOption *p, char *str, char keyword)
{
    struct struct_cmdLineOption *temp;

    char *arg = str;
    char *argReturn = NULL;

    unsigned int argBufLength = 0;
    char argBuf[51] = {0};

/*
    if(*(arg)=='{')
    {
        printf("tie-up options detected.\n");
        arg++;
    }
     */

    while(*arg)
    {
        switch(*arg)
        {
            case '-'://find same option in struct_cmdLineOption Set
            {
                arg++;
                                                            //debugMes;    printf("\t\tbefore arg : \"%s\".",arg);

                strncpy(argBuf, arg, (argBufLength=strcspn(arg, cmdOtionKeyWords)));
                arg+=argBufLength;

                (*(argBuf+argBufLength)) = 0;

                                                            //printf("\t\targBuf : \"%s\"", argBuf);
                                                            //printf("\t\tafter arg : \"%s\".\n",arg);

                if(!(temp = instFindSameLongNameInInstSet(p, argBuf))) return str;
                /* Find same long name, do something start*/
                if(instrocessInstElementWhenDetected(temp)) return str;
                /* Find same long name, do something end*/

                switch(*arg)//enter sub struct_cmdLineOption Set
                {
                    //case '{':
                    //    arg++;
                    case '-':
                        if(temp->cmdAddedOption)
                        {
                            /* if processing sub job, need to update arg that is return value from instProcessLongNameCmdOption(...). */
                            if(arg == (argReturn = instProcessLongNameCmdOption(temp->cmdAddedOption, arg, 0))) return str;
                            arg = argReturn;
                            argReturn = NULL;
                        }

                    default:
                    break;
                }
                break;
            }

            case '{'://enter same struct_cmdLineOption Set
            {
                arg++;
                if(arg == (argReturn=instProcessLongNameCmdOption(temp->cmdAddedOption, arg, 0))) return str;
                arg = argReturn;
                argReturn = NULL;
                break;
            }

            case '}'://exit present struct_cmdLineOption Set
            {
                arg++;
                return arg;
                break;
            }

            case ','://arg ++ and then find others cmd options in present struct_cmdLineOption Set
            {
                                                            //printf("find combination word.\n");
                arg++;
                                                            //printf("\t\targ : \"%s\"\n", arg);
                break;
            }

            case ':'://arg ++ and then move next cmdLineOpton elements and then input value
            {
                            printf("set lange value resies.\n");
                break;
            }

            case '='://input value
            {
                arg++;
                switch(temp->instType)
                {
                    case OPT_SET_VALUE:
                    {
                        if(temp->value)
                        {
                            if(*arg == '-')
                            {
                                strncpy(argBuf, arg, (argBufLength=(strcspn(arg+1, cmdOtionKeyWords)+1)));
                            }
                            else
                            {
                                strncpy(argBuf, arg, (argBufLength=strcspn(arg, cmdOtionKeyWords)));
                            }

                            arg+=argBufLength;

                            (*(argBuf+argBufLength)) = 0;
                                                                        //printf("argBuf : '%s'\n", argBuf);
                                                                        //printf("arg : %s\r\n", arg);

                            /* set value */
                            if(instSetValue(temp, argBuf))
                            {
                                errorMes;
                                printf("input value is invalid.\n");
                                return str;
                            }
                        }
                    }
                    break;

                    case OPT_SET_VALUES:
                    {
                        if(arg == (argReturn = instSetValues(temp->cmdAddedOption, arg))) return str;
                        arg = argReturn;
                        argReturn = NULL;
                        /* call a set values function using sub set pointer */
                    }
                    break;

                    case OPT_SET_LANGE:
                    {
                        /* call a set lange value function using sub set pointer */
                    }
                    break;

                    case OPT_STRING:
                    {
                        if(temp->value)//if instruct set have any pointer to store value
                        {
                            strncpy(argBuf, arg, (argBufLength=strcspn(arg, cmdOtionKeyWords)));
                            arg+=argBufLength;

                            (*(argBuf+argBufLength)) = 0;
                                                                    //printf("\t\t\targBuf : '%s'\n", argBuf);
                            /* set value */
                            if(instSetValue(temp, argBuf))
                            {
                                errorMes;
                                printf("input value is invalid.\n");
                                return str;
                            }
                        }
                    }
                    break;

                    default:
                        return str;
                }
                                //printf("set value parts.\n");
                break;
            }

            default:
            {
                errorMes;
                printf("each long option have to one or more key word like \"%s\" at head.\n", cmdOtionKeyWords);
                return str;
            }
        }
    }

    return arg;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int instMainProcessing(struct struct_cmdLineOption *p, char *str)
{
    //unsigned int strLength=strlen(str);
    struct struct_cmdLineOption *temp = p;

    char *arg=str;
    char *argBuffer=NULL;

    if(*(arg+0)!='-')
    {
        return -1;
    }
    else if(*(arg+1)!='-')//single word
    {
        arg++;
        if(!(*(arg+1)))
        {
            for(temp = p; temp->instType != STRUCT_END; temp++)
            {
                if(temp->shortName == *arg)
                {
                    if(instSetFlag(temp))
                    {
                        errorMes;
                        printf("\"%s\" instruction set wrong.\n", temp->longName?temp->longName:"[empty long name]");
                        return -1;
                    }
                    return 0;
                }
            }
        }
    }
    else//string
    {
        arg++;
                                        //printf("\tinitial arg : \"%s\"\n", arg);
        if(arg != (argBuffer=instProcessLongNameCmdOption(p, arg, 0)))
        {
            infoMes;
            printf("\"%s\" apply.\n", str);
            return 0;
        }
                        //printf("arg : 0x%000000008x , argBuffer : 0x%000000008x\n", arg, argBuffer);
    }

    errorMes;
    printf("'%s' is unknown command option.\n", str);
    return -1;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int main(int argc, char **argv)
{
    /*Test variables*/
    unsigned int test_fileIO_errorCorrectingCount=0;
    /*Tempolary variables*/
    unsigned int i=0, tmp_i_start, tmp_i_end;
    unsigned int j=0;
    unsigned int k=0;
    unsigned int cmd_tmp_i=0;
    unsigned int cmd_tmp_cnt=0;

    struct_galoisField_info **main_com_listOf_GF=NULL;
    struct_galoisField_info *main_com_used_GF=NULL;

    char *main_com_genPoly=NULL;
    char *main_com_limitedGenerationPolynomial=NULL;
    unsigned int main_com_hd_correctability=0;
    unsigned int *main_indi_hd_correctability=NULL;

    unsigned int main_com_totalOrderOfLCM=0;
    struct_galoisFieldElements *main_com_expandEqOfLcmFromRoot=NULL;
    struct_galoisFieldElements *main_com_limitExpandEqOfLCM_fromRoot=NULL;
    struct_galoisFieldElements *main_com_addedLCM=NULL;

    struct_galoisFieldElements *main_com_finalEqOfrootOfLCM_andAddedLCM=NULL;

    unsigned int main_com_codeLength=0;//codeLength can be shorter than number of element in GF
    unsigned int main_com_infoBitLength=0;
    unsigned int main_com_parityBitsLength=0;
    double main_com_codeRate=0.0;

    struct_scheduling_FIFO *processingUseThisAlgorithm;

    unsigned int main_tmp_degErrLocPoly=0;
    unsigned int main_com_numsTP=0;
    unsigned int main_tmp_soft_i=0;

    uint32_t main_tmp_sel_decAlgo_cnt=0;
    uint32_t main_tmp_sel_decAlgo_i=0;
    unsigned int *main_indi_sel_TP_i=NULL;

    char **main_com_strBuf_TP_saved=NULL;
    struct_powerFormPolynomials **main_com_list_TP_pwrFormPoly=NULL;
    struct_powerFormPolynomials **main_indi_sel_decoding_CW_pwrFormPoly=NULL;

    unsigned int main_com_channel_errCnt = 0;
    unsigned int *main_indi_HD_errCnt = NULL;
    unsigned int *main_indi_sel_TP_errCnt = NULL;

    unsigned int *main_indi_list_errCntOf_TP_beforeDec = NULL;
    unsigned int **main_indi_list_errCntOf_TP_afterDec=NULL;
    // unsigned int main_list_err_cnt_after_dec_whole_TP_best=0;

    /* soft Decision correctable test pattern list */
    unsigned char **main_indi_list_flag_theo_ly_can_be_correctable = NULL;

    /* In actual decoding mode, to recognize which test-pattern can try correction, some flag needs */
    unsigned char **main_indi_list_flag_beforeDec_syndZero = NULL;
    unsigned char **main_indi_list_flag_after_dec_synd_zero = NULL;
    unsigned char **main_indi_list_flag_degOfErrLocPolyNotMax = NULL;
    unsigned char **main_indi_list_flag_metricChk_degErrLocPolySame = NULL;

    unsigned char **main_indi_list_flag_deccFail = NULL;
    unsigned char *main_indi_list_flag_fail_allDec = 0;


    /* algorithm loop count count */
    //9223372036854775807
    //100000000
    unsigned long main_com_limit_algorithm_loop_cnt = 100000000;
    unsigned long main_com_algorithm_loop_cnt = 0;

    /* error bits are over correctables */
    unsigned long *main_indi_errNumExceed_but_degErrLocPoly_not_max_cnt = NULL;
    unsigned long *main_indi_err_corection_fail_cnt = NULL;
    unsigned long *main_indi_errExceed_but_synd_zero_cnt = NULL;
    unsigned long *main_indi_errExceed_but_syndSum_zero_cnt = NULL;
    unsigned long *main_indi_err_detect_and_correct_fail_cnt = NULL;


    unsigned long *main_indi_total_err_cnt_HD = NULL;
    unsigned long *main_indi_total_err_cnt_CW = NULL;
    unsigned long main_com_total_err_cnt_CH = 0;
    unsigned long main_com_total_bit=0;


    double main_com_EbN0 = 0.0f;
    //double main_indi_BER = 0.0f;
    double *main_indi_HD_BER = NULL;
    double *main_indi_BER = NULL;
    double main_com_channel_BER = 0.0f;



    struct_encodingComponentInGF *main_encodingComponentInGF=NULL;

    //struct_HD_BM_algorithmComponent *main_indi_HD_decordComponents=NULL;
    //struct_HD_BM_algorithmComponent ***main_indi_SD_list_of_decordingComponents=NULL;
    /* decoding components list creating */
    struct_generalForm_algorithmComponent **main_indi_HD_decordComponents=NULL;
    struct_generalForm_algorithmComponent ****main_indi_SD_list_of_decordingComponents=NULL;

    struct_errorComponent *main_com_errComponents=NULL;
    struct_bpskComponents *main_com_bpskComponents=NULL;

    char *main_com_randInfoBitsStream=NULL;

    //unsigned int main_count=0;
    unsigned int main_com_errNums_inDecodingAlgorithmTest=0;
    unsigned long main_com_loopCount=0;
    unsigned long main_com_totalSamlingLoop=0;
    //unsigned int main_shiftedError=0;
//////////////////////////////////////////////////////////////////////////////////
//////////////////// Temporary file input output variables ///////////////////////
//////////////////////////////////////////////////////////////////////////////////
    char tmp_fileio_target_dir_path[1001] = {0};
    char tmp_fileio_target_file_path[1001] = {0};
    DIR*    tmp_dirio_var;

    FILE*   fileio_channelBER;
    FILE*   fileio_codingBER;

    FILE*   fileio_total_bit_cnt;
    FILE*   fileio_channel_err_bit_cnt;

    FILE**  fileio_indi_algorithm_coding_HD_BER;
    FILE**  fileio_indi_algorithm_codingBER;

    FILE**  fileio_indi_algorithm_coding_HD_err_bit_cnt;
    FILE**  fileio_indi_algorithm_coding_err_bit_cnt;

    FILE*   fileio_channelFER;
    FILE*   fileio_codingFER;

    FILE*   fildio_EbN0;
    FILE*   fildio_loop_Cnt;

    FILE*   fileio_simEnv;

    char *dirio_Name_home_path = NULL;// Get User home path
    char dirio_Name_simEnv_dir[] = "bch-sim_env";
    char dirio_Name_common_dir[] = "common";

    char dirio_Name_GF_info[] = "GaloisFields";

    char fileio_Name_GF_infoCommon[] = "GF";

    char fileio_Name_GaloisField_common[] = "GaloisField";
    char fileio_Name_Conjugate_common[] = "Conjugate";
    char fileio_Name_SumMaxrix_common[] = "SumMatrix";
    //char fileio_Name_GF_confugate[] = "conju";
    char fileio_Name_GF_infoBuf[256]={0};
    char fileio_Path_common[512]={0};
    char fileio_Path_Target[768]={0};
    char fileio_Path_TargetTmp[768]={0};
    struct dirent  *tmp_dir_entry;

    char fileio_Name_Output_RootPath[26]={0};
    char fileio_Name_Report_Path[128]={0};
    char fileio_Name_Pattern_Path[128]={0};
    char fileio_Name_PatternLog_Path[128]={0};

    char fileio_Name_channel_BER[513]={0};
    char fileio_Name_coding_BER[513]={0};

    char fileio_Name_total_bit_cnt[513]={0};
    char fileio_Name_channel_err_bit_cnt[513]={0};

    char fileio_Name_indi_algorithm_coding_HD_BER[KIND_OF_BCH_DECODING_END][513]={0};
    char fileio_Name_indi_algorithm_coding_BER[KIND_OF_BCH_DECODING_END][513]={0};

    char fileio_Name_indi_algorithm_coding_HD_err_bit_cnt[KIND_OF_BCH_DECODING_END][513]={0};
    char fileio_Name_indi_algorithm_coding_err_bit_cnt[KIND_OF_BCH_DECODING_END][513]={0};

    char fileio_Name_channel_FER[513]={0};
    char fileio_Name_coding_FER[513]={0};

    char fileio_Name_EbN0[513]={0};
    char fileio_Name_loop_Cnt[513]={0};

    char fileio_Name_simEnv[513]={0};

    /* pattern file io */
    FILE*    fileio_HD_mes;
    FILE*    fileio_HD_mes_log;
    FILE*    fileio_HD_mes_verilog;
    FILE*    fileio_HD_encoding_mes;
    FILE*    fileio_HD_encoding_mes_log;
    FILE*    fileio_HD_encoding_mes_verilog;
    FILE*    fileio_HD_received_mes;
    FILE*    fileio_HD_received_mes_log;
    FILE*    fileio_HD_received_mes_verilog;

    FILE*    fileio_SD_received_LLR;
    FILE*    fileio_SD_received_LLR_log;
    FILE*    fileio_SD_received_LLR_verilog;

    FILE*    fileio_SD_received_LLR_mag;
    FILE*    fileio_SD_received_LLR_mag_log;
    FILE*    fileio_SD_received_LLR_mag_verilog;

    FILE*    fileio_ErrCnt_log;

    /* hve to concern test patterns */
    FILE*    fileio_corrected_mes[KIND_OF_BCH_DECODING_END];
    FILE*    fileio_corrected_mes_log[KIND_OF_BCH_DECODING_END];
    FILE*    fileio_corrected_mes_verilog[KIND_OF_BCH_DECODING_END];

    char fileio_Name_HD_mes[513] = {0};
    char fileio_Name_HD_mes_log[513] = {0};
    char fileio_Name_HD_mes_verilog[513] = {0};
    char fileio_Name_HD_encoding_mes[513] = {0};
    char fileio_Name_HD_encoding_mes_log[513] = {0};
    char fileio_Name_HD_encoding_mes_verilog[513] = {0};
    char fileio_Name_HD_received_mes[513] = {0};
    char fileio_Name_HD_received_mes_log[513] = {0};
    char fileio_Name_HD_received_mes_verilog[513] = {0};

    char fileio_Name_SD_received_LLR[513] = {0};
    char fileio_Name_SD_received_LLR_log[513] = {0};
    char fileio_Name_SD_received_LLR_verilog[513] = {0};

    char fileio_Name_SD_received_LLR_mag[513] = {0};
    char fileio_Name_SD_received_LLR_mag_log[513] = {0};
    char fileio_Name_SD_received_LLR_mag_verilog[513] = {0};

    char fileio_Name_ErrCnt_log[513] = {0};

    /* have to concern test patterns */
    char fileio_Name_corrected_mes[KIND_OF_BCH_DECODING_END][513] = {0};
    char fileio_Name_corrected_mes_log[KIND_OF_BCH_DECODING_END][513] = {0};
    char fileio_Name_corrected_mes_verilog[KIND_OF_BCH_DECODING_END][513] = {0};

//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
    if(!(global_flag_Channel_Mode_Func&FLAG_MASK_CHANNEL_MODE_FUNC_RANDOM_SEED_DISABLE))
    {
        global_seed = (unsigned int)time(NULL)+(unsigned int)getpid();
        srand(global_seed);
    }
//////////////////////////////////////////////////////////////////////////////////
    fileio_indi_algorithm_coding_HD_BER = (FILE**)malloc(sizeof(FILE*)*KIND_OF_BCH_DECODING_END);
    memset(fileio_indi_algorithm_coding_HD_BER, 0, sizeof(FILE*)*KIND_OF_BCH_DECODING_END);

    fileio_indi_algorithm_codingBER = (FILE**)malloc(sizeof(FILE*)*KIND_OF_BCH_DECODING_END);
    memset(fileio_indi_algorithm_codingBER, 0, sizeof(FILE*)*KIND_OF_BCH_DECODING_END);

    fileio_indi_algorithm_coding_HD_err_bit_cnt = (FILE**)malloc(sizeof(FILE*)*KIND_OF_BCH_DECODING_END);
    memset(fileio_indi_algorithm_coding_HD_err_bit_cnt, 0, sizeof(FILE*)*KIND_OF_BCH_DECODING_END);

    fileio_indi_algorithm_coding_err_bit_cnt = (FILE**)malloc(sizeof(FILE*)*KIND_OF_BCH_DECODING_END);
    memset(fileio_indi_algorithm_coding_err_bit_cnt, 0, sizeof(FILE*)*KIND_OF_BCH_DECODING_END);
//////////////////////////////////////////////////////////////////////////////////
    /* To convert dataStream to powerFormPolynomial, define variable structure start */
    /*
    //struct_variableSetConvertBitStreamToPowerForm loadStream;
    //struct_variableSetConvertBitStreamToPowerForm noisedStream;
    //struct_variableSetConvertBitStreamToPowerForm unloadStream;
    */
    /* To convert dataStream to powerFormPolynomial, define variable structure end */
#ifndef RELEASE
    infoMes; printf("COMPILING DEBUG MODE\n");
#else
    infoMes; printf("COMPILING RELEASE MODE\n");
#endif

#ifndef USING_OPEN_MP
    infoMes; printf("PROGRAM USING SINGLE CORE\n");
#else
    infoMes; printf("PROGRAM USING MULTI CORE\n");
    infoMes; printf("USE) Thread nums in parallel region : %d\n", omp_get_num_threads());
    infoMes; printf("MAX) Thread nums in parallel region : %d\n", omp_get_max_threads());
    infoMes; printf("NUM) Multi-Core nums : %d\n", omp_get_num_procs());
    infoMes; printf("NUM) Multi-Core scheduling : Dynamic %s\n", omp_get_dynamic()?"Enabled":"Disabled");
#endif
    /******************************************************************/
    /***************** cmd arguments processing start *****************/
    /******************************************************************/
    for(i=1; i<argc; i++)
    {
        instMainProcessing(struct_cmdLineOption, *(argv+i));
    }
    if(global_flag_cmdSystemHold!=0)
    {
        if(global_flag_cmdSystemHold&FLAG_MASK_HOLD_HELP)
        {
            printInstList(struct_cmdLineOption, 0, NULL);
            return 0;
        }
    }
    /******************************************************************/
    /****************** cmd arguments processing end ******************/
    /******************************************************************/


/******************************************************************/
/******************************************************************/
/******************************************************************/
    gettimeofday(&time_program_init, NULL);
    date_simulation_date=localtime(&time_program_init.tv_sec);

    sprintf(fileio_Name_Output_RootPath, "%04d_%02d_%02d_%02d_%02d_%02d",
        date_simulation_date->tm_year+1900,
        date_simulation_date->tm_mon+1,
        date_simulation_date->tm_mday,
        date_simulation_date->tm_hour,
        date_simulation_date->tm_min,
        date_simulation_date->tm_sec
    );

    /* make directory(sim result info save only) start */
    tmp_dirio_var = opendir(fileio_Name_Output_RootPath);
    if(!tmp_dirio_var)
    {
        mkdir(fileio_Name_Output_RootPath, 0775);
        tmp_dirio_var = opendir(fileio_Name_Output_RootPath);
    }
    closedir(tmp_dirio_var);
    /* make directory(sim result info save only) end */
    /* make REPORT directory start */
    sprintf(fileio_Name_Report_Path, "%s/REPORT", fileio_Name_Output_RootPath);
    tmp_dirio_var = opendir(fileio_Name_Report_Path);
    if(!tmp_dirio_var)
    {
        mkdir(fileio_Name_Report_Path, 0775);
        tmp_dirio_var = opendir(fileio_Name_Report_Path);
    }
    closedir(tmp_dirio_var);
    /* make REPORT directory end */
    /* make PATTERN directory start */
    sprintf(fileio_Name_Pattern_Path, "%s/PATTERN", fileio_Name_Output_RootPath);
    tmp_dirio_var = opendir(fileio_Name_Pattern_Path);
    if(!tmp_dirio_var)
    {
        mkdir(fileio_Name_Pattern_Path, 0775);
        tmp_dirio_var = opendir(fileio_Name_Pattern_Path);
    }
    closedir(tmp_dirio_var);

    sprintf(fileio_Name_PatternLog_Path, "%s/PATTERN_LOG", fileio_Name_Output_RootPath);
    tmp_dirio_var = opendir(fileio_Name_PatternLog_Path);
    if(!tmp_dirio_var)
    {
        mkdir(fileio_Name_PatternLog_Path, 0775);
        tmp_dirio_var = opendir(fileio_Name_PatternLog_Path);
    }
    closedir(tmp_dirio_var);
    /* make PATTERN directory end */

    /* make directory common used start */
    /* get home directory */
    dirio_Name_home_path = getenv("HOME");
    /* dirio_Name_simEnv_root_path and dirio_Name_simEnv_dir */
    sprintf(fileio_Path_Target, "%s/%s", dirio_Name_home_path, dirio_Name_simEnv_dir);
    tmp_dirio_var = opendir((char*)fileio_Path_Target);
    if(!tmp_dirio_var)
    {
        if(mkdir((char*)fileio_Path_Target, 0775))
        {
            errorMes;
            printf("Can't make directory, path : %s\r\n", fileio_Path_Target);
            printf("error No : %d\r\n", errno);
            printf("EACCES : %d\r\nEEXIST : %d\r\nEMLINK : %d\r\nENOSPC : %d\r\nEROFS : %d\r\n",
                EACCES, EEXIST, EMLINK, ENOSPC, EROFS
            );
            return -1;
        }
        tmp_dirio_var = opendir((char*)fileio_Path_Target);
        if(!tmp_dirio_var)
        {
            errorMes;
            printf("Can't open directory, path : %s\r\n", fileio_Path_Target);
            printf("error No : %d\r\n", errno);
            return -1;
        }
    }
    closedir(tmp_dirio_var);

    /* dirio_Name_common_dir */
    strncpy(fileio_Path_TargetTmp, fileio_Path_Target, strlen(fileio_Path_Target));
    sprintf(fileio_Path_Target, "%s/%s", fileio_Path_TargetTmp, dirio_Name_common_dir);
    /* copy common path start */
    strcpy(fileio_Path_common, fileio_Path_Target);
    /* copy common path end */
    tmp_dirio_var = opendir((char*)fileio_Path_Target);
    if(!tmp_dirio_var)
    {
        mkdir((char*)fileio_Path_Target, 0775);
        tmp_dirio_var = opendir((char*)fileio_Path_Target);
        if(!tmp_dirio_var)
        {
            errorMes;
            printf("Can't make directory, path : %s\r\n", fileio_Path_Target);
            return -1;
        }
    }
    closedir(tmp_dirio_var);

    /* make directory common used end */
    /* make directory(galois fields info save only) start */
    strncpy(fileio_Path_TargetTmp, fileio_Path_Target, strlen(fileio_Path_Target));
    sprintf(fileio_Path_Target, "%s/%s", fileio_Path_TargetTmp, dirio_Name_GF_info);
    tmp_dirio_var = opendir((char*)fileio_Path_Target);
    if(!tmp_dirio_var)
    {
        mkdir((char*)fileio_Path_Target, 0775);
        tmp_dirio_var = opendir((char*)fileio_Path_Target);
        if(!tmp_dirio_var)
        {
            errorMes;
            printf("Can't make directory, path : %s\r\n", fileio_Path_Target);
            return -1;
        }
    }
    closedir(tmp_dirio_var);
    /* make directory(galois fields info save only) end */
    /******************************************************************/
    /******************************************************************/
    /******************************************************************/


    /********************************************************************/
    /********** setting variables about global variables start **********/
    /********************************************************************/
    /* Exception process */
    /* code length is can't exceed GF length. */
    if(!( ( global_MaximumCodeLength = ((unsigned int)pow(2.0,(double)global_GaloisFieldExponential)) ) > global_CodeLength))
    {
        global_MaximumCodeLength -=1;
        global_CodeLength = global_MaximumCodeLength;
        warningMes;
        printf("Code word length is exceed Galois field length.\n");
        warningMes;
        printf("It is set that %d, which is maximum code length in GF(%d).\n", global_GaloisFieldExponential, global_GaloisFieldExponential);
        if(global_MaximumCodeLength < global_LimitCodeLength)
        {
            global_LimitCodeLength = global_CodeLength;

            warningMesShort;
            printf("Code limit length is not exceed number of galois fields elements.\n");
        }
    }
    printf("\r\n");

    /*get BER process*/
    if(!global_EbN0_Step)
    {
        global_EbN0_Step=DEFAULT_VALUE_EBN0_STEP;
    }
    printf("/*****************************************************************/\r\n");
    printf("/*************** Simulation mode checking started ****************/\r\n");
    printf("/*****************************************************************/\r\n");
    GLOBAL_flag_proc();
    if(!global_flag_sim_decodingMode)
    {
        errorMes; printf("Any simulator Mode is not enabled.\r\n");
        errorMes; printf("Simulator is Closed.\r\n");
        return -1;
    }

    /* setting decoding enable flag end */
    /* BCH ALGORITHM TEST */
    /*
     * chk any flag of algorithm test.
     * if any flag is set, simulation will process decoding algorithm test sequence.
     */

    /*******************************************************/
    /*************** modulation mode setting ***************/
    /*******************************************************/
    if(!global_flag_Modulation_Mode)
    {
        global_flag_Modulation_Mode = FLAG_MASK_MODULATION_MODE_BPSK;
        infoMes; printf("Modulation Mode is not selected, Set to default(default is bpsk modulation).\r\n");
    }

    /***********************************************************/
    /* Getting BER method set to default(exception processing) */
    /***********************************************************/
    if(!global_flag_case_selTP_before_dec)
    {
        global_flag_case_selTP_before_dec = FLAG_CASE_SEL_TP_BEFORE_DEC_THEO;
        infoMes; printf("Before Decoding, selecting test pattern Method is not selected, So set to default(Theoretical).\r\n");
        infoMes; printf("Default mode is Theoretical mode.\r\n");
    }
    //else if(global_flag_case_selTP_before_dec == FLAG_CASE_SEL_TP_BEFORE_DEC_ACT)
    //{
    //    if(!(global_flag_sim_decodingMode&FLAG_MASK_CODE_BCH_DEC_AL_SOFT_DECISION))
    //    {
    //        global_flag_case_selTP_before_dec = FLAG_CASE_SEL_TP_BEFORE_DEC_THEO;
    //        warningMesShort; printf("ACTUAL MODE support only soft decision.\r\n");
    //        warningMesShort; printf("Getting BER mode forcely change to Theoretical mode.\r\n");
    //    }
    //}
    /***********************************************************/
    /* Getting BER method set to default(exception processing) */
    /***********************************************************/
    if(!global_flag_case_choosingCodeWordMethod_inTheo)
    {
        if(global_flag_case_selTP_before_dec == FLAG_CASE_SEL_TP_BEFORE_DEC_THEO)
        {
            global_flag_case_choosingCodeWordMethod_inTheo = FLAG_CASE_CODEWORD_CHOOSEN_METHOD_IN_THEO_NORMAL;
            infoMes; printf("Choosing correctable CodeWord Method is not selected.\r\n");
            infoMes; printf("And Before Decoding, selecting test pattern Method is theoretical.\r\n");
            infoMes; printf("So choosing CodeWord Method is sest to default(Theoretical).\r\n");
        }
        else
        {
            //not care if decoding fail method is not theoretical.
        }
    }
    else
    {
        if(global_flag_case_selTP_before_dec != FLAG_CASE_SEL_TP_BEFORE_DEC_THEO)
        {
            warningMesShort; printf("Choosing CodeWord Method is selected, but Decoding Fail conclusion Method is not theoretical.\r\n");
            warningMesShort; printf("Selecting CodeWord Method is not vailid.\r\n");
        }
    }

    if(!global_addedRootToLCM_string)
    {
        global_addedRootToLCM_string=(char*)malloc(sizeof(char)*2);
        memset(global_addedRootToLCM_string, 0, sizeof(char)*2);
        global_addedRootToLCM_string[0]='1';
    }
    /********************************************************************/
    /*********** setting variables about global variables end ***********/
    /********************************************************************/

    /********************************************************************/
    /**** printf setted global variables about global variables start ****/
    /********************************************************************/
    printf("/*****************************************************************/\n");
    printf("/*********************** Enabled Code List ***********************/\n");
    printf("/*****************************************************************/\n");
    for(i=0; i<sizeof(global_flag_simulationCode)*8; i++)
    {
        if(global_flag_simulationCode)
        {
            switch(global_flag_simulationCode&(1<<i))
            {
                case FLAG_MASK_CODE_BCH:
                    infoMes; printf("Enabled BCH codes.\n");
                    break;
                default:
                    break;
            }
        }
        else
        {
            errorMes; printf("Disabled all codes.\n");
            break;
        }
    }

    printf("/*****************************************************************/\n");
    printf("/**************** Applied global variables values ****************/\n");
    printf("/*****************************************************************/\n");
    infoMes; printf("[Global Variables]\n");
    infoMes; printf("AWGN Coderate is %s\n", ((global_flag_Channel_Mode_Func&FLAG_MASK_CHANNEL_MODE_FUNC_BITRATE_DISABLE)?"disabled":"enabled"));

    infoMes; printf("EbN0 start : %g\n",global_EbN0_Start_Value);
    infoMes; printf("EbN0 end : %g\n",global_EbN0_End_Value);
    infoMes; printf("EbN0 step : %g\n",global_EbN0_Step);

    infoMes; printf("Error number : %d\n",global_ErrorNum);
    infoMes; printf("Galois Field exponential : %d\n",global_GaloisFieldExponential);
    infoMes; printf("Code length : %d\n",global_CodeLength);
    infoMes; printf("Maximum Code length : %d\n",global_MaximumCodeLength);
    infoMes; printf("Limit Code length : %d%s\n",global_LimitCodeLength, global_LimitCodeLength?"(Limited)":"(Unlimited)");

    infoMes; printf("Hard Decision correctable(t) : %d\n",global_bch_hardCorrectable);
    infoMes; printf("Soft Decision correctable(p) : %d\n",global_bch_SoftCorrectable);
    infoMes; printf("Over Syndrome number : %d\n", global_bch_Soft_OverSyndLen);
    infoMes; printf("Added Root to Least Common Multiple : %s\n", global_addedRootToLCM_string);
    printf("\n");

    printf("/*****************************************************************/\n");
    printf("/*********** (Global Flag)Applied Modulation Mode List ***********/\n");
    printf("/*****************************************************************/\n");
    for(i=0; i<sizeof(global_flag_Modulation_Mode)*8; i++)
    {
        if(global_flag_Modulation_Mode)
        {
            switch(global_flag_Modulation_Mode&(1<<i))
            {
                case (1<<0):
                    infoMes; printf("Applied BPSK Modulation.\n");
                    break;
                default:
                    break;
            }
        }
        else
        {
            warningMes; printf("Unknown Modulation.\n");
            break;
        }
    }

    printf("/*****************************************************************/\n");
    printf("/********* (Global Flag)Applied getting BER method Modes **********/\n");
    printf("/*****************************************************************/\n");
    cmd_tmp_cnt=0;
    for(cmd_tmp_i=0; cmd_tmp_i<KIND_OF_BCH_DECODING_END; cmd_tmp_i++)
    {
        if(global_flag_bchDecAlgo_Enable&uint32_bitMask[cmd_tmp_i])
        {
            if(!(global_list_flag_bch_sim_decodingMode[cmd_tmp_i]&FLAG_MASK_CODE_BCH_DEC_AL_SOFT_DECISION))//hard-decision
            {
                if(global_flag_case_choosingCodeWordMethod_inTheo!=FLAG_CASE_SEL_TP_BEFORE_DEC_THEO)
                {
                    infoMes; printf("[%s] #%d getting BER method is only meanful in soft-decision.\n",
                    KIND_OF_BCH_DECODING_ALGORITHM_NAME[cmd_tmp_i], global_flag_case_choosingCodeWordMethod_inTheo);
                }
                else
                {
                    infoMes; printf("[%s] #%d getting BER method is hard-decision kernel.\n",
                    KIND_OF_BCH_DECODING_ALGORITHM_NAME[cmd_tmp_i], global_flag_case_choosingCodeWordMethod_inTheo);
                }
            }
            else//soft-decision
            {
                switch(global_flag_case_choosingCodeWordMethod_inTheo)
                {
                    default:
                        cmd_tmp_cnt++;
                        warningMes; printf("[%s] #%d Method of gettig BER is not selected, So it will set to default.\n",
                        KIND_OF_BCH_DECODING_ALGORITHM_NAME[cmd_tmp_i], global_flag_case_choosingCodeWordMethod_inTheo);
                    break;

                    case FLAG_CASE_CODEWORD_CHOOSEN_METHOD_IN_THEO_NO_ERR:
                        infoMes; printf("[%s] #%d Applied Getting BER method to \"basic.\"\n",
                        KIND_OF_BCH_DECODING_ALGORITHM_NAME[cmd_tmp_i], global_flag_case_choosingCodeWordMethod_inTheo);
                    break;

                    case FLAG_CASE_CODEWORD_CHOOSEN_METHOD_IN_THEO_NORMAL:
                        infoMes; printf("[%s] #%d Applied Getting BER method to \"normal.\"\n",
                        KIND_OF_BCH_DECODING_ALGORITHM_NAME[cmd_tmp_i], global_flag_case_choosingCodeWordMethod_inTheo);
                    break;

                    case FLAG_CASE_CODEWORD_CHOOSEN_METHOD_IN_THEO_MIN_ERR:
                        infoMes; printf("[%s] #%d Applied Getting BER method to \"best.\"\n",
                        KIND_OF_BCH_DECODING_ALGORITHM_NAME[cmd_tmp_i], global_flag_case_choosingCodeWordMethod_inTheo);
                    break;

                    case FLAG_CASE_CODEWORD_CHOOSEN_METHOD_IN_THEO_MAX_ERR:
                        infoMes; printf("[%s] #%d Applied Getting BER method to \"worst.\"\n",
                        KIND_OF_BCH_DECODING_ALGORITHM_NAME[cmd_tmp_i], global_flag_case_choosingCodeWordMethod_inTheo);
                    break;
                }
            }
        }
    }
    if(cmd_tmp_cnt)
    {
        warningMes; printf("#%d Method of gettig BER is not selected, So set to default.\n", global_flag_case_choosingCodeWordMethod_inTheo);
        global_flag_case_choosingCodeWordMethod_inTheo = FLAG_CASE_CODEWORD_CHOOSEN_METHOD_IN_THEO_DEFAULT;
    }
    /********************************************************************/
    /***** printf setted global variables about global variables end *****/
    /********************************************************************/

    /********************************************************************/
    /************** initializing domestic variables start ***************/
    /********************************************************************/
    /****************************************************/
    /********** Channel variable setting start **********/
    /****************************************************/

    /* soft Decision variable start */
    /* Default value of main_tmp_degErrLocPoly is 2. */
    //if(!(global_flag_bchDecAlgo_Enable&FLAG_MASK_CODE_BCH_DEC_AL_SOFT_DECISION))
    cmd_tmp_cnt=0;
    for(cmd_tmp_i=0; cmd_tmp_i<KIND_OF_BCH_DECODING_END; cmd_tmp_i++)
    {
        if(global_list_flag_bch_sim_decodingMode[cmd_tmp_i]&FLAG_MASK_CODE_BCH_DEC_AL_SOFT_DECISION) cmd_tmp_cnt++;
    }
    if(cmd_tmp_cnt)
    {
        if(!global_bch_SoftCorrectable)
        {
            main_tmp_degErrLocPoly = DEFAULT_VALUE_BCH_SOFT_CORRECTABLE;
        }
        else
        {
            main_tmp_degErrLocPoly = global_bch_SoftCorrectable;
        }
    }
    main_com_numsTP = (unsigned int)pow(2.0,(double)main_tmp_degErrLocPoly);
    /* soft Decision variable end */


    /* soft Decision Channel Counting variable start */
    main_indi_list_errCntOf_TP_beforeDec = (unsigned int*)malloc(sizeof(unsigned int)*main_com_numsTP);
    memset(main_indi_list_errCntOf_TP_beforeDec, 0, sizeof(unsigned int)*main_com_numsTP);

    main_indi_list_errCntOf_TP_afterDec = (unsigned int**)malloc(sizeof(unsigned int*)*KIND_OF_BCH_DECODING_END);
    for(main_tmp_sel_decAlgo_i=0; main_tmp_sel_decAlgo_i<KIND_OF_BCH_DECODING_END; main_tmp_sel_decAlgo_i++)
    {
        main_indi_list_errCntOf_TP_afterDec[main_tmp_sel_decAlgo_i] = (unsigned int*)malloc(sizeof(unsigned int)*main_com_numsTP);
        memset(main_indi_list_errCntOf_TP_afterDec[main_tmp_sel_decAlgo_i], 0, sizeof(unsigned int)*main_com_numsTP);
    }

    /* setting up scheduler start*/
    processingUseThisAlgorithm = (struct_scheduling_FIFO*)malloc(sizeof(struct_scheduling_FIFO));
    memset(processingUseThisAlgorithm, 0, sizeof(struct_scheduling_FIFO));
    /* setting up scheduler start*/

    main_indi_hd_correctability=(unsigned int*)malloc(sizeof(unsigned int)*KIND_OF_BCH_DECODING_END);
    memset(main_indi_hd_correctability, 0, sizeof(unsigned int)*KIND_OF_BCH_DECODING_END);

    //main_list_err_cnt_after_dec_whole_TP_best=-1;

    //main_indi_sel_TP_errCnt=-1;
    /* Belong to dec algorithm HD kernel individually have main_indi_sel_TP_i */
    main_indi_sel_TP_i = (unsigned int*)malloc(sizeof(unsigned int)*KIND_OF_BCH_DECODING_END);
    memset(main_indi_sel_TP_i, 0, sizeof(unsigned int)*KIND_OF_BCH_DECODING_END);
    //*soft Decision Channel Counting variable end*/

    /* soft Decision correctable test pattern list start */
    main_indi_list_flag_theo_ly_can_be_correctable = (unsigned char**)malloc(sizeof(unsigned char*)*KIND_OF_BCH_DECODING_END);
    for(main_tmp_sel_decAlgo_i=0; main_tmp_sel_decAlgo_i<KIND_OF_BCH_DECODING_END; main_tmp_sel_decAlgo_i++)
    {
        main_indi_list_flag_theo_ly_can_be_correctable[main_tmp_sel_decAlgo_i] = (unsigned char*)malloc(sizeof(unsigned char)*main_com_numsTP);
        memset(main_indi_list_flag_theo_ly_can_be_correctable[main_tmp_sel_decAlgo_i], 0, sizeof(unsigned char)*main_com_numsTP);
                //printf("main_indi_list_flag_theo_ly_can_be_correctable[%d] 0x%x\n", main_tmp_sel_decAlgo_i, main_indi_list_flag_theo_ly_can_be_correctable[main_tmp_sel_decAlgo_i]);
    }
    /* soft Decision correctable test pattern list end */

    /* soft Decision in actual mode, after dec synd zero flag start */
    main_indi_list_flag_beforeDec_syndZero = (unsigned char**)malloc(sizeof(unsigned char*)*KIND_OF_BCH_DECODING_END);
    for(main_tmp_sel_decAlgo_i=0; main_tmp_sel_decAlgo_i<KIND_OF_BCH_DECODING_END; main_tmp_sel_decAlgo_i++)
    {
        main_indi_list_flag_beforeDec_syndZero[main_tmp_sel_decAlgo_i] = (unsigned char*)malloc(sizeof(unsigned char)*main_com_numsTP);
        memset(main_indi_list_flag_beforeDec_syndZero[main_tmp_sel_decAlgo_i], 0, sizeof(unsigned char)*main_com_numsTP);
    }

    main_indi_list_flag_after_dec_synd_zero = (unsigned char**)malloc(sizeof(unsigned char*)*KIND_OF_BCH_DECODING_END);
    for(main_tmp_sel_decAlgo_i=0; main_tmp_sel_decAlgo_i<KIND_OF_BCH_DECODING_END; main_tmp_sel_decAlgo_i++)
    {
        main_indi_list_flag_after_dec_synd_zero[main_tmp_sel_decAlgo_i] = (unsigned char*)malloc(sizeof(unsigned char)*main_com_numsTP);
        memset(main_indi_list_flag_after_dec_synd_zero[main_tmp_sel_decAlgo_i], 0, sizeof(unsigned char)*main_com_numsTP);
    }


    main_indi_list_flag_degOfErrLocPolyNotMax = (unsigned char**)malloc(sizeof(unsigned char*)*KIND_OF_BCH_DECODING_END);
    for(main_tmp_sel_decAlgo_i=0; main_tmp_sel_decAlgo_i<KIND_OF_BCH_DECODING_END; main_tmp_sel_decAlgo_i++)
    {
        main_indi_list_flag_degOfErrLocPolyNotMax[main_tmp_sel_decAlgo_i] = (unsigned char*)malloc(sizeof(unsigned char)*main_com_numsTP);
        memset(main_indi_list_flag_degOfErrLocPolyNotMax[main_tmp_sel_decAlgo_i], 0, sizeof(unsigned char)*main_com_numsTP);
    }

    main_indi_list_flag_metricChk_degErrLocPolySame = (unsigned char**)malloc(sizeof(unsigned char*)*KIND_OF_BCH_DECODING_END);
    for(main_tmp_sel_decAlgo_i=0; main_tmp_sel_decAlgo_i<KIND_OF_BCH_DECODING_END; main_tmp_sel_decAlgo_i++)
    {
        main_indi_list_flag_metricChk_degErrLocPolySame[main_tmp_sel_decAlgo_i] = (unsigned char*)malloc(sizeof(unsigned char)*main_com_numsTP);
        memset(main_indi_list_flag_metricChk_degErrLocPolySame[main_tmp_sel_decAlgo_i], 0, sizeof(unsigned char)*main_com_numsTP);
    }
    /* soft Decision in actual mode, after dec synd zero flag end */

    /* soft Decision in actual mode, fail flag start */
    main_indi_list_flag_deccFail = (unsigned char**)malloc(sizeof(unsigned char*)*KIND_OF_BCH_DECODING_END);
    for(main_tmp_sel_decAlgo_i=0; main_tmp_sel_decAlgo_i<KIND_OF_BCH_DECODING_END; main_tmp_sel_decAlgo_i++)
    {
        main_indi_list_flag_deccFail[main_tmp_sel_decAlgo_i] = (unsigned char*)malloc(sizeof(unsigned char)*main_com_numsTP);
        memset(main_indi_list_flag_deccFail[main_tmp_sel_decAlgo_i], 0, sizeof(unsigned char)*main_com_numsTP);
    }

    main_indi_list_flag_fail_allDec = (unsigned char*)malloc(sizeof(unsigned char)*KIND_OF_BCH_DECODING_END);
    memset(main_indi_list_flag_fail_allDec, 0, sizeof(unsigned char)*KIND_OF_BCH_DECODING_END);
    /* soft Decision in actual mode, fail flag end */


    /* belong to algorithm, variables of BER initializing start */
    main_indi_HD_BER = (double*)malloc(sizeof(double)*KIND_OF_BCH_DECODING_END);
    memset(main_indi_HD_BER, 0, sizeof(double)*KIND_OF_BCH_DECODING_END);

    main_indi_BER = (double*)malloc(sizeof(double)*KIND_OF_BCH_DECODING_END);
    memset(main_indi_BER, 0, sizeof(double)*KIND_OF_BCH_DECODING_END);
    /* belong to algorithm, variables of BER initializing end */


    /* variables associated with error bits are over correctables set start */
    main_indi_errNumExceed_but_degErrLocPoly_not_max_cnt = (unsigned long*)malloc(sizeof(unsigned long)*KIND_OF_BCH_DECODING_END);
    memset(main_indi_errNumExceed_but_degErrLocPoly_not_max_cnt, 0, sizeof(unsigned long)*KIND_OF_BCH_DECODING_END);

    main_indi_err_corection_fail_cnt = (unsigned long*)malloc(sizeof(unsigned long)*KIND_OF_BCH_DECODING_END);
    memset(main_indi_err_corection_fail_cnt, 0, sizeof(unsigned long)*KIND_OF_BCH_DECODING_END);

    main_indi_errExceed_but_synd_zero_cnt = (unsigned long*)malloc(sizeof(unsigned long)*KIND_OF_BCH_DECODING_END);
    memset(main_indi_errExceed_but_synd_zero_cnt, 0, sizeof(unsigned long)*KIND_OF_BCH_DECODING_END);

    main_indi_errExceed_but_syndSum_zero_cnt = (unsigned long*)malloc(sizeof(unsigned long)*KIND_OF_BCH_DECODING_END);
    memset(main_indi_errExceed_but_syndSum_zero_cnt, 0, sizeof(unsigned long)*KIND_OF_BCH_DECODING_END);

    main_indi_err_detect_and_correct_fail_cnt = (unsigned long*)malloc(sizeof(unsigned long)*KIND_OF_BCH_DECODING_END);
    memset(main_indi_err_detect_and_correct_fail_cnt, 0, sizeof(unsigned long)*KIND_OF_BCH_DECODING_END);
    /* variables associated with error bits are over correctables set end */


    /* to calculate ber, variables setting start */
    main_indi_total_err_cnt_HD = (unsigned long*)malloc(sizeof(unsigned long)*KIND_OF_BCH_DECODING_END);
    memset(main_indi_total_err_cnt_HD, 0, sizeof(unsigned long)*KIND_OF_BCH_DECODING_END);

    main_indi_total_err_cnt_CW = (unsigned long*)malloc(sizeof(unsigned long)*KIND_OF_BCH_DECODING_END);
    memset(main_indi_total_err_cnt_CW, 0, sizeof(unsigned long)*KIND_OF_BCH_DECODING_END);
    /* to calculate ber, variables setting end */


    /* decoding components list creating1 start */
    main_indi_HD_decordComponents = (struct_generalForm_algorithmComponent**)malloc(sizeof(struct_generalForm_algorithmComponent*)*KIND_OF_BCH_DECODING_END);
    memset(main_indi_HD_decordComponents, 0, sizeof(struct_generalForm_algorithmComponent*)*KIND_OF_BCH_DECODING_END);

    main_indi_SD_list_of_decordingComponents = (struct_generalForm_algorithmComponent****)malloc(sizeof(struct_generalForm_algorithmComponent***)*KIND_OF_BCH_DECODING_END);
    memset(main_indi_SD_list_of_decordingComponents, 0, sizeof(struct_generalForm_algorithmComponent***)*KIND_OF_BCH_DECODING_END);
    /* decoding components list creating1 end */

    //EBN0 start value
    if(global_EbN0_Start_Value)
    {
        main_com_EbN0=global_EbN0_Start_Value;

        infoMes;
        printf("setted start value of main_com_EbN0 is '%f'.\n", global_EbN0_Start_Value);
    }
    else
    {
        global_EbN0_Start_Value=DEFAULT_VALUE_EBN0;
        main_com_EbN0=DEFAULT_VALUE_EBN0;

        infoMes;
        printf("setted default start value of main_com_EbN0 is '%f'.\n", global_EbN0_Start_Value);
    }

    //EBN0 start value
    if(global_EbN0_End_Value)
    {
        main_com_EbN0=global_EbN0_End_Value;

        infoMes;
        printf("setted end value of main_com_EbN0 is '%f'.\n", global_EbN0_End_Value);
    }
    else
    {
        global_EbN0_End_Value=DEFAULT_VALUE_EBN0_END;
        main_com_EbN0=DEFAULT_VALUE_EBN0_END;

        infoMes;
        printf("setted default end value of main_com_EbN0 is '%f'.\n", global_EbN0_End_Value);
    }
    /* Channel variable end */


    /* Soft Decision Test Pattern Decording Coponents. If soft decision correctable bits is p,  number of test petern is (2^p) - 1. */
                                    #ifndef RELEASE
                                    if(global_flag_debug_display&FLAG_MASK_DISPLAY_FUNCNAME)
                                    {
                                        debugFuncNameMes;
                                        printf("char **main_com_strBuf_TP_saved=(char**)malloc(sizeof(char*)*main_com_numsTP);\n");
                                        debugFuncNameMes;
                                        printf("char **main_com_list_TP_pwrFormPoly=(struct_powerFormPolynomials**)malloc(sizeof(struct_powerFormPolynomials*)*main_com_numsTP);\n");
                                    }
                                    #endif
    main_com_strBuf_TP_saved=(char**)malloc(sizeof(char*)*main_com_numsTP);
    memset(main_com_strBuf_TP_saved, 0, sizeof(char*)*main_com_numsTP);

    main_com_list_TP_pwrFormPoly=(struct_powerFormPolynomials**)malloc(sizeof(struct_powerFormPolynomials*)*main_com_numsTP);
    memset(main_com_list_TP_pwrFormPoly, 0, sizeof(struct_powerFormPolynomials*)*main_com_numsTP);

    main_indi_sel_decoding_CW_pwrFormPoly=(struct_powerFormPolynomials**)malloc(sizeof(struct_powerFormPolynomials*)*KIND_OF_BCH_DECODING_END);
    memset(main_indi_sel_decoding_CW_pwrFormPoly, 0, sizeof(struct_powerFormPolynomials*)*KIND_OF_BCH_DECODING_END);


                                    #ifndef RELEASE
                                    if(global_flag_debug_display&FLAG_MASK_DISPLAY_FUNCNAME)
                                    {
                                        debugFuncNameMes;
                                        printf("memset(main_com_strBuf_TP_saved, 0, sizeof(char*)*main_com_numsTP);\n");
                                        debugFuncNameMes;
                                        printf("memset(main_com_list_TP_pwrFormPoly, 0, sizeof(struct_powerFormPolynomials*)*main_com_numsTP);\n");
                                    }
                                    #endif


    main_indi_HD_errCnt=(unsigned int*)malloc(sizeof(unsigned int)*KIND_OF_BCH_DECODING_END);
    memset(main_indi_HD_errCnt, -1, sizeof(unsigned int)*KIND_OF_BCH_DECODING_END);

    main_indi_sel_TP_errCnt=(unsigned int*)malloc(sizeof(unsigned int)*KIND_OF_BCH_DECODING_END);
    memset(main_indi_sel_TP_errCnt, -1, sizeof(unsigned int)*KIND_OF_BCH_DECODING_END);


    /* decoding components list creating2 start */
    for(main_tmp_sel_decAlgo_i=0; main_tmp_sel_decAlgo_i<KIND_OF_BCH_DECODING_END; main_tmp_sel_decAlgo_i++)
    {
                                        #ifndef RELEASE
                                        if(global_flag_debug_display&FLAG_MASK_DISPLAY_FUNCNAME)
                                        {
                                            debugFuncNameMes;
                                            printf("struct_HD_BM_algorithmComponent ***main_indi_SD_list_of_decordingComponents[main_tmp_sel_decAlgo_i]=(struct_HD_BM_algorithmComponent***)malloc(sizeof(struct_HD_BM_algorithmComponent**)*main_com_numsTP);\n");
                                        }
                                        #endif
        main_indi_SD_list_of_decordingComponents[main_tmp_sel_decAlgo_i]=(struct_generalForm_algorithmComponent***)malloc(sizeof(struct_generalForm_algorithmComponent**)*main_com_numsTP);
                                        #ifndef RELEASE
                                        if(global_flag_debug_display&FLAG_MASK_DISPLAY_FUNCNAME)
                                        {
                                            debugFuncNameMes;
                                            printf("*(main_indi_SD_list_of_decordingComponents[main_tmp_sel_decAlgo_i]+0)=(&main_indi_HD_decordComponents[main_tmp_sel_decAlgo_i]);\n");
                                        }
                                        #endif
        *(main_indi_SD_list_of_decordingComponents[main_tmp_sel_decAlgo_i]+0)=(&main_indi_HD_decordComponents[main_tmp_sel_decAlgo_i]);
                                        #ifndef RELEASE
                                        if(global_flag_debug_display&FLAG_MASK_DISPLAY_FUNCNAME)
                                        {
                                            debugFuncNameMes;
                                            printf("for(i=1; i<main_com_numsTP; i++)\n");
                                        }
                                        #endif
        for(i=1; i<main_com_numsTP; i++)
        {
            *(main_indi_SD_list_of_decordingComponents[main_tmp_sel_decAlgo_i]+i)=(struct_generalForm_algorithmComponent**)malloc(sizeof(struct_generalForm_algorithmComponent*));
            **(main_indi_SD_list_of_decordingComponents[main_tmp_sel_decAlgo_i]+i)=NULL;
        }
        //*(main_indi_SD_list_of_decordingComponents[main_tmp_sel_decAlgo_i]+1)=(&decordingTestPeternComponents1);
        //*(main_indi_SD_list_of_decordingComponents[main_tmp_sel_decAlgo_i]+2)=(&decordingTestPeternComponents2);
        //*(main_indi_SD_list_of_decordingComponents[main_tmp_sel_decAlgo_i]+3)=(&decordingTestPeternComponents3);
    }
    /* decoding components list creating2 end */

    /*Soft Decision Test Pattern Decording Coponents. end*/


    /******************************************************************/
    /******************** memory initializing start *******************/
    /******************************************************************/
    /*
    //memset(&loadStream, 0, sizeof(struct_variableSetConvertBitStreamToPowerForm));
    //memset(&noisedStream, 0, sizeof(struct_variableSetConvertBitStreamToPowerForm));
    //memset(&unloadStream, 0, sizeof(struct_variableSetConvertBitStreamToPowerForm));
    */
    /******************************************************************/
    /********************* memory initializing end ********************/
    /******************************************************************/
    /**************************************************************************/
    /****************** initializing domestic variables end *******************/
    /**************************************************************************/


    /********************************************************************/
    /********* setting variables about domestic variables start *********/
    /********************************************************************/
    printf("/*****************************************************************/\r\n");
    printf("/***************** Assiciate with galois fields ******************/\r\n");
    printf("/*****************************************************************/\r\n");
    /* Initializing essential conditions start */

    /* To generate galois fields, initializing primitive polynomials start */
    initPrimitivePolynomial(&PRIMITIVE_POLYNOMIAL_SET);
    initPrimitivePolynomial_inversed(&PRIMITIVE_POLYNOMIAL_SET_INVERSED, &PRIMITIVE_POLYNOMIAL_SET);
    initMes; printf("initialized primitive polynomials is done.\r\n");


    /* printPowerFormWithEnter((char_POWER_FORM*)PRIMITIVE_POLYNOMIAL_2048_INVERSE_INDEX); */
    if(global_flag_gfCommon_display&FLAG_MASK_DISPLAY_GF_COMMON_GENPOLY)
    {
        infoMes; printf("power formed primitive polynomial\n");
        for(i=0; i<PRIMITIVE_POLYNOMIAL_SET.PRIMITIVE_POLYNOMIAL_NUMBER; i++)
        {
            printPowerFormWithEnter((char_POWER_FORM*)PRIMITIVE_POLYNOMIAL_SET.PRIMITIVE_POLYNOMIAL[i]);
        }

        infoMes; printf("inversed power formed primitive polynomial\n");
        for(i=0; i<PRIMITIVE_POLYNOMIAL_SET_INVERSED.PRIMITIVE_POLYNOMIAL_NUMBER; i++)
        {
            printPowerFormWithEnter((char_POWER_FORM*)PRIMITIVE_POLYNOMIAL_SET_INVERSED.PRIMITIVE_POLYNOMIAL[i]);
        }
    }

            /* printf initializing result */
            /*
            //printPowerFormWithEnter((char_POWER_FORM*)PRIMITIVE_POLYNOMIAL_4_INVERSE_INDEX);
            //printPowerFormWithEnter((char_POWER_FORM*)PRIMITIVE_POLYNOMIAL_8_INVERSE_INDEX);
            //printPowerFormWithEnter((char_POWER_FORM*)PRIMITIVE_POLYNOMIAL_16_INVERSE_INDEX);
            //printPowerFormWithEnter((char_POWER_FORM*)PRIMITIVE_POLYNOMIAL_32_INVERSE_INDEX);
            //printPowerFormWithEnter((char_POWER_FORM*)PRIMITIVE_POLYNOMIAL_64_INVERSE_INDEX);
            //printPowerFormWithEnter((char_POWER_FORM*)PRIMITIVE_POLYNOMIAL_128_INVERSE_INDEX);
            //printPowerFormWithEnter((char_POWER_FORM*)PRIMITIVE_POLYNOMIAL_256_INVERSE_INDEX);
            //printPowerFormWithEnter((char_POWER_FORM*)PRIMITIVE_POLYNOMIAL_512_INVERSE_INDEX);
            //printPowerFormWithEnter((char_POWER_FORM*)PRIMITIVE_POLYNOMIAL_1024_INVERSE_INDEX);
            */
    /* To generate galois fields, initializing primitive polynomials end */

    /* Initializing essential conditions end */


    /* create galois field using primitive polynomial start */
    /* check file that is saved galois fields infomations start */
    /* check file that is saved galois fields infomations end */
    main_com_listOf_GF=(struct_galoisField_info **)malloc(sizeof(struct_galoisField_info*)*PRIMITIVE_POLYNOMIAL_SET_INVERSED.PRIMITIVE_POLYNOMIAL_NUMBER);
    main_com_listOf_GF[0]=NULL;

    if(global_flag_gfCommon&FLAG_MASK_GF_COMMON_MAKE_GF_ALL)
    {
        tmp_i_start=1;
        tmp_i_end=PRIMITIVE_POLYNOMIAL_SET_INVERSED.PRIMITIVE_POLYNOMIAL_NUMBER;
    }
    else
    {
        tmp_i_start=global_GaloisFieldExponential;
        tmp_i_end=global_GaloisFieldExponential+1;
    }

    infoMes;
    sprintf(fileio_Path_Target, "%s/%s", fileio_Path_common, dirio_Name_GF_info);
    printf("GF info path -> %s\r\n", fileio_Path_Target);
    for(i=tmp_i_start; i<tmp_i_end; i++)
    {
        /* Galois Fields infomation */
        /* check file that is saved galois fields infomations start */
        if(!(tmp_dirio_var = opendir((char*)fileio_Path_Target)))
        {
            errorMes; printf("Can't open path -> %s\r\n", fileio_Path_Target);
            errorMes; printf("Simulator is closed.\r\n");
            return -1;
        }


        sprintf(fileio_Name_GF_infoBuf, "%s_%d_px_%s_%s.txt",
            fileio_Name_GF_infoCommon,
            i,
            (char*)PRIMITIVE_POLYNOMIAL_SET_INVERSED.PRIMITIVE_POLYNOMIAL[i],
            fileio_Name_GaloisField_common
        );
            //printf("%s\r\n", fileio_Name_GF_infoBuf);
        rewinddir(tmp_dirio_var);
        while(tmp_dir_entry = readdir(tmp_dirio_var))  //디렉토리 안에 있는 모든 파일과 디렉토리 출력
        {
            if(!strcmp(fileio_Name_GF_infoBuf, tmp_dir_entry->d_name)) break;
        }
        /* check file that is saved galois fields infomations end */

        initMes; printf("Selected primitive poly is %s\r\n", (char*)PRIMITIVE_POLYNOMIAL_SET_INVERSED.PRIMITIVE_POLYNOMIAL[i]);
        main_com_listOf_GF[i]=
            //createGaloisFieldAndSumMatrix(createPowerFormPolynomialUsingString((char_POWER_FORM*)PRIMITIVE_POLYNOMIAL_SET_INVERSED.PRIMITIVE_POLYNOMIAL[i], 0));
            createGaloisField_info_emptySpace(createPowerFormPolynomialUsingString((char_POWER_FORM*)PRIMITIVE_POLYNOMIAL_SET_INVERSED.PRIMITIVE_POLYNOMIAL[i], 0));
        sprintf(fileio_Path_Target, "%s/%s/%s", fileio_Path_common, dirio_Name_GF_info, fileio_Name_GF_infoBuf);
            //printf("%s\r\n", fileio_Path_common);

                gettimeofday(&time_proc_start, NULL);
        if(!tmp_dir_entry)
        {
            infoMes; printf("Saved Path -> %s\r\n", fileio_Path_Target);
            calculateGaloisField(main_com_listOf_GF[i]->primitivePolynomial, main_com_listOf_GF[i]->nonSorted);
            save_struct_galoisFieldElements(
                fileio_Path_Target,
                (char*)PRIMITIVE_POLYNOMIAL_SET_INVERSED.PRIMITIVE_POLYNOMIAL[i],
                main_com_listOf_GF[i]->nonSorted
            );
            infoMes; printf("'%s' is saved.\r\n", fileio_Name_GF_infoBuf);
                    printf("[calculateing and saving galois field time]");    printExcutingTime(&time_proc_start);
        }
        else
        {
            infoMes; printf("File path -> %s is detected.\r\n", fileio_Path_Target);
            load_struct_galoisFieldElements(fileio_Path_Target, (char*)PRIMITIVE_POLYNOMIAL_SET_INVERSED.PRIMITIVE_POLYNOMIAL[i], main_com_listOf_GF[i]->nonSorted);
            infoMes; printf("'%s' is loaded.\r\n", fileio_Name_GF_infoBuf);
                    printf("[loading galois field time]");    printExcutingTime(&time_proc_start);
        }


        /* Sumation Matrix infomation */
        /* check file that is saved galois fields infomations start */
        sprintf(fileio_Name_GF_infoBuf, "%s_%d_px_%s_%s.txt",
            fileio_Name_GF_infoCommon,
            i,
            (char*)PRIMITIVE_POLYNOMIAL_SET_INVERSED.PRIMITIVE_POLYNOMIAL[i],
            fileio_Name_SumMaxrix_common
        );
            //printf("%s\r\n", fileio_Name_GF_infoBuf);
        rewinddir(tmp_dirio_var);
        while(tmp_dir_entry = readdir(tmp_dirio_var))  // 디렉토리 안에 있는 모든 파일과 디렉토리 출력
        {
            if(!strcmp(fileio_Name_GF_infoBuf, tmp_dir_entry->d_name)) break;
        }
        /* check file that is saved galois fields infomations end */

        sprintf(fileio_Path_Target, "%s/%s/%s", fileio_Path_common, dirio_Name_GF_info, fileio_Name_GF_infoBuf);
                    gettimeofday(&time_proc_start, NULL);
        if(!tmp_dir_entry)
        {
            infoMes; printf("Saved Path -> %s\r\n", fileio_Path_Target);
            //char save_struct_summationMatrix(char *path, char *primitivePoly, struct_summationMatrix *p)
            calculateSummationMatrix(main_com_listOf_GF[i]->nonSorted, main_com_listOf_GF[i]->nonSorted->summationReferMatrix);
            save_struct_summationMatrix(
                fileio_Path_Target,
                (char*)PRIMITIVE_POLYNOMIAL_SET_INVERSED.PRIMITIVE_POLYNOMIAL[i],
                main_com_listOf_GF[i]->nonSorted->summationReferMatrix
            );
            infoMes; printf("'%s' is saved.\r\n", fileio_Name_GF_infoBuf);
                    printf("[calculateing and saving summation matrix time]");    printExcutingTime(&time_proc_start);
        }
        else
        {
            infoMes; printf("File path -> %s is detected.\r\n", fileio_Path_Target);
            load_struct_summationMatrix(fileio_Path_Target, (char*)PRIMITIVE_POLYNOMIAL_SET_INVERSED.PRIMITIVE_POLYNOMIAL[i], main_com_listOf_GF[i]->nonSorted, main_com_listOf_GF[i]->nonSorted->summationReferMatrix);
            infoMes; printf("'%s' is loaded.\r\n", fileio_Name_GF_infoBuf);
                    printf("[loading summation matrix time]");    printExcutingTime(&time_proc_start);
        }


        /* check file that is saved galois fields infomations start */
        sprintf(fileio_Name_GF_infoBuf, "%s_%d_px_%s_%s.txt",
            fileio_Name_GF_infoCommon,
            i,
            (char*)PRIMITIVE_POLYNOMIAL_SET_INVERSED.PRIMITIVE_POLYNOMIAL[i],
            fileio_Name_Conjugate_common
        );
                //printf("%s\r\n", fileio_Name_GF_infoBuf);
        rewinddir(tmp_dirio_var);
        while(tmp_dir_entry = readdir(tmp_dirio_var))  // 디렉토리 안에 있는 모든 파일과 디렉토리 출력
        {
            if(!strcmp(fileio_Name_GF_infoBuf, tmp_dir_entry->d_name)) break;
        }
        /* check file that is saved galois fields infomations end */
        sprintf(fileio_Path_Target, "%s/%s/%s", fileio_Path_common, dirio_Name_GF_info, fileio_Name_GF_infoBuf);
                //printf("%s\r\n", fileio_Path_common);

                gettimeofday(&time_proc_start, NULL);
        if(!tmp_dir_entry)
        {
            infoMes; printf("Saved Path -> %s\r\n", fileio_Path_Target);
            init_ConjugacyClasses(main_com_listOf_GF[i]->nonSorted, &(main_com_listOf_GF[i]->conjugacies), global_LimitCodeLength);

            save_struct_setOfGaloisFieldElements(
                fileio_Path_Target,
                (char*)PRIMITIVE_POLYNOMIAL_SET_INVERSED.PRIMITIVE_POLYNOMIAL[i],
                main_com_listOf_GF[i]->conjugacies
            );
            infoMes; printf("'%s' is saved.\r\n", fileio_Name_GF_infoBuf);
                    printf("[calculateing and saving conjugacies time]");    printExcutingTime(&time_proc_start);
        }
        else
        {
            infoMes; printf("File path -> %s is detected.\r\n", fileio_Path_Target);
            load_struct_setOfGaloisFieldElements(fileio_Path_Target, (char*)PRIMITIVE_POLYNOMIAL_SET_INVERSED.PRIMITIVE_POLYNOMIAL[i], main_com_listOf_GF[i]->nonSorted, &(main_com_listOf_GF[i]->conjugacies));
            infoMes; printf("'%s' is loaded.\r\n", fileio_Name_GF_infoBuf);
                    printf("[loading conjugacies time]");    printExcutingTime(&time_proc_start);
        }

        closedir(tmp_dirio_var);


        initMes; printf("Initializing galois field(2^%d) is done.\r\n", i);
    }


    main_com_used_GF=main_com_listOf_GF[global_GaloisFieldExponential];
    main_com_codeLength = main_com_used_GF->nonSorted->length-1;//codeLength can be shorter than number of element in GF
    initMes; printf("Mainly used value, galois field is set to gf(2^%d).\r\n", global_GaloisFieldExponential);

    if(global_flag_gfCommon_display&FLAG_MASK_DISPLAY_GF_COMMON_FIELD)
    {
        infoMes; printf("/********** Used galois Field elements **********/\n");
        printGaloisField(main_com_used_GF->nonSorted);
    }
    if(global_flag_gfCommon_display&FLAG_MASK_DISPLAY_GF_COMMON_CONJUGACIES)
    {
        infoMes; printf("/********** Used galois Field conjugacies **********/\n");
        printConjugacyClasses(main_com_used_GF->nonSorted, main_com_used_GF->conjugacies);
    }

    //main_com_used_GF=createGaloisFieldAndSumMatrix(createPowerFormPolynomialUsingString((char_POWER_FORM*)PRIMITIVE_POLYNOMIAL_SET.PRIMITIVE_POLYNOMIAL[global_GaloisFieldExponential],0));

    /*
    //struct_galoisField_info *galoisField8=createGaloisFieldAndSumMatrix(createPowerFormPolynomialUsingString((char_POWER_FORM*)PRIMITIVE_POLYNOMIAL_8_INVERSE_INDEX,0));
    //struct_galoisField_info *galoisField16=createGaloisFieldAndSumMatrix(createPowerFormPolynomialUsingString((char_POWER_FORM*)PRIMITIVE_POLYNOMIAL_16_INVERSE_INDEX,0));
    //struct_galoisField_info *galoisField32=createGaloisFieldAndSumMatrix(createPowerFormPolynomialUsingString((char_POWER_FORM*)PRIMITIVE_POLYNOMIAL_32_INVERSE_INDEX,0));
    //struct_galoisField_info *galoisField64=createGaloisFieldAndSumMatrix(createPowerFormPolynomialUsingString((char_POWER_FORM*)PRIMITIVE_POLYNOMIAL_64_INVERSE_INDEX,0));
    //struct_galoisField_info *galoisField256=createGaloisFieldAndSumMatrix(createPowerFormPolynomialUsingString((char_POWER_FORM*)PRIMITIVE_POLYNOMIAL_256_INVERSE_INDEX,0));
    //struct_galoisField_info *galoisField1024=createGaloisFieldAndSumMatrix(createPowerFormPolynomialUsingString((char_POWER_FORM*)PRIMITIVE_POLYNOMIAL_1024_INVERSE_INDEX,0));
    //
    //printGaloisFieldToCsv(galoisField8->nonSorted);
    //printGaloisFieldToCsv(galoisField16->nonSorted);
    //printGaloisFieldToCsv(galoisField32->nonSorted);
    //printGaloisFieldToCsv(galoisField64->nonSorted);
    //printGaloisFieldToCsv(galoisField256->nonSorted);
    //printGaloisFieldToCsv(galoisField1024->nonSorted);
    */
    /* create galois field using primitive polynomial end */


    /* Using conjugacy in galois Field, make generation polynomial start */
    printf("\n\n");

    printf("/*****************************************************************/\r\n");
    printf("/************ Setting value associated with BCH codes ************/\r\n");
    printf("/*****************************************************************/\r\n");


    if(global_CodeLength!=DEFAULT_VALUE_CODE_LENGTH)
    {
        main_com_codeLength = global_CodeLength;//codeLength can be shorter than number of element in GF
        if(!(main_com_codeLength < main_com_used_GF->nonSorted->length))
        {
            warningMes; printf("if(main_com_used_GF->nonSorted->length(='%d')<main_com_codeLength(='%d')) is true.\n", main_com_used_GF->nonSorted->length, main_com_codeLength);
            warningMes; printf("main_com_codeLength is forced to set maximum length of main_galoisFiels(='%d').\n", main_com_used_GF->nonSorted->length-1);
            main_com_codeLength = main_com_used_GF->nonSorted->length-1;
        }
    }
    else
    {
        main_com_codeLength = DEFAULT_VALUE_CODE_LENGTH;
    }
    infoMes; printf("Mainly used value, code length = '%d'.\r\n", main_com_codeLength);


    printf("/*****************************************************************/\r\n");
    printf("/********** Setting value associated with Hard Decision **********/\r\n");
    printf("/*****************************************************************/\r\n");

    /* set hard-decision correctable value(t) start*/
    main_com_genPoly=NULL;
    if(global_bch_hardCorrectable!=DEFAULT_VALUE_BCH_HARD_CORRECTABLE)
    {
        main_com_hd_correctability = global_bch_hardCorrectable;
    }
    else
    {
        main_com_hd_correctability = DEFAULT_VALUE_BCH_HARD_CORRECTABLE;
    }
    infoMes; printf("Mainly used value, hard decision correctable number = '%d'\r\n", main_com_hd_correctability);
    /* set hard-decision correctable value(t) end*/


    /*To test BCH Algorithm, setting number of errors start*/
    if(global_flag_sim_decodingMode&FLAG_MASK_CODE_BCH_DEC_AL_TEST_THIS_ALGORITHM)
    {
        if(!global_ErrorNum)
        {
            main_com_errNums_inDecodingAlgorithmTest=main_com_hd_correctability;
        }
        else
        {
            main_com_errNums_inDecodingAlgorithmTest = global_ErrorNum;
            if(main_com_errNums_inDecodingAlgorithmTest > main_com_hd_correctability)
            {
                warningMes; printf("main_com_errNums_inDecodingAlgorithmTest is bigger than main_com_hd_correctability.(%d > %d)\r\n", main_com_errNums_inDecodingAlgorithmTest, main_com_hd_correctability);
                warningMesShort; printf("While simulating, simulator can be broken.\r\n");
            }
        }
        infoMes; printf("Mainly used value, to check algorithm, error number = '%d'\r\n", main_com_errNums_inDecodingAlgorithmTest);
    }
        /* To test BCH Algorithm, setting number of errors end */

    /* calculate total order of LCM. */
    if((main_com_used_GF->conjugacies->length<main_com_hd_correctability))
    {
        //errorMes;
        warningMes;    printf("In test generation of LCM, (main_com_used_GF->conjugacies->length(=\'%d\') < main_com_hd_correctability(=\'%d\'))\n", main_com_used_GF->conjugacies->length, main_com_hd_correctability);

        main_com_hd_correctability = main_com_used_GF->conjugacies->length;
        warningMes;    printf("Force to set main_com_hd_correctability that \'%d\'.\n", main_com_hd_correctability);
        //return -1;
    }
    for(i=0; i<main_com_hd_correctability; i++)
    {
        main_com_totalOrderOfLCM+=(*(main_com_used_GF->conjugacies->conjugateSet+i))->length;
    }
    /* calculate total order of LCM. */
    infoMes; printf("Total Order(Condjugacies only) Of LCM is \'%d\'\n", main_com_totalOrderOfLCM);

    /* create */
    recreateGaloisFieldExceptElements(&main_com_expandEqOfLcmFromRoot, main_com_totalOrderOfLCM+1);
    initEquation(main_com_used_GF->nonSorted, main_com_expandEqOfLcmFromRoot);

    /* abstract */
    main_com_expandEqOfLcmFromRoot=expandRootOfLCM_usingEleOfGF(main_com_used_GF->nonSorted, main_com_expandEqOfLcmFromRoot, main_com_used_GF->conjugacies, main_com_hd_correctability);

    /* added LCM expantion */
    global_degAddedRootToLCM=(strlen(global_addedRootToLCM_string)-1);
    recreateGaloisFieldExceptElements(&main_com_addedLCM, global_degAddedRootToLCM+1);
    initBinEquationUsingStr(main_com_used_GF->nonSorted, main_com_addedLCM, global_addedRootToLCM_string, global_degAddedRootToLCM+1);

    main_com_finalEqOfrootOfLCM_andAddedLCM=multipleTwoEqReturnAddr(
        main_com_used_GF->nonSorted,
        &main_com_finalEqOfrootOfLCM_andAddedLCM,
        main_com_expandEqOfLcmFromRoot,
        main_com_addedLCM);

    initMes; printf("Initializing expand LCM is done.\r\n");
            //printGaloisField2(main_com_used_GF->nonSorted, main_com_expandEqOfLcmFromRoot, PRINT_FIELD_EQUATION_TITLE, PRINT_FIELD_PRINT_OPTION_NUMBERING);
    if(global_flag_gfCommon_display&FLAG_MASK_DISPLAY_GF_COMMON_PRIMEPOLY)
    {
        printGaloisField2(main_com_used_GF->nonSorted, main_com_expandEqOfLcmFromRoot, PRINT_FIELD_EQUATION_TITLE, PRINT_FIELD_PRINT_OPTION_NUMBERING);
        printGaloisField2(main_com_used_GF->nonSorted, main_com_addedLCM, PRINT_FIELD_EQUATION_TITLE, PRINT_FIELD_PRINT_OPTION_NUMBERING);
    }

    main_com_genPoly=convertGaloisFielsAndElementsToStringOnlyZeroOrOne(main_com_used_GF->nonSorted, main_com_finalEqOfrootOfLCM_andAddedLCM);
    initMes; printf("Converting galois field element to string is done.\r\n");
    infoMes; printf("Expanded Least Common Multiple(generation polynomial) is \"%s\"\n", main_com_genPoly);

        /* reate */
        recreateGaloisFieldExceptElements(&main_com_limitExpandEqOfLCM_fromRoot, main_com_totalOrderOfLCM+1);
        initEquation(main_com_used_GF->nonSorted, main_com_limitExpandEqOfLCM_fromRoot);

        /* abstract */
        main_com_limitExpandEqOfLCM_fromRoot=test_expandLimitedLeastCommonMultipleUsingElementOfGaloisFieldToRoot(main_com_used_GF->nonSorted, main_com_limitExpandEqOfLCM_fromRoot, main_com_used_GF->conjugacies, main_com_hd_correctability);
        initMes; printf("Initializing expand LCM is done.\r\n");
                //printGaloisField2(main_com_used_GF->nonSorted, main_com_limitExpandEqOfLCM_fromRoot, PRINT_FIELD_EQUATION_TITLE, PRINT_FIELD_PRINT_OPTION_NUMBERING);
        if(global_flag_gfCommon_display&FLAG_MASK_DISPLAY_GF_COMMON_PRIMEPOLY)
        {
            printGaloisField2(main_com_used_GF->nonSorted, main_com_limitExpandEqOfLCM_fromRoot, PRINT_FIELD_EQUATION_TITLE, PRINT_FIELD_PRINT_OPTION_NUMBERING);
        }

        main_com_limitedGenerationPolynomial=convertGaloisFielsAndElementsToStringOnlyZeroOrOne(main_com_used_GF->nonSorted, main_com_limitExpandEqOfLCM_fromRoot);
        initMes; printf("Converting galois field element to string is done.\r\n");
        infoMes; printf(" Limited Least Common Multiple(generation polynomial) is \"%s\"\n", main_com_limitedGenerationPolynomial);


    main_com_infoBitLength=main_com_codeLength-(strlen(main_com_genPoly)-1);
    main_com_parityBitsLength=main_com_codeLength-main_com_infoBitLength;
    infoMes; printf("Code length : \'%d\', Infomation bits length : \'%d\', Parity bits length : \'%d\'\n", main_com_codeLength, main_com_infoBitLength, main_com_parityBitsLength);
    main_com_codeRate=((double)main_com_infoBitLength)/((double)main_com_codeLength);
    infoMes; printf("Code rate : '%f' \n", main_com_codeRate);
    /* Using conjugacy in galois Field, make generation polynomial end */


    /* Create encoding component using converted g(x) to power form(string) start */
    infoMes; printf("Test generation of LCM, GF(%d), set t=%d, select \'%d\' number of set of conjugacy\n", main_com_used_GF->nonSorted->length, main_com_hd_correctability, main_com_hd_correctability);
    //struct_encodingComponentInGF *main_encodingComponentInGF=createEncodingComponent(main_com_codeLength, main_com_hd_correctability, (char_POWER_FORM*)main_com_genPoly);
    recreateEncodingComponent(&main_encodingComponentInGF, main_com_codeLength, main_com_hd_correctability, (char_POWER_FORM*)main_com_genPoly);
    initMes; printf("Creating encoding componenet is done.\r\n");
    /* Create encoding component using converted g(x) to power form(string) end */


    /* Soft Decision Test Pattern Decording Coponents. If soft decision correctable bits is p,  number of test petern is (2^p) - 1. */
    /* Soft Decision Test Pattern Decording Coponents. end */
    printf("/*****************************************************************/\r\n");
    printf("/********** Setting value associated with Soft Decision **********/\r\n");
    printf("/*****************************************************************/\r\n");
    infoMes; printf("Mainly used value, soft decision correctable number = '%d'\n", main_tmp_degErrLocPoly);
    infoMes; printf("Mainly used value, test pattern number = '%d'\n", main_com_numsTP);

    *(main_com_list_TP_pwrFormPoly+0) = NULL;
    for(main_tmp_soft_i=1; main_tmp_soft_i<main_com_numsTP; main_tmp_soft_i++)
    {
        *(main_com_list_TP_pwrFormPoly+main_tmp_soft_i) = createPowerFormPolynomial(main_com_codeLength);
        initMes; printf("#%d : create dummy test partern component.\n", main_tmp_soft_i);
    }


    printf("/*****************************************************************/\r\n");
    printf("/************* Initialzing a component of Modulation *************/\r\n");
    printf("/*****************************************************************/\r\n");
    /* creation BPSK communication compoenets start */
    /* initial global quantization info */
    initMes; printf("Setted global value of quantization filter.\r\n");
    infoMes; printf("(global) llr quantization offset : %f\r\n", global_LLR_quantizFilter_offset);
    infoMes; printf("(global) llr quantization step : %f\r\n", global_LLR_quantizFilter_step);
    infoMes; printf("(global) llr quantization bit : %d\r\n", global_LLR_quantizFilter_bitWidth);

    initGlobalQuantizInfo
        (
            global_LLR_quantizFilter_offset,
            global_LLR_quantizFilter_step,
            global_LLR_quantizFilter_bitWidth,
            global_flag_case_llr_numberic_system,
            global_flag_case_llr_mag_ones_complement_zero_handling
        );
    initMes; printf("Initialized value of quantization filter.\r\n");
    infoMes; printf("numberic system is '%s'.\r\n",
            NAME_QUANTIZ_NUMBERIC_SYSTEM[global_flag_case_llr_numberic_system]);
    infoMes; printf("llr quantization offset : %f\r\n", global_QuantizationInfo.offset);
    infoMes; printf("llr quantization step : %f\r\n", global_QuantizationInfo.step);
    infoMes; printf("llr quantization bit : %d\r\n", global_QuantizationInfo.numsOfBits);
    infoMes; printf("llr quantization numberic system : %s\r\n", NAME_QUANTIZ_NUMBERIC_SYSTEM[global_QuantizationInfo.numberic_system]);
    infoMes; printf("llr quantization zero range handling: %s\r\n", NAME_QUANTIZ_ONE_S_COMPLEMENT_ZERO_HANDLING[global_QuantizationInfo.ones_zero_handling]);
    printf("/***********************************************************/\r\n");


    /* modulation components */
    if(!global_flag_Modulation_Mode)
    {
        warningMes; printf("modulation mode is not selected!!!\n");
    }
    else
    {
        if(global_flag_Modulation_Mode&FLAG_MASK_MODULATION_MODE_BPSK)
        {
            printf("/***********************************************************/\r\n");
            printf("/************* Initialzing a component of BPSK *************/\r\n");
            printf("/***********************************************************/\r\n");
            main_com_bpskComponents=createBpskComponents(main_com_codeLength);
            initMes; printf("Create bpsk components.\r\n");
            /* initial LLR components start */
            setUsedLengthOfLogLikeHoodRatio(main_com_bpskComponents->bpskReceivedLLR, main_com_codeLength);
            initMes; printf("Setting up log like hood ratio in bpsk componenets.\r\n");
            /* initial LLR magnitudeShort locator start */
            initLogLikeHoodRatioLocator(main_com_bpskComponents->bpskReceivedLLR, main_tmp_degErrLocPoly);
            initMes; printf("Initializing log like hood ratio components in bpsk componenets.\r\n");


            setQuantizedLLRMaskOfLogLikeHoodRatio(main_com_bpskComponents->bpskReceivedLLR, global_QuantizationInfo.numsOfBits);
            initMes; printf("Initializing log like hood ratio mask in bpsk componenets.\r\n");
            /*** if selecting tree structure is posible, treeType have to change other variable like global variable. ***/

            infoMes; printf("Finding Minimum LLR magnitude method is %s.\r\n",
                NAME_FLAG_CASE_FINDING_MIN_MTEHOD[global_flag_case_find_LLR_method]);
            switch(global_flag_case_find_LLR_method)
            {
                case FLAG_CASE_FINDING_MIN_LLR_METHOD_NONE:
                    global_flag_case_find_LLR_method=FLAG_CASE_FINDING_MIN_LLR_METHOD_DEFAULT;
                    initializingVariablesAssociateWithTree(main_com_bpskComponents->bpskReceivedLLR, TREE_CHASE);
                    initMes; printf("Initializing tree structure associate with log like hood ratio in bpsk componenets.\r\n");
                break;
                case FLAG_CASE_FINDING_MIN_LLR_METHOD_MINIMUM:
                break;
                case FLAG_CASE_FINDING_MIN_LLR_METHOD_GROUPING:

                    global_group_bit_num = main_com_codeLength / global_grouping_stream_nums;
                    /* global_group_last_bit_num always less than global_grouping_stream_nums */
                    global_group_last_bit_num = main_com_codeLength % global_group_bit_num;

                    if(global_group_last_bit_num)
                    {
                        global_group_bit_num++;
                        global_group_last_bit_num = main_com_codeLength % global_group_bit_num;
                    }
                    else
                    {
                        global_group_last_bit_num = global_group_bit_num;
                    }
                    infoMes; printf("Code Length : %d\r\n", main_com_codeLength);
                    infoMes; printf("Grouping num : %d\r\n", global_grouping_stream_nums);
                    infoMes; printf("A group has bit : %d\r\n", global_group_bit_num);
                    infoMes; printf("The last bits has bit : %d\r\n", global_group_last_bit_num);
                break;
                case FLAG_CASE_FINDING_MIN_LLR_METHOD_TREE_FOLLOWING_MIN1_PATH:
                    initializingVariablesAssociateWithTree(main_com_bpskComponents->bpskReceivedLLR, TREE_CHASE);
                    initMes; printf("Initializing tree structure associate with log like hood ratio in bpsk componenets.\r\n");
                break;
                default:
                    global_flag_case_find_LLR_method=FLAG_CASE_FINDING_MIN_LLR_METHOD_DEFAULT;
                    initializingVariablesAssociateWithTree(main_com_bpskComponents->bpskReceivedLLR, TREE_CHASE);
                    initMes; printf("Initializing tree structure associate with log like hood ratio in bpsk componenets.\r\n");
                break;
            }
            infoMes; printf("0-LLR MAG handling mode : %s.\r\n",
                NAME_FLAG_CASE_PASS_MAG0_HD_1[global_flag_case_pass_hd_1]);

            infoMes; printf("Same-LLR MAG prior mode : %s.\r\n",
                NAME_FLAG_CASE_SAME_LLR_HANDLING[global_flag_case_same_llr_handling]);

                    #ifndef RELEASE
                    /*************** struct_treeStructure address start ***************/
                    // debugMes; printf("TREE_NONE = %d\n", TREE_NONE);
                    // debugMes; printf("TREE_CHASE = %d\n", TREE_CHASE);
                    // testTreeStruct(main_com_bpskComponents->bpskReceivedLLR->treeStruct);
                    // testPrintTreeStructIndicateAddr(main_com_bpskComponents->bpskReceivedLLR->treeStruct);
                    /**************** struct_treeStructure address end ****************/
                    /***** struct_treeStructure create and close memory test start *****/
                    // struct_treeStructure *testStructure = NULL;
                    // for(i=0; i<10000000; i++)
                    // {
                    //    if(!(testStructure = createTreeStruct(127, TREE_CHASE)))  debugMes; printf("%d : testStructure = createTreeStruct(127, TREE_CHASE)\n", i);
                    //    if(closeTreeStruct(&testStructure)) debugMes; printf("%d : closeTreeStruct(&testStructure)\n", i);
                    // }
                    /****** struct_treeStructure create and close memory test end ******/
                    //printf("test : mask value is '%d'\n", main_com_bpskComponents->bpskReceivedLLR->quantizedLLRMask);
                    #endif
            /*initial LLR magnitudeShort locator end*/
            /*initial LLR components end*/
            /*creation BPSK communication compoenets end*/
        }
    }


    printf("/*****************************************************************/\r\n");
    printf("/************* Initialzing a simulation enviroments **************/\r\n");
    printf("/*****************************************************************/\r\n");
    //if(!global_minimum_loop)
    //{
    //    global_minimum_loop=DEFAULT_MINIMUM_LOOP;
    //    infoMes; printf("Minumum loop is not assigned, set to default, is %ld\r\n", global_minimum_loop);
    //}
    infoMes; printf("Minumum loop is %ld\r\n", global_minimum_loop);
    infoMes; printf("Base of loop is %ld\r\n", global_base_of_loop);
    infoMes; printf("Rate of loop is %d\r\n", global_rate_of_loop);


    printf("/*****************************************************************/\r\n");
    printf("/******************** Simulation Enviroments *********************/\r\n");
    printf("/*****************************************************************/\r\n");
    infoMes; printf("EbN0 start value is '%f'.\n", global_EbN0_Start_Value);
    infoMes; printf("EbN0 step value is '%f'.\n", global_EbN0_Step);
    infoMes; printf("EbN0 end value is '%f'.\n", global_EbN0_End_Value);


    //if(!(global_flag_bchDecAlgo_Enable&FLAG_MASK_CODE_BCH_DEC_AL_SOFT_DECISION))
    cmd_tmp_cnt=0;
    for(cmd_tmp_i=0; cmd_tmp_i<KIND_OF_BCH_DECODING_END; cmd_tmp_i++)
    {
        if(global_flag_bchDecAlgo_Enable&uint32_bitMask[cmd_tmp_i])
        {
            cmd_tmp_cnt++;
            if(global_list_flag_bch_sim_decodingMode[cmd_tmp_i]&FLAG_MASK_CODE_BCH_DEC_AL_TEST_THIS_ALGORITHM)
            {
                infoMes; printf("[%s] %s algorithm test is enabled.\r\n",
                KIND_OF_BCH_DECODING_ALGORITHM_NAME[cmd_tmp_i], KIND_OF_BCH_DECODING_ALGORITHM_NAME[cmd_tmp_i]);
            }
            if(global_list_flag_bch_sim_decodingMode[cmd_tmp_i]&FLAG_MASK_CODE_BCH_DEC_AL_HARD_DECISION)
            {
                infoMes; printf("[%s] Hard decision decording enabled.\r\n", KIND_OF_BCH_DECODING_ALGORITHM_NAME[cmd_tmp_i]);
            }
            if(global_list_flag_bch_sim_decodingMode[cmd_tmp_i]&FLAG_MASK_CODE_BCH_DEC_AL_SOFT_DECISION)
            {
                infoMes; printf("[%s] Soft decision decording enabled.\r\n", KIND_OF_BCH_DECODING_ALGORITHM_NAME[cmd_tmp_i]);
            }

            if(global_list_flag_bch_sim_decodingMode[cmd_tmp_i]&FLAG_MASK_CODE_BCH_DEC_AL_SOFT_DECISION)
            {
                infoMes; printf("[%s] * Soft decision decording is primary. *\r\n", KIND_OF_BCH_DECODING_ALGORITHM_NAME[cmd_tmp_i]);
            }
            else if(global_list_flag_bch_sim_decodingMode[cmd_tmp_i]&FLAG_MASK_CODE_BCH_DEC_AL_HARD_DECISION)
            {
                infoMes; printf("[%s] * Hard decision decording is primary. *\r\n", KIND_OF_BCH_DECODING_ALGORITHM_NAME[cmd_tmp_i]);
            }
            else if(global_list_flag_bch_sim_decodingMode[cmd_tmp_i]&FLAG_MASK_CODE_BCH_DEC_AL_TEST_THIS_ALGORITHM)
            {
                infoMes; printf("[%s] algorithm test only mode is primary than hard and soft decision. *\r\n", KIND_OF_BCH_DECODING_ALGORITHM_NAME[cmd_tmp_i]);
            }

            if((global_list_flag_bch_sim_decodingMode[cmd_tmp_i]&FLAG_MASK_CODE_BCH_DEC_AL_SKIP))
            {
                if((global_list_flag_bch_sim_decodingMode[cmd_tmp_i]&FLAG_MASK_CODE_BCH_DEC_AL_HARD_DECISION)||(global_list_flag_bch_sim_decodingMode[cmd_tmp_i]&FLAG_MASK_CODE_BCH_DEC_AL_SOFT_DECISION))
                {
                    infoMes; printf("[%s] * Decoding Process skip is enabled. *\r\n", KIND_OF_BCH_DECODING_ALGORITHM_NAME[cmd_tmp_i]);
                }
            }
        }
        //else
        //{
        //    infoMes; printf("[%s] %s algorithm test is disabled.\r\n",
        //        KIND_OF_BCH_DECODING_ALGORITHM_NAME[cmd_tmp_i], KIND_OF_BCH_DECODING_ALGORITHM_NAME[cmd_tmp_i]);
        //}

    }
    /* Algorithm except conditions */
    if(!(global_bch_hardCorrectable<4))
    {
        warningMesShort; printf("mSBS algorithm optimized at error bits are less than 4. But Error correctablility is '%d'\r\n", global_bch_hardCorrectable);
        if(global_flag_bchDecAlgo_Enable&uint32_bitMask[KIND_OF_BCH_DECODING_mSBS])
        {
            cmd_tmp_cnt--;
            global_flag_bchDecAlgo_Enable&=(~uint32_bitMask[KIND_OF_BCH_DECODING_mSBS]);
            global_list_flag_bch_sim_decodingMode[KIND_OF_BCH_DECODING_mSBS]=0;
        }
    }

    if(cmd_tmp_cnt)
    {
        infoMes; printf(">> Making and Updating scheduler. <<\r\n");
        processingUseThisAlgorithm->valueType = VALUE_TYPE_UNSIGNED_CHAR;
        processingUseThisAlgorithm->list = (void*)malloc(sizeof(uint8_t)*cmd_tmp_cnt);
        memset(processingUseThisAlgorithm->list, 0, sizeof(uint8_t)*cmd_tmp_cnt);
        processingUseThisAlgorithm->length = cmd_tmp_cnt;

        main_tmp_sel_decAlgo_i = 0;
        for(cmd_tmp_i=0; cmd_tmp_i<KIND_OF_BCH_DECODING_END; cmd_tmp_i++)
        {
            if(global_flag_bchDecAlgo_Enable&uint32_bitMask[cmd_tmp_i])
            {
                (*((uint8_t*)(processingUseThisAlgorithm->list)+main_tmp_sel_decAlgo_i)) = ((uint8_t)cmd_tmp_i);
                infoMes; printf("[%s] algorithm is selected.\r\n", KIND_OF_BCH_DECODING_ALGORITHM_NAME[cmd_tmp_i]);
                main_tmp_sel_decAlgo_i++;
            }
        }

        for(main_tmp_sel_decAlgo_i=0; main_tmp_sel_decAlgo_i<processingUseThisAlgorithm->length; main_tmp_sel_decAlgo_i++)
        {
            infoMes; printf("chk Enable algorithm is \"%s\".\r\n", KIND_OF_BCH_DECODING_ALGORITHM_NAME[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]);
        }
    }
    else
    {
        warningMes; printf("*** Any simulation modes are not selected!!!! ***\r\n");
        warningMesShort; printf("*** Now, program is closed ***\r\n");
        return -1;
    }

    //main_indi_hd_correctability[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]
    //(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))
    for(main_tmp_sel_decAlgo_i=0; main_tmp_sel_decAlgo_i<processingUseThisAlgorithm->length; main_tmp_sel_decAlgo_i++)
    {
        if((*((uint8_t*)(processingUseThisAlgorithm->list)+main_tmp_sel_decAlgo_i)) == KIND_OF_BCH_DECODING_mSBS)
        {
            if(main_com_hd_correctability <= 3)
            {
                main_indi_hd_correctability[(*((uint8_t*)(processingUseThisAlgorithm->list)+main_tmp_sel_decAlgo_i))] = main_com_hd_correctability;
            }
            else
            {
                main_indi_hd_correctability[(*((uint8_t*)(processingUseThisAlgorithm->list)+main_tmp_sel_decAlgo_i))] = 3;
            }
        }
        else
        {
            main_indi_hd_correctability[(*((uint8_t*)(processingUseThisAlgorithm->list)+main_tmp_sel_decAlgo_i))] = main_com_hd_correctability;
        }
        infoMes; printf("%s algorithm setted, hard decision correctable number(individual) = '%d'\r\n",
            KIND_OF_BCH_DECODING_ALGORITHM_NAME[(*((uint8_t*)(processingUseThisAlgorithm->list)+main_tmp_sel_decAlgo_i))],
            main_indi_hd_correctability[(*((uint8_t*)(processingUseThisAlgorithm->list)+main_tmp_sel_decAlgo_i))]
        );
    }
    printf("\r\n");


    infoMes; printf(">> Setted Common Paramters are... <<\r\n");

    infoMes; printf("Hard decision, Correctable code length is \'%d\'\r\n", main_com_hd_correctability);
    //(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))
    for(main_tmp_sel_decAlgo_i=0; main_tmp_sel_decAlgo_i<processingUseThisAlgorithm->length; main_tmp_sel_decAlgo_i++)
    {
        infoMes; printf("%s - algorithm Hard decision, Correctable code length is \'%d\'\n",
            KIND_OF_BCH_DECODING_ALGORITHM_NAME[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))],
            main_indi_hd_correctability[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]
        );
    }
    infoMes; printf("Soft decision, Correctable code length is \'%d\'\r\n", main_tmp_degErrLocPoly);
    infoMes; printf("Number of test pattern number is \'%d\'\r\n", main_com_numsTP);

    infoMes; printf("Using Galois Field is \'%d\'\r\n", main_com_used_GF->nonSorted->length);
    infoMes; printf("Code word length is \'%d\'\r\n", main_com_codeLength);
    infoMes; printf("Information word length is \'%d\'\r\n", main_com_infoBitLength);
    infoMes; printf("Parity bits length is \'%d\'\r\n", main_com_parityBitsLength);
    infoMes; printf("Code rate is '%f' %s\r\n", main_com_codeRate,((global_flag_Channel_Mode_Func&FLAG_MASK_CHANNEL_MODE_FUNC_BITRATE_DISABLE)?"[disabled]":"[enabled]"));

    printf("/*****************************************************************/\r\n");
    printf("/******************** Notice Simulation Mode *********************/\r\n");
    printf("/*****************************************************************/\r\n");
    if(global_flag_sim_decodingMode&FLAG_MASK_CODE_BCH_DEC_AL_TEST_THIS_ALGORITHM)
    {
        infoMes; printf("Simulation will be entered to algorithm verification mode.\r\n");

        for(main_tmp_sel_decAlgo_i=0; main_tmp_sel_decAlgo_i<processingUseThisAlgorithm->length; main_tmp_sel_decAlgo_i++)
        {
            if(global_list_flag_bch_sim_decodingMode[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]&FLAG_MASK_CODE_BCH_DEC_AL_TEST_THIS_ALGORITHM)
            {
                infoMes; printf("[%s] algorithm verification mode.\r\n", KIND_OF_BCH_DECODING_ALGORITHM_NAME[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]);
            }
        }
        infoMes; printf("In verification Mode Artificial Error number : %d\n",main_com_errNums_inDecodingAlgorithmTest);
    }
    else
    {
        if(global_flag_sim_decodingMode&FLAG_MASK_CODE_BCH_DEC_AL_SOFT_DECISION)
        {
            infoMes; printf("Simulation will be entered to Soft Decision mode.\r\n");

            for(main_tmp_sel_decAlgo_i=0; main_tmp_sel_decAlgo_i<processingUseThisAlgorithm->length; main_tmp_sel_decAlgo_i++)
            {
                if(global_list_flag_bch_sim_decodingMode[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]&FLAG_MASK_CODE_BCH_DEC_AL_SOFT_DECISION)
                {
                    infoMes; printf("[%s] algorithm Soft Decision mode.\r\n", KIND_OF_BCH_DECODING_ALGORITHM_NAME[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]);
                }
            }
        }
        else if(global_flag_sim_decodingMode&FLAG_MASK_CODE_BCH_DEC_AL_HARD_DECISION)
        {
            infoMes; printf("Simulation will be entered to Hard Decision mode.\r\n");

            for(main_tmp_sel_decAlgo_i=0; main_tmp_sel_decAlgo_i<processingUseThisAlgorithm->length; main_tmp_sel_decAlgo_i++)
            {
                if(global_list_flag_bch_sim_decodingMode[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]&FLAG_MASK_CODE_BCH_DEC_AL_HARD_DECISION)
                {
                    infoMes; printf("[%s] algorithm Hard Decision mode.\r\n", KIND_OF_BCH_DECODING_ALGORITHM_NAME[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]);
                }
            }
        }
        else
        {
            errorMes; printf("ANY ALGORITHM IS NOT ENABLED.\r\n");
            errorMes; printf("SIMULATION IS CLOSED.\r\n");
            return -1;
        }
    }

    /*******************************************************************/
    /*******************************************************************/
    /********** Associate with file input output source start **********/
    /*******************************************************************/
    /*******************************************************************/
    printf("/*****************************************************************/\r\n");
    printf("/******************** Simulation Report Files ********************/\r\n");
    printf("/*****************************************************************/\r\n");

    ////loadStream, unloadStream
    //infoMes; printf(">>BMP file io initial variable set<<\r\n");
    //warningMes;
    //printf("File IO function is not completed.\r\n");
    //infoMes;
    //printf("dataBitLength is \'%u\'.\r\n", loadStream.dataBitLength);
    //infoMes;
    //printf("codeWordPerDataNumber is \'%u\'.\r\n", loadStream.codeWordPerDataNumber);
    //infoMes;
    //printf("totalCodeWordNumber is \'%u\'.\r\n", loadStream.totalCodeWordNumber);
    //infoMes;
    //printf("totalDataNumber is \'%u\'.\r\n", loadStream.totalDataNumber);
    //printf("\r\n");
    //if(global_flag_fileInputOutput)
    //{
    //    /* file io is not completed */
    //    warningMes;
    //    printf("/*************** file io function is not completed ***************/\r\n");
    //    printf("Please press ctrl + c or ctrl + z\r\n");


    //    if(loadBmpFile("image/1.bmp"))
    //    {
    //        printf("fail open image/1.bmp \r\n");
    //    }
    //    else
    //    {
    //        printf("open image/1.bmp\r\n");
    //    }

    //    setVariableSetConvertBitStreamToPowerForm(&loadStream, getBmpData(), main_com_infoBitLength, sizeof(uint8_t), BMP_FILE_IO_DEFAULT_BITMAP_DATA_SIZE);
    //    setVariableSetConvertBitStreamToPowerForm(&unloadStream, getBmpData2(), main_com_infoBitLength, sizeof(uint8_t), BMP_FILE_IO_DEFAULT_BITMAP_DATA_SIZE);
    //    setVariableSetConvertBitStreamToPowerForm(&noisedStream, getBmpData3(), main_com_infoBitLength, sizeof(uint8_t), BMP_FILE_IO_DEFAULT_BITMAP_DATA_SIZE);
    //    printf("\r\n>>BMP file io initial variable set<<\r\n");
    //    warningMes;
    //    printf("File IO function is not completed.\r\n");
    //    infoMes;
    //    printf("dataBitLength is \'%u\'.\r\n", loadStream.dataBitLength);
    //    infoMes;
    //    printf("codeWordPerDataNumber is \'%u\'.\r\n", loadStream.codeWordPerDataNumber);
    //    infoMes;
    //    printf("totalCodeWordNumber is \'%u\'.\r\n", loadStream.totalCodeWordNumber);
    //    infoMes;
    //    printf("totalDataNumber is \'%u\'.\r\n", loadStream.totalDataNumber);
    //    printf("\r\n\r\n");

    //    passingThroughAwgnData
    //    (
    //        global_EbN0_Start_Value,
    //        main_com_parityBitsLength,
    //        main_com_genPoly,
    //        main_com_hd_correctability,
    //        main_com_codeLength,
    //        main_com_used_GF,
    //        &loadStream,
    //        &unloadStream,
    //        &noisedStream
    //    );
    //    saveBmpFile2(0);
    //    saveBmpFile3(0);
    //    saveBmpFile4(0);

    //    setVariableSetConvertBitStreamToPowerForm(&loadStream, getBmpData(), main_com_infoBitLength, sizeof(uint8_t), BMP_FILE_IO_DEFAULT_BITMAP_DATA_SIZE);
    //    setVariableSetConvertBitStreamToPowerForm(&unloadStream, getBmpData2(), main_com_infoBitLength, sizeof(uint8_t), BMP_FILE_IO_DEFAULT_BITMAP_DATA_SIZE);
    //    setVariableSetConvertBitStreamToPowerForm(&noisedStream, getBmpData3(), main_com_infoBitLength, sizeof(uint8_t), BMP_FILE_IO_DEFAULT_BITMAP_DATA_SIZE);
    //    printf("\r\n>>BMP file io initial variable set<<\r\n");
    //    warningMes;
    //    printf("File IO function is not completed.\r\n");
    //    infoMes;
    //    printf("dataBitLength is \'%u\'.\r\n", loadStream.dataBitLength);
    //    infoMes;
    //    printf("codeWordPerDataNumber is \'%u\'.\r\n", loadStream.codeWordPerDataNumber);
    //    infoMes;
    //    printf("totalCodeWordNumber is \'%u\'.\r\n", loadStream.totalCodeWordNumber);
    //    infoMes;
    //    printf("totalDataNumber is \'%u\'.\r\n", loadStream.totalDataNumber);
    //    printf("\r\n\r\n");

    //    passingThroughAwgnData
    //    (
    //        global_EbN0_Start_Value,
    //        main_com_parityBitsLength,
    //        main_com_genPoly,
    //        main_com_hd_correctability,
    //        main_com_codeLength,
    //        main_com_used_GF,
    //        &loadStream,
    //        &unloadStream,
    //        &noisedStream
    //    );
    //    saveBmpFile2("OUTPUT_CORRECT.bmp");
    //    saveBmpFile3("OUTPUT_ADDITIVE.bmp");
    //    saveBmpFile4("OUTPUT_NOISE_ONLY.bmp");

    //    /* not complete part of decoding start */
    //    test_fileIO_errorCorrectingCount++;
    //    if(test_fileIO_errorCorrectingCount!=0)
    //    {
    //        if(loadStream.totalCodeWordNumber)
    //        {
    //            if(!(test_fileIO_errorCorrectingCount%loadStream.totalCodeWordNumber))
    //            {
    //                infoMes;
    //                printf("one stream sending is done. test_fileIO_errorCorrectingCount is \'%u\'\r\n", test_fileIO_errorCorrectingCount);
    //            }
    //        }
    //    }
    //    /* not complete part of decoding start */
    //}


    /* REPORT Channel BER */
    //FILE *fileio_channelBER;
    //char fileio_Name_channel_BER[256]={0};
    sprintf(fileio_Name_channel_BER, "%s/REPORT_GF(2^%d)_bch(%d,%d)_p%d_t%d_%s_%s_%s_%s_BER_CH.txt",
        fileio_Name_Report_Path,
        global_GaloisFieldExponential,

        main_com_codeLength,
        main_com_infoBitLength,
        main_tmp_degErrLocPoly,
        main_com_hd_correctability,

            ((global_flag_sim_decodingMode&FLAG_MASK_CODE_BCH_DEC_AL_TEST_THIS_ALGORITHM)?"algo":
            (global_flag_sim_decodingMode&FLAG_MASK_CODE_BCH_DEC_AL_SOFT_DECISION)?"soft":
            (global_flag_sim_decodingMode&FLAG_MASK_CODE_BCH_DEC_AL_HARD_DECISION)?"hard":"unkn"),

        NAME_FLAG_CASE_SEL_TP_BEFORE_DEC_MTEHOD_FILE_ADDED[global_flag_case_selTP_before_dec],
        NAME_FLAG_CASE_FINDING_MIN_MTEHOD_FILE_ADDED[global_flag_case_find_LLR_method],
        ((global_flag_Channel_Mode_Func&FLAG_MASK_CHANNEL_MODE_FUNC_BITRATE_DISABLE)?"non-BRa":"bitRate")
        );
    if(!(fileio_channelBER=fopen(fileio_Name_channel_BER, "w")))
    {
        errorMes;
        printf("Can not open \"%s\"\r\n", fileio_Name_channel_BER);
        return -1;
    }
    infoMes; printf("Enable file I/O \"%s\"\r\n", fileio_Name_channel_BER);

    /* REPORT algorithm BER is included all algorithm */
    //FILE *fileio_codingBER;
    //char fileio_Name_coding_BER[256]={0};
    sprintf(fileio_Name_coding_BER, "%s/REPORT_GF(2^%d)_bch(%d,%d)_p%d_t%d_%s_%s_%s_%s_BER_ALGO.txt",
        fileio_Name_Report_Path,
        global_GaloisFieldExponential,

        main_com_codeLength,
        main_com_infoBitLength,
        main_tmp_degErrLocPoly,
        main_com_hd_correctability,

            ((global_flag_sim_decodingMode&FLAG_MASK_CODE_BCH_DEC_AL_TEST_THIS_ALGORITHM)?"algo":
            (global_flag_sim_decodingMode&FLAG_MASK_CODE_BCH_DEC_AL_SOFT_DECISION)?"soft":
            (global_flag_sim_decodingMode&FLAG_MASK_CODE_BCH_DEC_AL_HARD_DECISION)?"hard":"unkn"),

        NAME_FLAG_CASE_SEL_TP_BEFORE_DEC_MTEHOD_FILE_ADDED[global_flag_case_selTP_before_dec],
        NAME_FLAG_CASE_FINDING_MIN_MTEHOD_FILE_ADDED[global_flag_case_find_LLR_method],
        ((global_flag_Channel_Mode_Func&FLAG_MASK_CHANNEL_MODE_FUNC_BITRATE_DISABLE)?"":"bitRate")
        );
    if(!(fileio_codingBER=fopen(fileio_Name_coding_BER, "w")))
    {
        errorMes;
        printf("Can not open \"%s\"\r\n", fileio_Name_coding_BER);
        return -1;
    }
    infoMes; printf("Enable file I/O \"%s\"\r\n", fileio_Name_coding_BER);

    /* REPORT all passed bit count */
    //FILE *fileio_total_bit_cnt;
    //char fileio_Name_total_bit_cnt[256]={0};
    sprintf(fileio_Name_total_bit_cnt, "%s/REPORT_GF(2^%d)_bch(%d,%d)_p%d_t%d_%s_%s_%s_%s_bit_cnt_total.txt",
        fileio_Name_Report_Path,
        global_GaloisFieldExponential,

        main_com_codeLength,
        main_com_infoBitLength,
        main_tmp_degErrLocPoly,
        main_com_hd_correctability,

            ((global_flag_sim_decodingMode&FLAG_MASK_CODE_BCH_DEC_AL_TEST_THIS_ALGORITHM)?"algo":
            (global_flag_sim_decodingMode&FLAG_MASK_CODE_BCH_DEC_AL_SOFT_DECISION)?"soft":
            (global_flag_sim_decodingMode&FLAG_MASK_CODE_BCH_DEC_AL_HARD_DECISION)?"hard":"unkn"),

        NAME_FLAG_CASE_SEL_TP_BEFORE_DEC_MTEHOD_FILE_ADDED[global_flag_case_selTP_before_dec],
        NAME_FLAG_CASE_FINDING_MIN_MTEHOD_FILE_ADDED[global_flag_case_find_LLR_method],
        ((global_flag_Channel_Mode_Func&FLAG_MASK_CHANNEL_MODE_FUNC_BITRATE_DISABLE)?"":"bitRate")
        );
    if(!(fileio_total_bit_cnt=fopen(fileio_Name_total_bit_cnt, "w")))
    {
        errorMes;
        printf("Can not open \"%s\"\r\n", fileio_Name_total_bit_cnt);
        return -1;
    }
    infoMes; printf("Enable file I/O \"%s\"\r\n", fileio_Name_total_bit_cnt);

    /* REPORT counted error bit in chennel */
    //FILE *fileio_channel_err_bit_cnt;
    //char fileio_Name_channel_err_bit_cnt[256]={0};
    sprintf(fileio_Name_channel_err_bit_cnt, "%s/REPORT_GF(2^%d)_bch(%d,%d)_p%d_t%d_%s_%s_%s_%s_bit_cnt_CH_err.txt",
        fileio_Name_Report_Path,
        global_GaloisFieldExponential,

        main_com_codeLength,
        main_com_infoBitLength,
        main_tmp_degErrLocPoly,
        main_com_hd_correctability,

            ((global_flag_sim_decodingMode&FLAG_MASK_CODE_BCH_DEC_AL_TEST_THIS_ALGORITHM)?"algo":
            (global_flag_sim_decodingMode&FLAG_MASK_CODE_BCH_DEC_AL_SOFT_DECISION)?"soft":
            (global_flag_sim_decodingMode&FLAG_MASK_CODE_BCH_DEC_AL_HARD_DECISION)?"hard":"unkn"),

        NAME_FLAG_CASE_SEL_TP_BEFORE_DEC_MTEHOD_FILE_ADDED[global_flag_case_selTP_before_dec],
        NAME_FLAG_CASE_FINDING_MIN_MTEHOD_FILE_ADDED[global_flag_case_find_LLR_method],
        ((global_flag_Channel_Mode_Func&FLAG_MASK_CHANNEL_MODE_FUNC_BITRATE_DISABLE)?"":"bitRate")
        );
    if(!(fileio_channel_err_bit_cnt=fopen(fileio_Name_channel_err_bit_cnt, "w")))
    {
        errorMes;
        printf("Can not open \"%s\"\r\n", fileio_Name_channel_err_bit_cnt);
        return -1;
    }
    infoMes; printf("Enable file I/O \"%s\"\r\n", fileio_Name_channel_err_bit_cnt);

    /* REPORT individually recored BER and error bit, they are belong to algorithm */
    for(main_tmp_sel_decAlgo_i=0; main_tmp_sel_decAlgo_i<processingUseThisAlgorithm->length; main_tmp_sel_decAlgo_i++)
    {
        /* Hard Decision BER */
        //(*(((uint8_t*)processingUseThisAlgorithm->list)+main_tmp_sel_decAlgo_i))
        sprintf(fileio_Name_indi_algorithm_coding_HD_BER[(*(((uint8_t*)processingUseThisAlgorithm->list)+main_tmp_sel_decAlgo_i))],
            "%s/REPORT_GF(2^%d)_bch(%d,%d)_p%d_t%d_%s_%s_%s_%s_BER(Static-HD)(%s).txt",
            fileio_Name_Report_Path,
            global_GaloisFieldExponential,

            main_com_codeLength,
            main_com_infoBitLength,
            main_tmp_degErrLocPoly,
            main_com_hd_correctability,

                ((global_flag_sim_decodingMode&FLAG_MASK_CODE_BCH_DEC_AL_TEST_THIS_ALGORITHM)?"algo":
                (global_flag_sim_decodingMode&FLAG_MASK_CODE_BCH_DEC_AL_SOFT_DECISION)?"soft":
                (global_flag_sim_decodingMode&FLAG_MASK_CODE_BCH_DEC_AL_HARD_DECISION)?"hard":"unkn"),

            NAME_FLAG_CASE_SEL_TP_BEFORE_DEC_MTEHOD_FILE_ADDED[global_flag_case_selTP_before_dec],
            NAME_FLAG_CASE_FINDING_MIN_MTEHOD_FILE_ADDED[global_flag_case_find_LLR_method],
            ((global_flag_Channel_Mode_Func&FLAG_MASK_CHANNEL_MODE_FUNC_BITRATE_DISABLE)?"":"bitRate"),
            KIND_OF_BCH_DECODING_ALGORITHM_NAME[(*(((uint8_t*)processingUseThisAlgorithm->list)+main_tmp_sel_decAlgo_i))]
            );
        if(!(fileio_indi_algorithm_coding_HD_BER[(*(((uint8_t*)processingUseThisAlgorithm->list)+main_tmp_sel_decAlgo_i))]=fopen(fileio_Name_indi_algorithm_coding_HD_BER[(*(((uint8_t*)processingUseThisAlgorithm->list)+main_tmp_sel_decAlgo_i))], "w")))
        {
            errorMes;
            printf("Can not open \"%s\"\r\n", fileio_Name_indi_algorithm_coding_HD_BER[(*(((uint8_t*)processingUseThisAlgorithm->list)+main_tmp_sel_decAlgo_i))]);
            return -1;
        }
        infoMes; printf("Enable file I/O \"%s\"\r\n", fileio_Name_indi_algorithm_coding_HD_BER[(*(((uint8_t*)processingUseThisAlgorithm->list)+main_tmp_sel_decAlgo_i))]);

        /* Soft Decision BER */
        sprintf(fileio_Name_indi_algorithm_coding_BER[(*(((uint8_t*)processingUseThisAlgorithm->list)+main_tmp_sel_decAlgo_i))],
            "%s/REPORT_GF(2^%d)_bch(%d,%d)_p%d_t%d_%s_%s_%s_%s_BER(%s).txt",
            fileio_Name_Report_Path,
            global_GaloisFieldExponential,

            main_com_codeLength,
            main_com_infoBitLength,
            main_tmp_degErrLocPoly,
            main_com_hd_correctability,

                ((global_flag_sim_decodingMode&FLAG_MASK_CODE_BCH_DEC_AL_TEST_THIS_ALGORITHM)?"algo":
                (global_flag_sim_decodingMode&FLAG_MASK_CODE_BCH_DEC_AL_SOFT_DECISION)?"soft":
                (global_flag_sim_decodingMode&FLAG_MASK_CODE_BCH_DEC_AL_HARD_DECISION)?"hard":"unkn"),

            NAME_FLAG_CASE_SEL_TP_BEFORE_DEC_MTEHOD_FILE_ADDED[global_flag_case_selTP_before_dec],
            NAME_FLAG_CASE_FINDING_MIN_MTEHOD_FILE_ADDED[global_flag_case_find_LLR_method],
            ((global_flag_Channel_Mode_Func&FLAG_MASK_CHANNEL_MODE_FUNC_BITRATE_DISABLE)?"":"bitRate"),
            KIND_OF_BCH_DECODING_ALGORITHM_NAME[(*(((uint8_t*)processingUseThisAlgorithm->list)+main_tmp_sel_decAlgo_i))]
            );
        if(!(fileio_indi_algorithm_codingBER[(*(((uint8_t*)processingUseThisAlgorithm->list)+main_tmp_sel_decAlgo_i))]=fopen(fileio_Name_indi_algorithm_coding_BER[(*(((uint8_t*)processingUseThisAlgorithm->list)+main_tmp_sel_decAlgo_i))], "w")))
        {
            errorMes;
            printf("Can not open \"%s\"\r\n", fileio_Name_indi_algorithm_coding_BER[(*(((uint8_t*)processingUseThisAlgorithm->list)+main_tmp_sel_decAlgo_i))]);
            return -1;
        }
        infoMes; printf("Enable file I/O \"%s\"\r\n", fileio_Name_indi_algorithm_coding_BER[(*(((uint8_t*)processingUseThisAlgorithm->list)+main_tmp_sel_decAlgo_i))]);

        /* Hard decision error count */
        //FILE **fileio_indi_algorithm_coding_HD_err_bit_cnt;
        //char fileio_Name_indi_algorithm_coding_HD_err_bit_cnt[KIND_OF_BCH_DECODING_END][256]={0};
        sprintf(fileio_Name_indi_algorithm_coding_HD_err_bit_cnt[(*(((uint8_t*)processingUseThisAlgorithm->list)+main_tmp_sel_decAlgo_i))],
            "%s/REPORT_GF(2^%d)_bch(%d,%d)_p%d_t%d_%s_%s_%s_%s_bit_cnt_(Static-HD)%s.txt",
            fileio_Name_Report_Path,
            global_GaloisFieldExponential,

            main_com_codeLength,
            main_com_infoBitLength,
            main_tmp_degErrLocPoly,
            main_com_hd_correctability,

                ((global_flag_sim_decodingMode&FLAG_MASK_CODE_BCH_DEC_AL_TEST_THIS_ALGORITHM)?"algo":
                (global_flag_sim_decodingMode&FLAG_MASK_CODE_BCH_DEC_AL_SOFT_DECISION)?"soft":
                (global_flag_sim_decodingMode&FLAG_MASK_CODE_BCH_DEC_AL_HARD_DECISION)?"hard":"unkn"),

            NAME_FLAG_CASE_SEL_TP_BEFORE_DEC_MTEHOD_FILE_ADDED[global_flag_case_selTP_before_dec],
            NAME_FLAG_CASE_FINDING_MIN_MTEHOD_FILE_ADDED[global_flag_case_find_LLR_method],
            ((global_flag_Channel_Mode_Func&FLAG_MASK_CHANNEL_MODE_FUNC_BITRATE_DISABLE)?"":"bitRate"),
            KIND_OF_BCH_DECODING_ALGORITHM_NAME[(*(((uint8_t*)processingUseThisAlgorithm->list)+main_tmp_sel_decAlgo_i))]
            );
        if(!(fileio_indi_algorithm_coding_HD_err_bit_cnt[(*(((uint8_t*)processingUseThisAlgorithm->list)+main_tmp_sel_decAlgo_i))]=fopen(fileio_Name_indi_algorithm_coding_HD_err_bit_cnt[(*(((uint8_t*)processingUseThisAlgorithm->list)+main_tmp_sel_decAlgo_i))], "w")))
        {
            errorMes;
            printf("Can not open \"%s\"\r\n", fileio_Name_indi_algorithm_coding_HD_err_bit_cnt[(*(((uint8_t*)processingUseThisAlgorithm->list)+main_tmp_sel_decAlgo_i))]);
            return -1;
        }
        infoMes; printf("Enable file I/O \"%s\"\r\n", fileio_Name_indi_algorithm_coding_HD_err_bit_cnt[(*(((uint8_t*)processingUseThisAlgorithm->list)+main_tmp_sel_decAlgo_i))]);

        /* Soft decision error count */
        //FILE **fileio_indi_algorithm_coding_err_bit_cnt;
        //char fileio_Name_indi_algorithm_coding_err_bit_cnt[KIND_OF_BCH_DECODING_END][256]={0};
        sprintf(fileio_Name_indi_algorithm_coding_err_bit_cnt[(*(((uint8_t*)processingUseThisAlgorithm->list)+main_tmp_sel_decAlgo_i))],
            "%s/REPORT_GF(2^%d)_bch(%d,%d)_p%d_t%d_%s_%s_%s_%s_bit_cnt_%s_err.txt",
            fileio_Name_Report_Path,
            global_GaloisFieldExponential,

            main_com_codeLength,
            main_com_infoBitLength,
            main_tmp_degErrLocPoly,
            main_com_hd_correctability,

                ((global_flag_sim_decodingMode&FLAG_MASK_CODE_BCH_DEC_AL_TEST_THIS_ALGORITHM)?"algo":
                (global_flag_sim_decodingMode&FLAG_MASK_CODE_BCH_DEC_AL_SOFT_DECISION)?"soft":
                (global_flag_sim_decodingMode&FLAG_MASK_CODE_BCH_DEC_AL_HARD_DECISION)?"hard":"unkn"),

            NAME_FLAG_CASE_SEL_TP_BEFORE_DEC_MTEHOD_FILE_ADDED[global_flag_case_selTP_before_dec],
            NAME_FLAG_CASE_FINDING_MIN_MTEHOD_FILE_ADDED[global_flag_case_find_LLR_method],
            ((global_flag_Channel_Mode_Func&FLAG_MASK_CHANNEL_MODE_FUNC_BITRATE_DISABLE)?"":"bitRate"),
            KIND_OF_BCH_DECODING_ALGORITHM_NAME[(*(((uint8_t*)processingUseThisAlgorithm->list)+main_tmp_sel_decAlgo_i))]
            );
        if(!(fileio_indi_algorithm_coding_err_bit_cnt[(*(((uint8_t*)processingUseThisAlgorithm->list)+main_tmp_sel_decAlgo_i))]=fopen(fileio_Name_indi_algorithm_coding_err_bit_cnt[(*(((uint8_t*)processingUseThisAlgorithm->list)+main_tmp_sel_decAlgo_i))], "w")))
        {
            errorMes;
            printf("Can not open \"%s\"\r\n", fileio_Name_indi_algorithm_coding_err_bit_cnt[(*(((uint8_t*)processingUseThisAlgorithm->list)+main_tmp_sel_decAlgo_i))]);
            return -1;
        }
        infoMes; printf("Enable file I/O \"%s\"\r\n", fileio_Name_indi_algorithm_coding_err_bit_cnt[(*(((uint8_t*)processingUseThisAlgorithm->list)+main_tmp_sel_decAlgo_i))]);

    }

    /* REPORT channel frame error rate (not surpport yet) */
    //FILE *fileio_channelFER;
    //char fileio_Name_channel_FER[256]={0};
    sprintf(fileio_Name_channel_FER, "%s/REPORT_GF(2^%d)_bch(%d,%d)_p%d_t%d_%s_%s_%s_%s_CH_FER.txt",
        fileio_Name_Report_Path,
        global_GaloisFieldExponential,

        main_com_codeLength,
        main_com_infoBitLength,
        main_tmp_degErrLocPoly,
        main_com_hd_correctability,

            ((global_flag_sim_decodingMode&FLAG_MASK_CODE_BCH_DEC_AL_TEST_THIS_ALGORITHM)?"algo":
            (global_flag_sim_decodingMode&FLAG_MASK_CODE_BCH_DEC_AL_SOFT_DECISION)?"soft":
            (global_flag_sim_decodingMode&FLAG_MASK_CODE_BCH_DEC_AL_HARD_DECISION)?"hard":"unkn"),

        NAME_FLAG_CASE_SEL_TP_BEFORE_DEC_MTEHOD_FILE_ADDED[global_flag_case_selTP_before_dec],
        NAME_FLAG_CASE_FINDING_MIN_MTEHOD_FILE_ADDED[global_flag_case_find_LLR_method],
        ((global_flag_Channel_Mode_Func&FLAG_MASK_CHANNEL_MODE_FUNC_BITRATE_DISABLE)?"":"bitRate")
        );
    if(!(fileio_channelFER=fopen(fileio_Name_channel_FER, "w")))
    {
        errorMes;
        printf("Can not open \"%s\"\r\n", fileio_Name_channel_FER);
        return -1;
    }
    infoMes; printf("Enable file I/O \"%s\"\r\n", fileio_Name_channel_FER);

    /* REPORT frame error rate, recorded all algorithm(not support yet) */
    //FILE *fileio_codingFER;
    //char fileio_Name_coding_FER[256]={0};
    sprintf(fileio_Name_coding_FER, "%s/REPORT_GF(2^%d)_bch(%d,%d)_p%d_t%d_%s_%s_%s_%s_FER.txt",
        fileio_Name_Report_Path,
        global_GaloisFieldExponential,

        main_com_codeLength,
        main_com_infoBitLength,
        main_tmp_degErrLocPoly,
        main_com_hd_correctability,

            ((global_flag_sim_decodingMode&FLAG_MASK_CODE_BCH_DEC_AL_TEST_THIS_ALGORITHM)?"algo":
            (global_flag_sim_decodingMode&FLAG_MASK_CODE_BCH_DEC_AL_SOFT_DECISION)?"soft":
            (global_flag_sim_decodingMode&FLAG_MASK_CODE_BCH_DEC_AL_HARD_DECISION)?"hard":"unkn"),

        NAME_FLAG_CASE_SEL_TP_BEFORE_DEC_MTEHOD_FILE_ADDED[global_flag_case_selTP_before_dec],
        NAME_FLAG_CASE_FINDING_MIN_MTEHOD_FILE_ADDED[global_flag_case_find_LLR_method],
        ((global_flag_Channel_Mode_Func&FLAG_MASK_CHANNEL_MODE_FUNC_BITRATE_DISABLE)?"":"bitRate")
        );
    if(!(fileio_codingFER=fopen(fileio_Name_coding_FER, "w")))
    {
        errorMes;
        printf("Can not open \"%s\"\r\n", fileio_Name_coding_FER);
        return -1;
    }
    infoMes; printf("Enable file I/O \"%s\"\r\n", fileio_Name_coding_FER);

    /* REPORT channel EbN0 */
    //FILE *fildio_EbN0;
    //char fileio_Name_EbN0[256]={0};
    sprintf(fileio_Name_EbN0, "%s/REPORT_GF(2^%d)_bch(%d,%d)_p%d_t%d_%s_%s_%s_%s_EbN0.txt",
        fileio_Name_Report_Path,
        global_GaloisFieldExponential,

        main_com_codeLength,
        main_com_infoBitLength,
        main_tmp_degErrLocPoly,
        main_com_hd_correctability,

            ((global_flag_sim_decodingMode&FLAG_MASK_CODE_BCH_DEC_AL_TEST_THIS_ALGORITHM)?"algo":
            (global_flag_sim_decodingMode&FLAG_MASK_CODE_BCH_DEC_AL_SOFT_DECISION)?"soft":
            (global_flag_sim_decodingMode&FLAG_MASK_CODE_BCH_DEC_AL_HARD_DECISION)?"hard":"unkn"),

        NAME_FLAG_CASE_SEL_TP_BEFORE_DEC_MTEHOD_FILE_ADDED[global_flag_case_selTP_before_dec],
        NAME_FLAG_CASE_FINDING_MIN_MTEHOD_FILE_ADDED[global_flag_case_find_LLR_method],
        ((global_flag_Channel_Mode_Func&FLAG_MASK_CHANNEL_MODE_FUNC_BITRATE_DISABLE)?"":"bitRate")
        );
    if(!(fildio_EbN0=fopen(fileio_Name_EbN0, "w")))
    {
        errorMes;
        printf("Can not open \"%s\"\r\n", fileio_Name_EbN0);
        return -1;
    }
    infoMes; printf("Enable file I/O \"%s\"\r\n", fileio_Name_EbN0);

    /* REPORT Loop count actually it mean codeword passing channel */
    //FILE *fildio_loop_Cnt;
    //char fileio_Name_loop_Cnt[256]={0};
    sprintf(fileio_Name_loop_Cnt, "%s/REPORT_GF(2^%d)_bch(%d,%d)_p%d_t%d_%s_%s_%s_%s_LoopCnt.txt",
        fileio_Name_Report_Path,
        global_GaloisFieldExponential,

        main_com_codeLength,
        main_com_infoBitLength,
        main_tmp_degErrLocPoly,
        main_com_hd_correctability,

            ((global_flag_sim_decodingMode&FLAG_MASK_CODE_BCH_DEC_AL_TEST_THIS_ALGORITHM)?"algo":
            (global_flag_sim_decodingMode&FLAG_MASK_CODE_BCH_DEC_AL_SOFT_DECISION)?"soft":
            (global_flag_sim_decodingMode&FLAG_MASK_CODE_BCH_DEC_AL_HARD_DECISION)?"hard":"unkn"),

        NAME_FLAG_CASE_SEL_TP_BEFORE_DEC_MTEHOD_FILE_ADDED[global_flag_case_selTP_before_dec],
        NAME_FLAG_CASE_FINDING_MIN_MTEHOD_FILE_ADDED[global_flag_case_find_LLR_method],
        ((global_flag_Channel_Mode_Func&FLAG_MASK_CHANNEL_MODE_FUNC_BITRATE_DISABLE)?"":"bitRate")
        );
    if(!(fildio_loop_Cnt=fopen(fileio_Name_loop_Cnt, "w")))
    {
        errorMes;
        printf("Can not open \"%s\"\r\n", fileio_Name_loop_Cnt);
        return -1;
    }
    infoMes; printf("Enable file I/O \"%s\"\r\n", fileio_Name_loop_Cnt);

    /* REPORT simulation environment(setted values record) */
    //FILE *fileio_simEnv;
    //char fileio_Name_simEnv[256]={0};
    sprintf(fileio_Name_simEnv, "%s/REPORT_GF(2^%d)_bch(%d,%d)_p%d_t%d_%s_%s_%s_%s_SimEnv.txt",
        fileio_Name_Report_Path,
        global_GaloisFieldExponential,

        main_com_codeLength,
        main_com_infoBitLength,
        main_tmp_degErrLocPoly,
        main_com_hd_correctability,

            ((global_flag_sim_decodingMode&FLAG_MASK_CODE_BCH_DEC_AL_TEST_THIS_ALGORITHM)?"algo":
            (global_flag_sim_decodingMode&FLAG_MASK_CODE_BCH_DEC_AL_SOFT_DECISION)?"soft":
            (global_flag_sim_decodingMode&FLAG_MASK_CODE_BCH_DEC_AL_HARD_DECISION)?"hard":"unkn"),

        NAME_FLAG_CASE_SEL_TP_BEFORE_DEC_MTEHOD_FILE_ADDED[global_flag_case_selTP_before_dec],
        NAME_FLAG_CASE_FINDING_MIN_MTEHOD_FILE_ADDED[global_flag_case_find_LLR_method],
        ((global_flag_Channel_Mode_Func&FLAG_MASK_CHANNEL_MODE_FUNC_BITRATE_DISABLE)?"":"bitRate")
        );
    if(!(fileio_simEnv=fopen(fileio_Name_simEnv, "w")))
    {
        errorMes;
        printf("Can not open \"%s\"\r\n", fileio_Name_simEnv);
        return -1;
    }
    infoMes; printf("Enable file I/O \"%s\"\r\n", fileio_Name_simEnv);


    /* configuration of test pattern mes start */
    printf("/*****************************************************************/\r\n");
    printf("/************************* Pattern Files *************************/\r\n");
    printf("/*****************************************************************/\r\n");
    /******************************************************/
    /*********************** Pattern **********************/
    /******************************************************/
    /* sample code */
    /*
    //sprintf(fileio_Name_total_bit_cnt, "%s/PATTERB_GF(2^%d)_bch(%d,%d)_p%d_t%d_%s_%s_%s_%s_bit_cnt_total.txt",
    //    fileio_Name_Pattern_Path,
    //    global_GaloisFieldExponential,

    //    main_com_codeLength,
    //    main_com_infoBitLength,
    //    main_tmp_degErrLocPoly,
    //    main_com_hd_correctability,

    //        ((global_flag_sim_decodingMode&FLAG_MASK_CODE_BCH_DEC_AL_TEST_THIS_ALGORITHM)?"algo":
    //        (global_flag_sim_decodingMode&FLAG_MASK_CODE_BCH_DEC_AL_SOFT_DECISION)?"soft":
    //        (global_flag_sim_decodingMode&FLAG_MASK_CODE_BCH_DEC_AL_HARD_DECISION)?"hard":"unkn"),

    //    NAME_FLAG_CASE_SEL_TP_BEFORE_DEC_MTEHOD_FILE_ADDED[global_flag_case_selTP_before_dec],
    //    NAME_FLAG_CASE_FINDING_MIN_MTEHOD_FILE_ADDED[global_flag_case_find_LLR_method],
    //    ((global_flag_Channel_Mode_Func&FLAG_MASK_CHANNEL_MODE_FUNC_BITRATE_DISABLE)?"":"bitRate")
    //    );
    //if(!(fileio_total_bit_cnt=fopen(fileio_Name_total_bit_cnt, "w")))
    //{
    //    errorMes;
    //    printf("Can not open \"%s\"\r\n", fileio_Name_total_bit_cnt);
    //    return -1;
    //}
    //infoMes; printf("Enable file I/O \"%s\"\r\n", fileio_Name_total_bit_cnt);
    */

    /* PATTERN infomation(origin data) bit stream */
    if(global_flag_file_io_hd_pattern_output&FLAG_MASK_FILE_IO_HD_MES)
    {
        sprintf(fileio_Name_HD_mes, "%s/PATTERN_GF_2p%d_bch_%d,%d_p%d_t%d_mes.bin",
            fileio_Name_Pattern_Path,
            global_GaloisFieldExponential,

            main_com_codeLength,
            main_com_infoBitLength,
            main_tmp_degErrLocPoly,
            main_com_hd_correctability
        );
        if(!(fileio_HD_mes=fopen(fileio_Name_HD_mes, "w")))
        {
            errorMes;
            printf("Can not open \"%s\"\r\n", fileio_Name_HD_mes);
            return -1;
        }
        infoMes; printf("Enable file I/O \"%s\"\r\n", fileio_Name_HD_mes);
    }
    else
    {
        infoMes; printf("infomation bit stream output is disabled.\r\n");
    }
    /* PATTERN encoded bit stream */
    if(global_flag_file_io_hd_pattern_output&FLAG_MASK_FILE_IO_HD_ENCODING_MES)
    {
        sprintf(fileio_Name_HD_encoding_mes, "%s/PATTERN_GF_2p%d_bch_%d,%d_p%d_t%d_encoding.bin",
            fileio_Name_Pattern_Path,
            global_GaloisFieldExponential,

            main_com_codeLength,
            main_com_infoBitLength,
            main_tmp_degErrLocPoly,
            main_com_hd_correctability
        );
        if(!(fileio_HD_encoding_mes=fopen(fileio_Name_HD_encoding_mes, "w")))
        {
            errorMes;
            printf("Can not open \"%s\"\r\n", fileio_Name_HD_encoding_mes);
            return -1;
        }
        infoMes; printf("Enable file I/O \"%s\"\r\n", fileio_Name_HD_encoding_mes);
    }
    else
    {
        infoMes; printf("encoding bit stream output is disabled.\r\n");
    }
    /* PATTERN received bit stream(hard decision) */
    if(global_flag_file_io_hd_pattern_output&FLAG_MASK_FILE_IO_HD_RECEIVED_MES)
    {
        sprintf(fileio_Name_HD_received_mes, "%s/PATTERN_GF_2p%d_bch_%d,%d_p%d_t%d_receive(HD).bin",
            fileio_Name_Pattern_Path,
            global_GaloisFieldExponential,

            main_com_codeLength,
            main_com_infoBitLength,
            main_tmp_degErrLocPoly,
            main_com_hd_correctability
        );
        if(!(fileio_HD_received_mes=fopen(fileio_Name_HD_received_mes, "w")))
        {
            errorMes;
            printf("Can not open \"%s\"\r\n", fileio_Name_HD_received_mes);
            return -1;
        }
        infoMes; printf("Enable file I/O \"%s\"\r\n", fileio_Name_HD_received_mes);
    }
    else
    {
        infoMes; printf("received bit stream(hard decision) output is disabled.\r\n");
    }
    /* PATTERN received LLR bit stream(quantized bit) */
    if(global_flag_file_io_sd_pattern_output&FLAG_MASK_FILE_IO_SD_RECEIVED_LLR)
    {
        sprintf(fileio_Name_SD_received_LLR, "%s/PATTERN_GF_2p%d_bch_%d,%d_p%d_t%d_quantized_LLR.hex",
            fileio_Name_Pattern_Path,
            global_GaloisFieldExponential,

            main_com_codeLength,
            main_com_infoBitLength,
            main_tmp_degErrLocPoly,
            main_com_hd_correctability
        );
        if(!(fileio_SD_received_LLR=fopen(fileio_Name_SD_received_LLR, "w")))
        {
            errorMes;
            printf("Can not open \"%s\"\r\n", fileio_Name_SD_received_LLR);
            return -1;
        }
        infoMes; printf("Enable file I/O \"%s\"\r\n", fileio_Name_SD_received_LLR);
    }
    else
    {
        infoMes; printf("received LLR bit stream output is disabled.\r\n");
    }
    /* PATTERN received LLR magnitude bit stream(quantized bit) */
    if(global_flag_file_io_sd_pattern_output&FLAG_MASK_FILE_IO_SD_RECEIVED_LLR_mag)
    {
        sprintf(fileio_Name_SD_received_LLR_mag, "%s/PATTERN_GF_2p%d_bch_%d,%d_p%d_t%d_quantized_LLR_mag.hex",
            fileio_Name_Pattern_Path,
            global_GaloisFieldExponential,

            main_com_codeLength,
            main_com_infoBitLength,
            main_tmp_degErrLocPoly,
            main_com_hd_correctability
        );
        if(!(fileio_SD_received_LLR_mag=fopen(fileio_Name_SD_received_LLR_mag, "w")))
        {
            errorMes;
            printf("Can not open \"%s\"\r\n", fileio_Name_SD_received_LLR_mag);
            return -1;
        }
        infoMes; printf("Enable file I/O \"%s\"\r\n", fileio_Name_SD_received_LLR_mag);
    }
    else
    {
        infoMes; printf("received LLR magnitude bit stream output is disabled.\r\n");
    }

    /* PATTERN corrected bit stream(depend on algorithm) */
    if(global_flag_file_io_hd_pattern_output&FLAG_MASK_FILE_IO_HD_CORRECTED_MES)
    {
        for(main_tmp_sel_decAlgo_i=0; main_tmp_sel_decAlgo_i<processingUseThisAlgorithm->length; main_tmp_sel_decAlgo_i++)
        {
            sprintf(fileio_Name_corrected_mes\
                    [(*(((uint8_t*)processingUseThisAlgorithm->list)+main_tmp_sel_decAlgo_i))],\
                    "%s/PATTERN_GF_2p%d_bch_%d,%d_p%d_t%d_%s_%s_%s_%s_corrected.bin",\
                fileio_Name_Pattern_Path,
                global_GaloisFieldExponential,

                main_com_codeLength,
                main_com_infoBitLength,
                main_tmp_degErrLocPoly,
                main_com_hd_correctability,

                ((global_flag_sim_decodingMode&FLAG_MASK_CODE_BCH_DEC_AL_TEST_THIS_ALGORITHM)?"algo":
                    (global_flag_sim_decodingMode&FLAG_MASK_CODE_BCH_DEC_AL_SOFT_DECISION)?"soft":
                    (global_flag_sim_decodingMode&FLAG_MASK_CODE_BCH_DEC_AL_HARD_DECISION)?"hard":"unkn"),

                NAME_FLAG_CASE_SEL_TP_BEFORE_DEC_MTEHOD_FILE_ADDED[global_flag_case_selTP_before_dec],
                NAME_FLAG_CASE_FINDING_MIN_MTEHOD_FILE_ADDED[global_flag_case_find_LLR_method],
                ((global_flag_Channel_Mode_Func&FLAG_MASK_CHANNEL_MODE_FUNC_BITRATE_DISABLE)?"":"bitRate")
            );

            if(!(fileio_corrected_mes\
                        [(*(((uint8_t*)processingUseThisAlgorithm->list)+main_tmp_sel_decAlgo_i))]\
                        =fopen(fileio_Name_corrected_mes\
                            [(*(((uint8_t*)processingUseThisAlgorithm->list)+main_tmp_sel_decAlgo_i))], \
                            "w")))
            {
                errorMes;
                printf("Can not open \"%s\"\r\n", fileio_Name_corrected_mes[(*(((uint8_t*)processingUseThisAlgorithm->list)+main_tmp_sel_decAlgo_i))]);
                return -1;
            }
            infoMes; printf("Enable file I/O \"%s\"\r\n", fileio_Name_corrected_mes[(*(((uint8_t*)processingUseThisAlgorithm->list)+main_tmp_sel_decAlgo_i))]);
        }
    }
    else
    {
        infoMes; printf("corrected bit stream output is disabled.\r\n");
    }

    printf("/*****************************************************************/\r\n");
    printf("/******************* Verilog Pattern Files ***********************/\r\n");
    printf("/*****************************************************************/\r\n");
    /******************************************************/
    /*********************** Pattern **********************/
    /******************************************************/
    /* sample code */
    /*
    //sprintf(fileio_Name_total_bit_cnt, "%s/PATTERB_GF(2^%d)_bch(%d,%d)_p%d_t%d_%s_%s_%s_%s_bit_cnt_total.txt",
    //    fileio_Name_Pattern_Path,
    //    global_GaloisFieldExponential,

    //    main_com_codeLength,
    //    main_com_infoBitLength,
    //    main_tmp_degErrLocPoly,
    //    main_com_hd_correctability,

    //        ((global_flag_sim_decodingMode&FLAG_MASK_CODE_BCH_DEC_AL_TEST_THIS_ALGORITHM)?"algo":
    //        (global_flag_sim_decodingMode&FLAG_MASK_CODE_BCH_DEC_AL_SOFT_DECISION)?"soft":
    //        (global_flag_sim_decodingMode&FLAG_MASK_CODE_BCH_DEC_AL_HARD_DECISION)?"hard":"unkn"),

    //        NAME_FLAG_CASE_SEL_TP_BEFORE_DEC_MTEHOD_FILE_ADDED[global_flag_case_selTP_before_dec],
    //        NAME_FLAG_CASE_FINDING_MIN_MTEHOD_FILE_ADDED[global_flag_case_find_LLR_method],
    //    ((global_flag_Channel_Mode_Func&FLAG_MASK_CHANNEL_MODE_FUNC_BITRATE_DISABLE)?"":"bitRate")
    //    );
    //if(!(fileio_total_bit_cnt=fopen(fileio_Name_total_bit_cnt, "w")))
    //{
    //    errorMes;
    //    printf("Can not open \"%s\"\r\n", fileio_Name_total_bit_cnt);
    //    return -1;
    //}
    //infoMes; printf("Enable file I/O \"%s\"\r\n", fileio_Name_total_bit_cnt);
    */

    /* PATTERN infomation(origin data) bit stream */
    if(global_flag_file_io_hd_pattern_output_verilog&FLAG_MASK_FILE_IO_HD_MES)
    {
        sprintf(fileio_Name_HD_mes_verilog, "%s/VERILOG_GF_2p%d_bch_%d,%d_p%d_t%d_mes.bin",
            fileio_Name_Pattern_Path,
            global_GaloisFieldExponential,

            main_com_codeLength,
            main_com_infoBitLength,
            main_tmp_degErrLocPoly,
            main_com_hd_correctability
        );
        if(!(fileio_HD_mes_verilog=fopen(fileio_Name_HD_mes_verilog, "w")))
        {
            errorMes;
            printf("Can not open \"%s\"\r\n", fileio_Name_HD_mes_verilog);
            return -1;
        }
        infoMes; printf("Enable file I/O \"%s\"\r\n", fileio_Name_HD_mes_verilog);
    }
    else
    {
        infoMes; printf("infomation bit stream output is disabled.\r\n");
    }
    /* PATTERN encoded bit stream */
    if(global_flag_file_io_hd_pattern_output_verilog&FLAG_MASK_FILE_IO_HD_ENCODING_MES)
    {
        sprintf(fileio_Name_HD_encoding_mes_verilog, "%s/VERILOG_GF_2p%d_bch_%d,%d_p%d_t%d_encoding.bin",
            fileio_Name_Pattern_Path,
            global_GaloisFieldExponential,

            main_com_codeLength,
            main_com_infoBitLength,
            main_tmp_degErrLocPoly,
            main_com_hd_correctability
        );
        if(!(fileio_HD_encoding_mes_verilog=fopen(fileio_Name_HD_encoding_mes_verilog, "w")))
        {
            errorMes;
            printf("Can not open \"%s\"\r\n", fileio_Name_HD_encoding_mes_verilog);
            return -1;
        }
        infoMes; printf("Enable file I/O \"%s\"\r\n", fileio_Name_HD_encoding_mes_verilog);
    }
    else
    {
        infoMes; printf("encoding bit stream output is disabled.\r\n");
    }
    /* PATTERN received bit stream(hard decision) */
    if(global_flag_file_io_hd_pattern_output_verilog&FLAG_MASK_FILE_IO_HD_RECEIVED_MES)
    {
        sprintf(fileio_Name_HD_received_mes_verilog, "%s/VERILOG_GF_2p%d_bch_%d,%d_p%d_t%d_receive(HD).bin",
            fileio_Name_Pattern_Path,
            global_GaloisFieldExponential,

            main_com_codeLength,
            main_com_infoBitLength,
            main_tmp_degErrLocPoly,
            main_com_hd_correctability
        );
        if(!(fileio_HD_received_mes_verilog=fopen(fileio_Name_HD_received_mes_verilog, "w")))
        {
            errorMes;
            printf("Can not open \"%s\"\r\n", fileio_Name_HD_received_mes_verilog);
            return -1;
        }
        infoMes; printf("Enable file I/O \"%s\"\r\n", fileio_Name_HD_received_mes_verilog);
    }
    else
    {
        infoMes; printf("received bit stream(hard decision) output is disabled.\r\n");
    }
    /* PATTERN received LLR bit stream(quantized bit) */
    if(global_flag_file_io_sd_pattern_output_verilog&FLAG_MASK_FILE_IO_SD_RECEIVED_LLR)
    {
        sprintf(fileio_Name_SD_received_LLR_verilog, "%s/VERILOG_GF_2p%d_bch_%d,%d_p%d_t%d_quantized_LLR.hex",
            fileio_Name_Pattern_Path,
            global_GaloisFieldExponential,

            main_com_codeLength,
            main_com_infoBitLength,
            main_tmp_degErrLocPoly,
            main_com_hd_correctability
        );
        if(!(fileio_SD_received_LLR_verilog=fopen(fileio_Name_SD_received_LLR_verilog, "w")))
        {
            errorMes;
            printf("Can not open \"%s\"\r\n", fileio_Name_SD_received_LLR_verilog);
            return -1;
        }
        infoMes; printf("Enable file I/O \"%s\"\r\n", fileio_Name_SD_received_LLR_verilog);
    }
    else
    {
        infoMes; printf("received LLR bit stream output is disabled.\r\n");
    }
    /* PATTERN received LLR magnitude bit stream(quantized bit) */
    if(global_flag_file_io_sd_pattern_output_verilog&FLAG_MASK_FILE_IO_SD_RECEIVED_LLR_mag)
    {
        sprintf(fileio_Name_SD_received_LLR_mag_verilog, "%s/VERILOG_GF_2p%d_bch_%d,%d_p%d_t%d_quantized_LLR_mag.hex",
            fileio_Name_Pattern_Path,
            global_GaloisFieldExponential,

            main_com_codeLength,
            main_com_infoBitLength,
            main_tmp_degErrLocPoly,
            main_com_hd_correctability
        );
        if(!(fileio_SD_received_LLR_mag_verilog=fopen(fileio_Name_SD_received_LLR_mag_verilog, "w")))
        {
            errorMes;
            printf("Can not open \"%s\"\r\n", fileio_Name_SD_received_LLR_mag_verilog);
            return -1;
        }
        infoMes; printf("Enable file I/O \"%s\"\r\n", fileio_Name_SD_received_LLR_mag_verilog);
    }
    else
    {
        infoMes; printf("received LLR magnitude bit stream output is disabled.\r\n");
    }

    /* PATTERN corrected bit stream(depend on algorithm) */
    if(global_flag_file_io_hd_pattern_output_verilog&FLAG_MASK_FILE_IO_HD_CORRECTED_MES)
    {
        for(main_tmp_sel_decAlgo_i=0; main_tmp_sel_decAlgo_i<processingUseThisAlgorithm->length; main_tmp_sel_decAlgo_i++)
        {
            sprintf(fileio_Name_corrected_mes_verilog\
                    [(*(((uint8_t*)processingUseThisAlgorithm->list)+main_tmp_sel_decAlgo_i))],\
                    "%s/VERILOG_GF_2p%d_bch_%d,%d_p%d_t%d_%s_%s_%s_%s_corrected.bin",\
                fileio_Name_Pattern_Path,
                global_GaloisFieldExponential,

                main_com_codeLength,
                main_com_infoBitLength,
                main_tmp_degErrLocPoly,
                main_com_hd_correctability,

                ((global_flag_sim_decodingMode&FLAG_MASK_CODE_BCH_DEC_AL_TEST_THIS_ALGORITHM)?"algo":
                    (global_flag_sim_decodingMode&FLAG_MASK_CODE_BCH_DEC_AL_SOFT_DECISION)?"soft":
                    (global_flag_sim_decodingMode&FLAG_MASK_CODE_BCH_DEC_AL_HARD_DECISION)?"hard":"unkn"),

                NAME_FLAG_CASE_SEL_TP_BEFORE_DEC_MTEHOD_FILE_ADDED[global_flag_case_selTP_before_dec],
                NAME_FLAG_CASE_FINDING_MIN_MTEHOD_FILE_ADDED[global_flag_case_find_LLR_method],
                ((global_flag_Channel_Mode_Func&FLAG_MASK_CHANNEL_MODE_FUNC_BITRATE_DISABLE)?"":"bitRate")
            );

            if(!(fileio_corrected_mes_verilog\
                        [(*(((uint8_t*)processingUseThisAlgorithm->list)+main_tmp_sel_decAlgo_i))]\
                        =fopen(fileio_Name_corrected_mes_verilog\
                            [(*(((uint8_t*)processingUseThisAlgorithm->list)+main_tmp_sel_decAlgo_i))], \
                            "w")))
            {
                errorMes;
                printf("Can not open \"%s\"\r\n", fileio_Name_corrected_mes_verilog[(*(((uint8_t*)processingUseThisAlgorithm->list)+main_tmp_sel_decAlgo_i))]);
                return -1;
            }
            infoMes; printf("Enable file I/O \"%s\"\r\n", fileio_Name_corrected_mes_verilog[(*(((uint8_t*)processingUseThisAlgorithm->list)+main_tmp_sel_decAlgo_i))]);
        }
    }
    else
    {
        infoMes; printf("corrected bit stream output is disabled.\r\n");
    }


    printf("/*****************************************************************/\r\n");
    printf("/*********************** Pattern log Files ***********************/\r\n");
    printf("/*****************************************************************/\r\n");
    /******************************************************/
    /******************** Pattern Log ********************/
    /******************************************************/
    /* PATTERN LOG infomation(origin data) bit stream */
    if(global_flag_file_io_hd_pattern_output_log&FLAG_MASK_FILE_IO_HD_MES)
    {
        sprintf(fileio_Name_HD_mes_log, "%s/PATTERN_LOG_GF(2^%d)_bch(%d,%d)_p%d_t%d_mes.log",
            fileio_Name_PatternLog_Path,
            global_GaloisFieldExponential,

            main_com_codeLength,
            main_com_infoBitLength,
            main_tmp_degErrLocPoly,
            main_com_hd_correctability
        );
        if(!(fileio_HD_mes_log=fopen(fileio_Name_HD_mes_log, "w")))
        {
            errorMes;
            printf("Can not open \"%s\"\r\n", fileio_Name_HD_mes_log);
            return -1;
        }
        infoMes; printf("Enable file I/O \"%s\"\r\n", fileio_Name_HD_mes_log);
    }
    else
    {
        infoMes; printf("infomation bit stream logging is disabled.\r\n");
    }
    /* PATTERN LOG encoded bit stream */
    if(global_flag_file_io_hd_pattern_output_log&FLAG_MASK_FILE_IO_HD_ENCODING_MES)
    {
        sprintf(fileio_Name_HD_encoding_mes_log, "%s/PATTERN_LOG_GF(2^%d)_bch(%d,%d)_p%d_t%d_encoding.log",
            fileio_Name_PatternLog_Path,
            global_GaloisFieldExponential,

            main_com_codeLength,
            main_com_infoBitLength,
            main_tmp_degErrLocPoly,
            main_com_hd_correctability
        );
        if(!(fileio_HD_encoding_mes_log=fopen(fileio_Name_HD_encoding_mes_log, "w")))
        {
            errorMes;
            printf("Can not open \"%s\"\r\n", fileio_Name_HD_encoding_mes_log);
            return -1;
        }
        infoMes; printf("Enable file I/O \"%s\"\r\n", fileio_Name_HD_encoding_mes_log);
    }
    else
    {
        infoMes; printf("encoding bit stream logging is disabled.\r\n");
    }
    /* PATTERN LOG received bit stream(hard decision) */
    if(global_flag_file_io_hd_pattern_output_log&FLAG_MASK_FILE_IO_HD_RECEIVED_MES)
    {
        sprintf(fileio_Name_HD_received_mes_log, "%s/PATTERN_LOG_GF(2^%d)_bch(%d,%d)_p%d_t%d_receive(HD).log",
            fileio_Name_PatternLog_Path,
            global_GaloisFieldExponential,

            main_com_codeLength,
            main_com_infoBitLength,
            main_tmp_degErrLocPoly,
            main_com_hd_correctability
        );
        if(!(fileio_HD_received_mes_log=fopen(fileio_Name_HD_received_mes_log, "w")))
        {
            errorMes;
            printf("Can not open \"%s\"\r\n", fileio_Name_HD_received_mes_log);
            return -1;
        }
        infoMes; printf("Enable file I/O \"%s\"\r\n", fileio_Name_HD_received_mes_log);
    }
    else
    {
        infoMes; printf("received bit stream(hard decision) logginh is disabled.\r\n");
    }
    /* PATTERN LOG received LLR bit stream(quantized bit) */
    if(global_flag_file_io_sd_pattern_output_log&FLAG_MASK_FILE_IO_SD_RECEIVED_LLR)
    {
        sprintf(fileio_Name_SD_received_LLR_log, "%s/PATTERN_LOG_GF(2^%d)_bch(%d,%d)_p%d_t%d_quantized_LLR.log",
            fileio_Name_PatternLog_Path,
            global_GaloisFieldExponential,

            main_com_codeLength,
            main_com_infoBitLength,
            main_tmp_degErrLocPoly,
            main_com_hd_correctability
        );
        if(!(fileio_SD_received_LLR_log=fopen(fileio_Name_SD_received_LLR_log, "w")))
        {
            errorMes;
            printf("Can not open \"%s\"\r\n", fileio_Name_SD_received_LLR_log);
            return -1;
        }
        infoMes; printf("Enable file I/O \"%s\"\r\n", fileio_Name_SD_received_LLR_log);
    }
    else
    {
        infoMes; printf("received LLR bit stream logging is disabled.\r\n");
    }
    /* Error Cnt */
    if(global_flag_file_io_sd_pattern_output_log&FLAG_MASK_FILE_IO_SD_RECEIVED_LLR)
    {
        sprintf(fileio_Name_ErrCnt_log, "%s/ERROR_LOG_GF(2^%d)_bch(%d,%d)_p%d_t%d.log",
            fileio_Name_PatternLog_Path,
            global_GaloisFieldExponential,

            main_com_codeLength,
            main_com_infoBitLength,
            main_tmp_degErrLocPoly,
            main_com_hd_correctability
        );
        if(!(fileio_ErrCnt_log=fopen(fileio_Name_ErrCnt_log, "w")))
        {
            errorMes;
            printf("Can not open \"%s\"\r\n", fileio_Name_ErrCnt_log);
            return -1;
        }
        infoMes; printf("Enable file I/O \"%s\"\r\n", fileio_Name_ErrCnt_log);
    }
    else
    {
        infoMes; printf("received LLR bit stream logging is disabled.\r\n");
    }
    /* PATTERN LOG received LLR magnitude bit stream(quantized bit) */
    if(global_flag_file_io_sd_pattern_output_log&FLAG_MASK_FILE_IO_SD_RECEIVED_LLR_mag)
    {
        sprintf(fileio_Name_SD_received_LLR_mag_log, "%s/PATTERN_LOG_GF(2^%d)_bch(%d,%d)_p%d_t%d_quantized_LLR_mag.log",
            fileio_Name_PatternLog_Path,
            global_GaloisFieldExponential,

            main_com_codeLength,
            main_com_infoBitLength,
            main_tmp_degErrLocPoly,
            main_com_hd_correctability
        );
        if(!(fileio_SD_received_LLR_mag_log=fopen(fileio_Name_SD_received_LLR_mag_log, "w")))
        {
            errorMes;
            printf("Can not open \"%s\"\r\n", fileio_Name_SD_received_LLR_mag_log);
            return -1;
        }
        infoMes; printf("Enable file I/O \"%s\"\r\n", fileio_Name_SD_received_LLR_mag_log);
    }
    else
    {
        infoMes; printf("received LLR bit stream logging is disabled.\r\n");
    }

    /* PATTERN LOG corrected bit stream(depend on algorithm) */
    if(global_flag_file_io_hd_pattern_output_log&FLAG_MASK_FILE_IO_HD_CORRECTED_MES)
    {
        for(main_tmp_sel_decAlgo_i=0; main_tmp_sel_decAlgo_i<processingUseThisAlgorithm->length; main_tmp_sel_decAlgo_i++)
        {
            sprintf(fileio_Name_corrected_mes_log\
                    [(*(((uint8_t*)processingUseThisAlgorithm->list)+main_tmp_sel_decAlgo_i))],\
                    "%s/PATTERN_LOG_GF(2^%d)_bch(%d,%d)_p%d_t%d_%s_%s_%s_%s_corrected.log",\
                fileio_Name_PatternLog_Path,
                global_GaloisFieldExponential,

                main_com_codeLength,
                main_com_infoBitLength,
                main_tmp_degErrLocPoly,
                main_com_hd_correctability,

                ((global_flag_sim_decodingMode&FLAG_MASK_CODE_BCH_DEC_AL_TEST_THIS_ALGORITHM)?"algo":
                    (global_flag_sim_decodingMode&FLAG_MASK_CODE_BCH_DEC_AL_SOFT_DECISION)?"soft":
                    (global_flag_sim_decodingMode&FLAG_MASK_CODE_BCH_DEC_AL_HARD_DECISION)?"hard":"unkn"),

                NAME_FLAG_CASE_SEL_TP_BEFORE_DEC_MTEHOD_FILE_ADDED[global_flag_case_selTP_before_dec],
                NAME_FLAG_CASE_FINDING_MIN_MTEHOD_FILE_ADDED[global_flag_case_find_LLR_method],
                ((global_flag_Channel_Mode_Func&FLAG_MASK_CHANNEL_MODE_FUNC_BITRATE_DISABLE)?"":"bitRate")
            );

            if(!(fileio_corrected_mes_log\
                        [(*(((uint8_t*)processingUseThisAlgorithm->list)+main_tmp_sel_decAlgo_i))]\
                        =fopen(fileio_Name_corrected_mes_log\
                            [(*(((uint8_t*)processingUseThisAlgorithm->list)+main_tmp_sel_decAlgo_i))], \
                            "w"))\
            )
            {
                errorMes;
                printf("Can not open \"%s\"\r\n", fileio_Name_corrected_mes_log\
                        [(*(((uint8_t*)processingUseThisAlgorithm->list)+main_tmp_sel_decAlgo_i))]\
                        );
                return -1;
            }
            infoMes; printf("Enable file I/O \"%s\"\r\n", fileio_Name_corrected_mes_log\
                    [(*(((uint8_t*)processingUseThisAlgorithm->list)+main_tmp_sel_decAlgo_i))]\
                    );
        }
    }
    else
    {
        infoMes; printf("corrected bit stream logging is disabled.\r\n");
    }
    /* configuration of test pattern mes end */


    /* Test Mode Log File IO*/
    if(global_flag_case_sim_testOpt)
    {
        sprintf(tmp_fileio_target_dir_path, "%s/%s", fileio_Name_Output_RootPath, "TestModeLog");
        if(!(tmp_dirio_var = opendir(tmp_fileio_target_dir_path)))
        {
            mkdir(tmp_fileio_target_dir_path, 0775);
            tmp_dirio_var = opendir(tmp_fileio_target_dir_path);
        }
        closedir(tmp_dirio_var);

        sprintf(tmp_fileio_target_file_path, "%s/%s", tmp_fileio_target_dir_path, "testLog.log");
        infoMes; printf("Enable file I/O \"%s\"\r\n", tmp_fileio_target_file_path);
        fp_testMode_log = fopen(tmp_fileio_target_file_path, "w");


        sprintf(tmp_fileio_target_file_path, "%s/%s", tmp_fileio_target_dir_path, "test_sum_bit_cnt_arranged_by_bitRatio.csv");
        infoMes; printf("Enable file I/O \"%s\"\r\n", tmp_fileio_target_file_path);
        fp_testMode_sum_bit_cnt_arranged_by_bitRetio_csv = fopen(tmp_fileio_target_file_path, "w");

        sprintf(tmp_fileio_target_file_path, "%s/%s", tmp_fileio_target_dir_path, "test_sum_bit_cnt_classified_arranged_by_bitRatio.csv");
        infoMes; printf("Enable file I/O \"%s\"\r\n", tmp_fileio_target_file_path);
        fp_testMode_sum_bit_cnt_classified_arranged_by_bitRatio_csv = fopen(tmp_fileio_target_file_path, "w");

        sprintf(tmp_fileio_target_file_path, "%s/%s", tmp_fileio_target_dir_path, "test_sum_llr_mag_arranged_by_bitRatio.csv");
        infoMes; printf("Enable file I/O \"%s\"\r\n", tmp_fileio_target_file_path);
        fp_testMode_sum_llr_mag_arranged_by_bitRatio_csv = fopen(tmp_fileio_target_file_path, "w");

        sprintf(tmp_fileio_target_file_path, "%s/%s", tmp_fileio_target_dir_path, "test_sum_err_bit_cnt_chg_classified_arranged_by_bitRatio.csv");
        infoMes; printf("Enable file I/O \"%s\"\r\n", tmp_fileio_target_file_path);
        fp_testMode_sum_err_bit_cnt_chg_classified_arranged_by_bitRatio_csv = fopen(tmp_fileio_target_file_path, "w");

        sprintf(tmp_fileio_target_file_path, "%s/%s", tmp_fileio_target_dir_path, "test_all_arranged_by_bitRatio.csv");
        infoMes; printf("Enable file I/O \"%s\"\r\n", tmp_fileio_target_file_path);
        fp_testMode_all_arranged_by_bitRatio_csv = fopen(tmp_fileio_target_file_path, "w");

        sprintf(tmp_fileio_target_file_path, "%s/%s", tmp_fileio_target_dir_path, "test_sum_codeword_cnt_arranged_by_errorNums.csv");
        infoMes; printf("Enable file I/O \"%s\"\r\n", tmp_fileio_target_file_path);
        fp_testMode_sum_codeword_cnt_arranged_by_errorNums_csv = fopen(tmp_fileio_target_file_path, "w");

        sprintf(tmp_fileio_target_file_path, "%s/%s", tmp_fileio_target_dir_path, "test_sum_llr_mag_distribution_cnt_arranged_by_errorNums.csv");
        infoMes; printf("Enable file I/O \"%s\"\r\n", tmp_fileio_target_file_path);
        fp_testMode_sum_llr_mag_distribution_cnt_arranged_by_errorNums_csv = fopen(tmp_fileio_target_file_path, "w");

        sprintf(tmp_fileio_target_file_path, "%s/%s", tmp_fileio_target_dir_path, "test_sum_bit_ratio_cnt_arranged_by_errorNums.csv");
        infoMes; printf("Enable file I/O \"%s\"\r\n", tmp_fileio_target_file_path);
        fp_testMode_sum_bit_ratio_cnt_arranged_by_errorNums_csv = fopen(tmp_fileio_target_file_path, "w");

        sprintf(tmp_fileio_target_file_path, "%s/%s", tmp_fileio_target_dir_path, "test_all_arranged_by_errorNums.csv");
        infoMes; printf("Enable file I/O \"%s\"\r\n", tmp_fileio_target_file_path);
        fp_testMode_all_arranged_by_errorNums_csv = fopen(tmp_fileio_target_file_path, "w");

    }


    fprintf(fileio_simEnv,"/*****************************************************************/\r\n");
    fprintf(fileio_simEnv,"/**************** Applied global variables values ****************/\r\n");
    fprintf(fileio_simEnv,"/*****************************************************************/\r\n");
    fprintf(fileio_simEnv,"[Global Variables]\r\n");
    fprintf(fileio_simEnv,"AWGN Coderate is %s\r\n", ((global_flag_Channel_Mode_Func&FLAG_MASK_CHANNEL_MODE_FUNC_BITRATE_DISABLE)?"disabled":"enabled"));

    fprintf(fileio_simEnv,"EbN0 start : %g\r\n",global_EbN0_Start_Value);
    fprintf(fileio_simEnv,"EbN0 end : %g\r\n",global_EbN0_End_Value);
    fprintf(fileio_simEnv,"EbN0 step : %g\r\n",global_EbN0_Step);

    fprintf(fileio_simEnv,"Error number : %d\r\n",global_ErrorNum);
    fprintf(fileio_simEnv,"Galois Field exponential : %d\r\n",global_GaloisFieldExponential);
    fprintf(fileio_simEnv,"Code length : %d\r\n",global_CodeLength);
    fprintf(fileio_simEnv,"Maximum Code length : %d\r\n",global_MaximumCodeLength);
    fprintf(fileio_simEnv,"Limit Code length : %d%s\r\n",global_LimitCodeLength, global_LimitCodeLength?"(Limited)":"(Unlimited)");

    fprintf(fileio_simEnv,"Hard Decision correctable(t) : %d\r\n",global_bch_hardCorrectable);
    fprintf(fileio_simEnv,"Soft Decision correctable(p) : %d\r\n",global_bch_SoftCorrectable);
    fprintf(fileio_simEnv,"Over Syndrome number : %d\r\n", global_bch_Soft_OverSyndLen);
    fprintf(fileio_simEnv,"Added Root to Least Common Multiple : %s\n", global_addedRootToLCM_string);

    fprintf(fileio_simEnv,"LLR quantization offset : %f\r\n", global_LLR_quantizFilter_offset);
    fprintf(fileio_simEnv,"LLR quantization step : %f\r\n", global_LLR_quantizFilter_step);
    fprintf(fileio_simEnv,"LLR quantization bit : %d\r\n", global_LLR_quantizFilter_bitWidth);

    fprintf(fileio_simEnv,"\r\n");


    fprintf(fileio_simEnv,"/*****************************************************************/\r\n");
    fprintf(fileio_simEnv,"/******************** Simulation Enviroments *********************/\r\n");
    fprintf(fileio_simEnv,"/*****************************************************************/\r\n");
    fprintf(fileio_simEnv,">> Setted Paramters are... <<\r\n");

    fprintf(fileio_simEnv,"Hard decision, Correctable code length is \'%d\'\r\n", main_com_hd_correctability);
    //(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))
    for(main_tmp_sel_decAlgo_i=0; main_tmp_sel_decAlgo_i<processingUseThisAlgorithm->length; main_tmp_sel_decAlgo_i++)
    {
        fprintf(fileio_simEnv,"%s - algorithm Hard decision, Correctable code length is \'%d\'\r\n",
            KIND_OF_BCH_DECODING_ALGORITHM_NAME[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))],
            main_indi_hd_correctability[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]
        );
    }

    fprintf(fileio_simEnv,"Soft decision, Correctable code length is \'%d\'\r\n", main_tmp_degErrLocPoly);
    fprintf(fileio_simEnv,"Number of test pattern number is \'%d\'\r\n", main_com_numsTP);

    fprintf(fileio_simEnv,"Using Galois Field is \'%d\'\r\n", main_com_used_GF->nonSorted->length);
    fprintf(fileio_simEnv,"Code word length is \'%d\'\r\n", main_com_codeLength);
    fprintf(fileio_simEnv,"Information word length is \'%d\'\r\n", main_com_infoBitLength);
    fprintf(fileio_simEnv,"Parity bits length is \'%d\'\r\n", main_com_parityBitsLength);
    fprintf(fileio_simEnv,"Code rate is \'%f\' %s\r\n", main_com_codeRate,((global_flag_Channel_Mode_Func&FLAG_MASK_CHANNEL_MODE_FUNC_BITRATE_DISABLE)?"[disabled]":"[enabled]"));
    fprintf(fileio_simEnv,"\n");

    switch(global_flag_case_selTP_before_dec)
    {
        case FLAG_CASE_SEL_TP_BEFORE_DEC_ACT:
            fprintf(fileio_simEnv, "Get BER Mode is Practical.\r\n");
        break;

        case FLAG_CASE_SEL_TP_BEFORE_DEC_THEO:
            fprintf(fileio_simEnv, "Get BER Mode is Theoretical.\r\n");

            switch(global_flag_case_choosingCodeWordMethod_inTheo)
            {
                case FLAG_CASE_CODEWORD_CHOOSEN_METHOD_IN_THEO_NO_ERR:
                    /* not-support */
                    fprintf(fileio_simEnv, "(Option, all ZERO,[NOT SUPPORT])\r\n");
                break;

                case FLAG_CASE_CODEWORD_CHOOSEN_METHOD_IN_THEO_NORMAL:
                    fprintf(fileio_simEnv, "(Option, Normal)\r\n");
                break;

                case FLAG_CASE_CODEWORD_CHOOSEN_METHOD_IN_THEO_MIN_ERR:
                    fprintf(fileio_simEnv, "(Option, Best)\r\n");
                break;

                case FLAG_CASE_CODEWORD_CHOOSEN_METHOD_IN_THEO_MAX_ERR:
                    fprintf(fileio_simEnv, "(Option, Worst)\r\n");
                break;

                default:
                break;
            }
        break;

        default:
            fprintf(fileio_simEnv, "Get BER Mode is Unknown.\r\n");
        break;
    }


    switch(global_flag_case_find_LLR_method)
    {
        case FLAG_CASE_FINDING_MIN_LLR_METHOD_NONE:
        {
            fprintf(fileio_simEnv, "Finding method a minimum llr is None.\r\n");
        }
        break;

        case FLAG_CASE_FINDING_MIN_LLR_METHOD_MINIMUM:
        {
            fprintf(fileio_simEnv, "Finding method a minimum llr is Minumum.\r\n");
        }
        break;

        case FLAG_CASE_FINDING_MIN_LLR_METHOD_GROUPING:
        {
            fprintf(fileio_simEnv, "Finding method a minimum llr is grouping minumum.\r\n");
        }
        break;

        case FLAG_CASE_FINDING_MIN_LLR_METHOD_TREE_FOLLOWING_MIN1_PATH:
        {
            fprintf(fileio_simEnv, "Finding method a minimum llr is Chase.\r\n");
        }
        break;

        default:
        {
            fprintf(fileio_simEnv, "Finding method a minimum llr is Unknwon.\r\n");
        }
        break;
    }
    fprintf(fileio_simEnv,"LLR filter configure offset : %f\r\n", global_QuantizationInfo.offset);
    fprintf(fileio_simEnv,"LLR filter configure step : %f\r\n", global_QuantizationInfo.step);
    fprintf(fileio_simEnv,"LLR filter configure bit : %d\r\n", global_QuantizationInfo.numsOfBits);
    fprintf(fileio_simEnv,"LLR filter configure numberic system : %s\r\n", NAME_QUANTIZ_NUMBERIC_SYSTEM[global_QuantizationInfo.numberic_system]);
    fprintf(fileio_simEnv,"LLR filter configure zero handling : %s\r\n", NAME_QUANTIZ_ONE_S_COMPLEMENT_ZERO_HANDLING[global_QuantizationInfo.ones_zero_handling]);

    fprintf(fileio_simEnv, "/*****************************************************************/\r\n");
    fprintf(fileio_simEnv, "/******************** Notice Simulation Mode *********************/\r\n");
    fprintf(fileio_simEnv, "/*****************************************************************/\r\n");
    if(global_flag_sim_decodingMode&FLAG_MASK_CODE_BCH_DEC_AL_TEST_THIS_ALGORITHM)
    {
        infoMes; fprintf(fileio_simEnv, "Simulation will be entered to algorithm verification mode.\r\n");

        for(main_tmp_sel_decAlgo_i=0; main_tmp_sel_decAlgo_i<processingUseThisAlgorithm->length; main_tmp_sel_decAlgo_i++)
        {
            if(global_list_flag_bch_sim_decodingMode[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]&FLAG_MASK_CODE_BCH_DEC_AL_TEST_THIS_ALGORITHM)
            {
                infoMes; fprintf(fileio_simEnv, "[%s] algorithm verification mode.\r\n", KIND_OF_BCH_DECODING_ALGORITHM_NAME[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]);
            }
        }
        infoMes; fprintf(fileio_simEnv, "In verification Mode Artificial Error number : %d\n",main_com_errNums_inDecodingAlgorithmTest);
    }
    else
    {
        if(global_flag_sim_decodingMode&FLAG_MASK_CODE_BCH_DEC_AL_SOFT_DECISION)
        {
            infoMes; fprintf(fileio_simEnv, "Simulation will be entered to Soft Decision mode.\r\n");

            for(main_tmp_sel_decAlgo_i=0; main_tmp_sel_decAlgo_i<processingUseThisAlgorithm->length; main_tmp_sel_decAlgo_i++)
            {
                if(global_list_flag_bch_sim_decodingMode[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]&FLAG_MASK_CODE_BCH_DEC_AL_SOFT_DECISION)
                {
                    infoMes; fprintf(fileio_simEnv, "[%s] algorithm Soft Decision mode.\r\n", KIND_OF_BCH_DECODING_ALGORITHM_NAME[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]);
                }
            }
        }
        else if(global_flag_sim_decodingMode&FLAG_MASK_CODE_BCH_DEC_AL_HARD_DECISION)
        {
            infoMes; fprintf(fileio_simEnv, "Simulation will be entered to Hard Decision mode.\r\n");

            for(main_tmp_sel_decAlgo_i=0; main_tmp_sel_decAlgo_i<processingUseThisAlgorithm->length; main_tmp_sel_decAlgo_i++)
            {
                if(global_list_flag_bch_sim_decodingMode[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]&FLAG_MASK_CODE_BCH_DEC_AL_HARD_DECISION)
                {
                    infoMes; fprintf(fileio_simEnv, "[%s] algorithm Hard Decision mode.\r\n", KIND_OF_BCH_DECODING_ALGORITHM_NAME[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]);
                }
            }
        }
        else
        {
            return -1;
        }
    }

    fclose(fileio_simEnv);
    fileio_simEnv = NULL;
    /*******************************************************************/
    /*******************************************************************/
    /*********** Associate with file input output source end ***********/
    /*******************************************************************/
    /*******************************************************************/

    /********************************************************************/
    /********* setting variables about domestic variables end *********/
    /********************************************************************/


    /*******************************************************************/
    /*******************************************************************/
    /******************* simulation EbN0 loop start ********************/
    /*******************************************************************/
    /*******************************************************************/
    switch(global_flag_case_sim_testOpt)
    {
        case FLAG_CASE_SIM_TEST_MODE_GET_LLR_MAG_AVR_BELONG_EBN0:
        /* Test Mode calculation */
        printf("\r\n");

        tmp_testMode_loops_bit_inversion_val = (unsigned long)(tmp_testMode_loops_zero_bit_ratio * (double)tmp_testMode_loops);

        printf("zero stream ratio: %f\r\n", tmp_testMode_loops_zero_bit_ratio);
        printf("total loops : %ld\r\n", tmp_testMode_loops);
        printf("zero stream loops : %ld\r\n", tmp_testMode_loops_bit_inversion_val);
        printf("zero bit cnt : %ld\r\n",
                (((unsigned long)global_CodeLength)*tmp_testMode_loops_bit_inversion_val)
        );

        printf("one stream loops : %ld\r\n", tmp_testMode_loops - tmp_testMode_loops_bit_inversion_val);
        printf("one bit cnt: %ld\r\n",
                (((unsigned long)global_CodeLength)*(tmp_testMode_loops - tmp_testMode_loops_bit_inversion_val))
        );


        fprintf(fp_testMode_log, "zero stream ratio: %e\r\n", tmp_testMode_loops_zero_bit_ratio);
        fprintf(fp_testMode_log, "total loops : %ld\r\n", tmp_testMode_loops);
        fprintf(fp_testMode_log, "zero stream loops : %ld\r\n", tmp_testMode_loops_bit_inversion_val);
        fprintf(fp_testMode_log, "zero bit cnt : %ld\r\n",
                (((unsigned long)global_CodeLength)*tmp_testMode_loops_bit_inversion_val)
        );

        fprintf(fp_testMode_log, "one stream loops : %ld\r\n", tmp_testMode_loops - tmp_testMode_loops_bit_inversion_val);
        fprintf(fp_testMode_log, "one bit cnt: %ld\r\n",
                (((unsigned long)global_CodeLength)*(tmp_testMode_loops - tmp_testMode_loops_bit_inversion_val))
        );

        for(main_com_EbN0=global_EbN0_Start_Value; main_com_EbN0<=global_EbN0_End_Value; main_com_EbN0+=global_EbN0_Step)
        {
            printf("EbN0:%f start\r\n", main_com_EbN0);

            tmp_bit_num_inC_ref_zero_OZ_ratio = (unsigned long**)malloc(sizeof(unsigned long*)*(main_com_codeLength+1));

            tmp_bit_num_inR_ref_zero_OZ_ratio = (unsigned long**)malloc(sizeof(unsigned long*)*(main_com_codeLength+1));
            tmp_bit_num_inR_ref_one_OZ_ratio = (unsigned long**)malloc(sizeof(unsigned long*)*(main_com_codeLength+1));

            tmp_llr_mag_sum_inR_OZ_ratio = (unsigned long**)malloc(sizeof(unsigned long*)*(main_com_codeLength+1));
            tmp_bit_chg_cnt_inR_OZ_ratio = (unsigned long**)malloc(sizeof(unsigned long*)*(main_com_codeLength+1));
            tmp_llr_mag_inR = (double**)malloc(sizeof(double*)*main_com_codeLength+1);


            /* arrange by err cnt */
            tmp_sum_err_codeword_cnt_arranged_by_err_nums = (unsigned long*)malloc(sizeof(unsigned long)*(main_com_codeLength+1));
            memset(tmp_sum_err_codeword_cnt_arranged_by_err_nums, 0, sizeof(unsigned long)*(main_com_codeLength+1));
            tmp_err_llr_mag_distribution_in_all_loop_arranged_by_err_nums = (unsigned long**)malloc(sizeof(unsigned long*)*(main_com_codeLength+1));
            tmp_err_llr_mag_distribution_in_one_codeword = (unsigned int*)malloc(sizeof(unsigned int)*(main_com_codeLength+1));
            memset(tmp_err_llr_mag_distribution_in_one_codeword, 0, sizeof(unsigned int)*(main_com_codeLength+1));

            tmp_sum_cor_codeword_cnt_arranged_by_err_nums = (unsigned long*)malloc(sizeof(unsigned long)*(main_com_codeLength+1));
            memset(tmp_sum_cor_codeword_cnt_arranged_by_err_nums, 0, sizeof(unsigned long)*(main_com_codeLength+1));
            tmp_cor_llr_mag_distribution_in_all_loop_arranged_by_err_nums = (unsigned long**)malloc(sizeof(unsigned long*)*(main_com_codeLength+1));
            tmp_cor_llr_mag_distribution_in_one_codeword  = (unsigned int*)malloc(sizeof(unsigned int)*(main_com_codeLength+1));
            memset(tmp_cor_llr_mag_distribution_in_one_codeword, 0, sizeof(unsigned int)*(main_com_codeLength+1));

            tmp_sum_zero_and_one_bit_cnt_arranged_by_err_nums = (unsigned long**)malloc(sizeof(unsigned long*)*(main_com_codeLength+1));


            for(i=0; i<main_com_codeLength+1; i++)
            {
                tmp_bit_num_inC_ref_zero_OZ_ratio[i] = (unsigned long*)malloc(sizeof(unsigned long)*2);
                memset(tmp_bit_num_inC_ref_zero_OZ_ratio[i], 0, sizeof(unsigned long)*2);

                tmp_bit_num_inR_ref_zero_OZ_ratio[i] = (unsigned long*)malloc(sizeof(unsigned long)*3);
                memset(tmp_bit_num_inR_ref_zero_OZ_ratio[i], 0, sizeof(unsigned long)*3);
                tmp_bit_num_inR_ref_one_OZ_ratio[i] = (unsigned long*)malloc(sizeof(unsigned long)*3);
                memset(tmp_bit_num_inR_ref_one_OZ_ratio[i], 0, sizeof(unsigned long)*3);
                tmp_llr_mag_sum_inR_OZ_ratio[i] = (unsigned long*)malloc(sizeof(unsigned long)*3);
                memset(tmp_llr_mag_sum_inR_OZ_ratio[i], 0, sizeof(unsigned long)*3);
                tmp_bit_chg_cnt_inR_OZ_ratio[i] = (unsigned long*)malloc(sizeof(unsigned long)*3);
                memset(tmp_bit_chg_cnt_inR_OZ_ratio[i], 0, sizeof(unsigned long)*3);
                tmp_llr_mag_inR[i] = (double*)malloc(sizeof(double)*3);
                memset(tmp_llr_mag_inR[i], 0, sizeof(double)*3);

                /* arrange by err cnt */
                tmp_err_llr_mag_distribution_in_all_loop_arranged_by_err_nums[i] = (unsigned long*)malloc(sizeof(unsigned long)*((unsigned int)(global_QuantizationInfo.mag_bitMask + 1)));
                memset(tmp_err_llr_mag_distribution_in_all_loop_arranged_by_err_nums[i], 0, sizeof(unsigned long)*(global_QuantizationInfo.mag_bitMask + 1));
                tmp_cor_llr_mag_distribution_in_all_loop_arranged_by_err_nums[i] = (unsigned long*)malloc(sizeof(unsigned long)*((unsigned int)(global_QuantizationInfo.mag_bitMask + 1)));
                memset(tmp_cor_llr_mag_distribution_in_all_loop_arranged_by_err_nums[i], 0, sizeof(unsigned long)*(global_QuantizationInfo.mag_bitMask + 1));
                tmp_sum_zero_and_one_bit_cnt_arranged_by_err_nums[i] = (unsigned long*)malloc(sizeof(unsigned long)*(4));
                memset(tmp_sum_zero_and_one_bit_cnt_arranged_by_err_nums[i], 0, sizeof(unsigned long)*4);
            }

            for(tmp_testMode_loops_cnt=0UL; tmp_testMode_loops_cnt<tmp_testMode_loops; tmp_testMode_loops_cnt++)
            {

                /*
                //switch(global_flag_case_testMode_msg_gen_method)
                //{
                //    case ENUM_CASE_TESTMODE_ERR_RATE_MSG_GEN_METH_RAND:

                //    break;

                //    case ENUM_CASE_TESTMODE_ERR_RATE_MSG_GEN_METH_STATIC:

                //    break;
                //}
                */

                switch(global_flag_case_testMode_msg_gen_method)
                {
                    case ENUM_CASE_TESTMODE_ERR_RATE_MSG_GEN_METH_RAND:
                        main_com_randInfoBitsStream =
                            randdomInfoBitGenerator(main_com_infoBitLength, main_com_parityBitsLength, &main_com_randInfoBitsStream);

                        copyPowerFormPolynomialFromString(main_encodingComponentInGF->codeWord, main_com_randInfoBitsStream);

                        shiftHighSidePowerFormPolynomial(main_encodingComponentInGF->codeWord, main_com_parityBitsLength);

                        calculateParityInGaloisFieldAttachLowSide(main_encodingComponentInGF->codeWord, main_com_parityBitsLength, main_encodingComponentInGF->generationPolynomial);
                    break;

                    case ENUM_CASE_TESTMODE_ERR_RATE_MSG_GEN_METH_STATIC:
                        if(tmp_testMode_loops_cnt<tmp_testMode_loops_bit_inversion_val)
                        {
                        main_com_randInfoBitsStream =
                            allSameInfoBitGenerator('0', main_com_infoBitLength, main_com_parityBitsLength, &main_com_randInfoBitsStream);
                        }
                        else
                        {
                            main_com_randInfoBitsStream =
                                allSameInfoBitGenerator('1', main_com_infoBitLength, main_com_parityBitsLength, &main_com_randInfoBitsStream);
                        }

                        copyPowerFormPolynomialFromString(main_encodingComponentInGF->codeWord, main_com_randInfoBitsStream);

                    break;
                }

                main_com_errComponents=recreateErrorComponent(&main_com_errComponents, main_com_codeLength);

                BPSK_Mod(main_com_bpskComponents->bpskTransData, main_encodingComponentInGF->codeWord->equation, main_encodingComponentInGF->codeWord->usedLength);

                ADD_AWGN_CAL_LLR(
                    main_com_bpskComponents->bpskTransData,
                    main_com_bpskComponents->bpskReceivedDataAddedAwgn,
                    main_com_bpskComponents->bpskReceivedLLR->llr,
                    ((global_flag_Channel_Mode_Func&FLAG_MASK_CHANNEL_MODE_FUNC_BITRATE_DISABLE)?1.0:main_com_codeRate),
                    main_com_codeLength,
                    main_com_EbN0
                );//need to add bitrate

                switch(global_flag_case_testMode_decoding_method)
                {
                    case ENUM_CASE_TEST_MODE_ERR_RATE_DECODING_METH_HD:
                    {
                        BPSK_Demod(main_com_errComponents->erroredCodeWord->equation, main_com_bpskComponents->bpskReceivedDataAddedAwgn, main_com_codeLength);
                    }
                    break;

                    default:
                    case ENUM_CASE_TEST_MODE_ERR_RATE_DECODING_METH_SD:
                    {
                        quantizationWithGlobalAdaptive(
                            main_com_bpskComponents->bpskReceivedLLR->llr,
                            main_com_bpskComponents->bpskReceivedLLR->quantizedLLR,
                            main_com_errComponents->erroredCodeWord->equation,
                            main_com_errComponents->erroredCodeWord->usedLength
                        );

                        convertQuantizedLLRToMagnitude(main_com_bpskComponents->bpskReceivedLLR);
                    }
                    break;
                }


                /* In codeword, zero and one bit count */
                for(i=0; i<main_com_codeLength; i++)
                {
                    if(main_encodingComponentInGF->codeWord->equation[i] == '0')
                    {
                        tmp_testMode_zero_bit_nums_inC++;
                    }
                    else
                    {
                        tmp_testMode_one_bit_nums_inC++;
                    }
                }
                for(i=0; i<main_com_codeLength; i++)
                {
                        /* ones complement verify code start */
                        /*
                        //if(main_com_bpskComponents->bpskReceivedLLR->llr[i] > 0.0f)
                        //{
                        //    printf("llr:+%5.8f, Q llr:0x%08x, Q llr 0d%d, mag:0x%08x, mag:%d mag+0:0d+%d\r\n",
                        //            main_com_bpskComponents->bpskReceivedLLR->llr[i],
                        //            main_com_bpskComponents->bpskReceivedLLR->quantizedLLR[i],
                        //            main_com_bpskComponents->bpskReceivedLLR->quantizedLLR[i],
                        //            main_com_bpskComponents->bpskReceivedLLR->magnitude[i],
                        //            main_com_bpskComponents->bpskReceivedLLR->magnitude[i],
                        //            main_com_bpskComponents->bpskReceivedLLR->magnitude[i]
                        //    );
                        //}
                        //else
                        //{
                        //    printf("llr:%5.8f, Q llr:0x%08x, Q llr 0d%d, mag:0x%08x, mag:%d,  mag+1:0d%d\r\n",

                        //            main_com_bpskComponents->bpskReceivedLLR->llr[i],
                        //            main_com_bpskComponents->bpskReceivedLLR->quantizedLLR[i],
                        //            main_com_bpskComponents->bpskReceivedLLR->quantizedLLR[i],
                        //            main_com_bpskComponents->bpskReceivedLLR->magnitude[i],
                        //            main_com_bpskComponents->bpskReceivedLLR->magnitude[i],
                        //            main_com_bpskComponents->bpskReceivedLLR->magnitude[i]+1
                        //    );
                        //}
                        */
                        /* ones complement verify code end */
                    /* if received bit is error */
                    if(main_encodingComponentInGF->codeWord->equation[i] != main_com_errComponents->erroredCodeWord->equation[i])
                    {
                        //tmp_testMode_llr_mag_sum_w+=(unsigned long)main_com_bpskComponents->bpskReceivedLLR->magnitude[i];
                        /* note. ==> tmp_testMode_llr_mag_sum_w += tmp_testMode_llr_mag_sum_inR_w; */
                        tmp_testMode_llr_mag_w[main_com_bpskComponents->bpskReceivedLLR->magnitude[i]]++;
                        tmp_testMode_count_bit_w++;
                        if(main_com_bpskComponents->bpskReceivedLLR->magnitude[i] > tmp_testMode_llr_mag_max_w) tmp_testMode_llr_mag_max_w = main_com_bpskComponents->bpskReceivedLLR->magnitude[i];
                        if(main_com_bpskComponents->bpskReceivedLLR->magnitude[i] < tmp_testMode_llr_mag_min_w) tmp_testMode_llr_mag_min_w = main_com_bpskComponents->bpskReceivedLLR->magnitude[i];

                        /* llr sum in received codeword wrong case */
                        tmp_testMode_llr_mag_sum_inR_w += (unsigned long)main_com_bpskComponents->bpskReceivedLLR->magnitude[i];

                        /* received bits cnt, which are zero or one and is error */
                        if(main_com_errComponents->erroredCodeWord->equation[i] == '0')
                        {
                            tmp_testMode_zero_is_inR_w++;
                        }
                        else
                        {
                            tmp_testMode_one_is_inR_w++;
                        }

                        /* received bit change detect */
                        if(main_encodingComponentInGF->codeWord->equation[i] == '0')
                        {
                            tmp_testMode_zeroToOne_bit_nums_inR++;
                        }
                        else
                        {
                            tmp_testMode_oneToZero_bit_nums_inR++;
                        }

                        /* err llr mag distribution in codeword */
                        tmp_err_llr_mag_distribution_in_one_codeword[main_com_bpskComponents->bpskReceivedLLR->magnitude[i]]++;
                    }
                    else
                    {
                        //tmp_testMode_llr_mag_sum_c+=(unsigned long)main_com_bpskComponents->bpskReceivedLLR->magnitude[i];
                        /* note. ==> tmp_testMode_llr_mag_sum_c += tmp_testMode_llr_mag_sum_inR_c; */
                        tmp_testMode_llr_mag_c[main_com_bpskComponents->bpskReceivedLLR->magnitude[i]]++;
                        tmp_testMode_count_bit_c++;
                        if(main_com_bpskComponents->bpskReceivedLLR->magnitude[i] > tmp_testMode_llr_mag_max_c) tmp_testMode_llr_mag_max_c = main_com_bpskComponents->bpskReceivedLLR->magnitude[i];
                        if(main_com_bpskComponents->bpskReceivedLLR->magnitude[i] < tmp_testMode_llr_mag_min_c) tmp_testMode_llr_mag_min_c = main_com_bpskComponents->bpskReceivedLLR->magnitude[i];


                        /* llr sum in received codeword good case */
                        tmp_testMode_llr_mag_sum_inR_c += (unsigned long)main_com_bpskComponents->bpskReceivedLLR->magnitude[i];

                        /* received bits cnt, which are zero or one and is correct */
                        if(main_com_errComponents->erroredCodeWord->equation[i] == '0')
                        {
                            tmp_testMode_zero_is_inR_c++;
                        }
                        else
                        {
                            tmp_testMode_one_is_inR_c++;
                        }

                        /* cor llr mag distribution in codeword */
                        tmp_cor_llr_mag_distribution_in_one_codeword[main_com_bpskComponents->bpskReceivedLLR->magnitude[i]]++;
                    }
                }
                if(main_com_randInfoBitsStream)
                {
                    free(main_com_randInfoBitsStream);
                    main_com_randInfoBitsStream = NULL;
                }


                tmp_bit_num_inC_ref_zero_OZ_ratio[tmp_testMode_zero_bit_nums_inC][0]
                    += (unsigned long)tmp_testMode_zero_bit_nums_inC;
                tmp_bit_num_inC_ref_zero_OZ_ratio[tmp_testMode_zero_bit_nums_inC][1]
                    += (unsigned long)tmp_testMode_one_bit_nums_inC;

                //index reference sended Codeword
                //tmp_testMode_zero_bit_nums_inC
                //index reference Received Codeword
                //tmp_testMode_zero_is_inR_w + tmp_testMode_zero_is_inR_c
                tmp_bit_num_inR_ref_zero_OZ_ratio[tmp_testMode_zero_bit_nums_inC][0]
                    += (tmp_testMode_zero_is_inR_w + tmp_testMode_zero_is_inR_c);
                tmp_bit_num_inR_ref_zero_OZ_ratio[tmp_testMode_zero_bit_nums_inC][1]
                    += tmp_testMode_zero_is_inR_c;
                tmp_bit_num_inR_ref_zero_OZ_ratio[tmp_testMode_zero_bit_nums_inC][2]
                    += tmp_testMode_zero_is_inR_w;


                tmp_bit_num_inR_ref_one_OZ_ratio[tmp_testMode_zero_bit_nums_inC][0]
                    += tmp_testMode_one_is_inR_w + tmp_testMode_one_is_inR_c;
                tmp_bit_num_inR_ref_one_OZ_ratio[tmp_testMode_zero_bit_nums_inC][1]
                    += tmp_testMode_one_is_inR_c;
                tmp_bit_num_inR_ref_one_OZ_ratio[tmp_testMode_zero_bit_nums_inC][2]
                    += tmp_testMode_one_is_inR_w;

                tmp_bit_chg_cnt_inR_OZ_ratio[tmp_testMode_zero_bit_nums_inC][0]
                    += tmp_testMode_zeroToOne_bit_nums_inR + tmp_testMode_oneToZero_bit_nums_inR;
                tmp_bit_chg_cnt_inR_OZ_ratio[tmp_testMode_zero_bit_nums_inC][1]
                    += tmp_testMode_zeroToOne_bit_nums_inR;
                tmp_bit_chg_cnt_inR_OZ_ratio[tmp_testMode_zero_bit_nums_inC][2]
                    += tmp_testMode_oneToZero_bit_nums_inR;

                tmp_llr_mag_sum_inR_OZ_ratio[tmp_testMode_zero_bit_nums_inC][0]
                    += tmp_testMode_llr_mag_sum_inR_w + tmp_testMode_llr_mag_sum_inR_c;
                tmp_llr_mag_sum_inR_OZ_ratio[tmp_testMode_zero_bit_nums_inC][1]
                    += tmp_testMode_llr_mag_sum_inR_c;
                tmp_llr_mag_sum_inR_OZ_ratio[tmp_testMode_zero_bit_nums_inC][2]
                    += tmp_testMode_llr_mag_sum_inR_w;

                //tmp_llr_mag_inR[tmp_testMode_one_is_inR_w + tmp_testMode_one_is_inR_c][0]
                //    = ((double)(1tmp_testMode_llr_mag_sum_inR_w + tmp_testMode_llr_mag_sum_inR_c))/((double)main_com_codeLength);

                tmp_testMode_llr_mag_sum_w += tmp_testMode_llr_mag_sum_inR_w;
                tmp_testMode_llr_mag_sum_c += tmp_testMode_llr_mag_sum_inR_c;

                tmp_testMode_totalBit+=(unsigned long)main_com_codeLength;

                /* arranging by number of error */
                /* count up correct or err codeword */
                /* have any error */
                tmp_sum_err_codeword_cnt_arranged_by_err_nums[tmp_testMode_zeroToOne_bit_nums_inR + tmp_testMode_oneToZero_bit_nums_inR]++;

                for(i = 0; i < (unsigned int)(global_QuantizationInfo.mag_bitMask + 1); i++)
                {
                    tmp_err_llr_mag_distribution_in_all_loop_arranged_by_err_nums \
                        [tmp_testMode_zeroToOne_bit_nums_inR + tmp_testMode_oneToZero_bit_nums_inR] \
                        [i] += tmp_err_llr_mag_distribution_in_one_codeword[i];
                    tmp_cor_llr_mag_distribution_in_all_loop_arranged_by_err_nums \
                        [tmp_testMode_zeroToOne_bit_nums_inR + tmp_testMode_oneToZero_bit_nums_inR] \
                        [i] += tmp_cor_llr_mag_distribution_in_one_codeword[i];
                }

                tmp_sum_zero_and_one_bit_cnt_arranged_by_err_nums \
                    [tmp_testMode_zeroToOne_bit_nums_inR + tmp_testMode_oneToZero_bit_nums_inR][0] += \
                    ( tmp_testMode_zero_bit_nums_inC );
                tmp_sum_zero_and_one_bit_cnt_arranged_by_err_nums \
                    [tmp_testMode_zeroToOne_bit_nums_inR + tmp_testMode_oneToZero_bit_nums_inR][1] += \
                    ( tmp_testMode_one_bit_nums_inC );
                tmp_sum_zero_and_one_bit_cnt_arranged_by_err_nums \
                    [tmp_testMode_zeroToOne_bit_nums_inR + tmp_testMode_oneToZero_bit_nums_inR][2] += \
                    ( tmp_testMode_zero_is_inR_w + tmp_testMode_zero_is_inR_c );
                tmp_sum_zero_and_one_bit_cnt_arranged_by_err_nums \
                    [tmp_testMode_zeroToOne_bit_nums_inR + tmp_testMode_oneToZero_bit_nums_inR][3] += \
                    ( tmp_testMode_one_is_inR_w + tmp_testMode_one_is_inR_c );


                /* encoding parts */
                tmp_testMode_zero_bit_nums_inC = 0;
                tmp_testMode_one_bit_nums_inC = 0;

                /* decoding parts */
                tmp_testMode_zeroToOne_bit_nums_inR = 0;//received bit change detect
                tmp_testMode_oneToZero_bit_nums_inR = 0;//received bit change detect

                tmp_testMode_zero_is_inR_w = 0;//received bits cnt, which are zero or one and is error
                tmp_testMode_one_is_inR_w = 0;//received bits cnt, which are zero or one and is error

                tmp_testMode_zero_is_inR_c = 0;//received bits cnt, which are zero or one and is correct
                tmp_testMode_one_is_inR_c = 0;//received bits cnt, which are zero or one and is correct

                tmp_testMode_llr_mag_sum_inR_w =0UL;//llr sum in received codeword wrong case
                tmp_testMode_llr_mag_sum_inR_c =0UL;//llr sum in received codeword good case

                /* clear arranging by err nums */
                for(i = 0; i < (global_QuantizationInfo.mag_bitMask + 1); i++)
                {
                    tmp_err_llr_mag_distribution_in_one_codeword[i] = 0;
                    tmp_cor_llr_mag_distribution_in_one_codeword[i] = 0;
                }
                //exit(0);
                //printf("%ld\r", tmp_testMode_loops_cnt);
            }
            /* exit all loop */

            /* Labeling */
            fprintf(fp_testMode_sum_bit_cnt_arranged_by_bitRetio_csv,
                    "0-bit cnt, 1-bit cnt\r\n");

            fprintf(fp_testMode_sum_bit_cnt_classified_arranged_by_bitRatio_csv,
                    "sum 0bit ,0 Cor,0 Err,sum 1bit,1 Cor,1 Err\r\n");

            fprintf(fp_testMode_sum_err_bit_cnt_chg_classified_arranged_by_bitRatio_csv,
                    "sum Err,Tran 0->1,Tran 1->0\r\n");

            fprintf(fp_testMode_sum_llr_mag_arranged_by_bitRatio_csv,
                    "sum llr mag,sum llr mag Cor,sum llr mag Err\n");

            fprintf(fp_testMode_all_arranged_by_bitRatio_csv,
                    "codeword 0-bit,codeword 1-bit,sum 0bit,0 Cor,0 Err,sum 1bit,1 Cor,1 Err,sum Err,Tran 0->1,Tran 1->0,sum llr mag,sum llr mag Cor,sum llr mag Err\r\n");

            /* arranging by err nums labeling */
            fprintf(fp_testMode_sum_codeword_cnt_arranged_by_errorNums_csv,
                    "err Nums,codeword cnt\r\n"
                    );
            fprintf(fp_testMode_all_arranged_by_errorNums_csv,
                    "err Nums,codeword cnt"
                    );


            fprintf(fp_testMode_sum_llr_mag_distribution_cnt_arranged_by_errorNums_csv,
                    "err Nums, "
                   );

            fprintf(fp_testMode_sum_llr_mag_distribution_cnt_arranged_by_errorNums_csv,
                    "e llr_mag 0,e llr_mag 1,e llr_mag 2,e llr_mag 3,e llr_mag 4,e llr_mag 5,e llr_mag 6,e llr_mag 7,"
                    );
            fprintf(fp_testMode_all_arranged_by_errorNums_csv,
                    ",e llr_mag 0,e llr_mag 1,e llr_mag 2,e llr_mag 3,e llr_mag 4,e llr_mag 5,e llr_mag 6,e llr_mag 7"
                    );
            fprintf(fp_testMode_sum_llr_mag_distribution_cnt_arranged_by_errorNums_csv,
                    "c llr_mag 0,c llr_mag 1,c llr_mag 2,c llr_mag 3,c llr_mag 4,c llr_mag 5,c llr_mag 6,c llr_mag 7\r\n"
                    );
            fprintf(fp_testMode_all_arranged_by_errorNums_csv,
                    ",c llr_mag 0,c llr_mag 1,c llr_mag 2,c llr_mag 3,c llr_mag 4,c llr_mag 5,c llr_mag 6,c llr_mag 7"
                    );

            fprintf(fp_testMode_sum_bit_ratio_cnt_arranged_by_errorNums_csv,
                    "err Nums,0bit cnt in C,1bit cnt in C,0bit cnt in R,1bit cnt inR\r\n"
                   );
            fprintf(fp_testMode_all_arranged_by_errorNums_csv,
                    ",0bit cnt in C,1bit cnt in C,0bit cnt in R,1bit cnt inR\r\n"
                   );
            for(i=0; i<main_com_codeLength+1; i++)
            {
                fprintf(fp_testMode_sum_bit_cnt_arranged_by_bitRetio_csv, "%ld, %ld\r\n",
                tmp_bit_num_inC_ref_zero_OZ_ratio[i][0],
                tmp_bit_num_inC_ref_zero_OZ_ratio[i][1]
                );

                fprintf(fp_testMode_sum_bit_cnt_classified_arranged_by_bitRatio_csv, "%ld,%ld,%ld,%ld,%ld,%ld\r\n",
                tmp_bit_num_inR_ref_zero_OZ_ratio[i][0],
                tmp_bit_num_inR_ref_zero_OZ_ratio[i][1],
                tmp_bit_num_inR_ref_zero_OZ_ratio[i][2],

                tmp_bit_num_inR_ref_one_OZ_ratio[i][0],
                tmp_bit_num_inR_ref_one_OZ_ratio[i][1],
                tmp_bit_num_inR_ref_one_OZ_ratio[i][2]
                );

                fprintf(fp_testMode_sum_err_bit_cnt_chg_classified_arranged_by_bitRatio_csv, "%ld,%ld,%ld\r\n",
                tmp_bit_chg_cnt_inR_OZ_ratio[i][0],
                tmp_bit_chg_cnt_inR_OZ_ratio[i][1],
                tmp_bit_chg_cnt_inR_OZ_ratio[i][2]
                );

                fprintf(fp_testMode_sum_llr_mag_arranged_by_bitRatio_csv, "%ld,%ld,%ld\r\n",
                tmp_llr_mag_sum_inR_OZ_ratio[i][0],
                tmp_llr_mag_sum_inR_OZ_ratio[i][1],
                tmp_llr_mag_sum_inR_OZ_ratio[i][2]
                );

                fprintf(fp_testMode_all_arranged_by_bitRatio_csv, "%ld,%ld,%ld,%ld,%ld,%ld,%ld,%ld,%ld,%ld,%ld,%ld,%ld,%ld\r\n",
                tmp_bit_num_inC_ref_zero_OZ_ratio[i][0],
                tmp_bit_num_inC_ref_zero_OZ_ratio[i][1],

                tmp_bit_num_inR_ref_zero_OZ_ratio[i][0],
                tmp_bit_num_inR_ref_zero_OZ_ratio[i][1],
                tmp_bit_num_inR_ref_zero_OZ_ratio[i][2],

                tmp_bit_num_inR_ref_one_OZ_ratio[i][0],
                tmp_bit_num_inR_ref_one_OZ_ratio[i][1],
                tmp_bit_num_inR_ref_one_OZ_ratio[i][2],

                tmp_bit_chg_cnt_inR_OZ_ratio[i][0],
                tmp_bit_chg_cnt_inR_OZ_ratio[i][1],
                tmp_bit_chg_cnt_inR_OZ_ratio[i][2],

                tmp_llr_mag_sum_inR_OZ_ratio[i][0],
                tmp_llr_mag_sum_inR_OZ_ratio[i][1],
                tmp_llr_mag_sum_inR_OZ_ratio[i][2]
                );

                /* arranging by err nums record */
                fprintf(fp_testMode_sum_codeword_cnt_arranged_by_errorNums_csv,
                    "%d, %ld\r\n",
                    i,
                    tmp_sum_err_codeword_cnt_arranged_by_err_nums[i]
                );
                fprintf(fp_testMode_all_arranged_by_errorNums_csv,
                    "%d, %ld",
                    i,
                    tmp_sum_err_codeword_cnt_arranged_by_err_nums[i]
                );


                fprintf(fp_testMode_sum_llr_mag_distribution_cnt_arranged_by_errorNums_csv,
                    "%d", i
                );
                for(j = 0; j < (global_QuantizationInfo.mag_bitMask + 1); j++)
                {
                    fprintf(fp_testMode_sum_llr_mag_distribution_cnt_arranged_by_errorNums_csv,
                        ",%ld", tmp_err_llr_mag_distribution_in_all_loop_arranged_by_err_nums[i][j]
                    );
                    fprintf(fp_testMode_all_arranged_by_errorNums_csv,
                        ",%ld", tmp_err_llr_mag_distribution_in_all_loop_arranged_by_err_nums[i][j]
                    );
                }
                for(j = 0; j < (global_QuantizationInfo.mag_bitMask + 1); j++)
                {
                    fprintf(fp_testMode_sum_llr_mag_distribution_cnt_arranged_by_errorNums_csv,
                        ",%ld", tmp_cor_llr_mag_distribution_in_all_loop_arranged_by_err_nums[i][j]
                    );
                    fprintf(fp_testMode_all_arranged_by_errorNums_csv,
                        ",%ld", tmp_cor_llr_mag_distribution_in_all_loop_arranged_by_err_nums[i][j]
                    );
                }
                fprintf(fp_testMode_sum_llr_mag_distribution_cnt_arranged_by_errorNums_csv,"\r\n");

                fprintf(fp_testMode_sum_bit_ratio_cnt_arranged_by_errorNums_csv ,
                    "%d, %ld, %ld, %ld, %ld\r\n",
                    i,
                    tmp_sum_zero_and_one_bit_cnt_arranged_by_err_nums[i][0],//cnt 0 bit cnt in C
                    tmp_sum_zero_and_one_bit_cnt_arranged_by_err_nums[i][1],//cnt 1 bit cnt in C
                    tmp_sum_zero_and_one_bit_cnt_arranged_by_err_nums[i][2],//cnt 0 bit cnt in R
                    tmp_sum_zero_and_one_bit_cnt_arranged_by_err_nums[i][3]//cnt 1 vit cnt in R
                        );
                fprintf(fp_testMode_all_arranged_by_errorNums_csv,
                    ",%ld, %ld, %ld, %ld\r\n",
                    tmp_sum_zero_and_one_bit_cnt_arranged_by_err_nums[i][0],//cnt 0 bit cnt in C
                    tmp_sum_zero_and_one_bit_cnt_arranged_by_err_nums[i][1],//cnt 1 bit cnt in C
                    tmp_sum_zero_and_one_bit_cnt_arranged_by_err_nums[i][2],//cnt 0 bit cnt in R
                    tmp_sum_zero_and_one_bit_cnt_arranged_by_err_nums[i][3]//cnt 1 vit cnt in R
                        );

                free(tmp_bit_num_inC_ref_zero_OZ_ratio[i]);

                free(tmp_bit_num_inR_ref_zero_OZ_ratio[i]);
                free(tmp_bit_num_inR_ref_one_OZ_ratio[i]);
                free(tmp_llr_mag_sum_inR_OZ_ratio[i]);
                free(tmp_bit_chg_cnt_inR_OZ_ratio[i]);
                free(tmp_llr_mag_inR[i]);

                /* arranging by err nums release 2nd degree */
                free(tmp_err_llr_mag_distribution_in_all_loop_arranged_by_err_nums[i]);
                free(tmp_cor_llr_mag_distribution_in_all_loop_arranged_by_err_nums[i]);

                free(tmp_sum_zero_and_one_bit_cnt_arranged_by_err_nums[i]);
            }
            free(tmp_bit_num_inC_ref_zero_OZ_ratio);
            free(tmp_bit_num_inR_ref_zero_OZ_ratio);
            free(tmp_bit_num_inR_ref_one_OZ_ratio);
            free(tmp_llr_mag_sum_inR_OZ_ratio);
            free(tmp_bit_chg_cnt_inR_OZ_ratio);
            free(tmp_llr_mag_inR);

            /* arranging by err nums 1st degree */
            free(tmp_sum_err_codeword_cnt_arranged_by_err_nums);
            free(tmp_err_llr_mag_distribution_in_all_loop_arranged_by_err_nums);
            free(tmp_err_llr_mag_distribution_in_one_codeword);

            free(tmp_sum_cor_codeword_cnt_arranged_by_err_nums);
            free(tmp_cor_llr_mag_distribution_in_all_loop_arranged_by_err_nums);
            free(tmp_cor_llr_mag_distribution_in_one_codeword);

            free(tmp_sum_zero_and_one_bit_cnt_arranged_by_err_nums);




            printf("\n");
            tmp_testMode_llr_mag_avr_w = ((double)tmp_testMode_llr_mag_sum_w / (double)tmp_testMode_count_bit_w);
            tmp_testMode_llr_mag_avr_c = ((double)tmp_testMode_llr_mag_sum_c / (double)tmp_testMode_count_bit_c);


            printf("[e] Ebn0:%.2f, loops:%ld, total bit:%ld, err bit:%ld, mag_sum :%ld, mag_avr:%e, mag_avr_int:%d\r\n",
                    main_com_EbN0,
                    tmp_testMode_loops_cnt,
                    tmp_testMode_totalBit,
                    tmp_testMode_count_bit_w,
                    tmp_testMode_llr_mag_sum_w,
                    tmp_testMode_llr_mag_avr_w,
                    (unsigned int)tmp_testMode_llr_mag_avr_w
            );
            for(i=0; i<LLR_CASE_NUM; i++)
            {
                printf("[e] %d:%ld %.2f%%\r\n", i, tmp_testMode_llr_mag_w[i], (double)tmp_testMode_llr_mag_w[i]/(double)tmp_testMode_count_bit_w);
            }
            printf("[e] max:%d min:%d\r\n", tmp_testMode_llr_mag_max_w, tmp_testMode_llr_mag_min_w);

            printf("[c] Ebn0:%.2f, loops:%ld, total bit:%ld, cor bit:%ld, mag_sum :%ld, mag_avr:%e, mag_avr_int:%d\r\n",
                    main_com_EbN0,
                    tmp_testMode_loops_cnt,
                    tmp_testMode_totalBit,
                    tmp_testMode_count_bit_c,
                    tmp_testMode_llr_mag_sum_c,
                    tmp_testMode_llr_mag_avr_c,
                    (unsigned int)tmp_testMode_llr_mag_avr_c
            );
            for(i=0; i<LLR_CASE_NUM; i++)
            {
                printf("[c] %d:%ld %.2f%%\r\n", i, tmp_testMode_llr_mag_c[i], (double)tmp_testMode_llr_mag_c[i]/(double)tmp_testMode_count_bit_c);
            }
            printf("[c] max:%d min:%d\r\n", tmp_testMode_llr_mag_max_c, tmp_testMode_llr_mag_min_c);



            fprintf(fp_testMode_log, "[e] EbN0:%.2f, loops:%ld, total bit:%ld, err bit:%ld, mag_sum :%ld, mag_avr:%e, mag_avr_int:%d\r\n",
                    main_com_EbN0,
                    tmp_testMode_loops_cnt,
                    tmp_testMode_totalBit,
                    tmp_testMode_count_bit_w,
                    tmp_testMode_llr_mag_sum_w,
                    tmp_testMode_llr_mag_avr_w,
                    (unsigned int)tmp_testMode_llr_mag_avr_w
            );
            for(i=0; i<LLR_CASE_NUM; i++)
            {
                fprintf(fp_testMode_log, "[e]%d:%ld %.2f%%\r\n", i, tmp_testMode_llr_mag_w[i], (double)tmp_testMode_llr_mag_w[i]/(double)tmp_testMode_count_bit_w);
            }
            fprintf(fp_testMode_log, "[e]max:%d min:%d\r\n", tmp_testMode_llr_mag_max_w, tmp_testMode_llr_mag_min_w);

            fprintf(fp_testMode_log, "[c] Ebn0:%.2f, loops:%ld, total bit:%ld, cor bit:%ld, mag_sum :%ld, mag_avr:%e, mag_avr_int:%d\r\n",
                    main_com_EbN0,
                    tmp_testMode_loops_cnt,
                    tmp_testMode_totalBit,
                    tmp_testMode_count_bit_c,
                    tmp_testMode_llr_mag_sum_c,
                    tmp_testMode_llr_mag_avr_c,
                    (unsigned int)tmp_testMode_llr_mag_avr_c
            );
            for(i=0; i<LLR_CASE_NUM; i++)
            {
                fprintf(fp_testMode_log, "[c] %d:%ld %.2f%%\r\n", i, tmp_testMode_llr_mag_c[i], ((double)tmp_testMode_llr_mag_c[i]/(double)tmp_testMode_count_bit_c)*100.0f);
            }
            fprintf(fp_testMode_log, "[c] max:%d min:%d\r\n", tmp_testMode_llr_mag_max_c, tmp_testMode_llr_mag_min_c);




            tmp_testMode_totalBit= 0UL;

            tmp_testMode_count_bit_w = 0UL;

            for(i=0; i<LLR_CASE_NUM; i++) tmp_testMode_llr_mag_w[i] = 0UL;

            tmp_testMode_llr_mag_sum_w = 0UL;

            tmp_testMode_llr_mag_max_w = 0;
            tmp_testMode_llr_mag_min_w = 7;

            tmp_testMode_llr_mag_avr_w = 0.0f;


            tmp_testMode_count_bit_c = 0UL;

            for(i=0; i<LLR_CASE_NUM; i++) tmp_testMode_llr_mag_c [i] = 0UL;

            tmp_testMode_llr_mag_sum_c  = 0UL;

            tmp_testMode_llr_mag_max_c  = 0;
            tmp_testMode_llr_mag_min_c  = 7;

            tmp_testMode_llr_mag_avr_c = 0.0f;

            printf("EbN0:%f end\r\n", main_com_EbN0);
        }
        break;

        case FLAG_CASE_SIM_TEST_MODE_GET_PROBABILITY_OF_MINIMUM_VAL:

        initTestMode_prob_min_counting(&tmp_testMode_prob_min_counting, &tmp_testMode_prob_min_counting_length, global_QuantizationInfo.mag_bitMask);
        printf("tmp_testMode_prob_min_counting = 0x%lx\r\n", (unsigned long)tmp_testMode_prob_min_counting);
        printf("tmp_testMode_prob_min_counting_length = %d\r\n", tmp_testMode_prob_min_counting_length);

        for(main_com_EbN0=global_EbN0_Start_Value; main_com_EbN0<=global_EbN0_End_Value; main_com_EbN0+=global_EbN0_Step)
        {
            for(tmp_testMode_loops_cnt=0UL; tmp_testMode_loops_cnt<tmp_testMode_loops; tmp_testMode_loops_cnt++)
            {
            //case ENUM_CASE_TESTMODE_ERR_RATE_MSG_GEN_METH_RAND:
                /* Stream Generation */
                main_com_randInfoBitsStream =
                    randdomInfoBitGenerator(main_com_infoBitLength, main_com_parityBitsLength, &main_com_randInfoBitsStream);

                /* Copy stream to codeword components */
                copyPowerFormPolynomialFromString(main_encodingComponentInGF->codeWord, main_com_randInfoBitsStream);

                /* To calculate parity, shifting stream parity length */
                shiftHighSidePowerFormPolynomial(main_encodingComponentInGF->codeWord, main_com_parityBitsLength);

                /* Calculating Parity */
                calculateParityInGaloisFieldAttachLowSide(main_encodingComponentInGF->codeWord, main_com_parityBitsLength, main_encodingComponentInGF->generationPolynomial);
            //break;

                /* Create error components */
                main_com_errComponents=recreateErrorComponent(&main_com_errComponents, main_com_codeLength);

                BPSK_Mod(main_com_bpskComponents->bpskTransData, main_encodingComponentInGF->codeWord->equation, main_encodingComponentInGF->codeWord->usedLength);

                ADD_AWGN_CAL_LLR(
                    main_com_bpskComponents->bpskTransData,
                    main_com_bpskComponents->bpskReceivedDataAddedAwgn,
                    main_com_bpskComponents->bpskReceivedLLR->llr,
                    ((global_flag_Channel_Mode_Func&FLAG_MASK_CHANNEL_MODE_FUNC_BITRATE_DISABLE)?1.0:main_com_codeRate),
                    main_com_codeLength,
                    main_com_EbN0
                ); //need to add bitrate

                /* Decoding */
            //case ENUM_CASE_TEST_MODE_ERR_RATE_DECODING_METH_SD:
            //{
                quantizationWithGlobalAdaptive(
                    main_com_bpskComponents->bpskReceivedLLR->llr,
                    main_com_bpskComponents->bpskReceivedLLR->quantizedLLR,
                    main_com_errComponents->erroredCodeWord->equation,
                    main_com_errComponents->erroredCodeWord->usedLength
                );

                convertQuantizedLLRToMagnitude(main_com_bpskComponents->bpskReceivedLLR);
            //}
            //break;

                for(i=0; i<main_com_codeLength; i++)
                {
                }



            }
            clearTestMode_prob_min_counting(tmp_testMode_prob_min_counting, tmp_testMode_prob_min_counting_length, global_QuantizationInfo.mag_bitMask);

        }
        break;

        case FLAG_CASE_SIM_TEST_MODE_SHOW_LOOP_CNT:
        {
            printf("/*****************************************************************/\r\n");
            printf("/********************** Simulation Showing ***********************/\r\n");
            printf("/*****************************************************************/\r\n");

            for(main_com_EbN0=global_EbN0_Start_Value; main_com_EbN0<=global_EbN0_End_Value; main_com_EbN0+=global_EbN0_Step)
            {
                /********************************************/
                /***** loop count decision codes start ******/
                /********************************************/
                if(global_base_of_loop)
                {
                    main_com_totalSamlingLoop = global_base_of_loop;
                }
                else
                {
                    main_com_totalSamlingLoop = (unsigned long)main_com_codeLength;
                }

                if(global_rate_of_loop)
                {
                    main_com_totalSamlingLoop *= ((unsigned long)pow(global_rate_of_loop, main_com_EbN0));
                }
                else
                {
                    main_com_totalSamlingLoop *= ((unsigned long)pow(10, main_com_EbN0));
                }

                if(main_com_totalSamlingLoop < global_minimum_loop) main_com_totalSamlingLoop = global_minimum_loop;
                // if(main_com_totalSamlingLoop<global_minimum_loop) main_com_totalSamlingLoop=global_minimum_loop;
                /********************************************/
                /****** loop count decision codes end *******/
                /********************************************/

                infoMes; printf("main EbN0 is '%g'.\r\n", main_com_EbN0);
                infoMes; printf("main_com_EbN0 : 10^(-%g),\tTotal loops : %ld\r\n", main_com_EbN0, main_com_totalSamlingLoop);
            }
        }
        break;

        case FLAG_CASE_SIM_TEST_MODE_NONE:
        default:
        printExcutingTime(&time_program_init);
        for(main_com_EbN0=global_EbN0_Start_Value; main_com_EbN0<=global_EbN0_End_Value; main_com_EbN0+=global_EbN0_Step)
        {
            gettimeofday(&time_EbN0_start, NULL);
            /*******************************************************************/
            /********** Associated with BER variables are clear start **********/
            /*******************************************************************/

            for(main_tmp_sel_decAlgo_i=0; main_tmp_sel_decAlgo_i<processingUseThisAlgorithm->length; main_tmp_sel_decAlgo_i++)
            {
                main_indi_total_err_cnt_HD[(*(((uint8_t*)processingUseThisAlgorithm->list)+main_tmp_sel_decAlgo_i))] = 0;
                main_indi_total_err_cnt_CW[(*(((uint8_t*)processingUseThisAlgorithm->list)+main_tmp_sel_decAlgo_i))] = 0;
            }
            main_com_total_err_cnt_CH = 0;
            main_com_total_bit = 0;

            test_fileIO_errorCorrectingCount=0;
            /*******************************************************************/
            /*********** Associated with BER variables are clear end ***********/
            /*******************************************************************/

            /********************************************/
            /***** loop count decision codes start ******/
            /********************************************/
            if(global_base_of_loop)
            {
                main_com_totalSamlingLoop = global_base_of_loop;
            }
            else
            {
                main_com_totalSamlingLoop = (unsigned long)main_com_codeLength;
            }

            if(global_rate_of_loop)
            {
                main_com_totalSamlingLoop *= ((unsigned long)pow(global_rate_of_loop, main_com_EbN0));
            }
            else
            {
                main_com_totalSamlingLoop *= ((unsigned long)pow(10, main_com_EbN0));
            }

            if(main_com_totalSamlingLoop < global_minimum_loop) main_com_totalSamlingLoop = global_minimum_loop;
            // if(main_com_totalSamlingLoop<global_minimum_loop) main_com_totalSamlingLoop=global_minimum_loop;
            /********************************************/
            /****** loop count decision codes end *******/
            /********************************************/

            printf("/*****************************************************************/\r\n");
            printf("/*********************** Simulation Start ************************/\r\n");
            printf("/*****************************************************************/\r\n");
            infoMes; printf("main EbN0 is '%g'.\r\n", main_com_EbN0);
            infoMes; printf("main_com_EbN0 : 10^(-%g),\tTotal loops : %ld\r\n", main_com_EbN0, main_com_totalSamlingLoop);

        /*****************************************************************************/
        /*****************************************************************************/
        /******************* simulation main_com_loopCount loop start ********************/
        /*****************************************************************************/
        /*****************************************************************************/
            for(main_com_loopCount=0; main_com_loopCount<main_com_totalSamlingLoop; main_com_loopCount++)
            {
                /*
                 * Making a random infomation bits and encoding codeword is
                 * common block in blockAlgorithm Test, Hard - Decision and Soft - Decision Mode
                 * start
                 */
                gettimeofday(&time_encoding_start, NULL);
                                        #ifndef RELEASE
                                        if(global_flag_debug_display&FLAG_MASK_DISPLAY_FUNCNAME)
                                        {
                                            debugFuncNameMes;
                                            printf("\r\nfor(main_com_loopCount=0; main_com_loopCount<main_com_totalSamlingLoop; main_com_loopCount++) start\r\n");
                                        }
                                        if(global_flag_debug_display&FLAG_MASK_DISPLAY_FUNCNAME)
                                        {
                                            debugFuncNameMes;
                                            printf("randdomInfoBitGenerator\r\n");
                                        }
                                        #endif
                main_com_randInfoBitsStream=randdomInfoBitGenerator(main_com_infoBitLength, main_com_parityBitsLength, &main_com_randInfoBitsStream);
                                        #ifndef RELEASE
                                        if(global_flag_debug_display&FLAG_MASK_DISPLAY_FUNCNAME)
                                        {
                                            debugFuncNameMes;
                                            printf("copyPowerFormPolynomialFromString\r\n");
                                        }
                                        #endif
                copyPowerFormPolynomialFromString(main_encodingComponentInGF->codeWord, main_com_randInfoBitsStream);

                        main_tmp_sel_decAlgo_cnt = 0;
                        for(main_tmp_sel_decAlgo_i=0; main_tmp_sel_decAlgo_i<processingUseThisAlgorithm->length; main_tmp_sel_decAlgo_i++)
                        {
                            if((global_list_flag_bch_sim_decodingMode[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]&FLAG_MASK_CODE_BCH_DEC_AL_TEST_THIS_ALGORITHM)) main_tmp_sel_decAlgo_cnt++;
                        }
                        if((global_flag_debug_display&FLAG_MASK_DISPLAY_PROGRESS)||(main_tmp_sel_decAlgo_cnt))
                        {
                            debugSequenceMes;
                            printf("origin Vector is below...\r\n");
                            printPowerFormWithEnterPolynomialWithEnterUsingAddress(main_encodingComponentInGF->codeWord);

                        }
                        if(global_flag_file_io_hd_pattern_output&FLAG_MASK_FILE_IO_HD_MES)
                        {
                            fprintPowerFormUsingAddress\
                                (fileio_HD_mes, main_encodingComponentInGF->codeWord, "\r\n");
                        }
                        if(global_flag_file_io_hd_pattern_output_verilog&FLAG_MASK_FILE_IO_HD_MES)
                        {
                            fprintVerilogPowerFormUsingAddress\
                                (fileio_HD_mes_verilog, main_encodingComponentInGF->codeWord, NULL);
                        }
                        if(global_flag_file_io_hd_pattern_output_log&FLAG_MASK_FILE_IO_HD_MES)
                        {
                            fprintPowerFormFullDescriptionUsingAddresss\
                                (fileio_HD_mes_log, main_encodingComponentInGF->codeWord, "\r\n");
                        }

                                        #ifndef RELEASE
                                        if(global_flag_debug_display&FLAG_MASK_DISPLAY_FUNCNAME)
                                        {
                                            debugFuncNameMes;
                                            printf("shiftHighSidePowerFormPolynomial\r\n");
                                        }
                                        #endif
                shiftHighSidePowerFormPolynomial(main_encodingComponentInGF->codeWord, main_com_parityBitsLength);
                        main_tmp_sel_decAlgo_cnt = 0;
                        for(main_tmp_sel_decAlgo_i=0; main_tmp_sel_decAlgo_i<processingUseThisAlgorithm->length; main_tmp_sel_decAlgo_i++)
                        {
                            if((global_list_flag_bch_sim_decodingMode[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]&FLAG_MASK_CODE_BCH_DEC_AL_TEST_THIS_ALGORITHM)) main_tmp_sel_decAlgo_cnt++;
                        }
                        if((global_flag_debug_display&FLAG_MASK_DISPLAY_PROGRESS)||(main_tmp_sel_decAlgo_cnt))
                        {
                            debugSequenceMes;
                            printf("shifted Value is below...\r\n");
                            printPowerFormWithEnterPolynomialWithEnterUsingAddress(main_encodingComponentInGF->codeWord);
                        }
                                        #ifndef RELEASE
                                        if(global_flag_debug_display&FLAG_MASK_DISPLAY_FUNCNAME)
                                        {
                                            debugFuncNameMes;
                                            printf("calculateParityInGaloisFieldAttachLowSide\r\n");
                                        }
                                        #endif
                calculateParityInGaloisFieldAttachLowSide(main_encodingComponentInGF->codeWord, main_com_parityBitsLength, main_encodingComponentInGF->generationPolynomial);
                        main_tmp_sel_decAlgo_cnt = 0;
                        for(main_tmp_sel_decAlgo_i=0; main_tmp_sel_decAlgo_i<processingUseThisAlgorithm->length; main_tmp_sel_decAlgo_i++)
                        {
                            if((global_list_flag_bch_sim_decodingMode[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]&FLAG_MASK_CODE_BCH_DEC_AL_TEST_THIS_ALGORITHM)) main_tmp_sel_decAlgo_cnt++;
                        }
                        if((global_flag_debug_display&FLAG_MASK_DISPLAY_PROGRESS)||(main_tmp_sel_decAlgo_cnt))
                        {
                                debugSequenceMes;
                                printf("Encoded Vector is below...\r\n");
                                //printPowerFormWithEnterPolynomialWithEnterUsingAddress(main_encodingComponentInGF->codeWord);
                                printPowerFormWithEnterPolynomialWithEnterUsingAddress(main_encodingComponentInGF->codeWord);
                        }
                        if(global_flag_file_io_hd_pattern_output&FLAG_MASK_FILE_IO_HD_ENCODING_MES)
                        {
                            fprintPowerFormUsingAddress\
                                (fileio_HD_encoding_mes, main_encodingComponentInGF->codeWord, "\r\n");
                        }
                        if(global_flag_file_io_hd_pattern_output_verilog&FLAG_MASK_FILE_IO_HD_ENCODING_MES)
                        {
                            fprintVerilogPowerFormUsingAddress\
                                (fileio_HD_encoding_mes_verilog, main_encodingComponentInGF->codeWord, NULL);
                        }
                        if(global_flag_file_io_hd_pattern_output_log&FLAG_MASK_FILE_IO_HD_ENCODING_MES)
                        {
                            fprintPowerFormFullDescriptionUsingAddresss\
                                (fileio_HD_encoding_mes_log, main_encodingComponentInGF->codeWord, "\r\n");
                        }

                /*
                 * Making a random infomation bits and encoding codeword is
                 * common block in blockAlgorithm Test, Hard - Decision and Soft - Decision Mode
                 * end
                 */


                /********************************************************/
                /********** (Global Flag) Algorithm simulation **********/
                /********************************************************/
                if(global_flag_sim_decodingMode&FLAG_MASK_CODE_BCH_DEC_AL_TEST_THIS_ALGORITHM)
                {
                    printf("[Encoding time]"); printExcutingTime(&time_encoding_start);
                    gettimeofday(&time_decoding_start, NULL);
                    printf("/*****************************************************************/\r\n");
                    printf("/********************* Algorithm Test Start **********************/\r\n");
                    printf("/*****************************************************************/\r\n");

                    if(global_flag_sim_decodingMode&FLAG_MASK_CODE_BCH_DEC_AL_SOFT_DECISION)
                    {
                        warningMes;
                        printf("BCH algorithm option is enabled. So it will pass Soft Decision simulation. [Option] [--...soft]\r\n");
                    }
                    infoMes; printf(">> BCH Algorithm Test <<\r\n");
                    //recreate error codeword
                            #ifndef RELEASE
                            if(global_flag_debug_display&FLAG_MASK_DISPLAY_FUNCNAME)
                            {
                                debugFuncNameMes;
                                printf("recreateErrorComponent\r\n");
                            }
                            #endif
                    main_com_errComponents=recreateErrorComponent(&main_com_errComponents, main_com_codeLength);
                    //main_com_errComponents=ifNotExistCreateErrorComponent(&main_com_errComponents, main_com_codeLength);

                            #ifndef RELEASE
                            if(global_flag_debug_display&FLAG_MASK_DISPLAY_FUNCNAME)
                            {
                                debugFuncNameMes;
                                printf("errorOccurationWhichAreRandomPosition\r\n");
                            }
                            #endif
                    errorOccurationWhichAreRandomPosition(main_com_errComponents->errorLocationVector, main_com_errNums_inDecodingAlgorithmTest);
                    infoMes; printf("Generated Error Vector is below...\r\n");
                    printPowerFormWithEnterPolynomialWithEnterUsingAddress(main_com_errComponents->errorLocationVector);


                    //summation encoded codeword and error codeword
                            #ifndef RELEASE
                            if(global_flag_debug_display&FLAG_MASK_DISPLAY_FUNCNAME)
                            {
                                debugFuncNameMes;
                                printf("addErrorCodeWordToUnErrorCodeWord\r\n");
                            }
                            #endif
                    addErrorCodeWordToUnErrorCodeWord(main_com_errComponents->erroredCodeWord, main_encodingComponentInGF->codeWord, main_com_errComponents->errorLocationVector);
                    infoMes; printf("Added error Vector is...\r\n");
                    printPowerFormWithEnterPolynomialWithEnterUsingAddress(main_com_errComponents->erroredCodeWord);
                    printf("[Addictive Error Peoc time]"); printExcutingTime(&time_decoding_start);


                    gettimeofday(&time_bm_algorithm_start,NULL);
                            #ifndef RELEASE
                            if(global_flag_debug_display&FLAG_MASK_DISPLAY_FUNCNAME)
                            {
                                debugFuncNameMes;
                                printf("[recreateAlgoriehmComponent start]\r\n");
                            }
                            #endif
                    #ifndef EXCEED_T
                    for(main_tmp_sel_decAlgo_i=0; main_tmp_sel_decAlgo_i<processingUseThisAlgorithm->length; main_tmp_sel_decAlgo_i++)
                    {
                                #ifndef RELEASE
                                if(global_flag_debug_display&FLAG_MASK_DISPLAY_FUNCNAME)
                                {
                                    debugFuncNameMes;
                                    printf("[%s][%d] #%d [HD] [recreateAlgoriehmComponent start]\r\n",
                                        KIND_OF_BCH_DECODING_ALGORITHM_NAME[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))],
                                        //KIND_OF_BCH_DECODING_ALGORITHM_NAME[(*(main_indi_HD_decordComponents+(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))))->KIND_OF_BCH_ALGORITHM],
                                        (*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i)),
                                        (*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))
                                    );
                                }
                                #endif


                        main_indi_HD_decordComponents[(*(((uint8_t*)processingUseThisAlgorithm->list)+main_tmp_sel_decAlgo_i))] =
                            recreateAlgoriehmComponent(
                                (main_indi_HD_decordComponents+(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))),
                                main_com_used_GF->nonSorted,
                                main_com_hd_correctability,
                                (char*)main_com_errComponents->erroredCodeWord->equation,
                                main_com_codeLength,
                                (*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))
                            );
                                #ifndef RELEASE
                                if(global_flag_debug_display&FLAG_MASK_DISPLAY_FUNCNAME)
                                {
                                    debugFuncNameMes;
                                    printf("[%s][%d] #%d [HD] [recreateAlgoriehmComponent end]\r\n",
                                        KIND_OF_BCH_DECODING_ALGORITHM_NAME[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))],
                                        //KIND_OF_BCH_DECODING_ALGORITHM_NAME[(*(main_indi_HD_decordComponents+(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))))->KIND_OF_BCH_ALGORITHM],
                                        (*(main_indi_HD_decordComponents+(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))))->KIND_OF_BCH_ALGORITHM,
                                        (*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))
                                    );
                                }
                                #endif
                    }
                    #else
                    if(main_com_errNums_inDecodingAlgorithmTest>main_com_hd_correctability)
                    {
                        infoMes; printf("condition main_com_errNums_inDecodingAlgorithmTest>main_com_hd_correctability, main_com_errNums_inDecodingAlgorithmTest = %d\r\n",main_com_errNums_inDecodingAlgorithmTest);
                        for(main_tmp_sel_decAlgo_i=0; main_tmp_sel_decAlgo_i<processingUseThisAlgorithm->length; main_tmp_sel_decAlgo_i++)
                        {
                            main_indi_HD_decordComponents[(*(((uint8_t*)processingUseThisAlgorithm->list)+main_tmp_sel_decAlgo_i))] =
                                recreateAlgoriehmComponent(
                                    (main_indi_HD_decordComponents+(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))),
                                        main_com_used_GF->nonSorted,
                                        main_com_errNums_inDecodingAlgorithmTest,
                                        (char*)main_com_errComponents->erroredCodeWord->equation,
                                        main_com_codeLength,
                                        (*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))
                                );
                        }
                    }
                    else
                    {
                        infoMes; printf("condition main_com_errNums_inDecodingAlgorithmTest<=main_com_hd_correctability, main_com_hd_correctability = %d\r\n",main_com_hd_correctability);
                        for(main_tmp_sel_decAlgo_i=0; main_tmp_sel_decAlgo_i<processingUseThisAlgorithm->length; main_tmp_sel_decAlgo_i++)
                        {
                            main_indi_HD_decordComponents[(*(((uint8_t*)processingUseThisAlgorithm->list)+main_tmp_sel_decAlgo_i))] =
                                recreateAlgoriehmComponent(
                                    (main_indi_HD_decordComponents+(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))),
                                    main_com_used_GF->nonSorted,
                                    main_com_hd_correctability,
                                    (char*)main_com_errComponents->erroredCodeWord->equation,
                                    main_com_codeLength,
                                    (*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))
                                );
                        }
                    }
                    #endif
                            #ifndef RELEASE
                            if(global_flag_debug_display&FLAG_MASK_DISPLAY_FUNCNAME)
                            {
                                debugFuncNameMes;
                                printf("[recreateAlgoriehmComponent end]\r\n");
                            }
                            #endif
                    infoMes; printf("condition main_com_errNums_inDecodingAlgorithmTest<=main_com_hd_correctability, main_com_hd_correctability = %d\r\n",main_com_hd_correctability);
                    for(main_tmp_sel_decAlgo_i=0; main_tmp_sel_decAlgo_i<processingUseThisAlgorithm->length; main_tmp_sel_decAlgo_i++)
                    {
                        if
                        (0 < allSyndromeIsZero
                                (
                                    main_com_used_GF->nonSorted,
                                    main_indi_HD_decordComponents[(*(((uint8_t*)(processingUseThisAlgorithm->list))+0))]->syndrome
                                )
                        )
                        {
                            main_indi_list_flag_beforeDec_syndZero[(*(((uint8_t*)processingUseThisAlgorithm->list)+main_tmp_sel_decAlgo_i))][0]=1;
                        }
                        else
                        {
                            main_indi_list_flag_beforeDec_syndZero[(*(((uint8_t*)processingUseThisAlgorithm->list)+main_tmp_sel_decAlgo_i))][0]=0;
                        }
                    }
                    /*cop*/
                    infoMes;
                    printf("in main_indi_HD_decordComponents[(*(((uint8_t*)(processingUseThisAlgorithm->list))+0))] encoded Vector is below...\r\n");
                    printPowerFormWithEnterPolynomialWithEnterUsingAddress(main_indi_HD_decordComponents[(*(((uint8_t*)(processingUseThisAlgorithm->list))+0))]->codeWord);

                    //printSyndromeSeriesWithTapAndEnter(main_indi_HD_decordComponents[(*(((uint8_t*)(processingUseThisAlgorithm->list))+0))]->syndrome, main_com_used_GF->nonSorted);
                    //printSyndromeSeriesWithTap(main_indi_HD_decordComponents[(*(((uint8_t*)(processingUseThisAlgorithm->list))+0))]->syndrome, main_com_used_GF->nonSorted);
                            #ifndef RELEASE
                            if(global_flag_debug_display&FLAG_MASK_DISPLAY_FUNCNAME)
                            {
                                debugFuncNameMes;
                                printf("[calculateBCH_decodingAlgorithm start]\r\n");
                            }
                            #endif
                    for(main_tmp_sel_decAlgo_i=0; main_tmp_sel_decAlgo_i<processingUseThisAlgorithm->length; main_tmp_sel_decAlgo_i++)
                    {
                                #ifndef RELEASE
                                if(global_flag_debug_display&FLAG_MASK_DISPLAY_FUNCNAME)
                                {
                                    debugFuncNameMes;
                                    printf("[%s : #%d calculateBCH_decodingAlgorithm start]\r\n",
                                        KIND_OF_BCH_DECODING_ALGORITHM_NAME[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))],
                                        (*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))
                                    );
                                }
                                #endif
                        //calculateBmAlgorithm(main_com_used_GF->nonSorted, ((struct_HD_BM_algorithmComponent*)main_indi_HD_decordComponents[KIND_OF_BCH_DECODING_BM]));
                        calculateBCH_decodingAlgorithm(main_com_used_GF->nonSorted, main_indi_HD_decordComponents[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]);
                                #ifndef RELEASE
                                if(global_flag_debug_display&FLAG_MASK_DISPLAY_FUNCNAME)
                                {
                                    debugFuncNameMes;
                                    printf("[%s : #%d calculateBCH_decodingAlgorithm end]\r\n",
                                        KIND_OF_BCH_DECODING_ALGORITHM_NAME[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))],
                                        (*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))
                                    );
                                }
                                #endif
                    }
                            #ifndef RELEASE
                            if(global_flag_debug_display&FLAG_MASK_DISPLAY_FUNCNAME)
                            {
                                debugFuncNameMes;
                                printf("[calculateBCH_decodingAlgorithm end]\r\n");
                            }
                            #endif

                    infoMes; printf(">>> Error location polynomial <<<\r\n");
                    for(main_tmp_sel_decAlgo_i=0; main_tmp_sel_decAlgo_i<processingUseThisAlgorithm->length; main_tmp_sel_decAlgo_i++)
                    {
                        infoMes; printf("degree of error location polynomial : %d, metric Chk : %d and degree Chk : %d\r\n",
                            main_indi_HD_decordComponents[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]->degreeOfErrLocPoly,
                            main_indi_HD_decordComponents[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]->metricCheck,
                            checkDegreePolynomials(main_com_used_GF->nonSorted, main_indi_HD_decordComponents[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]->errLocPoly)
                        );
                        //printGaloisField2(main_com_used_GF->nonSorted, main_indi_HD_decordComponents[KIND_OF_BCH_DECODING_BM]->errLocPoly, PRINT_FIELD_EQUATION_TITLE, PRINT_FIELD_PRINT_OPTION_NUMBERING);
                        printf(">> [ %s ]<<\r\n", KIND_OF_BCH_DECODING_ALGORITHM_NAME[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]);
                        printGaloisField2(main_com_used_GF->nonSorted, main_indi_HD_decordComponents[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]->errLocPoly, PRINT_FIELD_EQUATION_TITLE, PRINT_FIELD_PRINT_OPTION_NUMBERING);

                    }


                    for(main_tmp_sel_decAlgo_i=0; main_tmp_sel_decAlgo_i<processingUseThisAlgorithm->length; main_tmp_sel_decAlgo_i++)
                    {
                        if(main_com_errNums_inDecodingAlgorithmTest > main_indi_hd_correctability[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))])
                        {
                            //if(main_indi_hd_correctability[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))] > (main_tmp_degErrLocPoly = checkDegreePolynomials(main_com_used_GF->nonSorted, main_indi_HD_decordComponents[KIND_OF_BCH_DECODING_BM]->errLocPoly)))
                            //{
                            //    main_indi_errNumExceed_but_degErrLocPoly_not_max_cnt[KIND_OF_BCH_DECODING_BM]++;

                            //    warningMes; printf("Error number ('%d') larget than hard-decision correctability ('%d')\r\n", main_com_errNums_inDecodingAlgorithmTest, main_indi_hd_correctability[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i)));
                            //    warningMesShort; printf("Degree of error locaion polynomial is '%d'\r\n", main_tmp_degErrLocPoly);
                            //    if(!(global_flag_cmdSystemUnhold&FLAG_MASK_SYSTEM_UNHOLD_FORCED))
                            //    {
                            //        systemHoldMes;    printf("input any charactor : ");
                            //        scanf("%s", global_buf_KeyBoardInput);
                            //    }
                            //}
                            if(main_indi_hd_correctability[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))] > (main_tmp_degErrLocPoly = checkDegreePolynomials(main_com_used_GF->nonSorted, main_indi_HD_decordComponents[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]->errLocPoly)))
                            {
                                main_indi_errNumExceed_but_degErrLocPoly_not_max_cnt[(*(((uint8_t*)processingUseThisAlgorithm->list)+main_tmp_sel_decAlgo_i))]++;

                                warningMes; printf("[%s] Error number ('%d') larget than hard-decision correctability ('%d')\r\n",
                                                KIND_OF_BCH_DECODING_ALGORITHM_NAME[(*(((uint8_t*)processingUseThisAlgorithm->list)+main_tmp_sel_decAlgo_i))],
                                                main_com_errNums_inDecodingAlgorithmTest,
                                                main_indi_hd_correctability[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]
                                            );
                                warningMesShort; printf("Degree of error locaion polynomial is '%d'\r\n", main_tmp_degErrLocPoly);
                                if(!(global_flag_cmdSystemUnhold&FLAG_MASK_SYSTEM_UNHOLD_FORCED))
                                {
                                    systemHoldMes;    printf("input any charactor : ");
                                    scanf("%s", global_buf_KeyBoardInput);
                                }
                            }
                        }
                    }


                    printf("[ALGORITHM CALCULATION TIME]"); printExcutingTime(&time_bm_algorithm_start);
                    gettimeofday(&time_chien_search_start, NULL);

                            #ifndef RELEASE
                            if(global_flag_debug_display&FLAG_MASK_DISPLAY_FUNCNAME)
                            {
                                debugFuncNameMes;
                                printf("[chienSearch_static start]\r\n");
                            }
                            #endif
                    for(main_tmp_sel_decAlgo_i=0; main_tmp_sel_decAlgo_i<processingUseThisAlgorithm->length; main_tmp_sel_decAlgo_i++)
                    {
                        //chienSearch_static(
                        //    main_com_used_GF->nonSorted,
                        //    main_indi_HD_decordComponents[(*(((uint8_t*)processingUseThisAlgorithm->list)+main_tmp_sel_decAlgo_i))]->errLocPoly,
                        //    main_indi_HD_decordComponents[(*(((uint8_t*)processingUseThisAlgorithm->list)+main_tmp_sel_decAlgo_i))]->errorLocationVector,
                        //    &(main_indi_HD_decordComponents[(*(((uint8_t*)processingUseThisAlgorithm->list)+main_tmp_sel_decAlgo_i))]->metricCheck)
                        //);
                        chienSearch(
                            main_com_used_GF->nonSorted,
                            main_indi_HD_decordComponents[(*(((uint8_t*)processingUseThisAlgorithm->list)+main_tmp_sel_decAlgo_i))]->errLocPoly,
                            main_indi_HD_decordComponents[(*(((uint8_t*)processingUseThisAlgorithm->list)+main_tmp_sel_decAlgo_i))]->errorLocationVector,
                            &(main_indi_HD_decordComponents[(*(((uint8_t*)processingUseThisAlgorithm->list)+main_tmp_sel_decAlgo_i))]->metricCheck),
                            (*(((uint8_t*)processingUseThisAlgorithm->list)+main_tmp_sel_decAlgo_i))
                        );
                    }
                            #ifndef RELEASE
                            if(global_flag_debug_display&FLAG_MASK_DISPLAY_FUNCNAME)
                            {
                                debugFuncNameMes;
                                printf("[chienSearch_static end]\r\n");
                            }
                            #endif

                    /* verification of metric chk and deg of err loc poly start */
                    for(main_tmp_sel_decAlgo_i=0; main_tmp_sel_decAlgo_i<processingUseThisAlgorithm->length; main_tmp_sel_decAlgo_i++)
                    {
                        if(main_indi_HD_decordComponents[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]->degreeOfErrLocPoly != main_indi_HD_decordComponents[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]->metricCheck)
                        {
                            warningMes;
                            printf("degree of error location polynomial : %d, metric Chk : %d are not same.\r\n",
                                main_indi_HD_decordComponents[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]->degreeOfErrLocPoly,
                                main_indi_HD_decordComponents[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]->metricCheck
                            );
                            //if(!(global_flag_cmdSystemUnhold&FLAG_MASK_SYSTEM_UNHOLD_FORCED))
                            //{
                                systemHoldMes;    printf("input any charactor : ");
                                scanf("%s", global_buf_KeyBoardInput);
                            //}
                        }
                    }
                    /* verification of metric chk and deg of err loc poly end */

                    /* !!! Only use BM algorithm errLocSyndrome and sumReceiveAndErrLocSyndrome !!! calculate syndrome of error location */
                    //if(global_flag_bchDecAlgo_Enable&uint32_bitMask[KIND_OF_BCH_DECODING_BM])
                    //{
                    //    if(global_bch_Soft_OverSyndLen)
                    //    {
                    //                #ifndef RELEASE
                    //                if(global_flag_debug_display&FLAG_MASK_DISPLAY_FUNCNAME)
                    //                {
                    //                    debugFuncNameMes;
                    //                    printf("[syndromeArrayCalculatorUsingString start]\r\n");
                    //                }
                    //                #endif
                    //        syndromeArrayCalculatorUsingString
                    //        (
                    //            main_com_used_GF->nonSorted,
                    //            ((struct_HD_BM_algorithmComponent*)main_indi_HD_decordComponents[KIND_OF_BCH_DECODING_BM])->errLocSyndrome,
                    //            main_indi_HD_decordComponents[KIND_OF_BCH_DECODING_BM]->vMAX+global_bch_Soft_OverSyndLen,
                    //            (char*)main_indi_HD_decordComponents[KIND_OF_BCH_DECODING_BM]->errorLocationVector->equation
                    //        );
                    //                #ifndef RELEASE
                    //                if(global_flag_debug_display&FLAG_MASK_DISPLAY_FUNCNAME)
                    //                {
                    //                    debugFuncNameMes;
                    //                    printf("[syndromeArrayCalculatorUsingString end]\r\n");
                    //                }
                    //                #endif

                    //                #ifndef RELEASE
                    //                if(global_flag_debug_display&FLAG_MASK_DISPLAY_FUNCNAME)
                    //                {
                    //                    debugFuncNameMes;
                    //                    printf("[allSyndromeIsZero start]\r\n");
                    //                }
                    //                #endif
                    //        if
                    //        (0 < allSyndromeIsZero
                    //                (
                    //                    main_com_used_GF->nonSorted,
                    //                    ((struct_HD_BM_algorithmComponent*)main_indi_HD_decordComponents[KIND_OF_BCH_DECODING_BM])->errLocSyndrome
                    //                )
                    //        )
                    //        {
                    //            main_indi_list_flag_after_dec_synd_zero[KIND_OF_BCH_DECODING_BM][0]=1;
                    //        }
                    //        else
                    //        {
                    //            main_indi_list_flag_after_dec_synd_zero[KIND_OF_BCH_DECODING_BM][0]=0;
                    //        }
                    //                #ifndef RELEASE
                    //                if(global_flag_debug_display&FLAG_MASK_DISPLAY_FUNCNAME)
                    //                {
                    //                    debugFuncNameMes;
                    //                    printf("[allSyndromeIsZero end]\r\n");
                    //                }
                    //                #endif
                    //        /*
                    //        //if(main_com_errNums_inDecodingAlgorithmTest>main_indi_hd_correctability[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i)))
                    //        //{
                    //        //    if(0<allSyndromeIsZero(main_com_used_GF->nonSorted, ((struct_HD_BM_algorithmComponent*)main_indi_HD_decordComponents[KIND_OF_BCH_DECODING_BM])->syndrome))
                    //        //    {
                    //        //        main_indi_errExceed_but_synd_zero_cnt[KIND_OF_BCH_DECODING_BM]++;

                    //        //        warningMes;
                    //        //        printf(" main_com_errNums_inDecodingAlgorithmTest = ('%d') > FLAG_CASE_DECODING_FAIL_METHOD_DEFAULT = ('%d')\r\n", main_com_errNums_inDecodingAlgorithmTest, main_indi_hd_correctability[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i)));
                    //        //        warningMesShort; printf("Syndrome value is zero is detected.\r\n");
                    //        //        warningMesShort; printf("This case will be serious problem in BM decoding algorithm.\r\n");
                    //        //        if(!(global_flag_cmdSystemUnhold&FLAG_MASK_SYSTEM_UNHOLD_FORCED))
                    //        //        {
                    //        //            systemHoldMes;    printf("input any charactor : ");
                    //        //            scanf("%s", global_buf_KeyBoardInput);
                    //        //        }
                    //        //        systemHoldMes;    printf("input : ");
                    //        //        scanf("%s", global_buf_KeyBoardInput);
                    //        //    }
                    //        //}
                    //        */
                    //                #ifndef RELEASE
                    //                if(global_flag_debug_display&FLAG_MASK_DISPLAY_FUNCNAME)
                    //                {
                    //                    debugFuncNameMes;
                    //                    printf("[summationSyndromeArray start]\r\n");
                    //                }
                    //                #endif
                    //        summationSyndromeArray
                    //        (
                    //            main_com_used_GF->nonSorted,
                    //            ((struct_HD_BM_algorithmComponent*)main_indi_HD_decordComponents[KIND_OF_BCH_DECODING_BM])->sumReceiveAndErrLocSyndrome,
                    //            main_indi_HD_decordComponents[KIND_OF_BCH_DECODING_BM]->syndrome,
                    //            ((struct_HD_BM_algorithmComponent*)main_indi_HD_decordComponents[KIND_OF_BCH_DECODING_BM])->errLocSyndrome
                    //        );
                    //                #ifndef RELEASE
                    //                if(global_flag_debug_display&FLAG_MASK_DISPLAY_FUNCNAME)
                    //                {
                    //                    debugFuncNameMes;
                    //                    printf("[summationSyndromeArray end]\r\n");
                    //                }
                    //                #endif
                    //    }

                    //}



                    for(main_tmp_sel_decAlgo_i=0; main_tmp_sel_decAlgo_i<processingUseThisAlgorithm->length; main_tmp_sel_decAlgo_i++)
                    {
                        infoMes; printf(">>> [ %s ] Syndrome of Received vector <<<\r\n", KIND_OF_BCH_DECODING_ALGORITHM_NAME[(*(((uint8_t*)processingUseThisAlgorithm->list)+main_tmp_sel_decAlgo_i))]);
                        printSyndromeArray(main_indi_HD_decordComponents[(*(((uint8_t*)processingUseThisAlgorithm->list)+main_tmp_sel_decAlgo_i))]->syndrome, main_com_used_GF->nonSorted);

                        /* !!! Only use BM algorithm errLocSyndrome and sumReceiveAndErrLocSyndrome !!! calculate syndrome of error location */
                        //if((*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i)) == KIND_OF_BCH_DECODING_BM)
                        //{
                        //    infoMes; printf(">>> [ %s : %d ] Syndrome of ErrorLocation vector <<<\r\n", KIND_OF_BCH_DECODING_ALGORITHM_NAME[(*(((uint8_t*)processingUseThisAlgorithm->list)+main_tmp_sel_decAlgo_i))], (*(((uint8_t*)processingUseThisAlgorithm->list)+main_tmp_sel_decAlgo_i)));
                        //    printSyndromeArray(((struct_HD_BM_algorithmComponent*)main_indi_HD_decordComponents[(*(((uint8_t*)processingUseThisAlgorithm->list)+main_tmp_sel_decAlgo_i))])->errLocSyndrome, main_com_used_GF->nonSorted);

                        //    infoMes; printf(">>> [ %s : %d ]sum Syndromes of Received and ErrorLocation vector <<<\r\n", KIND_OF_BCH_DECODING_ALGORITHM_NAME[(*(((uint8_t*)processingUseThisAlgorithm->list)+main_tmp_sel_decAlgo_i))], (*(((uint8_t*)processingUseThisAlgorithm->list)+main_tmp_sel_decAlgo_i)));
                        //    printSyndromeArray(((struct_HD_BM_algorithmComponent*)main_indi_HD_decordComponents[(*(((uint8_t*)processingUseThisAlgorithm->list)+main_tmp_sel_decAlgo_i))])->sumReceiveAndErrLocSyndrome, main_com_used_GF->nonSorted);
                        //}

                        infoMes; printf("[ %s ] Generated Error Vector is below...\r\n", KIND_OF_BCH_DECODING_ALGORITHM_NAME[(*(((uint8_t*)processingUseThisAlgorithm->list)+main_tmp_sel_decAlgo_i))]);
                        printPowerFormWithEnterPolynomialWithEnterUsingAddress(main_com_errComponents->errorLocationVector);
                        infoMes; printf("[ %s ] Error Location Vector is...\r\n", KIND_OF_BCH_DECODING_ALGORITHM_NAME[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]);
                        printPowerFormWithEnterPolynomialWithEnterUsingAddress((main_indi_HD_decordComponents[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))])->errorLocationVector);
                    }

                    printf("[errorCorrection start]\r\n");
                    for(main_tmp_sel_decAlgo_i=0; main_tmp_sel_decAlgo_i<processingUseThisAlgorithm->length; main_tmp_sel_decAlgo_i++)
                    {
                                #ifndef RELEASE
                                if(global_flag_debug_display&FLAG_MASK_DISPLAY_FUNCNAME)
                                {
                                    debugFuncNameMes;
                                    printf("\t[%s] errorCorrection [start]\r\n", KIND_OF_BCH_DECODING_ALGORITHM_NAME[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]);
                                }
                                #endif
                        errorCorrection(
                            main_indi_HD_decordComponents[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]->codeWord,
                            main_indi_HD_decordComponents[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]->errorLocationVector
                        );
                                #ifndef RELEASE
                                if(global_flag_debug_display&FLAG_MASK_DISPLAY_FUNCNAME)
                                {
                                    debugFuncNameMes;
                                    printf("\t[%s] errorCorrection [end]\r\n", KIND_OF_BCH_DECODING_ALGORITHM_NAME[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]);
                                }
                                #endif
                                /* File Out Corrected Vector start */
                                /* File Out Patterns */
                                if(global_flag_file_io_hd_pattern_output&FLAG_MASK_FILE_IO_HD_CORRECTED_MES)
                                {
                                    fprintPowerFormUsingAddress\
                                        (\
                                            fileio_corrected_mes[
                                                (*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))
                                            ],
                                            main_indi_HD_decordComponents[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]->codeWord,
                                            "\r\n"
                                        );
                                }
                                /* File Out Verilog Vectors start */
                                if(global_flag_file_io_hd_pattern_output_verilog&FLAG_MASK_FILE_IO_HD_CORRECTED_MES)
                                {
                                    fprintVerilogPowerFormUsingAddress\
                                        (
                                            fileio_corrected_mes_verilog[
                                                (*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))
                                            ],
                                            main_indi_HD_decordComponents[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]->codeWord,
                                            NULL
                                        );
                                }
                                /* File Out Pattern log */
                                if(global_flag_file_io_hd_pattern_output_log&FLAG_MASK_FILE_IO_HD_CORRECTED_MES)
                                {
                                    fprintPowerFormFullDescriptionUsingAddresss\
                                        (
                                            fileio_corrected_mes_log[
                                                (*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))
                                            ],
                                            main_indi_HD_decordComponents[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]->codeWord,
                                            "\r\n"
                                        );
                                }
                                /* File Out Corrected Vector end */
                    }
                    printf("[errorCorrection end]\r\n");


                    infoMes; printf("Encoded Vector is below...\r\n");
                    printPowerFormWithEnterPolynomialWithEnterUsingAddress(main_encodingComponentInGF->codeWord);
                    infoMes; printf("Recived Vector is...\r\n");
                    printPowerFormWithEnterPolynomialWithEnterUsingAddress(main_com_errComponents->erroredCodeWord);
                    for(main_tmp_sel_decAlgo_i=0; main_tmp_sel_decAlgo_i<processingUseThisAlgorithm->length; main_tmp_sel_decAlgo_i++)
                    {
                        infoMes; printf("[%s] Corrected Vector is...\r\n", KIND_OF_BCH_DECODING_ALGORITHM_NAME[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]);
                        printPowerFormWithEnterPolynomialWithEnterUsingAddress(main_indi_HD_decordComponents[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]->codeWord);

                        if(!strcmp((char*)main_encodingComponentInGF->codeWord->equation, (char*)main_indi_HD_decordComponents[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]->codeWord->equation))
                        {
                            infoMes; printf("[%s=#%d] : success\r\n",
                                        KIND_OF_BCH_DECODING_ALGORITHM_NAME[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))],
                                        (*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))
                                    );
                        }
                        else
                        {
                            infoMes; printf("[%s=#%d] : fail\r\n",
                                        KIND_OF_BCH_DECODING_ALGORITHM_NAME[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))],
                                        (*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))
                                    );
                            main_indi_err_corection_fail_cnt[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]++;

                            if(main_com_errNums_inDecodingAlgorithmTest > main_indi_hd_correctability[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]) main_indi_err_detect_and_correct_fail_cnt[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]++;

                            if(!(global_flag_cmdSystemUnhold&FLAG_MASK_SYSTEM_UNHOLD_FORCED))
                            {
                                systemHoldMes; printf("error correction is fail...\r\n");
                                systemHoldMes; printf("check why decording is failure, if you want.\r\n");

                                systemHoldMes; printf("and to be continue simulation, please insert any charactors...\r\n");
                                systemHoldMes; printf("you can ignore this exception, adding command option '-f' or \"--force\"\r\n");

                                systemHoldMes;    printf("input : ");
                                scanf("%s", global_buf_KeyBoardInput);
                            }
                            else
                            {
                                systemUnholdMes; printf("-f(\"--force\") command option is enabled, ignore hold conditions.\r\n");
                            }


                            /* !!! Only use BM algorithm errLocSyndrome and sumReceiveAndErrLocSyndrome !!! calculate syndrome of error location */
                            if((*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i)) == KIND_OF_BCH_DECODING_BM)
                            {
                                if(main_com_errNums_inDecodingAlgorithmTest > main_indi_hd_correctability[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))])
                                {
                                    if(0<allSyndromeIsZero(main_com_used_GF->nonSorted, ((struct_HD_BM_algorithmComponent*)main_indi_HD_decordComponents[KIND_OF_BCH_DECODING_BM])->sumReceiveAndErrLocSyndrome))
                                    {
                                        main_indi_errExceed_but_syndSum_zero_cnt[KIND_OF_BCH_DECODING_BM]++;

                                        warningMes;
                                        printf(" main_com_errNums_inDecodingAlgorithmTest = ('%d') > HD correctability[%d] = ('%d')\r\n", main_com_errNums_inDecodingAlgorithmTest, (*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i)), main_indi_hd_correctability[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]);
                                        warningMesShort; printf("sum syndrome and error syndrome is zero is detected.\r\n");
                                        warningMesShort; printf("This case will be serious problem in BM decoding algorithm.\r\n");
                                        if(!(global_flag_cmdSystemUnhold&FLAG_MASK_SYSTEM_UNHOLD_FORCED))
                                        {
                                            systemHoldMes;    printf("input any charactor : ");
                                            scanf("%s", global_buf_KeyBoardInput);
                                        }
                                        //systemHoldMes;    printf("input : ");
                                        //scanf("%s", global_buf_KeyBoardInput);
                                    }
                                }
                            }
                        }
                    }


                            #ifndef RELEASE
                            if(global_flag_debug_display&FLAG_MASK_DISPLAY_FUNCNAME)
                            {
                                debugFuncNameMes;
                                printf("closeBmAlgorithmComponent\r\n");
                            }
                            #endif
                    for(main_tmp_sel_decAlgo_i=0; main_tmp_sel_decAlgo_i<processingUseThisAlgorithm->length; main_tmp_sel_decAlgo_i++)
                    {
                                #ifndef RELEASE
                                if(global_flag_debug_display&FLAG_MASK_DISPLAY_FUNCNAME)
                                {
                                    debugFuncNameMes;
                                    printf("\t[%s] errorCorrection [start]\r\n", KIND_OF_BCH_DECODING_ALGORITHM_NAME[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]);
                                }
                                #endif
                        //closeBmAlgorithmComponent(((struct_HD_BM_algorithmComponent**)(main_indi_HD_decordComponents+KIND_OF_BCH_DECODING_BM)));
                        closeAlgoriehmComponent((main_indi_HD_decordComponents+(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))), (*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i)));
                                #ifndef RELEASE
                                if(global_flag_debug_display&FLAG_MASK_DISPLAY_FUNCNAME)
                                {
                                    debugFuncNameMes;
                                    printf("\t[%s] errorCorrection [end]\r\n", KIND_OF_BCH_DECODING_ALGORITHM_NAME[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]);
                                }
                                #endif
                    }

                    printf("[Chien Search Time]"); printExcutingTime(&time_chien_search_start);
                    printf("[Decoding Time]"); printExcutingTime(&time_decoding_start);

                                main_com_algorithm_loop_cnt++;
                                if(!(main_com_algorithm_loop_cnt<main_com_limit_algorithm_loop_cnt))
                                {
                                    for(main_tmp_sel_decAlgo_i=0; main_tmp_sel_decAlgo_i<processingUseThisAlgorithm->length; main_tmp_sel_decAlgo_i++)
                                    {
                                        printf("/***** algorithm test simulation result *****/\r\n");
                                        infoMes;    printf("[%s] Error occur nums = %d\r\n",
                                                        KIND_OF_BCH_DECODING_ALGORITHM_NAME[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))],
                                                        main_com_errNums_inDecodingAlgorithmTest
                                                    );
                                        infoMes;    printf("[%s] Error correctability = %d\r\n",
                                                        KIND_OF_BCH_DECODING_ALGORITHM_NAME[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))],
                                                        main_indi_hd_correctability[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]
                                                    );

                                        infoMes;    printf("[%s] FAIL cnt) err exceed correctale and less errLoc poly degree = %lu\r\n",
                                                        KIND_OF_BCH_DECODING_ALGORITHM_NAME[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))],
                                                        main_indi_errNumExceed_but_degErrLocPoly_not_max_cnt[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]
                                                    );
                                        infoMes;    printf("[%s] FAIL cnt) fail to correction = %lu\r\n",
                                                        KIND_OF_BCH_DECODING_ALGORITHM_NAME[(*(((uint8_t*)processingUseThisAlgorithm->list)+main_tmp_sel_decAlgo_i))],
                                                        main_indi_err_corection_fail_cnt[(*(((uint8_t*)processingUseThisAlgorithm->list)+main_tmp_sel_decAlgo_i))]
                                                    );
                                        infoMes;    printf("[%s] FAIL cnt) Error num is exceed and synds is zero = %lu\r\n",
                                                        KIND_OF_BCH_DECODING_ALGORITHM_NAME[(*(((uint8_t*)processingUseThisAlgorithm->list)+main_tmp_sel_decAlgo_i))],
                                                        main_indi_errExceed_but_synd_zero_cnt[(*(((uint8_t*)processingUseThisAlgorithm->list)+main_tmp_sel_decAlgo_i))]
                                                    );
                                        infoMes;    printf("[%s] FAIL cnt) Error num is exceed and sum synds and errLoc synd is zero = %lu\r\n",
                                                        KIND_OF_BCH_DECODING_ALGORITHM_NAME[(*(((uint8_t*)processingUseThisAlgorithm->list)+main_tmp_sel_decAlgo_i))],
                                                        main_indi_errExceed_but_syndSum_zero_cnt[(*(((uint8_t*)processingUseThisAlgorithm->list)+main_tmp_sel_decAlgo_i))]
                                                    );
                                        infoMes;    printf("[%s] FAIL cnt) fail to correction and all synds are zero = %lu\r\n",
                                                        KIND_OF_BCH_DECODING_ALGORITHM_NAME[(*(((uint8_t*)processingUseThisAlgorithm->list)+main_tmp_sel_decAlgo_i))],
                                                        main_indi_err_detect_and_correct_fail_cnt[(*(((uint8_t*)processingUseThisAlgorithm->list)+main_tmp_sel_decAlgo_i))]
                                                    );
                                        infoMes;    printf("[%s] FAIL cnt) Codeword loop %lu\r\n",
                                                        KIND_OF_BCH_DECODING_ALGORITHM_NAME[(*(((uint8_t*)processingUseThisAlgorithm->list)+main_tmp_sel_decAlgo_i))],
                                                        main_com_algorithm_loop_cnt
                                                    );

                                        infoMes;    printf("[%s] FAIL percent) err exceed correctale and less errLoc poly degree = %g\r\n",
                                                        KIND_OF_BCH_DECODING_ALGORITHM_NAME[(*(((uint8_t*)processingUseThisAlgorithm->list)+main_tmp_sel_decAlgo_i))],
                                                        (((double)main_indi_errNumExceed_but_degErrLocPoly_not_max_cnt[(*(((uint8_t*)processingUseThisAlgorithm->list)+main_tmp_sel_decAlgo_i))])/((double)main_com_algorithm_loop_cnt))
                                                    );
                                        infoMes;    printf("[%s] FAIL percent) fail to correction = %g\r\n",
                                                        KIND_OF_BCH_DECODING_ALGORITHM_NAME[(*(((uint8_t*)processingUseThisAlgorithm->list)+main_tmp_sel_decAlgo_i))],
                                                        (((double)main_indi_err_corection_fail_cnt[(*(((uint8_t*)processingUseThisAlgorithm->list)+main_tmp_sel_decAlgo_i))])/((double)main_com_algorithm_loop_cnt))
                                                    );
                                        infoMes;    printf("[%s] FAIL percent) Error num is exceed and synds is zero = = %g\r\n",
                                                        KIND_OF_BCH_DECODING_ALGORITHM_NAME[(*(((uint8_t*)processingUseThisAlgorithm->list)+main_tmp_sel_decAlgo_i))],
                                                        (((double)main_indi_errExceed_but_synd_zero_cnt[(*(((uint8_t*)processingUseThisAlgorithm->list)+main_tmp_sel_decAlgo_i))])/((double)main_com_algorithm_loop_cnt))
                                                    );
                                        infoMes;    printf("[%s] FAIL percent) Error num is exceed and sum synds and errLoc synd is zero = %g\r\n",
                                                        KIND_OF_BCH_DECODING_ALGORITHM_NAME[(*(((uint8_t*)processingUseThisAlgorithm->list)+main_tmp_sel_decAlgo_i))],
                                                        (((double)main_indi_errExceed_but_syndSum_zero_cnt[(*(((uint8_t*)processingUseThisAlgorithm->list)+main_tmp_sel_decAlgo_i))])/((double)main_com_algorithm_loop_cnt))
                                                    );
                                        infoMes;    printf("[%s] FAIL percent) fail to correction and all synds are zero = %g\r\n",
                                                        KIND_OF_BCH_DECODING_ALGORITHM_NAME[(*(((uint8_t*)processingUseThisAlgorithm->list)+main_tmp_sel_decAlgo_i))],
                                                        (((double)main_indi_err_detect_and_correct_fail_cnt[(*(((uint8_t*)processingUseThisAlgorithm->list)+main_tmp_sel_decAlgo_i))])/((double)main_com_algorithm_loop_cnt))
                                                    );
                                        return 0;
                                    }
                                }
                }
                /**************************************************************/
                /************ (Global Flag) AWGN simulation start *************/
                /**************************************************************/
                else
                {
                    /*
                     * Hard and Soft Decision is commonly use modulation of bpsk,
                     * passing through AWGN and demodulation of bpsk.
                     * If simulation is Hard decision mode, demodulating use just demodulation of bpsk.
                     * And if simulation is Soft decision mode, demodulating use quantization of bpsk.
                     * [start]
                     */
                    gettimeofday(&time_decoding_start, NULL);
                            #ifndef RELEASE
                            if(global_flag_debug_display&FLAG_MASK_DISPLAY_FUNCNAME)
                            {
                                debugFuncNameMes;
                                printf("recreateErrorComponent\r\n");
                            }
                            #endif
                    main_com_errComponents=recreateErrorComponent(&main_com_errComponents, main_com_codeLength);
                            #ifndef RELEASE
                            if((global_flag_debug_display&FLAG_MASK_DISPLAY_FUNCNAME)||(global_flag_debug_awgn&FLAG_MASK_DEBUG_AWGN_SEQUENCE))
                            {
                                debugFuncNameMes;
                                printf("BPSK_Mod\r\n");
                            }
                            #endif
                    BPSK_Mod(main_com_bpskComponents->bpskTransData, main_encodingComponentInGF->codeWord->equation, main_encodingComponentInGF->codeWord->usedLength);

                    /* HARD DECISION */
                    /* ADD_AWGN(main_com_bpskComponents->bpskTransData, main_com_bpskComponents->bpskReceivedDataAddedAwgn, 1, main_com_codeLength, main_com_EbN0); */ //need to add bitrate
                    /* SOFT DECISION */
                            #ifndef RELEASE
                            if(global_flag_debug_display&FLAG_MASK_DISPLAY_FUNCNAME)
                            {
                                debugFuncNameMes;
                                printf("ADD_AWGN_CAL_LLR\r\n");
                            }
                            #endif
                    /***** ADD_AWGN_CAL_LLR is don't care, hard-decision and soft-decision *****/
                    ADD_AWGN_CAL_LLR(
                        main_com_bpskComponents->bpskTransData,
                        main_com_bpskComponents->bpskReceivedDataAddedAwgn,
                        main_com_bpskComponents->bpskReceivedLLR->llr,
                        ((global_flag_Channel_Mode_Func&FLAG_MASK_CHANNEL_MODE_FUNC_BITRATE_DISABLE)?1.0:main_com_codeRate),
                        main_com_codeLength,
                        main_com_EbN0
                    );//need to add bitrate
                    //getSqureRootAvrBpskReceivedDataAddedAwgn(main_com_bpskComponents);
                    //getSqureRootAvrLLR(main_com_bpskComponents->bpskReceivedLLR);
                            #ifndef RELEASE
                            if((global_flag_debug_display&FLAG_MASK_DISPLAY_PROGRESS)||(global_flag_debug_awgn&FLAG_MASK_DEBUG_AWGN_SEQUENCE))
                            {
                                debugAwgnSequenceMes;
                                printf("SIGNAL + ADDITIVE WHITE GAUSIAN NOISE\r\n");
                                printBpskModulation(main_com_bpskComponents->usedLength, main_com_bpskComponents->bpskReceivedDataAddedAwgn);
                            }
                            if((global_flag_debug_display&FLAG_MASK_DISPLAY_PROGRESS)||(global_flag_debug_awgn&FLAG_MASK_DEBUG_AWGN_SEQUENCE))
                            {
                                debugAwgnSequenceMes;
                                printf("ADDITIVE WHITE GAUSIAN NOISE\r\n");
                                printBpskAWGN(main_com_bpskComponents->usedLength, main_com_bpskComponents->bpskReceivedDataAddedAwgn, main_com_bpskComponents->bpskTransData);
                            }
                            if((global_flag_debug_display&FLAG_MASK_DISPLAY_PROGRESS)||(global_flag_debug_awgnLLR&FLAG_MASK_DEBUG_AWGN_LLR_SEQUENCE))
                            {
                                debugAwgnLlrSequenceMes;
                                printLLRWithSpaceAndEnter(main_com_bpskComponents->bpskReceivedLLR);
                            }
                            #endif

                    gettimeofday(&time_decoding_start, NULL);
                    /* HARD DECISION */
                    if(!(global_flag_sim_decodingMode&FLAG_MASK_CODE_BCH_DEC_AL_SOFT_DECISION))
                    {
                                #ifndef RELEASE
                                if(global_flag_debug_display&FLAG_MASK_DISPLAY_FUNCNAME)
                                {
                                    debugFuncNameMes;
                                    printf("BPSK_Demod\r\n");
                                }
                                #endif
                        BPSK_Demod(main_com_errComponents->erroredCodeWord->equation, main_com_bpskComponents->bpskReceivedDataAddedAwgn, main_com_codeLength);
                    }
                    /* SOFT DECISION */
                    else
                    {
                                #ifndef RELEASE
                                if(global_flag_debug_display&FLAG_MASK_DISPLAY_FUNCNAME)
                                {
                                    debugFuncNameMes;
                                    printf("quantizationWithGlobalAdaptive\r\n");
                                }
                                #endif

                        quantizationWithGlobalAdaptive(
                            main_com_bpskComponents->bpskReceivedLLR->llr,
                            main_com_bpskComponents->bpskReceivedLLR->quantizedLLR,
                            main_com_errComponents->erroredCodeWord->equation,
                            main_com_errComponents->erroredCodeWord->usedLength
                        );
                        /* Received Vector == erroredCodeWord*/
                        /* File Out Patterns */
                        if(global_flag_file_io_hd_pattern_output&FLAG_MASK_FILE_IO_HD_RECEIVED_MES)
                        {
                            fprintPowerFormUsingAddress\
                                (fileio_HD_received_mes, main_com_errComponents->erroredCodeWord, "\r\n");
                        }
                        /* File Out Verilog Vectors start */
                        if(global_flag_file_io_hd_pattern_output_verilog&FLAG_MASK_FILE_IO_HD_RECEIVED_MES)
                        {
                            fprintVerilogPowerFormUsingAddress\
                                (fileio_HD_received_mes_verilog, main_com_errComponents->erroredCodeWord, NULL);
                        }
                        /* File Out Pattern log */
                        if(global_flag_file_io_hd_pattern_output_log&FLAG_MASK_FILE_IO_HD_RECEIVED_MES)
                        {
                            fprintPowerFormFullDescriptionUsingAddresss\
                                (fileio_HD_received_mes_log, main_encodingComponentInGF->codeWord, "\r\n");
                        }
                        /* File Out Verilog Vectors end */
                                #ifndef RELEASE
                                /* quantized LLR test print */
                                if((global_flag_debug_display&FLAG_MASK_DISPLAY_PROGRESS)||(global_flag_debug_awgnLLR&FLAG_MASK_DEBUG_AWGN_LLR_SEQUENCE))
                                {
                                    debugAwgnLlrSequenceMes;
                                    testPrintLLRWithSpaceAndEnter(main_com_bpskComponents->bpskReceivedLLR);

                                    debugAwgnLlrSequenceMes;
                                    printQuatizLLRWithSpaceAndEnter(main_com_bpskComponents->bpskReceivedLLR);

                                    debugAwgnLlrSequenceMes;
                                    printTestMagitudeQuatizLLRWithSpaceAndEnter(main_com_bpskComponents->bpskReceivedLLR);
                                }
                                #endif
                                if(global_flag_file_io_sd_pattern_output & FLAG_MASK_FILE_IO_SD_RECEIVED_LLR)
                                {
                                    fprintQuatizLLR_toHEX(fileio_SD_received_LLR, \
                                            main_com_bpskComponents->bpskReceivedLLR, "\r\n");
                                }
                                if(global_flag_file_io_sd_pattern_output_verilog & FLAG_MASK_FILE_IO_SD_RECEIVED_LLR)
                                {
                                    fprintVerilogQuatizLLR_toHEX(fileio_SD_received_LLR_verilog, \
                                            main_com_bpskComponents->bpskReceivedLLR, "\r\n");
                                }
                                if(global_flag_file_io_sd_pattern_output_log & FLAG_MASK_FILE_IO_SD_RECEIVED_LLR)
                                {
                                    fprintQuatizLLR_fullDescriptionToHEX(fileio_SD_received_LLR_log, \
                                            main_com_bpskComponents->bpskReceivedLLR, "\r\n");
                                }

                                            #ifndef RELEASE
                                            if(global_flag_debug_display&FLAG_MASK_DISPLAY_FUNCNAME)
                                            {
                                                debugFuncNameMes;
                                                printf("findMinimumMagnitudeFindLocPushAway\r\n");
                                            }
                                            #endif
                                    /*
                                    //convertQuantizedLLRToMagnitude(main_com_bpskComponents->bpskReceivedLLR);
                                    //        printMagnitudeOfLLR_andLocationWithSpaceAndEnter(main_com_bpskComponents->bpskReceivedLLR);
                                    //
                                    //sortMinimumMagnitudeLogLikeHoodRatio_chaseAlgorithm(main_com_bpskComponents->bpskReceivedLLR);
                                    //        testPrintValueOfTreeStruct(main_com_bpskComponents->bpskReceivedLLR->treeStruct);
                                    */

                        /*********************************************************************/
                        /* finding log likehood ratio based low reliable bits position start */
                        /*********************************************************************/
                        switch(global_flag_case_find_LLR_method)
                        {
                            case FLAG_CASE_FINDING_MIN_LLR_METHOD_MINIMUM:
                                convertQuantizedLLRToMagnitude(main_com_bpskComponents->bpskReceivedLLR);
                                findMinimumMagnitudeFindLocPushAway(
                                    main_com_bpskComponents->bpskReceivedLLR,
                                    main_com_errComponents->erroredCodeWord,
                                    global_flag_case_pass_hd_1,
                                    global_flag_case_same_llr_handling,
                                    global_flag_case_init_llr_mag_method
                                );

                                /*
                                //testPrintLLRWithSpaceAndEnter(
                                //    main_com_bpskComponents->bpskReceivedLLR);
                                //testPrintShortMinimumQuantizedLLRAndLocator(
                                //    main_com_bpskComponents->bpskReceivedLLR);
                                */

                                checkMinimumMagnitudeLocatorValidityLogLikeHoodRatio(
                                    main_com_bpskComponents->bpskReceivedLLR,
                                    main_encodingComponentInGF->codeWord,
                                    main_com_errComponents->erroredCodeWord
                                );
                            break;

                            case FLAG_CASE_FINDING_MIN_LLR_METHOD_GROUPING:
                                convertQuantizedLLRToMagnitude(main_com_bpskComponents->bpskReceivedLLR);
                                findMinimumMagnitudeGroupingPushAway
                                (
                                    main_com_bpskComponents->bpskReceivedLLR,
                                    main_com_errComponents->erroredCodeWord,
                                    global_flag_case_pass_hd_1,
                                    global_flag_case_same_llr_handling,
                                    global_flag_case_init_llr_mag_method,

                                    global_grouping_stream_nums,
                                    global_group_bit_num,
                                    global_group_last_bit_num
                                );
                                checkMinimumMagnitudeLocatorValidityLogLikeHoodRatio(
                                    main_com_bpskComponents->bpskReceivedLLR,
                                    main_encodingComponentInGF->codeWord,
                                    main_com_errComponents->erroredCodeWord
                                );
                            break;

                            case FLAG_CASE_FINDING_MIN_LLR_METHOD_TREE_FOLLOWING_MIN1_PATH:
                                convertQuantizedLLRToMagnitude(main_com_bpskComponents->bpskReceivedLLR);
                                        if(global_flag_file_io_sd_pattern_output & \
                                                FLAG_MASK_FILE_IO_SD_RECEIVED_LLR_mag)
                                        {
                                            fprintMagnitudeOfQuantizedLLR_toHex\
                                                (fileio_SD_received_LLR_mag, \
                                                 main_com_bpskComponents->bpskReceivedLLR, "\r\n");
                                        }
                                        if(global_flag_file_io_sd_pattern_output_verilog & \
                                                FLAG_MASK_FILE_IO_SD_RECEIVED_LLR_mag)
                                        {
                                            fprintVerilogMagnitudeOfQuantizedLLR_toHex\
                                                (fileio_SD_received_LLR_mag_verilog, \
                                                 main_com_bpskComponents->bpskReceivedLLR, "\r\n");
                                        }
                                        if(global_flag_file_io_sd_pattern_output_log & \
                                                FLAG_MASK_FILE_IO_SD_RECEIVED_LLR_mag)
                                        {
                                            fprintMagnitudeOfQuantizedLLR_fullDescription_toHex\
                                                (fileio_SD_received_LLR_mag_log, \
                                                 main_com_bpskComponents->bpskReceivedLLR, "\r\n");
                                        }

                                sortMinimumMagnitudeLogLikeHoodRatio_chaseAlgorithm\
                                    (main_com_bpskComponents->bpskReceivedLLR,
                                    main_com_errComponents->erroredCodeWord,
                                    global_flag_case_pass_hd_1,
                                    global_flag_case_same_llr_handling);
                                findMinimumMagnitudeLogLikeHoodRatio_chaseAlgorithm_followMinPath(
                                    main_com_bpskComponents->bpskReceivedLLR);
                                checkMinimumMagnitudeLocatorValidityLogLikeHoodRatio
                                    (
                                        main_com_bpskComponents->bpskReceivedLLR,
                                        main_encodingComponentInGF->codeWord,
                                        main_com_errComponents->erroredCodeWord
                                    );
                                //findMinimumMagnitudeFindLocPushAway(main_com_bpskComponents->bpskReceivedLLR);
                            break;
                        }
                        /*******************************************************************/
                        /* finding log likehood ratio based low reliable bits position end */
                        /*******************************************************************/

                                #ifndef RELEASE
                                if((global_flag_debug_display&FLAG_MASK_DISPLAY_PROGRESS)||(global_flag_debug_awgnLLR&FLAG_MASK_DEBUG_AWGN_LLR_SEQUENCE))
                                {
                                    debugAwgnLlrSequenceMes;
                                    testPrintShortMinimumQuantizedLLRAndLocator(main_com_bpskComponents->bpskReceivedLLR);
                                    printf("[main_com_EbN0]\t%g\r\n", main_com_EbN0);
                                }
                                #endif

                                #ifndef RELEASE
                                if((global_flag_debug_display&FLAG_MASK_DISPLAY_PROGRESS))
                                {
                                    infoMes;
                                    printf("AWGN Hard decision error vector.\r\n");
                                    printPowerFormExclusivedUsingAddress(main_com_errComponents->erroredCodeWord, main_encodingComponentInGF->codeWord);
                                }
                                #endif
                    }
                    /*
                     * Hard and Soft Decision is commonly use modulation of bpsk,
                     * passing through AWGN and demodulation of bpsk.
                     * If simulation is Hard decision mode, demodulating use just demodulation of bpsk.
                     * And if simulation is Soft decision mode, demodulating use quantization of bpsk.
                     * [start]
                     */


                    /************************************************************/
                    /** Hard Decision also use main_com_list_TP_pwrFormPoly **/
                    /************************************************************/
                    main_com_list_TP_pwrFormPoly[0] = main_com_errComponents->erroredCodeWord;
                    /************************************************************/
                    /***** Soft Decision Only, Test Vector Generation start *****/
                    /************************************************************/
                    if(global_flag_sim_decodingMode&FLAG_MASK_CODE_BCH_DEC_AL_SOFT_DECISION)
                    {
                        /* Test Pattern Sting Open start*/
                                    #ifndef RELEASE
                                    if(global_flag_debug_display&FLAG_MASK_DISPLAY_FUNCNAME)
                                    {
                                        debugFuncNameMes;
                                        printf("temporaryFunc_generateTestPatterns_using_LLR_Locator\r\n");
                                    }
                                    #endif
                        if(temporaryFunc_generateTestPatterns_using_LLR_Locator
                            (
                                (char*)main_com_errComponents->erroredCodeWord->equation,
                                main_com_errComponents->erroredCodeWord->usedLength,
                                main_com_bpskComponents->bpskReceivedLLR->locator,
                                main_com_bpskComponents->bpskReceivedLLR->locatorLength,
                                main_com_strBuf_TP_saved,
                                main_com_numsTP
                            )
                        )
                        {
                            errorMes;
                            printf("temporaryFunc_generateTestPatterns_using_LLR_Locator, return -1.\r\n");
                        }

                        for(main_tmp_soft_i=1; main_tmp_soft_i<main_com_numsTP; main_tmp_soft_i++)
                        {
                            setPowerFormPolynomialUsingStringAddr_clearStringPointer(
                                main_com_list_TP_pwrFormPoly[main_tmp_soft_i],
                                main_com_strBuf_TP_saved+main_tmp_soft_i,
                                main_com_codeLength,
                                main_com_codeLength
                            );
                        }
                        /* All main_com_strBuf_TP_saved[n] is NULL */

                                #ifndef RELEASE
                                if(
                                    (global_flag_case_getBER_method_Display&FLAG_MASK_DISPLAY_PROGRESS)||
                                    (global_flag_debug_display&FLAG_MASK_DISPLAY_PROGRESS)||
                                    (global_flag_bch_BM_SimSD_display&FLAG_MASK_DISPLAY_PROGRESS)||
                                    (global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)
                                )
                                {
                                    printf("[Hard-Decision %d]\r\n", main_tmp_soft_i);
                                    //printf("%s\r\n", *(main_com_strBuf_TP_saved+0));
                                    printPowerFormWithEnterPolynomialWithEnterUsingAddress(main_com_list_TP_pwrFormPoly[0]);


                                    /*main_tmp_soft_i is index of main_com_strBuf_TP_saved.*/
                                    for(main_tmp_soft_i=1; main_tmp_soft_i<main_com_numsTP; main_tmp_soft_i++)
                                    {
                                        printf("[TestPattern %d]\r\n", main_tmp_soft_i);
                                        //printf("%s\r\n", *(main_com_strBuf_TP_saved+main_tmp_soft_i));
                                        printPowerFormWithEnterPolynomialWithEnterUsingAddress(main_com_list_TP_pwrFormPoly[main_tmp_soft_i]);

                                        printf("[LLR Locator %d]\r\n", main_tmp_soft_i);

                                        //Just printf string use only value of locator.
                                        //Using i to printf string just related with locators, until i is less then codeword length
                                        for(i=0; i<main_com_errComponents->erroredCodeWord->usedLength; i++)
                                        {
                                            //use non-binary code calculate combination of bit position
                                            for(k=0; k<main_tmp_degErrLocPoly; k++)//k is index of locator
                                            {

                                                if(main_tmp_soft_i&(1<<k))//k is index of locatorArray.
                                                {
                                                    if(i==(*(main_com_bpskComponents->bpskReceivedLLR->locator+k)))
                                                    {
                                                        printf("1");
                                                        break;
                                                    }
                                                }
                                            }
                                            if(k==main_tmp_degErrLocPoly) printf("0");
                                        }

                                        printf("\r\n");

                                        printf("[Comparing testPattern and Hard Decision %d]\r\n", main_tmp_soft_i);
                                        printPowerFormExclusivedUsingAddress(main_com_list_TP_pwrFormPoly[0], main_com_list_TP_pwrFormPoly[main_tmp_soft_i]);

                                    }
                                }
                                #endif
                        /* Test Pattern Sting Open End*/
                    }
                    /************************************************************/
                    /****** Soft Decision Only, Test Vector Generation end ******/
                    /************************************************************/






                    /************************************************************/
                    /****** calculate error vector hamming weight start ******/
                    /************************************************************/
                    main_com_channel_errCnt = calculateHammingWeightFromDiffentPowerFormPolynomial(
                                                                main_encodingComponentInGF->codeWord,
                                                                main_com_list_TP_pwrFormPoly[0]
                                                            );
                    main_indi_list_errCntOf_TP_beforeDec[0] = main_com_channel_errCnt;

                    for(main_tmp_soft_i = 1; main_tmp_soft_i < main_com_numsTP; main_tmp_soft_i++)
                    {
                        main_indi_list_errCntOf_TP_beforeDec[main_tmp_soft_i] =
                            calculateHammingWeightFromDiffentPowerFormPolynomial(
                                main_encodingComponentInGF->codeWord,
                                main_com_list_TP_pwrFormPoly[main_tmp_soft_i]
                            );
                    }
                                if(global_flag_file_io_sd_pattern_output_log & FLAG_MASK_FILE_IO_SD_RECEIVED_LLR)
                                {
                                    for(main_tmp_soft_i = 0; main_tmp_soft_i < main_com_numsTP; main_tmp_soft_i++)
                                    {
                                        fprintf(fileio_ErrCnt_log, "%d,\t", main_indi_list_errCntOf_TP_beforeDec[main_tmp_soft_i]);
                                    }
                                    fprintf(fileio_ErrCnt_log, "\r\n");
                                }

                    for(main_tmp_sel_decAlgo_i = 0; main_tmp_sel_decAlgo_i < processingUseThisAlgorithm->length; main_tmp_sel_decAlgo_i++)
                    {
                        /******************************************************/
                        /* flagging can beging correctable test-pattern start */
                        /******************************************************/
                        set_flag_theo_suc_list_TP(
                            main_indi_list_flag_theo_ly_can_be_correctable[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))],
                            main_com_numsTP,
                            main_indi_list_errCntOf_TP_beforeDec,
                            main_indi_hd_correctability[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]
                        );
                        /******************************************************/
                        /** flagging can beging correctable test-pattern end **/
                        /******************************************************/
                    }



                    /********************************************************************/
                    /************** Error counting, to calculate BER and ****************/
                    /************** selecting Codeword to decoding start ****************/
                    /********************************************************************/
                    /*
                     * note. counting different number encoded and passing through soft-decision codeword, all test pattern.
                     * Result of calculateHammingWeightFromDiffentPowerFormPolynomial is considered hard and soft decision correctability.
                     */


                    switch(global_flag_case_selTP_before_dec)
                    {
                        case FLAG_CASE_SEL_TP_BEFORE_DEC_THEO:
                        {
                            for(main_tmp_sel_decAlgo_i=0; main_tmp_sel_decAlgo_i<processingUseThisAlgorithm->length; main_tmp_sel_decAlgo_i++)
                            {
                                /*
                                 * sel correctable TP(main_indi_sel_TP_i) and to calculate BER, update err cnt(main_indi_sel_TP_errCnt)
                                 */
                                set_selTP_before_decording_skipDecOnly
                                (

                                    (main_indi_HD_errCnt+(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))),

                                    (main_indi_sel_TP_i+(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))),
                                    (main_indi_sel_TP_errCnt+(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))),

                                    main_indi_list_errCntOf_TP_afterDec[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))],

                                    (main_indi_sel_decoding_CW_pwrFormPoly+(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))),

                                    main_indi_hd_correctability[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))],

                                    main_com_numsTP,

                                    main_com_channel_errCnt,

                                    main_com_list_TP_pwrFormPoly,
                                    main_indi_list_errCntOf_TP_beforeDec,

                                    KIND_OF_BCH_DECODING_ALGORITHM_NAME[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]
                                );

                                /* theoretical HD err cnt start */
                                if(main_indi_list_flag_theo_ly_can_be_correctable[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))][0])
                                {
                                    main_indi_HD_errCnt[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))] = 0;
                                }
                                else
                                {
                                    main_indi_HD_errCnt[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))] = main_com_channel_errCnt;
                                }
                                /* theoretical HD err cnt end */
                            }
                        }
                        break;

                        case FLAG_CASE_SEL_TP_BEFORE_DEC_ACT:
                        {
                            for(main_tmp_sel_decAlgo_i=0; main_tmp_sel_decAlgo_i<processingUseThisAlgorithm->length; main_tmp_sel_decAlgo_i++)
                            {
                                set_unSelTp_bofore_decoding
                                (
                                    (main_indi_sel_TP_i+(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))),
                                    (main_indi_sel_TP_errCnt+(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))),

                                    main_indi_list_errCntOf_TP_afterDec[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))],

                                    (main_indi_sel_decoding_CW_pwrFormPoly+(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))),

                                    main_com_numsTP,

                                    main_com_list_TP_pwrFormPoly,
                                    main_indi_list_errCntOf_TP_beforeDec,

                                    KIND_OF_BCH_DECODING_ALGORITHM_NAME[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]
                                );
                            }
                            //errorMes; printf("Decoding processing skip mode in decoder error fail detection mode is not support yet.\r\n");
                            //errorMes; printf("simulation is closed.\r\n");
                            //return -1;
                        }
                        break;

                        default:
                        {
                            errorMes;
                            printf("Ib decoding processing skip mode, selecting test pattern method is not defined.\r\n");
                            printf("Program is closed.\r\n");
                            return -1;
                        }
                        break;
                    }







                    switch(global_flag_case_selTP_before_dec)
                    {
                        case FLAG_CASE_SEL_TP_BEFORE_DEC_THEO:
                        {
                            /********************************************************************/
                            /***** Decide decording, and if can, compute decoding algorithm *****/
                            /********************************************************************/
                            /*
                             * note. belong to main_indi_sel_TP_errCnt[KIND_OF_BCH_DECODING_BM], decoding result(BER) can be changed.
                             * If main_indi_sel_TP_errCnt[KIND_OF_BCH_DECODING_BM] is exceed to correctable(hard decision) bits,
                             * error bits are increased, reduced or no changed.
                             * Decording result is not predictable.
                             *
                             * Decoding target codeword is save at main_indi_sel_decoding_CW_pwrFormPoly[KIND_OF_BCH_DECODING_BM], variable type is struct_powerFormPolynomials*.
                             *
                             * Look decoding result, carefully.
                             *
                             * main_indi_sel_TP_errCnt[KIND_OF_BCH_DECODING_BM]
                             */

                            //(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))
                            for(main_tmp_sel_decAlgo_i=0; main_tmp_sel_decAlgo_i<processingUseThisAlgorithm->length; main_tmp_sel_decAlgo_i++)
                            {
                                /*************************************************/
                                /***** Correctable code decoding block start *****/
                                /*************************************************/
                                if(
                                    main_indi_list_flag_theo_ly_can_be_correctable
                                        [(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]
                                        [main_indi_sel_TP_i[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]]
                                )
                                //if(main_indi_sel_TP_errCnt[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))] <= main_indi_hd_correctability[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))])//try correcting
                                {
                                    /* decrease to main_indi_hd_correctability[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))] */

                                    /*******************************************************/
                                    /********* no decoding option added loc start **********/
                                    /*******************************************************/
                                    if(!(global_list_flag_bch_sim_decodingMode[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]&FLAG_MASK_CODE_BCH_DEC_AL_SKIP))
                                    {
                                                #ifndef RELEASE
                                                /* printing can decoding */
                                                if((global_flag_debug_display&FLAG_MASK_DISPLAY_PROGRESS)||(global_flag_cmdOption&FLAG_MASK_PRINTF_LOG))
                                                {
                                                    infoMes;
                                                    printf("[Theo] %s TP #%dCan decording!!!\r\n", ((global_list_flag_bch_sim_decodingMode[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]&FLAG_MASK_CODE_BCH_DEC_AL_SOFT_DECISION)?"[Soft-Decision]":"[Hard-Decision](TP #0 ==)"), main_indi_sel_TP_i[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]);

                                                    infoMes;
                                                    if(main_indi_list_flag_theo_ly_can_be_correctable
                                                        [(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]
                                                        [main_indi_sel_TP_i[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]]
                                                    ) printf("flag, '%d' test pattern has, indicate 'can' decoding(selected test-pattern is correctable), value = '%d'.\r\n",
                                                        main_indi_sel_TP_i[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))],
                                                        main_indi_list_flag_theo_ly_can_be_correctable
                                                            [(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]
                                                            [main_indi_sel_TP_i[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]]
                                                        );
                                                    else printf("[Theo] flag, '%d' test pattern has, indicate 'can not' decoding(selected test-pattern is correctable), value = '%d'.\r\n!!!WARNING!!! check setting code that main_indi_list_flag_theo_ly_can_be_correctable[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))][main_indi_sel_TP_i[main_tmp_sel_decAlgo_i]]\r\n", main_indi_sel_TP_i[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))], main_indi_list_flag_theo_ly_can_be_correctable[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))][main_indi_sel_TP_i[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]]);
                                                }
                                                /* printing selected error vectors */
                                                if((global_flag_debug_display&FLAG_MASK_DISPLAY_PROGRESS)||(global_flag_debug_bmSim&FLAG_MASK_DEBUG_BCH_BM_CAL_DELTA))
                                                {
                                                    debugBmAlgorithmCalDeltaMes;
                                                    printf("[Theo] [Error Vector]\r\n");
                                                    printPowerFormExclusivedUsingAddress(main_encodingComponentInGF->codeWord, main_indi_sel_decoding_CW_pwrFormPoly[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]);
                                                    printf("\r\n");
                                                }
                                                #endif

                                        /*************************************************/
                                        /***** create BCH decording components start *****/
                                        /*************************************************/
                                        /*
                                         * notes. create new BCH decoding components are not needs.
                                         * Removing this block needs to verify.
                                         */
                                        /*
                                         * note. To varifing unsigned int t, which is argument of recreateBmAlgorithmComponent,
                                         * can change length of stage of BM algorithm.
                                         *
                                         * Error count(hamming weight of error vector) exceed it's correctability,
                                         * simulator can be broken, because accessing invalid or wrong memory adress.
                                         */
                                                #ifndef RELEASE
                                                if(global_flag_debug_display&FLAG_MASK_DISPLAY_FUNCNAME)
                                                {
                                                    debugFuncNameMes; printf("[Theo][%s] recreateAlgoriehmComponent, %s, TP[%d] [start]\r\n",
                                                        KIND_OF_BCH_DECODING_ALGORITHM_NAME[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))],
                                                        (((global_list_flag_bch_sim_decodingMode[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]&FLAG_MASK_CODE_BCH_DEC_AL_SOFT_DECISION))?"Soft-Decision":"Hard-Decision"),
                                                        main_indi_sel_TP_i[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]);
                                                }
                                                #endif

                                        /*
                                         * notes. main_indi_SD_list_of_decordingComponents[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))] is a aaray, length is main_indi_sel_TP_i[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))].
                                         * To decoding all test pattern, coded like below.
                                         *
                                         * for([var_cnt]=0; [var_cnt]<[max_TP_length]; [var_cnt]++)
                                         * {
                                         *     *main_indi_SD_list_of_decordingComponents[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))][[var_cnt]] = recreateBmAlgorithmComponent
                                         *                             (
                                         *                                 *(main_indi_SD_list_of_decordingComponents[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]+main_indi_sel_TP_i[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]),
                                         *                                 main_com_used_GF->nonSorted, main_com_hd_correctability,
                                         *                                 (char*)main_com_list_TP_pwrFormPoly[[var_cnt]]->equation,
                                         *                                 main_com_codeLength
                                         *                             );
                                         * }
                                         */
                                        //**(main_indi_SD_list_of_decordingComponents[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]+main_indi_sel_TP_i[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]) = recreateBmAlgorithmComponent
                                        //((struct_HD_BM_algorithmComponent**)(main_indi_HD_decordComponents+(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))))
                                        *main_indi_SD_list_of_decordingComponents[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))][main_indi_sel_TP_i[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]] =
                                            recreateAlgoriehmComponent(
                                                (*((main_indi_SD_list_of_decordingComponents[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))])+main_indi_sel_TP_i[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))])),
                                                main_com_used_GF->nonSorted,
                                                main_com_hd_correctability,
                                                (char*)main_indi_sel_decoding_CW_pwrFormPoly[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]->equation,
                                                main_com_codeLength,
                                                (*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))
                                            );
                                                #ifndef RELEASE
                                                if(global_flag_debug_display&FLAG_MASK_DISPLAY_FUNCNAME)
                                                {
                                                    debugFuncNameMes; printf("[Theo][%s] recreateAlgoriehmComponent, %s, TP[%d] [end]\r\n",
                                                        KIND_OF_BCH_DECODING_ALGORITHM_NAME[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))],
                                                        (((global_list_flag_bch_sim_decodingMode[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]&FLAG_MASK_CODE_BCH_DEC_AL_SOFT_DECISION))?"Soft-Decision":"Hard-Decision"),
                                                        main_indi_sel_TP_i[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]);
                                                }
                                                #endif
                                        if(0 < allSyndromeIsZero
                                                (
                                                    main_com_used_GF->nonSorted,
                                                    (**(main_indi_SD_list_of_decordingComponents[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]+main_indi_sel_TP_i[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]))->syndrome
                                                )
                                        )
                                        {
                                            main_indi_list_flag_beforeDec_syndZero[(*(((uint8_t*)processingUseThisAlgorithm->list)+main_tmp_sel_decAlgo_i))][main_indi_sel_TP_i[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]]=1;
                                        }
                                        else
                                        {
                                            main_indi_list_flag_beforeDec_syndZero[(*(((uint8_t*)processingUseThisAlgorithm->list)+main_tmp_sel_decAlgo_i))][main_indi_sel_TP_i[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]]=0;
                                        }
                                        /*************************************************/
                                        /****** create BCH decording components end ******/
                                        /*************************************************/




                                        /*****************************************************************************/
                                        /************ Decoding Block start, continously fallowing       **************/
                                        /************ verification corrected codeword has any error     **************/
                                        /************ If not needs verification process, remove         **************/
                                        /************ this parts, but recommand that doing verification **************/
                                        /*****************************************************************************/

                                                #ifndef RELEASE
                                                if(global_flag_debug_display&FLAG_MASK_DISPLAY_FUNCNAME)
                                                {
                                                    debugFuncNameMes;
                                                    printf("calculateBmAlgorithm(main_com_used_GF->nonSorted, **(main_indi_SD_list_of_decordingComponents[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]+main_indi_sel_TP_i[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]));\r\n");
                                                }
                                                #endif
                                        //calculateBmAlgorithm(
                                        //    main_com_used_GF->nonSorted,
                                        //    ((struct_HD_BM_algorithmComponent*)(**(main_indi_SD_list_of_decordingComponents[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]+main_indi_sel_TP_i[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))])))
                                        //);
                                        calculateBCH_decodingAlgorithm(
                                            main_com_used_GF->nonSorted,
                                            (**(main_indi_SD_list_of_decordingComponents
                                                [(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]+
                                                    main_indi_sel_TP_i
                                                    [(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]
                                                    ))
                                        );

                                        if(
                                            ((**(main_indi_SD_list_of_decordingComponents[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]+
                                                    main_indi_sel_TP_i[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]
                                                    ))->degreeOfErrLocPoly)
                                            <
                                            main_indi_hd_correctability[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]
                                        )
                                        {
                                            main_indi_list_flag_degOfErrLocPolyNotMax
                                                [(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]
                                                [main_indi_sel_TP_i[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]] = 1;
                                        }
                                        else
                                        {
                                            main_indi_list_flag_degOfErrLocPolyNotMax
                                                [(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]
                                                [main_indi_sel_TP_i[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]] = 0;
                                        }

                                        gettimeofday(&time_chien_search_start, NULL);
                                                #ifndef RELEASE
                                                if(global_flag_debug_display&FLAG_MASK_DISPLAY_FUNCNAME)
                                                {
                                                    debugFuncNameMes;
                                                    printf("chienSearch_static(main_com_used_GF->nonSorted, (**(main_indi_SD_list_of_decordingComponents[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]+main_indi_sel_TP_i[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]))->errLocPoly, (**(main_indi_SD_list_of_decordingComponents[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]+main_indi_sel_TP_i[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]))->errorLocationVector, &((**(main_indi_SD_list_of_decordingComponents[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]+main_indi_sel_TP_i[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]))->metricCheck) );\r\n");
                                                }
                                                #endif
                                        //chienSearch_static(
                                        //    main_com_used_GF->nonSorted,
                                        //    (**(main_indi_SD_list_of_decordingComponents[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]+main_indi_sel_TP_i[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]))->errLocPoly,
                                        //    (**(main_indi_SD_list_of_decordingComponents[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]+main_indi_sel_TP_i[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]))->errorLocationVector,
                                        //    &((**(main_indi_SD_list_of_decordingComponents[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]+main_indi_sel_TP_i[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]))->metricCheck)
                                        //);
                                        chienSearch(
                                            main_com_used_GF->nonSorted,
                                            (**(main_indi_SD_list_of_decordingComponents[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]+main_indi_sel_TP_i[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]))->errLocPoly,
                                            (**(main_indi_SD_list_of_decordingComponents[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]+main_indi_sel_TP_i[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]))->errorLocationVector,
                                            &((**(main_indi_SD_list_of_decordingComponents[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]+main_indi_sel_TP_i[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]))->metricCheck),
                                            (*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))
                                        );
                                                #ifndef RELEASE
                                                if((global_flag_debug_display&FLAG_MASK_DISPLAY_PROGRESS)||(global_flag_cmdOption&FLAG_MASK_PRINTF_LOG))
                                                {
                                                    debugSequenceMes;
                                                    printf("[Theo][%s], TP[%d] Error Location Vector is...\r\n",
                                                        KIND_OF_BCH_DECODING_ALGORITHM_NAME[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))],
                                                        main_indi_sel_TP_i[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]
                                                    );
                                                    printPowerFormWithEnterPolynomialWithEnterUsingAddress((**(main_indi_SD_list_of_decordingComponents[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]+main_indi_sel_TP_i[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]))->errorLocationVector);
                                                }
                                                #endif

                                                #ifndef RELEASE
                                                if(global_flag_debug_display&FLAG_MASK_DISPLAY_FUNCNAME)
                                                {
                                                    debugFuncNameMes;
                                                    printf("[Theo][%s], TP[%d] errorCorrection [start]\r\n",
                                                        KIND_OF_BCH_DECODING_ALGORITHM_NAME[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))],
                                                        main_indi_sel_TP_i[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]
                                                    );
                                                }
                                                #endif

                                        if(!main_indi_list_flag_beforeDec_syndZero[(*(((uint8_t*)processingUseThisAlgorithm->list)+main_tmp_sel_decAlgo_i))][main_indi_sel_TP_i[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]])
                                        {
                                            errorCorrection(
                                                (**(main_indi_SD_list_of_decordingComponents[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]+main_indi_sel_TP_i[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]))->codeWord,
                                                (**(main_indi_SD_list_of_decordingComponents[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]+main_indi_sel_TP_i[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]))->errorLocationVector
                                            );
                                                    /* File Out Corrected Vector start */
                                                    /* File Out Patterns */
                                                    if(global_flag_file_io_hd_pattern_output&FLAG_MASK_FILE_IO_HD_CORRECTED_MES)
                                                    {
                                                        fprintPowerFormUsingAddress\
                                                            (\
                                                                fileio_corrected_mes[
                                                                    (*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))
                                                                ],
                                                                main_indi_HD_decordComponents[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]->codeWord,
                                                                "\r\n"
                                                            );
                                                    }
                                                    /* File Out Verilog Vectors start */
                                                    if(global_flag_file_io_hd_pattern_output_verilog&FLAG_MASK_FILE_IO_HD_CORRECTED_MES)
                                                    {
                                                        fprintVerilogPowerFormUsingAddress\
                                                            (
                                                                fileio_corrected_mes_verilog[
                                                                    (*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))
                                                                ],
                                                                main_indi_HD_decordComponents[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]->codeWord,
                                                                NULL
                                                            );
                                                    }
                                                    /* File Out Pattern log */
                                                    if(global_flag_file_io_hd_pattern_output_log&FLAG_MASK_FILE_IO_HD_CORRECTED_MES)
                                                    {
                                                        fprintPowerFormFullDescriptionUsingAddresss\
                                                            (
                                                                fileio_corrected_mes_log[
                                                                    (*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))
                                                                ],
                                                                main_indi_HD_decordComponents[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]->codeWord,
                                                                "\r\n"
                                                            );
                                                    }
                                                    /* File Out Corrected Vector end */
                                        }

                                                #ifndef RELEASE
                                                if(global_flag_debug_display&FLAG_MASK_DISPLAY_FUNCNAME)
                                                {
                                                    debugFuncNameMes;
                                                    printf("[Theo][%s], TP[%d] errorCorrection [end]\r\n",
                                                        KIND_OF_BCH_DECODING_ALGORITHM_NAME[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))],
                                                        main_indi_sel_TP_i[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]
                                                    );
                                                }
                                                #endif

                                                #ifndef RELEASE
                                                if((global_flag_debug_display&FLAG_MASK_DISPLAY_PROGRESS)||(global_flag_cmdOption&FLAG_MASK_PRINTF_LOG))
                                                {
                                                    debugSequenceMes;
                                                    printf("[Theo][%s], TP[%d] Corrected Vector is...\r\n",
                                                        KIND_OF_BCH_DECODING_ALGORITHM_NAME[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))],
                                                        main_indi_sel_TP_i[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]
                                                    );
                                                    printPowerFormWithEnterPolynomialWithEnterUsingAddress(
                                                        (**(main_indi_SD_list_of_decordingComponents[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]+main_indi_sel_TP_i[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]))->codeWord
                                                    );
                                                }
                                                #endif
                                        /*****************************************************************************/
                                        /************ Decoding Block end, continously fallowing         **************/
                                        /************ verification corrected codeword has any error     **************/
                                        /************ If not needs verification process, remove         **************/
                                        /************ this parts, but recommand that doing verification **************/
                                        /*****************************************************************************/
                                        /****************** verification corrected codeword start ********************/
                                        /*****************************************************************************/
                                        /* If use multiDecording Mode, needs to initial this start */
                                        //for(main_tmp_soft_i=0; main_tmp_soft_i<main_com_numsTP; main_tmp_soft_i++)
                                        //{
                                        //    main_indi_list_errCntOf_TP_afterDec[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))][main_tmp_soft_i] = 0;
                                        //}
                                        /* If use multiDecording Mode, needs to initial this end */
                                        /*
                                         * note. main_indi_sel_decoding_CW_pwrFormPoly[KIND_OF_BCH_DECODING_BM]
                                         */
                                        main_indi_list_errCntOf_TP_afterDec
                                        [(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]
                                        [main_indi_sel_TP_i[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]] =
                                        calculateHammingWeightFromDiffentPowerFormPolynomial(
                                            main_encodingComponentInGF->codeWord,
                                            main_indi_sel_decoding_CW_pwrFormPoly[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]
                                        );
                                                #ifndef RELEASE
                                                if((global_flag_debug_display&FLAG_MASK_DISPLAY_PROGRESS)||(global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)||(testOption&FLAG_MASK_TEST_RESULT)||(global_flag_debug_bmSim&FLAG_MASK_DEBUG_BCH_BM_CAL_DELTA))
                                                {
                                                    infoMes; printf("[Theo][%s] sel TP = [%d], TP[%d] has '%d'(e = %d) errs\t",
                                                        KIND_OF_BCH_DECODING_ALGORITHM_NAME[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))],
                                                        main_indi_sel_TP_i[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))],
                                                        main_indi_sel_TP_i[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))],
                                                        main_indi_list_errCntOf_TP_beforeDec[main_indi_sel_TP_i[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]],
                                                        main_indi_list_errCntOf_TP_beforeDec[main_indi_sel_TP_i[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]]
                                                    );
                                                    infoMes; printf("[Theo]%s TP #%d Decording ", ((global_list_flag_bch_sim_decodingMode[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]&FLAG_MASK_CODE_BCH_DEC_AL_SOFT_DECISION)?"[Soft-Decision]":"[Hard-Decision](TP #0 ==)"), main_indi_sel_TP_i[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]);
                                                    if(!main_indi_list_errCntOf_TP_afterDec[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))][main_indi_sel_TP_i[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]])    printf("Success\r\n");
                                                    else                                                    printf("Fail\r\n");
                                                }
                                                #endif
                                        /*****************************************************************************/
                                        /****************** verification corrected codeword start ********************/
                                        /*****************************************************************************/




                                        /**************************************************/
                                        /****** close BCH decording components start ******/
                                        /**************************************************/
                                        /*
                                         * notes. Not onlly create block, close new BCH decoding components are not needs.
                                         * Removing this block needs to verify.
                                         */
                                        //if(!(global_list_flag_bch_sim_decodingMode[KIND_OF_BCH_DECODING_BM]&FLAG_MASK_CODE_BCH_DEC_AL_SOFT_DECISION))
                                        //closeBmAlgorithmComponent((struct_HD_BM_algorithmComponent**)(*(main_indi_SD_list_of_decordingComponents[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]+main_indi_sel_TP_i[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))])));
                                        closeAlgoriehmComponent((*(main_indi_SD_list_of_decordingComponents[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]+main_indi_sel_TP_i[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))])), (*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i)));

                                        /**************************************************/
                                        /******* close BCH decording components end *******/
                                        /**************************************************/
                                    }

                                    /*******************************************************/
                                    /********** no decoding option added loc end ***********/
                                    /*******************************************************/

                                }
                                /*************************************************/
                                /****** Correctable code decoding block end ******/
                                /*************************************************/
                            }
                        }
                        break;


                        /********************************************************************/
                        /************** get BER mode is actual mode, that is  ***************/
                        /************** BER can be different with theoretical ***************/
                        /************** case start                            ***************/
                        /********************************************************************/
                        /*
                         * note. Actual error count.
                         * Actual is mean, simulator does not known that channel enviroments,
                         * that is mean simulator can not recognize error number corrrectly.
                         * So in below code and actual BER mode,
                         * simulator can not choose best test pattern has minimum error number.
                         *
                         * Simulator have to identify best test pattern using decoding infomation.
                         * And choose best case.
                         */
                        case FLAG_CASE_SEL_TP_BEFORE_DEC_ACT:
                        {
                                #ifndef RELEASE
                                if((global_flag_case_getBER_method_Display&FLAG_MASK_DISPLAY_PROGRESS)||(global_flag_debug_display&FLAG_MASK_DISPLAY_PROGRESS)||(global_flag_bch_BM_SimSD_display&FLAG_MASK_DISPLAY_PROGRESS)||(global_flag_cmdOption&FLAG_MASK_PRINTF_LOG))
                                {
                                    printf("[ACT] >>>>> ******************************************************************** <<<<<\r\n");
                                    printf("[ACT] >>>>> *************************************************************** <<<<<\r\n");
                                    printf("[ACT] >>>>> *************************************************************** <<<<<\r\n");
                                    infoMes; printf("[ACT] Encoded Vector is below...\r\n");
                                    printPowerFormWithEnterPolynomialWithEnterUsingAddress(main_encodingComponentInGF->codeWord);
                                    infoMes; printf("[ACT] Error Vector is below...\r\n");
                                    printPowerFormExclusivedUsingAddress(main_com_errComponents->erroredCodeWord, main_encodingComponentInGF->codeWord);
                                    infoMes; printf("[ACT] Received Vector is below...\r\n");
                                    printPowerFormWithEnterPolynomialWithEnterUsingAddress(main_com_errComponents->erroredCodeWord);
                                    printf("[ACT] >>>>> *************************************************************** <<<<<\r\n");
                                }
                                #endif


                            //(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))
                            for(main_tmp_sel_decAlgo_i=0; main_tmp_sel_decAlgo_i<processingUseThisAlgorithm->length; main_tmp_sel_decAlgo_i++)
                            {
                                /* calculate all case of test pattern */
                                //main_indi_sel_TP_i[main_tmp_sel_decAlgo_i] = main_com_numsTP;//above switch satement, already do this.

                                for(main_tmp_soft_i=0; main_tmp_soft_i<main_com_numsTP; main_tmp_soft_i++)
                                {
                                            #ifndef RELEASE
                                            if(
                                                (global_flag_case_getBER_method_Display&FLAG_MASK_DISPLAY_PROGRESS)||
                                                (global_flag_debug_display&FLAG_MASK_DISPLAY_PROGRESS)||
                                                (global_flag_bch_BM_SimSD_display&FLAG_MASK_DISPLAY_PROGRESS)||
                                                (global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)
                                            )
                                            {
                                                printf("[ACT] >>>>> *************************************************************** <<<<<\r\n");
                                                infoMes; printf("[ACT] >>> [%s] Decoding of test pattern '%d', t='%d', numsOfErr='%d', %s <<<\r\n",
                                                            KIND_OF_BCH_DECODING_ALGORITHM_NAME[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))],
                                                            main_tmp_soft_i,
                                                            main_indi_hd_correctability[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))],
                                                            main_indi_list_errCntOf_TP_beforeDec[main_tmp_soft_i],
                                                            (main_indi_hd_correctability[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))] < main_indi_list_errCntOf_TP_beforeDec[main_tmp_soft_i])?"[ FAIL ]":"[ TRY ]"
                                                        );
                                                printf("[ACT] >>>>> *************************************************************** <<<<<\r\n");

                                            }
                                            #endif
                                    main_indi_sel_decoding_CW_pwrFormPoly[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))] =
                                        main_com_list_TP_pwrFormPoly[main_tmp_soft_i];


                                            #ifndef RELEASE
                                            if(global_flag_debug_display&FLAG_MASK_DISPLAY_FUNCNAME)
                                            {
                                                debugFuncNameMes;
                                                printf("[ACT] [%s] recreateAlgoriehmComponent [start]\n", KIND_OF_BCH_DECODING_ALGORITHM_NAME[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]);
                                            }
                                            #endif
                                    *main_indi_SD_list_of_decordingComponents[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))][main_tmp_soft_i] =
                                        recreateAlgoriehmComponent(
                                            (*(main_indi_SD_list_of_decordingComponents[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]+main_tmp_soft_i)),
                                            main_com_used_GF->nonSorted, main_com_hd_correctability,
                                            (char*)main_indi_sel_decoding_CW_pwrFormPoly[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]->equation,
                                            main_com_codeLength,
                                            (*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))
                                        );
                                            #ifndef RELEASE
                                            if(global_flag_debug_display&FLAG_MASK_DISPLAY_FUNCNAME)
                                            {
                                                debugFuncNameMes;
                                                printf("[ACT] [%s] recreateAlgoriehmComponent [end]\n", KIND_OF_BCH_DECODING_ALGORITHM_NAME[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]);
                                            }
                                            #endif

                                            #ifndef RELEASE
                                            if(global_flag_debug_display&FLAG_MASK_DISPLAY_FUNCNAME)
                                            {
                                                debugFuncNameMes;
                                                printf("[ACT] [%s] allSyndromeIsZero [start]\n", KIND_OF_BCH_DECODING_ALGORITHM_NAME[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]);
                                            }
                                            #endif
                                    if
                                    (0 < allSyndromeIsZero
                                            (
                                                main_com_used_GF->nonSorted,
                                                (**(main_indi_SD_list_of_decordingComponents[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]+main_tmp_soft_i))->syndrome
                                            )
                                    )
                                    {
                                        main_indi_list_flag_beforeDec_syndZero[(*(((uint8_t*)processingUseThisAlgorithm->list)+main_tmp_sel_decAlgo_i))][main_tmp_soft_i]=1;
                                    }
                                    else
                                    {
                                        main_indi_list_flag_beforeDec_syndZero[(*(((uint8_t*)processingUseThisAlgorithm->list)+main_tmp_sel_decAlgo_i))][main_tmp_soft_i]=0;
                                    }
                                            #ifndef RELEASE
                                            if(global_flag_debug_display&FLAG_MASK_DISPLAY_FUNCNAME)
                                            {
                                                debugFuncNameMes;
                                                printf("[ACT] [%s] allSyndromeIsZero [end]\n", KIND_OF_BCH_DECODING_ALGORITHM_NAME[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]);
                                            }
                                            #endif

                                            #ifndef RELEASE
                                            if(global_flag_debug_display&FLAG_MASK_DISPLAY_FUNCNAME)
                                            {
                                                debugFuncNameMes;
                                                printf("[ACT] [%s] calculateBCH_decodingAlgorithm [start]\n",
                                                    KIND_OF_BCH_DECODING_ALGORITHM_NAME[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]
                                                );
                                            }
                                            #endif
                                    //calculateBmAlgorithm(
                                    //    main_com_used_GF->nonSorted,
                                    //    ((struct_HD_BM_algorithmComponent*)(**(main_indi_SD_list_of_decordingComponents[KIND_OF_BCH_DECODING_BM]+main_tmp_soft_i)))
                                    //);
                                    calculateBCH_decodingAlgorithm(
                                        main_com_used_GF->nonSorted,
                                        (**(main_indi_SD_list_of_decordingComponents[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]+main_tmp_soft_i))
                                    );
                                            #ifndef RELEASE
                                            if(global_flag_debug_display&FLAG_MASK_DISPLAY_FUNCNAME)
                                            {
                                                debugFuncNameMes;
                                                printf("[ACT] [%s] calculateBCH_decodingAlgorithm [end], sel TP #%d -> ADDR : 0x%lx\n",
                                                    KIND_OF_BCH_DECODING_ALGORITHM_NAME[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))],
                                                    main_tmp_soft_i,
                                                    (unsigned long)(**(main_indi_SD_list_of_decordingComponents[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]+main_tmp_soft_i))

                                                );
                                            }
                                            #endif
                                            #ifndef RELEASE
                                            if(
                                                (global_flag_case_getBER_method_Display&FLAG_MASK_DISPLAY_PROGRESS)||
                                                (global_flag_debug_display&FLAG_MASK_DISPLAY_PROGRESS)||
                                                (global_flag_bch_BM_SimSD_display&FLAG_MASK_DISPLAY_PROGRESS)||(
                                                global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)
                                            )
                                            {
                                                infoMes; printf("[ACT] >>> TP(%d), Syndrome of Received vector <<<\r\n", main_tmp_soft_i);
                                                printSyndromeArray(
                                                    (**(main_indi_SD_list_of_decordingComponents[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]+main_tmp_soft_i))->syndrome,
                                                    main_com_used_GF->nonSorted
                                                );

                                                infoMes;    printf("[ACT] >>> TP(%d), Error location polynomial <<<\r\n", main_tmp_soft_i);
                                                            printGaloisField2(
                                                                main_com_used_GF->nonSorted,
                                                                (**(main_indi_SD_list_of_decordingComponents[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]+main_tmp_soft_i))->errLocPoly,
                                                                PRINT_FIELD_EQUATION_TITLE,
                                                                PRINT_FIELD_PRINT_OPTION_NUMBERING
                                                            );
                                                infoMes;    printf("[ACT] degree of TP(%d) err loc poly is '%d'\r\n",
                                                                main_tmp_soft_i,
                                                                (**(main_indi_SD_list_of_decordingComponents[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]+main_tmp_soft_i))->degreeOfErrLocPoly
                                                            );
                                            }
                                            #endif
                                    if(
                                        ((**(main_indi_SD_list_of_decordingComponents[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]+main_tmp_soft_i))->degreeOfErrLocPoly)
                                        <
                                        main_indi_hd_correctability[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]
                                    )
                                    {
                                        main_indi_list_flag_degOfErrLocPolyNotMax[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))][main_tmp_soft_i] = 1;
                                    }
                                    else
                                    {
                                        main_indi_list_flag_degOfErrLocPolyNotMax[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))][main_tmp_soft_i] = 0;
                                    }



                                    //chienSearch_static
                                    //    (
                                    //        main_com_used_GF->nonSorted,
                                    //        (**(main_indi_SD_list_of_decordingComponents[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]+main_tmp_soft_i))->errLocPoly,
                                    //        (**(main_indi_SD_list_of_decordingComponents[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]+main_tmp_soft_i))->errorLocationVector,
                                    //        &((**(main_indi_SD_list_of_decordingComponents[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]+main_tmp_soft_i))->metricCheck)
                                    //    );
                                    chienSearch(
                                        main_com_used_GF->nonSorted,
                                        (**(main_indi_SD_list_of_decordingComponents[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]+main_tmp_soft_i))->errLocPoly,
                                        (**(main_indi_SD_list_of_decordingComponents[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]+main_tmp_soft_i))->errorLocationVector,
                                        &((**(main_indi_SD_list_of_decordingComponents[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]+main_tmp_soft_i))->metricCheck),
                                        (*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))
                                    );
                                    if(
                                        ((**(main_indi_SD_list_of_decordingComponents[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]+main_tmp_soft_i))->degreeOfErrLocPoly) ==
                                        ((**(main_indi_SD_list_of_decordingComponents[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]+main_tmp_soft_i))->metricCheck)
                                    )
                                    {
                                        main_indi_list_flag_metricChk_degErrLocPolySame[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))][main_tmp_soft_i] = 1;
                                    }
                                    else
                                    {
                                        main_indi_list_flag_metricChk_degErrLocPolySame[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))][main_tmp_soft_i] = 0;
                                    }
                                            #ifndef RELEASE
                                            if(
                                                (global_flag_case_getBER_method_Display&FLAG_MASK_DISPLAY_PROGRESS)||
                                                (global_flag_debug_display&FLAG_MASK_DISPLAY_PROGRESS)||
                                                (global_flag_bch_BM_SimSD_display&FLAG_MASK_DISPLAY_PROGRESS)||
                                                (global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)
                                            )
                                            {
                                                infoMes; printf("[ACT] Encoded Vector is below...\r\n");
                                                printPowerFormWithEnterPolynomialWithEnterUsingAddress(main_encodingComponentInGF->codeWord);
                                                infoMes; printf("[ACT] TP(%d), Vector is...\r\n", main_tmp_soft_i);
                                                printPowerFormWithEnterPolynomialWithEnterUsingAddress(main_com_list_TP_pwrFormPoly[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]);
                                                debugSequenceMes;
                                                printf("[ACT] [%s] TP(%d), Error Location Vector is...\r\n", KIND_OF_BCH_DECODING_ALGORITHM_NAME[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))], main_tmp_soft_i);
                                                printPowerFormWithEnterPolynomialWithEnterUsingAddress(
                                                    (**(main_indi_SD_list_of_decordingComponents[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]+main_tmp_soft_i))->errorLocationVector
                                                );
                                                infoMes;    printf("[ACT] [%s] Metric Check value of TP(%d) is '%d'\r\n",
                                                                KIND_OF_BCH_DECODING_ALGORITHM_NAME[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))],
                                                                main_tmp_soft_i,
                                                                (**(main_indi_SD_list_of_decordingComponents[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]+main_tmp_soft_i))->metricCheck
                                                            );
                                                infoMes;    printf("[ACT] [%s]Hamming weight of error loc of TP(%d) is '%d'\r\n",
                                                                KIND_OF_BCH_DECODING_ALGORITHM_NAME[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))],
                                                                main_tmp_soft_i,
                                                                calculateHammingWeightPowerFormPolynomial((**(main_indi_SD_list_of_decordingComponents[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]+main_tmp_soft_i))->errorLocationVector)
                                                            );
                                            }
                                            #endif

                                    /* calculate syndrome of error location (BM algorithme only) start */
                                    //if((*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i)) == KIND_OF_BCH_DECODING_BM)
                                    //{
                                    //    syndromeArrayCalculatorUsingString
                                    //    (
                                    //        main_com_used_GF->nonSorted,
                                    //        ((struct_HD_BM_algorithmComponent*)(**(main_indi_SD_list_of_decordingComponents[KIND_OF_BCH_DECODING_BM]+main_tmp_soft_i)))->errLocSyndrome,
                                    //        (**(main_indi_SD_list_of_decordingComponents[KIND_OF_BCH_DECODING_BM]+main_tmp_soft_i))->vMAX+global_bch_Soft_OverSyndLen,
                                    //        (char*)(**(main_indi_SD_list_of_decordingComponents[KIND_OF_BCH_DECODING_BM]+main_tmp_soft_i))->errorLocationVector->equation
                                    //    );
                                    //    summationSyndromeArray
                                    //    (
                                    //        main_com_used_GF->nonSorted,
                                    //        ((struct_HD_BM_algorithmComponent*)(**(main_indi_SD_list_of_decordingComponents[KIND_OF_BCH_DECODING_BM]+main_tmp_soft_i)))->sumReceiveAndErrLocSyndrome,
                                    //        (**(main_indi_SD_list_of_decordingComponents[KIND_OF_BCH_DECODING_BM]+main_tmp_soft_i))->syndrome,
                                    //        ((struct_HD_BM_algorithmComponent*)(**(main_indi_SD_list_of_decordingComponents[KIND_OF_BCH_DECODING_BM]+main_tmp_soft_i)))->errLocSyndrome
                                    //    );
                                    //    if
                                    //    (0 < allSyndromeIsZero
                                    //            (
                                    //                main_com_used_GF->nonSorted,
                                    //                ((struct_HD_BM_algorithmComponent*)(**(main_indi_SD_list_of_decordingComponents[KIND_OF_BCH_DECODING_BM]+main_tmp_soft_i)))->sumReceiveAndErrLocSyndrome
                                    //            )
                                    //    )
                                    //    {
                                    //        main_indi_list_flag_after_dec_synd_zero[KIND_OF_BCH_DECODING_BM][main_tmp_soft_i]=1;
                                    //    }
                                    //    else
                                    //    {
                                    //        main_indi_list_flag_after_dec_synd_zero[KIND_OF_BCH_DECODING_BM][main_tmp_soft_i]=0;
                                    //    }
                                    //            #ifndef RELEASE
                                    //            if
                                    //            (
                                    //                (global_flag_case_getBER_method_Display&FLAG_MASK_DISPLAY_PROGRESS)||
                                    //                (global_flag_debug_display&FLAG_MASK_DISPLAY_PROGRESS)||
                                    //                (global_flag_bch_BM_SimSD_display&FLAG_MASK_DISPLAY_PROGRESS)||
                                    //                (global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)
                                    //            )
                                    //            {
                                    //                infoMes; printf("[ACT] [%s] AFTER) syndromes are zero? flag is %s, V='%d'\r\n",
                                    //                    KIND_OF_BCH_DECODING_ALGORITHM_NAME[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))],
                                    //                    (main_indi_list_flag_after_dec_synd_zero[KIND_OF_BCH_DECODING_BM][main_tmp_soft_i]?" ON":"OFF"),
                                    //                    main_indi_list_flag_after_dec_synd_zero[KIND_OF_BCH_DECODING_BM][main_tmp_soft_i]
                                    //                );
                                    //                infoMes; printf("[ACT] >>> Syndrome of Received vector <<<\r\n");
                                    //                printSyndromeArray((**(main_indi_SD_list_of_decordingComponents[KIND_OF_BCH_DECODING_BM]+main_tmp_soft_i))->syndrome, main_com_used_GF->nonSorted);
                                    //                infoMes; printf("[ACT] >>> Syndrome of ErrorLocation vector <<<\r\n");
                                    //                printSyndromeArray(((struct_HD_BM_algorithmComponent*)(**(main_indi_SD_list_of_decordingComponents[KIND_OF_BCH_DECODING_BM]+main_tmp_soft_i)))->errLocSyndrome, main_com_used_GF->nonSorted);
                                    //                infoMes; printf("[ACT] >>> sum Syndromes of Received and ErrorLocation vector <<<\r\n");
                                    //                printSyndromeArray(((struct_HD_BM_algorithmComponent*)(**(main_indi_SD_list_of_decordingComponents[KIND_OF_BCH_DECODING_BM]+main_tmp_soft_i)))->sumReceiveAndErrLocSyndrome, main_com_used_GF->nonSorted);
                                    //            }
                                    //            #endif
                                    //}
                                    /* calculate syndrome of error location (BM algorithm only) end */

                                    if(!main_indi_list_flag_beforeDec_syndZero[(*(((uint8_t*)processingUseThisAlgorithm->list)+main_tmp_sel_decAlgo_i))][main_tmp_soft_i])
                                    {
                                        errorCorrection(
                                            (**(main_indi_SD_list_of_decordingComponents[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]+main_tmp_soft_i))->codeWord,
                                            (**(main_indi_SD_list_of_decordingComponents[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]+main_tmp_soft_i))->errorLocationVector
                                        );
                                    }
                                            #ifndef RELEASE
                                            if(
                                                (global_flag_case_getBER_method_Display&FLAG_MASK_DISPLAY_PROGRESS)||
                                                (global_flag_debug_display&FLAG_MASK_DISPLAY_PROGRESS)||
                                                (global_flag_bch_BM_SimSD_display&FLAG_MASK_DISPLAY_PROGRESS)||
                                                (global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)
                                            )
                                            {
                                                    debugSequenceMes;
                                                    printf("[ACT] [%s] TP(%d), Corrected Vector is...\r\n",
                                                    KIND_OF_BCH_DECODING_ALGORITHM_NAME[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))],
                                                        main_tmp_soft_i
                                                    );
                                                    printPowerFormWithEnterPolynomialWithEnterUsingAddress(
                                                        (**(main_indi_SD_list_of_decordingComponents[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]+main_tmp_soft_i))->codeWord
                                                    );
                                            }
                                            #endif

                                    main_indi_list_errCntOf_TP_afterDec[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))][main_tmp_soft_i] =
                                        calculateHammingWeightFromDiffentPowerFormPolynomial(
                                            main_encodingComponentInGF->codeWord,
                                            (**(main_indi_SD_list_of_decordingComponents[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]+main_tmp_soft_i))->codeWord
                                        );
                                            #ifndef RELEASE
                                            if(
                                                (global_flag_case_getBER_method_Display&FLAG_MASK_DISPLAY_PROGRESS)||
                                                (global_flag_debug_display&FLAG_MASK_DISPLAY_PROGRESS)||
                                                (global_flag_bch_BM_SimSD_display&FLAG_MASK_DISPLAY_PROGRESS)||
                                                (global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)
                                            )
                                            {
                                                infoMes; printf("[ACT] TP[%d] has error number is [%d] = %d\r\n", main_tmp_soft_i, main_tmp_soft_i, main_indi_list_errCntOf_TP_beforeDec[main_tmp_soft_i]);
                                                infoMes; printf("[ACT] [%s] TP[%d] is %s, value = %d\r\n",
                                                    KIND_OF_BCH_DECODING_ALGORITHM_NAME[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))],
                                                    main_tmp_soft_i,
                                                    (main_indi_list_flag_theo_ly_can_be_correctable[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))][main_tmp_soft_i]?"CORRECTABLE":"NOT CORRECTABLE"),
                                                    main_indi_list_flag_theo_ly_can_be_correctable[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))][main_tmp_soft_i]
                                                );
                                                infoMes; printf("[ACT] [%s] %s TP[%d] Decording Result : ",
                                                            KIND_OF_BCH_DECODING_ALGORITHM_NAME[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))],
                                                            ((global_list_flag_bch_sim_decodingMode[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]&FLAG_MASK_CODE_BCH_DEC_AL_SOFT_DECISION)?"[Soft-Decision]":"[Hard-Decision](TP #0 ==)"),
                                                            main_tmp_soft_i
                                                        );
                                                if(!main_indi_list_errCntOf_TP_afterDec[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))][main_tmp_soft_i])
                                                {
                                                    printf("[ SUCCESS (e='%d') ]\r\n",
                                                        main_indi_list_errCntOf_TP_afterDec[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))][main_tmp_soft_i]
                                                    );
                                                }
                                                else
                                                {
                                                    printf("[ FAIL (e='%d') ]\r\n",
                                                        main_indi_list_errCntOf_TP_afterDec[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))][main_tmp_soft_i]
                                                    );
                                                }
                                            }
                                            #endif


                                }
                                /*
                                 * Correctable CodeWord Selection
                                 * 1. main_indi_list_flag_beforeDec_syndZero is true?
                                 *     -> received codeword has no err.
                                 * 2. main_indi_list_flag_deccFail is false?
                                 *     -> that is mean, metric chk is same with degree of errPoly.
                                */
                            }




                            /************************************************************/
                            /****** check get BER actual mode fail condition start ******/
                            /************************************************************/
                            //for(main_tmp_sel_decAlgo_i = 0; main_tmp_sel_decAlgo_i < processingUseThisAlgorithm->length; main_tmp_sel_decAlgo_i++)
                            //{
                            //    main_indi_sel_TP_i[main_tmp_sel_decAlgo_i] = main_com_numsTP;
                            //}
                            for(main_tmp_sel_decAlgo_i = 0; main_tmp_sel_decAlgo_i < processingUseThisAlgorithm->length; main_tmp_sel_decAlgo_i++)
                            {
                                set_flag_act_fail_list_TP
                                (
                                    (main_indi_sel_TP_i+(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))),//write
                                    main_indi_list_flag_deccFail[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))],
                                    (main_indi_list_flag_fail_allDec+(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))),

                                    main_indi_list_flag_beforeDec_syndZero[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))],
                                    main_indi_list_flag_degOfErrLocPolyNotMax[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))],
                                    main_indi_list_flag_metricChk_degErrLocPolySame[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))],

                                    main_com_numsTP,
                                    KIND_OF_BCH_DECODING_ALGORITHM_NAME[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]
                                );
                                //printf("test )))))AL[%s]:%d, selected TestPattern : %d =? %d, ADDR : 0x%x =? 0x%x\r\n",
                                //    KIND_OF_BCH_DECODING_ALGORITHM_NAME[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))],
                                //    (*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i)),
                                //    *(main_indi_sel_TP_i+(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))),
                                //    main_indi_sel_TP_i[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))],

                                //    (main_indi_sel_TP_i+(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))),
                                //    &main_indi_sel_TP_i[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]
                                //    );
                            }

                                    #ifndef RELEASE
                                    if(
                                        (global_flag_case_getBER_method_Display&FLAG_MASK_DISPLAY_PROGRESS)||
                                        (global_flag_debug_display&FLAG_MASK_DISPLAY_PROGRESS)||
                                        (global_flag_bch_BM_SimSD_display&FLAG_MASK_DISPLAY_PROGRESS)||
                                        (global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)||
                                        (global_flag_debug_display&FLAG_MASK_DISPLAY_FUNCNAME)
                                    )
                                    {
                                        printf(">>>>> *************************************************************** <<<<<\r\n");
                                        printf(">>>>> ******************* Summary of correctables ******************* <<<<<\r\n");
                                        printf(">>>>> *************************************************************** <<<<<\r\n");
                                    }
                                    #endif

                                    #ifndef RELEASE
                                    if(
                                        (global_flag_case_getBER_method_Display&FLAG_MASK_DISPLAY_PROGRESS)||
                                        (global_flag_debug_display&FLAG_MASK_DISPLAY_PROGRESS)||
                                        (global_flag_bch_BM_SimSD_display&FLAG_MASK_DISPLAY_PROGRESS)||
                                        (global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)||
                                        (global_flag_debug_display&FLAG_MASK_DISPLAY_FUNCNAME)
                                    )
                                    {
                                        printf(">>>>> *************************************************************** <<<<<\r\n");
                                    }
                                    #endif
                            /************************************************************/
                            /******* check get BER actual mode fail condition end *******/
                            /************************************************************/








                            /************************************************************/
                            /****** check get BER actual mode fail condition start ******/
                            /************************************************************/
                                    #ifndef RELEASE
                                    if((global_flag_case_getBER_method_Display&FLAG_MASK_DISPLAY_PROGRESS)||(global_flag_debug_display&FLAG_MASK_DISPLAY_PROGRESS)||(global_flag_bch_BM_SimSD_display&FLAG_MASK_DISPLAY_PROGRESS)||(global_flag_cmdOption&FLAG_MASK_PRINTF_LOG))
                                    {
                                        printf(">>>>> *************************************************************** <<<<<\r\n");

                                        //(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))
                                        for(main_tmp_sel_decAlgo_i=0; main_tmp_sel_decAlgo_i<processingUseThisAlgorithm->length; main_tmp_sel_decAlgo_i++)
                                        {
                                            infoMes; printf("[ACT] [%s] algorithm select #%d-th test-pattern\r\n" ,
                                                KIND_OF_BCH_DECODING_ALGORITHM_NAME[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))],
                                                main_indi_sel_TP_i[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]
                                            );

                                            for(main_tmp_soft_i=0; main_tmp_soft_i<main_com_numsTP; main_tmp_soft_i++)
                                            {
                                                infoMes;    printf("[ACT] [%s](%c)TP(%d) ",
                                                                KIND_OF_BCH_DECODING_ALGORITHM_NAME[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))],
                                                                ((main_indi_sel_TP_i[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]==main_tmp_soft_i)?'v':' '),
                                                                main_tmp_soft_i
                                                            );
                                                            printf("flag theo(%s, v='%d'), ",
                                                                (main_indi_list_flag_theo_ly_can_be_correctable[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))][main_tmp_soft_i]?" ON":"OFF"),
                                                                main_indi_list_flag_theo_ly_can_be_correctable[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))][main_tmp_soft_i]
                                                            );
                                                            printf("actFail(%s, v='%d'), ",
                                                                (main_indi_list_flag_deccFail[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))][main_tmp_soft_i]?" ON":"OFF"),
                                                                main_indi_list_flag_deccFail[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))][main_tmp_soft_i]
                                                            );

                                                            printf("0-synd(Bf)%s, v='%d'), (Af)(%s, v='%d'), ",
                                                                (main_indi_list_flag_beforeDec_syndZero[(*(((uint8_t*)processingUseThisAlgorithm->list)+main_tmp_sel_decAlgo_i))][main_tmp_soft_i]?" ON":"OFF"),
                                                                main_indi_list_flag_beforeDec_syndZero[(*(((uint8_t*)processingUseThisAlgorithm->list)+main_tmp_sel_decAlgo_i))][main_tmp_soft_i],

                                                                (main_indi_list_flag_after_dec_synd_zero[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))][main_tmp_soft_i]?" ON":"OFF"),
                                                                main_indi_list_flag_after_dec_synd_zero[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))][main_tmp_soft_i]
                                                            );
                                                            printf("E.P.N max(%s, v='%d'), ",
                                                                (main_indi_list_flag_degOfErrLocPolyNotMax[(*(((uint8_t*)processingUseThisAlgorithm->list)+main_tmp_sel_decAlgo_i))][main_tmp_soft_i]?" ON":"OFF"),
                                                                main_indi_list_flag_degOfErrLocPolyNotMax[(*(((uint8_t*)processingUseThisAlgorithm->list)+main_tmp_sel_decAlgo_i))][main_tmp_soft_i]
                                                            );

                                                            printf("t='%d', errCnt(BF='%d', AF='%d'), ",
                                                                main_indi_hd_correctability[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))],
                                                                main_indi_list_errCntOf_TP_beforeDec[main_tmp_soft_i],
                                                                main_indi_list_errCntOf_TP_afterDec[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))][main_tmp_soft_i]
                                                            );

                                                            printf("degE.Loc.P='%d', met.Chk='%d' = same='%d'\r\n",
                                                                (**(main_indi_SD_list_of_decordingComponents[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]+main_tmp_soft_i))->degreeOfErrLocPoly,
                                                                (**(main_indi_SD_list_of_decordingComponents[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]+main_tmp_soft_i))->metricCheck,
                                                                main_indi_list_flag_metricChk_degErrLocPolySame[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))][main_tmp_soft_i]
                                                            );
                                            }
                                            infoMes; printf("[ACT] [%s] decoding All fail flag is %s(=%d)\r\n",
                                                KIND_OF_BCH_DECODING_ALGORITHM_NAME[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))],
                                                (main_indi_list_flag_fail_allDec[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]?"ON":"OFF"),
                                                main_indi_list_flag_fail_allDec[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]
                                            );
                                            infoMes; printf("[ACT [%s] algorithm select TP[%d] and TP[%d] distance with encoded codeword = %d\r\n",
                                                KIND_OF_BCH_DECODING_ALGORITHM_NAME[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))],
                                                main_indi_sel_TP_i[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))],
                                                main_indi_sel_TP_i[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))],
                                                main_indi_list_errCntOf_TP_afterDec[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))][main_indi_sel_TP_i[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]]
                                            );

                                        }

                                        for(main_tmp_sel_decAlgo_i=0; main_tmp_sel_decAlgo_i<processingUseThisAlgorithm->length; main_tmp_sel_decAlgo_i++)
                                        {
                                            for(cmd_tmp_i=0; cmd_tmp_i<processingUseThisAlgorithm->length; cmd_tmp_i++)
                                            {
                                                if(
                                                    main_indi_sel_TP_i[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]
                                                    !=
                                                    main_indi_sel_TP_i[(*(((uint8_t*)(processingUseThisAlgorithm->list))+cmd_tmp_i))]
                                                )
                                                {
                                                    warningMesShort; printf("[%s] has sel tp = '%d', [%s] has sel tp = '%d'\r\n",
                                                        KIND_OF_BCH_DECODING_ALGORITHM_NAME[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))],
                                                        main_indi_sel_TP_i[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))],
                                                        KIND_OF_BCH_DECODING_ALGORITHM_NAME[(*(((uint8_t*)(processingUseThisAlgorithm->list))+cmd_tmp_i))],
                                                        main_indi_sel_TP_i[(*(((uint8_t*)(processingUseThisAlgorithm->list))+cmd_tmp_i))]
                                                    );
                                                    //if(!(global_flag_cmdSystemUnhold&FLAG_MASK_SYSTEM_UNHOLD_FORCED))
                                                    //{
                                                        systemHoldMes;    printf("input : ");
                                                        scanf("%s", global_buf_KeyBoardInput);
                                                    //}
                                                }
                                            }

                                        }
                                                if(!(global_flag_cmdSystemUnhold&FLAG_MASK_SYSTEM_UNHOLD_FORCED))
                                                {
                                                    systemHoldMes;    printf("input : ");
                                                    scanf("%s", global_buf_KeyBoardInput);
                                                }

                                        infoMes; printf("[ACT] Minumum LLR location and Values\r\n");
                                        testPrintShortMinimumQuantizedLLRAndLocator(main_com_bpskComponents->bpskReceivedLLR);
                                        printf("squreRootBpskReceivedDataAddedAwgn = '%g'\r\n", main_com_bpskComponents->squreRootBpskReceivedDataAddedAwgn);
                                        printf("squreRootAvrLLR = '%g'\r\n", main_com_bpskComponents->bpskReceivedLLR->squreRootAvrLLR);

                                        printf("[ACT] >>>>> *************************************************************** <<<<<\r\n");
                                        infoMes; printf("[ACT] >>>>>/***** selection selected test pattern infomation. *****/<<<<<\r\n");
                                        //(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))
                                        for(main_tmp_sel_decAlgo_i=0; main_tmp_sel_decAlgo_i<processingUseThisAlgorithm->length; main_tmp_sel_decAlgo_i++)
                                        {
                                            infoMes;    printf("[ACT] [%s] Selected test pattern is '#%d', TP(%d)\r\n",
                                                            KIND_OF_BCH_DECODING_ALGORITHM_NAME[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))],
                                                            main_indi_sel_TP_i[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))],
                                                            main_indi_sel_TP_i[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]
                                                        );
                                        }
                                        printf("\r\n");

                                        infoMes; printf("[ACT] Encoded Vector is below...\r\n");
                                        printPowerFormWithEnterPolynomialWithEnterUsingAddress(main_encodingComponentInGF->codeWord);
                                        infoMes; printf("[ACT] Received Vector is below...\r\n");
                                        printPowerFormWithEnterPolynomialWithEnterUsingAddress(main_com_errComponents->erroredCodeWord);
                                        printf("\r\n");

                                        //(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))
                                        for(main_tmp_sel_decAlgo_i=0; main_tmp_sel_decAlgo_i<processingUseThisAlgorithm->length; main_tmp_sel_decAlgo_i++)
                                        {
                                            infoMes; printf("[ACT] [%s] >>> TP(%d), Error location polynomial <<<\r\n",
                                                        KIND_OF_BCH_DECODING_ALGORITHM_NAME[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))],
                                                        main_indi_sel_TP_i[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]
                                            );
                                                            printf("ADDR : 0x%lx\r\n", (unsigned long)((**(main_indi_SD_list_of_decordingComponents[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]+main_indi_sel_TP_i[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]))->errLocPoly));

                                            printGaloisField2(
                                                main_com_used_GF->nonSorted,
                                                (**(main_indi_SD_list_of_decordingComponents[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]+main_indi_sel_TP_i[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]))->errLocPoly, 
                                                PRINT_FIELD_EQUATION_TITLE,
                                                PRINT_FIELD_PRINT_OPTION_NUMBERING
                                            );
                                            infoMes; printf("[ACT] [%s] degree of TP(%d) err loc poly is '%d'\r\n",
                                                    KIND_OF_BCH_DECODING_ALGORITHM_NAME[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))],
                                                    main_indi_sel_TP_i[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))],
                                                    (**(main_indi_SD_list_of_decordingComponents[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]+main_indi_sel_TP_i[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]))->degreeOfErrLocPoly
                                            );
                                        }
                                        printf("\r\n");


                                        //(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))
                                        for(main_tmp_sel_decAlgo_i=0; main_tmp_sel_decAlgo_i<processingUseThisAlgorithm->length; main_tmp_sel_decAlgo_i++)
                                        {
                                            printf("[ACT] [%s] TP(%d), Error Location Vector is...\r\n",
                                                KIND_OF_BCH_DECODING_ALGORITHM_NAME[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))],
                                                main_indi_sel_TP_i[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]
                                            );
                                            printPowerFormWithEnterPolynomialWithEnterUsingAddress(
                                                (**(main_indi_SD_list_of_decordingComponents[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]+main_indi_sel_TP_i[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]))->errorLocationVector
                                            );
                                            infoMes; printf("[ACT] [%s] Metric Check value of TP(%d) is '%d'\r\n",
                                                    KIND_OF_BCH_DECODING_ALGORITHM_NAME[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))],
                                                    main_indi_sel_TP_i[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))],
                                                    (**(main_indi_SD_list_of_decordingComponents[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]+main_indi_sel_TP_i[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]))->metricCheck
                                            );
                                        }
                                        printf("\r\n");

                                        //(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))
                                        for(main_tmp_sel_decAlgo_i=0; main_tmp_sel_decAlgo_i<processingUseThisAlgorithm->length; main_tmp_sel_decAlgo_i++)
                                        {
                                            infoMes;    printf("[ACT] [%s] Selected TP '#%d', err distance before decoding of TP[%d] = %d\r\n",
                                                KIND_OF_BCH_DECODING_ALGORITHM_NAME[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))],
                                                main_indi_sel_TP_i[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))],
                                                main_indi_sel_TP_i[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))],
                                                main_indi_list_errCntOf_TP_beforeDec[main_indi_sel_TP_i[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]]
                                            );
                                            infoMes;    printf("[ACT] [%s] %s TP #%d, err distance after decording of TP[%d] = %d\r\n",
                                                KIND_OF_BCH_DECODING_ALGORITHM_NAME[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))],
                                                ((global_list_flag_bch_sim_decodingMode[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]&FLAG_MASK_CODE_BCH_DEC_AL_SOFT_DECISION)?"[Soft-Decision]":"[Hard-Decision](TP)"),
                                                main_indi_sel_TP_i[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))],
                                                main_indi_sel_TP_i[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))],
                                                main_indi_list_errCntOf_TP_afterDec[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))][main_indi_sel_TP_i[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]]
                                            );
                                        }
                                        printf("\r\n");

                                        //(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))
                                        for(main_tmp_sel_decAlgo_i=0; main_tmp_sel_decAlgo_i<processingUseThisAlgorithm->length; main_tmp_sel_decAlgo_i++)
                                        {
                                            infoMes;    printf("[ACT] >>> Syndrome of Received vector <<<\r\n");
                                            printSyndromeArray(
                                                (**(main_indi_SD_list_of_decordingComponents[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]+main_indi_sel_TP_i[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]))->syndrome,
                                                main_com_used_GF->nonSorted
                                            );

                                            /* errLocSyndrome is only valud in BM algorithm start */
                                            //infoMes;    printf("[ACT] >>> Syndrome of ErrorLocation vector <<<\r\n");
                                            //printSyndromeArray(
                                            //    ((struct_HD_BM_algorithmComponent*)(**(main_indi_SD_list_of_decordingComponents[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]+main_indi_sel_TP_i[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))])))->errLocSyndrome,
                                            //    main_com_used_GF->nonSorted
                                            //);
                                            /* errLocSyndrome is only valud in BM algorithm end */

                                            /* sum syndromes of receive and err loc is only valid in BM algorithm start */
                                            //if((*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i)) == KIND_OF_BCH_DECODING_BM)
                                            //{
                                            //    infoMes;    printf("[ACT] >>> sum Syndromes of Received and ErrorLocation vector <<<\r\n");
                                            //    printSyndromeArray(
                                            //    ((struct_HD_BM_algorithmComponent*)(**(main_indi_SD_list_of_decordingComponents[KIND_OF_BCH_DECODING_BM]+main_indi_sel_TP_i[KIND_OF_BCH_DECODING_BM])))->sumReceiveAndErrLocSyndrome, main_com_used_GF->nonSorted);

                                            //    infoMes;    printf("[ACT] [%s] AFTER) syndromes are zero? flag is %s, V='%d'\r\n",
                                            //        KIND_OF_BCH_DECODING_ALGORITHM_NAME[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))],
                                            //        (main_indi_list_flag_after_dec_synd_zero[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))][main_indi_sel_TP_i[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]]?" ON":"OFF"),
                                            //        main_indi_list_flag_after_dec_synd_zero[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))][main_indi_sel_TP_i[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]]
                                            //    );
                                            //}
                                            /* sum syndromes of receive and err loc is only valid in BM algorithm end */
                                        }
                                        printf("\r\n");

                                        //(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))
                                        for(main_tmp_sel_decAlgo_i=0; main_tmp_sel_decAlgo_i<processingUseThisAlgorithm->length; main_tmp_sel_decAlgo_i++)
                                        {
                                            if(!main_indi_list_errCntOf_TP_afterDec[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))][main_indi_sel_TP_i[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]])
                                            {
                                                printf("[%s] [ SUCCESS (e='%d') ]\r\n",
                                                    KIND_OF_BCH_DECODING_ALGORITHM_NAME[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))],
                                                    main_indi_list_errCntOf_TP_afterDec[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))][main_indi_sel_TP_i[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]]);
                                            }
                                            else
                                            {
                                                printf("[%s] [ FAIL (e='%d') ]\r\n",
                                                KIND_OF_BCH_DECODING_ALGORITHM_NAME[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))],
                                                main_indi_list_errCntOf_TP_afterDec[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))][main_indi_sel_TP_i[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]]);
                                            }
                                            infoMes; printf("[ACT] [%s] Theoretically being correctable flag of TP(%d) is %s, value = %d\r\n",
                                                KIND_OF_BCH_DECODING_ALGORITHM_NAME[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))],
                                                main_indi_sel_TP_i[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))],
                                                (main_indi_list_flag_theo_ly_can_be_correctable[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))][main_indi_sel_TP_i[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]]?" ON":"OFF"),
                                                main_indi_list_flag_theo_ly_can_be_correctable[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))][main_indi_sel_TP_i[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]] );
                                            infoMes; printf("[ACT] [%s] Actually being not correctable flag of TP(%d) is %s, value = %d\r\n",
                                                KIND_OF_BCH_DECODING_ALGORITHM_NAME[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))],
                                                main_indi_sel_TP_i[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))],
                                                (main_indi_list_flag_deccFail[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))][main_indi_sel_TP_i[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]]?" ON":"OFF"),
                                                main_indi_list_flag_deccFail[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))][main_indi_sel_TP_i[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]] );
                                        }
                                        printf("\r\n");

                                    }
                                    #endif


                            //(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))
                            for(main_tmp_sel_decAlgo_i=0; main_tmp_sel_decAlgo_i<processingUseThisAlgorithm->length; main_tmp_sel_decAlgo_i++)
                            {
                                if(main_indi_list_flag_theo_ly_can_be_correctable[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))][main_indi_sel_TP_i[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]])//theoretically check can try correction?
                                {//theoretically check can try correction? -> YES
                                    if(!main_indi_list_flag_deccFail[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))][main_indi_sel_TP_i[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]])
                                    {//actually check can try correction? -> YES
                                        /*
                                         * Corrected bits number is maximum(t=e+-p), choosing algorithm that correctable test-pattern can have some discrepancy.
                                         * So check this, degreeOfErrLocPoly and main_com_hd_correctability are same.
                                         */
                                        if( (**(main_indi_SD_list_of_decordingComponents[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]+main_indi_sel_TP_i[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]))->degreeOfErrLocPoly == main_indi_hd_correctability[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))] )
                                        {//check to discrepancy of select condition of correctable test pattern.
                                            /*
                                             * chk from test pattern 0 to 2^p - 1 and start to 0.
                                             * Because priority is higher than low numbered test pattern.
                                             */
                                            for(main_tmp_soft_i=0; main_tmp_soft_i<main_com_numsTP; main_tmp_soft_i++)
                                            {
                                                if(main_indi_sel_TP_i[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))] == main_tmp_soft_i) continue;//except selected test pattern.
                                                if(main_indi_list_flag_theo_ly_can_be_correctable[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))][main_tmp_soft_i])
                                                {//check candidate testpattern is theoretically correctable? -> YES
                                                    /*
                                                     * check that non selected test pattern is correctable?
                                                     */
                                                    if( (**(main_indi_SD_list_of_decordingComponents[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]+main_tmp_soft_i))->degreeOfErrLocPoly == (**(main_indi_SD_list_of_decordingComponents[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]+main_tmp_soft_i))->metricCheck )
                                                    {//check that non selected test pattern is correctable? -> YES
                                                        if( (**(main_indi_SD_list_of_decordingComponents[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]+main_tmp_soft_i))->degreeOfErrLocPoly == (**(main_indi_SD_list_of_decordingComponents[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]+main_indi_sel_TP_i[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]))->degreeOfErrLocPoly )
                                                        {//Same condition with selected test-pattern? -> YES
                                                                    #ifndef RELEASE
                                                                    if(
                                                                        (global_flag_case_getBER_method_Display&FLAG_MASK_DISPLAY_PROGRESS)||
                                                                        (global_flag_debug_display&FLAG_MASK_DISPLAY_PROGRESS)||
                                                                        (global_flag_bch_BM_SimSD_display&FLAG_MASK_DISPLAY_PROGRESS)||
                                                                        (global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)
                                                                    )
                                                                    {
                                                                        printf("[ACT] >>>>> *************************************************************** <<<<<\r\n");
                                                                        warningMes; printf("[ACT] [%s] !!!! test pattern selecting is success, just for chk condition and right case.\r\n", 
                                                                            KIND_OF_BCH_DECODING_ALGORITHM_NAME[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]
                                                                        );

                                                                        if(!(global_flag_cmdSystemUnhold&FLAG_MASK_SYSTEM_UNHOLD_FORCED))
                                                                        {
                                                                            systemHoldMes;    printf("input : ");
                                                                            scanf("%s", global_buf_KeyBoardInput);
                                                                        }
                                                                    }
                                                                    #endif
                                                        }
                                                        else
                                                        {//Same condition with selected test-pattern? -> NO
                                                            //can be passing
                                                            //DO NOTHING...
                                                        }
                                                    }
                                                    else
                                                    {//check that non selected test pattern is correctable? -> NO
                                                        //DO NOTHING...
                                                    }
                                                }
                                                else
                                                {//check candidate testpattern is theoretically correctable? -> NO
                                                    //DO NOTHING...
                                                }
                                            }

                                            if(main_tmp_soft_i==main_com_numsTP)
                                            {
                                                        #ifndef RELEASE
                                                        if(
                                                            (global_flag_case_getBER_method_Display&FLAG_MASK_DISPLAY_PROGRESS)||
                                                            (global_flag_debug_display&FLAG_MASK_DISPLAY_PROGRESS)||
                                                            (global_flag_bch_BM_SimSD_display&FLAG_MASK_DISPLAY_PROGRESS)||
                                                            (global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)
                                                        )
                                                        {
                                                            printf("[ACT] >>>>> *************************************************************** <<<<<\r\n");
                                                            infoMes; printf("[ACT] [%s] Theoretically can falg is %s, actual fail flag is %s.\r\n",
                                                                KIND_OF_BCH_DECODING_ALGORITHM_NAME[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))],
                                                                (main_indi_list_flag_theo_ly_can_be_correctable[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))][main_indi_sel_TP_i[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]]?" ON":"OFF"),
                                                                (main_indi_list_flag_deccFail[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))][main_indi_sel_TP_i[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]]?" ON":"OFF")
                                                            );
                                                            infoMes; printf("[ACT] [%s] Theoretically another correctable test pattern is no exist....\r\n",
                                                                KIND_OF_BCH_DECODING_ALGORITHM_NAME[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]
                                                            );
                                                            infoMes; printf("[ACT] [%s] In other hands crrectable test pattern is only selected test pattern.\r\n",
                                                                KIND_OF_BCH_DECODING_ALGORITHM_NAME[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]
                                                            );

                                                                    //if(!(global_flag_cmdSystemUnhold&FLAG_MASK_SYSTEM_UNHOLD_FORCED))
                                                                    //{
                                                                    //    systemHoldMes;    printf("input : ");
                                                                    //    scanf("%s", global_buf_KeyBoardInput);
                                                                    //}
                                                        }
                                                        #endif
                                            }
                                            else
                                            {
                                                        #ifndef RELEASE
                                                        if(
                                                            (global_flag_case_getBER_method_Display&FLAG_MASK_DISPLAY_PROGRESS)||
                                                            (global_flag_debug_display&FLAG_MASK_DISPLAY_PROGRESS)||
                                                            (global_flag_bch_BM_SimSD_display&FLAG_MASK_DISPLAY_PROGRESS)||
                                                            (global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)
                                                        )
                                                        {
                                                            printf("[ACT] >>>>> *************************************************************** <<<<<\r\n");
                                                            infoMes; printf("[ACT] [%s] Theoretically can falg is %s, actual fail flag is %s.\r\n",
                                                                KIND_OF_BCH_DECODING_ALGORITHM_NAME[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))],
                                                                (main_indi_list_flag_theo_ly_can_be_correctable[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))][main_indi_sel_TP_i[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]]?" ON":"OFF"),
                                                                (main_indi_list_flag_deccFail[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))][main_indi_sel_TP_i[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]]?" ON":"OFF") );
                                                            warningMes; printf("[ACT] [%s] Theoretically correctable test pattern is exist. Choosen TP(%d), Co-able TP(%d)\r\n",
                                                                KIND_OF_BCH_DECODING_ALGORITHM_NAME[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))],
                                                                main_indi_sel_TP_i[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))], main_tmp_soft_i
                                                            );

                                                            if(!(global_flag_cmdSystemUnhold&FLAG_MASK_SYSTEM_UNHOLD_FORCED))
                                                            {
                                                                debugMes; printf("[ACT] [%s] To test, find a reason why theoretically correctable test pattern is exist.\r\n",
                                                                    KIND_OF_BCH_DECODING_ALGORITHM_NAME[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]
                                                                );
                                                                systemHoldMes;    printf("input : ");
                                                                scanf("%s", global_buf_KeyBoardInput);
                                                            }
                                                        }
                                                        #endif
                                            }
                                        }
                                        else
                                        {
                                                    #ifndef RELEASE
                                                    if(
                                                        (global_flag_case_getBER_method_Display&FLAG_MASK_DISPLAY_PROGRESS)||
                                                        (global_flag_debug_display&FLAG_MASK_DISPLAY_PROGRESS)||
                                                        (global_flag_bch_BM_SimSD_display&FLAG_MASK_DISPLAY_PROGRESS)||
                                                        (global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)
                                                    )
                                                    {
                                                        printf("[ACT] >>>>> *************************************************************** <<<<<\r\n");
                                                        infoMes; printf("[ACT] [%s] Theoretically can falg is %s, actual fail flag is %s.\r\n",
                                                            KIND_OF_BCH_DECODING_ALGORITHM_NAME[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))],
                                                            (main_indi_list_flag_theo_ly_can_be_correctable[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))][main_indi_sel_TP_i[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]]?" ON":"OFF"),
                                                            (main_indi_list_flag_deccFail[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))][main_indi_sel_TP_i[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]]?" ON":"OFF") );
                                                        infoMes; printf("[ACT] [%s] Choosen TP(%d) has less degee(deg is '%d') of poly than t='%d'.\r\n",
                                                            KIND_OF_BCH_DECODING_ALGORITHM_NAME[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))],
                                                            main_indi_sel_TP_i[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))],
                                                            (**(main_indi_SD_list_of_decordingComponents[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]+main_indi_sel_TP_i[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]))->degreeOfErrLocPoly, main_indi_hd_correctability[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]);
                                                    }
                                                    #endif
                                        }
                                    }
                                    else
                                    {//actually check can try correction? -> NO
                                                #ifndef RELEASE
                                                if(
                                                    (global_flag_case_getBER_method_Display&FLAG_MASK_DISPLAY_PROGRESS)||
                                                    (global_flag_debug_display&FLAG_MASK_DISPLAY_PROGRESS)||
                                                    (global_flag_bch_BM_SimSD_display&FLAG_MASK_DISPLAY_PROGRESS)||
                                                    (global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)
                                                )
                                                {
                                                    printf("[ACT] >>>>> *************************************************************** <<<<<\r\n");
                                                    infoMes; printf("[ACT] [%s] Theoretically can falg is %s, actual fail flag is %s.\r\n",
                                                        KIND_OF_BCH_DECODING_ALGORITHM_NAME[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))],
                                                        (main_indi_list_flag_theo_ly_can_be_correctable[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))][main_indi_sel_TP_i[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]]?" ON":"OFF"),
                                                        (main_indi_list_flag_deccFail[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))][main_indi_sel_TP_i[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]]?" ON":"OFF") );
                                                    warningMes; printf("[ACT] [%s] Theoretically can correction but actually can not.\r\n",
                                                        KIND_OF_BCH_DECODING_ALGORITHM_NAME[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]
                                                    );

                                                    if(!(global_flag_cmdSystemUnhold&FLAG_MASK_SYSTEM_UNHOLD_FORCED))
                                                    {
                                                        debugMes; printf("[ACT] [%s] To test, find a reason why theoretically can correction but actually can not\r\n",
                                                            KIND_OF_BCH_DECODING_ALGORITHM_NAME[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]
                                                        );
                                                        systemHoldMes;    printf("input : ");
                                                        scanf("%s", global_buf_KeyBoardInput);
                                                    }
                                                }
                                                #endif
                                    }
                                }
                                else
                                {//theoretically check can try correction? -> NO
                                    if(main_indi_list_flag_deccFail[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))][main_indi_sel_TP_i[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]])
                                    {//actually check can try correction? -> NO
                                        /*
                                         * if exist correctable test pattern?
                                         * from test pattern 0 to 2^p-1.
                                         */
                                        for(main_tmp_soft_i=0; main_tmp_soft_i<main_com_numsTP; main_tmp_soft_i++)
                                        {
                                        /*
                                         * theoretically correctable testpattern checker. if being correctable testpattern exist, break out for statements.
                                         * if not exist it, main_tmp_soft_i is set to main_com_numsTP.
                                         */
                                            if(main_indi_list_flag_theo_ly_can_be_correctable[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))][main_tmp_soft_i]) break;
                                        }

                                        if(main_tmp_soft_i==main_com_numsTP)
                                        {
                                                    #ifndef RELEASE
                                                    if(
                                                        (global_flag_case_getBER_method_Display&FLAG_MASK_DISPLAY_PROGRESS)||
                                                        (global_flag_debug_display&FLAG_MASK_DISPLAY_PROGRESS)||
                                                        (global_flag_bch_BM_SimSD_display&FLAG_MASK_DISPLAY_PROGRESS)||
                                                        (global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)
                                                    )
                                                    {
                                                        printf("[ACT] >>>>> *************************************************************** <<<<<\r\n");
                                                        infoMes; printf("[ACT] [%s] Theoretically can falg is %s, actual fail flag is %s.\r\n",
                                                            KIND_OF_BCH_DECODING_ALGORITHM_NAME[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))],
                                                            (main_indi_list_flag_theo_ly_can_be_correctable[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))][main_indi_sel_TP_i[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]]?" ON":"OFF"),
                                                            (main_indi_list_flag_deccFail[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))][main_indi_sel_TP_i[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]]?" ON":"OFF") );
                                                        infoMes; printf("[ACT] [%s] No test pattern to try correct.\r\n",
                                                            KIND_OF_BCH_DECODING_ALGORITHM_NAME[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]
                                                        );

                                                        infoMes; printf("[ACT] [%s] %s test pattern can %s try correction. All TP fail to dec-able flag is %s val = '%d'\r\n",
                                                            KIND_OF_BCH_DECODING_ALGORITHM_NAME[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))],
                                                            (main_indi_list_flag_fail_allDec[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]?"Any":"Some"),
                                                            (main_indi_list_flag_fail_allDec[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]?"not":""),
                                                            (main_indi_list_flag_fail_allDec[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]?" ON":"OFF"),
                                                            main_indi_list_flag_fail_allDec[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]
                                                         );
                                                    }
                                                    #endif
                                        }
                                        else
                                        {
                                        /*
                                         * theoretical and actual correctable test-patterns are exist.
                                         */
                                                    #ifndef RELEASE
                                                    if(
                                                        (global_flag_case_getBER_method_Display&FLAG_MASK_DISPLAY_PROGRESS)||
                                                        (global_flag_debug_display&FLAG_MASK_DISPLAY_PROGRESS)||
                                                        (global_flag_bch_BM_SimSD_display&FLAG_MASK_DISPLAY_PROGRESS)||
                                                        (global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)
                                                    )
                                                    {
                                                        printf("[ACT] >>>>> *************************************************************** <<<<<\r\n");
                                                        infoMes; printf("[ACT] [%s] Theoretically can falg is %s, actual fail flag is %s.\r\n",
                                                            KIND_OF_BCH_DECODING_ALGORITHM_NAME[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))],
                                                            (main_indi_list_flag_theo_ly_can_be_correctable[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))][main_indi_sel_TP_i[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]]?" ON":"OFF"),
                                                            (main_indi_list_flag_deccFail[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))][main_indi_sel_TP_i[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]]?" ON":"OFF") );
                                                        warningMes; printf("[ACT] [%s] !!!! test pattern selecting is failure. Another correctable test pattern is exist.\r\n",
                                                            KIND_OF_BCH_DECODING_ALGORITHM_NAME[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]
                                                        );
                                                        warningMesShort; printf("[ACT] [%s] !!!! Choosen TP(%d), co-able TP(%d)\r\n",
                                                            KIND_OF_BCH_DECODING_ALGORITHM_NAME[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))],
                                                            main_indi_sel_TP_i[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))],
                                                            main_tmp_soft_i
                                                        );

                                                        if(!(global_flag_cmdSystemUnhold&FLAG_MASK_SYSTEM_UNHOLD_FORCED))
                                                        {
                                                            debugMes; printf("[ACT] [%s] To test, find a reason why selecting is fail.\r\n",
                                                                KIND_OF_BCH_DECODING_ALGORITHM_NAME[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]
                                                            );
                                                            systemHoldMes;    printf("input : ");
                                                            scanf("%s", global_buf_KeyBoardInput);
                                                        }
                                                    }
                                                    #endif
                                        }
                                    }
                                    else
                                    {//actually check can try correction? -> YES
                                                #ifndef RELEASE
                                                if(
                                                    (global_flag_case_getBER_method_Display&FLAG_MASK_DISPLAY_PROGRESS)||
                                                    (global_flag_debug_display&FLAG_MASK_DISPLAY_PROGRESS)||
                                                    (global_flag_bch_BM_SimSD_display&FLAG_MASK_DISPLAY_PROGRESS)||
                                                    (global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)
                                                )
                                                {
                                                    printf("[ACT] >>>>> *************************************************************** <<<<<\r\n");
                                                    warningMes; printf("[ACT] [%s] Theoretically, TP(%d) is flag is %s val='%d'. fail act-decoding flag is %s, val='%d'.\r\n",
                                                        KIND_OF_BCH_DECODING_ALGORITHM_NAME[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))],
                                                        main_indi_sel_TP_i[main_tmp_sel_decAlgo_i], 
                                                        (main_indi_list_flag_theo_ly_can_be_correctable[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))][main_indi_sel_TP_i[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]]?" ON":"OFF"),
                                                        main_indi_list_flag_theo_ly_can_be_correctable[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))][main_indi_sel_TP_i[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]],
                                                        (main_indi_list_flag_deccFail[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))][main_indi_sel_TP_i[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]]?" ON":"OFF"),
                                                        main_indi_list_flag_deccFail[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))][main_indi_sel_TP_i[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]] );

                                                    warningMesShort; printf("[ACT] [%s] Theoretically can not try correction but, acutally can.\r\n",
                                                        KIND_OF_BCH_DECODING_ALGORITHM_NAME[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]
                                                    );
                                                            if(!(global_flag_cmdSystemUnhold&FLAG_MASK_SYSTEM_UNHOLD_FORCED))
                                                            {
                                                                debugMes; printf("[ACT] [%s] To test, find a reason why theoretically can not try correction but, acutally can.\r\n",
                                                                    KIND_OF_BCH_DECODING_ALGORITHM_NAME[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]
                                                                );
                                                                systemHoldMes;    printf("input : ");
                                                                scanf("%s", global_buf_KeyBoardInput);
                                                            }
                                                }
                                                #endif
                                    }


                                }
                            }


                                    #ifndef RELEASE
                                    if(
                                        (global_flag_case_getBER_method_Display&FLAG_MASK_DISPLAY_PROGRESS)||
                                        (global_flag_debug_display&FLAG_MASK_DISPLAY_PROGRESS)||
                                        (global_flag_bch_BM_SimSD_display&FLAG_MASK_DISPLAY_PROGRESS)||
                                        (global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)
                                    )
                                    {
                                        printf("[ACT] >>>>> *************************************************************** <<<<<\r\n");

                                        //(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))
                                        for(main_tmp_sel_decAlgo_i=0; main_tmp_sel_decAlgo_i<processingUseThisAlgorithm->length; main_tmp_sel_decAlgo_i++)
                                        {
                                            infoMes; printf("[ACT] [%s] %s test pattern can %s try correction. All TP fail to dec-able flag is %s val = '%d'\r\n",
                                            KIND_OF_BCH_DECODING_ALGORITHM_NAME[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))],
                                            (main_indi_list_flag_fail_allDec[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]?"All":"Some"),
                                            (main_indi_list_flag_fail_allDec[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]?"not":""),
                                            (main_indi_list_flag_fail_allDec[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]?" ON":"OFF"),
                                            main_indi_list_flag_fail_allDec[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]
                                             );
                                        }
                                        printf("\r\n\r\n");

                                        printf(">>>>> ******************************************************************** <<<<<\r\n");
                                        printf(">>>>> ******************************************************************** <<<<<\r\n");
                                    }
                                    #endif
                            /************************************************************/
                            /******* check get BER actual mode fail condition end *******/
                            /************************************************************/





                            /************************************************************/
                            /****** To update total error and bit count, setting up *****/
                            /********* selected test pattern error count start **********/
                            /************************************************************/
                            //(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))
                            for(main_tmp_sel_decAlgo_i=0; main_tmp_sel_decAlgo_i<processingUseThisAlgorithm->length; main_tmp_sel_decAlgo_i++)
                            {
                                /* actual HD err cnt start */
                                if(!main_indi_list_flag_deccFail[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))][0])
                                {
                                    /* Test pattern #0 is not dec fail */
                                    main_indi_HD_errCnt[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))] = main_indi_list_errCntOf_TP_afterDec[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))][0];
                                }
                                else
                                {
                                    /* Test pattern #0 is decoding fail */
                                    /* decoding fail handler */
                                    switch(global_flag_case_dec_fail_handling)
                                    {
                                        default:
                                        case FLAG_CASE_DEC_FAIL_HANDLING_NONE:
                                            //main_indi_HD_errCnt[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))] = main_indi_list_errCntOf_TP_beforeDec[0];
                                            main_indi_HD_errCnt[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))] = main_indi_list_errCntOf_TP_beforeDec[0];
                                        break;

                                        case FLAG_CASE_DEC_FAIL_HANDLING_RE_TRANFER_FAIL_NO_MORE:
                                            main_indi_HD_errCnt[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))] = 0;
                                        break;

                                        case FLAG_CASE_DEC_FAIL_HANDLING_RE_TRANFER_INTERATION:
                                            main_indi_HD_errCnt[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))] = 0;
                                            //not support yet
                                        break;
                                    }
                                }
                                /* actual HD err cnt end */

                                /*
                                 * actual selected CW err cnt.
                                 * one or more test pattern can try correction.
                                 */
                                if(!main_indi_list_flag_fail_allDec[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))])
                                {
                                    /* Selected Test Pattern is not decoding fail, update sel test patterb error ent  */
                                    main_indi_sel_TP_errCnt[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))] =
                                    main_indi_list_errCntOf_TP_afterDec[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))][main_indi_sel_TP_i[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]];
                                }

                                /*
                                 * all test pattern can not try correction.
                                 * selecting index change to test pattern 0.
                                 * if all patterns are decoding fail, get error count error bits before error correction.
                                 */
                                else
                                {
                                    /* Selected Test Pattern is decoding fail */

                                    /* decoding fail handler */
                                    switch(global_flag_case_dec_fail_handling)
                                    {
                                        default:
                                        case FLAG_CASE_DEC_FAIL_HANDLING_NONE:
                                            main_indi_sel_TP_i[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))] = 0;

                                            //main_indi_sel_TP_errCnt[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))] = main_indi_list_errCntOf_TP_beforeDec[0];
                                            main_indi_sel_TP_errCnt[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))] = main_indi_list_errCntOf_TP_beforeDec[0];
                                        break;

                                        case FLAG_CASE_DEC_FAIL_HANDLING_RE_TRANFER_FAIL_NO_MORE:
                                            main_indi_sel_TP_errCnt[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))] = 0;
                                        break;

                                        case FLAG_CASE_DEC_FAIL_HANDLING_RE_TRANFER_INTERATION:
                                            //not support yet
                                            main_indi_sel_TP_errCnt[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))] = 0;
                                        break;
                                    }
                                }

                                                if((global_flag_case_getBER_method_Display&FLAG_MASK_DISPLAY_PROGRESS))
                                                {
                                                    printf("[ACT] /**************************************************************************/\r\n");
                                                    infoMes; printf("[ACT] Error cnt varification.\r\n");
                                                    infoMes; printf("[ACT] main_indi_list_errCntOf_TP_afterDec[][%d] = '%d'\r\n", main_indi_sel_TP_i[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))], main_indi_list_errCntOf_TP_afterDec[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))][main_indi_sel_TP_i[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]]);
                                                    infoMes; printf("[ACT] main_indi_sel_TP_errCnt[] = '%d'\r\n", main_indi_sel_TP_errCnt[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]);
                                                    infoMes; printf("[ACT] BEFORE) main_indi_total_err_cnt_CW[] = '%ld'\r\n", main_indi_total_err_cnt_CW[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]);
                                                }

                                for(main_tmp_soft_i=0; main_tmp_soft_i<main_com_numsTP; main_tmp_soft_i++)
                                {
                                    //closeBmAlgorithmComponent(((struct_HD_BM_algorithmComponent**)*(main_indi_SD_list_of_decordingComponents[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]+main_tmp_soft_i)));
                                    closeAlgoriehmComponent((*(main_indi_SD_list_of_decordingComponents[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]+main_indi_sel_TP_i[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))])), (*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i)));
                                }

                            }
                            /************************************************************/
                            /****** To update total error and bit count, setting up *****/
                            /********** selected test pattern error count end ***********/
                            /************************************************************/


                                        /* verification update start */
                                        cmd_tmp_cnt = -1;
                                        for(main_tmp_sel_decAlgo_i=0; main_tmp_sel_decAlgo_i<processingUseThisAlgorithm->length; main_tmp_sel_decAlgo_i++)
                                        {
                                            if(
                                                (global_flag_case_getBER_method_Display&FLAG_MASK_DISPLAY_PROGRESS)||
                                                (global_flag_bch_BM_SimSD_display&FLAG_MASK_DISPLAY_PROGRESS)
                                            )
                                            {
                                                infoMes; printf("[ACT] err cnt in channel = %d, tp[0] err cnt = %d\r\n", main_com_channel_errCnt, main_indi_list_errCntOf_TP_beforeDec[0]);

                                                infoMes; printf("[ACT] [%s] decoding All fail flag is %s(=%d)\r\n",
                                                    KIND_OF_BCH_DECODING_ALGORITHM_NAME[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))],
                                                    (main_indi_list_flag_fail_allDec[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]?"ON":"OFF"),
                                                    main_indi_list_flag_fail_allDec[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]
                                                );

                                                infoMes; printf("[ACT] [%s] algorithm select TP[%d] and TP[%d] distance with encoded codeword = %d\r\n",
                                                    KIND_OF_BCH_DECODING_ALGORITHM_NAME[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))],
                                                    main_indi_sel_TP_i[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))],
                                                    main_indi_sel_TP_i[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))],
                                                    main_indi_list_errCntOf_TP_afterDec[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))][main_indi_sel_TP_i[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]]
                                                );

                                                infoMes; printf("[ACT] [%s] algorithm sel TP[%d] distance of selTP = %d, updated errCnt = %ld\r\n",
                                                    KIND_OF_BCH_DECODING_ALGORITHM_NAME[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))],
                                                    main_indi_sel_TP_i[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))],
                                                    main_indi_list_errCntOf_TP_afterDec[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))][main_indi_sel_TP_i[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]],
                                                    main_indi_total_err_cnt_CW[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]
                                                );

                                                if(cmd_tmp_cnt == -1) cmd_tmp_cnt = main_indi_list_errCntOf_TP_afterDec[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))][main_indi_sel_TP_i[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]];
                                                else
                                                {
                                                    if(cmd_tmp_cnt < main_indi_list_errCntOf_TP_afterDec[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))][main_indi_sel_TP_i[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]])
                                                    {
                                                        debugMes; printf("[ACT] cmd_tmp_cnt = %d < main_indi_list_errCntOf_TP_afterDec of %d = %d\r\n",
                                                            cmd_tmp_cnt,
                                                            (*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i)),
                                                            main_indi_list_errCntOf_TP_afterDec[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))][main_indi_sel_TP_i[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]]
                                                        );
                                                        systemHoldMes;    printf("input : ");
                                                        scanf("%s", global_buf_KeyBoardInput);
                                                    }
                                                }
                                            }
                                        }
                                        /* verification update start */



                        }
                        break;

                        default:
                            errorMes;
                            printf("Get BER mode is not selected so, can't procedure program.\r\n");
                            printf("Program is closed forcibly.\r\n");
                            return -1;
                            break;
                    }




                    /***********************************************************************************************/
                    /*************** Total channel and coding error bits, transfer bits, calculation ***************/
                    /***********************************************************************************************/
                    //(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))
                    for(main_tmp_sel_decAlgo_i=0; main_tmp_sel_decAlgo_i<processingUseThisAlgorithm->length; main_tmp_sel_decAlgo_i++)
                    {
                        main_indi_total_err_cnt_HD[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]+=(unsigned long)main_indi_HD_errCnt[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]; //printf("main_indi_sel_TP_errCnt[KIND_OF_BCH_DECODING_BM] %d\n", main_indi_sel_TP_errCnt[KIND_OF_BCH_DECODING_BM]);
                        main_indi_total_err_cnt_CW[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]+=(unsigned long)main_indi_sel_TP_errCnt[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]; //printf("main_indi_sel_TP_errCnt[KIND_OF_BCH_DECODING_BM] %d\n", main_indi_sel_TP_errCnt[KIND_OF_BCH_DECODING_BM]);
                    }

                    main_com_total_err_cnt_CH+=(unsigned long)main_com_channel_errCnt;    //printf("main_com_channel_errCnt %d\n", main_com_channel_errCnt);
                    main_com_total_bit+=(unsigned long)main_com_codeLength;


                                            if((global_flag_case_getBER_method_Display&FLAG_MASK_DISPLAY_PROGRESS))
                                            {
                                                //(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))
                                                for(main_tmp_sel_decAlgo_i=0; main_tmp_sel_decAlgo_i<processingUseThisAlgorithm->length; main_tmp_sel_decAlgo_i++)
                                                {
                                                    infoMes; printf("AFTER) main_indi_total_err_cnt_CW[] = '%ld'\r\n", main_indi_total_err_cnt_CW[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]);
                                                    printf("/**************************************************************************/\r\n");
                                                }
                                            }

                    //printf(",M_ToT_CW_ER[%ld], ONE_CW_ER[%d]", main_indi_total_err_cnt_CW[KIND_OF_BCH_DECODING_BM], main_indi_sel_TP_errCnt[KIND_OF_BCH_DECODING_BM]);
                    //printf("M_ToT_CH_ER[%ld], ONE_CH_ER%d", main_com_total_err_cnt_CH, main_com_channel_errCnt);
                    //printf("M_ToT_BITs[%ld]\n", main_com_total_bit);

                    /***********************************************************************************************/
                    /*************** Total channel and coding error bits, transfer bits, calculation ***************/
                    /***********************************************************************************************/
                }
                /**************************************************************/
                /************* (Global Flag) AWGN simulation end **************/
                /**************************************************************/
                            if((global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)||(global_flag_case_getBER_method_Display&FLAG_MASK_DISPLAY_PROGRESS))
                            {
                                //(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))
                                for(main_tmp_sel_decAlgo_i=0; main_tmp_sel_decAlgo_i<processingUseThisAlgorithm->length; main_tmp_sel_decAlgo_i++)
                                {
                                    infoMes; printf("total coding error bits nums\t: %ld, one codeword coding error bits nums\t: %d\r\n", main_indi_total_err_cnt_CW[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))], main_indi_sel_TP_errCnt[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]);
                                }
                                infoMes; printf("total channel error nums\t: %ld, one codeword channel error nums\t: %d\r\n", main_com_total_err_cnt_CH, main_com_channel_errCnt);
                                infoMes; printf("total bit nums\t: %ld, one codeword bit nums\t: %d\r\n", main_com_total_bit, main_com_codeLength);
                            }
            }
        /*****************************************************************************/
        /*****************************************************************************/
        /******************** simulation main_com_loopCount loop end *********************/
        /*****************************************************************************/
        /*****************************************************************************/

            printf("/*********************** Simulation Report ***********************/\r\n");
            //(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))
            for(main_tmp_sel_decAlgo_i=0; main_tmp_sel_decAlgo_i<processingUseThisAlgorithm->length; main_tmp_sel_decAlgo_i++)
            {
                resultMes;    printf("[%s], %s has = %ld bit errs\ttotal bit cnt = %ld\tchannel err bit cnt =%ld\r\n",
                                global_flag_sim_decodingMode&FLAG_MASK_CODE_BCH_DEC_AL_SOFT_DECISION?"Soft-Decision":
                                global_flag_sim_decodingMode&FLAG_MASK_CODE_BCH_DEC_AL_HARD_DECISION?"Hard-Decision":
                                "UNKN-Decision",

                                KIND_OF_BCH_DECODING_ALGORITHM_NAME[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))],
                                main_indi_total_err_cnt_CW[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))],
                                main_com_total_bit,
                                main_com_total_err_cnt_CH
                            );
            }


            //(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))
            main_com_channel_BER = (double)main_com_total_err_cnt_CH / (double)main_com_total_bit;

            resultMes; printf("EbN0 : %g\r\n", main_com_EbN0);
            resultMes; printf("[Ch]BER\t: %g\r\n", main_com_channel_BER);
            for(main_tmp_sel_decAlgo_i=0; main_tmp_sel_decAlgo_i<processingUseThisAlgorithm->length; main_tmp_sel_decAlgo_i++)
            {
                main_indi_HD_BER[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))] = (double)main_indi_total_err_cnt_HD[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]/(double)main_com_total_bit;
                main_indi_BER[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))] = (double)main_indi_total_err_cnt_CW[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]/(double)main_com_total_bit;

                resultMes; printf("[%s] BER\t: %g(Static HD)\r\n",  KIND_OF_BCH_DECODING_ALGORITHM_NAME[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))], main_indi_HD_BER[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]);
                resultMes; printf("[%s] BER\t: %g(Mode : %s)\r\n",
                    KIND_OF_BCH_DECODING_ALGORITHM_NAME[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))],
                    main_indi_BER[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))],
                        global_flag_sim_decodingMode&FLAG_MASK_CODE_BCH_DEC_AL_SOFT_DECISION?"SD":
                        global_flag_sim_decodingMode&FLAG_MASK_CODE_BCH_DEC_AL_HARD_DECISION?"HD":
                        "UNKN"
                    );
            }

            // printf("LLR MAX : \'%.2f\'\tLLR MIN : \'%.2f\'\r\n", LLR_MAX, LLR_MIN);

            resultMes; printf("in loop %ld, test_fileIO_errorCorrectingCount is %d\r\n", main_com_totalSamlingLoop, test_fileIO_errorCorrectingCount);
            printExcutingTime(&time_EbN0_start);
            printf("/*****************************************************************/\r\n");

            /*******************************************************/
            /*******************************************************/
            /******************** REPORT OUTPUT ********************/
            /*******************************************************/
            /*******************************************************/
            fprintf(fileio_channelBER, "@%g, CH_BER : %g\r\n", main_com_EbN0, main_com_channel_BER);

            fprintf(fileio_total_bit_cnt, "@%g, Total Bit Cnt : %ld\r\n", main_com_EbN0, main_com_total_bit);
            fprintf(fileio_channel_err_bit_cnt, "@%g, Channel Err Cnt : %ld\r\n", main_com_EbN0, main_com_total_err_cnt_CH);


            //(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))
            for(main_tmp_sel_decAlgo_i=0; main_tmp_sel_decAlgo_i<processingUseThisAlgorithm->length; main_tmp_sel_decAlgo_i++)
            {
                fprintf(fileio_codingBER, "@%g, [%s] BER : %g\r\n",
                    main_com_EbN0,
                    KIND_OF_BCH_DECODING_ALGORITHM_NAME[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))],
                    main_indi_BER[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]
                );

                fprintf(fileio_indi_algorithm_coding_HD_BER[(*(((uint8_t*)processingUseThisAlgorithm->list)+main_tmp_sel_decAlgo_i))], "[%s(HD)] BER : %g\r\n",
                    KIND_OF_BCH_DECODING_ALGORITHM_NAME[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))],
                    main_indi_HD_BER[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]
                );

                fprintf(fileio_indi_algorithm_codingBER[(*(((uint8_t*)processingUseThisAlgorithm->list)+main_tmp_sel_decAlgo_i))], "[%s] BER : %g\r\n",
                    KIND_OF_BCH_DECODING_ALGORITHM_NAME[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))],
                    main_indi_BER[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]
                );


                //main_indi_HD_BER[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))] = (double)main_indi_total_err_cnt_HD[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]/(double)main_com_total_bit;
                fprintf(fileio_indi_algorithm_coding_HD_err_bit_cnt[(*(((uint8_t*)processingUseThisAlgorithm->list)+main_tmp_sel_decAlgo_i))], "@%g, [%s(HD)] ERR Cnt : %ld\r\n",
                    main_com_EbN0,
                    KIND_OF_BCH_DECODING_ALGORITHM_NAME[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))],
                    main_indi_total_err_cnt_HD[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]
                );

                //main_indi_BER[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))] = (double)main_indi_total_err_cnt_CW[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]/(double)main_com_total_bit;
                fprintf(fileio_indi_algorithm_coding_err_bit_cnt[(*(((uint8_t*)processingUseThisAlgorithm->list)+main_tmp_sel_decAlgo_i))], "@%g, [%s] Err Cnt : %ld\r\n",
                    main_com_EbN0,
                    KIND_OF_BCH_DECODING_ALGORITHM_NAME[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))],
                    main_indi_total_err_cnt_CW[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]
                );
            }
            fprintf(fildio_EbN0, "%g\r\n", main_com_EbN0);
            fprintf(fildio_loop_Cnt, "%ld\r\n", main_com_totalSamlingLoop);
        }

        break;
    }
    /*******************************************************************/
    /*******************************************************************/
    /******************** simulation EbN0 loop end *********************/
    /*******************************************************************/
    /*******************************************************************/
    /*******************************************************/
    /****************** REPORT FILE CLOSE ******************/
    /*******************************************************/
    fclose(fileio_channelBER);
    fclose(fileio_codingBER);

    fclose(fileio_total_bit_cnt);
    fclose(fileio_channel_err_bit_cnt);

    for(main_tmp_sel_decAlgo_i=0; main_tmp_sel_decAlgo_i<processingUseThisAlgorithm->length; main_tmp_sel_decAlgo_i++)
    {
        fclose(fileio_indi_algorithm_coding_HD_BER\
                [(*(((uint8_t*)processingUseThisAlgorithm->list)+main_tmp_sel_decAlgo_i))]\
        );
        fclose(fileio_indi_algorithm_codingBER\
                [(*(((uint8_t*)processingUseThisAlgorithm->list)+main_tmp_sel_decAlgo_i))]\
        );

        fclose(fileio_indi_algorithm_coding_HD_err_bit_cnt\
                [(*(((uint8_t*)processingUseThisAlgorithm->list)+main_tmp_sel_decAlgo_i))]\
        );
        fclose(fileio_indi_algorithm_coding_err_bit_cnt\
                [(*(((uint8_t*)processingUseThisAlgorithm->list)+main_tmp_sel_decAlgo_i))]\
        );
    }
    free(fileio_indi_algorithm_coding_HD_BER);
    free(fileio_indi_algorithm_codingBER);

    fclose(fileio_channelFER);
    fclose(fileio_codingFER);

    fclose(fildio_EbN0);
    fclose(fildio_loop_Cnt);
    /*******************************************************/
    /****************** REPORT FILE CLOSE ******************/
    /*******************************************************/

    /*******************************************************/
    /***************** PATTEWRN FILE CLOSE *****************/
    /*******************************************************/
    /* pattern file io */
    if(global_flag_file_io_hd_pattern_output&FLAG_MASK_FILE_IO_HD_MES)
    {
        fclose(fileio_HD_mes);
    }
    if(global_flag_file_io_hd_pattern_output&FLAG_MASK_FILE_IO_HD_ENCODING_MES)
    {
        fclose(fileio_HD_encoding_mes);
    }
    if(global_flag_file_io_hd_pattern_output&FLAG_MASK_FILE_IO_HD_RECEIVED_MES)
    {
        fclose(fileio_HD_received_mes);
    }


    if(global_flag_file_io_sd_pattern_output&FLAG_MASK_FILE_IO_SD_RECEIVED_LLR)
    {
        fclose(fileio_SD_received_LLR);
    }

    /* have to concern test patterns */
    for(main_tmp_sel_decAlgo_i=0; main_tmp_sel_decAlgo_i<processingUseThisAlgorithm->length; main_tmp_sel_decAlgo_i++)
    {
        if(global_flag_file_io_hd_pattern_output&FLAG_MASK_FILE_IO_HD_CORRECTED_MES)
        {
            fclose(fileio_corrected_mes\
                    [(*(((uint8_t*)processingUseThisAlgorithm->list)+main_tmp_sel_decAlgo_i))]\
            );
        }
    }
    /*******************************************************/
    /***************** PATTEWRN FILE CLOSE *****************/
    /*******************************************************/

    /*******************************************************/
    /************* PATTERN VERILOG FILE CLOSE **************/
    /*******************************************************/
    /* pattern file io */
    if(global_flag_file_io_hd_pattern_output&FLAG_MASK_FILE_IO_HD_MES)
    {
        fclose(fileio_HD_mes_verilog);
    }
    if(global_flag_file_io_hd_pattern_output&FLAG_MASK_FILE_IO_HD_ENCODING_MES)
    {
        fclose(fileio_HD_encoding_mes_verilog);
    }
    if(global_flag_file_io_hd_pattern_output&FLAG_MASK_FILE_IO_HD_RECEIVED_MES)
    {
        fclose(fileio_HD_received_mes_verilog);
    }


    if(global_flag_file_io_sd_pattern_output&FLAG_MASK_FILE_IO_SD_RECEIVED_LLR)
    {
        fclose(fileio_SD_received_LLR_verilog);
    }

    /* have to concern test patterns */
    for(main_tmp_sel_decAlgo_i=0; main_tmp_sel_decAlgo_i<processingUseThisAlgorithm->length; main_tmp_sel_decAlgo_i++)
    {
        if(global_flag_file_io_hd_pattern_output&FLAG_MASK_FILE_IO_HD_CORRECTED_MES)
        {
            fclose(fileio_corrected_mes_verilog\
                    [(*(((uint8_t*)processingUseThisAlgorithm->list)+main_tmp_sel_decAlgo_i))]\
            );
        }
    }
    /*******************************************************/
    /************* PATTERN VERILOG FILE CLOSE **************/
    /*******************************************************/

    /*******************************************************/
    /*************** PATTEWRN LOG FILE CLOSE ***************/
    /*******************************************************/
    if(global_flag_file_io_hd_pattern_output_log&FLAG_MASK_FILE_IO_HD_MES)
    {
        fclose(fileio_HD_mes_log);
    }
    if(global_flag_file_io_hd_pattern_output_log&FLAG_MASK_FILE_IO_HD_ENCODING_MES)
    {
        fclose(fileio_HD_encoding_mes_log);
    }
    if(global_flag_file_io_hd_pattern_output_log&FLAG_MASK_FILE_IO_HD_RECEIVED_MES)
    {
        fclose(fileio_HD_received_mes_log);
    }
    if(global_flag_file_io_sd_pattern_output_log&FLAG_MASK_FILE_IO_SD_RECEIVED_LLR)
    {
        fclose(fileio_SD_received_LLR_log);
        fclose(fileio_ErrCnt_log);
    }

    for(main_tmp_sel_decAlgo_i=0; main_tmp_sel_decAlgo_i<processingUseThisAlgorithm->length; main_tmp_sel_decAlgo_i++)
    {
        if(global_flag_file_io_hd_pattern_output_log&FLAG_MASK_FILE_IO_HD_CORRECTED_MES)
        {
            fclose(fileio_corrected_mes_log\
                    [(*(((uint8_t*)processingUseThisAlgorithm->list)+main_tmp_sel_decAlgo_i))]\
            );
        }
    }
    /*******************************************************/
    /*************** PATTEWRN LOG FILE CLOSE ***************/
    /*******************************************************/
    /* Test Mode Log File IO Close */
    if(global_flag_case_sim_testOpt)
    {
        fclose(fp_testMode_log);

        fclose(fp_testMode_sum_bit_cnt_arranged_by_bitRetio_csv);
        fclose(fp_testMode_sum_bit_cnt_classified_arranged_by_bitRatio_csv);
        fclose(fp_testMode_sum_llr_mag_arranged_by_bitRatio_csv);
        fclose(fp_testMode_sum_err_bit_cnt_chg_classified_arranged_by_bitRatio_csv);
        fclose(fp_testMode_all_arranged_by_bitRatio_csv);

        fclose(fp_testMode_sum_codeword_cnt_arranged_by_errorNums_csv);
        fclose(fp_testMode_sum_llr_mag_distribution_cnt_arranged_by_errorNums_csv);
        fclose(fp_testMode_sum_bit_ratio_cnt_arranged_by_errorNums_csv);
        fclose(fp_testMode_all_arranged_by_errorNums_csv);
    }
    /* Test Mode Log File IO Close */



    closeErrorComponent(&main_com_errComponents);

    closeGaloisFieldExceptElements(&main_com_expandEqOfLcmFromRoot);
    free(main_com_genPoly);
    main_com_genPoly=NULL;
    closeEncodingComponent(&main_encodingComponentInGF);

    closeBpskComponents(&main_com_bpskComponents);

    for(main_tmp_sel_decAlgo_i=0; main_tmp_sel_decAlgo_i<processingUseThisAlgorithm->length; main_tmp_sel_decAlgo_i++)
    {
        for(main_tmp_soft_i=0; main_tmp_soft_i<main_com_numsTP; main_tmp_soft_i++)
        {
            //closeBmAlgorithmComponent(((struct_HD_BM_algorithmComponent**)*(main_indi_SD_list_of_decordingComponents[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]+main_indi_sel_TP_i[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))])));
            closeAlgoriehmComponent(\
                    (*(main_indi_SD_list_of_decordingComponents\
                        [(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]\
                        +main_indi_sel_TP_i\
                            [(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]\
                    )), (*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))\
                );
        }
    }


    closeGaloisFieldExceptElements(&main_com_expandEqOfLcmFromRoot);


    printf("[Simulation Time]"); printExcutingTime(&time_program_init);


    printf("\r\n\r\n");
    infoMes;
    printf("number of arguments is %d\r\n",argc);
    for(i=0; i<argc; i++)
    {
        printf("%s ",argv[i]);
    }
    printf("\r\n");


    return 0;
}
