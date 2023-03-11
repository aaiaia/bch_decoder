/* Log definition */
#define errorMes        printf("ERROR ) ")
#define warningMes      printf("/*********************************/\n/********** WARNING !!! **********/\n/*********************************/\nWARNING ) ")
#define warningMesShort printf("WARNING ) ")
#define infoMes         printf("Info ) ")

#define initMes         printf("Initial ) ")

#define resultMes       printf("Result ) ")

/* Flags for log*/
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
