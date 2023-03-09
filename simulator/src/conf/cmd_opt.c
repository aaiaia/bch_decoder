/* Displaying */
void printInstList(struct struct_cmdLineOption *p, unsigned int recursive, char *passedStrng)
{
    unsigned int i;
    struct struct_cmdLineOption *temp;

    char *passingString = NULL;

    if(((p+0)->instType==OPT_SET_VALUES_SERIES)||((p+0)->instType==OPT_SET_LANGE_SERIES))
    {
        printf("=");
        for(i = 0; (temp = (p + i)); i++)
        {
            printf("[%s]", temp->longName);
            if((p + i + 1)->instType == OPT_SET_VALUES_SERIES) printf(",");
            else if((p + i + 1)->instType == OPT_SET_LANGE_SERIES) printf(":");
            else if((p + i + 1)->instType == STRUCT_END) break;
        }
        printf("}\n");
    }
    else
    {
        for(i = 0; (temp = (p + i))->instType != STRUCT_END; i++)
        {
            printf("\t");

            if(temp->longName)
            {
                if(recursive)
                {
                    printf("%s", passedStrng);
                    if((temp->instType==OPT_SET_VALUES)||(temp->instType==OPT_SET_LANGE))    printf("{");
                    printf("-%s%c", temp->longName,((temp->instType==OPT_SET_VALUES)||(temp->instType==OPT_SET_LANGE)?'\0':((temp->instType==OPT_SET_VALUE)?'=':'\t')));
                }
                else
                {
                    printf("--%s%c", temp->longName,((temp->instType==OPT_SET_VALUES)||(temp->instType==OPT_SET_LANGE)?'\0':((temp->instType==OPT_SET_VALUE)?'=':'\t')));
                }
            }

            if(temp->shortName)        printf("-%c\t", temp->shortName);
            //printf("cmd typd : %d", temp->instType);

            if(temp->argument) printf("arg : %d\t", temp->argument);
            if(temp->argumentNumber) printf("argNums : %d\t", temp->argumentNumber);

            if((temp->instType==OPT_SET_VALUE))    printf("[value]");

            if((temp->instType!=OPT_SET_VALUES)&&(temp->instType!=OPT_SET_LANGE))    printf("\n");

            if(temp->cmdAddedOption)
            {
                if(!passingString)
                {
                    if(!recursive)
                    {
                        passingString=(char *)malloc(sizeof(char)*(2+strlen(temp->longName)+1));
                        sprintf(passingString, "--%s", temp->longName);
                    }
                    else
                    {
                        passingString=(char *)malloc(sizeof(char)*(1+strlen(passedStrng)+strlen(temp->longName)+1));
                        sprintf(passingString, "%s-%s", passedStrng, temp->longName);
                    }
                }
                printInstList(temp->cmdAddedOption, recursive + 1, passingString);
            }

            if(passingString)
            {
                free(passingString);
                passingString = NULL;
            }
        }
    }

    if(passingString) free(passingString);
}

/* Operation */
int instSetFlag(struct struct_cmdLineOption *p)
{
    enum INST_SET_FLAG_NONE_STATE processingFlag=NONE;

    if(p->value)
    {
        if(p->valueMask)
        {
            switch(p->valueType)
            {
                case VALUE_TYPE_UNSIGNED_CHAR:
                    if(p->instType == OPT_FLAG) (*((unsigned char*)p->value))|=(p->valueMask);
                    else if(p->instType == OPT_FLAG_CASE)
                    {
                        if((*((unsigned char*)p->value))) processingFlag = OVER_WRITED;
                        (*((unsigned char*)p->value))=(p->valueMask);
                    }
                    else (*((unsigned char*)p->value))|=(p->valueMask);
                    break;

                case VALUE_TYPE_UNSIGNED_INT:
                    if(p->instType == OPT_FLAG) (*((unsigned int*)p->value))|=(p->valueMask);
                    else if(p->instType == OPT_FLAG_CASE)
                    {
                        if((*((unsigned char*)p->value))) processingFlag = OVER_WRITED;
                        (*((unsigned int*)p->value))=(p->valueMask);
                    }
                    else (*((unsigned char*)p->value))|=(p->valueMask);
                    break;

                case VALUE_TYPE_UNSIGNED_LONG:
                    if(p->instType == OPT_FLAG) (*((unsigned long*)p->value))|=(p->valueMask);
                    else if(p->instType == OPT_FLAG_CASE)
                    {
                        if((*((unsigned char*)p->value))) processingFlag = OVER_WRITED;
                        (*((unsigned long*)p->value))|=(p->valueMask);
                    }
                    else (*((unsigned char*)p->value))|=(p->valueMask);
                    break;

                default:
                errorMes;
                printf("flagger is not signed number or float, double, void, none value type.\n");
                return -1;
            }

            if(processingFlag != NONE)
            {
                warningMesShort; printf("flag : flag \"%s(%c)\" is over-written.\n", (p->longName?p->longName:"[unknown]"), (p->shortName?p->shortName:'?'));
            }
            else
            {
                infoMes; printf("flag : \"%s(%c)\" is flagged.\n", (p->longName?p->longName:"[unknown]"), (p->shortName?p->shortName:'?'));
            }

            return 0;
        }
        else
        {
            errorMes;
            printf("Instruction %s(%c) not have a mask value of flag.\n", (p->longName?p->longName:"[unknown]"), (p->shortName?p->shortName:'?'));
        }
    }
    else if(p->instType == OPT_COMPONENT) return 0;

    errorMes;
    if(p->longName)
    {
        printf("Instruction \"%s\" not have a flag pointer.\n", p->longName);
    }
    else if(p->shortName)
    {
        printf("Instruction '%c' not have a flag pointer.\n", p->shortName);
    }
    else
    {
        printf("Instruction(unknown) not have a flag pointer.\n");
    }

    return -1;
}

