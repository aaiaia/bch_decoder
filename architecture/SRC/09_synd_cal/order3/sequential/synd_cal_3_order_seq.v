`ifndef SYND_CALCULATOR_3_ORDER_SEQ
`define SYND_CALCULATOR_3_ORDER_SEQ

`include "01_GF_cnt/gf_10/seq/general_gf10_cnt.v"

module synd_cal_3_order_seq
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
    parameter       GF_LEN = 10;
    input   clk;

    input   in_ctr_Srst;
    input   in_ctr_en;

    input   in_ctr_init;

    input   [GF_LEN - 1 : 0]   in_alpha;

    output  [GF_LEN - 1 : 0]   out_synd1_TP;
    output  [GF_LEN - 1 : 0]   out_synd3_TP;
    output  [GF_LEN - 1 : 0]   out_synd5_TP;

    
    wire    [GF_LEN - 1 : 0]   w_pow2;
    wire    [GF_LEN - 1 : 0]   w_pow3;
    wire    [GF_LEN - 1 : 0]   w_pow5;

    reg     [GF_LEN - 1 : 0]   buf_synd[2:0];

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

    always@(posedge clk)
    begin
        if(in_ctr_Srst)         buf_synd[0] <= {(GF_LEN){1'b0}};
        else
        begin
            if(in_ctr_en)
            begin    
                if(in_ctr_init) buf_synd[0] <= {(GF_LEN){1'b0}};
                else            buf_synd[0] <= in_alpha;
            end 
            else                buf_synd[0] <= buf_synd[0];
        end
    end

    always@(*)  
    begin
        if(in_ctr_Srst)         buf_synd[1] <= {(GF_LEN){1'b0}};
        else
        begin
            if(in_ctr_en)
            begin
                if(in_ctr_init) buf_synd[1] <= {(GF_LEN){1'b0}};
                else            buf_synd[1] <= w_pow3;
            end
            else                buf_synd[1] <= buf_synd[1];
        end
    end

    always@(*)  
    begin
        if(in_ctr_Srst)         buf_synd[2] <= {(GF_LEN){1'b0}};
        else
        begin
            if(in_ctr_en)
            begin
                if(in_ctr_init) buf_synd[2] <= {(GF_LEN){1'b0}};
                else            buf_synd[2] <= w_pow5;
            end
            else                buf_synd[2] <= buf_synd[2];
        end
    end

    assign  out_synd1_TP = buf_synd[0];
    assign  out_synd3_TP = buf_synd[1];
    assign  out_synd5_TP = buf_synd[2];


endmodule
`endif
