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
