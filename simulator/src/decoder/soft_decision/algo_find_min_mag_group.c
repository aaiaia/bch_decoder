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
