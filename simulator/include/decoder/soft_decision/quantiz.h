typedef signed int      s_int_QUANTIZ_DIGIT;
typedef unsigned int    u_int_QUANTIZ_MAGNITUDE_DIGIT;
typedef double          double_RATIONAL_NUMBER;
struct struct_quantizationInfo
{
    double_RATIONAL_NUMBER offset;
    double_RATIONAL_NUMBER step;
    double_RATIONAL_NUMBER coverage;
    double_RATIONAL_NUMBER criterion;

    unsigned char numsOfBits;

    s_int_QUANTIZ_DIGIT numberOfSteps;

    double_RATIONAL_NUMBER rangeMin;
    double_RATIONAL_NUMBER rangeMax;

    s_int_QUANTIZ_DIGIT quantizedIntMin;
    s_int_QUANTIZ_DIGIT quantizedIntMax;

    SIGNED_INT quatizing_bitMask;
    UNSIGNED_INT mag_bitMask;

    unsigned int numberic_system;
    unsigned char ones_zero_handling;
    unsigned char val_ones_zero_handling;
}typedef struct_quantizationInfo;

struct_quantizationInfo global_QuantizationInfo;

/*
//#define QUANTIZ_MODE_STATIC 0
//#define QUANTIZ_MODE_ADAPT  1
*/
