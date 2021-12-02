`ifndef M_SBS_HARD_T3_PN_SEQ_ONLY_EN_2
`define M_SBS_HARD_T3_PN_SEQ_ONLY_EN_2

`include "00_GF_mult/gf_10/comb/GF_Mult_2_10_comb.v"
`include "60_fifo_channel/fifo_multi_ch.v"
//`include "61_shiftReg/shiftReg.v"
`include "61_shiftReg/sequentialToSimultaneousReg.v"
`include "61_shiftReg/sequentialToSimultaneousReg_psLast.v"
module mSBS_t3_pn_seq_only_en_3
(
    clk,
    in_ctr_Srst,
    in_ctr_en,
    in_ctr_done,
    
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
    function    integer out_bus_en
        input   integer decision_mode;

        begin
            if(decision_mode == 1)      out_bus_en = 1;
            else if(decision_mode == 2) out_bus_en = 1;
            else if(decision_mode == 3) out_bus_en = 0;
            else    $display("in mSBS_t3_pn_seq_only_en_3, decision_mode is a wrong case"); 
        end
    endfunction
/*****************************************************/
/*****************************************************/
    parameter   OUTTER_NAME = "";
    parameter   MODULE_NAME = "mSBS_t3_pn_seq_only_en_3";
        localparam  DESIGN_NAME = "mSBS_t3_pn_seq_only_en_3";

    parameter   DECISION_MODE   =   1;

    parameter   GF_LEN  =   10;
    parameter   KEY_EQ_BUF  =   8;

    parameter   EQUATION_COEF_NUMS = 4; //static

        localparam  LO_PARAM_OUT_BUS_EN =   out_bus_en(DECISION_MODE);
        localparam  LO_PARAM_OUT_BIT_WIDTH  =   
            ((KEY_EQ_BUF - 1) * LO_PARAM_OUT_BUS_EN + 1) * GF_LEN;

    /* 0 is forward direction, -1 is reverse direction */
    parameter   OUT_BUF_SHIFING_DIRECTION = 1;

/*****************************************************/
    initial begin
        $display("!!! in) %s !!!", OUTTER_NAME);
        $display("!!! %s !!!", MODULE_NAME);
        $display("!!! %s !!!", DESIGN_NAME);
        $display("DECISION_MODE=%d", DECISION_MODE);
        $display("GF_LEN=%d", GF_LEN);
        $display("KEY_EQ_BUF=%d", KEY_EQ_BUF);
        $display("EQUATION_COEF_NUMS=%d", EQUATION_COEF_NUMS);
        $display("OUT_BUF_SHIFING_DIRECTION=%d", OUT_BUF_SHIFING_DIRECTION);
    end
/*****************************************************/

    input                       clk;
    input                       in_ctr_Srst;
    input                       in_ctr_en;
    input                       in_ctr_done;

    input       [GF_LEN - 1 : 0]    in_synd1;
    input       [GF_LEN - 1 : 0]    in_synd3;
    input       [GF_LEN - 1 : 0]    in_synd5;

//    output      [GF_LEN * KEY_EQ_BUF - 1 : 0]   out_tp_A;
//    output      [GF_LEN * KEY_EQ_BUF - 1 : 0]   out_tp_B;
//    output      [GF_LEN * KEY_EQ_BUF - 1 : 0]   out_tp_C;
//    output      [GF_LEN * KEY_EQ_BUF - 1 : 0]   out_tp_R;
    output      [GF_LEN * KEY_EQ_BUF * EQUATION_COEF_NUMS - 1 : 0]   out_coef;

    output      [KEY_EQ_BUF - 1 : 0]            out_tp_deg2;
    output      [KEY_EQ_BUF - 1 : 0]            out_tp_deg3;

    wire                            w_deg2_R;
    wire                            w_deg3_R;

    wire        [GF_LEN - 1 : 0]    wire_deg2_A;
    wire        [GF_LEN - 1 : 0]    wire_deg2_B;
    wire        [GF_LEN - 1 : 0]    wire_deg2_C;
    wire        [GF_LEN - 1 : 0]    wire_deg2_R;

    wire        [GF_LEN - 1 : 0]    wire_deg3_A;
    wire        [GF_LEN - 1 : 0]    wire_deg3_B;
    wire        [GF_LEN - 1 : 0]    wire_deg3_C;
    wire        [GF_LEN - 1 : 0]    wire_deg3_R;

    wire        [GF_LEN - 1 : 0]    wire_coef_A;
    wire        [GF_LEN - 1 : 0]    wire_coef_B;
    wire        [GF_LEN - 1 : 0]    wire_coef_C;
    wire        [GF_LEN - 1 : 0]    wire_coef_R;

    wire        [GF_LEN - 1 : 0]    synd1p2;
    wire        [GF_LEN - 1 : 0]    synd1p3;

    wire        [GF_LEN - 1 : 0]    synd1p2_synd3;

    wire        [GF_LEN - 1 : 0]    Cp2;
    wire        [GF_LEN - 1 : 0]    synd1_A;

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
        if(DECISION_MODE == 1) begin : gen_DM_1
            sequentialToSimultaneousRegister_psLast
            #(
                .OUTTER_MODULE(DESIGN_NAME),
                .MODULE_NAME("uut_seq2Sim_coef_DM_1"),

                .DIRECTION(OUT_BUF_SHIFING_DIRECTION),
                .SHIFT_LEN(KEY_EQ_BUF),
                .BIT_WIDTH(GF_LEN * EQUATION_COEF_NUMS)
            )
            uut_seq2Sim_coef_DM_1
            (
                .clk(clk),

                .in_ctr_Srst(in_ctr_Srst),
                .in_ctr_en(in_ctr_en),
                .in_ctr_last(in_ctr_done),

                .in({wire_coef_C, wire_coef_B, wire_coef_A, wire_coef_R}),
                .out(out_coef)
            );

            sequentialToSimultaneousRegister_psLast
            #(
                .OUTTER_MODULE(DESIGN_NAME),
                .MODULE_NAME("uut_seq2Sim_deg_DM_1"),

                .DIRECTION(OUT_BUF_SHIFING_DIRECTION),
                .SHIFT_LEN(KEY_EQ_BUF),
                .BIT_WIDTH(2)
            )
            uut_seq2Sim_deg_DM_1
            (
                .clk(clk),

                .in_ctr_Srst(in_ctr_Srst),
                .in_ctr_en(in_ctr_en),
                .in_ctr_last(in_ctr_done),

                .in({w_deg3_R, w_deg2_R}),
                .out({out_tp_deg3, out_tp_deg2})
            );
        end
        else if(DECISION_MODE == 2) begin : gen_DM_2
            sequentialToSimultaneousRegister_psLast
            #(
                .OUTTER_MODULE(DESIGN_NAME),
                .MODULE_NAME("uut_seq2Sim_coef_DM_2"),

                .DIRECTION(OUT_BUF_SHIFING_DIRECTION),
                .SHIFT_LEN(KEY_EQ_BUF),
                .BIT_WIDTH(GF_LEN * EQUATION_COEF_NUMS)
            )
            uut_seq2Sim_coef_DM_2
            (
                .clk(clk),

                .in_ctr_Srst(in_ctr_Srst),
                .in_ctr_en(in_ctr_en),
                .in_ctr_last(in_ctr_done),

                .in({wire_coef_C, wire_coef_B, wire_coef_A, wire_coef_R}),
                .out(out_coef)
            );

            sequentialToSimultaneousRegister_psLast
            #(
                .OUTTER_MODULE(DESIGN_NAME),
                .MODULE_NAME("uut_seq2Sim_deg_DM_2"),

                .DIRECTION(OUT_BUF_SHIFING_DIRECTION),
                .SHIFT_LEN(KEY_EQ_BUF),
                .BIT_WIDTH(2)
            )
            uut_seq2Sim_deg_DM_2
            (
                .clk(clk),

                .in_ctr_Srst(in_ctr_Srst),
                .in_ctr_en(in_ctr_en),
                .in_ctr_last(in_ctr_done),

                .in({w_deg3_R, w_deg2_R}),
                .out({out_tp_deg3, out_tp_deg2})
            );
        end
        else if(DECISION_MODE == 3) begin : gen_DM_3
            assign  out_coef    =   {wire_coef_C, wire_coef_B, wire_coef_A, wire_coef_R};

            assign  w_deg3_R    =   out_tp_deg3
            assign  w_deg2_R    =   out_tp_deg2
        end
        else begin : gen_DM_else
        end
    endgenerate


endmodule
`endif
