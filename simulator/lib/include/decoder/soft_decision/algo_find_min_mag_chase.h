#ifndef _DECODER_SOFT_DECISION_ALGO_FIND_MIN_MAG_CHASE_H_
#define _DECODER_SOFT_DECISION_ALGO_FIND_MIN_MAG_CHASE_H_

#define DIRECTION_LEFT    0
#define DIRECTION_RIGHT    1

char sortMinimumMagnitudeLogLikeHoodRatio_chaseAlgorithm(
    struct_logLikeHoodRatio *p,
    struct_powerFormPolynomials *hd_codeword,
    unsigned int zero_magnitude_sel_policy,
    unsigned int same_magnitude_sel_policy);
char findMinimumMagnitudeLogLikeHoodRatio_chaseAlgorithm_followMinPath(struct_logLikeHoodRatio *p);

#endif
