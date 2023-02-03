char calculateChienSearchVariableSet
(
    struct_galoisFieldElements *galoisFields,
    struct_galoisFieldPolyForm **variableSet,
    struct_galoisFieldPolyForm *firstOrder,
    unsigned int orderLength
)
{
    unsigned int i;

    #ifndef RELEASE
    if(!variableSet)
    {
        errorMes;
        printf("in calculateChienSearchVariable, struct_galoisFieldPolyForm **variableSet is NULL.\n");
        return -1;
    }
    if(!firstOrder)
    {
        errorMes;
        printf("in calculateChienSearchVariable, struct_galoisFieldPolyForm *firstOrder is NULL.\n");
        return -1;
    }
    #endif

    (*(variableSet+0))=firstOrder;
    if(orderLength<1)
    {
        return 0;
    }

    *(variableSet+1)=multiElementsInGF_returnAddr(galoisFields, (*(variableSet+0)), (*(variableSet+0)));
    if(orderLength<2)
    {
        return 0;
    }

    for(i=2; i<orderLength; i++)
    {
        *(variableSet+i)=multiElementsInGF_returnAddr(galoisFields, (*(variableSet+i-1)), (*(variableSet+0)));
    }
    return 0;
}

char calculateChienSearch_direction
(
    struct_galoisFieldElements *galoisFields,
    struct_galoisFieldElements *errLocPolynomial,
    struct_powerFormPolynomials *errorCorectableCodeWord,
    unsigned int *metricCheck,
    enum CHIEN_SEARCH_DIRECTION direction
)
{
    unsigned int i, j;

    struct_galoisFieldPolyForm **variableSet;

    struct_galoisFieldPolyForm *summationResult;

    struct_galoisFieldPolyForm *tmp_location;

    #ifndef RELEASE
    if(!galoisFields)
    {
        errorMes;
        printf("in calculateChienSearch_static_backWard, struct_galoisFieldElements *galoisFields is NULL.\n");
        return -1;
    }
    if(!errLocPolynomial)
    {
        errorMes;
        printf("in calculateChienSearch_static_backWard, struct_galoisFieldElements *errLocPolynomial is NULL.\n");
        return -1;
    }
    if(!errorCorectableCodeWord)
    {
        errorMes;
        printf("in calculateChienSearch_static_backWard, struct_powerFormPolynomials *errorCorectableCodeWord is NULL.\n");
        return -1;
    }
    #endif

            #ifndef RELEASE
            if((global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)==FLAG_MASK_PRINTF_LOG)
            {
                logMes;
                printf("in calculateChienSearch_static_backWard, formula errLocPolynomial is below...\n");
                printGaloisField2(galoisFields, errLocPolynomial, PRINT_FIELD_EQUATION_TITLE, PRINT_FIELD_PRINT_OPTION_NUMBERING);

                logMes;
                printf("Start from 1 to length of GF is \'%d\', errorCorectableCodeWord length is [full]\'%d\', [used]\'%d\'", galoisFields->length, errorCorectableCodeWord->length, errorCorectableCodeWord->usedLength);

            }
            #endif
    variableSet=(struct_galoisFieldPolyForm**)malloc(sizeof(struct_galoisFieldPolyForm*)*(errLocPolynomial->length-1));
    memset(variableSet, 0, sizeof(struct_galoisFieldPolyForm*)*(errLocPolynomial->length-1));

    for(i=1; i<errorCorectableCodeWord->usedLength+1; i++)
    {
        /* calculateChienSearchVariableSet is x , x^2, x^3, x^4 , etc... */
        if(direction)    tmp_location = (*(galoisFields->element+i));
        else             tmp_location = invertExponentialElementInGaloisField(galoisFields, *(galoisFields->element+i));
        #ifndef RELEASE
        if(calculateChienSearchVariableSet(galoisFields, variableSet, tmp_location, (errLocPolynomial->length-1)))
        {
            errorMes;
            printf("in calculateChienSearch_static_backWard, calculateChienSearchVariableSet is error.\n");
            return -1;
        }
        #else
        calculateChienSearchVariableSet(galoisFields, variableSet, tmp_location, (errLocPolynomial->length-1));
        #endif


                #ifndef RELEASE
                /* to confirm code start */
                if(global_flag_cmdOption&FLAG_MASK_PRINTF_CALCULATE_PROCESS)
                {
                    printf("a^%d\t", i-1);
                    printPolyForm(*(galoisFields->element+i));
                    printf("\t| -(exp)> |\ta^%d\t=a^%d\t", -(i-1), abstractIndexOfPowerFormInElementsOfGaloisFieldUsingIndex(galoisFields, invertExponentialElementInGaloisField(galoisFields, *(galoisFields->element+i)))-1);
                    printPolyForm(invertExponentialElementInGaloisField(galoisFields, *(galoisFields->element+i)));
                    printf("\t|\t");
                    for(j=0; j<(errLocPolynomial->length-1); j++)
                    {
                        printPolyForm(*(variableSet+j));
                        printf("\t");

                    }
                    printf("\n");
                }
                /* to confirm code end */
                #endif

                #ifndef RELEASE
                if((global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)==FLAG_MASK_PRINTF_LOG)
                {
                    logMes;
                    printf("<< variableSet >>\n");
                    printf("a^-%d = a^%d => ",
                        abstractIndexOfPowerFormInElementsOfGaloisFieldUsingIndex(galoisFields, *(galoisFields->element+i))-1,
                        abstractIndexOfPowerFormInElementsOfGaloisFieldUsingIndex(galoisFields, invertExponentialElementInGaloisField(galoisFields, *(galoisFields->element+i)))-1
                        );
                    for(j=0; j<(errLocPolynomial->length-1); j++)
                    {
                        printf("(j=%d)a^%d(a^-%d*%d)\n", j,
                            abstractIndexOfPowerFormInElementsOfGaloisFieldUsingIndex(galoisFields, *(variableSet+j))-1,
                            abstractIndexOfPowerFormInElementsOfGaloisFieldUsingIndex(galoisFields, *(galoisFields->element+i))-1,
                            j
                            );
                    }
                    printf("\n");
                }
                #endif

                #ifndef RELEASE
                if((global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)==FLAG_MASK_PRINTF_LOG)
                {
                    logMes;
                    printf("i=%d, A( a^-%d ) = \t", i, abstractIndexOfPowerFormInElementsOfGaloisFieldUsingIndex(galoisFields, *(galoisFields->element+i))-1);
                }
                #endif

        summationResult=*(errLocPolynomial->element+0);
                #ifndef RELEASE
                if((global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)==FLAG_MASK_PRINTF_LOG)
                {
                    logMes;
                    printf("a^-%d + \t", abstractIndexOfPowerFormInElementsOfGaloisFieldUsingIndex(galoisFields, summationResult)-1);
                }
                #endif

        /* ErrorLocPolynomials(errLocPolynomial) = 1 + ramda_0*errLocPolynomial^1 + ramda_0*errLocPolynomial^2 + ramda_0*errLocPolynomial^3 + ramda_0*errLocPolynomial^4 + ramda_0*errLocPolynomial^5 + ramda_0*errLocPolynomial^6 + ... */
        for(j=1; j<(errLocPolynomial->length); j++)
        {
            summationResult=sumElementInGF_usingSumMatrixReturnAddr(galoisFields, summationResult, multiElementsInGF_returnAddr(galoisFields, *(errLocPolynomial->element+j), *(variableSet+j-1)));

                    #ifndef RELEASE
                    if((global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)==FLAG_MASK_PRINTF_LOG)
                    {
                        logMes;
                        printf("a^%d *\ta^%d(a^(-%d*%d)) ",
                        abstractIndexOfPowerFormInElementsOfGaloisFieldUsingIndex(galoisFields, *(errLocPolynomial->element+j))-1,
                        abstractIndexOfPowerFormInElementsOfGaloisFieldUsingIndex(galoisFields, *(variableSet+j-1))-1,
                        abstractIndexOfPowerFormInElementsOfGaloisFieldUsingIndex(galoisFields, *(galoisFields->element+i))-1,
                        j);

                        if(j!=(errLocPolynomial->length)-1)
                        {
                            printf("+\t");
                        }
                        else
                        {
                            printf("\t");
                        }
                    }
                    #endif
        }

        if(checkValueFromPolyFormUsingGaloisFieldValueUsingIntValue_(*(galoisFields->element+0), summationResult))
        {
                    #ifndef RELEASE
                    if((global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)==FLAG_MASK_PRINTF_LOG)
                    {
                        printf("<success>\t");
                    }
                    #endif
            *(errorCorectableCodeWord->equation+(i-1))='1';
            if(metricCheck) (*metricCheck)++;
        }
        else
        {
                    #ifndef RELEASE
                    if((global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)==FLAG_MASK_PRINTF_LOG)
                    {
                        printf("<fail>\t");
                    }
                    #endif
            *(errorCorectableCodeWord->equation+(i-1))='0';
        }


                #ifndef RELEASE
                if((global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)==FLAG_MASK_PRINTF_LOG)
                {
                    printf("= a^%d\t", abstractIndexOfPowerFormInElementsOfGaloisFieldUsingIndex(galoisFields, summationResult)-1);
                    printf(" (%c)\n", *(errorCorectableCodeWord->equation+(i-1)));
                }
                #endif
    }

            #ifndef RELEASE
            if((global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)==FLAG_MASK_PRINTF_LOG)
            {
                logMes;
                printf("correctable codeWord \"%s\"\n", errorCorectableCodeWord->equation);
                printf("variableSet addr(0x%lx)\n", (unsigned long)variableSet);
            }
            #endif
    free(variableSet);
    return 0;
}

