`ifndef SD_BCH_DECODER
`define SD_BCH_DECODER

/* front module start */
`include "03_SC/sequential/syndrome_computation_2.v"
`include "08_TSC/test_synd_computation/sequential/test_synd_computation.v"
/* front module controler */
`include "71_ctr_time/ctr_general/ctr_general.v"
/* front module end */

/*`include "04_mSBS/t3/seq/mSBS_t3_seq.v"*/
//`include "04_mSBS/t3/seq/mSBS_t3_p3_seq_only_en.v"
//`include "04_mSBS/t3/seq/mSBS_t3_pn_seq_only_en_2.v"
//`include "04_mSBS/t3/seq/mSBS_t3_pn_seq_only_en_3.v"
`include "04_mSBS/t3/seq/mSBS_t3_pn_seq_only_en_4_psLastOpt.v"

/*`include "13_synd_merge/t3/synd_merge.v"*/

/* Chien Search */
//`include "05_CS/order_3/sequential/chien_search_series_sd.v"
`include "05_CS/seq/chien_search_series_sd.v"

//`include "06_LLR_mag/4bit/sequential/llr_magnitude_seq.v"
`include "06_LLR_mag/4bit/sequential/llr_magnitude_seq_addCnt.v"
/*Test Syndrome Generator*/
//`include "12_TSG/3case/test_syndrome_generator.v"
`include "12_TSG/3case/TSG_init_val_2.v"

`include "14_TSCS/gf_10/testSynd_chienSearch.v"
//`include "15_DU/t3/seq/decision_unit_2.v"
`include "15_DU/t3/seq/decision_unit_3.v"

`include "16_DC/decision_controller_2.v"

/* fifo channel */
/* fifo channels are given global enable signal */
`include "60_fifo_channel/fifo_single_ch.v"
`include "60_fifo_channel/fifo_multi_ch.v"

/* Info Loaders */
`include "61_shiftReg/sequentialToSimultaneousReg_psLastOpt.v"
`include "65_ctr_info/ctr_infoLoad_ind/ctr_infoLoaders_independent.v"

