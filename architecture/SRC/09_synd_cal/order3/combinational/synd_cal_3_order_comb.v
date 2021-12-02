`ifndef SYND_CALCULATOR_3_ORDER_COMB
`define SYND_CALCULATOR_3_ORDER_COMB

`include "00_GF_mult/gf_10/comb/GF_Mult_2_10_comb.v"

module synd_cal_3_order_comb
(
    clk,

    in_ctr_Srst,
    in_ctr_en,

    in_ctr_init,

    in_alpha,

    out_synd1_TP,
    out_synd3_TP,
    out_synd5_TP
);

    input   clk;

    input   in_ctr_Srst;
    input   in_ctr_en;

    input   in_ctr_init;

    input   [9:0]   in_alpha;

    output  [9:0]   out_synd1_TP;
    output  [9:0]   out_synd3_TP;
    output  [9:0]   out_synd5_TP;

    
    wire    [9:0]   w_pow2;
    wire    [9:0]   w_pow3;
    wire    [9:0]   w_pow5;

    reg     [9:0]   buf_synd[2:0];

    GF_Mult_2_10_comb    squaring
    (
        .out(w_pow2),
        .a(in_alpha),
        .b(in_alpha)
    );


    GF_Mult_2_10_comb    pow1ProductPow2
    (
        .out(w_pow3),
        .a(w_pow2),
        .b(in_alpha)
    );


    GF_Mult_2_10_comb    pow2ProductPow3
    (
        .out(w_pow5),
        .a(w_pow3),
        .b(w_pow2)
    );
    //synd register functions
    always@(*)  buf_synd[0] <= in_alpha;

    always@(*)  buf_synd[1] <= w_pow3;

    always@(*)  buf_synd[2] <= w_pow5;

    assign  out_synd1_TP = buf_synd[0];
    assign  out_synd3_TP = buf_synd[1];
    assign  out_synd5_TP = buf_synd[2];


endmodule
`endif