char chienSearch_offset_direction
(
    struct_galoisFieldElements *galoisFields,
    struct_galoisFieldElements *errLocPolynomial,
    struct_powerFormPolynomials *errorLocationPolynomial,
    unsigned int *metricCheck,
    enum CHIEN_SEARCH_DIRECTION direction
)
{
    #ifndef RELEASE
    if(!galoisFields)
    {
        errorMes;
        printf("in chienSearch_offset_direction, struct_galoisFieldElements *galoisFields is NULL.\n");
        return -1;
    }
    if(!errLocPolynomial)
    {
        errorMes;
        printf("in chienSearch_offset_direction, struct_HD_BM_algorithmComponent *errLocPolynomial is NULL.\n");
        return -1;
    }
    #endif

    #ifndef RELEASE
    if(calculateChienSearch_direction(galoisFields, errLocPolynomial, errorLocationPolynomial, metricCheck, direction))
    {
        errorMes;
        printf("in chienSearch_offset_direction, calculateChienSearch_direction is fail.\n");
        return -1;
    }
    #else
    calculateChienSearch_direction(galoisFields, errLocPolynomial, errorLocationPolynomial, metricCheck, direction);
    #endif

            #ifndef RELEASE
            if((global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)==FLAG_MASK_PRINTF_LOG)
            {
                logMes;
                printf("in chienSearch_offset_direction, calculated errorLocationPolynomial is below : \n");
                printPowerFormWithEnterPolynomialAndInfoUsingAddress(errorLocationPolynomial);
            }
            #endif

    return 0;
}

