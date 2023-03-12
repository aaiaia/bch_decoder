/* Log definition */
#define errorMes        printf("ERROR ) ")
#define warningMes      printf("/*********************************/\n/********** WARNING !!! **********/\n/*********************************/\nWARNING ) ")
#define warningMesShort printf("WARNING ) ")
#define infoMes         printf("Info ) ")

#define initMes         printf("Initial ) ")

#define resultMes       printf("Result ) ")

/* Flags for log*/
unsigned int global_flag_cmdOption = 0;
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
    unsigned int global_tmp_i_memoryCheck = 0;

/* Sysgem Hold command */
unsigned int global_flag_cmdSystemHold = 0;
#define PRINTF_HELP_WORD    "h" //not used
#define FLAG_MASK_HOLD_HELP (1<<0)
#define systemHoldMes       printf("HOLD ) ")

/* forced Sysgem unhold command */
unsigned int global_flag_cmdSystemUnhold = 0;
#define SYSTEM_UNHOLD_FORCED_WORD       "f" //not used
#define FLAG_MASK_SYSTEM_UNHOLD_FORCED  (1<<0)
#define systemUnholdMes                 printf("UNHOLD ) ")

    /* Algorithm debugging options start */
    /* BM Algorithm debug option start */
    unsigned int global_flag_debug_bmSim = 0;
    #define DEBUG_BCH_BM_ALGORITHM_WORD         "debug-bm"//OPT_DISPLAY_PROGRESS
    #define FLAG_MASK_DEBUG_BCH_BM_ALGORITHM    (1<<0)
    #define debugBmAlgorithmMes                 printf("debug-BM ) ");

    #define DEBUG_BCH_BM_CAL_DELTA_WORD         "debug-bm-cal-delta"//OPT_DISPLAY_PROGRESS
    #define FLAG_MASK_DEBUG_BCH_BM_CAL_DELTA    (1<<1)
    #define debugBmAlgorithmCalDeltaMes         printf("debug-BM-cal-delta ) ");
    /* BM Algorithm debug option end */

    /*mSBS Algorithm debug option start*/
    /* below variables could be unnecessary */
    unsigned int global_flag_debug_mSBS_Sim = 0;
    #define DEBUG_BCH_mSBS_ALGORITHM_WORD       "debug-mSBS"//OPT_DISPLAY_PROGRESS
    #define FLAG_MASK_DEBUG_BCH_mSBS_ALGORITHM  (1<<0)
    #define debug_mSBS_AlgorithmMes             printf("debug-mSBS ) ");

    #define DEBUG_BCH_M_SBS_CAL_DELTA_WORD      "debug-mSBS-cal-delta"//OPT_DISPLAY_PROGRESS
    #define FLAG_MASK_DEBUG_BCH_M_SBS_CAL_DELTA (1<<1)
    #define debug_mSBS_AlgorithmCalDeltaMes     printf("debug-mSBS-cal-delta ) ");
    /*mSBS Algorithm debug option end*/
    /* Algorithm debugging options end */

/*Global debug option start*/
unsigned int global_flag_deBugOption = 0; //not used
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

    unsigned int global_flag_debug_display = 0;
    #define debugFuncNameMes    printf("debug-funcname ) ");
    #define debugSequenceMes    printf("debug-progress ) ");

    unsigned int global_flag_bch_BM_SimHD_display = 0;
    #define infoMes printf("test BCH BM HARD ) ")

    unsigned int global_flag_bch_BM_SimSD_display = 0;
    #define test_BM_SoftDisplayMes  printf("test SBCH BM SOFT DISPLAY ) ");

    unsigned int global_flag_bch_mSBS_SimHD_display = 0;
    #define test_mSBS_HardMes   printf("test BCH mSBS HARD ) ");

    unsigned int global_flag_bch_mSBS_SimSD_display = 0;
    #define test_mSBS_SoftMes   printf("test BCH mSBS SOFT ) ");
    #define test_mSBS_SoftDisplayMes    printf("test BCH mSBS  SOFT DISPLAY ) ");

/*Log LikeHood Ratio debug option start*/
unsigned int global_flag_debug_awgnLLR = 0;
#define FLAG_MASK_DEBUG_AWGN_LLR_SEQUENCE   (1<<0)
#define debugAwgnLlrSequenceMes             printf("SEQUENCE LLR ) ");

#define FLAG_MASK_DEBUG_AWGN_LLR_LOCATOR    (1<<1)
#define debugAwgnLlrLocatorMes              printf("debug-LLR-Locator ) ");
/*Log LikeHood Ratio debug option end*/

/* Display Galois Field(GF) start */
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
/* Display Galois Field(GF) end */
/*Global debug option end*/

