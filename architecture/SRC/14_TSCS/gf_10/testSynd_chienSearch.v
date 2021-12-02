`ifndef TEST_SYND_CHIEN_SEARCH
`define TEST_SYND_CHIEN_SEARCH

`include "00_GF_mult/gf_10/comb/GF_Mult_2_10_comb.v"
`include "01_GF_cnt/gf_10/seq/general_gf10_cnt_2.v"
module testSynd_chienSearch
(
    clk,

    in_ctr_Srst,

    in_ctr_init,
    in_ctr_done,
    in_ctr_en,

    in_ctr_TSCS_en,
    in_alpha,
    
    out_alpha_loc
);

    parameter   OUTTER_NAME = "";
    parameter   MODULE_NAME = "testSynd_chienSearch";
        localparam  DESIGN_NAME = "testSynd_chienSearch";

    /*Parameter about Galois Fields start*/
    parameter   GF_LEN          =   10;
    
    parameter   GF_SH_FORWARD   =   10'd9;
    parameter   GF_SH_BACKWARD  =   10'd516;

    parameter   GF_SH_DIRECTION   =   1;
    /*Parameter about Galois Fields end*/

    parameter   PARALLELISM = 4;

    parameter   ALPHA_NUM = 3;//SD_CORRECTABLE

    parameter   GF_CNT_INIT_VAL = 10'd1;
        //parameter   PARAM_GF_CNT_INCREASE_VAL = 10'd2;
        localparam  PARAM_GF_CNT_INCREASE_VAL = 
                        gf_sh_index(
                            GF_CNT_INIT_VAL,
                            GF_SH_FORWARD,
                            PARALLELISM,
                            GF_SH_DIRECTION,
                            1,
                            0
                        );

    /***********************************************/
    /********** Function definition start **********/
    /***********************************************/
            function integer log2;
                input [31:0] value;
                    for(log2=0;value>0;log2=log2+1)value=value>>1;
            endfunction
    /***********************************************/
    /***********************************************/
            function integer log2_sum;
                input [31:0] value;

                for(log2_sum = 0; value > 0; log2_sum = log2_sum) begin
                    log2_sum = value + log2_sum;
                    value = value >> 1;
                end
            endfunction
    /***********************************************/
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
                if(!ref_index) begin
                    gf_sh_index = start;

                    $display("TSCS) print i:[%d], ref_index=%d, gf_sh_index=%d", prt_i, ref_index, gf_sh_index);
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
            endfunction
    /***********************************************/
    /*********** Function definition end ***********/
    /***********************************************/

/**************************************************************/

    genvar  i, j;

    input   clk;

    input   in_ctr_Srst;

    input   in_ctr_init;
    input   in_ctr_done;
    input   in_ctr_en;

    input   in_ctr_TSCS_en;
    input   [ALPHA_NUM * GF_LEN - 1 : 0]    in_alpha;
    
    output  [ALPHA_NUM * PARALLELISM - 1 : 0]             out_alpha_loc;



    wire    [GF_LEN - 1 : 0]    w_alpha;
    wire    [GF_LEN * PARALLELISM - 1 : 0]    w_sh_alpha;


/**************************************************************/
/**************************************************************/

    general_gf10_cnt_2
    #(.GF_LEN(GF_LEN))
    uut_gfCnt
    (
        .in_ctr_Srst(in_ctr_Srst),
        .in_ctr_en(in_ctr_en),
        .in_ctr_init(in_ctr_init),

        .in_ctr_sh_en(in_ctr_TSCS_en),

        .clk(clk),

        .in_Srst_val(GF_CNT_INIT_VAL),
        .in_init_val(GF_CNT_INIT_VAL),
        .in_increase(PARAM_GF_CNT_INCREASE_VAL),

        .out_alpha(w_alpha)
    );

    /********** shifting gf cnt value start **********/
    generate
        for(i = 0; i < PARALLELISM; i = i + 1) begin : gen_gf_cnt_shifter
                GF_Mult_2_10_comb uut_gfCnt_sh_multi(
                        .out(w_sh_alpha[    (i + 1) * GF_LEN - 1 : GF_LEN * i]
                            ),
                        .a(w_alpha),
                        .b( gf_sh_index(
                                    {{(GF_LEN - 1){1'b0}}, 1'b1},//start
                                    GF_SH_FORWARD,//modulo_val
                                    i,//ref_index
                                    GF_SH_DIRECTION,//direction
                                    1,//shift_exp
                                    i//prt_i
                                )
                        )
                    );
        end
    endgenerate

    /********** shifting gf cnt value end ************/


    /********** Test Syndrome equal start **********/
    generate
        for(j = 0; j < PARALLELISM; j = j + 1) begin : gen_parallel_TSC
            //index i indicate testSyndrome
            for(i = 0; i < ALPHA_NUM; i = i + 1) begin : gen_alpha_TSC
                assign  out_alpha_loc[i + j * ALPHA_NUM] = 
                            ~|(
                                w_sh_alpha[(j + 1) * GF_LEN - 1 : GF_LEN * j] ^ 
                                in_alpha[GF_LEN * (i + 1) - 1 : GF_LEN * i]
                            );

                initial $display("TSCS) P[%d], %d-th alpha => out_alpha_loc[%d]=w_sh_alpha[%d:%d]^in_alpha[%d:%d]",
                                j, i,
                                i + j * ALPHA_NUM,

                                (j + 1) * GF_LEN - 1,
                                GF_LEN * j,

                                GF_LEN * (i + 1) - 1,
                                GF_LEN * i
                            );
            end
        end
    endgenerate
    /*********** Test Syndrome equal end ***********/


/**************************************************************/
    initial begin
        $display("/**************************************************/");
        $display("Instanciating %s", DESIGN_NAME);

        $display("In %s," ,OUTTER_NAME);
        $display("MODULE_NAME : %s", MODULE_NAME);
        $display("DESIGN_NAME : %s", DESIGN_NAME);

        $display("GF_LEN=%d",GF_LEN);

        $display("GF_SH_FORWARD=%d",GF_SH_FORWARD);
        $display("GF_SH_BACKWARD=%d",GF_SH_BACKWARD);

        $display("GF_SH_DIRECTION=%d",GF_SH_DIRECTION);

        $display("PARALLELISM=%d",PARALLELISM);

        $display("ALPHA_NUM=%d",ALPHA_NUM);

        $display("GF_CNT_INIT_VAL=%d",GF_CNT_INIT_VAL);
        $display("PARAM_GF_CNT_INCREASE_VAL = %d", PARAM_GF_CNT_INCREASE_VAL);
        $display("/**************************************************/");
    end
endmodule
`endif