char calculateChienSearch_static_backWard
(
    struct_galoisFieldElements *galoisFields,
    struct_galoisFieldElements *errLocPolynomial,
    struct_powerFormPolynomials *errorCorectableCodeWord,
    unsigned int *metricCheck
)
{
    unsigned int i, j;

    struct_galoisFieldPolyForm **variableSet;

    struct_galoisFieldPolyForm *summationResult;

    #ifndef RELEASE
    if(!galoisFields)
    {
        errorMes;
        printf("in calculateChienSearch_static_backWard, struct_galoisFieldElements *galoisFields is NULL.\n");
        return -1;
    }
    if(!errLocPolynomial)
    {
        errorMes;
        printf("in calculateChienSearch_static_backWard, struct_galoisFieldElements *errLocPolynomial is NULL.\n");
        return -1;
    }
    if(!errorCorectableCodeWord)
    {
        errorMes;
        printf("in calculateChienSearch_static_backWard, struct_powerFormPolynomials *errorCorectableCodeWord is NULL.\n");
        return -1;
    }
    #endif

            #ifndef RELEASE
            if((global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)==FLAG_MASK_PRINTF_LOG)
            {
                logMes;
                printf("in calculateChienSearch_static_backWard, formula errLocPolynomial is below...\n");
                printGaloisField2(galoisFields, errLocPolynomial, PRINT_FIELD_EQUATION_TITLE, PRINT_FIELD_PRINT_OPTION_NUMBERING);

                logMes;
                printf("Start from 1 to length of GF is \'%d\', errorCorectableCodeWord length is [full]\'%d\', [used]\'%d\'", galoisFields->length, errorCorectableCodeWord->length, errorCorectableCodeWord->usedLength);

            }
            #endif
    variableSet=(struct_galoisFieldPolyForm**)malloc(sizeof(struct_galoisFieldPolyForm*)*(errLocPolynomial->length-1));
    memset(variableSet, 0, sizeof(struct_galoisFieldPolyForm*)*(errLocPolynomial->length-1));

    for(i=1; i<errorCorectableCodeWord->usedLength+1; i++)
    {
        /* calculateChienSearchVariableSet is x , x^2, x^3, x^4 , etc... */
        #ifndef RELEASE
        if(calculateChienSearchVariableSet(galoisFields, variableSet, invertExponentialElementInGaloisField(galoisFields, *(galoisFields->element+i)), (errLocPolynomial->length-1)))
        {
            errorMes;
            printf("in calculateChienSearch_static_backWard, calculateChienSearchVariableSet is error.\n");
            return -1;
        }
        #else
        calculateChienSearchVariableSet(galoisFields, variableSet, invertExponentialElementInGaloisField(galoisFields, *(galoisFields->element+i)), (errLocPolynomial->length-1));
        #endif


                #ifndef RELEASE
                /* to confirm code start */
                if(global_flag_cmdOption&FLAG_MASK_PRINTF_CALCULATE_PROCESS)
                {
                    printf("a^%d\t", i-1);
                    printPolyForm(*(galoisFields->element+i));
                    printf("\t| -(exp)> |\ta^%d\t=a^%d\t", -(i-1), abstractIndexOfPowerFormInElementsOfGaloisFieldUsingIndex(galoisFields, invertExponentialElementInGaloisField(galoisFields, *(galoisFields->element+i)))-1);
                    printPolyForm(invertExponentialElementInGaloisField(galoisFields, *(galoisFields->element+i)));
                    printf("\t|\t");
                    for(j=0; j<(errLocPolynomial->length-1); j++)
                    {
                        printPolyForm(*(variableSet+j));
                        printf("\t");

                    }
                    printf("\n");
                }
                /* to confirm code end */
                #endif

                #ifndef RELEASE
                if((global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)==FLAG_MASK_PRINTF_LOG)
                {
                    logMes;
                    printf("<< variableSet >>\n");
                    printf("a^-%d = a^%d => ",
                        abstractIndexOfPowerFormInElementsOfGaloisFieldUsingIndex(galoisFields, *(galoisFields->element+i))-1,
                        abstractIndexOfPowerFormInElementsOfGaloisFieldUsingIndex(galoisFields, invertExponentialElementInGaloisField(galoisFields, *(galoisFields->element+i)))-1
                        );
                    for(j=0; j<(errLocPolynomial->length-1); j++)
                    {
                        printf("(j=%d)a^%d(a^-%d*%d)\n", j,
                            abstractIndexOfPowerFormInElementsOfGaloisFieldUsingIndex(galoisFields, *(variableSet+j))-1,
                            abstractIndexOfPowerFormInElementsOfGaloisFieldUsingIndex(galoisFields, *(galoisFields->element+i))-1,
                            j
                            );
                    }
                    printf("\n");
                }
                #endif

                #ifndef RELEASE
                if((global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)==FLAG_MASK_PRINTF_LOG)
                {
                    logMes;
                    printf("i=%d, A( a^-%d ) = \t", i, abstractIndexOfPowerFormInElementsOfGaloisFieldUsingIndex(galoisFields, *(galoisFields->element+i))-1);
                }
                #endif

        summationResult=*(errLocPolynomial->element+0);
                #ifndef RELEASE
                if((global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)==FLAG_MASK_PRINTF_LOG)
                {
                    logMes;
                    printf("a^-%d + \t", abstractIndexOfPowerFormInElementsOfGaloisFieldUsingIndex(galoisFields, summationResult)-1);
                }
                #endif

        /* ErrorLocPolynomials(errLocPolynomial) = 1 + ramda_0*errLocPolynomial^1 + ramda_0*errLocPolynomial^2 + ramda_0*errLocPolynomial^3 + ramda_0*errLocPolynomial^4 + ramda_0*errLocPolynomial^5 + ramda_0*errLocPolynomial^6 + ... */
        for(j=1; j<(errLocPolynomial->length); j++)
        {
            summationResult=sumElementInGF_usingSumMatrixReturnAddr(galoisFields, summationResult, multiElementsInGF_returnAddr(galoisFields, *(errLocPolynomial->element+j), *(variableSet+j-1)));

                    #ifndef RELEASE
                    if((global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)==FLAG_MASK_PRINTF_LOG)
                    {
                        logMes;
                        printf("a^%d *\ta^%d(a^(-%d*%d)) ",
                        abstractIndexOfPowerFormInElementsOfGaloisFieldUsingIndex(galoisFields, *(errLocPolynomial->element+j))-1,
                        abstractIndexOfPowerFormInElementsOfGaloisFieldUsingIndex(galoisFields, *(variableSet+j-1))-1,
                        abstractIndexOfPowerFormInElementsOfGaloisFieldUsingIndex(galoisFields, *(galoisFields->element+i))-1,
                        j);

                        if(j!=(errLocPolynomial->length)-1)
                        {
                            printf("+\t");
                        }
                        else
                        {
                            printf("\t");
                        }
                    }
                    #endif
        }

        if(checkValueFromPolyFormUsingGaloisFieldValueUsingIntValue_(*(galoisFields->element+0), summationResult))
        {
                    #ifndef RELEASE
                    if((global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)==FLAG_MASK_PRINTF_LOG)
                    {
                        printf("<success>\t");
                    }
                    #endif
            *(errorCorectableCodeWord->equation+(i-1))='1';
            if(metricCheck) (*metricCheck)++;
        }
        else
        {
                    #ifndef RELEASE
                    if((global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)==FLAG_MASK_PRINTF_LOG)
                    {
                        printf("<fail>\t");
                    }
                    #endif
            *(errorCorectableCodeWord->equation+(i-1))='0';
        }


                #ifndef RELEASE
                if((global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)==FLAG_MASK_PRINTF_LOG)
                {
                    printf("= a^%d\t", abstractIndexOfPowerFormInElementsOfGaloisFieldUsingIndex(galoisFields, summationResult)-1);
                    printf(" (%c)\n", *(errorCorectableCodeWord->equation+(i-1)));
                }
                #endif
    }

            #ifndef RELEASE
            if((global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)==FLAG_MASK_PRINTF_LOG)
            {
                logMes;
                printf("correctable codeWord \"%s\"\n", errorCorectableCodeWord->equation);
                printf("variableSet addr(0x%lx)\n", (unsigned long)variableSet);
            }
            #endif
    free(variableSet);
    return 0;
}

