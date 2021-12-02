`ifndef LLR_MAGNITUDE_SEQ_ADD_CNT
`define LLR_MAGNITUDE_SEQ_ADD_CNT

`include "06_LLR_mag/4bit/sequential/llr_magnitude_seq.v"

module llr_magnitude_seq_addCnt
(
    clk,

    in_ctr_Srst,
    in_ctr_en,

    in_ctr_init,
    in_ctr_done,

    in_llr,

    out_llr_mag,
    out_alpha_cnt
);
    parameter   OUTTER_NAME = "";
    parameter   MODULE_NAME = "llr_magnitude_seq_addCnt";
        localparam  DESIGN_NAME = "llr_magnitude_seq_addCnt";

    genvar      i;

    parameter   LLR_LEN =   4;
        localparam  LLR_MAG_LEN = LLR_LEN - 1;

    parameter   ALPHA_0_XOR_MODE    =   0;

    parameter   PARALLELISM         =   1;

    /*****************************************/
    input                       clk;
    
    input                       in_ctr_Srst;
    input                       in_ctr_en;

    input                       in_ctr_init;
    input                       in_ctr_done;

    input   [LLR_LEN * PARALLELISM - 1 : 0]   in_llr;

    output  [LLR_MAG_LEN * PARALLELISM - 1 : 0]   out_llr_mag;
    output                      out_alpha_cnt;

    generate
        for(i = 0; i < PARALLELISM; i = i + 1) begin : gen_llr2magModule
            llr_magnitude_seq
            #(
                .LLR_LEN(LLR_LEN)
            )
            uut_llr2Mag
            (
                .clk(clk),

                .in_ctr_Srst(in_ctr_Srst),
                .in_ctr_en(in_ctr_en),

                .in_llr(in_llr[LLR_LEN * (i + 1) - 1 : LLR_LEN * i]),
                .out_llr_mag(out_llr_mag[LLR_MAG_LEN * (i + 1) - 1 : LLR_MAG_LEN * i])
            );
        end 
    endgenerate

    generate
        /*ALPHA_0_XOR_MODE is not work*/
        if(ALPHA_0_XOR_MODE) begin : alpha_cnt_en
            if(PARALLELISM == 1)    begin : alpha_cnt_en_1p
                reg cal_alpha_cnt;
                reg out_buf_alpha_cnt;

                always@(posedge clk) begin
                    if(in_ctr_Srst)         cal_alpha_cnt <= 0;
                    else begin
                        if(in_ctr_en) begin
                            if(in_ctr_init) cal_alpha_cnt <= 0;
                            else            cal_alpha_cnt <= cal_alpha_cnt ^ in_llr[LLR_LEN - 1];
                        end
                        else                cal_alpha_cnt <= cal_alpha_cnt;
                    end
                end

                always@(posedge clk) begin
                    if(in_ctr_Srst) out_buf_alpha_cnt <= 0;
                    else begin
                        if(in_ctr_en) begin
                            if(in_ctr_done) out_buf_alpha_cnt <= cal_alpha_cnt;
                            else            out_buf_alpha_cnt <= out_buf_alpha_cnt;
                        end
                        else                out_buf_alpha_cnt <= out_buf_alpha_cnt;
                    end
                end

                assign  out_alpha_cnt = out_buf_alpha_cnt; 
            end
            else    begin : alpha_cnt_en_Np
                reg [PARALLELISM - 1 : 0]   cal_alpha_cnt_Np;
                reg [PARALLELISM - 1 : 0]   out_buf_alpha_cnt_Np;

                for(i = 0; i < PARALLELISM; i = i + 1)  begin : alpha_cnt_en_func_loops
                    always@(posedge clk) begin
                        if(in_ctr_Srst)         cal_alpha_cnt_Np[PARALLELISM] <= 0;
                        else begin
                            if(in_ctr_en) begin
                                if(in_ctr_init) cal_alpha_cnt_Np[PARALLELISM] <= 0;
                                else            cal_alpha_cnt_Np[PARALLELISM] <= cal_alpha_cnt_Np[PARALLELISM] ^ in_llr[LLR_LEN * (PARALLELISM + 1) - 1];
                            end
                            else                cal_alpha_cnt_Np[PARALLELISM] <= cal_alpha_cnt_Np[PARALLELISM];
                        end
                    end

                    always@(posedge clk) begin
                        if(in_ctr_Srst) out_buf_alpha_cnt_Np[PARALLELISM] <= 0;
                        else begin
                            if(in_ctr_en) begin
                                if(in_ctr_done) out_buf_alpha_cnt_Np[PARALLELISM] <= cal_alpha_cnt_Np[PARALLELISM];
                                else            out_buf_alpha_cnt_Np[PARALLELISM] <= out_buf_alpha_cnt_Np[PARALLELISM];
                            end
                            else                out_buf_alpha_cnt_Np[PARALLELISM] <= out_buf_alpha_cnt_Np[PARALLELISM];
                        end
                    end
                end

                assign  out_alpha_cnt = out_buf_alpha_cnt_Np; 
            end
        end
        else begin : alpha_cnt_dis
        end
    endgenerate
    /*****************************************/
    initial begin
        $display("/**************************************************/");
        $display("Instanciating %s", DESIGN_NAME);

        $display("In %s," ,OUTTER_NAME);
        $display("MODULE_NAME : %s", MODULE_NAME);
        $display("DESIGN_NAME : %s", DESIGN_NAME);

        $display("LLR_LEN=%d",LLR_LEN);
        $display("ALPHA_0_XOR_MODE=%d",ALPHA_0_XOR_MODE);
        $display("PARALLELISM=%d",PARALLELISM);
        $display("/**************************************************/");
    end
endmodule
`endif
