/* Log definition */
#define errorMes        printf("ERROR ) ")
#define warningMes      printf("/*********************************/\n/********** WARNING !!! **********/\n/*********************************/\nWARNING ) ")
#define warningMesShort printf("WARNING ) ")
#define infoMes         printf("Info ) ")

#define initMes         printf("Initial ) ")

#define resultMes       printf("Result ) ")

/* Flags for log*/
unsigned int global_flag_cmdOption=0;
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
    unsigned int global_tmp_i_memoryCheck=0;

/* Sysgem Hold command */
unsigned int global_flag_cmdSystemHold=0;
#define PRINTF_HELP_WORD    "h" //not used
#define FLAG_MASK_HOLD_HELP (1<<0)
#define systemHoldMes       printf("HOLD ) ")

/* forced Sysgem unhold command */
unsigned int global_flag_cmdSystemUnhold=0;
#define SYSTEM_UNHOLD_FORCED_WORD       "f" //not used
#define FLAG_MASK_SYSTEM_UNHOLD_FORCED  (1<<0)
#define systemUnholdMes                 printf("UNHOLD ) ")

/*Global debug option start*/
unsigned int global_flag_deBugOption=0;
#define DEBUG_DEBUG_WORD        "debug"
#define FLAG_MASK_DEBUG_DEBUG   (1<<0)
#define debugMes                printf("debug ) ");

    /*Display RESULT or Progress or Function name*/
    enum DEBUG_DISPLAY_MODE
    {
        FLAG_MASK_DISPLAY_RESULT    = (1<<0),
        FLAG_MASK_DISPLAY_PROGRESS  = (1<<1),
        FLAG_MASK_DISPLAY_FUNCNAME  = (1<<2),
    };

    unsigned int global_flag_debug_display=0;
    #define debugFuncNameMes    printf("debug-funcname ) ");
    #define debugSequenceMes    printf("debug-progress ) ");

    unsigned int global_flag_bch_BM_SimHD_display=0;
    #define infoMes printf("test BCH BM HARD ) ")

    unsigned int global_flag_bch_BM_SimSD_display=0;
    #define test_BM_SoftDisplayMes  printf("test SBCH BM SOFT DISPLAY ) ");

    unsigned int global_flag_bch_mSBS_SimHD_display=0;
    #define test_mSBS_HardMes   printf("test BCH mSBS HARD ) ");

    unsigned int global_flag_bch_mSBS_SimSD_display=0;
    #define test_mSBS_SoftMes   printf("test BCH mSBS SOFT ) ");
    #define test_mSBS_SoftDisplayMes    printf("test BCH mSBS  SOFT DISPLAY ) ");
/*Global debug option end*/

