`ifndef GENERAL_GF10_CNT
`define GENERAL_GF10_CNT

`include "00_GF_mult/gf_10/comb/GF_Mult_2_10_comb.v"
module general_gf10_cnt
(
    in_ctr_Srst,
    in_ctr_en,
    in_ctr_init,

    clk,

    in_Srst_val,
    in_init_val,
    in_increase,

    out_alpha
);
    parameter       GF_MULTI    =   "comb";
    parameter       GF_LEN  =   10;

    input                           in_ctr_Srst;
    input                           in_ctr_en;
    input                           in_ctr_init;

    input                           clk;

    input           [GF_LEN-1:0]    in_Srst_val;
    input           [GF_LEN-1:0]    in_init_val;
    input           [GF_LEN-1:0]    in_increase;


    output          [GF_LEN-1:0]    out_alpha;

    reg             [GF_LEN-1:0]    cal_alpha;
    wire            [GF_LEN-1:0]    wire_mult_result;

    assign out_alpha = cal_alpha;

    generate
        if(GF_LEN == 10)
        begin : gf_10_multi
            if(GF_MULTI == "comb")
            begin : gf_10_multi_comb
            GF_Mult_2_10_comb    feedback_mult
            (
                .out(wire_mult_result),
                .a(in_increase),
                .b(cal_alpha)
            );
            end
            else
            begin : gf_10_multi_seq_not_supprt
            end
        end
        else
        begin : gf_mul_not_support
        end
    endgenerate

    always@(posedge clk)
    begin
        /* reset cal_alpha */
        /* count Up */
        if(in_ctr_Srst)             cal_alpha <= in_Srst_val;//{(GF_LEN){1'b0}};
        else
        begin
            if(in_ctr_en)
            begin
                //cal_alpha <= wire_mult_result;
                if(in_ctr_init)     cal_alpha <= in_init_val;
                else                cal_alpha <= wire_mult_result;
            end
            else                    cal_alpha <= cal_alpha;

        end
    end

endmodule
`endif
