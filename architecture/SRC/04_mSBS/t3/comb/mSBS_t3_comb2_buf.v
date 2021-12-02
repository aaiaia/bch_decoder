`ifndef M_SBS_HARD_T3_COMB
`define M_SBS_HARD_T3_COMB

`include "00_GF_mult/gf_10/comb/GF_Mult_2_10_comb.v"

module mSBS_t3_comb2_buf
(
    clk,

    synd1,
    synd3,
    synd5,

    out_deg3_A,
    out_deg3_B,
    out_deg3_C,
    out_deg3_R
);
    parameter   OUTTER_NAME = "";
    parameter   MODULE_NAME = "mSBS_t3_comb2_buf";
        localparam  DESIGN_NAME = "mSBS_t3_comb2_buf";

    parameter           GF_LEN  =   10;

    parameter   SYN_TEST    =   0;
    
    initial begin
        $display("!!! in) %s !!!", OUTTER_NAME);
        $display("!!! %s !!!", MODULE_NAME);
        $display("!!! %s !!!", DESIGN_NAME);

        $display("GF_LEN=%d", GF_LEN);
        $display("SYN_TEST=%d", SYN_TEST);
    end

    input                       clk;

    input       [GF_LEN-1:0]    synd1;
    input       [GF_LEN-1:0]    synd3;
    input       [GF_LEN-1:0]    synd5;

    output        [GF_LEN-1:0]    out_deg3_A;
    output    [GF_LEN-1:0]    out_deg3_B;
    output         [GF_LEN-1:0]    out_deg3_C;
    output    [GF_LEN-1:0]    out_deg3_R;


    reg     [GF_LEN-1:0]    buf_deg3_A;
    reg     [GF_LEN-1:0]    buf_deg3_B;
    reg     [GF_LEN-1:0]    buf_deg3_C;
    reg     [GF_LEN-1:0]    buf_deg3_R;

wire        [GF_LEN-1:0]    s1s3;   
wire        [GF_LEN-1:0]    s1p4;
wire        [GF_LEN-1:0]    s1p4_;
wire        [GF_LEN-1:0]    s1s5;
wire        [GF_LEN-1:0]    s1p3;
wire        [GF_LEN-1:0]    s1p3_;
wire        [GF_LEN-1:0]    s1p2;
wire        [GF_LEN-1:0]    s1p3s3;
wire        [GF_LEN-1:0]    s1p2s3;
wire        [GF_LEN-1:0]    s1p3_s3;
wire        [GF_LEN-1:0]    s1p3_s3_p2;



    GF_Mult_2_10_comb uut_synd1synd3
    (
        .out(s1s3),
        .a(synd1),
        .b(synd3)
    );

    GF_Mult_2_10_comb uut_synd1pow4
    (
        .out(s1p4),
        .a(s1p4_),
        .b(s1p4_)
    );

    GF_Mult_2_10_comb uut_synd1pow4_
    (
        .out(s1p4_),
        .a(synd1),
        .b(synd1)
    );

    GF_Mult_2_10_comb uut_synd1synd5 
    (
        .out(s1s5),
        .a(synd1),
        .b(synd5)
    );

    GF_Mult_2_10_comb uut_synd1pow3
    (
        .out(s1p3),
        .a(s1p3_),
        .b(synd1)
    );


    GF_Mult_2_10_comb uut_synd1pow2_
    (
        .out(s1p3_),
        .a(synd1),
        .b(synd1)
    );
    GF_Mult_2_10_comb uut_synd1pow3synd3
    (
        .out(s1p3s3),
        .a(s1p3),
        .b(synd3)
    );
    GF_Mult_2_10_comb uut_synd1pow2synd3
    (
        .out(s1p2s3),
        .a(s1p2),
        .b(synd3)
    );
    assign s1p3_s3 = s1p3^synd3;

    GF_Mult_2_10_comb uut_synd1pow3plusynd3pow2
    (
        .out(s1p3_s3_p2),
        .a(s1p3_s3),
        .b(s1p3_s3)
    );
          
always@(posedge clk)buf_deg3_A <= s1p2s3^synd5;
always@(posedge clk)buf_deg3_B <= s1s3^s1p4;
always@(posedge clk)buf_deg3_C <= s1p3^synd3;
always@(posedge clk)buf_deg3_R <= s1s5^s1p3s3^s1p3_s3_p2;

                               
assign  out_deg3_A = buf_deg3_A;
assign  out_deg3_B = buf_deg3_B;
assign  out_deg3_C = buf_deg3_C;
assign  out_deg3_R = buf_deg3_R;



endmodule

`endif