/*
//void *instructionCreateBufferValue(enum enum_list_valueType valueType)
//{
//    switch(valueType)
//    {
//        case VALUE_TYPE_CHAR:
//            return malloc(sizeof(char));
//        case VALUE_TYPE_UNSIGNED_CHAR:
//            return malloc(sizeof(unsigned char));
//
//        case VALUE_TYPE_INT:
//            return malloc(sizeof(int));
//        case VALUE_TYPE_UNSIGNED_INT:
//            return malloc(sizeof(unsigned int));
//
//        case VALUE_TYPE_LONG:
//            return malloc(sizeof(long));
//        case VALUE_TYPE_UNSIGNED_LONG:
//            return malloc(sizeof(unsigned long));
//
//        case VALUE_TYPE_FLOAT:
//            return malloc(sizeof(float));
//        case VALUE_TYPE_DOUBLE:
//            return malloc(sizeof(double));
//    }
//}
*/

char instSetValue(struct struct_cmdLineOption *p, char *valueString)
{
    unsigned int tmp;
    if(p->value)
    {
        switch(p->valueType)
        {
            case VALUE_TYPE_NONE:
                errorMes;
                printf("To input value, value type, which set to none , is wrong.\n");
                return -1;

            case VALUE_TYPE_VOID:
                errorMes;
                printf("To input value, value type, which set to void , is wrong.\n");
                return -1;

            case VALUE_TYPE_CHAR:
                warningMes;
                printf("VALUE_TYPE_CHAR not support yet.\n");
                (*((char*)p->value))=0;
                break;

            case VALUE_TYPE_UNSIGNED_CHAR:
                //warningMes;
                //printf("VALUE_TYPE_UNSIGNED_CHAR not support yet.\n");
                //(*((unsigned char*)p->value))=0;
                (*((unsigned char*)p->value))=convertStringToUnsignedChar(valueString);
                break;

            case VALUE_TYPE_INT:
                warningMes;
                printf("VALUE_TYPE_INT not support yet.\n");
                (*((int*)p->value))=0;
                break;

            case VALUE_TYPE_UNSIGNED_INT:
                (*((unsigned int*)p->value))=convertStringToUnsignedInt(valueString);
                break;

            case VALUE_TYPE_LONG:
                warningMes;
                printf("VALUE_TYPE_LONG not support yet.\n");
                (*((long*)p->value))=0;
                break;

            case VALUE_TYPE_UNSIGNED_LONG:
                warningMes;
                //printf("VALUE_TYPE_UNSIGNED_LONG not support yet.\n");
                (*((unsigned long*)p->value))=convertStringToUnsignedLong(valueString);
                break;

            case VALUE_TYPE_FLOAT:
                warningMes;
                printf("VALUE_TYPE_FLOAT not support yet.\n");
                (*((float*)p->value))=0;
                break;

            case VALUE_TYPE_FLOAT_RATIO:
                warningMes;
                printf("VALUE_TYPE_FLOAT not support yet.\n");
                (*((float*)p->value))=0;

                if( (*((float*)p->value)) < 0.0 )
                {
                    warningMes;
                    printf("Ratio value is smaller than zero.\r\n");
                }
                if( 1.0 < (*((float*)p->value)) )
                {
                    warningMes;
                    printf("Ratio value is bigger than zero.\r\n");
                }
                break;

            case VALUE_TYPE_FLOAT_RATIO_LIMIT:
                warningMes;
                printf("VALUE_TYPE_FLOAT not support yet.\n");
                (*((float*)p->value))=0;

                if( (*((float*)p->value)) < 0.0 )
                {
                    warningMes;
                    printf("Ratio value is smaller than zero.\r\n");
                    printf("%f -> %f",(*((float*)p->value)), (-1.0)*(*((float*)p->value)));
                    (*((float*)p->value)) = (-1.0)*(*((float*)p->value));
                }
                if( 1.0 < (*((float*)p->value)) )
                {
                    warningMes;
                    printf("Ratio value is bigger than zero.\r\n");
                    printf("%f -> %f",(*((float*)p->value)), 1.0);
                    (*((float*)p->value)) = 1.0f;
                }
                break;

            case VALUE_TYPE_DOUBLE:
                (*((double*)p->value))=convertStringToDouble(valueString);
                break;

            case VALUE_TYPE_DOUBLE_RATIO:
                (*((double*)p->value))=convertStringToDouble(valueString);
                if( (*((double*)p->value)) < 0.0 )
                {
                    warningMes;
                    printf("Ratio value is smaller than zero.\r\n");
                }
                if( 1.0 < (*((double*)p->value)) )
                {
                    warningMes;
                    printf("Ratio value is bigger than zero.\r\n");
                }
                break;

            case VALUE_TYPE_DOUBLE_RATIO_LIMIT:
                (*((double*)p->value))=convertStringToDouble(valueString);
                if( (*((double*)p->value)) < 0.0 )
                {
                    warningMes;
                    printf("Ratio value is smaller than zero.\r\n");
                    printf("%f -> %f",(*((double*)p->value)), (-1.0)*(*((double*)p->value)));
                    (*((double*)p->value)) = (-1.0)*(*((double*)p->value));
                }
                if( 1.0 < (*((double*)p->value)) )
                {
                    warningMes;
                    printf("Ratio value is bigger than zero.\r\n");
                    printf("%f -> %f",(*((double*)p->value)), 1.0);
                    (*((double*)p->value)) = 1.0f;
                }
                break;

            case VALUE_TYPE_STRING:
                tmp=strlen(valueString);
                (*((char**)p->value))=(char*)malloc(sizeof(char)*(tmp+1));
                memset((*((char**)p->value)), 0, (tmp+1));
                strcpy((*((char**)p->value)), valueString);
                break;

            default:
                errorMes;
                printf("flagger is not signed number or float, double, void, none value type.\n");
                return -1;
        }
        return 0;
    }
    else
    {
            errorMes;
            printf("Instruction %s(%c) not have a flag pointer.\n", (p->longName?p->longName:"[unknown]"), (p->shortName?p->shortName:'?'));
    }
    return -1;
}

