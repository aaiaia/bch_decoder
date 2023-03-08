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
char findMinimumMagnitudeFindLocPushAway(struct_logLikeHoodRatio *p,struct_powerFormPolynomials* hd_codeword,unsigned int zero_magnitude_sel_policy,unsigned int same_magnitude_sel_policy,unsigned int init_magnitude_policy)
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
