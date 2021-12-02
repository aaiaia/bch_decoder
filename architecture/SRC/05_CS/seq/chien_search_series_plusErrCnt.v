`ifndef CHIEN_SEARCH_SERIES_ADD_ERR_CNT
`define CHIEN_SEARCH_SERIES_ADD_ERR_CNT

`include "05_CS/seq/chien_search_series.v"
//`include "20_one_bit_half_adder/one_bit_half_adder.v"
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
module chien_search_series_plusErrCnt
(
    in_ctr_Srst,
    in_ctr_en,

    in_ctr_init,
    in_ctr_done,

    in_ctr_CS_en,

    clk,

    in_init_coef,
    //in_init_coef0,
    //in_init_coef1,
    //in_init_coef2,
    //in_init_coef3,
    
    out_equal,
    out_errCnt,

    out_done_coef
    //out_done_coef0,
    //out_done_coef1,
    //out_done_coef2,
    //out_done_coef3
);
    /*****************************************************/
    /*****************************************************/


    parameter   OUTTER_NAME =   "";
    parameter   MODULE_NAME =   "chien_search_series_plusErrCnt";
        localparam  DESIGN_NAME =   "chien_search_series_plusErrCnt";

    genvar      i, j;

    genvar      var_tree_depth;
    genvar      var_tree_length;
    /*****************************************************/
    function    integer valToBool;
        input   integer val;

        begin
            if(val) valToBool = 1;
            else    valToBool = 0;
        end
    endfunction
    /*****************************************************/
    parameter   PARALLELISM     = 4;

    parameter   EQUATION_POW     = 3;
        localparam  PARAM_EQATION_COEFFI_NUM    =   EQUATION_POW + 1;

    parameter   GF_LEN          = 10;


        localparam  PARAM_ROOT_CNT_LEN    = errorCntBitLenCal(EQUATION_POW);
    parameter   ROOT_CNT_LIMIT  = 2'd3;

    parameter   EQUAL_BUF_EN    =   1;

    parameter   DONE_COEF_BUF_EN    =   0;

    parameter   DECISION_MODE       =   1;

    parameter   ERR_CNT_OUT_BUF_EN  =   0;
        localparam  LO_PA_ERR_CNT_OUT_FORCELY_EN = (DECISION_MODE == 1) ? 1 : 0; 
        localparam  LO_PA_ERR_CNT_OUT_BUF_EN = 
            valToBool(ERR_CNT_OUT_BUF_EN + LO_PA_ERR_CNT_OUT_FORCELY_EN);

    /*****************************************************/
    input   clk;

    input   in_ctr_Srst;
    input   in_ctr_en;
    input   in_ctr_init;
    input   in_ctr_done;

    input   in_ctr_CS_en;

    input   [GF_LEN * PARAM_EQATION_COEFFI_NUM - 1 : 0]    in_init_coef;
    //input   [GF_LEN - 1 : 0]    in_init_coef0;
    //input   [GF_LEN - 1 : 0]    in_init_coef1;
    //input   [GF_LEN - 1 : 0]    in_init_coef2;
    //input   [GF_LEN - 1 : 0]    in_init_coef3;

    output  [PARALLELISM - 1 : 0]   out_equal;
    //output  out_equal;

    output  [PARAM_ROOT_CNT_LEN - 1 : 0]  out_errCnt;

    output  [GF_LEN * PARAM_EQATION_COEFFI_NUM - 1 : 0]    out_done_coef;
    //output  [GF_LEN - 1 : 0]    out_done_coef0;
    //output  [GF_LEN - 1 : 0]    out_done_coef1;
    //output  [GF_LEN - 1 : 0]    out_done_coef2;
    //output  [GF_LEN - 1 : 0]    out_done_coef3;

    wire    [PARALLELISM - 1 : 0]           w_equal;
    //wire    w_equal;
    wire    [PARAM_ROOT_CNT_LEN - 1 : 0]    w_equal_AL_sum;

    reg     [PARAM_ROOT_CNT_LEN - 1 : 0]    cal_buf_cnt_errCnt;


            /********** Functions **********/
            function integer errorCntBitLenCal;
                input   integer errorNums;

                integer inputValues;
                if(errorNums) begin
                    inputValues = errorNums;    

                    for(errorCntBitLenCal = 0; errorNums != 0; errorCntBitLenCal = errorCntBitLenCal + 1) begin
                        errorNums = (errorNums >> 1);
                    end

                    $display("func) errorCntBitLenCal : input=%d, errorCntBitLenCal=%d",
                            inputValues, errorCntBitLenCal);
                end
                else begin
                    errorCntBitLenCal = 0;

                    $display("func) errorCntBitLenCal : input=%d, errorCntBitLenCal=%d",
                            inputValues, errorCntBitLenCal);
                end
            endfunction
            /*******************************/


    chien_search_series
    #(
        .OUTTER_NAME(MODULE_NAME),
        .MODULE_NAME("uut_CS_series"),
        .GF_LEN(GF_LEN),
        .PARALLELISM(PARALLELISM),
        .EQUATION_POW(EQUATION_POW),
        .EQUAL_BUF_EN(EQUAL_BUF_EN),
        .DONE_COEF_BUF_EN(DONE_COEF_BUF_EN)
    )
    uut_CS_series
    (
        .in_ctr_Srst(in_ctr_Srst),
        .in_ctr_en(in_ctr_en),
        .in_ctr_init(in_ctr_init),
        .in_ctr_done(in_ctr_done),

        .in_ctr_CS_en(in_ctr_CS_en),

        .clk(clk),

        .in_init_coef(in_init_coef),
        //.in_init_coef0(in_init_coef0),
        //.in_init_coef1(in_init_coef1),
        //.in_init_coef2(in_init_coef2),
        //.in_init_coef3(in_init_coef3),

        .out_equal(out_equal),
        .out_w_equal(w_equal),

        .out_done_coef(out_done_coef)
        //.out_done_coef0(out_done_coef0),
        //.out_done_coef1(out_done_coef1),
        //.out_done_coef2(out_done_coef2),
        //.out_done_coef3(out_done_coef3)
    );


    generate
        if(PARALLELISM == 1) begin : gen_noParallel_equal_AL_sum
            if(PARAM_ROOT_CNT_LEN == 1) begin : gen_NP_equal_rootNumIs1
                assign w_equal_AL_sum =  w_equal;
            end
            else begin : gen_NP_equal_rootNumIsLargerThan1
                assign w_equal_AL_sum =  {{(PARAM_ROOT_CNT_LEN - 1){1'b0}} ,w_equal};
            end
        end
        else begin : gen_yesParallel_equal_AL_sum

            if(PARAM_ROOT_CNT_LEN == 1) begin : gen_YP_equal_rootNumIs1
                wire [PARALLELISM - 1 : 0]  tmp_AL_sum;

                assign tmp_AL_sum[0] = w_equal[0];
                for(i=1; i<PARALLELISM; i = i + 1) begin : gen_YP_equal_AL_sum
                    assign tmp_AL_sum[i] = tmp_AL_sum[i - 1] ^ w_equal[i];
                end

                assign w_equal_AL_sum = tmp_AL_sum[PARALLELISM - 1];
            end
            else begin : gen_YP_equal_rootNumIsLargerThan1
                wire [PARALLELISM * PARAM_ROOT_CNT_LEN - 1 : 0]  tmp_AL_sum;

                assign tmp_AL_sum[PARAM_ROOT_CNT_LEN * (0 + 1) - 1 : PARAM_ROOT_CNT_LEN * (0 + 0)] = 
                            {{(PARAM_ROOT_CNT_LEN - 1){1'b0}}, w_equal[0]};
                        initial $display("SC_ADD_ER_CNT_W) tmp_AL_sum[%d:%d] = w_qual[%d]",
                                        PARAM_ROOT_CNT_LEN * (0 + 1) - 1,
                                        PARAM_ROOT_CNT_LEN * (0 + 0),
                                        0
                                    );
                for(i=1; i<PARALLELISM; i = i + 1) begin : gen_YP_equal_AL_sum
                    assign tmp_AL_sum[PARAM_ROOT_CNT_LEN * (i + 1) - 1 : PARAM_ROOT_CNT_LEN * (i + 0)] = 
                            tmp_AL_sum[PARAM_ROOT_CNT_LEN * (i + 0) - 1 : PARAM_ROOT_CNT_LEN * (i - 1)] + 
                            {{(PARAM_ROOT_CNT_LEN - 1){1'b0}}, w_equal[i]};

                        initial $display("SC_ADD_ER_CNT_W) tmp_AL_sum[%d:%d] = tmp_AL_sum[%d:%d] + wqual[%d]",
                                        PARAM_ROOT_CNT_LEN * (i + 1) - 1,
                                        PARAM_ROOT_CNT_LEN * (i + 0),
                                        PARAM_ROOT_CNT_LEN * (i + 0) - 1,
                                        PARAM_ROOT_CNT_LEN * (i - 1),
                                        i
                                    );

                end

                assign w_equal_AL_sum = 
                            tmp_AL_sum[ PARAM_ROOT_CNT_LEN * PARALLELISM - 1 : 
                                        PARAM_ROOT_CNT_LEN * (PARALLELISM - 1)
                                ];
                        initial $display("SC_ADD_ER_CNT_W) w_equal_AL_sum = tmp_AL_sum[%d:%d]",
                                        PARAM_ROOT_CNT_LEN * PARALLELISM - 1,
                                        PARAM_ROOT_CNT_LEN * (PARALLELISM - 1)
                                    );
            end
        end
    endgenerate

    always@(posedge clk) begin
        if(in_ctr_Srst) cal_buf_cnt_errCnt <= {(PARAM_ROOT_CNT_LEN){1'b0}};
        else begin
            if(in_ctr_en) begin
                if(in_ctr_init) begin
                        cal_buf_cnt_errCnt <= {(PARAM_ROOT_CNT_LEN){1'b0}};
                end
                else if(cal_buf_cnt_errCnt == ROOT_CNT_LIMIT) begin
                        cal_buf_cnt_errCnt <= cal_buf_cnt_errCnt;
                end
                /*else if(w_equal) begin
                        cal_buf_cnt_errCnt <= cal_buf_cnt_errCnt + 1;
                end
                else begin
                        cal_buf_cnt_errCnt <= cal_buf_cnt_errCnt;
                end*/
                else begin
                        cal_buf_cnt_errCnt <= cal_buf_cnt_errCnt + w_equal_AL_sum;
                end
            end
            else begin
                        cal_buf_cnt_errCnt <= cal_buf_cnt_errCnt;
            end
        end
    end

    generate
        /*Error counter output buffer enabled*/
        if(LO_PA_ERR_CNT_OUT_BUF_EN) begin : gen_erCnt_outBuf
            reg     [PARAM_ROOT_CNT_LEN - 1 : 0]    out_buf_cnt_errCnt;

            always@(posedge clk) begin
                if(in_ctr_Srst) out_buf_cnt_errCnt <= {(PARAM_ROOT_CNT_LEN){1'b0}};
                else begin
                    if(in_ctr_en) begin
                        if(in_ctr_done) begin //get lastest cnt
                            if(|w_equal_AL_sum) out_buf_cnt_errCnt <= cal_buf_cnt_errCnt + w_equal_AL_sum;
                            else                out_buf_cnt_errCnt <= cal_buf_cnt_errCnt;
                            /*if(w_equal) begin
                                    out_buf_cnt_errCnt <= cal_buf_cnt_errCnt + 1;
                            end
                            else begin
                                    out_buf_cnt_errCnt <= cal_buf_cnt_errCnt;
                            end*/
                        end
                        else    out_buf_cnt_errCnt <= out_buf_cnt_errCnt;
                    end
                    else    out_buf_cnt_errCnt <= out_buf_cnt_errCnt;
                end
            end

            assign  out_errCnt = out_buf_cnt_errCnt;

        end
        else begin : wiring_erCnt_out
            /*assign  out_errCnt = (
                in_ctr_done ? 
                (w_equal ? cal_buf_cnt_errCnt + 1 : cal_buf_cnt_errCnt) : 
                out_buf_cnt_errCnt
            );*/
            assign  out_errCnt = (
                in_ctr_done ? 
                cal_buf_cnt_errCnt + w_equal_AL_sum : 
                {(PARAM_ROOT_CNT_LEN){1'b0}}
            );
            
            //assign  out_errCnt = out_buf_cnt_errCnt;
        end
    endgenerate


    /*****************************************************/
    initial begin
        $display("/**************************************************/");
        $display("Instanciating %s", DESIGN_NAME);

        $display("In %s," ,OUTTER_NAME);
        $display("MODULE_NAME : %s", MODULE_NAME);
        $display("DESIGN_NAME : %s", DESIGN_NAME);

        $display("PARALLELISM=%d",PARALLELISM);
        $display("EQUATION_POW=%d",EQUATION_POW);
        $display("PARAM_EQATION_COEFFI_NUM = %d", PARAM_EQATION_COEFFI_NUM);

        $display("GF_LEN=%d",GF_LEN);

        $display("PARAM_ROOT_CNT_LEN = %d", PARAM_ROOT_CNT_LEN);
        $display("ROOT_CNT_LIMIT=%d",ROOT_CNT_LIMIT);

        $display("EQUAL_BUF_EN=%d",EQUAL_BUF_EN);

        $display("DONE_COEF_BUF_EN=%d",DONE_COEF_BUF_EN);

        $display("DECISION_MODE=%d",DECISION_MODE);

        $display("ERR_CNT_OUT_BUF_EN = %d", ERR_CNT_OUT_BUF_EN);
        $display("LO_PA_ERR_CNT_OUT_FORCELY_EN : %s", 
            LO_PA_ERR_CNT_OUT_FORCELY_EN ? "enabled" : "disabled");
        $display("LO_PA_ERR_CNT_OUT_BUF_EN : %s", 
            LO_PA_ERR_CNT_OUT_BUF_EN ? "enabled" : "disabled");
        $display("/**************************************************/");
    end

endmodule
`endif
