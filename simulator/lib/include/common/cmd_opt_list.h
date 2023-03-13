#ifndef _COMMON_CMD_OPT_LIST_H_
#define _COMMON_CMD_OPT_LIST_H_

/* BCH Command Tree start */
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/************************************ bch_algorithm 4 t h - S t a r t ************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/***** extern const struct struct_cmdLineOption    bch_BM_algorithm_HD_option[] start *****/
    #ifndef RELEASE
    extern const struct struct_cmdLineOption    bch_BM_algorithm_HD_display_option[] =
    {
        {"result",  0, OPT_FLAG, 0, 0, &global_flag_bch_BM_SimHD_display, FLAG_MASK_DISPLAY_RESULT,  VALUE_TYPE_UNSIGNED_INT, NULL},
        {"progress", 0, OPT_FLAG, 0, 0, &global_flag_bch_BM_SimHD_display, FLAG_MASK_DISPLAY_PROGRESS, VALUE_TYPE_UNSIGNED_INT, NULL},
        {"funcname", 0, OPT_FLAG, 0, 0, &global_flag_bch_BM_SimHD_display, FLAG_MASK_DISPLAY_FUNCNAME, VALUE_TYPE_UNSIGNED_INT, NULL},
        {NULL,0,STRUCT_END,0,0,NULL,0,VALUE_TYPE_NONE,NULL}
    };
    #endif

    #ifndef RELEASE
    extern const struct struct_cmdLineOption    bch_BM_algorithm_SD_display_option[] =
    {
        {"result",  0, OPT_FLAG, 0, 0, &global_flag_bch_BM_SimSD_display, FLAG_MASK_DISPLAY_RESULT, VALUE_TYPE_UNSIGNED_INT, NULL},
        {"progress", 0, OPT_FLAG, 0, 0, &global_flag_bch_BM_SimSD_display, FLAG_MASK_DISPLAY_PROGRESS, VALUE_TYPE_UNSIGNED_INT, NULL},
        {"funcname", 0, OPT_FLAG, 0, 0, &global_flag_bch_BM_SimSD_display, FLAG_MASK_DISPLAY_FUNCNAME, VALUE_TYPE_UNSIGNED_INT, NULL},
        {NULL,0,STRUCT_END,0,0,NULL,0,VALUE_TYPE_NONE,NULL}
    };
    #endif
