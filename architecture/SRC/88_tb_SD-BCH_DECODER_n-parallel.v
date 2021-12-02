`ifndef TESTBENCH
`define TESTBENCH

`include "90_vector_load_module/vector_load_hex_module.v"
//`include "90_vector_load_module/vector_load_bin_module.v"
`include "90_vector_load_module/vector_load_bin_module_to_verifying.v"
`ifndef NETLIST_EN
    `include "88_SD-BCH_DECODER_n-parallel.v"
`else
    `ifdef DEF_1PARALLEL
        `include "94_net_list/SD_BCH_1020_990_1p.v"
    `elsif DEF_2PARALLEL
        `include "94_net_list/SD_BCH_1020_990_2p.v"
    `elsif DEF_4PARALLEL
        `include "94_net_list/SD_BCH_1020_990_4p.v"
    `else 
        `include "94_net_list/SD_BCH_1020_990_Np.v"
    `endif
`endif
`include "60_fifo_channel/fifo_single_ch.v"
`include "95_clk_cycle_confirm_module/95_clk_cycle_confirm_module.v"
module tb_sd_bch_dec
(
    clk,

    tb_ctr_stim_Srst,

    tb_ctr_Srst,
    tb_ctr_en,
    tb_ctr_init,

    out_stim_vec,

    out_codeword,
    out_sd_equal,
    out_corrected,

    out_out_start,
    out_dec_done,

    out_out_clk_cycle,
    out_out_clk_cnt
);
    parameter STIMULUS_VECTOR_PATH  =   "91_stimulis_vectors/1020_988_INPUT.hex";
    parameter COMPARISION_VECTPR_PATH   =   "91_stimulis_vectors/1020_988_COMPARISION.bin";

    parameter GF_LEN        =   10;
    parameter LLR_LEN       =   4;

    parameter PARALLELISM   =   4;

    parameter CW_LEN        =   1020;

    parameter HD_CORRECTABLE    =   3;
        parameter HD_CORRECTABLE_CNT_LEN    =   2;
    parameter SD_CORRECTABLE    =   3;
        parameter SD_CORRECTABLE_CNT_LEN    =   2;

    parameter TEST_SYND_NUM     =   8;

    parameter DECISION_MODE     =   1;

    parameter DEBUG_MODE    =   1;


    /* I/O definition */
    input clk;

    input tb_ctr_stim_Srst;

    input tb_ctr_Srst;
    input tb_ctr_en;
    input tb_ctr_init;

    output [LLR_LEN * PARALLELISM - 1 : 0]    out_stim_vec;

    output [1 * PARALLELISM - 1 : 0]    out_codeword;
    output [PARALLELISM - 1 : 0]        out_sd_equal;
    output [PARALLELISM - 1 : 0]        out_corrected;


    output                      out_out_start;
    output                      out_dec_done;

    output  [GF_LEN - 1 : 0]    out_out_clk_cycle;
    output  [GF_LEN - 1 : 0]    out_out_clk_cnt;
    /* IO buffer definition */
    reg ctr_main_stim_Srst;
    reg ctr_main_Srst;
    reg ctr_main_en;
    reg ctr_main_init;

    /* io buffer function description */
    always@(posedge clk)    ctr_main_stim_Srst <= tb_ctr_stim_Srst;
    always@(posedge clk)    ctr_main_Srst <= tb_ctr_Srst;
    always@(posedge clk)    ctr_main_en <= tb_ctr_en;
    always@(posedge clk)    ctr_main_init <= tb_ctr_init;


    /************************************************************/
    /************************************************************/
    /************************************************************/
    /************************************************************/
    /************************************************************/
    wire    debug_in_init;
    wire    debug_out_init;

    assign  debug_in_init = ctr_main_init;
        fifo_single_ch
        #(.FIFO_LEN((CW_LEN) * 2 + 10))
        uut_debug_init_sig_fifo
        (
            .clk(clk),

            .in_ctr_Srst(ctr_main_Srst),
            .in_ctr_en(ctr_main_en),

            .in(debug_in_init),
            .out(debug_out_init)
        );
    /************************************************************/
    /************************************************************/
    /************************************************************/
    /************************************************************/
    /************************************************************/
    /* Stimulus vector wire and module */
    wire [LLR_LEN * PARALLELISM - 1 : 0] w_stim_vec;
    assign  out_stim_vec = w_stim_vec;
    vector_load_hex_module
    #(
            .PATH(STIMULUS_VECTOR_PATH), 
            .BIT_LEN(LLR_LEN), 


            .PARALLELISM(PARALLELISM), 

            .WORD_LEN(CW_LEN), 
            .WORD_NUM(100)
    )
    uut_llr_vec
    (
        .clk(clk),

        .in_Srst(ctr_main_stim_Srst),
        .in_en(ctr_main_en),

        .out_vector(w_stim_vec)
    );


    /************************************************************/
    /************************************************************/
    /************************************************************/
    /************************************************************/
    /************************************************************/
    wire [PARALLELISM - 1 : 0] w_compare_vec;


    vector_load_bin_module_to_verify
    #(
            .PATH(COMPARISION_VECTPR_PATH),

            .PARALLELISM(PARALLELISM), 

            .WORD_LEN(CW_LEN), 
            .WORD_NUM(100)
    )
    uut_verify_encoding_vec
    (
        .clk(clk),

        .in_Srst(tb_ctr_Srst),
        .in_en(ctr_main_en),

        .in_next_word(out_out_start),

        .out_vector(w_compare_vec)
    );

    /************************************************************/
    /************************************************************/
    /************************************************************/
    /************************************************************/
    /************************************************************/
