struct struct_HD_mSBS_t3_algorithmComponent{
    enum KIND_OF_BCH_DECODING_ALGORITHM KIND_OF_BCH_ALGORITHM;

    struct_powerFormPolynomials *codeWord;
    struct_powerFormPolynomials *errorLocationVector;

    struct_galoisFieldElements *syndrome;

    struct_galoisFieldElements *errLocPoly;

    unsigned int numberOfCorrection;//In mSBS, maximum numberOfCorrection is 3. so this value is always fixed to 3.

    unsigned int vMAX;

    int degreeOfErrLocPoly;

    unsigned int metricCheck;

    /* A * alpha^1 + B * alpha^2 + C * alpha^3 + R = 0 */
    /* this polynomial is same with struct_galoisFieldElements *errLocPoly; */
    /*
     * refference coefficient is coefficient_of_alpha_three.
     *
     * If this value is 0, degree of err location polynomials is two.
     * That is mean error locations are two.
     *
     * Else this value is non 0,  degree of err location polynomials is three or more.
     */
    struct_galoisFieldPolyForm *coefficient_of_alpha_three;//(that is C)
    struct_galoisFieldPolyForm *coefficient_of_alpha_two;//(that is B)
    struct_galoisFieldPolyForm *coefficient_of_alpha_one;//(that is A)
    struct_galoisFieldPolyForm *coefficient_of_alpha_zero;//(that is R)

}typedef struct_HD_mSBS_t3_algorithmComponent;

/* Constructor and Desctructor */
struct_HD_mSBS_t3_algorithmComponent *create_mSBS_algorithmComponent(struct_galoisFieldElements *galoisFields, unsigned int t, char *string, unsigned int stringLength);
char close_mSBS_algorithmComponent(struct_HD_mSBS_t3_algorithmComponent **p);
struct_HD_mSBS_t3_algorithmComponent *recreate_mSBS_algorithmComponent(struct_HD_mSBS_t3_algorithmComponent **p, struct_galoisFieldElements *galoisFields, unsigned int t, char *string, unsigned int stringLength);

/* Operation */
char calculate_mSBS_algorithm(struct_galoisFieldElements *galoisFields, struct_HD_mSBS_t3_algorithmComponent *p);
