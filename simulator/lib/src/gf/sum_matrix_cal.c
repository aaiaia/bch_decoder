#include <string.h>

#include "file_label.h"
#include "file_type.h"
#include "common/cmd_opt_vars.h"
#include "gf/sum_matrix_cal.h"

char calculateSummationMatrix(struct_galoisFieldElements *galoisFields, struct_summationMatrix *p)
{
    unsigned int i,j;

    unsigned int tmp_row;
    unsigned int tmp_column;

    #ifndef RELEASE
    if(!galoisFields)
    {
        return -1;
    }

    if(!p)
    {
        return -2;
    }

    if(!(p->row))
    {
        return -3;
    }
    if(!(p->column))
    {
        return -4;
    }
    #endif

    tmp_row = p->row;
    tmp_column = p->column;

            #ifndef RELEASE
            infoMes;  printf("calculating a struct_summationMatrix ");
            #endif
    #ifdef USING_OPEN_MP
    #pragma omp parallel for schedule(guided) private(i, j) shared(tmp_row, p, tmp_column, galoisFields)
    #endif
    for(i=0; i<tmp_row; i++)
    {
        for(j=0; j<tmp_column; j++)
        {
            //summationElementsInGaloisFieldUsingGaloisFieldInt_finding is return (*(field->element+i));
            (*((*(p->element+i))+j))=summationElementsInGaloisFieldUsingGaloisFieldInt_finding(galoisFields, *(galoisFields->element+i), *(galoisFields->element+j));
        }
    }
            #ifndef RELEASE
            printf("is done.\n");
            #endif

    return 0;
}

/* 20160425_18:07 deleted */
/*
//struct_summationMatrix *createSummationMatrix(struct_galoisFieldElements *galoisFields, unsigned int row, unsigned int column)
//{
//    unsigned int i,j;
//    struct_summationMatrix *p;
//
//    unsigned int count;
//
//    #ifndef RELEASE
//    if(!galoisFields)
//    {
//        return NULL;
//    }
//    if(!row)
//    {
//        return NULL;
//    }
//    if(!column)
//    {
//        return NULL;
//    }
//    if(row!=column)
//    {
//        return NULL;
//    }
//    #endif
//
//
//    p=(struct_summationMatrix*)malloc(sizeof(struct_summationMatrix));
//
//    p->element=(struct_galoisFieldPolyForm ***)malloc(sizeof(struct_galoisFieldPolyForm **)*row);
//
//            #ifndef RELEASE
//            infoMes;  printf("calculating a struct_summationMatrix ");
//            #endif
//
//    #ifdef USING_OPEN_MP
//    #pragma omp parallel for schedule(guided) private(i, j) shared(row, p, column, galoisFields)
//    #endif
//    for(i=0; i<row; i++)
//    {
//        (*(p->element+i))=(struct_galoisFieldPolyForm **)malloc(sizeof(struct_galoisFieldPolyForm *)*column);
//        for(j=0; j<column; j++)
//        {
//            (*((*(p->element+i))+j))=summationElementsInGaloisFieldUsingGaloisFieldInt_finding(galoisFields, *(galoisFields->element+i), *(galoisFields->element+j));
//        }
//    }
//            #ifndef RELEASE
//            printf("is done.\n");
//            #endif
//
//    p->row=row;
//    p->column=column;
//    return p;
//}
*/

struct_summationMatrix *createSummationMatrix(struct_galoisFieldElements *galoisFields, unsigned int in_row, unsigned int in_column)
{
    struct_summationMatrix *p;

    p=createSummationMatrix_emptySpace(in_row, in_column);
    if(calculateSummationMatrix(galoisFields, p))
    {
        errorMes; printf("in createSummationMatrix, calculateSummationMatrix has error(close).\r\n");
        closeSummationMatrix(&p);
    }
    return p;
}

