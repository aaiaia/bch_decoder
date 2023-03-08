/* Converting */
void convertTwosComplementMagitude(s_int_QUANTIZ_DIGIT* to, s_int_QUANTIZ_DIGIT* from, unsigned int length, unsigned int max_val)
{
    unsigned int i;

     for(i = 0; i < length; i++)
     {
        if((*(from + i)) >= 0)//quantized LLR is positive, Hard decision is 0.
        {
            *(to + i) = (*(from + i));
        }
        else                        //quantized LLR is negative, Hard decision is 1.
        {
            *(to + i) = ((-1) * (*(from + i)));
            if(*(to + i) > max_val) *(to + i) = max_val;
        }
        //printf("Q:%d M:%d i:%d\r\n", (*(p->quantizedLLR + i)), *(p->magnitude + i), i);
     }
};

void convertOnesComplementMagnitude(s_int_QUANTIZ_DIGIT* to, s_int_QUANTIZ_DIGIT* from, unsigned int length)
{
    unsigned int i;

    for(i = 0; i < length; i++)
    {
        if((*(from + i)) >= 0)//quantized LLR is positive, Hard decision is 0.
        {
            *(to + i) = (*(from + i));
        }
        else                        //quantized LLR is negative, Hard decision is 1.
        {
            *(to + i) = (~(*(from + i)));
        }
        //printf("Q:%d M:%d i:%d\r\n", (*(p->quantizedLLR + i)), *(p->magnitude + i), i);
    }
};

/* Operation */
void quantizationWithGlobalStatic(double LLR[], SIGNED_INT LLR_quantization[], char *Codeword_MSG, unsigned int length)
{
    SIGNED_INT k;
    unsigned int i;



    for(i=0; i<length; i++)
    {
        if(LLR[i] < global_QuantizationInfo.rangeMin+global_QuantizationInfo.offset)//LLR is negative
        {
            //LLR_quantization[i]=(((signed int)(global_QuantizationInfo.numberOfSteps/2))*(-1)+1);
            LLR_quantization[i]=(((signed int)(global_QuantizationInfo.numberOfSteps/2))*(-1));
            if(Codeword_MSG) Codeword_MSG[i] = '1';
        }
        else if(global_QuantizationInfo.rangeMax+global_QuantizationInfo.offset <= LLR[i])//LLR is positive
        {
            LLR_quantization[i]=((((signed int)(global_QuantizationInfo.numberOfSteps/2)))-1);
            if(Codeword_MSG) Codeword_MSG[i] = '0';
        }
        else
        {
            if(global_QuantizationInfo.offset<LLR[i])//LLR is positive
            {
                for(k=0; k<(((signed int)(global_QuantizationInfo.numberOfSteps/2))); k++)
                {
                    if((((double_RATIONAL_NUMBER)k)+global_QuantizationInfo.offset<=LLR[i])&&(LLR[i]<(((double_RATIONAL_NUMBER)k)+global_QuantizationInfo.step)+global_QuantizationInfo.offset))
                    {
                        LLR_quantization[i]=k;
                        break;
                    }
                }
                if(Codeword_MSG) Codeword_MSG[i] = '0';
            }
            else//LLR is negative
            {
                for(k=(((signed int)(global_QuantizationInfo.numberOfSteps/2))*(-1)+0); k<0; k++)
                {
                    if((((double_RATIONAL_NUMBER)k)+global_QuantizationInfo.offset<=LLR[i])&&(LLR[i]<(((double_RATIONAL_NUMBER)k)+global_QuantizationInfo.step)+global_QuantizationInfo.offset))
                    {
                        LLR_quantization[i]=k;
                        break;
                    }
                }
                if(Codeword_MSG) Codeword_MSG[i] = '1';
            }
        }
    }
    return;
}