/* Net models */
`include "80_net_model/prior_sel/prior_w_di_sel_path.v"

module sd_bch_dec
(
    clk,

    ctr_Srst,
    ctr_en,
    ctr_init,

    in_llr,

    out_codeword,
    out_sd_equal,
    out_corrected,

    out_out_start,
    out_dec_done
);
/**********************************************************************/
    function    integer valToBool;
        input   integer val;

        begin
            if(val) valToBool = 1;
            else    valToBool = 0;
        end
    endfunction
/**********************************************************************/
    function    integer valToNotBool;
        input   integer val;

        begin
            if(val) valToNotBool = 0;
            else    valToNotBool = 1;
        end
    endfunction
/**********************************************************************/
    localparam  LO_PARAM_DISABLE   =   0;
    localparam  LO_PARAM_ENABLE    =   1;

    localparam  DESIGN_NAME   =   "sd_bch_dec";

    parameter   IGNORE_BLK_EN_SIG   =   0;
        localparam  LO_PA_IGNORE_BLK_EN_SIG = valToBool(IGNORE_BLK_EN_SIG);

    genvar i;
    parameter GF_LEN        =   10;
    parameter LLR_LEN       =   4;
    localparam LLR_MAG_LEN  =   LLR_LEN - 1;

    parameter PARALLELISM   =   4;
        localparam  LO_PARAM_PARALLELISM    =   PARALLELISM;

    parameter CW_LEN        =   10'd1020;

    parameter HD_CORRECTABLE    =   2'd3;
        parameter PARAM_HD_CORRECTABLE_CNT_LEN    =   dec2CntBitLenCal(HD_CORRECTABLE);

    parameter SD_CORRECTABLE    =   2'd3;
        parameter PARAM_SD_CORRECTABLE_CNT_LEN    =   dec2CntBitLenCal(SD_CORRECTABLE);

        localparam  PARAM_ERR_EQ_COEF_NUMS  =   HD_CORRECTABLE + 1;

        localparam  TEST_SYND_NUM = pow2(SD_CORRECTABLE);


    parameter DECISION_MODE     =   3;

    parameter OUTPUT_BUF = 1;

    /* HD syndrome computation cycle */
    localparam HD_SYND_COM_OP_CLK_CYCLE =   CW_LEN;
    localparam HD_SYND_COM_OP_ENB_CYCLE =   HD_SYND_COM_OP_CLK_CYCLE - 1;
    /* Galois Fields parameter */
    parameter GF_FORWARD_SH_VAL         =   10'd9;
    parameter GF_BACKWARD_SH_VAL        =   10'd516;
    /* HD syndrome computation configuration */
    parameter HD_SYND_COMP_INPUT_BUF    =   0;

    parameter HD_SYND1_BIT_WEIGHT       =   10'd580;
    parameter HD_SYND1_BIT_SHIFT        =   10'd516;

    parameter HD_SYND3_BIT_WEIGHT       =   10'd401;
    parameter HD_SYND3_BIT_SHIFT        =   10'd129;

    parameter HD_SYND5_BIT_WEIGHT       =   10'd359;
    parameter HD_SYND5_BIT_SHIFT        =   10'd290;

    /* LLR to LLR magnitude converter */
    localparam LLR_TO_MAG_OP_CLK_CYCLE  =   CW_LEN;
    localparam LLR_TO_MAG_OP_ENB_CYCLE  =   LLR_TO_MAG_OP_CLK_CYCLE - 1;

    /* re-chien configuration */
    parameter   RE_CHIEN_SYND1_BIT_SHIFT    =   10'd16;
    parameter   RE_CHIEN_SYND3_BIT_SHIFT    =   10'd256;
    parameter   RE_CHIEN_SYND5_BIT_SHIFT    =   10'd36;

    parameter   DEBUG_MODE = 0;



    /* Test Syndrome Generation cycle */
    //localparam TEST_SYND_GEN_OP_CLK_CYCLE   =   (2 ^ SD_CORRECTABLE);
    //localparam TEST_SYND_GEN_OP_ENB_CYCLE   =   TEST_SYND_GEN_OP_CLK_CYCLE - 1;


    /* I/O definition */
    input clk;

    input ctr_Srst;
    input ctr_en;
    input ctr_init;

    input   [LLR_LEN * LO_PARAM_PARALLELISM - 1 : 0] in_llr;

    output  [LO_PARAM_PARALLELISM - 1 : 0]   out_codeword;
    output  [LO_PARAM_PARALLELISM - 1 : 0]   out_sd_equal;
    output  [LO_PARAM_PARALLELISM - 1 : 0]   out_corrected;

    output                          out_out_start;
    output                          out_dec_done;

    wire                            ctr_en_wire;
    generate
        if(LO_PA_IGNORE_BLK_EN_SIG) assign  ctr_en_wire = 1'b1;
        else                        assign  ctr_en_wire = ctr_en;
    endgenerate
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
    /*****************************************************/
    /*****************************************************/
            function integer dec2CntBitLenCal;
                input   integer func_in_dec2CntBitLenCal_decimal;

                integer inputValues;
                if(func_in_dec2CntBitLenCal_decimal) begin
                    inputValues = func_in_dec2CntBitLenCal_decimal;    

                    for(dec2CntBitLenCal = 0; func_in_dec2CntBitLenCal_decimal != 0; dec2CntBitLenCal = dec2CntBitLenCal + 1) begin
                        func_in_dec2CntBitLenCal_decimal = (func_in_dec2CntBitLenCal_decimal >> 1);
                    end

                    $display("func) dec2CntBitLenCal : input=%d, dec2CntBitLenCal=%d",
                            inputValues, dec2CntBitLenCal);
                end
                else begin
                    dec2CntBitLenCal = 0;

                    $display("func) dec2CntBitLenCal : input=%d, dec2CntBitLenCal=%d",
                            inputValues, dec2CntBitLenCal);
                end
            endfunction
    /*****************************************************/
    /*****************************************************/
    /*****************************************************/
    /*****************************************************/
    /*****************************************************/
    /*****************************************************/
    /************************************************************/
    /************************************************************/
    /************************************************************/
    /************************************************************/
    /************************************************************/
    /* Parsing Hard decision line from in_llr */
    wire    [LO_PARAM_PARALLELISM - 1 : 0]   w_hd;

    generate
        if(LO_PARAM_PARALLELISM == 1) begin : wiring_llr2_w_hd
            assign w_hd = in_llr[LLR_LEN - 1]; 
        end
        else begin : wiring_llr2_w_hd_p
            initial $display("< Generate Assign in_llr to w_hd>");
            for(i = 0; i < LO_PARAM_PARALLELISM; i = i + 1) begin : wiring_for_llr2_w_hd_p
                assign w_hd[i] = in_llr[(LLR_LEN * (i + 1)) - 1];

                initial $display("w_hd[%d] = in_llr[%d]", i, (LLR_LEN * (i + 1)) - 1);
            end
            initial $display("\n");
        end
    endgenerate
    /************************************************************/
    /************************************************************/
    /************************************************************/
    /************************************************************/
    /************************************************************/
    wire    ctr_HD_FIFO_init;
    wire    ctr_HD_FIFO_done;
    wire    ctr_HD_FIFO_en;

    assign  ctr_HD_FIFO_en = ctr_en_wire;


    wire    [LO_PARAM_PARALLELISM - 1 : 0]   w_in_FIFO_hd;
    wire    [LO_PARAM_PARALLELISM - 1 : 0]   w_out_FIFO_hd;

    assign  w_in_FIFO_hd = w_hd;
    //assign  out_codeword = w_out_FIFO_hd;

    generate
        if(LO_PARAM_PARALLELISM == 1) begin : CW_bitWideIs1
            fifo_single_ch
            #(.FIFO_LEN((CW_LEN) * 2 + 10))
            uut_hd_fifo
            (
                .clk(clk),

                .in_ctr_Srst(ctr_Srst),
                .in_ctr_en(ctr_HD_FIFO_en),

                .in(w_in_FIFO_hd),
                .out(w_out_FIFO_hd)
            );
        end
        else begin : CW_bitWideIs2orMore
            fifo_multi_ch
            #(
                .FIFO_LEN((((CW_LEN) * 2) / LO_PARAM_PARALLELISM) + 10),
                .FIFO_CH_WIDTH(LO_PARAM_PARALLELISM)
            )
            uut_hd_p_fifo
            (
                .clk(clk),

                .in_ctr_Srst(ctr_Srst),
                .in_ctr_en(ctr_HD_FIFO_en),

                .in(w_in_FIFO_hd),
                .out(w_out_FIFO_hd)
            );
        end
    endgenerate

    /************************************************************/
    /************************************************************/
    /************************************************************/
    /************************************************************/
    /************************************************************/
    /* Front Controller wire definition */
    /* 
     * Front Controller control LLR2MAG, Syndrome Computation
     * and Test Syndrome Computation Block.
    */
    /************************************************************/
    wire    ctr_ctr0_init ;
    wire    ctr_ctr0_done ;
    assign  ctr_ctr0_init = ctr_init ;
    
    wire    [2-1 : 0]ctr_bus_ctr0_blk_init ;
    wire    [2-1 : 0]ctr_bus_ctr0_blk_done ;
    wire    [2-1 : 0]ctr_bus_ctr0_blk_en ;

    wire    ctr_sig_ctr0_ptl_init ;
    wire    ctr_sig_ctr0_ptl_last ;
    /************************************************************/
    /************************************************************/
    controller_general
    #(
            .MODULE_NAME("uut_ctr0_SC_TSC"),
            .INPUT_CTR_BUF(0),
            .PARALLELISM(LO_PARAM_PARALLELISM), 
            .OP_CLK_CYCLE(CW_LEN),

            .HAVING_CLK_DISTANCE_BLK_NUMS(2),
            .WORKING_CLK_DISTANCE_DIVIDER(1),
            
            .PARTIAL_CTR_EN(1),
            .PARTIAL_PHASE_NUM(SD_CORRECTABLE),
            .PARTIAL_CLK_DISTANCE(1),

            .MODULE_ENABLER_EN(1)
    )
    uut_ctr0_SC_TSC
    (
        .clk(clk),
        .in_ctr_Srst(ctr_Srst),
        .in_ctr_en(ctr_en_wire),
        .in_ctr_init(ctr_ctr0_init),

        .out_bus_ctr_bl_init(ctr_bus_ctr0_blk_init),
        .out_bus_ctr_bl_done(ctr_bus_ctr0_blk_done),
        .out_bus_ctr_bl_last(),
        .out_bus_ctr_bl_en(ctr_bus_ctr0_blk_en),

        .out_ctr_ptl_init(ctr_sig_ctr0_ptl_init),
        .out_ctr_ptl_last(ctr_sig_ctr0_ptl_last),

        .out_ctr_done(ctr_ctr0_done)
    );
    /************************************************************/
    /************************************************************/
    /************************************************************/
    /************************************************************/
    /************************************************************/
    /* controller start */
    /* Syndrome computation initial signal */
    wire    ctr_SC_init;
    wire    ctr_SC_done;
    wire    ctr_SC_en;

    assign  ctr_SC_init = ctr_bus_ctr0_blk_init[0];
    assign  ctr_SC_done = ctr_bus_ctr0_blk_done[0];
    assign  ctr_SC_en = ctr_bus_ctr0_blk_en[0];
    /************************************************************/
    /************************************************************/
    /************************************************************/
    /* LLR 2 Mag */
    wire    ctr_llrMag_init;
    wire    ctr_llrMag_done;
    wire    ctr_llrMag_en;

    assign  ctr_llrMag_init = ctr_bus_ctr0_blk_init[0];
    assign  ctr_llrMag_done = ctr_bus_ctr0_blk_done[0];
    assign  ctr_llrMag_en = ctr_en_wire;
    /************************************************************/
    /************************************************************/
    /************************************************************/
    /* Test Syndrome Computation */
    /* Controller start */
    wire    ctr_TSC_init;
    wire    ctr_TSC_done;
    wire    ctr_TSC_en;

    assign  ctr_TSC_init = ctr_bus_ctr0_blk_init[1];
    assign  ctr_TSC_done = ctr_bus_ctr0_blk_done[1];
    assign  ctr_TSC_en = ctr_bus_ctr0_blk_en[1];
    /************************************************************/
    /************************************************************/
    /* Test Syndrome Computation enable signal */
    /************************************************************/
    wire    ctr_TSC_phase;
    wire    ctr_TSC_last_bit;

    assign  ctr_TSC_phase = ctr_sig_ctr0_ptl_init;
    assign  ctr_TSC_last_bit = ctr_sig_ctr0_ptl_last;
    /* controller end */
    /************************************************************/
    /************************************************************/
    /************************************************************/
    /************************************************************/
    /************************************************************/
    /* Hard Decision module(Syndrome computation) */


    wire [GF_LEN - 1 : 0] w_SC_synd1;
    wire [GF_LEN - 1 : 0] w_SC_synd3;
    wire [GF_LEN - 1 : 0] w_SC_synd5;

    /* hard decision bit come from vector module */

    /* syndrome 1 computation module */
    syndrome_computation_2
    #(  
        .MODULE_NAME(" uut_0_synd1_compute"),

        .INPUT_BUF_LEN(HD_SYND_COMP_INPUT_BUF), 
        .PARALLELISM(LO_PARAM_PARALLELISM),
        .SYNDROME(1),
        .HD_BIT_WEIGHT(HD_SYND1_BIT_WEIGHT),

        .GF_LEN(GF_LEN),
        .GF_SH_FORWARD(GF_FORWARD_SH_VAL),
        .GF_SH_BACKWARD(GF_BACKWARD_SH_VAL)
        )
     uut_0_synd1_compute
    (
        .clk(clk),

        .in_ctr_Srst(ctr_Srst),
        .in_ctr_en(ctr_en_wire),

        .in_ctr_SC_en(ctr_SC_en),
        .in_ctr_init(ctr_SC_init),
        .in_ctr_done(ctr_SC_done),

        .in_sig_one_bit(w_hd),

        .out_sig_syndrome(w_SC_synd1)
    );

    /* syndrome 3 computation module */
    syndrome_computation_2
    #(  
        .MODULE_NAME(" uut_0_synd3_compute"),

        .INPUT_BUF_LEN(HD_SYND_COMP_INPUT_BUF), 
        .PARALLELISM(LO_PARAM_PARALLELISM),
        .SYNDROME(3),
        .HD_BIT_WEIGHT(HD_SYND3_BIT_WEIGHT),

        .GF_LEN(GF_LEN),
        .GF_SH_FORWARD(GF_FORWARD_SH_VAL),
        .GF_SH_BACKWARD(GF_BACKWARD_SH_VAL)
        )
     uut_0_synd3_compute
    (
        .clk(clk),

        .in_ctr_Srst(ctr_Srst),
        .in_ctr_en(ctr_en_wire),

        .in_ctr_SC_en(ctr_SC_en),
        .in_ctr_init(ctr_SC_init),
        .in_ctr_done(ctr_SC_done),

        .in_sig_one_bit(w_hd),

        .out_sig_syndrome(w_SC_synd3)
    );

    /* syndrome 5 computation module */
    syndrome_computation_2
    #(  
        .MODULE_NAME(" uut_0_synd5_compute"),

        .INPUT_BUF_LEN(HD_SYND_COMP_INPUT_BUF), 
        .PARALLELISM(LO_PARAM_PARALLELISM),
        .SYNDROME(5),
        .HD_BIT_WEIGHT(HD_SYND5_BIT_WEIGHT),

        .GF_LEN(GF_LEN),
        .GF_SH_FORWARD(GF_FORWARD_SH_VAL),
        .GF_SH_BACKWARD(GF_BACKWARD_SH_VAL)
        )
     uut_0_synd5_compute
    (
        .clk(clk),

        .in_ctr_Srst(ctr_Srst),
        .in_ctr_en(ctr_en_wire),

        .in_ctr_SC_en(ctr_SC_en),
        .in_ctr_init(ctr_SC_init),
        .in_ctr_done(ctr_SC_done),

        .in_sig_one_bit(w_hd),

        .out_sig_syndrome(w_SC_synd5)
    );
    /************************************************************/
    /************************************************************/
    /************************************************************/
    /************************************************************/
    /************************************************************/
    /* LLR magnitude */
    /************************************************************/
    wire [LLR_LEN * LO_PARAM_PARALLELISM - 1 : 0]      w_LLR;
    wire [LLR_MAG_LEN * LO_PARAM_PARALLELISM - 1 : 0]  w_LLR_mag;
    wire                        w_alpha_cnt;

    assign w_LLR = in_llr;

    /* LLR mag converter module */
    llr_magnitude_seq_addCnt
    #(
        .LLR_LEN(LLR_LEN),
        .ALPHA_0_XOR_MODE(0),
        .PARALLELISM(LO_PARAM_PARALLELISM)
    )
    uut_0_llr2mag_addCnt
    (
        .clk(clk),

        .in_ctr_Srst(ctr_Srst),
        .in_ctr_en(ctr_llrMag_en),

        .in_ctr_init(ctr_llrMag_init),
        .in_ctr_done(ctr_llrMag_done),

        .in_llr(w_LLR),

        .out_alpha_cnt(w_alpha_cnt),
        .out_llr_mag(w_LLR_mag)
    );
    /*
    llr_magnitude_seq
    #(.LLR_LEN(LLR_LEN))
    uut_llr2mag
    (
        .clk(clk),

        .in_ctr_Srst(ctr_Srst),
        .in_ctr_en(ctr_llrMag_en),

        .in_llr(w_LLR),
        .out_llr_mag(w_LLR_mag)
    );
    */
    /************************************************************/
    /************************************************************/
    /************************************************************/
    /************************************************************/
    /************************************************************/
    /************************************************************/
    /************************************************************/
    localparam  LO_PA_PARALLEL_TSC_BLK_EN = valToBool(0);
    localparam  LO_PA_TSC_ALPHA_NUMS    =   SD_CORRECTABLE;
    localparam  LO_PA_TSC_POWER_NUMS    =   3; //fixed in t = 3
    localparam  LO_PA_BUS_WIDTH_ALL_TS_OUT_WIDTH  =
            ((LO_PA_TSC_POWER_NUMS - 1) * LO_PA_PARALLEL_TSC_BLK_EN + 1)
            * LO_PA_TSC_ALPHA_NUMS * GF_LEN; 
    initial begin
        $display("//////* Test Syndrome computation *//////");

        $display("LO_PA_PARALLEL_TSC_BLK_EN : %s", 
            LO_PA_PARALLEL_TSC_BLK_EN ? "enabled" : "disabled");
        $display("LO_PA_TSC_ALPHA_NUMS = %d", LO_PA_TSC_ALPHA_NUMS);
        $display("LO_PA_TSC_POWER_NUMS = %d", LO_PA_TSC_POWER_NUMS);
        $display("LO_PA_BUS_WIDTH_ALL_TS_OUT_WIDTH = %d", LO_PA_BUS_WIDTH_ALL_TS_OUT_WIDTH);
        $display("w_bus_out_TSC [%d - 1 : 0]", LO_PA_BUS_WIDTH_ALL_TS_OUT_WIDTH);
    end
    /* Test Syndrome computation, pow1 */
    wire    [LO_PA_BUS_WIDTH_ALL_TS_OUT_WIDTH - 1 : 0]  w_bus_out_TSC ;
    /*
    To abstract indexing
    i = alphaNUMS
    increase of i is LO_PA_TSC_POWER_NUMS;
                                        
    i = 0(use variable)    
    alpha2_p1 : w_bus_out_TSC[GF_LEN * (i + 1 + 2) - 1 : GF_LEN * (i + 0 + 2)];
    alpha1_p1 : w_bus_out_TSC[GF_LEN * (i + 1 + 1) - 1 : GF_LEN * (i + 0 + 1)];
    alpha0_p1 : w_bus_out_TSC[GF_LEN * (i + 1 + 0) - 1 : GF_LEN * (i + 0 + 0)];
    (static val)
    alpha2_p1 : w_bus_out_TSC[GF_LEN * (0 + 1 + 2) - 1 : GF_LEN * (0 + 0 + 2)];
    alpha1_p1 : w_bus_out_TSC[GF_LEN * (0 + 1 + 1) - 1 : GF_LEN * (0 + 0 + 1)];
    alpha0_p1 : w_bus_out_TSC[GF_LEN * (0 + 1 + 0) - 1 : GF_LEN * (0 + 0 + 0)];

    i = 3(use variable)
    alpha2_p3 : w_bus_out_TSC[GF_LEN * (i + 1 + 2) - 1 : GF_LEN * (i + 0 + 2)];
    alpha1_p3 : w_bus_out_TSC[GF_LEN * (i + 1 + 1) - 1 : GF_LEN * (i + 0 + 1)];
    alpha0_p3 : w_bus_out_TSC[GF_LEN * (i + 1 + 0) - 1 : GF_LEN * (i + 0 + 0)];
    (static val)
    alpha2_p3 : w_bus_out_TSC[GF_LEN * (3 + 1 + 2) - 1 : GF_LEN * (3 + 0 + 2)];
    alpha1_p3 : w_bus_out_TSC[GF_LEN * (3 + 1 + 1) - 1 : GF_LEN * (3 + 0 + 1)];
    alpha0_p3 : w_bus_out_TSC[GF_LEN * (3 + 1 + 0) - 1 : GF_LEN * (3 + 0 + 0)];
    
    i = 6(use variable)
    alpha2_p5 : w_bus_out_TSC[GF_LEN * (i + 1 + 2) - 1 : GF_LEN * (i + 0 + 2)];
    alpha1_p5 : w_bus_out_TSC[GF_LEN * (i + 1 + 1) - 1 : GF_LEN * (i + 0 + 1)];
    alpha0_p5 : w_bus_out_TSC[GF_LEN * (i + 1 + 0) - 1 : GF_LEN * (i + 0 + 0)];
    (static val)
    alpha2_p5 : w_bus_out_TSC[GF_LEN * (6 + 1 + 2) - 1 : GF_LEN * (6 + 0 + 2)];
    alpha1_p5 : w_bus_out_TSC[GF_LEN * (6 + 1 + 1) - 1 : GF_LEN * (6 + 0 + 1)];
    alpha0_p5 : w_bus_out_TSC[GF_LEN * (6 + 1 + 0) - 1 : GF_LEN * (6 + 0 + 0)];
    
    * */

generate
    test_synd_computation
    #(
        .OUTTER_NAME("sd_bch_dec"),
        .MODULE_NAME("uut_0_TSCompute_p1"),

        .GF_LEN(GF_LEN), 
        .POW_OF_SYND(1), 
        .LLR_MAG_LEN(LLR_MAG_LEN), 

        .PARALLELISM(LO_PARAM_PARALLELISM),

        .GF_SH_FORWARD(GF_FORWARD_SH_VAL),
        .GF_SH_BACKWARD(GF_BACKWARD_SH_VAL)
        )
    uut_0_TSCompute_p1
    (
        .clk(clk),

        .in_ctr_Srst(ctr_Srst),
        .in_ctr_en(ctr_en_wire),

        .in_ctr_init(ctr_TSC_init),

        .in_ctr_TSC_en(ctr_TSC_en),

        .in_ctr_phase_shift(ctr_TSC_phase),

        .in_init_alpha_val({{(GF_LEN - 1){1'b0}}, 1'b1}),

        .in_llr_mag(w_LLR_mag),

        .out_sig_alpha3(w_bus_out_TSC[GF_LEN * (0 + 1 + 2) - 1 : GF_LEN * (0 + 0 + 2)]),
        .out_sig_alpha2(w_bus_out_TSC[GF_LEN * (0 + 1 + 1) - 1 : GF_LEN * (0 + 0 + 1)]),
        .out_sig_alpha1(w_bus_out_TSC[GF_LEN * (0 + 1 + 0) - 1 : GF_LEN * (0 + 0 + 0)])
    );
    if(LO_PA_PARALLEL_TSC_BLK_EN) begin : gen_TSC_PARALLEL
        /************************************************************/
        /************************************************************/
        /* Test Syndrome computation, pow3 */

        test_synd_computation
        #(
            .OUTTER_NAME("sd_bch_dec"),
            .MODULE_NAME("uut_0_TSCompute_p3"),

            .GF_LEN(GF_LEN), 
            .POW_OF_SYND(3), 
            .LLR_MAG_LEN(LLR_MAG_LEN), 

            .PARALLELISM(LO_PARAM_PARALLELISM),

            .GF_SH_FORWARD(GF_FORWARD_SH_VAL),
            .GF_SH_BACKWARD(GF_BACKWARD_SH_VAL)
            )
        uut_0_TSCompute_p3
        (
            .clk(clk),

            .in_ctr_Srst(ctr_Srst),
            .in_ctr_en(ctr_en_wire),

            .in_ctr_init(ctr_TSC_init),

            .in_ctr_TSC_en(ctr_TSC_en),

            .in_ctr_phase_shift(ctr_TSC_phase),

            .in_init_alpha_val({{(GF_LEN - 1){1'b0}}, 1'b1}),

            .in_llr_mag(w_LLR_mag),

            .out_sig_alpha3(w_bus_out_TSC[GF_LEN * (3 + 1 + 2) - 1 : GF_LEN * (3 + 0 + 2)]),
            .out_sig_alpha2(w_bus_out_TSC[GF_LEN * (3 + 1 + 1) - 1 : GF_LEN * (3 + 0 + 1)]),
            .out_sig_alpha1(w_bus_out_TSC[GF_LEN * (3 + 1 + 0) - 1 : GF_LEN * (3 + 0 + 0)])
        );
        /************************************************************/
        /************************************************************/
        /* Test Syndrome computation, pow5 */

        test_synd_computation
        #(
            .OUTTER_NAME("sd_bch_dec"),
            .MODULE_NAME("uut_0_TSCompute_p5"),

            .GF_LEN(GF_LEN), 
            .POW_OF_SYND(5), 
            .LLR_MAG_LEN(LLR_MAG_LEN), 

            .PARALLELISM(LO_PARAM_PARALLELISM),

            .GF_SH_FORWARD(GF_FORWARD_SH_VAL),
            .GF_SH_BACKWARD(GF_BACKWARD_SH_VAL)
            )
        uut_0_TSCompute_p5
        (
            .clk(clk),

            .in_ctr_Srst(ctr_Srst),
            .in_ctr_en(ctr_en_wire),

            .in_ctr_init(ctr_TSC_init),

            .in_ctr_TSC_en(ctr_TSC_en),

            .in_ctr_phase_shift(ctr_TSC_phase),

            .in_init_alpha_val({{(GF_LEN - 1){1'b0}}, 1'b1}),

            .in_llr_mag(w_LLR_mag),

            .out_sig_alpha3(w_bus_out_TSC[GF_LEN * (6 + 1 + 2) - 1 : GF_LEN * (6 + 0 + 2)]),
            .out_sig_alpha2(w_bus_out_TSC[GF_LEN * (6 + 1 + 1) - 1 : GF_LEN * (6 + 0 + 1)]),
            .out_sig_alpha1(w_bus_out_TSC[GF_LEN * (6 + 1 + 0) - 1 : GF_LEN * (6 + 0 + 0)])
        );
    end
endgenerate
    /************************************************************/
    /************************************************************/
    /************************************************************/
    /************************************************************/
    /************************************************************/
    /************************************************************/
    /************************************************************/
    /************************************************************/
    /************************************************************/
    /************************************************************/
    /************************************************************/
    wire    ctr_ctr1_init ;
    wire    ctr_ctr1_done ;
    assign  ctr_ctr1_init = ctr_ctr0_done ;

    wire    [1 - 1 : 0] ctr_bus_ctr1_blk_init ;
    wire    [1 - 1 : 0] ctr_bus_ctr1_blk_done ;
    wire    [1 - 1 : 0] ctr_bus_ctr1_blk_en ;

    wire    ctr_sig_ctr1_ptl_init ;
    wire    ctr_sig_ctr1_ptl_last ;
    /************************************************************/
    /************************************************************/
    controller_general
    #(
            .MODULE_NAME("uut_ctr1_MSBS_TSG"),
            .INPUT_CTR_BUF(0),
            .PARALLELISM(1), 
            .OP_CLK_CYCLE(8),

            .HAVING_CLK_DISTANCE_BLK_NUMS(1),
            .WORKING_CLK_DISTANCE_DIVIDER(1),
            
            .PARTIAL_CTR_EN(0),
            //.PARTIAL_CTR_EN(1),
            //.PARTIAL_PHASE_NUM(SD_CORRECTABLE),
            //.PARTIAL_CLK_DISTANCE(1),
            
            .MODULE_ENABLER_EN(1)
    )
    uut_ctr1_MSBS_TSG
    (
        .clk(clk),
        .in_ctr_Srst(ctr_Srst),
        .in_ctr_en(ctr_en_wire),
        .in_ctr_init(ctr_ctr1_init),

        .out_bus_ctr_bl_init(ctr_bus_ctr1_blk_init),
        .out_bus_ctr_bl_done(ctr_bus_ctr1_blk_done),
        .out_bus_ctr_bl_last(),
        .out_bus_ctr_bl_en(ctr_bus_ctr1_blk_en),

        .out_ctr_ptl_init(ctr_sig_ctr1_ptl_init),
        .out_ctr_ptl_last(ctr_sig_ctr1_ptl_last),

        .out_ctr_done(ctr_ctr1_done)
    );
    /************************************************************/
    /************************************************************/
    /************************************************************/
    /************************************************************/
    /************************************************************/
    wire    ctr_TSG_init;
    wire    ctr_TSG_done;
    wire    ctr_TSG_en;

    assign  ctr_TSG_init = ctr_bus_ctr1_blk_init[0];
    assign  ctr_TSG_done = ctr_bus_ctr1_blk_done[0];
    assign  ctr_TSG_en = ctr_bus_ctr1_blk_en[0];
    /************************************************************/
    wire    ctr_mSBS_init;
    wire    ctr_mSBS_done;

    assign  ctr_mSBS_init = ctr_bus_ctr1_blk_init[0];
    assign  ctr_mSBS_done = ctr_bus_ctr1_blk_done[0];
    /************************************************************/
    /************************************************************/
    /************************************************************/
    /************************************************************/
    /************************************************************/
    localparam  LO_PA_TSG_POW_CAL_EN = valToNotBool(LO_PA_PARALLEL_TSC_BLK_EN);
    localparam  LO_PA_TSG_POW_NUMS = 3;//Fixed param
    localparam  LO_PA_TO_TSG_INPUT_PORT_TS_WIDTH = 
        GF_LEN * ((LO_PA_TSG_POW_NUMS - 1) * LO_PA_PARALLEL_TSC_BLK_EN + 1);
    initial begin
        $display("//////* Test Syndrome Generator*//////");
        $display("LO_PA_TSG_POW_CAL_EN : %s", 
            LO_PA_TSG_POW_CAL_EN ? "enabled" : "disabled" );
        $display("LO_PA_TSG_POW_NUMS = %d", LO_PA_TSG_POW_NUMS);
        $display("LO_PA_TO_TSG_INPUT_PORT_TS_WIDTH = %d", LO_PA_TO_TSG_INPUT_PORT_TS_WIDTH);
        $display("w_in_data_alpha_0_TSG [%d - 1 : 0]", LO_PA_TO_TSG_INPUT_PORT_TS_WIDTH);
        $display("w_in_data_alpha_1_TSG [%d - 1 : 0]", LO_PA_TO_TSG_INPUT_PORT_TS_WIDTH);
        $display("w_in_data_alpha_2_TSG [%d - 1 : 0]", LO_PA_TO_TSG_INPUT_PORT_TS_WIDTH);
    end
    /* Test Syndrome Generator*/
    wire    [GF_LEN - 1 : 0]   w_TSG_init_val1;
    wire    [GF_LEN - 1 : 0]   w_TSG_init_val3;
    wire    [GF_LEN - 1 : 0]   w_TSG_init_val5;

    assign  w_TSG_init_val1 = w_SC_synd1;
    assign  w_TSG_init_val3 = w_SC_synd3;
    assign  w_TSG_init_val5 = w_SC_synd5;
    /************************************************************/
    /*wire    [GF_LEN - 1 : 0]    w_in_data_alpha_0_TSG;
    wire    [GF_LEN - 1 : 0]    w_in_data_alpha_1_TSG;
    wire    [GF_LEN - 1 : 0]    w_in_data_alpha_2_TSG;*/

    wire    [LO_PA_TO_TSG_INPUT_PORT_TS_WIDTH - 1 : 0]    w_in_data_alpha_0_TSG;
    wire    [LO_PA_TO_TSG_INPUT_PORT_TS_WIDTH - 1 : 0]    w_in_data_alpha_1_TSG;
    wire    [LO_PA_TO_TSG_INPUT_PORT_TS_WIDTH - 1 : 0]    w_in_data_alpha_2_TSG;

    generate
        if(LO_PA_TSG_POW_CAL_EN) 
        begin : gen_TSG_POW_EN
            assign  w_in_data_alpha_0_TSG = { 
                    w_bus_out_TSC[GF_LEN * (0 + 1 + 0) - 1 : GF_LEN * (0 + 0 + 0)]  // w_TSC_alpha1_p1
                };
            assign  w_in_data_alpha_1_TSG = { 
                    w_bus_out_TSC[GF_LEN * (0 + 1 + 1) - 1 : GF_LEN * (0 + 0 + 1)]   // w_TSC_alpha2_p1};
                };
            assign  w_in_data_alpha_2_TSG = { 
                    w_bus_out_TSC[GF_LEN * (0 + 1 + 2) - 1 : GF_LEN * (0 + 0 + 2)]   // w_TSC_alpha3_p1
                };
        end
        else 
        begin : gen_TSG_POW_DIS
            assign  w_in_data_alpha_0_TSG = { 
                    w_bus_out_TSC[GF_LEN * (6 + 1 + 0) - 1 : GF_LEN * (6 + 0 + 0)], // w_TSC_alpha1_p5, 
                    w_bus_out_TSC[GF_LEN * (3 + 1 + 0) - 1 : GF_LEN * (3 + 0 + 0)], // w_TSC_alpha1_p3, 
                    w_bus_out_TSC[GF_LEN * (0 + 1 + 0) - 1 : GF_LEN * (0 + 0 + 0)]  // w_TSC_alpha1_p1
                };
            assign  w_in_data_alpha_1_TSG = { 
                    w_bus_out_TSC[GF_LEN * (6 + 1 + 1) - 1 : GF_LEN * (6 + 0 + 1)],  // w_TSC_alpha2_p5, 
                    w_bus_out_TSC[GF_LEN * (3 + 1 + 1) - 1 : GF_LEN * (3 + 0 + 1)],  // w_TSC_alpha2_p3, 
                    w_bus_out_TSC[GF_LEN * (0 + 1 + 1) - 1 : GF_LEN * (0 + 0 + 1)]   // w_TSC_alpha2_p1};
                };
            assign  w_in_data_alpha_2_TSG = { 
                    w_bus_out_TSC[GF_LEN * (6 + 1 + 2) - 1 : GF_LEN * (6 + 0 + 2)],  // w_TSC_alpha3_p5, 
                    w_bus_out_TSC[GF_LEN * (3 + 1 + 2) - 1 : GF_LEN * (3 + 0 + 2)],  // w_TSC_alpha3_p3, 
                    w_bus_out_TSC[GF_LEN * (0 + 1 + 2) - 1 : GF_LEN * (0 + 0 + 2)]   // w_TSC_alpha3_p1
                };
        end
    endgenerate
    /************************************************************/
    wire [GF_LEN - 1 : 0] w_out_TS_P1;
    wire [GF_LEN - 1 : 0] w_out_TS_P3;
    wire [GF_LEN - 1 : 0] w_out_TS_P5;
    /************************************************************/
    TSG_init_val_2
    #(
    //.SYND_CAL_TYPE(0),
    .GF_LEN(GF_LEN),
    .TEST_SYND_NUM(SD_CORRECTABLE),
    .POWER_CAL_EN(LO_PA_TSG_POW_CAL_EN)
    )
    uut_1_TSG
    (
        .clk(clk),
        .in_ctr_Srst(ctr_Srst),
        .in_ctr_en(ctr_en_wire),

        .in_ctr_init(ctr_TSG_init),

        .in_ctr_TSG_en(ctr_TSG_en),

        .in_init_val_hd_alpha1(w_TSG_init_val1),
        .in_init_val_hd_alpha3(w_TSG_init_val3),
        .in_init_val_hd_alpha5(w_TSG_init_val5),

        .in_data_alpha_0(w_in_data_alpha_0_TSG),
        .in_data_alpha_1(w_in_data_alpha_1_TSG),
        .in_data_alpha_2(w_in_data_alpha_2_TSG),

        .out_testSyndPow1(w_out_TS_P1),
        .out_testSyndPow3(w_out_TS_P3),
        .out_testSyndPow5(w_out_TS_P5)
    );
    /************************************************************/
    /************************************************************/
    /************************************************************/
    /************************************************************/
    /* Hard Decision Kernel */
    wire    [1 - 1 : 0]         ctr_sig_mSBS;

    assign  ctr_sig_mSBS = {
        ctr_bus_ctr1_blk_en[0] // mSBS enable signal
        };


    wire    [GF_LEN - 1 : 0 ]   w_in_mSBS_synd_1;
    wire    [GF_LEN - 1 : 0 ]   w_in_mSBS_synd_3;
    wire    [GF_LEN - 1 : 0 ]   w_in_mSBS_synd_5;

    assign  w_in_mSBS_synd_1 = w_out_TS_P1;
    assign  w_in_mSBS_synd_3 = w_out_TS_P3;
    assign  w_in_mSBS_synd_5 = w_out_TS_P5;

//    wire    [GF_LEN * TEST_SYND_NUM - 1 : 0]    w_mSBS_2_tp_A;
//    wire    [GF_LEN * TEST_SYND_NUM - 1 : 0]    w_mSBS_2_tp_B;
//    wire    [GF_LEN * TEST_SYND_NUM - 1 : 0]    w_mSBS_2_tp_C;
//    wire    [GF_LEN * TEST_SYND_NUM - 1 : 0]    w_mSBS_2_tp_R;

//    wire    [GF_LEN * TEST_SYND_NUM * PARAM_ERR_EQ_COEF_NUMS - 1 : 0]    
//              w_out_mSBS_coef;//DECISION_MODE 1, 2
    wire    [GF_LEN * PARAM_ERR_EQ_COEF_NUMS - 1 : 0]    
                w_out_mSBS_coef;//DECISION_MODE 3

//    wire    [TEST_SYND_NUM - 1 : 0]
//    w_mSBS_2_tp_deg2;//DECISION_MODE1, 2
//    wire    [TEST_SYND_NUM - 1 : 0]
//    w_mSBS_2_tp_deg3;//DECISION_MODE1, 2
    wire    [1 - 1 : 0]             w_mSBS_2_tp_deg2;//DECISION_MODE 3
    wire    [1 - 1 : 0]             w_mSBS_2_tp_deg3;//DECISION_MODE 3

    mSBS_t3_pn_seq_only_en_4
    #(
        .OUTTER_NAME("sd_bch_dec"),
        .MODULE_NAME("uut_1_mSBS_kernal4"),
        .DECISION_MODE(DECISION_MODE),

        .GF_LEN(GF_LEN),
        .TEST_PATTEN_NUMS(TEST_SYND_NUM),

        .OUT_BUF_SHIFING_DIRECTION(-1),

        .PASSINS_LAST_EN(LO_PARAM_DISABLE)
    )
    uut_1_mSBS_kernal4
    (
        .clk(clk),
        .in_ctr_Srst(ctr_Srst),
        .in_ctr_en(ctr_en_wire),
       
        .in_ctr_sig(),

        .in_synd1(w_in_mSBS_synd_1),
        .in_synd3(w_in_mSBS_synd_3),
        .in_synd5(w_in_mSBS_synd_5),

        .out_coef(w_out_mSBS_coef),


        .out_tp_deg2(w_mSBS_2_tp_deg2),
        .out_tp_deg3(w_mSBS_2_tp_deg3)
    );
    /************************************************************/
    /************************************************************/
    /************************************************************/
    /************************************************************/
    /************************************************************/
    wire    ctr_ctr2_init ;
    wire    ctr_ctr2_done ;

//    assign  ctr_ctr2_init = ctr_ctr1_done ;//DECISION_MODE 1, 2
    assign  ctr_ctr2_init = ctr_ctr0_done;//DECISION_MODE 3
    /************************************************************/
//    wire    [2-1 : 0]ctr_bus_ctr2_blk_init ;//DECISION_MODE 1, 2
//    wire    [2-1 : 0]ctr_bus_ctr2_blk_done ;//DECISION_MODE 1, 2
//    wire    [2-1 : 0]ctr_bus_ctr2_blk_en ;//DECISION_MODE 1, 2
    wire    [8-1 : 0]ctr_bus_ctr2_blk_init ; //DECISION_MODE 3
    wire    [8-1 : 0]ctr_bus_ctr2_blk_done ; //DECISION_MODE 3
    wire    [8-1 : 0]ctr_bus_ctr2_blk_en ; //DECISION_MODE 3
    /************************************************************/
    wire    ctr_sig_ctr2_ptl_init ;
    wire    ctr_sig_ctr2_ptl_last ;
    /************************************************************/
    /************************************************************/
    controller_general
    #(
            .MODULE_NAME("uut_ctr2_preCS_MC"),
            .INPUT_CTR_BUF(1),
            .PARALLELISM(LO_PARAM_PARALLELISM), 
            .OP_CLK_CYCLE(CW_LEN),

//            .HAVING_CLK_DISTANCE_BLK_NUMS(2),//DECISION_MODE 1, 2
            .HAVING_CLK_DISTANCE_BLK_NUMS(8),//DECISION_MODE 3
            .WORKING_CLK_DISTANCE_DIVIDER(1),
            
            .PARTIAL_CTR_EN(0),
            //.PARTIAL_CTR_EN(1),
            //.PARTIAL_PHASE_NUM(SD_CORRECTABLE),
            //.PARTIAL_CLK_DISTANCE(1),
            
            .MODULE_ENABLER_EN(1)
    )
    uut_ctr2_preCS_MC
    (
        .clk(clk),
        .in_ctr_Srst(ctr_Srst),
        .in_ctr_en(ctr_en_wire),
        .in_ctr_init(ctr_ctr2_init),

        .out_bus_ctr_bl_init(ctr_bus_ctr2_blk_init),
        .out_bus_ctr_bl_done(ctr_bus_ctr2_blk_done),
        .out_bus_ctr_bl_last(),
        .out_bus_ctr_bl_en(ctr_bus_ctr2_blk_en),

        .out_ctr_ptl_init(ctr_sig_ctr2_ptl_init),
        .out_ctr_ptl_last(ctr_sig_ctr2_ptl_last),

        .out_ctr_done(ctr_ctr2_done)
    );
    /************************************************************/
    /************************************************************/
    /************************************************************/
    /************************************************************/
    /************************************************************/
//    wire    [1 - 1 : 0]ctr_bus_preCS_MC_en; //DECISION_MODE 1, 2
    wire    [8 - 1 : 0]ctr_bus_preCS_MC_en; //DECISION_MODE 3

//    assign  ctr_bus_preCS_MC_en = ctr_en_wire; //DECISION_MODE 1, 2
    assign  ctr_bus_preCS_MC_en = ctr_bus_ctr2_blk_en;//DECISION_MODE 3
    /************************************************************/
//    wire    ctr_bus_preCS_init_coef;//DECISION_MODE 1, 2
//    wire    ctr_bus_preCS_MC_done;//DECISION_MODE 1, 2
    wire    [8 - 1 : 0] ctr_bus_preCS_init_coef; //DECISION_MODE 3
    wire    [8 - 1 : 0] ctr_bus_preCS_MC_done; //DECISION_MODE 3


//    assign  ctr_bus_preCS_init_coef = ctr_bus_ctr2_blk_init[0]; //DECISION_MODE 1,2
//    assign  ctr_bus_preCS_MC_done = ctr_bus_ctr2_blk_done[0]; //DECISION_MODE 1,2
    assign  ctr_bus_preCS_init_coef = ctr_bus_ctr2_blk_init ; //DECISION_MODE 3
    assign  ctr_bus_preCS_MC_done = ctr_bus_ctr2_blk_done ; //DECISION_MODE 3

    assign  ctr_bus_preCS_MC_en   = ctr_bus_ctr2_blk_en ; //DECISION_MODE 3
    /************************************************************/
//    wire    [GF_LEN * TEST_SYND_NUM * PARAM_ERR_EQ_COEF_NUMS - 1 : 0]   
//                w_in_CS_init_coef;//DECISION_MODE 1, 3
    wire    [GF_LEN * PARAM_ERR_EQ_COEF_NUMS - 1 : 0]   
                w_in_CS_init_coef;//DECISION_MODE 3


    assign  w_in_CS_init_coef = w_out_mSBS_coef;
    /************************************************************/
//    wire    [TEST_SYND_NUM - 1 : 0] w_CS_i_tp_deg3;//DECISION_MODE 1, 2
//    wire    [TEST_SYND_NUM - 1 : 0] w_CS_i_tp_deg2;//DECISION_MODE 1, 2
//    assign  w_CS_i_tp_deg3 = w_mSBS_2_tp_deg3;//DECISION_MODE 1, 2
//    assign  w_CS_i_tp_deg2 = w_mSBS_2_tp_deg2;//DECISION_MODE 1, 2
//
//    wire    [TEST_SYND_NUM - 1 : 0] w_CS_o_tp_deg3;//DECISION_MODE 1, 2
//    wire    [TEST_SYND_NUM - 1 : 0] w_CS_o_tp_deg2;//DECISION_MODE 1, 2

    /************************************************************/
//    wire [GF_LEN - 1 : 0] w_CS_i_alpha1;//DECISION_MODE 1, 2
//    wire [GF_LEN - 1 : 0] w_CS_i_alpha2;//DECISION_MODE 1, 2
//    wire [GF_LEN - 1 : 0] w_CS_i_alpha3;//DECISION_MODE 1, 2
//
//    assign  w_CS_i_alpha1 = w_TSC_alpha1_p1;//DECISION_MODE 1, 2
//    assign  w_CS_i_alpha2 = w_TSC_alpha2_p1;//DECISION_MODE 1, 2
//    assign  w_CS_i_alpha3 = w_TSC_alpha3_p1;//DECISION_MODE 1, 2
    /************************************************************/
//    wire [GF_LEN - 1 : 0] w_CS_o_alpha1;//DECISION_MODE 1, 2
//    wire [GF_LEN - 1 : 0] w_CS_o_alpha2;//DECISION_MODE 1, 2
//    wire [GF_LEN - 1 : 0] w_CS_o_alpha3;//DECISION_MODE 1, 2
    /************************************************************/
    wire    [TEST_SYND_NUM * LO_PARAM_PARALLELISM - 1 : 0]                   
                    w_out_CS_equal;
    wire    [TEST_SYND_NUM * PARAM_SD_CORRECTABLE_CNT_LEN - 1 : 0]  
                    w_out_CS_errCnt;
    /************************************************************/
    /************************************************************/
//    wire    [GF_LEN * TEST_SYND_NUM - 1 : 0]    w_CS_doneCoef0;
//    wire    [GF_LEN * TEST_SYND_NUM - 1 : 0]    w_CS_doneCoef1;
//    wire    [GF_LEN * TEST_SYND_NUM - 1 : 0]    w_CS_doneCoef2;
//    wire    [GF_LEN * TEST_SYND_NUM - 1 : 0]    w_CS_doneCoef3;
    wire    [GF_LEN * TEST_SYND_NUM * PARAM_ERR_EQ_COEF_NUMS - 1 : 0]   w_out_CS_done_coef;
    /************************************************************/
    chien_search_series_sd
    #(
        .MODULE_NAME("uut_2_preCSandMC"),

        .GF_LEN(GF_LEN),

        .PARALLELISM(LO_PARAM_PARALLELISM),

        .HD_CORRECTABLE(HD_CORRECTABLE),
        .SD_CORRECTABLE(SD_CORRECTABLE),


        .EQUAL_BUF_EN(LO_PARAM_DISABLE),

        .DONE_COEF_BUF_EN(LO_PARAM_DISABLE),

        .ERR_CNT_LIMIT(SD_CORRECTABLE),

        .DECISION_MODE(DECISION_MODE),

        .DEBUG_MODE(DEBUG_MODE),
        .DEBUG_CNT_LIMIT(CW_LEN)

     )
    uut_2_preCSandMC
    (

        .in_ctr_Srst(ctr_Srst),
        .in_ctr_en(ctr_en_wire),
        .in_ctr_bus_CS_init(ctr_bus_preCS_init_coef),//DECISION_MODE_DEPEND
        .in_ctr_bus_CS_done(ctr_bus_preCS_MC_done),//DECISION_MODE_DEPEND

        .in_ctr_bus_CS_en(ctr_bus_preCS_MC_en),//DECISION_MODE_DEPEND

        .clk(clk),

        .in_init_coef(w_in_CS_init_coef),

        .out_equal(w_out_CS_equal),
        .out_erCnt(w_out_CS_errCnt),

        .out_bus_done_coef(w_out_CS_done_coef)
    );
    /************************************************************/
    /************************************************************/
    /************************************************************/
    /************************************************************/
    /************************************************************/
    wire    ctr_ctr3_init ;
    wire    ctr_ctr3_done ;

//    assign  ctr_ctr3_init = ctr_bus_ctr2_blk_done[0];//DECISION_MODE 1, 2
    assign  ctr_ctr3_init = ctr_bus_ctr2_blk_done[0];//DECISION_MODE 3
    /************************************************************/
    wire    [1-1 : 0]ctr_bus_ctr3_blk_init ;
    wire    [1-1 : 0]ctr_bus_ctr3_blk_done ;
    wire    [1-1 : 0]ctr_bus_ctr3_blk_en;
    /************************************************************/
    wire    ctr_sig_ctr3_ptl_init;
    wire    ctr_sig_ctr3_ptl_last;
    /************************************************************/
    /************************************************************/
    controller_general
    #(
            .MODULE_NAME("uut_ctr3_DU_DC"),
            .INPUT_CTR_BUF(0),
            .PARALLELISM(1), 
            .OP_CLK_CYCLE(8),

            .HAVING_CLK_DISTANCE_BLK_NUMS(1),
            .WORKING_CLK_DISTANCE_DIVIDER(0),
            
            .PARTIAL_CTR_EN(0),
            //.PARTIAL_CTR_EN(1),
            //.PARTIAL_PHASE_NUM(SD_CORRECTABLE),
            //.PARTIAL_CLK_DISTANCE(1),
            
            .MODULE_ENABLER_EN(1)
    )
    uut_ctr3_DU_DC
    (
        .clk(clk),
        .in_ctr_Srst(ctr_Srst),
        .in_ctr_en(ctr_en_wire),
        .in_ctr_init(ctr_ctr3_init),

        .out_bus_ctr_bl_init(ctr_bus_ctr3_blk_init),
        .out_bus_ctr_bl_done(ctr_bus_ctr3_blk_done),
        .out_bus_ctr_bl_last(),
        .out_bus_ctr_bl_en(ctr_bus_ctr3_blk_en),

        .out_ctr_ptl_init(ctr_sig_ctr3_ptl_init),
        .out_ctr_ptl_last(ctr_sig_ctr3_ptl_last),

        .out_ctr_done(ctr_ctr3_done)
    );
    /************************************************************/
    /************************************************************/
    /************************************************************/
    /************************************************************/
    /************************************************************/
    wire    ctr_ctr4_init ;
    wire    ctr_ctr4_done ;

//    assign  ctr_ctr4_init = ctr_bus_ctr2_blk_done[0];//DECISION_MODE 1, 2
    assign  ctr_ctr4_init = ctr_ctr3_done ;//DECISION_MODE 3
    /************************************************************/
    wire    [1-1 : 0]ctr_bus_ctr4_blk_init;
    wire    [1-1 : 0]ctr_bus_ctr4_blk_done;
    wire    [1-1 : 0]ctr_bus_ctr4_blk_en;
    /************************************************************/
    wire    ctr_sig_ctr4_ptl_init;
    wire    ctr_sig_ctr4_ptl_last;
    /************************************************************/
    /************************************************************/
    controller_general
    #(
            .MODULE_NAME("uut_ctr4_TSCS_reCS"),
            .INPUT_CTR_BUF(0),
            .PARALLELISM(LO_PARAM_PARALLELISM), 
            .OP_CLK_CYCLE(CW_LEN),

            .HAVING_CLK_DISTANCE_BLK_NUMS(1),
            .WORKING_CLK_DISTANCE_DIVIDER(0),
            
            .PARTIAL_CTR_EN(0),
            //.PARTIAL_CTR_EN(1),
            //.PARTIAL_PHASE_NUM(SD_CORRECTABLE),
            //.PARTIAL_CLK_DISTANCE(1),
            
            .MODULE_ENABLER_EN(1)
    )
    uut_ctr4_TSCS_reCS
    (
        .clk(clk),
        .in_ctr_Srst(ctr_Srst),
        .in_ctr_en(ctr_en_wire),
        .in_ctr_init(ctr_ctr4_init),

        .out_bus_ctr_bl_init(ctr_bus_ctr4_blk_init),
        .out_bus_ctr_bl_done(ctr_bus_ctr4_blk_done),
        .out_bus_ctr_bl_last(),
        .out_bus_ctr_bl_en(ctr_bus_ctr4_blk_en),

        .out_ctr_ptl_init(ctr_sig_ctr4_ptl_init),
        .out_ctr_ptl_last(ctr_sig_ctr4_ptl_last),

        .out_ctr_done(ctr_ctr4_done)
    );
    /************************************************************/
    /************************************************************/
    /************************************************************/
    /* Info Loader block start */
    /************************************************************/
    /* error count, which come from pre-CS and MC block, prior sel path */
    wire    [(TEST_SYND_NUM + 1) * PARAM_HD_CORRECTABLE_CNT_LEN - 1 : 0]  
                                                        w_in_priorSel_erCnt ;
    wire    [PARAM_HD_CORRECTABLE_CNT_LEN - 1 : 0]      w_out_priorSel_erCnt ;

    wire    [TEST_SYND_NUM * 1 - 1 : 0]                 w_ctr_priorSel_erCnt ;
    wire    [TEST_SYND_NUM * 1 - 1 : 0]                 w_ctr_priorSel_erCnt_D1 ;
    /************************************************************/

    assign  w_in_priorSel_erCnt = {
            {(PARAM_HD_CORRECTABLE_CNT_LEN){1'b0}}, 
            w_out_CS_errCnt
        } ;

    assign  w_ctr_priorSel_erCnt = ctr_bus_ctr2_blk_done;
//    assign  w_ctr_priorSel_erCnt_D1 = ctr_bus_ctr2_blk_done;
    /************************************************************/

//    fifo_multi_ch
//    #(
//        .OUTTER_NAME("sd_bch_dec"),
//        .MODULE_NAME("uut_x_priorSel_erCnt_sel_Buf"),
//
//        .FIFO_LEN(1),
//        .FIFO_CH_WIDTH(TEST_SYND_NUM)
//    )
//    uut_x_priorSel_erCnt_sel_Buf
//    (
//        .clk(clk),
//
//        .in_ctr_Srst(ctr_Srst),
//        .in_ctr_en(ctr_en_wire),
//
//        .in(w_ctr_priorSel_erCnt_D1 ),
//        .out(w_ctr_priorSel_erCnt)
//    );


    prior_w_di_sel_path
    #(
        .BIT_WIDTH(PARAM_HD_CORRECTABLE_CNT_LEN),
        .SEL_SIG_NUMS(TEST_SYND_NUM),
        .PRIORITY_DIRECTION(-1),
        .DEFAULT_INPUT_EN(1)
    )
    uut_x_priorSel_errCnt
    (
        .in(w_in_priorSel_erCnt), 
        .out(w_out_priorSel_erCnt), 
        .sel(w_ctr_priorSel_erCnt)
    );


    /* error count info loader */
    /***** wire *****/
//    wire    [1 - 1 : 0]                             w_in_ctr_sig_infoL_erCnt ;
    wire    [PARAM_HD_CORRECTABLE_CNT_LEN - 1 : 0]  w_in_infoL_erCnt ;
    wire    [PARAM_HD_CORRECTABLE_CNT_LEN - 1 : 0]  w_out_infoL_erCnt ;
/************************************************************/

    assign w_in_infoL_erCnt = w_out_priorSel_erCnt ;

    ctr_infoLoaders_independent
    #(
        .OUTTER_NAME("sd_bch_dec"),
        .MODULE_NAME("uut_x_loaderErrCnt"),

        .IN_BUF_LEN(0),
        .OUT_BUF_LEN(1),

        .IND_DISTANCE_INFO(0),
        .CLK_DISTANCE_INFO(0),

        .INPUT_BUS_EN(0),
        .OUTPUT_BUS_EN(0),

        .PASSING_LAST_EN(0),
        .OUT_SIG_EN(0),

        .INFO_NUMS(8),
        .BIT_WIDTH_INFO(PARAM_HD_CORRECTABLE_CNT_LEN),

        .BYPASS_INFO_EN(1)
    )
    uut_x_loaderErrCnt
    (
        .clk(clk),
        .in_ctr_Srst(ctr_Srst),
        .in_ctr_en(ctr_en_wire),

        .in_ctr_sig(),
        .in_info(w_in_infoL_erCnt),
        .out_info(w_out_infoL_erCnt)
    );
    /************************************************************/
    /* degree Loader block start */
    /***** wire *****/
    localparam  LO_PA_STT_LOAD_DEG_CTR_WIDTH    =   5;
    localparam  LO_PA_STT_LOAD_DEG_DISTANCE     =   3;

    wire    [LO_PA_STT_LOAD_DEG_CTR_WIDTH - 1 : 0]  w_in_ctr_sig_infoL_deg ;
    wire    [PARAM_HD_CORRECTABLE_CNT_LEN - 1 : 0]  w_in_infoL_deg ;
    wire    [PARAM_HD_CORRECTABLE_CNT_LEN - 1 : 0]  w_out_infoL_deg ;

    /************************************************************/
    assign  w_in_ctr_sig_infoL_deg = { 
            ctr_bus_ctr3_blk_init[0] ,// initial last infoloader <- done signal of  decision controller.
            ctr_ctr1_done ,// initial middle #0 infoloader. <- done signal of mSBS.
            ctr_ctr1_done ,// passing last value from first loader to middle #0 loader. <- done signal of mSBS.
            ctr_bus_ctr3_blk_en[0] ,// Enabled output of sequential loader. <- enable signal of decision controller.
            ctr_bus_ctr1_blk_en[0] // Enable input of sequential loader. <- enable signal of mSBS.
        };

    assign  w_in_infoL_deg =  

        {
            w_mSBS_2_tp_deg2[0],
            w_mSBS_2_tp_deg3[0]
        } ; //DECISION_MODE 3

    /************************************************************/
    ctr_infoLoaders_independent
    #(
        .OUTTER_NAME("sd_bch_dec"),
        .MODULE_NAME("uut_x_loaderDeg"),

        .IN_BUF_LEN(0),
        .OUT_BUF_LEN(0),

        .IND_DISTANCE_INFO(LO_PA_STT_LOAD_DEG_DISTANCE),
        .CLK_DISTANCE_INFO(1),

        .INPUT_BUS_EN(0),
        .OUTPUT_BUS_EN(0),

        .PASSING_LAST_EN(1),
        .OUT_SIG_EN(0),

        .INFO_NUMS(TEST_SYND_NUM),
        .BIT_WIDTH_INFO(PARAM_HD_CORRECTABLE_CNT_LEN),

        .BYPASS_INFO_EN(0)
    )
    uut_x_loaderDeg
    (
        .clk(clk),
        .in_ctr_Srst(ctr_Srst),
        .in_ctr_en(ctr_en_wire),

        .in_ctr_sig(w_in_ctr_sig_infoL_deg),
        .in_info(w_in_infoL_deg),
        .out_info(w_out_infoL_deg)
    );
    /************************************************************/
    /* alpha Loader block start */
    /***** wire *****/
    //
    localparam  LO_PA_STT_LOAD_ALPHA_CTR_WIDTH  =   3;
    localparam  LO_PA_STT_LODD_ALPHA_DISTANCE   =   3;
    wire    [LO_PA_STT_LOAD_ALPHA_CTR_WIDTH - 1 : 0]             
                                    w_in_ctr_sig_infoL_alpha ;
    wire    [GF_LEN * 3 - 1 : 0]    w_in_infoL_alpha ;
    wire    [GF_LEN * 3 - 1 : 0]    w_out_infoL_alpha ;

    assign  w_in_infoL_alpha = {
            w_bus_out_TSC[GF_LEN * (0 + 1 + 2) - 1 : GF_LEN * (0 + 0 + 2)], // w_TSC_alpha3_p1 , 
            w_bus_out_TSC[GF_LEN * (0 + 1 + 1) - 1 : GF_LEN * (0 + 0 + 1)], // w_TSC_alpha2_p1 , 
            w_bus_out_TSC[GF_LEN * (0 + 1 + 0) - 1 : GF_LEN * (0 + 0 + 0)] // w_TSC_alpha1_p1 
        };

    /************************************************************/
//    assign  w_in_ctr_sig_infoL_alpha = {
//        ctr_ctr3_done, // <- done signal of DC.
//        ctr_ctr2_done, // <- done signal of pre-CS and MC.
//        ctr_bus_ctr1_blk_done , // <- done signal of mSBS.
////        ctr_ctr0_done // <- done signal of TSC.
//        };

    assign  w_in_ctr_sig_infoL_alpha = {
        ctr_ctr3_done ,
        ctr_bus_ctr2_blk_done[0],
        ctr_ctr1_done
        };
    /************************************************************/
    ctr_infoLoaders_independent
    #(
        .OUTTER_NAME("sd_bch_dec"),
        .MODULE_NAME("uut_x_loaderAlpha"),

        .IN_BUF_LEN(0),
        .OUT_BUF_LEN(0),

        .IND_DISTANCE_INFO(LO_PA_STT_LODD_ALPHA_DISTANCE),
        .CLK_DISTANCE_INFO(1),

        .INPUT_BUS_EN(1),
        .OUTPUT_BUS_EN(1),

        .PASSING_LAST_EN(0),
        .OUT_SIG_EN(0),

        .INFO_NUMS(SD_CORRECTABLE),
        .BIT_WIDTH_INFO(GF_LEN),

        .BYPASS_INFO_EN(0)
    )
    uut_x_loaderAlpha
    (
        .clk(clk),
        .in_ctr_Srst(ctr_Srst),
        .in_ctr_en(ctr_en_wire),

        .in_ctr_sig(w_in_ctr_sig_infoL_alpha),
        .in_info(w_in_infoL_alpha),
        .out_info(w_out_infoL_alpha)
    );
    /************************************************************/


    /* Info Loader block end */
    /************************************************************/
    /************************************************************/
    /************************************************************/
    wire    ctr_TSCS_init;
    wire    ctr_TSCS_done;
    wire    ctr_TSCS_en;

    assign  ctr_TSCS_init = ctr_bus_ctr4_blk_init[0];
    assign  ctr_TSCS_done = ctr_bus_ctr4_blk_done[0];
    assign  ctr_TSCS_en = ctr_bus_ctr4_blk_en[0];
    /************************************************************/
    wire    [SD_CORRECTABLE * GF_LEN - 1 : 0]   w_in_TSCS_alpha ;

//    assign  w_in_TSCS_alpha = {w_CS_o_alpha3, w_CS_o_alpha2,
//    w_CS_o_alpha1};/DECISION_MODE 1, 2
assign  w_in_TSCS_alpha = w_out_infoL_alpha ; // DECISION_MODE 3
        
    /************************************************************/
    wire    [SD_CORRECTABLE * LO_PARAM_PARALLELISM - 1 : 0]            w_out_TSCS_alpha_loc;
    /************************************************************/

    /************************************************************/
    testSynd_chienSearch
    #(
        .OUTTER_NAME("sd_bch_dec"),
        .MODULE_NAME("uut_4_TSCS"),
        
        .GF_LEN(GF_LEN),
        .PARALLELISM(LO_PARAM_PARALLELISM),
        .ALPHA_NUM(SD_CORRECTABLE)
    )
    uut_4_TSCS
    (
        .clk(clk),

        .in_ctr_Srst(ctr_Srst),

        .in_ctr_init(ctr_TSCS_init),
        .in_ctr_done(ctr_TSCS_done),
        .in_ctr_en(ctr_en_wire),

        .in_ctr_TSCS_en(ctr_TSCS_en),
        .in_alpha(w_in_TSCS_alpha),
        
        .out_alpha_loc(w_out_TSCS_alpha_loc)
    );
    /************************************************************/
    /************************************************************/
    /************************************************************/
    /************************************************************/
    /************************************************************/
    wire    [1 - 1 : 0] ctr_DC_ctr_sig ; //DECISION_MODE 3 -> width is 6
    /************************************************************/
    assign  ctr_DC_ctr_sig = 
        {
            ctr_bus_ctr3_blk_en[0] //indicator sequential to simultaneous en(8clk) -> DU en sig
        };
    /************************************************************/
    /************************************************************/
    wire    [2 - 1 : 0] w_in_DC_tp_deg ; //DECISION_MODE 3
    /************************************************************/
    assign  w_in_DC_tp_deg = w_out_infoL_deg ; //DECISION_MODE 3
    /************************************************************/
    /************************************************************/
    wire    [PARAM_SD_CORRECTABLE_CNT_LEN - 1 : 0]
                                                    w_in_DC_tp_errCnt ;
    
    assign w_in_DC_tp_errCnt = w_out_infoL_erCnt ;

    /************************************************************/
//    wire    [TEST_SYND_NUM - 1 : 0] w_out_DC_tp_sel; // NOT using valid signal
    wire    [TEST_SYND_NUM + 1 - 1 : 0] w_out_DC_tp_sel ; // Using valid signal
    /************************************************************/
    decision_controller_2
    #(
        .MODULE_NAME("uut_3_DC"),

        .IN_BUF_LEN(0),
        .OUT_BUF_LEN(0),

        .DECISION_MODE(DECISION_MODE),

        .DECISION_NUMS(TEST_SYND_NUM)
    )
    uut_3_DC
    (
        .clk(clk) ,
        .in_ctr_Srst(ctr_Srst) ,
        .in_ctr_en(ctr_en_wire) ,

        .in_ctr_sig(ctr_DC_ctr_sig) ,

        .in_tp_deg(w_in_DC_tp_deg) ,
        .in_tp_erCnt(w_in_DC_tp_errCnt) ,

        .out_tp_sel(w_out_DC_tp_sel) 
    );
    /************************************************************/
    /************************************************************/
    /************************************************************/
    /************************************************************/
    /************************************************************/

/**************************************************/
// MODULE_NAME : uut_3_4_DU_reCS, DESIGN_NAME : decision_unit_3
// infomation about in_ctr_sig
// Valid data sig is  enabled
// valid data buf enable signal is  enabled
// Common sel signal is disabled
// reCS blk is  enabled
// input [         20 - 1 :           0] in_ctr_sig
/**************************************************/

    wire    [TEST_SYND_NUM + 2 + TEST_SYND_NUM + 2 - 1 : 0] ctr_DU_ctr_sig;
    assign  ctr_DU_ctr_sig = {
        // reCS init            : in_ctr_sig[         19]
        ctr_bus_ctr4_blk_init[0] , // re CS init signal. The last-th signal of pre CS done, can be it.
        // reCS en              : in_ctr_sig[         18]
        ctr_bus_ctr4_blk_en[0] , // re CS enable signal. it is come from ctr 4.
        // tp sel               : in_ctr_sig[         17 :          10]]
        w_out_DC_tp_sel[TEST_SYND_NUM - 1 : 0] ,  // SD_errLoc select siganl. It is come from decision controller
        // validDataSelBufEnSig : in_ctr_sig[          9]
        ctr_bus_ctr3_blk_en[0] , // to get valid data, buffer enable signal. ctr is generate enable signal. 
        // valid data           : in_ctr_sig[          8]
        w_out_DC_tp_sel[TEST_SYND_NUM - 1 + 1] , // valid data input signal. It is come decision controller.
        // data sel             : in_ctr_sig[          7 :           0]
        ctr_bus_ctr2_blk_done // prior data sel signal. It can be preCS and MC done signal.
        };

//    wire    ctr_DU_en;
//    wire    w_DU_init;
//    wire    w_DU_done;

//    assign  ctr_DU_en = ctr_en_wire;
//    assign  w_DU_init = ctr_bus_ctr3_blk_init[0];
//    assign  w_DU_done = ctr_bus_ctr3_blk_done[0];

    /************************************************************/
    wire    [TEST_SYND_NUM - 1 : 0]                             w_DU_tp_deg3;
    wire    [TEST_SYND_NUM - 1 : 0]                             w_DU_tp_deg2;

//    assign  w_DU_tp_deg3 = w_CS_o_tp_deg3;//DECISION_MODE 1, 2
//    assign  w_DU_tp_deg2 = w_CS_o_tp_deg2;//DECISION_MODE 1, 2

    /************************************************************/
    wire    [SD_CORRECTABLE * LO_PARAM_PARALLELISM - 1 : 0]                   
                                                w_in_DU_alpha_loc;

    assign  w_in_DU_alpha_loc = w_out_TSCS_alpha_loc ;
    /************************************************************/
    wire    [TEST_SYND_NUM * LO_PARAM_PARALLELISM - 1 : 0]                              
                                                w_in_DU_tp_HD_errLoc;
    assign  w_in_DU_tp_HD_errLoc = w_out_CS_equal ;
//    wire    [GF_LEN * TEST_SYND_NUM - 1 : 0]    w_DU_tp_coef0;
//    wire    [GF_LEN * TEST_SYND_NUM - 1 : 0]    w_DU_tp_coef1;
//    wire    [GF_LEN * TEST_SYND_NUM - 1 : 0]    w_DU_tp_coef2;
//    wire    [GF_LEN * TEST_SYND_NUM - 1 : 0]    w_DU_tp_coef3;

//    wire    [GF_LEN * TEST_SYND_NUM * PARAM_ERR_EQ_COEF_NUMS - 1 : 0]   w_out_CS_done_coef;
    wire    [GF_LEN * TEST_SYND_NUM * PARAM_ERR_EQ_COEF_NUMS - 1 : 0]   w_in_DU_tp_coef;

//    assign  w_DU_tp_coef0 = w_CS_doneCoef0;
//    assign  w_DU_tp_coef1 = w_CS_doneCoef1;
//    assign  w_DU_tp_coef2 = w_CS_doneCoef2;
//    assign  w_DU_tp_coef3 = w_CS_doneCoef3;
    assign w_in_DU_tp_coef = w_out_CS_done_coef;

    /************************************************************/
    wire    [PARAM_SD_CORRECTABLE_CNT_LEN - 1 : 0]                  w_DU_sel_tp_num;
    wire    [LO_PARAM_PARALLELISM - 1 : 0]                                   w_out_DU_sel_tp_errLoc;
    /************************************************************/

    decision_unit_3
    #(
        .OUTTER_NAME("sd_bch_dec"),
        .MODULE_NAME("uut_3_4_DU_reCS"),

        .GF_LEN(GF_LEN),

        .PARALLELISM(LO_PARAM_PARALLELISM),

        .CODEWORD_LEN(CW_LEN),

        .HD_CORRECTABLE(HD_CORRECTABLE),
        .SD_CORRECTABLE(SD_CORRECTABLE),

        .ERR_CNT_LEN(PARAM_SD_CORRECTABLE_CNT_LEN),

        .DECISION_MODE(DECISION_MODE),

        .FOR_RE_CS_PRIOR_SELED_BUF_EN(1)
    )
    uut_3_4_DU_reCS
    (
        .clk(clk),

        .in_ctr_Srst(ctr_Srst),
        .in_ctr_en(ctr_en_wire),

        .in_ctr_sig(ctr_DU_ctr_sig) ,

        .in_tp_data(w_in_DU_tp_coef) , // or w_in_DU_tp_HD_errLoc
        .in_alpha_loc(w_in_DU_alpha_loc) ,

        .out_sel_tp_errLoc(w_out_DU_sel_tp_errLoc)
    );

    /*Error correction */
    wire    [LO_PARAM_PARALLELISM - 1 : 0]   w_corrected;

    assign  w_corrected = w_out_FIFO_hd ^ w_out_DU_sel_tp_errLoc;
    /* Corrected Codword output */

    generate
        if(OUTPUT_BUF == 0) begin : sdbchdecNoOutBuf
            assign  out_out_start   = ctr_ctr4_init ;
            assign  out_dec_done    = ctr_ctr4_done ;
            assign  out_codeword    = w_out_FIFO_hd ;
            assign  out_sd_equal    = w_out_DU_sel_tp_errLoc ;
            assign  out_corrected   = w_corrected ;
        end
        else begin : sdbchdecHasOutBuf
            fifo_multi_ch
            #(.FIFO_LEN(OUTPUT_BUF), .FIFO_CH_WIDTH(2 + LO_PARAM_PARALLELISM * 3))
            uut_out_buf_FIFO
            (
                .clk(clk),

                .in_ctr_Srst(ctr_Srst),
                .in_ctr_en(ctr_en_wire),

                .in({ctr_ctr4_init, ctr_ctr4_done, w_out_DU_sel_tp_errLoc, w_out_FIFO_hd, w_corrected}),
                .out({out_out_start, out_dec_done, out_sd_equal, out_codeword, out_corrected})
            );
        end
    endgenerate





    initial begin
        $display("/**************************************************/");
        $display("<<< %s >>>", DESIGN_NAME);
        $display("GF_LEN = %d", GF_LEN);
        $display("LLR_LEN = %d", LLR_LEN);
        $display("LLR_MAG_LEN = %d", LLR_MAG_LEN);

        $display("PARALLELISM = %d", PARALLELISM);
        $display("LO_PARAM_PARALLELISM = %d", LO_PARAM_PARALLELISM);

        $display("CW_LEN = %d", CW_LEN);

        $display("HD_CORRECTABLE = %d", HD_CORRECTABLE);
        $display("PARAM_HD_CORRECTABLE_CNT_LEN = %d", PARAM_HD_CORRECTABLE_CNT_LEN);

        $display("SD_CORRECTABLE = %d", SD_CORRECTABLE);
        $display("PARAM_SD_CORRECTABLE_CNT_LEN = %d", PARAM_SD_CORRECTABLE_CNT_LEN);

        $display("PARAM_ERR_EQ_COEF_NUMS = %d", PARAM_ERR_EQ_COEF_NUMS);

        $display("TEST_SYND_NUM = %d", TEST_SYND_NUM);

        $display("DECISION_MODE = %d", DECISION_MODE);

        $display("OUTPUT_BUF = %d", OUTPUT_BUF);

        $display("HD_SYND_COM_OP_CLK_CYCLE = %d", HD_SYND_COM_OP_CLK_CYCLE);
        $display("HD_SYND_COM_OP_ENB_CYCLE = %d", HD_SYND_COM_OP_ENB_CYCLE);

        $display("GF_FORWARD_SH_VAL = %d", GF_FORWARD_SH_VAL);
        $display("GF_BACKWARD_SH_VAL = %d", GF_BACKWARD_SH_VAL);

        $display("HD_SYND_COMP_INPUT_BUF = %d", HD_SYND_COMP_INPUT_BUF);

        $display("HD_SYND1_BIT_WEIGHT = %d", HD_SYND1_BIT_WEIGHT);
        $display("HD_SYND1_BIT_SHIFT = %d", HD_SYND1_BIT_SHIFT);

        $display("HD_SYND3_BIT_WEIGHT = %d", HD_SYND3_BIT_WEIGHT);
        $display("HD_SYND3_BIT_SHIFT = %d", HD_SYND3_BIT_SHIFT);

        $display("HD_SYND5_BIT_WEIGHT = %d", HD_SYND5_BIT_WEIGHT);
        $display("HD_SYND5_BIT_SHIFT = %d", HD_SYND5_BIT_SHIFT);
        
        $display("LLR_TO_MAG_OP_CLK_CYCLE = %d", LLR_TO_MAG_OP_CLK_CYCLE);
        $display("LLR_TO_MAG_OP_ENB_CYCLE = %d", LLR_TO_MAG_OP_ENB_CYCLE);

        $display("RE_CHIEN_SYND1_BIT_SHIFT = %d", RE_CHIEN_SYND1_BIT_SHIFT);
        $display("RE_CHIEN_SYND3_BIT_SHIFT = %d", RE_CHIEN_SYND3_BIT_SHIFT);
        $display("RE_CHIEN_SYND5_BIT_SHIFT = %d", RE_CHIEN_SYND5_BIT_SHIFT);

        $display("DEBUG_MODE = %d", DEBUG_MODE);
        $display("/**************************************************/");
    end


endmodule

`endif
