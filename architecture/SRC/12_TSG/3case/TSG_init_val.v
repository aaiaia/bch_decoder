`ifndef TEST_SYND_GEN_INIT_VAL
`define TEST_SYND_GEN_INIT_VAL

//`include "09_synd_cal/order3/combinational/synd_cal_3_order_comb.v"
//`include "09_synd_cal/order3/sequential/synd_cal_3_order_seq.v"
//`include "10_gray_cnt/2bit/gray_cnt_2bit.v"
`include "10_gray_cnt/2bit/gray_cnt_2bit_init_val.v"

`include "11_gray_dec/2bit_to_3case/combination/gray_to_diff_decoder_comb.v"
module  TSG_init_val 
(
    clk,
    in_ctr_Srst,
    in_ctr_en,

    in_ctr_init,

    in_ctr_TSG_en,

    in_init_val_hd_alpha1,
    in_init_val_hd_alpha3,
    in_init_val_hd_alpha5,

    in_alpha1,
    in_alpha2,
    in_alpha3,

    out_testSyndPow1,
    out_testSyndPow3,
    out_testSyndPow5
);
    parameter   OUTTER_NAME = "";
    parameter   MODULE_NAME = "TSG_init_val";
        localparam  DESIGN_NAME= "TSG_init_val";
    /* 0 is combination logic */
    /* 1 is sequential logic */
    parameter   SYND_CAL_TYPE   =   0;
    parameter   GF_LEN = 10;
    parameter   TEST_SYND_NUM   =   3;

    parameter   INPUT_ALPHA_POW_NUMS_FIXED = 3;
