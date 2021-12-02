/**********************************************************/
/*
*   Design Name : 
*   Date : YYYY.MM.DD
*/
/**********************************************************/

`ifndef DECISION_INDICATOR
`define DECISION_INDICATOR

`include "60_fifo_channel/fifo_multi_ch.v"
`include "61_shiftReg/sequentialToSimultaneousReg_psLastOpt.v"

module decision_indicator(
    clk,
    in_ctr_Srst,
    in_ctr_en,
    in_ctr_shEnOutBuf,

    in_tp_deg,
    in_tp_errCnt,

    out_indicTP
);
    /*****************************************************/
    /********************* Functions *********************/
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
    /***********************************************************************/
    /***********************************************************************/
    function    integer valToNotBool;
        input   integer val;

        begin
            if(val) valToNotBool = 0;
            else    valToNotBool = 1;
        end
    endfunction
    /***********************************************************************/
    function    integer valToBool;
        input   integer val;

        begin
            if(val) valToBool = 1;
            else    valToBool = 0;
        end
    endfunction
    /***********************************************************************/
    function    integer notBeingZero;
        input   integer val;

        begin
            if(val  <   1)  notBeingZero = 1;
            else            notBeingZero = val;
        end
    endfunction
    /***********************************************************************/
    /***********************************************************************/
    function    integer proc_nums;
        input   integer testPattern_nums;
        input   integer decision_mode;

        begin
            if(decision_mode == 0)      proc_nums = testPattern_nums;
            else if(decision_mode == 1) proc_nums = testPattern_nums;
            else if(decision_mode == 2) proc_nums = testPattern_nums;
            else if(decision_mode == 3) proc_nums = 1;
            else                        proc_nums = testPattern_nums; 
        end
    endfunction
    /***********************************************************************/
    /***********************************************************************/
    function    integer deg_input_bus_en;
        input   integer decision_mode;

        begin
            if(decision_mode == 0)  deg_input_bus_en = 1;
            else if(decision_mode == 1)  deg_input_bus_en = 1;
            else if(decision_mode == 2)  deg_input_bus_en = 1;
            else if(decision_mode == 3)  deg_input_bus_en = 0;
            else begin
                $display("in decision_ctr) decision_mode has wrong value(%d)", decision_mode);
                deg_input_bus_en = 0;
            end
        end
    endfunction
    /***********************************************************************/
    /***********************************************************************/
    function    integer errCnt_input_bus_en;
        input   integer decision_mode;

        begin
            if(decision_mode == 0)  errCnt_input_bus_en = 1;
            else if(decision_mode == 1)  errCnt_input_bus_en = 1;
            else if(decision_mode == 2)  errCnt_input_bus_en = 1;
            else if(decision_mode == 3)  errCnt_input_bus_en = 0;
            else begin
                $display("in decision_ctr) decision_mode has wrong value(%d)", decision_mode);
                errCnt_input_bus_en = 0;
            end
        end
    endfunction
/**********************************************************/
/**********************************************************/
    parameter OUTTER_NAME = "";
    parameter MODULE_NAME = "";
        localparam DESIGN_NAME = "decision_indicator";
        localparam VCD_FILE = "decision_indicator.vcd";
/**********************************************************/
/**********************************************************/
    parameter   DECISION_MODE           =   3;
/**********************************************************/
    parameter   DECISION_NUMS       =   1;
        localparam  LO_PARAM_DECISION_NUMS  =   notBeingZero(DECISION_NUMS);
//    parameter   SD_CORRECTABLE    =   3;
//        localparam  LO_PARAM_TP_NUMS    =   pow2(SD_CORRECTABLE);
        localparam  LO_PARAM_TP_NUMS    =   LO_PARAM_DECISION_NUMS; 
        localparam PARAM_PROC_TP_INDIC_NUMS    =   proc_nums(LO_PARAM_TP_NUMS, DECISION_MODE);
