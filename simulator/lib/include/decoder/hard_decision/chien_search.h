#ifndef _DECODER_HARD_DECISION_CHIEN_SEARCH_H_
#define _DECODER_HARD_DECISION_CHIEN_SEARCH_H_

enum CHIEN_SEARCH_DIRECTION
{
    CHIEN_SEARCH_DIRECTION_BACKWARD = 0,
    CHIEN_SEARCH_DIRECTION_FORWARD = 1,
}typedef enum_CHIEN_SEARCH_DIRECTION;

char calculateChienSearchVariableSet
(
    struct_galoisFieldElements *galoisFields,
    struct_galoisFieldPolyForm **variableSet,
    struct_galoisFieldPolyForm *firstOrder,
    unsigned int orderLength
);
char calculateChienSearch_direction
(
    struct_galoisFieldElements *galoisFields,
    struct_galoisFieldElements *errLocPolynomial,
    struct_powerFormPolynomials *errorCorectableCodeWord,
    unsigned int *metricCheck,
    enum CHIEN_SEARCH_DIRECTION direction
);
char chienSearch_offset_direction
(
    struct_galoisFieldElements *galoisFields,
    struct_galoisFieldElements *errLocPolynomial,
    struct_powerFormPolynomials *errorLocationPolynomial,
    unsigned int *metricCheck,
    enum CHIEN_SEARCH_DIRECTION direction
);
char calculateChienSearch_static_backWard
(
    struct_galoisFieldElements *galoisFields,
    struct_galoisFieldElements *errLocPolynomial,
    struct_powerFormPolynomials *errorCorectableCodeWord,
    unsigned int *metricCheck
);
char chienSearch_static
(
    struct_galoisFieldElements *galoisFields,
    struct_galoisFieldElements *errLocPolynomial,
    struct_powerFormPolynomials *errorLocationPolynomial,
    unsigned int *metricCheck
);
char chienSearch
(
    struct_galoisFieldElements *galoisFields,
    struct_galoisFieldElements *errLocPolynomial,
    struct_powerFormPolynomials *errorLocationPolynomial,
    unsigned int *metricCheck,
    enum KIND_OF_BCH_DECODING_ALGORITHM KIND_OF_BCH_ALGORITHM
);

#endif