`ifndef NETLIST_EN
    sd_bch_dec
    #(
        .GF_LEN(GF_LEN),
        .LLR_LEN(LLR_LEN),

        .PARALLELISM(PARALLELISM),

        .CW_LEN(CW_LEN),

        .HD_CORRECTABLE(HD_CORRECTABLE),

        .SD_CORRECTABLE(SD_CORRECTABLE),

        .DECISION_MODE(DECISION_MODE),

        .DEBUG_MODE(DEBUG_MODE)
    )
    uut_sd_bch_dec
    (
        .clk(clk),

        .ctr_Srst(ctr_main_Srst),
        .ctr_en(ctr_main_en),
        .ctr_init(ctr_main_init),

        .in_llr(w_stim_vec),
        
        .out_codeword(out_codeword),
        .out_sd_equal(out_sd_equal),
        .out_corrected(out_corrected),

        .out_out_start(out_out_start),
        .out_dec_done(out_dec_done)
    );
`else
    `ifdef DEF_1PARALLEL
        sd_bch_dec_PARALLELISM1
        uut_sd_bch_dec
        (
            .clk(clk),

            .ctr_Srst(ctr_main_Srst),
            .ctr_en(ctr_main_en),
            .ctr_init(ctr_main_init),

            .in_llr(w_stim_vec),
            
            .out_codeword(out_codeword),
            .out_sd_equal(out_sd_equal),
            .out_corrected(out_corrected),

            .out_out_start(out_out_start),
            .out_dec_done(out_dec_done)
        );
    `endif
    `ifdef DEF_2PARALLEL
        sd_bch_dec_PARALLELISM2
        uut_sd_bch_dec
        (
            .clk(clk),

            .ctr_Srst(ctr_main_Srst),
            .ctr_en(ctr_main_en),
            .ctr_init(ctr_main_init),

            .in_llr(w_stim_vec),
            
            .out_codeword(out_codeword),
            .out_sd_equal(out_sd_equal),
            .out_corrected(out_corrected),

            .out_out_start(out_out_start),
            .out_dec_done(out_dec_done)
        );
    `endif
    `ifdef DEF_4PARALLEL
        sd_bch_dec_PARALLELISM4
        uut_sd_bch_dec
        (
            .clk(clk),

            .ctr_Srst(ctr_main_Srst),
            .ctr_en(ctr_main_en),
            .ctr_init(ctr_main_init),

            .in_llr(w_stim_vec),
            
            .out_codeword(out_codeword),
            .out_sd_equal(out_sd_equal),
            .out_corrected(out_corrected),

            .out_out_start(out_out_start),
            .out_dec_done(out_dec_done)
        );
    `endif
`endif

    wire [PARALLELISM - 1 : 0]w_errBit;

    assign w_errBit = out_codeword ^ w_compare_vec;


    /************************************************************/
    /************************************************************/
    /************************************************************/
    /************************************************************/
    /************************************************************/
    clk_cycle_confirm
    #(.CLK_CNT(CW_LEN), .CNT_LEN(GF_LEN))
    uut_output_clk_cycle
    (
        .clk(clk),
        .in_Srst(ctr_main_Srst),
        .in_en(ctr_main_en),
        .in_start(out_out_start),
        .out_clk_cycle(out_out_clk_cycle),
        .out_clk_cnt(out_out_clk_cnt)
    );
endmodule

`endif