/**********************************************************/
    parameter   BIT_WIDTH_DEG           =   2;
        localparam  LO_PARAM_BIT_WIDTH_DEG           =   notBeingZero(BIT_WIDTH_DEG);
/**********************************************************/
    localparam  LO_PARAM_IN_BUS_EN_DEG  =   deg_input_bus_en(DECISION_MODE);
    localparam  LO_PARAM_IN_N_BUS_EN_DEG    =   valToNotBool(LO_PARAM_IN_BUS_EN_DEG);
/**********************************************************/
/**********************************************************/
    parameter   BIT_WIDTH_ERR_CNT       =   2;
        localparam  LO_PARAM_BIT_WIDTH_ERR_CNT       =   notBeingZero(BIT_WIDTH_ERR_CNT);
        
/**********************************************************/
    localparam  LO_PARAM_IN_BUS_EN_ERR_CNT  =   errCnt_input_bus_en(DECISION_MODE);
    localparam  LO_PARAM_IN_N_BUS_EN_ERR_CNT    =   valToNotBool(LO_PARAM_IN_BUS_EN_ERR_CNT);
/**********************************************************/
    /*
    * DECISION_MODE 1 and 2 use this option.
    * DECISION_MODE 3
    * if this option is disabled, the block will be combination block.
    */
    parameter   USED_INDIC_PARALLEL_OUT_BUF_EN = 1;
        localparam  LO_PARAM_USED_INDIC_PARALLEL_OUT_BUF_EN = 
            valToBool(USED_INDIC_PARALLEL_OUT_BUF_EN);

    parameter   USED_INDIC_SER2SIM_OUT_BUF_AND_VALID_SIG_EN = 0;
        localparam  LO_PARAM_USED_INDIC_SER2SIM_OUT_BUF_AND_VALID_SIG_EN = 
            valToBool(USED_INDIC_SER2SIM_OUT_BUF_AND_VALID_SIG_EN);

    localparam  LO_INDEX_OUT_DEC_CTR_START      = -1; 
        localparam  LO_INDEX_BUS_OUT_SEL_TP_SIG_START   = LO_INDEX_OUT_DEC_CTR_START + 1;
        localparam  LO_INDEX_BUS_OUT_SEL_TP_SIG_END     = 
                        LO_INDEX_BUS_OUT_SEL_TP_SIG_START + LO_PARAM_TP_NUMS - 1;

        localparam  LO_INDEX_OUT_USED_INDIC_SER2SIM_OUT_BUF_AND_VALID_SIG = 
                        LO_INDEX_BUS_OUT_SEL_TP_SIG_END + 
                        USED_INDIC_SER2SIM_OUT_BUF_AND_VALID_SIG_EN;
        localparam  LO_INDEX_OUT_DEC_CTR_END    = 
                         LO_INDEX_OUT_USED_INDIC_SER2SIM_OUT_BUF_AND_VALID_SIG; 

    localparam  LO_PARAM_OUT_DEC_CTR_SIG_WIDTH = LO_INDEX_OUT_DEC_CTR_END + 1;


    genvar  i;

/**********************************************************/
/**********************************************************/
/**********************************************************/
/**********************************************************/
    input   clk;
    input   in_ctr_Srst;
    input   in_ctr_en;
    input   in_ctr_shEnOutBuf;

    /* In in_tp_deg signal, lower degree info is located at high side, 
    * and higher degree info is located at low side.
    * For example, if t is 3 and use m-SBS algorithm, from this algorithm,
    * infomation about degree is 2. The first one is about degree is lower
    * than 2 and the second one is about degree is 3.
    * In in_tp_deg, each infomation wire about deg like below.
    * in_tp_deg(bus_tp_deg2, bus_tp_deg3), even if input bus option is enabled.
    */
    input   [(LO_PARAM_IN_BUS_EN_DEG * (PARAM_PROC_TP_INDIC_NUMS - 1) + 1) * LO_PARAM_BIT_WIDTH_DEG - 1 : 0]
                in_tp_deg;
    input   [(LO_PARAM_IN_BUS_EN_ERR_CNT * (PARAM_PROC_TP_INDIC_NUMS - 1) + 1) * LO_PARAM_BIT_WIDTH_ERR_CNT - 1 : 0]
                in_tp_errCnt;

    output  [LO_PARAM_OUT_DEC_CTR_SIG_WIDTH - 1 : 0]                out_indicTP;