char *instSetValues(struct struct_cmdLineOption *p, char *str)
{
    char argBuf[51] = {0};
    char *arg = str;
    unsigned int argBufLength;
    struct struct_cmdLineOption *temp;

    for(temp = p; temp->instType != STRUCT_END; temp++)
    {
        if(temp->instType == OPT_SET_VALUES_SERIES)
        {
            /* abstract number from str */
            strncpy(argBuf, arg, (argBufLength=strcspn(arg, cmdOtionKeyWords)));
            arg+=argBufLength;

            (*(argBuf+argBufLength)) = 0;

            /* set value */
                                    //printf("\t\t\tinstSetValues) long name is \"%s\" argBuf = %s\n", (temp->longName?temp->longName:"[unknown long name]"), argBuf);
            if(instSetValue(temp, argBuf))
            {
                return str;
            }

            if(!(*arg))
            {
                if((temp+1)->instType != OPT_SET_VALUES_SERIES)
                {
                    errorMes;
                    printf("input values seriesly is wrong. To input, needs number of value and inserted values are not matched.\n");
                    return str;
                }
                else return arg;
            }

            if(*arg == '}')
            {
                arg++;
                break;
            }

            if((*arg)!=',')
            {
                return str;
            }
            else
            {
                if((temp+1)->instType != OPT_SET_VALUES_SERIES)
                {
                    errorMes;
                    printf("input values seriesly is wrong. To input, needs number of value and inserted values are not matched.\n");
                    return str;
                }
                arg++;
            }
        }
        else
        {
            break;
        }
    }

    return arg;
}