char chienSearch_static
(
    struct_galoisFieldElements *galoisFields,
    struct_galoisFieldElements *errLocPolynomial,
    struct_powerFormPolynomials *errorLocationPolynomial,
    unsigned int *metricCheck
)
{
    #ifndef RELEASE
    if(!galoisFields)
    {
        errorMes;
        printf("in chienSearch_static, struct_galoisFieldElements *galoisFields is NULL.\n");
        return -1;
    }
    if(!errLocPolynomial)
    {
        errorMes;
        printf("in chienSearch_static, struct_HD_BM_algorithmComponent *errLocPolynomial is NULL.\n");
        return -1;
    }
    #endif

    #ifndef RELEASE
    if(calculateChienSearch_static_backWard(galoisFields, errLocPolynomial, errorLocationPolynomial, metricCheck))
    {
        errorMes;
        printf("in chienSearch_static, calculateChienSearch_static_backWard is fail.\n");
        return -1;
    }
    #else
    calculateChienSearch_static_backWard(galoisFields, errLocPolynomial, errorLocationPolynomial, metricCheck);
    #endif

            #ifndef RELEASE
            if((global_flag_cmdOption&FLAG_MASK_PRINTF_LOG)==FLAG_MASK_PRINTF_LOG)
            {
                logMes;
                printf("in chienSearch_static, calculated errorLocationPolynomial is below : \n");
                printPowerFormWithEnterPolynomialAndInfoUsingAddress(errorLocationPolynomial);
            }
            #endif

    return 0;
}

