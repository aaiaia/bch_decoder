`ifndef CHIEN_SEARCH
`define CHIEN_SEARCH

`include "00_GF_mult/gf_10/comb/GF_Mult_2_10_comb.v"
`include "01_GF_cnt/gf_10/seq/general_gf10_cnt_2.v"
`include "60_fifo_channel/fifo_single_ch.v"
`include "60_fifo_channel/fifo_multi_ch.v"
/*
initializing condition
port in_ctr_init
value   :00010000...00100...0000100000
port in_ctr_en
value   :111X1111...11X11...1111X11111...
timing  :----|---...---|-...-----|----...
*/

//f(x) = co0 + co1*x + co2*x^2 + co3*x^3 = 0
//if f(x) == 0, out_equal signal is one.
module chien_search_series
(
    in_ctr_Srst,
    in_ctr_en,

    in_ctr_init,
    in_ctr_done,

    in_ctr_CS_en,

    clk,

    in_init_coef,
   
    out_equal,
    out_w_equal, 

    out_done_coef
);
    parameter   OUTTER_NAME =   "";
    parameter   MODULE_NAME =   "chien_search_series";
        localparam  DESIGN_NAME = "chien_search_series";

    genvar      i, j;

    genvar      var_tree_depth;
    genvar      var_tree_length;

    /*Parameter about Galois Fields start*/
    parameter   GF_LEN          =   10;
    
    parameter   GF_SH_FORWARD   =   10'd9;
    parameter   GF_SH_BACKWARD  =   10'd516;

    parameter   COEF_SH_DIRECTION   =   1;
    /*Parameter about Galois Fields end*/
    parameter   PARALLELISM     = 4;

    parameter   EQUATION_POW     = 3;
        localparam  PARAM_EQATION_COEFFI_NUM    =   EQUATION_POW + 1;


    parameter   EQUAL_BUF_EN    = 1;

    parameter   DONE_COEF_BUF_EN    =   0;

        /* Coefficient SUM_PARAMETERS */
        localparam  ZERO_SYND_TREE_LENGTH = PARAM_EQATION_COEFFI_NUM;
        localparam  ZERO_SYND_TREE_DEPTH = log2(ZERO_SYND_TREE_LENGTH);
        localparam  ZERO_SYND_TREE_INNER_CONNECTION = log2_sum(ZERO_SYND_TREE_LENGTH);

        /* Equation is zero detect */
        localparam  EQ_ZERO_TREE_LENGTH = ZERO_SYND_TREE_LENGTH;
        localparam  EQ_ZERO_TREE_DEPTH = ZERO_SYND_TREE_DEPTH;
        localparam  EQ_ZERO_TREE_INNER_CONNECTION = ZERO_SYND_TREE_INNER_CONNECTION;