char instrocessInstElementWhenDetected(struct struct_cmdLineOption *p)
{
struct struct_cmdLineOption *temp = p;

    switch(temp->instType)
    {
        case OPT_NOTHING:
                                                //printf("instType: OPT_NOTHING[%d], name: \"%s\"\n", temp->instType, temp->longName?temp->longName:"[unknown]");
            break;
        case OPT_FLAG:
        case OPT_FLAG_CASE:
            if(instSetFlag(temp))
            {
                errorMes;
                printf("\"%s\" instruction set wrong.\n", temp->longName?temp->longName:"[empty long name]");
                return -1;
            }
                                                //printf("instType: OPT_FLAG[%d], name: \"%s\"\n", temp->instType, temp->longName?temp->longName:"[unknown]");
            break;

        case OPT_DEBUG:
                                                //printf("instType: OPT_DEBUG[%d], name: \"%s\"\n", temp->instType, temp->longName?temp->longName:"[unknown]");
            break;

        case OPT_SET_VALUE:
                                                //printf("instType: OPT_SET_VALUE[%d], name: \"%s\"\n", temp->instType, temp->longName?temp->longName:"[unknown]");
            break;

        case OPT_SET_VALUES:
                                                //printf("instType: OPT_SET_VALUES[%d], name: \"%s\"\n", temp->instType, temp->longName?temp->longName:"[unknown]");
            break;
        case OPT_SET_VALUES_SERIES: //example) {-settingCategory=[value0],[value1],[value2],[value3]}
                                                //printf("instType: OPT_SET_VALUES_SERIES[%d], name: \"%s\"\n", temp->instType, temp->longName?temp->longName:"[unknown]");
            break;

        case OPT_SET_LANGE: //set lange, example) {-settingCategory=[start]:[step]:[end]}
                                                //printf("instType: OPT_SET_LANGE[%d], name: \"%s\"\n", temp->instType, temp->longName?temp->longName:"[unknown]");
            break;
        case OPT_SET_LANGE_SERIES:
                                                //printf("instType: OPT_SET_LANGE_SERIES[%d], name: \"%s\"\n", temp->instType, temp->longName?temp->longName:"[unknown]");
            break;

        case OPT_STRING:
                                                //printf("instType: OPT_STRING[%d], name: \"%s\"\n", temp->instType, temp->longName?temp->longName:"[unknown]");
            break;
        case OPT_COMPONENT:
            if(instSetFlag(temp))
            {
                errorMes;
                printf("\"%s\" instruction set wrong.\n", temp->longName?temp->longName:"[empty long name]");
                return -1;
            }
                                                //printf("instType: OPT_COMPONENT[%d], name: \"%s\"\n", temp->instType, temp->longName?temp->longName:"[unknown]");
            break;

        case STRUCT_END:
                                                //printf("instType: STRUCT_END[%d], name: \"%s\"\n", temp->instType, temp->longName?temp->longName:"[unknown]");
            break;

        default:
            errorMes;
            printf("instruction type is UNKNOWN.\n");
            return -1;
    }
    return 0;
}

struct struct_cmdLineOption *instFindSameLongNameInInstSet(struct struct_cmdLineOption *p, char *targetString)
{
    struct struct_cmdLineOption *temp;

    for(temp = p; temp->instType != STRUCT_END; temp++)
    {
        if(temp->longName)
        {
            if(!strcmp(temp->longName, targetString))
            {
                                            //printf("A option \"%s\" apply.\n", temp->longName);
                return temp;
            }
        }
    }

    return NULL;
}