char chienSearch
(
    struct_galoisFieldElements *galoisFields,
    struct_galoisFieldElements *errLocPolynomial,
    struct_powerFormPolynomials *errorLocationPolynomial,
    unsigned int *metricCheck,
    enum KIND_OF_BCH_DECODING_ALGORITHM KIND_OF_BCH_ALGORITHM
)
{
    switch(KIND_OF_BCH_ALGORITHM)
    {
        case KIND_OF_BCH_DECODING_BM:
            return chienSearch_static(galoisFields, errLocPolynomial, errorLocationPolynomial, metricCheck);
        break;
        case KIND_OF_BCH_DECODING_iBM:
            printf("in chienSearch, %s is not support yet.\r\n", KIND_OF_BCH_DECODING_ALGORITHM_NAME[KIND_OF_BCH_ALGORITHM]);
        break;
        case KIND_OF_BCH_DECODING_RiBM:
            printf("in chienSearch, %s is not support yet.\r\n", KIND_OF_BCH_DECODING_ALGORITHM_NAME[KIND_OF_BCH_ALGORITHM]);
        break;
        case KIND_OF_BCH_DECODING_SiBM:
            printf("in chienSearch, %s is not support yet.\r\n", KIND_OF_BCH_DECODING_ALGORITHM_NAME[KIND_OF_BCH_ALGORITHM]);
        break;
        case KIND_OF_BCH_DECODING_mSBS:
            return chienSearch_offset_direction(galoisFields, errLocPolynomial, errorLocationPolynomial, metricCheck, CHIEN_SEARCH_DIRECTION_FORWARD);
        break;
        case KIND_OF_BCH_DECODING_PETERSON:
            printf("in chienSearch, %s is not support yet.\r\n", KIND_OF_BCH_DECODING_ALGORITHM_NAME[KIND_OF_BCH_ALGORITHM]);
        break;
        case KIND_OF_BCH_DECODING_EUCLIDIAN:
            printf("in chienSearch, %s is not support yet.\r\n", KIND_OF_BCH_DECODING_ALGORITHM_NAME[KIND_OF_BCH_ALGORITHM]);
        default:
        break;
    }
    return -1;
}