/***** extern const struct struct_cmdLineOption    bch_BM_algorithm_HD_option[] end *****/
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/************************************ bch_algorithm 4 t h - E n d ************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/************************************ bch_algorithm 3 r d - S t a r t ************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/***** extern const struct struct_cmdLineOption bch_BM_algorithm_option[] start *****/
extern const struct struct_cmdLineOption    bch_BM_algorithm_test_option[] =
{
    {"used", 0, OPT_FLAG, 0, 0, &global_list_flag_bch_sim_decodingMode[FLAG_MASK_BCH_DECORDING_ALGORITHM_BM], FLAG_MASK_CODE_BCH_DEC_AL_TEST_THIS_ALGORITHM, VALUE_TYPE_UNSIGNED_INT, NULL},//if(global_flag_bchBM_simulationMode&FLAG_MASK_CODE_BCH_DEC_AL_TEST_THIS_ALGORITHM)

    {NULL,0,STRUCT_END,0,0,NULL,0,VALUE_TYPE_NONE,NULL}
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern const struct struct_cmdLineOption    bch_BM_algorithm_decoding_option[] =
{
    {"skip", 0, OPT_FLAG, 0, 0, &global_list_flag_bch_sim_decodingMode[KIND_OF_BCH_DECODING_BM], FLAG_MASK_CODE_BCH_DEC_AL_SKIP, VALUE_TYPE_UNSIGNED_INT, NULL},

    {NULL,0,STRUCT_END,0,0,NULL,0,VALUE_TYPE_NONE,NULL}
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern const struct struct_cmdLineOption    bch_BM_algorithm_debug_calculate_option[] =
{
    {"delta", 0, OPT_FLAG, 0, 0, &global_flag_debug_bmSim, FLAG_MASK_DEBUG_BCH_BM_CAL_DELTA, VALUE_TYPE_UNSIGNED_INT, NULL},

    {NULL,0,STRUCT_END,0,0,NULL,0,VALUE_TYPE_NONE,NULL}
};

    #ifndef RELEASE
    extern const struct struct_cmdLineOption    bch_BM_algorithm_debug_option[] =
    {
        {"sequence", 0, OPT_FLAG, 0, 0, &global_flag_debug_bmSim, FLAG_MASK_DEBUG_BCH_BM_ALGORITHM, VALUE_TYPE_UNSIGNED_INT, NULL},
        {"cal", 0, OPT_COMPONENT, 0, 0, NULL, 0, VALUE_TYPE_NONE, bch_BM_algorithm_debug_calculate_option},

        {NULL,0,STRUCT_END,0,0,NULL,0,VALUE_TYPE_NONE,NULL}
    };
    #endif
/***** extern const struct struct_cmdLineOption bch_BM_algorithm_option[] end *****/
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/************************************ bch_algorithm 3 r d - E n d ************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/************************************ bch_algorithm 2 n d - S t a r t ************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/***** extern const struct struct_cmdLineOption bch_algorithm[] start *****/
extern const struct struct_cmdLineOption bch_BM_algorithm_option[] =
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
/***** extern const struct struct_cmdLineOption bch_algorithm[] end *****/
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/************************************ bch_algorithm 2 n d - E n d ************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    #ifndef RELEASE
    extern const struct struct_cmdLineOption    bch_mSBS_algorithm_HD_display_option[] =
    {
        {"result", 0, OPT_FLAG, 0, 0, &global_flag_bch_mSBS_SimHD_display, FLAG_MASK_DISPLAY_RESULT, VALUE_TYPE_UNSIGNED_INT, NULL},
        {"progress", 0, OPT_FLAG, 0, 0, &global_flag_bch_mSBS_SimHD_display, FLAG_MASK_DISPLAY_PROGRESS, VALUE_TYPE_UNSIGNED_INT, NULL},
        {"funcname", 0, OPT_FLAG, 0, 0, &global_flag_bch_mSBS_SimHD_display, FLAG_MASK_DISPLAY_FUNCNAME, VALUE_TYPE_UNSIGNED_INT, NULL},
        {NULL,0,STRUCT_END,0,0,NULL,0,VALUE_TYPE_NONE,NULL}
    };
    #endif

    #ifndef RELEASE
    extern const struct struct_cmdLineOption    bch_mSBS_algorithm_SD_display_option[] =
    {
        {"result", 0, OPT_FLAG, 0, 0, &global_flag_bch_mSBS_SimSD_display, FLAG_MASK_DISPLAY_RESULT, VALUE_TYPE_UNSIGNED_INT, NULL},
        {"progress", 0, OPT_FLAG, 0, 0, &global_flag_bch_mSBS_SimSD_display, FLAG_MASK_DISPLAY_PROGRESS, VALUE_TYPE_UNSIGNED_INT, NULL},
        {"funcname", 0, OPT_FLAG, 0, 0, &global_flag_bch_mSBS_SimSD_display, FLAG_MASK_DISPLAY_FUNCNAME, VALUE_TYPE_UNSIGNED_INT, NULL},
        {NULL,0,STRUCT_END,0,0,NULL,0,VALUE_TYPE_NONE,NULL}
    };
    #endif
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern const struct struct_cmdLineOption    bch_mSBS_algorithm_test_option[] =
{
    {"used", 0, OPT_FLAG, 0, 0, &global_list_flag_bch_sim_decodingMode[KIND_OF_BCH_DECODING_mSBS], FLAG_MASK_CODE_BCH_DEC_AL_TEST_THIS_ALGORITHM, VALUE_TYPE_UNSIGNED_INT, NULL},//if(global_list_flag_bch_sim_decodingMode[KIND_OF_BCH_DECODING_mSBS]&FLAG_MASK_CODE_BCH_DEC_AL_TEST_THIS_ALGORITHM)

    {NULL,0,STRUCT_END,0,0,NULL,0,VALUE_TYPE_NONE,NULL}
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern const struct struct_cmdLineOption    bch_mSBS_algorithm_decoding_option[] =
{
    {"skip", 0, OPT_FLAG, 0, 0, &global_list_flag_bch_sim_decodingMode[KIND_OF_BCH_DECODING_mSBS], FLAG_MASK_CODE_BCH_DEC_AL_SKIP, VALUE_TYPE_UNSIGNED_INT, NULL},

    {NULL,0,STRUCT_END,0,0,NULL,0,VALUE_TYPE_NONE,NULL}
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern const struct struct_cmdLineOption    bch_mSBS_algorithm_debug_calculate_option[] =
{
    {"delta", 0, OPT_FLAG, 0, 0, &global_flag_debug_mSBS_Sim, FLAG_MASK_DEBUG_BCH_M_SBS_CAL_DELTA, VALUE_TYPE_UNSIGNED_INT, NULL},

    {NULL,0,STRUCT_END,0,0,NULL,0,VALUE_TYPE_NONE,NULL}
};

    #ifndef RELEASE
    extern const struct struct_cmdLineOption    bch_mSBS_algorithm_debug_option[] =
    {
        {"sequence", 0, OPT_FLAG, 0, 0, &global_flag_debug_mSBS_Sim, FLAG_MASK_DEBUG_BCH_mSBS_ALGORITHM, VALUE_TYPE_UNSIGNED_INT, NULL},
        {"cal", 0, OPT_COMPONENT, 0, 0, NULL, 0, VALUE_TYPE_NONE, bch_mSBS_algorithm_debug_calculate_option},

        {NULL,0,STRUCT_END,0,0,NULL,0,VALUE_TYPE_NONE,NULL}
    };
    #endif
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern const struct struct_cmdLineOption bch_mSBS_algorithm_option[] =
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
/***** extern const struct struct_cmdLineOption struct_cmdLineOption[] start *****/
extern const struct struct_cmdLineOption bch_common[] =
{
    {"oversynd", 0, OPT_SET_VALUE, 0, 0, &global_bch_Soft_OverSyndLen, 0, VALUE_TYPE_UNSIGNED_INT, NULL},
    {"addlcm", 0, OPT_SET_VALUE, 0, 0, &global_addedRootToLCM_string, 0, VALUE_TYPE_STRING, NULL},

    {NULL,0,STRUCT_END,0,0,NULL,0,VALUE_TYPE_NONE,NULL}
};

extern const struct struct_cmdLineOption bch_algorithm[] =
{
    {"used", 0, OPT_FLAG, 0, 0, &global_flag_simulationCode, FLAG_MASK_CODE_BCH, VALUE_TYPE_UNSIGNED_INT, NULL},
    {"common", 0, OPT_COMPONENT, 0, 0, NULL, 0, VALUE_TYPE_NONE, bch_common},

    {"bm", 0, OPT_COMPONENT, 0, 0, NULL, 0, VALUE_TYPE_NONE, bch_BM_algorithm_option},
    {"msbs_t3", 0, OPT_COMPONENT, 0, 0, NULL, 0, VALUE_TYPE_NONE, bch_mSBS_algorithm_option},
    {NULL,0,STRUCT_END,0,0,NULL,0,VALUE_TYPE_NONE,NULL}
};
/***** extern const struct struct_cmdLineOption struct_cmdLineOption[] end *****/
/************************************ bch_algorithm 1 s t - e n d ************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    #ifndef RELEASE
    extern const struct struct_cmdLineOption    usable_code_GaloisField_display_option[] =
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

extern const struct struct_cmdLineOption usable_code_GaloisField[] =
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



extern const struct struct_cmdLineOption usaable_code_com_algorithm_option[] =
{
    {"errnum", 0, OPT_SET_VALUE, 0, 0, &global_ErrorNum, 0, VALUE_TYPE_UNSIGNED_INT, NULL},

    {NULL,0,STRUCT_END,0,0,NULL,0,VALUE_TYPE_NONE,NULL}
};

extern const struct struct_cmdLineOption    usaable_code_com_HD_option[] =
{
    {"correctable", 0, OPT_SET_VALUE, 0, 0, &global_bch_hardCorrectable, 0, VALUE_TYPE_UNSIGNED_INT, NULL},
        #ifndef RELEASE
        {"display", 0, OPT_COMPONENT, 0, 0, NULL, 0, VALUE_TYPE_NONE, bch_BM_algorithm_HD_display_option},
        #endif
    {NULL,0,STRUCT_END,0,0,NULL,0,VALUE_TYPE_NONE,NULL}
};

extern const struct struct_cmdLineOption usaable_code_com_HD_SD_option [] =
{
    {"hard", 0, OPT_SET_VALUES_SERIES, 0, 0, &global_bch_hardCorrectable, 0, VALUE_TYPE_UNSIGNED_INT, NULL},
    {"soft", 0, OPT_SET_VALUES_SERIES, 0, 0, &global_bch_SoftCorrectable, 0, VALUE_TYPE_UNSIGNED_INT, NULL},
    {NULL,0,STRUCT_END,0,0,NULL,0,VALUE_TYPE_NONE,NULL}
};


extern const struct struct_cmdLineOption    usaable_code_com_SD_option[] =
{
    {"correctable", 0, OPT_SET_VALUE, 0, 0, &global_bch_SoftCorrectable, 0, VALUE_TYPE_UNSIGNED_INT, NULL},
    {"correctables", 0, OPT_SET_VALUES, 0, 0, NULL, 0, VALUE_TYPE_NONE, usaable_code_com_HD_SD_option},

        #ifndef RELEASE
        {"display", 0, OPT_COMPONENT, 0, 0, NULL, 0, VALUE_TYPE_NONE, bch_BM_algorithm_SD_display_option},
        #endif
    {NULL,0,STRUCT_END,0,0,NULL,0,VALUE_TYPE_NONE,NULL}
};

extern const struct struct_cmdLineOption usable_code_common[] =
{
    {"gf", 0, OPT_COMPONENT, 0, 0, NULL, 0, VALUE_TYPE_NONE, usable_code_GaloisField},

    //added correctables
    {"algorithm", 0, OPT_COMPONENT, 0, 0, &global_flag_sim_decodingMode, FLAG_MASK_CODE_BCH_DEC_AL_TEST_THIS_ALGORITHM, VALUE_TYPE_UNSIGNED_INT, usaable_code_com_algorithm_option},

    {"hard", 0, OPT_COMPONENT, 0, 0, &global_flag_sim_decodingMode, FLAG_MASK_CODE_BCH_DEC_AL_HARD_DECISION, VALUE_TYPE_UNSIGNED_INT, usaable_code_com_HD_option},//if(global_list_flag_bch_sim_decodingMode[KIND_OF_BCH_DECODING_BM]&FLAG_MASK_CODE_BCH_DEC_AL_HARD_DECISION)
    {"soft", 0, OPT_COMPONENT, 0, 0, &global_flag_sim_decodingMode, FLAG_MASK_CODE_BCH_DEC_AL_SOFT_DECISION, VALUE_TYPE_UNSIGNED_INT, usaable_code_com_SD_option},//if(global_list_flag_bch_sim_decodingMode[KIND_OF_BCH_DECODING_BM]&FLAG_MASK_CODE_BCH_DEC_AL_SOFT_DECISION)

    {NULL,0,STRUCT_END,0,0,NULL,0,VALUE_TYPE_NONE,NULL}
};

extern const struct struct_cmdLineOption cmdOpt_usable_code[] =
{
    {"bch", 0, OPT_COMPONENT, 0, 0, &global_flag_simulationCode, FLAG_MASK_CODE_BCH, VALUE_TYPE_UNSIGNED_INT, bch_algorithm},

    {"common", 0, OPT_COMPONENT, 0, 0, NULL, 0, VALUE_TYPE_NONE, usable_code_common},

    {NULL,0,STRUCT_END,0,0,NULL,0,VALUE_TYPE_NONE,NULL}
};

/* BCH Command Tree end */
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern const struct struct_cmdLineOption comm_channel_awgn_ebn0_range[] =
{
    {"start", 0, OPT_SET_LANGE_SERIES, 0, 0, &global_EbN0_Start_Value, 0, VALUE_TYPE_DOUBLE, NULL},
    {"end", 0, OPT_SET_LANGE_SERIES, 0, 0, &global_EbN0_End_Value, 0, VALUE_TYPE_DOUBLE, NULL},
    {"step", 0, OPT_SET_LANGE_SERIES, 0, 0, &global_EbN0_Step, 0, VALUE_TYPE_DOUBLE, NULL},
    {"(not complete analysys argument of range, in partial processing)", 0, OPT_NOTHING, 0, 0, NULL, 0, VALUE_TYPE_NONE, NULL},
    {NULL,0,STRUCT_END,0,0,NULL,0,VALUE_TYPE_NONE,NULL}
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern const struct struct_cmdLineOption comm_channel_awgn_ebn0[] =
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
    extern const struct struct_cmdLineOption comm_channel_awgn_llr_debug[] =
    {
        {"sequence", 0, OPT_FLAG, 0, 0, &global_flag_debug_awgnLLR, FLAG_MASK_DEBUG_AWGN_LLR_SEQUENCE, VALUE_TYPE_UNSIGNED_INT, NULL},
        {"locator", 0, OPT_FLAG, 0, 0, &global_flag_debug_awgnLLR, FLAG_MASK_DEBUG_AWGN_LLR_LOCATOR, VALUE_TYPE_UNSIGNED_INT, NULL},
        {NULL,0,STRUCT_END,0,0,NULL,0,VALUE_TYPE_NONE,NULL}
    };
    #endif
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern const struct struct_cmdLineOption comm_ch_awgn_same_llr_mag_handling[] =
{
    {"none", 0, OPT_FLAG_CASE, 0, 0, &global_flag_case_same_llr_handling, FLAG_CASE_SAME_LLR_HANDLING_NONE, VALUE_TYPE_UNSIGNED_INT, NULL},
    {"not_sel", 0, OPT_FLAG_CASE, 0, 0, &global_flag_case_same_llr_handling, FLAG_CASE_SAME_LLR_HANDLING_NOT_SEL, VALUE_TYPE_UNSIGNED_INT, NULL},
    {"prior_0", 0, OPT_FLAG_CASE, 0, 0, &global_flag_case_same_llr_handling, FLAG_CASE_SAME_LLR_HANDLING_PRIOR_HD_0, VALUE_TYPE_UNSIGNED_INT, NULL},
    {"prior_1", 0, OPT_FLAG_CASE, 0, 0, &global_flag_case_same_llr_handling, FLAG_CASE_SAME_LLR_HANDLING_PRIOR_HD_1, VALUE_TYPE_UNSIGNED_INT, NULL},

    {"(not sel option is not work on tree)", 0, OPT_NOTHING, 0, 0, NULL, 0, VALUE_TYPE_NONE, NULL},
    {NULL,0,STRUCT_END,0,0,NULL,0,VALUE_TYPE_NONE,NULL}
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern const struct struct_cmdLineOption comm_ch_awgn_init_llr_mag_method[] =
{
    {"none", 0, OPT_FLAG_CASE, 0, 0, &global_flag_case_init_llr_mag_method, FLAG_CASE_INIT_LLR_MAG_METHOD_NONE, VALUE_TYPE_UNSIGNED_INT, NULL},
    {"not_sel_max_llr", 0, OPT_FLAG_CASE, 0, 0, &global_flag_case_init_llr_mag_method, FLAG_CASE_INIT_LLR_MAG_METHOD_NOT_SEL_MAX_LLR, VALUE_TYPE_UNSIGNED_INT, NULL},

    {"(not sel max option is not work on tree)", 0, OPT_NOTHING, 0, 0, NULL, 0, VALUE_TYPE_NONE, NULL},
    {NULL,0,STRUCT_END,0,0,NULL,0,VALUE_TYPE_NONE,NULL}
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern const struct struct_cmdLineOption comm_ch_awgn_llr_mag_find_meth_grouping[] =
{
    {"enable", 0, OPT_FLAG_CASE, 0, 0, &global_flag_case_find_LLR_method, FLAG_CASE_FINDING_MIN_LLR_METHOD_GROUPING, VALUE_TYPE_UNSIGNED_INT, NULL},
    {"group_num", 0, OPT_SET_VALUE, 0, 0, &global_grouping_stream_nums, 0, VALUE_TYPE_UNSIGNED_INT, NULL},
    {NULL,0,STRUCT_END,0,0,NULL,0,VALUE_TYPE_NONE,NULL}
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern const struct struct_cmdLineOption comm_ch_awgn_llr_mag_find_meth_tree[] =
{
    {"follow_min_path", 0, OPT_FLAG_CASE, 0, 0, &global_flag_case_find_LLR_method, FLAG_CASE_FINDING_MIN_LLR_METHOD_TREE_FOLLOWING_MIN1_PATH, VALUE_TYPE_UNSIGNED_INT, NULL},
    {"probabilistic_choosen", 0, OPT_FLAG_CASE, 0, 0, &global_flag_case_find_LLR_method, FLAG_CASE_FINDING_MIN_LLR_METHOD_TREE_PROBABILISIC_CHOOSEN, VALUE_TYPE_UNSIGNED_INT, NULL},
    //{"prior_HD_zero", 0, OPT_FLAG_CASE, 0, 0, &global_flag_case_find_LLR_method, FLAG_CASE_FINDING_MIN_LLR_METHOD_TREE_PRIOR_HD_ZERO, VALUE_TYPE_UNSIGNED_INT, NULL},
    {"(not complete a option of probabilistic_choosen)", 0, OPT_NOTHING, 0, 0, NULL, 0, VALUE_TYPE_NONE, NULL},
    {NULL,0,STRUCT_END,0,0,NULL,0,VALUE_TYPE_NONE,NULL}
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern const struct struct_cmdLineOption comm_channel_awgn_llr_mag_finding_meth[] =
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
extern const struct struct_cmdLineOption comm_channel_llr_quantiz_filter_set[] =
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
extern const struct struct_cmdLineOption comm_channel_llr_quantiz_numberic_system[] =
{
    {"twos_complement", 0, OPT_FLAG_CASE, 0, 0, &global_flag_case_llr_numberic_system, ENUM_FLAG_CASE_QUANTIZ_NUMBERIC_SYSTEM_TWO_S_COMPLEMENT, VALUE_TYPE_UNSIGNED_INT, NULL},
    {"ones_complement", 0, OPT_FLAG_CASE, 0, 0, &global_flag_case_llr_numberic_system, ENUM_FLAG_CASE_QUANTIZ_NUMBERIC_SYSTEM_ONE_S_COMPLEMENT, VALUE_TYPE_UNSIGNED_INT, NULL},

    {NULL,0,STRUCT_END,0,0,NULL,0,VALUE_TYPE_NONE,NULL}
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern const struct struct_cmdLineOption comm_channel_llr_quantiz_zero_symmetry[] =
{
    //{"disable", 0, OPT_FLAG_CASE, 0, 0, &global_flag_case_llr_mag_ones_complement_zero_handling, ENUM_FLAG_CASE_QUANTIZ_NUMBERIC_SYS_ONE_S_COM_NONE, VALUE_TYPE_UNSIGNED_INT, NULL},
    {"enbale", 0, OPT_FLAG_CASE, 0, 0, &global_flag_case_llr_mag_ones_complement_zero_handling, ENUM_FLAG_CASE_QUANTIZ_NUMBERIC_SYS_ONE_S_COM_ZERO_SYMMETRY, VALUE_TYPE_UNSIGNED_INT, NULL},

    {NULL,0,STRUCT_END,0,0,NULL,0,VALUE_TYPE_NONE,NULL}
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern const struct struct_cmdLineOption comm_channel_llr_quantiz_zero_duplicated[] =
{
    //{"disable", 0, OPT_FLAG_CASE, 0, 0, &global_flag_case_llr_mag_ones_complement_zero_handling, ENUM_FLAG_CASE_QUANTIZ_NUMBERIC_SYS_ONE_S_COM_NONE, VALUE_TYPE_UNSIGNED_INT, NULL},
    {"enbale", 0, OPT_FLAG_CASE, 0, 0, &global_flag_case_llr_mag_ones_complement_zero_handling, ENUM_FLAG_CASE_QUANTIZ_NUMBERIC_SYS_ONE_S_COM_ZERO_DUPLICATED, VALUE_TYPE_UNSIGNED_INT, NULL},

    {NULL,0,STRUCT_END,0,0,NULL,0,VALUE_TYPE_NONE,NULL}
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern const struct struct_cmdLineOption comm_channel_llr_quantization[] =
{
    {"filter_set", 0, OPT_COMPONENT, 0, 0, NULL, 0, VALUE_TYPE_NONE, comm_channel_llr_quantiz_filter_set},

    {"numberic_system", 0, OPT_COMPONENT, 0, 0, NULL, 0, VALUE_TYPE_NONE, comm_channel_llr_quantiz_numberic_system},
    {"zero_symmetry", 0, OPT_COMPONENT, 0, 0, NULL, 0, VALUE_TYPE_NONE, comm_channel_llr_quantiz_zero_symmetry},
    {"zero_duplicated", 0, OPT_COMPONENT, 0, 0, NULL, 0, VALUE_TYPE_NONE, comm_channel_llr_quantiz_zero_duplicated},

    {NULL,0,STRUCT_END,0,0,NULL,0,VALUE_TYPE_NONE,NULL}
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern const struct struct_cmdLineOption comm_channel_awgn_llr[] =
{
    {"quantization", 0, OPT_COMPONENT, 0, 0, NULL, 0, VALUE_TYPE_NONE, comm_channel_llr_quantization},

    {"min_val_finding_meth", 0, OPT_COMPONENT, 0, 0, NULL, 0, VALUE_TYPE_NONE, comm_channel_awgn_llr_mag_finding_meth},
    {NULL,0,STRUCT_END,0,0,NULL,0,VALUE_TYPE_NONE,NULL}
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    #ifndef RELEASE
    extern const struct struct_cmdLineOption comm_channel_awgn_debug[] =
    {
        {"sequence", 0, OPT_FLAG, 0, 0, &global_flag_debug_awgn, FLAG_MASK_DEBUG_AWGN_SEQUENCE, VALUE_TYPE_UNSIGNED_INT, NULL},
        {NULL,0,STRUCT_END,0,0,NULL,0,VALUE_TYPE_NONE,NULL}
    };
    #endif
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern const struct struct_cmdLineOption comm_channel_awgn[] =
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
extern const struct struct_cmdLineOption cmdOpt_comm_channel[] =
{
    {"awgn", 0, OPT_COMPONENT, 0, 0, &global_Channel_Mode, FLAG_MASK_CHANNEL_MODE_AWGN, VALUE_TYPE_UNSIGNED_INT, comm_channel_awgn},
    {NULL,0,STRUCT_END,0,0,NULL,0,VALUE_TYPE_NONE,NULL}
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/************************************ cmdOpt_comm_channel 1 s t - e n d ************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    #ifndef RELEASE
    extern const struct struct_cmdLineOption bpsk_modulation_display[] =
    {
        {"result", 0, OPT_FLAG, 0, 0, &global_flag_Modulation_Mode_BPSK_Display, FLAG_MASK_DISPLAY_RESULT, VALUE_TYPE_UNSIGNED_INT, NULL},
        {"progress", 0, OPT_FLAG, 0, 0, &global_flag_Modulation_Mode_BPSK_Display, FLAG_MASK_DISPLAY_PROGRESS, VALUE_TYPE_UNSIGNED_INT, NULL},
        {"funcname", 0, OPT_FLAG, 0, 0, &global_flag_Modulation_Mode_BPSK_Display, FLAG_MASK_DISPLAY_FUNCNAME, VALUE_TYPE_UNSIGNED_INT, NULL},

        {NULL,0,STRUCT_END,0,0,NULL,0,VALUE_TYPE_NONE,NULL}
    };
    #endif

extern const struct struct_cmdLineOption bpsk_modulation[] =
{
    {"used", 0, OPT_FLAG, 0, 0, &global_flag_Modulation_Mode, DEFAULT_MODULATION_MODE, VALUE_TYPE_UNSIGNED_INT, NULL},

        #ifndef RELEASE
        {"display", 0, OPT_COMPONENT, 0, 0, NULL, 0, VALUE_TYPE_NONE, bpsk_modulation_display},
        #endif
    {NULL,0,STRUCT_END,0,0,NULL,0,VALUE_TYPE_NONE,NULL}
};
extern const struct struct_cmdLineOption cmdOpt_comm_modulation[] =
{
    {"bpsk", 0, OPT_COMPONENT, 0, 0, NULL, 0, VALUE_TYPE_NONE, bpsk_modulation},
    {NULL,0,STRUCT_END,0,0,NULL,0,VALUE_TYPE_NONE,NULL}
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern const struct struct_cmdLineOption cmdOpt_sim_loop[] =
{
    //{"start", 0, OPT_SET_VALUE, 0, 0, &global_EbN0_Start_Value, 0, VALUE_TYPE_DOUBLE, NULL},

    {"min", 0, OPT_SET_VALUE, 0, 0, &global_minimum_loop, 0, VALUE_TYPE_UNSIGNED_LONG, NULL},
    {"base", 0, OPT_SET_VALUE, 0, 0, &global_base_of_loop, 0, VALUE_TYPE_UNSIGNED_LONG, NULL},
    {"rate", 0, OPT_SET_VALUE, 0, 0, &global_rate_of_loop, 0, VALUE_TYPE_UNSIGNED_INT, NULL},

    {NULL,0,STRUCT_END,0,0,NULL,0,VALUE_TYPE_NONE,NULL}
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern const struct struct_cmdLineOption cmdOpt_simulation[] =
{
    //{"start", 0, OPT_SET_VALUE, 0, 0, &global_EbN0_Start_Value, 0, VALUE_TYPE_DOUBLE, NULL},

    {"loop", 0, OPT_COMPONENT, 0, 0, NULL, 0, VALUE_TYPE_NONE, cmdOpt_sim_loop},

    {NULL,0,STRUCT_END,0,0,NULL,0,VALUE_TYPE_NONE,NULL}
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    #ifndef RELEASE
    extern const struct struct_cmdLineOption ber_actual_display[] =
    {
        {"result", 0, OPT_FLAG, 0, 0, &global_flag_case_getBER_method_Display, FLAG_MASK_DISPLAY_RESULT, VALUE_TYPE_UNSIGNED_INT, NULL},
        {"progress", 0, OPT_FLAG, 0, 0, &global_flag_case_getBER_method_Display, FLAG_MASK_DISPLAY_PROGRESS, VALUE_TYPE_UNSIGNED_INT, NULL},
        {"funcname", 0, OPT_FLAG, 0, 0, &global_flag_case_getBER_method_Display, FLAG_MASK_DISPLAY_FUNCNAME, VALUE_TYPE_UNSIGNED_INT, NULL},

        {NULL,0,STRUCT_END,0,0,NULL,0,VALUE_TYPE_NONE,NULL}
    };
    #endif

extern const struct struct_cmdLineOption ber_theo_decision[] =
{
    //{"(not-support)", 0, OPT_NOTHING, 0, 0, NULL, 0, VALUE_TYPE_NONE, NULL},
    {"allzero", 0, OPT_FLAG_CASE, 0, 0, &global_flag_case_choosingCodeWordMethod_inTheo, FLAG_CASE_CODEWORD_CHOOSEN_METHOD_IN_THEO_NO_ERR, VALUE_TYPE_UNSIGNED_INT, NULL},
    {"normal", 0, OPT_FLAG_CASE, 0, 0, &global_flag_case_choosingCodeWordMethod_inTheo, FLAG_CASE_CODEWORD_CHOOSEN_METHOD_IN_THEO_NORMAL, VALUE_TYPE_UNSIGNED_INT, NULL},
    {"best", 0, OPT_FLAG_CASE, 0, 0, &global_flag_case_choosingCodeWordMethod_inTheo, FLAG_CASE_CODEWORD_CHOOSEN_METHOD_IN_THEO_MIN_ERR, VALUE_TYPE_UNSIGNED_INT, NULL},
    {"worst", 0, OPT_FLAG_CASE, 0, 0, &global_flag_case_choosingCodeWordMethod_inTheo, FLAG_CASE_CODEWORD_CHOOSEN_METHOD_IN_THEO_MAX_ERR, VALUE_TYPE_UNSIGNED_INT, NULL},

    {NULL,0,STRUCT_END,0,0,NULL,0,VALUE_TYPE_NONE,NULL}
};



extern const struct struct_cmdLineOption ber_dec_decision_handing[] =
{
    {"none", 0, OPT_FLAG_CASE, 0, 0, &global_flag_case_dec_fail_handling, FLAG_CASE_DEC_FAIL_HANDLING_NONE, VALUE_TYPE_UNSIGNED_INT, NULL},
    {"retrans_no_err", 0, OPT_FLAG_CASE, 0, 0, &global_flag_case_dec_fail_handling, FLAG_CASE_DEC_FAIL_HANDLING_RE_TRANFER_FAIL_NO_MORE, VALUE_TYPE_UNSIGNED_INT, NULL},
    {"retrans", 0, OPT_FLAG_CASE, 0, 0, &global_flag_case_dec_fail_handling, FLAG_CASE_DEC_FAIL_HANDLING_RE_TRANFER_INTERATION, VALUE_TYPE_UNSIGNED_INT, NULL},

    {NULL,0,STRUCT_END,0,0,NULL,0,VALUE_TYPE_NONE,NULL}
};

extern const struct struct_cmdLineOption ber_dec_decision[] =
{
    //{"(not-support)", 0, OPT_NOTHING, 0, 0, NULL, 0, VALUE_TYPE_NONE, NULL},
    {"used", 0, OPT_FLAG_CASE, 0, 0, &global_flag_case_selTP_before_dec, FLAG_CASE_SEL_TP_BEFORE_DEC_ACT, VALUE_TYPE_UNSIGNED_INT, NULL},

    {"handling", 0, OPT_COMPONENT, 0, 0, &global_flag_case_selTP_before_dec, FLAG_CASE_SEL_TP_BEFORE_DEC_ACT, VALUE_TYPE_UNSIGNED_INT, ber_dec_decision_handing},

        #ifndef RELEASE
        {"display", 0, OPT_COMPONENT, 0, 0, NULL, 0, VALUE_TYPE_NONE, ber_actual_display},
        #endif

    {NULL,0,STRUCT_END,0,0,NULL,0,VALUE_TYPE_NONE,NULL}
};

extern const struct struct_cmdLineOption ber_options[] =
{
    //{"(not-support)", 0, OPT_NOTHING, 0, 0, NULL, 0, VALUE_TYPE_NONE, NULL},
    {"theoretical_decision", 0, OPT_COMPONENT, 0, 0, NULL, 0, VALUE_TYPE_NONE, ber_theo_decision},

    {"decoder_decision", 0, OPT_COMPONENT, 0, 0, &global_flag_case_selTP_before_dec, FLAG_CASE_SEL_TP_BEFORE_DEC_ACT, VALUE_TYPE_UNSIGNED_INT, ber_dec_decision},
    {NULL,0,STRUCT_END,0,0,NULL,0,VALUE_TYPE_NONE,NULL}
};
extern const struct struct_cmdLineOption cmdOpt_get_thing[] =
{
    //{"(not-support)", 0, OPT_NOTHING, 0, 0, NULL, 0, VALUE_TYPE_NONE, NULL},

    //{"ber", 0, OPT_FLAG, 0, 0, &global_flag_berOptions, FLAG_MASK_TEST_GET_BER, VALUE_TYPE_UNSIGNED_INT, NULL},
    {"ber", 0, OPT_COMPONENT, 0, 0, &global_flag_berOptions, FLAG_MASK_TEST_GET_BER, VALUE_TYPE_UNSIGNED_INT, ber_options},
    {NULL,0,STRUCT_END,0,0,NULL,0,VALUE_TYPE_NONE,NULL}
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern const struct struct_cmdLineOption pattern_io[] =
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
extern const struct struct_cmdLineOption pattern_io_log[] =
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
extern const struct struct_cmdLineOption pattern_io_verilog[] =
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
extern const struct struct_cmdLineOption cmdOpt_fileIO[] =
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
extern const struct struct_cmdLineOption cmdOpt_testMode_err_distri_llr_static_msg[] =
{
    {"zero_stream_ratio", 0, OPT_SET_VALUE, 0, 0, &tmp_testMode_loops_zero_bit_ratio, 0, VALUE_TYPE_DOUBLE_RATIO_LIMIT, NULL},

    {NULL,0,STRUCT_END,0,0,NULL,0,VALUE_TYPE_NONE,NULL}
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern const struct struct_cmdLineOption cmdOpt_testMode_err_distri_llr_msg_gen_meth[] =
{
    {"static", 0, OPT_FLAG_CASE, 0, 0, &global_flag_case_testMode_msg_gen_method, ENUM_CASE_TESTMODE_ERR_RATE_MSG_GEN_METH_STATIC, VALUE_TYPE_UNSIGNED_INT, cmdOpt_testMode_err_distri_llr_static_msg},
    {"rand", 0, OPT_FLAG_CASE, 0, 0, &global_flag_case_testMode_msg_gen_method, ENUM_CASE_TESTMODE_ERR_RATE_MSG_GEN_METH_RAND, VALUE_TYPE_UNSIGNED_INT, NULL},
    {NULL,0,STRUCT_END,0,0,NULL,0,VALUE_TYPE_NONE,NULL}
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern const struct struct_cmdLineOption cmdOpt_testMode_err_distri_decoding_meth[] =
{
    {"hard", 0, OPT_FLAG_CASE, 0, 0, &global_flag_case_testMode_decoding_method, ENUM_CASE_TEST_MODE_ERR_RATE_DECODING_METH_HD, VALUE_TYPE_UNSIGNED_INT, NULL},
    {"soft", 0, OPT_FLAG_CASE, 0, 0, &global_flag_case_testMode_decoding_method, ENUM_CASE_TEST_MODE_ERR_RATE_DECODING_METH_SD, VALUE_TYPE_UNSIGNED_INT, NULL},
    {NULL,0,STRUCT_END,0,0,NULL,0,VALUE_TYPE_NONE,NULL}
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern const struct struct_cmdLineOption cmdOpt_testMode_err_distribution[] =
{

    {"msg_gen_method", 0, OPT_COMPONENT, 0, 0, NULL, 0, VALUE_TYPE_NONE, cmdOpt_testMode_err_distri_llr_msg_gen_meth},
    {"decoding_method", 0, OPT_COMPONENT, 0, 0, NULL, 0, VALUE_TYPE_NONE, cmdOpt_testMode_err_distri_decoding_meth},
    {"tot_loops", 0, OPT_SET_VALUE, 0, 0, &tmp_testMode_loops, 0, VALUE_TYPE_UNSIGNED_LONG, NULL},
    {NULL,0,STRUCT_END,0,0,NULL,0,VALUE_TYPE_NONE,NULL}
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern const struct struct_cmdLineOption cmdOpt_testMode_prob_distribution[] =
{

    {"group_num", 0, OPT_SET_VALUE, 0, 0, &tmp_testMode_prob_distribution_group_nums, 0, VALUE_TYPE_UNSIGNED_LONG, NULL},

    {"politics_same_val", 0, OPT_FLAG_CASE, 0, 0, &tmp_testMode_prob_min_choosen_politics, ENUM_CASE_TEST_MODE_PROB_MIN_CHOOSE_SAME_VAL, VALUE_TYPE_UNSIGNED_INT, NULL},
    {"politics_low_val", 0, OPT_FLAG_CASE, 0, 0, &tmp_testMode_prob_min_choosen_politics, ENUM_CASE_TEST_MODE_PROB_MIN_CHOOSE_LOW_VAL, VALUE_TYPE_UNSIGNED_INT, NULL},

    {"tot_loops", 0, OPT_SET_VALUE, 0, 0, &tmp_testMode_loops, 0, VALUE_TYPE_UNSIGNED_LONG, NULL},
    {NULL,0,STRUCT_END,0,0,NULL,0,VALUE_TYPE_NONE,NULL}
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern const struct struct_cmdLineOption cmdOpt_testMode[] =
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
extern const struct struct_cmdLineOption struct_cmdLineOption[] =
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

#endif