void quantizationWithGlobalAdaptive(double LLR[], SIGNED_INT LLR_quantization[], char *Codeword_MSG, unsigned int length)
{
    unsigned int tmp_i;
    int tmp_q;
    double_RATIONAL_NUMBER tmp_step = global_QuantizationInfo.step;
    double_RATIONAL_NUMBER tmp_zeroSymmetry = \
        (double_RATIONAL_NUMBER)global_QuantizationInfo.val_ones_zero_handling;
    double_RATIONAL_NUMBER tmp_offset = global_QuantizationInfo.offset;

    for(tmp_i = 0; tmp_i < length; tmp_i++)
    {
        /* bigger than criterion? */
        if(LLR[tmp_i] >= global_QuantizationInfo.criterion)
        {
            /*
             * LLR is larger than criterion
             * Quantized value is positive
             */
            for(tmp_q = global_QuantizationInfo.quantizedIntMax; tmp_q > 0; tmp_q--)
            {
                if( (( ((double_RATIONAL_NUMBER)tmp_q) + tmp_offset)) * tmp_step <= \
                        LLR[tmp_i])
                {
                    LLR_quantization[tmp_i] = tmp_q;
                    break;
                }
            }
            if(tmp_q == 0)  LLR_quantization[tmp_i] = tmp_q;
            Codeword_MSG[tmp_i] = '0';
        }
        else
        {
            /*
             * LLR is smaller than criterion
             * Quantized value is negative
             */
            for(tmp_q = global_QuantizationInfo.quantizedIntMin; tmp_q < -1; tmp_q++)
            {
                if( ( ((double_RATIONAL_NUMBER)tmp_q) + tmp_offset + 1.0 + tmp_zeroSymmetry ) * \
                     tmp_step > LLR[tmp_i])
                {
                    LLR_quantization[tmp_i] = tmp_q;
                    break;
                }
            }
            if(tmp_q == -1)
            {
                switch(global_QuantizationInfo.ones_zero_handling)
                {
                    default:
                    case ENUM_FLAG_CASE_QUANTIZ_NUMBERIC_SYS_ONE_S_COM_ZERO_SYMMETRY:
                        LLR_quantization[tmp_i] = tmp_q;
                    break;

                    case ENUM_FLAG_CASE_QUANTIZ_NUMBERIC_SYS_ONE_S_COM_ZERO_DUPLICATED:
                        LLR_quantization[tmp_i] = ++tmp_q;
                    break;
                }
            }
            Codeword_MSG[tmp_i] = '1';
        }
    }
}

/*
 * default value
 *     global_QuantizationInfo.offset=global_LLR_quantizFilter_offset;//-0.5
 *     global_QuantizationInfo.step=global_LLR_quantizFilter_step;//1.0
 *     global_QuantizationInfo.numsOfBits=global_LLR_quantizFilter_bitWidth;//4
 */
