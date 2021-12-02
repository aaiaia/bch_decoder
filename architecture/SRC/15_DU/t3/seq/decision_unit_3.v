`ifndef DECISION_UNIT_3
`define DECISION_UNIT_3

`include "00_GF_mult/gf_10/comb/GF_Mult_2_10_comb.v"
`include "60_fifo_channel/fifo_multi_ch.v"
`include "62_buffer/prior_buffer_1.v"

/* Chien Search */
`include "05_CS/seq/chien_search_series.v"
/* Having priority net modeling */
/*Low side wire has a high priority.*/
`include "80_net_model/prior_sel/prior_w_di_sel_path.v"
module decision_unit_3
(
    clk ,

    in_ctr_Srst ,
    in_ctr_en ,

    in_ctr_sig ,

    in_tp_data ,
    in_alpha_loc ,

    out_sel_tp_errLoc

);
    /************************************************************/
    parameter   OUTTER_NAME =   "";
    parameter   MODULE_NAME =   "decision_unit_3";
        localparam  DESIGN_NAME =   "decision_unit_3";

    parameter   GF_LEN          =   10;
        localparam  PARAM_MAX_GF_LEN    =   pow2(GF_LEN) - 1;

    parameter   GF_SH_FORWARD   =   10'd9;
    parameter   GF_SH_BACKWARD  =   10'd516;

    parameter   PARALLELISM     =   4;
        localparam  LO_PARAM_PARALLELISM    =   chk_notBeingZero(PARALLELISM);
        localparam  LO_PARAM_TP_ERR_LOC_WIDTH  =   LO_PARAM_PARALLELISM * 1;

    parameter   CODEWORD_LEN    =   1020;
        localparam  LO_PARAM_CODEWORD_LEN   =   chk_notBeingZero(CODEWORD_LEN);

    localparam  LO_PARAM_SHIFT_LEN  =   LO_PARAM_CODEWORD_LEN;
        localparam  LO_PARAM_ACTUAL_SHIFT_LEN = 
                        cal_op_clk(LO_PARAM_SHIFT_LEN, LO_PARAM_PARALLELISM) - 1;

        localparam  LO_PARAM_COEF_SHIFT_LEN    =   
                        PARAM_MAX_GF_LEN - LO_PARAM_ACTUAL_SHIFT_LEN * LO_PARAM_PARALLELISM;

        localparam  LO_PARAM_REDUNDANT = LO_PARAM_SHIFT_LEN % LO_PARAM_PARALLELISM;

    parameter   HD_CORRECTABLE  =   3;
        localparam  PARAM_COEF_NUMS =   HD_CORRECTABLE + 1;
    parameter   SD_CORRECTABLE    =   3;
        localparam LO_PARAM_TP_EQ_NUMS    =   pow2(SD_CORRECTABLE);

    parameter   ERR_CNT_LEN     =   2;

    /************************************************************/
    /*
     * 0 is not indicated anything. But FIFO MODE
     * 1 is FIFO,
     * 2 is RE_CHIEN_SEARCH (Not support yet)
     *
    */
    parameter   DECISION_MODE  =   1;
    /************************************************************/
    /* DECISION_MODE 1 uses FIFO channel to save bit streams of chien search,
    * which come from previous block(chien search and metric check block.)*/
    localparam  LO_PARAM_USE_FIFO_CHANNEL_EN   =   useFIFOch_en(DECISION_MODE);
    /* MODE 1 FIFO */
        localparam  LO_PARAM_FIFO_CH_LEN = LO_PARAM_CODEWORD_LEN;

        localparam  LO_PARAM_INPUT_BUS_TP_ERR_LOC_WIDTH =   
                        LO_PARAM_TP_EQ_NUMS * LO_PARAM_TP_ERR_LOC_WIDTH;
    /************************************************************/
    /* DECISION_MODE 2 and 3 use chien search block. */
    localparam  LO_PARAM_USE_RE_CS_EN    =   useCSblk_en(DECISION_MODE);
        localparam  LO_PARAM_INPUT_BUS_COEFICIENT_WIDTH =   
                        GF_LEN * LO_PARAM_TP_EQ_NUMS * PARAM_COEF_NUMS ;
    localparam  LO_PATAM_INDIP_DATA_SEL_TP_SEL_EN = indipendDataAndTpSelSig(DECISION_MODE);
            
        localparam  LO_PARAM_USE_RECS_Y_COM_TP_SEL_EN = val2NotBool(LO_PATAM_INDIP_DATA_SEL_TP_SEL_EN);
        localparam  LO_PARAM_USE_RECS_N_COM_TP_SEL_EN = LO_PATAM_INDIP_DATA_SEL_TP_SEL_EN;

        localparam  LO_PARAM_USE_RECS_VALID_DATA_SIG_EN =   LO_PATAM_INDIP_DATA_SEL_TP_SEL_EN;
        localparam  LO_PARAM_USE_RECS_VALID_D_BUF_EN_SIG_EN = LO_PATAM_INDIP_DATA_SEL_TP_SEL_EN;

        localparam  LO_PARAM_USE_RE_CS_USING_IN_DATA_BUF = LO_PATAM_INDIP_DATA_SEL_TP_SEL_EN;


    /************************************************************/
    //timming ctr buffer for re CS,
    parameter   FOR_RE_CS_PRIOR_SELED_BUF_EN    =   0;
        localparam  LO_PA_FOR_RE_CS_P_SELED_BUF_EN  = 
            LO_PARAM_USE_RE_CS_EN * val2Bool(FOR_RE_CS_PRIOR_SELED_BUF_EN);

    /************************************************************/
    localparam  LO_INDEX_IN_CTR_START   =   -1;
        localparam  LO_INDEX_IN_CTR_BUS_DATA_SEL_START = 
            LO_INDEX_IN_CTR_START + 1;
        localparam  LO_INDEX_IN_CTR_BUS_DATA_SEL_END = 
            LO_INDEX_IN_CTR_BUS_DATA_SEL_START + LO_PARAM_TP_EQ_NUMS - 1;
        
        localparam  LO_INDEX_IN_CTR_VALID_DATA_SIG  =   
            LO_INDEX_IN_CTR_BUS_DATA_SEL_END + LO_PARAM_USE_RECS_VALID_DATA_SIG_EN;
        localparam  LO_INDEX_IN_CTR_TO_SEL_VALID_DATA_BUF_ENSIG = 
            LO_INDEX_IN_CTR_VALID_DATA_SIG + LO_PARAM_USE_RECS_VALID_D_BUF_EN_SIG_EN;

        localparam LO_INDEX_IN_CTR_BUS_TP_SEL_START =   
            LO_PARAM_USE_RECS_Y_COM_TP_SEL_EN * (LO_INDEX_IN_CTR_BUS_DATA_SEL_START) +
            LO_PARAM_USE_RECS_N_COM_TP_SEL_EN * (LO_INDEX_IN_CTR_TO_SEL_VALID_DATA_BUF_ENSIG + 1);
        localparam  LO_INDEX_IN_CTR_BUS_TP_SEL_END = 
            LO_PARAM_USE_RECS_Y_COM_TP_SEL_EN * (LO_INDEX_IN_CTR_BUS_DATA_SEL_END) + 
            LO_PARAM_USE_RECS_N_COM_TP_SEL_EN * (LO_INDEX_IN_CTR_BUS_TP_SEL_START + LO_PARAM_TP_EQ_NUMS - 1);

        localparam  LO_INDEX_USE_RECS_EN_SIG_EN = LO_INDEX_IN_CTR_BUS_TP_SEL_END + LO_PARAM_USE_RE_CS_EN;
        localparam  LO_INDEX_USE_RECS_INIT_SIG_EN = LO_INDEX_USE_RECS_EN_SIG_EN + LO_PARAM_USE_RE_CS_EN;

    localparam  LO_INDEX_IN_CTR_END = LO_INDEX_USE_RECS_INIT_SIG_EN;

    localparam  LO_PARAM_IN_CTR_SIG_WIDTH = LO_INDEX_IN_CTR_END + 1;
    initial begin
        $display("/**************************************************/");
        $display("MODULE_NAME : %s, DESIGN_NAME : %s", MODULE_NAME, DESIGN_NAME);
        $display("infomation about in_ctr_sig");

        $display("data sel             : in_ctr_sig[%d : %d]", LO_INDEX_IN_CTR_BUS_DATA_SEL_END, LO_INDEX_IN_CTR_BUS_DATA_SEL_START);
            $display("Valid data sig is %s", LO_PARAM_USE_RECS_VALID_DATA_SIG_EN ? "enabled" : "disabled");
        $display("valid data           : in_ctr_sig[%d]", LO_INDEX_IN_CTR_VALID_DATA_SIG);
            $display("valid data buf enable signal is %s", LO_PARAM_USE_RECS_VALID_D_BUF_EN_SIG_EN ? "enabled" : "disabled");
        $display("validDataSelBufEnSig : in_ctr_sig[%d]", LO_INDEX_IN_CTR_TO_SEL_VALID_DATA_BUF_ENSIG);
            $display("Common sel signal is %s", LO_PARAM_USE_RECS_Y_COM_TP_SEL_EN ? "enabled" : "disabled");
        $display("tp sel               : in_ctr_sig[%d : %d]]", LO_INDEX_IN_CTR_BUS_TP_SEL_END, LO_INDEX_IN_CTR_BUS_TP_SEL_START);
            $display("reCS blk is %s", LO_PARAM_USE_RE_CS_EN ? "enabled" : "disabled");
        $display("reCS en              : in_ctr_sig[%d]", LO_INDEX_USE_RECS_EN_SIG_EN);
        $display("reCS init            : in_ctr_sig[%d]", LO_INDEX_USE_RECS_INIT_SIG_EN);
        $display("input [%d - 1 : %d] in_ctr_sig", LO_PARAM_IN_CTR_SIG_WIDTH, 0);

        $display("/**************************************************/");
    end
        

    /* re-chien configuration */
    localparam  PARAM_MODE2_COEF1_SHFT    =   
                    gf_sh_index
                        (
                            {{(GF_LEN - 1){1'b0}}, 1'b1},
                            GF_SH_FORWARD,
                            LO_PARAM_COEF_SHIFT_LEN,
                            1,
                            1,
                            0
                        );
    localparam  PARAM_MODE2_COEF2_SHFT    =   
                    gf_sh_index
                        (
                            {{(GF_LEN - 1){1'b0}}, 1'b1},
                            GF_SH_FORWARD,
                            LO_PARAM_COEF_SHIFT_LEN,
                            1,
                            2,
                            0
                        );
    localparam  PARAM_MODE2_COEF3_SHFT    =   
                    gf_sh_index
                        (
                            {{(GF_LEN - 1){1'b0}}, 1'b1},
                            GF_SH_FORWARD,
                            LO_PARAM_COEF_SHIFT_LEN,
                            1,
                            3,
                            0
                        );
    /************************************************************/
    /************************************************************/
    localparam  LO_PARAM_INPUT_DATA_BUS_WIDTH    =   
        calInputBusWidth(LO_PARAM_INPUT_BUS_TP_ERR_LOC_WIDTH,   
                        LO_PARAM_INPUT_BUS_COEFICIENT_WIDTH,
                        DECISION_MODE);
    /************************************************************/
    /************************************************************/

    parameter   DEBUG_MODE      =   0;
    /************************************************************/

    /************************************************************/
    /************************************************************/
    function    integer val2Bool;
        input   integer val;

        begin
            if(val) val2Bool = 1;
            else    val2Bool = 0;
        end
    endfunction
    /************************************************************/
    function    integer val2NotBool;
        input   integer val;

        begin
            if(val) val2NotBool = 0;
            else    val2NotBool = 1;
        end
    endfunction
    /************************************************************/
    /************************************************************/
            function integer pow2;
                input integer value;

                integer i;

                if(!value) begin
                    pow2 = 1;

                    $display("pow2) input value=%d, output value=%d",
                            value, pow2);
                end
                else begin
                    pow2 = 1;
                    for(i = 0; i < value; i = i + 1) begin
                        pow2 = (pow2 << 1);
                    end

                    $display("pow2) input value=%d, output value=%d",
                            value, pow2);
                end
            endfunction
    /*****************************************************/
    /*****************************************************/
            function    [GF_LEN - 1 : 0]    gf_sh_index;
                /*inputs start*/
                input   [GF_LEN - 1 : 0]    start;
                input   [GF_LEN - 1 : 0]    modulo_val;
                input   [32 - 1 : 0]    ref_index;
                input   integer         direction;
                input   integer         shift_exp;

                input   integer         prt_i;

                integer                 cal_index;
                integer                 cal_exp;
                /*inputs end*/
                if(!ref_index) begin
                    gf_sh_index = start;

                    $display("gf_sh_index in DU) print i:[%d], ref_index=%d, gf_sh_index=%d", prt_i, ref_index, gf_sh_index);
                end
                else begin
                    gf_sh_index = start;
                   
                    if(direction) begin
                        for(cal_index = 0; cal_index < ref_index; cal_index = cal_index + 1) begin
                            if(direction > 0) begin : shift_left
                                for(cal_exp=0; cal_exp<shift_exp; cal_exp=cal_exp+1) begin
                                    if(gf_sh_index[GF_LEN - 1] == 1'b1)  gf_sh_index = (gf_sh_index << 1) ^ modulo_val;
                                    else                                gf_sh_index = (gf_sh_index << 1);
                                end
                            end
                            else if(direction < 0) begin : shift_right
                                for(cal_exp=0; cal_exp<shift_exp; cal_exp=cal_exp+1) begin
                                    if(gf_sh_index[0] == 1'b1)  gf_sh_index = (gf_sh_index >> 1) ^ modulo_val;
                                    else                                gf_sh_index = (gf_sh_index >> 1);
                                end
                            end
                            else begin : shift_not
                                gf_sh_index = gf_sh_index;
                            end
                        end
                    end

                    $display("gf_sh_index in DU) print i:[%d], ref_index=%d, gf_sh_index=%d", prt_i, ref_index, gf_sh_index);
                end
            endfunction
    /************************************************************/
    /************************************************************/
            function integer cal_op_clk;
                input integer   op_clk;
                input integer   parallelism;

                if(op_clk) begin
                    cal_op_clk = op_clk / parallelism;

                    if(op_clk % parallelism) begin
                        cal_op_clk = cal_op_clk + 1;
                    end
                    
                    $display("in cal_op_clk, op_clk=%d, parallelism=%d, cal_op_clk=%d\n",
                                op_clk, parallelism, cal_op_clk);
                end
                else begin
                    cal_op_clk = op_clk;

                    $display("in cal_op_clk, op_clk=%d, parallelism=%d, cal_op_clk=%d\n",
                                op_clk, parallelism, cal_op_clk);
                end
            endfunction
    /************************************************************/
            function    integer chk_notBeingZero;
                input   integer val;

                begin
                    if(!val)    chk_notBeingZero = 1;
                    else        chk_notBeingZero = val;
                end
            endfunction
    /************************************************************/
    /************************************************************/
    function    integer useFIFOch_en;
        input   integer decision_mode;

        begin
            if(decision_mode == 0) begin
                $display("in %s, decision_mode %d is not support.", DESIGN_NAME, decision_mode);
            end

            if(decision_mode == 1)      useFIFOch_en = 1;
            else                        useFIFOch_en = 0;
        end
    endfunction
    /************************************************************/
    function    integer useCSinit_bus_en;
        input   integer decision_mode;

        begin
            if(decision_mode == 0) begin
                    $display("ERROR!!!) useCSinit_bus_en, decicion mode = %d is not support!!!", decision_mode);
                    useCSinit_bus_en = 0;
            end
            else if(decision_mode == 1) useCSinit_bus_en = 0;
            else if(decision_mode == 2) useCSinit_bus_en = 0;
            else if(decision_mode == 3) useCSinit_bus_en = 1;
            else begin
                    $display("ERROR!!!) useCSinit_bus_en, decicion mode = %d is not support!!!", decision_mode);
                    useCSinit_bus_en = 0;
            end
        end
    endfunction
    /************************************************************/
    function    integer useCSblk_en;
        input   integer decision_mode;

        begin
            if(decision_mode == 0) begin
                $display("in %s, decision_mode %d is not support.", DESIGN_NAME, decision_mode);
            end
            else if(decision_mode == 1) useCSblk_en = 0;
            else if(decision_mode == 2) useCSblk_en = 1;
            else if(decision_mode == 3) useCSblk_en = 1;
            else    $display("in %s, decision_mode %d is not support.", DESIGN_NAME, decision_mode);
        end
    endfunction
    /************************************************************/
    function    integer indipendDataAndTpSelSig;
        input   integer decision_mode;

        begin
            if(decision_mode == 0) begin
                $display("in %s, decision_mode %d is not support.", DESIGN_NAME, decision_mode);
            end
            else if(decision_mode == 1) indipendDataAndTpSelSig = 0;
            else if(decision_mode == 2) indipendDataAndTpSelSig = 0;
            else if(decision_mode == 3) indipendDataAndTpSelSig = 1;
            else    $display("in %s, decision_mode %d is not support.", DESIGN_NAME, decision_mode);
        end
    endfunction
    /************************************************************/
    function    integer selBusEn;
        input   integer decision_mode;

        begin
            if(decision_mode == 0)
                $display("ERROR!!!) selBusEn, ecision_mode = %d, is not support", decision_mode);
            else if(decision_mode == 1) selBusEn = 1;
            else if(decision_mode == 2) selBusEn = 1;
            else if(decision_mode == 3) selBusEn = 0;
            else 
                $display("ERROR!!!) selBusEn, decision_mode = %d, is not support", decision_mode);
        end
    endfunction
    /************************************************************/
    function    integer calInputBusWidth;
        input   integer testPattern_errLoc;
        input   integer testPattern_coef;
        input   integer decision_mode;

        begin
            if(decision_mode == 1)      
                    calInputBusWidth = testPattern_errLoc;
            else if((decision_mode == 2) || (decision_mode == 3)) begin
                    calInputBusWidth = testPattern_coef;
            end
            else $display("ERROR!!!) decision mode = %d is not support!!!", decision_mode);
        end 
    endfunction
    /************************************************************/
    /**********************************************************/
    function integer indexCal;
        input integer interVal;
        input integer initVal;

        begin
            indexCal = initVal + interVal;
        end
    endfunction
    /**********************************************************/
    /****************************************************/

    genvar i, j;

    input   clk;

    input   in_ctr_Srst;
    input   in_ctr_en;

    input   [LO_PARAM_IN_CTR_SIG_WIDTH - 1 : 0]                 in_ctr_sig;

    input   [LO_PARAM_INPUT_DATA_BUS_WIDTH - 1 : 0]             in_tp_data; 

    input   [SD_CORRECTABLE * LO_PARAM_PARALLELISM - 1: 0]      in_alpha_loc;

    output  [LO_PARAM_PARALLELISM - 1 : 0]                      out_sel_tp_errLoc;

    /************************************************************/
    wire    [LO_PARAM_TP_EQ_NUMS - 1 : 0]                       w_tp_selDatalEn;
    wire    [LO_PARAM_TP_EQ_NUMS - 1 : 0]                       w_tp_selTSCSEn;

    wire    [LO_PARAM_TP_EQ_NUMS * LO_PARAM_PARALLELISM - 1: 0] w_SD_equal;


    wire    [LO_PARAM_PARALLELISM - 1 : 0]                      w_seled_tp_SD_errLoc;
    wire    [LO_PARAM_PARALLELISM - 1 : 0]                      w_seled_tp_HD_errLoc;

    wire    [LO_PARAM_PARALLELISM - 1 : 0]                      w_fin_tp_HD_SD_errLoc;
    /************************************************************/
    /************************************************************/
    generate
        /* Generate enable signal to select Correctable Codeword start */
        /* This function is moved to decision_indicator, 
        *  which is included in decision_controller. */
        /* Generate enable signal to select Correctable Codeword end */

        /* Test Syndrome Chien Search start */
        /*
        * The index of w_SD_equal bus is followed below,
            * index of 0-th parallel is start from 0, end number of test pattern * 1 - 1,
            * and 1-st parallel is is start from test pattern and end number
            * of test pattern * 2 - 1.
        */
        if(LO_PARAM_TP_EQ_NUMS == 8) begin : tpNumIs8_SD_dec
            for(i = 0; i < LO_PARAM_PARALLELISM; i = i + 1) begin : gen_YP_tpNumIs8_SD_dec
                assign  w_SD_equal[(i * LO_PARAM_TP_EQ_NUMS) + 0] = 
                                                                        1'b0;

                assign  w_SD_equal[(i * LO_PARAM_TP_EQ_NUMS) + 1] = 
                                    in_alpha_loc[1 + i * SD_CORRECTABLE];

                assign  w_SD_equal[(i * LO_PARAM_TP_EQ_NUMS) + 2] = 
                                    in_alpha_loc[0 + i * SD_CORRECTABLE] ^ 
                                    in_alpha_loc[1 + i * SD_CORRECTABLE];

                assign  w_SD_equal[(i * LO_PARAM_TP_EQ_NUMS) + 3] = 
                                    in_alpha_loc[0 + i * SD_CORRECTABLE] ^ 
                                    in_alpha_loc[1 + i * SD_CORRECTABLE] ^ 
                                    in_alpha_loc[2 + i * SD_CORRECTABLE];

                assign  w_SD_equal[(i * LO_PARAM_TP_EQ_NUMS) + 4] = 
                                    in_alpha_loc[1 + i * SD_CORRECTABLE] ^ 
                                    in_alpha_loc[2 + i * SD_CORRECTABLE];

                assign  w_SD_equal[(i * LO_PARAM_TP_EQ_NUMS) + 5] = 
                                    in_alpha_loc[2 + i * SD_CORRECTABLE];

                assign  w_SD_equal[(i * LO_PARAM_TP_EQ_NUMS) + 6] = 
                                    in_alpha_loc[0 + i * SD_CORRECTABLE] ^ 
                                    in_alpha_loc[2 + i * SD_CORRECTABLE];

                assign  w_SD_equal[(i * LO_PARAM_TP_EQ_NUMS) + 7] = 
                                    in_alpha_loc[0 + i * SD_CORRECTABLE];
                initial begin
                    $display("DU) p=%d, assign sd_equal", i);
                    $display("p[%d], w_SD_equal[%d]=none", 
                                    i, 0 + i * LO_PARAM_TP_EQ_NUMS);
                    $display("p[%d], w_SD_equal[%d]=in_alpha_loc [%d]", 
                                    i, 1 + i * LO_PARAM_TP_EQ_NUMS,
                                    1 + i * SD_CORRECTABLE
                                );
                    $display("p[%d], w_SD_equal[%d]=in_alpha_loc [%d], [%d]", 
                                    i, 2 + i * LO_PARAM_TP_EQ_NUMS,
                                    0 + i * SD_CORRECTABLE,
                                    1 + i * SD_CORRECTABLE
                                );
                    $display("p[%d], w_SD_equal[%d]=in_alpha_loc [%d], [%d], [%d]", 
                                    i, 3 + i * LO_PARAM_TP_EQ_NUMS,
                                    0 + i * SD_CORRECTABLE,
                                    1 + i * SD_CORRECTABLE,
                                    2 + i * SD_CORRECTABLE
                                );
                    $display("p[%d], w_SD_equal[%d]=in_alpha_loc [%d], [%d]", 
                                    i, 4 + i * LO_PARAM_TP_EQ_NUMS,
                                    1 + i * SD_CORRECTABLE,
                                    2 + i * SD_CORRECTABLE
                                );
                    $display("p[%d], w_SD_equal[%d]=in_alpha_loc [%d]", 
                                    i, 5 + i * LO_PARAM_TP_EQ_NUMS,
                                    2 + i * SD_CORRECTABLE
                                );
                    $display("p[%d], w_SD_equal[%d]=in_alpha_loc [%d], [%d]", 
                                    i, 6 + i * LO_PARAM_TP_EQ_NUMS,
                                    0 + i * SD_CORRECTABLE,
                                    2 + i * SD_CORRECTABLE
                                );
                    $display("p[%d], w_SD_equal[%d]=in_alpha_loc [%d]", 
                                    i, 7 + i * LO_PARAM_TP_EQ_NUMS,
                                    0 + i * SD_CORRECTABLE
                                );
                end
            end

        end
        else begin : notSupYetTPnumIs8_SD_dec
            for(j = 0; j < LO_PARAM_PARALLELISM; j = j + 1) begin : gen_YP_assi_w_tp_eqWire
                for(i = 0; i < LO_PARAM_TP_EQ_NUMS; i = i + 1) begin : assi_w_tp_eqWire
                    assign  w_SD_equal[i + j * LO_PARAM_TP_EQ_NUMS] = 1'b0;
                end
            end
            initial $display("\n!!! WARNING !!! in decision_unit_3, label notSupYetTPnumIs8_SD_dec is not supported!!!!\n");
        end
        /* Test Syndrome Chien Search end*/


        /* sel signal is already decided by parameters start */
        assign w_tp_selDatalEn = in_ctr_sig[LO_INDEX_IN_CTR_BUS_DATA_SEL_END : LO_INDEX_IN_CTR_BUS_DATA_SEL_START];
        assign w_tp_selTSCSEn = in_ctr_sig[LO_INDEX_IN_CTR_BUS_TP_SEL_END : LO_INDEX_IN_CTR_BUS_TP_SEL_START];
        /* sel signal is already decided by parameters end */


        /* Chien Search process Regarding Decision mode start */
        if(LO_PARAM_USE_FIFO_CHANNEL_EN) begin : gen_fifoCh
            wire    [LO_PARAM_TP_EQ_NUMS * LO_PARAM_PARALLELISM - 1 : 0]         
                        w_tp_HD_errLoc ;//mode 1 used

            wire    [LO_PARAM_TP_EQ_NUMS * LO_PARAM_PARALLELISM - 1 : 0] w_FIFO_out_tpHDerrLoc;

            assign  w_tp_HD_errLoc = in_tp_data;
            
            fifo_multi_ch
            #(
             
                .FIFO_LEN(LO_PARAM_FIFO_CH_LEN),
                .FIFO_CH_WIDTH(LO_PARAM_TP_EQ_NUMS * LO_PARAM_PARALLELISM)
            )
            uut_fifo_tp_equal
            (
                .clk(clk),

                .in_ctr_Srst(in_ctr_Srst),
                .in_ctr_en(in_ctr_en),

                .in(w_tp_HD_errLoc),
                .out(w_FIFO_out_tpHDerrLoc)
            );


            initial $display("DU) mode1 canditate and LRB sum , %d-parallel", LO_PARAM_PARALLELISM);

            for(j = 0; j < LO_PARAM_PARALLELISM; j = j + 1) begin : gen_YP_DU_mode1
                /* Decision Mode 1 out of FIFO ch selection start */
                prior_w_di_sel_path
                #(
                    .BIT_WIDTH(1),
                    .SEL_SIG_NUMS(LO_PARAM_TP_EQ_NUMS),
                    .PRIORITY_DIRECTION(-1),
                    .DEFAULT_INPUT_EN(1)
                )
                uut_decM1_priorSelHDerrLoc
                (
                    .in({1'b0, w_FIFO_out_tpHDerrLoc[j * LO_PARAM_TP_EQ_NUMS]}), 
                    .out(w_seled_tp_HD_errLoc[j]), 
                    .sel(w_tp_selDatalEn)
                );
                /* Decision Mode 1 out of FIFO ch selection end */

                prior_w_di_sel_path
                #(
                    .BIT_WIDTH(1),
                    .SEL_SIG_NUMS(LO_PARAM_TP_EQ_NUMS),
                    .PRIORITY_DIRECTION(-1),
                    .DEFAULT_INPUT_EN(1)
                )
                uut_decM1_priorSelSDerrLoc
                (
                    .in({1'b0, w_SD_equal[(j + 1) * LO_PARAM_TP_EQ_NUMS - 1 : j * LO_PARAM_TP_EQ_NUMS]}), 
                    .out(w_seled_tp_SD_errLoc[j]), 
                    .sel(w_tp_selDatalEn)
                );

            end
        end
       

        if(LO_PARAM_USE_RE_CS_EN) begin : gen_reCSblk
            wire    [GF_LEN * LO_PARAM_TP_EQ_NUMS * PARAM_COEF_NUMS - 1 : 0]
                        w_tp_coef ; //mode2, 3 used

            assign  w_tp_coef = in_tp_data;

            wire    w_in_ctr_reCSen ;
            assign  w_in_ctr_reCSen = in_ctr_sig[LO_INDEX_USE_RECS_EN_SIG_EN];

            wire    w_in_ctr_reCSinit ;
            assign  w_in_ctr_reCSinit = in_ctr_sig[LO_INDEX_USE_RECS_INIT_SIG_EN];

            /* Coefficient selection based on w_tp_selDatalEn signal start */
            /* used wire list start */
            wire    [GF_LEN * PARAM_COEF_NUMS - 1 : 0]      w_useReCS_seledTPcoef_out;

            wire    [GF_LEN * PARAM_COEF_NUMS - 1 : 0]      w_useReCS_seledTPcoef_toShter;

            wire    [GF_LEN * PARAM_COEF_NUMS - 1 : 0]      w_useReCS_shftTPcoef;
            wire    [GF_LEN * PARAM_COEF_NUMS - 1 : 0]      w_useReCS_coefInsertReCS;

            prior_w_di_sel_path
            #(
                .BIT_WIDTH(GF_LEN * PARAM_COEF_NUMS),
                .SEL_SIG_NUMS(LO_PARAM_TP_EQ_NUMS),

                .PRIORITY_DIRECTION(-1),

                .DEFAULT_INPUT_EN(1)
            )
            uut_useReCScoef_sel_wire
            (
                .in({{{(GF_LEN * PARAM_COEF_NUMS - 1){1'b0}}, 1'b1}, w_tp_coef}), 
                .out(w_useReCS_seledTPcoef_out),
                .sel(w_tp_selDatalEn)
            );

            /*for timming controll, prior selected done coefficient. */
            if(LO_PA_FOR_RE_CS_P_SELED_BUF_EN) begin : timCTR_ps_coef
                if(LO_PARAM_USE_RECS_VALID_DATA_SIG_EN) begin : enValidSig

                    wire    w_psBuf_init;
                    assign  w_psBuf_init = in_ctr_sig[LO_INDEX_IN_CTR_BUS_DATA_SEL_START];

                    wire    w_psBuf_done;
                    assign  w_psBuf_done = in_ctr_sig[LO_INDEX_USE_RECS_INIT_SIG_EN];

                    wire    w_psBuf_valid; // LO_INDEX_IN_CTR_VALID_DATA_SIG
                    assign  w_psBuf_valid = in_ctr_sig[LO_INDEX_IN_CTR_VALID_DATA_SIG];

                    wire    w_psBuf_en; // LO_INDEX_IN_CTR_TO_SEL_VALID_DATA_BUF_ENSIG
                    assign  w_psBuf_en = in_ctr_sig[LO_INDEX_IN_CTR_TO_SEL_VALID_DATA_BUF_ENSIG];


                    
                    prior_buffer_1
                    #(
                        .OUTTER_NAME(DESIGN_NAME),
                        .MODULE_NAME("uut_priorBuf"),
                        
                        .WIDTH(GF_LEN * PARAM_COEF_NUMS), 

                        .RST_VAL(1)
                    )
                    uut_priorBuf
                    (
                        .clk(clk),
                        .in_ctr_Srst(in_ctr_Srst),
                        .in_ctr_en(in_ctr_en),

                        .in_ctr_init(w_psBuf_init),
                        .in_ctr_done(w_psBuf_done),

                        .in_ctr_buf_en(w_psBuf_en),
                        .in_ctr_valid(w_psBuf_valid),

                        .in(w_useReCS_seledTPcoef_out),
                        .out(w_useReCS_seledTPcoef_toShter)
                    );
                end
                else begin : enCoefBuf
                    fifo_multi_ch
                    #(
                        .OUTTER_NAME(DESIGN_NAME),
                        .MODULE_NAME("uut_coefBuf"),

                        .FIFO_LEN(1),
                        .FIFO_CH_WIDTH(GF_LEN * PARAM_COEF_NUMS)
                    )
                    uut_coefBuf
                    (
                        .clk(clk),

                        .in_ctr_Srst(in_ctr_Srst),
                        .in_ctr_en(in_ctr_en),

                        .in(w_useReCS_seledTPcoef_out),
                        .out(w_useReCS_seledTPcoef_toShter)
                    );
                end


                
            end
            else begin : wiring_ps_coef2shfter
                assign w_useReCS_seledTPcoef_toShter = w_useReCS_seledTPcoef_out;
            end

            /* used wire list end */
            assign  w_useReCS_shftTPcoef[GF_LEN * (0 + 1) - 1 : GF_LEN * 0] = 
                        w_useReCS_seledTPcoef_toShter[GF_LEN * (0 + 1) - 1 : GF_LEN * 0];
            GF_Mult_2_10_comb uut_coef1_shft
                (
                    .out(w_useReCS_shftTPcoef[GF_LEN * (1 + 1) - 1 : GF_LEN * 1]),
                    .a(w_useReCS_seledTPcoef_toShter[GF_LEN * (1 + 1) - 1 : GF_LEN * 1]),
                    .b(PARAM_MODE2_COEF1_SHFT)
                );
            GF_Mult_2_10_comb uut_coef2_shft
                (
                    .out(w_useReCS_shftTPcoef[GF_LEN * (2 + 1) - 1 : GF_LEN * 2]),
                    .a(w_useReCS_seledTPcoef_toShter[GF_LEN * (2 + 1) - 1 : GF_LEN * 2]),
                    .b(PARAM_MODE2_COEF2_SHFT)
                );
            GF_Mult_2_10_comb uut_coef3_shft
                (
                    .out(w_useReCS_shftTPcoef[GF_LEN * (3 + 1) - 1 : GF_LEN * 3]),
                    .a(w_useReCS_seledTPcoef_toShter[GF_LEN * (3 + 1) - 1 : GF_LEN * 3]),
                    .b(PARAM_MODE2_COEF3_SHFT)
                );


            assign w_useReCS_coefInsertReCS = w_useReCS_shftTPcoef;

            chien_search_series
            #(
                .OUTTER_NAME(MODULE_NAME),
                .MODULE_NAME("uut_re_chienSearch"),

                .GF_LEN(GF_LEN),

                .PARALLELISM(LO_PARAM_PARALLELISM),

                .EQUAL_BUF_EN(0),

                .DONE_COEF_BUF_EN(0)

            )
            uut_re_chienSearch
            (
                .in_ctr_Srst(in_ctr_Srst),
                .in_ctr_en(in_ctr_en),
                .in_ctr_init(w_in_ctr_reCSinit),
                .in_ctr_done(1'b0),

                .in_ctr_CS_en(w_in_ctr_reCSen),

                .clk(clk),

                .in_init_coef(w_useReCS_coefInsertReCS),
               
                .out_w_equal(w_seled_tp_HD_errLoc), 
                .out_equal(),       //not used

                .out_done_coef()      //not used
            );


            initial $display("DU) mode 2 canditate and LRB sum , %d-parallel", LO_PARAM_PARALLELISM);

            for(j = 0; j < LO_PARAM_PARALLELISM; j = j + 1) begin : gen_YP_DU_mode2 
                prior_w_di_sel_path
                #(
                    .BIT_WIDTH(1),
                    .SEL_SIG_NUMS(LO_PARAM_TP_EQ_NUMS),
                    .PRIORITY_DIRECTION(-1),
                    .DEFAULT_INPUT_EN(1)
                )
                uut_decM2_priorSelSDerrLoc
                (
                    .in({1'b0, w_SD_equal[(j + 1) * LO_PARAM_TP_EQ_NUMS - 1 : j * LO_PARAM_TP_EQ_NUMS]}), 
                    .out(w_seled_tp_SD_errLoc[j]), 
                    .sel(w_tp_selTSCSEn)
                );

            end
        end
        /* Chien Search process Regarding Decision mode end */

        for(j = 0; j < LO_PARAM_PARALLELISM; j = j + 1) begin : gen_HD_SD_XOR
            assign  w_fin_tp_HD_SD_errLoc[j] = w_seled_tp_HD_errLoc[j] ^ w_seled_tp_SD_errLoc[j];
        end
    endgenerate


    assign  out_sel_tp_errLoc = w_fin_tp_HD_SD_errLoc;



    /************************************************************/
    initial begin
        $display("/**************************************************/");
        $display("Instanciating %s", DESIGN_NAME);

        $display("In %s," ,OUTTER_NAME);
        $display("MODULE_NAME : %s", MODULE_NAME);
        $display("DESIGN_NAME : %s", DESIGN_NAME);


        $display("GF_LEN = %d", GF_LEN);
        $display("PARAM_MAX_GF_LEN = %d", PARAM_MAX_GF_LEN);

        $display("GF_SH_FORWARD = %d", GF_SH_FORWARD);
        $display("GF_SH_BACKWARD = %d", GF_SH_BACKWARD);

        $display("PARALLELISM = %d", PARALLELISM);
        $display("LO_PARAM_PARALLELISM = %d", LO_PARAM_PARALLELISM);
        $display("LO_PARAM_TP_ERR_LOC_WIDTH = %d", LO_PARAM_TP_ERR_LOC_WIDTH);

        $display("CODEWORD_LEN = %d", CODEWORD_LEN);
        $display("LO_PARAM_CODEWORD_LEN = %d", LO_PARAM_CODEWORD_LEN);

        $display("LO_PARAM_SHIFT_LEN = %d", LO_PARAM_SHIFT_LEN);
        $display("LO_PARAM_ACTUAL_SHIFT_LEN = %d", LO_PARAM_ACTUAL_SHIFT_LEN);

        $display("LO_PARAM_COEF_SHIFT_LEN = %d", LO_PARAM_COEF_SHIFT_LEN);
        $display("LO_PARAM_REDUNDANT = %d", LO_PARAM_REDUNDANT);

        $display("HD_CORRECTABLE = %d", HD_CORRECTABLE);
        $display("PARAM_COEF_NUMS = %d", PARAM_COEF_NUMS);

        $display("SD_CORRECTABLE = %d", SD_CORRECTABLE);
        $display("LO_PARAM_TP_EQ_NUMS = %d", LO_PARAM_TP_EQ_NUMS);

        $display("ERR_CNT_LEN = %d", ERR_CNT_LEN);

        $display("DECISION_MODE = %d", DECISION_MODE);

        $display("LO_PARAM_USE_FIFO_CHANNEL_EN : %s", 
            LO_PARAM_USE_FIFO_CHANNEL_EN ? "enabled" : "disabled");
        $display("LO_PARAM_FIFO_CH_LEN = %d", LO_PARAM_FIFO_CH_LEN);
        $display("LO_PARAM_INPUT_BUS_TP_ERR_LOC_WIDTH = %d", LO_PARAM_INPUT_BUS_TP_ERR_LOC_WIDTH);


        $display("LO_PARAM_USE_RE_CS_EN : %s", 
            LO_PARAM_USE_RE_CS_EN ? "enabled" : "disabled");
        $display("LO_PARAM_INPUT_BUS_COEFICIENT_WIDTH = %d", LO_PARAM_INPUT_BUS_COEFICIENT_WIDTH);

        $display("FOR_RE_CS_PRIOR_SELED_BUF_EN = %d", FOR_RE_CS_PRIOR_SELED_BUF_EN);
        $display("LO_PA_FOR_RE_CS_P_SELED_BUF_EN : %s", 
            LO_PA_FOR_RE_CS_P_SELED_BUF_EN ? "enabled" : "disabled");

        
        $display("DEBUG_MODE = %d", DEBUG_MODE);
        $display("/**************************************************/");
    end
    /************************************************************/

endmodule
`endif
