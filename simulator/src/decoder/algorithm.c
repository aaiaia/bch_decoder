/* Constructor and Destructor */
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

char closeAlgoriehmComponent(struct_generalForm_algorithmComponent **p, enum KIND_OF_BCH_DECODING_ALGORITHM KIND_OF_BCH_ALGORITHM)
{
    switch(KIND_OF_BCH_ALGORITHM)
    {
        case KIND_OF_BCH_DECODING_BM:
            /* Example code in main function
            //closeBmAlgorithmComponent(((struct_HD_BM_algorithmComponent**)(main_indi_HD_decordComponents+KIND_OF_BCH_DECODING_BM)));
            */
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