void initGlobalQuantizInfo(double offset, double step, unsigned char quantizBitNum, unsigned int numbericSystem, unsigned char overping)
{
    double* display_stimulus;
    signed int* display_result;
    char* display_hard_decision;
    signed int* display_twos_mag;
    signed int* display_ones_mag;
    double  tmp_double = 0.0f;
    unsigned int tmp_u_int = 0;
    unsigned int i;

    printf("INITIALIZING QUANTIZATION FILTER\r\n");

    global_QuantizationInfo.numberic_system = numbericSystem;

    global_QuantizationInfo.offset=((double_RATIONAL_NUMBER)offset);

    switch(numbericSystem)
    {
        default:
        warningMesShort;
        printf("LLR numberic system is not selected, default: 2's complement\r\n");
        global_flag_case_llr_numberic_system = ENUM_FLAG_CASE_QUANTIZ_NUMBERIC_SYSTEM_TWO_S_COMPLEMENT;
        case ENUM_FLAG_CASE_QUANTIZ_NUMBERIC_SYSTEM_TWO_S_COMPLEMENT:
        /* zero symmetry */
        /* no zero symmetry */
        global_QuantizationInfo.ones_zero_handling = ENUM_FLAG_CASE_QUANTIZ_NUMBERIC_SYS_ONE_S_COM_NONE;
        global_QuantizationInfo.val_ones_zero_handling = 0;

        global_QuantizationInfo.criterion = ((double_RATIONAL_NUMBER)offset);
        break;

        case ENUM_FLAG_CASE_QUANTIZ_NUMBERIC_SYSTEM_ONE_S_COMPLEMENT:
        {
            /*
            //if(global_QuantizationInfo.offset != 0.0f)
            //{
            //    warningMesShort;
            //    printf("Numberic system set to 1's complement.\r\n");
            //    warningMesShort;
            //    printf("Qauntizing offset is '%f', but will be set 0.0\r\n");
            //}
            //global_QuantizationInfo.offset = 0.0f;
            */
        }


        /* zero symmetry */
        if(global_flag_case_llr_mag_ones_complement_zero_handling == \
            ENUM_FLAG_CASE_QUANTIZ_NUMBERIC_SYS_ONE_S_COM_ZERO_SYMMETRY)
        {
            /* zero symmetry */
            global_QuantizationInfo.ones_zero_handling = ENUM_FLAG_CASE_QUANTIZ_NUMBERIC_SYS_ONE_S_COM_ZERO_SYMMETRY;//zero_symmetry enable value is not change any value except with 1.
            global_QuantizationInfo.val_ones_zero_handling = 1;//zero_symmetry enable value is not change any value except with 1.
            global_QuantizationInfo.criterion = 0.0f;
        }
        else if(global_flag_case_llr_mag_ones_complement_zero_handling == \
            ENUM_FLAG_CASE_QUANTIZ_NUMBERIC_SYS_ONE_S_COM_ZERO_DUPLICATED)
        {
            /* zero duplication */
            global_QuantizationInfo.ones_zero_handling = ENUM_FLAG_CASE_QUANTIZ_NUMBERIC_SYS_ONE_S_COM_ZERO_DUPLICATED;//zero_symmetry enable value is not change any value except with 1.
            global_QuantizationInfo.val_ones_zero_handling = 1;//zero_symmetry enable value is not change any value except with 1.
            global_QuantizationInfo.criterion = ((double_RATIONAL_NUMBER)offset);
        }
        else
        {
            /* no zero symmetry */
            global_QuantizationInfo.ones_zero_handling = ENUM_FLAG_CASE_QUANTIZ_NUMBERIC_SYS_ONE_S_COM_NONE;
            global_QuantizationInfo.val_ones_zero_handling = 0;
            global_QuantizationInfo.criterion = ((double_RATIONAL_NUMBER)offset);
        }
        break;
    }

    global_QuantizationInfo.step=((double_RATIONAL_NUMBER)step);
    global_QuantizationInfo.coverage=((double_RATIONAL_NUMBER)step*0.5);
    global_QuantizationInfo.numsOfBits=quantizBitNum;
    global_QuantizationInfo.numberOfSteps = \
        (s_int_QUANTIZ_DIGIT)(pow(2.0,(double)global_QuantizationInfo.numsOfBits));
    /* zero symmetry */
    if(global_flag_case_llr_mag_ones_complement_zero_handling == \
        ENUM_FLAG_CASE_QUANTIZ_NUMBERIC_SYS_ONE_S_COM_ZERO_SYMMETRY)
    {
        /* overaping */
        global_QuantizationInfo.numberOfSteps -= 1;
    }
    else if(global_flag_case_llr_mag_ones_complement_zero_handling == \
        ENUM_FLAG_CASE_QUANTIZ_NUMBERIC_SYS_ONE_S_COM_ZERO_DUPLICATED)
    {
        /* overaping */
        global_QuantizationInfo.numberOfSteps -= 1;
    }


    global_QuantizationInfo.rangeMax = \
        ((double_RATIONAL_NUMBER)(global_QuantizationInfo.numberOfSteps/2)) * \
        global_QuantizationInfo.step*(1.0);
    global_QuantizationInfo.rangeMin = \
        ((double_RATIONAL_NUMBER)(global_QuantizationInfo.numberOfSteps/2)) * \
        global_QuantizationInfo.step*(-1.0);
    /* zero symmetry */
    if(global_flag_case_llr_mag_ones_complement_zero_handling == \
        ENUM_FLAG_CASE_QUANTIZ_NUMBERIC_SYS_ONE_S_COM_ZERO_SYMMETRY)
    {
        /* overaping */
        global_QuantizationInfo.rangeMin += 1.0;
    }
    else if(global_flag_case_llr_mag_ones_complement_zero_handling == \
        ENUM_FLAG_CASE_QUANTIZ_NUMBERIC_SYS_ONE_S_COM_ZERO_DUPLICATED)
    {
        /* overaping */
        global_QuantizationInfo.rangeMin += 1.0;
    }


    global_QuantizationInfo.quantizedIntMax = \
        (( (s_int_QUANTIZ_DIGIT)(pow(2.0,(double)global_QuantizationInfo.numsOfBits)) / 2)) - 1;
    global_QuantizationInfo.quantizedIntMin = \
        ((-1)*( (s_int_QUANTIZ_DIGIT)(pow(2.0,(double)global_QuantizationInfo.numsOfBits)) / 2));

    global_QuantizationInfo.quatizing_bitMask = 0;
    for(i=0; i<global_QuantizationInfo.numsOfBits; i++)
    {
        global_QuantizationInfo.quatizing_bitMask |= (1 << i);
    }
    global_QuantizationInfo.mag_bitMask = (global_QuantizationInfo.quatizing_bitMask >> 1);


    infoMes; printf("Quantiz Offset is %f\r\n", global_QuantizationInfo.offset);

    infoMes; printf("Quantiz Step is %f\r\n", global_QuantizationInfo.step);

    infoMes; printf("Quantiz Coverage is %f\r\n", global_QuantizationInfo.coverage);

    infoMes; printf("Quantiz Criterion is %f\r\n", global_QuantizationInfo.criterion);

    infoMes; printf("Quantiz number of bit is %d\r\n", global_QuantizationInfo.numsOfBits);

    infoMes; printf("Quantiz Number of Steps is %d\r\n", global_QuantizationInfo.numberOfSteps);

    infoMes; printf("Detectable Maximum range %f\r\n", \
            global_QuantizationInfo.rangeMax + global_QuantizationInfo.offset);
    infoMes; printf("Detectable Minumum range %f\r\n", \
            global_QuantizationInfo.rangeMin + global_QuantizationInfo.offset);

    infoMes; printf("Quantized Maximum range %d\r\n", global_QuantizationInfo.quantizedIntMax);
    infoMes; printf("Quantized Minumum range %d\r\n", global_QuantizationInfo.quantizedIntMin);


    infoMes; printf("Quantiz Bit MasK 0x%x\r\n", global_QuantizationInfo.quatizing_bitMask);
    infoMes; printf("Quan mag Bit MasK 0x%x\r\n", global_QuantizationInfo.mag_bitMask);

    infoMes; printf("numberic system %s\r\n", \
        NAME_QUANTIZ_NUMBERIC_SYSTEM[global_QuantizationInfo.numberic_system]);

    infoMes; printf("zero %s\r\n", \
         NAME_QUANTIZ_ONE_S_COMPLEMENT_ZERO_HANDLING\
         [global_QuantizationInfo.ones_zero_handling]);

    if(global_flag_llr_display&FLAG_LLR_DISPLAY_QUANTIZATION_FILTERING)
    {
        tmp_double = (global_QuantizationInfo.step / 10.0);
        infoMes; printf(">> segment of step : %f\r\n", tmp_double);
        tmp_u_int = (unsigned int)\
            (( (global_QuantizationInfo.rangeMax + global_QuantizationInfo.step \
                - global_QuantizationInfo.rangeMin + global_QuantizationInfo.step)) / \
            tmp_double);
        infoMes; printf(">> stimulus vector nums : %d\r\n", tmp_u_int);

        display_stimulus = (double*)malloc(sizeof(double) * tmp_u_int);
        display_result = (signed int*)malloc(sizeof(signed int) * tmp_u_int);
        display_hard_decision = (char*)malloc(sizeof(char) * tmp_u_int);
        display_twos_mag = (signed int*)malloc(sizeof(signed int) * tmp_u_int);
        display_ones_mag = (signed int*)malloc(sizeof(signed int) * tmp_u_int);

        for(i=0; i<tmp_u_int; i++) display_stimulus[i] = \
            (((double_RATIONAL_NUMBER)i) * tmp_double) + \
            global_QuantizationInfo.rangeMin - global_QuantizationInfo.step + \
            global_QuantizationInfo.offset;
        infoMes; printf("[Quantizing Filter Range Display Start]\r\n");
        quantizationWithGlobalAdaptive
        (
            display_stimulus,
            (SIGNED_INT*)display_result,
            display_hard_decision,
            tmp_u_int
        );

        convertTwosComplementMagitude(
            display_twos_mag,
            display_result,
            tmp_u_int,
            global_QuantizationInfo.quantizedIntMax);

        convertOnesComplementMagnitude(
            display_ones_mag, display_result, tmp_u_int);

        switch(numbericSystem)
        {
            default:
            case ENUM_FLAG_CASE_QUANTIZ_NUMBERIC_SYSTEM_TWO_S_COMPLEMENT:
                for(i=0; i<tmp_u_int; i++)
                {
                    printf("stim : %f, q_val : %d, HD : %c, 2's mag : %d, 1's mag : %d\r\n",
                        display_stimulus[i], display_result[i], display_hard_decision[i],
                        display_twos_mag[i], display_ones_mag[i]);
                }
            break;

            case ENUM_FLAG_CASE_QUANTIZ_NUMBERIC_SYSTEM_ONE_S_COMPLEMENT:
                for(i=0; i<tmp_u_int; i++)
                {
                    printf("stim : %f, q_val : %d, HD : %c, 1's mag : %d, 2's mag : %d\r\n",
                        display_stimulus[i], display_result[i], display_hard_decision[i],
                        display_ones_mag[i], display_twos_mag[i]);
                }
            break;
        }

        infoMes; printf("[Quantizing Filter Range Display End]\r\n");
        free(display_stimulus);
        free(display_result);
        free(display_hard_decision);
        free(display_twos_mag);
        free(display_ones_mag);
    }
}
