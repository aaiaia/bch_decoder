#include "polynomial.h"
#include "llr.h"

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

char findMinimumMagnitudeLogLikeHoodRatio_chaseAlgorithm_followMinPath(struct_logLikeHoodRatio *p)
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
