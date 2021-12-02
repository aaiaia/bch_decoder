//`define DEF_1PARALLEL
//`define DEF_2PARALLEL
//`define DEF_4PARALLEL


`ifndef TOP
`define TOP

`timescale  1us / 1us

`define one_clk #10
`define one_clk_time 10
`define half_clk #5
`define half_clk_time 5


`include "88_tb_SD-BCH_DECODER_n-parallel.v"

module top_tb_sd_bch_dec();

    parameter               TOP_PARAM_PARALLELISM   =   4;
    localparam              PARALLELISM = TOP_PARAM_PARALLELISM;

    parameter               TOP_PARAM_DECISION_MODE =   3;
    localparam              DECISION_MODE = TOP_PARAM_DECISION_MODE;

    parameter               CW_LEN      = 1020;
        localparam          OP_CLK_CYCLE = cal_op_clk(CW_LEN, PARALLELISM);

    parameter               GF_LEN        =   10;
    parameter               LLR_LEN       =   4;
    localparam              LLR_MAG_LEN   =   LLR_LEN - 1;

    

    reg                     top_clk;

    reg                     top_ctr_stim_Srst;

    reg                     top_ctr_Srst;
    reg                     top_ctr_en;
    reg                     top_ctr_init;

    wire [LLR_LEN * PARALLELISM - 1 : 0]  w_stim_vec;

    wire[PARALLELISM - 1 : 0]w_codeword;
    wire[PARALLELISM - 1 : 0]w_sd_equal;
    wire[PARALLELISM - 1 : 0]w_corrected;

    wire                    w_out_start;
    wire                    w_dec_done;

    wire    [GF_LEN - 1 : 0]w_out_clk_cycle;
    wire    [GF_LEN - 1 : 0]w_out_clk_cnt;

    parameter   VCD_NAME = "88_sd_bch_decoder_mode2_Np.vcd";

    /************************************************************/
    /************************************************************/
    /************************************************************/
    /************************************************************/
    /************************************************************/
            function integer cal_op_clk;
                input integer   op_clk;
                input integer   parallelism;

                if(op_clk) begin
                    cal_op_clk = op_clk / parallelism;

                    if(op_clk % parallelism) begin
                        cal_op_clk = cal_op_clk + 1;
                    end
                    
                    $display("in cal_op_clk, op_clk=%d, parallelism=%d, cal_op_clk=%d\n",
                                op_clk, parallelism, cal_op_clk);
                end
                else begin
                    cal_op_clk = op_clk;

                    $display("in cal_op_clk, op_clk=%d, parallelism=%d, cal_op_clk=%d\n",
                                op_clk, parallelism, cal_op_clk);
                end
            endfunction
    /************************************************************/
    /************************************************************/
    /************************************************************/
    /************************************************************/
    /************************************************************/

    tb_sd_bch_dec
        #(  .PARALLELISM(PARALLELISM),
            .CW_LEN(CW_LEN),
            .DECISION_MODE(DECISION_MODE)
            )
    uut_testbench
    (
        .clk(top_clk),

        .tb_ctr_stim_Srst(top_ctr_stim_Srst),

        .tb_ctr_Srst(top_ctr_Srst),
        .tb_ctr_en(top_ctr_en),
        .tb_ctr_init(top_ctr_init),

        .out_stim_vec(w_stim_vec),

        .out_codeword(w_codeword),
        .out_sd_equal(w_sd_equal),
        .out_corrected(w_corrected),

        .out_out_start(w_out_start),
        .out_dec_done(w_dec_done),

        .out_out_clk_cycle(w_out_clk_cycle),
        .out_out_clk_cnt(w_out_clk_cnt)
    );

    always
    begin : clock
        `half_clk;
        top_clk <= ~top_clk;
    end


    initial begin
        $display("<Top module>");
        $display("TOP_PARAM_PARALLELISM=%d", TOP_PARAM_PARALLELISM);
        $display("PARALLELISM=%d", PARALLELISM);
        $display("CW_LEN=%d", CW_LEN);
        $display("OP_CLK_CYCLE=%d", OP_CLK_CYCLE);
        $display("GF_LEN=%d", GF_LEN);
        $display("LLR_LEN=%d", LLR_LEN);
        $display("LLR_MAG_LEN=%d", LLR_MAG_LEN);
        
        if(TOP_PARAM_PARALLELISM == 1) begin
        `ifdef NETLIST_EN
        `ifdef DEF_1PARALLE
            $sdf_annotate("95_sdf_file/SD_BCH_1020_990_1p.sdf",
                    uut_testbench.uut_sd_bch_dec);
            $display("95_sdf_file/SD_BCH_1020_990_1p.sdf");
        `endif
        `endif

            $dumpfile("88_sd_bch_decoder_mode2_1p.vcd");
            $display("VCD files : %s", "88_sd_bch_decoder_mode2_1p.vcd");
        end
        else if(TOP_PARAM_PARALLELISM == 2) begin
        `ifdef NETLIST_EN
        `ifdef DEF_2PARALLE
            $sdf_annotate("95_sdf_file/SD_BCH_1020_990_2p.sdf",
                    uut_testbench.uut_sd_bch_dec);
            $display("95_sdf_file/SD_BCH_1020_990_2p.sdf");
        `endif
        `endif

            $dumpfile("88_sd_bch_decoder_mode2_2p.vcd");
            $display("VCD files : %s", "88_sd_bch_decoder_mode2_2p.vcd");
        end
        else if(TOP_PARAM_PARALLELISM == 4) begin
        `ifdef NETLIST_EN
        `ifdef DEF_4PARALLE
            $sdf_annotate("95_sdf_file/SD_BCH_1020_990_4p.sdf",
                    uut_testbench.uut_sd_bch_dec);
            $display("95_sdf_file/SD_BCH_1020_990_4p.sdf");
        `endif
        `endif

            $dumpfile("88_sd_bch_decoder_mode2_4p.vcd");
            $display("VCD files : %s", "88_sd_bch_decoder_mode2_4p.vcd");
        end
        /*else begin
            `ifdef NETLIST_EN
                $sdf_annotate("95_sdf_file/SD_BCH_1020_990_Np.sdf",
                        uut_testbench.uut_sd_bch_dec);
                $display("95_sdf_file/SD_BCH_1020_990_Np.sdf");
            `endif
            $dumpfile(VCD_NAME);
            $display("VCD files : %s", VCD_NAME);
        end*/

        $dumpvars;


        top_clk <= 0;
        top_ctr_stim_Srst <= 0;
        top_ctr_Srst <= 0;
        top_ctr_en <= 0;
        top_ctr_init <= 0;
        #(`one_clk_time*3);
        
        top_ctr_stim_Srst <= 1;
        `one_clk;

        top_ctr_stim_Srst <= 0;
        #(`one_clk_time*16);
        
        top_ctr_Srst <= 1;
        `one_clk;

        top_ctr_Srst <= 0;
        #(`one_clk_time*3);

        top_ctr_en <= 1;
        #(`one_clk_time*14);

        /* First init */
        top_ctr_init <= 1;
        top_ctr_stim_Srst <= 1;
        `one_clk;
        top_ctr_init <= 0;
        top_ctr_stim_Srst <= 0;
        /* First init */

        /*1 codeword*/
        #(`one_clk_time*(OP_CLK_CYCLE - 1));

        top_ctr_en <= 0;
        top_ctr_init <= 1;
        `one_clk;//disable
        `one_clk;//disable
        `one_clk;//disable
        `one_clk;//disable
        `one_clk;//disable
        `one_clk;//disable
        `one_clk;//disable

        top_ctr_en <= 1;
        top_ctr_init <= 1;
        `one_clk;
        top_ctr_init <= 0;
        /*1 codeword*/


        /*2 codeword*/
        #(`one_clk_time*2);//2 clk

        top_ctr_en <= 0;
        `one_clk; //disable


        top_ctr_en <= 1;
        #(`one_clk_time*3);//5 clk


        top_ctr_en <= 0;
        #(`one_clk_time*2);//disable

        top_ctr_en <= 1;
        #(`one_clk_time*4);//9 clk;

        #(`one_clk_time*(OP_CLK_CYCLE - 1 - 9));

        top_ctr_init <= 1;
        `one_clk;//1020 clk;
        top_ctr_init <= 0;
        /*2 codeword*/

        /*3 codeword*/
        #(`one_clk_time*(OP_CLK_CYCLE - 1));

        top_ctr_init <= 1;
        `one_clk; 
        top_ctr_init <= 0;
        /*3 codeword*/

        /*4 codeword*/
        #(`one_clk_time*(OP_CLK_CYCLE - 1));

        top_ctr_init <= 1;
        `one_clk; 
        top_ctr_init <= 0;
        /*4 codeword*/


        /*5 codeword*/
        #(`one_clk_time*(OP_CLK_CYCLE - 1));

        top_ctr_init <= 1;
        `one_clk; 
        top_ctr_init <= 0;
        /*5 codeword*/


        /*6 codeword*/
        #(`one_clk_time*(OP_CLK_CYCLE - 1));

        top_ctr_init <= 1;
        `one_clk; 
        top_ctr_init <= 0;
        /*6 codeword*/



        /* codeword*/
        #(`one_clk_time*(OP_CLK_CYCLE - 1));

        top_ctr_init <= 1;
        `one_clk; 
        top_ctr_init <= 0;
        /* codeword*/

        /* codeword*/
        #(`one_clk_time*(OP_CLK_CYCLE - 1));

        top_ctr_init <= 1;
        `one_clk; 
        top_ctr_init <= 0;
        /* codeword*/
        /* codeword*/
        #(`one_clk_time*(OP_CLK_CYCLE - 1));

        top_ctr_init <= 1;
        `one_clk; 
        top_ctr_init <= 0;
        /* codeword*/
        /* codeword*/
        #(`one_clk_time*(OP_CLK_CYCLE - 1));

        top_ctr_init <= 1;
        `one_clk; 
        top_ctr_init <= 0;
        /* codeword*/
        /* codeword*/
        #(`one_clk_time*(OP_CLK_CYCLE - 1));

        top_ctr_init <= 1;
        `one_clk; 
        top_ctr_init <= 0;
        /* codeword*/
        /* codeword*/
        #(`one_clk_time*(OP_CLK_CYCLE - 1));

        top_ctr_init <= 1;
        `one_clk; 
        top_ctr_init <= 0;
        /* codeword*/
        /* codeword*/
        #(`one_clk_time*(OP_CLK_CYCLE - 1));

        top_ctr_init <= 1;
        `one_clk; 
        top_ctr_init <= 0;
        /* codeword*/
        /* codeword*/
        #(`one_clk_time*(OP_CLK_CYCLE - 1));

        top_ctr_init <= 1;
        `one_clk; 
        top_ctr_init <= 0;
        /* codeword*/
        /* codeword*/
        #(`one_clk_time*(OP_CLK_CYCLE - 1));

        top_ctr_init <= 1;
        `one_clk; 
        top_ctr_init <= 0;
        /* codeword*/
        /* codeword*/
        #(`one_clk_time*(OP_CLK_CYCLE - 1));

        top_ctr_init <= 1;
        `one_clk; 
        top_ctr_init <= 0;
        /* codeword*/
        /* codeword*/
        #(`one_clk_time*(OP_CLK_CYCLE - 1));

        top_ctr_init <= 1;
        `one_clk; 
        top_ctr_init <= 0;
        /* codeword*/
        /* codeword*/
        #(`one_clk_time*(OP_CLK_CYCLE - 1));

        top_ctr_init <= 1;
        `one_clk; 
        top_ctr_init <= 0;
        /* codeword*/
        /* codeword*/
        #(`one_clk_time*(OP_CLK_CYCLE - 1));

        top_ctr_init <= 1;
        `one_clk; 
        top_ctr_init <= 0;
        /* codeword*/
        /* codeword*/
        #(`one_clk_time*(OP_CLK_CYCLE - 1));

        top_ctr_init <= 1;
        `one_clk; 
        top_ctr_init <= 0;
        /* codeword*/
        /* codeword*/
        #(`one_clk_time*(OP_CLK_CYCLE - 1));

        top_ctr_init <= 1;
        `one_clk; 
        top_ctr_init <= 0;
        /* codeword*/
        /* codeword*/
        #(`one_clk_time*(OP_CLK_CYCLE - 1));

        top_ctr_init <= 1;
        `one_clk; 
        top_ctr_init <= 0;
        /* codeword*/
        /* codeword*/
        #(`one_clk_time*(OP_CLK_CYCLE - 1));

        top_ctr_init <= 1;
        `one_clk; 
        top_ctr_init <= 0;
        /* codeword*/
        /* codeword*/
        #(`one_clk_time*(OP_CLK_CYCLE - 1));

        top_ctr_init <= 1;
        `one_clk; 
        top_ctr_init <= 0;
        /* codeword*/
        /* codeword*/
        #(`one_clk_time*(OP_CLK_CYCLE - 1));

        top_ctr_init <= 1;
        `one_clk; 
        top_ctr_init <= 0;
        /* codeword*/
        /* codeword*/
        #(`one_clk_time*(OP_CLK_CYCLE - 1));

        top_ctr_init <= 1;
        `one_clk; 
        top_ctr_init <= 0;
        /* codeword*/
        /* codeword*/
        #(`one_clk_time*(OP_CLK_CYCLE - 1));

        top_ctr_init <= 1;
        `one_clk; 
        top_ctr_init <= 0;
        /* codeword*/
        /* codeword*/
        #(`one_clk_time*(OP_CLK_CYCLE - 1));

        top_ctr_init <= 1;
        `one_clk; 
        top_ctr_init <= 0;
        /* codeword*/
        /* codeword*/
        #(`one_clk_time*(OP_CLK_CYCLE - 1));

        top_ctr_init <= 1;
        `one_clk; 
        top_ctr_init <= 0;
        /* codeword*/
        /* codeword*/
        #(`one_clk_time*(OP_CLK_CYCLE - 1));

        top_ctr_init <= 1;
        `one_clk; 
        top_ctr_init <= 0;
        /* codeword*/
        /* codeword*/
        #(`one_clk_time*(OP_CLK_CYCLE - 1));

        top_ctr_init <= 1;
        `one_clk; 
        top_ctr_init <= 0;
        /* codeword*/
        /* codeword*/
        #(`one_clk_time*(OP_CLK_CYCLE - 1));

        top_ctr_init <= 1;
        `one_clk; 
        top_ctr_init <= 0;
        /* codeword*/
        /* codeword*/
        #(`one_clk_time*(OP_CLK_CYCLE - 1));

        top_ctr_init <= 1;
        `one_clk; 
        top_ctr_init <= 0;
        /* codeword*/
        /* codeword*/
        #(`one_clk_time*(OP_CLK_CYCLE - 1));

        top_ctr_init <= 1;
        `one_clk; 
        top_ctr_init <= 0;
        /* codeword*/
        /* codeword*/
        #(`one_clk_time*(OP_CLK_CYCLE - 1));

        top_ctr_init <= 1;
        `one_clk; 
        top_ctr_init <= 0;
        /* codeword*/
        /* codeword*/
        #(`one_clk_time*(OP_CLK_CYCLE - 1));

        top_ctr_init <= 1;
        `one_clk; 
        top_ctr_init <= 0;
        /* codeword*/
        /* codeword*/
        #(`one_clk_time*(OP_CLK_CYCLE - 1));

        top_ctr_init <= 1;
        `one_clk; 
        top_ctr_init <= 0;
        /* codeword*/
        /* codeword*/
        #(`one_clk_time*(OP_CLK_CYCLE - 1));

        top_ctr_init <= 1;
        `one_clk; 
        top_ctr_init <= 0;
        /* codeword*/
        /* codeword*/
        #(`one_clk_time*(OP_CLK_CYCLE - 1));

        top_ctr_init <= 1;
        `one_clk; 
        top_ctr_init <= 0;
        /* codeword*/
        /* codeword*/
        #(`one_clk_time*(OP_CLK_CYCLE - 1));

        top_ctr_init <= 1;
        `one_clk; 
        top_ctr_init <= 0;
        /* codeword*/
        /* codeword*/
        #(`one_clk_time*(OP_CLK_CYCLE - 1));

        top_ctr_init <= 1;
        `one_clk; 
        top_ctr_init <= 0;
        /* codeword*/
        /* codeword*/
        #(`one_clk_time*(OP_CLK_CYCLE - 1));

        top_ctr_init <= 1;
        `one_clk; 
        top_ctr_init <= 0;
        /* codeword*/
        /* codeword*/
        #(`one_clk_time*(OP_CLK_CYCLE - 1));

        top_ctr_init <= 1;
        `one_clk; 
        top_ctr_init <= 0;
        /* codeword*/
        /* codeword*/
        #(`one_clk_time*(OP_CLK_CYCLE - 1));

        top_ctr_init <= 1;
        `one_clk; 
        top_ctr_init <= 0;
        /* codeword*/
        /* codeword*/
        #(`one_clk_time*(OP_CLK_CYCLE - 1));

        top_ctr_init <= 1;
        `one_clk; 
        top_ctr_init <= 0;
        /* codeword*/
        /* codeword*/
        #(`one_clk_time*(OP_CLK_CYCLE - 1));

        top_ctr_init <= 1;
        `one_clk; 
        top_ctr_init <= 0;
        /* codeword*/
        /* codeword*/
        #(`one_clk_time*(OP_CLK_CYCLE - 1));

        top_ctr_init <= 1;
        `one_clk; 
        top_ctr_init <= 0;
        /* codeword*/
        /* codeword*/
        #(`one_clk_time*(OP_CLK_CYCLE - 1));

        top_ctr_init <= 1;
        `one_clk; 
        top_ctr_init <= 0;
        /* codeword*/
        /* codeword*/
        #(`one_clk_time*(OP_CLK_CYCLE - 1));

        top_ctr_init <= 1;
        `one_clk; 
        top_ctr_init <= 0;
        /* codeword*/
        /* codeword*/
        #(`one_clk_time*(OP_CLK_CYCLE - 1));

        top_ctr_init <= 1;
        `one_clk; 
        top_ctr_init <= 0;
        /* codeword*/
        /* codeword*/
        #(`one_clk_time*(OP_CLK_CYCLE - 1));

        top_ctr_init <= 1;
        `one_clk; 
        top_ctr_init <= 0;
        /* codeword*/
        /* codeword*/
        #(`one_clk_time*(OP_CLK_CYCLE - 1));

        top_ctr_init <= 1;
        `one_clk; 
        top_ctr_init <= 0;
        /* codeword*/
        /* codeword*/
        #(`one_clk_time*(OP_CLK_CYCLE - 1));

        top_ctr_init <= 1;
        `one_clk; 
        top_ctr_init <= 0;
        /* codeword*/
        /* codeword*/
        #(`one_clk_time*(OP_CLK_CYCLE - 1));

        top_ctr_init <= 1;
        `one_clk; 
        top_ctr_init <= 0;
        /* codeword*/
        /* codeword*/
        #(`one_clk_time*(OP_CLK_CYCLE - 1));

        top_ctr_init <= 1;
        `one_clk; 
        top_ctr_init <= 0;
        /* codeword*/
        /* codeword*/
        #(`one_clk_time*(OP_CLK_CYCLE - 1));

        top_ctr_init <= 1;
        `one_clk; 
        top_ctr_init <= 0;
        /* codeword*/
        /* codeword*/
        #(`one_clk_time*(OP_CLK_CYCLE - 1));

        top_ctr_init <= 1;
        `one_clk; 
        top_ctr_init <= 0;
        /* codeword*/
        /* codeword*/
        #(`one_clk_time*(OP_CLK_CYCLE - 1));

        top_ctr_init <= 1;
        `one_clk; 
        top_ctr_init <= 0;
        /* codeword*/
        /* codeword*/
        #(`one_clk_time*(OP_CLK_CYCLE - 1));

        top_ctr_init <= 1;
        `one_clk; 
        top_ctr_init <= 0;
        /* codeword*/
        /* codeword*/
        #(`one_clk_time*(OP_CLK_CYCLE - 1));

        top_ctr_init <= 1;
        `one_clk; 
        top_ctr_init <= 0;
        /* codeword*/
        /* codeword*/
        #(`one_clk_time*(OP_CLK_CYCLE - 1));

        top_ctr_init <= 1;
        `one_clk; 
        top_ctr_init <= 0;
        /* codeword*/
        /* codeword*/
        #(`one_clk_time*(OP_CLK_CYCLE - 1));

        top_ctr_init <= 1;
        `one_clk; 
        top_ctr_init <= 0;
        /* codeword*/
        /* codeword*/
        #(`one_clk_time*(OP_CLK_CYCLE - 1));

        top_ctr_init <= 1;
        `one_clk; 
        top_ctr_init <= 0;
        /* codeword*/
        /* codeword*/
        #(`one_clk_time*(OP_CLK_CYCLE - 1));

        top_ctr_init <= 1;
        `one_clk; 
        top_ctr_init <= 0;
        /* codeword*/
        /* codeword*/
        #(`one_clk_time*(OP_CLK_CYCLE - 1));

        top_ctr_init <= 1;
        `one_clk; 
        top_ctr_init <= 0;
        /* codeword*/
        /* codeword*/
        #(`one_clk_time*(OP_CLK_CYCLE - 1));

        top_ctr_init <= 1;
        `one_clk; 
        top_ctr_init <= 0;
        /* codeword*/
        /* codeword*/
        #(`one_clk_time*(OP_CLK_CYCLE - 1));

        top_ctr_init <= 1;
        `one_clk; 
        top_ctr_init <= 0;
        /* codeword*/
        /* codeword*/
        #(`one_clk_time*(OP_CLK_CYCLE - 1));

        top_ctr_init <= 1;
        `one_clk; 
        top_ctr_init <= 0;
        /* codeword*/
        /* codeword*/
        #(`one_clk_time*(OP_CLK_CYCLE - 1));

        top_ctr_init <= 1;
        `one_clk; 
        top_ctr_init <= 0;
        /* codeword*/
        /* codeword*/
        #(`one_clk_time*(OP_CLK_CYCLE - 1));

        top_ctr_init <= 1;
        `one_clk; 
        top_ctr_init <= 0;
        /* codeword*/
        /* codeword*/
        #(`one_clk_time*(OP_CLK_CYCLE - 1));

        top_ctr_init <= 1;
        `one_clk; 
        top_ctr_init <= 0;
        /* codeword*/
        /* codeword*/
        #(`one_clk_time*(OP_CLK_CYCLE - 1));

        top_ctr_init <= 1;
        `one_clk; 
        top_ctr_init <= 0;
        /* codeword*/
        /* codeword*/
        #(`one_clk_time*(OP_CLK_CYCLE - 1));

        top_ctr_init <= 1;
        `one_clk; 
        top_ctr_init <= 0;
        /* codeword*/
        /* codeword*/
        #(`one_clk_time*(OP_CLK_CYCLE - 1));

        top_ctr_init <= 1;
        `one_clk; 
        top_ctr_init <= 0;
        /* codeword*/


        /* codeword*/
        #(`one_clk_time*(OP_CLK_CYCLE - 1));

        top_ctr_init <= 1;
        `one_clk; 
        top_ctr_init <= 0;
        /* codeword*/



        #(`one_clk_time*1000);//damping clk
        #(`one_clk_time*1000);//damping clk
        #(`one_clk_time*1000);//damping clk
        //$display("Simulation is done\n");
        $finish;
    end

endmodule

`endif
