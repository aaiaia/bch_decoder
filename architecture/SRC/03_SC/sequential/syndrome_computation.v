/* optimized gf multiplier */
`ifndef SYND_COMPUTATION

`define SYND_COMPUTATION

`include "00_GF_mult/gf_10/comb/GF_Mult_2_10_comb.v"
`include "60_fifo_channel/fifo_multi_ch.v"
/*
initializing condition
port in_ctr_en
value   :1110111111111...111101111...110111...
timing  :----|--------...-----|---...---|--...

*/
module syndrome_computation
(
    clk,

    in_ctr_Srst,
    in_ctr_en,
    in_ctr_init,
    in_ctr_done,

    in_sig_one_bit,
//    in_sig_input_bit_weight,
//    in_sig_increase,

    out_sig_syndrome
);
    parameter   OUTTER_NAME = "";
    parameter   MODULE_NAME =   "syndrome_computation";
        localparam  DESIGN_NAME = "syndrome_computation";
    genvar      i;

    genvar      var_tree_depth;
    genvar      var_tree_length;

    parameter   INPUT_BUF_LEN   =   0;
    parameter   PARALLELISM     =   1;
    parameter   SYNDROME        =   1;
    parameter   HD_BIT_WEIGHT      =   10'd580;
    parameter   SH_DIRECTION    =   -1;
    
    parameter   SH_BIT_WEIGHT_DIRECTION =  -1;

    /*Parameter about Galois Fields */
    parameter   GF_LEN          =   10;
    
    parameter   GF_SH_FORWARD   =   10'd9;
    parameter   GF_SH_BACKWARD  =   10'd516;
        /*GF Shifting value calculation*/
        localparam  GF_DIRECTION_SH_VAL =   gf_sh_index(
                        {{(GF_LEN - 1){1'b0}}, 1'b1},
                        GF_SH_BACKWARD,
                        SYNDROME,
                        SH_DIRECTION,
                        1,
                        SYNDROME);

        localparam  STEP_SYND_SH_VAL    =   gf_sh_index(
                        {{(GF_LEN - 1){1'b0}}, 1'b1},
                        GF_SH_BACKWARD,
                        PARALLELISM,
                        SH_DIRECTION,
                        SYNDROME,
                        PARALLELISM);


        /* GF_SUM_PARAMETERS */
        localparam  TREE_LENGTH = PARALLELISM;
        localparam  TREE_DEPTH = log2(TREE_LENGTH);
        localparam  TREE_INNER_CONNECTION = log2_sum(TREE_LENGTH);
/*********************************************************************/


    input                           clk;

    input                           in_ctr_Srst;
    input                           in_ctr_en;
    input                           in_ctr_init;
    input                           in_ctr_done;

    input   [PARALLELISM - 1 : 0]   in_sig_one_bit;
//    input       [GF_LEN - 1 : 0]    in_sig_input_bit_weight;
//    input       [GF_LEN - 1 : 0]    in_sig_increase;

    output      [GF_LEN - 1 : 0]    out_sig_syndrome;

    wire        [GF_LEN * (PARALLELISM) - 1 : 0]    w_bit_weight_out;
    wire        [GF_LEN - 1 : 0]    w_bit_weight_sum;

    wire        [GF_LEN - 1 : 0]    w_fb_from_reg_synd;
    wire        [GF_LEN - 1 : 0]    w_sum_bit_and_step_synd;
    wire        [GF_LEN - 1 : 0]    w_step_synd_multi;

    reg         [GF_LEN - 1 : 0]    reg_synd_cal;
    reg         [GF_LEN - 1 : 0]    reg_synd_out;

    /* INPUT BUFFER */
    //parameter   INPUT_BUF_LEN                       =   0;
    wire                            w_ctr_en;
    wire                            w_ctr_init;
    wire                            w_ctr_done;

    wire    [PARALLELISM - 1 : 0]   w_hd;


    /*Function definition*/
            initial $display("Syndrome Computation(%d)\n", SYNDROME);

            function integer log2;
                input [31:0] value;

                begin
                    if(value != 0) begin
                        for(log2 = 0; value > 0; log2 = log2 + 1) begin
                            value = (value>>1);
                        end
                    end
                    else begin
                        log2 = 0;
                    end
                end
            endfunction

            function integer log2_sum;
                input [31:0] value;

                begin
                    for(log2_sum = 0; value > 0; log2_sum = log2_sum) begin
                        log2_sum = value + log2_sum;
                        value = value >> 1;
                    end
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

            function    [GF_LEN - 1 : 0]    gf_sh_index;
                /*inputs start*/
                input   [GF_LEN - 1 : 0]    start;
                input   [GF_LEN - 1 : 0]    modulo_val;
                input   [32 - 1 : 0]    ref_index;
                input   integer         direction;
                input   integer         shift_exp;

                input   integer         prt_i;

                integer                 cal_index;
                integer                 cal_exp;
                /*inputs end*/
                if(!ref_index) begin
                    gf_sh_index = start;

                    $display("print i:[%d], ref_index=%d, gf_sh_index=%d", prt_i, ref_index, gf_sh_index);
                end
                else begin
                    gf_sh_index = start;
                   
                    if(direction) begin
                        for(cal_index = 0; cal_index < ref_index; cal_index = cal_index + 1) begin
                            if(direction > 0) begin : shift_left
                                for(cal_exp=0; cal_exp<shift_exp; cal_exp=cal_exp+1) begin
                                    if(gf_sh_index[GF_LEN - 1] == 1'b1)  gf_sh_index = (gf_sh_index << 1) ^ modulo_val;
                                    else                                gf_sh_index = (gf_sh_index << 1);
                                end
                            end
                            else if(direction < 0) begin : shift_right
                                for(cal_exp=0; cal_exp<shift_exp; cal_exp=cal_exp+1) begin
                                    if(gf_sh_index[0] == 1'b1)  gf_sh_index = (gf_sh_index >> 1) ^ modulo_val;
                                    else                                gf_sh_index = (gf_sh_index >> 1);
                                end
                            end
                            else begin : shift_not
                                gf_sh_index = gf_sh_index;
                            end
                        end
                    end

                    $display("print i:[%d], ref_index=%d, gf_sh_index=%d", prt_i, ref_index, gf_sh_index);
                end
            endfunction

            function    [GF_LEN - 1 : 0]    gf_sh_one;
                /*INPUTS*/
                input   [GF_LEN - 1 : 0]    modulo_val;
                input   [GF_LEN - 1 : 0]    insert_val;
                input   integer         direction;
                /*ENd inputs*/
                if(insert_val == 0) begin
                    gf_sh_one = insert_val;

                    $display("sh_up_val=%d", gf_sh_one);
                end
                else begin
                    if(direction > 0) begin
                        if(insert_val[GF_LEN - 1] == 1'b1)  gf_sh_one = (insert_val << 1) ^ modulo_val;
                        else                                gf_sh_one = (insert_val << 1);

                        $display("sh_up_val=%d", gf_sh_one);
                    end
                    else if(direction < 0) begin
                        if(insert_val[0] == 1'b1)  gf_sh_one = (insert_val >> 1) ^ modulo_val;
                        else                                gf_sh_one = (insert_val >> 1);

                        $display("sh_down_val=%d", gf_sh_one);
                    end
                    else begin
                        $display("no_sh_val=%d", gf_sh_one);
                    end
                end
            endfunction


    generate
        if(INPUT_BUF_LEN == 0)
        begin : gen_inputBuf0
            assign w_ctr_en    = in_ctr_en;
            assign w_ctr_init  = in_ctr_init;
            assign w_ctr_done  = in_ctr_done;

            assign w_hd = in_sig_one_bit;
        end
        else
        begin : gen_inputBuf
            if(INPUT_BUF_LEN != 0) begin : hasLength
                fifo_multi_ch
                #(.FIFO_LEN(INPUT_BUF_LEN), .FIFO_CH_WIDTH(3 + PARALLELISM))
                uut_inputBuf
                (
                    .clk(clk),

                    .in_ctr_Srst(in_ctr_Srst),
                    .in_ctr_en(in_ctr_en),

                    .in({in_ctr_en, in_ctr_init, in_ctr_done, in_sig_one_bit}),
                    .out({w_ctr_en, w_ctr_init, w_ctr_done, w_hd})
                );
            end
            else begin : notHasLength
                assign  {w_ctr_en, w_ctr_init, w_ctr_done, w_hd} = 
                            {in_ctr_en, in_ctr_init, in_ctr_done, in_sig_one_bit};
            end

        end
    endgenerate


    /* GENERATE PARALLEL BIT WEIGHT start */

    //gf_sh_one(modulo_val, insert_val, direction);
    generate
        initial $display("<Syndrome Bit weight calculation>");

        for(i=0; i<PARALLELISM; i=i+1)begin : gen_bit_weight
/*            GF_Mult_2_10_comb multiplier_bit_synd
            (
                .a({{(GF_LEN-1){1'b0}}, w_hd[i]}),
                .b(gf_sh_index(
                        HD_BIT_WEIGHT,
                        GF_SH_BACKWARD,
                        (PARALLELISM - i - 1),
                        SH_DIRECTION,
                        SYNDROME,
                        i)
                    ),
                .out(w_bit_weight_out[(i + 1) * GF_LEN - 1 : (i + 0) * GF_LEN])
            );*/

            GF_Mult_2_10_comb multiplier_bit_synd
            (
                .a({{(GF_LEN-1){1'b0}}, w_hd[i]}),
                .b(gf_sh_index(
                        //{{(GF_LEN - 1){1'b0}}, 1'b1},
                        HD_BIT_WEIGHT,
                        GF_SH_BACKWARD,
                        ((PARALLELISM - 1 ) - i),
                        SH_BIT_WEIGHT_DIRECTION,
                        SYNDROME,
                        i)
                    ),
                .out(w_bit_weight_out[(i + 1) * GF_LEN - 1 : (i + 0) * GF_LEN])
            );

        end
    endgenerate

    /* GENERATE PARALLEL BIT WEIGHT start */

    generate
        if(PARALLELISM == 1) begin : gen_noTree_bitWeightSum
            assign w_bit_weight_sum = w_bit_weight_out;
        end
        else begin : gen_yesTree_bitWeightSum
            initial begin
                $display("TREE_LENGTH:%d, TREE_DEPTH:%d, TREE_IN_CON:%d\n",
                    TREE_LENGTH,
                    TREE_DEPTH,
                    TREE_INNER_CONNECTION);
            end

            
            wire    [TREE_INNER_CONNECTION * GF_LEN - 1 : 0]   w_tree_wire;


            assign w_tree_wire
                [
                    (TREE_INNER_CONNECTION) * GF_LEN - 1 : 
                    (TREE_INNER_CONNECTION - TREE_LENGTH) * GF_LEN
                ] = w_bit_weight_out;

            for(var_tree_depth = TREE_LENGTH; var_tree_depth != 1; var_tree_depth = (var_tree_depth >> 1)) begin : gen_comp_tree_depth

                for(    var_tree_length = log2_sum(var_tree_depth); 
                        var_tree_length > log2_sum(var_tree_depth >> 1); 
                        var_tree_length = var_tree_length - 2) 
                begin : gen_comp_tree_length
                    /* sample wiring index
                    assign w_tree_wire[(((((var_tree_length) - 1) >> 1) - 1) + 1) * GF_LEN - 1 : (((((var_tree_length) - 1) >> 1) - 1) + 0) * GF_LEN] = 
                        w_tree_wire[(((var_tree_length) - 1) + 1) * GF_LEN - 1 : (((var_tree_length) - 1) + 0) * GF_LEN] ^
                        w_tree_wire[(((var_tree_length) - 2) + 1) * GF_LEN - 1 : (((var_tree_length) - 2) + 0) * GF_LEN];

                     * */
                    initial $display("depth:%d,length:%d, [%d:%d] = [%d:%d] ^ [%d:%d]\n",
                            var_tree_depth,
                            var_tree_length,
                            (((((var_tree_length) - 1) >> 1) - 1) + 1) * GF_LEN - 1,   (((((var_tree_length) - 1) >> 1) - 1) + 0) * GF_LEN,
                            (((var_tree_length) - 1) + 1) * GF_LEN - 1,                (((var_tree_length) - 1) + 0) * GF_LEN,
                            (((var_tree_length) - 2) + 1) * GF_LEN - 1,                (((var_tree_length) - 2) + 0) * GF_LEN
                    );


                    assign
                    w_tree_wire[(((((var_tree_length) - 1) >> 1) - 1) + 1) * GF_LEN - 1 : (((((var_tree_length) - 1) >> 1) - 1) + 0) * GF_LEN] = 
                    w_tree_wire[(((var_tree_length) - 2) + 1) * GF_LEN - 1 : (((var_tree_length) - 2) + 0) * GF_LEN] ^ 
                    w_tree_wire[(((var_tree_length) - 1) + 1) * GF_LEN - 1 : (((var_tree_length) - 1) + 0) * GF_LEN] ;
                end

            end

            assign w_bit_weight_sum = w_tree_wire[GF_LEN - 1 : 0];
        end
    endgenerate


    GF_Mult_2_10_comb multiplier_step_synd
    (
        .a(w_fb_from_reg_synd),
        .b(STEP_SYND_SH_VAL),
        .out(w_step_synd_multi)
    );

    assign w_sum_bit_and_step_synd = w_bit_weight_sum ^ w_step_synd_multi;

    always@(posedge clk)
    begin
        if(in_ctr_Srst)     reg_synd_cal <= {(GF_LEN){1'b0}};
        else if(w_ctr_en)
        begin
            //if(w_ctr_init)  reg_synd_cal <= w_bit_weight_out;
            if(w_ctr_init)  reg_synd_cal <= {(GF_LEN){1'b0}};
            else            reg_synd_cal <= w_sum_bit_and_step_synd;
        end
        else                reg_synd_cal <= reg_synd_cal;

    end

    assign  w_fb_from_reg_synd = reg_synd_cal;
    //assign  w_fb_from_reg_synd = w_ctr_en ? reg_synd_cal : {(GF_LEN){1'b0}};

    //assign  out_sig_syndrome = w_ctr_done ? reg_synd_cal : {(GF_LEN){1'b0}};
    //assign  out_sig_syndrome = w_ctr_done ? w_sum_bit_and_step_synd : {(GF_LEN){1'b0}};

    always@(posedge clk) begin
        if(in_ctr_Srst)     reg_synd_out <= 0;
        else begin
            if(w_ctr_en) begin
                if(w_ctr_done)  reg_synd_out <= w_sum_bit_and_step_synd;
                else            reg_synd_out <= reg_synd_out;
            end
            else                reg_synd_out <= reg_synd_out;
        end
    end
    assign out_sig_syndrome = reg_synd_out;

/*********************************************************************/
/*********************************************************************/
    initial begin
        $display("/**************************************************/");
        $display("Instanciating %s", DESIGN_NAME);

        $display("In %s," ,OUTTER_NAME);
        $display("MODULE_NAME : %s", MODULE_NAME);
        $display("DESIGN_NAME : %s", DESIGN_NAME);

        $display("INPUT_BUF_LEN = %d", INPUT_BUF_LEN);
        $display("PARALLELISM = %d", PARALLELISM);
        $display("SYNDROME = %d", SYNDROME);
        $display("HD_BIT_WEIGHT = %d", HD_BIT_WEIGHT);
        $display("SH_DIRECTION = %d", SH_DIRECTION);
        $display("SH_BIT_WEIGHT_DIRECTION = %d", SH_BIT_WEIGHT_DIRECTION);
        $display("GF_LEN = %d", GF_LEN);
        $display("GF_SH_FORWARD = %d", GF_SH_FORWARD);
        $display("GF_SH_BACKWARD = %d", GF_SH_BACKWARD);

        $display("GF_DIRECTION_SH_VAL = %d", GF_DIRECTION_SH_VAL);
        $display("STEP_SYND_SH_VAL = %d", STEP_SYND_SH_VAL);

        $display("TREE_LENGTH = %d", TREE_LENGTH);
        $display("TREE_DEPTH = %d", TREE_DEPTH);
        $display("TREE_INNER_CONNECTION = %d", TREE_INNER_CONNECTION);
        $display("/**************************************************/");
//        $display(" = %d", );
//        $display(" : %s",
//         ? "enabled" : "disabled" );
    end
/*********************************************************************/
/*********************************************************************/
endmodule
`endif