/* FILE IO */
char save_struct_summationMatrix(char *path, char *primitivePoly, struct_summationMatrix *p)
{
    unsigned int tmp_i;
    unsigned int tmp_j;
    FILE *fileio;
    if(!path)            return -1;
    if(!primitivePoly)    return -2;
    if(!p)                return -3;

    fileio = fopen(path, "w");

    fprintf(fileio, "primitive polynomials, %s\r\n", primitivePoly);


    fprintf(fileio, "%s\r\n", FILE_IO_DATA_PARSING_KEY_STRING_SAVED_DATA_START);

    fprintf(fileio, "%s\r\n", FILE_IO_DATA_PARSING_KEY_STRING_KIND_OF_DATA_START);
    fprintf(fileio, "Kind Of Data, %d\r\n", FILE_IO_KIND_OF_DATA_SUMATION_MATRIX);
    fprintf(fileio, "%s\r\n", FILE_IO_DATA_PARSING_KEY_STRING_KIND_OF_DATA_END);


    fprintf(fileio, "%s\r\n", FILE_IO_DATA_PARSING_KEY_STRING_ARRAY_TYPE_START);
    fprintf(fileio, "Saved Data Type, %d\r\n", FILD_IO_DATA_ARRAY_TYPE_LINEAR);
    fprintf(fileio, "%s\r\n", FILE_IO_DATA_PARSING_KEY_STRING_ARRAY_TYPE_END);

    fprintf(fileio, "%s\r\n", FILE_IO_DATA_PARSING_KEY_STRING_BIT_WIDTH_START);
    fprintf(fileio, "Bit Width, %d\r\n", (*((*(p->element+0))+0))->length);
    fprintf(fileio, "%s\r\n", FILE_IO_DATA_PARSING_KEY_STRING_BIT_WIDTH_END);


    fprintf(fileio, "%s\r\n", FILE_IO_DATA_PARSING_KEY_STRING_ROW_DATA_START);
    fprintf(fileio, "Matrix row length, %d\r\n", p->row);
    fprintf(fileio, "%s\r\n", FILE_IO_DATA_PARSING_KEY_STRING_ROW_DATA_END);


    fprintf(fileio, "%s\r\n", FILE_IO_DATA_PARSING_KEY_STRING_COL_DATA_START);
    fprintf(fileio, "Matrix column length, %d\r\n", p->column);
    fprintf(fileio, "%s\r\n", FILE_IO_DATA_PARSING_KEY_STRING_COL_DATA_END);

    fprintf(fileio, "pow(index)\r\n");

    fprintf(fileio, "%s\r\n", FILE_IO_DATA_PARSING_KEY_STRING_RAW_DATA_START);
    for(tmp_i=0; tmp_i<p->row; tmp_i++)
    {
        for(tmp_j=0; tmp_j<p->column; tmp_j++)
        {
            if((*((*(p->element+tmp_i))+tmp_j))->index)
            {
                fprintf(fileio, "%d(%d)%s",
                    ((*((*(p->element+tmp_i))+tmp_j))->index - 1),
                    (*((*(p->element+tmp_i))+tmp_j))->index,
                    ((tmp_j<(p->column-1))?", ":"")
                );
            }
            else
            {
                fprintf(fileio, "%c(%d)%s",
                    '-',
                    (*((*(p->element+tmp_i))+tmp_j))->index,
                    ((tmp_j<(p->column-1))?", ":"")
                );

            }

        }
        fprintf(fileio, "\r\n");
    }
    fprintf(fileio, "%s\r\n", FILE_IO_DATA_PARSING_KEY_STRING_RAW_DATA_END);

    fprintf(fileio, "%s\r\n", FILE_IO_DATA_PARSING_KEY_STRING_SAVED_DATA_END);

    fclose(fileio);

    return 0;
}

