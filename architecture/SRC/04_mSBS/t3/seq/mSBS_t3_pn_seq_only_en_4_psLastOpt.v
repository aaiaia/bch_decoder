`ifndef M_SBS_HARD_T3_PN_SEQ_ONLY_EN_4
`define M_SBS_HARD_T3_PN_SEQ_ONLY_EN_4

`include "00_GF_mult/gf_10/comb/GF_Mult_2_10_comb.v"
`include "60_fifo_channel/fifo_multi_ch.v"
//`include "61_shiftReg/shiftReg.v"
//`include "61_shiftReg/sequentialToSimultaneousReg_psLast.v"
`include "61_shiftReg/sequentialToSimultaneousReg_psLastOpt.v"
module mSBS_t3_pn_seq_only_en_4
(
    clk,
    in_ctr_Srst,
    in_ctr_en,

    in_ctr_sig,
    
    in_synd1,
    in_synd3,
    in_synd5,

//    out_tp_A,
//    out_tp_B,
//    out_tp_C,
//    out_tp_R,
    out_coef,

    out_tp_deg2,
    out_tp_deg3
);
/*****************************************************/
/*****************************************************/
    function    integer outBusEn;
        input   integer decision_mode;

        begin
            if(decision_mode == 1)      outBusEn = 1;
            else if(decision_mode == 2) outBusEn = 1;
            else if(decision_mode == 3) outBusEn = 0;
            else    $display("in mSBS_t3_pn_seq_only_en_4, decision_mode is a wrong case"); 
        end
    endfunction
/*****************************************************/
    function    integer outBufEn;
        input   integer decision_mode;

        begin
            if(decision_mode == 1)      outBufEn = 1;
            else if(decision_mode == 2) outBufEn = 1; 
            else if(decision_mode == 3) outBufEn = 0;
            else    $display("in mSBS_t3_pn_seq_only_en_4, decision_mode is a wrong case"); 
        end
    endfunction
/*****************************************************/
    function    integer val2Bool;
        input   integer val;
        begin
            if(val) val2Bool = 1;
            else    val2Bool = 0;
        end
    endfunction
/*****************************************************/
    function    integer val2NotBool;
        input   integer val;
        begin
            if(val) val2NotBool = 0;
            else    val2NotBool = 1;
        end
    endfunction
/*****************************************************/
    function integer indexCal;
        input integer interVal;
        input integer initVal;

        begin
            indexCal = initVal + interVal;
        end
    endfunction
/**********************************************************/
/*****************************************************/
    parameter   OUTTER_NAME = "";
    parameter   MODULE_NAME = "mSBS_t3_pn_seq_only_en_4";
        localparam  DESIGN_NAME = "mSBS_t3_pn_seq_only_en_4";

    parameter   DECISION_MODE   =   1;

    parameter   GF_LEN  =   10;
    parameter   TEST_PATTEN_NUMS  =   8;

    parameter   EQUATION_COEF_NUMS = 4; //static

        localparam  LO_PARAM_OUT_BUS_EN =   outBusEn(DECISION_MODE);
        localparam  LO_PARAM_OUTPUT_WIDTH_COEF  =   
            ((TEST_PATTEN_NUMS - 1) * LO_PARAM_OUT_BUS_EN + 1) * (GF_LEN * EQUATION_COEF_NUMS);
        localparam  LO_PARAM_OUTPUT_WIDTH_DEG2  =   
            ((TEST_PATTEN_NUMS - 1) * LO_PARAM_OUT_BUS_EN + 1) * 1;
        localparam  LO_PARAM_OUTPUT_WIDTH_DEG3  =   
            ((TEST_PATTEN_NUMS - 1) * LO_PARAM_OUT_BUS_EN + 1) * 1;

    /* 0 is forward direction, -1 is reverse direction */
    localparam  LO_PARAM_OUTPUT_BUF_EN  =   outBufEn(DECISION_MODE);
    localparam  LO_PARAM_OUTPUT_BUF_DIS =   val2NotBool(LO_PARAM_OUTPUT_BUF_EN); 

    parameter   OUT_BUF_SHIFING_DIRECTION = 1;

    parameter   PASSINS_LAST_EN =   0;
        localparam  LO_PARAM_PASSING_LAST_EN    =   val2Bool(PASSINS_LAST_EN);

    localparam  LO_IDX_CTR_SIG_MSBS_START = -1;
        localparam  LO_INDEX_CTR_SH_EN = LO_IDX_CTR_SIG_MSBS_START + 
            LO_PARAM_OUTPUT_BUF_EN * 1;
        localparam  LO_INDEX_CTR_PASS_LAST = 
            indexCal(LO_INDEX_CTR_SH_EN, 
            LO_PARAM_OUTPUT_BUF_EN * LO_INDEX_CTR_SH_EN);
    localparam  LO_IDX_CTR_SIG_MSBS_END = LO_INDEX_CTR_PASS_LAST;

        localparam  LO_INDEX_CTR_WIDTH = LO_IDX_CTR_SIG_MSBS_END + 1;

/*****************************************************/

    input                                               clk;
    input                                               in_ctr_Srst;
    input                                               in_ctr_en;

    input        [LO_INDEX_CTR_WIDTH - 1 : 0]           in_ctr_sig;

    input       [GF_LEN - 1 : 0]                        in_synd1;
    input       [GF_LEN - 1 : 0]                        in_synd3;
    input       [GF_LEN - 1 : 0]                        in_synd5;

    output      [LO_PARAM_OUTPUT_WIDTH_COEF - 1 : 0]    out_coef;

    output      [LO_PARAM_OUTPUT_WIDTH_DEG2 - 1 : 0]    out_tp_deg2;
    output      [LO_PARAM_OUTPUT_WIDTH_DEG3 - 1 : 0]    out_tp_deg3;

    wire                                                w_deg2_R;
    wire                                                w_deg3_R;

    wire        [GF_LEN - 1 : 0]                        wire_deg2_A;
    wire        [GF_LEN - 1 : 0]                        wire_deg2_B;
    wire        [GF_LEN - 1 : 0]                        wire_deg2_C;
    wire        [GF_LEN - 1 : 0]                        wire_deg2_R;

    wire        [GF_LEN - 1 : 0]                        wire_deg3_A;
    wire        [GF_LEN - 1 : 0]                        wire_deg3_B;
    wire        [GF_LEN - 1 : 0]                        wire_deg3_C;
    wire        [GF_LEN - 1 : 0]                        wire_deg3_R;

    wire        [GF_LEN - 1 : 0]                        wire_coef_A;
    wire        [GF_LEN - 1 : 0]                        wire_coef_B;
    wire        [GF_LEN - 1 : 0]                        wire_coef_C;
    wire        [GF_LEN - 1 : 0]                        wire_coef_R;

    wire        [GF_LEN - 1 : 0]                        synd1p2;
    wire        [GF_LEN - 1 : 0]                        synd1p3;

    wire        [GF_LEN - 1 : 0]                        synd1p2_synd3;

    wire        [GF_LEN - 1 : 0]                        Cp2;
    wire        [GF_LEN - 1 : 0]                        synd1_A;

    assign  w_deg2_R = (|wire_deg3_C);
    assign  w_deg3_R = (|wire_deg3_R);

    assign  wire_deg2_A = synd1p2;
    assign  wire_deg2_B = in_synd1;
    assign  wire_deg2_C = {(GF_LEN){1'b0}};
    assign  wire_deg2_R = wire_deg3_C;

    /* base start */
    GF_Mult_2_10_comb synd1_pow_2
    (
        .out(synd1p2),
        .a(in_synd1),
        .b(in_synd1)
    );

    GF_Mult_2_10_comb synd1_pow_3
    (
        .out(synd1p3),
        .a(synd1p2),
        .b(in_synd1)
    );
    /* base end */


    /* coeffi A start */
    GF_Mult_2_10_comb mul_synd1p2_synd3
    (
        .out(synd1p2_synd3),
        .a(synd1p2),
        .b(in_synd3)
    );

    assign wire_deg3_A = synd1p2_synd3 ^ in_synd5;
    /* coeffi A end */


    /* coeffi C start */
    assign wire_deg3_C = synd1p3 ^ in_synd3;
    /* coeffi C end */


    /* coeffi B start */
    GF_Mult_2_10_comb mul_C_synd1
    (
        .out(wire_deg3_B),
        .a(wire_deg3_C),
        .b(in_synd1)
    );
    /* coeffi B end */


    /* coeffi R start */
    /* coeffi R1*/
    GF_Mult_2_10_comb mul_C_pow2
    (
        .out(Cp2),
        .a(wire_deg3_C),
        .b(wire_deg3_C)
    );

    /* coeffi R2 */
    GF_Mult_2_10_comb mul_synd1_A
    (
        .out(synd1_A),
        .a(wire_deg3_A),
        .b(in_synd1)
    );

    assign wire_deg3_R = Cp2 ^ synd1_A;
    /* coeffi R end */

    assign  wire_coef_A = (w_deg2_R ? wire_deg3_A : wire_deg2_A);
    assign  wire_coef_B = (w_deg2_R ? wire_deg3_B : wire_deg2_B);
    assign  wire_coef_C = (w_deg2_R ? wire_deg3_C : wire_deg2_C);
    assign  wire_coef_R = (w_deg2_R ? wire_deg3_R : wire_deg2_R);

    generate
        if(LO_PARAM_OUTPUT_BUF_EN) begin : gen_outBuf
            /*save coefficient of polynomials*/
            sequentialToSimultaneousReg_psLastOpt
            #(
                .OUTTER_MODULE(DESIGN_NAME),
                .MODULE_NAME("uut_seq2Sim_coef"),

                .DIRECTION(OUT_BUF_SHIFING_DIRECTION),
                .SHIFT_LEN(TEST_PATTEN_NUMS),
                .BIT_WIDTH(GF_LEN * EQUATION_COEF_NUMS),

                .PASSING_LAST(LO_PARAM_PASSING_LAST_EN)
            )
            uut_seq2Sim_coef
            (
                .clk(clk),

                .in_ctr_Srst(in_ctr_Srst),
                .in_ctr_en(in_ctr_en),
                .in_ctr_sig(in_ctr_sig),

                .in({wire_coef_C, wire_coef_B, wire_coef_A, wire_coef_R}),
                .out(out_coef)
            );

            /*save degree infomation*/
            sequentialToSimultaneousReg_psLastOpt
            #(
                .OUTTER_MODULE(DESIGN_NAME),
                .MODULE_NAME("uut_seq2Sim_deg"),

                .DIRECTION(OUT_BUF_SHIFING_DIRECTION),
                .SHIFT_LEN(TEST_PATTEN_NUMS),
                .BIT_WIDTH(2),

                .PASSING_LAST(LO_PARAM_PASSING_LAST_EN)
            )
            uut_seq2Sim_deg
            (
                .clk(clk),

                .in_ctr_Srst(in_ctr_Srst),
                .in_ctr_en(in_ctr_en),
                .in_ctr_sig(in_ctr_sig),

                .in({w_deg3_R, w_deg2_R}),
                .out({out_tp_deg3, out_tp_deg2})
            );
        end
        else begin : gen_outWiring
            assign  out_coef    = {wire_coef_C, wire_coef_B, wire_coef_A, wire_coef_R};

            assign  out_tp_deg3 = w_deg3_R;
            assign  out_tp_deg2 = w_deg2_R;
        end
    endgenerate

/*****************************************************/
    initial begin
        $display("/**************************************************/");
        $display("Instanciating %s", DESIGN_NAME);

        $display("In %s," ,OUTTER_NAME);
        $display("MODULE_NAME : %s", MODULE_NAME);
        $display("DESIGN_NAME : %s", DESIGN_NAME);

        $display("DECISION_MODE = %d", DECISION_MODE);
        $display("GF_LEN = %d", GF_LEN);
        $display("TEST_PATTEN_NUMS = %d", TEST_PATTEN_NUMS);
        $display("EQUATION_COEF_NUMS = %d", EQUATION_COEF_NUMS);

        $display("LO_PARAM_OUT_BUS_EN : %s", LO_PARAM_OUT_BUS_EN ? "enabled" : "disabled");
        $display("LO_PARAM_OUTPUT_WIDTH_COEF = %d", LO_PARAM_OUTPUT_WIDTH_COEF);
        $display("LO_PARAM_OUTPUT_WIDTH_DEG2 = %d", LO_PARAM_OUTPUT_WIDTH_DEG2);
        $display("LO_PARAM_OUTPUT_WIDTH_DEG3 = %d", LO_PARAM_OUTPUT_WIDTH_DEG3);

        $display("LO_PARAM_OUTPUT_BUF_EN : %s", LO_PARAM_OUTPUT_BUF_EN ? "enabled" : "disabled");
        $display("LO_PARAM_OUTPUT_BUF_DIS : %s", LO_PARAM_OUTPUT_BUF_DIS ? "enabled" : "disabled");
        $display("OUT_BUF_SHIFING_DIRECTION = %d (%s)", OUT_BUF_SHIFING_DIRECTION,
            (0 < OUT_BUF_SHIFING_DIRECTION ? "forward" : "reverse"));

        $display("PASSINS_LAST_EN = %d", PASSINS_LAST_EN);
        $display("LO_PARAM_PASSING_LAST_EN : %s", LO_PARAM_PASSING_LAST_EN ? "enabled" : "disabled");

        $display("=====> About. in_ctr_sig");
        $display("LO_IDX_CTR_SIG_MSBS_START = %d", LO_IDX_CTR_SIG_MSBS_START);
        $display("LO_INDEX_CTR_SH_EN = %d", LO_INDEX_CTR_SH_EN);
        $display("LO_INDEX_CTR_PASS_LAST = %d", LO_INDEX_CTR_PASS_LAST);
        $display("LO_IDX_CTR_SIG_MSBS_END = %d", LO_IDX_CTR_SIG_MSBS_END);
        $display("LO_INDEX_CTR_WIDTH = %d", LO_INDEX_CTR_WIDTH);
        $display("/**************************************************/");
    end
/*****************************************************/
endmodule
`endif
