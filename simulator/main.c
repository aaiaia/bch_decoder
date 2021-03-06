/*
 *
 *
 *
 *
 *
*/

#include <stdio.h>
#include <unistd.h>
#include <dirent.h>//directory lib
#include <errno.h>//error number
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <malloc.h>
#include <memory.h>
#include <time.h>	//time()
#include <unistd.h>  // getpid()

#include <errno.h> //errno

#include <sys/time.h>

	#ifdef USING_OPEN_MP
	#include<omp.h>

	#ifndef THREAD_NUMS
		#define THREAD_NUMS 4
	#endif

	#endif

// #include "./bmp_file_io.h"


unsigned int global_seed;


const uint8_t uint8_bitMask[8] = {(1<<0),(1<<1),(1<<2),(1<<3),(1<<4),(1<<5),(1<<6),(1<<7)};
const uint16_t uint16_bitMask[16] =
{
	(1<<0),(1<<1),(1<<2),(1<<3),(1<<4),(1<<5),(1<<6),(1<<7),\
	(1<<8),(1<<9),(1<<10),(1<<11),(1<<12),(1<<13),(1<<14),(1<<15)\
};
const uint32_t uint32_bitMask[32] = 
{
	(1<<0),(1<<1),(1<<2),(1<<3),(1<<4),(1<<5),(1<<6),(1<<7),\
	(1<<8),(1<<9),(1<<10),(1<<11),(1<<12),(1<<13),(1<<14),(1<<15),\
	(1<<16),(1<<17),(1<<18),(1<<19),(1<<20),(1<<21),(1<<22),(1<<23),\
	(1<<24),(1<<25),(1<<26),(1<<27),(1<<28),(1<<29),(1<<30),(1<<31)\
};
const uint64_t uint64_bitMask[64] = 
{
	(1UL<<0),(1UL<<1),(1UL<<2),(1UL<<3),(1UL<<4),(1UL<<5),(1UL<<6),(1UL<<7),\
	(1UL<<8),(1UL<<9),(1UL<<10),(1UL<<11),(1UL<<12),(1UL<<13),(1UL<<14),(1UL<<15),\
	(1UL<<16),(1UL<<17),(1UL<<18),(1UL<<19),(1UL<<20),(1UL<<21),(1UL<<22),(1UL<<23),\
	(1UL<<24),(1UL<<25),(1UL<<26),(1UL<<27),(1UL<<28),(1UL<<29),(1UL<<30),(1UL<<31),\
	(1UL<<32),(1UL<<33),(1UL<<34),(1UL<<35),(1UL<<36),(1UL<<37),(1UL<<38),(1UL<<39),\
	(1UL<<40),(1UL<<41),(1UL<<42),(1UL<<43),(1UL<<44),(1UL<<45),(1UL<<46),(1UL<<47),\
	(1UL<<48),(1UL<<49),(1UL<<50),(1UL<<51),(1UL<<52),(1UL<<53),(1UL<<54),(1UL<<55),\
	(1UL<<56),(1UL<<57),(1UL<<58),(1UL<<59),(1UL<<60),(1UL<<61),(1UL<<62),(1UL<<63)\
};






/*************************************************/
/* DEFINITION OF FILE INPUT OUTPUT PARSING START */
/*************************************************/
enum enum_FILE_IO_KIND_OF_DATA
{
	FILE_IO_KIND_OF_DATA_UNKWON,
	FILE_IO_KIND_OF_DATA_GALOIS_FIELDS,
	FILE_IO_KIND_OF_DATA_CONJUGATE_SET,
	FILE_IO_KIND_OF_DATA_SUMATION_MATRIX,
}typedef enum_FILE_IO_KIND_OF_DATA;


enum enum_FILD_IO_DATA_ARRAY_TYPE
{
	FILD_IO_DATA_ARRAY_TYPE_UNKNOWN,
	FILD_IO_DATA_ARRAY_TYPE_LINEAR,
	FILD_IO_DATA_ARRAY_TYPE_NONE_LINEAR,
}typedef enum_FILD_IO_DATA_ARRAY_TYPE;

enum enum_FILE_IO_DATA_TYPE
{
	FILE_IO_DATA_TYPE_UNKOWN,
	FILE_IO_DATA_TYPE_RAW_DATA,
	FILE_IO_DATA_TYPE_ADDRESS_OFFSET,
}typedef enum_FILE_IO_DATA_TYPE;
char *FILE_IO_DATA_PARSING_KEY_STRING_SAVED_DATA_START = "[DATA_save_START]";
char *FILE_IO_DATA_PARSING_KEY_STRING_SAVED_DATA_END = "[DATA_save_END]";

char *FILE_IO_DATA_PARSING_KEY_STRING_KIND_OF_DATA_START = "[Kind_Of_Data_START]";
char *FILE_IO_DATA_PARSING_KEY_STRING_KIND_OF_DATA_END = "[Kind_Of_Data_END]";

char *FILE_IO_DATA_PARSING_KEY_STRING_ARRAY_TYPE_START = "[Data_Array_Type_START]";
char *FILE_IO_DATA_PARSING_KEY_STRING_ARRAY_TYPE_END = "[Data_Array_Type_END]";

char *FILE_IO_DATA_PARSING_KEY_STRING_BIT_WIDTH_START = "[Data_Bit_Width_START]";
char *FILE_IO_DATA_PARSING_KEY_STRING_BIT_WIDTH_END = "[Data_Bit_Width_END]";

char *FILE_IO_DATA_PARSING_KEY_STRING_ROW_DATA_START = "[Row_Length_START]";
char *FILE_IO_DATA_PARSING_KEY_STRING_ROW_DATA_END = "[Row_Length_END]";

char *FILE_IO_DATA_PARSING_KEY_STRING_COL_DATA_START = "[Column_Length_START]";
char *FILE_IO_DATA_PARSING_KEY_STRING_COL_DATA_END = "[Column_Length_END]";

char *FILE_IO_DATA_PARSING_KEY_STRING_RAW_DATA_START = "[Raw_Data_START]";
char *FILE_IO_DATA_PARSING_KEY_STRING_RAW_DATA_END = "[Raw_Data_START_END]";

/*************************************************/
/* DEFINITION OF FILE INPUT OUTPUT PARSING END   */
/*************************************************/





struct tm *date_simulation_date;
//date_simulation_date=localtime(&time_program_init.tv_sec);
//date_simulation_date->tm_year+1900
//date_simulation_date->tm_mon+1
//date_simulation_date->tm_mday
//date_simulation_date->tm_hour
//date_simulation_date->tm_min
//date_simulation_date->tm_sec
//	gettimeofday(&time_proc_start, NULL);
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
		timeMes;	printf("%ld [sec], %ld [usec]\n", diff.tv_sec, diff.tv_usec);
	}
	else
	{
		timeMes;	printf("%ld [sec], %ld [usec]\n", diff.tv_sec, diff.tv_usec);	
	}
}




char global_buf_KeyBoardInput[255]={0};


#define errorMes	printf("ERROR ) ")
#define warningMes	printf("/*********************************/\n/********** WARNING !!! **********/\n/*********************************/\nWARNING ) ")
#define warningMesShort	printf("WARNING ) ")
#define infoMes		printf("Info ) ")

#define initMes		printf("Initial ) ")

#define resultMes	printf("Result ) ")


#define OPTION_WORD	'-'


unsigned int global_flag_cmdOption=0;
#define PRINTF_LOG_WORD	"l"
#define FLAG_MASK_PRINTF_LOG	(1<<0)
#define logMes printf("LOG ) ")

#define PRINTF_CALCULATE_PROCESS_WORD	"c"
#define FLAG_MASK_PRINTF_CALCULATE_PROCESS	(1<<1)
#define calculateMes printf("CALCULATE ) ")

#define PRINTF_MEMORY_CHECK_PROCESS_WORD	"m"
#define FLAG_MASK_PRINTF_MEMORY_CHECK_PROCESS	(1<<2)
#define memoryMes printf("MEMORY ) ")

#define IGNORE_WARNING_MES_WORD	"w"
#define FLAG_MASK_PRINTF_WARNING_MES	(1<<3)
	/* variables to check memory lineary */
	unsigned int global_tmp_i_memoryCheck=0;


/* Sysgem Hold command */
unsigned int global_flag_cmdSystemHold=0;
#define PRINTF_HELP_WORD	"h"
#define FLAG_MASK_HOLD_HELP		(1<<0)
#define systemHoldMes		printf("HOLD ) ")



/* forced Sysgem unhold command */
unsigned int global_flag_cmdSystemUnhold=0;
#define SYSTEM_UNHOLD_FORCED_WORD	"f"
#define FLAG_MASK_SYSTEM_UNHOLD_FORCED	(1<<0)//if(global_flag_cmdSystemUnhold&FLAG_MASK_SYSTEM_UNHOLD_FORCED)
#define systemUnholdMes		printf("UNHOLD ) ")




#define ADDED_OPTION_WORD	'-'



	/*Algorithm Functional Test*/
	
	#define ALGORITHM_FUNC			(1<<0)

	#define ALGORITHM_CHANNEL		(1<<1)
	
	/*Display RESULT or Progress or Function name*/
	enum DEBUG_DISPLAY_MODE
	{
		FLAG_MASK_DISPLAY_RESULT = (1<<0),
		FLAG_MASK_DISPLAY_PROGRESS = (1<<1),
		FLAG_MASK_DISPLAY_FUNCNAME = (1<<2),
	};


	
/*Global debug option start*/
unsigned int global_flag_deBugOption=0;
#define DEBUG_DEBUG_WORD	 "debug"//FLAG_MASK_DISPLAY_PROGRESS
#define FLAG_MASK_DEBUG_DEBUG	(1<<0)
#define debugMes	printf("debug ) ");

	unsigned int global_flag_debug_display=0;
	/*
	#define FLAG_MASK_DISPLAY_RESULT			(1<<0)
	#define FLAG_MASK_DISPLAY_PROGRESS		(1<<1)
	#define FLAG_MASK_DISPLAY_FUNCNAME		(1<<2)
	*/
	#define debugFuncNameMes	printf("debug-funcname ) ");
	#define debugSequenceMes	printf("debug-progress ) ");
/*Global debug option end*/



	/*BM Algorithm debug option start*/
	unsigned int global_flag_debug_bmSim=0;
	#define DEBUG_BCH_BM_ALGORITHM_WORD	 "debug-bm"//OPT_DISPLAY_PROGRESS
	#define FLAG_MASK_DEBUG_BCH_BM_ALGORITHM	(1<<0)
	#define debugBmAlgorithmMes	printf("debug-BM ) ");

	#define DEBUG_BCH_BM_CAL_DELTA_WORD	 "debug-bm-cal-delta"//OPT_DISPLAY_PROGRESS
	#define FLAG_MASK_DEBUG_BCH_BM_CAL_DELTA	(1<<1)
	#define debugBmAlgorithmCalDeltaMes	printf("debug-BM-cal-delta ) ");
	/*BM Algorithm debug option end*/

	/*mSBS Algorithm debug option start*/
	/* below variables could be unnecessary */
	unsigned int global_flag_debug_mSBS_Sim=0;
	#define DEBUG_BCH_mSBS_ALGORITHM_WORD	 "debug-mSBS"//OPT_DISPLAY_PROGRESS
	#define FLAG_MASK_DEBUG_BCH_mSBS_ALGORITHM	(1<<0)
	#define debug_mSBS_AlgorithmMes	printf("debug-mSBS ) ");

	#define DEBUG_BCH_M_SBS_CAL_DELTA_WORD	 "debug-mSBS-cal-delta"//OPT_DISPLAY_PROGRESS
	#define FLAG_MASK_DEBUG_BCH_M_SBS_CAL_DELTA	(1<<1)
	#define debug_mSBS_AlgorithmCalDeltaMes	printf("debug-mSBS-cal-delta ) ");
	/*mSBS Algorithm debug option end*/



unsigned int global_flag_berOptions=0;
// #define TEST_GET_BER_WORD	"get-ber"//SETVALUE
#define FLAG_MASK_TEST_GET_BER	(1<<0)
#define testGetBerMes	printf("test GET-BER ) ");
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
	
	#define FLAG_CASE_DECODING_FAIL_METHOD_DEFAULT		FLAG_CASE_SEL_TP_BEFORE_DEC_THEO
	

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
			#define FLAG_MASK_DISPLAY_RESULT			(1<<0)
			#define FLAG_MASK_DISPLAY_PROGRESS		(1<<1)
			#define FLAG_MASK_DISPLAY_FUNCNAME		(1<<2)
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
#define FLAG_MASK_CODE_BCH			(1<<0)
#define mes_bchCode					printf("BCH code) ");

	//////////////////////////////////////////////////////////////////////////////////
	//////////////////////* Common BCH Codes Calculations start */////////////////////
	//////////////////////////////////////////////////////////////////////////////////
	enum KIND_OF_BCH_DECODING_ALGORITHM
	{
		KIND_OF_BCH_DECODING_BM,
		// KIND_OF_BCH_DECODING_mSBS,
		KIND_OF_BCH_DECODING_iBM,
		KIND_OF_BCH_DECODING_RiBM,
		KIND_OF_BCH_DECODING_SiBM,
		KIND_OF_BCH_DECODING_mSBS,
		// KIND_OF_BCH_DECODING_BM,
		KIND_OF_BCH_DECODING_PETERSON,
		KIND_OF_BCH_DECODING_EUCLIDIAN,
		KIND_OF_BCH_DECODING_END,
	};
	
	const char KIND_OF_BCH_DECODING_ALGORITHM_NAME[KIND_OF_BCH_DECODING_END][21] = \
	{					
		"BM",			
		// "mSBS",			
		"iBM",			
		"RiBM",			
		"SiBM",			
		"mSBS",			
		// "BM",			
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
		FLAG_MASK_BCH_DECORDING_ALGORITHM_BM		= (1<<KIND_OF_BCH_DECODING_BM),
		FLAG_MASK_BCH_DECORDING_ALGORITHM_iBM		= (1<<KIND_OF_BCH_DECODING_iBM),
		FLAG_MASK_BCH_DECORDING_ALGORITHM_RiBM		= (1<<KIND_OF_BCH_DECODING_RiBM),
		FLAG_MASK_BCH_DECORDING_ALGORITHM_SiBM		= (1<<KIND_OF_BCH_DECODING_SiBM),
		FLAG_MASK_BCH_DECORDING_ALGORITHM_M_SBS		= (1<<KIND_OF_BCH_DECODING_mSBS),
		FLAG_MASK_BCH_DECORDING_ALGORITHM_PETERSON	= (1<<KIND_OF_BCH_DECODING_PETERSON),
		FLAG_MASK_BCH_DECORDING_ALGORITHM_EUCLIDIAN	= (1<<KIND_OF_BCH_DECODING_EUCLIDIAN),
	};

    /* Simulation Working Mode */
    unsigned int global_flag_sim_workingMode = 0;
    enum SIMULATION_WORKING_MODE
    {
        FLAG_MASK_SIM_DECOING = (1<<0),
        FLAG_MASK_SIM_ABSTRACT_DATA = (1<<1),//reserved Working mode
        FLAG_MASK_SIM_TEST = (1<<2),
    };

	/* global_list_flag_bch_sim_decodingMode */
	unsigned int global_flag_sim_decodingMode = 0;
	unsigned int global_list_flag_bch_sim_decodingMode[KIND_OF_BCH_DECODING_END] = {0};
		/*******************************************************************/
		/********** global flag associate wieh bch - BM algorithm **********/
		/*******************************************************************/
		enum BCH_ALGORITHM_COMMON_DECODING_OPTIONS
		{
			FLAG_MASK_CODE_BCH_DEC_AL_TEST_THIS_ALGORITHM = (1<<0),
			FLAG_MASK_CODE_BCH_DEC_AL_HARD_DECISION = (1<<1),
			FLAG_MASK_CODE_BCH_DEC_AL_SOFT_DECISION = (1<<2),
		
            FLAG_MASK_LRB_LLR_TEST = (1<<3),        

			FLAG_MASK_CODE_BCH_DEC_AL_SKIP = (1<<15),			
		};
		// unsigned int global_flag_bchBM_simulationMode=0;
		// enum BCH_BM_SIMULATION_MODE
		// {
			// FLAG_MASK_TEST_BM_ALGORITHM = (1<<0),
			// FLAG_MASK_CODE_BCH_BM_HARD_DECISION = (1<<1),
			// FLAG_MASK_CODE_BCH_BM_SOFT_DECISION = (1<<2),
			
			// FLAG_MASK_CODE_BCH_BM_DECODING_SKIP = (1<<15),
		// };
		
		/*Correctable bits length, set global_flag_bchBM_simulationMode|=FLAG_MASK_CODE_BCH_DEC_AL_HARD_DECISION*/
		/*
		#define FLAG_MASK_DISPLAY_RESULT			(1<<0)
		#define FLAG_MASK_DISPLAY_PROGRESS		(1<<1)
		#define FLAG_MASK_DISPLAY_FUNCNAME		(1<<2)
		*/
		// #define infoMes		printf("test BCH BM HARD ) ")
			unsigned int global_flag_bch_BM_SimHD_display=0;
		/*LLR Quantization value Set*/
		// #define infoMes		printf("test BCH BM SOFT ) ")
			unsigned int global_flag_bch_BM_SimSD_display=0;//make common thing.
			#define test_BM_SoftDisplayMes	printf("test SBCH BM SOFT DISPLAY ) ");
			

		/*******************************************************************/
		/********** global flag associate wieh bch - mSBS algorithm **********/
		/*******************************************************************/
		// unsigned int global_flag_bch_mSBS_simulationMode = 0;
		// enum BCH_M_SBS_SIMULATION_MODE
		// {
			// FLAG_MASK_CODE_BCH_DEC_AL_TEST_THIS_ALGORITHM = (1<<0),
			// FLAG_MASK_CODE_BCH_DEC_AL_HARD_DECISION = (1<<1),
			// FLAG_MASK_CODE_BCH_DEC_AL_SOFT_DECISION = (1<<2),
			
			// FLAG_MASK_CODE_BCH_DEC_AL_SKIP = (1<<15),
		// };
		
		/*Correctable bits length, set global_flag_bchBM_simulationMode|=FLAG_MASK_CODE_BCH_DEC_AL_HARD_DECISION*/
		/*
		#define FLAG_MASK_DISPLAY_RESULT			(1<<0)
		#define FLAG_MASK_DISPLAY_PROGRESS		(1<<1)
		#define DISPLAY_FUNCNAME_WORD	"funcname"
		#define FLAG_MASK_DISPLAY_FUNCNAME		(1<<2)
		*/
		#define test_mSBS_HardMes		printf("test BCH mSBS HARD ) ");
			unsigned int global_flag_bch_mSBS_SimHD_display=0;
		/*LLR Quantization value Set*/
		#define test_mSBS_SoftMes		printf("test BCH mSBS SOFT ) ");
			unsigned int global_flag_bch_mSBS_SimSD_display=0;
			#define test_mSBS_SoftDisplayMes	printf("test BCH mSBS  SOFT DISPLAY ) ");
			
			
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
#define FLAG_MASK_FILE_IO	(1<<0)
unsigned int global_flag_fileInputOutput = 0;

    unsigned int global_flag_file_io_hd_pattern_output = 0;
    unsigned int global_flag_file_io_hd_pattern_output_log = 0;
    unsigned int global_flag_file_io_hd_pattern_output_verilog = 0;
    enum HD_PATTERN_OUTPUT
    {
        FLAG_MASK_FILE_IO_HD_MES            =   (1<<0),
        FLAG_MASK_FILE_IO_HD_ENCODING_MES   =   (1<<1),
        FLAG_MASK_FILE_IO_HD_RECEIVED_MES   =   (1<<2),
        FLAG_MASK_FILE_IO_HD_CORRECTED_MES  =   (1<<3),
    };

    unsigned int global_flag_file_io_sd_pattern_output = 0;
    unsigned int global_flag_file_io_sd_pattern_output_log = 0;
    unsigned int global_flag_file_io_sd_pattern_output_verilog = 0;
    enum SD_PATTERN_OUTPUT
    {
        FLAG_MASK_FILE_IO_SD_RECEIVED_LLR       =   (1<<0),
        FLAG_MASK_FILE_IO_SD_RECEIVED_LLR_mag   =   (1<<1),
    };

    

//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
				unsigned int testOption=0;
				#define FLAG_MASK_TEST_RESULT		(1<<5)

				unsigned int enviromentsOption=0;

				
/*****************************************/				
/********** Associate with AWGN **********/
/*****************************************/				
#define VALUE_EBN0_WORD	"ebn0"//SET_VALUE
/*	double main_com_EbN0 = 1.0;*/
#define DEFAULT_VALUE_EBN0	1.0f
double global_EbN0_Start_Value = DEFAULT_VALUE_EBN0;

#define VALUE_EBN0_END_WORD	"ebn0-end"//SET_VALUE
#define DEFAULT_VALUE_EBN0_END	8.0f
double global_EbN0_End_Value = DEFAULT_VALUE_EBN0_END;

#define VALUE_EBN0_STEP_WORD	"ebn0-step"//SET_VALUE
#define DEFAULT_VALUE_EBN0_STEP	0.25f
double global_EbN0_Step	= DEFAULT_VALUE_EBN0_STEP;


unsigned int global_flag_debug_awgn=0;
#define FLAG_MASK_DEBUG_AWGN_SEQUENCE	(1<<0)
#define debugAwgnSequenceMes	printf("SEQUENCE AWGN ) ");
					
					

/********************************************************/				
/********** Associate with Log Like Hood Ratio **********/
/********************************************************/
unsigned int global_flag_llr_display=0;
enum FLAG_LLR_DISPLAY
{
	FLAG_LLR_DISPLAY_NONE						= 0,//if(global_flag_llr_display&FLAG_LLR_DISPLAY_QUANTIZATION_FILTERING)
	FLAG_LLR_DISPLAY_QUANTIZATION_FILTERING		= (1<<0),
}typedef enum_FLAG_LLR_DISPLAY;

//#define FLAG_CASE_FINDING_MIN_LLR_METHOD_NONE                       0
//#define FLAG_CASE_FINDING_MIN_LLR_METHOD_MINIMUM                    1
//#define FLAG_CASE_FINDING_MIN_LLR_METHOD_TREE_FOLLOWING_MIN1_PATH   2
//#define FLAG_CASE_FINDING_MIN_LLR_METHOD_TREE_PROBABILISIC_CHOOSEN  3//NOT SUPPORT YET
//#define FLAG_CASE_FINDING_MIN_LLR_METHOD_TREE_PRIOR_HD_ZERO         4//NOT SUPPORT YET
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


#define FLAG_CASE_FINDING_MIN_LLR_METHOD_DEFAULT	FLAG_CASE_FINDING_MIN_LLR_METHOD_TREE_FOLLOWING_MIN1_PATH
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
#define FLAG_MASK_DEBUG_AWGN_LLR_SEQUENCE (1<<0)
#define debugAwgnLlrSequenceMes	printf("SEQUENCE LLR ) ");

#define FLAG_MASK_DEBUG_AWGN_LLR_LOCATOR (1<<1)
#define debugAwgnLlrLocatorMes	printf("debug-LLR-Locator ) ");
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
double global_LLR_quantizFilter_offset			= -0.5;
double global_LLR_quantizFilter_step			= 1.0;
unsigned char global_LLR_quantizFilter_bitWidth	= 4;
/* LLR QUANTIZATION FILTER VARIABLES end */
/* LLR QUANTIZATION FILTER options end */













#define DEFAULT_MINIMUM_LOOP	10000
unsigned long global_minimum_loop = 0;
unsigned long global_base_of_loop = 0;
unsigned int global_rate_of_loop = 0;

				
#define VALUE_ERROR_NUM_WORD	"error-number"//SET_VALUE
unsigned int global_ErrorNum = 0;




unsigned int global_flag_gfCommon_display = 0;
enum FLAG_MASK_DISPLAY_GF_COMMON
{
	FLAG_MASK_DISPLAY_GF_COMMON_GENPOLY		= (1<<0),//if(global_flag_gfCommon_display&FLAG_MASK_DISPLAY_GF_COMMON_GENPOLY)
	FLAG_MASK_DISPLAY_GF_COMMON_CONJUGACIES	= (1<<2),//if(global_flag_gfCommon_display&FLAG_MASK_DISPLAY_GF_COMMON_CONJUGACIES)
	FLAG_MASK_DISPLAY_GF_COMMON_FIELD		= (1<<1),//if(global_flag_gfCommon_display&FLAG_MASK_DISPLAY_GF_COMMON_FIELD)
	FLAG_MASK_DISPLAY_GF_COMMON_PRIMEPOLY	= (1<<3),//if(global_flag_gfCommon_display&FLAG_MASK_DISPLAY_GF_COMMON_PRIMEPOLY)
	FLAG_MASK_DISPLAY_GF_COMMON_SUM_MATRIX	= (1<<4),//if(global_flag_gfCommon_display&FLAG_MASK_DISPLAY_GF_COMMON_SUM_MATRIX)
	FLAG_MASK_DISPLAY_GF_COMMON_FILE_IO		= (1<<5),//if(global_flag_gfCommon_display&FLAG_MASK_DISPLAY_GF_COMMON_FILE_IO)
};


	unsigned int global_flag_gfCommon = 0;
	#define FLAG_MASK_GF_COMMON_MAKE_GF_ALL	(1<<0)//if(global_flag_gfCommon&FLAG_MASK_GF_COMMON_MAKE_GF_ALL)


/* Code Specification start */
				#define VALUE_GF_EXP_WORD	"gf-exp"//SET_VALUE
				#define DEFAULT_VALUE_GF_EXP 10
				unsigned int global_GaloisFieldExponential = DEFAULT_VALUE_GF_EXP;

#define VALUE_CODE_LENGTH_WORD	"codelength"//SET_VALUE
#define DEFAULT_VALUE_CODE_LENGTH	1023
unsigned int global_MaximumCodeLength = 0;
unsigned int global_CodeLength = DEFAULT_VALUE_CODE_LENGTH;
unsigned int global_LimitCodeLength = 0;
/* Code Specification end */

				/* Error correction specification start */
				// #define VALUE_INFO_LENGTH_WORD	"infolength"
				// #define DEFAULT_VALUE_INFO_LENGTH	1024
				// unsigned int global_InfoLength = DEFAULT_VALUE_INFO_LENGTH;
				#define VALUE_HARD_CORRECTABLE_WORD	"hard-correctable"//SET_VALUE
				#define DEFAULT_VALUE_BCH_HARD_CORRECTABLE	4
				unsigned int global_bch_hardCorrectable = DEFAULT_VALUE_BCH_HARD_CORRECTABLE;

				#define VALUE_SOFT_CORRECTABLE_WORD "soft-correctable"//SET_VALUE
				#define DEFAULT_VALUE_BCH_SOFT_CORRECTABLE	2
				unsigned int global_bch_SoftCorrectable = DEFAULT_VALUE_BCH_SOFT_CORRECTABLE;
				/* Error correction specification end */

				#define DEFAULT_VALUE_BCH_SOFT_OVER_SYND 0
				unsigned int global_bch_Soft_OverSyndLen = DEFAULT_VALUE_BCH_SOFT_OVER_SYND;
				
				//unsigned int global_correctables[2] = {&global_bch_hardCorrectable, &global_bch_SoftCorrectable};
				
char *global_AddedRoot = NULL;


#define FLAG_MASK_CHANNEL_MODE_AWGN	(1<<0)

#define DEFAULT_CHANNEL_MODE	FLAG_MASK_CHANNEL_MODE_AWGN

unsigned int global_Channel_Mode = DEFAULT_CHANNEL_MODE;

	enum ENUM_GLO_CH_MODE_FUNC
	{
		FLAG_MASK_CHANNEL_MODE_FUNC_NONE = 0,
		FLAG_MASK_CHANNEL_MODE_FUNC_BITRATE_DISABLE = (1<<0),//if(global_flag_Channel_Mode_Func&FLAG_MASK_CHANNEL_MODE_FUNC_BITRATE_DISABLE)
		FLAG_MASK_CHANNEL_MODE_FUNC_RANDOM_SEED_DISABLE 	= (1<<1),//if(global_flag_Channel_Mode_Func&FLAG_MASK_CHANNEL_MODE_FUNC_RANDOM_SEED_DISABLE)
	}typedef ENUM_GLO_CH_MODE_FUNC;
	
	unsigned int global_flag_Channel_Mode_Func = FLAG_MASK_CHANNEL_MODE_FUNC_NONE;
				
				
				
/***************************************************************/
unsigned int global_flag_Modulation_Mode = 0;
#define FLAG_MASK_MODULATION_MODE_BPSK	(1<<0)						//if(global_flag_Modulation_Mode&FLAG_MASK_MODULATION_MODE_BPSK)
#define DEFAULT_MODULATION_MODE	FLAG_MASK_MODULATION_MODE_BPSK
	unsigned int global_flag_Modulation_Mode_BPSK_Display = 0;
	/*
	#define FLAG_MASK_DISPLAY_RESULT			(1<<0)
	#define FLAG_MASK_DISPLAY_PROGRESS		(1<<1)
	#define FLAG_MASK_DISPLAY_FUNCNAME		(1<<2)
	#define DISPLAY_DEBUG_WORD		"debug"
	#define DISPLAY_DEBUG			(1<<3)
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
    #define TMP_TEST_MODE_LOOPS 1000000UL
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
    account of tmp_bit_num_inC_ref_zero_OZ_ratio
    index 0 -> zero bit num in Codeword.
    index 1 -> one bit num in Codeword.
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
    account of tmp_bit_num_inR_ref_zero_OZ_ratio
    index 0 -> total zero bit nums are counted.
    index 1 -> zero bit nums in correct bit
    index 2 -> zero bit nums in error bit
    */
    unsigned long** tmp_bit_num_inR_ref_zero_OZ_ratio = NULL;

    /*
    account of tmp_bit_num_inR_ref_one_OZ_ratio
    index 0 -> total one bit nums are counted.
    index 1 -> one bit nums in correct bit
    index 2 -> one bit nums in error bit
    */
    unsigned long** tmp_bit_num_inR_ref_one_OZ_ratio = NULL;

    /*
    account of tmp_bit_chg_cnt_inR_OZ_ratio 
    index 0 -> total error bit transaction nums actually error bit nums.
    index 1 -> nums of error bit transacting 0 to 1.
    index 2 -> nums of error bit transacting 1 to 0.
    */
    unsigned long** tmp_bit_chg_cnt_inR_OZ_ratio = NULL;

    /*
    account of tmp_llr_mag_sum_inR_OZ_ratio
    index 0 -> summation of llr magnitude at all bit, don't care error or not.
    index 1 -> summation of llr magnitude at corrected bit.
    index 2 -> summation of llr magnitude at error bit.
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

    #define LLR_CASE_NUM 8

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
double convertStringToDouble(char *string)
{
	double tmp=0.0;
	unsigned int strLengh=0;
	unsigned int i=0;
	unsigned int dotFlag=0;
	
	unsigned char minusFlag=0;

	if(!string)
	{
		return 0.0;
	}


	strLengh=strlen(string);
	
	if(*(string+0) == '-')
	{
		minusFlag = 1;
	}
	else
	{
		minusFlag = 0;
	}
	
	
	for(i=minusFlag; i<strLengh; i++)
	{
		if(('0'<=*(string+i))&&((*(string+i))<='9'))
		{
			if(!(dotFlag))
			{
				tmp*=10.0;
				tmp+=((double)((*(string+i))-'0'));
			}
			else
			{
				tmp+=((double)((*(string+i))-'0'))*pow(0.1, ((double)(i-dotFlag)));
			}
		}
		else if(*(string+i)=='.')
		{
			dotFlag=i;
		}
		else
		{
			warningMes;
			printf("in convertStringToDouble, ");
			printf("Include Invalied charactor, value : %s.\n", string);
			return 0.0;
		}
	}
	if(minusFlag) tmp*=(-1.0);
	
	return tmp;
}
//////////////////////////////////////////////////////////////////////////////////
unsigned char convertStringToUnsignedChar(char *string)
{
	unsigned char tmp=0;
	unsigned int strLengh=0;
	unsigned int i=0;

	if(!string)
	{
		return 0;
	}
	
	strLengh=strlen(string);
	for(i=0; i<strLengh; i++)
	{
		if(('0'<=*(string+i))&&((*(string+i))<='9'))
		{
			tmp*=10;
			tmp+=((*(string+i))-'0');
		}
		else
		{
			warningMes;
			printf("in convertStringToUnsignedInt, ");
			printf("Include Invalied charactor, value : %s.\n", string);
			return 0;
		}
	}
	return tmp;
}
//////////////////////////////////////////////////////////////////////////////////
unsigned int convertStringToUnsignedInt(char *string)
{
	unsigned int tmp=0;
	unsigned int strLengh=0;
	unsigned int i=0;

	if(!string)
	{
		return 0;
	}
	
	strLengh=strlen(string);
	for(i=0; i<strLengh; i++)
	{
		if(('0'<=*(string+i))&&((*(string+i))<='9'))
		{
			tmp*=10;
			tmp+=((*(string+i))-'0');
		}
		else
		{
			warningMes;
			printf("in convertStringToUnsignedInt, ");
			printf("Include Invalied charactor, value : %s.\n", string);
			return 0;
		}
	}
	return tmp;
}
//////////////////////////////////////////////////////////////////////////////////
unsigned long convertStringToUnsignedLong(char *string)
{
	unsigned long tmp=0;
	unsigned int strLengh=0;
	unsigned int i=0;

	if(!string)
	{
		return 0;
	}
	
	strLengh=strlen(string);
	for(i=0; i<strLengh; i++)
	{
		if(('0'<=*(string+i))&&((*(string+i))<='9'))
		{
			tmp*=10UL;
			tmp+=((*(string+i))-'0');
		}
		else
		{
			warningMes;
			printf("in convertStringToUnsignedLong, ");
			printf("Include Invalied charactor, value : %s.\n", string);
			return 0;
		}
	}
	return tmp;
}//////////////////////////////////////////////////////////////////////////////////
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
	//		#pragma omp for private(i) shared(locatorArrayLength, locatorArray, p)
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
	// memset(p, '0', sizeof(char)*stringLength);
	// *(p+stringLength)=0;

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
	
	// unsigned char usingBitNumber = (unsigned char)(log((double)locatorArrayLength)/log(2.0));
	unsigned char usingBitNumber = locatorArrayLength;
	// for(i=0; i<sizeof(usingBitNumber)*8; i++)
	// {
		// if(locatorArrayLength&(1<<i)) break;
	// }
	// usingBitNumber=i;
	unsigned char bitLocator=0;
	
	#ifndef RELEASE
	if(!testPattern)
	{
		errorMes;
		printf("in temporaryFunc_generateTestPatterns_using_LLR_Locator, testPattern is NULL.\n");
		return -1;
	}
	// if(!*testPattern)
	// {
		// errorMes;
		// printf("in temporaryFunc_generateTestPatterns_using_LLR_Locator, *testPattern is NULL.\n");
		// return -1;
	// }
	#endif
	
	if(!refferStringLength)
	{
		tmpStringLength=strlen(refferString);
		refferStringLength=tmpStringLength;
	}	
	
	testPattern[0]=refferString;//printf("\n");
	
	// #ifdef USING_OPEN_MP
		// #pragma omp parallel for schedule(guided) private(i,j) shared(numsOfTestPattern, usingBitNumber, testPattern, locatorArray, refferString, refferStringLength)
	// #endif
	for(i=1; i<numsOfTestPattern; i++)//To generate Test pattern(is same with combination), index i is used to Mask. if we can select 4-case(00 01 10 11).
	{
										// printf("for(i=1; i<locatorArrayLength; i++)  i=%d\n", i);
			if((*(testPattern+i)))
			{
				free(*(testPattern+i));
				#ifndef RELEASE
				warningMes;	printf("free(*(testPattern+%d));\n", i);
				#endif
			}
			
		*(testPattern+i)=(char*)malloc(sizeof(char)*refferStringLength+1);
										// printf("*(testPattern+i)=(char*)malloc(sizeof(char)*refferStringLength+1);\n");
		#ifndef USING_OPEN_MP
		memcpy(*(testPattern+i), refferString, sizeof(char)*refferStringLength);
		#else
		#pragma omp parallel for schedule(guided) private(j) shared(refferStringLength, testPattern, i, refferString)	
		for(j=0; j<refferStringLength; j++)
		{
			*((*(testPattern+i))+j) = *(refferString+j);
		}
		#endif
										// printf("memcpy(*(testPattern+i), refferString, sizeof(char)*refferStringLength);\n");
		*((*(testPattern+i))+refferStringLength)=0;
										// printf("*((*(testPattern+i))+refferStringLength)=0;\n");
		
		//use non-binary code calculate combination of bit position, j is mean that offset locator pointer
										// printf("for(j=0; j<usingBitNumber; j++)//j is mask\n");
		// #ifdef USING_OPEN_MP
		// #pragma omp parallel for schedule(guided) private(j) shared(usingBitNumber, locatorArray, testPattern)
		// #endif
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
						warningMesShort;	printf("character is not '0' or '1'.\r\n");
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
								// printf("%s\n",((*(testPattern+i))+j));
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
typedef char char_POWER_FORM;

typedef signed int SIGNED_INT;
typedef unsigned int UNSIGNED_INT;

/*to generate elements of galois field 2, primitive polynomials*/
const unsigned int primmeNumber[]={07, 013, 023, 045, 0103, 0211, 0435, 01021, 02011};

char primitivePringBuffer[33] = {0};
//////////////////////////////////////////////////////////////////////////////////
char *convertPrimeNumberToString(char *string, unsigned int exponential, const unsigned int *primeNumberArray)
{
	int i;

	#ifndef RELEASE
	if(!string)
	{
		return NULL;
	}
	if(!string)
	{
		return NULL;
	}
	if(exponential<2)
	{
		return NULL;
	}
	#endif
	
	for(i=31; 0<=i; i--)
	{
		if(((*(primeNumberArray+exponential-2))>>i)&1)
		{
			*(string+i+1)=0;
			break;
		}
	}
	for(; 0<=i; i--)
	{
		if(((*(primeNumberArray+exponential-2))>>i)&1)
		{
			printf("%d", ((*(primeNumberArray+exponential-2))>>i)&1);
			*(string+i)='1';
		}
		else
		{
			printf("%d", ((*(primeNumberArray+exponential-2))>>i)&1);
			*(string+i)='0';
		}
	}
	printf("\n");
	return string;
}
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
#define MAXUMUM_PRIMITIVE_POLYNOMIAL_NUMBER	17
// const char_POWER_FORM *PRIMITIVE_POLYNOMIAL_1_INVERSE_INDEX		="1";
// const char_POWER_FORM *PRIMITIVE_POLYNOMIAL_2_INVERSE_INDEX		="11";

// const char_POWER_FORM *PRIMITIVE_POLYNOMIAL_4_INVERSE_INDEX		="111";
// const char_POWER_FORM *PRIMITIVE_POLYNOMIAL_8_INVERSE_INDEX		="1101";
// const char_POWER_FORM *PRIMITIVE_POLYNOMIAL_16_INVERSE_INDEX	="11001";
// const char_POWER_FORM *PRIMITIVE_POLYNOMIAL_32_INVERSE_INDEX	="101001";

// const char_POWER_FORM *PRIMITIVE_POLYNOMIAL_64_INVERSE_INDEX	="1100001";
// const char_POWER_FORM *PRIMITIVE_POLYNOMIAL_128_INVERSE_INDEX	="11000001";
// const char_POWER_FORM *PRIMITIVE_POLYNOMIAL_256_INVERSE_INDEX	="101110001";
// const char_POWER_FORM *PRIMITIVE_POLYNOMIAL_512_INVERSE_INDEX	="1100000001";

// const char_POWER_FORM *PRIMITIVE_POLYNOMIAL_1024_INVERSE_INDEX	="10010000001";
// const char_POWER_FORM *PRIMITIVE_POLYNOMIAL_2048_INVERSE_INDEX	="101000000001";

char_POWER_FORM *PRIMITIVE_POLYNOMIAL_1_INVERSE_INDEX		="1";
char_POWER_FORM *PRIMITIVE_POLYNOMIAL_2_INVERSE_INDEX		="11";

char_POWER_FORM *PRIMITIVE_POLYNOMIAL_4_INVERSE_INDEX		="111";
char_POWER_FORM *PRIMITIVE_POLYNOMIAL_8_INVERSE_INDEX		="1011";
char_POWER_FORM *PRIMITIVE_POLYNOMIAL_16_INVERSE_INDEX	="10011";
char_POWER_FORM *PRIMITIVE_POLYNOMIAL_32_INVERSE_INDEX	="100101";

//const char_POWER_FORM *PRIMITIVE_POLYNOMIAL_64_INVERSE_INDEX	="1000011";
char_POWER_FORM *PRIMITIVE_POLYNOMIAL_64_INVERSE_INDEX	="1000011";
char_POWER_FORM *PRIMITIVE_POLYNOMIAL_128_INVERSE_INDEX	="10001001";
char_POWER_FORM *PRIMITIVE_POLYNOMIAL_256_INVERSE_INDEX	="100011101";
char_POWER_FORM *PRIMITIVE_POLYNOMIAL_512_INVERSE_INDEX	="1000010001";

char_POWER_FORM *PRIMITIVE_POLYNOMIAL_1024_INVERSE_INDEX	="10000001001";

char_POWER_FORM *PRIMITIVE_POLYNOMIAL_2048_INVERSE_INDEX	="100000000101";

char_POWER_FORM *PRIMITIVE_POLYNOMIAL_4096_INVERSE_INDEX	="1000001010011";

char_POWER_FORM *PRIMITIVE_POLYNOMIAL_8192_INVERSE_INDEX	="10000000011011";

//char_POWER_FORM *PRIMITIVE_POLYNOMIAL_16384_INVERSE_INDEX	="100000000100001";
char_POWER_FORM *PRIMITIVE_POLYNOMIAL_16384_INVERSE_INDEX	="100010001000011";
char_POWER_FORM *PRIMITIVE_POLYNOMIAL_32768_INVERSE_INDEX	="1000000000000011";
char_POWER_FORM *PRIMITIVE_POLYNOMIAL_65536_INVERSE_INDEX ="10001000000001011";
//////////////////////////////////////////////////////////////////////////////////
char *global_addedRootToLCM_string=NULL;
unsigned int global_degAddedRootToLCM=0;
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
struct struct_primitivePolynomialSet
{
	char_POWER_FORM **PRIMITIVE_POLYNOMIAL;
	unsigned int PRIMITIVE_POLYNOMIAL_NUMBER;
}typedef struct_primitivePolynomialSet;
//////////////////////////////////////////////////////////////////////////////////
struct_primitivePolynomialSet PRIMITIVE_POLYNOMIAL_SET_INVERSED;


struct_primitivePolynomialSet PRIMITIVE_POLYNOMIAL_SET;

void initPrimitivePolynomial(struct_primitivePolynomialSet *p)
{
	p->PRIMITIVE_POLYNOMIAL_NUMBER=MAXUMUM_PRIMITIVE_POLYNOMIAL_NUMBER;
	p->PRIMITIVE_POLYNOMIAL=(char_POWER_FORM**)malloc(sizeof(const char_POWER_FORM*)*(p->PRIMITIVE_POLYNOMIAL_NUMBER));

	p->PRIMITIVE_POLYNOMIAL[0]=PRIMITIVE_POLYNOMIAL_1_INVERSE_INDEX;
	p->PRIMITIVE_POLYNOMIAL[1]=PRIMITIVE_POLYNOMIAL_2_INVERSE_INDEX;
	p->PRIMITIVE_POLYNOMIAL[2]=PRIMITIVE_POLYNOMIAL_4_INVERSE_INDEX;
	p->PRIMITIVE_POLYNOMIAL[3]=PRIMITIVE_POLYNOMIAL_8_INVERSE_INDEX;
	p->PRIMITIVE_POLYNOMIAL[4]=PRIMITIVE_POLYNOMIAL_16_INVERSE_INDEX;
	p->PRIMITIVE_POLYNOMIAL[5]=PRIMITIVE_POLYNOMIAL_32_INVERSE_INDEX;
	p->PRIMITIVE_POLYNOMIAL[6]=PRIMITIVE_POLYNOMIAL_64_INVERSE_INDEX;
	p->PRIMITIVE_POLYNOMIAL[7]=PRIMITIVE_POLYNOMIAL_128_INVERSE_INDEX;
	p->PRIMITIVE_POLYNOMIAL[8]=PRIMITIVE_POLYNOMIAL_256_INVERSE_INDEX;
	p->PRIMITIVE_POLYNOMIAL[9]=PRIMITIVE_POLYNOMIAL_512_INVERSE_INDEX;
	
	p->PRIMITIVE_POLYNOMIAL[10]=PRIMITIVE_POLYNOMIAL_1024_INVERSE_INDEX;
	p->PRIMITIVE_POLYNOMIAL[11]=PRIMITIVE_POLYNOMIAL_2048_INVERSE_INDEX;

	p->PRIMITIVE_POLYNOMIAL[12]=PRIMITIVE_POLYNOMIAL_4096_INVERSE_INDEX;
	p->PRIMITIVE_POLYNOMIAL[13]=PRIMITIVE_POLYNOMIAL_8192_INVERSE_INDEX;
	p->PRIMITIVE_POLYNOMIAL[14]=PRIMITIVE_POLYNOMIAL_16384_INVERSE_INDEX;
	p->PRIMITIVE_POLYNOMIAL[15]=PRIMITIVE_POLYNOMIAL_32768_INVERSE_INDEX;
	p->PRIMITIVE_POLYNOMIAL[16]=PRIMITIVE_POLYNOMIAL_65536_INVERSE_INDEX;
	
	
}
void initPrimitivePolynomial_inversed(struct_primitivePolynomialSet *inversed, struct_primitivePolynomialSet *non_inversed)
{
	unsigned int i;
	int j;
	unsigned int bitLoc;
	unsigned int polyLength;

	inversed->PRIMITIVE_POLYNOMIAL=(char_POWER_FORM**)malloc(sizeof(char_POWER_FORM*)*(non_inversed->PRIMITIVE_POLYNOMIAL_NUMBER));
	for(i=0; i<non_inversed->PRIMITIVE_POLYNOMIAL_NUMBER; i++)
	{
		polyLength=strlen(non_inversed->PRIMITIVE_POLYNOMIAL[i]);
		inversed->PRIMITIVE_POLYNOMIAL[i]=(char_POWER_FORM*)malloc((sizeof(char_POWER_FORM)*polyLength)+1);
		*(((char_POWER_FORM*)inversed->PRIMITIVE_POLYNOMIAL[i])+polyLength)=0;

		bitLoc=0;
		for(j=(polyLength-1); j>-1; j--)
		{
			if(*(non_inversed->PRIMITIVE_POLYNOMIAL[i]+j)=='1') *(((char_POWER_FORM*)inversed->PRIMITIVE_POLYNOMIAL[i])+bitLoc)='1';
			else if(*(non_inversed->PRIMITIVE_POLYNOMIAL[i]+j)=='0') *(((char_POWER_FORM*)inversed->PRIMITIVE_POLYNOMIAL[i])+bitLoc)='0';
			else{ errorMes; printf("detected!!! ilegal word, 0x%02x, %c", *(non_inversed->PRIMITIVE_POLYNOMIAL[i]+j), *(non_inversed->PRIMITIVE_POLYNOMIAL[i]+j)); return; }
			bitLoc++;
		}
		
	}
	
	inversed->PRIMITIVE_POLYNOMIAL_NUMBER=non_inversed->PRIMITIVE_POLYNOMIAL_NUMBER;
}
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
signed long intPower(int radix, int exponent)
{
	signed long buffer = 1;
	for(;exponent;exponent--)
	{
		buffer*=radix;
	}
	return buffer;
}
//////////////////////////////////////////////////////////////////////////////////
void printPowerFormWithEnter(char_POWER_FORM *p)
{
	unsigned int length = strlen(p);
	unsigned int i;
	
	printf("power form polynomial length is %d\n", length);
	
	printf("\"");
	for(i=0; i<length; i++)
	{
		if(*(p+i)=='0')
		{
			printf("%c", *(p+i));
		}
		else if(*(p+i)=='1')
		{
			printf("%c", *(p+i));
		}
		else
		{
			errorMes;
			printf("%c\" power form polynomial coefficient values are must '0', '1'\n", *(p+i));
			return;
		}
	}
	printf("\"\n");
	
	for(i=0; i<length; i++)
	{
		if(*(p+i)=='0')
		{
			continue;
		}
		else if(*(p+i)=='1')
		{
			printf("a^%d\t", i);
		}
		else
		{
			errorMes;
			printf("power form polynomial coefficient values are must '0', '1'\n");
			return;
		}
	}
	printf("\n");
}
//////////////////////////////////////////////////////////////////////////////////
struct struct_powerFormPolynomials/*power form poly is mean that bit stream like 0100011000100.*/
{
	//unsigned int *numberOfelement;
	unsigned int length;
	unsigned int usedLength;
	char_POWER_FORM *equation;
}typedef struct_powerFormPolynomials;
//////////////////////////////////////////////////////////////////////////////////
/* bits is mean that length of bits */
struct_powerFormPolynomials *createPowerFormPolynomial(unsigned int length)
{
	unsigned int i;

	struct_powerFormPolynomials *p;

	#ifndef RELEASE
	if(!length)
	{
		errorMes;
		printf("length of polynomial is %d.\n",length);
		return NULL;
	}
	#endif
	
	p=(struct_powerFormPolynomials*)malloc(sizeof(struct_powerFormPolynomials));
	memset(p, 0, sizeof(struct_powerFormPolynomials));

	//p->numberOfelement=&(p->length);
	p->length=length;

	p->usedLength=length;
	
	p->equation=(char_POWER_FORM*)malloc(sizeof(char_POWER_FORM)*(length+1));//+1 is to null
	memset(p->equation, '0', sizeof(char_POWER_FORM)*(length));
	*(p->equation+length)=0;
	
	/*quantization setting. default setting is zero.*/
	// p->llrMask=0;
	// p->llr=NULL;
	
	return p;
}
//////////////////////////////////////////////////////////////////////////////////
struct_powerFormPolynomials *createPowerFormPolynomialUsingString(char *string, unsigned int strLength)
{
	unsigned int i;

	struct_powerFormPolynomials *p;

	#ifndef RELEASE
	if(!string)
	{
		errorMes;
		printf("in createPowerFormPolynomialUsingString, string is NULL.\n");
		return NULL;
	}
	#endif
	
	if(!strLength)
	{
		strLength=strlen(string);
	}
	
	p = createPowerFormPolynomial(strLength);
	
/* 	memcpy(p->equation, string, strLength); */
	#ifdef USING_OPEN_MP
	#pragma omp parallel for schedule(guided) private(i) shared(strLength, string, p)
	#endif
	for(i=0; i<strLength; i++)
	{
		if((*(string+i)!='0')&&(*(string+i)!='1'))
		{
			#ifndef RELEASE
			errorMes; printf("in createPowerFormPolynomialUsingString, string have another character(can use only '0' and '1').\niuputString : %d-th '%c'\n", i, *(string+i));
			#endif
			
			#ifdef USING_OPEN_MP
			if(*(p->equation+i)%2)
			{
				*(p->equation+i)='1';
			}
			else
			{
				*(p->equation+i)='0';
			}
			#else
			if(rand()%2)
			{
				*(p->equation+i)='1';
			}
			else
			{
				*(p->equation+i)='0';
			}
			#endif
		}
		else
		{
			*(p->equation+i)=*(string+i);
		}
	}
	*(p->equation+strLength)=0;
	
	return p;
}
//////////////////////////////////////////////////////////////////////////////////
char closePowerFormPolynomial(struct_powerFormPolynomials **p)
{
	#ifndef RELEASE
	if(!p)
	{
		warningMes;
		printf("in closePowerFormPolynomial, struct_powerFormPolynomials **p has NULL;\n");
		printf("it is already removed.\n");
		return 0;
	}
	if(!(*p))
	{
		warningMes;
		printf("in closePowerFormPolynomial, struct_powerFormPolynomials *p is already removed.\n",(*p));
		printf("it is already removed.\n");
		return -1;
	}
	#endif
	
	if((*p)->equation)
	{
		free((*p)->equation);
	}
	// if((*p)->llr!=NULL)
	// {
		// free((*p)->llr);
	// }
	
	free((*p));
	*p=NULL;
	return 0;
}
//////////////////////////////////////////////////////////////////////////////////
struct_powerFormPolynomials *recreatePowerFormPolynomial(struct_powerFormPolynomials **p, unsigned int length)
{
	if(*p)
	{
		#ifndef RELEASE
		if((global_flag_cmdOption&FLAG_MASK_PRINTF_LOG))
		{
			printf("in recreatePowerFormPolynomial, struct_powerFormPolynomials *p is already have data.\n");
			printf("*p will be closePowerFormPolynomial(p).\n");
		}
		#endif
		
		closePowerFormPolynomial(p);
	}
	*p=createPowerFormPolynomial(length);
	return *p;
}
//////////////////////////////////////////////////////////////////////////////////
char setPowerFormPolynomialUsingStringAddr(struct_powerFormPolynomials *p, char *string, unsigned int usedLength, unsigned int length)
{
	unsigned int i;
	unsigned int strLength;
	
	unsigned int tmp_length=0;
	unsigned int tmp_usedLength=0;
	char_POWER_FORM *tmp_char_powerForm=NULL;
	
	#ifndef RELEASE
	if(!p)
	{
		errorMes;
		printf("in setPowerFormPolynomialUsingStringAddr, struct_powerFormPolynomials p is NULL\n");
		return -1;
	}
	
	if(!string)
	{
		errorMes;
		printf("in setPowerFormPolynomialUsingStringAddr, not exist string.\n");
		return -1;
	}
	#endif
	
	#ifndef RELEASE
	if(!usedLength)
	{
		strLength=strlen(string);
		usedLength=strLength;
		if(!strLength)
		{
			/*no length infomation at all*/
			errorMes;
			printf("in setPowerFormPolynomialUsingStringAddr, string length(%d) is ZERO.\n", usedLength);
			return -1;
		}
		else if(!length)
		{
			warningMes; printf("in setPowerFormPolynomialUsingStringAddr, unsigned int  length is ZERO, p->length can set to strlen(string).\n");
			warningMesShort; printf("So simulator can be broken, when operate with different length of p->equation each other.\n");
			length=strLength;
		}
	}
	#endif
	
	#ifndef RELEASE
	if(length<usedLength)
	{
		errorMes;
		printf("in setPowerFormPolynomialUsingStringAddr, length < usedLength.\n");
		return -1;
	}
	#endif
	
	#ifndef RELEASE
	if(!(p->length))
	{
		warningMes; printf("in setPowerFormPolynomialUsingStringAddr, p->length is ZERO.\n");
		warningMesShort; printf("p->length can be over wroten to arg of length\n"); 
	}
	else
	{
		if(p->length != length)
		{
			warningMes; printf("in setPowerFormPolynomialUsingStringAddr, p->length != length.\n");
			warningMesShort; printf("p->length is will over wroten to arg of length.\n");
			warningMesShort; printf("Simulation can be broken.\n");
		}
	}
	if(!(p->usedLength))
	{
		warningMes; printf("in setPowerFormPolynomialUsingStringAddr, p->usedLength is ZERO.\n");
		warningMesShort; printf("p->usedLength can be over wroten to arg of usedLength\n"); 
	}
	else
	{
		if(p->usedLength != usedLength)
		{
			warningMes; printf("in setPowerFormPolynomialUsingStringAddr, p->usedLength != usedLength.\n");
			warningMesShort; printf("p->usedLength is will over wroten to arg of usedLength.\n");
			warningMesShort; printf("Simulation can be broken.\n");
		}
	}
	if(!(p->equation))
	{
		/*dont care case*/
		warningMes; printf("in setPowerFormPolynomialUsingStringAddr, p->equation(0x%x) is not NULL.\n", p->equation);
	}
	#endif

	
	/* backuped */
	tmp_length=p->length;
	tmp_usedLength=p->usedLength;
	tmp_char_powerForm=p->equation;
	
	p->length=length;
	p->usedLength=usedLength;
	p->equation=((char_POWER_FORM*)string);

			#ifndef RELEASE
			if((global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)==FLAG_MASK_PRINTF_LOG)
			{
				logMes;
				printf("in copyPowerFormPolynomialFromString, previous value is...\n");
				printPowerFormWithEnter(tmp_char_powerForm);
				printf("And replaced value is...\n");
				printPowerFormWithEnter(p->equation);
				printf("And buffered Length is \'%d\', used length is \'%d\'\n", p->length, p->usedLength);
				printf("Previous value will be deleted.\n");
			}
			#endif
	
	if(tmp_char_powerForm)
	{
		free(tmp_char_powerForm);
	}
	
	return 0;
}

char setPowerFormPolynomialUsingStringAddr_clearStringPointer(struct_powerFormPolynomials *p, char **string, unsigned int usedLength, unsigned int length)
{
	if(!string)
	{
		errorMes; printf("in setPowerFormPolynomialUsingStringAddr_clearStringPointer, char **string is NULL.\n");
		return -1;
	}
	if(setPowerFormPolynomialUsingStringAddr(p, *string, usedLength, length))
	{
		errorMes; printf("in setPowerFormPolynomialUsingStringAddr_clearStringPointer, setPowerFormPolynomialUsingStringAddr return to not ZERO.\n");
		return -1;
	}
	*string = NULL;
	
	return 0;
}
//////////////////////////////////////////////////////////////////////////////////
char copyPowerFormPolynomialFromString(struct_powerFormPolynomials *p, char *string)
{
	unsigned int i, strLength;
	char *backupP;

	#ifndef RELEASE
	if(!string)
	{
		errorMes;
		printf("in copyPowerFormPolynomialFromString, not exist string.\n");
		return -1;
	}
	#endif
	
	strLength=strlen(string);
	if(strLength>p->length)
	{
		errorMes;
		printf("in copyPowerFormPolynomialFromString, string length(%d) is have to shorter than power form polynomial length(%d).\n", strLength, p->length);
		return -1;
	}
	#ifndef RELEASE
	if(!(p->equation))
	{
		errorMes;
		printf("in copyPowerFormPolynomialFromString, p->equation(0x%x) is not defined.\n", p->equation);
		return -1;
	}
	#endif
	
	backupP = (char*)malloc(sizeof(char)*(p->length+1));
	#ifndef USING_OPEN_MP
	memcpy(backupP, p->equation, sizeof(sizeof(char)*(p->usedLength+1)));
	#else
	#pragma omp parallel for schedule(guided) private(i) shared(p, backupP)
	for(i=0; i<p->usedLength; i++)
	{
		*(backupP+i) = *(p->equation+i);
	}
	#endif
	*(backupP+p->usedLength)=0;
	
	#ifndef USING_OPEN_MP
	memcpy(p->equation, string, sizeof(char)*(strLength+1));
	#else
	#pragma omp parallel for schedule(guided) private(i) shared(p, string, strLength)
	for(i=0; i<strLength; i++)
	{
		*(p->equation+i) = *(string+i);
	}
	#endif
	*(p->equation+strLength)=0;
	
			#ifndef RELEASE
			for(i=0; i<strLength; i++)
			{
				if((*(string+i)!='0')&&(*(string+i)!='1'))
				{
					errorMes;
					printf("in copyPowerFormPolynomialFromString, string have another character(can use only '0' and '1').\niuputString : %s\n", string);
					printf("Can't copy input string \"%s\" to struct_powerFormPolynomials *p->equation.\n", string);
					printf("p->length is \'%d\', strLength is \'%d\', loops num is \'%d\', input character is \'%c(%d)\'.\n", p->length, strLength, i, *(string+i), *(string+i));

					// if(p->equation)
					// {
						free(p->equation);
					// }
					p->equation=backupP;
					return -1;
				}
			}
			#endif
	/*insert NULL*/

	p->usedLength=strLength;
			#ifndef RELEASE
			if((global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)==FLAG_MASK_PRINTF_LOG)
			{
				logMes;
				printf("in copyPowerFormPolynomialFromString, setted power form equation is...\n");
				printPowerFormWithEnter(p->equation);
			}
			#endif
	
	free(backupP);
	return 0;
}
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
/* struct_powerFormPolynomials *createPowerFormPolynomialAndInputValue(char *string)
{
	struct_powerFormPolynomials *p = createPowerFormPolynomial(strlen(string));
	if((global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)==FLAG_MASK_PRINTF_LOG)
	{
		logMes;
		printf("in createPowerFormPolynomialAndInputValue, CreationPowerFormPolynomial. input String is : \"%s\"\n", string);
	}
	if(copyPowerFormPolynomialFromString(p, string))
	{
		//closePowerFormPolynomial(p);
		errorMes;
		printf("in createPowerFormPolynomialAndInputValue, copyPowerFormPolynomialFromString(p, string).\n");
		return NULL;
	}
	return p;
} */
struct_powerFormPolynomials *recreatePowerFormPolynomialUsingString(struct_powerFormPolynomials **p, char *string, unsigned int strLength)
{
	#ifndef RELEASE
	if(*p)
	{
		if((global_flag_cmdOption&FLAG_MASK_PRINTF_LOG))
		{
			printf("in recreatePowerFormPolynomialUsingString, struct_powerFormPolynomials *p is already have data.\n");
			printf("it is cleared and recreate.\n");
		}
		closePowerFormPolynomial(p);
	}
	#endif
	
	
	if(!strLength) strLength = strlen(string);
	
	*p=createPowerFormPolynomialUsingString(string, strLength);
	return *p;
}
//////////////////////////////////////////////////////////////////////////////////
void shiftHighSidePowerFormPolynomial(struct_powerFormPolynomials *p, unsigned int length)
{
	char_POWER_FORM *buffer;

	#ifndef RELEASE
	if(!p)
	{
		errorMes;
		printf("in shiftHighSidePowerFormPolynomial, struct_powerFormPolynomials *p is NULL.\n");
		return;
	}
	if(!length)
	{
		return;
	}
	if(!p->length)
	{
		infoMes;
		printf("in shiftHighSidePowerFormPolynomial, p->length is zero.\n");
		return;
	}
	if(p->length<length)
	{
		infoMes;
		printf("in shiftHighSidePowerFormPolynomial, shift length(%d) is can not exceed p->length(%d).\n", length, p->length);
		return;
	}

	if(p->usedLength<length)
	{
		warningMes;
		printf("in shiftHighSidePowerFormPolynomial, used length(%d) is shortter than shifted length.\n");
		printf("Result can have incorrected value.\n");
		printf("Because shifted string will be cuf off at \'%d\'-th value\n", p->usedLength+1);
	}
	if((global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)==FLAG_MASK_PRINTF_LOG)
	{
		logMes;
		printf("in shiftHighSidePowerFormPolynomial, Shift length is \'%d\'.\n", length);
		printf("struct_powerFormPolynomials p->length is \'%d\'.\n", p->length);
		printf("struct_powerFormPolynomials p->usedLength is \'%d\'.\n", p->usedLength);
		printf("non-shifted codeWord is\n\"%s\".\n", p->equation);
	}
	#endif
	
	buffer=(char_POWER_FORM*)malloc(sizeof(char_POWER_FORM)*(p->usedLength-length));
	memcpy(buffer, p->equation, sizeof(char_POWER_FORM)*(p->usedLength-length));
	memset(p->equation, '0', sizeof(char_POWER_FORM)*length);
	memcpy(p->equation+length, buffer, sizeof(char_POWER_FORM)*(p->usedLength-length));
	*(p->equation+p->usedLength)=0;//insert NULL
	
			#ifndef RELEASE
			if((global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)==FLAG_MASK_PRINTF_LOG)
			{
				logMes;
				printf("in shiftHighSidePowerFormPolynomial, ");
				printf("Shift codeWord\n\"%s\".\n", p->equation);
			}
			#endif
	free(buffer);
}
//////////////////////////////////////////////////////////////////////////////////
void shiftLowSidePowerFormPolynomial(struct_powerFormPolynomials *p, unsigned int length)
{
	return;
}
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
unsigned int calculateHammingWeightPowerFormPolynomial(struct_powerFormPolynomials *p)
{
	unsigned int i;
	unsigned int result=0;
	if(!p) return 0;
	if(!p->equation) return 0;
	if(!p->usedLength) return 0;
	
	#ifdef USING_OPEN_MP
	#pragma omp parallel for schedule(guided) private(i) shared(p) reduction(+:result)
	#endif
	for(i=0; i<(p->usedLength); i++)
	{
		if(*(p->equation+i)=='1') result++;
	}
	return result;
}
//////////////////////////////////////////////////////////////////////////////////
unsigned int calculateHammingWeightFromDiffentPowerFormPolynomial(struct_powerFormPolynomials *p1, struct_powerFormPolynomials *p2)
{
	unsigned int i;
	unsigned int result=0;
	
	#ifndef RELEASE
	if(!p1)
	{
		errorMes;
		printf("in calculateHammingWeightFromDiffentPowerFormPolynomial, struct_powerFormPolynomials p1.\n");
		return -1;
	}
	if(!p2)
	{
		errorMes;
		printf("in calculateHammingWeightFromDiffentPowerFormPolynomial, struct_powerFormPolynomials p2.\n");
		return -1;
	}
	if(p1->usedLength!=p2->usedLength)
	{
		errorMes;
		printf("in calculateHammingWeightFromDiffentPowerFormPolynomial, p1->usedLength!=p2->usedLength, can't comparing.\n");
		return -1;
	}
	#endif
	
	#ifdef USING_OPEN_MP
	#pragma omp parallel for schedule(guided) private(i) shared(p1, p2) reduction(+:result)
	#endif
	for(i=0; i<p1->usedLength; i++)
	{
		// if(((*(p1->equation+i))=='0'))
		// {
			// if(((*(p2->equation+i))!='0')) result++;
		// }
		// else if(((*(p1->equation+i))=='1'))
		// {
			// if(((*(p2->equation+i))!='1')) result++;
		// }
		if((*(p1->equation+i))!=(*(p2->equation+i))) result++;
	}
	return result;
}
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
void printPowerFormWithEnterPolynomialAndInfoUsingAddress(struct_powerFormPolynomials *p)
{
	if(!p)
	{
		errorMes;
		printf("in printPowerFormWithEnterPolynomialAndInfoUsingAddress, struct_powerFormPolynomials *p\n");
		return;
	}
	
	printf("Stored length is \'%d\' and used length of struct_powerFormPolynomials is \'%d\'\n", p->length, p->usedLength);
	printf("value : \"");
	printf("%s", p->equation);
	printf("\"\n");
}
//////////////////////////////////////////////////////////////////////////////////
void printPowerFormWithEnterPolynomialWithEnterUsingAddress(struct_powerFormPolynomials *p)
{
	unsigned int i;
	
	unsigned int even_bit;
	unsigned int odd_bit;
	unsigned int all_bit;
	if(!p)
	{
		errorMes;
		printf("in printPowerFormWithEnterPolynomialWithEnterUsingAddress, struct_powerFormPolynomials *p\n");
		return;
	}
	
	//printf("%s", p->equation);
	even_bit = 0;
	odd_bit = 0;
	all_bit = 0;
	for(i=0; i<p->usedLength; i++)
	{
		printf("%c", *(p->equation+i));
		if(i%2)//odd
		{
			if(*(p->equation+i) == '0')
			{
				odd_bit ^= 0;
				all_bit ^= 0;
			}
			else if(*(p->equation+i) == '1')
			{
				odd_bit ^= 1;				
				all_bit ^= 1;
			}
			else
			{
				errorMes;
				printf("character is not '0' and '1'\n");
			}
		}
		else//even
		{
			if(*(p->equation+i) == '0')
			{
				even_bit ^= 0;
				all_bit ^= 0;
			}
			else if(*(p->equation+i) == '1')
			{
				even_bit ^= 1;				
				all_bit ^= 1;
			}
			else
			{
				errorMes;
				printf("character is not '0' and '1'\n");
			}
		}
		
	}
	printf("[even]:%d",even_bit);
	printf("[odd]:%d",odd_bit);
	printf("[all]:%d",all_bit);
	printf("[used]:%d", p->usedLength);	
	printf("[strLen]:%d", strlen(p->equation));
	printf("[full]:%d\n", p->length);
}
//////////////////////////////////////////////////////////////////////////////////
void printPowerFormWithEnterPolynomialWithTapUsingAddress(struct_powerFormPolynomials *p)
{
	if(!p)
	{
		errorMes;
		printf("in printPowerFormWithEnterPolynomialWithTapUsingAddress, struct_powerFormPolynomials *p\n");
		return;
	}
	
	printf("%s\t", p->equation);
}
//////////////////////////////////////////////////////////////////////////////////
void printPowerFormExclusivedUsingAddress(struct_powerFormPolynomials *p1, struct_powerFormPolynomials *p2)
{
	unsigned int i;
	
	if(!p1)
	{
		errorMes;
		printf("in printPowerFormExclusivedUsingAddress, struct_powerFormPolynomials p1\n");
		return;
	}
	if(!p2)
	{
		errorMes;
		printf("in printPowerFormExclusivedUsingAddress, struct_powerFormPolynomials p2\n");
		return;
	}
	if(p1->usedLength!=p2->usedLength)
	{
		errorMes;
		printf("in printPowerFormExclusivedUsingAddress, p1->usedLength!=p2->usedLength\n");
		return;
	}
	

	for(i=0; i<p1->usedLength; i++)
	{
		if(((*(p1->equation+i))=='0'))
		{
			if(((*(p2->equation+i))=='0'))
			{
				printf("0");
			}
			else if(((*(p2->equation+i))=='1'))
			{
				printf("1");
			}
			else
			{
				printf("X");
			}
		}
		else if(((*(p1->equation+i))=='1'))
		{
			if(((*(p2->equation+i))=='0'))
			{
				printf("1");
			}
			else if(((*(p2->equation+i))=='1'))
			{
				printf("0");
			}
			else
			{
				printf("X");
			}
		}
		else
		{
			printf("X");
		}
	
	}
	printf("[used]:%d", p1->usedLength);
	printf("[full]:%d,%d\n", p1->length, p2->length);
	// printf("\n");
}
//////////////////////////////////////////////////////////////////////////////////
void fprintPowerFormUsingAddress(FILE *fp, struct_powerFormPolynomials *p, char* str_tail)
{
    #ifndef RELEASE
        if(!fp)
        {
		    errorMes;
		    printf("in fprintPowerFormWithEnterPolynomialWithTapUsingAddress, FILE *fp\n");
		    return;
        }
	    if(!p)
	    {
		    errorMes;
		    printf("in fprintPowerFormWithEnterPolynomialWithTapUsingAddress, struct_powerFormPolynomials *p\n");
		    return;
	    }
    #endif

	fprintf(fp, "%s", p->equation);
    if(str_tail) fprintf(fp, str_tail);
}
//////////////////////////////////////////////////////////////////////////////////
void fprintVerilogPowerFormUsingAddress(FILE *fp, struct_powerFormPolynomials *p, char* str_tail)
{
    unsigned int i;
    #ifndef RELEASE
        if(!fp)
        {
		    errorMes;
		    printf("in fprintPowerFormWithEnterPolynomialWithTapUsingAddress, FILE *fp\n");
		    return;
        }
	    if(!p)
	    {
		    errorMes;
		    printf("in fprintPowerFormWithEnterPolynomialWithTapUsingAddress, struct_powerFormPolynomials *p\n");
		    return;
	    }
    #endif

    for(i=0; i<p->usedLength; i++)
    {
	    fprintf(fp, "%c\r\n", p->equation[i]);
    }
    if(str_tail) fprintf(fp, str_tail);
}
//////////////////////////////////////////////////////////////////////////////////
void fprintPowerFormFullDescriptionUsingAddresss(FILE *fp, struct_powerFormPolynomials *p, char *str_tail)
{
	unsigned int i;
	
	unsigned int even_bit;
	unsigned int odd_bit;
	unsigned int all_bit;
    unsigned int illegal_bit;

    #ifndef RELEASE
	if(!p)
	{
	    errorMes;
	    printf("in printPowerFormWithEnterPolynomialWithEnterUsingAddress, struct_powerFormPolynomials *p\n");
	    return;
	}
	
	if(!fp)
	{
	    errorMes;
	    printf("in printPowerFormWithEnterPolynomialWithEnterUsingAddress, FILE *fp\n");
	    return;
	}
    #endif
	
	//printf("%s", p->equation);
	even_bit = 0;
	odd_bit = 0;
	all_bit = 0;
    illegal_bit = 0;
	for(i=0; i<p->usedLength; i++)
	{
		fprintf(fp, "%c", *(p->equation+i));
		if(i%2)//odd
		{
			if(*(p->equation+i) == '0')
			{
				odd_bit ^= 0;
				all_bit ^= 0;
			}
			else if(*(p->equation+i) == '1')
			{
				odd_bit ^= 1;				
				all_bit ^= 1;
			}
			else
			{
                illegal_bit++;
			}
		}
		else//even
		{
			if(*(p->equation+i) == '0')
			{
				even_bit ^= 0;
				all_bit ^= 0;
			}
			else if(*(p->equation+i) == '1')
			{
				even_bit ^= 1;				
				all_bit ^= 1;
			}
			else
			{
                illegal_bit++;
			}
		}
		
	}
	fprintf(fp, "[even]:%d",even_bit);
	fprintf(fp, "[odd]:%d",odd_bit);
	fprintf(fp, "[all]:%d",all_bit);
	fprintf(fp, "[used]:%d", p->usedLength);	
	fprintf(fp, "[strLen]:%d", strlen(p->equation));
	fprintf(fp, "[full]:%d", p->length);
    fprintf(fp, "[illegal]:%d", illegal_bit);

    if(str_tail) fprintf(fp, str_tail);
}
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
/********** FILE IO STRUCTURE **********/
struct struct_variableSetConvertBitStreamToPowerForm
{
	unsigned char *data;

	unsigned char dataBitLength;
	
	unsigned int codeWordPerDataNumber;
	
	unsigned long totalCodeWordNumber;
	unsigned long countedCodeWordNumber;
	
	unsigned long totalDataNumber;
	unsigned long countedDataNumber;

}typedef struct_variableSetConvertBitStreamToPowerForm;
//////////////////////////////////////////////////////////////////////////////////
char setVariableSetConvertBitStreamToPowerForm(struct_variableSetConvertBitStreamToPowerForm *p, unsigned char *data, unsigned int infoBitLength, unsigned char dataBytes, unsigned long arrayIndex)
{
	#ifndef RELEASE
	if(!p)
	{
		return -1;
	}
	if(!infoBitLength)
	{
		return -1;
	}
	if(!dataBytes)
	{
		return -1;
	}
	if(!arrayIndex)
	{
		return -1;
	}
	if(!data)
	{
		return -1;
	}
	#endif
	
	p->data=data;
	
	p->dataBitLength=dataBytes*8;
	p->codeWordPerDataNumber=infoBitLength/(p->dataBitLength);
	if(p->codeWordPerDataNumber)//infoBitLength>(p->dataBitLength)
	{
		p->totalCodeWordNumber=arrayIndex/(p->codeWordPerDataNumber);
		if(arrayIndex%(p->codeWordPerDataNumber))
		{
			p->totalCodeWordNumber++;
		}
	}
	else
	{
		p->totalCodeWordNumber=0;
	}
	p->totalDataNumber=arrayIndex;
	
	p->countedCodeWordNumber=0;
	
	p->countedDataNumber=0;
	
	return 0;
}
//////////////////////////////////////////////////////////////////////////////////
struct_variableSetConvertBitStreamToPowerForm *createVariableSetConvertBitStreamToPowerForm(struct_variableSetConvertBitStreamToPowerForm **p)
{
	if(!p)
	{
		return NULL;
	}
	else if(!(*p))
	{
		free(*p);
	}
	(*p)=(struct_variableSetConvertBitStreamToPowerForm*)malloc(sizeof(struct_variableSetConvertBitStreamToPowerForm));
	memset(*p, 0, sizeof(struct_variableSetConvertBitStreamToPowerForm));
	return *p;
}
//////////////////////////////////////////////////////////////////////////////////
char closeVariableSetConvertBitStreamToPowerForm(struct_variableSetConvertBitStreamToPowerForm **p)
{
	if(!p)
	{
		return 0;
	}
	else if(!(*p))
	{
		free(*p);
	}
	(*p)=NULL;
	return 0;
}
//////////////////////////////////////////////////////////////////////////////////
// struct struct_variableSetConvertBitStreamToPowerForm
// {
	// unsigned char *data;

	// unsigned char dataBitLength;
	
	// unsigned int codeWordPerDataNumber;
	
	// unsigned long totalCodeWordNumber;
	// unsigned long countedCodeWordNumber;
	
	// unsigned long totalDataNumber;
	// unsigned long countedDataNumber;

// }typedef struct_variableSetConvertBitStreamToPowerForm;
//////////////////////////////////////////////////////////////////////////////////
char loadToCodeWordStartAtHighSide(struct_powerFormPolynomials *p, struct_variableSetConvertBitStreamToPowerForm *variables)
{
	unsigned int i;
	unsigned char shifted;
	unsigned int equationIndex;

	#ifndef RELEASE
	if(!p)
	{
		return -1;
	}
	if(!variables)
	{
		return -1;
	}
	#endif

	/*initial high side condition*/
	shifted=variables->dataBitLength;
	equationIndex=p->usedLength;

	/*load 1 codeword start*/
	for(i=0; i<variables->codeWordPerDataNumber; i++)
	{
		/*load 8 bits start*/
		if(!(variables->countedDataNumber<variables->totalDataNumber))
		{
			break;
		}
		for(shifted=variables->dataBitLength; shifted!=0; shifted--)
		{
			if(((1<<(shifted-1))&(*(variables->data+variables->countedCodeWordNumber))))
			{
				*(p->equation+equationIndex-1)='1';
			}
			else
			{
				*(p->equation+equationIndex-1)='0';
			}
			equationIndex--;
		}
		variables->countedDataNumber++;
		/*load 8 bits end*/
		variables->countedCodeWordNumber++;
	}
	/*load 1 codeword end*/
	
	for(; equationIndex!=0; equationIndex--)
	{
		*(p->equation+equationIndex-1)='0';
	}

	return 0;
}
//////////////////////////////////////////////////////////////////////////////////
char unloadFromCodeWordStartAtHighSide(struct_variableSetConvertBitStreamToPowerForm *variables, struct_powerFormPolynomials *p)
{
	unsigned int i;
	unsigned char shifted;
	unsigned int equationIndex;

	#ifndef RELEASE
	if(!p)
	{
		return -1;
	}
	if(!variables)
	{
		return -1;
	}
	#endif

	/*initial high side condition*/
	shifted=variables->dataBitLength;
	equationIndex=p->usedLength;

	/*load 1 codeword start*/
	for(i=0; i<variables->codeWordPerDataNumber; i++)
	{
		/*load 8 bits start*/
		if(!(variables->countedDataNumber<variables->totalDataNumber))
		{
			break;
		}
		(*(variables->data+variables->countedCodeWordNumber))=0;
		for(shifted=variables->dataBitLength; shifted!=0; shifted--)
		{
			if(*(p->equation+equationIndex-1)=='1')
			{
				(*(variables->data+variables->countedCodeWordNumber))|=(1<<(shifted-1));
			}
			equationIndex--;
		}
		variables->countedDataNumber++;
		/*load 8 bits end*/
		variables->countedCodeWordNumber++;
	}
	/*load 1 codeword end*/
	
	return 0;
}
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
/*
Elements of galois field have finite values are represented like 0110.
And if polynomial represented by power form have 15 bits length, length of elements of galois field is represented (15+1)/2.
*/
typedef char char_GALOIS_FIELD_VALUE;
typedef unsigned int u_int_GALOIS_FIELD_INTEGER;
struct struct_galoisFieldPolyForm
{
	//unsigned int *bits;
	unsigned int length;
	char_GALOIS_FIELD_VALUE *value;
	u_int_GALOIS_FIELD_INTEGER intValue;
	u_int_GALOIS_FIELD_INTEGER index;
}typedef struct_galoisFieldPolyForm;
//////////////////////////////////////////////////////////////////////////////////
struct_galoisFieldPolyForm *createPolyForm(unsigned int length)
{
	struct_galoisFieldPolyForm *p;

	#ifndef RELEASE
	if(!length)
	{
		errorMes;
		printf("arg[0], poly form length is %d\n", length);
		return NULL;
	}
	#endif
	
	p = (struct_galoisFieldPolyForm*)malloc(sizeof(struct_galoisFieldPolyForm));
	memset(p, 0, sizeof(struct_galoisFieldPolyForm));
	
	//p->bits=&(p->length);
	
	p->length=length;
	
	p->value = (char_GALOIS_FIELD_VALUE*)malloc((sizeof(char_GALOIS_FIELD_VALUE)*length)+1);//+1 is to null.
	memset(p->value, '0', sizeof(char_GALOIS_FIELD_VALUE)*length);
	*(p->value+length)=0;//insert null.
	
	p->intValue=0;
	p->index=0;
	
	return p;
}
//////////////////////////////////////////////////////////////////////////////////
void closePolyForm(struct_galoisFieldPolyForm **p)
{
	#ifndef RELEASE
	if(!p)
	{
		warningMes;
		printf("in struct_galoisFieldPolyForm, struct_galoisFieldPolyForm **p = 0x%x\n", p);
	}
	if(!*p)
	{
		warningMes;
		printf("in struct_galoisFieldPolyForm, struct_galoisFieldPolyForm **p = 0x%x\n", *p);
		return;
	}
	#endif
	
	free((*p)->value);
	free((*p));
	*p=NULL;
}
//////////////////////////////////////////////////////////////////////////////////
struct_galoisFieldPolyForm *recreatePolyForm(struct_galoisFieldPolyForm **p, unsigned int length)
{
	if(*p)
	{
		#ifndef RELEASE
		if((global_flag_cmdOption&FLAG_MASK_PRINTF_LOG))
		{
			printf("in recreatePolyForm, struct_galoisFieldPolyForm *p is already have data.\n");
			printf("*p will be closePolyForm(p).\n");
		}
		#endif
		
		closePolyForm(p);
	}
	*p=createPolyForm(length);
	return *p;
}
//////////////////////////////////////////////////////////////////////////////////
unsigned int convertGaloisFieldPolyFormUnsignedInt(struct_galoisFieldPolyForm *p)
{
	unsigned int i;
	unsigned int result=0;
	
	#ifdef USING_OPEN_MP
	#pragma omp parallel for schedule(guided) private(i) shared(p) reduction(|:result)
	#endif
	for(i=0; i<p->length; i++)
	{
		if(*(p->value+i)=='1')
		{
			result|=(1<<i);
		}
	}
	return result;
}
//////////////////////////////////////////////////////////////////////////////////
void printPolyFormWithEnter(struct_galoisFieldPolyForm *p)
{
	unsigned int i;

	if(!p)
	{
		errorMes;
		printf("struct_galoisFieldPolyForm addr is wrong(addr:0x%x).\n", p);
	}

	printf("\"");
	for(i=0; i<p->length; i++)
	{
		printf("%c", *(p->value+i));
	}
	printf("\"");
	printf("\n");
}
//////////////////////////////////////////////////////////////////////////////////
void printPolyFormPure(struct_galoisFieldPolyForm *p)
{
	unsigned int i;

	if(!p)
	{
		errorMes;
		printf("struct_galoisFieldPolyForm addr is wrong(addr:0x%x).\n", p);
	}

	for(i=0; i<p->length; i++)
	{
		printf("%c", *(p->value+i));
	}
}
//////////////////////////////////////////////////////////////////////////////////
void printPolyForm(struct_galoisFieldPolyForm *p)
{
	unsigned int i;

	if(!p)
	{
		errorMes;
		printf("struct_galoisFieldPolyForm addr is wrong(addr:0x%x).\n", p);
	}

	printf("\"");
	for(i=0; i<p->length; i++)
	{
		printf("%c", *(p->value+i));
	}
	printf("\"");
}
//////////////////////////////////////////////////////////////////////////////////
void printPolyFormWithTapAndEnter(struct_galoisFieldPolyForm *p)
{
	unsigned int i;

	if(!p)
	{
		errorMes;
		printf("struct_galoisFieldPolyForm addr is wrong(addr:0x%x).\n", p);
	}

	for(i=0; i<p->length; i++)
	{
		printf("%c\t", *(p->value+i));
	}
	printf("\n");
}
//////////////////////////////////////////////////////////////////////////////////
void printPolyFormWithTap(struct_galoisFieldPolyForm *p)
{
	unsigned int i;

	if(!p)
	{
		errorMes;
		printf("struct_galoisFieldPolyForm addr is wrong(addr:0x%x).\n", p);
	}

	for(i=0; i<p->length; i++)
	{
		printf("%c\t", *(p->value+i));
	}
}
//////////////////////////////////////////////////////////////////////////////////
void inputValueToPolyForm(struct_galoisFieldPolyForm *p, char *string)
{
	unsigned int i;

	#ifndef RELEASE
	if(!p)
	{
		errorMes;
		printf("in inputValueToPolyForm, struct_galoisFieldPolyForm addr is wrong(addr:0x%x).\n", p);
	}
	if(p->length!=strlen(string))
	{
		errorMes;
		printf("in inputValueToPolyForm, %s(%d) and struct_galoisFieldPolyForm->length(%d) are not same.\n", string, strlen(string), p->length);
	}
	#endif
	
	#ifndef RELEASE
	for(i=0; i<p->length; i++)
	{
		if((*(string+i)=='0')&&(*(string+i)=='1'))
		{
			errorMes;
			printf("%s(%d) is composed with wrong character.\n", string, strlen(string), p->length);
			return;
		}
	}
	#endif

	#ifdef USING_OPEN_MP
	#pragma omp parallel for schedule(guided) private(i) shared(p, string)
	#endif
	for(i=0; i<p->length; i++)
	{
		*(p->value+i)=*(string+i);
	}
}
//////////////////////////////////////////////////////////////////////////////////
char checkValueFromPolyFormUsingGaloisFieldValue(struct_galoisFieldPolyForm *operandA, struct_galoisFieldPolyForm *operandB)
{
	unsigned int i;

	#ifndef RELEASE
	if(!operandA)
	{
		errorMes;
		printf("struct_galoisFieldPolyForm addr is wrong(addr:0x%x).\n", operandA);
		return 0;
	}
	if(!operandB)
	{
		errorMes;
		printf("struct_galoisFieldPolyForm addr is wrong(addr:0x%x).\n", operandB);
		return 0;
	}
	if(operandA->length!=operandB->length)
	{
		errorMes;
		printf("galois field polynomial A and B lengths(%d!=%d) are not same each other.\n", operandA->length, operandB->length);
		return 0;
	}
	#endif
	
	for(i=0; i<operandA->length; i++)
	{
		if((*(operandA->value+i))!=(*(operandB->value+i)))
		{
			return 0;
		}
	}
	/* if((global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)==FLAG_MASK_PRINTF_LOG)
	{
		logMes;
		printf("find!\n");
	} */
	return 1;
}
//////////////////////////////////////////////////////////////////////////////////
char checkValueFromPolyFormUsingGaloisFieldValueUsingIntValue_(struct_galoisFieldPolyForm *operandA, struct_galoisFieldPolyForm *operandB)
{
	unsigned int i;

	#ifndef RELEASE
	if(!operandA)
	{
		errorMes;
		printf("struct_galoisFieldPolyForm addr is wrong(addr:0x%x).\n", operandA);
		return 0;
	}
	if(!operandB)
	{
		errorMes;
		printf("struct_galoisFieldPolyForm addr is wrong(addr:0x%x).\n", operandB);
		return 0;
	}
	if(operandA->length!=operandB->length)
	{
		errorMes;
		printf("galois field polynomial A and B lengths(%d!=%d) are not same each other.\n", operandA->length, operandB->length);
		return 0;
	}
	#endif

	
	if(!((operandA->intValue) ^ (operandB->intValue)))
	{
		return 1;
	}

	return 0;
}
//////////////////////////////////////////////////////////////////////////////////
char checkAllValueUsingCharacterFromPolyForm(struct_galoisFieldPolyForm *p, char c)
{
	unsigned int i;

	#ifndef RELEASE
	if(!p)
	{
		errorMes;
		printf("in checkAllValueUsingCharacterFromPolyForm, struct_galoisFieldPolyForm addr is wrong(addr:0x%x).\n", p);
		return 0;
	}
	if((c!='0')&&(c!='0'))
	{
		errorMes;
		printf("in checkAllValueUsingCharacterFromPolyForm, \'%c\'(0x%x) is not \'0\' or \'1\'.\n", c, c);
		return 0;
	}
	#endif
	
	for(i=0; i<p->length; i++)
	{
		if((*(p->value+i))!=c)
		{
			return 0;
		}
	}
	return 1;
}
//////////////////////////////////////////////////////////////////////////////////
char checkValueUsingStringFromPolyForm(struct_galoisFieldPolyForm *p, char *string)
{
	unsigned int i;

	#ifndef RELEASE
	if(!p)
	{
		errorMes;
		printf("in checkValueUsingStringFromPolyForm, struct_galoisFieldPolyForm addr is wrong(addr:0x%x).\n", p);
		return 0;
	}
	if(p->length!=strlen(string))
	{
		errorMes;
		printf("in checkValueUsingStringFromPolyForm, \"%s\"(%d) and struct_galoisFieldPolyForm->length(%d) is not same.\n", string, strlen(string), p->length);
		return 0;
	}
	#endif
	
	for(i=0; i<p->length; i++)
	{
		if((*(string+i)!='0')&&(*(string+i)!='1'))
		{
			#ifndef RELEASE
				errorMes;
				printf("in checkValueUsingStringFromPolyForm, \"%s\"(%d) is composed with wrong character.\n", string, strlen(string));
			#endif
			return 0;
		}
		else if((*(p->value+i))!=(*(string+i)))
		{
			return 0;
		}
	}
	return 1;
}
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
struct struct_summationMatrix
{
	unsigned int row;
	unsigned int column;
	struct_galoisFieldPolyForm ***element;
}typedef struct_summationMatrix;
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
/*
	!!! WARNING !!!
	Avoid to use functions are related with struct_galoisFieldPolyForm directly.
	As possible, use functions of struct_galoisFieldElements.
*/
struct_galoisFieldPolyForm **createListOfGaloisField(unsigned int lengthOfList)
{
	unsigned int i;
	struct_galoisFieldPolyForm **p;

	#ifndef RELEASE
	if(!lengthOfList)
	{
		errorMes;
		printf("createListOfGaloisField lengthOfList is 0\n");
		return NULL;
	}
	#endif
	
	p=(struct_galoisFieldPolyForm**)malloc(sizeof(struct_galoisFieldPolyForm*)*lengthOfList);
	memset(p, 0, sizeof(struct_galoisFieldPolyForm*)*lengthOfList);
	
	return p;
}
//////////////////////////////////////////////////////////////////////////////////
/*
	!!! WARNING !!!
	Avoid to use functions are related with struct_galoisFieldPolyForm directly.
	As possible, use functions of struct_galoisFieldElements.
*/
char addLengthOfListOfGaloisField_VariableLength(struct_galoisFieldPolyForm *(**p), unsigned int lengthOfList, unsigned int addedLengthOfList)
{
	unsigned int i;
	
	struct_galoisFieldPolyForm **newGaloisFieldPolyForm=NULL;
	#ifndef RELEASE
	if(!(*p))
	{
		errorMes;
		printf("in addLengthOfListOfGaloisField_VariableLength, struct_galoisFieldPolyForm *(**p) is NULL\n");
		return -1;
	}
	if(!lengthOfList)
	{
		errorMes;
		printf("in addLengthOfListOfGaloisField_VariableLength, unsigned int lengthOfList is 0\n");
		return -1;
	}
	#endif
	
	
	#ifndef RELEASE
	if(!addedLengthOfList)
	{
		warningMes;
		printf("in addLengthOfListOfGaloisField_VariableLength, unsigned int addedLengthOfList is 0\n");
		return 0;
	}
	#endif
	
	
	
	/* Sometime realloc has memory leakage. So to increase memory size, using this func is invalid. */
	//p=(struct_galoisFieldPolyForm**)realloc(p, sizeof(struct_galoisFieldPolyForm*)*addedLengthOfList);
	newGaloisFieldPolyForm=(struct_galoisFieldPolyForm**)malloc(sizeof(struct_galoisFieldPolyForm*)*(lengthOfList+addedLengthOfList));
	memset(newGaloisFieldPolyForm+lengthOfList, 0, (sizeof(struct_galoisFieldPolyForm*)*(addedLengthOfList)));
	memcpy(newGaloisFieldPolyForm, (*p), (sizeof(struct_galoisFieldPolyForm*)*(lengthOfList)));
	free((*p));
	(*p)=newGaloisFieldPolyForm;

	return 0;
}

//////////////////////////////////////////////////////////////////////////////////
/*
	!!! WARNING !!!
	Avoid to use functions are related with struct_galoisFieldPolyForm directly.
	As possible, use functions of struct_galoisFieldElements.
*/
char closeListOfGaloisField(struct_galoisFieldPolyForm ***p)
{
	#ifndef RELEASE
	if(!p)
	{
		errorMes;
		printf("closeListOfGaloisField ***p is NULL\n");
		return -1;
	}
	if(!(*p))
	{
		warningMes;
		printf("closeListOfGaloisField **p is NULL\n");
		return -1;
	}
	
	if(global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)
	{
		printf("in closeListOfGaloisField, struct_galoisFieldPolyForm p = \'0x%x\'\n", p);
		printf("in closeListOfGaloisField, struct_galoisFieldPolyForm *p = \'0x%x\'\n", *p);
	}
	#endif
	
	
	free (*p);
	*p=NULL;
	return 0;
}
//////////////////////////////////////////////////////////////////////////////////
/*
	!!! WARNING !!!
	Avoid to use functions are related with struct_galoisFieldPolyForm directly.
	As possible, use functions of struct_galoisFieldElements.
*/
struct_galoisFieldPolyForm **recreateListOfGaloisField(struct_galoisFieldPolyForm ***p, unsigned int lengthOfList)
{
	#ifndef RELEASE
	if(p)
	{
		errorMes;
		printf("in recreateListOfGaloisField, struct_galoisFieldPolyForm ***p is NULL.\n");
		return NULL;
	}
	#endif
	if(*p)
	{
				#ifndef RELEASE
				if((global_flag_cmdOption&FLAG_MASK_PRINTF_LOG))
				{
					printf("in createListOfGaloisField, struct_galoisFieldPolyForm **p is already have data.\n");
					printf("**p will be closePowerFormPolynomial(p).\n");
				}
				#endif
		closeListOfGaloisField(p);
	}
	*p=createListOfGaloisField(lengthOfList);
	return *p;
}
//////////////////////////////////////////////////////////////////////////////////
char copyListOfGaloisField(struct_galoisFieldPolyForm **to, struct_galoisFieldPolyForm **from, unsigned int size)
{
	unsigned int i;

	#ifndef RELEASE
	if(!to)
	{
		errorMes;
		printf("in copyListOfGaloisField, struct_galoisFieldPolyForm **to is NULL.\n");
		return -1;
	}
	if(!from)
	{
		errorMes;
		printf("in copyListOfGaloisField, struct_galoisFieldPolyForm **from is NULL.\n");
		return -1;
	}
	if(!size)
	{
		errorMes;
		printf("in copyListOfGaloisField, unsigned int size is 0.\n");
		return -1;
	}
	
	if((global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)==FLAG_MASK_PRINTF_LOG)
	{
		logMes;
		printf("in copyListOfGaloisField, addr(to)=0x%x, addr(from)=0x%x\n", to, from);
	}
	#endif

	#ifdef USING_OPEN_MP
	#pragma omp parallel for schedule(guided) private(i) shared(size, to, from)
	#endif
	for(i=0; i<size; i++)
	{
		*(to+i)=*(from+i);
	}
	
	return 0;
}
//////////////////////////////////////////////////////////////////////////////////
char createGaloisFieldElementsAtList(struct_galoisFieldPolyForm **p, unsigned int lengthOfList, unsigned int lengthOfPolyForm)
{
	unsigned int i;

	#ifndef RELEASE
	if(!p)
	{
		errorMes;
		printf("in createGaloisFieldElementsAtList, struct_galoisFieldPolyForm **p is NULL.\n");
		return -1;
	}
	if(!lengthOfList)
	{
		errorMes;
		printf("in createGaloisFieldElementsAtList,  unsigned int lengthOfList is 0.\n");
		return -1;
	}
	if(!lengthOfPolyForm)
	{
		errorMes;
		printf("in createGaloisFieldElementsAtList, unsigned int lengthOfPolyForm is 0.\n");
		return -1;
	}
	#endif
	
	#ifdef USING_OPEN_MP
	#pragma omp parallel for schedule(guided) private(i) shared(lengthOfList, p, lengthOfPolyForm)
	#endif
	for(i=0; i<lengthOfList; i++)
	{
		*(p+i)=createPolyForm(lengthOfPolyForm);
	}
	return 0;
}
//////////////////////////////////////////////////////////////////////////////////
char closeGaloisFieldElementsAtList(struct_galoisFieldPolyForm ***p, unsigned int lengthOfList)
{
	unsigned int i;

	#ifndef RELEASE
	if(!p)
	{
		errorMes;
		printf("in closeGaloisFieldElementsAtList, struct_galoisFieldPolyForm ***p is NULL.\n");
		return -1;
	}
	if(!(*p))
	{
		warningMes;
		printf("in closeGaloisFieldElementsAtList, struct_galoisFieldPolyForm **p is NULL.\n");
		return -1;
	}
	if(!lengthOfList)
	{
		errorMes;
		printf("in closeGaloisFieldElementsAtList, unsigned int lengthOfList is 0.\n");
		return -1;
	}
	#endif
	
	#ifdef USING_OPEN_MP
	#pragma omp parallel for schedule(guided) private(i) shared(lengthOfList, p)
	#endif
	for(i=0; i<lengthOfList; i++)
	{
		closePolyForm(&(*((*p)+i)));
	}
	return 0;
}
//////////////////////////////////////////////////////////////////////////////////
char recreateGaloisFieldElementsAtList(struct_galoisFieldPolyForm **p, unsigned int lengthOfList, unsigned int lengthOfPolyForm)
{
	if(closeGaloisFieldElementsAtList(&p, (*p)->length))
	{
		warningMes;
		printf("in recreateGaloisFieldElementsAtList, closeGaloisFieldElementsAtList(&p, (*p)->length) is failed.\n");
	}
	createGaloisFieldElementsAtList(p, lengthOfList, lengthOfPolyForm);
	return 0;
}
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
struct_galoisFieldPolyForm **createListOfGaloisFieldAndComponents(unsigned int lengthOfList, unsigned int polyLength)
{
	unsigned int i;
	// struct_galoisFieldPolyForm **p=(struct_galoisFieldPolyForm**)malloc(sizeof(struct_galoisFieldPolyForm*)*lengthOfList);
	// memset(p, 0, sizeof(struct_galoisFieldPolyForm*)*lengthOfList);
	struct_galoisFieldPolyForm **p=NULL;
	if(!(p=createListOfGaloisField(lengthOfList)))
	{
				#ifndef RELEASE
				errorMes; printf("in createListOfGaloisFieldAndComponents, fail that createListOfGaloisField(lengthOfList).\n");
				errorMes; printf("return value struct_galoisFieldPolyForm **p = 0x%x\n",p);
				#endif
		return NULL;
	}
	if(createGaloisFieldElementsAtList(p, lengthOfList, polyLength))
	{
		closeGaloisFieldElementsAtList(&p, lengthOfList);
		closeListOfGaloisField(&p);
				#ifndef RELEASE
				errorMes;	printf("in createListOfGaloisFieldAndComponents, fail that createGaloisFieldElementsAtList(p, lengthOfList, polyLength).\n");
				#endif
		return NULL;
	}
	
	return p;
}
//////////////////////////////////////////////////////////////////////////////////
char closeListOfGaloisFieldAndElements(struct_galoisFieldPolyForm ***p, unsigned int lengthOfList)
{
	if(closeGaloisFieldElementsAtList(p, lengthOfList))
	{
		#ifndef RELEASE
		errorMes; printf("in closeListOfGaloisFieldAndElements, fail close closeGaloisFieldElementsAtList(p, lengthOfList)\n");
		#endif
		return -1;
	}
	if(closeListOfGaloisField(p))
	{
				#ifndef RELEASE
				errorMes; printf("in closeListOfGaloisFieldAndElements, fail close closeListOfGaloisField(p)\n");
				#endif
		return -1;
	}
	*p=NULL;
	return 0;
}
//////////////////////////////////////////////////////////////////////////////////
/* struct_galoisFieldPolyForm **recreateListOfGaloisFieldAndComponents(struct_galoisFieldPolyForm ***p, unsigned int lengthOfList, unsigned int polyLength)
{
	if(*p)
	{
		if((global_flag_cmdOption&FLAG_MASK_PRINTF_LOG))
		{
			infoMes;
			printf("in createListOfGaloisFieldAndComponents, struct_galoisFieldPolyForm *p have data already.\n");
			printf("it will be recreated.\n");
		}
		closeListOfGaloisFieldAndComponents((*p)->length);
	}
	*p=createListOfGaloisFieldAndComponents(lengthOfList, polyLength);
	return *p;
} */
//////////////////////////////////////////////////////////////////////////////////
struct struct_galoisFieldElements
{
	unsigned int length;
	struct_galoisFieldPolyForm **element;
	
	struct_summationMatrix *summationReferMatrix;
}typedef struct_galoisFieldElements;
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
/* handle galois field except elements start */
//////////////////////////////////////////////////////////////////////////////////
struct_galoisFieldElements *createGaloisFieldExceptElements(unsigned int numberOfElement)
{
	struct_galoisFieldElements *p;

	if(!numberOfElement)
	{
				#ifndef RELEASE
				errorMes; printf("in *createGaloisFieldExceptElements, unsigned int numberOfElement is ZERO\n");
				#endif
		return NULL;
	}
	p=(struct_galoisFieldElements*)malloc(sizeof(struct_galoisFieldElements));
	memset(p, 0, sizeof(struct_galoisFieldElements));
	
	p->length=numberOfElement;
	p->element=createListOfGaloisField(numberOfElement);
	return p;
}
//////////////////////////////////////////////////////////////////////////////////
struct_galoisFieldElements *createGaloisFieldExceptElements_VariableLength()
{
	struct_galoisFieldElements *p;


	p=(struct_galoisFieldElements*)malloc(sizeof(struct_galoisFieldElements));
	memset(p, 0, sizeof(struct_galoisFieldElements));
	
//	p->length=numberOfElement;
//	p->element=createListOfGaloisField(numberOfElement);
	return p;
}
//////////////////////////////////////////////////////////////////////////////////
char addLengthOfGaloisFieldExceptElements_VariableLength(struct_galoisFieldElements *p, unsigned int addedLength)
{
	#ifndef RELEASE
	if(!p)
	{
		errorMes;
		printf("in *addedLengthOfGaloisFieldExceptElements, struct_galoisFieldElements *p is NULL\n");
		return -1;
	}
	if(!addedLength)
	{
		errorMes;
		printf("in *addedLengthOfGaloisFieldExceptElements, unsigned int addedLength is ZERO\n");
		return -1;
	}
	#endif
	
	
	if(!(p->length))
	{
		if(!(p->element))
		{
			p->element=createListOfGaloisField(addedLength);
		}
		else
		{
			p->element=recreateListOfGaloisField(&(p->element), addedLength);
		}
		p->length=addedLength;
	}
	else
	{
		if(addLengthOfListOfGaloisField_VariableLength(&(p->element), p->length, addedLength))
		{
					#ifndef RELEASE
					warningMes; printf("in addLengthOfGaloisFieldExceptElements_VariableLength, addLengthOfListOfGaloisField_VariableLength(p->element, p->length, addedLength) is return to Not ZERO.\n");
					#endif
			return -1;
		}
		else
		{
			p->length+=addedLength;
		}
	}
	return 0;
}
//////////////////////////////////////////////////////////////////////////////////
char closeGaloisFieldExceptElements(struct_galoisFieldElements **p)
{
	#ifndef RELEASE
	if(!p)
	{
		errorMes;
		printf("in closeGaloisFieldExceptElements, struct_galoisFieldElements **p is NULL\n");
		return -1;
	}
	if(!*p)
	{
		warningMes;
		printf("in closeGaloisFieldExceptElements, struct_galoisFieldElements *p is NULL\n");
		return -1;
	}
	#endif
	
			#ifndef RELEASE
			if((global_flag_cmdOption&FLAG_MASK_PRINTF_LOG))
			{
				infoMes;
				printf("closeGaloisFieldExceptElements, struct_galoisFieldElements p = \'0x%x\', struct_galoisFieldElements *p = \'0x%x\'\n", p, *p);
			}
			if(global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)
			{
				infoMes;
				printf("in closeGaloisFieldExceptElements, &((*p)->element)) = \'0x%x\'\n", &((*p)->element));
				printf("in closeGaloisFieldExceptElements, ((*p)->element)) = \'0x%x\'\n", ((*p)->element));
			}
			#endif
	if(closeListOfGaloisField(&((*p)->element)))
	{
		#ifndef RELEASE
		errorMes; printf("in closeGaloisFieldExceptElements, closeListOfGaloisField(&((*p)->element)) is fail.\n");
		#endif
	}
	free((*p));
	*p=NULL;
	return 0;
}
//////////////////////////////////////////////////////////////////////////////////
struct_galoisFieldElements *recreateGaloisFieldExceptElements(struct_galoisFieldElements **p, unsigned int numberOfElement)
{
	if(*p)
	{
				#ifndef RELEASE
				if((global_flag_cmdOption&FLAG_MASK_PRINTF_LOG))
				{
					infoMes;
					printf("in recreateGaloisFieldExceptElements, struct_galoisFieldElements *p have data already.\n");
					printf("it will be recreated.\n");
				}
				#endif
		closeGaloisFieldExceptElements(p);
	}
	*p=createGaloisFieldExceptElements(numberOfElement);
	return *p;
}
//////////////////////////////////////////////////////////////////////////////////
/* handle galois field except elements end */
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
/* handle galois field list and elements start */
//////////////////////////////////////////////////////////////////////////////////
struct_galoisFieldElements *createGaloisFieldAndElements(unsigned int numberOfElement, unsigned int polyLength)
{
	unsigned int i;
	
	struct_galoisFieldElements *p;

	#ifndef RELEASE
	if(!numberOfElement)
	{
		errorMes;
		printf("in *createGaloisFieldAndElements, GaloisFielsElements numberOfElement is %d\n", numberOfElement);
		return NULL;
	}
	#endif

	p = (struct_galoisFieldElements*)malloc(sizeof(struct_galoisFieldElements));
	memset(p, 0, sizeof(struct_galoisFieldElements));
	
	p->length=numberOfElement;
	
	p->element = createListOfGaloisFieldAndComponents(numberOfElement, polyLength);

			#ifndef RELEASE
			if((global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)==FLAG_MASK_PRINTF_LOG)
			{
				logMes;
				printf("GaloisFielsElements length is %d\n", numberOfElement);
			}
			#endif
	return p;
}
//////////////////////////////////////////////////////////////////////////////////
char closeGaloisFielsAndElements(struct_galoisFieldElements **p)
{
	#ifndef RELEASE
	if(!p)
	{
		errorMes;
		printf("in closeGaloisFielsAndElements, **p addr = 0x%x\n", (p));
		return -1;
	}
	if(!(*p))
	{
		warningMes;
		printf("in closeGaloisFielsAndElements, *p addr = 0x%x\n", (*p));
		return -1;
	}
	#endif
	
	closeListOfGaloisFieldAndElements(&((*p)->element), (*p)->length);
	
	free((*p));
	*p=NULL;
	return 0;
}
//////////////////////////////////////////////////////////////////////////////////
struct_galoisFieldElements *recreateGaloisFieldAndElements(struct_galoisFieldElements **p, unsigned int numberOfElement, unsigned int polyLength)
{
	if(*p)
	{
				#ifndef RELEASE
				if(global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)
				{
					infoMes;
					printf("in recreateGaloisFieldAndElements, struct_galoisFieldElements *p have data already.\n");
					printf("it will be recreated.\n");
				}
				#endif
		closeGaloisFielsAndElements(p);
	}
	*p=createGaloisFieldAndElements(numberOfElement, polyLength);
	return *p;
}
//////////////////////////////////////////////////////////////////////////////////
char calculateGaloisField(struct_powerFormPolynomials *primitivePolynomial, struct_galoisFieldElements *p)
{
	unsigned int i,j;

	char_GALOIS_FIELD_VALUE shiftBuffer;
	
	#ifndef RELEASE
	if(!p)
	{
		return -1;
	}

	if(!primitivePolynomial)
	{
		return -3;
	}
	#endif
	

	*((*(p->element+1))->value+0)='1';
	// printf("((*(p->element+1))->length) %d <= (sizeof(u_int_GALOIS_FIELD_INTEGER)*8) %d\n", ((*(p->element+1))->length), (sizeof(uint32_t)*8));
	if(((*(p->element+1))->length)<=(sizeof(u_int_GALOIS_FIELD_INTEGER)*8))
	{
		(*(p->element+1))->intValue=convertGaloisFieldPolyFormUnsignedInt(*(p->element+1));
	}
	(*(p->element+1))->index=1;
	
			#ifndef RELEASE
			if((global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)==FLAG_MASK_PRINTF_LOG)
			{
				logMes;
				printf("(p->length) = %d\n", (p->length));
				printf("(*(p->element+0))->length = %d\n", (*(p->element+0))->length);
			}
			#endif
	
	
	// printf("[%05d]%s\n", 0,(*(p->element+0))->value);
	// printf("[%05d]%s\n", 1,(*(p->element+1))->value);
	infoMes; printf("[START][Generating GaloisField Elements]\n");
	for(i=2; i<(p->length); i++)
	{
		shiftBuffer=(*((*(p->element+i-1))->value+((*(p->element+i))->length-1)));
		for(j=(*(p->element+i))->length-1; j!=0; j--)
		{
			(*((*(p->element+i))->value+j))=(*((*(p->element+i-1))->value+j-1));
		}
		(*((*(p->element+i))->value+j))='0';
		(*((*(p->element+i))->value+(*(p->element+i))->length))=0;
		// (*((*(p->element+i-1))->value+((*(p->element+i))->length-1)))=0;
		
		if(shiftBuffer=='1')
		{
			for(j=0; j<(*(p->element+i))->length; j++)
			{
				if((*(primitivePolynomial->equation+j))!=(*((*(p->element+i))->value+j)))
				{
					(*((*(p->element+i))->value+j))='1';
				}
				else
				{
					(*((*(p->element+i))->value+j))='0';
				}
			}
			shiftBuffer='0';
		}
		
		
		if(((*(p->element+i))->length)<=(sizeof(u_int_GALOIS_FIELD_INTEGER)*8))
		{
			(*(p->element+i))->intValue=convertGaloisFieldPolyFormUnsignedInt(*(p->element+i));
		}
		(*(p->element+i))->index=i;
		
		// printf("[%05d]%s\n", i,(*(p->element+i))->value);
	}
	infoMes; printf("[END][Generating GaloisField Elements]\n");

/* 
	#ifndef RELEASE
	if(global_flag_gfCommon_display&FLAG_MASK_DISPLAY_GF_COMMON_FIELD)
	{
		initMes; printGaloisField(p);
	}
	#endif
*/
		/*
		Update. 2016/02/22
		seperated this parts, creating parts of conjugacy class is moved init_ConjugacyClasses
		
		*/
		/* create conjugacy Class (static) */
		// p->conjugacies=createConjugacyClasses(p);
		// calculateConjugacyClasses(p, p->conjugacies);
		
		/* create conjugacy Class (variable) */
		// p->conjugacies=createConjugacyClasses_VariableLength();
		/* 
		infoMes; printf("[START][Generating conjugacy set]\n");
		calculateConjugacyClasses_VariableLength(p, p->conjugacies);
		infoMes; printf("[END][Generating conjugacy set]\n");
		
		#ifndef RELEASE
		if(global_flag_gfCommon_display&FLAG_MASK_DISPLAY_GF_COMMON_CONJUGACIES)
		{
			initMes;
			printConjugacyClasses(p, p->conjugacies);
		}
		#endif
		*/

	return 0;
}
//////////////////////////////////////////////////////////////////////////////////
struct_galoisFieldElements *createGaloisField(struct_powerFormPolynomials *primitivePolynomial, unsigned int numberOfElement, unsigned int polyLength)
{
	struct_galoisFieldElements *p=NULL;
	if(!(p=createGaloisFieldAndElements(numberOfElement, polyLength)))
	{
		errorMes; printf("in createGaloisField, createGaloisFieldAndElements return NULL.\r\n");
		return p;
	}
	
	if(calculateGaloisField(primitivePolynomial, p))
	{
		errorMes; printf("in createGaloisField, calculateGaloisField return not 0.\r\n");
		closeGaloisFielsAndElements(&p);
		return NULL;
	}
	return p;
}
//////////////////////////////////////////////////////////////////////////////////
char closeGaloisField(struct_galoisFieldElements **p)
{
	#ifndef RELEASE
	if(!p) return -1;
	if(!(*p)) return -2;
	#endif
	
	if(closeGaloisFielsAndElements(p))
	{
		return -3;
	}
	return 0;
}
//////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////
/* handle galois field list and elements end */
//////////////////////////////////////////////////////////////////////////////////
/*
struct struct_galoisFieldElements
{
	unsigned int length;
	struct_galoisFieldPolyForm **element;
	
	struct_summationMatrix *summationReferMatrix;
}typedef struct_galoisFieldElements;
*/
/*
	char_GALOIS_FIELD_VALUE *value;
	u_int_GALOIS_FIELD_INTEGER intValue;
	u_int_GALOIS_FIELD_INTEGER index;
*/
//////////////////////////////////////////////////////////////////////////////////
void printGaloisFieldSavedForm(struct_galoisFieldElements *p)
{
	unsigned int tmp_i=0;
	
	#ifndef RELEASE
	if(!p) return;
	#endif
	
	printf(">> Galois Field File I/O Form <<\n");
	
	printf("pow(index), string, int value\r\n");
	printf("%c(%d), %s, %d\r\n", '-', (*(p->element+0))->index, (*(p->element+0))->value, (*(p->element+0))->intValue);
	for(tmp_i=1; tmp_i<p->length; tmp_i++)
	{
		printf("%d(%d), %s, %d\r\n", ((*(p->element+tmp_i))->index - 1), (*(p->element+tmp_i))->index, (*(p->element+tmp_i))->value, (*(p->element+tmp_i))->intValue);
	}
}
//////////////////////////////////////////////////////////////////////////////////
char save_struct_galoisFieldElements(char *path, char *primitivePoly, struct_galoisFieldElements *p)
{
	unsigned int tmp_i;
	FILE *fileio;
	if(!path)			return -1;
	if(!primitivePoly)	return -2;
	if(!p)				return -3;
	
	fileio = fopen(path, "w");
	
	fprintf(fileio, "primitive polynomials, %s\r\n", primitivePoly);

	fprintf(fileio, "%s\r\n", FILE_IO_DATA_PARSING_KEY_STRING_SAVED_DATA_START);


	fprintf(fileio, "%s\r\n", FILE_IO_DATA_PARSING_KEY_STRING_KIND_OF_DATA_START);
	fprintf(fileio, "Kind Of Data, %d\r\n", FILE_IO_KIND_OF_DATA_GALOIS_FIELDS);
	fprintf(fileio, "%s\r\n", FILE_IO_DATA_PARSING_KEY_STRING_KIND_OF_DATA_END);

	
	fprintf(fileio, "%s\r\n", FILE_IO_DATA_PARSING_KEY_STRING_ARRAY_TYPE_START);
	fprintf(fileio, "Saved Data Type, %d\r\n", FILD_IO_DATA_ARRAY_TYPE_LINEAR);
	fprintf(fileio, "%s\r\n", FILE_IO_DATA_PARSING_KEY_STRING_ARRAY_TYPE_END);

	fprintf(fileio, "%s\r\n", FILE_IO_DATA_PARSING_KEY_STRING_BIT_WIDTH_START);
	fprintf(fileio, "Bit Width, %d\r\n", (*(p->element+0))->length);
	fprintf(fileio, "%s\r\n", FILE_IO_DATA_PARSING_KEY_STRING_BIT_WIDTH_END);
	
	fprintf(fileio, "%s\r\n", FILE_IO_DATA_PARSING_KEY_STRING_ROW_DATA_START);
	fprintf(fileio, "galois field row length, %d\r\n", p->length);
	fprintf(fileio, "%s\r\n", FILE_IO_DATA_PARSING_KEY_STRING_ROW_DATA_END);

	fprintf(fileio, "%s\r\n", FILE_IO_DATA_PARSING_KEY_STRING_COL_DATA_START);
	fprintf(fileio, "galois field column length, %d\r\n", 1);
	fprintf(fileio, "%s\r\n", FILE_IO_DATA_PARSING_KEY_STRING_COL_DATA_END);
	
	fprintf(fileio, "pow(index),string,intValue\r\n");
	fprintf(fileio, "%s\r\n", FILE_IO_DATA_PARSING_KEY_STRING_RAW_DATA_START);
	fprintf(fileio, "%c(%d), %s, %d\r\n", '-', (*(p->element+0))->index, (*(p->element+0))->value, (*(p->element+0))->intValue);
	for(tmp_i=1; tmp_i<p->length; tmp_i++)
	{
		fprintf(fileio, "%d(%d), %s, %d\r\n", ((*(p->element+tmp_i))->index - 1), (*(p->element+tmp_i))->index, (*(p->element+tmp_i))->value, (*(p->element+tmp_i))->intValue);
	}
	fprintf(fileio, "%s\r\n", FILE_IO_DATA_PARSING_KEY_STRING_RAW_DATA_END);

	
	fprintf(fileio, "%s\r\n", FILE_IO_DATA_PARSING_KEY_STRING_SAVED_DATA_END);
	
	fclose(fileio);
	
	return 0;
}
//////////////////////////////////////////////////////////////////////////////////
char load_struct_galoisFieldElements(char *path, char *primitivePoly, struct_galoisFieldElements *p)
{
	unsigned int tmp_i;
	FILE *load_fileio_GF;
	
	unsigned int primitivePoly_length=0;
	char load_primitivePoly[1025];
	
	int load_kindOfData=0;
	int load_saveOfData=0;

	unsigned int load_bitWidth;
	unsigned int load_row;
	unsigned int load_column;

	unsigned int load_intVal;
	unsigned int load_index;
	// char load_exp_str[256]={0};
	char load_exp_char=0;
	unsigned int load_exp_integer=0;
	
	
	unsigned int load_integer=0;

	char charBuf=0;
	char load_stringBuf[1025]={0};

	
	if(!path)			return -1;
	if(!primitivePoly)	return -2;
	if(!p)				return -3;
	
	load_fileio_GF = fopen(path, "r");
	
			printf("[load_struct_galoisFieldElements START]\r\n");
	
	fscanf(load_fileio_GF, "primitive polynomials, %s\r\n", load_primitivePoly);
			// printf("primitive polynomials, %s\r\n", load_primitivePoly);

	fscanf(load_fileio_GF, "%s\r\n", load_stringBuf);
			// printf("%s\r\n", load_stringBuf);


	fscanf(load_fileio_GF, "%s\r\n", load_stringBuf);
			// printf("%s\r\n", load_stringBuf);
	fscanf(load_fileio_GF, "Kind Of Data, %d\r\n", &load_kindOfData);
	fscanf(load_fileio_GF, "%s\r\n", load_stringBuf);
			// printf("%s\r\n", load_stringBuf);

	
	fscanf(load_fileio_GF, "%s\r\n", load_stringBuf);
			// printf("%s\r\n", load_stringBuf);
	fscanf(load_fileio_GF, "Saved Data Type, %d\r\n", &load_saveOfData);
	fscanf(load_fileio_GF, "%s\r\n", load_stringBuf);
			// printf("%s\r\n", load_stringBuf);

	fscanf(load_fileio_GF, "%s\r\n", load_stringBuf);
			// printf("%s\r\n", load_stringBuf);
	fscanf(load_fileio_GF, "Bit Width, %d\r\n", &load_bitWidth);
		//(*(p->element+0))->length
	fscanf(load_fileio_GF, "%s\r\n", load_stringBuf);
			// printf("%s\r\n", load_stringBuf);
	
	fscanf(load_fileio_GF, "%s\r\n", load_stringBuf);
			// printf("%s\r\n", load_stringBuf);
	fscanf(load_fileio_GF, "galois field row length, %d\r\n", &load_row);
		//p->length
	fscanf(load_fileio_GF, "%s\r\n", load_stringBuf);
			// printf("%s\r\n", load_stringBuf);

	fscanf(load_fileio_GF, "%s\r\n", load_stringBuf);
			// printf("%s\r\n", load_stringBuf);
	fscanf(load_fileio_GF, "galois field column length, %d\r\n", &load_column);
		//1
	fscanf(load_fileio_GF, "%s\r\n", load_stringBuf);
			// printf("%s\r\n", load_stringBuf);
	
	fscanf(load_fileio_GF, "%s\r\n", load_stringBuf);
			//printf("%s\r\n", load_stringBuf);
	
	fscanf(load_fileio_GF, "%s\r\n", load_stringBuf);
			// printf("%s\r\n", load_stringBuf);
/* 	fscanf(load_fileio_GF, "%c(%d), %s, %d\r\n", '-', (*(p->element+0))->index, (*(p->element+0))->value, (*(p->element+0))->intValue);
	for(tmp_i=1; tmp_i<p->length; tmp_i++)
	{
		fscanf(load_fileio_GF, "%d(%d), %s, %d\r\n", ((*(p->element+tmp_i))->index - 1), (*(p->element+tmp_i))->index, (*(p->element+tmp_i))->value, (*(p->element+tmp_i))->intValue);
	} */
	// fscanf(load_fileio_GF, "%c(%d), %s, %d\r\n", &load_exp_char, &load_index, load_stringBuf, &load_intVal);
		fscanf(load_fileio_GF, "%c(%d), ", &load_exp_char, &load_index);
		fgets(load_stringBuf, load_bitWidth+1, load_fileio_GF);
		load_stringBuf[load_bitWidth] = 0;
		fscanf(load_fileio_GF, ", %d\r\n", &load_intVal);

			// printf("%c(%d), %s, %d\r\n", load_exp_char, load_index, load_stringBuf, load_intVal);

	strcpy((*(p->element+0))->value, load_stringBuf);
	(*(p->element+0))->intValue = load_intVal;
	(*(p->element+0))->index = load_index;
	for(tmp_i=1; tmp_i<p->length; tmp_i++)
	{
		fscanf(load_fileio_GF, "%d(%d), ", &load_exp_integer, &load_index);
		fgets(load_stringBuf, load_bitWidth+1, load_fileio_GF);
		load_stringBuf[load_bitWidth] = 0;
		fscanf(load_fileio_GF, ", %d\r\n", &load_intVal);
		strcpy((*(p->element+tmp_i))->value, load_stringBuf);
		(*(p->element+tmp_i))->intValue = load_intVal;
		(*(p->element+load_index))->index = load_index;
				// printf("%d(%d), %s, %d\r\n", load_exp_integer, load_index, load_stringBuf, load_intVal);
	}
	fscanf(load_fileio_GF, "%s\r\n", load_stringBuf);
			// printf("%s\r\n", load_stringBuf);

	
	fscanf(load_fileio_GF, "%s\r\n", load_stringBuf);
				// printf("%s\r\n", load_stringBuf);

	fclose(load_fileio_GF);
	
				printf("[load_struct_galoisFieldElements END]\r\n");
	
			#ifndef RELEASE
				if(global_flag_gfCommon_display&FLAG_MASK_DISPLAY_GF_COMMON_FILE_IO)
				{
					printf(">> Loaded Galois Fields <<\r\n");
					printGaloisFieldSavedForm(p);
				}
			#endif
	
	return 0;
}
//////////////////////////////////////////////////////////////////////////////////
char copyListOfGaloisFieldIngaloisFieldElements(struct_galoisFieldElements *galoisFields, struct_galoisFieldElements *to, struct_galoisFieldElements *from)
{
	unsigned int i;

	#ifndef RELEASE
	if(!to)
	{
		errorMes;
		printf("in copyListOfGaloisFieldIngaloisFieldElements, struct_galoisFieldPolyForm **to is NULL.\n");
		return -1;
	}
	if(!from)
	{
		errorMes;
		printf("in copyListOfGaloisFieldIngaloisFieldElements, struct_galoisFieldPolyForm **from is NULL.\n");
		return -1;
	}
	if((to->length)<(from->length))
	{
		errorMes;
		printf("in copyListOfGaloisFieldIngaloisFieldElements, to->length(=\'%d\') is shorter than from->length(=\'%d\').\n", (to->length), (from->length));
		return -1;
	}
	#endif

	#ifndef RELEASE	
	if((global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)==FLAG_MASK_PRINTF_LOG)
	{
		logMes;
		printf("in copyListOfGaloisFieldIngaloisFieldElements, addr(to)=0x%x, addr(from)=0x%x\n", to, from);
	}
	if((to->length)!=(from->length))
	{
		warningMes;
		printf("in copyListOfGaloisFieldIngaloisFieldElements, to->length(=\'%d\') and from->length(=\'%d\') is not same.\n", (to->length), (from->length));
		printf("exceed order is cleared to all zero.");
	}
	#endif
	
	if(copyListOfGaloisField(to->element, from->element, from->length))
	{
		return -1;
	}

	#ifdef USING_OPEN_MP
	#pragma omp parallel for schedule(guided) private(i) shared(from, to, galoisFields)
	#endif
	for(i=from->length; i<to->length; i++)
	{
		*(to->element+i)=*(galoisFields->element+0);
	}
	return 0;
}
//////////////////////////////////////////////////////////////////////////////////
char *convertGaloisFielsAndElementsToStringOnlyZeroOrOne(struct_galoisFieldElements *field, struct_galoisFieldElements *convertedPolynomial)
{
	unsigned int i;
	
	char *p;

	#ifndef RELEASE
	if(!field)
	{
		errorMes;
		printf("in convertGaloisFielsAndElementsToStringOnlyZeroOrOne, struct_galoisFieldElements *field is NULL.\n");
		return NULL;
	}
	if(!convertedPolynomial)
	{
		errorMes;
		printf("in convertGaloisFielsAndElementsToStringOnlyZeroOrOne, struct_galoisFieldElements *p is NULL.\n");
		return NULL;
	}
	#endif
	
	p=(char*)malloc(sizeof(char)*(convertedPolynomial->length+1));
	*(p+convertedPolynomial->length)=0;
	
	for(i=0; i<convertedPolynomial->length; i++)
	{
		if(checkValueFromPolyFormUsingGaloisFieldValueUsingIntValue_((*(convertedPolynomial->element+i)), (*(field->element+0))))
		{
			*(p+i)='0';
		}
		else if(checkValueFromPolyFormUsingGaloisFieldValueUsingIntValue_((*(convertedPolynomial->element+i)), (*(field->element+1))))
		{
			*(p+i)='1';
		}
		else
		{
					#ifndef RELEASE
					errorMes; printf("in convertGaloisFielsAndElementsToStringOnlyZeroOrOne, (*(convertedPolynomial->element+i)) have a ilegal valie.\n");
					#endif
			free(p);
			return NULL;
		}
	}
	return p;
}

//////////////////////////////////////////////////////////////////////////////////
struct_galoisFieldPolyForm *findSameElementOfGaloisField(struct_galoisFieldElements *field, struct_galoisFieldPolyForm *p)
{
	unsigned int i,j;
	struct_galoisFieldPolyForm *result=NULL;
	#ifndef RELEASE
	if((*(field->element))->length!=p->length)
	{
		errorMes;
		printf("in *findSameElementOfGaloisField, field poly length: %d, target field poly length: %d\n", (*(field->element))->length, p->length);
	}
	#endif

	for(i=0; i<field->length; i++)
	{
		if(checkValueFromPolyFormUsingGaloisFieldValueUsingIntValue_(*(field->element+i), p))
		{
			result=(*(field->element+i));
		}
	}
	return result;
}
//////////////////////////////////////////////////////////////////////////////////
struct_galoisFieldPolyForm *findSameElementOfGaloisFieldUsingString(struct_galoisFieldElements *field, char *string)
{
	unsigned int strLength = strlen(string);

	unsigned int i,j;

	#ifndef RELEASE
	if((*(field->element))->length!=strLength)
	{
		errorMes;
		printf("in *findSameElementOfGaloisFieldUsingString, field poly length: %d, string length: %d\n", (*(field->element))->length, strLength);
		printf("string : \"%s\"\n", string);
	}
	#endif
	
	for(i=0; i<field->length; i++)
	{
		if(checkValueUsingStringFromPolyForm(*(field->element+i), string)) return (*(field->element+i));
	}
	return NULL;
}
//////////////////////////////////////////////////////////////////////////////////
struct_galoisFieldPolyForm *invertValueElementOfGaloisField(struct_galoisFieldElements *field, struct_galoisFieldPolyForm *p)//minus
{
	char_GALOIS_FIELD_VALUE *buffer;

	struct_galoisFieldPolyForm *elementBuffer;

	unsigned int i;

	#ifndef RELEASE
	if(!field)
	{
		errorMes;
		printf("in invertValueElementOfGaloisField, struct_galoisFieldElements *field is NULL.\n");
		return NULL;
	}
	if(!p)
	{
		errorMes;
		printf("in invertValueElementOfGaloisField, struct_galoisFieldPolyForm *p is NULL.\n");
		return NULL;
	}
	#endif
	
	buffer = (char_GALOIS_FIELD_VALUE*)malloc(sizeof(char_GALOIS_FIELD_VALUE)*(p->length+1));
	
	for(i=0; i<p->length; i++)
	{
		if(*(p->value+i)=='0')
		{
			*(buffer+i)='0';
			continue;
		}
		else
		{
			*(buffer+i)='1';
			i++;
			break;
		}
	}
	
	for(; i<p->length; i++)
	{
		if(*(p->value+i)=='0')
		{
			*(buffer+i)='1';
		}
		else if(*(p->value+i)=='1')
		{
			*(buffer+i)='0';
		}
		else
		{
					#ifndef RELEASE
					errorMes; printf("in invertValueElementOfGaloisField, wrong character : %c(dec:%d, hex:0x%x).\n", *(p->value+i), *(p->value+i), *(p->value+i));
					#endif
			free(buffer);
			return NULL;
		}
	}
	*(buffer+i)=0;
		
	elementBuffer=findSameElementOfGaloisFieldUsingString(field, buffer);
	free(buffer);
	return elementBuffer;
}
//////////////////////////////////////////////////////////////////////////////////
unsigned int abstractIndexOfPowerFormInElementsOfGaloisField_finding(struct_galoisFieldElements *field, struct_galoisFieldPolyForm *p)
{
	unsigned int i;

	#ifndef RELEASE
	if((*(field->element))->length!=p->length)
	{
		errorMes;
		printf("in abstractIndexOfPowerFormInElementsOfGaloisField_finding, field poly length: %d, target field poly length: %d.\n", (*(field->element))->length, p->length);
		return -1;
	}
	#endif
	
	
	for(i=0; i<field->length; i++)
	{
		if(checkValueFromPolyFormUsingGaloisFieldValue (*(field->element+i), p)) return i;
	}
			#ifndef RELEASE
			printf("can't abstract exponential from GF.\n");
			#endif
	return -1;
}
//////////////////////////////////////////////////////////////////////////////////
unsigned int abstractIndexOfPowerFormInElementsOfGaloisFieldUsingIndex(struct_galoisFieldElements *field, struct_galoisFieldPolyForm *p)
{
	unsigned int i;

	if((*(field->element))->length!=p->length)
	{
		errorMes;
		printf("in abstractIndexOfPowerFormInElementsOfGaloisFieldUsingIndex, field poly length: %d, target field poly length: %d.\n", (*(field->element))->length, p->length);
		return -1;
	}
	
	return p->index;
	// for(i=0; i<field->length; i++)
	// {
		// if(checkValueFromPolyFormUsingGaloisFieldValue (*(field->element+i), p)) return i;
	// }
}
//////////////////////////////////////////////////////////////////////////////////
unsigned int abstractIndexOfPowerFormInElementsOfGaloisFieldUsingIntValue(struct_galoisFieldElements *field, struct_galoisFieldPolyForm *p)
{
	unsigned int i;

	#ifndef RELEASE
	if((*(field->element))->length!=p->length)
	{
		errorMes;
		printf("in abstractIndexOfPowerFormInElementsOfGaloisFieldUsingIntValue, field poly length: %d, target field poly length: %d.\n", (*(field->element))->length, p->length);
		return -1;
	}
	#endif
	
	for(i=0; i<field->length; i++)
	{
		if(checkValueFromPolyFormUsingGaloisFieldValueUsingIntValue_(*(field->element+i), p)) return i;
	}
			#ifndef RELEASE
			printf("can't abstract exponential from GF.\n");
			#endif
	return -1;
}
//////////////////////////////////////////////////////////////////////////////////
struct_galoisFieldPolyForm *invertExponentialElementInGaloisField(struct_galoisFieldElements *field, struct_galoisFieldPolyForm *operand)
{
	unsigned int exponential;

	#ifndef RELEASE
	if(!field)
	{
		errorMes;
		printf("in *invertExponentialElementInGaloisField, struct_galoisFieldElements *field is NULL.\n");
	}
	if(!operand)
	{
		errorMes;
		printf("in *invertExponentialElementInGaloisField, struct_galoisFieldPolyForm *operand is NULL.\n");
	}
	#endif
	
	
	exponential = abstractIndexOfPowerFormInElementsOfGaloisFieldUsingIndex(field, operand);
	if(!exponential)
	{
		return *(field->element+0);
	}
	exponential-=1;
	exponential=((field->length-1)-exponential)%(field->length-1);
	
	return *(field->element+exponential+1);
}
//////////////////////////////////////////////////////////////////////////////////
/*Use character, but it is slow, so this function is alternated.*/
struct_galoisFieldPolyForm *summationElementsInGaloisFieldUsingGaloisFieldValue(struct_galoisFieldElements *field, struct_galoisFieldPolyForm *operandA, struct_galoisFieldPolyForm *operandB)
{
	unsigned int i;

	struct_galoisFieldPolyForm *temp;

	#ifndef RELEASE
	if(!field)
	{
		errorMes;
		printf("in summationElementsInGaloisFieldUsingGaloisFieldValue, struct_galoisFieldElements *field is NULL.\n");
		return NULL;
	}
	if(!operandA)
	{
		errorMes;
		printf("in summationElementsInGaloisFieldUsingGaloisFieldValue, struct_galoisFieldPolyForm *operandA is NULL.\n");
		return NULL;
	}
	if(!operandB)
	{
		errorMes;
		printf("in summationElementsInGaloisFieldUsingGaloisFieldValue, struct_galoisFieldPolyForm *operandB is NULL.\n");
		return NULL;
	}
	#endif


	#ifndef RELEASE
	if((*(field->element))->length!=operandA->length)
	{
		errorMes;
		printf("in *summationElementsInGaloisFieldUsingGaloisFieldValue, field poly length: %d, operandA field poly length: %d\n", (*(field->element))->length, operandA->length);
		return NULL;
	}
	if(operandB->length!=operandA->length)
	{
		errorMes;
		printf("in *summationElementsInGaloisFieldUsingGaloisFieldValue, operandA poly length: %d, operandA poly length: %d\n", operandA->length, operandB->length);
		return NULL;
	}
	#endif

	temp = createPolyForm((*(field->element))->length);
	#ifdef USING_OPEN_MP
	#pragma omp parallel for schedule(guided) private(i) shared(field, operandA, operandB, temp)
	#endif
	for(i=0; i<(*(field->element))->length; i++)
	{
		if((*(operandA->value+i))==(*(operandB->value+i))) (*(temp->value+i)) = '0';
		else (*(temp->value+i)) = '1';
	}

	for(i=0; i<field->length; i++)
	{
		if(checkValueFromPolyFormUsingGaloisFieldValueUsingIntValue_(*(field->element+i), temp))
		{
			closePolyForm(&temp);
			return (*(field->element+i));
		}
	}
	
			#ifndef RELEASE
			errorMes; printf("in *summationElementsInGaloisFieldUsingGaloisFieldValue, checkValueFromPolyFormUsingGaloisFieldValueUsingIntValue_(*(field->element+i), temp) is not working...\n");
			errorMes; printf("operandA->length = \'%d\', ADDR : 0x%x, operandB->length = \'%d\', ADDR : 0x%x\n", operandA->length, operandA, operandB->length, operandB);
			#endif
	closePolyForm(&temp);
	return NULL;
}
//////////////////////////////////////////////////////////////////////////////////
struct_galoisFieldPolyForm *summationElementsInGaloisFieldUsingGaloisFieldInt_finding(struct_galoisFieldElements *field, struct_galoisFieldPolyForm *operandA, struct_galoisFieldPolyForm *operandB)
{
	unsigned int i;

	u_int_GALOIS_FIELD_INTEGER temp;

	#ifndef RELEASE
	if(!field)
	{
		errorMes;
		printf("in summationElementsInGaloisFieldUsingGaloisFieldInt_finding, struct_galoisFieldElements *field is NULL.\n");
		return NULL;
	}
	if(!operandA)
	{
		errorMes;
		printf("in summationElementsInGaloisFieldUsingGaloisFieldInt_finding, struct_galoisFieldPolyForm *operandA is NULL.\n");
		return NULL;
	}
	if(!operandB)
	{
		errorMes;
		printf("in summationElementsInGaloisFieldUsingGaloisFieldInt_finding, struct_galoisFieldPolyForm *operandB is NULL.\n");
		return NULL;
	}
	#endif


	#ifndef RELEASE
	if((*(field->element))->length!=operandA->length)
	{
		errorMes;
		printf("in *summationElementsInGaloisFieldUsingGaloisFieldInt_finding, field poly length: %d, operandA field poly length: %d\n", (*(field->element))->length, operandA->length);
		return NULL;
	}
	if(operandB->length!=operandA->length)
	{
		errorMes;
		printf("in *summationElementsInGaloisFieldUsingGaloisFieldInt_finding, operandA poly length: %d, operandA poly length: %d\n", operandA->length, operandB->length);
		return NULL;
	}
	#endif

	temp=((operandA->intValue)^(operandB->intValue));
	for(i=0; i<field->length; i++)
	{
		if(!(((*(field->element+i))->intValue) ^ temp))
		{
			return (*(field->element+i));
		}
	}
	
			#ifndef RELEASE
			errorMes; printf("in summationElementsInGaloisFieldUsingGaloisFieldInt_finding, can't find same integer value(%d ^ %d = %d).\n", (operandA->intValue), (operandB->intValue), temp);
			#endif
	return NULL;
}


//////////////////////////////////////////////////////////////////////////////////
struct_summationMatrix *createSummationMatrix_emptySpace(unsigned int row, unsigned int column)
{
	unsigned int i;
	struct_summationMatrix *p;
	
	#ifndef RELEASE
	if(!row)
	{
		return NULL;
	}
	if(!column)
	{
		return NULL;
	}
	if(row!=column)
	{
		return NULL;
	}
	#endif
	
	
	p=(struct_summationMatrix*)malloc(sizeof(struct_summationMatrix));
	
	p->element=(struct_galoisFieldPolyForm ***)malloc(sizeof(struct_galoisFieldPolyForm **)*row);
	for(i=0; i<row; i++)
	{
		(*(p->element+i))=(struct_galoisFieldPolyForm **)malloc(sizeof(struct_galoisFieldPolyForm *)*column);
	}

	p->row=row;
	p->column=column;
	return p;
}
//////////////////////////////////////////////////////////////////////////////////
char closeSummationMatrix(struct_summationMatrix **p)
{
	unsigned int i;
	if(!p)
	{
		return -1;
	}
	if(!(*p))
	{
		return -1;
	}
	#ifdef USING_OPEN_MP
	#pragma omp parallel for schedule(guided) private(i) shared(p)
	#endif
	for(i=0; i<(*p)->row; i++)
	{
		free(*((*p)->element+i));
	}
	free((*p)->element);
	
	free(*p);
	
	*p=NULL;
	return 0;
}
//////////////////////////////////////////////////////////////////////////////////
char calculateSummationMatrix(struct_galoisFieldElements *galoisFields, struct_summationMatrix *p)
{
	unsigned int i,j;
	
	unsigned int tmp_row;
	unsigned int tmp_column;
	
	#ifndef RELEASE
	if(!galoisFields)
	{
		return -1;
	}
	
	if(!p)
	{
		return -2;
	}
	
	if(!(p->row))
	{
		return -3;
	}
	if(!(p->column))
	{
		return -4;
	}
	#endif
	
	tmp_row = p->row;
	tmp_column = p->column;
	
			#ifndef RELEASE
			infoMes;  printf("calculating a struct_summationMatrix ");
			#endif
	#ifdef USING_OPEN_MP
	#pragma omp parallel for schedule(guided) private(i, j) shared(tmp_row, p, tmp_column, galoisFields)
	#endif
	for(i=0; i<tmp_row; i++)
	{
		for(j=0; j<tmp_column; j++)
		{
			//summationElementsInGaloisFieldUsingGaloisFieldInt_finding is return (*(field->element+i));
			(*((*(p->element+i))+j))=summationElementsInGaloisFieldUsingGaloisFieldInt_finding(galoisFields, *(galoisFields->element+i), *(galoisFields->element+j));
		}
	}
			#ifndef RELEASE
			printf("is done.\n");
			#endif

	return 0;
}
//////////////////////////////////////////////////////////////////////////////////
/* 20160425_18:07 deleted */
/* struct_summationMatrix *createSummationMatrix(struct_galoisFieldElements *galoisFields, unsigned int row, unsigned int column)
{
	unsigned int i,j;
	struct_summationMatrix *p;
	
	unsigned int count;
	
	#ifndef RELEASE
	if(!galoisFields)
	{
		return NULL;
	}
	if(!row)
	{
		return NULL;
	}
	if(!column)
	{
		return NULL;
	}
	if(row!=column)
	{
		return NULL;
	}
	#endif
	
	
	p=(struct_summationMatrix*)malloc(sizeof(struct_summationMatrix));
	
	p->element=(struct_galoisFieldPolyForm ***)malloc(sizeof(struct_galoisFieldPolyForm **)*row);
	
			#ifndef RELEASE
			infoMes;  printf("calculating a struct_summationMatrix ");
			#endif

	#ifdef USING_OPEN_MP
	#pragma omp parallel for schedule(guided) private(i, j) shared(row, p, column, galoisFields)
	#endif
	for(i=0; i<row; i++)
	{
		(*(p->element+i))=(struct_galoisFieldPolyForm **)malloc(sizeof(struct_galoisFieldPolyForm *)*column);
		for(j=0; j<column; j++)
		{
			(*((*(p->element+i))+j))=summationElementsInGaloisFieldUsingGaloisFieldInt_finding(galoisFields, *(galoisFields->element+i), *(galoisFields->element+j));
		}
	}
			#ifndef RELEASE
			printf("is done.\n");
			#endif
	
	p->row=row;
	p->column=column;
	return p;
} */
struct_summationMatrix *createSummationMatrix(struct_galoisFieldElements *galoisFields, unsigned int in_row, unsigned int in_column)
{
	unsigned int i,j;
	struct_summationMatrix *p;
	
	p=createSummationMatrix_emptySpace(in_row, in_column);
	if(calculateSummationMatrix(galoisFields, p))
	{
		errorMes; printf("in createSummationMatrix, calculateSummationMatrix has error(close).\r\n");
		closeSummationMatrix(&p);
	}
	return p;
}
//////////////////////////////////////////////////////////////////////////////////
void printSummationMatrixSavedForm(struct_summationMatrix *p)
{
	unsigned int tmp_i,tmp_j;
	unsigned int exponentialTemp;

	#ifndef RELEASE
	if(!p)
	{
		errorMes;
		printf("in printSummationMatrixSavedForm, struct_summationMatrix *p is NULL\r\n");
		return;
	}
	#endif
	
	printf(">> Summation Matrix File I/O Form <<\n");
	
	for(tmp_i=0; tmp_i<p->row; tmp_i++)
	{
		for(tmp_j=0; tmp_j<p->column; tmp_j++)
		{
			if((*((*(p->element+tmp_i))+tmp_j))->index)
			{
				printf("%d(%d)%s", 
					((*((*(p->element+tmp_i))+tmp_j))->index - 1), 
					(*((*(p->element+tmp_i))+tmp_j))->index, 
					((tmp_j<(p->column-1))?", ":"")
				);
			}
			else
			{
				printf("%c(%d)%s", 
					'-', 
					(*((*(p->element+tmp_i))+tmp_j))->index, 
					((tmp_j<(p->column-1))?", ":"")
				);	

			}
			
		}
		printf("\r\n");
	}
}
//////////////////////////////////////////////////////////////////////////////////
/* 
struct struct_summationMatrix
{
	unsigned int row;
	unsigned int column;
	struct_galoisFieldPolyForm ***element;
}typedef struct_summationMatrix;
 */
char save_struct_summationMatrix(char *path, char *primitivePoly, struct_summationMatrix *p)
{
	unsigned int tmp_i;
	unsigned int tmp_j;
	FILE *fileio;
	if(!path)			return -1;
	if(!primitivePoly)	return -2;
	if(!p)				return -3;
	
	fileio = fopen(path, "w");
	
	fprintf(fileio, "primitive polynomials, %s\r\n", primitivePoly);


	fprintf(fileio, "%s\r\n", FILE_IO_DATA_PARSING_KEY_STRING_SAVED_DATA_START);
	
	fprintf(fileio, "%s\r\n", FILE_IO_DATA_PARSING_KEY_STRING_KIND_OF_DATA_START);
	fprintf(fileio, "Kind Of Data, %d\r\n", FILE_IO_KIND_OF_DATA_SUMATION_MATRIX);
	fprintf(fileio, "%s\r\n", FILE_IO_DATA_PARSING_KEY_STRING_KIND_OF_DATA_END);

	
	fprintf(fileio, "%s\r\n", FILE_IO_DATA_PARSING_KEY_STRING_ARRAY_TYPE_START);
	fprintf(fileio, "Saved Data Type, %d\r\n", FILD_IO_DATA_ARRAY_TYPE_LINEAR);
	fprintf(fileio, "%s\r\n", FILE_IO_DATA_PARSING_KEY_STRING_ARRAY_TYPE_END);

	fprintf(fileio, "%s\r\n", FILE_IO_DATA_PARSING_KEY_STRING_BIT_WIDTH_START);
	fprintf(fileio, "Bit Width, %d\r\n", (*((*(p->element+0))+0))->length);
	fprintf(fileio, "%s\r\n", FILE_IO_DATA_PARSING_KEY_STRING_BIT_WIDTH_END);
	

	fprintf(fileio, "%s\r\n", FILE_IO_DATA_PARSING_KEY_STRING_ROW_DATA_START);
	fprintf(fileio, "Matrix row length, %d\r\n", p->row);
	fprintf(fileio, "%s\r\n", FILE_IO_DATA_PARSING_KEY_STRING_ROW_DATA_END);

	
	fprintf(fileio, "%s\r\n", FILE_IO_DATA_PARSING_KEY_STRING_COL_DATA_START);
	fprintf(fileio, "Matrix column length, %d\r\n", p->column);
	fprintf(fileio, "%s\r\n", FILE_IO_DATA_PARSING_KEY_STRING_COL_DATA_END);
	
	fprintf(fileio, "pow(index)\r\n");

	fprintf(fileio, "%s\r\n", FILE_IO_DATA_PARSING_KEY_STRING_RAW_DATA_START);
	for(tmp_i=0; tmp_i<p->row; tmp_i++)
	{
		for(tmp_j=0; tmp_j<p->column; tmp_j++)
		{
			if((*((*(p->element+tmp_i))+tmp_j))->index)
			{
				fprintf(fileio, "%d(%d)%s", 
					((*((*(p->element+tmp_i))+tmp_j))->index - 1), 
					(*((*(p->element+tmp_i))+tmp_j))->index, 
					((tmp_j<(p->column-1))?", ":"")
				);
			}
			else
			{
				fprintf(fileio, "%c(%d)%s", 
					'-', 
					(*((*(p->element+tmp_i))+tmp_j))->index, 
					((tmp_j<(p->column-1))?", ":"")
				);	

			}
			
		}
		fprintf(fileio, "\r\n");
	}
	fprintf(fileio, "%s\r\n", FILE_IO_DATA_PARSING_KEY_STRING_RAW_DATA_END);
	
	fprintf(fileio, "%s\r\n", FILE_IO_DATA_PARSING_KEY_STRING_SAVED_DATA_END);
	
	fclose(fileio);
		
	return 0;
}
char load_struct_summationMatrix(char *path, char *primitivePoly, struct_galoisFieldElements *galoisFields, struct_summationMatrix *p)
{
	unsigned int tmp_i=0;
	unsigned int tmp_j=0;
	FILE *fileio_load_sumMatrix;
	
	unsigned int primitivePoly_length=0;
	char load_primitivePoly[1025];
	
	int load_kindOfData=0;
	int load_saveOfData=0;
	
	unsigned int load_bitWidth;
	unsigned int load_row;
	unsigned int load_column;
	
	unsigned int load_index;
	// char load_exp_str[256]={0};
	char load_exp_char=0;
	unsigned int load_exp_integer=0;
	
	
	unsigned int load_integer=0;

	char charBuf=0;
	char load_stringBuf[1025]={0};
	
	unsigned int chk_incorrectFormCnt=0;
	unsigned int chk_incorrectDataCnt=0;
	
	if(!path)			return -1;
	if(!primitivePoly)	return -2;
	if(!p)				return -3;
	
	/* condition chk */
	if(!(primitivePoly_length=strlen(primitivePoly)))
	{
		return -4;
	}
	
	
	/* buffer set up to varify */
	
	
	fileio_load_sumMatrix = fopen(path, "r");
	
				printf("[load_struct_summationMatrix START]\r\n");
	
	if(fscanf(fileio_load_sumMatrix, "primitive polynomials, %s\r\n", load_primitivePoly)<0)			chk_incorrectFormCnt++;
			// printf("primitive polynomials, %s\r\n", load_primitivePoly);
		// if(strcmp(load_primitivePoly, primitivePoly))
		// {
			// chk_incorrectDataCnt++;
		// }

	if(fscanf(fileio_load_sumMatrix, "%s\r\n", load_stringBuf)<0)		chk_incorrectFormCnt++;
		// printf("%s\r\n", load_stringBuf);//FILE_IO_DATA_PARSING_KEY_STRING_SAVED_DATA_START
	
	if(fscanf(fileio_load_sumMatrix, "%s\r\n", load_stringBuf)<0)	chk_incorrectFormCnt++;
		// printf("%s\r\n", load_stringBuf);// FILE_IO_DATA_PARSING_KEY_STRING_KIND_OF_DATA_START
	if(fscanf(fileio_load_sumMatrix, "Kind Of Data, %d\r\n", &load_kindOfData)<0)	chk_incorrectFormCnt++;
		// printf("Kind Of Data, %d\r\n", load_kindOfData);// FILE_IO_KIND_OF_DATA_SUMATION_MATRIX
	if(fscanf(fileio_load_sumMatrix, "%s\r\n", load_stringBuf)<0)		chk_incorrectFormCnt++;
		// printf("%s\r\n", load_stringBuf);// FILE_IO_DATA_PARSING_KEY_STRING_KIND_OF_DATA_END

	
	if(fscanf(fileio_load_sumMatrix, "%s\r\n", load_stringBuf)<0)		chk_incorrectFormCnt++;
		// printf("%s\r\n", load_stringBuf);// FILE_IO_DATA_PARSING_KEY_STRING_ARRAY_TYPE_START
	if(fscanf(fileio_load_sumMatrix, "Saved Data Type, %d\r\n", &load_saveOfData)<0)		chk_incorrectFormCnt++;
		// printf("Saved Data Type, %d\r\n", load_saveOfData);// FILD_IO_DATA_ARRAY_TYPE_LINEAR
	if(fscanf(fileio_load_sumMatrix, "%s\r\n", load_stringBuf)<0)		chk_incorrectFormCnt++;
		// printf("%s\r\n", load_stringBuf);// FILE_IO_DATA_PARSING_KEY_STRING_ARRAY_TYPE_END

	if(fscanf(fileio_load_sumMatrix, "%s\r\n", load_stringBuf)<0)		chk_incorrectFormCnt++;
		// printf("%s\r\n", load_stringBuf);// FILE_IO_DATA_PARSING_KEY_STRING_BIT_WIDTH_START
	if(fscanf(fileio_load_sumMatrix, "Bit Width, %d\r\n", &load_bitWidth)<0)								chk_incorrectFormCnt++;
			// printf("Bit Width, %d\r\n", load_bitWidth);
		// if((*((*(p->element+0))+0))->length != load_bitWidth)
		// {
			// chk_incorrectDataCnt++;
		// }
	if(fscanf(fileio_load_sumMatrix, "%s\r\n", load_stringBuf)<0)		chk_incorrectFormCnt++;
		// printf("%s\r\n", load_stringBuf);// FILE_IO_DATA_PARSING_KEY_STRING_BIT_WIDTH_END
	

	if(fscanf(fileio_load_sumMatrix, "%s\r\n", load_stringBuf)<0)		chk_incorrectFormCnt++;
		// printf("%s\r\n", load_stringBuf);// FILE_IO_DATA_PARSING_KEY_STRING_ROW_DATA_START
	if(fscanf(fileio_load_sumMatrix, "Matrix row length, %d\r\n", &load_row)<0)							chk_incorrectFormCnt++;
			// printf("Matrix row length, %d\r\n", load_row);
		// if(p->row != load_row)
		// {
			// chk_incorrectDataCnt++;	
		// }
	
	if(fscanf(fileio_load_sumMatrix, "%s\r\n", load_stringBuf)<0)			chk_incorrectFormCnt++;
		// printf("%s\r\n", load_stringBuf);// FILE_IO_DATA_PARSING_KEY_STRING_ROW_DATA_END

	
	if(fscanf(fileio_load_sumMatrix, "%s\r\n", load_stringBuf)<0)		chk_incorrectFormCnt++;
		// printf("%s\r\n", load_stringBuf);// FILE_IO_DATA_PARSING_KEY_STRING_COL_DATA_START
	if(fscanf(fileio_load_sumMatrix, "Matrix column length, %d\r\n", &load_column)<0)					chk_incorrectFormCnt++;
			// printf("Matrix column length, %d\r\n", load_column);
		// if(p->column != load_column)
		// {
			// chk_incorrectDataCnt++;
		// }
	if(fscanf(fileio_load_sumMatrix, "%s\r\n", load_stringBuf)<0)			chk_incorrectFormCnt++;
		// printf("%s\r\n", load_stringBuf);// FILE_IO_DATA_PARSING_KEY_STRING_COL_DATA_END
	
	if(fscanf(fileio_load_sumMatrix, "%s\r\n", load_stringBuf)<0)												chk_incorrectFormCnt++;
		// printf("%s\r\n", load_stringBuf);

	if(fscanf(fileio_load_sumMatrix, "%s\r\n", load_stringBuf)<0)		chk_incorrectFormCnt++;
		// printf("%s\r\n", load_stringBuf);// FILE_IO_DATA_PARSING_KEY_STRING_RAW_DATA_START
	for(tmp_i=0; tmp_i<load_row; tmp_i++)
	{
		for(tmp_j=0; tmp_j<load_column; tmp_j++)
		{
			if(tmp_i != tmp_j)
			{
				if(tmp_j<(load_column-1))
				{
					if(fscanf(fileio_load_sumMatrix, "%d(%d), ", 
						&load_exp_integer,
						&load_index
					)<0)	chk_incorrectFormCnt++;
				}
				else
				{
					if(fscanf(fileio_load_sumMatrix, "%d(%d)", 
						&load_exp_integer,
						&load_index
					)<0)	chk_incorrectFormCnt++;
				}
					// printf("%d(%d)%s", load_exp_integer, load_index, ((tmp_j<(load_column-1))?", ":""));
				(*((*(p->element+tmp_i))+tmp_j)) = (*(galoisFields->element+load_index));
			}
			else
			{
				if(tmp_j<(load_column-1))
				{
					if(fscanf(fileio_load_sumMatrix, "%c(%d), ", 
						&load_exp_char,
						&load_index
					)<0)	chk_incorrectFormCnt++;
				}
				else
				{
					if(fscanf(fileio_load_sumMatrix, "%c(%d)", 
						&load_exp_char,
						&load_index
					)<0)	chk_incorrectFormCnt++;
				}
					// printf("%c(%d)%s", load_exp_char, load_index, ((tmp_j<(load_column-1))?", ":""));
				(*((*(p->element+tmp_i))+tmp_j)) = (*(galoisFields->element+load_index));
			}
		}
		if(fscanf(fileio_load_sumMatrix, "\r\n")<0)	chk_incorrectFormCnt++;
			// printf("\r\n");
	}
	if(fscanf(fileio_load_sumMatrix, "%s\r\n", load_stringBuf)<0)			chk_incorrectFormCnt++;
		// printf("%s\r\n", load_stringBuf);// FILE_IO_DATA_PARSING_KEY_STRING_RAW_DATA_END
	if(fscanf(fileio_load_sumMatrix, "%s\r\n", load_stringBuf)<0)		chk_incorrectFormCnt++;
		// printf("%s\r\n", load_stringBuf);// FILE_IO_DATA_PARSING_KEY_STRING_SAVED_DATA_END
	
	fclose(fileio_load_sumMatrix);
	
				printf("[load_struct_summationMatrix END]\r\n");
	
			#ifndef RELEASE
			if(global_flag_gfCommon_display&FLAG_MASK_DISPLAY_GF_COMMON_FILE_IO)	
			{
				printf(">> Loaded Summation Matrix <<\r\n");
				printSummationMatrixSavedForm(p);
			}
			#endif


	
	if(chk_incorrectFormCnt || chk_incorrectDataCnt)	return 1;
	else												return 0;
}
//////////////////////////////////////////////////////////////////////////////////
struct_galoisFieldPolyForm *sumElementInGF_usingSumMatrixReturnAddr(struct_galoisFieldElements *field, struct_galoisFieldPolyForm *operandA, struct_galoisFieldPolyForm *operandB)
{
	unsigned int i;

	u_int_GALOIS_FIELD_INTEGER temp;

	#ifndef RELEASE
	if(!field)
	{
		errorMes;
		printf("in sumElementInGF_usingSumMatrixReturnAddr, struct_galoisFieldElements *field is NULL.\n");
		return NULL;
	}
	if(!operandA)
	{
		errorMes;
		printf("in sumElementInGF_usingSumMatrixReturnAddr, struct_galoisFieldPolyForm *operandA is NULL.\n");
		return NULL;
	}
	if(!operandB)
	{
		errorMes;
		printf("in sumElementInGF_usingSumMatrixReturnAddr, struct_galoisFieldPolyForm *operandB is NULL.\n");
		return NULL;
	}
	if(!field->summationReferMatrix)
	{
		errorMes;
		printf("in sumElementInGF_usingSumMatrixReturnAddr, if(!field->summationReferMatrix)\n");
		return NULL;
	}
	if(field->summationReferMatrix->element[0][0]->length!=operandA->length)
	{
		errorMes;
		printf("in *sumElementInGF_usingSumMatrixReturnAddr, field poly length: %d, operandA field poly length: %d\n", field->summationReferMatrix->element[0][0]->length, operandA->length);
		return NULL;
	}
	if(operandB->length!=operandA->length)
	{
		errorMes;
		printf("in *sumElementInGF_usingSumMatrixReturnAddr, operandA poly length: %d, operandA poly length: %d\n", operandA->length, operandB->length);
		return NULL;
	}
	#endif

	return field->summationReferMatrix->element[operandA->index][operandB->index];
	//return (*((*(matrix->element+(operandA->index)))+(operandB->index)));
}
//////////////////////////////////////////////////////////////////////////////////
/* unsigned int multiplicationWithExponential(struct_galoisFieldElements *field, unsigned int exponentialA, unsigned int exponentialB)
{

	return 
} */
//////////////////////////////////////////////////////////////////////////////////
struct_galoisFieldPolyForm *multiElementsInGF_returnAddr(struct_galoisFieldElements *field, struct_galoisFieldPolyForm *operandA, struct_galoisFieldPolyForm *operandB)
{
	unsigned int exponentialOfOperandA;
	unsigned int exponentialOfOperandB;
	unsigned int exponentialSummation;

	#ifndef RELEASE
	if(!field)
	{
		errorMes;
		printf("in multiElementsInGF_returnAddr, struct_galoisFieldElements *field is NULL.\n");
		return NULL;
	}
	if(!operandA)
	{
		errorMes;
		printf("in multiElementsInGF_returnAddr, struct_galoisFieldPolyForm *operandA is NULL.\n");
		return NULL;
	}
	if(!operandB)
	{
		errorMes;
		printf("in multiElementsInGF_returnAddr, struct_galoisFieldPolyForm *operandB is NULL.\n");
		return NULL;
	}
	#endif

	exponentialOfOperandA=abstractIndexOfPowerFormInElementsOfGaloisFieldUsingIndex(field, operandA);
	#ifndef RELEASE
	if(exponentialOfOperandA==-1)
	{
		errorMes; printf("in multiElementsInGF_returnAddr, (exponentialOfOperandA=abstractIndexOfPowerFormInElementsOfGaloisField_finding(field, operandA))==-1\n");
		return NULL;
	}
	#endif
	
	exponentialOfOperandB=abstractIndexOfPowerFormInElementsOfGaloisFieldUsingIndex(field, operandB);
	#ifndef RELEASE
	if(exponentialOfOperandB==-1)
	{
		errorMes; printf("in multiElementsInGF_returnAddr, (exponentialOfOperandA=abstractIndexOfPowerFormInElementsOfGaloisField_finding(field, operandB))==-1\n");
		return NULL;
	}
	#endif

	if((exponentialOfOperandA==0)||(exponentialOfOperandB==0))
	{
		return *(field->element+0);
	}

	if(exponentialOfOperandA==1)
	{
		return *(field->element+exponentialOfOperandB);
	}
	else if(exponentialOfOperandB==1)
	{
		return *(field->element+exponentialOfOperandA);
	}
	
	exponentialSummation=((exponentialOfOperandA-1)+(exponentialOfOperandB-1))%(field->length-1);

			#ifndef RELEASE
			if(((global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)==FLAG_MASK_PRINTF_LOG)&&((global_flag_cmdOption&FLAG_MASK_PRINTF_CALCULATE_PROCESS)==FLAG_MASK_PRINTF_CALCULATE_PROCESS))
			{
				logMes;
				printf("in multiElementsInGF_returnAddr, (exp(A)+exp(B)) %% (field->length-1) => ");
				printf("%d %% %d = %d\n", (exponentialOfOperandA+exponentialOfOperandB), (field->length-1), (exponentialOfOperandA+exponentialOfOperandB)%(field->length-1));
			}
			#endif

	return *(field->element+exponentialSummation+1);
}
//////////////////////////////////////////////////////////////////////////////////
// struct struct_summationMatrix
// {
	// unsigned int row;
	// unsigned int column;
	// struct_galoisFieldPolyForm ***element;
// }typedef struct_summationMatrix;
// void printSummationMatrixGFabstraction(struct_summationMatrix *p)
// {
	// unsigned int tmp_row;
	// unsigned int tmp_col;

	// #ifndef RELEASE
	// if(!p)
	// {
		
		// return;
	// }
	// #endif
	
	// for(tmp_row=0; tmp_row<p->row; tmp_row++)
	// {
		// for(tmp_col=0; tmp_col<p->column; tmp_col++)
		// {
			
		// }
	// }
// }
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
///*Example of making equation*///
/*
	struct_galoisFieldElements *alpha;
	struct_galoisFieldElements *beta;

	p->alpha = createGaloisFieldExceptElements(t+1);
	p->beta = createGaloisFieldExceptElements(t+1);

	initEquation(galoisFields, p->alpha);
	initEquation(galoisFields, p->beta);
	
	copyListOfGaloisField(p->beforeAlpha->element, p->alpha->element, p->alpha->length);
*/
char initEquation(struct_galoisFieldElements *galoisFields, struct_galoisFieldElements *p)
{
	unsigned int i;

	#ifndef RELEASE
	if(!galoisFields)
	{
		errorMes;
		printf("in initEquation, struct_galoisFieldElements *galoisFields is zero\n");
		return -1;
	}
	if(!p)
	{
		errorMes;
		printf("in initEquation, struct_galoisFieldElements *p\n");
		return -1;
	}
	if(!(p->length))
	{
		errorMes;
		printf("in initEquation, p->length is zero\n");
		return -1;
	}
	#endif

	
	(*(p->element+0)) = (*(galoisFields->element+1));
	#ifdef USING_OPEN_MP
	#pragma omp parallel for schedule(guided) private(i) shared(p, galoisFields)
	#endif
	for(i=1; i<p->length; i++)
	{
		(*(p->element+i)) = (*(galoisFields->element+0));
	}
	return 0;
}
//////////////////////////////////////////////////////////////////////////////////
char initEquationFillAllZero(struct_galoisFieldElements *galoisFields, struct_galoisFieldElements *p)
{
	unsigned int i;

	#ifndef RELEASE
	if(!galoisFields)
	{
		errorMes;
		printf("in initEquation, struct_galoisFieldElements *galoisFields is zero\n");
		return -1;
	}
	if(!p)
	{
		errorMes;
		printf("in initEquation, struct_galoisFieldElements *p\n");
		return -1;
	}
	if(!(p->length))
	{
		errorMes;
		printf("in initEquation, p->length is zero\n");
		return -1;
	}
	#endif

	
	#ifdef USING_OPEN_MP
	#pragma omp parallel for schedule(guided) private(i) shared(p, galoisFields)
	#endif
	for(i=0; i<p->length; i++)
	{
		(*(p->element+i)) = (*(galoisFields->element+0));
	}
	return 0;
}
//////////////////////////////////////////////////////////////////////////////////
char initBinEquationUsingStr(struct_galoisFieldElements *galoisFields, struct_galoisFieldElements *p, char *str, unsigned int strlen)
{
	unsigned int i;
	unsigned int tmp_cnt;

	#ifndef RELEASE
	if(!galoisFields)
	{
		errorMes;
		printf("in initBinEquationUsingStr, struct_galoisFieldElements *galoisFields is zero\n");
		return -1;
	}
	if(!p)
	{
		errorMes;
		printf("in initBinEquationUsingStr, struct_galoisFieldElements *p\n");
		return -1;
	}
	if(!(p->length))
	{
		errorMes;
		printf("in initBinEquationUsingStr, p->length is zero\n");
		return -1;
	}
	if(!str)
	{
		errorMes;
		printf("in initBinEquationUsingStr, char *str is NULL\n");
		return -1;
	}
	if(!strlen)
	{
		errorMes;
		printf("in initBinEquationUsingStr, unsigned int strlen is zero\n");
		return -1;
	}
	#endif

	
	tmp_cnt=0;
	// #ifdef USING_OPEN_MP
	// #pragma omp parallel for schedule(guided) private(i) shared(p, galoisFields)
	// #endif
	for(i=0; i<strlen; i++)
	{
		if(str[i]=='1')
		{
			(*(p->element+i)) = (*(galoisFields->element+1));
		}
		else if(str[i]=='0')
		{
			(*(p->element+i)) = (*(galoisFields->element+0));
		}
		else
		{
			tmp_cnt++;
			(*(p->element+i)) = (*(galoisFields->element+1));
		}
	}
	
	if(tmp_cnt)
	{
		warningMesShort; printf("in initBinEquationUsingStr, %d characters are not 1 or 0 among %d characters.\n", tmp_cnt, i);
		warningMesShort; printf("in initBinEquationUsingStr, %d characters are alternated to 1.\n", tmp_cnt);
	}
	
	return 0;
}
//////////////////////////////////////////////////////////////////////////////////
struct_galoisFieldElements* multipleTwoEqReturnAddr(
	struct_galoisFieldElements *galoisFields, 
	struct_galoisFieldElements **result, 
	struct_galoisFieldElements *operandEqA, 
	struct_galoisFieldElements *operandEqB
)
{
	unsigned int tmp_order=0;
	struct_galoisFieldPolyForm *tmp_calBuf=NULL;
	
	unsigned int i;
	unsigned int j;
	#ifndef RELEASE
	if(!galoisFields)
	{
		errorMes;
		printf("in multipleTwoEqReturnAddr, struct_galoisFieldElements *galoisFields is null\n");
		return NULL;
	}
	if(!result)
	{
		errorMes;
		printf("in multipleTwoEqReturnAddr, struct_galoisFieldElements **result is null\n");
		return NULL;
	}
	if(!operandEqA)
	{
		errorMes;
		printf("in multipleTwoEqReturnAddr, struct_galoisFieldElements *p\n");
		return NULL;
	}
	if(!(operandEqA->length))
	{
		errorMes;
		printf("in multipleTwoEqReturnAddr, p->length is zero\n");
		return NULL;
	}
	if(!operandEqB)
	{
		errorMes;
		printf("in multipleTwoEqReturnAddr, struct_galoisFieldElements *p\n");
		return NULL;
	}
	if(!(operandEqB->length))
	{
		errorMes;
		printf("in multipleTwoEqReturnAddr, p->length is zero\n");
		return NULL;
	}
	#endif
	
	tmp_order=((operandEqA->length)+(operandEqB->length)-2);
	recreateGaloisFieldExceptElements(result, tmp_order+1);
	initEquationFillAllZero(galoisFields, *result);

	// *(equation->element+k+1)=sumElementInGF_usingSumMatrixReturnAddr(galoisFields, *(beforeEquation->element+k), *(equation->element+k+1));
	// *(equation->element+k)=multiElementsInGF_returnAddr(galoisFields, *((*(multipledRootSet->conjugateSet+i))->element+j), *(beforeEquation->element+k));
	for(i=0; i<operandEqA->length; i++)
	{
		for(j=0; j<operandEqB->length; j++)
		{
			tmp_calBuf=multiElementsInGF_returnAddr(galoisFields, (*(operandEqA->element+i)), (*(operandEqB->element+j)));
			(*((*result)->element+i+j))=sumElementInGF_usingSumMatrixReturnAddr(galoisFields, (*((*result)->element+i+j)), tmp_calBuf);
		}
	}
	return *result;
}
//////////////////////////////////////////////////////////////////////////////////
char abstractOrderOfEquation(struct_galoisFieldElements *equation)
{
	unsigned int i;

	#ifndef RELEASE
	if(!equation)
	{
		errorMes;
		printf("in checkBlankLengthOfEquation, struct_galoisFieldElements *equation is NULL.\n");
		return 0;
	}
	if(!(equation->length))
	{
		errorMes;
		printf("in checkBlankLengthOfEquation, (equation->length) is ZERO.\n");
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
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
/*
struct struct_galoisFieldElements
{
	unsigned int length;
	struct_galoisFieldPolyForm **element;
}typedef struct_galoisFieldElements;
*/
//typedef struct_galoisFieldElements struct_galoisFieldElements;
//////////////////////////////////////////////////////////////////////////////////
//#define createGaloisFieldExceptElements(numberOfElementOfgaloisField) createGaloisFieldExceptElements(numberOfElementOfgaloisField)
/* struct_galoisFieldElements *createConjugacyClass(unsigned int numberOfElementOfgaloisField)
{
	if(!numberOfElementOfgaloisField)
	{
		errorMes;
		printf("in createConjugacyClass, unsigned int numberOfElementOfgaloisField is zero.\n");
		return NULL;
	}
	
	
	struct_galoisFieldElements *p;
	if(!(p = (struct_galoisFieldElements*)createGaloisFieldExceptElements(numberOfElementOfgaloisField)))
	{
		errorMes;
		printf("in createConjugacyClass, (p = createGaloisFieldExceptElements(numberOfElementOfgaloisField)) return NULL.\n");
		return p;
	}
	
	return p;
} */
//////////////////////////////////////////////////////////////////////////////////
//#define closeGaloisFieldExceptElements(p) closeGaloisFieldExceptElements(p)
/* char closeConjugacyClass(struct_galoisFieldElements *p)
{
	if(!p)
	{
		errorMes;
		printf("in closeConjugacyClass, struct_galoisFieldElements *p is NULL.\n");
		return -1;
	}
	
	if(closeGaloisFieldExceptElements(struct_galoisFieldElements *p))
	{
		errorMes;
		printf("in closeConjugacyClass, closeGaloisFieldExceptElements(struct_galoisFieldElements *p) return error.\n");
		return -1;
	}
	return 0;
} */
//#define recreateGaloisFieldExceptElements(p, num) recreateGaloisFieldExceptElements(p, num)
//////////////////////////////////////////////////////////////////////////////////
/*
note about struct_setOfGaloisFieldElements.
test. 2016/02/22/14:28, limitedExponential is mean that, when getting conjugacies class, limit exponential of galois fields.
*/
struct struct_setOfGaloisFieldElements
{
	unsigned int length;
	struct_galoisFieldElements **conjugateSet;

	unsigned int limitedExponential;
	
	struct_galoisFieldElements **limitedConjugateSet;
}typedef struct_setOfGaloisFieldElements;
//////////////////////////////////////////////////////////////////////////////////
char setLengthOfLimittedLengthOfConjugacyClasse(struct_setOfGaloisFieldElements *p, unsigned int limitedExponential)
{
	#ifndef RELEASE
	if(!p)
	{
		errorMes;
		printf("in setLengthOfLimittedLengthOfConjugacyClasse, struct_setOfGaloisFieldElements *p is NULL\n");
		return -1;
	}
	#endif
	
	p->limitedExponential = limitedExponential;
	return 0;
}
//////////////////////////////////////////////////////////////////////////////////
struct_setOfGaloisFieldElements *createConjugacyClasses(struct_galoisFieldElements *galoisFields)
{
	struct_setOfGaloisFieldElements *p;
	unsigned int numberOfConjugatesSet;
	unsigned int restSetOfConjugates;

	unsigned int i;

	#ifndef RELEASE
	if(!galoisFields)
	{
		errorMes;
		printf("in createConjugacyClasses, struct_galoisFieldElements *galoisFields is NULL.\n");
		return NULL;
	}
	#endif
	
	p=(struct_setOfGaloisFieldElements*)malloc(sizeof(struct_setOfGaloisFieldElements));
	memset(p, 0, sizeof(struct_setOfGaloisFieldElements));
	
	numberOfConjugatesSet=(galoisFields->length);

	
	restSetOfConjugates=numberOfConjugatesSet%((*(galoisFields->element+0))->length);
	numberOfConjugatesSet/=((*(galoisFields->element+0))->length);
	


	////setup number of conjugate set
	p->length=numberOfConjugatesSet;

			#ifndef RELEASE
			if((global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)==FLAG_MASK_PRINTF_LOG)
			{
				logMes;
				printf("in createConjugacyClasses,\n");
				printf("p->length(is that mean number of conjugates set) is \'%d\'\n", p->length);
				if(restSetOfConjugates)
				{
					printf("all conjugates set have \'%d\' elements.\n", ((*(galoisFields->element+0))->length));
				}
				else
				{
					printf("except lastest, conjugates set have \'%d\' elements.\n", ((*(galoisFields->element+0))->length));
					printf("lastest conjugates set have \'%d\'\n", restSetOfConjugates);
				}
			}
			#endif

	
	
	////making conjugacy set
	p->conjugateSet=(struct_galoisFieldElements**)malloc(sizeof(struct_galoisFieldElements*)*p->length);
	memset(p->conjugateSet, 0, sizeof(struct_galoisFieldElements*)*p->length);
	p->limitedConjugateSet=(struct_galoisFieldElements**)malloc(sizeof(struct_galoisFieldElements*)*p->length);
	memset(p->limitedConjugateSet, 0, sizeof(struct_galoisFieldElements*)*p->length);
	#ifdef USING_OPEN_MP
	#pragma omp parallel for schedule(guided) private(i) shared(p, galoisFields)
	#endif
	for(i=0;i<p->length; i++)//to except lastes one, looping numberOfConjugatesSet-1;
	{
		*(p->conjugateSet+i)=createGaloisFieldExceptElements(((*(galoisFields->element+0))->length));
		*(p->limitedConjugateSet+i)=createGaloisFieldExceptElements(((*(galoisFields->element+0))->length));
	}
	
	return p;
}
//////////////////////////////////////////////////////////////////////////////////
struct_setOfGaloisFieldElements *createConjugacyClasses_staticSetNums(unsigned int setLength, unsigned int *setElementLength)
{
	struct_setOfGaloisFieldElements *p;

	unsigned int i;
	
	p=(struct_setOfGaloisFieldElements*)malloc(sizeof(struct_setOfGaloisFieldElements));
	memset(p, 0, sizeof(struct_setOfGaloisFieldElements));
	
	p->length=setLength;
	
	////making conjugacy set
	p->conjugateSet=(struct_galoisFieldElements**)malloc(sizeof(struct_galoisFieldElements*)*setLength);
	memset(p->conjugateSet, 0, sizeof(struct_galoisFieldElements*)*setLength);
	p->limitedConjugateSet=(struct_galoisFieldElements**)malloc(sizeof(struct_galoisFieldElements*)*setLength);
	memset(p->limitedConjugateSet, 0, sizeof(struct_galoisFieldElements*)*setLength);
	#ifdef USING_OPEN_MP
	#pragma omp parallel for schedule(guided) private(i) shared(p)
	#endif
	for(i=0;i<setLength; i++)//to except lastes one, looping numberOfConjugatesSet-1;
	{
		*(p->conjugateSet+i)=createGaloisFieldExceptElements(setElementLength[i]);
		*(p->limitedConjugateSet+i)=createGaloisFieldExceptElements(setElementLength[i]);
	}
	
	return p;
}
//////////////////////////////////////////////////////////////////////////////////
struct_setOfGaloisFieldElements *createConjugacyClasses_VariableLength()
{
	struct_setOfGaloisFieldElements *p;
	
	p=(struct_setOfGaloisFieldElements*)malloc(sizeof(struct_setOfGaloisFieldElements));
	memset(p, 0, sizeof(struct_setOfGaloisFieldElements));
	//p->length=0;//it is already reset.

	////making conjugacy set
	//p->conjugateSet=NULL;//it is already reset.
	
	return p;
}
//////////////////////////////////////////////////////////////////////////////////
// char addLengthOfConjugacySet_VariableLength(struct_galoisFieldElements *galoisFields, struct_setOfGaloisFieldElements *p, unsigned int addedLength)
// {
	// unsigned int i;
	// struct_galoisFieldElements **newConjugateSet=NULL;
	
	// #ifndef RELEASE
	// if(!p)
	// {
		// errorMes;
		// printf("in addConjugacyClassesVariableLength, struct_setOfGaloisFieldElements *p is NULL.\n");
		// return -1;
	// }
	// if(!addedLength)
	// {
		// warningMes;
		// printf("in addConjugacyClassesVariableLength, unsigned int addedLength is 0.\n");
		// return 0;
	// }
	// #endif

	///*making conjugacy set array and initializing*/
	// if(!(p->length))
	// {
		// p->conjugateSet=(struct_galoisFieldElements**)malloc(sizeof(struct_galoisFieldElements*)*addedLength);//making conjugacy set array
		
		// #ifdef USING_OPEN_MP
		// #pragma omp parallel for schedule(guided) private(i) shared(addedLength, p)
		// #endif
		// for(i=0; i<addedLength; i++)
		// {
			// *(p->conjugateSet+i)=createGaloisFieldExceptElements_VariableLength();
		// }
		// p->length=addedLength;
	// }
	// else
	// {
		/* Sometime realloc has memory leakage. So to increase memory size, using this func is invalid. */
		/*p->conjugateSet=(struct_galoisFieldElements**)realloc(p->conjugateSet, sizeof(struct_galoisFieldElements*)*addedLength);*///increasing conjugacy set array
		// newConjugateSet=(struct_galoisFieldElements**)malloc((sizeof(struct_galoisFieldElements*)*((p->length)+addedLength)));
		// memset(newConjugateSet , 0, (sizeof(struct_galoisFieldElements*)*(p->length+addedLength)));
		// memcpy(newConjugateSet, p->conjugateSet, (sizeof(struct_galoisFieldElements*)*(p->length)));
		// free(p->conjugateSet);
		// p->conjugateSet=newConjugateSet;
		
		/*clear added conjugacy set*/
		// #ifdef USING_OPEN_MP
		// #pragma omp parallel for schedule(guided) private(i) shared(addedLength, p)
		// #endif
		// for(i=p->length; i<((p->length)+addedLength); i++)
		// {
			// *(p->conjugateSet+i)=createGaloisFieldExceptElements_VariableLength();
		// }
		// p->length+=addedLength;
	// }
	
	// return 0;
// }
char addLengthOfConjugacySet_VariableLength(struct_galoisFieldElements *galoisFields, struct_setOfGaloisFieldElements *p, unsigned int addedLength)
{
	unsigned int i;
	struct_galoisFieldElements **newConjugateSet=NULL;
	struct_galoisFieldElements **newLimitedConjugateSet=NULL;
	
	
	#ifndef RELEASE
	if(!p)
	{
		errorMes;
		printf("in addConjugacyClassesVariableLength, struct_setOfGaloisFieldElements *p is NULL.\n");
		return -1;
	}
	if(!addedLength)
	{
		warningMes;
		printf("in addConjugacyClassesVariableLength, unsigned int addedLength is 0.\n");
		return 0;
	}
	#endif

	////making conjugacy set array and initializing
	if(!(p->length))
	{
		p->conjugateSet=(struct_galoisFieldElements**)malloc(sizeof(struct_galoisFieldElements*)*addedLength);//making conjugacy set array
		
		p->limitedConjugateSet=(struct_galoisFieldElements**)malloc(sizeof(struct_galoisFieldElements*)*addedLength);//making conjugacy set array
		//memset(p->conjugateSet, 0, sizeof(struct_galoisFieldElements*)addedLength);
		
		#ifdef USING_OPEN_MP
		#pragma omp parallel for schedule(guided) private(i) shared(addedLength, p)
		#endif
		for(i=0; i<addedLength; i++)
		{
			*(p->conjugateSet+i)=createGaloisFieldExceptElements_VariableLength();
			
			*(p->limitedConjugateSet+i)=createGaloisFieldExceptElements_VariableLength();
		}
		p->length=addedLength;
	}
	else
	{
		/* Sometime realloc has memory leakage. So to increase memory size, using this func is invalid. */
		//p->conjugateSet=(struct_galoisFieldElements**)realloc(p->conjugateSet, sizeof(struct_galoisFieldElements*)*addedLength);//increasing conjugacy set array
		newConjugateSet=(struct_galoisFieldElements**)malloc((sizeof(struct_galoisFieldElements*)*((p->length)+addedLength)));
		newLimitedConjugateSet=(struct_galoisFieldElements**)malloc((sizeof(struct_galoisFieldElements*)*((p->length)+addedLength)));

		memset(newConjugateSet , 0, (sizeof(struct_galoisFieldElements*)*(p->length+addedLength)));
		memset(newLimitedConjugateSet , 0, (sizeof(struct_galoisFieldElements*)*(p->length+addedLength)));

		memcpy(newConjugateSet, p->conjugateSet, (sizeof(struct_galoisFieldElements*)*(p->length)));
		memcpy(newLimitedConjugateSet, p->limitedConjugateSet, (sizeof(struct_galoisFieldElements*)*(p->length)));
		
		free(p->conjugateSet);
		free(p->limitedConjugateSet);
		
		p->conjugateSet=newConjugateSet;
		p->limitedConjugateSet=newLimitedConjugateSet;
		
		//clear added conjugacy set
		#ifdef USING_OPEN_MP
		#pragma omp parallel for schedule(guided) private(i) shared(addedLength, p)
		#endif
		for(i=p->length; i<((p->length)+addedLength); i++)
		{
			*(p->conjugateSet+i)=createGaloisFieldExceptElements_VariableLength();
			
			*(p->limitedConjugateSet+i)=createGaloisFieldExceptElements_VariableLength();
		}
		p->length+=addedLength;
	}
	
	return 0;
}
//////////////////////////////////////////////////////////////////////////////////
char closeConjugacyClasses(struct_setOfGaloisFieldElements **p)
{
	unsigned int i;

	#ifndef RELEASE
	if(!p)
	{
		errorMes;
		printf("in closeConjugacyClasses, struct_setOfGaloisFieldElements **p is NULL.\n");
		return -1;
	}

	{
		warningMes;
		printf("in closeConjugacyClasses, struct_setOfGaloisFieldElements *p is NULL.\n");
		return -1;
	}
	#endif

	#ifdef USING_OPEN_MP
	#pragma omp parallel for schedule(guided) private(i) shared(p)
	#endif
	for(i=0; i<(*p)->length; i++)
	{
		#ifdef RELEASE
		closeGaloisFieldExceptElements(((*p)->conjugateSet+i));
		closeGaloisFieldExceptElements(((*p)->limitedConjugateSet+i));
		
		#else
		if(closeGaloisFieldExceptElements(((*p)->conjugateSet+i)))//if(closeGaloisFieldExceptElements(&(*((*p)->conjugateSet+i))))
		{
			errorMes;
			printf("in closeConjugacyClasses, closeGaloisFieldExceptElements(*(p->conjugateSet+%d)) return any value.\n", i);
		}		
		if(closeGaloisFieldExceptElements(((*p)->limitedConjugateSet+i)))//if(closeGaloisFieldExceptElements(&(*((*p)->conjugateSet+i))))
		{
			errorMes;
			printf("in closeConjugacyClasses, closeGaloisFieldExceptElements(((*p)->limitedConjugateSet+i)) return any value.\n", i);
		}		
		#endif

	}
	
			#ifndef RELEASE
			if((global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)==FLAG_MASK_PRINTF_LOG)
			{
				logMes;
				printf("in closeConjugacyClasses, before process free(p->conjugateSet);\n");
			}
			#endif
			
	free((*p)->conjugateSet);
	free((*p)->limitedConjugateSet);
	
			#ifndef RELEASE
			if((global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)==FLAG_MASK_PRINTF_LOG)
			{
				logMes;
				printf("in closeConjugacyClasses, before process free(p);\n");
			}
			#endif
	free(*p);
	*p=NULL;
	return 0;
}
//////////////////////////////////////////////////////////////////////////////////
struct_setOfGaloisFieldElements *recreateConjugacyClasses(struct_setOfGaloisFieldElements **p, struct_galoisFieldElements *galoisFields)
{
	if(*p)
	{
				#ifndef RELEASE
				if((global_flag_cmdOption&FLAG_MASK_PRINTF_LOG))
				{
					printf("in recreateConjugacyClasses, struct_setOfGaloisFieldElements *p is already have data.\n");
					printf("it will be removed.\n");
				}
				#endif
		closeConjugacyClasses(p);
	}
	*p=createConjugacyClasses(galoisFields);
	return *p;
}
//////////////////////////////////////////////////////////////////////////////////
void print_setOfGaloisFieldElementsSavedForm(struct_setOfGaloisFieldElements *p)
{
	unsigned int tmp_i;
	unsigned int tmp_j;
	
	#ifndef RELEASE
	if(!p)
	{
		errorMes;
		printf("in print_setOfGaloisFieldElementsSavedForm, struct_setOfGaloisFieldElements *p is NULL\r\n");
		return;
	}
	#endif
	
	printf(">> Set Of Galois Field File I/O Form <<\n");
	
	for(tmp_i=0; tmp_i<(p->length); tmp_i++)
	{
		for(tmp_j=0; tmp_j<(*(p->conjugateSet+tmp_i))->length; tmp_j++)
		{
			if((*((*(p->conjugateSet+tmp_i))->element+tmp_j))->index)
			{
				printf("%d(%d)%s", 
					((*((*(p->conjugateSet+tmp_i))->element+tmp_j))->index - 1), 
					(*((*(p->conjugateSet+tmp_i))->element+tmp_j))->index, 
					(tmp_j<((*(p->conjugateSet+tmp_i))->length-1))?", ":""
				);	
			}
			else
			{
				printf("%c(%d)%s", 
					'-', 
					(*((*(p->conjugateSet+tmp_i))->element+tmp_j))->index, 
					(tmp_j<((*(p->conjugateSet+tmp_i))->length-1))?", ":""
				);	

			}
		}
		printf("\r\n");
	}
}
//////////////////////////////////////////////////////////////////////////////////
/*
struct struct_setOfGaloisFieldElements
{
	unsigned int length;
	struct_galoisFieldElements **conjugateSet;

	unsigned int limitedExponential;
	
	struct_galoisFieldElements **limitedConjugateSet;
}typedef struct_setOfGaloisFieldElements;
*/
char save_struct_setOfGaloisFieldElements(char *path, char *primitivePoly, struct_setOfGaloisFieldElements *p)
{
	//*((*(p->conjugateSet+i))->element+j)=calculationBuffer;
	//(*(galoisFields->element+0));

	unsigned int tmp_i, tmp_j;
	FILE *fileio;
	if(!path)			return -1;
	if(!primitivePoly)	return -2;
	if(!p)				return -3;
	
	fileio = fopen(path, "w");
	
	fprintf(fileio, "primitive polynomials, %s\r\n", primitivePoly);
	
	fprintf(fileio, "%s\r\n", FILE_IO_DATA_PARSING_KEY_STRING_SAVED_DATA_START);
	
	fprintf(fileio, "%s\r\n", FILE_IO_DATA_PARSING_KEY_STRING_KIND_OF_DATA_START);
	fprintf(fileio, "Kind Of Data, %d\r\n", FILE_IO_KIND_OF_DATA_CONJUGATE_SET);
	fprintf(fileio, "%s\r\n", FILE_IO_DATA_PARSING_KEY_STRING_KIND_OF_DATA_END);

	fprintf(fileio, "%s\r\n", FILE_IO_DATA_PARSING_KEY_STRING_ARRAY_TYPE_START);
	fprintf(fileio, "Saved Data Type, %d\r\n", FILD_IO_DATA_ARRAY_TYPE_NONE_LINEAR);
	fprintf(fileio, "%s\r\n", FILE_IO_DATA_PARSING_KEY_STRING_ARRAY_TYPE_END);

	fprintf(fileio, "%s\r\n", FILE_IO_DATA_PARSING_KEY_STRING_BIT_WIDTH_START);
	fprintf(fileio, "Bit Width, %d\r\n", (*((*(p->conjugateSet+0))->element+0))->length);
	fprintf(fileio, "%s\r\n", FILE_IO_DATA_PARSING_KEY_STRING_BIT_WIDTH_END);
	
	fprintf(fileio, "%s\r\n", FILE_IO_DATA_PARSING_KEY_STRING_ROW_DATA_START);
	fprintf(fileio, "conjugate set nums, %d\r\n", p->length);
	fprintf(fileio, "%s\r\n", FILE_IO_DATA_PARSING_KEY_STRING_ROW_DATA_END);
	
	
	
	fprintf(fileio, "%s\r\n", FILE_IO_DATA_PARSING_KEY_STRING_COL_DATA_START);
	for(tmp_i=0; tmp_i<(p->length); tmp_i++)
	{
		fprintf(fileio, "%d-th set conjugate elements nums, %d\r\n", tmp_i, (*(p->conjugateSet+tmp_i))->length);
	}
	fprintf(fileio, "%s\r\n", FILE_IO_DATA_PARSING_KEY_STRING_COL_DATA_END);
	
	fprintf(fileio, "pow(index)\r\n", p->length);

	fprintf(fileio, "%s\r\n", FILE_IO_DATA_PARSING_KEY_STRING_RAW_DATA_START);
	//(*(p->conjugateSet+i))->length
	for(tmp_i=0; tmp_i<(p->length); tmp_i++)
	{
		for(tmp_j=0; tmp_j<(*(p->conjugateSet+tmp_i))->length; tmp_j++)
		{
			if((*((*(p->conjugateSet+tmp_i))->element+tmp_j))->index)
			{
				fprintf(fileio, "%d(%d)%s", 
					((*((*(p->conjugateSet+tmp_i))->element+tmp_j))->index - 1), 
					(*((*(p->conjugateSet+tmp_i))->element+tmp_j))->index, 
					(tmp_j<((*(p->conjugateSet+tmp_i))->length-1))?", ":""
				);	
			}
			else
			{
				fprintf(fileio, "%c(%d)%s", 
					'-', 
					(*((*(p->conjugateSet+tmp_i))->element+tmp_j))->index, 
					(tmp_j<((*(p->conjugateSet+tmp_i))->length-1))?", ":""
				);	

			}
		}
		fprintf(fileio, "\r\n");
	}
	fprintf(fileio, "%s\r\n", FILE_IO_DATA_PARSING_KEY_STRING_RAW_DATA_END);

	fprintf(fileio, "%s\r\n", FILE_IO_DATA_PARSING_KEY_STRING_SAVED_DATA_END);
	
	fclose(fileio);
	
	return 0;
}
//////////////////////////////////////////////////////////////////////////////////
char load_struct_setOfGaloisFieldElements(char *path, char *primitivePoly, struct_galoisFieldElements *galoisFields, struct_setOfGaloisFieldElements **p)
{
	//*((*(p->conjugateSet+i))->element+j)=calculationBuffer;
	//(*(galoisFields->element+0));

	unsigned int tmp_i, tmp_j;

	FILE *fileio_load_setOfGaloisField;
	
	unsigned int primitivePoly_length=0;
	char load_primitivePoly[1025];
	
	int load_kindOfData=0;
	int load_saveOfData=0;

	unsigned int load_bitWidth=0;
	unsigned int load_setOfGF_length=0;
	unsigned int load_row=0;
	unsigned int *load_column=NULL;
	
	unsigned int load_index;
	// char load_exp_str[256]={0};
	char load_exp_char=0;
	unsigned int load_exp_integer=0;

	
	unsigned int load_integer=0;
	
	char charBuf=0;
	char load_stringBuf[1025]={0};
	
	unsigned int chk_incorrectFormCnt=0;
	unsigned int chk_incorrectDataCnt=0;
	
	if(!path)			return -1;
	if(!primitivePoly)	return -2;
	if(!p)				return -3;
	
	fileio_load_setOfGaloisField = fopen(path, "r");
	
			printf("[load_struct_setOfGaloisFieldElements START]\r\n");
	
	fscanf(fileio_load_setOfGaloisField, "primitive polynomials, %s\r\n", load_primitivePoly);
			// printf("primitive polynomials, %s\r\n", load_primitivePoly);
	
	fscanf(fileio_load_setOfGaloisField, "%s\r\n", load_stringBuf);
			// printf("%s\r\n", load_stringBuf);
	
	fscanf(fileio_load_setOfGaloisField, "%s\r\n", load_stringBuf);
			// printf("%s\r\n", load_stringBuf);
	fscanf(fileio_load_setOfGaloisField, "Kind Of Data, %d\r\n", &load_kindOfData);
			// printf("Kind Of Data, %d\r\n", load_kindOfData);
	fscanf(fileio_load_setOfGaloisField, "%s\r\n", load_stringBuf);
			// printf("%s\r\n", load_stringBuf);

	fscanf(fileio_load_setOfGaloisField, "%s\r\n", load_stringBuf);
			// printf("%s\r\n", load_stringBuf);
	fscanf(fileio_load_setOfGaloisField, "Saved Data Type, %d\r\n", &load_saveOfData);
			// printf("Saved Data Type, %d\r\n", load_saveOfData);
	fscanf(fileio_load_setOfGaloisField, "%s\r\n", load_stringBuf);
			// printf("%s\r\n", load_stringBuf);

	fscanf(fileio_load_setOfGaloisField, "%s\r\n", load_stringBuf);
			// printf("%s\r\n", load_stringBuf);
	fscanf(fileio_load_setOfGaloisField, "Bit Width, %d\r\n", &load_bitWidth);
			// printf("Bit Width, %d\r\n", load_bitWidth);
	fscanf(fileio_load_setOfGaloisField, "%s\r\n", load_stringBuf);
			// printf("%s\r\n", load_stringBuf);
	
	fscanf(fileio_load_setOfGaloisField, "%s\r\n", load_stringBuf);
			// printf("%s\r\n", load_stringBuf);
	fscanf(fileio_load_setOfGaloisField, "conjugate set nums, %d\r\n", &load_setOfGF_length);
			// printf("conjugate set nums, %d\r\n", load_setOfGF_length);
	fscanf(fileio_load_setOfGaloisField, "%s\r\n", load_stringBuf);
			// printf("%s\r\n", load_stringBuf);
	load_column=(unsigned int*)malloc(sizeof(unsigned int)*load_setOfGF_length);
	memset(load_column, 0, sizeof(unsigned int)*load_setOfGF_length);
	
	fscanf(fileio_load_setOfGaloisField, "%s\r\n", load_stringBuf);
			// printf("%s\r\n", load_stringBuf);
	for(tmp_i=0; tmp_i<load_setOfGF_length; tmp_i++)
	{
		fscanf(fileio_load_setOfGaloisField, "%d-th set conjugate elements nums, %d\r\n", &load_integer, load_column+tmp_i);
				// printf("%d-th set conjugate elements nums, %d\r\n", load_integer, *(load_column+tmp_i));
	}
	fscanf(fileio_load_setOfGaloisField, "%s\r\n", load_stringBuf);
			// printf("%s\r\n", load_stringBuf);


			// printf("[createConjugacyClasses_staticSetNums START]\r\n");
	*p = createConjugacyClasses_staticSetNums(load_setOfGF_length, load_column);
			// printf("[createConjugacyClasses_staticSetNums END]\r\n");

	
	fscanf(fileio_load_setOfGaloisField, "%s\r\n", load_stringBuf);
			// printf("%s\r\n", load_stringBuf);

	fscanf(fileio_load_setOfGaloisField, "%s\r\n", load_stringBuf);
	for(tmp_i=0; tmp_i<load_setOfGF_length; tmp_i++)
	{
		for(tmp_j=0; tmp_j<load_column[tmp_i]; tmp_j++)
		{
			if(tmp_j<(load_column[tmp_i]-1))
			{
				fscanf(fileio_load_setOfGaloisField, "%d(%d), ", 
					&load_exp_integer, 
					&load_index
				);
						// printf("%d(%d), ", 
							// load_exp_integer, 
							// load_index
						// );
			}
			else
			{
				fscanf(fileio_load_setOfGaloisField, "%d(%d)", 
					&load_exp_integer, 
					&load_index
				);	
						// printf("%d(%d)", 
							// load_exp_integer, 
							// load_index
						// );
			}
			(*((*((*p)->conjugateSet+tmp_i))->element+tmp_j)) = (*(galoisFields->element+load_index));
			(*((*((*p)->limitedConjugateSet+tmp_i))->element+tmp_j)) = (*(galoisFields->element+load_index));
		}
		fscanf(fileio_load_setOfGaloisField, "\r\n");
				// printf("\r\n");
	}
	fscanf(fileio_load_setOfGaloisField, "%s\r\n", load_stringBuf);
			// printf("%s\r\n", load_stringBuf);
	fscanf(fileio_load_setOfGaloisField, "%s\r\n", load_stringBuf);
			// printf("%s\r\n", load_stringBuf);
	
	fclose(fileio_load_setOfGaloisField);
	
	free(load_column);
	
			#ifndef RELEASE
			if(global_flag_gfCommon_display&FLAG_MASK_DISPLAY_GF_COMMON_FILE_IO)	
			{
				printf(">> Set of Condjugacies <<\r\n");
				print_setOfGaloisFieldElementsSavedForm(*p);
			}
			#endif

	
			printf("[load_struct_setOfGaloisFieldElements END]\r\n");
	
	return 0;
}
//////////////////////////////////////////////////////////////////////////////////
char calculateConjugacyClasses(struct_galoisFieldElements *galoisFields, struct_setOfGaloisFieldElements *p)
{
	char *elementFlagger;

	unsigned int i, j;
	unsigned int elementFlaggerSecelcted;//k;

	unsigned int exponential;

	struct_galoisFieldPolyForm *calculationBuffer;

	#ifndef RELEASE
	if(!galoisFields)
	{
		errorMes;
		printf("in calculateConjugacyClasses, struct_galoisFieldElements *galoisFields is NULL.\n");
		return -1;
	}
	if(!p)
	{
		errorMes;
		printf("in calculateConjugacyClasses, struct_setOfGaloisFieldElements *p is NULL.\n");
		return -1;
	}
	#endif

	//*(elementFlagger+0) is mean that a^1 is selected already.
	elementFlagger=(char*)malloc(sizeof(char)*((galoisFields->length)-2+1));//index 0 is mean that a^1, and latest index(except NULL) is total number of element in GF -(minus) 2.
	memset(elementFlagger, '0', sizeof(char)*((galoisFields->length)-2));
	*(elementFlagger+((galoisFields->length)-2))=0;//last string have to set NULL.
	
	elementFlaggerSecelcted=1;//elementFlaggerSecelcted is indicate elementsFlagger location.
		
	for(i=0; i<p->length; i++)
	{
		*((*(p->conjugateSet+i))->element+0)=*(galoisFields->element+2+(elementFlaggerSecelcted-1));
		elementFlagger[(elementFlaggerSecelcted-1)]='1';
		
		for(j=1; j<(*(p->conjugateSet+i))->length; j++)
		{
			calculationBuffer=multiElementsInGF_returnAddr(galoisFields, *((*(p->conjugateSet+i))->element-1+j), *((*(p->conjugateSet+i))->element-1+j));
			if(checkValueFromPolyFormUsingGaloisFieldValueUsingIntValue_(*((*(p->conjugateSet+i))->element+0), calculationBuffer))//check, it is same to first value?
			{
				if(j!=(*(p->conjugateSet+i))->length)//length is number and j-1 is offset. So, j-1+1 is number.
				{
							#ifndef RELEASE
							warningMes; printf("in calculateConjugacyClasses, j(=\'%d\')!=(*(p->conjugateSet+i))->length(=\'%d\').\n", j, (*(p->conjugateSet+i))->length);
							#endif
					(*(p->conjugateSet+i))->length=j;

					#ifndef RELEASE
					for(; j<(*(p->conjugateSet+i))->length; j++)
					{
						if(*((*(p->conjugateSet+i))->element+j))
						{
							warningMes; printf("in calculateConjugacyClasses, *((*(p->conjugateSet+i))->element+j)(Addr = \'0x%x\') is not NULL.\n", *((*(p->conjugateSet+i))->element+j));
							warningMesShort; printf("this link information(it is pointer) will be lost.\n");
						}
					}
					#endif
				}
				break;
			}
			else
			{
				exponential=abstractIndexOfPowerFormInElementsOfGaloisFieldUsingIndex(galoisFields, calculationBuffer);
				
				if((exponential==-1)||(exponential==0)||(exponential==1))
				{
							#ifndef RELEASE
							errorMes; printf("in calculateConjugacyClasses, exponential can not be \'%s\'.\n", exponential);
							#endif
					return -1;
				}

				elementFlagger[(exponential-2)]='1';
				*((*(p->conjugateSet+i))->element+j)=calculationBuffer;
			}
		}
		
		//find non selected elements using elementFlagger.
		for(elementFlaggerSecelcted=1; elementFlaggerSecelcted<((galoisFields->length)-2+1); elementFlaggerSecelcted++)
		{
			if((elementFlagger[elementFlaggerSecelcted-1]=='0'))
			{
				break;
			}
		}
		if(elementFlaggerSecelcted==((galoisFields->length)-2+1))
		{
			break;
		}
	}
	free(elementFlagger);
	return 0;
}
//////////////////////////////////////////////////////////////////////////////////
char calculateConjugacyClasses_VariableLength(struct_galoisFieldElements *galoisFields, struct_setOfGaloisFieldElements *p)
{
	char *elementFlagger;

	unsigned int i, j;
	unsigned int selectedConjugacySet;
	unsigned int selectedElementOfConjugacy;

	
	
	struct_galoisFieldPolyForm *firstElementOfConjugacySet;
	struct_galoisFieldPolyForm *calculationBuffer;

	#ifndef RELEASE
	if(!galoisFields)
	{
		errorMes;
		printf("in calculateConjugacyClasses, struct_galoisFieldElements *galoisFields is NULL.\n");
		return -1;
	}
	if(!p)
	{
		errorMes;
		printf("in calculateConjugacyClasses, struct_setOfGaloisFieldElements *p is NULL.\n");
		return -1;
	}
	#endif

	/* flagger start a^0, but a^0 one of conjugacy any conjugacy set. */
	elementFlagger=(char*)malloc(sizeof(char)*((galoisFields->length)+1));
	memset(elementFlagger, '0', sizeof(char)*((galoisFields->length)-1));
	
	elementFlagger[(galoisFields->length)]=0;//last string have to set NULL.
	elementFlagger[0]='1';//a^- already select, because it is not change when doubled itself.
	elementFlagger[1]='1';//a^0 already select, because it is not change when doubled itself.
	
	selectedConjugacySet=0;//start 0-th conjugacy set
	
	for(i=0; i<galoisFields->length; i++)
	{
		/* pass selected location of element*/
		if(elementFlagger[i]=='1')
		{
			continue;
		}
		
		/* if not enough space of conjugacy set to charge conjugacy, added space */
		if(!((selectedConjugacySet)<(p->length)))
		{
			if(addLengthOfConjugacySet_VariableLength(galoisFields, p, 1))
			{
						#ifndef RELEASE
						errorMes; printf("in calculateConjugacyClasses_VariableLength, addLengthOfConjugacySet_VariableLength(galoisFields, p, 1) return to not zero.");
						printf("it is mean that add conjugacy set is fail. conjugacy set will be closed and simulator can not work normally.\n");
						#endif
				closeConjugacyClasses(&p);
				return -1;
			}
			
		}
		
		/* To fill up conjugacy set, initialize conditions */
		firstElementOfConjugacySet=galoisFields->element[i];
		calculationBuffer=firstElementOfConjugacySet;
		
		if(!(p->conjugateSet[selectedConjugacySet]->length))
		{
			if(p->conjugateSet[selectedConjugacySet]->element)
			{
				p->conjugateSet[selectedConjugacySet]=recreateGaloisFieldExceptElements(&(p->conjugateSet[selectedConjugacySet]), 1);
			}
			
		}
/* 		
		if(!(p->limitedConjugateSet[selectedConjugacySet]->length))
		{
			if(p->limitedConjugateSet[selectedConjugacySet]->element)
			{
				p->limitedConjugateSet[selectedConjugacySet]=recreateGaloisFieldExceptElements(&(p->limitedConjugateSet[selectedConjugacySet]), 1);
			}
		}
 */		
		
		selectedElementOfConjugacy=0;		
		do
		{
			if(!((selectedElementOfConjugacy)<(p->conjugateSet[selectedConjugacySet]->length)))
			{
				addLengthOfGaloisFieldExceptElements_VariableLength(p->conjugateSet[selectedConjugacySet], 1);
			}
			((p->conjugateSet[selectedConjugacySet])->element[selectedElementOfConjugacy])=calculationBuffer;

			/*
			//This code is wrong.... 1
			if(!((selectedElementOfConjugacy)<(p->limitedConjugateSet[selectedConjugacySet]->length)))
			{			
				addLengthOfGaloisFieldExceptElements_VariableLength(p->limitedConjugateSet[selectedConjugacySet], 1);
				if( !( (abstractIndexOfPowerFormInElementsOfGaloisFieldUsingIndex(galoisFields, calculationBuffer)-1) > p->limitedExponential ) )
				{
					((p->limitedConjugateSet[selectedConjugacySet])->element[selectedElementOfConjugacy])=calculationBuffer;
				}
				else
				{
					((p->limitedConjugateSet[selectedConjugacySet])->element[selectedElementOfConjugacy])=galoisFields->element[0];
				}
			}
			*/
			/*
			//This code is wrong.... 2
			if(!((selectedElementOfConjugacy)<(p->limitedConjugateSet[selectedConjugacySet]->length)))
			{			
				addLengthOfGaloisFieldExceptElements_VariableLength(p->limitedConjugateSet[selectedConjugacySet], 1);
				if( !(selectedElementOfConjugacy%2) )
				{
					((p->limitedConjugateSet[selectedConjugacySet])->element[selectedElementOfConjugacy])=calculationBuffer;
				}
				else
				{
					((p->limitedConjugateSet[selectedConjugacySet])->element[selectedElementOfConjugacy])=galoisFields->element[0];
				}
			}
			*/
			
			elementFlagger[abstractIndexOfPowerFormInElementsOfGaloisFieldUsingIndex(galoisFields, calculationBuffer)]='1';//set selected location
			selectedElementOfConjugacy++;
			calculationBuffer=multiElementsInGF_returnAddr(galoisFields, calculationBuffer, calculationBuffer);
		}
		while(firstElementOfConjugacySet!=calculationBuffer);
		/* increase 1, selectedConjugacySet */
		selectedConjugacySet++;
	}
	
	free(elementFlagger);
	return 0;
}
//////////////////////////////////////////////////////////////////////////////////
void printConjugacyClasses(struct_galoisFieldElements *galoisFields, struct_setOfGaloisFieldElements *p)
{
	unsigned int i,j;
	unsigned int exponential;

	
	if(!galoisFields)
	{
		errorMes;
		printf("in printConjugacyClasses, struct_galoisFieldElements *galoisFields is NULL.\n");
		return;
	}
	if(!p)
	{
		errorMes;
		printf("in printConjugacyClasses, struct_setOfGaloisFieldElements *p is NULL.\n");
		return;
	}
	
	
	infoMes;
	printf(">> Conjugacy Class in GF(%d)<<\n", galoisFields->length);
	for(i=0; i<p->length; i++)
	{
		printf("%d : {\t", i);
		for(j=0; j<(*(p->conjugateSet+i))->length; j++)
		{
			//*((*(p->conjugateSet+i))->element+j)
			exponential=abstractIndexOfPowerFormInElementsOfGaloisFieldUsingIndex(galoisFields, *((*(p->conjugateSet+i))->element+j));
			if(exponential==0)
			{
				printf("a^-\t");
			}
			else
			{
				printf("a^%d\t", exponential-1);
			}
			
			printPolyForm(*((*(p->conjugateSet+i))->element+j));
			printf("\t");
			printf("=%d\t", (*((*(p->conjugateSet+i))->element+j))->intValue);
		}
		printf("}\n");
	}
	
	infoMes;
	printf(">> Limited Conjugacy Class in GF(%d)<<\n", galoisFields->length);
	for(i=0; i<p->length; i++)
	{
		printf("%d : {\t", i);
		for(j=0; j<(*(p->limitedConjugateSet+i))->length; j++)
		{
			//*((*(p->limitedConjugateSet+i))->element+j)
			exponential=abstractIndexOfPowerFormInElementsOfGaloisFieldUsingIndex(galoisFields, *((*(p->limitedConjugateSet+i))->element+j));
			if(exponential==0)
			{
				printf("a^-\t");
			}
			else
			{
				printf("a^%d\t", exponential-1);
			}
			
			printPolyForm(*((*(p->limitedConjugateSet+i))->element+j));
			printf("\t");
			printf("=%d\t", (*((*(p->limitedConjugateSet+i))->element+j))->intValue);
		}
		printf("}\n");
	}
}
//////////////////////////////////////////////////////////////////////////////////
char init_ConjugacyClasses(struct_galoisFieldElements *galoisFields, struct_setOfGaloisFieldElements **p, unsigned int limitedExponential)
{
	#ifndef RELEASE
	if(!galoisFields)
	{
		errorMes;
		printf("in init_ConjugacyClasses, struct_galoisFieldElements *galoisFields is NULL.\n");
		return -1;
	}
	if(!p)
	{
		errorMes;
		printf("in init_ConjugacyClasses, struct_setOfGaloisFieldElements **p.\n");
		return -1;
	}
	#endif

	/* create conjugacy Class (static) */
	// p->conjugacies=createConjugacyClasses(p->nonSorted);
	// calculateConjugacyClasses(p->nonSorted, p->conjugacies);
	
	/* create conjugacy Class (variable) */
	*p=createConjugacyClasses_VariableLength();
	setLengthOfLimittedLengthOfConjugacyClasse(*p, limitedExponential);

	infoMes; printf("[START][Generating conjugacy set]\n");
	calculateConjugacyClasses_VariableLength(galoisFields, *p);
	infoMes; printf("[END][Generating conjugacy set]\n");
	

			// initMes; printConjugacyClasses(galoisFields, *p);

	
	#ifndef RELEASE
	if(global_flag_gfCommon_display&FLAG_MASK_DISPLAY_GF_COMMON_CONJUGACIES)
	{
		initMes;
		printConjugacyClasses(galoisFields, *p);
	}
	#endif
	
	return 0;
}
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
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
				warningMes;	printf("in calculateLeastCommonMultipleUsingElementsOfGaloisField, ");
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
	
	/*if order is 0, length is 1, if order is n-th, length is n+1.*/
	multipledOrder = abstractOrderOfEquation(equation)+totalLcmOrder;
	multipledRootSet->length;
	
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
							printf("*(beforeEquation->element+global_tmp_i_memoryCheck) = 0x%x.\n", *(beforeEquation->element+global_tmp_i_memoryCheck));
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
							printf("*((*(multipledRootSet->conjugateSet+%d))->element+%d)\t{=>>}(ADDR:\'0x%x\') == \t", i, j, *((*(multipledRootSet->conjugateSet+i))->element+j));
							printPolyFormWithEnter(*((*(multipledRootSet->conjugateSet+i))->element+j));
							printf("\t\t\t*(beforeEquation->element+%d)\t{=>>}(ADDR:\'0x%x\') == \t", k, *(beforeEquation->element+k));
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
							printf("*(equation->element+global_tmp_i_memoryCheck) = 0x%x.\n", *(equation->element+global_tmp_i_memoryCheck));
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
							printf("*(equation->element+global_tmp_i_memoryCheck) = 0x%x.\n", *(equation->element+global_tmp_i_memoryCheck));
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
				warningMes;	printf("in test_expandLimitedLeastCommonMultipleUsingElementOfGaloisFieldToRoot, ");
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
	
	/*if order is 0, length is 1, if order is n-th, length is n+1.*/
	multipledOrder = abstractOrderOfEquation(equation)+totalLcmOrder;
	multipledRootSet->length;
	
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
							printf("*(beforeEquation->element+global_tmp_i_memoryCheck) = 0x%x.\n", *(beforeEquation->element+global_tmp_i_memoryCheck));
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
							printf("*((*(multipledRootSet->limitedConjugateSet+%d))->element+%d)\t{=>>}(ADDR:\'0x%x\') == \t", i, j, *((*(multipledRootSet->limitedConjugateSet+i))->element+j));
							printPolyFormWithEnter(*((*(multipledRootSet->limitedConjugateSet+i))->element+j));
							printf("\t\t\t*(beforeEquation->element+%d)\t{=>>}(ADDR:\'0x%x\') == \t", k, *(beforeEquation->element+k));
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
							printf("*(equation->element+global_tmp_i_memoryCheck) = 0x%x.\n", *(equation->element+global_tmp_i_memoryCheck));
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
							printf("*(equation->element+global_tmp_i_memoryCheck) = 0x%x.\n", *(equation->element+global_tmp_i_memoryCheck));
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
struct struct_galoisField_info
{
	// unsigned int *gfBits;
	// unsigned int length;
	
	
	struct_powerFormPolynomials *primitivePolynomial;
	
	struct_galoisFieldElements *nonSorted;
	struct_galoisFieldElements *sorted;
	
	struct_setOfGaloisFieldElements *conjugacies;
}typedef struct_galoisField_info;
//////////////////////////////////////////////////////////////////////////////////
void printGaloisField(struct_galoisFieldElements *p)
{
	unsigned int i,j;

	unsigned int decimal=0;

	unsigned int exponential;

	printf("\n\n------------------ Galois Field ------------------\n");
	infoMes;
	printf("number of element is \'%d\', GF(%d)\n", p->length, p->length);
	
	printf("pow\t");
	for(i=0; i<(*(p->element))->length; i++)
	{
		printf("a^%d\t", i);
	}
	printf("decimal\tindex");
	printf("\n");
	
	for(i=0; i<p->length; i++)
	{
		exponential=abstractIndexOfPowerFormInElementsOfGaloisFieldUsingIndex(p, *(p->element+i));
		if(exponential==0)
		{
			printf("a^-\t");
		}
		else
		{
			printf("a^%d\t", exponential-1);
		}
		printPolyFormWithTap(*(p->element+i));
		printf("%d\t", (*(p->element+i))->intValue);
		//intValue
//		printf("%d", convertGaloisFieldPolyFormUnsignedInt(*(p->element+i)));
		printf("%d", (*(p->element+i))->index);
			if((global_flag_cmdOption&FLAG_MASK_PRINTF_MEMORY_CHECK_PROCESS)==FLAG_MASK_PRINTF_MEMORY_CHECK_PROCESS)
			{
				printf("ADDR : 0x%x\n", *(p->element+i));
			}
		printf("\n");
	}
	printf("---------------------- END ----------------------\n");
}
//////////////////////////////////////////////////////////////////////////////////
void printGaloisFieldToCsv(struct_galoisFieldElements *p)
{
	unsigned int i,j;

	unsigned int decimal=0;

	unsigned int exponential;
	
	printf("------------------ Galois Field ------------------\n");
	infoMes;
	printf("number of element is \'%d\' GF(%d)\n", p->length, p->length);

	printf("pow\t");
	for(i=0; i<(*(p->element))->length; i++)
	{
		printf("a^%d\t", i);
	}
	printf("dec(abs)");
	printf("\n");
	
	for(i=0; i<p->length; i++)
	{
		exponential=abstractIndexOfPowerFormInElementsOfGaloisFieldUsingIndex(p, *(p->element+i));
		if(exponential==0)
		{
			printf("a^-, ");
		}
		else
		{
			printf("a^%d, ", exponential-1);
		}
		printf("/");
		printPolyFormPure(*(p->element+i));
		printf(", ");
		
		printf("%d\n", convertGaloisFieldPolyFormUnsignedInt(*(p->element+i)));
			if((global_flag_cmdOption&FLAG_MASK_PRINTF_MEMORY_CHECK_PROCESS)==FLAG_MASK_PRINTF_MEMORY_CHECK_PROCESS)
			{
				printf("ADDR : 0x%x\n", *(p->element+i));
			}
	}
	printf("---------------------- END ----------------------\n");
}
//////////////////////////////////////////////////////////////////////////////////
#define PRINT_FIELD_GALOIS_FIELD_TITLE	1
#define PRINT_FIELD_SYNDROME_TITLE		2
#define PRINT_FIELD_EQUATION_TITLE		3

#define PRINT_FIELD_PRINT_OPTION_NUMBERING	(1<<1)


void printGaloisField2(struct_galoisFieldElements *baseGaloisField, struct_galoisFieldElements *printedField, unsigned int title, unsigned int options)
{
	unsigned int i,j;

	unsigned int decimal=0;

	unsigned int exponential;

	if(!baseGaloisField)
	{
		errorMes;
		printf("in printGaloisField2, struct_galoisFieldElements *baseGaloisField is NULL.\n");
		return;
	}
	if(!printedField)
	{
		errorMes;
		printf("in printGaloisField2, struct_galoisFieldElements *printedField is NULL.\n");
		return;
	}
	
	printf("\n------------------ ");
	if(title==PRINT_FIELD_GALOIS_FIELD_TITLE)
	{
		printf("Galois Field");
	}
	else if(title==PRINT_FIELD_SYNDROME_TITLE)
	{
		printf("Syndrome");
	}
	else if(title==PRINT_FIELD_EQUATION_TITLE)
	{
		printf("Equation");
	}
	else
	{
		printf("Undefined kind");
	}
	printf(" ------------------\n\n");
		
	printf(">> numberOfElement = %d\n", printedField->length);
	if((options&PRINT_FIELD_PRINT_OPTION_NUMBERING)==PRINT_FIELD_PRINT_OPTION_NUMBERING)
	{
		printf("index\t");
	}
	printf("pow\t");
	for(i=0; i<(*(printedField->element))->length; i++)
	{
		printf("a^%d\t", i);
	}
	printf("decimal\tdec[abstract]");
	printf("\n");
	
	for(i=0; i<printedField->length; i++)
	{
		if((options&PRINT_FIELD_PRINT_OPTION_NUMBERING)==PRINT_FIELD_PRINT_OPTION_NUMBERING)
		{
			printf("%d : \t", i);
		}
		exponential=abstractIndexOfPowerFormInElementsOfGaloisFieldUsingIndex(baseGaloisField, *(printedField->element+i));
		if(exponential==0)
		{
			printf("a^-\t");
		}
		else
		{
			printf("a^%d\t", exponential-1);
		}
		printPolyFormWithTap(*(printedField->element+i));
		printf("%d\t%d", (*(printedField->element+i))->intValue, convertGaloisFieldPolyFormUnsignedInt(*(printedField->element+i)));

		if((global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)==FLAG_MASK_PRINTF_LOG)
		{
			logMes;
			printf("\t\tpolyForm length=%d", (*(printedField->element+i))->length);
		}
		printf("\n");

	}
	printf("---------------------- END ----------------------\n");
}
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
struct_galoisField_info *createGaloisField_info_emptySpace(struct_powerFormPolynomials *primitivePolynomial)
{
	unsigned int i,j;

	struct_galoisField_info *p;

	char_GALOIS_FIELD_VALUE shiftBuffer;
	
	#ifndef RELEASE
	if(!primitivePolynomial->length)
	{
		errorMes;
		printf("primitive polynomial length is %d\n", primitivePolynomial->length);
		return NULL;
	}
	if(!primitivePolynomial->usedLength)
	{
		errorMes;
		printf("primitive polynomial usedLength is %d\n", primitivePolynomial->usedLength);
		return NULL;
	}
	#endif
	
	
	p=(struct_galoisField_info*)malloc(sizeof(struct_galoisField_info));
	memset(p, 0, sizeof(struct_galoisField_info));
	
	// p->gfBits=&(p->length);
	
	p->primitivePolynomial=primitivePolynomial;
	
	
	if(!(p->nonSorted=createGaloisFieldAndElements(intPower(2, (p->primitivePolynomial->usedLength)-1), p->primitivePolynomial->usedLength-1)))
	{
		errorMes; printf("in createGaloisField_info_emptySpace, createGaloisFieldAndElements has error.\r\n");
	}
	
			#ifndef RELEASE
			if((global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)==FLAG_MASK_PRINTF_LOG)
			{
				logMes;
				printf("primitive polynomial length is \'%d\'(used length) and allocated memory length is \'%d\' \n", primitivePolynomial->length, primitivePolynomial->usedLength);
				printf("number of elements of galois field is %d\n", p->nonSorted->length);
			}
			#endif

	if(!(p->nonSorted->summationReferMatrix=createSummationMatrix_emptySpace(p->nonSorted->length, p->nonSorted->length)))
	{
		errorMes; printf("in createGaloisField_info_emptySpace, createSummationMatrix_emptySpace has error.\r\n");
	}

	return p;
}
//////////////////////////////////////////////////////////////////////////////////
// struct_galoisField_info *createGaloisField(struct_powerFormPolynomials *primitivePolynomial)
// {
	// #ifndef RELEASE
	// if(!primitivePolynomial)
	// {
		// in
	// }
	// #endif
	// struct_galoisField_info *p;
// }
// struct_galoisField_info *createGaloisField_deleted_201604251820(struct_powerFormPolynomials *primitivePolynomial)
// {
	// unsigned int i,j;

	// struct_galoisField_info *p;

	// char_GALOIS_FIELD_VALUE shiftBuffer;
	
	// #ifndef RELEASE
	// if(!primitivePolynomial->length)
	// {
		// errorMes;
		// printf("primitive polynomial length is %d\n", primitivePolynomial->length);
		// return NULL;
	// }
	// if(!primitivePolynomial->usedLength)
	// {
		// errorMes;
		// printf("primitive polynomial usedLength is %d\n", primitivePolynomial->usedLength);
		// return NULL;
	// }
	// #endif
	
	
	// p=(struct_galoisField_info*)malloc(sizeof(struct_galoisField_info));
	// memset(p, 0, sizeof(struct_galoisField_info));
	
	// // p->gfBits=&(p->length);
	
	// p->primitivePolynomial=primitivePolynomial;
	
	
	// p->nonSorted=createGaloisFieldAndElements(intPower(2, (p->primitivePolynomial->usedLength)-1), p->primitivePolynomial->usedLength-1);
	
			// #ifndef RELEASE
			// if((global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)==FLAG_MASK_PRINTF_LOG)
			// {
				// logMes;
				// printf("primitive polynomial length is \'%d\'(used length) and allocated memory length is \'%d\' \n", primitivePolynomial->length, primitivePolynomial->usedLength);
				// printf("number of elements of galois field is %d\n", p->nonSorted->length);
			// }
			// #endif

	// *((*(p->nonSorted->element+1))->value+0)='1';
	// // printf("((*(p->nonSorted->element+1))->length) %d <= (sizeof(u_int_GALOIS_FIELD_INTEGER)*8) %d\n", ((*(p->nonSorted->element+1))->length), (sizeof(uint32_t)*8));
	// if(((*(p->nonSorted->element+1))->length)<=(sizeof(u_int_GALOIS_FIELD_INTEGER)*8))
	// {
		// (*(p->nonSorted->element+1))->intValue=convertGaloisFieldPolyFormUnsignedInt(*(p->nonSorted->element+1));
	// }
	// (*(p->nonSorted->element+1))->index=1;
	
			// #ifndef RELEASE
			// if((global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)==FLAG_MASK_PRINTF_LOG)
			// {
				// logMes;
				// printf("(p->nonSorted->length) = %d\n", (p->nonSorted->length));
				// printf("(*(p->nonSorted->element+0))->length = %d\n", (*(p->nonSorted->element+0))->length);
			// }
			// #endif
	
	
	// // printf("[%05d]%s\n", 0,(*(p->nonSorted->element+0))->value);
	// // printf("[%05d]%s\n", 1,(*(p->nonSorted->element+1))->value);
	// infoMes; printf("[START][Generating GaloisField Elements]\n");
	// for(i=2; i<(p->nonSorted->length); i++)
	// {
		// shiftBuffer=(*((*(p->nonSorted->element+i-1))->value+((*(p->nonSorted->element+i))->length-1)));
		// for(j=(*(p->nonSorted->element+i))->length-1; j!=0; j--)
		// {
			// (*((*(p->nonSorted->element+i))->value+j))=(*((*(p->nonSorted->element+i-1))->value+j-1));
		// }
		// (*((*(p->nonSorted->element+i))->value+j))='0';
		// (*((*(p->nonSorted->element+i))->value+(*(p->nonSorted->element+i))->length))=0;
		// // (*((*(p->nonSorted->element+i-1))->value+((*(p->nonSorted->element+i))->length-1)))=0;
		
		// if(shiftBuffer=='1')
		// {
			// for(j=0; j<(*(p->nonSorted->element+i))->length; j++)
			// {
				// if((*(primitivePolynomial->equation+j))!=(*((*(p->nonSorted->element+i))->value+j)))
				// {
					// (*((*(p->nonSorted->element+i))->value+j))='1';
				// }
				// else
				// {
					// (*((*(p->nonSorted->element+i))->value+j))='0';
				// }
			// }
			// shiftBuffer='0';
		// }
		
		
		// if(((*(p->nonSorted->element+i))->length)<=(sizeof(u_int_GALOIS_FIELD_INTEGER)*8))
		// {
			// (*(p->nonSorted->element+i))->intValue=convertGaloisFieldPolyFormUnsignedInt(*(p->nonSorted->element+i));
		// }
		// (*(p->nonSorted->element+i))->index=i;
		
		// // printf("[%05d]%s\n", i,(*(p->nonSorted->element+i))->value);
	// }
	// infoMes; printf("[END][Generating GaloisField Elements]\n");



		
	// /*create struct_summationMatrix*/
	// infoMes; printf("[START][Generating summation matrix]\n");
	// p->nonSorted->summationReferMatrix=createSummationMatrix(p->nonSorted, p->nonSorted->length, p->nonSorted->length);
	// infoMes; printf("[END][Generating summation matrix]\n");

	// return p;
// }
struct_galoisField_info *createGaloisFieldAndSumMatrix(struct_powerFormPolynomials *primitivePolynomial)
{
	struct_galoisField_info *p;
	
	#ifndef RELEASE
	if(!primitivePolynomial->length)
	{
		errorMes;
		printf("primitive polynomial length is %d\n", primitivePolynomial->length);
		return NULL;
	}
	if(!primitivePolynomial->usedLength)
	{
		errorMes;
		printf("primitive polynomial usedLength is %d\n", primitivePolynomial->usedLength);
		return NULL;
	}
	#endif
	
	if(!(p=createGaloisField_info_emptySpace(primitivePolynomial)))
	{
		errorMes; printf("in createGaloisFieldAndSumMatrix, createGaloisField_info_emptySpace has error.\r\n");
	}
	else
	{
		if(calculateGaloisField(primitivePolynomial, p->nonSorted)<0)
		{
			errorMes; printf("in createGaloisFieldAndSumMatrix, calculateGaloisField has error.\r\n");
		}
		
		/*create struct_summationMatrix*/
		// p->nonSorted->summationReferMatrix=createSummationMatrix(p->nonSorted, p->nonSorted->length, p->nonSorted->length);\
		// printSummationMatrixSavedForm(p->nonSorted->summationReferMatrix);
		if(calculateSummationMatrix(p->nonSorted, p->nonSorted->summationReferMatrix))
		{
			errorMes; printf("in createGaloisFieldAndSumMatrix, calculateSummationMatrix has error.\r\n");
		}
	}
	return p;
}
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
		printf("struct_galoisFieldElements address is wrong(Addr:0x%x).",p);
	}
	
	printf(">> Summation Matrix\n");
	
	printf("+\t|\t");
	
	for(i=0; i<p->length; i++)
	{
		exponentialTemp=abstractIndexOfPowerFormInElementsOfGaloisField_finding(p, *(p->element+i));
		if(exponentialTemp)	printf("a^%d\t", exponentialTemp-1);
		else				printf("a^%c\t", '-');
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
		if(exponentialTemp)	printf("a^%d\t|\t", exponentialTemp-1);
		else				printf("a^%c\t|\t", '-');
		
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
		printf("struct_galoisFieldElements address is wrong(Addr:0x%x).",p);
	}
	
	printf(">> Multiplication Matrix\n");
	
	printf("*\t|\t");
	
	for(i=0; i<p->length; i++)
	{
		exponentialTemp=abstractIndexOfPowerFormInElementsOfGaloisField_finding(p, *(p->element+i));
		if(exponentialTemp)	printf("a^%d\t", exponentialTemp-1);
		else				printf("a^%c\t", '-');
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
		if(exponentialTemp)	printf("a^%d\t|\t", exponentialTemp-1);
		else				printf("a^%c\t|\t", '-');
		
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
unsigned int checkDegreePolynomials(struct_galoisFieldElements *galoisFields, struct_galoisFieldElements *targetPolynomial)
{
	unsigned int i,j;
	// #ifndef RELEASE
		// unsigned int bebuggingTemp;
		// unsigned int bebuggingTemp2;
	// #endif
	if(!targetPolynomial) return -1;
	if(!galoisFields) return -1;

	for(i=targetPolynomial->length; i!=0; i--)
	{
		// bebuggingTemp = abstractIndexOfPowerFormInElementsOfGaloisFieldUsingIndex(galoisFields, *(galoisFields->element+(0)));
		// bebuggingTemp2 = abstractIndexOfPowerFormInElementsOfGaloisFieldUsingIndex(galoisFields, *(targetPolynomial->element+(i-1)));
		// debugMes; printf("index %d == index %d ??\r\n", bebuggingTemp, bebuggingTemp2);

		if(*(targetPolynomial->element+(i-1)) != *(galoisFields->element+(0))) break;
	}
	for(j=0; j<targetPolynomial->length; j++)
	{
		if(*(targetPolynomial->element+(j)) != *(galoisFields->element+(0)))	return i-1;
		else i--;
	}
	
	return 0;
}

//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//////////////////////* Common BCH Codes Calculations start */////////////////////
//////////////////////////////////////////////////////////////////////////////////
/* 
enum KIND_OF_BCH_DECODING_ALGORITHM
{
	KIND_OF_BCH_DECODING_BM,
	KIND_OF_BCH_DECODING_iBM,
	KIND_OF_BCH_DECODING_RiBM,
	KIND_OF_BCH_DECODING_SiBM,
	KIND_OF_BCH_DECODING_mSBS,
	KIND_OF_BCH_DECODING_PETERSON,
	KIND_OF_BCH_DECODING_EUCLIDIAN,
	KIND_OF_BCH_DECODING_END,
};
 */
//////////////////////////////////////////////////////////////////////////////////
struct struct_generalForm_algorithmComponent{
	enum KIND_OF_BCH_DECODING_ALGORITHM KIND_OF_BCH_ALGORITHM;

	struct_powerFormPolynomials *codeWord;
	struct_powerFormPolynomials *errorLocationVector;

	struct_galoisFieldElements *syndrome;
	
	struct_galoisFieldElements *errLocPoly;//it is same with struct_galoisFieldElements *alpha;
	
	// struct_powerFormPolynomials *errorCorectableCodeWord;
	// struct_powerFormPolynomials *correctedCodeWord;
	
	
	unsigned int numberOfCorrection;//correction number t;
	
	unsigned int vMAX;

	int degreeOfErrLocPoly;
	
	unsigned int metricCheck;
}typedef struct_generalForm_algorithmComponent;
//////////////////////////////////////////////////////////////////////////////////
unsigned int cal_numsOfDegreeOfSynd(unsigned int t)
{
	return 2*t;
}
//////////////////////////////////////////////////////////////////////////////////
char syndromeArrayCalculatorUsingString(struct_galoisFieldElements *galoisFields, struct_galoisFieldElements *syndrome, unsigned int lengthOfSyndromeArray, char *string)
{
	unsigned int i, j;
	struct_galoisFieldPolyForm **calculationBuffer;

	unsigned int strLength = strlen(string);
	
	
			#ifndef RELEASE
			if((strLength+1)>galoisFields->length)
			{
				errorMes;
				printf("strLength+1(%d) > galoisFields->length(%d)\n", (strLength+1), galoisFields->length);
				printf("Sring is \"%s\"\n", string);
				return -1;
			}
			#endif
	
	calculationBuffer = createListOfGaloisField(lengthOfSyndromeArray);

	#ifdef USING_OPEN_MP
	#pragma omp parallel for schedule(guided) private(i) shared(lengthOfSyndromeArray, syndrome, calculationBuffer, galoisFields)
	#endif
	/* to calculate syndrome, set initial condition. */
	for(i=0; i<lengthOfSyndromeArray; i++)
	{
		(*(syndrome->element+i))=(*(galoisFields->element+0));
		(*(calculationBuffer+i))=(*(galoisFields->element+0));
	}
	
	
	for(i=0; i<strLength; i++)
	{
		if(*(string+i)=='1')
		{
			
			*(calculationBuffer+0)=(*(galoisFields->element+1+i));//a^0 is decimalic '1'. so it is 1st element of galoisFields(start 0th).
			(*(syndrome->element+0))=sumElementInGF_usingSumMatrixReturnAddr(galoisFields, (*(syndrome->element+0)), *(calculationBuffer+0));
			
			*(calculationBuffer+1)=multiElementsInGF_returnAddr(galoisFields, *(calculationBuffer+0), *(calculationBuffer+0));
			(*(syndrome->element+1))=sumElementInGF_usingSumMatrixReturnAddr(galoisFields, (*(syndrome->element+1)), *(calculationBuffer+1));
		
			for(j=2; j<lengthOfSyndromeArray; j++)
			{
				*(calculationBuffer+j)=multiElementsInGF_returnAddr(galoisFields, *(calculationBuffer+j-1), *(calculationBuffer+0));
				(*(syndrome->element+j))=sumElementInGF_usingSumMatrixReturnAddr(galoisFields, (*(syndrome->element+j)), *(calculationBuffer+j));
			}
		}
	}
	
	closeListOfGaloisField(&calculationBuffer);
	
			#ifndef RELEASE
			if((global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)==FLAG_MASK_PRINTF_LOG)
			{
				logMes;
				printf("result of syndrome calculation\n");
				printGaloisField2(galoisFields, syndrome, PRINT_FIELD_SYNDROME_TITLE, 0);
			}
			#endif
	return 0;
}
//////////////////////////////////////////////////////////////////////////////////
char summationSyndromeArray
	(
		struct_galoisFieldElements *galoisFields, 
		struct_galoisFieldElements *resultSyndrome, 
		struct_galoisFieldElements *operandA, 
		struct_galoisFieldElements *operandB
	)
{
	unsigned int i;
	
	if(!galoisFields) return -1;
	if(!resultSyndrome) return -1;
	if(!operandA) return -1;
	if(!operandB) return -1;
	
	if(!resultSyndrome->length) return -2;
	if(resultSyndrome->length!=operandA->length) return -3;
	if(operandA->length!=operandB->length) return -3;
	
	
	for(i=0; i<resultSyndrome->length; i++)
	{
		(*(resultSyndrome->element+i))=sumElementInGF_usingSumMatrixReturnAddr(galoisFields, (*(operandA->element+i)), (*(operandB->element+i)));
	}
	
	return 0;
}
//////////////////////////////////////////////////////////////////////////////////
char allSyndromeIsZero(struct_galoisFieldElements *galoisFields, struct_galoisFieldElements *targetSyndrome)
{
	unsigned int i;
	
	#ifndef RELEASE
	if(!galoisFields)
	{
		printf("in allSyndromeIsZero, if(!galoisFields) is NULL\r\n");
		return -1;
	}
	if(!targetSyndrome)
	{
		printf("in allSyndromeIsZero, if(!targetSyndrome) is NULL\r\n");
		return -1;
	}	
	if(!targetSyndrome->length)
	{
		printf("in allSyndromeIsZero, if(!targetSyndrome->length) is zero\r\n");
		return -2;
	}
	#endif

	for(i=0; i<targetSyndrome->length; i++)
	{
		if((*(galoisFields->element+0))!=(*(targetSyndrome->element+i)))
		{
			return 0;
		}
	}
	return 1;
}
//////////////////////////////////////////////////////////////////////////////////
void printSyndromeArray(struct_galoisFieldElements *p, struct_galoisFieldElements *galoisFields)
{
	unsigned int i,j;

	unsigned int decimal=0;

	unsigned int exponential;

	if(!p)
	{
		printf("in printSyndromeArray, struct_galoisFieldElements *p is NULL.\r\n");
		return;
	}

	
	printf("------------------ Syndrome Array ------------------\n");
	
	printf("syndrome length is \'%d\'\n", p->length);
	
	printf("index\tSn\tpow\t");
	for(i=0; i<(*(p->element))->length; i++)
	{
		printf("a^%d\t", i);
	}
	printf("\n");
	
	for(i=0; i<p->length; i++)
	{
		printf("%d :\tS_%d : ", i, i+1);
		exponential=abstractIndexOfPowerFormInElementsOfGaloisFieldUsingIndex(galoisFields, *(p->element+i));
		if(exponential==0)
		{
			printf("a^-\t");
		}
		else
		{
			printf("a^%d\t", exponential-1);
		}
		printPolyFormWithTapAndEnter(*(p->element+i));
	}
	printf("----------------------- END -----------------------\n");
}
//////////////////////////////////////////////////////////////////////////////////
void printSyndromeSeriesWithTap(struct_galoisFieldElements *p, struct_galoisFieldElements *galoisFields)
{
	unsigned int i,j;

	unsigned int decimal=0;

	unsigned int exponential;
	
	for(i=0; i<p->length; i++)
	{
		printf("S_%d : ", i);
		exponential=abstractIndexOfPowerFormInElementsOfGaloisFieldUsingIndex(galoisFields, *(p->element+i));
		printf("a^");
		if(exponential==0)
		{
			printf("-\t");
		}
		else
		{
			printf("%d\t", exponential-1);
		}
	}
}
//////////////////////////////////////////////////////////////////////////////////
void printSyndromeSeriesWithTapAndEnter(struct_galoisFieldElements *p, struct_galoisFieldElements *galoisFields)
{
	unsigned int i,j;

	unsigned int decimal=0;

	unsigned int exponential;
	
	for(i=0; i<p->length; i++)
	{
		printf("S_%d : ", i);
		exponential=abstractIndexOfPowerFormInElementsOfGaloisFieldUsingIndex(galoisFields, *(p->element+i));
		printf("a^");
		if(exponential==0)
		{
			printf("-\t");
		}
		else
		{
			printf("%d\t", exponential-1);
		}
	}
	printf("\n");
}
//////////////////////////////////////////////////////////////////////////////////
///////////////////////////////* chein search start */////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
char calculateChienSearchVariableSet(struct_galoisFieldElements *galoisFields, struct_galoisFieldPolyForm **variableSet, struct_galoisFieldPolyForm *firstOrder, unsigned int orderLength)
{
	unsigned int i;

	#ifndef RELEASE
	if(!variableSet)
	{
		errorMes;
		printf("in calculateChienSearchVariable, struct_galoisFieldPolyForm **variableSet is NULL.\n");
		return -1;
	}
	if(!firstOrder)
	{
		errorMes;
		printf("in calculateChienSearchVariable, struct_galoisFieldPolyForm *firstOrder is NULL.\n");
		return -1;
	}
	#endif

	(*(variableSet+0))=firstOrder;
	if(orderLength<1)
	{
		return 0;
	}

	*(variableSet+1)=multiElementsInGF_returnAddr(galoisFields, (*(variableSet+0)), (*(variableSet+0)));
	if(orderLength<2)
	{
		return 0;
	}

	for(i=2; i<orderLength; i++)
	{
		*(variableSet+i)=multiElementsInGF_returnAddr(galoisFields, (*(variableSet+i-1)), (*(variableSet+0)));
	}
	return 0;
}
//////////////////////////////////////////////////////////////////////////////////
enum CHIEN_SEARCH_DIRECTION
{
	CHIEN_SEARCH_DIRECTION_BACKWARD = 0,
	CHIEN_SEARCH_DIRECTION_FORWARD = 1,
}typedef enum_CHIEN_SEARCH_DIRECTION;
char calculateChienSearch_direction
	(
		struct_galoisFieldElements *galoisFields, 
		struct_galoisFieldElements *errLocPolynomial, 
		struct_powerFormPolynomials *errorCorectableCodeWord,
		unsigned int *metricCheck,
		enum CHIEN_SEARCH_DIRECTION direction
		
	)
{
	unsigned int i, j;

	struct_galoisFieldPolyForm **variableSet;

	struct_galoisFieldPolyForm *summationResult;

	struct_galoisFieldPolyForm *tmp_location;
	
	#ifndef RELEASE
	if(!galoisFields)
	{
		errorMes;
		printf("in calculateChienSearch_static_backWard, struct_galoisFieldElements *galoisFields is NULL.\n");
		return -1;
	}
	if(!errLocPolynomial)
	{
		errorMes;
		printf("in calculateChienSearch_static_backWard, struct_galoisFieldElements *errLocPolynomial is NULL.\n");
		return -1;
	}
	if(!errorCorectableCodeWord)
	{
		errorMes;
		printf("in calculateChienSearch_static_backWard, struct_powerFormPolynomials *errorCorectableCodeWord is NULL.\n");
		return -1;
	}
	#endif
	
			#ifndef RELEASE
			if((global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)==FLAG_MASK_PRINTF_LOG)
			{
				logMes;
				printf("in calculateChienSearch_static_backWard, formula errLocPolynomial is below...\n");
				printGaloisField2(galoisFields, errLocPolynomial, PRINT_FIELD_EQUATION_TITLE, PRINT_FIELD_PRINT_OPTION_NUMBERING);
				
				logMes;
				printf("Start from 1 to length of GF is \'%d\', errorCorectableCodeWord length is [full]\'%d\', [used]\'%d\'", galoisFields->length, errorCorectableCodeWord->length, errorCorectableCodeWord->usedLength);
				
			}
			#endif
	variableSet=(struct_galoisFieldPolyForm**)malloc(sizeof(struct_galoisFieldPolyForm*)*(errLocPolynomial->length-1));
	memset(variableSet, 0, sizeof(struct_galoisFieldPolyForm*)*(errLocPolynomial->length-1));
	
	for(i=1; i<errorCorectableCodeWord->usedLength+1; i++)
	{
		/*calculateChienSearchVariableSet is x , x^2, x^3, x^4 , etc...*/
		if(direction)	tmp_location = (*(galoisFields->element+i));
		else 			tmp_location = invertExponentialElementInGaloisField(galoisFields, *(galoisFields->element+i));
		#ifndef RELEASE
		if(calculateChienSearchVariableSet(galoisFields, variableSet, tmp_location, (errLocPolynomial->length-1)))
		{
			errorMes;
			printf("in calculateChienSearch_static_backWard, calculateChienSearchVariableSet is error.\n");
			return -1;
		}
		#else
		calculateChienSearchVariableSet(galoisFields, variableSet, tmp_location, (errLocPolynomial->length-1));
		#endif
	

				#ifndef RELEASE
				/*to confirm code start*/
				if(global_flag_cmdOption&FLAG_MASK_PRINTF_CALCULATE_PROCESS)
				{
					printf("a^%d\t", i-1);
					printPolyForm(*(galoisFields->element+i));
					printf("\t| -(exp)> |\ta^%d\t=a^%d\t", -(i-1), abstractIndexOfPowerFormInElementsOfGaloisFieldUsingIndex(galoisFields, invertExponentialElementInGaloisField(galoisFields, *(galoisFields->element+i)))-1);
					printPolyForm(invertExponentialElementInGaloisField(galoisFields, *(galoisFields->element+i)));
					printf("\t|\t");
					for(j=0; j<(errLocPolynomial->length-1); j++)
					{
						printPolyForm(*(variableSet+j));
						printf("\t");

					}
					printf("\n");
				}
				/*to confirm code end*/
				#endif
				
				#ifndef RELEASE
				if((global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)==FLAG_MASK_PRINTF_LOG)
				{
					logMes;
					printf("<< variableSet >>\n");
					printf("a^-%d = a^%d => ", 
						abstractIndexOfPowerFormInElementsOfGaloisFieldUsingIndex(galoisFields, *(galoisFields->element+i))-1,
						abstractIndexOfPowerFormInElementsOfGaloisFieldUsingIndex(galoisFields, invertExponentialElementInGaloisField(galoisFields, *(galoisFields->element+i)))-1
						);
					for(j=0; j<(errLocPolynomial->length-1); j++)
					{
						printf("(j=%d)a^%d(a^-%d*%d)\n", j,
							abstractIndexOfPowerFormInElementsOfGaloisFieldUsingIndex(galoisFields, *(variableSet+j))-1,
							abstractIndexOfPowerFormInElementsOfGaloisFieldUsingIndex(galoisFields, *(galoisFields->element+i))-1, 
							j
							);
					}
					printf("\n");
				}
				#endif
		
				#ifndef RELEASE
				if((global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)==FLAG_MASK_PRINTF_LOG)
				{
					logMes;
					printf("i=%d, A( a^-%d ) = \t", i, abstractIndexOfPowerFormInElementsOfGaloisFieldUsingIndex(galoisFields, *(galoisFields->element+i))-1);
				}
				#endif
		
		summationResult=*(errLocPolynomial->element+0);
				#ifndef RELEASE
				if((global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)==FLAG_MASK_PRINTF_LOG)
				{
					logMes;
					printf("a^-%d + \t", abstractIndexOfPowerFormInElementsOfGaloisFieldUsingIndex(galoisFields, summationResult)-1);
				}
				#endif
		
		/*
			ErrorLocPolynomials(errLocPolynomial) = 1 + ramda_0*errLocPolynomial^1 + ramda_0*errLocPolynomial^2 + ramda_0*errLocPolynomial^3 + ramda_0*errLocPolynomial^4 + ramda_0*errLocPolynomial^5 + ramda_0*errLocPolynomial^6 + ...
		*/
		for(j=1; j<(errLocPolynomial->length); j++)
		{
			summationResult=sumElementInGF_usingSumMatrixReturnAddr(galoisFields, summationResult, multiElementsInGF_returnAddr(galoisFields, *(errLocPolynomial->element+j), *(variableSet+j-1)));

					#ifndef RELEASE
					if((global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)==FLAG_MASK_PRINTF_LOG)
					{
						logMes;
						printf("a^%d *\ta^%d(a^(-%d*%d)) ", 
						abstractIndexOfPowerFormInElementsOfGaloisFieldUsingIndex(galoisFields, *(errLocPolynomial->element+j))-1, 
						abstractIndexOfPowerFormInElementsOfGaloisFieldUsingIndex(galoisFields, *(variableSet+j-1))-1, 
						abstractIndexOfPowerFormInElementsOfGaloisFieldUsingIndex(galoisFields, *(galoisFields->element+i))-1,
						j);
						
						if(j!=(errLocPolynomial->length)-1)
						{
							printf("+\t");
						}
						else
						{
							printf("\t");
						}
					}
					#endif
		}
		
		if(checkValueFromPolyFormUsingGaloisFieldValueUsingIntValue_(*(galoisFields->element+0), summationResult))
		{
					#ifndef RELEASE
					if((global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)==FLAG_MASK_PRINTF_LOG)
					{
						printf("<success>\t");
					}
					#endif
			*(errorCorectableCodeWord->equation+(i-1))='1';
			if(metricCheck) (*metricCheck)++;
		}
		else
		{
					#ifndef RELEASE
					if((global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)==FLAG_MASK_PRINTF_LOG)
					{
						printf("<fail>\t");
					}
					#endif
			*(errorCorectableCodeWord->equation+(i-1))='0';
		}

		
				#ifndef RELEASE
				if((global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)==FLAG_MASK_PRINTF_LOG)
				{
					printf("= a^%d\t", abstractIndexOfPowerFormInElementsOfGaloisFieldUsingIndex(galoisFields, summationResult)-1);
					printf(" (%c)\n", *(errorCorectableCodeWord->equation+(i-1)));
				}
				#endif
	}
	
			#ifndef RELEASE
			if((global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)==FLAG_MASK_PRINTF_LOG)
			{
				logMes;
				printf("correctable codeWord \"%s\"\n", errorCorectableCodeWord->equation);
				printf("variableSet addr(0x%x)\n", variableSet);
			}
			#endif
	free(variableSet);
	return 0;
}
//////////////////////////////////////////////////////////////////////////////////
char chienSearch_offset_direction
	(
		struct_galoisFieldElements *galoisFields, 
		struct_galoisFieldElements *errLocPolynomial, 
		struct_powerFormPolynomials *errorLocationPolynomial,
		unsigned int *metricCheck,
		enum CHIEN_SEARCH_DIRECTION direction
		
	)
{
	#ifndef RELEASE
	if(!galoisFields)
	{
		errorMes;
		printf("in chienSearch_offset_direction, struct_galoisFieldElements *galoisFields is NULL.\n");
		return -1;
	}
	if(!errLocPolynomial)
	{
		errorMes;
		printf("in chienSearch_offset_direction, struct_HD_BM_algorithmComponent *errLocPolynomial is NULL.\n");
		return -1;
	}
	#endif
	
	#ifndef RELEASE
	if(calculateChienSearch_direction(galoisFields, errLocPolynomial, errorLocationPolynomial, metricCheck, direction))
	{
		errorMes;
		printf("in chienSearch_offset_direction, calculateChienSearch_direction is fail.\n");
		return -1;
	}
	#else
	calculateChienSearch_direction(galoisFields, errLocPolynomial, errorLocationPolynomial, metricCheck, direction);
	#endif

			#ifndef RELEASE
			if((global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)==FLAG_MASK_PRINTF_LOG)
			{
				logMes;
				printf("in chienSearch_offset_direction, calculated errorLocationPolynomial is below : \n");
				printPowerFormWithEnterPolynomialAndInfoUsingAddress(errorLocationPolynomial);
			}
			#endif
	
	return 0;
}
//////////////////////////////////////////////////////////////////////////////////
char calculateChienSearch_static_backWard
	(
		struct_galoisFieldElements *galoisFields, 
		struct_galoisFieldElements *errLocPolynomial, 
		struct_powerFormPolynomials *errorCorectableCodeWord,
		unsigned int *metricCheck
	)
{
	unsigned int i, j;

	struct_galoisFieldPolyForm **variableSet;

	struct_galoisFieldPolyForm *summationResult;

	#ifndef RELEASE
	if(!galoisFields)
	{
		errorMes;
		printf("in calculateChienSearch_static_backWard, struct_galoisFieldElements *galoisFields is NULL.\n");
		return -1;
	}
	if(!errLocPolynomial)
	{
		errorMes;
		printf("in calculateChienSearch_static_backWard, struct_galoisFieldElements *errLocPolynomial is NULL.\n");
		return -1;
	}
	if(!errorCorectableCodeWord)
	{
		errorMes;
		printf("in calculateChienSearch_static_backWard, struct_powerFormPolynomials *errorCorectableCodeWord is NULL.\n");
		return -1;
	}
	#endif
	
			#ifndef RELEASE
			if((global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)==FLAG_MASK_PRINTF_LOG)
			{
				logMes;
				printf("in calculateChienSearch_static_backWard, formula errLocPolynomial is below...\n");
				printGaloisField2(galoisFields, errLocPolynomial, PRINT_FIELD_EQUATION_TITLE, PRINT_FIELD_PRINT_OPTION_NUMBERING);
				
				logMes;
				printf("Start from 1 to length of GF is \'%d\', errorCorectableCodeWord length is [full]\'%d\', [used]\'%d\'", galoisFields->length, errorCorectableCodeWord->length, errorCorectableCodeWord->usedLength);
				
			}
			#endif
	variableSet=(struct_galoisFieldPolyForm**)malloc(sizeof(struct_galoisFieldPolyForm*)*(errLocPolynomial->length-1));
	memset(variableSet, 0, sizeof(struct_galoisFieldPolyForm*)*(errLocPolynomial->length-1));
	
	for(i=1; i<errorCorectableCodeWord->usedLength+1; i++)
	{
		/*calculateChienSearchVariableSet is x , x^2, x^3, x^4 , etc...*/
		#ifndef RELEASE
		if(calculateChienSearchVariableSet(galoisFields, variableSet, invertExponentialElementInGaloisField(galoisFields, *(galoisFields->element+i)), (errLocPolynomial->length-1)))
		{
			errorMes;
			printf("in calculateChienSearch_static_backWard, calculateChienSearchVariableSet is error.\n");
			return -1;
		}
		#else
		calculateChienSearchVariableSet(galoisFields, variableSet, invertExponentialElementInGaloisField(galoisFields, *(galoisFields->element+i)), (errLocPolynomial->length-1));
		#endif
	

				#ifndef RELEASE
				/*to confirm code start*/
				if(global_flag_cmdOption&FLAG_MASK_PRINTF_CALCULATE_PROCESS)
				{
					printf("a^%d\t", i-1);
					printPolyForm(*(galoisFields->element+i));
					printf("\t| -(exp)> |\ta^%d\t=a^%d\t", -(i-1), abstractIndexOfPowerFormInElementsOfGaloisFieldUsingIndex(galoisFields, invertExponentialElementInGaloisField(galoisFields, *(galoisFields->element+i)))-1);
					printPolyForm(invertExponentialElementInGaloisField(galoisFields, *(galoisFields->element+i)));
					printf("\t|\t");
					for(j=0; j<(errLocPolynomial->length-1); j++)
					{
						printPolyForm(*(variableSet+j));
						printf("\t");

					}
					printf("\n");
				}
				/*to confirm code end*/
				#endif
				
				#ifndef RELEASE
				if((global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)==FLAG_MASK_PRINTF_LOG)
				{
					logMes;
					printf("<< variableSet >>\n");
					printf("a^-%d = a^%d => ", 
						abstractIndexOfPowerFormInElementsOfGaloisFieldUsingIndex(galoisFields, *(galoisFields->element+i))-1,
						abstractIndexOfPowerFormInElementsOfGaloisFieldUsingIndex(galoisFields, invertExponentialElementInGaloisField(galoisFields, *(galoisFields->element+i)))-1
						);
					for(j=0; j<(errLocPolynomial->length-1); j++)
					{
						printf("(j=%d)a^%d(a^-%d*%d)\n", j,
							abstractIndexOfPowerFormInElementsOfGaloisFieldUsingIndex(galoisFields, *(variableSet+j))-1,
							abstractIndexOfPowerFormInElementsOfGaloisFieldUsingIndex(galoisFields, *(galoisFields->element+i))-1, 
							j
							);
					}
					printf("\n");
				}
				#endif
		
				#ifndef RELEASE
				if((global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)==FLAG_MASK_PRINTF_LOG)
				{
					logMes;
					printf("i=%d, A( a^-%d ) = \t", i, abstractIndexOfPowerFormInElementsOfGaloisFieldUsingIndex(galoisFields, *(galoisFields->element+i))-1);
				}
				#endif
		
		summationResult=*(errLocPolynomial->element+0);
				#ifndef RELEASE
				if((global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)==FLAG_MASK_PRINTF_LOG)
				{
					logMes;
					printf("a^-%d + \t", abstractIndexOfPowerFormInElementsOfGaloisFieldUsingIndex(galoisFields, summationResult)-1);
				}
				#endif
		
		/*
			ErrorLocPolynomials(errLocPolynomial) = 1 + ramda_0*errLocPolynomial^1 + ramda_0*errLocPolynomial^2 + ramda_0*errLocPolynomial^3 + ramda_0*errLocPolynomial^4 + ramda_0*errLocPolynomial^5 + ramda_0*errLocPolynomial^6 + ...
		*/
		for(j=1; j<(errLocPolynomial->length); j++)
		{
			summationResult=sumElementInGF_usingSumMatrixReturnAddr(galoisFields, summationResult, multiElementsInGF_returnAddr(galoisFields, *(errLocPolynomial->element+j), *(variableSet+j-1)));

					#ifndef RELEASE
					if((global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)==FLAG_MASK_PRINTF_LOG)
					{
						logMes;
						printf("a^%d *\ta^%d(a^(-%d*%d)) ", 
						abstractIndexOfPowerFormInElementsOfGaloisFieldUsingIndex(galoisFields, *(errLocPolynomial->element+j))-1, 
						abstractIndexOfPowerFormInElementsOfGaloisFieldUsingIndex(galoisFields, *(variableSet+j-1))-1, 
						abstractIndexOfPowerFormInElementsOfGaloisFieldUsingIndex(galoisFields, *(galoisFields->element+i))-1,
						j);
						
						if(j!=(errLocPolynomial->length)-1)
						{
							printf("+\t");
						}
						else
						{
							printf("\t");
						}
					}
					#endif
		}
		
		if(checkValueFromPolyFormUsingGaloisFieldValueUsingIntValue_(*(galoisFields->element+0), summationResult))
		{
					#ifndef RELEASE
					if((global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)==FLAG_MASK_PRINTF_LOG)
					{
						printf("<success>\t");
					}
					#endif
			*(errorCorectableCodeWord->equation+(i-1))='1';
			if(metricCheck) (*metricCheck)++;
		}
		else
		{
					#ifndef RELEASE
					if((global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)==FLAG_MASK_PRINTF_LOG)
					{
						printf("<fail>\t");
					}
					#endif
			*(errorCorectableCodeWord->equation+(i-1))='0';
		}

		
				#ifndef RELEASE
				if((global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)==FLAG_MASK_PRINTF_LOG)
				{
					printf("= a^%d\t", abstractIndexOfPowerFormInElementsOfGaloisFieldUsingIndex(galoisFields, summationResult)-1);
					printf(" (%c)\n", *(errorCorectableCodeWord->equation+(i-1)));
				}
				#endif
	}
	
			#ifndef RELEASE
			if((global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)==FLAG_MASK_PRINTF_LOG)
			{
				logMes;
				printf("correctable codeWord \"%s\"\n", errorCorectableCodeWord->equation);
				printf("variableSet addr(0x%x)\n", variableSet);
			}
			#endif
	free(variableSet);
	return 0;
}
//////////////////////////////////////////////////////////////////////////////////
char chienSearch_static
	(
		struct_galoisFieldElements *galoisFields, 
		struct_galoisFieldElements *errLocPolynomial, 
		struct_powerFormPolynomials *errorLocationPolynomial,
		unsigned int *metricCheck
	)
{
	#ifndef RELEASE
	if(!galoisFields)
	{
		errorMes;
		printf("in chienSearch_static, struct_galoisFieldElements *galoisFields is NULL.\n");
		return -1;
	}
	if(!errLocPolynomial)
	{
		errorMes;
		printf("in chienSearch_static, struct_HD_BM_algorithmComponent *errLocPolynomial is NULL.\n");
		return -1;
	}
	#endif
	
	#ifndef RELEASE
	if(calculateChienSearch_static_backWard(galoisFields, errLocPolynomial, errorLocationPolynomial, metricCheck))
	{
		errorMes;
		printf("in chienSearch_static, calculateChienSearch_static_backWard is fail.\n");
		return -1;
	}
	#else
	calculateChienSearch_static_backWard(galoisFields, errLocPolynomial, errorLocationPolynomial, metricCheck);
	#endif

			#ifndef RELEASE
			if((global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)==FLAG_MASK_PRINTF_LOG)
			{
				logMes;
				printf("in chienSearch_static, calculated errorLocationPolynomial is below : \n");
				printPowerFormWithEnterPolynomialAndInfoUsingAddress(errorLocationPolynomial);
			}
			#endif
	
	return 0;
}
//////////////////////////////////////////////////////////////////////////////////
char chienSearch
	(
		struct_galoisFieldElements *galoisFields, 
		struct_galoisFieldElements *errLocPolynomial, 
		struct_powerFormPolynomials *errorLocationPolynomial,
		unsigned int *metricCheck,
		enum KIND_OF_BCH_DECODING_ALGORITHM KIND_OF_BCH_ALGORITHM
	)
{
	switch(KIND_OF_BCH_ALGORITHM)
	{
		case KIND_OF_BCH_DECODING_BM:
			return chienSearch_static(galoisFields, errLocPolynomial, errorLocationPolynomial, metricCheck);
		break;
		case KIND_OF_BCH_DECODING_iBM:
			printf("in chienSearch, %s is not support yet.\r\n", KIND_OF_BCH_DECODING_ALGORITHM_NAME[KIND_OF_BCH_ALGORITHM]);
		break;
		case KIND_OF_BCH_DECODING_RiBM:
			printf("in chienSearch, %s is not support yet.\r\n", KIND_OF_BCH_DECODING_ALGORITHM_NAME[KIND_OF_BCH_ALGORITHM]);
		break;
		case KIND_OF_BCH_DECODING_SiBM:
			printf("in chienSearch, %s is not support yet.\r\n", KIND_OF_BCH_DECODING_ALGORITHM_NAME[KIND_OF_BCH_ALGORITHM]);
		break;
		case KIND_OF_BCH_DECODING_mSBS:
			return chienSearch_offset_direction(galoisFields, errLocPolynomial, errorLocationPolynomial, metricCheck, CHIEN_SEARCH_DIRECTION_FORWARD);
		break;
		case KIND_OF_BCH_DECODING_PETERSON:
			printf("in chienSearch, %s is not support yet.\r\n", KIND_OF_BCH_DECODING_ALGORITHM_NAME[KIND_OF_BCH_ALGORITHM]);
		break;
		case KIND_OF_BCH_DECODING_EUCLIDIAN:
			printf("in chienSearch, %s is not support yet.\r\n", KIND_OF_BCH_DECODING_ALGORITHM_NAME[KIND_OF_BCH_ALGORITHM]);
		default:
		break;
	}
}
//////////////////////////////////////////////////////////////////////////////////
char errorCorrection(struct_powerFormPolynomials *targetCodeWord, struct_powerFormPolynomials *errorLocationPowerFormPolynomial)
{
	unsigned int i;
	
	#ifndef RELEASE
	if(!targetCodeWord)
	{
		errorMes;
		printf("in errorCorrection, struct_powerFormPolynomials *targetCodeWord is NULL.\n");
		return -1;
	}
	if(!errorLocationPowerFormPolynomial)
	{
		errorMes;
		printf("in errorCorrection, struct_powerFormPolynomials *errorLocationPowerFormPolynomial is NULL.\n");
		return -1;
	}
	if(targetCodeWord->usedLength!=errorLocationPowerFormPolynomial->usedLength)
	{
		errorMes;
		printf("in errorCorrection, targetCodeWord->usedLength(%d)!=errorLocationPowerFormPolynomial->usedLength(%d).\n", targetCodeWord->usedLength, errorLocationPowerFormPolynomial->usedLength);
		printf("targetCodeWord->length(%d)!=errorLocationPowerFormPolynomial->length(%d).\n", targetCodeWord->length, errorLocationPowerFormPolynomial->length);
		return -1;
	}
	if(!targetCodeWord->usedLength)
	{
		errorMes;
		printf("in errorCorrection, targetCodeWord->usedLength(%d) and errorLocationPowerFormPolynomial->usedLength(%d) are zero.\n", targetCodeWord->usedLength, errorLocationPowerFormPolynomial->usedLength);
		return -1;
	}
	#endif

	
	#ifdef USING_OPEN_MP
	#pragma omp parallel for schedule(guided) private(i) shared(targetCodeWord, errorLocationPowerFormPolynomial)
	#endif
	for(i=0; i<targetCodeWord->usedLength; i++)
	{
		if(*(errorLocationPowerFormPolynomial->equation+i)=='1')
		{
			if(*(targetCodeWord->equation+i)=='1')
			{
				*(targetCodeWord->equation+i)='0';
			}
			else
			{
				*(targetCodeWord->equation+i)='1';
			}
		}
	}

			#ifndef RELEASE
			if((global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)==FLAG_MASK_PRINTF_LOG)
			{
				logMes;
				printf("corrected code : ");
				printPowerFormWithEnterPolynomialAndInfoUsingAddress(targetCodeWord);
			}
			#endif
	
	return 0;
}
//////////////////////////////////////////////////////////////////////////////////
///////////////////////* Common BCH Codes Calculations end *//////////////////////
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
/////////////////////////* BM algorithm components start *////////////////////////
//////////////////////////////////////////////////////////////////////////////////
struct struct_HD_BM_algorithmComponent{
	enum KIND_OF_BCH_DECODING_ALGORITHM KIND_OF_BCH_ALGORITHM;

	struct_powerFormPolynomials *codeWord;
	struct_powerFormPolynomials *errorLocationVector;

	struct_galoisFieldElements *syndrome;
	
	struct_galoisFieldElements *errLocPoly;//it is same with struct_galoisFieldElements *alpha;
	
	// struct_powerFormPolynomials *errorCorectableCodeWord;
	// struct_powerFormPolynomials *correctedCodeWord;
	
	
	unsigned int numberOfCorrection;//correction number t;
	
	unsigned int vMAX;

	int degreeOfErrLocPoly;
	
	unsigned int metricCheck;
	
	unsigned int value_r;
	
	unsigned char calculationSelector;
		
	struct_galoisFieldPolyForm *delta;//just pointer, not create a anything.
	
	struct_galoisFieldElements *alpha;
	struct_galoisFieldElements *beforeAlpha;
	struct_galoisFieldElements *beta;
	struct_galoisFieldElements *beforeBeta;
	
	unsigned int lengthOfStage;
	
	/*to test ,soft decision*/
	struct_galoisFieldElements *errLocSyndrome;
	struct_galoisFieldElements *sumReceiveAndErrLocSyndrome;

}typedef struct_HD_BM_algorithmComponent;
//////////////////////////////////////////////////////////////////////////////////
struct_HD_BM_algorithmComponent *createBmAlgorithmComponent(struct_galoisFieldElements *galoisFields, unsigned int t, char *string, unsigned int stringLength)
{
	struct_HD_BM_algorithmComponent *p;

	double galoisFieldElementLengthBuffer;
	unsigned int galoisFieldElementLength;

	#ifndef RELEASE
	if(!t)
	{
		errorMes;
		printf("correctable number(t) is %d\n", t);
	}
	if(!string)
	{
		errorMes;
		printf("to calculate syndrome, string(cordWord) is not inserted\n");
	}

	if(!stringLength)
	{
		warningMesShort; printf("to calculate syndrome, string(cordWord) length is %s\n", stringLength);
		warningMesShort; printf("string : %s\n", string);
		stringLength=strlen(string);
	}
	#endif
	
			#ifndef RELEASE
			if((global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)==FLAG_MASK_PRINTF_LOG)
			{
				logMes; printf("codeWord is received and set a information to correct error.\n");
			}
			#endif
	p = (struct_HD_BM_algorithmComponent*)malloc(sizeof(struct_HD_BM_algorithmComponent));
	memset(p, 0, sizeof(struct_HD_BM_algorithmComponent));

	
	p->numberOfCorrection = t;
			#ifndef RELEASE
			if((global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)==FLAG_MASK_PRINTF_LOG)
			{
				logMes; printf("number of correctable error is \'%d\', ", p->numberOfCorrection);
			}
			#endif
	
	p->vMAX = cal_numsOfDegreeOfSynd(t);/*use for, when variable is 6, it is break.*/
			#ifndef RELEASE
			if((global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)==FLAG_MASK_PRINTF_LOG)
			{
				logMes; printf("vMAX is \'%d\'\n", p->vMAX);
			}
			#endif
	
	p->codeWord = createPowerFormPolynomialUsingString(string, stringLength);
	p->errorLocationVector = createPowerFormPolynomial(stringLength);

			#ifndef RELEASE
			if((global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)==FLAG_MASK_PRINTF_LOG)
			{
				logMes; printf("received codeword is below : \n");
				printPowerFormWithEnterPolynomialAndInfoUsingAddress(p->codeWord);
			}
			#endif
	
		// galoisFieldElementLengthBuffer=log((double)stringLength)/log(2.0);
		// galoisFieldElementLength=(unsigned int)galoisFieldElementLengthBuffer;

		// while((galoisFieldElementLengthBuffer-((double)galoisFieldElementLength))!=0)
		// {
			// stringLength++;
			// galoisFieldElementLengthBuffer=log((double)stringLength)/log(2.0);
			// galoisFieldElementLength=(unsigned int)galoisFieldElementLengthBuffer;
		// }
	
	/* create syndrome of received vector */
	p->syndrome = createGaloisFieldExceptElements(p->vMAX+global_bch_Soft_OverSyndLen);
	/* calculate syndrome */
	syndromeArrayCalculatorUsingString(galoisFields, p->syndrome, p->vMAX+global_bch_Soft_OverSyndLen, string);
	
	/* create syndrome of error location */
	p->errLocSyndrome = createGaloisFieldExceptElements(p->vMAX+global_bch_Soft_OverSyndLen);
	/* create syndrome about sum syndromes of received and error location vector */
	p->sumReceiveAndErrLocSyndrome = createGaloisFieldExceptElements(p->vMAX+global_bch_Soft_OverSyndLen);
	
	
	
	/* BM algorithm initial condition setting. */
	/*//p->value_r = 0;*/
	
	/* Initializing degree of Error Location Poly */
	p->degreeOfErrLocPoly=-1;
	
	/* clear metricCheck */
	p->metricCheck=0;
	
	/* create alpha and beta formula and set initial condition when value_r is -1. */
	p->alpha = createGaloisFieldExceptElements(t+1);
	p->beta = createGaloisFieldExceptElements(t+1);
		p->errLocPoly = p->alpha;
	initEquation(galoisFields, p->alpha);
	initEquation(galoisFields, p->beta);

			#ifndef RELEASE
			if(global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)
			{
				infoMes;
				printf(">> alpha");
				printGaloisField2(galoisFields, p->alpha, PRINT_FIELD_EQUATION_TITLE, 0);
				infoMes;
				printf(">> beta");
				printGaloisField2(galoisFields, p->beta, PRINT_FIELD_EQUATION_TITLE, 0);
			}
			#endif
	
	/* create output of register */
	p->beforeAlpha = createGaloisFieldExceptElements(t+1);
	p->beforeBeta = createGaloisFieldExceptElements(t+1);
	/* synchronize register and wire, pulse input liked */
	copyListOfGaloisField(p->beforeAlpha->element, p->alpha->element, p->alpha->length);
	copyListOfGaloisField(p->beforeBeta->element, p->beta->element, p->alpha->length);

	
	/* initial condition of length of Stage is 0. */
	p->lengthOfStage=0;
	
	/* indexing kind of bch decoding algorithm */
	p->KIND_OF_BCH_ALGORITHM = KIND_OF_BCH_DECODING_BM;
	
	return p;
}
//////////////////////////////////////////////////////////////////////////////////
char closeBmAlgorithmComponent(struct_HD_BM_algorithmComponent **p)
{
	#ifndef RELEASE
	if(!p)
	{
		if(!(global_flag_cmdSystemUnhold&FLAG_MASK_SYSTEM_UNHOLD_FORCED))
		{
			errorMes;
			printf("in closeBmAlgorithmComponent, closeBmAlgorithmComponent &p = 0x%x\n", p);
		}
		return -1;
	}
	if(!*p)
	{
		if(!(global_flag_cmdSystemUnhold&FLAG_MASK_SYSTEM_UNHOLD_FORCED))
		{
			warningMes;
			printf("in closeBmAlgorithmComponent, closeBmAlgorithmComponent *p = 0x%x\n", *p);
		}
		return -1;
	}
	#endif

	#ifndef RELEASE
		if(closePowerFormPolynomial(&((*p)->codeWord)))
		{
			warningMes; printf("in closeBmAlgorithmComponent, closePowerFormPolynomial(&((*p)->codeWord)) is fail.\n");
		}
	#else
		closePowerFormPolynomial(&((*p)->codeWord));
	#endif
		
	#ifndef RELEASE
		if(closePowerFormPolynomial(&((*p)->errorLocationVector)))
		{
			warningMes; printf("in closeBmAlgorithmComponent, closePowerFormPolynomial(&((*p)->codeWord)) is fail.\n");
		}
	#else
		closePowerFormPolynomial(&((*p)->errorLocationVector));
	#endif

	#ifndef RELEASE
		if(closeGaloisFieldExceptElements(&((*p)->syndrome)))
		{
			warningMes;
			printf("in closeBmAlgorithmComponent, closeGaloisFieldExceptElements(&((*p)->syndrome)) is fail.\n");
		}
		
		if(closeGaloisFieldExceptElements(&((*p)->errLocSyndrome)))
		{
			warningMes;
			printf("in closeBmAlgorithmComponent, closeGaloisFieldExceptElements(&((*p)->errLocSyndrome)) is fail.\n");
		}
		if(closeGaloisFieldExceptElements(&((*p)->sumReceiveAndErrLocSyndrome)))
		{
			warningMes;
			printf("in closeBmAlgorithmComponent, closeGaloisFieldExceptElements(&((*p)->sumReceiveAndErrLocSyndrome)) is fail.\n");
		}
	#else
		closeGaloisFieldExceptElements(&((*p)->syndrome));

		closeGaloisFieldExceptElements(&((*p)->errLocSyndrome));
		closeGaloisFieldExceptElements(&((*p)->sumReceiveAndErrLocSyndrome));
	#endif

	#ifndef RELEASE
		if(closeGaloisFieldExceptElements(&((*p)->alpha)))
		{
			warningMes;
			printf("in closeBmAlgorithmComponent, closeGaloisFieldExceptElements(&((*p)->alpha)) is fail.\n");
		}
	#else
		closeGaloisFieldExceptElements(&((*p)->alpha));
	#endif

	#ifndef RELEASE
		if(closeGaloisFieldExceptElements(&((*p)->beta)))
		{
			warningMes;
			printf("in closeBmAlgorithmComponent, closeGaloisFieldExceptElements(&((*p)->beta)) is fail.\n");
		}
	#else
		closeGaloisFieldExceptElements(&((*p)->beta));
	#endif

	#ifndef RELEASE
		if(closeGaloisFieldExceptElements(&((*p)->beforeAlpha)))
		{
			warningMes;
			printf("in closeBmAlgorithmComponent, closeGaloisFieldExceptElements(&((*p)->beforeAlpha)) is fail.\n");
		}
	#else
		closeGaloisFieldExceptElements(&((*p)->beforeAlpha));
	#endif

	#ifndef RELEASE
		if(closeGaloisFieldExceptElements(&((*p)->beforeBeta)))
		{
			warningMes;
			printf("in closeBmAlgorithmComponent, closeGaloisFieldExceptElements(&((*p)->beforeBeta)) is fail.\n");
		}
	#else
		closeGaloisFieldExceptElements(&((*p)->beforeBeta));
	#endif

	free(*p);
	*p=NULL;
			#ifndef RELEASE
				if(global_flag_cmdOption&FLAG_MASK_PRINTF_MEMORY_CHECK_PROCESS)
				{
					memoryMes;
					printf("free(*p), p = \'0x%x\', *p = \'0x%x\'\n", p, *p);
				}
			#endif

			#ifndef RELEASE
				if((global_flag_cmdOption&FLAG_MASK_PRINTF_LOG))
				{
					infoMes;
					printf("in closeBmAlgorithmComponent, closeBmAlgorithmComponent is done.\n");
				}
			#endif
	return 0;
}
//////////////////////////////////////////////////////////////////////////////////
struct_HD_BM_algorithmComponent *recreateBmAlgorithmComponent(struct_HD_BM_algorithmComponent **p, struct_galoisFieldElements *galoisFields, unsigned int t, char *string, unsigned int stringLength)
{
	#ifndef RELEASE
	if(!p)
	{
		errorMes;
		printf("in recreateBmAlgorithmComponent, struct_HD_BM_algorithmComponent **p is NULL.\n");
		return NULL;
	}
	#endif
	
	if(*p)
	{
			#ifndef RELEASE
			if((global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)==FLAG_MASK_PRINTF_LOG)
			{
				infoMes;
				printf("in recreateBmAlgorithmComponent, closeBmAlgorithmComponent(p); will be excute.\n");
			}
			if(!(global_flag_cmdSystemUnhold&FLAG_MASK_SYSTEM_UNHOLD_FORCED))
			{
				warningMes; printf("in recreateBmAlgorithmComponent, struct_HD_BM_algorithmComponent *p have data already.\n");
				warningMesShort; printf("it will be closed.\n");
			}
			#endif
			
		closeBmAlgorithmComponent(p);
	}

			#ifndef RELEASE
			if((global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)==FLAG_MASK_PRINTF_LOG)
			{
				infoMes;
				printf("in recreateBmAlgorithmComponent, *p=createBmAlgorithmComponent(galoisFields,t, string); will be excute.\n");
				printf("it will be closed.\n");
			}
			#endif
	
	if(!stringLength) stringLength=strlen(string);
		
	*p=createBmAlgorithmComponent(galoisFields,t, string, stringLength);
	return *p;
}
//////////////////////////////////////////////////////////////////////////////////
unsigned char calculateCalculationSelector(struct_galoisFieldPolyForm *delta, unsigned int value_r, unsigned int beforeLengthOfStage)
{
	unsigned int i;

	#ifndef RELEASE
	if(!delta)
	{
		errorMes;
		printf("in calculateCalculationSelector, struct_galoisFieldElements *delta is NULL.\n");
		return -1;
	}
	#endif
		
	if(value_r<(2*beforeLengthOfStage))
	{
		return 0;
	}
	
	for(i=0; i<(delta->length); i++)
	{
		if(*(delta->value+i)!='0')
		{
			break;
		}
	}
	if(i==(delta->length))//delta is all zero.
	{
		return 0;
	}
	
	return 1;
}
//////////////////////////////////////////////////////////////////////////////////
struct_galoisFieldPolyForm *calculcateDeltaInBmAlgorithm
(
	struct_galoisFieldElements *galoisFields, struct_galoisFieldElements *syndrome, 
	struct_galoisFieldElements *beforeStageAlpha, unsigned int lengthOfStage, 
	unsigned int value_r
)
{
	unsigned int i;
	struct_galoisFieldPolyForm *buffer=*(galoisFields->element+0);
	struct_galoisFieldPolyForm *buffer2;
	
	
	unsigned int debugingTemp;
	struct_galoisFieldPolyForm *debuggingTemp2;

	#ifndef RELEASE
	if(!galoisFields)
	{
		errorMes;
		printf("in struct_galoisFieldPolyForm *calculcateDeltaInBmAlgorithm, struct_galoisFieldElements *galoisFields is NULL.\n");
		return NULL;
	}
	if(!syndrome)
	{
		errorMes;
		printf("in struct_galoisFieldPolyForm *calculcateDeltaInBmAlgorithm, struct_galoisFieldElements *syndrome is NULL.\n");
		return NULL;
	}
	if(!beforeStageAlpha)
	{
		errorMes;
		printf("in struct_galoisFieldPolyForm *calculcateDeltaInBmAlgorithm, struct_galoisFieldElements *beforeStageAlpha is NULL.\n");
		return NULL;
	}
	#endif

			#ifndef RELEASE
			if((global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)==FLAG_MASK_PRINTF_LOG)
			{
				logMes;	printf("in calculcateDeltaInBmAlgorithm, [Delta calculation is start]\n");

				printf("(confirm all zero)clear buffer value of summation of delta is... ");
				printPolyFormWithTapAndEnter(buffer);

				printf("start from : %d, end to : %d\n", 0, lengthOfStage);
				
				printf("delta(%d) = ", value_r);
			}
			#endif
	
			#ifndef RELEASE
			if(global_flag_debug_bmSim&FLAG_MASK_DEBUG_BCH_BM_CAL_DELTA)
			{
				debugBmAlgorithmCalDeltaMes;
				printf("for(i=0; i<=lengthOfStage; i++) start, lengthOfStage='%d'\n", lengthOfStage);
			}
			#endif
			
			
			#ifndef RELEASE
			if(beforeStageAlpha->length-1 < lengthOfStage)
			{
				if(!(global_flag_cmdSystemUnhold&FLAG_MASK_SYSTEM_UNHOLD_FORCED))
				{
					warningMes; printf("Case, lengthOfStage is '%d' equal or larger than length of beforeStageAlpha is '%d', and value_r is '%d', MAX(value_r-i) is '%d'\r\n", lengthOfStage, beforeStageAlpha->length, value_r, value_r-lengthOfStage);
					// warningMesShort; printf("occur. press input any string.\n", lengthOfStage);
					// systemHoldMes; printf("you can ignore this exception, adding command option '-f' or \"--force\"\r\n");

					// systemHoldMes; printf("insert any char : "); scanf("%s", global_buf_KeyBoardInput);
				}
			}
			#endif

	for(i=0; i<=lengthOfStage; i++)
	{
				#ifndef RELEASE
				if((global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)==FLAG_MASK_PRINTF_LOG)
				{
					if(debugingTemp)
					{
						printf("(A)a^%d * ",debugingTemp-1);
					}
					else
					{
						printf("(A)a^- * ");
					}
					debugingTemp=abstractIndexOfPowerFormInElementsOfGaloisFieldUsingIndex(galoisFields, *(syndrome->element+(value_r-i)));
					if(debugingTemp)
					{
						printf("(S)a^%d(=",debugingTemp-1);
					}
					else
					{
						printf("(S)a^-(=");
					}

					debuggingTemp2=multiElementsInGF_returnAddr(galoisFields, *(beforeStageAlpha->element+(i)), *(syndrome->element+(value_r-i)));
					debugingTemp=abstractIndexOfPowerFormInElementsOfGaloisFieldUsingIndex(galoisFields, debuggingTemp2);
					if(debugingTemp)
					{
						printf("a^%d)",debugingTemp-1);
					}
					else
					{
						printf("a^-)");
					}
					if(i!=lengthOfStage)
					{
						printf(" +\t");
					}
				}
				#endif
			
				#ifndef RELEASE
				if(global_flag_debug_bmSim&FLAG_MASK_DEBUG_BCH_BM_CAL_DELTA)
				{
					debugBmAlgorithmCalDeltaMes;
					printf("buffer=sumElementInGF_usingSumMatrixReturnAddr(galoisFields, buffer, multiElementsInGF_returnAddr(galoisFields, *(beforeStageAlpha->element+(i)), *(syndrome->element+(value_r-i))));\n");
				}
				#endif
				#ifndef RELEASE
				if(global_flag_debug_bmSim&FLAG_MASK_DEBUG_BCH_BM_CAL_DELTA)
				{
					debugBmAlgorithmCalDeltaMes;
					printf("i = %d, value_r = %d, value_r - i = %d\r\n", i, value_r, value_r-i);
				}
				#endif
		/*
			note about " if(beforeStageAlpha->length-1 < i). "
			If lengthOfStage is larger or equal than length of beforeStageAlpha,
			there is no process elements, so calculate 0
		*/
		if(beforeStageAlpha->length-1 < i)
		{
			buffer2=multiElementsInGF_returnAddr(galoisFields, *(galoisFields->element+(0)), *(syndrome->element+(value_r-i)));
		}
		else
		{
			buffer2=multiElementsInGF_returnAddr(galoisFields, *(beforeStageAlpha->element+(i)), *(syndrome->element+(value_r-i)));
		}
				#ifndef RELEASE
				if(global_flag_debug_bmSim&FLAG_MASK_DEBUG_BCH_BM_CAL_DELTA)
				{
					if(beforeStageAlpha->length<=i)
					{
						debugingTemp=abstractIndexOfPowerFormInElementsOfGaloisFieldUsingIndex(galoisFields, *(galoisFields->element+(0)));
					}
					else
					{
						debugingTemp=abstractIndexOfPowerFormInElementsOfGaloisFieldUsingIndex(galoisFields, *(beforeStageAlpha->element+(i)));
					}

					debugBmAlgorithmCalDeltaMes;
					debugingTemp=abstractIndexOfPowerFormInElementsOfGaloisFieldUsingIndex(galoisFields, buffer);
					if(debugingTemp)
					{
						printf("\t(buffer = )a^%d + ",debugingTemp-1);
					}
					else
					{
						printf("\t(buffer = )a^- + ");
					}
					
					
					debugingTemp=abstractIndexOfPowerFormInElementsOfGaloisFieldUsingIndex(galoisFields, buffer2);
					if(debugingTemp)
					{
						printf("\t(buffer2 = )a^%d = ",debugingTemp-1);
					}
					else
					{
						printf("\t(buffer2 = )a^- = ");
					}
				}
				#endif
				
		buffer=sumElementInGF_usingSumMatrixReturnAddr(galoisFields, buffer, buffer2);
											if(global_flag_debug_bmSim&FLAG_MASK_DEBUG_BCH_BM_CAL_DELTA)
											{
												debugingTemp=abstractIndexOfPowerFormInElementsOfGaloisFieldUsingIndex(galoisFields, buffer);
												if(debugingTemp)
												{
													printf("\t(buffer = )a^%d\n",debugingTemp-1);
												}
												else
												{
													printf("\t(buffer = )a^-\n");
												}
											}
	}
			#ifndef RELEASE
			if(global_flag_debug_bmSim&FLAG_MASK_DEBUG_BCH_BM_CAL_DELTA)
			{
				debugBmAlgorithmCalDeltaMes;
				printf("for(i=0; i<=lengthOfStage; i++) end\n");
			}
			#endif
	
			#ifndef RELEASE
			if((global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)==FLAG_MASK_PRINTF_LOG)
			{
					debugingTemp=abstractIndexOfPowerFormInElementsOfGaloisFieldUsingIndex(galoisFields, buffer);
					if(debugingTemp)
					{
						printf(" = a^%d",debugingTemp-1);
					}
					else
					{
						printf(" = a^-");
					}
				printf("\n");
				logMes;	printf("in calculcateDeltaInBmAlgorithm, [Delta calculation is done]\n");
			}
			#endif
	return buffer;
}
//////////////////////////////////////////////////////////////////////////////////
char calculateAlphaInBmAlgorithm
(
	struct_galoisFieldElements *beforeAlpha, struct_galoisFieldElements *beforeBeta, 
	struct_galoisFieldPolyForm *delta, struct_galoisFieldElements *galoisFields,
	struct_galoisFieldElements *alpha
)
{
	unsigned int i;

	#ifndef RELEASE
	if(!beforeAlpha)
	{
		errorMes;
		printf("in calculateAlphaInBmAlgorithm, struct_galoisFieldElements *beforeAlpha is NULL\n");
		return -1;
	}
	if(!beforeBeta)
	{
		errorMes;
		printf("in calculateAlphaInBmAlgorithm, struct_galoisFieldElements *beforeBeta is NULL\n");
		return -1;
	}
	if(!delta)
	{
		errorMes;
		printf("in calculateAlphaInBmAlgorithm, struct_galoisFieldPolyForm *delta is NULL\n");
		return -1;
	}
	if(!galoisFields)
	{
		errorMes;
		printf("in calculateAlphaInBmAlgorithm, struct_galoisFieldElements *galoisFields is NULL\n");
		return -1;
	}
	if(!alpha)
	{
		errorMes;
		printf("in calculateAlphaInBmAlgorithm, struct_galoisFieldElements *alpha is NULL\n");
		return -1;
	}
	#endif
	
	for(i=1; i<alpha->length; i++)
	{
		*(alpha->element+i)=sumElementInGF_usingSumMatrixReturnAddr(galoisFields, *(beforeAlpha->element+i), multiElementsInGF_returnAddr(galoisFields, delta, *(beforeBeta->element+i-1)));
	}
	return 0;
}
//////////////////////////////////////////////////////////////////////////////////
char calculateBetaInBmAlgorithm
(
	struct_galoisFieldElements *beforeAlpha, struct_galoisFieldElements *beforeBeta, 
	struct_galoisFieldPolyForm *delta, struct_galoisFieldElements *galoisFields,
	unsigned char calculationSelector, struct_galoisFieldElements *beta
)
{
	unsigned int i;

	#ifndef RELEASE
	if(!beforeAlpha)
	{
		errorMes;
		printf("in calculateBetaInBmAlgorithm, struct_galoisFieldElements *beforeAlpha is NULL\n");
		return -1;
	}
	if(!beforeBeta)
	{
		errorMes;
		printf("in calculateBetaInBmAlgorithm, struct_galoisFieldElements *beforeBeta is NULL\n");
		return -1;
	}
	if(!delta)
	{
		errorMes;
		printf("in calculateBetaInBmAlgorithm, struct_galoisFieldPolyForm *delta is NULL\n");
		return -1;
	}
	if(!galoisFields)
	{
		errorMes;
		printf("in calculateBetaInBmAlgorithm, struct_galoisFieldElements *galoisFields is NULL\n");
		return -1;
	}
	if(!beta)
	{
		errorMes;
		printf("in calculateBetaInBmAlgorithm, struct_galoisFieldElements *beta is NULL\n");
		return -1;
	}
	#endif

	/*calculationSelect is '1'.*/
	if(calculationSelector)
	{
		for(i=0; i<beta->length; i++)
		{
			*(beta->element+i)=multiElementsInGF_returnAddr(galoisFields, *(beforeAlpha->element+i), invertExponentialElementInGaloisField(galoisFields, delta));
		}
	}
	/*calculationSelect is '0'. Formula beta is shifted.*/
	else
	{
		for(i=1; i<beta->length; i++)
		{
			*(beta->element+i)=*(beforeBeta->element+i-1);
			
		*(beta->element+0)=*(galoisFields->element+0);
		}
	}

	return 0;
}
//////////////////////////////////////////////////////////////////////////////////
unsigned int calculateBmAlgorithmLengthOfStage(unsigned int value_r, unsigned int lengthOfStage, unsigned char calculationSelector)
{
	/* value_r is even number. */
	if(calculationSelector)
	{
		return (value_r+1-lengthOfStage);
	}
	/* value_r is odd number. */
	else
	{
		return lengthOfStage;
	}
}
//////////////////////////////////////////////////////////////////////////////////

char calculateBmAlgorithm(struct_galoisFieldElements *galoisFields, struct_HD_BM_algorithmComponent *p)
{
	unsigned int bebuggingTemp;

			#ifndef RELEASE
			if(global_flag_debug_bmSim&FLAG_MASK_DEBUG_BCH_BM_ALGORITHM)
			{
				debugBmAlgorithmMes;
				printf("calculateBmAlgorithm\n");
			}
			#endif

	#ifndef RELEASE
	if(!galoisFields)
	{
		errorMes;
		printf("in calculateBmAlgorithm, struct_galoisFieldElements *galoisFields is NULL.\n");
		return -1;
	}
	if(!p)
	{
		errorMes;
		printf("in calculateBmAlgorithm, struct_HD_BM_algorithmComponent *p is NULL.\n");
		return -1;
	}
	#endif
	
			#ifndef RELEASE
			if(((global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)==FLAG_MASK_PRINTF_LOG)||(global_flag_debug_bmSim&FLAG_MASK_DEBUG_BCH_BM_CAL_DELTA))
			{
				logMes;
				printf("Error correction formula calculation is started.\n");
				printf("initial condition is...\n");
				printf(">> lengthOfStage is %d\n", p->lengthOfStage);
				printf(">> alpha");
				printGaloisField2(galoisFields, p->alpha, PRINT_FIELD_EQUATION_TITLE, PRINT_FIELD_PRINT_OPTION_NUMBERING);
				printf(">> beta");
				printGaloisField2(galoisFields, p->beta, PRINT_FIELD_EQUATION_TITLE, PRINT_FIELD_PRINT_OPTION_NUMBERING);
			}
			#endif
			
			#ifndef RELEASE
			if(global_flag_debug_bmSim&FLAG_MASK_DEBUG_BCH_BM_ALGORITHM)
			{
				debugBmAlgorithmMes;
				printf("for(p->value_r=0; p->value_r<p->vMAX; p->value_r++)\n");
			}
			#endif
	for(p->value_r=0; p->value_r<p->vMAX; p->value_r++)
	{
				#ifndef RELEASE
				if(((global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)==FLAG_MASK_PRINTF_LOG)||(global_flag_debug_bmSim&FLAG_MASK_DEBUG_BCH_BM_CAL_DELTA))
				{
					logMes;
					printf("Error correction formula calculation %d-th stage\n", p->value_r);
				}
				#endif

					#ifndef RELEASE
					if(global_flag_debug_bmSim&FLAG_MASK_DEBUG_BCH_BM_ALGORITHM)
					{
						debugBmAlgorithmMes;
						printf("p->delta=calculcateDeltaInBmAlgorithm(galoisFields, p->syndrome, p->beforeAlpha, p->lengthOfStage, p->value_r);\n");
					}
					#endif
		p->delta=calculcateDeltaInBmAlgorithm(galoisFields, p->syndrome, p->beforeAlpha, p->lengthOfStage, p->value_r);
				#ifndef RELEASE
				if(((global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)==FLAG_MASK_PRINTF_LOG)||(global_flag_debug_bmSim&FLAG_MASK_DEBUG_BCH_BM_CAL_DELTA))
				{

					logMes;
					printf("delta is...\n");
					bebuggingTemp=abstractIndexOfPowerFormInElementsOfGaloisFieldUsingIndex(galoisFields, p->delta);
					
					if(bebuggingTemp)
					{
						printf("a^%d\t=\t", bebuggingTemp-1);
					}
					else
					{
						printf("a^-\t=\t");
					}
					printPolyFormWithEnter(p->delta);
				}
				#endif

		/*calculate calculationSelector*/
									#ifndef RELEASE
									if(global_flag_debug_bmSim&FLAG_MASK_DEBUG_BCH_BM_ALGORITHM)
									{
										debugBmAlgorithmMes;
										printf("p->calculationSelector=calculateCalculationSelector(p->delta, p->value_r, p->lengthOfStage);\n");
									}
									#endif
		p->calculationSelector=calculateCalculationSelector(p->delta, p->value_r, p->lengthOfStage);

									#ifndef RELEASE
									if(global_flag_debug_bmSim&FLAG_MASK_DEBUG_BCH_BM_ALGORITHM)
									{
										debugBmAlgorithmMes;
										printf("calculateAlphaInBmAlgorithm(p->beforeAlpha, p->beforeBeta, p->delta, galoisFields, p->alpha);\n");
									}
									#endif
		calculateAlphaInBmAlgorithm(p->beforeAlpha, p->beforeBeta, p->delta, galoisFields, p->alpha);
									#ifndef RELEASE
									if(global_flag_debug_bmSim&FLAG_MASK_DEBUG_BCH_BM_ALGORITHM)
									{
										debugBmAlgorithmMes;
										printf("calculateBetaInBmAlgorithm(p->beforeAlpha, p->beforeBeta, p->delta, galoisFields, p->calculationSelector, p->beta);\n");
									}
									#endif
		calculateBetaInBmAlgorithm(p->beforeAlpha, p->beforeBeta, p->delta, galoisFields, p->calculationSelector, p->beta);
				#ifndef RELEASE
				if(((global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)==FLAG_MASK_PRINTF_LOG)||(global_flag_debug_bmSim&FLAG_MASK_DEBUG_BCH_BM_CAL_DELTA))
				{
					logMes; printf("Small delta(p->calculationSelector) ls '%d'\n", p->calculationSelector); 
					logMes;	printf("check >> alpha");
					printGaloisField2(galoisFields, p->alpha, PRINT_FIELD_EQUATION_TITLE, PRINT_FIELD_PRINT_OPTION_NUMBERING);

					logMes;	printf("check >> beforeAlpha");
					printGaloisField2(galoisFields, p->beforeAlpha, PRINT_FIELD_EQUATION_TITLE, PRINT_FIELD_PRINT_OPTION_NUMBERING);
					
					
					logMes;	printf("check >> beta");
					printGaloisField2(galoisFields, p->beta, PRINT_FIELD_EQUATION_TITLE, PRINT_FIELD_PRINT_OPTION_NUMBERING);
					

					logMes;	printf("check >> beforeBeta");
					printGaloisField2(galoisFields, p->beforeBeta, PRINT_FIELD_EQUATION_TITLE, PRINT_FIELD_PRINT_OPTION_NUMBERING);
				}
				#endif

				
									#ifndef RELEASE
									if(global_flag_debug_bmSim&FLAG_MASK_DEBUG_BCH_BM_ALGORITHM)
									{
										debugBmAlgorithmMes;
										printf("p->lengthOfStage=calculateBmAlgorithmLengthOfStage(p->value_r, p->lengthOfStage, p->calculationSelector);\n");
									}
									#endif
		p->lengthOfStage=calculateBmAlgorithmLengthOfStage(p->value_r, p->lengthOfStage, p->calculationSelector);

				#ifndef RELEASE
				if(((global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)==FLAG_MASK_PRINTF_LOG)||(global_flag_debug_bmSim&FLAG_MASK_DEBUG_BCH_BM_CAL_DELTA))
				{
					logMes;
					printf("L(%d) is %d.\n", p->value_r, p->lengthOfStage);
				}
				#endif
		
				#ifndef RELEASE
				/* synchronize register and wire, pulse input liked */
				if(((global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)==FLAG_MASK_PRINTF_LOG)||(global_flag_debug_bmSim&FLAG_MASK_DEBUG_BCH_BM_CAL_DELTA))
				{
					logMes; printf("before alpha, beta are synchronized.\n");
				}
				#endif
				
									#ifndef RELEASE
									if(global_flag_debug_bmSim&FLAG_MASK_DEBUG_BCH_BM_ALGORITHM)
									{
										debugBmAlgorithmMes;
										printf("copyListOfGaloisField(p->beforeAlpha->element, p->alpha->element, p->alpha->length);\n");
									}
									#endif
		copyListOfGaloisField(p->beforeAlpha->element, p->alpha->element, p->alpha->length);
		
									#ifndef RELEASE
									if(global_flag_debug_bmSim&FLAG_MASK_DEBUG_BCH_BM_ALGORITHM)
									{
										debugBmAlgorithmMes;
										printf("copyListOfGaloisField(p->beforeBeta->element, p->beta->element, p->alpha->length);\n");
									}
									#endif
		copyListOfGaloisField(p->beforeBeta->element, p->beta->element, p->alpha->length);
	}

	p->degreeOfErrLocPoly = checkDegreePolynomials(galoisFields, p->alpha);
									#ifndef RELEASE
									if(global_flag_debug_bmSim&FLAG_MASK_DEBUG_BCH_BM_ALGORITHM)
									{
										debugBmAlgorithmMes; printf("Degree of Polynomials : %d\r\n",p->degreeOfErrLocPoly);
										systemHoldMes;	printf("input any charactor : ");
										scanf("%s", global_buf_KeyBoardInput);
									}
									#endif
	
	return 0;
}
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////* BM algorithm components end */////////////////////////
//////////////////////////////////////////////////////////////////////////////////
///////////////////////* mSBS algorithm components start *////////////////////////
//////////////////////////////////////////////////////////////////////////////////
struct struct_HD_mSBS_t3_algorithmComponent{
	enum KIND_OF_BCH_DECODING_ALGORITHM KIND_OF_BCH_ALGORITHM;

	struct_powerFormPolynomials *codeWord;
	struct_powerFormPolynomials *errorLocationVector;

	struct_galoisFieldElements *syndrome;

	struct_galoisFieldElements *errLocPoly;
	
	unsigned int numberOfCorrection;//In mSBS, maximum numberOfCorrection is 3. so this value is always fixed to 3.

	unsigned int vMAX;

	int degreeOfErrLocPoly;
	
	unsigned int metricCheck;
	
	/* A * alpha^1 + B * alpha^2 + C * alpha^3 + R = 0 */
	/* this polynomial is same with struct_galoisFieldElements *errLocPoly; */
	/*
		refference coefficient is coefficient_of_alpha_three.
		
		If this value is 0, degree of err location polynomials is two.
		That is mean error locations are two.
		
		Else this value is non 0,  degree of err location polynomials is three or more.
	*/
	struct_galoisFieldPolyForm *coefficient_of_alpha_three;//(that is C)
	struct_galoisFieldPolyForm *coefficient_of_alpha_two;//(that is B)
	struct_galoisFieldPolyForm *coefficient_of_alpha_one;//(that is A)
	struct_galoisFieldPolyForm *coefficient_of_alpha_zero;//(that is R)
	
}typedef struct_HD_mSBS_t3_algorithmComponent;
//////////////////////////////////////////////////////////////////////////////////
/*
	ignore t, that is mean correctable bits are already constants.
	maximum correctables are 3.
*/
struct_HD_mSBS_t3_algorithmComponent *create_mSBS_algorithmComponent(struct_galoisFieldElements *galoisFields, unsigned int t, char *string, unsigned int stringLength)
{
	struct_HD_mSBS_t3_algorithmComponent *p;

		// double galoisFieldElementLengthBuffer;
		// unsigned int galoisFieldElementLength;

	#ifndef RELEASE
	
		// if(!t)
		// {
			// errorMes;
			// printf("correctable number(t) is %d\n", t);
		// }
	 
	if(!string)
	{
		errorMes;
		printf("to calculate syndrome, string(cordWord) is not inserted\n");
	}

	if(!stringLength)
	{
		warningMesShort; printf("to calculate syndrome, string(cordWord) length is %s\n", stringLength);
		warningMesShort; printf("string : %s\n", string);
		stringLength=strlen(string);
	}
	#endif
	if(t > 3)
	{
		t = 3;
	}
			#ifndef RELEASE
			if((global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)==FLAG_MASK_PRINTF_LOG)
			{
				logMes; printf("codeWord is received and set a information to correct error.\n");
			}
			#endif
	p = (struct_HD_mSBS_t3_algorithmComponent*)malloc(sizeof(struct_HD_mSBS_t3_algorithmComponent));
	memset(p, 0, sizeof(struct_HD_mSBS_t3_algorithmComponent));
	

	p->numberOfCorrection = t;
			#ifndef RELEASE
			if((global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)==FLAG_MASK_PRINTF_LOG)
			{
				logMes; printf("number of correctable error is \'%d\', ", p->numberOfCorrection);
			}
			#endif

		p->vMAX = cal_numsOfDegreeOfSynd(t);/*use for, when variable is 6, it is break.*/
				// #ifndef RELEASE
				// if((global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)==FLAG_MASK_PRINTF_LOG)
				// {
					// logMes; printf("vMAX is \'%d\'\n", p->vMAX);
				// }
				// #endif
	
	p->codeWord = createPowerFormPolynomialUsingString(string, stringLength);
	p->errorLocationVector = createPowerFormPolynomial(stringLength);

			#ifndef RELEASE
			if((global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)==FLAG_MASK_PRINTF_LOG)
			{
				logMes; printf("received codeword is below : \n");
				printPowerFormWithEnterPolynomialAndInfoUsingAddress(p->codeWord);
			}
			#endif
	
		// galoisFieldElementLengthBuffer=log((double)stringLength)/log(2.0);
		// galoisFieldElementLength=(unsigned int)galoisFieldElementLengthBuffer;

		// while((galoisFieldElementLengthBuffer-((double)galoisFieldElementLength))!=0)
		// {
			// stringLength++;
			// galoisFieldElementLengthBuffer=log((double)stringLength)/log(2.0);
			// galoisFieldElementLength=(unsigned int)galoisFieldElementLengthBuffer;
		// }
	
	/* create syndrome of received vector */
	p->syndrome = createGaloisFieldExceptElements(p->vMAX);
	/* calculate syndrome */
	syndromeArrayCalculatorUsingString(galoisFields, p->syndrome, p->vMAX, string);
	
		// /* create syndrome of error location */
		// p->errLocSyndrome = createGaloisFieldExceptElements(p->vMAX+global_bch_Soft_OverSyndLen);
		// /* create syndrome about sum syndromes of received and error location vector */
		// p->sumReceiveAndErrLocSyndrome = createGaloisFieldExceptElements(p->vMAX+global_bch_Soft_OverSyndLen);
	
	
	
		/* BM algorithm initial condition setting. */
		/*//p->value_r = 0;*/
	
	/* Initializing degree of Error Location Poly */
	p->degreeOfErrLocPoly=-1;
	
	/* clear metricCheck */
	p->metricCheck=0;
	
		/* create alpha and beta formula and set initial condition when value_r is -1. */
		// p->alpha = createGaloisFieldExceptElements(t+1);
		// p->beta = createGaloisFieldExceptElements(t+1);
			// p->errLocPoly = p->alpha;
	p->errLocPoly = createGaloisFieldExceptElements(t+1);
		// initEquation(galoisFields, p->alpha);
		// initEquation(galoisFields, p->beta);
	initEquation(galoisFields, p->errLocPoly);

			#ifndef RELEASE
			if(global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)
			{
				// infoMes;
				// printf(">> alpha");
				// printGaloisField2(galoisFields, p->alpha, PRINT_FIELD_EQUATION_TITLE, 0);
				// infoMes;
				// printf(">> beta");
				// printGaloisField2(galoisFields, p->beta, PRINT_FIELD_EQUATION_TITLE, 0);
					infoMes;
					printf(">> errLocPoly");
					printGaloisField2(galoisFields, p->errLocPoly, PRINT_FIELD_EQUATION_TITLE, 0);
			}
			#endif
	
		// /* create output of register */
		// p->beforeAlpha = createGaloisFieldExceptElements(t+1);
		// p->beforeBeta = createGaloisFieldExceptElements(t+1);
		// /* synchronize register and wire, pulse input liked */
		// copyListOfGaloisField(p->beforeAlpha->element, p->alpha->element, p->alpha->length);
		// copyListOfGaloisField(p->beforeBeta->element, p->beta->element, p->alpha->length);

	
		// /* initial condition of length of Stage is 0. */
		// p->lengthOfStage=0;
	
	/* indexing kind of bch decoding algorithm */
	p->KIND_OF_BCH_ALGORITHM = KIND_OF_BCH_DECODING_mSBS;
	
	return p;
}
//////////////////////////////////////////////////////////////////////////////////
char close_mSBS_algorithmComponent(struct_HD_mSBS_t3_algorithmComponent **p)
{
	#ifndef RELEASE
		if(!p)
		{
			if(!(global_flag_cmdSystemUnhold&FLAG_MASK_SYSTEM_UNHOLD_FORCED))
			{
				errorMes;
				printf("in close_mSBS_algorithmComponent, close_mSBS_algorithmComponent &p = 0x%x\n", p);
			}
			return -1;
		}
		if(!*p)
		{
			if(!(global_flag_cmdSystemUnhold&FLAG_MASK_SYSTEM_UNHOLD_FORCED))
			{
				warningMes;
				printf("in close_mSBS_algorithmComponent, close_mSBS_algorithmComponent *p = 0x%x\n", *p);
			}
			return -1;
	}
	#endif

	#ifndef RELEASE
		if(closePowerFormPolynomial(&((*p)->codeWord)))
		{
			warningMes; printf("in close_mSBS_algorithmComponent, closePowerFormPolynomial(&((*p)->codeWord)) is fail.\n");
		}
	#else
		closePowerFormPolynomial(&((*p)->codeWord));
	#endif
		
	#ifndef RELEASE
		if(closePowerFormPolynomial(&((*p)->errorLocationVector)))
		{
			warningMes; printf("in close_mSBS_algorithmComponent, closePowerFormPolynomial(&((*p)->codeWord)) is fail.\n");
		}
	#else
		closePowerFormPolynomial(&((*p)->errorLocationVector));
	#endif

	#ifndef RELEASE
		if(closeGaloisFieldExceptElements(&((*p)->syndrome)))
		{
			warningMes;
			printf("in close_mSBS_algorithmComponent, closeGaloisFieldExceptElements(&((*p)->syndrome)) is fail.\n");
		}
	#else
		closeGaloisFieldExceptElements(&((*p)->syndrome));
	#endif

	
	#ifndef RELEASE
	if(closeGaloisFieldExceptElements(&((*p)->errLocPoly)))
	{
		warningMes;
		printf("in close_mSBS_algorithmComponent, closeGaloisFieldExceptElements(&((*p)->errLocPoly)) is fail.\n");
	}
	#else
		closeGaloisFieldExceptElements(&((*p)->errLocPoly));
	#endif

	free(*p);
	*p=NULL;
			#ifndef RELEASE
				if(global_flag_cmdOption&FLAG_MASK_PRINTF_MEMORY_CHECK_PROCESS)
				{
					memoryMes;
					printf("free(*p), p = \'0x%x\', *p = \'0x%x\'\n", p, *p);
				}
			#endif

			#ifndef RELEASE
				if((global_flag_cmdOption&FLAG_MASK_PRINTF_LOG))
				{
					infoMes;
					printf("in close_mSBS_algorithmComponent, close_mSBS_algorithmComponent is done.\n");
				}
			#endif
	return 0;
}
//////////////////////////////////////////////////////////////////////////////////
struct_HD_mSBS_t3_algorithmComponent *recreate_mSBS_algorithmComponent(struct_HD_mSBS_t3_algorithmComponent **p, struct_galoisFieldElements *galoisFields, unsigned int t, char *string, unsigned int stringLength)
{
	#ifndef RELEASE
	if(!p)
	{
		errorMes;
		printf("in recreate_mSBS_algorithmComponent, struct_HD_mSBS_t3_algorithmComponent **p is NULL.\n");
		return NULL;
	}
	#endif
	
	if(*p)
	{
			#ifndef RELEASE
			if((global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)==FLAG_MASK_PRINTF_LOG)
			{
				infoMes;
				printf("in recreate_mSBS_algorithmComponent, closeBmAlgorithmComponent(p); will be excute.\n");
			}
			if(!(global_flag_cmdSystemUnhold&FLAG_MASK_SYSTEM_UNHOLD_FORCED))
			{
				warningMes; printf("in recreate_mSBS_algorithmComponent, struct_HD_mSBS_t3_algorithmComponent *p have data already.\n");
				warningMesShort; printf("it will be closed.\n");
			}
			#endif
			
		close_mSBS_algorithmComponent(p);
	}

			#ifndef RELEASE
			if((global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)==FLAG_MASK_PRINTF_LOG)
			{
				infoMes;
				printf("in recreate_mSBS_algorithmComponent, *p=create_mSBS_algorithmComponent(galoisFields,t, string); will be excute.\n");
				printf("it will be closed.\n");
			}
			#endif
	
	if(!stringLength) stringLength=strlen(string);
		
	return create_mSBS_algorithmComponent(galoisFields,t, string, stringLength);;
}
//////////////////////////////////////////////////////////////////////////////////
char calculate_mSBS_algorithm(struct_galoisFieldElements *galoisFields, struct_HD_mSBS_t3_algorithmComponent *p)
{
	// unsigned int bebuggingTemp;
	unsigned int tmp_i;

	struct_galoisFieldPolyForm *tmp_S_1_pow_2;
	struct_galoisFieldPolyForm *tmp_S_1_pow_3;
	struct_galoisFieldPolyForm *tmp_S_1_pow_4;
	struct_galoisFieldPolyForm *tmp_S_1_pow_6;
	
	struct_galoisFieldPolyForm *tmp_S_3_pow_2;
	
	struct_galoisFieldPolyForm *tmp_ref_coefficient;//it is same with S_1^3 + S_3 is tmp_C_t3.
	
	struct_galoisFieldPolyForm *tmp_C_t3;
	struct_galoisFieldPolyForm *tmp_B_t3;
	struct_galoisFieldPolyForm *tmp_A_t3;
	struct_galoisFieldPolyForm *tmp_R_t3;
	

			#ifndef RELEASE
			if(global_flag_debug_bmSim&FLAG_MASK_DEBUG_BCH_BM_ALGORITHM)
			{
				debugBmAlgorithmMes;
				printf("calculate_mSBS_algorithm\n");
			}
			#endif

	#ifndef RELEASE
	if(!galoisFields)
	{
		errorMes;
		printf("in calculate_mSBS_algorithm, struct_galoisFieldElements *galoisFields is NULL.\n");
		return -1;
	}
	if(!p)
	{
		errorMes;
		printf("in calculate_mSBS_algorithm, struct_HD_BM_algorithmComponent *p is NULL.\n");
		return -1;
	}
	#endif

	/*
		if correctables is set up to larger than 3, it is no make any sense.
		And then, to block changing any received, So make error location polynomial 0 = 1 + 0 * a^1 + 0 * a^2 + 0 * a^3 ...
		So the err loc poly can not change any bits.
	*/
	// if(p->numberOfCorrection == 3)
	// {
		// #ifndef RELEASE
		// warningMesShort; printf("in calculate_mSBS_algorithm, if(!(p->numberOfCorrection < p->syndrome->length)).\n");
		// #endif
		
		// *(p->errLocPoly->element+3) = (*(galoisFields->element+0));
		// *(p->errLocPoly->element+2) = (*(galoisFields->element+0));
		// *(p->errLocPoly->element+1) = (*(galoisFields->element+0));
		// *(p->errLocPoly->element+0) = (*(galoisFields->element+1));

		// return -2;
	// }
	
			#ifndef RELEASE
			if(((global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)==FLAG_MASK_PRINTF_LOG)||(global_flag_debug_bmSim&FLAG_MASK_DEBUG_BCH_BM_CAL_DELTA))
			{
				logMes;
				printf("Error correction formula(mSBS t=3) calculation is started.\n");
				printf("initial condition is...\n");
				printf(">> errLocPoly");
				printGaloisField2(galoisFields, p->errLocPoly, PRINT_FIELD_EQUATION_TITLE, PRINT_FIELD_PRINT_OPTION_NUMBERING);
			}
			#endif
	
	//struct_galoisFieldPolyForm *sumElementInGF_usingSumMatrixReturnAddr(struct_galoisFieldElements *field, struct_galoisFieldPolyForm *operandA, struct_galoisFieldPolyForm *operandB)
	//struct_galoisFieldPolyForm *multiElementsInGF_returnAddr(struct_galoisFieldElements *field, struct_galoisFieldPolyForm *operandA, struct_galoisFieldPolyForm *operandB)
	/*
		case t >= 3
		C_t3 = S_1^3 + S_3
		B_t3 = S_1^4 + ( S_1 * S_3 )
		A_t3 = S_5 + ( S_1^2 * S_3 )
		R_t3 = S_1^6 + S_3^2 + ( S_1^3 * S_3 ) + ( S_1 * S_5 )
		
		case t <= 2
		C_t2 = 0
		B_t2 = S_1
		A_t2 = S_1^2
		R_t2 = S_1^3 + S_3
		
		S_1 is *(syndrome->element+0)
		S_3 is *(syndrome->element+2)
		S_5 is *(syndrome->element+4)
	*/
			#ifndef RELEASE
			if(((global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)==FLAG_MASK_PRINTF_LOG)||(global_flag_debug_bmSim&FLAG_MASK_DEBUG_BCH_BM_CAL_DELTA))
			{
				logMes;	printf("[calculate syndrome factors start]\r\n");
			}
			#endif
	tmp_S_1_pow_2 = multiElementsInGF_returnAddr(galoisFields, *(p->syndrome->element+0), *(p->syndrome->element+0));
			#ifndef RELEASE
			if(((global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)==FLAG_MASK_PRINTF_LOG)||(global_flag_debug_bmSim&FLAG_MASK_DEBUG_BCH_BM_CAL_DELTA))
			{
				logMes;	printf("[tmp_S_1_pow_2 is done]\r\n");
			}
			#endif	
	
	tmp_S_1_pow_3 = multiElementsInGF_returnAddr(galoisFields, tmp_S_1_pow_2, *(p->syndrome->element+0));
			#ifndef RELEASE
			if(((global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)==FLAG_MASK_PRINTF_LOG)||(global_flag_debug_bmSim&FLAG_MASK_DEBUG_BCH_BM_CAL_DELTA))
			{
				logMes;	printf("[tmp_S_1_pow_3 is done]\r\n");
			}
			#endif	
	
	tmp_S_1_pow_4 = multiElementsInGF_returnAddr(galoisFields, tmp_S_1_pow_2, tmp_S_1_pow_2);
			#ifndef RELEASE
			if(((global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)==FLAG_MASK_PRINTF_LOG)||(global_flag_debug_bmSim&FLAG_MASK_DEBUG_BCH_BM_CAL_DELTA))
			{
				logMes;	printf("[tmp_S_1_pow_4 is done]\r\n");
			}
			#endif	
	
	tmp_S_1_pow_6 = multiElementsInGF_returnAddr(galoisFields, tmp_S_1_pow_3, tmp_S_1_pow_3);
			#ifndef RELEASE
			if(((global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)==FLAG_MASK_PRINTF_LOG)||(global_flag_debug_bmSim&FLAG_MASK_DEBUG_BCH_BM_CAL_DELTA))
			{
				logMes;	printf("[tmp_S_1_pow_6 is done]\r\n");
			}
			#endif	
	
	if(p->numberOfCorrection > 1)
	{
		tmp_S_3_pow_2 = multiElementsInGF_returnAddr(galoisFields, *(p->syndrome->element+2), *(p->syndrome->element+2));
				#ifndef RELEASE
				if(((global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)==FLAG_MASK_PRINTF_LOG)||(global_flag_debug_bmSim&FLAG_MASK_DEBUG_BCH_BM_CAL_DELTA))
				{
					logMes;	printf("[tmp_S_3_pow_2 is done]\r\n");
				}
				#endif
	}


	if(p->numberOfCorrection > 1)
	{
		tmp_C_t3 = sumElementInGF_usingSumMatrixReturnAddr(galoisFields, tmp_S_1_pow_3, *(p->syndrome->element+2));
				#ifndef RELEASE
				if(((global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)==FLAG_MASK_PRINTF_LOG)||(global_flag_debug_bmSim&FLAG_MASK_DEBUG_BCH_BM_CAL_DELTA))
				{
					logMes;	printf("[tmp_C_t3 is done]\r\n");
				}
				#endif	

		tmp_B_t3 = sumElementInGF_usingSumMatrixReturnAddr(galoisFields, tmp_S_1_pow_4, multiElementsInGF_returnAddr(galoisFields, *(p->syndrome->element+0), *(p->syndrome->element+2)));
				#ifndef RELEASE
				if(((global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)==FLAG_MASK_PRINTF_LOG)||(global_flag_debug_bmSim&FLAG_MASK_DEBUG_BCH_BM_CAL_DELTA))
				{
					logMes;	printf("[tmp_B_t3 is done]\r\n");
				}
				#endif
	}
		

	if(p->numberOfCorrection > 2)
	{
		tmp_A_t3 = sumElementInGF_usingSumMatrixReturnAddr(galoisFields, *(p->syndrome->element+4), multiElementsInGF_returnAddr(galoisFields, tmp_S_1_pow_2, *(p->syndrome->element+2)));
				#ifndef RELEASE
				if(((global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)==FLAG_MASK_PRINTF_LOG)||(global_flag_debug_bmSim&FLAG_MASK_DEBUG_BCH_BM_CAL_DELTA))
				{
					logMes;	printf("[tmp_A_t3 is done]\r\n");
				}
				#endif
/*		
		tmp_R_t3 = sumElementInGF_usingSumMatrixReturnAddr(galoisFields, 
					sumElementInGF_usingSumMatrixReturnAddr(galoisFields, tmp_S_1_pow_6, multiElementsInGF_returnAddr(galoisFields, tmp_S_1_pow_3, *(p->syndrome->element+2))),
					sumElementInGF_usingSumMatrixReturnAddr(galoisFields, tmp_S_3_pow_2, multiElementsInGF_returnAddr(galoisFields, *(p->syndrome->element+0), *(p->syndrome->element+4)))
					);
*/
		tmp_R_t3 = sumElementInGF_usingSumMatrixReturnAddr(galoisFields, 
					multiElementsInGF_returnAddr(galoisFields, tmp_C_t3, tmp_C_t3),
                    multiElementsInGF_returnAddr(galoisFields, *(p->syndrome->element+0), tmp_A_t3)
					);

				#ifndef RELEASE
				if(((global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)==FLAG_MASK_PRINTF_LOG)||(global_flag_debug_bmSim&FLAG_MASK_DEBUG_BCH_BM_CAL_DELTA))
				{
					logMes;	printf("[tmp_R_t3 is done]\r\n");
				}
				#endif	
	}

	
	
	
	
	
	if(p->numberOfCorrection > 2)
	{
		tmp_ref_coefficient = tmp_C_t3;
				#ifndef RELEASE
				if(((global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)==FLAG_MASK_PRINTF_LOG)||(global_flag_debug_bmSim&FLAG_MASK_DEBUG_BCH_BM_CAL_DELTA))
				{
					logMes;	printf("[calculate syndrome factors end]\r\n");
				}
				#endif
				
				#ifndef RELEASE
				if(((global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)==FLAG_MASK_PRINTF_LOG)||(global_flag_debug_bmSim&FLAG_MASK_DEBUG_BCH_BM_CAL_DELTA))
				{
					logMes;	printf("[saving errLocPoly start]\r\n");
				}
				#endif			
		if( tmp_ref_coefficient != (*(galoisFields->element+0)) )//check refference coefficient is zero?
		//NO -> degree of errLoc poly is 3.
		{
					#ifndef RELEASE
					if(((global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)==FLAG_MASK_PRINTF_LOG)||(global_flag_debug_bmSim&FLAG_MASK_DEBUG_BCH_BM_CAL_DELTA))
					{
						logMes;	printf("degree of errLoc poly is 3\r\n");
					}
					#endif	
			*(p->errLocPoly->element+3) = tmp_C_t3;
			*(p->errLocPoly->element+2) = tmp_B_t3;
			*(p->errLocPoly->element+1) = tmp_A_t3;
			*(p->errLocPoly->element+0) = tmp_R_t3;
			
			// p->degreeOfErrLocPoly=3;

		}
		else
		//YES -> degree of errLoc poly is 2.
		{
					#ifndef RELEASE
					if(((global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)==FLAG_MASK_PRINTF_LOG)||(global_flag_debug_bmSim&FLAG_MASK_DEBUG_BCH_BM_CAL_DELTA))
					{
						logMes;	printf("degree of errLoc poly is 2\r\n");
					}
					#endif	
			*(p->errLocPoly->element+3) = (*(galoisFields->element+0));
			*(p->errLocPoly->element+2) = *(p->syndrome->element+0);
			*(p->errLocPoly->element+1) = tmp_S_1_pow_2;
			*(p->errLocPoly->element+0) = tmp_C_t3;
			
			// p->degreeOfErrLocPoly=2;
			
		}

				#ifndef RELEASE
				if(((global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)==FLAG_MASK_PRINTF_LOG)||(global_flag_debug_bmSim&FLAG_MASK_DEBUG_BCH_BM_CAL_DELTA))
				{
					logMes;	printf("[saving errLocPoly end]\r\n");
				}
				#endif
		p->coefficient_of_alpha_three = *(p->errLocPoly->element+3);
		p->coefficient_of_alpha_two = *(p->errLocPoly->element+2);
		p->coefficient_of_alpha_one = *(p->errLocPoly->element+1);
		p->coefficient_of_alpha_zero = *(p->errLocPoly->element+0);
		
	}
	else if(p->numberOfCorrection > 1)
	{
		{
					#ifndef RELEASE
					if(((global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)==FLAG_MASK_PRINTF_LOG)||(global_flag_debug_bmSim&FLAG_MASK_DEBUG_BCH_BM_CAL_DELTA))
					{
						logMes;	printf("degree of errLoc poly is 3\r\n");
					}
					#endif	
			*(p->errLocPoly->element+2) = *(p->syndrome->element+0);
			*(p->errLocPoly->element+1) = tmp_S_1_pow_2;
			*(p->errLocPoly->element+0) = tmp_C_t3;
			
			// if( tmp_C_t3 != (*(galoisFields->element+0)) )
			// {
				// p->degreeOfErrLocPoly=2;
			// }
			// else
			// {
				// p->degreeOfErrLocPoly=1;
			// }
			
		}
	}
	
	
	p->degreeOfErrLocPoly = checkDegreePolynomials(galoisFields, p->errLocPoly);

	return 0;
}
//////////////////////////////////////////////////////////////////////////////////
////////////////////////* mSBS algorithm components end */////////////////////////
//////////////////////////////////////////////////////////////////////////////////
struct_generalForm_algorithmComponent* recreateAlgoriehmComponent(struct_generalForm_algorithmComponent **p, struct_galoisFieldElements *galoisFields, unsigned int t, char *string, unsigned int stringLength, enum KIND_OF_BCH_DECODING_ALGORITHM KIND_OF_BCH_ALGORITHM)
{
							#ifndef RELEASE
							if(global_flag_debug_display&FLAG_MASK_DISPLAY_FUNCNAME)
							{
								debugFuncNameMes;
								printf("[in recreateAlgoriehmComponent]\r\n");
							}
							#endif
	switch(KIND_OF_BCH_ALGORITHM)
	{
		case KIND_OF_BCH_DECODING_BM:
							#ifndef RELEASE
							if(global_flag_debug_display&FLAG_MASK_DISPLAY_FUNCNAME)
							{
								debugFuncNameMes; printf("[%s] in recreateAlgoriehmComponent, enter\r\n", KIND_OF_BCH_DECODING_ALGORITHM_NAME[KIND_OF_BCH_ALGORITHM]);
							}
							#endif
			//struct_HD_BM_algorithmComponent *recreateBmAlgorithmComponent(struct_HD_BM_algorithmComponent **p, struct_galoisFieldElements *galoisFields, unsigned int t, char *string, unsigned int stringLength)
			// closeBmAlgorithmComponent(((struct_HD_BM_algorithmComponent**)p));
			return (struct_generalForm_algorithmComponent*)recreateBmAlgorithmComponent(((struct_HD_BM_algorithmComponent**)p), galoisFields,  t,  string,  stringLength);
		break;
		case KIND_OF_BCH_DECODING_iBM:
			printf("in recreateAlgoriehmComponent, %s is not support yet.\r\n", KIND_OF_BCH_DECODING_ALGORITHM_NAME[KIND_OF_BCH_ALGORITHM]);
		break;
		case KIND_OF_BCH_DECODING_RiBM:
			printf("in recreateAlgoriehmComponent, %s is not support yet.\r\n", KIND_OF_BCH_DECODING_ALGORITHM_NAME[KIND_OF_BCH_ALGORITHM]);
		break;
		case KIND_OF_BCH_DECODING_SiBM:
			printf("in recreateAlgoriehmComponent, %s is not support yet.\r\n", KIND_OF_BCH_DECODING_ALGORITHM_NAME[KIND_OF_BCH_ALGORITHM]);
		break;
		case KIND_OF_BCH_DECODING_mSBS:
							#ifndef RELEASE
							if(global_flag_debug_display&FLAG_MASK_DISPLAY_FUNCNAME)
							{
								debugFuncNameMes; printf("[%s] in recreateAlgoriehmComponent, enter.\r\n", KIND_OF_BCH_DECODING_ALGORITHM_NAME[KIND_OF_BCH_ALGORITHM]);
							}
							#endif
			//struct_HD_mSBS_t3_algorithmComponent *recreate_mSBS_algorithmComponent(struct_HD_mSBS_t3_algorithmComponent **p, struct_galoisFieldElements *galoisFields, unsigned int t, char *string, unsigned int stringLength)
			// close_mSBS_algorithmComponent(((struct_HD_mSBS_t3_algorithmComponent**)p));
			return (struct_generalForm_algorithmComponent*)recreate_mSBS_algorithmComponent(((struct_HD_mSBS_t3_algorithmComponent**)p), galoisFields,  t,  string,  stringLength);
		break;
		case KIND_OF_BCH_DECODING_PETERSON:
			printf("in recreateAlgoriehmComponent, %s is not support yet.\r\n", KIND_OF_BCH_DECODING_ALGORITHM_NAME[KIND_OF_BCH_ALGORITHM]);
		break;
		case KIND_OF_BCH_DECODING_EUCLIDIAN:
			printf("in recreateAlgoriehmComponent, %s is not support yet.\r\n", KIND_OF_BCH_DECODING_ALGORITHM_NAME[KIND_OF_BCH_ALGORITHM]);
		default:
		break;
	}
	return NULL;
}
//////////////////////////////////////////////////////////////////////////////////
//char calculateBmAlgorithm(struct_galoisFieldElements *galoisFields, struct_HD_BM_algorithmComponent *p)
//char calculate_mSBS_algorithm(struct_galoisFieldElements *galoisFields, struct_HD_mSBS_t3_algorithmComponent *p)
char calculateBCH_decodingAlgorithm(struct_galoisFieldElements *galoisFields, struct_generalForm_algorithmComponent *p)
{
	switch(p->KIND_OF_BCH_ALGORITHM)
	{
		case KIND_OF_BCH_DECODING_BM:
			return calculateBmAlgorithm(galoisFields, (struct_HD_BM_algorithmComponent*)p);
		break;
		case KIND_OF_BCH_DECODING_iBM:
			printf("in calculateBCH_decodingAlgorithm, %s is not support yet.\r\n", KIND_OF_BCH_DECODING_ALGORITHM_NAME[(p)->KIND_OF_BCH_ALGORITHM]);
		break;
		case KIND_OF_BCH_DECODING_RiBM:
			printf("in calculateBCH_decodingAlgorithm, %s is not support yet.\r\n", KIND_OF_BCH_DECODING_ALGORITHM_NAME[(p)->KIND_OF_BCH_ALGORITHM]);
		break;
		case KIND_OF_BCH_DECODING_SiBM:
			printf("in calculateBCH_decodingAlgorithm, %s is not support yet.\r\n", KIND_OF_BCH_DECODING_ALGORITHM_NAME[(p)->KIND_OF_BCH_ALGORITHM]);
		break;
		case KIND_OF_BCH_DECODING_mSBS:
			return calculate_mSBS_algorithm(galoisFields, (struct_HD_mSBS_t3_algorithmComponent*)p);
		break;
		case KIND_OF_BCH_DECODING_PETERSON:
			printf("in calculateBCH_decodingAlgorithm, %s is not support yet.\r\n", KIND_OF_BCH_DECODING_ALGORITHM_NAME[(p)->KIND_OF_BCH_ALGORITHM]);
		break;
		case KIND_OF_BCH_DECODING_EUCLIDIAN:
			printf("in calculateBCH_decodingAlgorithm, %s is not support yet.\r\n", KIND_OF_BCH_DECODING_ALGORITHM_NAME[(p)->KIND_OF_BCH_ALGORITHM]);
		default:
		break;
	}
	return -15;
}
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
// closeBmAlgorithmComponent(((struct_HD_BM_algorithmComponent**)(main_indi_HD_decordComponents+KIND_OF_BCH_DECODING_BM)));
char closeAlgoriehmComponent(struct_generalForm_algorithmComponent **p, enum KIND_OF_BCH_DECODING_ALGORITHM KIND_OF_BCH_ALGORITHM)
{
	switch(KIND_OF_BCH_ALGORITHM)
	{
		case KIND_OF_BCH_DECODING_BM:
			//char closeBmAlgorithmComponent(struct_HD_BM_algorithmComponent **p)
			return closeBmAlgorithmComponent((struct_HD_BM_algorithmComponent**)p);
		break;
		case KIND_OF_BCH_DECODING_iBM:
			printf("in closeAlgoriehmComponent, %s is not support yet.\r\n", KIND_OF_BCH_DECODING_ALGORITHM_NAME[KIND_OF_BCH_ALGORITHM]);
		break;
		case KIND_OF_BCH_DECODING_RiBM:
			printf("in closeAlgoriehmComponent, %s is not support yet.\r\n", KIND_OF_BCH_DECODING_ALGORITHM_NAME[KIND_OF_BCH_ALGORITHM]);
		break;
		case KIND_OF_BCH_DECODING_SiBM:
			printf("in closeAlgoriehmComponent, %s is not support yet.\r\n", KIND_OF_BCH_DECODING_ALGORITHM_NAME[KIND_OF_BCH_ALGORITHM]);
		break;
		case KIND_OF_BCH_DECODING_mSBS:
			// char close_mSBS_algorithmComponent(struct_HD_mSBS_t3_algorithmComponent **p)
			return close_mSBS_algorithmComponent((struct_HD_mSBS_t3_algorithmComponent**)p);
		break;
		case KIND_OF_BCH_DECODING_PETERSON:
			printf("in closeAlgoriehmComponent, %s is not support yet.\r\n", KIND_OF_BCH_DECODING_ALGORITHM_NAME[KIND_OF_BCH_ALGORITHM]);
		break;
		case KIND_OF_BCH_DECODING_EUCLIDIAN:
			printf("in closeAlgoriehmComponent, %s is not support yet.\r\n", KIND_OF_BCH_DECODING_ALGORITHM_NAME[KIND_OF_BCH_ALGORITHM]);
		default:
		break;
	}
	return -15;
}
//////////////////////////////////////////////////////////////////////////////////
////////////////////////* mSBS algorithm components end */////////////////////////
//////////////////////////////////////////////////////////////////////////////////
struct struct_encodingComponentInGF
{
	struct_powerFormPolynomials *codeWord;
	// struct_powerFormPolynomials *encodedCodeWord;
	// struct_powerFormPolynomials *receivedCodeWord;
	// struct_powerFormPolynomials *errorCodeWord;
	//struct_powerFormPolynomials *erroredCodeWord;
	
	unsigned int minimumDistance;
	unsigned int parityBitsLength;//shifted....
	
	char_POWER_FORM *generationPolynomial;
}typedef struct_encodingComponentInGF;
//////////////////////////////////////////////////////////////////////////////////
struct_encodingComponentInGF *createEncodingComponent(unsigned int length, unsigned int numberOfCorrectableBit, char_POWER_FORM *generationPolynomial)
{
	struct_encodingComponentInGF *p;

	#ifndef RELEASE
	if(!length)
	{
		errorMes;
		printf("in createEncodingComponent, unsigned int length is zero.\n");
		return NULL;
	}
	if(!numberOfCorrectableBit)
	{
		errorMes;
		printf("in createEncodingComponent, unsigned int numberOfCorrectableBit is zero.\n");
		return NULL;
	}
	if(!generationPolynomial)
	{
		errorMes;
		printf("in createEncodingComponent, char_POWER_FORM *generationPolynomial is NULL.\n");
		return NULL;
	}
	#endif

	p=(struct_encodingComponentInGF*)malloc(sizeof(struct_encodingComponentInGF));
	memset(p, 0, sizeof(struct_encodingComponentInGF));
	
	p->minimumDistance=2*numberOfCorrectableBit+1;
	
	p->parityBitsLength=strlen(generationPolynomial)-1;
	
	p->codeWord=createPowerFormPolynomial(length);
	
	p->generationPolynomial=generationPolynomial;
	return p;
}
//////////////////////////////////////////////////////////////////////////////////
char closeEncodingComponent(struct_encodingComponentInGF **p)
{
	#ifndef RELEASE
	if(!p)
	{
		errorMes;
		printf("in closeEncodingComponent, struct_encodingComponentInGF **p is NULL.\n");
		return -1;
	}
	if(!*p)
	{
		errorMes;
		printf("in closeEncodingComponent, struct_encodingComponentInGF *p is NULL.\n");
		return -1;
	}
	#endif
	
	closePowerFormPolynomial(&((*p)->codeWord));
	
	free(*p);
	*p=NULL;
}
//////////////////////////////////////////////////////////////////////////////////
struct_encodingComponentInGF *recreateEncodingComponent(struct_encodingComponentInGF **p, unsigned int length, unsigned int numberOfCorrectableBit, char_POWER_FORM *generationPolynomial)
{
	if(*p)
	{
				#ifndef RELEASE
				if(global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)
				{
					infoMes;
					printf("in recreateEncodingComponent, struct_encodingComponentInGF *p have data already.\n");
					printf("it will be deleted.\n");
				}
				#endif
		closeEncodingComponent(p);
	}
	*p=createEncodingComponent(length, numberOfCorrectableBit, generationPolynomial);
	return *p;
}
//////////////////////////////////////////////////////////////////////////////////
struct_encodingComponentInGF *createEncodingComponentAndInputValue(unsigned int length, unsigned int numberOfCorrectableBit, char_POWER_FORM *generationPolynomial, char *value)
{
	struct_encodingComponentInGF *p;

	p=createEncodingComponent(length, numberOfCorrectableBit, generationPolynomial);
	#ifndef RELEASE
	if(!p)
	{
		errorMes;
		closeEncodingComponent(&p);
		printf("in createEncodingComponentAndInputValue, (p=createEncodingComponent(length, numberOfCorrectableBit, generationPolynomial)) return NULL.\n");
		return NULL;
	}
	#endif
	if(copyPowerFormPolynomialFromString(p->codeWord, value))
	{
				#ifndef RELEASE
				errorMes; printf("in createEncodingComponentAndInputValue, copyPowerFormPolynomialFromString(p->codeWord, value) return not 0.\n");
				#endif

		closeEncodingComponent(&p);
		return NULL;
	}
	return p;
}
//////////////////////////////////////////////////////////////////////////////////
struct_encodingComponentInGF *recreateEncodingComponentAndInputValue(struct_encodingComponentInGF **p, unsigned int length, unsigned int numberOfCorrectableBit, char_POWER_FORM *generationPolynomial, char *value)
{
	if(*p)
	{
				#ifndef RELEASE
				if(global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)
				{
					infoMes;
					printf("in recreateEncodingComponentAndInputValue, struct_encodingComponentInGF *p have data already.\n");
					printf("it will be deleted.\n");
				}
				#endif
				
		closeEncodingComponent(p);
	}
	*p=createEncodingComponentAndInputValue(length, numberOfCorrectableBit, generationPolynomial, value);
	return *p;
}
//////////////////////////////////////////////////////////////////////////////////
char calculateParityInGaloisFieldAttachLowSide(struct_powerFormPolynomials *encodedCodeWord, unsigned int numberOfParityBits, char_POWER_FORM *generationPolynomial)
{
	unsigned int i;
	unsigned int j;
	unsigned int genPolyLength=strlen(generationPolynomial);

	char_POWER_FORM *remainder;

			#ifndef RELEASE
			if(global_flag_debug_display&FLAG_MASK_DEBUG_DEBUG)
			{
				infoMes; printf("[start] calculateParityInGaloisFieldAttachLowSide\n");
			}
			#endif
	
	#ifndef RELEASE
	if(!encodedCodeWord)
	{
		errorMes;
		printf("in calculateParityInGaloisFieldAttachLowSide, struct_powerFormPolynomials *encodedCodeWord is NULL.\n");
		return -1;
	}
	if(!generationPolynomial)
	{
		errorMes;
		printf("in calculateParityInGaloisFieldAttachLowSide, char_POWER_FORM *generationPolynomial is NULL.\n");
		return -1;
	}
	#endif
		
	// char_POWER_FORM *remainder=(char_POWER_FORM*)malloc(sizeof(char_POWER_FORM)*(encodedCodeWord->length+1));
	// memcpy(remainder, encodedCodeWord->equation, sizeof(char_POWER_FORM)*(encodedCodeWord->length+1));
	remainder=(char_POWER_FORM*)malloc(sizeof(char_POWER_FORM)*(encodedCodeWord->usedLength+1));
	#ifndef USING_OPEN_MP
	memcpy(remainder, encodedCodeWord->equation, sizeof(char_POWER_FORM)*(encodedCodeWord->usedLength));
	#else
	#ifdef USING_OPEN_MP
	#pragma omp parallel for schedule(guided) private(i) shared(encodedCodeWord, remainder)
	#endif
	for(i=0; i<encodedCodeWord->usedLength; i++)
	{
		*(remainder+i)=*(encodedCodeWord->equation+i);
	}
	#endif
	*(remainder+encodedCodeWord->usedLength)=0;
	

			#ifndef RELEASE
			if(global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)
			{
				logMes; printf("in calculateParityInGaloisFieldAttachLowSide, memcpy result is below...\n\"%s\"\n", remainder);
			}
			#endif
	

	for(i=0; i<encodedCodeWord->usedLength; i++)
	{
		if((encodedCodeWord->usedLength-genPolyLength)<i)
		{
			break;
		}
		if(*(remainder+encodedCodeWord->usedLength-1-i)=='1')
		{
			#ifdef USING_OPEN_MP
			#pragma omp parallel for schedule(guided) private(j) shared(i, genPolyLength, remainder, encodedCodeWord, generationPolynomial)
			#endif
			for(j=0; j<genPolyLength; j++)
			{
				if(*(remainder+encodedCodeWord->usedLength-1-i-j)!=*(generationPolynomial+genPolyLength-1-j))
				{
					*(remainder+encodedCodeWord->usedLength-1-i-j)='1';
				}
				else
				{
					*(remainder+encodedCodeWord->usedLength-1-i-j)='0';
				}
			}
		}
	}
	
			#ifndef RELEASE
			if((global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)==FLAG_MASK_PRINTF_LOG)
			{
				logMes; printf("encoded(not op done : \n\"%s\"\n", encodedCodeWord->equation);
				logMes; printf("remainder : \n\"%s\"\n", remainder);
			}
			#endif
	#ifndef USING_OPEN_MP
	memcpy(encodedCodeWord->equation, remainder, sizeof(char_POWER_FORM)*numberOfParityBits);
	#else
	#pragma omp parallel for schedule(guided) private(i) shared(numberOfParityBits, encodedCodeWord, remainder)
	for(i=0; i<numberOfParityBits; i++)
	{
		*(encodedCodeWord->equation+i)=*(remainder+i);
	}
	#endif
	
	free(remainder);

			#ifndef RELEASE
			if(global_flag_debug_display&FLAG_MASK_DEBUG_DEBUG)
			{
				infoMes; printf("[end] calculateParityInGaloisFieldAttachLowSide\n");
			}
			#endif
	return 0;
}
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
char calculateParityInGaloisFieldAttachHighSide(struct_powerFormPolynomials *encodedCodeWord, unsigned int numberOfParityBits, char_POWER_FORM *generationPolynomial)
{
	unsigned int i;
	unsigned int j;
	unsigned int genPolyLength=strlen(generationPolynomial);
	
	char_POWER_FORM *remainder;

	#ifndef RELEASE
	if(!encodedCodeWord)
	{
		errorMes;
		printf("in calculateParityInGaloisFieldAttachHighSide, struct_powerFormPolynomials *encodedCodeWord is NULL.\n");
		return -1;
	}
	if(!generationPolynomial)
	{
		errorMes;
		printf("in calculateParityInGaloisFieldAttachHighSide, char_POWER_FORM *generationPolynomial is NULL.\n");
		return -1;
	}
	#endif
	
	remainder=(char_POWER_FORM*)malloc(sizeof(char_POWER_FORM)*(encodedCodeWord->usedLength+1));
	memcpy(remainder, encodedCodeWord->equation, sizeof(char_POWER_FORM)*(encodedCodeWord->usedLength+1));

			#ifndef RELEASE
			if((global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)==FLAG_MASK_PRINTF_LOG)
			{
				logMes;
				printf("in calculateParityInGaloisFieldAttachHighSide, memcpy result : \"%s\"\n", remainder);
			}
			#endif
	
	for(i=0; i<encodedCodeWord->usedLength; i++)
	{
		if((encodedCodeWord->usedLength-genPolyLength)<i)
		{
			break;
		}
		if(*(remainder+encodedCodeWord->usedLength-1-i)=='1')
		{
			for(j=0; j<genPolyLength; j++)
			{
				if(*(remainder+encodedCodeWord->usedLength-1-i-j)!=*(generationPolynomial+genPolyLength-1-j))
				{
					*(remainder+encodedCodeWord->usedLength-1-i-j)='1';
				}
				else
				{
					*(remainder+encodedCodeWord->usedLength-1-i-j)='0';
				}
			}
		}
	}
		
	memcpy(encodedCodeWord->equation+encodedCodeWord->usedLength-numberOfParityBits-1, remainder, sizeof(char_POWER_FORM)*numberOfParityBits);
	
	free(remainder);
	return 0;
}
//////////////////////////////////////////////////////////////////////////////////
// void shiftHighSidePowerFormPolynomial(struct_powerFormPolynomials *p, unsigned int length)
// char calculateParityInGaloisFieldAttachLowSide(struct_powerFormPolynomials *encodedCodeWord, unsigned int numberOfParityBits, char_POWER_FORM *generationPolynomial)

// char convertInformationBitsToBCH(struct_encodingComponentInGF *p)
//////////////////////////////////////////////////////////////////////////////////
struct struct_errorComponent
{
	struct_powerFormPolynomials *erroredCodeWord;
	struct_powerFormPolynomials *errorLocationVector;
}typedef struct_errorComponent;
//////////////////////////////////////////////////////////////////////////////////
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
//////////////////////////////////////////////////////////////////////////////////
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
//////////////////////////////////////////////////////////////////////////////////
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
//////////////////////////////////////////////////////////////////////////////////
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
//////////////////////////////////////////////////////////////////////////////////
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
	/*initial all zero condition*/
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
//////////////////////////////////////////////////////////////////////////////////
void summationError(struct_powerFormPolynomials *codeWord, struct_powerFormPolynomials *error)
{
	unsigned int i;
	
	
	#ifndef RELEASE
	if(!codeWord)
	{
		errorMes;
		printf("in summationError, struct_powerFormPolynomials *codeWord is NULL.\n");
	}
	if(!error)
	{
		errorMes;
		printf("in summationError, struct_powerFormPolynomials *error is NULL.\n");
	}

	if(codeWord->usedLength!=error->usedLength)
	{
		errorMes;
		printf("in summationError, codeWord->length!=error->length.\n");
	}
	#endif
	
	for(i=0; i<codeWord->usedLength; i++)
	{
		if(*(error->equation+i)=='1')
		{
			if(*(codeWord->equation+i)=='0')
			{
				*(codeWord->equation+i)='1';
			}
			else
			{
				*(codeWord->equation+i)='0';
			}
		}
	}
}
//////////////////////////////////////////////////////////////////////////////////
void addErrorCodeWordToUnErrorCodeWord(struct_powerFormPolynomials *result, struct_powerFormPolynomials *codeWord, struct_powerFormPolynomials *error)
{
	unsigned int i;

	#ifndef RELEASE
	if(!result)
	{
		errorMes;
		printf("in summationError, struct_powerFormPolynomials *result is NULL.\n");
	}
	if(!codeWord)
	{
		errorMes;
		printf("in summationError, struct_powerFormPolynomials *codeWord is NULL.\n");
	}
	if(!error)
	{
		errorMes;
		printf("in summationError, struct_powerFormPolynomials *error is NULL.\n");
	}

	if(result->usedLength!=codeWord->usedLength)
	{
		errorMes;
		printf("in summationError, result->usedLength!=codeWord->usedLength.\n");
	}
	if(codeWord->usedLength!=error->usedLength)
	{
		errorMes;
		printf("in summationError, codeWord->usedLength!=error->usedLength.\n");
	}
	#endif
	
	#ifdef USING_OPEN_MP
	#pragma omp parallel for schedule(guided) private(i) shared(codeWord, error, result)
	#endif
	for(i=0; i<codeWord->usedLength; i++)
	{
		
		if(*(error->equation+i)=='1')
		{
			if(*(codeWord->equation+i)=='0')
			{
				*(result->equation+i)='1';
			}
			else
			{
				*(result->equation+i)='0';
			}
		}
		else
		{
			*(result->equation+i)=*(codeWord->equation+i);
		}
	}
	
	*(result->equation+i)=0;
}
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
char *randdomInfoBitStreamChanger(unsigned int infoBitsLength, unsigned int parityBitsLength, char *p)
{
	unsigned int holeLength=infoBitsLength+parityBitsLength;
	unsigned int i, j;
	double numberOfSameBits;
	double probability;
		
	if(!p)
	{
		p=(char*)malloc(sizeof(char)*(infoBitsLength+parityBitsLength+1));
		
		warningMes;
		printf("in randdomInfoBitStreamChanger, memory allocation. size is '%d'+1='%d'\n", infoBitsLength+parityBitsLength,infoBitsLength+parityBitsLength+1);
	}
	*(p+infoBitsLength+parityBitsLength)=0;
	
			#ifndef RELEASE
			if((global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)==FLAG_MASK_PRINTF_LOG)
			{
				infoMes;
				printf("in randdomInfoBitStreamChanger, code length is \'%d\', info length is \'%d\', parity length is \'%d\'\n", holeLength, infoBitsLength, parityBitsLength);
			}
			if((global_flag_cmdOption&FLAG_MASK_PRINTF_CALCULATE_PROCESS)==FLAG_MASK_PRINTF_CALCULATE_PROCESS)
			{
				calculateMes;
				printf("in randdomInfoBitStreamChanger, Generated info Bits is...\n");
			}
			#endif
		
	j=0;
	for(i=0; i<holeLength; i++)
	{
		if(i<infoBitsLength)
		{
			if(!j)
			{
				probability=(double)((double)rand() / ((double)RAND_MAX + 1));
				numberOfSameBits=((double)rand() / ((double)RAND_MAX + 1) * 5)+1;
				// probability=(double)((double)rand_r(&tmp_seed) / ((double)RAND_MAX + 1));
				// numberOfSameBits=((double)rand_r(&tmp_seed) / ((double)RAND_MAX + 1) * 5)+1;
				
				j=(unsigned int)numberOfSameBits;
			}
			if(probability>0.5)
			{
				*(p+i) = '1';
			}
			else
			{
				*(p+i) = '0';
			}
			j--;
		}
		else
		{
			*(p+i) = '0';
		}
				#ifndef RELEASE
				if((global_flag_cmdOption&FLAG_MASK_PRINTF_CALCULATE_PROCESS)==FLAG_MASK_PRINTF_CALCULATE_PROCESS)
				{
					printf("%c(%d)",*(p+i), i);
					if((global_flag_cmdOption&FLAG_MASK_PRINTF_CALCULATE_PROCESS)==FLAG_MASK_PRINTF_CALCULATE_PROCESS)
					{
						if((i+1)==infoBitsLength)
						{
							printf("[infoBitsEnd]");
						}
					}
				}
				#endif
	}
	
			#ifndef RELEASE
			if((global_flag_cmdOption&FLAG_MASK_PRINTF_CALCULATE_PROCESS)==FLAG_MASK_PRINTF_CALCULATE_PROCESS)
			{
				printf("\n");
			}
			if((global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)==FLAG_MASK_PRINTF_LOG)
			{
				infoMes;
				printf("randdomInfoBitStreamChanger is end.\n");
			}
			#endif
	return p;
}
//////////////////////////////////////////////////////////////////////////////////
char *randdomInfoBitChanger(unsigned int infoBitsLength, unsigned int parityBitsLength, char *p)
{
	unsigned int holeLength=infoBitsLength+parityBitsLength;
	unsigned int i;
		
	if(!p)
	{
		p=(char*)malloc(sizeof(char)*(infoBitsLength+parityBitsLength+1));
		
		warningMes;
		printf("in randdomInfoBitChanger, memory allocation. size is '%d'+1='%d'\n", infoBitsLength+parityBitsLength,infoBitsLength+parityBitsLength+1);
	}
	*(p+infoBitsLength+parityBitsLength)=0;
	
			#ifndef RELEASE
			if((global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)==FLAG_MASK_PRINTF_LOG)
			{
				infoMes;
				printf("in randdomInfoBitChanger, code length is \'%d\', info length is \'%d\', parity length is \'%d\'\n", holeLength, infoBitsLength, parityBitsLength);
			}
			if((global_flag_cmdOption&FLAG_MASK_PRINTF_CALCULATE_PROCESS)==FLAG_MASK_PRINTF_CALCULATE_PROCESS)
			{
				calculateMes;
				printf("in randdomInfoBitChanger, Generated info Bits is...\n");
			}
			#endif
		
	for(i=0; i<holeLength; i++)
	{
		if(rand()%2)
		{
			*(p+i) = '1';
		}
		else
		{
			*(p+i) = '0';
		}
	}
	
			#ifndef RELEASE
			if((global_flag_cmdOption&FLAG_MASK_PRINTF_CALCULATE_PROCESS)==FLAG_MASK_PRINTF_CALCULATE_PROCESS)
			{
				printf("\n");
			}
			if((global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)==FLAG_MASK_PRINTF_LOG)
			{
				infoMes;
				printf("randdomInfoBitChanger is end.\n");
			}
			#endif
	return p;
}

//////////////////////////////////////////////////////////////////////////////////
char *allSameInfoBitGenerator(char initChar, unsigned int infoBitsLength, unsigned int parityBitsLength, char **p)
{
	if(!(*p))
	{
		(*p)=(char*)malloc(sizeof(char)*(infoBitsLength+parityBitsLength+1));
        memset((*p), initChar, sizeof(char)*(infoBitsLength+parityBitsLength));
        *((*p)+infoBitsLength+parityBitsLength) = 0;
    }
	return (*p);
}
//////////////////////////////////////////////////////////////////////////////////

char *randdomInfoBitGenerator(unsigned int infoBitsLength, unsigned int parityBitsLength, char **p)
{
	if(!(*p))
	{
		(*p)=(char*)malloc(sizeof(char)*(infoBitsLength+parityBitsLength+1));
	}
    
	return randdomInfoBitChanger(infoBitsLength, parityBitsLength, *p);
	//return randdomInfoBitStreamChanger(infoBitsLength, parityBitsLength, *p);
}
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
typedef signed int	s_int_QUANTIZ_DIGIT;
typedef unsigned int u_int_QUANTIZ_MAGNITUDE_DIGIT;
typedef double double_RATIONAL_NUMBER;
struct struct_quantizationInfo
{
	double_RATIONAL_NUMBER offset;
	double_RATIONAL_NUMBER step;
    double_RATIONAL_NUMBER coverage;
    double_RATIONAL_NUMBER criterion;

	unsigned char numsOfBits;

	s_int_QUANTIZ_DIGIT numberOfSteps;
	
	double_RATIONAL_NUMBER rangeMin;
	double_RATIONAL_NUMBER rangeMax;

	s_int_QUANTIZ_DIGIT quantizedIntMin;
	s_int_QUANTIZ_DIGIT quantizedIntMax;

	SIGNED_INT quatizing_bitMask;
    UNSIGNED_INT mag_bitMask;

    unsigned int numberic_system;
    unsigned char ones_zero_handling;
    unsigned char val_ones_zero_handling;
}typedef struct_quantizationInfo;
//////////////////////////////////////////////////////////////////////////////////
struct_quantizationInfo global_QuantizationInfo;
//////////////////////////////////////////////////////////////////////////////////
/*
default value
	global_QuantizationInfo.offset=-0.5;
	global_QuantizationInfo.step=1.0;
	global_QuantizationInfo.numsOfBits=4;
*/
void initGlobalQuantizInfo
(
    double offset, 
    double step,
    unsigned char quantizBitNum, 
    unsigned int numbericSystem,
    unsigned char overping
)
{
    double* display_stimulus;
    signed int* display_result;
    char* display_hard_decision;
    signed int* display_twos_mag;
    signed int* display_ones_mag;
    double  tmp_double = 0.0f;
    unsigned int tmp_u_int = 0;
    unsigned int i;
	SIGNED_INT k;
	double_RATIONAL_NUMBER tmp_ratePerInteger_nums;

	printf("INITIALIZING QUANTIZATION FILTER\r\n");

    global_QuantizationInfo.numberic_system = numbericSystem;

	global_QuantizationInfo.offset=((double_RATIONAL_NUMBER)offset);

    switch(numbericSystem)
    {
        default:
        warningMesShort;
        printf("LLR numberic system is not selected, default: 2's complement\r\n");
        global_flag_case_llr_numberic_system = ENUM_FLAG_CASE_QUANTIZ_NUMBERIC_SYSTEM_TWO_S_COMPLEMENT;
        case ENUM_FLAG_CASE_QUANTIZ_NUMBERIC_SYSTEM_TWO_S_COMPLEMENT:
        /* zero symmetry */
        /* no zero symmetry */
        global_QuantizationInfo.ones_zero_handling = ENUM_FLAG_CASE_QUANTIZ_NUMBERIC_SYS_ONE_S_COM_NONE;
        global_QuantizationInfo.val_ones_zero_handling = 0;

        global_QuantizationInfo.criterion = ((double_RATIONAL_NUMBER)offset); 
        break;

        case ENUM_FLAG_CASE_QUANTIZ_NUMBERIC_SYSTEM_ONE_S_COMPLEMENT:
        {
            /*
            if(global_QuantizationInfo.offset != 0.0f)
            {
                warningMesShort;
                printf("Numberic system set to 1's complement.\r\n");
                warningMesShort;
                printf("Qauntizing offset is '%f', but will be set 0.0\r\n");
            }
	        global_QuantizationInfo.offset = 0.0f;
            */
        }
        

        /* zero symmetry */
        if(global_flag_case_llr_mag_ones_complement_zero_handling == \
            ENUM_FLAG_CASE_QUANTIZ_NUMBERIC_SYS_ONE_S_COM_ZERO_SYMMETRY)
        {
            /* zero symmetry */
            global_QuantizationInfo.ones_zero_handling = ENUM_FLAG_CASE_QUANTIZ_NUMBERIC_SYS_ONE_S_COM_ZERO_SYMMETRY;//zero_symmetry enable value is not change any value except with 1.
            global_QuantizationInfo.val_ones_zero_handling = 1;//zero_symmetry enable value is not change any value except with 1.
            global_QuantizationInfo.criterion = 0.0f;
        }
        else if(global_flag_case_llr_mag_ones_complement_zero_handling == \
            ENUM_FLAG_CASE_QUANTIZ_NUMBERIC_SYS_ONE_S_COM_ZERO_DUPLICATED)
        {
            /* zero duplication */
            global_QuantizationInfo.ones_zero_handling = ENUM_FLAG_CASE_QUANTIZ_NUMBERIC_SYS_ONE_S_COM_ZERO_DUPLICATED;//zero_symmetry enable value is not change any value except with 1.
            global_QuantizationInfo.val_ones_zero_handling = 1;//zero_symmetry enable value is not change any value except with 1.
            global_QuantizationInfo.criterion = ((double_RATIONAL_NUMBER)offset);
        }
        else
        {
            /* no zero symmetry */
            global_QuantizationInfo.ones_zero_handling = ENUM_FLAG_CASE_QUANTIZ_NUMBERIC_SYS_ONE_S_COM_NONE;
            global_QuantizationInfo.val_ones_zero_handling = 0;
            global_QuantizationInfo.criterion = ((double_RATIONAL_NUMBER)offset);
        }
        break;
    }

    global_QuantizationInfo.step=((double_RATIONAL_NUMBER)step);
	global_QuantizationInfo.coverage=((double_RATIONAL_NUMBER)step*0.5);
	global_QuantizationInfo.numsOfBits=quantizBitNum;
	global_QuantizationInfo.numberOfSteps = \
        (s_int_QUANTIZ_DIGIT)(pow(2.0,(double)global_QuantizationInfo.numsOfBits));
    /* zero symmetry */
    if(global_flag_case_llr_mag_ones_complement_zero_handling == \
        ENUM_FLAG_CASE_QUANTIZ_NUMBERIC_SYS_ONE_S_COM_ZERO_SYMMETRY)
    {
        /* overaping */
	    global_QuantizationInfo.numberOfSteps -= 1;
    }
    else if(global_flag_case_llr_mag_ones_complement_zero_handling == \
        ENUM_FLAG_CASE_QUANTIZ_NUMBERIC_SYS_ONE_S_COM_ZERO_DUPLICATED)
    {
        /* overaping */
	    global_QuantizationInfo.numberOfSteps -= 1;
    }
    

	global_QuantizationInfo.rangeMax = \
        ((double_RATIONAL_NUMBER)(global_QuantizationInfo.numberOfSteps/2)) * \
        global_QuantizationInfo.step*(1.0);
	global_QuantizationInfo.rangeMin = \
        ((double_RATIONAL_NUMBER)(global_QuantizationInfo.numberOfSteps/2)) * \
        global_QuantizationInfo.step*(-1.0);
    /* zero symmetry */
    if(global_flag_case_llr_mag_ones_complement_zero_handling == \
        ENUM_FLAG_CASE_QUANTIZ_NUMBERIC_SYS_ONE_S_COM_ZERO_SYMMETRY)
    {
        /* overaping */
	    global_QuantizationInfo.rangeMin += 1.0;
    }
    else if(global_flag_case_llr_mag_ones_complement_zero_handling == \
        ENUM_FLAG_CASE_QUANTIZ_NUMBERIC_SYS_ONE_S_COM_ZERO_DUPLICATED)
    {
        /* overaping */
	    global_QuantizationInfo.rangeMin += 1.0;
    }


	global_QuantizationInfo.quantizedIntMax = \
        (( (s_int_QUANTIZ_DIGIT)(pow(2.0,(double)global_QuantizationInfo.numsOfBits)) / 2)) - 1;
	global_QuantizationInfo.quantizedIntMin = \
        ((-1)*( (s_int_QUANTIZ_DIGIT)(pow(2.0,(double)global_QuantizationInfo.numsOfBits)) / 2));

	global_QuantizationInfo.quatizing_bitMask = 0;
	for(i=0; i<global_QuantizationInfo.numsOfBits; i++)
	{
		global_QuantizationInfo.quatizing_bitMask |= (1 << i);
	}
    global_QuantizationInfo.mag_bitMask = (global_QuantizationInfo.quatizing_bitMask >> 1);


    infoMes; printf("Quantiz Offset is %f\r\n", global_QuantizationInfo.offset);

    infoMes; printf("Quantiz Step is %f\r\n", global_QuantizationInfo.step);

    infoMes; printf("Quantiz Coverage is %f\r\n", global_QuantizationInfo.coverage);

    infoMes; printf("Quantiz Criterion is %f\r\n", global_QuantizationInfo.criterion);

    infoMes; printf("Quantiz number of bit is %d\r\n", global_QuantizationInfo.numsOfBits);

	infoMes; printf("Quantiz Number of Steps is %d\r\n", global_QuantizationInfo.numberOfSteps);
	
	infoMes; printf("Detectable Maximum range %f\r\n", \
            global_QuantizationInfo.rangeMax + global_QuantizationInfo.offset);
	infoMes; printf("Detectable Minumum range %f\r\n", \
            global_QuantizationInfo.rangeMin + global_QuantizationInfo.offset);
	
	infoMes; printf("Quantized Maximum range %d\r\n", global_QuantizationInfo.quantizedIntMax);
	infoMes; printf("Quantized Minumum range %d\r\n", global_QuantizationInfo.quantizedIntMin);
	
	
	infoMes; printf("Quantiz Bit MasK 0x%x\r\n", global_QuantizationInfo.quatizing_bitMask);		
	infoMes; printf("Quan mag Bit MasK 0x%x\r\n", global_QuantizationInfo.mag_bitMask);		

    infoMes; printf("numberic system %s\r\n", \
        NAME_QUANTIZ_NUMBERIC_SYSTEM[global_QuantizationInfo.numberic_system]);

    infoMes; printf("zero %s\r\n", \
         NAME_QUANTIZ_ONE_S_COMPLEMENT_ZERO_HANDLING\
         [global_QuantizationInfo.ones_zero_handling]);

    if(global_flag_llr_display&FLAG_LLR_DISPLAY_QUANTIZATION_FILTERING)
	{
        tmp_double = (global_QuantizationInfo.step / 10.0);
        infoMes; printf(">> segment of step : %f\r\n", tmp_double);
        tmp_u_int = (unsigned int)\
            (( (global_QuantizationInfo.rangeMax + global_QuantizationInfo.step \
                - global_QuantizationInfo.rangeMin + global_QuantizationInfo.step)) / \
            tmp_double);
        infoMes; printf(">> stimulus vector nums : %d\r\n", tmp_u_int);

        display_stimulus = (double*)malloc(sizeof(double) * tmp_u_int);
        display_result = (signed int*)malloc(sizeof(signed int) * tmp_u_int);
        display_hard_decision = (char*)malloc(sizeof(char) * tmp_u_int);
        display_twos_mag = (signed int*)malloc(sizeof(signed int) * tmp_u_int);
        display_ones_mag = (signed int*)malloc(sizeof(signed int) * tmp_u_int);

        for(i=0; i<tmp_u_int; i++) display_stimulus[i] = \
            (((double_RATIONAL_NUMBER)i) * tmp_double) + \
            global_QuantizationInfo.rangeMin - global_QuantizationInfo.step + \
            global_QuantizationInfo.offset;
		infoMes; printf("[Quantizing Filter Range Display Start]\r\n");
        quantizationWithGlobalAdaptive
        (
            display_stimulus,
            (SIGNED_INT*)display_result,
            display_hard_decision, 
            tmp_u_int
        );

        convertTwosComplementMagitude(
            display_twos_mag, 
            display_result, 
            tmp_u_int, 
            global_QuantizationInfo.quantizedIntMax);

        convertOnesComplementMagnitude(
            display_ones_mag, display_result, tmp_u_int);

        switch(numbericSystem)
        {
            default:
            case ENUM_FLAG_CASE_QUANTIZ_NUMBERIC_SYSTEM_TWO_S_COMPLEMENT:
                for(i=0; i<tmp_u_int; i++)
                {
                    printf("stim : %f, q_val : %d, HD : %c, 2's mag : %d, 1's mag : %d\r\n",
                        display_stimulus[i], display_result[i], display_hard_decision[i],
                        display_twos_mag[i], display_ones_mag[i]);
                }
            break;

            case ENUM_FLAG_CASE_QUANTIZ_NUMBERIC_SYSTEM_ONE_S_COMPLEMENT:
                for(i=0; i<tmp_u_int; i++)
                {
                    printf("stim : %f, q_val : %d, HD : %c, 1's mag : %d, 2's mag : %d\r\n",
                        display_stimulus[i], display_result[i], display_hard_decision[i],
                        display_ones_mag[i], display_twos_mag[i]);
                }
            break;
        }

		infoMes; printf("[Quantizing Filter Range Display End]\r\n");
        free(display_stimulus);
        free(display_result);
        free(display_hard_decision);
        free(display_twos_mag);
        free(display_ones_mag);
	}
}
//////////////////////////////////////////////////////////////////////////////////
#define QUANTIZ_MODE_STATIC	0
#define QUANTIZ_MODE_ADAPT	1
void quantizationWithGlobalStatic(double LLR[], SIGNED_INT LLR_quantization[], char *Codeword_MSG, unsigned int length)
{
	SIGNED_INT k;
	unsigned int i;
	
	
	
	for(i=0; i<length; i++)
	{
		if(LLR[i] < global_QuantizationInfo.rangeMin+global_QuantizationInfo.offset)//LLR is negative
		{
			// LLR_quantization[i]=(((signed int)(global_QuantizationInfo.numberOfSteps/2))*(-1)+1);
			LLR_quantization[i]=(((signed int)(global_QuantizationInfo.numberOfSteps/2))*(-1));
			if(Codeword_MSG) Codeword_MSG[i] = '1';
		}
		else if(global_QuantizationInfo.rangeMax+global_QuantizationInfo.offset <= LLR[i])//LLR is positive
		{
			LLR_quantization[i]=((((signed int)(global_QuantizationInfo.numberOfSteps/2)))-1);
			if(Codeword_MSG) Codeword_MSG[i] = '0';
		}
		else
		{
			if(global_QuantizationInfo.offset<LLR[i])//LLR is positive
			{
				for(k=0; k<(((signed int)(global_QuantizationInfo.numberOfSteps/2))); k++)
				{
					// if((((double_RATIONAL_NUMBER)k)+global_QuantizationInfo.offset<=LLR[i])&&(LLR[i]<(((double_RATIONAL_NUMBER)k)+global_QuantizationInfo.step)+global_QuantizationInfo.offset))
					if((((double_RATIONAL_NUMBER)k)+global_QuantizationInfo.offset<=LLR[i])&&(LLR[i]<(((double_RATIONAL_NUMBER)k)+global_QuantizationInfo.step)+global_QuantizationInfo.offset))
					{
						LLR_quantization[i]=k;
						break;
					}
				}
				if(Codeword_MSG) Codeword_MSG[i] = '0';
			}
			else//LLR is negative
			{
				for(k=(((signed int)(global_QuantizationInfo.numberOfSteps/2))*(-1)+0); k<0; k++)
				{
					// if((((double_RATIONAL_NUMBER)k)+global_QuantizationInfo.offset<=LLR[i])&&(LLR[i]<(((double_RATIONAL_NUMBER)k)+global_QuantizationInfo.step)+global_QuantizationInfo.offset))
					if((((double_RATIONAL_NUMBER)k)+global_QuantizationInfo.offset<=LLR[i])&&(LLR[i]<(((double_RATIONAL_NUMBER)k)+global_QuantizationInfo.step)+global_QuantizationInfo.offset))
					{
						LLR_quantization[i]=k;
						break;
					}
				}
				if(Codeword_MSG) Codeword_MSG[i] = '1';
			}
		}
	}
	return;
}
//////////////////////////////////////////////////////////////////////////////////
void quantizationWithGlobalAdaptive(
        double LLR[], 
        SIGNED_INT LLR_quantization[], 
        char *Codeword_MSG, 
        unsigned int length)
{
    unsigned int tmp_i;
    int tmp_q;
    double_RATIONAL_NUMBER tmp_step = global_QuantizationInfo.step;
    double_RATIONAL_NUMBER tmp_zeroSymmetry = \
        (double_RATIONAL_NUMBER)global_QuantizationInfo.val_ones_zero_handling;
    double_RATIONAL_NUMBER tmp_offset = global_QuantizationInfo.offset;

    for(tmp_i = 0; tmp_i < length; tmp_i++)
    {
        /* bigger than criterion? */
        if(LLR[tmp_i] >= global_QuantizationInfo.criterion)
        {
            /* 
             * LLR is larger than criterion 
             * Quantized value is positive
            */
            for(tmp_q = global_QuantizationInfo.quantizedIntMax; tmp_q > 0; tmp_q--)
            {
                if( (( ((double_RATIONAL_NUMBER)tmp_q) + tmp_offset)) * tmp_step <= \
                        LLR[tmp_i])
                {
                    LLR_quantization[tmp_i] = tmp_q;
                    break;
                }
            }
            if(tmp_q == 0)  LLR_quantization[tmp_i] = tmp_q;
            Codeword_MSG[tmp_i] = '0';
        }
        else
        {
            /* 
             * LLR is smaller than criterion 
             * Quantized value is negative
            */
            for(tmp_q = global_QuantizationInfo.quantizedIntMin; tmp_q < -1; tmp_q++)
            {
                if( ( ((double_RATIONAL_NUMBER)tmp_q) + tmp_offset + 1.0 + tmp_zeroSymmetry ) * \
                     tmp_step > LLR[tmp_i])
                {
                    LLR_quantization[tmp_i] = tmp_q;
                    break;
                }
            }
            if(tmp_q == -1)
            {
                switch(global_QuantizationInfo.ones_zero_handling)
                {
                    default:
                    case ENUM_FLAG_CASE_QUANTIZ_NUMBERIC_SYS_ONE_S_COM_ZERO_SYMMETRY:
                        LLR_quantization[tmp_i] = tmp_q;
                    break;

                    case ENUM_FLAG_CASE_QUANTIZ_NUMBERIC_SYS_ONE_S_COM_ZERO_DUPLICATED:
                        LLR_quantization[tmp_i] = ++tmp_q;
                    break;
                }
            }
            Codeword_MSG[tmp_i] = '1';
        }
    }
}

//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////enum treeTypes
enum treeTypes
{
	TREE_NONE,
	TREE_CHASE
};
struct struct_treeStructure
{
	/*
	Tree that is triple pointer indicate magnitudeShort.
	And it is using when find minimum magnitudeShort.
	*/
	enum treeTypes treeType;
	u_int_QUANTIZ_MAGNITUDE_DIGIT*** tree_mag;
    char*** tree_hd;
	unsigned char** direction;
	unsigned int** treeIndex;
	
	unsigned int* treeLength;
	unsigned int treeDepth;

}typedef struct_treeStructure;
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
			
			if((tmp_log-((double)tmp_treeDepth))==0.0)	tmp_treeDepth--;
			
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
		return 0;
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
	printf("p = 0x%08x\n", p);
	
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
	printf("p->tree_mag = 0x%08x, p->tree_hd = %c\n", p->tree_mag, p->tree_hd);

	if(!(p->treeIndex))
	{
		warningMes;
		printf("in testTreeStruct, struct_treeStructure p->treeIndex is NULL.\n");
		return;
	}
	debugMes;
	printf("p->treeIndex = 0x%08x\n", p->treeIndex);
	
	if(!(p->treeLength))
	{
		warningMes;
		printf("in testTreeStruct, struct_treeStructure p->treeLength is NULL.\n");
		return;
	}
	debugMes;
	printf("p->treeLength = 0x%08x\n", p->treeLength);
	
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
	debugMes; printf("treeStruct pointer : 0x%08x\n", p);
	debugMes; printf("root pointer : p->tree_mag = 0x%08x, p->tree_hd = 0x%08x\n", 
        p->tree_mag, p->tree_hd);
	for(i=0; i<p->treeDepth; i++)
	{
		debugMes; printf("%d-stage tree_mag pointer = 0x%08x, tree_hd pointer = 0x%08x\n",
                i, *(p->tree_mag+i), *(p->tree_hd+i));
		debugMes; printf("%d-stage length = %d\n",i, *(p->treeLength+i));		
		debugMes; printf("%d-stage tree_mag and tree_hd => ", i);
		for(j=0; j<(*(p->treeLength+i)); j++)
		{
			printf("0x%08x, 0x%08x(%u) ", 
                *((*(p->tree_mag+i))+j), 
                *((*(p->tree_hd+i))+j), 
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
	// s_int_QUANTIZ_DIGIT magnitudeMask;
	s_int_QUANTIZ_DIGIT *magnitude;
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
	unsigned int tmp_treeDepth;
	double tmp_log;

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
	
	// p->magnitudeShort=(s_int_QUANTIZ_DIGIT*)malloc(sizeof(s_int_QUANTIZ_DIGIT)*length);
	// memset(p->magnitudeShort, 0, sizeof(sizeof(s_int_QUANTIZ_DIGIT)*length));
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
	unsigned int i;
	
	
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
	findMinimumMagnitudeFindLocPushAway function is a convert quantized LLR to magnitudeShort and also compare each magnitudeShort so sort minimum magnitudeShort.
	For example, let's assume that quantization bit length is 4(include sign bit), LLR values are {-1, 0, 2, 7, 8, -5, 1, 3}.
	So LLR values are				{1111,	0000,	0010,	0111,	0111,	1011,	0001,	0011}.
	magnitudeShort values are			{0001,	0000,	0010,	0111,	0111,	0101,	0001,	0011}.
	Location indexes are			{0,		1,		2,		3,		4,		5,		6,		7}.
	
	Sorted minimum magnitudes are	{0000,	0001,	0001,	0010,	0011,	0101,	0111,	0111}.
	Sorted magnitudeShort locations are	{1,		0,		6,		2,		7,		5,		3,		4}.
	
	Results of this function are(caution that, result length belong to length of locator)
	Sorted minimum magnitudeShort are	{0000,	0001,	0001,	0010,	0011,	0101,	0111,	0111}.
	Sorted magnitudeShort location are	{1,		0,		6,		2,		7,		5,		3,		4}.
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
	unsigned int    tmp_criteria_loc = -1;

    char            tmp_criteria_HD;

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
	if(!hd_codeword) return -1;
	if(!(hd_codeword->length)) return -1;
	if(!(hd_codeword->usedLength)) return -1;
	if(!(hd_codeword->equation)) return -1;	
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
                                         * */
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
                                         * */
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
                     * */
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

	if(!hd_codeword) return -1;
	if(!(hd_codeword->length)) return -1;
	if(!(hd_codeword->usedLength)) return -1;
	if(!(hd_codeword->equation)) return -1;	
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
        //  previous min llr is larger or same than compared_mag => proccessing
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
                        //tmp_criteria_HD is not '1' that is tmp_criteria_HD has '0'.
                        if(tmp_criteria_HD == tmp_priorHD)
                        //criteria has prior HD.
                        {
                            //needs to compare with the others location value
                            //find non prior HD
                            //or
                            //lower llr mag
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
        else						//quantized LLR is negative, Hard decision is 1.
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
        else						//quantized LLR is negative, Hard decision is 1.
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
	unsigned int i;
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
		if(*(p->quantizedLLR+i)>0)	fprintf(fp, "+%d(", *(p->quantizedLLR+i));
		else						fprintf(fp, "+%d(", -1*(*(p->quantizedLLR+i)));

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
	To find minumum value of magnitude, initializing values associate with tree in struct struct_logLikeHoodRatio.
	Tree, can use, is define that is in enum treeTypes.
	If you needs this list, find a enum treeTypes.
*/
#define DIRECTION_LEFT	0
#define DIRECTION_RIGHT	1
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
				warningMes;	printf("in initializingVariablesAssociateWithTree, p->treeStruct is not NULL.\n");
				warningMesShort;	printf("p->treeStruct will be removed.\n");
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
		// if(!tmp_depth) printf("if(!tmp_depth) i[%d]\n", i);
	}

	// for(i=0; i<p->locatorLength; i++)
	// {
			// printf("V[%d]L[%d] ", *(p->magnitude+i), *(p->locator+i));
	// }
	// printf("\n");
	// printf("test) value[%d],loc[%d]", **(*(p->treeStruct->tree_mag+(p->treeStruct->treeDepth-1))+0), *((*(p->treeStruct->treeIndex+(p->treeStruct->treeDepth-1)))+0));
	
	return 0;
}
/*

char sortMinimumMagnitudeLogLikeHoodRatio_chaseAlgorithm(struct_logLikeHoodRatio *p)

m_(0)   \
        comp -> t(0,0), val:m_(0), direct:left, index:0
m_(1)   /       |
             comp -> t(1,0), val:t(0,0), direct:left, index:0   \
m_(2)   \       |                                               |
        comp -> t(0,1), val:m_(3), direct:right, index:3        |
m_(3)   /                                                       |
                                                            comp -> t(2,0), val:t(1,1), direct:right, index:6
m_(4)   \                                                       |
        comp -> t(0,2), val:m_(5), direct:right, index:5        |
m_(5)   /       |                                               |
            comp -> t(1,1), val:t(0,3), direct right. index:6   /
m_(6)   \       |
        comp -> t(0,3), val:m_(6), direct:left, index:6
m_(7)   /

.

.
                                                                \
.                                                               |
                                                                |
.                                                               |
                                                            comp -> t( ), val:t(2,(n-1)/2), direct:right, index:n-2
m_(n-4) \                                                       |
        comp -> t(0,(n-1/2)-1), m_(n-4), left, index:(n-4)      |
m_(n-3) /       |                                               |
            comp -> t(2,(n-1)/2), right, index:(n-2)            /
m_(n-2) \       |
        comp -> t(0,(n-1)/2), m_(n-2), left, index:(n-2)
m_(n-1) /
*/
char sortMinimumMagnitudeLogLikeHoodRatio_chaseAlgorithm(
    struct_logLikeHoodRatio *p,
    struct_powerFormPolynomials *hd_codeword,
    unsigned int zero_magnitude_sel_policy,
    unsigned int same_magnitude_sel_policy)
{
	unsigned int depth;
	unsigned int i,j,k;

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
	if(!hd_codeword) return -1;
	if(!(hd_codeword->length)) return -1;
	if(!(hd_codeword->usedLength)) return -1;
	if(!(hd_codeword->equation)) return -1;
    #endif
	
	if(global_bch_SoftCorrectable>p->treeStruct->treeDepth)
	{
		errorMes;
		printf("Finding minimum magnitude of LLR using chaseAlgorithm, is not support this condition(p(%d) larger than depth(%d)), yet.\n", global_bch_SoftCorrectable, p->treeStruct->treeDepth);
	}
	
	/* 0-th stage of tree */
			// debugMes; printf(">>> 0-th stage, tot length(%d)\n", p->usedLength);
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
                                (p->magnitude + i + DIRECTION_RIGHT);
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
                                    (p->magnitude + i + DIRECTION_LEFT);
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
                                    (p->magnitude + i + DIRECTION_RIGHT);
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
                                    (p->magnitude + i + DIRECTION_LEFT);
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
                                    (p->magnitude + i + DIRECTION_RIGHT);
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
                    (p->magnitude + i + DIRECTION_LEFT);
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
                                (p->magnitude + i + DIRECTION_LEFT);
                            *(*(p->treeStruct->tree_hd + 0) + i / 2) = \
                                (hd_codeword->equation + i + DIRECTION_LEFT);
                            *(*(p->treeStruct->direction + 0) + i / 2) = DIRECTION_LEFT;
                            *(*(p->treeStruct->treeIndex + 0) + i / 2) = i + DIRECTION_LEFT;
                            continue;
                        }
                    }
                }
                *(*(p->treeStruct->tree_mag + 0) + i / 2) = \
                    (p->magnitude + i + DIRECTION_RIGHT);
                *(*(p->treeStruct->tree_hd + 0) + i / 2) = \
                    (hd_codeword->equation + i + DIRECTION_RIGHT);
                *(*(p->treeStruct->direction + 0) + i / 2) = DIRECTION_RIGHT;
                *(*(p->treeStruct->treeIndex + 0) + i / 2) = i + DIRECTION_RIGHT;
			}
					// printf("depth(0),length(%d), %d(%d) <= %d(%d) --->>> ", i,  *(p->magnitude+i), i, *(p->magnitude+i+1), i+1);
					// printf("%d(%d)\n", **(*(p->treeStruct->tree_mag+0)+i/2), *(*(p->treeStruct->treeIndex+0)+i/2));
		}
		else
		{
            /* tree element is only one. */

			/* store redundant, no comparing, directly update to 0th stage tree. */
            /* update left one, because right one is not exist */
			*((*(p->treeStruct->tree_mag) + 0) + i / 2) = (p->magnitude + i - 1);
            *(*(p->treeStruct->tree_hd + 0) + i / 2) = \
                (hd_codeword->equation + i + DIRECTION_LEFT);
			*(*(p->treeStruct->direction + 0) + i / 2) = DIRECTION_LEFT;
			*(*(p->treeStruct->treeIndex + 0) + i / 2) = i - 1;
					// printf("EXTRA)) depth(0),length(%d), %d(%d) --->>> ", i, *(p->magnitude+i-1), i-1);
					// printf("%d(%d)\n", **(*(p->treeStruct->tree_mag+0)+i/2), *(*(p->treeStruct->treeIndex+0)+i/2));			
		}
	}
	
	/* 1st and 2nd, 3rd .... stage(depth) tree */
	for(depth = 1; depth < p->treeStruct->treeDepth; depth++)
	{
				// debugMes; printf(">>> %d-th stage, tot Length(%d)\n", depth, *(p->treeStruct->treeLength+depth-1));
		/* check element of stage(depth) */
		for(i = 0; i < *(p->treeStruct->treeLength + depth - 1); i += 2)
		{
			/* if can comparing */
			if(*(p->treeStruct->treeLength+depth - 1) - i != 1)
			{
						// printf("depth(%d), length(%d), %d(%d) <= %d(%d) --->>> ", depth, i, **(*(p->treeStruct->tree_mag+depth-1)+i), *(*(p->treeStruct->treeIndex+depth-1)+i), **(*(p->treeStruct->tree_mag+depth-1)+i+1), *(*(p->treeStruct->treeIndex+depth-1)+i+1));
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
						// printf("%d(%d)\n", **(*(p->treeStruct->tree_mag+depth)+i/2), *(*(p->treeStruct->treeIndex+depth)+i/2));
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
						// printf("EXTRA)) %d(%d) --->>> ", **(*(p->treeStruct->tree_mag+depth-1)+i-1), *(*(p->treeStruct->treeIndex+depth-1)+i-1));
						// printf("%d(%d)\n", **(*(p->treeStruct->tree_mag+depth)+i/2), *(*(p->treeStruct->treeIndex+depth)+i/2));			
			}
		}

	}
	// testPrintTreeStructValue(p->treeStruct);
	// findMinimumMagnitudeLogLikeHoodRatio_chaseAlgorithm_followMinPath(p);
	return 0;
}
//////////////////////////////////////////////////////////////////////////////////
/*
checkMinimumMagnitudeLogLikeHoodRatio(
	main_com_bpskComponents->bpskReceivedLLR,
	main_encodingComponentInGF->codeWord,
	main_com_errComponents->erroredCodeWord
)
*/
char checkMinimumMagnitudeLocatorValidityLogLikeHoodRatio(
    struct_logLikeHoodRatio *p, 
    struct_powerFormPolynomials *operandA, 
    struct_powerFormPolynomials *operandB)
{
	unsigned int i;
	#ifndef RELEASE
	if(!p)	return -1;
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
			if((*(operandA->equation+(*(p->locator+i)))) != (*(operandB->equation+(*(p->locator+i)))))	*(p->locatorValidity+i) = 1;
			else																						*(p->locatorValidity+i) = 0;
		}
        /* not indication any location */
        else if((*(p->locator+i)) == -1)
        {
            *(p->locatorValidity+i) = 1;
        }
		else
		{
			errorMes;
			printf("in checkMinimumMagnitudeLogLikeHoodRatio, (*(p->locator+%d)='%d' exceed (operandA->usedLength)='%d' or (operandB->usedLength)='%d'\n",
				i, (*(p->locator+i)), (operandA->usedLength), (operandB->usedLength) );
			return -2;
		}		
	}
	return 0;
}
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
char printQuatizLLRWithSpaceAndEnter(struct_logLikeHoodRatio *p)
{
	unsigned int i;

	if(!p)
	{
		errorMes;
		printf("in printQuatizLLRWithSpaceAndEnter, struct_logLikeHoodRatio p is NULL.\n");
		return -1;
	}
	if(!(p->quantizedLLR))
	{
		warningMes;
		printf("in printQuatizLLRWithSpaceAndEnter, struct_logLikeHoodRatio p->quantizedLLR is NULL.\n");
		return -1;
	}
	if(!(p->usedLength))
	{
		warningMes;
		printf("in printQuatizLLRWithSpaceAndEnter, struct_logLikeHoodRatio p->usedLength is zero.\n");
		return -1;
	}
	if(!(p->quantizedLLRMask))
	{
		warningMes;
		printf("in printQuatizLLRWithSpaceAndEnter, struct_logLikeHoodRatio p->quantizedLLRMask is zero.\n");
		return -1;
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
char printTestMagitudeQuatizLLRWithSpaceAndEnter(struct_logLikeHoodRatio *p)
{
	unsigned int i;

	if(!p)
	{
		errorMes;
		printf("in printTestMagitudeQuatizLLRWithSpaceAndEnter, struct_logLikeHoodRatio p is NULL.\n");
		return -1;
	}
	if(!(p->quantizedLLR))
	{
		warningMes;
		printf("in printTestMagitudeQuatizLLRWithSpaceAndEnter, struct_logLikeHoodRatio p->quantizedLLR is NULL.\n");
		return -1;
	}
	if(!(p->usedLength))
	{
		warningMes;
		printf("in printTestMagitudeQuatizLLRWithSpaceAndEnter, struct_logLikeHoodRatio p->usedLength is zero.\n");
		return -1;
	}
	if(!(p->quantizedLLRMask))
	{
		warningMes;
		printf("in printTestMagitudeQuatizLLRWithSpaceAndEnter, struct_logLikeHoodRatio p->quantizedLLRMask is zero.\n");
		return -1;
	}
	printf("[magnitudeShort Quantiz LLR] : \t");
	for(i=0; i<p->usedLength; i++)
	{
		if(*(p->quantizedLLR+i)>0)	printf("+%d '%c' ", *(p->quantizedLLR+i), *(p->hardDecisionShort+i));
		else						printf("+%d '%c' ", -1*(*(p->quantizedLLR+i)), *(p->hardDecisionShort+i));
	}
	printf("[Used Length : %d]\n", p->usedLength);
}
//////////////////////////////////////////////////////////////////////////////////
char printLLRWithSpaceAndEnter(struct_logLikeHoodRatio *p)
{
	unsigned int i;

	if(!p)
	{
		errorMes;
		printf("in printLLRWithSpaceAndEnter, struct_logLikeHoodRatio p is NULL.\n");
		return -1;
	}
	if(!(p->llr))
	{
		warningMes;
		printf("in printLLRWithSpaceAndEnter, struct_logLikeHoodRatio p->llr is NULL.\n");
		return -1;
	}
	if(!(p->usedLength))
	{
		warningMes;
		printf("in printLLRWithSpaceAndEnter, struct_logLikeHoodRatio p->usedLength is zero.\n");
		return -1;
	}
	// if(!(p->quantizedLLRMask))
	// {
		// warningMes;
		// printf("in printLLRWithSpaceAndEnter, struct_logLikeHoodRatio p->quantizedLLRMask is zero.\n");
		// return -1;
	// }
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
char testPrintLLRWithSpaceAndEnter(struct_logLikeHoodRatio *p)
{
	unsigned int i;

	if(!p)
	{
		errorMes;
		printf("in testPrintLLRWithSpaceAndEnter, struct_logLikeHoodRatio p is NULL.\n");
		return -1;
	}
	if(!(p->llr))
	{
		warningMes;
		printf("in testPrintLLRWithSpaceAndEnter, struct_logLikeHoodRatio p->llr is NULL.\n");
		return -1;
	}
	if(!(p->usedLength))
	{
		warningMes;
		printf("in testPrintLLRWithSpaceAndEnter, struct_logLikeHoodRatio p->usedLength is zero.\n");
		return -1;
	}

	printf("[LLR and Qantized LLR]\n");
	for(i=0; i<p->usedLength; i++)
	{
		if(*(p->llr+i)>=0)			printf("+%g\t", *(p->llr+i));
		else						printf("%g\t", *(p->llr+i));
		if(*(p->quantizedLLR+i)>=0)	printf("+%d\n", *(p->quantizedLLR+i));
		else						printf("%d\n", *(p->quantizedLLR+i));
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
	unsigned int i;
	struct_logLikeHoodRatio *p=NULL;

	p=createLogLikeHoodRatio(length);
			#ifndef RELEASE
			if(!(p))
			{
				warningMes;
				printf("in createLogLikeHoodRatioUsingNumberOfMaskBits, p=createLogLikeHoodRatio(length) return NULL.\n", length);
				return NULL;
			}
			#endif

	if(setQuantizedLLRMaskOfLogLikeHoodRatio(p, numberOfMask))
	{
		closeLogLikeHoodRatio(&p);
				#ifndef RELEASE
				errorMes; printf("in createLogLikeHoodRatioUsingNumberOfMaskBits, setQuantizedLLRMaskOfLogLikeHoodRatio(p, numberOfMask) return Non-zero.\n", length);
				#endif
		return NULL;
	}
	
	
	// if(!(p->llr))
	// {
		// if(!(p->length))
		// {
			// p->quantizedLLRMask=0;
			// for(i=0; i<numberOfMask; i++)
			// {
				// p->quantizedLLRMask|=(1<<i);
			// }
			// p->magnitudeMask=p->quantizedLLRMask&(~(1<<(i-1)));
					// printf("Mask test quantizat mask %d\nMask test magnitudeShort mask %d\n",p->quantizedLLRMask, p->magnitudeMask);
		// }
		// else
		// {
			// p->quantizedLLRMask=0;
			// warningMes;
			// printf("in initialLlrValueUsingMask, (*p)->length is zero.\n");
			// return -1;
		// }
	// }
	return p;
}
//////////////////////////////////////////////////////////////////////////////////
struct_logLikeHoodRatio* createLogLikeHoodRatioUsingLlrMask(unsigned int length, SIGNED_INT quantizedLLRMask)
{
	unsigned int i;
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
		printf("in createLogLikeHoodRatioUsingLlrMask, p=createLogLikeHoodRatio(length) return NULL.\n", length);
		return NULL;
	}
	#endif

	// if(!(p->llr))
	// {
		// if(!(p->length))
		// {
			p->quantizedLLRMask=quantizedLLRMask;
			//for(i=0; (i<(sizeof(s_int_QUANTIZ_DIGIT)*8))&&((1<<i)&quantizedLLRMask); i++);
			// p->magnitudeMask=(p->quantizedLLRMask&(~(1<<(i-1))));
					// printf("Mask test quantizat mask %d\nMask test magnitudeShort mask %d\n",p->quantizedLLRMask, p->magnitudeMask);
		// }
		// else
		// {
			// p->quantizedLLRMask=0;
			// warningMes;
			// printf("in initialLlrValueUsingMask, (*p)->length is zero.\n");
			// return -1;
		// }
	// }
	return p;
}
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
#define BPSK_TRANCEIVE_FLAG_DATA_INIT	0
#define BPSK_TRANCEIVE_FLAG_DATA_LOAD	(1<<0)
#define BPSK_TRANCEIVE_FLAG_DATA_TRANS	(1<<1)
#define BPSK_TRANCEIVE_FLAG_DATA_RECEIV	(1<<2)

typedef double double_BPSK_DATA;

struct struct_bpskComponents
{
	unsigned int length;
	unsigned int usedLength;
	double_BPSK_DATA *bpskTransData;
	double_BPSK_DATA *bpskReceivedDataAddedAwgn;
	double_BPSK_DATA squreRootBpskReceivedDataAddedAwgn;
	
	// double_BPSK_DATA *bpskReceivedDataLLR;
	struct_logLikeHoodRatio *bpskReceivedLLR;
	unsigned char statusFlag;
}typedef struct_bpskComponents;
//////////////////////////////////////////////////////////////////////////////////
struct_bpskComponents *createBpskComponents(unsigned int length)
{
	struct_bpskComponents *p=(struct_bpskComponents*)malloc(sizeof(struct_bpskComponents));
	memset(p, 0, sizeof(struct_bpskComponents));
	
	p->bpskTransData=(double_BPSK_DATA*)malloc(sizeof(double_BPSK_DATA)*(length));
	memset(p->bpskTransData, 0, sizeof(double_BPSK_DATA)*length);
			debugMes; printf("in createBpskComponents, p->bpskTransData=(double_BPSK_DATA*)malloc(sizeof(double_BPSK_DATA)*(length));\n");
			debugMes; printf("in createBpskComponents, p->bpskTransData=0x%016x\n", p->bpskTransData);
	
	p->bpskReceivedDataAddedAwgn=(double_BPSK_DATA*)malloc(sizeof(double_BPSK_DATA)*length);
	memset(p->bpskReceivedDataAddedAwgn, 0, sizeof(double_BPSK_DATA)*length);
	
	// p->bpskReceivedDataLLR=(double_BPSK_DATA*)malloc(sizeof(double_BPSK_DATA)*length);
	// memset(p->bpskReceivedDataLLR, 0, sizeof(double_BPSK_DATA)*length);
	
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
//////////////////////////////////////////////////////////////////////////////////
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
//////////////////////////////////////////////////////////////////////////////////
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
//////////////////////////////////////////////////////////////////////////////////
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
//////////////////////////////////////////////////////////////////////////////////
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
	
	// for(i=0; i<WordLength; i++)
	// {
		// printf("%c ", Codeword_MSG[i]);
	// }
	// for(i=0; i<WordLength; i++)
	// {
		// printf("%f ", Transmitted_MSG[i]);
	// }
	
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
//////////////////////////////////////////////////////////////////////////////////
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
//////////////////////////////////////////////////////////////////////////////////
double gaussian(double mean, double var)
{
		const double PI = 3.14159265358979323846;
		double temp1, temp2, x1, x2;
		
		x1 = (rand() + 1);				//x1??? 0??? ???????????? ?????????
		x2 = rand();					//0??? ???????????? ?????? ????????????.
		// x1 = (rand_r(&tmp_seed) + 1);				//x1??? 0??? ???????????? ?????????
		// x2 = rand_r(&tmp_seed);					//0??? ???????????? ?????? ????????????.
				// infoMes;
				// printf("x1: %g,\tx2: %g,\t",x1,x2);

		x1 = x1 / (((double)RAND_MAX) + 1.0);
		// x1 = x1 / (RAND_MAX);
		x2 = x2 / ((double)RAND_MAX);
				// printf("x1: %g,\tx2: %g\n",x1,x2);
		temp1 = log(x1);
		temp1 = temp1 * (-2);
		temp1 = sqrt(temp1);			//??????????????? ???. sqrt(-2*log(x1))
		
		temp2 = 2 * PI;
		temp2 = temp2 * x2;
		temp2 = cos(temp2);				//??????????????? ???. cos(x2*2*PI)

		temp1 = temp1 * temp2;			//??? ??? ?????? ????????? N[0,1] ??????

		temp1 = (sqrt(var) * temp1);	//adjust variance

		return temp1 + mean;			//adjust mean
}
//////////////////////////////////////////////////////////////////////////////////
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
//////////////////////////////////////////////////////////////////////////////////
double_BPSK_DATA LLR_MAX=0;
double_BPSK_DATA LLR_MIN=0;
void ADD_AWGN_CAL_LLR(double_BPSK_DATA *transmitted_msg, double_BPSK_DATA *received_msg, double_BPSK_DATA *received_LLR, double bitrate, unsigned int number_of_loop, double main_com_EbN0) //need to add bitrate
{
	unsigned int i;

	double Sum_Pow = 0.0f;
	double spow, npow, attn;

	double *channel_noise = (double*)malloc(sizeof(double)*number_of_loop);
	memset(channel_noise, 0, sizeof(double)*number_of_loop);


	
	for(i=0;i<number_of_loop;i++)
	{
		channel_noise[i] = gaussian(0, 1);
	}
	#ifdef USING_OPEN_MP
	#pragma omp parallel for schedule(guided) private(i) shared(number_of_loop, transmitted_msg) reduction(+:Sum_Pow)
	#endif
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
		received_LLR[i] = (-2 * received_msg[i]) / (attn*attn);
					/* printf("test : [+1.5] => %g\n", (-2 * +1.5 / (attn*attn)));
					printf("test : [+1.0] => %g\n", (-2 * +1.0 / (attn*attn)));
					printf("test : [+0.5] => %g\n", (-2 * +0.5 / (attn*attn)));
					printf("test : [+0.0] => %g\n", (-2 * +0.0 / (attn*attn)));
					printf("test : [-0.5] => %g\n", (-2 * -0.5 / (attn*attn)));
					printf("test : [-1.0] => %g\n", (-2 * -1.0 / (attn*attn)));
					printf("test : [-1.5] => %g\n", (-2 * -1.5 / (attn*attn)));
					printf("\n\n"); */
		if(LLR_MAX<received_LLR[i]) LLR_MAX=received_LLR[i];
		if(LLR_MIN>received_LLR[i]) LLR_MIN=received_LLR[i];
	}
	free(channel_noise);
	return;
}
//////////////////////////////////////////////////////////////////////////////////
void printBpskModulation(unsigned int printLength, double_BPSK_DATA *p)
{
	unsigned int i;
	
	if(!printLength)
	{
		warningMes;
		printf("in printBpskModulation, printLength is zero.\n");
		return;
	}
	if(!p)
	{
		errorMes;
		printf("in printBpskModulation, double_BPSK_DATA p is NULL.\n");
		return;
	}
	
	for(i=0; i<printLength; i++)
	{
		printf("%.2f ", *(p+i));
	}
	printf("used L:%d\n", i);
}
//////////////////////////////////////////////////////////////////////////////////
void printBpskAWGN(unsigned int printLength, double_BPSK_DATA *operandA, double_BPSK_DATA *operandB)
{
	unsigned int i;

	if(!printLength)
	{
		warningMes;
		printf("in printBpskAWGN, printLength is zero.\n");
		return;
	}
	if(!operandA)
	{
		errorMes;
		printf("in printBpskAWGN, double_BPSK_DATA operandA is NULL.\n");
		return;
	}
	if(!operandB)
	{
		errorMes;
		printf("in printBpskAWGN, double_BPSK_DATA operandA is NULL.\n");
		return;
	}
	
	for(i=0; i<printLength; i++)
	{
		printf("%.2f ", *(operandA+i)-*(operandB+i));
	}
	printf("used L:%d\n", i);
}
//////////////////////////////////////////////////////////////////////////////////
void printLLR(unsigned int printLength, double_BPSK_DATA *p)
{
	unsigned int i;
	
	if(!printLength)
	{
		warningMes;
		printf("in printLLR, printLength is zero.\n");
		return;
	}
	if(!p)
	{
		errorMes;
		printf("in printLLR, double_BPSK_DATA p is NULL.\n");
		return;
	}
	
	for(i=0; i<printLength; i++)
	{
		printf("%.2f ", *(p+i));
	}
	printf("used L:%d, MAX:\'%.2f\', MIN:\'%.2f\'\n", i, LLR_MAX, LLR_MIN);
}
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
		errorMes; printf("in set_unSelTp_bofore_decoding, struct_powerFormPolynomials *(INFO_list_TP_CW_pwrFormPoly+%d) is NULL.\r\n", tmp_i);
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
		note. Theoretical error count.
		Theoretic is mean, simulator known that channel enviroments,
		that is mean simulator recognize error number corrrectly and already.
		So in below code and theoretical BER mode, 
		Simulator identify best test-pattern and,
		choose best test pattern has minimum error number.
		*/

		case FLAG_CASE_SEL_TP_BEFORE_DEC_THEO:
		{
		/* initializing... */
		//(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))

			tmp_sel_TP = INFO_testPatternNum;//debug mode, to accur core dump
			/********** selecting which error count number of test patterns **********/
			/*
				note. belong to which chosen, BER result is effected.
			*/
			switch(global_flag_case_choosingCodeWordMethod_inTheo)
			{
				default:
					warningMes; printf("Method of gettig BER is not selected, So selecting method is pixed to normal(default).\r\n");

				/*
					note. if no errors in all test patterns, select this.
					if not, select hard-decision error number.
					!!! WARNING !!!
					not support FLAG_CASE_CODEWORD_CHOOSEN_METHOD_IN_THEO_NO_ERR.
				*/
				case FLAG_CASE_CODEWORD_CHOOSEN_METHOD_IN_THEO_NO_ERR:
					// printf("[Z]");
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
						note. if no error bit count, error count to fix hard-decision. 
					*/
					if(tmp_sel_TP == INFO_testPatternNum)
					{
						tmp_selTP_errCnt = INFO_list_errCntOfTP_beforeDec_theo[0];
						tmp_sel_TP = 0;
					}
				break;
					
				/*
					note. selecting error bits belong to correctable test pattern's error number.
					if can't correct, select hard-decision error number.
					in normal(this) case, designer have to think about how to select correctable codeword. 
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
					note. selecting minimum error bits comparing each error bits number, patterns have.
						In best(this) case, not guarantee decording. 
				*/
				case FLAG_CASE_CODEWORD_CHOOSEN_METHOD_IN_THEO_MIN_ERR:
					// printf("[B]");
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
					note. selecting maximum error bits comparing each error bits number, patterns have.
						In worst(this) case, not guarantee decording. 
				*/
				case FLAG_CASE_CODEWORD_CHOOSEN_METHOD_IN_THEO_MAX_ERR:
					// for(main_tmp_sel_decAlgo_i=0; main_tmp_sel_decAlgo_i<processingUseThisAlgorithm->length; main_tmp_sel_decAlgo_i++)
					// {
						// tmp_sel_TP = 0;//debug mode, to accur core dump
					// }							
					// printf("[W]");
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
				note. main_indi_sel_decoding_CW_pwrFormPoly[KIND_OF_BCH_DECODING_BM] have a memory address of decorded codewoard, which type is struct_powerFormPolynomials*.
					In soft-dedision case, it has same function.
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
		note. Actual error count.
		Actual is mean, simulator does not known that channel enviroments,
		that is mean simulator can not recognize error number corrrectly.
		So in below code and actual BER mode, 
		simulator can not choose best test pattern has minimum error number.
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
		/************** case end                            ***************/
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
	
	
	// for(tmp_i = 0; tmp_i < INFO_TP_Nums; tmp_i++)
	// {
		// printf("test)) W_list_flag_decodingFail[%d] = %s, R_list_flag_beforeDec_synd_zero[%d] = %s, selected TP = '%d', ADDR : 0x%x\r\n",
			// tmp_i, 
			// W_list_flag_decodingFail[tmp_i]?"[ON]":"[OFF]", 
			// tmp_i, 
			// R_list_flag_beforeDec_synd_zero[tmp_i]?"[ON]":"[OFF]",
			// *W_setlectedTP,
			// W_setlectedTP
		// );
	// }
	
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
/*initializing*/

//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
	// while((loadStream.countedDataNumber<loadStream.totalDataNumber))
	// {
		// printf("[%0000006d] : ", loadStream.countedCodeWordNumber);
		// loadToCodeWordStartAtHighSide(main_encodingComponentInGF->codeWord, &loadStream);
		// unloadFromCodeWordStartAtHighSide(&unloadStream, main_encodingComponentInGF->codeWord);
		// printPowerFormWithEnterPolynomialWithTapUsingAddress(main_encodingComponentInGF->codeWord);
		// printf("[S:%d, U:%d, M:%d]\n",strlen(main_encodingComponentInGF->codeWord->equation), main_encodingComponentInGF->codeWord->usedLength, main_encodingComponentInGF->codeWord->length);
	// }
	// printBmpFileToHexCode2();
	// saveBmpFile2(0);
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
			for(i=0; i<filePass_codeLength; i++)		//?????? ?????????
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


/*
switch(temp->instType)
{
	case OPT_NOTHING:
		printf("instType: OPT_NOTHING[%d]\n", temp->instType);
		break;
	case OPT_FLAG:
		printf("instType: OPT_FLAG[%d]\n", temp->instType);
		break;
	
	case OPT_DEBUG:
		printf("instType: OPT_DEBUG[%d]\n", temp->instType);
		break;
	
	case OPT_DISPLAY:
		printf("instType: OPT_DISPLAY[%d]\n", temp->instType);
		break;
	case OPT_DISPLAY_RESULT:
		printf("instType: OPT_DISPLAY_RESULT[%d]\n", temp->instType);
		break;
	case OPT_DISPLAY_PROGRESS:
		printf("instType: OPT_DISPLAY_PROGRESS[%d]\n", temp->instType);
		break;
	case OPT_DISPLAY_FUNCNAME:
		printf("instType: OPT_DISPLAY_FUNCNAME[%d]\n", temp->instType);
		break;

	case OPT_SET_VALUE:
		printf("instType: OPT_SET_VALUE[%d]\n", temp->instType);
		break;
	
	case OPT_SET_VALUES:
		printf("instType: OPT_SET_VALUES[%d]\n", temp->instType);
		break;
	case OPT_SET_VALUES_SERIES: //example) {-settingCategory=[value0],[value1],[value2],[value3]}
		printf("instType: OPT_SET_VALUES_SERIES[%d]\n", temp->instType);
		break;

	case OPT_SET_LANGE: //set lange, example) {-settingCategory=[start]:[step]:[end]}
		printf("instType: OPT_SET_LANGE[%d]\n", temp->instType);
		break;
	case OPT_SET_LANGE_SERIES:
		printf("instType: OPT_SET_LANGE_SERIES[%d]\n", temp->instType);
		break;

	case OPT_STRING:
		printf("instType: OPT_STRING[%d]\n", temp->instType);
		break;
	case OPT_COMPONENT: //move to sub category
		printf("instType: OPT_COMPONENT[%d]\n", temp->instType);
		break;
	
	case STRUCT_END:
		printf("instType: STRUCT_END[%d]\n", temp->instType);
		break;
		
	default:
		printf("instType: UNKNOWN[?]\n");
}
*/

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
/*BCH Command Tree start*/
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* 
switch(temp->instType)
{
	case OPT_NOTHING:
		printf("instType: OPT_NOTHING[%d]\n", temp->instType);
		break;
	case OPT_FLAG:
		printf("instType: OPT_FLAG[%d]\n", temp->instType);
		break;
	
	case OPT_DEBUG:
		printf("instType: OPT_DEBUG[%d]\n", temp->instType);
		break;
	
	case OPT_DISPLAY:
		printf("instType: OPT_DISPLAY[%d]\n", temp->instType);
		break;
	case OPT_DISPLAY_RESULT:
		printf("instType: OPT_DISPLAY_RESULT[%d]\n", temp->instType);
		break;
	case OPT_DISPLAY_PROGRESS:
		printf("instType: OPT_DISPLAY_PROGRESS[%d]\n", temp->instType);
		break;
	case OPT_DISPLAY_FUNCNAME:
		printf("instType: OPT_DISPLAY_FUNCNAME[%d]\n", temp->instType);
		break;

	case OPT_SET_VALUE:
		printf("instType: OPT_SET_VALUE[%d]\n", temp->instType);
		break;
	
	case OPT_SET_VALUES:
		printf("instType: OPT_SET_VALUES[%d]\n", temp->instType);
		break;
	case OPT_SET_VALUES_SERIES: //example) {-settingCategory=[value0],[value1],[value2],[value3]}
		printf("instType: OPT_SET_VALUES_SERIES[%d]\n", temp->instType);
		break;

	case OPT_SET_LANGE: //set lange, example) {-settingCategory=[start]:[step]:[end]}
		printf("instType: OPT_SET_LANGE[%d]\n", temp->instType);
		break;
	case OPT_SET_LANGE_SERIES:
		printf("instType: OPT_SET_LANGE_SERIES[%d]\n", temp->instType);
		break;

	case OPT_STRING:
		printf("instType: OPT_STRING[%d]\n", temp->instType);
		break;
	case OPT_COMPONENT: //move to sub category
		printf("instType: OPT_COMPONENT[%d]\n", temp->instType);
		break;
	
	case STRUCT_END:
		printf("instType: STRUCT_END[%d]\n", temp->instType);
		break;
		
	default:
		printf("instType: UNKNOWN[?]\n");
}
 */
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/************************************ bch_algorithm 4 t h - S t a r t ************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/***** static struct struct_cmdLineOption	bch_BM_algorithm_HD_option[] start *****/
	#ifndef RELEASE
	static struct struct_cmdLineOption	bch_BM_algorithm_HD_display_option[] =
	{
		{"result",		0,	OPT_FLAG,	0,	0,	&global_flag_bch_BM_SimHD_display,	FLAG_MASK_DISPLAY_RESULT,		VALUE_TYPE_UNSIGNED_INT,	NULL},
		{"progress",	0,	OPT_FLAG,	0,	0,	&global_flag_bch_BM_SimHD_display,	FLAG_MASK_DISPLAY_PROGRESS,	VALUE_TYPE_UNSIGNED_INT,	NULL},
		{"funcname",	0,	OPT_FLAG,	0,	0,	&global_flag_bch_BM_SimHD_display,	FLAG_MASK_DISPLAY_FUNCNAME,	VALUE_TYPE_UNSIGNED_INT,	NULL},
		{NULL,0,STRUCT_END,0,0,NULL,0,VALUE_TYPE_NONE,NULL}
	};
	#endif

	#ifndef RELEASE
	static struct struct_cmdLineOption	bch_BM_algorithm_SD_display_option[] =
	{
		{"result",		0,	OPT_FLAG,	0,	0,	&global_flag_bch_BM_SimSD_display,	FLAG_MASK_DISPLAY_RESULT,	VALUE_TYPE_UNSIGNED_INT,	NULL},
		{"progress",	0,	OPT_FLAG,	0,	0,	&global_flag_bch_BM_SimSD_display,	FLAG_MASK_DISPLAY_PROGRESS,	VALUE_TYPE_UNSIGNED_INT,	NULL},
		{"funcname",	0,	OPT_FLAG,	0,	0,	&global_flag_bch_BM_SimSD_display,	FLAG_MASK_DISPLAY_FUNCNAME,	VALUE_TYPE_UNSIGNED_INT,	NULL},
		{NULL,0,STRUCT_END,0,0,NULL,0,VALUE_TYPE_NONE,NULL}
	};
	#endif
/***** static struct struct_cmdLineOption	bch_BM_algorithm_HD_option[] end *****/
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/************************************ bch_algorithm 4 t h - E n d ************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/************************************ bch_algorithm 3 r d - S t a r t ************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/***** static struct struct_cmdLineOption bch_BM_algorithm_option[] start *****/
static struct struct_cmdLineOption	bch_BM_algorithm_test_option[] =
{
	{"used",	0,	OPT_FLAG,		0,	0,	&global_list_flag_bch_sim_decodingMode[FLAG_MASK_BCH_DECORDING_ALGORITHM_BM],			FLAG_MASK_CODE_BCH_DEC_AL_TEST_THIS_ALGORITHM,	VALUE_TYPE_UNSIGNED_INT,	NULL},//if(global_flag_bchBM_simulationMode&FLAG_MASK_CODE_BCH_DEC_AL_TEST_THIS_ALGORITHM)

	{NULL,0,STRUCT_END,0,0,NULL,0,VALUE_TYPE_NONE,NULL}
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static struct struct_cmdLineOption	bch_BM_algorithm_decoding_option[] =
{
	{"skip",	0,		OPT_FLAG,		0,	0,	&global_list_flag_bch_sim_decodingMode[KIND_OF_BCH_DECODING_BM],		FLAG_MASK_CODE_BCH_DEC_AL_SKIP,		VALUE_TYPE_UNSIGNED_INT,	NULL},

	{NULL,0,STRUCT_END,0,0,NULL,0,VALUE_TYPE_NONE,NULL}
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static struct struct_cmdLineOption	bch_BM_algorithm_debug_calculate_option[] =
{
	{"delta",	0,		OPT_FLAG,		0,	0,	&global_flag_debug_bmSim,		FLAG_MASK_DEBUG_BCH_BM_CAL_DELTA,			VALUE_TYPE_UNSIGNED_INT,	NULL},

	{NULL,0,STRUCT_END,0,0,NULL,0,VALUE_TYPE_NONE,NULL}
};

	#ifndef RELEASE
	static struct struct_cmdLineOption	bch_BM_algorithm_debug_option[] =
	{
		{"sequence",	0,	OPT_FLAG,		0,	0,	&global_flag_debug_bmSim,	FLAG_MASK_DEBUG_BCH_BM_ALGORITHM,	VALUE_TYPE_UNSIGNED_INT,	NULL},
		{"cal",			0,	OPT_COMPONENT,	0,	0,	NULL,						0,									VALUE_TYPE_NONE,			bch_BM_algorithm_debug_calculate_option},

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
	{"used",			0,	OPT_FLAG,		0,	0,	&global_flag_bchDecAlgo_Enable,										FLAG_MASK_BCH_DECORDING_ALGORITHM_BM,			VALUE_TYPE_UNSIGNED_INT,	NULL},

	{"algorithm",		0,	OPT_COMPONENT,	0,	0,	&global_list_flag_bch_sim_decodingMode[KIND_OF_BCH_DECODING_BM],		FLAG_MASK_CODE_BCH_DEC_AL_TEST_THIS_ALGORITHM,	VALUE_TYPE_UNSIGNED_INT,	bch_BM_algorithm_test_option},
	#ifndef RELEASE
	{"display_HD",			0,	OPT_COMPONENT,	0,	0,	NULL,																0,												VALUE_TYPE_NONE,			bch_BM_algorithm_HD_display_option},
	{"display_SD",			0,	OPT_COMPONENT,	0,	0,	NULL,																0,												VALUE_TYPE_NONE,			bch_BM_algorithm_SD_display_option},
	#endif
		
	{"hard_decision",	0,	OPT_FLAG,		0,	0,	&global_list_flag_bch_sim_decodingMode[KIND_OF_BCH_DECODING_BM],		FLAG_MASK_CODE_BCH_DEC_AL_HARD_DECISION,		VALUE_TYPE_UNSIGNED_INT,	NULL},//if(global_list_flag_bch_sim_decodingMode[KIND_OF_BCH_DECODING_BM]&FLAG_MASK_CODE_BCH_DEC_AL_HARD_DECISION)
	{"soft_decision",	0,	OPT_FLAG,		0,	0,	&global_list_flag_bch_sim_decodingMode[KIND_OF_BCH_DECODING_BM],		FLAG_MASK_CODE_BCH_DEC_AL_SOFT_DECISION,		VALUE_TYPE_UNSIGNED_INT,	NULL},//if(global_list_flag_bch_sim_decodingMode[KIND_OF_BCH_DECODING_BM]&FLAG_MASK_CODE_BCH_DEC_AL_SOFT_DECISION)

		
	{"decoding",		0,	OPT_COMPONENT,	0,	0,	NULL,																0,												VALUE_TYPE_NONE,			bch_BM_algorithm_decoding_option},

	#ifndef RELEASE
	{"debug",			0,	OPT_DEBUG,		0,	0,	NULL,																0,												VALUE_TYPE_NONE,			bch_BM_algorithm_debug_option},
	#endif
	
	{NULL,0,STRUCT_END,0,0,NULL,0,VALUE_TYPE_NONE,NULL}
};
/***** static struct struct_cmdLineOption bch_algorithm[] end *****/
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/************************************ bch_algorithm 2 n d - E n d ************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/******************************* bch_mSBS_algorithm_option *******************************/
/******************************* bch_mSBS_algorithm_option *******************************/
/******************************* bch_mSBS_algorithm_option *******************************/


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	#ifndef RELEASE
	static struct struct_cmdLineOption	bch_mSBS_algorithm_HD_display_option[] =
	{
		{"result",		0,	OPT_FLAG,	0,	0,	&global_flag_bch_mSBS_SimHD_display,	FLAG_MASK_DISPLAY_RESULT,		VALUE_TYPE_UNSIGNED_INT,	NULL},
		{"progress",	0,	OPT_FLAG,	0,	0,	&global_flag_bch_mSBS_SimHD_display,	FLAG_MASK_DISPLAY_PROGRESS,	VALUE_TYPE_UNSIGNED_INT,	NULL},
		{"funcname",	0,	OPT_FLAG,	0,	0,	&global_flag_bch_mSBS_SimHD_display,	FLAG_MASK_DISPLAY_FUNCNAME,	VALUE_TYPE_UNSIGNED_INT,	NULL},
		{NULL,0,STRUCT_END,0,0,NULL,0,VALUE_TYPE_NONE,NULL}
	};
	#endif

	#ifndef RELEASE
	static struct struct_cmdLineOption	bch_mSBS_algorithm_SD_display_option[] =
	{
		{"result",		0,	OPT_FLAG,	0,	0,	&global_flag_bch_mSBS_SimSD_display,	FLAG_MASK_DISPLAY_RESULT,	VALUE_TYPE_UNSIGNED_INT,	NULL},
		{"progress",	0,	OPT_FLAG,	0,	0,	&global_flag_bch_mSBS_SimSD_display,	FLAG_MASK_DISPLAY_PROGRESS,	VALUE_TYPE_UNSIGNED_INT,	NULL},
		{"funcname",	0,	OPT_FLAG,	0,	0,	&global_flag_bch_mSBS_SimSD_display,	FLAG_MASK_DISPLAY_FUNCNAME,	VALUE_TYPE_UNSIGNED_INT,	NULL},
		{NULL,0,STRUCT_END,0,0,NULL,0,VALUE_TYPE_NONE,NULL}
	};
	#endif
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static struct struct_cmdLineOption	bch_mSBS_algorithm_test_option[] =
{
	{"used",	0,	OPT_FLAG,		0,	0,	&global_list_flag_bch_sim_decodingMode[KIND_OF_BCH_DECODING_mSBS],			FLAG_MASK_CODE_BCH_DEC_AL_TEST_THIS_ALGORITHM,	VALUE_TYPE_UNSIGNED_INT,	NULL},//if(global_list_flag_bch_sim_decodingMode[KIND_OF_BCH_DECODING_mSBS]&FLAG_MASK_CODE_BCH_DEC_AL_TEST_THIS_ALGORITHM)

	{NULL,0,STRUCT_END,0,0,NULL,0,VALUE_TYPE_NONE,NULL}
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static struct struct_cmdLineOption	bch_mSBS_algorithm_decoding_option[] =
{
	{"skip",	0,		OPT_FLAG,		0,	0,	&global_list_flag_bch_sim_decodingMode[KIND_OF_BCH_DECODING_mSBS],		FLAG_MASK_CODE_BCH_DEC_AL_SKIP,		VALUE_TYPE_UNSIGNED_INT,	NULL},

	{NULL,0,STRUCT_END,0,0,NULL,0,VALUE_TYPE_NONE,NULL}
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static struct struct_cmdLineOption	bch_mSBS_algorithm_debug_calculate_option[] =
{
	{"delta",	0,		OPT_FLAG,		0,	0,	&global_flag_debug_mSBS_Sim,		FLAG_MASK_DEBUG_BCH_M_SBS_CAL_DELTA,			VALUE_TYPE_UNSIGNED_INT,	NULL},

	{NULL,0,STRUCT_END,0,0,NULL,0,VALUE_TYPE_NONE,NULL}
};

	#ifndef RELEASE
	static struct struct_cmdLineOption	bch_mSBS_algorithm_debug_option[] =
	{
		{"sequence",	0,	OPT_FLAG,		0,	0,	&global_flag_debug_mSBS_Sim,	FLAG_MASK_DEBUG_BCH_mSBS_ALGORITHM,	VALUE_TYPE_UNSIGNED_INT,	NULL},
		{"cal",			0,	OPT_COMPONENT,	0,	0,	NULL,						0,									VALUE_TYPE_NONE,			bch_mSBS_algorithm_debug_calculate_option},

		{NULL,0,STRUCT_END,0,0,NULL,0,VALUE_TYPE_NONE,NULL}
	};
	#endif
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static struct struct_cmdLineOption bch_mSBS_algorithm_option[] =
{
	{"used",		0,	OPT_FLAG,		0,	0,	&global_flag_bchDecAlgo_Enable,										FLAG_MASK_BCH_DECORDING_ALGORITHM_M_SBS,		VALUE_TYPE_UNSIGNED_INT,	NULL},

	{"algorithm",	0,	OPT_COMPONENT,	0,	0,	&global_list_flag_bch_sim_decodingMode[KIND_OF_BCH_DECODING_mSBS],	FLAG_MASK_CODE_BCH_DEC_AL_TEST_THIS_ALGORITHM,	VALUE_TYPE_UNSIGNED_INT,	bch_mSBS_algorithm_test_option},
	#ifndef RELEASE
	{"display_HD",		0,	OPT_COMPONENT,	0,	0,	NULL,																0,												VALUE_TYPE_NONE,			bch_mSBS_algorithm_HD_display_option},
	{"display_SD",		0,	OPT_COMPONENT,	0,	0,	NULL,																0,												VALUE_TYPE_NONE,			bch_mSBS_algorithm_SD_display_option},
	#endif
		
	{"hard_decision",	0,	OPT_FLAG,	0,	0,	&global_list_flag_bch_sim_decodingMode[KIND_OF_BCH_DECODING_mSBS],	FLAG_MASK_CODE_BCH_DEC_AL_HARD_DECISION,		VALUE_TYPE_UNSIGNED_INT,	NULL},//if(global_list_flag_bch_sim_decodingMode[KIND_OF_BCH_DECODING_BM]&FLAG_MASK_CODE_BCH_DEC_AL_HARD_DECISION)
	{"soft_decision",	0,	OPT_FLAG,	0,	0,	&global_list_flag_bch_sim_decodingMode[KIND_OF_BCH_DECODING_mSBS],	FLAG_MASK_CODE_BCH_DEC_AL_SOFT_DECISION,		VALUE_TYPE_UNSIGNED_INT,	NULL},//if(global_list_flag_bch_sim_decodingMode[KIND_OF_BCH_DECODING_BM]&FLAG_MASK_CODE_BCH_DEC_AL_SOFT_DECISION)
	
	{"decoding",	0,	OPT_COMPONENT,	0,	0,	NULL,																0,												VALUE_TYPE_NONE,			bch_mSBS_algorithm_decoding_option},

	#ifndef RELEASE
	{"debug",		0,	OPT_DEBUG,		0,	0,	NULL,																0,												VALUE_TYPE_NONE,			bch_mSBS_algorithm_debug_option},
	#endif
	
	{NULL,0,STRUCT_END,0,0,NULL,0,VALUE_TYPE_NONE,NULL}
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



/******************************* bch_mSBS_algorithm_option *******************************/
/******************************* bch_mSBS_algorithm_option *******************************/
/******************************* bch_mSBS_algorithm_option *******************************/






////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/************************************ bch_algorithm 1 s t - s t a r t ************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/***** static struct struct_cmdLineOption struct_cmdLineOption[] start *****/
static struct struct_cmdLineOption bch_common[] =
{
	{"oversynd",		0,	OPT_SET_VALUE,	0,	0,	&global_bch_Soft_OverSyndLen,	0,	VALUE_TYPE_UNSIGNED_INT,	NULL},
	{"addlcm",			0,	OPT_SET_VALUE,	0,	0,	&global_addedRootToLCM_string,			0,	VALUE_TYPE_STRING,			NULL},
	
	

	{NULL,0,STRUCT_END,0,0,NULL,0,VALUE_TYPE_NONE,NULL}
};

static struct struct_cmdLineOption bch_algorithm[] =
{
	{"used",		0,	OPT_FLAG,				0,	0,	&global_flag_simulationCode,	FLAG_MASK_CODE_BCH,		VALUE_TYPE_UNSIGNED_INT,	NULL},
	{"common",	0,		OPT_COMPONENT,			0,	0,	NULL,							0,						VALUE_TYPE_NONE,			bch_common},
	
	{"bm",			0,	OPT_COMPONENT,			0,	0,	NULL,							0,						VALUE_TYPE_NONE,			bch_BM_algorithm_option},
	{"msbs_t3",			0,	OPT_COMPONENT,		0,	0,	NULL,							0,						VALUE_TYPE_NONE,			bch_mSBS_algorithm_option},
	{NULL,0,STRUCT_END,0,0,NULL,0,VALUE_TYPE_NONE,NULL}
};
/***** static struct struct_cmdLineOption struct_cmdLineOption[] end *****/
/************************************ bch_algorithm 1 s t - e n d ************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	#ifndef RELEASE
	static struct struct_cmdLineOption	usable_code_GaloisField_display_option[] =
	{
		{"primepoly",		0,	OPT_FLAG,	0,	0,	&global_flag_gfCommon_display,	FLAG_MASK_DISPLAY_GF_COMMON_PRIMEPOLY,		VALUE_TYPE_UNSIGNED_INT,	NULL},	
		{"field",			0,	OPT_FLAG,	0,	0,	&global_flag_gfCommon_display,	FLAG_MASK_DISPLAY_GF_COMMON_FIELD,			VALUE_TYPE_UNSIGNED_INT,	NULL},
		{"conjugacies",		0,	OPT_FLAG,	0,	0,	&global_flag_gfCommon_display,	FLAG_MASK_DISPLAY_GF_COMMON_CONJUGACIES,	VALUE_TYPE_UNSIGNED_INT,	NULL},
		{"genpoly",			0,	OPT_FLAG,	0,	0,	&global_flag_gfCommon_display,	FLAG_MASK_DISPLAY_GF_COMMON_GENPOLY,		VALUE_TYPE_UNSIGNED_INT,	NULL},	
		{"sum_matrix",		0,	OPT_FLAG,	0,	0,	&global_flag_gfCommon_display,	FLAG_MASK_DISPLAY_GF_COMMON_SUM_MATRIX,		VALUE_TYPE_UNSIGNED_INT,	NULL},	
		
		{"fileio",			0,	OPT_FLAG,	0,	0,	&global_flag_gfCommon_display,	FLAG_MASK_DISPLAY_GF_COMMON_FILE_IO,		VALUE_TYPE_UNSIGNED_INT,	NULL},	
		{NULL,0,STRUCT_END,0,0,NULL,0,VALUE_TYPE_NONE,NULL}
	};
	#endif

static struct struct_cmdLineOption usable_code_GaloisField[] =
{
	{"exp",				0,	OPT_SET_VALUE,	0,	0,	&global_GaloisFieldExponential,		0,								VALUE_TYPE_UNSIGNED_INT,	NULL},
	{"codelength",		0,	OPT_SET_VALUE,	0,	0,	&global_CodeLength,					0,								VALUE_TYPE_UNSIGNED_INT,	NULL},
	{"limitcodelength",	0,	OPT_SET_VALUE,	0,	0,	&global_LimitCodeLength,			0,								VALUE_TYPE_UNSIGNED_INT,	NULL},
	
	
	{"makeall",		0,	OPT_FLAG,		0,	0,	&global_flag_gfCommon,						FLAG_MASK_GF_COMMON_MAKE_GF_ALL,		VALUE_TYPE_UNSIGNED_INT,	NULL},
		#ifndef RELEASE
	{"display",		0,	OPT_COMPONENT,	0,	0,	NULL,								0,								VALUE_TYPE_NONE,			usable_code_GaloisField_display_option},
		#endif
	{NULL,0,STRUCT_END,0,0,NULL,0,VALUE_TYPE_NONE,NULL}
};



static struct struct_cmdLineOption usaable_code_com_algorithm_option[] =
{
	{"errnum",	0,	OPT_SET_VALUE,	0,	0,	&global_ErrorNum,	0,					VALUE_TYPE_UNSIGNED_INT,	NULL},

	{NULL,0,STRUCT_END,0,0,NULL,0,VALUE_TYPE_NONE,NULL}
};

static struct struct_cmdLineOption	usaable_code_com_HD_option[] =
{
	{"correctable",	0,	OPT_SET_VALUE,	0,	0,	&global_bch_hardCorrectable,	0,	VALUE_TYPE_UNSIGNED_INT,	NULL},
		#ifndef RELEASE
		{"display",		0,	OPT_COMPONENT,	0,	0,	NULL,						0,	VALUE_TYPE_NONE,			bch_BM_algorithm_HD_display_option},
		#endif
	{NULL,0,STRUCT_END,0,0,NULL,0,VALUE_TYPE_NONE,NULL}
};

static struct struct_cmdLineOption usaable_code_com_HD_SD_option [] = 
{
	{"hard",	0,	OPT_SET_VALUES_SERIES,	0,	0,	&global_bch_hardCorrectable,	0,	VALUE_TYPE_UNSIGNED_INT,	NULL},
	{"soft",	0,	OPT_SET_VALUES_SERIES,	0,	0,	&global_bch_SoftCorrectable,	0,	VALUE_TYPE_UNSIGNED_INT,	NULL},
	{NULL,0,STRUCT_END,0,0,NULL,0,VALUE_TYPE_NONE,NULL}
};


static struct struct_cmdLineOption	usaable_code_com_SD_option[] =
{
	{"correctable",		0,	OPT_SET_VALUE,	0,	0,	&global_bch_SoftCorrectable,	0,	VALUE_TYPE_UNSIGNED_INT,	NULL},
	{"correctables",	0,	OPT_SET_VALUES,	0,	0,	NULL,							0,	VALUE_TYPE_NONE,			usaable_code_com_HD_SD_option},
	
		#ifndef RELEASE
		{"display",			0,	OPT_COMPONENT,	0,	0,	NULL,						0,	VALUE_TYPE_NONE,			bch_BM_algorithm_SD_display_option},
		#endif
	{NULL,0,STRUCT_END,0,0,NULL,0,VALUE_TYPE_NONE,NULL}
};

static struct struct_cmdLineOption usable_code_common[] =
{
	{"gf",		0,	OPT_COMPONENT,		0,	0,	NULL,							0,						    					VALUE_TYPE_NONE,			usable_code_GaloisField},
	
	//added correctables
	{"algorithm",	0,	OPT_COMPONENT,	0,	0,	&global_flag_sim_decodingMode,	FLAG_MASK_CODE_BCH_DEC_AL_TEST_THIS_ALGORITHM,	VALUE_TYPE_UNSIGNED_INT,	usaable_code_com_algorithm_option},
	
	{"hard",		0,	OPT_COMPONENT,	0,	0,	&global_flag_sim_decodingMode,	FLAG_MASK_CODE_BCH_DEC_AL_HARD_DECISION,		VALUE_TYPE_UNSIGNED_INT,	usaable_code_com_HD_option},//if(global_list_flag_bch_sim_decodingMode[KIND_OF_BCH_DECODING_BM]&FLAG_MASK_CODE_BCH_DEC_AL_HARD_DECISION)
	{"soft",		0,	OPT_COMPONENT,	0,	0,	&global_flag_sim_decodingMode,	FLAG_MASK_CODE_BCH_DEC_AL_SOFT_DECISION,		VALUE_TYPE_UNSIGNED_INT,	usaable_code_com_SD_option},//if(global_list_flag_bch_sim_decodingMode[KIND_OF_BCH_DECODING_BM]&FLAG_MASK_CODE_BCH_DEC_AL_SOFT_DECISION)

	{NULL,0,STRUCT_END,0,0,NULL,0,VALUE_TYPE_NONE,NULL}
};

static struct struct_cmdLineOption cmdOpt_usable_code[] =
{
	{"bch",		0,		OPT_COMPONENT,	0,	0,	&global_flag_simulationCode,	FLAG_MASK_CODE_BCH,		VALUE_TYPE_UNSIGNED_INT,	bch_algorithm},

	{"common",	0,		OPT_COMPONENT,	0,	0,	NULL,							0,						VALUE_TYPE_NONE,			usable_code_common},

	{NULL,0,STRUCT_END,0,0,NULL,0,VALUE_TYPE_NONE,NULL}
};

/*BCH Command Tree end*/
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static struct struct_cmdLineOption comm_channel_awgn_ebn0_range[] =
{
	{"start",	0,	OPT_SET_LANGE_SERIES,	0,	0,	&global_EbN0_Start_Value,	0,	VALUE_TYPE_DOUBLE,	NULL},
	{"end",		0,	OPT_SET_LANGE_SERIES,	0,	0,	&global_EbN0_End_Value,		0,	VALUE_TYPE_DOUBLE,	NULL},
	{"step",	0,	OPT_SET_LANGE_SERIES,	0,	0,	&global_EbN0_Step,			0,	VALUE_TYPE_DOUBLE,	NULL},
	{"(not complete analysys argument of range, in partial processing)",	0,	OPT_NOTHING,	0,	0,	NULL,	0,	VALUE_TYPE_NONE,	NULL},	
	{NULL,0,STRUCT_END,0,0,NULL,0,VALUE_TYPE_NONE,NULL}
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static struct struct_cmdLineOption comm_channel_awgn_ebn0[] =
{
	{"range",	0,	OPT_SET_LANGE,	0,	0,	NULL,						0,	VALUE_TYPE_NONE,	comm_channel_awgn_ebn0_range},
	
	{"start",	0,	OPT_SET_VALUE,	0,	0,	&global_EbN0_Start_Value,	0,	VALUE_TYPE_DOUBLE,	NULL},
	{"end",		0,	OPT_SET_VALUE,	0,	0,	&global_EbN0_End_Value,		0,	VALUE_TYPE_DOUBLE,	NULL},
	{"step",	0,	OPT_SET_VALUE,	0,	0,	&global_EbN0_Step,			0,	VALUE_TYPE_DOUBLE,	NULL},
	{NULL,0,STRUCT_END,0,0,NULL,0,VALUE_TYPE_NONE,NULL}
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	#ifndef RELEASE
	static struct struct_cmdLineOption comm_channel_awgn_llr_debug[] =
	{
		{"sequence",	0,		OPT_FLAG,		0,	0,	&global_flag_debug_awgnLLR,			FLAG_MASK_DEBUG_AWGN_LLR_SEQUENCE,	VALUE_TYPE_UNSIGNED_INT,	NULL},
		{"locator",		0,		OPT_FLAG,		0,	0,	&global_flag_debug_awgnLLR,			FLAG_MASK_DEBUG_AWGN_LLR_LOCATOR,	VALUE_TYPE_UNSIGNED_INT,	NULL},
		{NULL,0,STRUCT_END,0,0,NULL,0,VALUE_TYPE_NONE,NULL}
	};
	#endif
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static struct struct_cmdLineOption comm_ch_awgn_same_llr_mag_handling[] =
{
	{"none",    0,	OPT_FLAG_CASE,	0,	0,	&global_flag_case_same_llr_handling,	FLAG_CASE_SAME_LLR_HANDLING_NONE,	    VALUE_TYPE_UNSIGNED_INT,	NULL},		
	{"not_sel", 0,	OPT_FLAG_CASE,	0,	0,	&global_flag_case_same_llr_handling,	FLAG_CASE_SAME_LLR_HANDLING_NOT_SEL,	VALUE_TYPE_UNSIGNED_INT,	NULL},		
	{"prior_0",	0,	OPT_FLAG_CASE,	0,	0,	&global_flag_case_same_llr_handling,	FLAG_CASE_SAME_LLR_HANDLING_PRIOR_HD_0,	VALUE_TYPE_UNSIGNED_INT,	NULL},	
	{"prior_1",	0,	OPT_FLAG_CASE,	0,	0,	&global_flag_case_same_llr_handling,	FLAG_CASE_SAME_LLR_HANDLING_PRIOR_HD_1,	VALUE_TYPE_UNSIGNED_INT,	NULL},	

	{"(not sel option is not work on tree)",	0,	OPT_NOTHING,	0,	0,	NULL,	0,	VALUE_TYPE_NONE,	NULL},	
	{NULL,0,STRUCT_END,0,0,NULL,0,VALUE_TYPE_NONE,NULL}
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static struct struct_cmdLineOption comm_ch_awgn_init_llr_mag_method[] =
{
	{"none",            0,	OPT_FLAG_CASE,	0,	0,	&global_flag_case_init_llr_mag_method,	FLAG_CASE_INIT_LLR_MAG_METHOD_NONE,	            VALUE_TYPE_UNSIGNED_INT,	NULL},		
	{"not_sel_max_llr", 0,	OPT_FLAG_CASE,	0,	0,	&global_flag_case_init_llr_mag_method,	FLAG_CASE_INIT_LLR_MAG_METHOD_NOT_SEL_MAX_LLR,  VALUE_TYPE_UNSIGNED_INT,	NULL},		

	{"(not sel max option is not work on tree)",	0,	OPT_NOTHING,	0,	0,	NULL,	0,	VALUE_TYPE_NONE,	NULL},	
	{NULL,0,STRUCT_END,0,0,NULL,0,VALUE_TYPE_NONE,NULL}
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static struct struct_cmdLineOption comm_ch_awgn_llr_mag_find_meth_grouping[] =
{
	{"enable",	    0,	OPT_FLAG_CASE,	0,	0,	&global_flag_case_find_LLR_method,	FLAG_CASE_FINDING_MIN_LLR_METHOD_GROUPING,	VALUE_TYPE_UNSIGNED_INT,	NULL},
    {"group_num",   0,	OPT_SET_VALUE,	0,	0,	&global_grouping_stream_nums,		0,	                                        VALUE_TYPE_UNSIGNED_INT,	NULL},
	{NULL,0,STRUCT_END,0,0,NULL,0,VALUE_TYPE_NONE,NULL}
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static struct struct_cmdLineOption comm_ch_awgn_llr_mag_find_meth_tree[] =
{
	{"follow_min_path",	        0,	OPT_FLAG_CASE,	0,	0,	&global_flag_case_find_LLR_method,	FLAG_CASE_FINDING_MIN_LLR_METHOD_TREE_FOLLOWING_MIN1_PATH,	VALUE_TYPE_UNSIGNED_INT,	NULL},		
	{"probabilistic_choosen",	0,	OPT_FLAG_CASE,	0,	0,	&global_flag_case_find_LLR_method,	FLAG_CASE_FINDING_MIN_LLR_METHOD_TREE_PROBABILISIC_CHOOSEN,	VALUE_TYPE_UNSIGNED_INT,	NULL},	
	//{"prior_HD_zero",	        0,	OPT_FLAG_CASE,	0,	0,	&global_flag_case_find_LLR_method,	FLAG_CASE_FINDING_MIN_LLR_METHOD_TREE_PRIOR_HD_ZERO,	    VALUE_TYPE_UNSIGNED_INT,	NULL},	
	{"(not complete a option of probabilistic_choosen)",	0,	OPT_NOTHING,	0,	0,	NULL,	0,	VALUE_TYPE_NONE,	NULL},	
	{NULL,0,STRUCT_END,0,0,NULL,0,VALUE_TYPE_NONE,NULL}
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static struct struct_cmdLineOption comm_channel_awgn_llr_mag_finding_meth[] =
{
	{"pass_hd_1_dis",	    0,	OPT_FLAG_CASE,	0,	0,	&global_flag_case_pass_hd_1,	    FLAG_CASE_PASS_MAG0_HD_1_DISABLE,	            VALUE_TYPE_UNSIGNED_INT,    NULL},		
	{"pass_hd_1_en",	    0,	OPT_FLAG_CASE,	0,	0,	&global_flag_case_pass_hd_1,	    FLAG_CASE_PASS_MAG0_HD_1_ENABLE,                 VALUE_TYPE_UNSIGNED_INT,    NULL},		

	{"pri_same_LLR_mag",	0,	OPT_COMPONENT,	0,	0,	NULL,	                            0,	                                        VALUE_TYPE_NONE,            comm_ch_awgn_same_llr_mag_handling},		
	{"init_LLR_loc_val",	0,	OPT_COMPONENT,	0,	0,	NULL,	                            0,	                                        VALUE_TYPE_NONE,            comm_ch_awgn_init_llr_mag_method},		

	{"minimum",	            0,	OPT_FLAG_CASE,	0,	0,	&global_flag_case_find_LLR_method,	FLAG_CASE_FINDING_MIN_LLR_METHOD_MINIMUM,	VALUE_TYPE_UNSIGNED_INT,    NULL},		
	{"group_min_push_away",	0,	OPT_COMPONENT,	0,	0,	&global_flag_case_find_LLR_method,	FLAG_CASE_FINDING_MIN_LLR_METHOD_GROUPING,	VALUE_TYPE_NONE,            comm_ch_awgn_llr_mag_find_meth_grouping},	
	{"tree",	            0,	OPT_COMPONENT,	0,	0,	NULL,	                            0,                                          VALUE_TYPE_NONE,	        comm_ch_awgn_llr_mag_find_meth_tree},

	{"notice)pass_hd_1_is_not_tested_in_tree_structure",0,OPT_NOTHING,0,0,NULL,0,VALUE_TYPE_NONE,NULL},		
	{NULL,0,STRUCT_END,0,0,NULL,0,VALUE_TYPE_NONE,NULL}
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static struct struct_cmdLineOption comm_channel_llr_quantiz_filter_set[] =
{
	{"print_filtering_range",	0,	OPT_FLAG,	    0,	0,	&global_flag_llr_display,		        FLAG_LLR_DISPLAY_QUANTIZATION_FILTERING,	VALUE_TYPE_UNSIGNED_INT,	NULL},

    {"offset",		            0,	OPT_SET_VALUE,	0,	0,	&global_LLR_quantizFilter_offset,		0,	VALUE_TYPE_DOUBLE,			NULL},
	{"step",		            0,	OPT_SET_VALUE,	0,	0,	&global_LLR_quantizFilter_step,			0,	VALUE_TYPE_DOUBLE,			NULL},
	{"bitwidth",	            0,	OPT_SET_VALUE,	0,	0,	&global_LLR_quantizFilter_bitWidth,		0,	VALUE_TYPE_UNSIGNED_CHAR,	NULL},
		#ifndef RELEASE
		{"debug",	            0,	OPT_COMPONENT,	0,	0,	NULL,	0,	VALUE_TYPE_NONE,	comm_channel_awgn_llr_debug},
		#endif
	{NULL,0,STRUCT_END,0,0,NULL,0,VALUE_TYPE_NONE,NULL}
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static struct struct_cmdLineOption comm_channel_llr_quantiz_numberic_system[] =
{
	{"twos_complement",	        0,	OPT_FLAG_CASE,	0,	0,	&global_flag_case_llr_numberic_system,	ENUM_FLAG_CASE_QUANTIZ_NUMBERIC_SYSTEM_TWO_S_COMPLEMENT,	VALUE_TYPE_UNSIGNED_INT,	NULL},		
	{"ones_complement",	        0,	OPT_FLAG_CASE,	0,	0,	&global_flag_case_llr_numberic_system,	ENUM_FLAG_CASE_QUANTIZ_NUMBERIC_SYSTEM_ONE_S_COMPLEMENT,	VALUE_TYPE_UNSIGNED_INT,	NULL},		

    {NULL,0,STRUCT_END,0,0,NULL,0,VALUE_TYPE_NONE,NULL}
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static struct struct_cmdLineOption comm_channel_llr_quantiz_zero_symmetry[] =
{
//	{"disable", 0,	OPT_FLAG_CASE,	0,	0,	&global_flag_case_llr_mag_ones_complement_zero_handling,	ENUM_FLAG_CASE_QUANTIZ_NUMBERIC_SYS_ONE_S_COM_NONE, 	    VALUE_TYPE_UNSIGNED_INT,	NULL},		
	{"enbale",	0,	OPT_FLAG_CASE,	0,	0,	&global_flag_case_llr_mag_ones_complement_zero_handling,	ENUM_FLAG_CASE_QUANTIZ_NUMBERIC_SYS_ONE_S_COM_ZERO_SYMMETRY,	VALUE_TYPE_UNSIGNED_INT,	NULL},		

    {NULL,0,STRUCT_END,0,0,NULL,0,VALUE_TYPE_NONE,NULL}
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static struct struct_cmdLineOption comm_channel_llr_quantiz_zero_duplicated[] =
{
//	{"disable", 0,	OPT_FLAG_CASE,	0,	0,	&global_flag_case_llr_mag_ones_complement_zero_handling,	ENUM_FLAG_CASE_QUANTIZ_NUMBERIC_SYS_ONE_S_COM_NONE, 	    VALUE_TYPE_UNSIGNED_INT,	NULL},		
	{"enbale",	0,	OPT_FLAG_CASE,	0,	0,	&global_flag_case_llr_mag_ones_complement_zero_handling,	ENUM_FLAG_CASE_QUANTIZ_NUMBERIC_SYS_ONE_S_COM_ZERO_DUPLICATED,	VALUE_TYPE_UNSIGNED_INT,	NULL},		

    {NULL,0,STRUCT_END,0,0,NULL,0,VALUE_TYPE_NONE,NULL}
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static struct struct_cmdLineOption comm_channel_llr_quantization[] =
{
	{"filter_set",	            0,	OPT_COMPONENT,	0,	0,	NULL,	0,	VALUE_TYPE_NONE,	comm_channel_llr_quantiz_filter_set},

	{"numberic_system",	        0,	OPT_COMPONENT,	0,	0,	NULL,	0,	VALUE_TYPE_NONE,	comm_channel_llr_quantiz_numberic_system},	
	{"zero_symmetry",	        0,	OPT_COMPONENT,	0,	0,	NULL,	0,	VALUE_TYPE_NONE,	comm_channel_llr_quantiz_zero_symmetry},	
	{"zero_duplicated",	        0,	OPT_COMPONENT,	0,	0,	NULL,	0,	VALUE_TYPE_NONE,	comm_channel_llr_quantiz_zero_duplicated},	
	
	{NULL,0,STRUCT_END,0,0,NULL,0,VALUE_TYPE_NONE,NULL}
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static struct struct_cmdLineOption comm_channel_awgn_llr[] =
{
	{"quantization",            0,	OPT_COMPONENT,	0,	0,	NULL,	0,	VALUE_TYPE_NONE,	comm_channel_llr_quantization},	

	{"min_val_finding_meth",	0,	OPT_COMPONENT,	0,	0,	NULL,	0,	VALUE_TYPE_NONE,	comm_channel_awgn_llr_mag_finding_meth},	
	{NULL,0,STRUCT_END,0,0,NULL,0,VALUE_TYPE_NONE,NULL}
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	#ifndef RELEASE
	static struct struct_cmdLineOption comm_channel_awgn_debug[] =
	{
		{"sequence",	0,		OPT_FLAG,		0,	0,	&global_flag_debug_awgn,			FLAG_MASK_DEBUG_AWGN_SEQUENCE,				VALUE_TYPE_UNSIGNED_INT,	NULL},
		{NULL,0,STRUCT_END,0,0,NULL,0,VALUE_TYPE_NONE,NULL}
	};
	#endif
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static struct struct_cmdLineOption comm_channel_awgn[] =
{
	{"used",				0,	OPT_FLAG,		0,	0,	&global_Channel_Mode,			FLAG_MASK_CHANNEL_MODE_AWGN,						VALUE_TYPE_UNSIGNED_INT,	NULL},

	{"random_seed_disable",	0,	OPT_FLAG,		0,	0,	&global_flag_Channel_Mode_Func,	FLAG_MASK_CHANNEL_MODE_FUNC_RANDOM_SEED_DISABLE,	VALUE_TYPE_UNSIGNED_INT,	NULL},
	{"bitrate_disable",		0,	OPT_FLAG,		0,	0,	&global_flag_Channel_Mode_Func,	FLAG_MASK_CHANNEL_MODE_FUNC_BITRATE_DISABLE,		VALUE_TYPE_UNSIGNED_INT,	NULL},
	
	{"ebn0",				0,	OPT_COMPONENT,	0,	0,	NULL,							0,													VALUE_TYPE_NONE,			comm_channel_awgn_ebn0},
	{"llr",					0,	OPT_COMPONENT,	0,	0,	NULL,							0,													VALUE_TYPE_NONE,			comm_channel_awgn_llr},
	
		#ifndef RELEASE
		{"debug",	        0,	OPT_COMPONENT,	0,	0,	NULL,					        0,					                                0,					        comm_channel_awgn_debug},
		#endif
	{NULL,0,STRUCT_END,0,0,NULL,0,VALUE_TYPE_NONE,NULL}
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/************************************ cmdOpt_comm_channel 1 s t - s t a r t ************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static struct struct_cmdLineOption cmdOpt_comm_channel[] =
{
	{"awgn",	0,	OPT_COMPONENT,	0,	0,	&global_Channel_Mode,	FLAG_MASK_CHANNEL_MODE_AWGN,	VALUE_TYPE_UNSIGNED_INT,	comm_channel_awgn},
	{NULL,0,STRUCT_END,0,0,NULL,0,VALUE_TYPE_NONE,NULL}
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/************************************ cmdOpt_comm_channel 1 s t - e n d ************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	#ifndef RELEASE
	static struct struct_cmdLineOption bpsk_modulation_display[] =
	{
		{"result",		0,	OPT_FLAG,	0,	0,	&global_flag_Modulation_Mode_BPSK_Display,	FLAG_MASK_DISPLAY_RESULT,	VALUE_TYPE_UNSIGNED_INT,	NULL},
		{"progress",	0,	OPT_FLAG,	0,	0,	&global_flag_Modulation_Mode_BPSK_Display,	FLAG_MASK_DISPLAY_PROGRESS,	VALUE_TYPE_UNSIGNED_INT,	NULL},
		{"funcname",	0,	OPT_FLAG,	0,	0,	&global_flag_Modulation_Mode_BPSK_Display,	FLAG_MASK_DISPLAY_FUNCNAME,	VALUE_TYPE_UNSIGNED_INT,	NULL},

		{NULL,0,STRUCT_END,0,0,NULL,0,VALUE_TYPE_NONE,NULL}
	};
	#endif

static struct struct_cmdLineOption bpsk_modulation[] =
{
	{"used",	0,	OPT_FLAG,		0,	0,	&global_flag_Modulation_Mode,	DEFAULT_MODULATION_MODE,	VALUE_TYPE_UNSIGNED_INT,	NULL},

		#ifndef RELEASE
		{"display",	0,	OPT_COMPONENT,	0,	0,	NULL,							0,							VALUE_TYPE_NONE,			bpsk_modulation_display},
		#endif
	{NULL,0,STRUCT_END,0,0,NULL,0,VALUE_TYPE_NONE,NULL}
};
static struct struct_cmdLineOption cmdOpt_comm_modulation[] =
{
	{"bpsk",	0,	OPT_COMPONENT,	0,	0,	NULL,	0,	VALUE_TYPE_NONE,	bpsk_modulation},
	{NULL,0,STRUCT_END,0,0,NULL,0,VALUE_TYPE_NONE,NULL}
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static struct struct_cmdLineOption cmdOpt_sim_loop[] =
{
	// {"start",	0,	OPT_SET_VALUE,	0,	0,	&global_EbN0_Start_Value,	0,	VALUE_TYPE_DOUBLE,	NULL},

	{"min",		0,	OPT_SET_VALUE,	0,	0,	&global_minimum_loop,	0,	VALUE_TYPE_UNSIGNED_LONG,	NULL},
	{"base",	0,	OPT_SET_VALUE,	0,	0,	&global_base_of_loop,	0,	VALUE_TYPE_UNSIGNED_LONG,	NULL},
	{"rate",	0,	OPT_SET_VALUE,	0,	0,	&global_rate_of_loop,	0,	VALUE_TYPE_UNSIGNED_INT,	NULL},
	
	{NULL,0,STRUCT_END,0,0,NULL,0,VALUE_TYPE_NONE,NULL}
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static struct struct_cmdLineOption cmdOpt_simulation[] =
{
	// {"start",	0,	OPT_SET_VALUE,	0,	0,	&global_EbN0_Start_Value,	0,	VALUE_TYPE_DOUBLE,	NULL},

	{"loop",	0,	OPT_COMPONENT,	0,	0,	NULL,	0,	VALUE_TYPE_NONE,    cmdOpt_sim_loop},
	
	{NULL,0,STRUCT_END,0,0,NULL,0,VALUE_TYPE_NONE,NULL}
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	#ifndef RELEASE
	static struct struct_cmdLineOption ber_actual_display[] =
	{
		{"result",		0,	OPT_FLAG,	0,	0,	&global_flag_case_getBER_method_Display,	FLAG_MASK_DISPLAY_RESULT,	VALUE_TYPE_UNSIGNED_INT,	NULL},
		{"progress",	0,	OPT_FLAG,	0,	0,	&global_flag_case_getBER_method_Display,	FLAG_MASK_DISPLAY_PROGRESS,	VALUE_TYPE_UNSIGNED_INT,	NULL},
		{"funcname",	0,	OPT_FLAG,	0,	0,	&global_flag_case_getBER_method_Display,	FLAG_MASK_DISPLAY_FUNCNAME,	VALUE_TYPE_UNSIGNED_INT,	NULL},

		{NULL,0,STRUCT_END,0,0,NULL,0,VALUE_TYPE_NONE,NULL}
	};
	#endif

static struct struct_cmdLineOption ber_theo_decision[] =
{
	// {"(not-support)",	0,	OPT_NOTHING,	0,	0,	NULL,						0,						VALUE_TYPE_NONE,			NULL},	
	{"allzero",	0,	OPT_FLAG_CASE,	0,	0,	&global_flag_case_choosingCodeWordMethod_inTheo,	FLAG_CASE_CODEWORD_CHOOSEN_METHOD_IN_THEO_NO_ERR,	VALUE_TYPE_UNSIGNED_INT,	NULL},
	{"normal",	0,	OPT_FLAG_CASE,	0,	0,	&global_flag_case_choosingCodeWordMethod_inTheo,	FLAG_CASE_CODEWORD_CHOOSEN_METHOD_IN_THEO_NORMAL,	VALUE_TYPE_UNSIGNED_INT,	NULL},
	{"best",	0,	OPT_FLAG_CASE,	0,	0,	&global_flag_case_choosingCodeWordMethod_inTheo,	FLAG_CASE_CODEWORD_CHOOSEN_METHOD_IN_THEO_MIN_ERR,		VALUE_TYPE_UNSIGNED_INT,	NULL},
	{"worst",	0,	OPT_FLAG_CASE,	0,	0,	&global_flag_case_choosingCodeWordMethod_inTheo,	FLAG_CASE_CODEWORD_CHOOSEN_METHOD_IN_THEO_MAX_ERR,		VALUE_TYPE_UNSIGNED_INT,	NULL},

	{NULL,0,STRUCT_END,0,0,NULL,0,VALUE_TYPE_NONE,NULL}
};



static struct struct_cmdLineOption ber_dec_decision_handing[] =
{
	{"none",			0,	OPT_FLAG_CASE,	0,	0,	&global_flag_case_dec_fail_handling,	FLAG_CASE_DEC_FAIL_HANDLING_NONE,						VALUE_TYPE_UNSIGNED_INT,	NULL},
	{"retrans_no_err",	0,	OPT_FLAG_CASE,	0,	0,	&global_flag_case_dec_fail_handling,	FLAG_CASE_DEC_FAIL_HANDLING_RE_TRANFER_FAIL_NO_MORE,	VALUE_TYPE_UNSIGNED_INT,	NULL},
	{"retrans",			0,	OPT_FLAG_CASE,	0,	0,	&global_flag_case_dec_fail_handling,	FLAG_CASE_DEC_FAIL_HANDLING_RE_TRANFER_INTERATION,		VALUE_TYPE_UNSIGNED_INT,	NULL},

	{NULL,0,STRUCT_END,0,0,NULL,0,VALUE_TYPE_NONE,NULL}
};
	
static struct struct_cmdLineOption ber_dec_decision[] =
{
	// {"(not-support)",	0,	OPT_NOTHING,	0,	0,	NULL,						0,						VALUE_TYPE_NONE,			NULL},
	{"used",				0,	OPT_FLAG_CASE,		0,	0,	&global_flag_case_selTP_before_dec,	FLAG_CASE_SEL_TP_BEFORE_DEC_ACT,	VALUE_TYPE_UNSIGNED_INT,	NULL},	

	{"handling",			0,	OPT_COMPONENT,		0,	0,	&global_flag_case_selTP_before_dec,	FLAG_CASE_SEL_TP_BEFORE_DEC_ACT,	VALUE_TYPE_UNSIGNED_INT,	ber_dec_decision_handing},	

		#ifndef RELEASE
		{"display",	0,	OPT_COMPONENT,	0,	0,	NULL,							0,							VALUE_TYPE_NONE,			ber_actual_display},
		#endif
	
	{NULL,0,STRUCT_END,0,0,NULL,0,VALUE_TYPE_NONE,NULL}
};

static struct struct_cmdLineOption ber_options[] =
{
	// {"(not-support)",	0,	OPT_NOTHING,	0,	0,	NULL,						0,						VALUE_TYPE_NONE,			NULL},
	{"theoretical_decision",		0,	OPT_COMPONENT,	0,	0,	NULL,								0,									VALUE_TYPE_NONE,			ber_theo_decision},

	{"decoder_decision",		0,	OPT_COMPONENT,		0,	0,	&global_flag_case_selTP_before_dec,	FLAG_CASE_SEL_TP_BEFORE_DEC_ACT,	VALUE_TYPE_UNSIGNED_INT,	ber_dec_decision},
	{NULL,0,STRUCT_END,0,0,NULL,0,VALUE_TYPE_NONE,NULL}
};
static struct struct_cmdLineOption cmdOpt_get_thing[] =
{
	// {"(not-support)",	0,	OPT_NOTHING,	0,	0,	NULL,						0,						VALUE_TYPE_NONE,			NULL},

	// {"ber",				0,	OPT_FLAG,		0,	0,	&global_flag_berOptions,	FLAG_MASK_TEST_GET_BER,	VALUE_TYPE_UNSIGNED_INT,	NULL},
	{"ber",				0,	OPT_COMPONENT,		0,	0,	&global_flag_berOptions,	FLAG_MASK_TEST_GET_BER,	VALUE_TYPE_UNSIGNED_INT,	ber_options},
	{NULL,0,STRUCT_END,0,0,NULL,0,VALUE_TYPE_NONE,NULL}
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static struct struct_cmdLineOption pattern_io[] =
{
	{"infomation",              0,		OPT_FLAG,	0,	0,	&global_flag_file_io_hd_pattern_output, FLAG_MASK_FILE_IO_HD_MES,               VALUE_TYPE_UNSIGNED_INT,	NULL},
	{"encoding",	            0,		OPT_FLAG,	0,	0,	&global_flag_file_io_hd_pattern_output,	FLAG_MASK_FILE_IO_HD_ENCODING_MES,	    VALUE_TYPE_UNSIGNED_INT,	NULL},
	{"received",	            0,		OPT_FLAG,	0,	0,	&global_flag_file_io_hd_pattern_output,	FLAG_MASK_FILE_IO_HD_RECEIVED_MES,	    VALUE_TYPE_UNSIGNED_INT,	NULL},
	{"corrected",	            0,		OPT_FLAG,	0,	0,	&global_flag_file_io_hd_pattern_output,	FLAG_MASK_FILE_IO_HD_CORRECTED_MES,	    VALUE_TYPE_UNSIGNED_INT,	NULL},

    {"quantized_LLR",           0,		OPT_FLAG,	0,	0,	&global_flag_file_io_sd_pattern_output,	FLAG_MASK_FILE_IO_SD_RECEIVED_LLR,      VALUE_TYPE_UNSIGNED_INT,	NULL},
    {"quantized_LLR_magnitude", 0,		OPT_FLAG,	0,	0,	&global_flag_file_io_sd_pattern_output,	FLAG_MASK_FILE_IO_SD_RECEIVED_LLR_mag,  VALUE_TYPE_UNSIGNED_INT,	NULL},

    {"all",                     0,		OPT_FLAG,	0,	0,	&global_flag_file_io_sd_pattern_output,	-1,                                     VALUE_TYPE_UNSIGNED_INT,	NULL},
	{NULL,0,STRUCT_END,0,0,NULL,0,VALUE_TYPE_NONE,NULL}
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static struct struct_cmdLineOption pattern_io_log[] =
{
	{"infomation",              0,		OPT_FLAG,	0,	0,	&global_flag_file_io_hd_pattern_output_log, FLAG_MASK_FILE_IO_HD_MES,               VALUE_TYPE_UNSIGNED_INT,	NULL},
	{"encoding",	            0,		OPT_FLAG,	0,	0,	&global_flag_file_io_hd_pattern_output_log,	FLAG_MASK_FILE_IO_HD_ENCODING_MES,	    VALUE_TYPE_UNSIGNED_INT,	NULL},
	{"received",	            0,		OPT_FLAG,	0,	0,	&global_flag_file_io_hd_pattern_output_log,	FLAG_MASK_FILE_IO_HD_RECEIVED_MES,  	VALUE_TYPE_UNSIGNED_INT,	NULL},
	{"corrected",	            0,		OPT_FLAG,	0,	0,	&global_flag_file_io_hd_pattern_output_log,	FLAG_MASK_FILE_IO_HD_CORRECTED_MES,	    VALUE_TYPE_UNSIGNED_INT,	NULL},

    {"quantized_LLR",           0,		OPT_FLAG,	0,	0,	&global_flag_file_io_sd_pattern_output_log,	FLAG_MASK_FILE_IO_SD_RECEIVED_LLR,      VALUE_TYPE_UNSIGNED_INT,	NULL},
    {"quantized_LLR_magnitude", 0,		OPT_FLAG,	0,	0,	&global_flag_file_io_sd_pattern_output_log,	FLAG_MASK_FILE_IO_SD_RECEIVED_LLR_mag,  VALUE_TYPE_UNSIGNED_INT,	NULL},

    {"all",                     0,		OPT_FLAG,	0,	0,	&global_flag_file_io_sd_pattern_output_log,	-1,                                     VALUE_TYPE_UNSIGNED_INT,	NULL},
	{NULL,0,STRUCT_END,0,0,NULL,0,VALUE_TYPE_NONE,NULL}
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static struct struct_cmdLineOption pattern_io_verilog[] =
{
	{"infomation",              0,		OPT_FLAG,	0,	0,	&global_flag_file_io_hd_pattern_output_verilog, FLAG_MASK_FILE_IO_HD_MES,               VALUE_TYPE_UNSIGNED_INT,	NULL},
	{"encoding",	            0,		OPT_FLAG,	0,	0,	&global_flag_file_io_hd_pattern_output_verilog, FLAG_MASK_FILE_IO_HD_ENCODING_MES,	    VALUE_TYPE_UNSIGNED_INT,	NULL},
	{"received",	            0,		OPT_FLAG,	0,	0,	&global_flag_file_io_hd_pattern_output_verilog, FLAG_MASK_FILE_IO_HD_RECEIVED_MES,	    VALUE_TYPE_UNSIGNED_INT,	NULL},
	{"corrected",	            0,		OPT_FLAG,	0,	0,	&global_flag_file_io_hd_pattern_output_verilog, FLAG_MASK_FILE_IO_HD_CORRECTED_MES,	    VALUE_TYPE_UNSIGNED_INT,	NULL},

    {"quantized_LLR",           0,		OPT_FLAG,	0,	0,	&global_flag_file_io_sd_pattern_output_verilog,	FLAG_MASK_FILE_IO_SD_RECEIVED_LLR,      VALUE_TYPE_UNSIGNED_INT,	NULL},
    {"quantized_LLR_magnitude", 0,		OPT_FLAG,	0,	0,	&global_flag_file_io_sd_pattern_output_verilog,	FLAG_MASK_FILE_IO_SD_RECEIVED_LLR_mag,  VALUE_TYPE_UNSIGNED_INT,	NULL},

    {"all",                     0,		OPT_FLAG,	0,	0,	&global_flag_file_io_sd_pattern_output_verilog,	-1,                                     VALUE_TYPE_UNSIGNED_INT,	NULL},
	{NULL,0,STRUCT_END,0,0,NULL,0,VALUE_TYPE_NONE,NULL}
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static struct struct_cmdLineOption cmdOpt_fileIO[] =
{
	{"testFlag",		0,		OPT_FLAG,	    0,	0,	&global_flag_fileInputOutput,	FLAG_MASK_FILE_IO,	VALUE_TYPE_UNSIGNED_INT,	NULL},
	{"pattern",         0,		OPT_COMPONENT,	0,	0,	NULL,	                        0,      		    VALUE_TYPE_NONE,        	pattern_io},
	{"pattern_log",     0,		OPT_COMPONENT,	0,	0,	NULL,	                        0,      		    VALUE_TYPE_NONE,        	pattern_io_log},
	{"pattern_verilog", 0,		OPT_COMPONENT,	0,	0,	NULL,	                        0,      		    VALUE_TYPE_NONE,        	pattern_io_verilog},
	{NULL,0,STRUCT_END,0,0,NULL,0,VALUE_TYPE_NONE,NULL}
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static struct struct_cmdLineOption cmdOpt_testMode_err_distri_llr_static_msg[] =
{
	{"zero_stream_ratio",		0,	OPT_SET_VALUE,	0,	0,	&tmp_testMode_loops_zero_bit_ratio,	0,	VALUE_TYPE_DOUBLE_RATIO_LIMIT,	NULL},

	{NULL,0,STRUCT_END,0,0,NULL,0,VALUE_TYPE_NONE,NULL}
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static struct struct_cmdLineOption cmdOpt_testMode_err_distri_llr_msg_gen_meth[] =
{
	{"static",  0,	OPT_FLAG_CASE,	0,	0,	&global_flag_case_testMode_msg_gen_method,   ENUM_CASE_TESTMODE_ERR_RATE_MSG_GEN_METH_STATIC,    VALUE_TYPE_UNSIGNED_INT,    cmdOpt_testMode_err_distri_llr_static_msg},
	{"rand",    0,	OPT_FLAG_CASE,	0,	0,	&global_flag_case_testMode_msg_gen_method,   ENUM_CASE_TESTMODE_ERR_RATE_MSG_GEN_METH_RAND,      VALUE_TYPE_UNSIGNED_INT,    NULL},
	{NULL,0,STRUCT_END,0,0,NULL,0,VALUE_TYPE_NONE,NULL}
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static struct struct_cmdLineOption cmdOpt_testMode_err_distri_decoding_meth[] =
{
	{"hard",  0,	OPT_FLAG_CASE,	0,	0,	&global_flag_case_testMode_decoding_method,   ENUM_CASE_TEST_MODE_ERR_RATE_DECODING_METH_HD,    VALUE_TYPE_UNSIGNED_INT,    NULL},
	{"soft",  0,	OPT_FLAG_CASE,	0,	0,	&global_flag_case_testMode_decoding_method,   ENUM_CASE_TEST_MODE_ERR_RATE_DECODING_METH_SD,    VALUE_TYPE_UNSIGNED_INT,    NULL},
	{NULL,0,STRUCT_END,0,0,NULL,0,VALUE_TYPE_NONE,NULL}
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static struct struct_cmdLineOption cmdOpt_testMode_err_distribution[] =
{

	{"msg_gen_method",  0,	OPT_COMPONENT,	0,	0,	NULL,                   0,  VALUE_TYPE_NONE,            cmdOpt_testMode_err_distri_llr_msg_gen_meth},
	{"decoding_method", 0,	OPT_COMPONENT,	0,	0,	NULL,                   0,  VALUE_TYPE_NONE,            cmdOpt_testMode_err_distri_decoding_meth},
	{"tot_loops",		0,	OPT_SET_VALUE,	0,	0,	&tmp_testMode_loops,	0,	VALUE_TYPE_UNSIGNED_LONG,	NULL},
	{NULL,0,STRUCT_END,0,0,NULL,0,VALUE_TYPE_NONE,NULL}
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static struct struct_cmdLineOption cmdOpt_testMode_prob_distribution[] =
{

	{"group_num",		0,	OPT_SET_VALUE,	0,	0,	&tmp_testMode_prob_distribution_group_nums, 0,	VALUE_TYPE_UNSIGNED_LONG,	NULL},

	{"politics_same_val", 0,	OPT_FLAG_CASE,	0,	0,	&tmp_testMode_prob_min_choosen_politics,   ENUM_CASE_TEST_MODE_PROB_MIN_CHOOSE_SAME_VAL,    VALUE_TYPE_UNSIGNED_INT,    NULL},
	{"politics_low_val", 0,		OPT_FLAG_CASE,	0,	0,	&tmp_testMode_prob_min_choosen_politics,   ENUM_CASE_TEST_MODE_PROB_MIN_CHOOSE_LOW_VAL,     VALUE_TYPE_UNSIGNED_INT,    NULL},

	{"tot_loops",		0,	OPT_SET_VALUE,	0,	0,	&tmp_testMode_loops,	0,	VALUE_TYPE_UNSIGNED_LONG,	NULL},
	{NULL,0,STRUCT_END,0,0,NULL,0,VALUE_TYPE_NONE,NULL}
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static struct struct_cmdLineOption cmdOpt_testMode[] =
{

	{"errDistribution_EbN0", 0,		OPT_FLAG_CASE,	0,	0,	&global_flag_case_sim_testOpt,   FLAG_CASE_SIM_TEST_MODE_GET_LLR_MAG_AVR_BELONG_EBN0,       VALUE_TYPE_UNSIGNED_INT,    cmdOpt_testMode_err_distribution},

	{"prob_distribution",   0,		OPT_FLAG_CASE,	0,	0,	&global_flag_case_sim_testOpt,   FLAG_CASE_SIM_TEST_MODE_GET_PROBABILITY_OF_MINIMUM_VAL,    VALUE_TYPE_UNSIGNED_INT,    cmdOpt_testMode_prob_distribution},
    
	{"show_loop_cnt",       0,		OPT_FLAG_CASE,	0,	0,	&global_flag_case_sim_testOpt,   FLAG_CASE_SIM_TEST_MODE_SHOW_LOOP_CNT,                     VALUE_TYPE_UNSIGNED_INT,    NULL},

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
		{"log",			'l',	OPT_FLAG,	0,	0,	&global_flag_cmdOption,			FLAG_MASK_PRINTF_LOG,					VALUE_TYPE_UNSIGNED_INT,	NULL},
		{"calc",		'c',	OPT_FLAG,	0,	0,	&global_flag_cmdOption,			FLAG_MASK_PRINTF_CALCULATE_PROCESS,		VALUE_TYPE_UNSIGNED_INT,	NULL},
		{"memchk",		'm',	OPT_FLAG,	0,	0,	&global_flag_cmdOption,			FLAG_MASK_PRINTF_MEMORY_CHECK_PROCESS,	VALUE_TYPE_UNSIGNED_INT,	NULL},	
		{"warning",		'w',	OPT_FLAG,	0,	0,	&global_flag_cmdOption,			FLAG_MASK_PRINTF_WARNING_MES,			VALUE_TYPE_UNSIGNED_INT,	NULL},
		#endif

		#ifndef RELEASE
		{"debug",		'd',	OPT_FLAG,	0,	0,	&global_flag_debug_display,		FLAG_MASK_DEBUG_DEBUG,					VALUE_TYPE_UNSIGNED_INT,	NULL},
		{"debug_d_func",	0,	OPT_FLAG,	0,	0,	&global_flag_debug_display,		FLAG_MASK_DISPLAY_FUNCNAME,				VALUE_TYPE_UNSIGNED_INT,	NULL},
		#endif
	
	{"help",		'h',	OPT_FLAG,	    0,	0,	&global_flag_cmdSystemHold,		FLAG_MASK_HOLD_HELP,					VALUE_TYPE_UNSIGNED_INT,	NULL},

	{"force",		'f',	OPT_FLAG,	    0,	0,	&global_flag_cmdSystemUnhold,	FLAG_MASK_SYSTEM_UNHOLD_FORCED,			VALUE_TYPE_UNSIGNED_INT,	NULL},

	{"code",		0,		OPT_COMPONENT,	0,	0,	&global_flag_sim_workingMode,	FLAG_MASK_SIM_DECOING,					VALUE_TYPE_UNSIGNED_INT,	cmdOpt_usable_code},

	{"modulation",	0,		OPT_COMPONENT,	0,	0,	NULL,			                0,								        VALUE_TYPE_NONE,			cmdOpt_comm_modulation},
	{"channel",		0,		OPT_COMPONENT,	0,	0,	NULL,			                0,								        VALUE_TYPE_NONE,			cmdOpt_comm_channel},
	{"simulation",	0,		OPT_COMPONENT,	0,	0,	NULL,			                0,								        VALUE_TYPE_NONE,			cmdOpt_simulation},
	
	
	{"get",			0,		OPT_COMPONENT,	0,	0,	NULL,			                0,			    	                    VALUE_TYPE_NONE,			cmdOpt_get_thing},

	{"file",		0,		OPT_COMPONENT,	0,	0,	NULL,			                0,				                        VALUE_TYPE_NONE,			cmdOpt_fileIO},

	{"test_mode",	0,		OPT_COMPONENT,	0,	0,	&global_flag_sim_workingMode,	FLAG_MASK_SIM_TEST,	                    VALUE_TYPE_UNSIGNED_INT,	cmdOpt_testMode},

	{NULL,0,STRUCT_END,0,0,NULL,0,VALUE_TYPE_NONE,NULL}
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void printInstList(struct struct_cmdLineOption *p, unsigned int recursive, char *passedStrng)
{
	unsigned int i, j;
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
			//for(j = recursive; j!=0; j--) printf("\t");
			

			if(temp->longName)
			{
				if(recursive)
				{
					printf("%s", passedStrng);
					if((temp->instType==OPT_SET_VALUES)||(temp->instType==OPT_SET_LANGE))	printf("{");
					printf("-%s%c", temp->longName,((temp->instType==OPT_SET_VALUES)||(temp->instType==OPT_SET_LANGE)?:((temp->instType==OPT_SET_VALUE)?'=':'\t')));
				}
				else
				{
					printf("--%s%c", temp->longName,((temp->instType==OPT_SET_VALUES)||(temp->instType==OPT_SET_LANGE)?:((temp->instType==OPT_SET_VALUE)?'=':'\t')));
				}
			}
			
			if(temp->shortName)		printf("-%c\t", temp->shortName);
			//printf("cmd typd : %d", temp->instType);
			
			if(temp->argument) printf("arg : %d\t", temp->argument);
			if(temp->argumentNumber) printf("argNums : %d\t", temp->argumentNumber);

			if((temp->instType==OPT_SET_VALUE))	printf("[value]");
			
			if((temp->instType!=OPT_SET_VALUES)&&(temp->instType!=OPT_SET_LANGE))	printf("\n");

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
#define INST_SET_FLAG_NONE	0;
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
			printf("Instruction %s not have a mask value of flag.\n");
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
// void *instructionCreateBufferValue(enum enum_list_valueType valueType)
// {
	// switch(valueType)
	// {
		// case VALUE_TYPE_CHAR:
			// return malloc(sizeof(char));
		// case VALUE_TYPE_UNSIGNED_CHAR:
			// return malloc(sizeof(unsigned char));
			
		// case VALUE_TYPE_INT:
			// return malloc(sizeof(int));
		// case VALUE_TYPE_UNSIGNED_INT:
			// return malloc(sizeof(unsigned int));
			
		// case VALUE_TYPE_LONG:
			// return malloc(sizeof(long));
		// case VALUE_TYPE_UNSIGNED_LONG:
			// return malloc(sizeof(unsigned long));
			
		// case VALUE_TYPE_FLOAT:
			// return malloc(sizeof(float));
		// case VALUE_TYPE_DOUBLE:
			// return malloc(sizeof(double));
	// }
// }
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
double convertStringToDouble(char *string)
unsigned int convertStringToUnsignedInt(char *string)
*/
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
				// warningMes;
				// printf("VALUE_TYPE_UNSIGNED_CHAR not support yet.\n");
				// (*((unsigned char*)p->value))=0;
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
				// printf("VALUE_TYPE_UNSIGNED_LONG not support yet.\n");
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
			printf("Instruction %s not have a flag pointer.\n");
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
									// printf("\t\t\tinstSetValues) long name is \"%s\" argBuf = %s\n", (temp->longName?temp->longName:"[unknown long name]"), argBuf);
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
												// printf("instType: OPT_NOTHING[%d], name: \"%s\"\n", temp->instType, temp->longName?temp->longName:"[unknown]");
			break;
		case OPT_FLAG:
		case OPT_FLAG_CASE:
			if(instSetFlag(temp))
			{
				errorMes;
				printf("\"%s\" instruction set wrong.\n", temp->longName?temp->longName:"[empty long name]");
				return -1;
			}
												// printf("instType: OPT_FLAG[%d], name: \"%s\"\n", temp->instType, temp->longName?temp->longName:"[unknown]");
			break;
		
		case OPT_DEBUG:
												// printf("instType: OPT_DEBUG[%d], name: \"%s\"\n", temp->instType, temp->longName?temp->longName:"[unknown]");
			break;

		case OPT_SET_VALUE:
												// printf("instType: OPT_SET_VALUE[%d], name: \"%s\"\n", temp->instType, temp->longName?temp->longName:"[unknown]");
			break;
		
		case OPT_SET_VALUES:
												// printf("instType: OPT_SET_VALUES[%d], name: \"%s\"\n", temp->instType, temp->longName?temp->longName:"[unknown]");
			break;
		case OPT_SET_VALUES_SERIES: //example) {-settingCategory=[value0],[value1],[value2],[value3]}
												// printf("instType: OPT_SET_VALUES_SERIES[%d], name: \"%s\"\n", temp->instType, temp->longName?temp->longName:"[unknown]");
			break;

		case OPT_SET_LANGE: //set lange, example) {-settingCategory=[start]:[step]:[end]}
												// printf("instType: OPT_SET_LANGE[%d], name: \"%s\"\n", temp->instType, temp->longName?temp->longName:"[unknown]");
			break;
		case OPT_SET_LANGE_SERIES:
												// printf("instType: OPT_SET_LANGE_SERIES[%d], name: \"%s\"\n", temp->instType, temp->longName?temp->longName:"[unknown]");
			break;

		case OPT_STRING:
												// printf("instType: OPT_STRING[%d], name: \"%s\"\n", temp->instType, temp->longName?temp->longName:"[unknown]");
			break;
		case OPT_COMPONENT:
			if(instSetFlag(temp))
			{
				errorMes;
				printf("\"%s\" instruction set wrong.\n", temp->longName?temp->longName:"[empty long name]");
				return -1;
			}
												// printf("instType: OPT_COMPONENT[%d], name: \"%s\"\n", temp->instType, temp->longName?temp->longName:"[unknown]");
			break;
		
		case STRUCT_END:
												// printf("instType: STRUCT_END[%d], name: \"%s\"\n", temp->instType, temp->longName?temp->longName:"[unknown]");
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
											// printf("A option \"%s\" apply.\n", temp->longName);
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
	
	void *value = NULL;
	
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
															// debugMes;	printf("\t\tbefore arg : \"%s\".",arg);
			
				strncpy(argBuf, arg, (argBufLength=strcspn(arg, cmdOtionKeyWords)));
				arg+=argBufLength;
				
				(*(argBuf+argBufLength)) = 0;
				
															// printf("\t\targBuf : \"%s\"", argBuf);
															// printf("\t\tafter arg : \"%s\".\n",arg);
														
				if(!(temp = instFindSameLongNameInInstSet(p, argBuf))) return str;
				/* Find same long name, do something start*/
				if(instrocessInstElementWhenDetected(temp)) return str;
				/* Find same long name, do something end*/
			

				switch(*arg)//enter sub struct_cmdLineOption Set
				{
//					case '{':
//						arg++;
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
															// printf("find combination word.\n");
				arg++;
															// printf("\t\targ : \"%s\"\n", arg);
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
																		// printf("argBuf : '%s'\n", argBuf);
																		// printf("arg : %s\r\n", arg);

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
																	// printf("\t\t\targBuf : '%s'\n", argBuf);
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
								// printf("set value parts.\n");
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
										// printf("\tinitial arg : \"%s\"\n", arg);
		if(arg != (argBuffer=instProcessLongNameCmdOption(p, arg, 0)))
		{
			infoMes;
			printf("\"%s\" apply.\n", str);
			return 0;
		}
						// printf("arg : 0x%000000008x , argBuffer : 0x%000000008x\n", arg, argBuffer);
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
			global_flag_sim_decodingMode |= FLAG_MASK_CODE_BCH_DEC_AL_TEST_THIS_ALGORITHM;
			global_flag_sim_decodingMode &= ~FLAG_MASK_CODE_BCH_DEC_AL_SOFT_DECISION;
			global_flag_sim_decodingMode &= ~FLAG_MASK_CODE_BCH_DEC_AL_HARD_DECISION;
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
int main(unsigned int argc, char **argv)
{
	
	/*Test variables*/
	unsigned int test_fileIO_errorCorrectingCount=0;
	/*Tempolary variables*/
    unsigned int tmp_loop_cnt = 0, tmp_u_int = 0;
	unsigned int i=0, tmp_i_start, tmp_i_end;
	unsigned int j=0, tmp_j_start, tmp_j_end;
	unsigned int k=0, tmp_k_start, tmp_k_end;
	unsigned int cmd_flag=0;
	unsigned int cmd_length=0;
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
	// double main_indi_BER = 0.0f;
	double *main_indi_HD_BER = NULL;
	double *main_indi_BER = NULL;
	double main_com_channel_BER = 0.0f;



	struct_encodingComponentInGF *main_encodingComponentInGF=NULL;

	// struct_HD_BM_algorithmComponent *main_indi_HD_decordComponents=NULL;
	// struct_HD_BM_algorithmComponent ***main_indi_SD_list_of_decordingComponents=NULL;
	/* decoding components list creating */
	struct_generalForm_algorithmComponent **main_indi_HD_decordComponents=NULL;
	struct_generalForm_algorithmComponent ****main_indi_SD_list_of_decordingComponents=NULL;

	struct_errorComponent *main_com_errComponents=NULL;
	struct_bpskComponents *main_com_bpskComponents=NULL;

	
	
		
	char *main_com_randInfoBitsStream=NULL;
	
	// unsigned int main_count=0;
	unsigned int main_com_errNums_inDecodingAlgorithmTest=0;
	unsigned long main_com_loopCount=0;
	unsigned long main_com_totalSamlingLoop=0;
	// unsigned int main_shiftedError=0;
	
	
	
	
//////////////////////////////////////////////////////////////////////////////////
//////////////////// Temporary file input output variables ///////////////////////
//////////////////////////////////////////////////////////////////////////////////
    char tmp_fileio_target_dir_path[1001] = {0};
    char tmp_fileio_target_file_path[1001] = {0};
    unsigned int fileio_name_length = 0;
    unsigned int fileio_name_tmp    = 0;
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

    char *dirio_Name_home_path = NULL;
    char dirio_Name_simEnv_root_path[] = "~/";
    char dirio_Name_simEnv_dir[] = "bch-sim_env";
    char dirio_Name_common_dir[] = "common";

	char dirio_Name_GF_info[] = "GaloisFields";

    char fileio_Name_GF_infoCommon[] = "GF";

	char fileio_Name_GaloisField_common[] = "GaloisField";
	char fileio_Name_Conjugate_common[] = "Conjugate";
	char fileio_Name_SumMaxrix_common[] = "SumMatrix";
	// char fileio_Name_GF_confugate[] = "conju";
	char fileio_Name_GF_infoBuf[256]={0};
	char fileio_Path_common[512]={0};
    char fileio_Path_Target[768]={0};
	struct dirent  *tmp_dir_entry;
	struct dirent  *tmp_dir_entry2;
	
	unsigned int tmp_dirCnt;

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
	/*To convert dataStream to powerFormPolynomial, define variable structure start*/
/* 	struct_variableSetConvertBitStreamToPowerForm loadStream;
	struct_variableSetConvertBitStreamToPowerForm noisedStream;
	struct_variableSetConvertBitStreamToPowerForm unloadStream; */
	/*To convert dataStream to powerFormPolynomial, define variable structure end*/
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
    sprintf(fileio_Path_Target, "%s/%s", fileio_Path_Target, dirio_Name_common_dir);
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
    sprintf(fileio_Path_Target, "%s/%s", fileio_Path_Target, dirio_Name_GF_info);
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
	
	/*Exception process*/
	/*code length is can't exceed GF length.*/
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
	/*BCH ALGORITHM TEST*/
	/*
		chk any flag of algorithm test.
		if any flag is set, simulation will process decoding algorithm test sequence.
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
	// else if(global_flag_case_selTP_before_dec == FLAG_CASE_SEL_TP_BEFORE_DEC_ACT)
	// {
		// if(!(global_flag_sim_decodingMode&FLAG_MASK_CODE_BCH_DEC_AL_SOFT_DECISION))
		// {
			// global_flag_case_selTP_before_dec = FLAG_CASE_SEL_TP_BEFORE_DEC_THEO;
			// warningMesShort; printf("ACTUAL MODE support only soft decision.\r\n");
			// warningMesShort; printf("Getting BER mode forcely change to Theoretical mode.\r\n");
		// }
	// }
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

	/*soft Decision variable start*/
	//Default value of main_tmp_degErrLocPoly is 2.;
	// if(!(global_flag_bchDecAlgo_Enable&FLAG_MASK_CODE_BCH_DEC_AL_SOFT_DECISION))
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

	

	/*soft Decision variable end*/
		
		
	//*soft Decision Channel Counting variable start*/
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
	
	// main_list_err_cnt_after_dec_whole_TP_best=-1;

	// main_indi_sel_TP_errCnt=-1;
	/*Belong to dec algorithm HD kernel individually have main_indi_sel_TP_i */
	main_indi_sel_TP_i = (unsigned int*)malloc(sizeof(unsigned int)*KIND_OF_BCH_DECODING_END);
	memset(main_indi_sel_TP_i, 0, sizeof(unsigned int)*KIND_OF_BCH_DECODING_END);
	//*soft Decision Channel Counting variable end*/
	
	/* soft Decision correctable test pattern list start */
	main_indi_list_flag_theo_ly_can_be_correctable = (unsigned char**)malloc(sizeof(unsigned char*)*KIND_OF_BCH_DECODING_END);
	for(main_tmp_sel_decAlgo_i=0; main_tmp_sel_decAlgo_i<KIND_OF_BCH_DECODING_END; main_tmp_sel_decAlgo_i++)
	{
		main_indi_list_flag_theo_ly_can_be_correctable[main_tmp_sel_decAlgo_i] = (unsigned char*)malloc(sizeof(unsigned char)*main_com_numsTP);
		memset(main_indi_list_flag_theo_ly_can_be_correctable[main_tmp_sel_decAlgo_i], 0, sizeof(unsigned char)*main_com_numsTP);
				// printf("main_indi_list_flag_theo_ly_can_be_correctable[%d] 0x%x\n", main_tmp_sel_decAlgo_i, main_indi_list_flag_theo_ly_can_be_correctable[main_tmp_sel_decAlgo_i]);
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
	
	//	struct_generalForm_algorithmComponent ****main_indi_SD_list_of_decordingComponents=NULL;
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
	/*Channel variable end*/
	
	
	
	
	/*Soft Decision Test Pattern Decording Coponents. If soft decision correctable bits is p,  number of test petern is (2^p) - 1.*/
	// unsigned int main_com_numsTP=main_com_numsTP;
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
		// *(main_indi_SD_list_of_decordingComponents[main_tmp_sel_decAlgo_i]+1)=(&decordingTestPeternComponents1);
		// *(main_indi_SD_list_of_decordingComponents[main_tmp_sel_decAlgo_i]+2)=(&decordingTestPeternComponents2);
		// *(main_indi_SD_list_of_decordingComponents[main_tmp_sel_decAlgo_i]+3)=(&decordingTestPeternComponents3);
	}
	/* decoding components list creating2 end */

	
	/*Soft Decision Test Pattern Decording Coponents. end*/
	
	
	
	
	
	/******************************************************************/
	/******************** memory initializing start *******************/
	/******************************************************************/
/* 	memset(&loadStream, 0, sizeof(struct_variableSetConvertBitStreamToPowerForm));
	memset(&noisedStream, 0, sizeof(struct_variableSetConvertBitStreamToPowerForm));
	memset(&unloadStream, 0, sizeof(struct_variableSetConvertBitStreamToPowerForm)); */
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
		printPowerFormWithEnter((char_POWER_FORM*)PRIMITIVE_POLYNOMIAL_4_INVERSE_INDEX);
		printPowerFormWithEnter((char_POWER_FORM*)PRIMITIVE_POLYNOMIAL_8_INVERSE_INDEX);
		printPowerFormWithEnter((char_POWER_FORM*)PRIMITIVE_POLYNOMIAL_16_INVERSE_INDEX);
		printPowerFormWithEnter((char_POWER_FORM*)PRIMITIVE_POLYNOMIAL_32_INVERSE_INDEX);
		printPowerFormWithEnter((char_POWER_FORM*)PRIMITIVE_POLYNOMIAL_64_INVERSE_INDEX);
		printPowerFormWithEnter((char_POWER_FORM*)PRIMITIVE_POLYNOMIAL_128_INVERSE_INDEX);
		printPowerFormWithEnter((char_POWER_FORM*)PRIMITIVE_POLYNOMIAL_256_INVERSE_INDEX);
		printPowerFormWithEnter((char_POWER_FORM*)PRIMITIVE_POLYNOMIAL_512_INVERSE_INDEX);
		printPowerFormWithEnter((char_POWER_FORM*)PRIMITIVE_POLYNOMIAL_1024_INVERSE_INDEX);
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
				// printf("%s\r\n", fileio_Name_GF_infoBuf);
			rewinddir(tmp_dirio_var);
			while(tmp_dir_entry = readdir(tmp_dirio_var))  // ???????????? ?????? ?????? ?????? ????????? ???????????? ??????
			{
				if(!strcmp(fileio_Name_GF_infoBuf, tmp_dir_entry->d_name)) break;
			}
			/* check file that is saved galois fields infomations end */
		
			initMes; printf("Selected primitive poly is %s\r\n", (char*)PRIMITIVE_POLYNOMIAL_SET_INVERSED.PRIMITIVE_POLYNOMIAL[i]);
			main_com_listOf_GF[i]=
				// createGaloisFieldAndSumMatrix(createPowerFormPolynomialUsingString((char_POWER_FORM*)PRIMITIVE_POLYNOMIAL_SET_INVERSED.PRIMITIVE_POLYNOMIAL[i], 0));
				createGaloisField_info_emptySpace(createPowerFormPolynomialUsingString((char_POWER_FORM*)PRIMITIVE_POLYNOMIAL_SET_INVERSED.PRIMITIVE_POLYNOMIAL[i], 0));
			sprintf(fileio_Path_Target, "%s/%s/%s", fileio_Path_common, dirio_Name_GF_info, fileio_Name_GF_infoBuf);
				// printf("%s\r\n", fileio_Path_common);

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
						printf("[calculateing and saving galois field time]");	printExcutingTime(&time_proc_start);
			}
			else
			{
				infoMes; printf("File path -> %s is detected.\r\n", fileio_Path_Target);
				load_struct_galoisFieldElements(fileio_Path_Target, (char*)PRIMITIVE_POLYNOMIAL_SET_INVERSED.PRIMITIVE_POLYNOMIAL[i], main_com_listOf_GF[i]->nonSorted);
				infoMes; printf("'%s' is loaded.\r\n", fileio_Name_GF_infoBuf);
						printf("[loading galois field time]");	printExcutingTime(&time_proc_start);
			}


			/* Sumation Matrix infomation */
			/* check file that is saved galois fields infomations start */
			sprintf(fileio_Name_GF_infoBuf, "%s_%d_px_%s_%s.txt", 
				fileio_Name_GF_infoCommon, 
				i, 
				(char*)PRIMITIVE_POLYNOMIAL_SET_INVERSED.PRIMITIVE_POLYNOMIAL[i],
				fileio_Name_SumMaxrix_common
			);
				// printf("%s\r\n", fileio_Name_GF_infoBuf);
			rewinddir(tmp_dirio_var);
			while(tmp_dir_entry = readdir(tmp_dirio_var))  // ???????????? ?????? ?????? ?????? ????????? ???????????? ??????
			{
				if(!strcmp(fileio_Name_GF_infoBuf, tmp_dir_entry->d_name)) break;
			}
			/* check file that is saved galois fields infomations end */
			
			sprintf(fileio_Path_Target, "%s/%s/%s", fileio_Path_common, dirio_Name_GF_info, fileio_Name_GF_infoBuf);
						gettimeofday(&time_proc_start, NULL);
			if(!tmp_dir_entry)
			{
			    infoMes; printf("Saved Path -> %s\r\n", fileio_Path_Target);
				// char save_struct_summationMatrix(char *path, char *primitivePoly, struct_summationMatrix *p)
				calculateSummationMatrix(main_com_listOf_GF[i]->nonSorted, main_com_listOf_GF[i]->nonSorted->summationReferMatrix);
				save_struct_summationMatrix(
					fileio_Path_Target, 
					(char*)PRIMITIVE_POLYNOMIAL_SET_INVERSED.PRIMITIVE_POLYNOMIAL[i], 
					main_com_listOf_GF[i]->nonSorted->summationReferMatrix
				);
				infoMes; printf("'%s' is saved.\r\n", fileio_Name_GF_infoBuf);
						printf("[calculateing and saving summation matrix time]");	printExcutingTime(&time_proc_start);
			}
			else
			{
				infoMes; printf("File path -> %s is detected.\r\n", fileio_Path_Target);
				load_struct_summationMatrix(fileio_Path_Target, (char*)PRIMITIVE_POLYNOMIAL_SET_INVERSED.PRIMITIVE_POLYNOMIAL[i], main_com_listOf_GF[i]->nonSorted, main_com_listOf_GF[i]->nonSorted->summationReferMatrix);
				infoMes; printf("'%s' is loaded.\r\n", fileio_Name_GF_infoBuf);
						printf("[loading summation matrix time]");	printExcutingTime(&time_proc_start);
			}


			
			/* check file that is saved galois fields infomations start */
			sprintf(fileio_Name_GF_infoBuf, "%s_%d_px_%s_%s.txt", 
				fileio_Name_GF_infoCommon, 
				i, 
				(char*)PRIMITIVE_POLYNOMIAL_SET_INVERSED.PRIMITIVE_POLYNOMIAL[i],
				fileio_Name_Conjugate_common
			);
				// printf("%s\r\n", fileio_Name_GF_infoBuf);
			rewinddir(tmp_dirio_var);
			while(tmp_dir_entry = readdir(tmp_dirio_var))  // ???????????? ?????? ?????? ?????? ????????? ???????????? ??????
			{
				if(!strcmp(fileio_Name_GF_infoBuf, tmp_dir_entry->d_name)) break;
			}
			/* check file that is saved galois fields infomations end */
			sprintf(fileio_Path_Target, "%s/%s/%s", fileio_Path_common, dirio_Name_GF_info, fileio_Name_GF_infoBuf);
				// printf("%s\r\n", fileio_Path_common);
				
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
						printf("[calculateing and saving conjugacies time]");	printExcutingTime(&time_proc_start);
			}
			else
			{
				infoMes; printf("File path -> %s is detected.\r\n", fileio_Path_Target);
				load_struct_setOfGaloisFieldElements(fileio_Path_Target, (char*)PRIMITIVE_POLYNOMIAL_SET_INVERSED.PRIMITIVE_POLYNOMIAL[i], main_com_listOf_GF[i]->nonSorted, &(main_com_listOf_GF[i]->conjugacies));
				infoMes; printf("'%s' is loaded.\r\n", fileio_Name_GF_infoBuf);
						printf("[loading conjugacies time]");	printExcutingTime(&time_proc_start);
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

		// main_com_used_GF=createGaloisFieldAndSumMatrix(createPowerFormPolynomialUsingString((char_POWER_FORM*)PRIMITIVE_POLYNOMIAL_SET.PRIMITIVE_POLYNOMIAL[global_GaloisFieldExponential],0));
		
		/*
		struct_galoisField_info *galoisField8=createGaloisFieldAndSumMatrix(createPowerFormPolynomialUsingString((char_POWER_FORM*)PRIMITIVE_POLYNOMIAL_8_INVERSE_INDEX,0));
		struct_galoisField_info *galoisField16=createGaloisFieldAndSumMatrix(createPowerFormPolynomialUsingString((char_POWER_FORM*)PRIMITIVE_POLYNOMIAL_16_INVERSE_INDEX,0));
		struct_galoisField_info *galoisField32=createGaloisFieldAndSumMatrix(createPowerFormPolynomialUsingString((char_POWER_FORM*)PRIMITIVE_POLYNOMIAL_32_INVERSE_INDEX,0));
		struct_galoisField_info *galoisField64=createGaloisFieldAndSumMatrix(createPowerFormPolynomialUsingString((char_POWER_FORM*)PRIMITIVE_POLYNOMIAL_64_INVERSE_INDEX,0));
		struct_galoisField_info *galoisField256=createGaloisFieldAndSumMatrix(createPowerFormPolynomialUsingString((char_POWER_FORM*)PRIMITIVE_POLYNOMIAL_256_INVERSE_INDEX,0));
		struct_galoisField_info *galoisField1024=createGaloisFieldAndSumMatrix(createPowerFormPolynomialUsingString((char_POWER_FORM*)PRIMITIVE_POLYNOMIAL_1024_INVERSE_INDEX,0));*/

		/*
		printGaloisFieldToCsv(galoisField8->nonSorted);
		printGaloisFieldToCsv(galoisField16->nonSorted);
		printGaloisFieldToCsv(galoisField32->nonSorted);
		printGaloisFieldToCsv(galoisField64->nonSorted);
		printGaloisFieldToCsv(galoisField256->nonSorted);
		printGaloisFieldToCsv(galoisField1024->nonSorted);
		 */
	/* create galois field using primitive polynomial end */
	
	
	
	
	/*Using conjugacy in galois Field, make generation polynomial start*/
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
		/*To test BCH Algorithm, setting number of errors end*/
		
	//calculate total order of LCM.
	if((main_com_used_GF->conjugacies->length<main_com_hd_correctability))
	{
		//errorMes;
		warningMes;	printf("In test generation of LCM, (main_com_used_GF->conjugacies->length(=\'%d\') < main_com_hd_correctability(=\'%d\'))\n", main_com_used_GF->conjugacies->length, main_com_hd_correctability);

		main_com_hd_correctability = main_com_used_GF->conjugacies->length;
		warningMes;	printf("Force to set main_com_hd_correctability that \'%d\'.\n", main_com_hd_correctability);
		//return -1;
	}
	for(i=0; i<main_com_hd_correctability; i++)
	{
		main_com_totalOrderOfLCM+=(*(main_com_used_GF->conjugacies->conjugateSet+i))->length;
	}
	//calculate total order of LCM.
	infoMes; printf("Total Order(Condjugacies only) Of LCM is \'%d\'\n", main_com_totalOrderOfLCM);

	////create////
	recreateGaloisFieldExceptElements(&main_com_expandEqOfLcmFromRoot, main_com_totalOrderOfLCM+1);
	initEquation(main_com_used_GF->nonSorted, main_com_expandEqOfLcmFromRoot);
	
	////abstract////
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
		// printGaloisField2(main_com_used_GF->nonSorted, main_com_expandEqOfLcmFromRoot, PRINT_FIELD_EQUATION_TITLE, PRINT_FIELD_PRINT_OPTION_NUMBERING);
	if(global_flag_gfCommon_display&FLAG_MASK_DISPLAY_GF_COMMON_PRIMEPOLY)
	{
		printGaloisField2(main_com_used_GF->nonSorted, main_com_expandEqOfLcmFromRoot, PRINT_FIELD_EQUATION_TITLE, PRINT_FIELD_PRINT_OPTION_NUMBERING);
		printGaloisField2(main_com_used_GF->nonSorted, main_com_addedLCM, PRINT_FIELD_EQUATION_TITLE, PRINT_FIELD_PRINT_OPTION_NUMBERING);
	}
	
	main_com_genPoly=convertGaloisFielsAndElementsToStringOnlyZeroOrOne(main_com_used_GF->nonSorted, main_com_finalEqOfrootOfLCM_andAddedLCM);
	initMes; printf("Converting galois field element to string is done.\r\n");
	infoMes; printf("Expanded Least Common Multiple(generation polynomial) is \"%s\"\n", main_com_genPoly);

		////create////
		recreateGaloisFieldExceptElements(&main_com_limitExpandEqOfLCM_fromRoot, main_com_totalOrderOfLCM+1);
		initEquation(main_com_used_GF->nonSorted, main_com_limitExpandEqOfLCM_fromRoot);

		////abstract////	
		main_com_limitExpandEqOfLCM_fromRoot=test_expandLimitedLeastCommonMultipleUsingElementOfGaloisFieldToRoot(main_com_used_GF->nonSorted, main_com_limitExpandEqOfLCM_fromRoot, main_com_used_GF->conjugacies, main_com_hd_correctability);
		initMes; printf("Initializing expand LCM is done.\r\n");
				// printGaloisField2(main_com_used_GF->nonSorted, main_com_limitExpandEqOfLCM_fromRoot, PRINT_FIELD_EQUATION_TITLE, PRINT_FIELD_PRINT_OPTION_NUMBERING);
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
	/*Using conjugacy in galois Field, make generation polynomial end*/

	
	/*Create encoding component using converted g(x) to power form(string) start*/
 	infoMes; printf("Test generation of LCM, GF(%d), set t=%d, select \'%d\' number of set of conjugacy\n", main_com_used_GF->nonSorted->length, main_com_hd_correctability, main_com_hd_correctability);
	//struct_encodingComponentInGF *main_encodingComponentInGF=createEncodingComponent(main_com_codeLength, main_com_hd_correctability, (char_POWER_FORM*)main_com_genPoly);
	recreateEncodingComponent(&main_encodingComponentInGF, main_com_codeLength, main_com_hd_correctability, (char_POWER_FORM*)main_com_genPoly);
	initMes; printf("Creating encoding componenet is done.\r\n");
	/*Create encoding component using converted g(x) to power form(string) end*/

	

	/*Soft Decision Test Pattern Decording Coponents. If soft decision correctable bits is p,  number of test petern is (2^p) - 1.*/
	/*Soft Decision Test Pattern Decording Coponents. end*/
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
	/*creation BPSK communication compoenets start*/
	/*initial global quantization info*/
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
			/*initial LLR components start*/
			setUsedLengthOfLogLikeHoodRatio(main_com_bpskComponents->bpskReceivedLLR, main_com_codeLength);
			initMes; printf("Setting up log like hood ratio in bpsk componenets.\r\n");
			/*initial LLR magnitudeShort locator start*/
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
																			// if(!(testStructure = createTreeStruct(127, TREE_CHASE)))  debugMes; printf("%d : testStructure = createTreeStruct(127, TREE_CHASE)\n", i);
																			// if(closeTreeStruct(&testStructure)) debugMes; printf("%d : closeTreeStruct(&testStructure)\n", i);
																		// }
																		/****** struct_treeStructure create and close memory test end ******/
			//		printf("test : mask value is '%d'\n", main_com_bpskComponents->bpskReceivedLLR->quantizedLLRMask);
																		#endif
			/*initial LLR magnitudeShort locator end*/
			/*initial LLR components end*/
			/*creation BPSK communication compoenets end*/
		}
	}

	
	printf("/*****************************************************************/\r\n");
	printf("/************* Initialzing a simulation enviroments **************/\r\n");
	printf("/*****************************************************************/\r\n");
	// if(!global_minimum_loop)
	// {
		// global_minimum_loop=DEFAULT_MINIMUM_LOOP;
		// infoMes; printf("Minumum loop is not assigned, set to default, is %ld\r\n", global_minimum_loop);
	// }
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
				infoMes; printf("[%s] * %s algorithm test is primary than hard and soft decision. *\r\n");				
			}

			if((global_list_flag_bch_sim_decodingMode[cmd_tmp_i]&FLAG_MASK_CODE_BCH_DEC_AL_SKIP))
			{
				if((global_list_flag_bch_sim_decodingMode[cmd_tmp_i]&FLAG_MASK_CODE_BCH_DEC_AL_HARD_DECISION)||(global_list_flag_bch_sim_decodingMode[cmd_tmp_i]&FLAG_MASK_CODE_BCH_DEC_AL_SOFT_DECISION))
				{
					infoMes; printf("[%s] * Decoding Process skip is enabled. *\r\n", KIND_OF_BCH_DECODING_ALGORITHM_NAME[cmd_tmp_i]);
				}
			}
		}
		// else
		// {
			// infoMes; printf("[%s] %s algorithm test is disabled.\r\n", 
				// KIND_OF_BCH_DECODING_ALGORITHM_NAME[cmd_tmp_i], KIND_OF_BCH_DECODING_ALGORITHM_NAME[cmd_tmp_i]);
		// }

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

	//loadStream, unloadStream
	// infoMes; printf(">>BMP file io initial variable set<<\r\n");
	// warningMes;
	// printf("File IO function is not completed.\r\n");
	// infoMes;
	// printf("dataBitLength is \'%u\'.\r\n", loadStream.dataBitLength);
	// infoMes;
	// printf("codeWordPerDataNumber is \'%u\'.\r\n", loadStream.codeWordPerDataNumber);
	// infoMes;
	// printf("totalCodeWordNumber is \'%u\'.\r\n", loadStream.totalCodeWordNumber);
	// infoMes;
	// printf("totalDataNumber is \'%u\'.\r\n", loadStream.totalDataNumber);
	// printf("\r\n");
	// if(global_flag_fileInputOutput)
	// {
		// /*file io is not completed*/
		// warningMes;
		// printf("/*************** file io function is not completed ***************/\r\n");
		// printf("Please press ctrl + c or ctrl + z\r\n");
		
		
		// if(loadBmpFile("image/1.bmp"))
		// {
			// printf("fail open image/1.bmp \r\n");
		// }
		// else
		// {
			// printf("open image/1.bmp\r\n");
		// }
		
		// setVariableSetConvertBitStreamToPowerForm(&loadStream, getBmpData(), main_com_infoBitLength, sizeof(uint8_t), BMP_FILE_IO_DEFAULT_BITMAP_DATA_SIZE);
		// setVariableSetConvertBitStreamToPowerForm(&unloadStream, getBmpData2(), main_com_infoBitLength, sizeof(uint8_t), BMP_FILE_IO_DEFAULT_BITMAP_DATA_SIZE);
		// setVariableSetConvertBitStreamToPowerForm(&noisedStream, getBmpData3(), main_com_infoBitLength, sizeof(uint8_t), BMP_FILE_IO_DEFAULT_BITMAP_DATA_SIZE);
		// printf("\r\n>>BMP file io initial variable set<<\r\n");
		// warningMes;
		// printf("File IO function is not completed.\r\n");
		// infoMes;
		// printf("dataBitLength is \'%u\'.\r\n", loadStream.dataBitLength);
		// infoMes;
		// printf("codeWordPerDataNumber is \'%u\'.\r\n", loadStream.codeWordPerDataNumber);
		// infoMes;
		// printf("totalCodeWordNumber is \'%u\'.\r\n", loadStream.totalCodeWordNumber);
		// infoMes;
		// printf("totalDataNumber is \'%u\'.\r\n", loadStream.totalDataNumber);
		// printf("\r\n\r\n");

		// passingThroughAwgnData
		// (
			// global_EbN0_Start_Value,
			// main_com_parityBitsLength,
			// main_com_genPoly,
			// main_com_hd_correctability,
			// main_com_codeLength,
			// main_com_used_GF,
			// &loadStream,
			// &unloadStream,
			// &noisedStream
		// );
		// saveBmpFile2(0);
		// saveBmpFile3(0);
		// saveBmpFile4(0);
		
		
		
		
		// setVariableSetConvertBitStreamToPowerForm(&loadStream, getBmpData(), main_com_infoBitLength, sizeof(uint8_t), BMP_FILE_IO_DEFAULT_BITMAP_DATA_SIZE);
		// setVariableSetConvertBitStreamToPowerForm(&unloadStream, getBmpData2(), main_com_infoBitLength, sizeof(uint8_t), BMP_FILE_IO_DEFAULT_BITMAP_DATA_SIZE);
		// setVariableSetConvertBitStreamToPowerForm(&noisedStream, getBmpData3(), main_com_infoBitLength, sizeof(uint8_t), BMP_FILE_IO_DEFAULT_BITMAP_DATA_SIZE);
		// printf("\r\n>>BMP file io initial variable set<<\r\n");
		// warningMes;
		// printf("File IO function is not completed.\r\n");
		// infoMes;
		// printf("dataBitLength is \'%u\'.\r\n", loadStream.dataBitLength);
		// infoMes;
		// printf("codeWordPerDataNumber is \'%u\'.\r\n", loadStream.codeWordPerDataNumber);
		// infoMes;
		// printf("totalCodeWordNumber is \'%u\'.\r\n", loadStream.totalCodeWordNumber);
		// infoMes;
		// printf("totalDataNumber is \'%u\'.\r\n", loadStream.totalDataNumber);
		// printf("\r\n\r\n");

		// passingThroughAwgnData
		// (
			// global_EbN0_Start_Value,
			// main_com_parityBitsLength,
			// main_com_genPoly,
			// main_com_hd_correctability,
			// main_com_codeLength,
			// main_com_used_GF,
			// &loadStream,
			// &unloadStream,
			// &noisedStream
		// );
		// saveBmpFile2("OUTPUT_CORRECT.bmp");
		// saveBmpFile3("OUTPUT_ADDITIVE.bmp");
		// saveBmpFile4("OUTPUT_NOISE_ONLY.bmp");
		
		// /* not complete part of decoding start */
		// test_fileIO_errorCorrectingCount++;
		// if(test_fileIO_errorCorrectingCount!=0)
		// {
			// if(loadStream.totalCodeWordNumber)
			// {
				// if(!(test_fileIO_errorCorrectingCount%loadStream.totalCodeWordNumber))
				// {
					// infoMes;
					// printf("one stream sending is done. test_fileIO_errorCorrectingCount is \'%u\'\r\n", test_fileIO_errorCorrectingCount);
				// }
			// }
		// }
		// /* not complete part of decoding start */
	// }


    /* REPORT Channel BER */
	// FILE *fileio_channelBER;
	// char fileio_Name_channel_BER[256]={0};
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
	// FILE *fileio_codingBER;
	// char fileio_Name_coding_BER[256]={0};
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
	// FILE	*fileio_total_bit_cnt;
	// char fileio_Name_total_bit_cnt[256]={0};
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
	// FILE	*fileio_channel_err_bit_cnt;
	// char fileio_Name_channel_err_bit_cnt[256]={0};
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
		// (*(((uint8_t*)processingUseThisAlgorithm->list)+main_tmp_sel_decAlgo_i))
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
		//	FILE	**fileio_indi_algorithm_coding_HD_err_bit_cnt;
		// char fileio_Name_indi_algorithm_coding_HD_err_bit_cnt[KIND_OF_BCH_DECODING_END][256]={0};
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
		// FILE	**fileio_indi_algorithm_coding_err_bit_cnt;
		// char fileio_Name_indi_algorithm_coding_err_bit_cnt[KIND_OF_BCH_DECODING_END][256]={0};
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
	// FILE *fileio_channelFER;
	// char fileio_Name_channel_FER[256]={0};
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
	// FILE *fileio_codingFER;
	// char fileio_Name_coding_FER[256]={0};
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
	// FILE *fildio_EbN0;
	// char fileio_Name_EbN0[256]={0};
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
	// FILE *fildio_loop_Cnt;
	// char fileio_Name_loop_Cnt[256]={0};
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
	// FILE *fileio_simEnv;
	// char fileio_Name_simEnv[256]={0};
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
	sprintf(fileio_Name_total_bit_cnt, "%s/PATTERB_GF(2^%d)_bch(%d,%d)_p%d_t%d_%s_%s_%s_%s_bit_cnt_total.txt", 
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
	if(!(fileio_total_bit_cnt=fopen(fileio_Name_total_bit_cnt, "w")))
	{
		errorMes;
		printf("Can not open \"%s\"\r\n", fileio_Name_total_bit_cnt);
		return -1;
	}
	infoMes; printf("Enable file I/O \"%s\"\r\n", fileio_Name_total_bit_cnt);
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
	sprintf(fileio_Name_total_bit_cnt, "%s/PATTERB_GF(2^%d)_bch(%d,%d)_p%d_t%d_%s_%s_%s_%s_bit_cnt_total.txt", 
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
	if(!(fileio_total_bit_cnt=fopen(fileio_Name_total_bit_cnt, "w")))
	{
		errorMes;
		printf("Can not open \"%s\"\r\n", fileio_Name_total_bit_cnt);
		return -1;
	}
	infoMes; printf("Enable file I/O \"%s\"\r\n", fileio_Name_total_bit_cnt);
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
		case FLAG_CASE_SEL_TP_BEFORE_DEC_ACT: //#define FLAG_CASE_SEL_TP_BEFORE_DEC_ACT		1
			fprintf(fileio_simEnv, "Get BER Mode is Practical.\r\n");
		break;
		
		case FLAG_CASE_SEL_TP_BEFORE_DEC_THEO: //#define FLAG_CASE_SEL_TP_BEFORE_DEC_THEO	2
			fprintf(fileio_simEnv, "Get BER Mode is Theoretical.\r\n");

			switch(global_flag_case_choosingCodeWordMethod_inTheo)
			{
				case FLAG_CASE_CODEWORD_CHOOSEN_METHOD_IN_THEO_NO_ERR:		/* not-support */
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
            switch(global_flag_case_testMode_msg_gen_method)
            {
                case ENUM_CASE_TESTMODE_ERR_RATE_MSG_GEN_METH_RAND:

                break;

                case ENUM_CASE_TESTMODE_ERR_RATE_MSG_GEN_METH_STATIC:

                break;
            }

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
            ); //need to add bitrate

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
                        if(main_com_bpskComponents->bpskReceivedLLR->llr[i] > 0.0f)
                        {
                            printf("llr:+%5.8f, Q llr:0x%08x, Q llr 0d%d, mag:0x%08x, mag:%d mag+0:0d+%d\r\n",
                                    main_com_bpskComponents->bpskReceivedLLR->llr[i],
                                    main_com_bpskComponents->bpskReceivedLLR->quantizedLLR[i],
                                    main_com_bpskComponents->bpskReceivedLLR->quantizedLLR[i],
                                    main_com_bpskComponents->bpskReceivedLLR->magnitude[i],
                                    main_com_bpskComponents->bpskReceivedLLR->magnitude[i],
                                    main_com_bpskComponents->bpskReceivedLLR->magnitude[i]
                            );
                        }
                        else
                        {
                            printf("llr:%5.8f, Q llr:0x%08x, Q llr 0d%d, mag:0x%08x, mag:%d,  mag+1:0d%d\r\n",

                                    main_com_bpskComponents->bpskReceivedLLR->llr[i],
                                    main_com_bpskComponents->bpskReceivedLLR->quantizedLLR[i],
                                    main_com_bpskComponents->bpskReceivedLLR->quantizedLLR[i],
                                    main_com_bpskComponents->bpskReceivedLLR->magnitude[i],
                                    main_com_bpskComponents->bpskReceivedLLR->magnitude[i],
                                    main_com_bpskComponents->bpskReceivedLLR->magnitude[i]+1
                            );
                        }
                        */
                        /* ones complement verify code end */
                /* if received bit is error */
                if(main_encodingComponentInGF->codeWord->equation[i] != main_com_errComponents->erroredCodeWord->equation[i])
                {
                    //tmp_testMode_llr_mag_sum_w+=(unsigned long)main_com_bpskComponents->bpskReceivedLLR->magnitude[i];
                    //==> tmp_testMode_llr_mag_sum_w += tmp_testMode_llr_mag_sum_inR_w;
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
                    //==> tmp_testMode_llr_mag_sum_c += tmp_testMode_llr_mag_sum_inR_c;
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

            

            
            /*encoding parts*/
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
                    ",%d", tmp_err_llr_mag_distribution_in_all_loop_arranged_by_err_nums[i][j]  
                );
                fprintf(fp_testMode_all_arranged_by_errorNums_csv,
                    ",%d", tmp_err_llr_mag_distribution_in_all_loop_arranged_by_err_nums[i][j]
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
            printf("[e] %d:%ld %.2f\%\r\n", i, tmp_testMode_llr_mag_w[i], (double)tmp_testMode_llr_mag_w[i]/(double)tmp_testMode_count_bit_w);
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
            printf("[c] %d:%ld %.2f\%\r\n", i, tmp_testMode_llr_mag_c[i], (double)tmp_testMode_llr_mag_c[i]/(double)tmp_testMode_count_bit_c);
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
            fprintf(fp_testMode_log, "[e]%d:%ld %.2f\%\r\n", i, tmp_testMode_llr_mag_w[i], (double)tmp_testMode_llr_mag_w[i]/(double)tmp_testMode_count_bit_w);
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
            fprintf(fp_testMode_log, "[c] %d:%ld %.2f\%\r\n", i, tmp_testMode_llr_mag_c[i], ((double)tmp_testMode_llr_mag_c[i]/(double)tmp_testMode_count_bit_c))*100.0f;
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
    printf("tmp_testMode_prob_min_counting = 0x%04x\r\n", &tmp_testMode_prob_min_counting);
    printf("tmp_testMode_prob_min_counting_length = %d\r\n", &tmp_testMode_prob_min_counting_length);

	for(main_com_EbN0=global_EbN0_Start_Value; main_com_EbN0<=global_EbN0_End_Value; main_com_EbN0+=global_EbN0_Step)
    {
        for(tmp_testMode_loops_cnt=0UL; tmp_testMode_loops_cnt<tmp_testMode_loops; tmp_testMode_loops_cnt++)
        {
        //case ENUM_CASE_TESTMODE_ERR_RATE_MSG_GEN_METH_RAND:
            /*Stream Generation*/
            main_com_randInfoBitsStream = 
                randdomInfoBitGenerator(main_com_infoBitLength, main_com_parityBitsLength, &main_com_randInfoBitsStream);

            /*Copy stream to codeword components*/
            copyPowerFormPolynomialFromString(main_encodingComponentInGF->codeWord, main_com_randInfoBitsStream);

            /*To calculate parity, shifting stream parity length*/
            shiftHighSidePowerFormPolynomial(main_encodingComponentInGF->codeWord, main_com_parityBitsLength);

            /*Calculating Parity*/
            calculateParityInGaloisFieldAttachLowSide(main_encodingComponentInGF->codeWord, main_com_parityBitsLength, main_encodingComponentInGF->generationPolynomial);
        //break;

            /*Create error components*/
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

            /*Decoding*/
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
				Making a random infomation bits and encoding codeword is 
				common block in blockAlgorithm Test, Hard - Decision and Soft - Decision Mode 
				start
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
							// printPowerFormWithEnterPolynomialWithEnterUsingAddress(main_encodingComponentInGF->codeWord);
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
				Making a random infomation bits and encoding codeword is 
				common block in blockAlgorithm Test, Hard - Decision and Soft - Decision Mode 
				end
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

				/*
					#define FLAG_MASK_CODE_BCH_DEC_AL_SOFT_DECISION	(1<<3)
					#define infoMes	printf("test SOFT ) ");
				*/
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
				// main_com_errComponents=ifNotExistCreateErrorComponent(&main_com_errComponents, main_com_codeLength);

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
									// KIND_OF_BCH_DECODING_ALGORITHM_NAME[(*(main_indi_HD_decordComponents+(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))))->KIND_OF_BCH_ALGORITHM], 
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
									// KIND_OF_BCH_DECODING_ALGORITHM_NAME[(*(main_indi_HD_decordComponents+(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))))->KIND_OF_BCH_ALGORITHM], 
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
								printf("[%s : #d calculateBCH_decodingAlgorithm start]\r\n", 
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
								printf("[%s : #d calculateBCH_decodingAlgorithm end]\r\n", 
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
					// printGaloisField2(main_com_used_GF->nonSorted, main_indi_HD_decordComponents[KIND_OF_BCH_DECODING_BM]->errLocPoly, PRINT_FIELD_EQUATION_TITLE, PRINT_FIELD_PRINT_OPTION_NUMBERING);
					printf(">> [ %s ]<<\r\n", KIND_OF_BCH_DECODING_ALGORITHM_NAME[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]);
					printGaloisField2(main_com_used_GF->nonSorted, main_indi_HD_decordComponents[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]->errLocPoly, PRINT_FIELD_EQUATION_TITLE, PRINT_FIELD_PRINT_OPTION_NUMBERING);

				}


				for(main_tmp_sel_decAlgo_i=0; main_tmp_sel_decAlgo_i<processingUseThisAlgorithm->length; main_tmp_sel_decAlgo_i++)
				{
					if(main_com_errNums_inDecodingAlgorithmTest > main_indi_hd_correctability[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))])
					{
						// if(main_indi_hd_correctability[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))] > (main_tmp_degErrLocPoly = checkDegreePolynomials(main_com_used_GF->nonSorted, main_indi_HD_decordComponents[KIND_OF_BCH_DECODING_BM]->errLocPoly)))
						// {
							// main_indi_errNumExceed_but_degErrLocPoly_not_max_cnt[KIND_OF_BCH_DECODING_BM]++;
							
							// warningMes; printf("Error number ('%d') larget than hard-decision correctability ('%d')\r\n", main_com_errNums_inDecodingAlgorithmTest, main_indi_hd_correctability[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i)));
							// warningMesShort; printf("Degree of error locaion polynomial is '%d'\r\n", main_tmp_degErrLocPoly);
							// if(!(global_flag_cmdSystemUnhold&FLAG_MASK_SYSTEM_UNHOLD_FORCED))
							// {
								// systemHoldMes;	printf("input any charactor : ");
								// scanf("%s", global_buf_KeyBoardInput);
							// }
						// }
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
								systemHoldMes;	printf("input any charactor : ");
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
					// chienSearch_static(
						// main_com_used_GF->nonSorted, 
						// main_indi_HD_decordComponents[(*(((uint8_t*)processingUseThisAlgorithm->list)+main_tmp_sel_decAlgo_i))]->errLocPoly, 
						// main_indi_HD_decordComponents[(*(((uint8_t*)processingUseThisAlgorithm->list)+main_tmp_sel_decAlgo_i))]->errorLocationVector, 
						// &(main_indi_HD_decordComponents[(*(((uint8_t*)processingUseThisAlgorithm->list)+main_tmp_sel_decAlgo_i))]->metricCheck) 
					// );
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
						// if(!(global_flag_cmdSystemUnhold&FLAG_MASK_SYSTEM_UNHOLD_FORCED))
						// {
							systemHoldMes;	printf("input any charactor : ");
							scanf("%s", global_buf_KeyBoardInput);
						// }
					}
				}
				/* verification of metric chk and deg of err loc poly end */
				
				/* !!! Only use BM algorithm errLocSyndrome and sumReceiveAndErrLocSyndrome !!! calculate syndrome of error location */
				// if(global_flag_bchDecAlgo_Enable&uint32_bitMask[KIND_OF_BCH_DECODING_BM])
				// {
					// if(global_bch_Soft_OverSyndLen)
					// {
								// #ifndef RELEASE
								// if(global_flag_debug_display&FLAG_MASK_DISPLAY_FUNCNAME)
								// {
									// debugFuncNameMes;
									// printf("[syndromeArrayCalculatorUsingString start]\r\n");
								// }
								// #endif	
						// syndromeArrayCalculatorUsingString
						// (
							// main_com_used_GF->nonSorted, 
							// ((struct_HD_BM_algorithmComponent*)main_indi_HD_decordComponents[KIND_OF_BCH_DECODING_BM])->errLocSyndrome, 
							// main_indi_HD_decordComponents[KIND_OF_BCH_DECODING_BM]->vMAX+global_bch_Soft_OverSyndLen, 
							// (char*)main_indi_HD_decordComponents[KIND_OF_BCH_DECODING_BM]->errorLocationVector->equation
						// );
								// #ifndef RELEASE
								// if(global_flag_debug_display&FLAG_MASK_DISPLAY_FUNCNAME)
								// {
									// debugFuncNameMes;
									// printf("[syndromeArrayCalculatorUsingString end]\r\n");
								// }
								// #endif	

								// #ifndef RELEASE
								// if(global_flag_debug_display&FLAG_MASK_DISPLAY_FUNCNAME)
								// {
									// debugFuncNameMes;
									// printf("[allSyndromeIsZero start]\r\n");
								// }
								// #endif	
						// if
						// (0 < allSyndromeIsZero
								// (
									// main_com_used_GF->nonSorted, 
									// ((struct_HD_BM_algorithmComponent*)main_indi_HD_decordComponents[KIND_OF_BCH_DECODING_BM])->errLocSyndrome
								// )
						// )
						// {
							// main_indi_list_flag_after_dec_synd_zero[KIND_OF_BCH_DECODING_BM][0]=1;
						// }
						// else
						// {
							// main_indi_list_flag_after_dec_synd_zero[KIND_OF_BCH_DECODING_BM][0]=0;
						// }
								// #ifndef RELEASE
								// if(global_flag_debug_display&FLAG_MASK_DISPLAY_FUNCNAME)
								// {
									// debugFuncNameMes;
									// printf("[allSyndromeIsZero end]\r\n");
								// }
								// #endif	
						// /*
						// if(main_com_errNums_inDecodingAlgorithmTest>main_indi_hd_correctability[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i)))
						// {
							// if(0<allSyndromeIsZero(main_com_used_GF->nonSorted, ((struct_HD_BM_algorithmComponent*)main_indi_HD_decordComponents[KIND_OF_BCH_DECODING_BM])->syndrome))
							// {
								// main_indi_errExceed_but_synd_zero_cnt[KIND_OF_BCH_DECODING_BM]++;
								
								// warningMes;
								// printf(" main_com_errNums_inDecodingAlgorithmTest = ('%d') > FLAG_CASE_DECODING_FAIL_METHOD_DEFAULT = ('%d')\r\n", main_com_errNums_inDecodingAlgorithmTest, main_indi_hd_correctability[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i)));
								// warningMesShort; printf("Syndrome value is zero is detected.\r\n");
								// warningMesShort; printf("This case will be serious problem in BM decoding algorithm.\r\n");
								// if(!(global_flag_cmdSystemUnhold&FLAG_MASK_SYSTEM_UNHOLD_FORCED))
								// {
									// systemHoldMes;	printf("input any charactor : ");
									// scanf("%s", global_buf_KeyBoardInput);
								// }
								// systemHoldMes;	printf("input : ");
								// scanf("%s", global_buf_KeyBoardInput);
							// }
						// }
						// */
								// #ifndef RELEASE
								// if(global_flag_debug_display&FLAG_MASK_DISPLAY_FUNCNAME)
								// {
									// debugFuncNameMes;
									// printf("[summationSyndromeArray start]\r\n");
								// }
								// #endif	
						// summationSyndromeArray
						// (
							// main_com_used_GF->nonSorted, 
							// ((struct_HD_BM_algorithmComponent*)main_indi_HD_decordComponents[KIND_OF_BCH_DECODING_BM])->sumReceiveAndErrLocSyndrome, 
							// main_indi_HD_decordComponents[KIND_OF_BCH_DECODING_BM]->syndrome, 
							// ((struct_HD_BM_algorithmComponent*)main_indi_HD_decordComponents[KIND_OF_BCH_DECODING_BM])->errLocSyndrome
						// );
								// #ifndef RELEASE
								// if(global_flag_debug_display&FLAG_MASK_DISPLAY_FUNCNAME)
								// {
									// debugFuncNameMes;
									// printf("[summationSyndromeArray end]\r\n");
								// }
								// #endif	
					// }

				// }

				
				
				for(main_tmp_sel_decAlgo_i=0; main_tmp_sel_decAlgo_i<processingUseThisAlgorithm->length; main_tmp_sel_decAlgo_i++)
				{
					infoMes; printf(">>> [ %s ] Syndrome of Received vector <<<\r\n", KIND_OF_BCH_DECODING_ALGORITHM_NAME[(*(((uint8_t*)processingUseThisAlgorithm->list)+main_tmp_sel_decAlgo_i))]);
					printSyndromeArray(main_indi_HD_decordComponents[(*(((uint8_t*)processingUseThisAlgorithm->list)+main_tmp_sel_decAlgo_i))]->syndrome, main_com_used_GF->nonSorted);

					/* !!! Only use BM algorithm errLocSyndrome and sumReceiveAndErrLocSyndrome !!! calculate syndrome of error location */
					// if((*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i)) == KIND_OF_BCH_DECODING_BM)
					// {
						// infoMes; printf(">>> [ %s : %d ] Syndrome of ErrorLocation vector <<<\r\n", KIND_OF_BCH_DECODING_ALGORITHM_NAME[(*(((uint8_t*)processingUseThisAlgorithm->list)+main_tmp_sel_decAlgo_i))], (*(((uint8_t*)processingUseThisAlgorithm->list)+main_tmp_sel_decAlgo_i)));
						// printSyndromeArray(((struct_HD_BM_algorithmComponent*)main_indi_HD_decordComponents[(*(((uint8_t*)processingUseThisAlgorithm->list)+main_tmp_sel_decAlgo_i))])->errLocSyndrome, main_com_used_GF->nonSorted);

						// infoMes; printf(">>> [ %s : %d ]sum Syndromes of Received and ErrorLocation vector <<<\r\n", KIND_OF_BCH_DECODING_ALGORITHM_NAME[(*(((uint8_t*)processingUseThisAlgorithm->list)+main_tmp_sel_decAlgo_i))], (*(((uint8_t*)processingUseThisAlgorithm->list)+main_tmp_sel_decAlgo_i)));
						// printSyndromeArray(((struct_HD_BM_algorithmComponent*)main_indi_HD_decordComponents[(*(((uint8_t*)processingUseThisAlgorithm->list)+main_tmp_sel_decAlgo_i))])->sumReceiveAndErrLocSyndrome, main_com_used_GF->nonSorted);
					// }

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

							systemHoldMes;	printf("input : ");
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
										systemHoldMes;	printf("input any charactor : ");
										scanf("%s", global_buf_KeyBoardInput);
									}
									// systemHoldMes;	printf("input : ");
									// scanf("%s", global_buf_KeyBoardInput);
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
					// closeBmAlgorithmComponent(((struct_HD_BM_algorithmComponent**)(main_indi_HD_decordComponents+KIND_OF_BCH_DECODING_BM)));
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
									infoMes;	printf("[%s] Error occur nums = %d\r\n", 
													KIND_OF_BCH_DECODING_ALGORITHM_NAME[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))], 
													main_com_errNums_inDecodingAlgorithmTest
												);
									infoMes;	printf("[%s] Error correctability = %d\r\n", 
													KIND_OF_BCH_DECODING_ALGORITHM_NAME[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))], 
													main_indi_hd_correctability[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]
												);

									infoMes;	printf("[%s] FAIL cnt) err exceed correctale and less errLoc poly degree = %ul\r\n", 
													KIND_OF_BCH_DECODING_ALGORITHM_NAME[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))], 
													main_indi_errNumExceed_but_degErrLocPoly_not_max_cnt[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]
												);
									infoMes;	printf("[%s] FAIL cnt) fail to correction = %ul\r\n", 
													KIND_OF_BCH_DECODING_ALGORITHM_NAME[(*(((uint8_t*)processingUseThisAlgorithm->list)+main_tmp_sel_decAlgo_i))], 
													main_indi_err_corection_fail_cnt[(*(((uint8_t*)processingUseThisAlgorithm->list)+main_tmp_sel_decAlgo_i))]
												);
									infoMes;	printf("[%s] FAIL cnt) Error num is exceed and synds is zero = %ul\r\n", 
													KIND_OF_BCH_DECODING_ALGORITHM_NAME[(*(((uint8_t*)processingUseThisAlgorithm->list)+main_tmp_sel_decAlgo_i))], 
													main_indi_errExceed_but_synd_zero_cnt[(*(((uint8_t*)processingUseThisAlgorithm->list)+main_tmp_sel_decAlgo_i))]
												);
									infoMes;	printf("[%s] FAIL cnt) Error num is exceed and sum synds and errLoc synd is zero = %ul\r\n", 
													main_indi_errExceed_but_syndSum_zero_cnt[(*(((uint8_t*)processingUseThisAlgorithm->list)+main_tmp_sel_decAlgo_i))]
												);
									infoMes;	printf("[%s] FAIL cnt) fail to correction and all synds are zero = %ul\r\n", 
													KIND_OF_BCH_DECODING_ALGORITHM_NAME[(*(((uint8_t*)processingUseThisAlgorithm->list)+main_tmp_sel_decAlgo_i))], 
													main_indi_err_detect_and_correct_fail_cnt[(*(((uint8_t*)processingUseThisAlgorithm->list)+main_tmp_sel_decAlgo_i))]
												);
									infoMes;	printf("[%s] FAIL cnt) Codeword loop %ul\r\n", 
													KIND_OF_BCH_DECODING_ALGORITHM_NAME[(*(((uint8_t*)processingUseThisAlgorithm->list)+main_tmp_sel_decAlgo_i))], 
													main_com_algorithm_loop_cnt
												);
									
									infoMes;	printf("[%s] FAIL percent) err exceed correctale and less errLoc poly degree = %g\r\n", 
													KIND_OF_BCH_DECODING_ALGORITHM_NAME[(*(((uint8_t*)processingUseThisAlgorithm->list)+main_tmp_sel_decAlgo_i))], 
													(((double)main_indi_errNumExceed_but_degErrLocPoly_not_max_cnt[(*(((uint8_t*)processingUseThisAlgorithm->list)+main_tmp_sel_decAlgo_i))])/((double)main_com_algorithm_loop_cnt))
												);
									infoMes;	printf("[%s] FAIL percent) fail to correction = %g\r\n", 
													KIND_OF_BCH_DECODING_ALGORITHM_NAME[(*(((uint8_t*)processingUseThisAlgorithm->list)+main_tmp_sel_decAlgo_i))], 
													(((double)main_indi_err_corection_fail_cnt[(*(((uint8_t*)processingUseThisAlgorithm->list)+main_tmp_sel_decAlgo_i))])/((double)main_com_algorithm_loop_cnt))
												);
									infoMes;	printf("[%s] FAIL percent) Error num is exceed and synds is zero = = %g\r\n", 
													KIND_OF_BCH_DECODING_ALGORITHM_NAME[(*(((uint8_t*)processingUseThisAlgorithm->list)+main_tmp_sel_decAlgo_i))], 
													(((double)main_indi_errExceed_but_synd_zero_cnt[(*(((uint8_t*)processingUseThisAlgorithm->list)+main_tmp_sel_decAlgo_i))])/((double)main_com_algorithm_loop_cnt))
												);
									infoMes;	printf("[%s] FAIL percent) Error num is exceed and sum synds and errLoc synd is zero = %g\r\n", 
													KIND_OF_BCH_DECODING_ALGORITHM_NAME[(*(((uint8_t*)processingUseThisAlgorithm->list)+main_tmp_sel_decAlgo_i))], 
													(((double)main_indi_errExceed_but_syndSum_zero_cnt[(*(((uint8_t*)processingUseThisAlgorithm->list)+main_tmp_sel_decAlgo_i))])/((double)main_com_algorithm_loop_cnt))
												);
									infoMes;	printf("[%s] FAIL percent) fail to correction and all synds are zero = %g\r\n", 
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
					Hard and Soft Decision is commonly use modulation of bpsk, 
					passing through AWGN and demodulation of bpsk.
					If simulation is Hard decision mode, demodulating use just demodulation of bpsk.
					And if simulation is Soft decision mode, demodulating use quantization of bpsk.
					[start]
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
	
				/*HARD DECISION*/
				/* ADD_AWGN(main_com_bpskComponents->bpskTransData, main_com_bpskComponents->bpskReceivedDataAddedAwgn, 1, main_com_codeLength, main_com_EbN0); */ //need to add bitrate
				/*SOFT DECISION*/
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
				); //need to add bitrate
				// getSqureRootAvrBpskReceivedDataAddedAwgn(main_com_bpskComponents);
				// getSqureRootAvrLLR(main_com_bpskComponents->bpskReceivedLLR);
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
				/*HARD DECISION*/
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
				/*SOFT DECISION*/
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
								/* convertQuantizedLLRToMagnitude(main_com_bpskComponents->bpskReceivedLLR);
										printMagnitudeOfLLR_andLocationWithSpaceAndEnter(main_com_bpskComponents->bpskReceivedLLR);
								
								sortMinimumMagnitudeLogLikeHoodRatio_chaseAlgorithm(main_com_bpskComponents->bpskReceivedLLR);
										testPrintValueOfTreeStruct(main_com_bpskComponents->bpskReceivedLLR->treeStruct); */
								
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
                            testPrintLLRWithSpaceAndEnter(
                                main_com_bpskComponents->bpskReceivedLLR);
							testPrintShortMinimumQuantizedLLRAndLocator(
                                main_com_bpskComponents->bpskReceivedLLR);
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
							// findMinimumMagnitudeFindLocPushAway(main_com_bpskComponents->bpskReceivedLLR);
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
					Hard and Soft Decision is commonly use modulation of bpsk, 
					passing through AWGN and demodulation of bpsk.
					If simulation is Hard decision mode, demodulating use just demodulation of bpsk.
					And if simulation is Soft decision mode, demodulating use quantization of bpsk.
					[start]
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
								printf("[Hard-Decision]\r\n", main_tmp_soft_i);
								// printf("%s\r\n", *(main_com_strBuf_TP_saved+0));
								printPowerFormWithEnterPolynomialWithEnterUsingAddress(main_com_list_TP_pwrFormPoly[0]);
								
								
								/*main_tmp_soft_i is index of main_com_strBuf_TP_saved.*/
								for(main_tmp_soft_i=1; main_tmp_soft_i<main_com_numsTP; main_tmp_soft_i++)
								{
									printf("[TestPattern %d]\r\n", main_tmp_soft_i);
									// printf("%s\r\n", *(main_com_strBuf_TP_saved+main_tmp_soft_i));
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
					note. counting different number encoded and passing through soft-decision codeword, all test pattern.
						Result of calculateHammingWeightFromDiffentPowerFormPolynomial is considered hard and soft decision correctability.
				*/
				
				
				switch(global_flag_case_selTP_before_dec)
				{
					case FLAG_CASE_SEL_TP_BEFORE_DEC_THEO:
					{
						for(main_tmp_sel_decAlgo_i=0; main_tmp_sel_decAlgo_i<processingUseThisAlgorithm->length; main_tmp_sel_decAlgo_i++)
						{
							/*
								sel correctable TP(main_indi_sel_TP_i) and to calculate BER, update err cnt(main_indi_sel_TP_errCnt)
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
						// errorMes; printf("Decoding processing skip mode in decoder error fail detection mode is not support yet.\r\n");
						// errorMes; printf("simulation is closed.\r\n");
						// return -1;
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
							note. belong to main_indi_sel_TP_errCnt[KIND_OF_BCH_DECODING_BM], decoding result(BER) can be changed.
								If main_indi_sel_TP_errCnt[KIND_OF_BCH_DECODING_BM] is exceed to correctable(hard decision) bits, 
								error bits are increased, reduced or no changed.
								Decording result is not predictable.
								
								Decoding target codeword is save at main_indi_sel_decoding_CW_pwrFormPoly[KIND_OF_BCH_DECODING_BM], variable type is struct_powerFormPolynomials*.

								Look decoding result, carefully.
								
								main_indi_sel_TP_errCnt[KIND_OF_BCH_DECODING_BM]
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
							// if(main_indi_sel_TP_errCnt[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))] <= main_indi_hd_correctability[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))])//try correcting
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
										notes. create new BCH decoding components are not needs.
											Removing this block needs to verify.
									*/
									/*
										note. To varifing unsigned int t, which is argument of recreateBmAlgorithmComponent,
										can change length of stage of BM algorithm.
										
										Error count(hamming weight of error vector) exceed it's correctability,
										simulator can be broken, because accessing invalid or wrong memory adress.
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
										notes. main_indi_SD_list_of_decordingComponents[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))] is a aaray, length is main_indi_sel_TP_i[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))].
										To decoding all test pattern, coded like below.
										
										for([var_cnt]=0; [var_cnt]<[max_TP_length]; [var_cnt]++)
										{
											*main_indi_SD_list_of_decordingComponents[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))][[var_cnt]] = recreateBmAlgorithmComponent
																	(
																		*(main_indi_SD_list_of_decordingComponents[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]+main_indi_sel_TP_i[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]), 
																		main_com_used_GF->nonSorted, main_com_hd_correctability, 
																		(char*)main_com_list_TP_pwrFormPoly[[var_cnt]]->equation,
																		main_com_codeLength
																	);
										}
									*/
									// **(main_indi_SD_list_of_decordingComponents[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]+main_indi_sel_TP_i[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]) = recreateBmAlgorithmComponent
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
									// calculateBmAlgorithm(
										// main_com_used_GF->nonSorted, 
										// ((struct_HD_BM_algorithmComponent*)(**(main_indi_SD_list_of_decordingComponents[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]+main_indi_sel_TP_i[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))])))
									// );
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
									// chienSearch_static(
										// main_com_used_GF->nonSorted, 
										// (**(main_indi_SD_list_of_decordingComponents[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]+main_indi_sel_TP_i[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]))->errLocPoly, 
										// (**(main_indi_SD_list_of_decordingComponents[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]+main_indi_sel_TP_i[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]))->errorLocationVector, 
										// &((**(main_indi_SD_list_of_decordingComponents[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]+main_indi_sel_TP_i[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]))->metricCheck)
									// );
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
									// for(main_tmp_soft_i=0; main_tmp_soft_i<main_com_numsTP; main_tmp_soft_i++)
									// {
										// main_indi_list_errCntOf_TP_afterDec[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))][main_tmp_soft_i] = 0;
									// }
									/* If use multiDecording Mode, needs to initial this end */
									/*
										 note. main_indi_sel_decoding_CW_pwrFormPoly[KIND_OF_BCH_DECODING_BM]
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
												if(!main_indi_list_errCntOf_TP_afterDec[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))][main_indi_sel_TP_i[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]])	printf("Success\r\n");
												else													printf("Fail\r\n");
											}
											#endif
									/*****************************************************************************/
									/****************** verification corrected codeword start ********************/
									/*****************************************************************************/



									
									/**************************************************/
									/****** close BCH decording components start ******/
									/**************************************************/
									/*
										notes. Not onlly create block, close new BCH decoding components are not needs.
											Removing this block needs to verify.
									*/
									// if(!(global_list_flag_bch_sim_decodingMode[KIND_OF_BCH_DECODING_BM]&FLAG_MASK_CODE_BCH_DEC_AL_SOFT_DECISION))
									// closeBmAlgorithmComponent((struct_HD_BM_algorithmComponent**)(*(main_indi_SD_list_of_decordingComponents[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]+main_indi_sel_TP_i[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))])));
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
					note. Actual error count.
					Actual is mean, simulator does not known that channel enviroments,
					that is mean simulator can not recognize error number corrrectly.
					So in below code and actual BER mode, 
					simulator can not choose best test pattern has minimum error number.
					
					Simulator have to identify best test pattern using decoding infomation.
					And choose best case.
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
								// calculateBmAlgorithm(
									// main_com_used_GF->nonSorted, 
									// ((struct_HD_BM_algorithmComponent*)(**(main_indi_SD_list_of_decordingComponents[KIND_OF_BCH_DECODING_BM]+main_tmp_soft_i)))
								// );
								calculateBCH_decodingAlgorithm(
									main_com_used_GF->nonSorted, 
									(**(main_indi_SD_list_of_decordingComponents[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]+main_tmp_soft_i))
								);
										#ifndef RELEASE
										if(global_flag_debug_display&FLAG_MASK_DISPLAY_FUNCNAME)
										{
											debugFuncNameMes;
											printf("[ACT] [%s] calculateBCH_decodingAlgorithm [end], sel TP #%d -> ADDR : 0x%x\n", 
												KIND_OF_BCH_DECODING_ALGORITHM_NAME[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))], 
												main_tmp_soft_i,
												(**(main_indi_SD_list_of_decordingComponents[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]+main_tmp_soft_i))

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
					
											infoMes; 	printf("[ACT] >>> TP(%d), Error location polynomial <<<\r\n", main_tmp_soft_i);
														printGaloisField2(
															main_com_used_GF->nonSorted, 
															(**(main_indi_SD_list_of_decordingComponents[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]+main_tmp_soft_i))->errLocPoly, 
															PRINT_FIELD_EQUATION_TITLE, 
															PRINT_FIELD_PRINT_OPTION_NUMBERING
														);
											infoMes;	printf("[ACT] degree of TP(%d) err loc poly is '%d'\r\n", 
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
										
					

								// chienSearch_static
									// (
										// main_com_used_GF->nonSorted, 
										// (**(main_indi_SD_list_of_decordingComponents[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]+main_tmp_soft_i))->errLocPoly, 
										// (**(main_indi_SD_list_of_decordingComponents[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]+main_tmp_soft_i))->errorLocationVector, 
										// &((**(main_indi_SD_list_of_decordingComponents[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]+main_tmp_soft_i))->metricCheck) 
									// );
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
											infoMes; 	printf("[ACT] [%s] Metric Check value of TP(%d) is '%d'\r\n", 
															KIND_OF_BCH_DECODING_ALGORITHM_NAME[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))],
															main_tmp_soft_i, 
															(**(main_indi_SD_list_of_decordingComponents[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]+main_tmp_soft_i))->metricCheck
														);
											infoMes;	printf("[ACT] [%s]Hamming weight of error loc of TP(%d) is '%d'\r\n", 
															KIND_OF_BCH_DECODING_ALGORITHM_NAME[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))],
															main_tmp_soft_i, 
															calculateHammingWeightPowerFormPolynomial((**(main_indi_SD_list_of_decordingComponents[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]+main_tmp_soft_i))->errorLocationVector)
														);
										}
										#endif
										
								/* calculate syndrome of error location (BM algorithme only) start */
								// if((*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i)) == KIND_OF_BCH_DECODING_BM)
								// {
									// syndromeArrayCalculatorUsingString
									// (
										// main_com_used_GF->nonSorted, 
										// ((struct_HD_BM_algorithmComponent*)(**(main_indi_SD_list_of_decordingComponents[KIND_OF_BCH_DECODING_BM]+main_tmp_soft_i)))->errLocSyndrome, 
										// (**(main_indi_SD_list_of_decordingComponents[KIND_OF_BCH_DECODING_BM]+main_tmp_soft_i))->vMAX+global_bch_Soft_OverSyndLen, 
										// (char*)(**(main_indi_SD_list_of_decordingComponents[KIND_OF_BCH_DECODING_BM]+main_tmp_soft_i))->errorLocationVector->equation
									// );
									// summationSyndromeArray
									// (
										// main_com_used_GF->nonSorted, 
										// ((struct_HD_BM_algorithmComponent*)(**(main_indi_SD_list_of_decordingComponents[KIND_OF_BCH_DECODING_BM]+main_tmp_soft_i)))->sumReceiveAndErrLocSyndrome, 
										// (**(main_indi_SD_list_of_decordingComponents[KIND_OF_BCH_DECODING_BM]+main_tmp_soft_i))->syndrome, 
										// ((struct_HD_BM_algorithmComponent*)(**(main_indi_SD_list_of_decordingComponents[KIND_OF_BCH_DECODING_BM]+main_tmp_soft_i)))->errLocSyndrome
									// );
									// if
									// (0 < allSyndromeIsZero
											// (
												// main_com_used_GF->nonSorted, 
												// ((struct_HD_BM_algorithmComponent*)(**(main_indi_SD_list_of_decordingComponents[KIND_OF_BCH_DECODING_BM]+main_tmp_soft_i)))->sumReceiveAndErrLocSyndrome
											// )
									// )
									// {
										// main_indi_list_flag_after_dec_synd_zero[KIND_OF_BCH_DECODING_BM][main_tmp_soft_i]=1;
									// }
									// else
									// {
										// main_indi_list_flag_after_dec_synd_zero[KIND_OF_BCH_DECODING_BM][main_tmp_soft_i]=0;
									// }
											// #ifndef RELEASE
											// if
											// (
												// (global_flag_case_getBER_method_Display&FLAG_MASK_DISPLAY_PROGRESS)||
												// (global_flag_debug_display&FLAG_MASK_DISPLAY_PROGRESS)||
												// (global_flag_bch_BM_SimSD_display&FLAG_MASK_DISPLAY_PROGRESS)||
												// (global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)
											// )
											// {
												// infoMes; printf("[ACT] [%s] AFTER) syndromes are zero? flag is %s, V='%d'\r\n", 
													// KIND_OF_BCH_DECODING_ALGORITHM_NAME[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))], 
													// (main_indi_list_flag_after_dec_synd_zero[KIND_OF_BCH_DECODING_BM][main_tmp_soft_i]?" ON":"OFF"), 
													// main_indi_list_flag_after_dec_synd_zero[KIND_OF_BCH_DECODING_BM][main_tmp_soft_i] 
												// );
												// infoMes; printf("[ACT] >>> Syndrome of Received vector <<<\r\n");
												// printSyndromeArray((**(main_indi_SD_list_of_decordingComponents[KIND_OF_BCH_DECODING_BM]+main_tmp_soft_i))->syndrome, main_com_used_GF->nonSorted);
												// infoMes; printf("[ACT] >>> Syndrome of ErrorLocation vector <<<\r\n");
												// printSyndromeArray(((struct_HD_BM_algorithmComponent*)(**(main_indi_SD_list_of_decordingComponents[KIND_OF_BCH_DECODING_BM]+main_tmp_soft_i)))->errLocSyndrome, main_com_used_GF->nonSorted);
												// infoMes; printf("[ACT] >>> sum Syndromes of Received and ErrorLocation vector <<<\r\n");
												// printSyndromeArray(((struct_HD_BM_algorithmComponent*)(**(main_indi_SD_list_of_decordingComponents[KIND_OF_BCH_DECODING_BM]+main_tmp_soft_i)))->sumReceiveAndErrLocSyndrome, main_com_used_GF->nonSorted);
											// }
											// #endif
								// }
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
								Correctable CodeWord Selection
								1. main_indi_list_flag_beforeDec_syndZero is true?
									-> received codeword has no err.
								2. main_indi_list_flag_deccFail is false?
									-> that is mean, metric chk is same with degree of errPoly.
							*/
							
						}
						

						
						
						
						
						/************************************************************/
						/****** check get BER actual mode fail condition start ******/
						/************************************************************/
						/*
						main_indi_list_flag_beforeDec_syndZero
						
						main_indi_list_flag_degOfErrLocPolyNotMax
						
						main_indi_list_flag_metricChk_degErrLocPolySame
						
						
						
						main_indi_sel_TP_i
						
						main_indi_list_errCntOf_TP_afterDec
						*/
						// for(main_tmp_sel_decAlgo_i = 0; main_tmp_sel_decAlgo_i < processingUseThisAlgorithm->length; main_tmp_sel_decAlgo_i++)
						// {	
							// main_indi_sel_TP_i[main_tmp_sel_decAlgo_i] = main_com_numsTP;
						// }
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
							// printf("test )))))AL[%s]:%d, selected TestPattern : %d =? %d, ADDR : 0x%x =? 0x%x\r\n", 
								// KIND_OF_BCH_DECODING_ALGORITHM_NAME[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))],
								// (*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i)),
								// *(main_indi_sel_TP_i+(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))), 
								// main_indi_sel_TP_i[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))],
								
								// (main_indi_sel_TP_i+(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))), 
								// &main_indi_sel_TP_i[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]
								// );
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
											infoMes;	printf("[ACT] [%s](%c)TP(%d) ",
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
												// if(!(global_flag_cmdSystemUnhold&FLAG_MASK_SYSTEM_UNHOLD_FORCED))
												// {
													systemHoldMes;	printf("input : ");
													scanf("%s", global_buf_KeyBoardInput);
												// }	
											}
										}

									}
																	if(!(global_flag_cmdSystemUnhold&FLAG_MASK_SYSTEM_UNHOLD_FORCED))
																	{
																		systemHoldMes;	printf("input : ");
																		scanf("%s", global_buf_KeyBoardInput);
																	}	
								
									infoMes; printf("[ACT] Minumum LLR location and Values\r\n");
									testPrintShortMinimumQuantizedLLRAndLocator(main_com_bpskComponents->bpskReceivedLLR);
									// printf("squreRootBpskReceivedDataAddedAwgn = '%g'\r\n", main_com_bpskComponents->squreRootBpskReceivedDataAddedAwgn);
									// printf("squreRootAvrLLR = '%g'\r\n", main_com_bpskComponents->bpskReceivedLLR->squreRootAvrLLR);

									printf("[ACT] >>>>> *************************************************************** <<<<<\r\n");
									infoMes; printf("[ACT] >>>>>/***** selection selected test pattern infomation. *****/<<<<<\r\n");
									//(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))
									for(main_tmp_sel_decAlgo_i=0; main_tmp_sel_decAlgo_i<processingUseThisAlgorithm->length; main_tmp_sel_decAlgo_i++)
									{	
										infoMes;	printf("[ACT] [%s] Selected test pattern is '#%d', TP(%d)\r\n", 
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
														printf("ADDR : 0x%x\r\n", (**(main_indi_SD_list_of_decordingComponents[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]+main_indi_sel_TP_i[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]))->errLocPoly);
														
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
										infoMes;	printf("[ACT] [%s] Selected TP '#%d', err distans before decoding of TP[%d] = %d\r\n", 
											KIND_OF_BCH_DECODING_ALGORITHM_NAME[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))], 
											main_indi_sel_TP_i[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))], 
											main_indi_sel_TP_i[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))], 
											main_indi_list_errCntOf_TP_beforeDec[main_indi_sel_TP_i[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]]
										);
										infoMes;	printf("[ACT] [%s] %s TP #%d Decording %d", 
											KIND_OF_BCH_DECODING_ALGORITHM_NAME[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))], 
											((global_list_flag_bch_sim_decodingMode[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]&FLAG_MASK_CODE_BCH_DEC_AL_SOFT_DECISION)?"[Soft-Decision]":"[Hard-Decision](TP)"), 
											main_indi_sel_TP_i[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]
										);
									}
									printf("\r\n");

									//(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))
									for(main_tmp_sel_decAlgo_i=0; main_tmp_sel_decAlgo_i<processingUseThisAlgorithm->length; main_tmp_sel_decAlgo_i++)
									{
										infoMes;	printf("[ACT] >>> Syndrome of Received vector <<<\r\n");
										printSyndromeArray(
											(**(main_indi_SD_list_of_decordingComponents[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]+main_indi_sel_TP_i[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]))->syndrome, 
											main_com_used_GF->nonSorted
										);
										
										/* errLocSyndrome is only valud in BM algorithm start */
										// infoMes;	printf("[ACT] >>> Syndrome of ErrorLocation vector <<<\r\n");
										// printSyndromeArray(
											// ((struct_HD_BM_algorithmComponent*)(**(main_indi_SD_list_of_decordingComponents[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]+main_indi_sel_TP_i[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))])))->errLocSyndrome, 
											// main_com_used_GF->nonSorted
										// );
										/* errLocSyndrome is only valud in BM algorithm end */
										
										/* sum syndromes of receive and err loc is only valid in BM algorithm start */
										// if((*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i)) == KIND_OF_BCH_DECODING_BM)
										// {
											// infoMes;	printf("[ACT] >>> sum Syndromes of Received and ErrorLocation vector <<<\r\n");
											// printSyndromeArray(
											// ((struct_HD_BM_algorithmComponent*)(**(main_indi_SD_list_of_decordingComponents[KIND_OF_BCH_DECODING_BM]+main_indi_sel_TP_i[KIND_OF_BCH_DECODING_BM])))->sumReceiveAndErrLocSyndrome, main_com_used_GF->nonSorted);

											// infoMes;	printf("[ACT] [%s] AFTER) syndromes are zero? flag is %s, V='%d'\r\n", 
												// KIND_OF_BCH_DECODING_ALGORITHM_NAME[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))], 
												// (main_indi_list_flag_after_dec_synd_zero[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))][main_indi_sel_TP_i[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]]?" ON":"OFF"), 
												// main_indi_list_flag_after_dec_synd_zero[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))][main_indi_sel_TP_i[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]] 
											// );
										// }									
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
									Corrected bits number is maximum(t=e+-p), choosing algorithm that correctable test-pattern can have some discrepancy.
									So check this, degreeOfErrLocPoly and main_com_hd_correctability are same.
									*/
									if( (**(main_indi_SD_list_of_decordingComponents[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]+main_indi_sel_TP_i[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]))->degreeOfErrLocPoly == main_indi_hd_correctability[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))] ) 
									{//check to discrepancy of select condition of correctable test pattern.
										/*
										chk from test pattern 0 to 2^p - 1 and start to 0.
										Because priority is higher than low numbered test pattern.
										*/
										for(main_tmp_soft_i=0; main_tmp_soft_i<main_com_numsTP; main_tmp_soft_i++)
										{
											if(main_indi_sel_TP_i[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))] == main_tmp_soft_i) continue;//except selected test pattern.
											if(main_indi_list_flag_theo_ly_can_be_correctable[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))][main_tmp_soft_i])
											{//check candidate testpattern is theoretically correctable? -> YES
												/*
												check that non selected test pattern is correctable?
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
																		systemHoldMes;	printf("input : ");
																		scanf("%s", global_buf_KeyBoardInput);
																	}
																}
																#endif
													}
													else
													{//Same condition with selected test-pattern? -> NO
														//can be passing
														//DO NOTHING...
														//DO NOTHING...
														//DO NOTHING...
													}
												}
												else
												{//check that non selected test pattern is correctable? -> NO
													//DO NOTHING...
													//DO NOTHING... 
													//DO NOTHING...
												}
											}
											else
											{//check candidate testpattern is theoretically correctable? -> NO
												//DO NOTHING...
												//DO NOTHING...
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
														
																// if(!(global_flag_cmdSystemUnhold&FLAG_MASK_SYSTEM_UNHOLD_FORCED))
																// {
																	// systemHoldMes;	printf("input : ");
																	// scanf("%s", global_buf_KeyBoardInput);								
																// }
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
															systemHoldMes;	printf("input : ");
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
													systemHoldMes;	printf("input : ");
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
									if exist correctable test pattern?
									from test pattern 0 to 2^p-1.
									*/
									for(main_tmp_soft_i=0; main_tmp_soft_i<main_com_numsTP; main_tmp_soft_i++)
									{
									/*
									theoretically correctable testpattern checker. if being correctable testpattern exist, break out for statements.
									if not exist it, main_tmp_soft_i is set to main_com_numsTP.
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
									theoretical and actual correctable test-patterns are exist.
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
														systemHoldMes;	printf("input : ");
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
															systemHoldMes;	printf("input : ");
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
										// main_indi_HD_errCnt[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))] = main_indi_list_errCntOf_TP_beforeDec[0];
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
							actual selected CW err cnt.
							one or more test pattern can try correction.
							*/
							if(!main_indi_list_flag_fail_allDec[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))])
							{
								/* Selected Test Pattern is not decoding fail, update sel test patterb error ent  */
								main_indi_sel_TP_errCnt[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))] = 
								main_indi_list_errCntOf_TP_afterDec[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))][main_indi_sel_TP_i[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]];
							}
							
							/*
							all test pattern can not try correction.
							selecting index change to test pattern 0.
							if all patterns are decoding fail, get error count error bits before error correction.
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
										
										// main_indi_sel_TP_errCnt[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))] = main_indi_list_errCntOf_TP_beforeDec[0];
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
								// closeBmAlgorithmComponent(((struct_HD_BM_algorithmComponent**)*(main_indi_SD_list_of_decordingComponents[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]+main_tmp_soft_i)));
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

											infoMes; printf("[ACT] [%s] algorithm sel TP[%d] distance of selTP = %d, updated errCnt = %d\r\n",
												KIND_OF_BCH_DECODING_ALGORITHM_NAME[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))],
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
													systemHoldMes;	printf("input : ");
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

				main_com_total_err_cnt_CH+=(unsigned long)main_com_channel_errCnt;	//printf("main_com_channel_errCnt %d\n", main_com_channel_errCnt);
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
				
				// printf(",M_ToT_CW_ER[%ld], ONE_CW_ER[%d]", main_indi_total_err_cnt_CW[KIND_OF_BCH_DECODING_BM], main_indi_sel_TP_errCnt[KIND_OF_BCH_DECODING_BM]);
				// printf("M_ToT_CH_ER[%ld], ONE_CH_ER%d", main_com_total_err_cnt_CH, main_com_channel_errCnt);
				// printf("M_ToT_BITs[%ld]\n", main_com_total_bit);

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
			resultMes;	printf("[%s], %s has = %ld bit errs\ttotal bit cnt = %ld\tchannel err bit cnt =%ld\r\n", 
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
			

			// main_indi_HD_BER[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))] = (double)main_indi_total_err_cnt_HD[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]/(double)main_com_total_bit;
			fprintf(fileio_indi_algorithm_coding_HD_err_bit_cnt[(*(((uint8_t*)processingUseThisAlgorithm->list)+main_tmp_sel_decAlgo_i))], "@%g, [%s(HD)] ERR Cnt : %ld\r\n",
				main_com_EbN0, 
				KIND_OF_BCH_DECODING_ALGORITHM_NAME[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))],  
				main_indi_total_err_cnt_HD[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]
			);
			
			// main_indi_BER[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))] = (double)main_indi_total_err_cnt_CW[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]/(double)main_com_total_bit;
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
/***************** PATTERN VERILOG FILE CLOSE *****************/
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
/***************** PATTERN VERILOG FILE CLOSE *****************/
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
			// closeBmAlgorithmComponent(((struct_HD_BM_algorithmComponent**)*(main_indi_SD_list_of_decordingComponents[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))]+main_indi_sel_TP_i[(*(((uint8_t*)(processingUseThisAlgorithm->list))+main_tmp_sel_decAlgo_i))])));
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
