#include <stdio.h>

#include "util/bit_mask.h"
#include "common/cmd_opt_vars.h"

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