/*********************************************************************/
/*********************************************************************/

    input   clk;

    input   in_ctr_Srst;
    input   in_ctr_en;
    input   in_ctr_init;
    input   in_ctr_done;

    input   in_ctr_CS_en;

    input   [(PARAM_EQATION_COEFFI_NUM) * GF_LEN - 1 : 0]   in_init_coef;

    output  [PARALLELISM - 1 : 0]   out_w_equal;
    output  [PARALLELISM - 1 : 0]   out_equal;

    output  [(PARAM_EQATION_COEFFI_NUM) * GF_LEN - 1 : 0]   out_done_coef;

    wire    w_zeroSyndDetect;

    wire    [(PARAM_EQATION_COEFFI_NUM) * GF_LEN - 1 : 0]   w_init_coef;

    wire    [(PARAM_EQATION_COEFFI_NUM) * GF_LEN - 1 : 0]   wire_coef_from_gfCnt;

    wire    [PARALLELISM - 1 : 0]                           w_equal;

    /* 
     * GF_LEN = 10
     * PARALLELISM = 4
     * PARAM_EQATION_COEFFI_NUM = 4
     * index i is coefficient nums i = 0 to PARALLELISM - 1 
     * index j is coefficient nums j = 0 to PARAM_EQATION_COEFFI_NUM - 1 
     *
     * 0th parallel use
     * w_shifting_coef  [PARAM_EQATION_COEFFI_NUM * (0 + 1) * GF_LEN - 1 : PARAM_EQATION_COEFFI_NUM * 0 * GF_LEN]
     *                  [4*1*10-1 : 0] = [39 : 0]
     *      0th coefficient use
     *      [   (0 + 0 * PARAM_EQATION_COEFFI_NUM + 1) * GF_LEN - 1 : 
     *          (0 + 0 * PARAM_EQATION_COEFFI_NUM + 0) * GF_LEN]
     *          = [9 : 0]
     *
     *      1st coefficient use
     *      [   (1 + 0 * PARAM_EQATION_COEFFI_NUM + 1) * GF_LEN - 1 : 
     *          (1 + 0 * PARAM_EQATION_COEFFI_NUM + 0) * GF_LEN]
     *          = [(1+0+1)*10-1 : (1+0)*10]
     *          = [19 : 10]
     *
     *      2nd coefficient use
     *      [   (2 + 0 * PARAM_EQATION_COEFFI_NUM + 1) * gf_len - 1 : 
     *          (2 + 0 * PARAM_EQATION_COEFFI_NUM + 0) * gf_len]
     *          = [(2+0+1)*10-1 : (2+0+0)*10]
     *          = [29 : 20]
     *
     *      3rd coefficient use
     *      [   (3 + 0 * PARAM_EQATION_COEFFI_NUM + 1) * GF_LEN - 1 : 
     *          (3 + 0 * PARAM_EQATION_COEFFI_NUM + 0) * GF_LEN] 
     *          = [(3+1)*10-1 : (3+0+0)*10] 
     *          = [39 : 30]
     *
     *
     *      jth coefficient use(nomalization)
     *      [   (j + i * PARAM_EQATION_COEFFI_NUM + 1) * GF_LEN - 1 : 
     *          (j + i * PARAM_EQATION_COEFFI_NUM + 0) * GF_LEN] 
     *
     *
     * 1st parallel use
     * w_shifting_coef  [PARAM_EQATION_COEFFI_NUM * (1 + 1) * GF_LEN - 1 : PARAM_EQATION_COEFFI_NUM * 1 * GF_LEN]
     *                  [4*2*10-1 : 4*1*10] = [79 : 40]
     *      0th coefficient use
     *      [   (0 + 1 * PARAM_EQATION_COEFFI_NUM + 1) * GF_LEN - 1 : 
     *          (0 + 1 * PARAM_EQATION_COEFFI_NUM + 0) * GF_LEN]
     *          = [(0+1*4+1)*10-1 : (0+1*4+0)*10] = 5*10-1 : 40
     *          = [49 : 40]
     *
     *      1st coefficient use
     *      [   (1 + 1 * PARAM_EQATION_COEFFI_NUM + 1) * GF_LEN - 1 : 
     *          (1 + 1 * PARAM_EQATION_COEFFI_NUM + 0) * GF_LEN]
     *          = [(1+1*4+1)*10-1 : (1+1*4+0)*10] = 6*10-1 : 5*10
     *          = [59 : 50]
     *
     *      2nd coefficient use
     *      [   (2 + 1 * PARAM_EQATION_COEFFI_NUM + 1) * gf_len - 1 : 
     *          (2 + 1 * PARAM_EQATION_COEFFI_NUM + 0) * gf_len]
     *          = [(2+1*4+1)*10-1 : (2+1*4+0)*10] = 7*10-1 : 6*10
     *          = [69 : 60]
     *
     *      3rd coefficient use
     *      [   (3 + 1 * PARAM_EQATION_COEFFI_NUM + 1) * GF_LEN - 1 : 
     *          (3 + 1 * PARAM_EQATION_COEFFI_NUM + 0) * GF_LEN] 
     *          = [(3+1*4+1)*10-1 : (3+1*4+0)*10] = 8*10-1 : 7*10
     *          = [79 : 70]
     *
     *
     *      jth coefficient use(nomalization)
     *      [   (j + i * PARAM_EQATION_COEFFI_NUM + 1) * GF_LEN - 1 : 
     *          (j + i * PARAM_EQATION_COEFFI_NUM + 0) * GF_LEN] 
     *
     *
     * 2nd parallel use
     * w_shifting_coef  [PARAM_EQATION_COEFFI_NUM * (2 + 1) * GF_LEN - 1 : PARAM_EQATION_COEFFI_NUM * 2 * GF_LEN]
     *                  [4*3*10-1 : 4*2*10] = [119 : 80]
     *      0th coefficient use
     *      [   (0 + 2 * PARAM_EQATION_COEFFI_NUM + 1) * GF_LEN - 1 : 
     *          (0 + 2 * PARAM_EQATION_COEFFI_NUM + 0) * GF_LEN]
     *          = [(0+2*4+1)*10-1 : (0+2*4+0)*10] = 9*10-1 : 80
     *          = [89 : 80]
     *
     *      1st coefficient use
     *      [   (1 + 2 * PARAM_EQATION_COEFFI_NUM + 1) * GF_LEN - 1 : 
     *          (1 + 2 * PARAM_EQATION_COEFFI_NUM + 0) * GF_LEN]
     *          = [(1+2*4+1)*10-1 : (1+2*4+0)*10] = 10*10-1 : 9*10
     *          = [99 : 90]
     *
     *      2nd coefficient use
     *      [   (2 + 2 * PARAM_EQATION_COEFFI_NUM + 1) * gf_len - 1 : 
     *          (2 + 2 * PARAM_EQATION_COEFFI_NUM + 0) * gf_len]
     *          = [(2+2*4+1)*10-1 : (2+2*4+0)*10] = 11*10-1 : 10*10
     *          = [109 : 100]
     *
     *      3rd coefficient use
     *      [   (3 + 2 * PARAM_EQATION_COEFFI_NUM + 1) * GF_LEN - 1 : 
     *          (3 + 2 * PARAM_EQATION_COEFFI_NUM + 0) * GF_LEN] 
     *          = [(3+2*4+1)*10-1 : (3+2*4+0)*10] = 12*10-1 : 11*10
     *          = [119 : 110]
     *
     *
     *      jth coefficient use(nomalization)
     *      [   (j + i * PARAM_EQATION_COEFFI_NUM + 1) * GF_LEN - 1 : 
     *          (j + i * PARAM_EQATION_COEFFI_NUM + 0) * GF_LEN] 
     *
     *
     *
     * 3rd parallel use
     * w_shifting_coef  [PARAM_EQATION_COEFFI_NUM * (3 + 1) * GF_LEN - 1 : PARAM_EQATION_COEFFI_NUM * 3 * GF_LEN]
     * .
     * .
     * .
     */
    wire    [(PARAM_EQATION_COEFFI_NUM * PARALLELISM) * GF_LEN - 1 : 0]    w_shifting_coef;


    /***********************************************/
    /********** Function definition start **********/
    /***********************************************/
            function integer log2;
                input [31:0] value;

                begin
                    for(log2 = 0; value > 0; log2 = log2 + 1) begin
                        value=value>>1;
                    end
                end
            endfunction
    /***********************************************/
            function integer log2_sum;
                input [31:0] value;

                begin
                    for(log2_sum = 0; value > 0; log2_sum = log2_sum) begin
                        log2_sum = value + log2_sum;
                        value = value >> 1;
                    end
                end
            endfunction
    /***********************************************/
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
                begin
                    if(!ref_index) begin
                        gf_sh_index = start;

                        $display("Coef_sh_val) print i:[%d], ref_index=%d, gf_sh_index=%d", prt_i, ref_index, gf_sh_index);
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

                        $display("Coef_sh_val) print i:[%d], ref_index=%d, gf_sh_index=%d", prt_i, ref_index, gf_sh_index);
                    end
                end
            endfunction
    /***********************************************/
    /*********** Function definition end ***********/
    /***********************************************/

    /* zero Syndrome Dectect */
    //assign  w_zeroSyndDetect = ~((|in_init_coef0) | (|in_init_coef1) | (|in_init_coef2) | (|in_init_coef3));
    generate
        if(PARAM_EQATION_COEFFI_NUM == 1) begin : gen_noTree_zeroSyndDetect
            assign w_zeroSyndDetect = ~(|in_init_coef);
        end
        else begin : gen_yesTree_zeroSyndDetect
            initial begin
                $display("ZERO_SYND_TREE_LENGTH:%d, ZERO_SYND_TREE_DEPTH:%d, TREE_IN_CON:%d\n",
                    ZERO_SYND_TREE_LENGTH,
                    ZERO_SYND_TREE_DEPTH,
                    ZERO_SYND_TREE_INNER_CONNECTION);
            end

            
            wire    [ZERO_SYND_TREE_INNER_CONNECTION * GF_LEN - 1 : 0]   w_tree_zeroSyndDetect;


            assign w_tree_zeroSyndDetect
                [
                    (ZERO_SYND_TREE_INNER_CONNECTION) * GF_LEN - 1 : 
                    (ZERO_SYND_TREE_INNER_CONNECTION - ZERO_SYND_TREE_LENGTH) * GF_LEN
                ] = in_init_coef;

            for(var_tree_depth = ZERO_SYND_TREE_LENGTH; var_tree_depth != 1; var_tree_depth = (var_tree_depth >> 1)) begin : gen_tree_depth_zeroSyndDetect

                for(    var_tree_length = log2_sum(var_tree_depth); 
                        var_tree_length > log2_sum(var_tree_depth >> 1); 
                        var_tree_length = var_tree_length - 2) 
                begin : gen_tree_length_zeroSyndDetect
                    /* sample wiring index
                    assign w_tree_zeroSyndDetect[(((((var_tree_length) - 1) >> 1) - 1) + 1) * GF_LEN - 1 : (((((var_tree_length) - 1) >> 1) - 1) + 0) * GF_LEN] = 
                        w_tree_zeroSyndDetect[(((var_tree_length) - 1) + 1) * GF_LEN - 1 : (((var_tree_length) - 1) + 0) * GF_LEN] ^
                        w_tree_zeroSyndDetect[(((var_tree_length) - 2) + 1) * GF_LEN - 1 : (((var_tree_length) - 2) + 0) * GF_LEN];

                     * */
                    initial $display("zero synd detect) depth:%d,length:%d, [%d:%d] = [%d:%d] ^ [%d:%d]\n",
                            var_tree_depth,
                            var_tree_length,
                            (((((var_tree_length) - 1) >> 1) - 1) + 1) * GF_LEN - 1,   (((((var_tree_length) - 1) >> 1) - 1) + 0) * GF_LEN,
                            (((var_tree_length) - 1) + 1) * GF_LEN - 1,                (((var_tree_length) - 1) + 0) * GF_LEN,
                            (((var_tree_length) - 2) + 1) * GF_LEN - 1,                (((var_tree_length) - 2) + 0) * GF_LEN
                    );


                    assign
                    w_tree_zeroSyndDetect[(((((var_tree_length) - 1) >> 1) - 1) + 1) * GF_LEN - 1 : (((((var_tree_length) - 1) >> 1) - 1) + 0) * GF_LEN] = 
                    w_tree_zeroSyndDetect[(((var_tree_length) - 2) + 1) * GF_LEN - 1 : (((var_tree_length) - 2) + 0) * GF_LEN] | 
                    w_tree_zeroSyndDetect[(((var_tree_length) - 1) + 1) * GF_LEN - 1 : (((var_tree_length) - 1) + 0) * GF_LEN] ;
                end

            end

            assign w_zeroSyndDetect = ~(|w_tree_zeroSyndDetect[GF_LEN - 1 : 0]);
        end
    endgenerate



    /* Zero Syndrome Handling */
    //assign  w_init_coef0 = (w_zeroSyndDetect ? ({{(GF_LEN - 1){1'b0}}, {1'b1}}) : in_init_coef0);
    //assign  w_init_coef1 = (w_zeroSyndDetect ? {(GF_LEN){1'b0}} : in_init_coef1);
    //assign  w_init_coef2 = (w_zeroSyndDetect ? {(GF_LEN){1'b0}} : in_init_coef2);
    //assign  w_init_coef3 = (w_zeroSyndDetect ? {(GF_LEN){1'b0}} : in_init_coef3);
    generate
        for(i = 0; i < PARAM_EQATION_COEFFI_NUM; i = i + 1) begin : gen_zeroSyndHandling
            if(i == 0) begin : gen_zeroSyndHanding_0thException
                assign w_init_coef[(i + 1) * GF_LEN - 1 : i * GF_LEN] = 
                            (w_zeroSyndDetect ? 
                                {{(GF_LEN - 1){1'b0}}, {1'b1}} : 
                                in_init_coef[(i + 1) * GF_LEN - 1 : i * GF_LEN]
                            );
            end
            else begin : gen_zeroSyndHandling_othersException
                assign w_init_coef[(i + 1) * GF_LEN - 1 : i * GF_LEN] = 
                            (w_zeroSyndDetect ? 
                                {(GF_LEN){1'b0}} : 
                                in_init_coef[(i + 1) * GF_LEN - 1 : i * GF_LEN]
                            );
            end
        
        end
    endgenerate


    /*************************************/
    /********** Coefficient Cnt **********/
    /*************************************/
    generate
        for(i = 0; i < PARAM_EQATION_COEFFI_NUM; i = i + 1) begin : gen_coef_cnt
            if(i == 0) begin : gen_coef_cnt_0thExcept
                general_gf10_cnt_2   coef_cnt
                (
                    .in_ctr_Srst(in_ctr_Srst),
                    .in_ctr_en(in_ctr_en),
                    .in_ctr_init(in_ctr_init),

                    .in_ctr_sh_en(in_ctr_CS_en),

                    .clk(clk),

                    .in_Srst_val({{(GF_LEN - 1){1'b0}}, 1'b1}),
                    .in_init_val(w_init_coef[(i + 1) * GF_LEN - 1 : i * GF_LEN]),

                    .in_increase(gf_sh_index(
                            {{(GF_LEN - 1){1'b0}}, 1'b1},
                            GF_SH_FORWARD,
                            i,
                            COEF_SH_DIRECTION,
                            PARALLELISM,
                            i)

                        ),

                    .out_alpha(wire_coef_from_gfCnt[(i + 1) * GF_LEN - 1 : i * GF_LEN])
                );
                    initial $display("SC GF_CNT) %d-th, wire_coef_from_gfCnt[%d:%d]",
                            i,
                            (i + 1) * GF_LEN - 1,
                            i * GF_LEN
                            );
            end
            else
            begin : gen_coef_cnt_others
                general_gf10_cnt_2   coef_cnt
                (
                    .in_ctr_Srst(in_ctr_Srst),
                    .in_ctr_en(in_ctr_en),
                    .in_ctr_init(in_ctr_init),

                    .in_ctr_sh_en(in_ctr_CS_en),

                    .clk(clk),

                    .in_Srst_val({(GF_LEN){1'b0}}),
                    .in_init_val(w_init_coef[(i + 1) * GF_LEN - 1 : i * GF_LEN]),

                    .in_increase(gf_sh_index(
                            {{(GF_LEN - 1){1'b0}}, 1'b1},
                            GF_SH_FORWARD,
                            i,
                            COEF_SH_DIRECTION,
                            PARALLELISM,
                            i)

                        ),

                    .out_alpha(wire_coef_from_gfCnt[(i + 1) * GF_LEN - 1 : i * GF_LEN])
                );
                    initial $display("SC GF_CNT) %d-th, wire_coef_from_gfCnt[%d:%d]",
                            i,
                            (i + 1) * GF_LEN - 1,
                            i * GF_LEN
                            );
            end
        end

        for(i = 0; i < PARALLELISM; i = i + 1) begin : gen_coef_sh
            for(j = 0; j < PARAM_EQATION_COEFFI_NUM; j = j + 1) begin : gen_coef_sh_multi
                GF_Mult_2_10_comb uut_coef_sh_multi(
                    .out(w_shifting_coef[   (j + i * PARAM_EQATION_COEFFI_NUM + 1) * GF_LEN - 1 : 
                                            (j + i * PARAM_EQATION_COEFFI_NUM + 0) * GF_LEN ]),
                    .a(wire_coef_from_gfCnt[(j + 1) * GF_LEN - 1 : j * GF_LEN]),

                    .b(gf_sh_index(
                            {{(GF_LEN - 1){1'b0}}, 1'b1},
                            GF_SH_FORWARD,
                            1,
                            COEF_SH_DIRECTION,
                            j * i,
                            i)
                      )
                    );
                initial begin
                    $display("SC_GF_CNT_SHIFT) w_sh_coef[%d:%d] = wire_coef_fr_gfCnt[%d:%d]",
                        (j + i * PARAM_EQATION_COEFFI_NUM + 1) * GF_LEN - 1,
                        (j + i * PARAM_EQATION_COEFFI_NUM + 0) * GF_LEN,
                        (j + 1) * GF_LEN - 1,
                        j * GF_LEN
                        );
                end
            end
        end
    endgenerate
    /*************************************/
    /*************************************/
    /*************************************/



    /********** Solve Equation **********/
    generate
        if(PARAM_EQATION_COEFFI_NUM == 1) begin : gen_noTree_0thPow
            for(i = 0; i < PARALLELISM; i = i + 1) begin : gen_equal_one_coef_parallel
                initial $display("w_equal[i] = ~|(w_shifting_coef[%d:%d])",
                        (0 + i * PARAM_EQATION_COEFFI_NUM + 1) * GF_LEN - 1,
                        (0 + i * PARAM_EQATION_COEFFI_NUM + 0) * GF_LEN);
                assign w_equal[i] = ~|(w_shifting_coef
                        [
                            (0 + i * PARAM_EQATION_COEFFI_NUM + 1) * GF_LEN - 1 : 
                            (0 + i * PARAM_EQATION_COEFFI_NUM + 0) * GF_LEN 
                        ]);
            end
        end
        else begin : gen_yesTree_othersPow
            for(i = 0; i < PARALLELISM; i = i + 1) begin : gen_equal_twoMore_coef_parallel
                initial begin
                    $display("EQ_ZERO_TREE_LENGTH:%d, EQ_ZERO_TREE_DEPTH:%d, TREE_IN_CON:%d\n",
                        EQ_ZERO_TREE_LENGTH,
                        EQ_ZERO_TREE_DEPTH,
                        EQ_ZERO_TREE_INNER_CONNECTION);
                end

                
                wire    [EQ_ZERO_TREE_INNER_CONNECTION * GF_LEN - 1 : 0]   w_tree_eq_coef;


                assign w_tree_eq_coef
                    [
                        (EQ_ZERO_TREE_INNER_CONNECTION) * GF_LEN - 1 : 
                        (EQ_ZERO_TREE_INNER_CONNECTION - EQ_ZERO_TREE_LENGTH) * GF_LEN
                    ] = w_shifting_coef[    PARAM_EQATION_COEFFI_NUM * (i + 1) * GF_LEN - 1 : 
                                            PARAM_EQATION_COEFFI_NUM * (i + 0) * GF_LEN];
                    initial $display("tree struct w_shift_coef[%d:%d]",
                                PARAM_EQATION_COEFFI_NUM * (i + 1) * GF_LEN - 1,
                                PARAM_EQATION_COEFFI_NUM * (i + 0) * GF_LEN);

                for(var_tree_depth = EQ_ZERO_TREE_LENGTH; var_tree_depth != 1; var_tree_depth = (var_tree_depth >> 1)) begin : gen_comp_tree_depth

                    for(    var_tree_length = log2_sum(var_tree_depth); 
                            var_tree_length > log2_sum(var_tree_depth >> 1); 
                            var_tree_length = var_tree_length - 2) 
                    begin : gen_comp_tree_length
                        /* sample wiring index
                        assign w_tree_eq_coef[(((((var_tree_length) - 1) >> 1) - 1) + 1) * GF_LEN - 1 : (((((var_tree_length) - 1) >> 1) - 1) + 0) * GF_LEN] = 
                            w_tree_eq_coef[(((var_tree_length) - 1) + 1) * GF_LEN - 1 : (((var_tree_length) - 1) + 0) * GF_LEN] ^
                            w_tree_eq_coef[(((var_tree_length) - 2) + 1) * GF_LEN - 1 : (((var_tree_length) - 2) + 0) * GF_LEN];

                         * */
                        initial $display("SC_EQ_SUM) depth:%d,length:%d, [%d:%d] = [%d:%d] ^ [%d:%d]\n",
                                var_tree_depth,
                                var_tree_length,
                                (((((var_tree_length) - 1) >> 1) - 1) + 1) * GF_LEN - 1,   (((((var_tree_length) - 1) >> 1) - 1) + 0) * GF_LEN,
                                (((var_tree_length) - 1) + 1) * GF_LEN - 1,                (((var_tree_length) - 1) + 0) * GF_LEN,
                                (((var_tree_length) - 2) + 1) * GF_LEN - 1,                (((var_tree_length) - 2) + 0) * GF_LEN
                        );


                        assign
                        w_tree_eq_coef[(((((var_tree_length) - 1) >> 1) - 1) + 1) * GF_LEN - 1 : (((((var_tree_length) - 1) >> 1) - 1) + 0) * GF_LEN] = 
                        w_tree_eq_coef[(((var_tree_length) - 2) + 1) * GF_LEN - 1 : (((var_tree_length) - 2) + 0) * GF_LEN] ^ 
                        w_tree_eq_coef[(((var_tree_length) - 1) + 1) * GF_LEN - 1 : (((var_tree_length) - 1) + 0) * GF_LEN] ;
                    end

                end

                assign w_equal[i] = ~|(w_tree_eq_coef[GF_LEN - 1 : 0]);
            end
        end
    endgenerate

    /************************************/ 

    generate
        if(PARALLELISM == 1) begin : eq_equal_noParallel
            if(EQUAL_BUF_EN) begin : eq_equal_bufFunc
                fifo_single_ch
                #(.FIFO_LEN(EQUAL_BUF_EN))
                uut_equal_buf
                (
                    .clk(clk),

                    .in_ctr_Srst(in_ctr_Srst),
                    .in_ctr_en(in_ctr_en),

                    .in(w_equal),
                    .out(out_equal)
                );

                assign out_w_equal = w_equal;
            end
            else begin : notEqEqualBufFunc
                assign out_equal = w_equal;

                assign out_w_equal = w_equal;
            end
        end
        else begin : eq_equal_yesParallel
            if(EQUAL_BUF_EN) begin : eq_equal_bufFunc
                fifo_multi_ch
                #(.FIFO_LEN(EQUAL_BUF_EN), .FIFO_CH_WIDTH(PARALLELISM))
                uut_equal_buf
                (
                    .clk(clk),

                    .in_ctr_Srst(in_ctr_Srst),
                    .in_ctr_en(in_ctr_en),

                    .in(w_equal),
                    .out(out_equal)
                );

                assign out_w_equal = w_equal;
            end
            else begin : notEqEqualBufFunc
                assign out_equal = w_equal;

                assign out_w_equal = w_equal;
            end

        end
    endgenerate





    generate
        if(DONE_COEF_BUF_EN) begin : coef_bufFunc
            fifo_multi_ch
            #(.FIFO_LEN(DONE_COEF_BUF_EN), .FIFO_CH_WIDTH(GF_LEN * PARAM_EQATION_COEFFI_NUM))
            uut_coef_buf
            (
                .clk(clk),

                .in_ctr_Srst(in_ctr_Srst),
                .in_ctr_en(in_ctr_en & in_ctr_done),

                .in(wire_coef_from_gfCnt),
                .out(out_done_coef)
            );

        end
        else begin : notCoef_bufFunc
            assign  out_done_coef = 
                        (   in_ctr_done ? 
                            wire_coef_from_gfCnt : 
                            {(GF_LEN * PARAM_EQATION_COEFFI_NUM){1'b0}}
                        );
        end
    endgenerate
/*********************************************************************/
/*********************************************************************/
    initial begin
        $display("/**************************************************/");
        $display("Instanciating %s", DESIGN_NAME);

        $display("In %s," ,OUTTER_NAME);
        $display("MODULE_NAME : %s", MODULE_NAME);
        $display("DESIGN_NAME : %s", DESIGN_NAME);

        $display("GF_LEN=%d",GF_LEN);
        $display("GF_SH_FORWARD=%d",GF_SH_FORWARD);
        $display("GF_SH_BACKWARD=%d",GF_SH_BACKWARD);

        $display("COEF_SH_DIRECTION=%d",COEF_SH_DIRECTION);

        $display("PARALLELISM=%d",PARALLELISM);

        $display("EQUATION_POW=%d",EQUATION_POW);
        $display("PARAM_EQATION_COEFFI_NUM = %d", PARAM_EQATION_COEFFI_NUM);

        $display("EQUAL_BUF_EN=%d",EQUAL_BUF_EN);

        $display("DONE_COEF_BUF_EN = %d", DONE_COEF_BUF_EN);

        $display("ZERO_SYND_TREE_LENGTH = %d", ZERO_SYND_TREE_LENGTH);
        $display("ZERO_SYND_TREE_DEPTH = %d", ZERO_SYND_TREE_DEPTH);
        $display("ZERO_SYND_TREE_INNER_CONNECTION = %d", ZERO_SYND_TREE_INNER_CONNECTION);

        $display("EQ_ZERO_TREE_LENGTH = %d", EQ_ZERO_TREE_LENGTH);
        $display("EQ_ZERO_TREE_DEPTH = %d", EQ_ZERO_TREE_DEPTH);
        $display("EQ_ZERO_TREE_INNER_CONNECTION = %d", EQ_ZERO_TREE_INNER_CONNECTION);
        $display("/**************************************************/");
    end
/*********************************************************************/
/*********************************************************************/
endmodule
`endif