/***********************************************/


    input clk;
    input in_ctr_Srst;
    input in_ctr_en;

    input in_ctr_init;

    input   in_ctr_TSG_en;

    input   [GF_LEN - 1 : 0]        in_init_val_hd_alpha1;
    input   [GF_LEN - 1 : 0]        in_init_val_hd_alpha3;
    input   [GF_LEN - 1 : 0]        in_init_val_hd_alpha5;


    input   [GF_LEN * INPUT_ALPHA_POW_NUMS_FIXED - 1 : 0]        in_alpha1;
    input   [GF_LEN * INPUT_ALPHA_POW_NUMS_FIXED - 1 : 0]        in_alpha2;
    input   [GF_LEN * INPUT_ALPHA_POW_NUMS_FIXED - 1 : 0]        in_alpha3;

    output  [GF_LEN - 1 : 0]        out_testSyndPow1;
    output  [GF_LEN - 1 : 0]        out_testSyndPow3;
    output  [GF_LEN - 1 : 0]        out_testSyndPow5;

    reg     [GF_LEN - 1 : 0]        buf_cal_testSynd1;
    reg     [GF_LEN - 1 : 0]        buf_cal_testSynd3;
    reg     [GF_LEN - 1 : 0]        buf_cal_testSynd5;

    wire    [GF_LEN - 1 : 0]        w_sel_TS1;
    wire    [GF_LEN - 1 : 0]        w_sel_TS3;
    wire    [GF_LEN - 1 : 0]        w_sel_TS5;

    wire    [GF_LEN - 1 : 0]        w_sum_selTS1AndHDsynd1;
    wire    [GF_LEN - 1 : 0]        w_sum_selTS3AndHDsynd3;
    wire    [GF_LEN - 1 : 0]        w_sum_selTS5AndHDsynd5;

    wire    [1:0]                   w_gray_code;
    wire    [TEST_SYND_NUM - 1 : 0] w_tp_sel;

    reg     [GF_LEN - 1 : 0]        w_sel_tp_synd_p1;
    reg     [GF_LEN - 1 : 0]        w_sel_tp_synd_p3;
    reg     [GF_LEN - 1 : 0]        w_sel_tp_synd_p5;


    GC_2bit_init_val uut_GCcnt_initV
    (
        .clk(clk),

        .in_ctr_Srst(in_ctr_Srst),
        .in_ctr_en(in_ctr_TSG_en),
        .in_ctr_init(in_ctr_init),

        .in_val_Srst(2'b00),
        .in_val_init(2'b01),

        .out_GC(w_gray_code) 
    );
    /*gray_cnt_2bit uut_gray_code
    (
        .clk(clk),

        .in_ctr_Srst(in_ctr_Srst),
        .in_ctr_en(in_ctr_en),

        .out_GC(w_gray_code)
    );*/


    gray_to_diff_decoder_comb uut_gray_code_decoder
    (
        .in_GC(w_gray_code),
        .out_sel_TP(w_tp_sel)
    );

    always@(*)
    begin
        if(w_tp_sel[0])         w_sel_tp_synd_p1 <= in_alpha1[GF_LEN * 1 - 1 : GF_LEN * 0];
        else if(w_tp_sel[1])    w_sel_tp_synd_p1 <= in_alpha2[GF_LEN * 1 - 1 : GF_LEN * 0];
        else if(w_tp_sel[2])    w_sel_tp_synd_p1 <= in_alpha3[GF_LEN * 1 - 1 : GF_LEN * 0];
        else                    w_sel_tp_synd_p1 <= {(GF_LEN){1'b0}};
    end

    always@(*)
    begin
        if(w_tp_sel[0])         w_sel_tp_synd_p3 <= in_alpha1[GF_LEN * 2 - 1 : GF_LEN * 1];
        else if(w_tp_sel[1])    w_sel_tp_synd_p3 <= in_alpha2[GF_LEN * 2 - 1 : GF_LEN * 1];
        else if(w_tp_sel[2])    w_sel_tp_synd_p3 <= in_alpha3[GF_LEN * 2 - 1 : GF_LEN * 1];
        else                    w_sel_tp_synd_p3 <= {(GF_LEN){1'b0}};
    end

    always@(*)
    begin
        if(w_tp_sel[0])         w_sel_tp_synd_p5 <= in_alpha1[GF_LEN * 3 - 1 : GF_LEN * 2];
        else if(w_tp_sel[1])    w_sel_tp_synd_p5 <= in_alpha2[GF_LEN * 3 - 1 : GF_LEN * 2];
        else if(w_tp_sel[2])    w_sel_tp_synd_p5 <= in_alpha3[GF_LEN * 3 - 1 : GF_LEN * 2];
        else                    w_sel_tp_synd_p5 <= {(GF_LEN){1'b0}};
    end

/*
    generate
        if(SYND_CAL_TYPE == 0)
        begin : synd_cal_comb
*/
            /*synd_cal_3_order_comb uut_sel_tp_synd_compute_comb
            (
                .clk(clk),

                .in_ctr_Srst(in_ctr_Srst),
                .in_ctr_en(in_ctr_en),

                .in_ctr_init(in_ctr_init),

                .in_alpha(w_sel_tp_synd_p1),

                .out_synd1_TP(w_sel_TS1),
                .out_synd3_TP(w_sel_TS3),
                .out_synd5_TP(w_sel_TS5)
            );*/
            /* uut_sel_tp_synd_compute_comb is alterated to below ...  */
            assign w_sel_TS1 = w_sel_tp_synd_p1;
            assign w_sel_TS3 = w_sel_tp_synd_p3;
            assign w_sel_TS5 = w_sel_tp_synd_p5;
/*
        end
        else
        begin : synd_cal_seq
            synd_cal_3_order_seq uut_sel_tp_synd_compute_seq
            (
                .clk(clk),

                .in_ctr_Srst(in_ctr_Srst),
                .in_ctr_en(in_ctr_en),

                .in_ctr_calInit(in_ctr_init),

                .in_alpha(w_sel_tp_synd_p1),

                .out_synd1_TP(out_testSyndPow1),
                .out_synd3_TP(out_testSyndPow3),
                .out_synd5_TP(out_testSyndPow5)
            );
        end
    endgenerate
*/

    assign        w_sum_selTS1AndHDsynd1 = w_sel_TS1 ^ buf_cal_testSynd1;
    assign        w_sum_selTS3AndHDsynd3 = w_sel_TS3 ^ buf_cal_testSynd3;
    assign        w_sum_selTS5AndHDsynd5 = w_sel_TS5 ^ buf_cal_testSynd5;

    always@(posedge clk) begin
        if(in_ctr_Srst) begin
            buf_cal_testSynd1 <= {(GF_LEN){1'b0}};
            buf_cal_testSynd3 <= {(GF_LEN){1'b0}};
            buf_cal_testSynd5 <= {(GF_LEN){1'b0}};
        end
        else begin
            if(in_ctr_en) begin
                if(in_ctr_init) begin
                    buf_cal_testSynd1 <= in_init_val_hd_alpha1;
                    buf_cal_testSynd3 <= in_init_val_hd_alpha3;
                    buf_cal_testSynd5 <= in_init_val_hd_alpha5;
                end
                else if(in_ctr_TSG_en) begin
                    buf_cal_testSynd1 <= w_sum_selTS1AndHDsynd1;
                    buf_cal_testSynd3 <= w_sum_selTS3AndHDsynd3;
                    buf_cal_testSynd5 <= w_sum_selTS5AndHDsynd5;
                end
                else begin
                    buf_cal_testSynd1 <= buf_cal_testSynd1;
                    buf_cal_testSynd3 <= buf_cal_testSynd3;
                    buf_cal_testSynd5 <= buf_cal_testSynd5;
                end
            end
            else begin
                buf_cal_testSynd1 <= buf_cal_testSynd1;
                buf_cal_testSynd3 <= buf_cal_testSynd3;
                buf_cal_testSynd5 <= buf_cal_testSynd5;
            end    
        end
    end

    assign  out_testSyndPow1 = buf_cal_testSynd1;
    assign  out_testSyndPow3 = buf_cal_testSynd3;
    assign  out_testSyndPow5 = buf_cal_testSynd5;

/***********************************************/
    initial begin
        $display("/**************************************************/");
        $display("Instanciating %s", DESIGN_NAME);

        $display("In %s," ,OUTTER_NAME);
        $display("MODULE_NAME : %s", MODULE_NAME);
        $display("DESIGN_NAME : %s", DESIGN_NAME);

        $display("SYND_CAL_TYPE = %d",SYND_CAL_TYPE);
        $display("GF_LEN = %d",GF_LEN);
        $display("TEST_SYND_NUM = %d",TEST_SYND_NUM);

        $display("INPUT_ALPHA_POW_NUMS_FIXED = %d", INPUT_ALPHA_POW_NUMS_FIXED);
        $display("/**************************************************/");
    end
/***********************************************/
endmodule

`endif
