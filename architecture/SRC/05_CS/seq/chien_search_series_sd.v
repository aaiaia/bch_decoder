`ifndef CHIEN_SEARCH_SERIES_SOFT_DECISION
`define CHIEN_SEARCH_SERIES_SOFT_DECISION

`include "05_CS/seq/chien_search_series_plusErrCnt.v"
`include "60_fifo_channel/fifo_multi_ch.v"
`include "80_net_model/prior_sel/prior_w_di_sel_path.v"
/*
initializing condition
port in_ctr_bus_CS_init
value   :00010000...00100...0000100000
port in_ctr_en
value   :111X1111...11X11...1111X11111...
timing  :----|---...---|-...-----|----...
*/

//f(x) = co0 + co1*x + co2*x^2 + co3*x^3 = 0
//if f(x) == 0, out_equal signal is one.
module chien_search_series_sd
(
    in_ctr_Srst,
    in_ctr_en,
    in_ctr_bus_CS_init,
    in_ctr_bus_CS_done,

    in_ctr_bus_CS_en,

    clk,

    in_init_coef,
    
    out_equal,
    out_erCnt,

    out_bus_done_coef
);
    /*****************************************************/
    /********************* Functions *********************/
    /*****************************************************/
    function    integer valToBool;
        input   integer val;

        begin
            if(val) valToBool = 1;
            else    valToBool = 0;
        end
    endfunction
    /*****************************************************/
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
            function integer errorCntBitLenCal;
                input   integer errorNums;

                integer inputValues;
                if(errorNums) begin
                    inputValues = errorNums;    

                    for(errorCntBitLenCal = 0; errorNums != 0; errorCntBitLenCal = errorCntBitLenCal + 1) begin
                        errorNums = (errorNums >> 1);
                    end

                    $display("func) errorCntBitLenCal : input=%d, errorCntBitLenCal=%d",
                            inputValues, errorCntBitLenCal);
                end
                else begin
                    errorCntBitLenCal = 0;

                    $display("func) errorCntBitLenCal : input=%d, errorCntBitLenCal=%d",
                            inputValues, errorCntBitLenCal);
                end
            endfunction
    /*****************************************************/
    /*****************************************************/
    /*****************************************************/
    /***********************************************************************/
    /***********************************************************************/
    function    integer func_in_cs_init_bus_en;
        input   integer decision_mode;

        begin
            if(decision_mode == 0)  func_in_cs_init_bus_en = 0;
            else if(decision_mode == 1) func_in_cs_init_bus_en = 0;
            else if(decision_mode == 2) func_in_cs_init_bus_en = 0;
            else if(decision_mode == 3) func_in_cs_init_bus_en = 1;
            else                        func_in_cs_init_bus_en = 0;
        end
    endfunction
    function    integer func_in_coef_bus_en;
        input   integer decision_mode;

        begin
            if(decision_mode == 0)  func_in_coef_bus_en = 1;
            else if(decision_mode == 1)  func_in_coef_bus_en = 1;
            else if(decision_mode == 2)  func_in_coef_bus_en = 1;
            else if(decision_mode == 3)  func_in_coef_bus_en = 0;
            else begin
                $display("in desicion_loader) decision_mode has wrong value(%d)", decision_mode);
                func_in_coef_bus_en = 0;
            end
        end
    endfunction
    /***********************************************************************/
    function    integer func_out_coef_bus_en;
        input   integer decision_mode;

        begin
            if(decision_mode == 0)  func_out_coef_bus_en = 1;
            else if(decision_mode == 1)  func_out_coef_bus_en = 1;
            else if(decision_mode == 2)  func_out_coef_bus_en = 1;
            else if(decision_mode == 3)  func_out_coef_bus_en = 1;
            else begin
                $display("in desicion_loader) decision_mode has wrong value(%d)", decision_mode);
                func_out_coef_bus_en = 0;
            end
        end
    endfunction
    /***********************************************************************/
    /***********************************************************************/
    parameter   OUTTER_NAME =   "";
    parameter   MODULE_NAME =   "chien_search_series_sd";
        localparam  DESIGN_NAME =   "chien_search_series_sd";

    parameter   GF_LEN  = 10;

    parameter   PARALLELISM = 4;

    parameter   HD_CORRECTABLE = 3;
        localparam  PARAM_EQATION_COEFFI_NUM = HD_CORRECTABLE + 1;

    parameter   SD_CORRECTABLE = 3;
        localparam  PARAM_SD_EQUATION_NUMS = pow2(SD_CORRECTABLE);


    parameter   EQUAL_BUF_EN = 1;

    parameter   DONE_COEF_BUF_EN    =   0;

        localparam  PARAM_ROOT_CNT_LEN    = errorCntBitLenCal(HD_CORRECTABLE);
    parameter   ERR_CNT_LIMIT   = 2'd3;

    parameter   DECISION_MODE   =   1;
        localparam  LO_PARAM_BUS_CTR_CS_SIG_EN    =   func_in_cs_init_bus_en(DECISION_MODE);
        localparam  LO_PARAM_WIDTH_BUS_CTR_CS_SIG  =   
            ((PARAM_SD_EQUATION_NUMS - 1) * LO_PARAM_BUS_CTR_CS_SIG_EN + 1) * 1;

        localparam  LO_PARAM_BUS_IN_COEF_EN     =   func_in_coef_bus_en(DECISION_MODE);
        localparam  LO_PARAM_WIDTH_BUS_IN_COEF  =   
            ((PARAM_SD_EQUATION_NUMS - 1) * LO_PARAM_BUS_IN_COEF_EN + 1) * GF_LEN * PARAM_EQATION_COEFFI_NUM;
        
        localparam  LO_PARAM_BUS_OUT_COEF_EN    =   func_out_coef_bus_en(DECISION_MODE);
        localparam  LO_PARAM_WIDTH_BUS_OUT_COEF =   
            ((PARAM_SD_EQUATION_NUMS - 1) * LO_PARAM_BUS_OUT_COEF_EN + 1) * GF_LEN * PARAM_EQATION_COEFFI_NUM;

    parameter   DEBUG_MODE      =   0;
    parameter   DEBUG_CNT_LIMIT =   1023;

    /*****************************************************/

    genvar i;

    input   clk;

    input   in_ctr_Srst;
    input   in_ctr_en;

    input   [LO_PARAM_WIDTH_BUS_CTR_CS_SIG - 1 : 0] in_ctr_bus_CS_init;
    input   [LO_PARAM_WIDTH_BUS_CTR_CS_SIG - 1 : 0] in_ctr_bus_CS_done;

    input   [LO_PARAM_WIDTH_BUS_CTR_CS_SIG - 1 : 0] in_ctr_bus_CS_en;

    /*
     * in_init_coef, out_bus_done_coef
     * ASSUME
     * GF_LEN = 10,
     * PARAM_SD_EQUATION_NUMS = 8,
     * PARAM_EQATION_COEFFI_NUM = 4,
     * i = testpattern index
     * j = coef pow index
     * test pattern 0 coef
     * i = 0;
     * j = 0 ~ 3
     * in_init_coef[GF_LEN * (j(0) + 1 + PARAM_EQATION_COEFFI_NUM * i(0)) - 1 = 009 : GF_LEN * (j(0) + 0 + PARAM_EQATION_COEFFI_NUM * i(0)) = 000]; => pow0 coef
     * in_init_coef[GF_LEN * (j(1) + 1 + PARAM_EQATION_COEFFI_NUM * i(0)) - 1 = 019 : GF_LEN * (j(1) + 0 + PARAM_EQATION_COEFFI_NUM * i(0)) = 010]; => pow1 coef
     * in_init_coef[GF_LEN * (j(2) + 1 + PARAM_EQATION_COEFFI_NUM * i(0)) - 1 = 029 : GF_LEN * (j(2) + 0 + PARAM_EQATION_COEFFI_NUM * i(0)) = 020]; => pow2 coef
     * in_init_coef[GF_LEN * (j(3) + 1 + PARAM_EQATION_COEFFI_NUM * i(0)) - 1 = 039 : GF_LEN * (j(3) + 0 + PARAM_EQATION_COEFFI_NUM * i(0)) = 030]; => pow3 coef
     *
     * test pattern 1 coef
     * i = 1;
     * j = 0 ~ 3
     * in_init_coef[GF_LEN * (j(0) + 1 + PARAM_EQATION_COEFFI_NUM * i(1)) - 1 = 009 : GF_LEN * (j(0) + 0 + PARAM_EQATION_COEFFI_NUM * i(1)) = 000]; => pow0 coef
     * in_init_coef[GF_LEN * (j(1) + 1 + PARAM_EQATION_COEFFI_NUM * i(1)) - 1 = 019 : GF_LEN * (j(1) + 0 + PARAM_EQATION_COEFFI_NUM * i(1)) = 010]; => pow1 coef
     * in_init_coef[GF_LEN * (j(2) + 1 + PARAM_EQATION_COEFFI_NUM * i(1)) - 1 = 029 : GF_LEN * (j(2) + 0 + PARAM_EQATION_COEFFI_NUM * i(1)) = 020]; => pow2 coef
     * in_init_coef[GF_LEN * (j(3) + 1 + PARAM_EQATION_COEFFI_NUM * i(1)) - 1 = 039 : GF_LEN * (j(3) + 0 + PARAM_EQATION_COEFFI_NUM * i(1)) = 030]; => pow3 coef
     *
     *
     * .
     * .
     * .
     *
     * OR
     * test pattern index i = 0 ~ 3
     * test pattern 0
     * in_init_coef[GF_LEN * PARAM_EQATION_COEFFI_NUM * (i(0) + 1) - 1 : GF_LEN * PARAM_EQATION_COEFFI_NUM * (i(0) + 0)];
     *                  10 * 4 * 1 - 1 : 10 * 4 * 0 =   39 : 0
     * in_init_coef[GF_LEN * PARAM_EQATION_COEFFI_NUM * (i(1) + 1) - 1 : GF_LEN * PARAM_EQATION_COEFFI_NUM * (i(1) + 0)];
     *                  10 * 4 * 2 - 1 : 10 * 4 * 1 =   79 : 40
     * .
     * .
     * .
    */
//    input   [(GF_LEN * PARAM_SD_EQUATION_NUMS) - 1 : 0]   in_init_coef0;
//    input   [(GF_LEN * PARAM_SD_EQUATION_NUMS) - 1 : 0]   in_init_coef1;
//    input   [(GF_LEN * PARAM_SD_EQUATION_NUMS) - 1 : 0]   in_init_coef2;
//    input   [(GF_LEN * PARAM_SD_EQUATION_NUMS) - 1 : 0]   in_init_coef3;
//    input   [(GF_LEN * PARAM_SD_EQUATION_NUMS * PARAM_EQATION_COEFFI_NUM) - 1 : 0]   in_init_coef;
    input   [LO_PARAM_WIDTH_BUS_IN_COEF - 1 : 0]   in_init_coef;

    output  [PARAM_SD_EQUATION_NUMS * PARALLELISM - 1 : 0]   out_equal;
    output  [PARAM_SD_EQUATION_NUMS * PARAM_ROOT_CNT_LEN - 1 : 0]    out_erCnt;

//    output  [(GF_LEN * PARAM_SD_EQUATION_NUMS) - 1 : 0]   out_done_coef0;
//    output  [(GF_LEN * PARAM_SD_EQUATION_NUMS) - 1 : 0]   out_done_coef1;
//    output  [(GF_LEN * PARAM_SD_EQUATION_NUMS) - 1 : 0]   out_done_coef2;
//    output  [(GF_LEN * PARAM_SD_EQUATION_NUMS) - 1 : 0]   out_done_coef3;
//    output  [(GF_LEN * PARAM_SD_EQUATION_NUMS * PARAM_EQATION_COEFFI_NUM) - 1 : 0]   out_bus_done_coef;
    output  [LO_PARAM_WIDTH_BUS_OUT_COEF - 1 : 0]   out_bus_done_coef;


    /*****************************************************/
    /*****************************************************/
    generate

            for(i = 0; i < PARAM_SD_EQUATION_NUMS; i = i + 1) begin : solverGen
                chien_search_series_plusErrCnt
                #(
                    .OUTTER_NAME(MODULE_NAME),
                    .MODULE_NAME("uut_CS_series_Acnt"),

                    .PARALLELISM(PARALLELISM),

                    .EQUATION_POW(HD_CORRECTABLE),

                    .GF_LEN(GF_LEN),

                    .ROOT_CNT_LIMIT(ERR_CNT_LIMIT),

                    .EQUAL_BUF_EN(EQUAL_BUF_EN),

                    .DONE_COEF_BUF_EN(DONE_COEF_BUF_EN),

                    .DECISION_MODE(DECISION_MODE) 
                )
                uut_CS_series_Acnt
                (
                    .in_ctr_Srst(in_ctr_Srst),
                    .in_ctr_en(in_ctr_en),

                    .in_ctr_init(in_ctr_bus_CS_init[i]),
                    .in_ctr_done(in_ctr_bus_CS_done[i]),

                    .in_ctr_CS_en(in_ctr_bus_CS_en[i]),

                    .clk(clk),

//                    .in_init_coef0(in_init_coef0[GF_LEN * (i + 1) - 1 : GF_LEN * (i)]),
//                    .in_init_coef1(in_init_coef1[GF_LEN * (i + 1) - 1 : GF_LEN * (i)]),
//                    .in_init_coef2(in_init_coef2[GF_LEN * (i + 1) - 1 : GF_LEN * (i)]),
//                    .in_init_coef3(in_init_coef3[GF_LEN * (i + 1) - 1 : GF_LEN * (i)]),
                    .in_init_coef(
                        in_init_coef[
                            GF_LEN * PARAM_EQATION_COEFFI_NUM * ((i * LO_PARAM_BUS_IN_COEF_EN) + 1) - 1 : 
                            GF_LEN * PARAM_EQATION_COEFFI_NUM * (i * LO_PARAM_BUS_IN_COEF_EN)]
                    ),
    
                    .out_equal(out_equal[PARALLELISM * (i + 1) - 1 : PARALLELISM * i]),
                    .out_errCnt(out_erCnt[PARAM_ROOT_CNT_LEN * (i + 1) - 1 : PARAM_ROOT_CNT_LEN * (i)]),

//                    .out_done_coef0(out_done_coef0[GF_LEN * (i + 1) - 1 : GF_LEN * (i)]),
//                    .out_done_coef1(out_done_coef1[GF_LEN * (i + 1) - 1 : GF_LEN * (i)]),
//                    .out_done_coef2(out_done_coef2[GF_LEN * (i + 1) - 1 : GF_LEN * (i)]),
//                    .out_done_coef3(out_done_coef3[GF_LEN * (i + 1) - 1 : GF_LEN * (i)])
                    .out_done_coef(out_bus_done_coef[GF_LEN * PARAM_EQATION_COEFFI_NUM * (i + 1) - 1 : GF_LEN * PARAM_EQATION_COEFFI_NUM * (i)])

                );
                    initial begin
                        $display("SC_sd)%d-th in_init_coef[%d : %d]", 
                                i,
                                GF_LEN * PARAM_EQATION_COEFFI_NUM * (i + 1) - 1,
                                GF_LEN * PARAM_EQATION_COEFFI_NUM * (i)
                                );
                        $display("SC_sd)%d-th out_bus_done_coef[%d : %d]", 
                                i,
                                GF_LEN * PARAM_EQATION_COEFFI_NUM * (i + 1) - 1,
                                GF_LEN * PARAM_EQATION_COEFFI_NUM * (i)
                                );
                    end
            end
    endgenerate

    /*****************************************************/
    /*****************************************************/
    /*****************************************************/
    initial begin
        $display("/**************************************************/");
        $display("Instanciating %s", DESIGN_NAME);

        $display("In %s," ,OUTTER_NAME);
        $display("MODULE_NAME : %s", MODULE_NAME);
        $display("DESIGN_NAME : %s", DESIGN_NAME);

        $display("GF_LEN=%d",GF_LEN);

        $display("PARALLELISM=%d",PARALLELISM);

        $display("HD_CORRECTABLE=%d",HD_CORRECTABLE);
        $display("PARAM_EQATION_COEFFI_NUM = %d", PARAM_EQATION_COEFFI_NUM);

        $display("SD_CORRECTABLE=%d",SD_CORRECTABLE);
        $display("PARAM_SD_EQUATION_NUMS = %d", PARAM_SD_EQUATION_NUMS);

        $display("EQUAL_BUF_EN=%d",EQUAL_BUF_EN);

        $display("DONE_COEF_BUF_EN=%d",DONE_COEF_BUF_EN);

        $display("PARAM_ROOT_CNT_LEN = %d", PARAM_ROOT_CNT_LEN);
        $display("ERR_CNT_LIMIT=%d",ERR_CNT_LIMIT);

        $display("DECISION_MODE=%d",DECISION_MODE);

        $display("LO_PARAM_BUS_CTR_CS_SIG_EN : %s", LO_PARAM_BUS_CTR_CS_SIG_EN ? "enabled" : "disabled");
        $display("LO_PARAM_WIDTH_BUS_CTR_CS_SIG = %d", LO_PARAM_WIDTH_BUS_CTR_CS_SIG);
        
        $display("LO_PARAM_BUS_IN_COEF_EN : %s", LO_PARAM_BUS_IN_COEF_EN ? "enabled" : "disabled");
        $display("LO_PARAM_WIDTH_BUS_IN_COEF = %d", LO_PARAM_WIDTH_BUS_IN_COEF);

        $display("LO_PARAM_BUS_OUT_COEF_EN : %s", LO_PARAM_BUS_OUT_COEF_EN ? "enabled" : "disabled");
        $display("LO_PARAM_WIDTH_BUS_OUT_COEF = %d", LO_PARAM_WIDTH_BUS_OUT_COEF);

        $display("DEBUG_MODE=%d",DEBUG_MODE);
        $display("DEBUG_CNT_LIMIT=%d",DEBUG_CNT_LIMIT);
        $display("/**************************************************/");
    end
    /*****************************************************/
    /*****************************************************/
    /*****************************************************/
endmodule
`endif
