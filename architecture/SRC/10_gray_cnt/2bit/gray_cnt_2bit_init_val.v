`ifndef GRAY_CNT_2BIT_INIT_VAL
`define GRAY_CNT_2BIT_INIT_VAL

module GC_2bit_init_val
(
    clk,

    in_ctr_Srst,
    in_ctr_en,
   
    in_ctr_init,

    in_val_Srst,
    in_val_init,

    out_GC
);
    parameter   LENGTH = 2;

    input   clk;

    input   in_ctr_Srst;
    input   in_ctr_en;
    input   in_ctr_init;

    input   [LENGTH - 1 : 0]    in_val_Srst;
    input   [LENGTH - 1 : 0]    in_val_init;

    output  [LENGTH - 1 : 0]    out_GC;

    /*
    reg                         ctr_cycle;
    */
    reg     [LENGTH - 1 : 0]    buf_gray_cnt;

    /*
    always@(posedge clk) begin
        if(in_ctr_Srst)         ctr_cycle <= 0;
        else begin
            if(in_ctr_en) begin
                if(in_ctr_init)     ctr_cycle <= 1;
                else if(buf_gray_cnt == 2'b0)   ctr_cycle <= 0;
                else                            ctr_cycle <= ctr_cycle;
            end
            else                                ctr_cycle <= ctr_cycle;
        end
    end
    */

    always@(posedge clk)
    begin
        if(in_ctr_Srst)         buf_gray_cnt[0] <= in_val_Srst[0]; 
        else if(in_ctr_init)    buf_gray_cnt[0] <= in_val_init[0];
        else begin
            if(in_ctr_en)       buf_gray_cnt[0] <= ~buf_gray_cnt[1];
            else                buf_gray_cnt[0] <= buf_gray_cnt[0];
        end
    end

    always@(posedge clk) begin
        if(in_ctr_Srst)         buf_gray_cnt[1] <= in_val_Srst[1]; 
        else if(in_ctr_init)    buf_gray_cnt[1] <= in_val_init[1]; 
        else begin
            if(in_ctr_en)       buf_gray_cnt[1] <= buf_gray_cnt[0];
            else                buf_gray_cnt[1] <= buf_gray_cnt[1];
        end
    end

    assign out_GC = buf_gray_cnt;

endmodule
`endif
