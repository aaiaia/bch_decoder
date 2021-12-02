`ifndef CTR_INFO_LOADERS_INDE
`define CTR_INFO_LOADERS_INDE

`include "60_fifo_channel/fifo_multi_ch.v"
`include "61_shiftReg/sequentialToSimultaneousReg_psLast.v"
`include "61_shiftReg/simultaneousToSequantialReg_2.v"

module ctr_infoLoaders_independent
(
    clk,
    in_ctr_Srst,
    in_ctr_en,

    in_ctr_sig,
    in_info,
    out_info
);
    /****************************************************************************
    ********************************************************************************
    ********************************************************************************
    CASE 1(INPUT_BUS_EN = 0, OUTPUT_BUS_EN = 1, sereis to parallel)
    ********************************************************************************
    In case 1-0-0(IND_DISTANCE_INFO = 3, PASSING_LAST_EN = 0, OUT_SIG_EN = 0),
    module 0 (Series)
        ind_en(EN_0) signal.
    module 1
        init(CTR_0) signal.
    module 2
        init(CTR_1) signal.
    EN BUS WIDTH = 1
    CTR BUS WIDTH = 2

    CTR[0] : EN_0(module 0 enable)
    CTR[1] : CTR_0(module 1 init)
    CTR[2] : CTR_1(module 2 init)
    ********************************************************************************
    In case 1-1-0(IND_DISTANCE_INFO = 3, PASSING_LAST_EN = 1, OUT_SIG_EN = 0),
    module 0 (Series)
        ind_en(EN_0)
        passingLast(CTR_0, module 1 done signal).
    module 1
        init(CTR_1).
    module 2
        init(CTR_2).
    EN BUS WIDTH = 1
    CTR BUS WIDTH = 3

    CTR[0] : EN_0(module 0 enable)
    CTR[1] : CTR_0(module 0 done)
    CTR[2] : CTR_1(module 1 init)
    CTR[3] : CTR_2(module 2 init)
    ********************************************************************************
    In case 1-0-1(IND_DISTANCE_INFO = 3, PASSING_LAST_EN = 0, OUT_SIG_EN = 1),
    module 0 (Series)
        ind_en(EN_0).
    module 1
        init(CTR_0).
    module 2
        init(CTR_1)
        outEnable(CTR_2, module 2 done signal).
    EN BUS WIDTH = 1
    CTR BUS WIDTH = 3

    CTR[0] : EN_0(module 0 enable)
    CTR[1] : CTR_0(module 1 init)
    CTR[2] : CTR_1(module 2 init)
    CTR[3] : CTR_2(module 3 done)
    ********************************************************************************
    In case 1-1-1(IND_DISTANCE_INFO = 3, PASSING_LAST_EN = 1, OUT_SIG_EN = 1),
    module 0 (Series)
        ind_en(EN_0)
        passingLast(CTR_0, module 0 done signal).
    module 1
        init(CTR_1)
    module 2
        init(CTR_2)
        outEnable(CTR_3, module 2 done signal).
    EN BUS WIDTH = 1
    CTR BUS WIDTH = 4

    CTR[0] : EN_0(module 0 enable)
    CTR[1] : CTR_0(module 0 done)
    CTR[2] : CTR_1(module 1 init)
    CTR[3] : CTR_2(module 2 init)
    CTR[4] : CTR_3(module 2 done)
    ********************************************************************************
    ********************************************************************************
    CASE 2(INPUT_BUS_EN = 1, OUTPUT_BUS_EN = 0, parallel to serial)
    ********************************************************************************
    In case 2-0-0(IND_DISTANCE_INFO = 3, PASSING_LAST_EN = 0, OUT_SIG_EN = 0),
    In case 2-1-0(IND_DISTANCE_INFO = 3, PASSING_LAST_EN = 1, OUT_SIG_EN = 0),
    module 0
        init(CTR_0)
    module 1
        init(CTR_1)
    module 2 (Series)
        init(CTR_2)
        ind_en(EN_0)
    EN BUS WIDTH = 1
    CTR BUS WIDTH = 3

    CTR[0] : EN_1(module 2 shift en)
    CTR[1] : CTR_0(module 0 init)
    CTR[2] : CTR_1(module 1 init)
    CTR[3] : CTR_2(module 2 init)
    ********************************************************************************
    In case 2-0-1(IND_DISTANCE_INFO = 3, PASSING_LAST_EN = 0, OUT_SIG_EN = 1),
    In case 2-1-1(IND_DISTANCE_INFO = 3, PASSING_LAST_EN = 1, OUT_SIG_EN = 1),
    module 0
        init(CTR_0)
    module 1
        init(CTR_1)
    module 2 (Series)
        init(CTR_2)
        ind_en(EN_0)
        outEnable(CTR_3, module 3 done signal).
    EN BUS WIDTH = 1
    CTR BUS WIDTH = 4

    CTR[0] : EN_1(module 2 shift en)
    CTR[1] : CTR_0(module 0 init)
    CTR[2] : CTR_1(module 1 init)
    CTR[3] : CTR_2(module 2 init)
    CTR[4] : CTR_3(module 2 out en)
    ********************************************************************************
    ********************************************************************************
    CASE 3(INPUT_BUS_EN = 0, OUTPUT_BUS_EN = 0, series to series)
    ********************************************************************************
    In case 3-0-0(IND_DISTANCE_INFO = 3, PASSING_LAST_EN = 0, OUT_SIG_EN = 0),
    module 0 (Series)
        ind_en(EN_0).
    module 1
        init(CTR_0)
    module 2 (Series)
        init(CTR_1)
        ind_en(EN_1)
    EN BUS WIDTH = 2
    CTR BUS WIDTH = 2

    CTR[0] : EN_0
    CTR[1] : EN_1
    CTR[2] : CTR_0
    CTR[3] : CTR_1
    ********************************************************************************
    In case 3-1-0(IND_DISTANCE_INFO = 3, PASSING_LAST_EN = 1, OUT_SIG_EN = 0),
    module 0 (Series)
        ind_en(EN_0).
        passingLast(CTR_0, module 0 done signal).
    module 1
        init(CTR_1)
    module 2 (Series)
        init(CTR_2)
        ind_en(EN_1)
    EN BUS WIDTH = 2
    CTR BUS WIDTH = 3

    CTR[0] : EN_0
    CTR[1] : EN_1
    CTR[2] : CTR_0
    CTR[3] : CTR_1
    CTR[4] : CTR_2
    ********************************************************************************
    In case 3-0-1(IND_DISTANCE_INFO = 3, PASSING_LAST_EN = 0, OUT_SIG_EN = 1),
    module 0 (Series)
        ind_en(EN_0).
    module 1
        init(CTR_0)
    module 2 (Series)
        init(CTR_1)
        ind_en(EN_1)
        outEnable(CTR_2, module 2 enable signal).
    EN BUS WIDTH = 2
    CTR BUS WIDTH = 3

    CTR[0] : EN_0
    CTR[1] : EN_1
    CTR[2] : CTR_0
    CTR[3] : CTR_1
    CTR[4] : CTR_2
    ********************************************************************************
    In case 3-1-1(IND_DISTANCE_INFO = 3, PASSING_LAST_EN = 1, OUT_SIG_EN = 1),
    module 0 (Series)
        ind_en(EN_0).
        passingLast(CTR_0, module 1 done signal).
    module 1
        init(CTR_1)
    module 2 (Series)
        init(CTR_2)
        ind_en(EN_1)
        outEnable(CTR_3, module 2 enable signal).
    EN BUS WIDTH = 2
    CTR BUS WIDTH = 4

    CTR[0] : EN_0
    CTR[1] : EN_1
    CTR[2] : CTR_0
    CTR[3] : CTR_1
    CTR[4] : CTR_2
    CTR[5] : CTR_3
    ********************************************************************************
    ********************************************************************************
    CASE 4(INPUT_BUS_EN = 1, OUTPUT_BUS_EN = 1, parallel to parallel)
    ********************************************************************************
    In case 4-0-0(IND_DISTANCE_INFO = 3, PASSING_LAST_EN = 0, OUT_SIG_EN = 0),
    In case 4-1-0(IND_DISTANCE_INFO = 3, PASSING_LAST_EN = 1, OUT_SIG_EN = 0),
    module 0
        init(CTR_0)
    module 1
        init(CTR_1)
    module 2
        init(CTR_2)
    EN BUS WIDTH = 0
    CTR BUS WIDTH = 3

    CTR[0] : CTR_0 (module 0 init)
    CTR[1] : CTR_1 (module 1 init)
    CTR[2] : CTR_2 (module 2 init)
    ********************************************************************************
    In case 4-0-1(IND_DISTANCE_INFO = 3, PASSING_LAST_EN = 0, OUT_SIG_EN = 1),
    In case 4-1-1(IND_DISTANCE_INFO = 3, PASSING_LAST_EN = 1, OUT_SIG_EN = 1),
    module 0
        init(CTR_0)
    module 1
        init(CTR_1)
    module 2
        init(CTR_2)
        outEnable(CTR_3, module 2 enable signal).
    EN BUS WIDTH = 0
    CTR BUS WIDTH = 4

    CTR[0] : CTR_0 (module 0 init)
    CTR[1] : CTR_1 (module 1 init)
    CTR[2] : CTR_2 (module 2 init)
    CTR[3] : CTR_3 (module 2 out en)
    ********************************************************************************
    *********************************************************************************/
    /***********************************************************************/
    /***********************************************************************/
    function integer    notBeingZero;
        input   integer value;

        begin
            if(value > 0)   notBeingZero = value;
            else            notBeingZero = 1;
        end
    endfunction
    /***********************************************************************/
    function integer    minValChker;
        input   integer minVal;
        input   integer value;

        begin
            if(value < minVal)
            begin
                    minValChker = minVal;
                    $display("ERROR!!!) in ctr_infoLoaders_independent, value(%d) has to larger or equal then minVal(%d).", value, minVal);
            end
            else    minValChker = value;
        end
    endfunction
    /***********************************************************************/
    function integer    valueToBool;
        input   integer value;

        begin
            if(value)   valueToBool = 1;
            else        valueToBool = 0;
        end
    endfunction
    /***********************************************************************/
    function integer    valueToNotBool;
        input   integer value;

        begin
            if(!value)   valueToNotBool = 1;
            else        valueToNotBool = 0;
        end
    endfunction
    /***********************************************************************/
    /***********************************************************************/

    /****************************************
    * Offset is started at -1.
        *
        * Values are concluded below sequence.
        * 1. LO_INDEX_EN_FIR (MODULE 0)
        * 2. LO_INDEX_EN_LAS (MODULE LAST)
        * 3. LO_INDEX_PASS_LAST_FIR (MODULE 0)
        * 4. LO_INDEX_INIT_BUS_MID2LAST_START (MODULE 1 ~ LAST - 1)
        * 5. LO_INDEX_INIT_BUS_MID_END (MODULE 1 ~ LAST - 1)
        * 6. LO_INDEX_OUT_EN(MODULE LAST)
        * ****************************************/
       /****************************************
       * 1. LO_INDEX_EN_FIR (MODULE 0)
       * ****************************************/
//      function integer first_en_index;
//          input integer startVal;
//          input integer in_n_bus_en;
//
//          begin
//              first_en_index = in_n_bus_en + startVal;
//          end
//
//      endfunction
      /******************************/

      /****************************************
      * 2. LO_INDEX_EN_LAS (MODULE LAST)
      * ****************************************/
     function integer last_en_index;
         input integer startVal;
         input integer out_n_bus_en;

         begin
             last_en_index = out_n_bus_en + startVal;
         end

     endfunction
     /******************************/

     /****************************************
     * 3. LO_INDEX_PASS_LAST_FIR (MODULE 0)
     * ****************************************/
//    function integer passLast_index;
//        input integer startVal;
//        input integer passLast_en;
//
//        begin
//            passLast_index = passLast_en + startVal;
//        end
//
//    endfunction
    /****************************************
    * 4. LO_INDEX_INIT_BUS_MID2LAST_START (MODULE 1 ~ LAST - 1)
    * ****************************************/
    function integer cal_bus_index_start;
        input integer startVal;
        input integer bus_width;

        integer bus_valid_chk;

        begin
            if(bus_width)   bus_valid_chk = 1;
            else            bus_valid_chk = 0;

            cal_bus_index_start = startVal + bus_valid_chk;
        end

    endfunction
    /****************************************
    * 5. LO_INDEX_INIT_BUS_MID_END (MODULE 1 ~ LAST - 1)
    * ****************************************/
    function integer cal_bus_index_end;
        input integer startVal;
        input integer bus_width;

        integer bus_valid_chk;
        begin
            if(bus_width)   bus_valid_chk = 1;
            else            bus_valid_chk = 0;

            cal_bus_index_end = startVal + (bus_width - 1) * bus_valid_chk;
        end

    endfunction
    /****************************************
    * 6. LO_INDEX_OUT_EN(MODULE LAST)
    * ****************************************/
//    function integer out_en_index;
//        input integer startVal;
//        input integer out_en;
//
//        begin
//            out_en_index = out_en + startVal;
//        end
//
//    endfunction
    /***********************************************************************/
    /***********************************************************************/
    function    integer sameValChk;
        input   integer val1;
        input   integer val2;

        begin
            if(val1 == val2)    sameValChk = 1;
            else begin
                sameValChk = 0;
            end
        end
    endfunction
    /***********************************************************************/
    /***********************************************************************/


    genvar i;

    parameter   OUTTER_NAME   = "";
    parameter   MODULE_NAME     = "ctr_infoLoaders_independent";
        localparam  DESIGN_NAME = "ctr_infoLoaders_independent";

    /* input / output buffer start */
    parameter   IN_BUF_LEN              =   0;
        localparam  LO_PARAM_IN_BUF_EN  = valueToBool(IN_BUF_LEN);
    parameter   OUT_BUF_LEN             =   0;
        localparam  LO_PARAM_OUT_BUF_EN = valueToBool(OUT_BUF_LEN);
    /* input / output buffer end */

    parameter   IND_DISTANCE_INFO   =   2;
        localparam  LO_PARAM_IND_DISTANCE_INFO = minValChker(2, IND_DISTANCE_INFO);

        localparam  LO_PARAM_GLUE_BLK_NUMS      = LO_PARAM_IND_DISTANCE_INFO - 2;

    parameter   CLK_DISTANCE_INFO   =   1;
        localparam  LO_PARAM_CLK_DISTANCE_INFO   =   notBeingZero(CLK_DISTANCE_INFO);

    /* input bus enable */
    parameter   INPUT_BUS_EN                =   0;
        localparam  LO_PARAM_IN_BUS_EN      =   valueToBool(INPUT_BUS_EN);
        localparam  LO_PARAM_IN_N_BUS_EN    =   valueToNotBool(INPUT_BUS_EN);
    /* input bus enable */

    /* output bus enable */
    parameter   OUTPUT_BUS_EN               =   0;
        localparam  LO_PARAM_OUT_BUS_EN     =   valueToBool(OUTPUT_BUS_EN);
        localparam  LO_PARAM_OUT_N_BUS_EN   =   valueToNotBool(OUTPUT_BUS_EN);
    /* output bus enable */

    parameter   PASSING_LAST_EN =   0;
        localparam  LO_PARAM_PASSING_LAST_EN    =   valueToBool(PASSING_LAST_EN);

        localparam  LO_PARAM_USE_INIT_SIG_FIR       =   LO_PARAM_IN_BUS_EN;

    parameter   OUT_SIG_EN              =   0;
        localparam  LO_PARAM_OUT_SIG_EN    =   valueToBool(OUT_SIG_EN);

    parameter   INFO_NUMS                           =   8;
        localparam  LO_PARAM_INFO_NUMS              =   notBeingZero(INFO_NUMS);

    parameter   BIT_WIDTH_INFO                   =   2;
        localparam  LO_PARAM_BIT_WIDTH_INFO      =   notBeingZero(BIT_WIDTH_INFO);

        localparam  LO_PARAM_BUS_BIT_WIDTH_INFO =   LO_PARAM_INFO_NUMS * LO_PARAM_BIT_WIDTH_INFO;  

        localparam  LO_PARAM_IN_BUS_WIDTH_INFO      =   
            ((LO_PARAM_INFO_NUMS - 1) * LO_PARAM_IN_BUS_EN + 1) * LO_PARAM_BIT_WIDTH_INFO;
        localparam  LO_PARAM_OUT_BUS_WIDTH_INFO     =   
            ((LO_PARAM_INFO_NUMS - 1) * LO_PARAM_OUT_BUS_EN + 1) * LO_PARAM_BIT_WIDTH_INFO;

    /* bypass is enabled, any register is not instanciated.  */
    parameter   BYPASS_INFO_EN  =   0;
        localparam  LO_PA_BYPASS_INFO_EN    =   
            valueToBool(BYPASS_INFO_EN) * sameValChk(LO_PARAM_IN_BUS_EN, LO_PARAM_OUT_BUS_EN);
        localparam  LO_PA_BYPASS_INFO_DIS   =   valueToNotBool(LO_PA_BYPASS_INFO_EN);


    /* Controlling signal indexing start */
    localparam  LO_INDEX_CTR_START              = -1;
        localparam  LO_INDEX_EN_FIR             = LO_INDEX_CTR_START + 
                                                    LO_PA_BYPASS_INFO_DIS * LO_PARAM_IN_N_BUS_EN;
        localparam  LO_INDEX_EN_LAS             = LO_INDEX_EN_FIR + 
                                                    LO_PA_BYPASS_INFO_DIS * LO_PARAM_OUT_N_BUS_EN;
        localparam  LO_INDEX_PASS_LAST_FIR      = LO_INDEX_EN_LAS + 
                                                    LO_PA_BYPASS_INFO_DIS * LO_PARAM_PASSING_LAST_EN;
        localparam  LO_INDEX_INIT_SIG_FIR       = LO_INDEX_PASS_LAST_FIR + 
                                                    LO_PA_BYPASS_INFO_DIS * LO_PARAM_USE_INIT_SIG_FIR;
        localparam  LO_INDEX_INIT_BUS_MID_START = cal_bus_index_start(LO_INDEX_INIT_SIG_FIR, 
                                                    LO_PA_BYPASS_INFO_DIS * LO_PARAM_GLUE_BLK_NUMS);
        localparam  LO_INDEX_INIT_BUS_MID_END   = cal_bus_index_end(LO_INDEX_INIT_BUS_MID_START, 
                                                    LO_PA_BYPASS_INFO_DIS * LO_PARAM_GLUE_BLK_NUMS);
        localparam  LO_INDEX_INIT_SIG_LAS       = LO_INDEX_INIT_BUS_MID_END + 
                                                    LO_PA_BYPASS_INFO_DIS * 1;
        localparam  LO_INDEX_OUT_EN             = LO_INDEX_INIT_SIG_LAS + 
                                                    LO_PA_BYPASS_INFO_DIS * LO_PARAM_OUT_SIG_EN;
    localparam  LO_INDEX_CTR_END                = LO_INDEX_OUT_EN;
    /* Controlling signal indexing end */
        localparam  LO_PARAM_CTR_INFO_BUS_WIDTH     = 
            LO_INDEX_CTR_END + 1;


    /***********************************************************************/
    /***********************************************************************/
    input   clk;
    input   in_ctr_Srst;
    input   in_ctr_en;
    /***********************************************************************/
    /* If OUT_SIG_EN has any value except 0, in_ctr_sig[0] is a get
    * signal about in_info and in_ctr_sig[1] is a out signal of out_info */
    input   [LO_PARAM_CTR_INFO_BUS_WIDTH - 1 : 0]       in_ctr_sig;
    input   [LO_PARAM_IN_BUS_WIDTH_INFO - 1 : 0]        in_info;

    output  [LO_PARAM_OUT_BUS_WIDTH_INFO - 1 : 0]       out_info;
    /***********************************************************************/
    wire    [LO_PARAM_CTR_INFO_BUS_WIDTH - 1: 0]        w_in_ctr_sig;
    wire    [LO_PARAM_IN_BUS_WIDTH_INFO - 1 : 0]        w_in_info;

    wire    [LO_PARAM_OUT_BUS_WIDTH_INFO - 1 : 0]       w_out_info;
    /***********************************************************************/
    /***********************************************************************/


    /***********************************************************************/
    /***********************************************************************/

    /*Input Buffer definition*/
    generate
    /* input buffer */
        /*input buffer degree*/
        if(LO_PARAM_IN_BUF_EN) begin : gen_in_buf_en
            fifo_multi_ch
            #(
                .OUTTER_NAME(DESIGN_NAME),
                .MODULE_NAME("uut_in_buf"),

                .FIFO_LEN(IN_BUF_LEN),
                .FIFO_CH_WIDTH(LO_PARAM_CTR_INFO_BUS_WIDTH + LO_PARAM_IN_BUS_WIDTH_INFO)
            )
            uut_in_buf
            (
                .clk(clk),

                .in_ctr_Srst(in_ctr_Srst),
                .in_ctr_en(in_ctr_en),

                .in({in_ctr_sig, in_info}),
                .out({w_in_ctr_sig, w_in_info})
            );
        end
        else begin : gen_in_buf_dis
            assign  w_in_ctr_sig   =   in_ctr_sig;
            assign  w_in_info       =   in_info;
        end
        /*input buffer degree*/
    /* input buffer */
    endgenerate 

    /*First, middle, last infomation Loader definition*/
    generate
        if(LO_PA_BYPASS_INFO_EN) begin : byPassData
            assign  w_out_info = w_in_info;
        end
        else begin : no_byPassData
            wire    [LO_PARAM_BUS_BIT_WIDTH_INFO * (LO_PARAM_IND_DISTANCE_INFO - 1) - 1 : 0]
                        w_inner_wire;
            /***** Input information Loader *****/
            /*Input is sereis*/
            if(!LO_PARAM_IN_BUS_EN)
            begin : gen_in_ser_Loader
                /*Passing last is disabled*/
                if(!PASSING_LAST_EN)
                begin : passLastDis
                    simultaneousToSequantialReg_2
                    #(
                        .DIRECTION(1),
                        .SHIFT_LEN(INFO_NUMS),
                        .BIT_WIDTH(BIT_WIDTH_INFO)
                    )
                    uut_in_ser_infoLader_nonPL
                    (
                        .clk(clk),

                        .in_ctr_Srst(in_ctr_Srst),
                        .in_ctr_en(in_ctr_en),
                        .in_ctr_sft_en(w_in_ctr_sig[LO_INDEX_EN_FIR]),

                        .in(w_in_info),
                        .out(w_inner_wire[LO_PARAM_BUS_BIT_WIDTH_INFO * 1 - 1 : 0])
                    );
                end
                /*Passing last is enabled*/
                else
                begin : passLastEn
                    sequentialToSimultaneousReg_psLast
                    #(
                        .DIRECTION(1),
                        .SHIFT_LEN(INFO_NUMS),
                        .BIT_WIDTH(BIT_WIDTH_INFO)
                    )
                    uut_in_ser_infoLoader_PL
                    (
                        .clk(clk),

                        .in_ctr_Srst(in_ctr_Srst),
                        .in_ctr_en(w_in_ctr_sig[LO_INDEX_EN_FIR] & in_ctr_en),
                        .in_ctr_last(w_in_ctr_sig[LO_INDEX_PASS_LAST_FIR]),

                        .in(w_in_info),
                        .out(w_inner_wire[LO_PARAM_BUS_BIT_WIDTH_INFO * 1 - 1 : 0])
                    );
                end
            end
            /*Input is parallel*/
            else
            begin : gen_in_par_Loader
                /* parallel init signal handling */
                fifo_multi_ch
                #(
                    .FIFO_LEN(1),
                    .FIFO_CH_WIDTH(LO_PARAM_IN_BUS_WIDTH_INFO) 
                )
                uut_in_par_infoLader
                (
                    .clk(clk),

                    .in_ctr_Srst(in_ctr_Srst),
                    .in_ctr_en(w_in_ctr_sig[LO_INDEX_INIT_SIG_FIR] & in_ctr_en),

                    .in(w_in_info),
                    .out(w_inner_wire[LO_PARAM_BUS_BIT_WIDTH_INFO * 1 - 1 : 0])
                );
            end

            /***** Middle information Loader *****/
            for(i = 0; i < LO_PARAM_GLUE_BLK_NUMS; i = i + 1)
            begin : gen_mid_infoLoader
                fifo_multi_ch
                #(
                    .FIFO_LEN(1),
                    .FIFO_CH_WIDTH(LO_PARAM_BUS_BIT_WIDTH_INFO) 
                )
                uut_mid_infoLader
                (
                    .clk(clk),

                    .in_ctr_Srst(in_ctr_Srst),
                    .in_ctr_en(w_in_ctr_sig[LO_INDEX_INIT_BUS_MID_START + i] & in_ctr_en),

                    .in(w_inner_wire[LO_PARAM_BUS_BIT_WIDTH_INFO * (i + 1) - 1 : LO_PARAM_BUS_BIT_WIDTH_INFO * (i - 0)]),
                    .out(w_inner_wire[LO_PARAM_BUS_BIT_WIDTH_INFO * (i + 2) - 1 : LO_PARAM_BUS_BIT_WIDTH_INFO * (i + 1)])
                );
            end

            /***** Output information Loader *****/
            /*Output is sereis*/
            wire    [LO_PARAM_OUT_BUS_WIDTH_INFO - 1 : 0]       w_bf_O_EN_info;

            if(!LO_PARAM_OUT_BUS_EN)
            begin : gen_out_ser_Loader
                simultaneousToSequantialReg_2
                #(
                .DIRECTION(1),
                .SHIFT_LEN(INFO_NUMS),
                .BIT_WIDTH(BIT_WIDTH_INFO)
                )
                uut_out_ser_infoLoader
                (
                    .clk(clk),

                    .in_ctr_Srst(in_ctr_Srst),
                    .in_ctr_init(w_in_ctr_sig[LO_INDEX_INIT_SIG_LAS]),
                    .in_ctr_en(in_ctr_en),
                    .in_ctr_sft_en(w_in_ctr_sig[LO_INDEX_EN_LAS]),

                    .in(w_inner_wire[LO_PARAM_BUS_BIT_WIDTH_INFO * (LO_PARAM_IND_DISTANCE_INFO - 1) - 1 : 
                                    LO_PARAM_BUS_BIT_WIDTH_INFO * (LO_PARAM_IND_DISTANCE_INFO - 2)]),
                    .out(w_bf_O_EN_info)
                );
            end
            /*Output is parallel*/
            else
            begin : gen_out_par_Loader
                fifo_multi_ch
                #(
                    .FIFO_LEN(1),
                    .FIFO_CH_WIDTH(LO_PARAM_OUT_BUS_WIDTH_INFO) 
                )
                uut_out_par_infoLader
                (
                    .clk(clk),

                    .in_ctr_Srst(in_ctr_Srst),
                    .in_ctr_en(w_in_ctr_sig[LO_INDEX_INIT_SIG_LAS] & in_ctr_en),

                    .in(w_inner_wire[LO_PARAM_OUT_BUS_WIDTH_INFO * (LO_PARAM_IND_DISTANCE_INFO - 1) - 1 : 
                                    LO_PARAM_OUT_BUS_WIDTH_INFO * (LO_PARAM_IND_DISTANCE_INFO - 2)]),
                    .out(w_bf_O_EN_info)
                );
            end

            /*Output enable signal*/
            if(LO_PARAM_OUT_SIG_EN)
            begin : gen_out_enSig_en
                assign w_out_info = w_in_ctr_sig[LO_INDEX_OUT_EN] ? 
                    w_bf_O_EN_info : {(LO_PARAM_OUT_BUS_WIDTH_INFO){1'b0}};
            end
            else
            begin : gen_out_enSig_dis
                assign w_out_info = w_bf_O_EN_info;
            end
        end
    endgenerate

    /*Output Buffer Definition*/
    generate
        /* output buffer */
        if(LO_PARAM_OUT_BUF_EN) begin : gen_out_buf_en
            fifo_multi_ch
            #(
                .OUTTER_NAME(DESIGN_NAME),
                .MODULE_NAME("uut_out_buf"),

                .FIFO_LEN(LO_PARAM_OUT_BUF_EN),
                .FIFO_CH_WIDTH(LO_PARAM_OUT_BUS_WIDTH_INFO)
            )
            uut_out_buf
            (
                .clk(clk),

                .in_ctr_Srst(in_ctr_Srst),
                .in_ctr_en(in_ctr_en),

                .in(w_out_info),
                .out(out_info)
            );
        end
        else
        begin : gen_out_buf_dis
            assign out_info = w_out_info;
        end
    endgenerate

    /***********************************************************************/
    /***********************************************************************/
    initial begin
        $display("/**************************************************/");
        $display("Instanciating %s", DESIGN_NAME);

        $display("In %s," ,OUTTER_NAME);
        $display("MODULE_NAME : %s", MODULE_NAME);
        $display("DESIGN_NAME : %s", DESIGN_NAME);

        $display("IN_BUF_LEN = %d",                     IN_BUF_LEN);
        $display("LO_PARAM_IN_BUF_EN : %s",             LO_PARAM_IN_BUF_EN ? "Enabled":"Disabled");
        $display("OUT_BUF_LEN = %d",                    OUT_BUF_LEN);
        $display("LO_PARAM_OUT_BUF_EN : %s",            LO_PARAM_OUT_BUF_EN ? "Enabled":"Disabled");

        $display("IND_DISTANCE_INFO = %d",              IND_DISTANCE_INFO);
        $display("LO_PARAM_IND_DISTANCE_INFO = %d",     LO_PARAM_IND_DISTANCE_INFO);
        $display("LO_PARAM_GLUE_BLK_NUMS = %d",         LO_PARAM_GLUE_BLK_NUMS);

        $display("CLK_DISTANCE_INFO = %d",              CLK_DISTANCE_INFO);
        $display("LO_PARAM_CLK_DISTANCE_INFO = %d",     LO_PARAM_CLK_DISTANCE_INFO);

        $display("INPUT_BUS_EN = %d",                   INPUT_BUS_EN);
        $display("LO_PARAM_IN_BUS_EN : %s",             LO_PARAM_IN_BUS_EN ? "Enabled":"Disabled");
        $display("LO_PARAM_IN_N_BUS_EN : %s",           LO_PARAM_IN_N_BUS_EN ? "Enabled":"Disabled");

        $display("OUTPUT_BUS_EN = %d",                  OUTPUT_BUS_EN);
        $display("LO_PARAM_OUT_BUS_EN : %s",            LO_PARAM_OUT_BUS_EN ? "Enabled":"Disabled");
        $display("LO_PARAM_OUT_N_BUS_EN : %s",          LO_PARAM_OUT_N_BUS_EN ? "Enabled":"Disabled");

        $display("PASSING_LAST_EN = %d",                PASSING_LAST_EN);
        $display("LO_PARAM_PASSING_LAST_EN : %s",       LO_PARAM_PASSING_LAST_EN ? "Enabled" : "Disabled");
        
        $display("LO_PARAM_USE_INIT_SIG_FIR = %d",      LO_PARAM_USE_INIT_SIG_FIR);

        $display("OUT_SIG_EN = %d",                     OUT_SIG_EN);
        $display("LO_PARAM_OUT_SIG_EN : %s",            LO_PARAM_OUT_SIG_EN ? "Enabled" : "Disabled");

        $display("INFO_NUMS = %d",                      INFO_NUMS);
        $display("LO_PARAM_INFO_NUMS = %d",             LO_PARAM_INFO_NUMS);

        $display("BIT_WIDTH_INFO = %d",                 BIT_WIDTH_INFO);
        $display("LO_PARAM_BIT_WIDTH_INFO = %d",        LO_PARAM_BIT_WIDTH_INFO);
        $display("LO_PARAM_BUS_BIT_WIDTH_INFO = %d",    LO_PARAM_BUS_BIT_WIDTH_INFO);
        $display("LO_PARAM_IN_BUS_WIDTH_INFO = %d",     LO_PARAM_IN_BUS_WIDTH_INFO);
        $display("LO_PARAM_OUT_BUS_WIDTH_INFO = %d",    LO_PARAM_OUT_BUS_WIDTH_INFO);

        $display("/* Infoloader bypassing mode? */");
        $display("BYPASS_INFO_EN = %d", BYPASS_INFO_EN);
        $display("LO_PA_BYPASS_INFO_EN = %s", 
            LO_PA_BYPASS_INFO_EN ? "enabled" : "disabled");
        $display("LO_PA_BYPASS_INFO_DIS = %s", 
            LO_PA_BYPASS_INFO_DIS ? "enabled" : "disabled");

        $display("/* Contolling signal index start */");
        $display("LO_INDEX_CTR_START = %d",             LO_INDEX_CTR_START);
        $display("LO_INDEX_EN_FIR = %d",                LO_INDEX_EN_FIR);
        $display("LO_INDEX_EN_LAS = %d",                LO_INDEX_EN_LAS);
        $display("LO_INDEX_PASS_LAST_FIR = %d",         LO_INDEX_PASS_LAST_FIR);
        $display("LO_INDEX_INIT_SIG_FIR = %d",          LO_INDEX_INIT_SIG_FIR);
        $display("LO_INDEX_INIT_BUS_MID_START = %d",    LO_INDEX_INIT_BUS_MID_START);
        $display("LO_INDEX_INIT_BUS_MID_END = %d",      LO_INDEX_INIT_BUS_MID_END);
        $display("LO_INDEX_INIT_SIG_LAS = %d",          LO_INDEX_INIT_SIG_LAS);
        $display("LO_INDEX_OUT_EN = %d",                LO_INDEX_OUT_EN);
        $display("LO_INDEX_CTR_END = %d",               LO_INDEX_CTR_END);
        $display("LO_PARAM_CTR_INFO_BUS_WIDTH = %d",    LO_PARAM_CTR_INFO_BUS_WIDTH);
        $display("/* Contolling signal index end */");
        $display("/**************************************************/");
    end
    /***********************************************************************/
    /***********************************************************************/
endmodule

`endif