/**********************************************************/
/**********************************************************/

    wire    [PARAM_PROC_TP_INDIC_NUMS - 1 : 0]                w_degLess2;
    wire    [PARAM_PROC_TP_INDIC_NUMS - 1 : 0]                w_degViolation;
    wire    [PARAM_PROC_TP_INDIC_NUMS - 1 : 0]                w_tp_selEn;


    generate
//        if((DECISION_MODE == 1)||(DECISION_MODE == 2))  begin : gen_decMode1and2
        /* Generate enable signal to select Correctable Codeword start */
        for(i = 0; i < PARAM_PROC_TP_INDIC_NUMS; i = i + 1) begin : tpEnSignalGen 
            assign  w_degLess2[i] = ~(in_tp_deg[PARAM_PROC_TP_INDIC_NUMS + i] | in_tp_deg[i]);

            assign  w_degViolation[i] = 
                    (
                        w_degLess2[i] ? 
                        1'b0 : 
                        |(in_tp_errCnt[(i * 2) + 2 - 1 : (i * 2)]  ^ {in_tp_deg[PARAM_PROC_TP_INDIC_NUMS + i], in_tp_deg[i]})
                    );

            assign  w_tp_selEn[i] = ~w_degViolation[i];
        end
        
        if(LO_PARAM_USED_INDIC_PARALLEL_OUT_BUF_EN) begin : gen_parallelOutBuf

            fifo_multi_ch
            #(
                .FIFO_LEN(1),
                .FIFO_CH_WIDTH(LO_PARAM_TP_NUMS)
            )
            uut_outBufParallelProc
            (
                .clk(clk),

                .in_ctr_Srst(in_ctr_Srst),
                .in_ctr_en(in_ctr_en & in_ctr_shEnOutBuf),

                .in(w_tp_selEn),
                .out(out_indicTP[LO_INDEX_BUS_OUT_SEL_TP_SIG_END : LO_INDEX_BUS_OUT_SEL_TP_SIG_START])
            );

        end
        else if(USED_INDIC_SER2SIM_OUT_BUF_AND_VALID_SIG_EN) begin : gen_serialOutBuf 
            sequentialToSimultaneousReg_psLastOpt
            #(
                .DIRECTION(-1),
                .SHIFT_LEN(LO_PARAM_TP_NUMS),
                .BIT_WIDTH(1),
                .PASSING_LAST(0)
            )
            uut_outBufSerialProc
            (
                .clk(clk),

                .in_ctr_Srst(in_ctr_Srst),
                .in_ctr_en(in_ctr_en),
                .in_ctr_sig(in_ctr_shEnOutBuf),

                .in(w_tp_selEn),
                .out(out_indicTP[LO_INDEX_BUS_OUT_SEL_TP_SIG_END : LO_INDEX_BUS_OUT_SEL_TP_SIG_START])
            );

            assign out_indicTP[LO_INDEX_OUT_USED_INDIC_SER2SIM_OUT_BUF_AND_VALID_SIG] = 
                w_tp_selEn;
        end
        else begin : gen_noOutBuf
        end

        /* Generate enable signal to select Correctable Codeword end */
    endgenerate
/**********************************************************/
/**********************************************************/
    initial begin
        $display("/**************************************************/");
        $display("Instanciating %s", DESIGN_NAME);

        $display("In %s," ,OUTTER_NAME);
        $display("MODULE_NAME : %s", MODULE_NAME);
        $display("DESIGN_NAME : %s", DESIGN_NAME);

        $display("DECISION_MODE = %d", DECISION_MODE);

        $display("DECISION_NUMS = %d", DECISION_NUMS);
        $display("LO_PARAM_DECISION_NUMS = %d", LO_PARAM_DECISION_NUMS);