char load_struct_summationMatrix(char *path, char *primitivePoly, struct_galoisFieldElements *galoisFields, struct_summationMatrix *p)
{
    unsigned int tmp_i=0;
    unsigned int tmp_j=0;
    FILE *fileio_load_sumMatrix;

    unsigned int primitivePoly_length=0;
    char load_primitivePoly[1025];

    int load_kindOfData=0;
    int load_saveOfData=0;

    unsigned int load_bitWidth;
    unsigned int load_row;
    unsigned int load_column;

    unsigned int load_index;
    char load_exp_char=0;
    unsigned int load_exp_integer=0;

    char load_stringBuf[1025]={0};

    unsigned int chk_incorrectFormCnt=0;
    unsigned int chk_incorrectDataCnt=0;

    if(!path)            return -1;
    if(!primitivePoly)    return -2;
    if(!p)                return -3;

    /* condition chk */
    if(!(primitivePoly_length=strlen(primitivePoly)))
    {
        return -4;
    }


    /* buffer set up to varify */


    fileio_load_sumMatrix = fopen(path, "r");

                printf("[load_struct_summationMatrix START]\r\n");

    if(fscanf(fileio_load_sumMatrix, "primitive polynomials, %s\r\n", load_primitivePoly)<0)            chk_incorrectFormCnt++;
            //printf("primitive polynomials, %s\r\n", load_primitivePoly);
        //if(strcmp(load_primitivePoly, primitivePoly))
        //{
        //    chk_incorrectDataCnt++;
        //}

    if(fscanf(fileio_load_sumMatrix, "%s\r\n", load_stringBuf)<0)        chk_incorrectFormCnt++;
            //printf("%s\r\n", load_stringBuf);//FILE_IO_DATA_PARSING_KEY_STRING_SAVED_DATA_START

    if(fscanf(fileio_load_sumMatrix, "%s\r\n", load_stringBuf)<0)    chk_incorrectFormCnt++;
            //printf("%s\r\n", load_stringBuf);//FILE_IO_DATA_PARSING_KEY_STRING_KIND_OF_DATA_START
    if(fscanf(fileio_load_sumMatrix, "Kind Of Data, %d\r\n", &load_kindOfData)<0)    chk_incorrectFormCnt++;
            //printf("Kind Of Data, %d\r\n", load_kindOfData);//FILE_IO_KIND_OF_DATA_SUMATION_MATRIX
    if(fscanf(fileio_load_sumMatrix, "%s\r\n", load_stringBuf)<0)        chk_incorrectFormCnt++;
            //printf("%s\r\n", load_stringBuf);//FILE_IO_DATA_PARSING_KEY_STRING_KIND_OF_DATA_END


    if(fscanf(fileio_load_sumMatrix, "%s\r\n", load_stringBuf)<0)        chk_incorrectFormCnt++;
            //printf("%s\r\n", load_stringBuf);//FILE_IO_DATA_PARSING_KEY_STRING_ARRAY_TYPE_START
    if(fscanf(fileio_load_sumMatrix, "Saved Data Type, %d\r\n", &load_saveOfData)<0)        chk_incorrectFormCnt++;
            //printf("Saved Data Type, %d\r\n", load_saveOfData);//FILD_IO_DATA_ARRAY_TYPE_LINEAR
    if(fscanf(fileio_load_sumMatrix, "%s\r\n", load_stringBuf)<0)        chk_incorrectFormCnt++;
            //printf("%s\r\n", load_stringBuf);//FILE_IO_DATA_PARSING_KEY_STRING_ARRAY_TYPE_END

    if(fscanf(fileio_load_sumMatrix, "%s\r\n", load_stringBuf)<0)        chk_incorrectFormCnt++;
            //printf("%s\r\n", load_stringBuf);//FILE_IO_DATA_PARSING_KEY_STRING_BIT_WIDTH_START
    if(fscanf(fileio_load_sumMatrix, "Bit Width, %d\r\n", &load_bitWidth)<0)                                chk_incorrectFormCnt++;
            //printf("Bit Width, %d\r\n", load_bitWidth);
        //if((*((*(p->element+0))+0))->length != load_bitWidth)
        //{
        //    chk_incorrectDataCnt++;
        //}
    if(fscanf(fileio_load_sumMatrix, "%s\r\n", load_stringBuf)<0)        chk_incorrectFormCnt++;
            //printf("%s\r\n", load_stringBuf);//FILE_IO_DATA_PARSING_KEY_STRING_BIT_WIDTH_END


    if(fscanf(fileio_load_sumMatrix, "%s\r\n", load_stringBuf)<0)        chk_incorrectFormCnt++;
            //printf("%s\r\n", load_stringBuf);//FILE_IO_DATA_PARSING_KEY_STRING_ROW_DATA_START
    if(fscanf(fileio_load_sumMatrix, "Matrix row length, %d\r\n", &load_row)<0)                            chk_incorrectFormCnt++;
            //printf("Matrix row length, %d\r\n", load_row);
        //if(p->row != load_row)
        //{
        //    chk_incorrectDataCnt++;
        //}

    if(fscanf(fileio_load_sumMatrix, "%s\r\n", load_stringBuf)<0)            chk_incorrectFormCnt++;
            //printf("%s\r\n", load_stringBuf);//FILE_IO_DATA_PARSING_KEY_STRING_ROW_DATA_END


    if(fscanf(fileio_load_sumMatrix, "%s\r\n", load_stringBuf)<0)        chk_incorrectFormCnt++;
            //printf("%s\r\n", load_stringBuf);//FILE_IO_DATA_PARSING_KEY_STRING_COL_DATA_START
    if(fscanf(fileio_load_sumMatrix, "Matrix column length, %d\r\n", &load_column)<0)                    chk_incorrectFormCnt++;
            //printf("Matrix column length, %d\r\n", load_column);
        //if(p->column != load_column)
        //{
        //    chk_incorrectDataCnt++;
        //}
    if(fscanf(fileio_load_sumMatrix, "%s\r\n", load_stringBuf)<0)            chk_incorrectFormCnt++;
            //printf("%s\r\n", load_stringBuf);//FILE_IO_DATA_PARSING_KEY_STRING_COL_DATA_END

    if(fscanf(fileio_load_sumMatrix, "%s\r\n", load_stringBuf)<0)                                                chk_incorrectFormCnt++;
            //printf("%s\r\n", load_stringBuf);

    if(fscanf(fileio_load_sumMatrix, "%s\r\n", load_stringBuf)<0)        chk_incorrectFormCnt++;
            //printf("%s\r\n", load_stringBuf);//FILE_IO_DATA_PARSING_KEY_STRING_RAW_DATA_START
    for(tmp_i=0; tmp_i<load_row; tmp_i++)
    {
        for(tmp_j=0; tmp_j<load_column; tmp_j++)
        {
            if(tmp_i != tmp_j)
            {
                if(tmp_j<(load_column-1))
                {
                    if(fscanf(fileio_load_sumMatrix, "%d(%d), ",
                        &load_exp_integer,
                        &load_index
                    )<0)    chk_incorrectFormCnt++;
                }
                else
                {
                    if(fscanf(fileio_load_sumMatrix, "%d(%d)",
                        &load_exp_integer,
                        &load_index
                    )<0)    chk_incorrectFormCnt++;
                }
                        //printf("%d(%d)%s", load_exp_integer, load_index, ((tmp_j<(load_column-1))?", ":""));
                (*((*(p->element+tmp_i))+tmp_j)) = (*(galoisFields->element+load_index));
            }
            else
            {
                if(tmp_j<(load_column-1))
                {
                    if(fscanf(fileio_load_sumMatrix, "%c(%d), ",
                        &load_exp_char,
                        &load_index
                    )<0)    chk_incorrectFormCnt++;
                }
                else
                {
                    if(fscanf(fileio_load_sumMatrix, "%c(%d)",
                        &load_exp_char,
                        &load_index
                    )<0)    chk_incorrectFormCnt++;
                }
                        //printf("%c(%d)%s", load_exp_char, load_index, ((tmp_j<(load_column-1))?", ":""));
                (*((*(p->element+tmp_i))+tmp_j)) = (*(galoisFields->element+load_index));
            }
        }
        if(fscanf(fileio_load_sumMatrix, "\r\n")<0)    chk_incorrectFormCnt++;
                //printf("\r\n");
    }
    if(fscanf(fileio_load_sumMatrix, "%s\r\n", load_stringBuf)<0)            chk_incorrectFormCnt++;
            //printf("%s\r\n", load_stringBuf);//FILE_IO_DATA_PARSING_KEY_STRING_RAW_DATA_END
    if(fscanf(fileio_load_sumMatrix, "%s\r\n", load_stringBuf)<0)        chk_incorrectFormCnt++;
            //printf("%s\r\n", load_stringBuf);//FILE_IO_DATA_PARSING_KEY_STRING_SAVED_DATA_END

    fclose(fileio_load_sumMatrix);

                printf("[load_struct_summationMatrix END]\r\n");

            #ifndef RELEASE
            if(global_flag_gfCommon_display&FLAG_MASK_DISPLAY_GF_COMMON_FILE_IO)
            {
                printf(">> Loaded Summation Matrix <<\r\n");
                printSummationMatrixSavedForm(p);
            }
            #endif



    if(chk_incorrectFormCnt || chk_incorrectDataCnt)    return 1;
    else                                                return 0;
}

void printSummationMatrixSavedForm(struct_summationMatrix *p)
{
    unsigned int tmp_i,tmp_j;

    #ifndef RELEASE
    if(!p)
    {
        errorMes;
        printf("in printSummationMatrixSavedForm, struct_summationMatrix *p is NULL\r\n");
        return;
    }
    #endif

    printf(">> Summation Matrix File I/O Form <<\n");

    for(tmp_i=0; tmp_i<p->row; tmp_i++)
    {
        for(tmp_j=0; tmp_j<p->column; tmp_j++)
        {
            if((*((*(p->element+tmp_i))+tmp_j))->index)
            {
                printf("%d(%d)%s",
                    ((*((*(p->element+tmp_i))+tmp_j))->index - 1),
                    (*((*(p->element+tmp_i))+tmp_j))->index,
                    ((tmp_j<(p->column-1))?", ":"")
                );
            }
            else
            {
                printf("%c(%d)%s",
                    '-',
                    (*((*(p->element+tmp_i))+tmp_j))->index,
                    ((tmp_j<(p->column-1))?", ":"")
                );

            }

        }
        printf("\r\n");
    }
}
