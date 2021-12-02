/*****************************************************/
/*
 * Test Syndrome Computation module
 * version 1.
 * main functions
 * Sampling 3 alpha^1 value from inserted LLR magnitude values
*/
/*****************************************************/

`ifndef TEST_SYND_COMPUTATION
`define TEST_SYND_COMPUTATION

`include "00_GF_mult/gf_10/comb/GF_Mult_2_10_comb.v"

`include "01_GF_cnt/gf_10/seq/general_gf10_cnt.v"
`include "07_mag_comp/3bit/mag_comp_CLA_comb/mag_comp_CLA_3bit_comb.v"
`include "07_mag_comp/3bit/mag_comp_CLA_comb/mag_comp_CLA_3bit_comb_out_min_val.v"

module test_synd_computation
(
    clk,

    in_ctr_Srst,
    in_ctr_en,

    in_ctr_init,
    in_ctr_phase_shift,

    in_init_alpha_val,

    in_llr_mag,

    out_sig_alpha1,
    out_sig_alpha2,
    out_sig_alpha3
);
    parameter   OUTTER_NAME =   "";
    parameter   MODULE_NAME =   "test_synd_computation";
        localparam  DESIGN_NAME =   "test_synd_computation";

    genvar      i;

    genvar      var_tree_depth;
    genvar      var_tree_length;

    parameter   GF_LEN = 10;

    parameter   LLR_MAG_LEN = 3;
    parameter   PARALLELISM = 4;

        parameter   GF_SH_FORWARD   =   10'd9;
        parameter   GF_SH_BACKWARD  =   10'd516;

        localparam  TREE_LENGTH = PARALLELISM;
        localparam  TREE_DEPTH = log2(TREE_LENGTH);
        localparam  TREE_INNER_CONNECTION = log2_sum(TREE_LENGTH);

        localparam  PARAM_GF_CNT_SH_VAL = func_sh_left(GF_SH_FORWARD, PARALLELISM);

    /*************************************************/
    initial begin
        $display("!!! in) %s !!!", OUTTER_NAME);
        $display("!!! %s !!!" ,MODULE_NAME);
        $display("!!! %s !!!" ,DESIGN_NAME);
        $display("GF_LEN=%d", GF_LEN);
        $display("LLR_MAG_LEN=%d", LLR_MAG_LEN);
        $display("PARALLELISM=%d", PARALLELISM);
        $display("GF_SH_FORWARD=%d", GF_SH_FORWARD);
        $display("GF_SH_BACKWARD=%d", GF_SH_BACKWARD);
    end
    /*************************************************/
    input   clk;

    input   in_ctr_Srst;
    input   in_ctr_en;

    input   in_ctr_init;
    input   in_ctr_phase_shift;

    input   [GF_LEN - 1 : 0]    in_init_alpha_val;

    input   [PARALLELISM * LLR_MAG_LEN - 1 : 0]  in_llr_mag;

    output  [GF_LEN - 1 : 0]    out_sig_alpha1;
    output  [GF_LEN - 1 : 0]    out_sig_alpha2;
    output  [GF_LEN - 1 : 0]    out_sig_alpha3;

    /* selected llr magnitude. If parallel factor is not applied, input llr is
    * connected to this wire.*/
    wire    [LLR_MAG_LEN - 1 : 0]   w_comparedWithBufMinLLR;

    /* saved llr mag wire and reg definition */
    reg     [LLR_MAG_LEN - 1 : 0]  buf_min_llr;

    /* comparator wire and reg definition */
    wire                            w_sig_cp;

    /* gf counter wire and reg definition */
    wire    [GF_LEN - 1 : 0]                w_alpha_val;
    wire    [PARALLELISM * GF_LEN - 1 : 0]  w_alpha_val_p;


    /* min llr mag alpha save wire and reg definition */
    reg     [GF_LEN - 1 : 0]    buf_min_alpha;

    /* min alpha 0 1 2 wire and reg definition */
    reg     [GF_LEN - 1 : 0]    buf_alpha[2:0];


    /*Function definition*/
            initial $display("Test Syndrome Computation\n");

            function integer log2;
                input [31:0] value;
                    for(log2=0;value>0;log2=log2+1)value=value>>1;
            endfunction

            function integer log2_sum;
                input [31:0] value;

                for(log2_sum = 0; value > 0; log2_sum = log2_sum) begin
                    log2_sum = value + log2_sum;
                    value = value >> 1;
                end
            endfunction

            function integer log2_sum_until;
                input [31:0] startWidth;
                input [31:0] untilDepth;

                integer     loopCnt;
                
                if(untilDepth < 0) begin
                    log2_sum_until = 0;
                end
                else if(untilDepth) begin
                    for(loopCnt = 0; loopCnt < untilDepth; loopCnt = loopCnt + 1) begin
                        if(!loopCnt) log2_sum_until = 0;
                        log2_sum_until = log2_sum_until + startWidth;

                        startWidth = startWidth >> 1;
                    end
                end
                else begin
                    log2_sum_until = 0;
                end
            endfunction

            function    [GF_LEN - 1 : 0]    func_sh_left;
                input   [GF_LEN - 1 : 0]    modulo_val;
                input   [32 - 1 : 0]    ref_index;

                integer                 cal_index;

                if(!ref_index) begin
                    func_sh_left = {{(GF_LEN - 1){1'b0}}, 1'b1};

                    $display("ref=%d,modulo=%d", ref_index, func_sh_left);
                end
                else begin
                    func_sh_left = {{(GF_LEN - 1){1'b0}}, 1'b1};
                    for(cal_index = 0; cal_index < ref_index; cal_index = cal_index + 1) begin
                        if(func_sh_left[10 - 1] == 1'b1)  func_sh_left = (func_sh_left << 1) ^ modulo_val;
                        else                                func_sh_left = (func_sh_left << 1);
                    end

                    $display("ref=%d,modulo=%d", ref_index, func_sh_left);
                end
            endfunction

            function    [10 - 1 : 0]    ten_bit_sh_up_and_mod;
                input   [10 - 1 : 0]    modulo_val;
                input   [10 - 1 : 0]    insert_val;

                if(insert_val == 0) begin
                    ten_bit_sh_up_and_mod = insert_val;

                    $display("sh_up_val=%d", ten_bit_sh_up_and_mod);
                end
                else begin
                    if(insert_val[10 - 1] == 1'b1)  ten_bit_sh_up_and_mod = (insert_val << 1) ^ modulo_val;
                    else                                ten_bit_sh_up_and_mod = (insert_val << 1);

                    $display("sh_up_val=%d", ten_bit_sh_up_and_mod);
                end
            endfunction


    /* saved llr mag */
    always@(posedge clk)
    begin
        if(in_ctr_Srst)                     buf_min_llr <= {(LLR_MAG_LEN){1'b1}};
        else begin
            if(in_ctr_en)
            begin
                if(in_ctr_init)             buf_min_llr <= {(LLR_MAG_LEN){1'b1}};
                else if(in_ctr_phase_shift) buf_min_llr <= {(LLR_MAG_LEN){1'b1}};//w_comparedWithBufMinLLR;
                else if(w_sig_cp)           buf_min_llr <= w_comparedWithBufMinLLR;
                else                        buf_min_llr <= buf_min_llr;
            end
            else                            buf_min_llr <= buf_min_llr;
        end
    end


    /* Last depth Comparator */
    mag_comp_CLA_3bit_comb
    uut_mag_comp
    (
        .in_a(buf_min_llr), 
        .in_b(w_comparedWithBufMinLLR), 
        .out_cp(w_sig_cp)
    );
    generate
        if(PARALLELISM == 1) begin : gen_notParallelComp
            /* Series Comparator */
            assign w_comparedWithBufMinLLR = in_llr_mag;
        end
        else begin : gen_parallelComp
            /* Parallel Comparator */
            //!!!CONDITIONS log2(TREE_LENGTH) == integer
            //localparam  TREE_LENGTH = PARALLELISM;
            //localparam  TREE_DEPTH = log2(TREE_LENGTH);
            //localparam  TREE_INNER_CONNECTION = log2_sum(TREE_LENGTH);
            //
            //Example, PARALLELISM = 4
            //TREE_LENGTH = 4
            //TREE_DEPTH ( 4 -> 2 -> 1 ) = 3
            //TREE_INNER_CONNECTION = 4 + 2 + 1 = 7
            //
            //Tree wiring example(input output)
            //If TREE_LENGTH = 4;
            //  
            //  tree_out[6] tree_out[5] tree_out[4] tree_out[3]
            //  tree_in[6]  tree_in[5]  tree_in[4]  tree_in[3]
            //
            //          module[2]               module[1]
            //
            //          tree_out[2]             tree_out[1]
            //          tree_in[2]              tree_in[1]
            //
            //                      module[0]
            //
            //                      tree_out[0]
            //                      tree_in[0]
            //

            initial begin
                $display("TREE_LENGTH:%d, TREE_DEPTH:%d, TREE_IN_CON:%d\n",
                    TREE_LENGTH,
                    TREE_DEPTH,
                    TREE_INNER_CONNECTION);
            end

            
            wire    [TREE_INNER_CONNECTION * LLR_MAG_LEN - 1 : 0]   w_tree_wire;
            wire    [TREE_INNER_CONNECTION * GF_LEN - 1 : 0]        w_tree_passing_wire;


            assign w_tree_wire
                [
                    (TREE_INNER_CONNECTION) * LLR_MAG_LEN - 1 : 
                    (TREE_INNER_CONNECTION - TREE_LENGTH) * LLR_MAG_LEN
                ] = in_llr_mag;


            assign w_tree_passing_wire
                [
                    (TREE_INNER_CONNECTION) * GF_LEN - 1 : 
                    (TREE_INNER_CONNECTION - TREE_LENGTH) * GF_LEN
                ] = w_alpha_val_p;

            for(var_tree_depth = TREE_LENGTH; var_tree_depth != 1; var_tree_depth = (var_tree_depth >> 1)) begin : gen_comp_tree_depth

                for(    var_tree_length = log2_sum(var_tree_depth); 
                        var_tree_length > log2_sum(var_tree_depth >> 1); 
                        var_tree_length = var_tree_length - 2) 
                begin : gen_comp_tree_length
                    /* sample wiring index
                    assign w_tree_wire[(((((var_tree_length) - 1) >> 1) - 1) + 1) * LLR_MAG_LEN - 1 : (((((var_tree_length) - 1) >> 1) - 1) + 0) * LLR_MAG_LEN] = 
                        w_tree_wire[(((var_tree_length) - 1) + 1) * LLR_MAG_LEN - 1 : (((var_tree_length) - 1) + 0) * LLR_MAG_LEN] ^
                        w_tree_wire[(((var_tree_length) - 2) + 1) * LLR_MAG_LEN - 1 : (((var_tree_length) - 2) + 0) * LLR_MAG_LEN];

                     * */
                            initial $display("comp depth:%d,length:%d, [%d:%d] = [%d:%d] ^ [%d:%d]\n",
                                    var_tree_depth,
                                    var_tree_length,
                                    (((((var_tree_length) - 1) >> 1) - 1) + 1) * LLR_MAG_LEN - 1,   (((((var_tree_length) - 1) >> 1) - 1) + 0) * LLR_MAG_LEN,
                                    (((var_tree_length) - 1) + 1) * LLR_MAG_LEN - 1,                (((var_tree_length) - 1) + 0) * LLR_MAG_LEN,
                                    (((var_tree_length) - 2) + 1) * LLR_MAG_LEN - 1,                (((var_tree_length) - 2) + 0) * LLR_MAG_LEN
                            );
                        
                            mag_comp_CLA_3bit_comb_out_min_val #(
                                    .OUTTER_NAME(MODULE_NAME),
                                    .MODULE_NAME("uut_mag_comp"),
                                    .ADDITIONAL_PASSING_VAL_WIDTH(GF_LEN)
                                )
                            uut_mag_comp
                            (
                             .in_a({    w_tree_passing_wire[(((var_tree_length) - 1) + 1) * GF_LEN - 1 : (((var_tree_length) - 1) + 0) * GF_LEN], 
                                        w_tree_wire[(((var_tree_length) - 1) + 1) * LLR_MAG_LEN - 1 : (((var_tree_length) - 1) + 0) * LLR_MAG_LEN]
                                }), 
                             .in_b({     w_tree_passing_wire[(((var_tree_length) - 2) + 1) * GF_LEN - 1 : (((var_tree_length) - 2) + 0) * GF_LEN],
                                        w_tree_wire[(((var_tree_length) - 2) + 1) * LLR_MAG_LEN - 1 : (((var_tree_length) - 2) + 0) * LLR_MAG_LEN]
                                }),
                             .out({    w_tree_passing_wire[(((((var_tree_length) - 1) >> 1) - 1) + 1) * GF_LEN - 1 : (((((var_tree_length) - 1) >> 1) - 1) + 0) * GF_LEN], 
                                        w_tree_wire[(((((var_tree_length) - 1) >> 1) - 1) + 1) * LLR_MAG_LEN - 1 : (((((var_tree_length) - 1) >> 1) - 1) + 0) * LLR_MAG_LEN]
                                })
                             );
                end

            end

            assign w_comparedWithBufMinLLR = w_tree_wire[LLR_MAG_LEN - 1 : 0];
            assign w_alpha_val = w_tree_passing_wire[GF_LEN - 1 : 0];
        end
    endgenerate



    /* gf counter */
    generate
        wire    [GF_LEN - 1 : 0]    gen_w_gf_cnt;
        general_gf10_cnt uut_alpha_gen
        (
            .in_ctr_Srst(in_ctr_Srst),
            .in_ctr_en(in_ctr_en),
            .in_ctr_init(in_ctr_init),

            .clk(clk),

            //.in_Srst_val(in_init_alpha_val),
            .in_Srst_val({{(GF_LEN - 1){1'b0}}, 1'd1}),
            .in_init_val({{(GF_LEN - 1){1'b0}}, 1'd1}),
            //.in_init_val(in_init_alpha_val),
            //.in_init_val({{(GF_LEN - 2){1'b0}},2'd2}),
            //.in_increase({{(GF_LEN - 2){1'b0}},2'd2}),
            .in_increase(PARAM_GF_CNT_SH_VAL),

            .out_alpha(gen_w_gf_cnt)
        );

        

        if(PARALLELISM == 1) begin : gen_gfCnt_noParallel
            assign  w_alpha_val_p = gen_w_gf_cnt;
            assign  w_alpha_val = w_alpha_val_p ;
        end
        else begin : gen_gfCnt_yesParallel
            for(i = 0; i < PARALLELISM; i = i + 1) begin : gen_gfCnt_shifting
                GF_Mult_2_10_comb
                uut_gf_multi(
                    .out(w_alpha_val_p[(i + 1) * GF_LEN - 1 : (i + 0) * GF_LEN]),
                    .a(gen_w_gf_cnt),
                    .b(func_sh_left(GF_SH_FORWARD, i))
                );
            end
        end
    endgenerate

    /* min llr mag alpha save wire and reg definition */
    always@(posedge clk)
    begin
        if(in_ctr_Srst)                     buf_min_alpha <= {(GF_LEN){1'b0}};
        else
        begin
            if(in_ctr_en)
            begin
                if(in_ctr_init)             buf_min_alpha <= {(GF_LEN){1'b0}};//in_init_alpha_val;
                else if(in_ctr_phase_shift) buf_min_alpha <= {(GF_LEN){1'b0}};//w_alpha_val;//{(GF_LEN){1'b0}};
                else if(w_sig_cp)           buf_min_alpha <= w_alpha_val;
                else                        buf_min_alpha <= buf_min_alpha;
            end
            else                            buf_min_alpha <= buf_min_alpha;
        end
    end




    /* min alpha 0 1 2 */
    always@(posedge clk)
    begin
        if(in_ctr_Srst)             buf_alpha[0] <= 0;
        else
        begin
            if(in_ctr_en)
            begin
                if(in_ctr_phase_shift)
                begin
                    if(w_sig_cp)    buf_alpha[0] <= w_alpha_val;
                    else            buf_alpha[0] <= buf_min_alpha;
                end
                else                buf_alpha[0] <= buf_alpha[0];
            end
            else                    buf_alpha[0] <= buf_alpha[0];
        end
    end
    assign out_sig_alpha1 = buf_alpha[0];

    always@(posedge clk)
    begin
        if(in_ctr_Srst) buf_alpha[1] <= 0;
        else
        begin
            if(in_ctr_en)
            begin
                if(in_ctr_phase_shift)  buf_alpha[1] <= buf_alpha[0];
                else                    buf_alpha[1] <= buf_alpha[1];
            end
            else                    buf_alpha[1] <= buf_alpha[1];
        end
    end
    assign out_sig_alpha2 = buf_alpha[1];
    
    always@(posedge clk)
    begin
        if(in_ctr_Srst) buf_alpha[2] <= 0;
        else
        begin
            if(in_ctr_en)
            begin
                if(in_ctr_phase_shift)  buf_alpha[2] <= buf_alpha[1];
                else                    buf_alpha[2] <= buf_alpha[2];
            end
            else                    buf_alpha[2] <= buf_alpha[2];
        end
    end
    assign out_sig_alpha3 = buf_alpha[2];
    

endmodule

`endif