//        $display("SD_CORRECTABLE = %d", SD_CORRECTABLE);
        $display("LO_PARAM_TP_NUMS = %d", LO_PARAM_TP_NUMS);
        $display("PARAM_PROC_TP_INDIC_NUMS = %d", PARAM_PROC_TP_INDIC_NUMS);

        $display("BIT_WIDTH_DEG = %d", BIT_WIDTH_DEG);
        $display("LO_PARAM_BIT_WIDTH_DEG = %d", LO_PARAM_BIT_WIDTH_DEG);

        $display("LO_PARAM_IN_BUS_EN_DEG : %s", LO_PARAM_IN_BUS_EN_DEG ? "Enabled" : "Disabled");
        $display("LO_PARAM_IN_N_BUS_EN_DEG : %s", LO_PARAM_IN_N_BUS_EN_DEG ? "Enabled" : "Disabled");

        $display("BIT_WIDTH_ERR_CNT = %d", BIT_WIDTH_ERR_CNT);
        $display("LO_PARAM_BIT_WIDTH_ERR_CNT = %d", LO_PARAM_BIT_WIDTH_ERR_CNT);

        $display("LO_PARAM_IN_BUS_EN_ERR_CNT : %s", LO_PARAM_IN_BUS_EN_ERR_CNT ? "Enabled" : "Disabled");
        $display("LO_PARAM_IN_N_BUS_EN_ERR_CNT : %s", LO_PARAM_IN_N_BUS_EN_ERR_CNT ? "Enabled" : "Disabled");

        $display("USED_INDIC_PARALLEL_OUT_BUF_EN = %d", USED_INDIC_PARALLEL_OUT_BUF_EN);
        $display("LO_PARAM_USED_INDIC_PARALLEL_OUT_BUF_EN : %s", LO_PARAM_USED_INDIC_PARALLEL_OUT_BUF_EN ? "Enabled" : "Disabled");

        $display("USED_INDIC_SER2SIM_OUT_BUF_AND_VALID_SIG_EN = %d", USED_INDIC_SER2SIM_OUT_BUF_AND_VALID_SIG_EN);
        $display("LO_PARAM_USED_INDIC_SER2SIM_OUT_BUF_AND_VALID_SIG_EN = %s", 
            LO_PARAM_USED_INDIC_SER2SIM_OUT_BUF_AND_VALID_SIG_EN ? "enabled" : "disabled");
        $display("LO_INDEX_OUT_DEC_CTR_START = %d", LO_INDEX_OUT_DEC_CTR_START);
        $display("LO_INDEX_BUS_OUT_SEL_TP_SIG_START = %d", LO_INDEX_BUS_OUT_SEL_TP_SIG_START);
        $display("LO_INDEX_BUS_OUT_SEL_TP_SIG_END = %d", LO_INDEX_BUS_OUT_SEL_TP_SIG_END);
        $display("LO_INDEX_OUT_USED_INDIC_SER2SIM_OUT_BUF_AND_VALID_SIG = %d", LO_INDEX_OUT_USED_INDIC_SER2SIM_OUT_BUF_AND_VALID_SIG);
        $display("LO_INDEX_OUT_DEC_CTR_END = %d", LO_INDEX_OUT_DEC_CTR_END);
        $display("LO_PARAM_OUT_DEC_CTR_SIG_WIDTH = %d", LO_PARAM_OUT_DEC_CTR_SIG_WIDTH);

//        $display(" = %d", );
//        $dumpfile(VCD_FILE);
//        $dumpvars;
//        $display("VCD FILES : %s", VCD_FILE);

//      clk <= 0;



//      #1000;
//      $finish;
        $display("/**************************************************/");
    end
/**********************************************************/
/**********************************************************/
//      always #5 clk <= ~clk;
/**********************************************************/
/**********************************************************/
endmodule

`endif