/*
    If cmd options are wrong, return to input str address.
    Else cmd options process normally, return moved str address, which is same to char *arg.
*/
char *instProcessLongNameCmdOption(struct struct_cmdLineOption *p, char *str, char keyword)
{
    struct struct_cmdLineOption *temp;

    char *arg = str;
    char *argReturn = NULL;

    unsigned int argBufLength = 0;
    char argBuf[51] = {0};

/*
    if(*(arg)=='{')
    {
        printf("tie-up options detected.\n");
        arg++;
    }
     */

    while(*arg)
    {
        switch(*arg)
        {
            case '-'://find same option in struct_cmdLineOption Set
            {
                arg++;
                                                            //debugMes;    printf("\t\tbefore arg : \"%s\".",arg);

                strncpy(argBuf, arg, (argBufLength=strcspn(arg, cmdOtionKeyWords)));
                arg+=argBufLength;

                (*(argBuf+argBufLength)) = 0;

                                                            //printf("\t\targBuf : \"%s\"", argBuf);
                                                            //printf("\t\tafter arg : \"%s\".\n",arg);

                if(!(temp = instFindSameLongNameInInstSet(p, argBuf))) return str;
                /* Find same long name, do something start*/
                if(instrocessInstElementWhenDetected(temp)) return str;
                /* Find same long name, do something end*/

                switch(*arg)//enter sub struct_cmdLineOption Set
                {
                    //case '{':
                    //    arg++;
                    case '-':
                        if(temp->cmdAddedOption)
                        {
                            /* if processing sub job, need to update arg that is return value from instProcessLongNameCmdOption(...). */
                            if(arg == (argReturn = instProcessLongNameCmdOption(temp->cmdAddedOption, arg, 0))) return str;
                            arg = argReturn;
                            argReturn = NULL;
                        }

                    default:
                    break;
                }
                break;
            }

            case '{'://enter same struct_cmdLineOption Set
            {
                arg++;
                if(arg == (argReturn=instProcessLongNameCmdOption(temp->cmdAddedOption, arg, 0))) return str;
                arg = argReturn;
                argReturn = NULL;
                break;
            }

            case '}'://exit present struct_cmdLineOption Set
            {
                arg++;
                return arg;
                break;
            }

            case ','://arg ++ and then find others cmd options in present struct_cmdLineOption Set
            {
                                                            //printf("find combination word.\n");
                arg++;
                                                            //printf("\t\targ : \"%s\"\n", arg);
                break;
            }

            case ':'://arg ++ and then move next cmdLineOpton elements and then input value
            {
                            printf("set lange value resies.\n");
                break;
            }

            case '='://input value
            {
                arg++;
                switch(temp->instType)
                {
                    case OPT_SET_VALUE:
                    {
                        if(temp->value)
                        {
                            if(*arg == '-')
                            {
                                strncpy(argBuf, arg, (argBufLength=(strcspn(arg+1, cmdOtionKeyWords)+1)));
                            }
                            else
                            {
                                strncpy(argBuf, arg, (argBufLength=strcspn(arg, cmdOtionKeyWords)));
                            }

                            arg+=argBufLength;

                            (*(argBuf+argBufLength)) = 0;
                                                                        //printf("argBuf : '%s'\n", argBuf);
                                                                        //printf("arg : %s\r\n", arg);

                            /* set value */
                            if(instSetValue(temp, argBuf))
                            {
                                errorMes;
                                printf("input value is invalid.\n");
                                return str;
                            }
                        }
                    }
                    break;

                    case OPT_SET_VALUES:
                    {
                        if(arg == (argReturn = instSetValues(temp->cmdAddedOption, arg))) return str;
                        arg = argReturn;
                        argReturn = NULL;
                        /* call a set values function using sub set pointer */
                    }
                    break;

                    case OPT_SET_LANGE:
                    {
                        /* call a set lange value function using sub set pointer */
                    }
                    break;

                    case OPT_STRING:
                    {
                        if(temp->value)//if instruct set have any pointer to store value
                        {
                            strncpy(argBuf, arg, (argBufLength=strcspn(arg, cmdOtionKeyWords)));
                            arg+=argBufLength;

                            (*(argBuf+argBufLength)) = 0;
                                                                    //printf("\t\t\targBuf : '%s'\n", argBuf);
                            /* set value */
                            if(instSetValue(temp, argBuf))
                            {
                                errorMes;
                                printf("input value is invalid.\n");
                                return str;
                            }
                        }
                    }
                    break;

                    default:
                        return str;
                }
                                //printf("set value parts.\n");
                break;
            }

            default:
            {
                errorMes;
                printf("each long option have to one or more key word like \"%s\" at head.\n", cmdOtionKeyWords);
                return str;
            }
        }
    }

    return arg;
}

int instMainProcessing(struct struct_cmdLineOption *p, char *str)
{
    //unsigned int strLength=strlen(str);
    struct struct_cmdLineOption *temp = p;

    char *arg=str;
    char *argBuffer=NULL;

    if(*(arg+0)!='-')
    {
        return -1;
    }
    else if(*(arg+1)!='-')//single word
    {
        arg++;
        if(!(*(arg+1)))
        {
            for(temp = p; temp->instType != STRUCT_END; temp++)
            {
                if(temp->shortName == *arg)
                {
                    if(instSetFlag(temp))
                    {
                        errorMes;
                        printf("\"%s\" instruction set wrong.\n", temp->longName?temp->longName:"[empty long name]");
                        return -1;
                    }
                    return 0;
                }
            }
        }
    }
    else//string
    {
        arg++;
                                        //printf("\tinitial arg : \"%s\"\n", arg);
        if(arg != (argBuffer=instProcessLongNameCmdOption(p, arg, 0)))
        {
            infoMes;
            printf("\"%s\" apply.\n", str);
            return 0;
        }
                        //printf("arg : 0x%000000008x , argBuffer : 0x%000000008x\n", arg, argBuffer);
    }

    errorMes;
    printf("'%s' is unknown command option.\n", str);
    return -1;
}
