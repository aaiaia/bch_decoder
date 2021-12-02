`ifndef M_SBS_HARD_T3_COMB
`define M_SBS_HARD_T3_COMB

`include "00_GF_mult/gf_10/comb/GF_Mult_2_10_comb.v"

module mSBS_t3_comb
(
    clk,

    synd1,
    synd3,
    synd5,

    out_deg2_A,
    out_deg2_B,
    out_deg2_C,
    out_deg2_R,

    out_deg3_A,
    out_deg3_B,
    out_deg3_C,
    out_deg3_R
);
    parameter   OUTTER_NAME = "";
    parameter   MODULE_NAME = "mSBS_t3_comb";
        localparam  DESIGN_NAME = "mSBS_t3_comb";

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

    output      [GF_LEN-1:0]    out_deg2_A;
    output      [GF_LEN-1:0]    out_deg2_B;
    output      [GF_LEN-1:0]    out_deg2_C;
    output      [GF_LEN-1:0]    out_deg2_R;

    output      [GF_LEN-1:0]    out_deg3_A;
    output      [GF_LEN-1:0]    out_deg3_B;
    output      [GF_LEN-1:0]    out_deg3_C;
    output      [GF_LEN-1:0]    out_deg3_R;


    wire        [GF_LEN-1:0]    synd1p2;
    wire        [GF_LEN-1:0]    synd1p3;

    wire        [GF_LEN-1:0]    synd1p2_synd3;

    wire        [GF_LEN-1:0]    Cp2;
    wire        [GF_LEN-1:0]    synd1_A;

    wire        [GF_LEN-1:0]    t2_R;
    wire        [GF_LEN-1:0]    t2_A;
    wire        [GF_LEN-1:0]    t2_B;
    wire        [GF_LEN-1:0]    t2_C;

    wire        [GF_LEN-1:0]    t3_R;
    wire        [GF_LEN-1:0]    t3_A;
    wire        [GF_LEN-1:0]    t3_B;
    wire        [GF_LEN-1:0]    t3_C;

    /* if error less than 3 start */
    assign t2_R = t3_C;
    assign t2_A = synd1p2;
    assign t2_B = synd1;
    assign t2_C = 0;
    /* if error less than 3 end */

    /* base start */
    GF_Mult_2_10_comb synd1_pow_2
    (
        .out(synd1p2),
        .a(synd1),
        .b(synd1)
    );

    GF_Mult_2_10_comb synd1_pow_3
    (
        .out(synd1p3),
        .a(synd1p2),
        .b(synd1)
    );
    /* base end */


    /* coeffi t3 A start */
    GF_Mult_2_10_comb mul_synd1p2_synd3
    (
        .out(synd1p2_synd3),
        .a(synd1p2),
        .b(synd3)
    );

    assign t3_A = synd1p2_synd3 ^ synd5;
    /* coeffi t3 A end */


    /* coeffi t3 C start */
    assign t3_C = synd1p3 ^ synd3;
    /* coeffi t3 C end */


    /* coeffi t3 B start */
    GF_Mult_2_10_comb mul_C_synd1
    (
        .out(t3_B),
        .a(t3_C),
        .b(synd1)
    );
    /* coeffi t3 B end */


    /* coeffi t3 R start */
    /* coeffi t3 R1*/
    GF_Mult_2_10_comb mul_C_pow2
    (
        .out(Cp2),
        .a(t3_C),
        .b(t3_C)
    );

    /* coeffi t3 R2 */
    GF_Mult_2_10_comb mul_synd1_A
    (
        .out(synd1_A),
        .a(t3_A),
        .b(synd1)
    );

    assign t3_R = Cp2 ^ synd1_A;
    /* coeffi t3 R end */
  
    /* error less than 3 detector */
    //assign detector = |t3_C;

    //assign R = detector ? t3_R : t2_R;
    //assign A = detector ? t3_A : t2_A;
    //assign B = detector ? t3_B : t2_B;
    //assign C = detector ? t3_C : t2_C;

    generate
        if(!SYN_TEST) begin : gen_not_syn_test
            assign  out_deg2_R = t2_R;
            assign  out_deg2_A = t2_A;
            assign  out_deg2_B = t2_B;
            assign  out_deg2_C = t2_C;
                      
            assign  out_deg3_R = t3_R;
            assign  out_deg3_A = t3_A;
            assign  out_deg3_B = t3_B;
            assign  out_deg3_C = t3_C;
        end
        else begin
            reg [GF_LEN - 1 : 0]    buf_deg3_R;
            reg [GF_LEN - 1 : 0]    buf_deg3_A;
            reg [GF_LEN - 1 : 0]    buf_deg3_B;
            reg [GF_LEN - 1 : 0]    buf_deg3_C;

            reg [GF_LEN - 1 : 0]    buf_deg2_R;
            reg [GF_LEN - 1 : 0]    buf_deg2_A;
            reg [GF_LEN - 1 : 0]    buf_deg2_B;
            reg [GF_LEN - 1 : 0]    buf_deg2_C;
                                              
            always@(posedge clk) begin
                buf_deg3_R <= t2_R;
            end    
            always@(posedge clk) begin
                buf_deg3_A <= t2_A;
            end    
            always@(posedge clk) begin
                buf_deg3_B <= t2_B;
            end    
            always@(posedge clk) begin
                buf_deg3_C <= t2_C;
            end    
                                 
            always@(posedge clk) begin
                buf_deg2_R <= t3_R; 
            end    
            always@(posedge clk) begin
                buf_deg2_A <= t3_A;
            end    
            always@(posedge clk) begin
                buf_deg2_B <= t3_B;
            end    
            always@(posedge clk) begin
                buf_deg2_C <= t3_C;
            end    


            assign  out_deg2_R = buf_deg2_R;
            assign  out_deg2_A = buf_deg2_A;
            assign  out_deg2_B = buf_deg2_B;
            assign  out_deg2_C = buf_deg2_C;
                                   
            assign  out_deg3_R = buf_deg3_R;
            assign  out_deg3_A = buf_deg3_A;
            assign  out_deg3_B = buf_deg3_B;
            assign  out_deg3_C = buf_deg3_C;


        end
    endgenerate







endmodule

`endif
