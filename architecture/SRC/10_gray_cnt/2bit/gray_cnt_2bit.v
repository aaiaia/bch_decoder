`ifndef GRAY_CNT_2BIT
`define GRAY_CNT_2BIT

module gray_cnt_2bit
(
    clk,

    in_ctr_Srst,
    in_ctr_en,
    in_ctr_init,

    out_GC
);
    parameter   LENGTH = 2;

    input   clk;

    input   in_ctr_Srst;
    input   in_ctr_en;
    input   in_ctr_init;

    output  [LENGTH - 1 : 0]    out_GC;

    reg     [LENGTH - 1 : 0]    buf_gray_cnt;

    always@(posedge clk)
    begin
        if(in_ctr_Srst)     buf_gray_cnt[0] <= 1'b0; 
        else
        begin
            if(in_ctr_en)
            begin
                if(in_ctr_init) buf_gray_cnt[0] <= 1'b0; 
                else            buf_gray_cnt[0] <= ~buf_gray_cnt[1];
            end
            else                buf_gray_cnt[0] <= buf_gray_cnt[0];
        end
    end

    always@(posedge clk)
    begin
        if(in_ctr_Srst)         buf_gray_cnt[1] <= 1'b1; 
        else
        begin
            if(in_ctr_en)
            begin
                if(in_ctr_init) buf_gray_cnt[0] <= 1'b1; 
                else            buf_gray_cnt[1] <= buf_gray_cnt[0];
            end
            else                buf_gray_cnt[1] <= buf_gray_cnt[1];
        end
    end

    assign out_GC = buf_gray_cnt;

endmodule
`endif
