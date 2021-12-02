`ifndef CLK_CYCLE_CONFIRM
`define CLK_CYCLE_CONFIRM


module  clk_cycle_confirm
(
    clk,
    in_Srst,
    in_en,
    in_start,
    out_clk_cycle,
    out_clk_cnt
);

    parameter   CLK_CNT = 10'd1023;
    parameter   CNT_LEN = 10;

    input   clk;

    input   in_Srst;
    input   in_en;
    input   in_start;

    output  [CNT_LEN - 1 : 0]   out_clk_cycle;
    output  [CNT_LEN - 1 : 0]   out_clk_cnt;


    reg     [CNT_LEN - 1 : 0]   cal_clk_cycle;
    reg     [CNT_LEN - 1 : 0]   cal_clk_cnt;


    always@(posedge clk) begin
        if(in_Srst) cal_clk_cnt <= {(CNT_LEN){1'b0}};
        else begin
            if(in_en) begin
                if(in_start)                    cal_clk_cnt <= {{(CNT_LEN - 1){1'b0}}, 1'b1};
                else if(cal_clk_cnt == 0)       cal_clk_cnt <= 0;
                else if(cal_clk_cnt == CLK_CNT) cal_clk_cnt <= {{(CNT_LEN - 1){1'b0}}, 1'b1};
                else                            cal_clk_cnt <= cal_clk_cnt + 1;
            end
            else                                cal_clk_cnt <= cal_clk_cnt;
        end
    end

    always@(posedge clk) begin
        if(in_Srst) cal_clk_cycle <= {(CNT_LEN){1'b0}};
        else begin
            if(in_en) begin
                if(cal_clk_cnt == CLK_CNT)  cal_clk_cycle <= cal_clk_cycle + 1;
                else                        cal_clk_cycle <= cal_clk_cycle;
            end
        end
    end

    assign out_clk_cycle = cal_clk_cycle;
    assign out_clk_cnt = cal_clk_cnt;

endmodule

`endif
