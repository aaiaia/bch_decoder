`ifndef DECISION_UNIT_2
`define DECISION_UNIT_2

`include "00_GF_mult/gf_10/comb/GF_Mult_2_10_comb.v"
`include "60_fifo_channel/fifo_multi_ch.v"

/* Chien Search */
`include "05_CS/order_3/sequential/chien_search_series.v"
module decision_unit_2
(
    clk,

    in_ctr_Srst,
    in_ctr_en,

    in_ctr_init,
    in_ctr_done,

    in_ctr_getDeg,
    in_tp_deg,
    in_alpha_equal,

    in_tp_equal,
    in_tp_errCnt,

//    in_tp_coef0,
//    in_tp_coef1,
//    in_tp_coef2,
//    in_tp_coef3,
    in_tp_coef,

    out_sel_tp_num,
    out_sel_tp_equal

);
    parameter   OUTTER_NAME =   "";
    parameter   MODULE_NAME =   "decision_unit_2";
        localparam  DESIGN_NAME =   "decision_unit_2";

    parameter   GF_LEN          =   10;
        localparam  PARAM_MAX_GF_LEN    =   pow2(GF_LEN) - 1;

    parameter   GF_SH_FORWARD   =   10'd9;
    parameter   GF_SH_BACKWARD  =   10'd516;

    parameter   PARALLELISM     =   4;
    parameter   SHIFT_LEN       =   1020;
        localparam  PARAM_SHIFT_LEN =   cal_op_clk(SHIFT_LEN, PARALLELISM) - 1;

        localparam  PARAM_COEF_SHIFT_LEN    =   PARAM_MAX_GF_LEN - PARAM_SHIFT_LEN * PARALLELISM;

        localparam  PARAM_REDUNDANT = SHIFT_LEN % PARALLELISM;

    parameter   HD_CORRECTABLE  =   3;
        localparam  PARAM_COEF_NUMS =   HD_CORRECTABLE + 1;
    parameter   SD_CORRECTABLE    =   3;
        localparam PARAM_TP_EQUATION_NUMS    =   pow2(SD_CORRECTABLE);

    parameter   ERR_CNT_LEN     =   2;

    /*
     * 0 is not indicated anything. But FIFO MODE
     * 1 is FIFO,
     * 2 is RE_CHIEN_SEARCH (Not support yet)
     *
    */
    parameter   DECISION_MODE  =   1;
    /* MODE 1 FIFO */
        parameter   MODE1_FIFO_LEN = 1023;

    /* re-chien configuration */
    localparam  PARAM_MODE2_COEF1_SHFT    =   
                    gf_sh_index
                        (
                            {{(GF_LEN - 1){1'b0}}, 1'b1},
                            GF_SH_FORWARD,
                            PARAM_COEF_SHIFT_LEN,
                            1,
                            1,
                            0
                        );
    localparam  PARAM_MODE2_COEF2_SHFT    =   
                    gf_sh_index
                        (
                            {{(GF_LEN - 1){1'b0}}, 1'b1},
                            GF_SH_FORWARD,
                            PARAM_COEF_SHIFT_LEN,
                            1,
                            2,
                            0
                        );
    localparam  PARAM_MODE2_COEF3_SHFT    =   
                    gf_sh_index
                        (
                            {{(GF_LEN - 1){1'b0}}, 1'b1},
                            GF_SH_FORWARD,
                            PARAM_COEF_SHIFT_LEN,
                            1,
                            3,
                            0
                        );


    parameter   DEBUG_MODE      =   0;
    /****************************************************/
    initial begin
        $display("!!! in) %s !!!", OUTTER_NAME);
        $display("!!! %s !!!", MODULE_NAME);
        $display("!!! %s !!!", DESIGN_NAME);
        $display("GF_LEN=%d", GF_LEN);
        $display("GF_SH_FORWARD=%d", GF_SH_FORWARD);
        $display("GF_SH_BACKWARD=%d", GF_SH_BACKWARD);
        $display("PARALLELISM=%d", PARALLELISM);
        $display("SHIFT_LEN=%d", SHIFT_LEN);
        $display("HD_CORRECTABLE=%d", HD_CORRECTABLE);
        $display("SD_CORRECTABLE=%d", SD_CORRECTABLE);
        $display("ERR_CNT_LEN=%d", ERR_CNT_LEN);
        $display("DECISION_MODE=%d", DECISION_MODE);
        $display("MODE1_FIFO_LEN=%d", MODE1_FIFO_LEN);
        $display("DEBUG_MODE=%d", DEBUG_MODE);
    end

    /****************************************************/

    genvar i, j;

    input   clk;

    input   in_ctr_Srst;
    input   in_ctr_en;

    input   in_ctr_init;
    input   in_ctr_done;

    input   in_ctr_getDeg;
    input   [PARAM_TP_EQUATION_NUMS * (SD_CORRECTABLE - 1) - 1 : 0] in_tp_deg;
    input   [SD_CORRECTABLE * PARALLELISM - 1: 0]                   in_alpha_equal;

    input   [PARAM_TP_EQUATION_NUMS * PARALLELISM - 1 : 0]   in_tp_equal;//mode 1 used
    input   [PARAM_TP_EQUATION_NUMS * ERR_CNT_LEN - 1 : 0]    in_tp_errCnt;

//    input   [GF_LEN * PARAM_TP_EQUATION_NUMS - 1 : 0]         in_tp_coef0;
//    input   [GF_LEN * PARAM_TP_EQUATION_NUMS - 1 : 0]         in_tp_coef1;
//    input   [GF_LEN * PARAM_TP_EQUATION_NUMS - 1 : 0]         in_tp_coef2;
//    input   [GF_LEN * PARAM_TP_EQUATION_NUMS - 1 : 0]         in_tp_coef3;
    input   [GF_LEN * PARAM_TP_EQUATION_NUMS * PARAM_COEF_NUMS - 1 : 0]         in_tp_coef;


    output  [ERR_CNT_LEN - 1 : 0]                           out_sel_tp_num;

    output  [PARALLELISM - 1 : 0]                           out_sel_tp_equal;

    wire    [PARAM_TP_EQUATION_NUMS - 1 : 0]                w_degLess2;
    wire    [PARAM_TP_EQUATION_NUMS - 1 : 0]                w_degViolation;
    wire    [PARAM_TP_EQUATION_NUMS - 1 : 0]                w_tp_selEn;
        reg [PARAM_TP_EQUATION_NUMS - 1 : 0]                w_tp_selEnBuf;

    wire    [PARAM_TP_EQUATION_NUMS * PARALLELISM - 1: 0]   w_SD_equal;

    wire    [PARALLELISM - 1 : 0]                           w_tp_equal_finSeled;


            wire    [PARAM_TP_EQUATION_NUMS * PARALLELISM - 1 : 0]  w_mode1_prior_errLoc;
            wire    [PARAM_TP_EQUATION_NUMS * PARALLELISM - 1 : 0]  w_mode1_fin_errLocTpHas;
            wire    [PARAM_TP_EQUATION_NUMS * PARALLELISM - 1 : 0]  w_mode1FIFOoutEqual;

//            wire    [GF_LEN * PARAM_TP_EQUATION_NUMS - 1 : 0]         w_mode2_prior_coef0_TP_has;
//            wire    [GF_LEN * PARAM_TP_EQUATION_NUMS - 1 : 0]         w_mode2_prior_coef1_TP_has;
//            wire    [GF_LEN * PARAM_TP_EQUATION_NUMS - 1 : 0]         w_mode2_prior_coef2_TP_has;
//            wire    [GF_LEN * PARAM_TP_EQUATION_NUMS - 1 : 0]         w_mode2_prior_coef3_TP_has;
            wire    [GF_LEN * PARAM_TP_EQUATION_NUMS * PARAM_COEF_NUMS - 1 : 0]
                                                                        w_mode2_prior_coef_TP_has;

//            wire    [GF_LEN - 1 : 0]                        w_mode2_finSeledTPcoef0;
//            wire    [GF_LEN - 1 : 0]                        w_mode2_finSeledTPcoef1;
//            wire    [GF_LEN - 1 : 0]                        w_mode2_finSeledTPcoef2;
//            wire    [GF_LEN - 1 : 0]                        w_mode2_finSeledTPcoef3;
            wire    [GF_LEN * PARAM_COEF_NUMS - 1 : 0]      w_mode2_finSeledTPcoef;

//            wire    [GF_LEN - 1 : 0]                        w_mode2_shft_coef0;
//            wire    [GF_LEN - 1 : 0]                        w_mode2_shft_coef1;
//            wire    [GF_LEN - 1 : 0]                        w_mode2_shft_coef2;
//            wire    [GF_LEN - 1 : 0]                        w_mode2_shft_coef3;
            wire    [GF_LEN * PARAM_COEF_NUMS - 1 : 0]      w_mode2_shft_coef;


            wire    [PARALLELISM - 1 : 0]                   w_mode2_seledTP_errLoc;

            wire    [PARAM_TP_EQUATION_NUMS * PARALLELISM - 1: 0]         w_mode2_prior_SD_equal;



    /*****************************************************/
    /********************* Functions *********************/
    /*****************************************************/
            function integer pow2;
                input integer value;

                integer i;

                if(!value) begin
                    pow2 = 1;

                    $display("pow2) input value=%d, output value=%d",
                            value, pow2);
                end
                else begin
                    pow2 = 1;
                    for(i = 0; i < value; i = i + 1) begin
                        pow2 = (pow2 << 1);
                    end

                    $display("pow2) input value=%d, output value=%d",
                            value, pow2);
                end
            endfunction
    /*****************************************************/
    /*****************************************************/
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

                    $display("gf_sh_index in DU) print i:[%d], ref_index=%d, gf_sh_index=%d", prt_i, ref_index, gf_sh_index);
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

                    $display("gf_sh_index in DU) print i:[%d], ref_index=%d, gf_sh_index=%d", prt_i, ref_index, gf_sh_index);
                end
            endfunction
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


    generate
        /* Generate enable signal to select Correctable Codeword start */
        for(i = 0; i < PARAM_TP_EQUATION_NUMS; i = i + 1) begin : tpEnSignalGen 
            assign  w_degLess2[i] = ~(in_tp_deg[PARAM_TP_EQUATION_NUMS + i] | in_tp_deg[i]);

            assign  w_degViolation[i] = 
                    (
                        w_degLess2[i] ? 
                        1'b0 : 
                        |(in_tp_errCnt[(i * 2) + 1 : (i * 2)]  ^ {in_tp_deg[PARAM_TP_EQUATION_NUMS + i], in_tp_deg[i]})
                    );

            assign  w_tp_selEn[i] = ~w_degViolation[i];
        end

        always@(posedge clk) begin : tp_selEnBuf
            if(in_ctr_Srst)     w_tp_selEnBuf <= {(PARAM_TP_EQUATION_NUMS){1'b0}};
            else if(in_ctr_en) begin
                if(in_ctr_init) w_tp_selEnBuf <= w_tp_selEn;
                else            w_tp_selEnBuf <= w_tp_selEnBuf;
            end
            else                w_tp_selEnBuf <= w_tp_selEnBuf;
            
        end

        /* Generate enable signal to select Correctable Codeword end */


        /* Test Syndrome Chien Search start */
        if(PARAM_TP_EQUATION_NUMS == 8) begin : tpNumIs8_SD_dec
            for(i = 0; i < PARALLELISM; i = i + 1) begin : gen_YP_tpNumIs8_SD_dec
                assign  w_SD_equal[0 + i * PARAM_TP_EQUATION_NUMS] = 
                                                                    1'b0;

                assign  w_SD_equal[1 + i * PARAM_TP_EQUATION_NUMS] = 
                                    in_alpha_equal[1 + i * SD_CORRECTABLE];

                assign  w_SD_equal[2 + i * PARAM_TP_EQUATION_NUMS] = 
                                    in_alpha_equal[0 + i * SD_CORRECTABLE] ^ 
                                    in_alpha_equal[1 + i * SD_CORRECTABLE];

                assign  w_SD_equal[3 + i * PARAM_TP_EQUATION_NUMS] = 
                                    in_alpha_equal[0 + i * SD_CORRECTABLE] ^ 
                                    in_alpha_equal[1 + i * SD_CORRECTABLE] ^ 
                                    in_alpha_equal[2 + i * SD_CORRECTABLE];

                assign  w_SD_equal[4 + i * PARAM_TP_EQUATION_NUMS] = 
                                    in_alpha_equal[1 + i * SD_CORRECTABLE] ^ 
                                    in_alpha_equal[2 + i * SD_CORRECTABLE];

                assign  w_SD_equal[5 + i * PARAM_TP_EQUATION_NUMS] = 
                                    in_alpha_equal[2 + i * SD_CORRECTABLE];

                assign  w_SD_equal[6 + i * PARAM_TP_EQUATION_NUMS] = 
                                    in_alpha_equal[0 + i * SD_CORRECTABLE] ^ 
                                    in_alpha_equal[2 + i * SD_CORRECTABLE];

                assign  w_SD_equal[7 + i * PARAM_TP_EQUATION_NUMS] = 
                                    in_alpha_equal[0 + i * SD_CORRECTABLE];
                initial begin
                    $display("DU) p=%d, assign sd_equal", i);
                    $display("p[%d], w_SD_equal[%d]=none", 
                                    i, 0 + i * PARAM_TP_EQUATION_NUMS);
                    $display("p[%d], w_SD_equal[%d]=in_alpha_equal [%d]", 
                                    i, 1 + i * PARAM_TP_EQUATION_NUMS,
                                    1 + i * SD_CORRECTABLE
                                );
                    $display("p[%d], w_SD_equal[%d]=in_alpha_equal [%d], [%d]", 
                                    i, 2 + i * PARAM_TP_EQUATION_NUMS,
                                    0 + i * SD_CORRECTABLE,
                                    1 + i * SD_CORRECTABLE
                                );
                    $display("p[%d], w_SD_equal[%d]=in_alpha_equal [%d], [%d], [%d]", 
                                    i, 3 + i * PARAM_TP_EQUATION_NUMS,
                                    0 + i * SD_CORRECTABLE,
                                    1 + i * SD_CORRECTABLE,
                                    2 + i * SD_CORRECTABLE
                                );
                    $display("p[%d], w_SD_equal[%d]=in_alpha_equal [%d], [%d]", 
                                    i, 4 + i * PARAM_TP_EQUATION_NUMS,
                                    1 + i * SD_CORRECTABLE,
                                    2 + i * SD_CORRECTABLE
                                );
                    $display("p[%d], w_SD_equal[%d]=in_alpha_equal [%d]", 
                                    i, 5 + i * PARAM_TP_EQUATION_NUMS,
                                    2 + i * SD_CORRECTABLE
                                );
                    $display("p[%d], w_SD_equal[%d]=in_alpha_equal [%d], [%d]", 
                                    i, 6 + i * PARAM_TP_EQUATION_NUMS,
                                    0 + i * SD_CORRECTABLE,
                                    2 + i * SD_CORRECTABLE
                                );
                    $display("p[%d], w_SD_equal[%d]=in_alpha_equal [%d]", 
                                    i, 7 + i * PARAM_TP_EQUATION_NUMS,
                                    0 + i * SD_CORRECTABLE
                                );
                end
            end

        end
        else begin : notSupYetTPnumIs8_SD_dec
            for(j = 0; j < PARALLELISM; j = j + 1) begin : gen_YP_assi_w_tp_eqWire
                for(i = 0; i < PARAM_TP_EQUATION_NUMS; i = i + 1) begin : assi_w_tp_eqWire
                    assign  w_SD_equal[i + j * PARAM_TP_EQUATION_NUMS] = 1'b0;
                end
            end
            initial $display("\n!!! WARNING !!! in decision_unit_2, label notSupYetTPnumIs8_SD_dec is not supported!!!!\n");
        end
        /* Test Syndrome Chien Search end*/


        /* Chien Search process Regarding Decision mode start */
        if(DECISION_MODE == 1) begin : outMode1Fifo
            fifo_multi_ch
            #(
             
                .FIFO_LEN(MODE1_FIFO_LEN),
                .FIFO_CH_WIDTH(PARAM_TP_EQUATION_NUMS * PARALLELISM)
            )
            uut_fifo_tp_equal
            (
                .clk(clk),

                .in_ctr_Srst(in_ctr_Srst),
                .in_ctr_en(in_ctr_en),

                .in(in_tp_equal),
                .out(w_mode1FIFOoutEqual)
            );
        end
        else if(DECISION_MODE == 2) begin : outMode2reChien
            /*Low side wire has a high priority.*/
            /* Coefficient selection based on w_tp_selEn signal start */
            assign  w_mode2_finSeledTPcoef = 
                        w_tp_selEn[0] ? 
                        in_tp_coef
                            [
                                (GF_LEN * (PARAM_COEF_NUMS * 1)) - 1 : 
                                (GF_LEN * (PARAM_COEF_NUMS * 0))
                            ] : 
                        w_mode2_prior_coef_TP_has
                            [
                                (GF_LEN * (PARAM_COEF_NUMS * 1)) - 1 : 
                                (GF_LEN * (PARAM_COEF_NUMS * 0))
                            ];

            for(i = 1; i < PARAM_TP_EQUATION_NUMS; i = i + 1) begin : gen_mode2_prior_coef_tp
                assign  w_mode2_prior_coef_TP_has
                            [
                                (GF_LEN * (PARAM_COEF_NUMS * (i + 0))) - 1 : 
                                (GF_LEN * (PARAM_COEF_NUMS * (i - 1)))
                            ] = 
                            w_tp_selEn[i] ? 
                            in_tp_coef
                                [
                                    (GF_LEN * (PARAM_COEF_NUMS * (i + 1))) - 1 : 
                                    (GF_LEN * (PARAM_COEF_NUMS * (i + 0)))
                                ] : 
                            w_mode2_prior_coef_TP_has
                                [
                                    (GF_LEN * (PARAM_COEF_NUMS * (i + 1))) - 1 : 
                                    (GF_LEN * (PARAM_COEF_NUMS * (i + 0)))
                                ];
            end

            assign  w_mode2_prior_coef_TP_has
                        [
                            GF_LEN * PARAM_COEF_NUMS * (PARAM_TP_EQUATION_NUMS + 0) - 1 : 
                            GF_LEN * PARAM_COEF_NUMS * (PARAM_TP_EQUATION_NUMS - 1)
                        ] = 
                            {(GF_LEN * PARAM_COEF_NUMS){1'b0}};

            /* coefficient 0 */
/*
            assign  w_mode2_finSeledTPcoef0 = 
                        w_tp_selEn[0] ? 
                        in_tp_coef0[(GF_LEN * (0 + 1)) - 1 : (GF_LEN * (0 + 0))] : 
                        w_mode2_prior_coef0_TP_has[(GF_LEN * (0 + 1)) - 1 : (GF_LEN * (0 + 0))];

            for(i = 1; i < PARAM_TP_EQUATION_NUMS; i = i + 1) begin
                assign  w_mode2_prior_coef0_TP_has[(GF_LEN * (i + 0)) - 1 : (GF_LEN * (i - 1))] = 
                            w_tp_selEn[i] ? 
                            in_tp_coef0[(GF_LEN * (i + 1)) - 1 : (GF_LEN * (i + 0))] : 
                            w_mode2_prior_coef0_TP_has[(GF_LEN * (i + 1)) - 1 : (GF_LEN * (i + 0))];
            end

            assign  w_mode2_prior_coef0_TP_has[GF_LEN * PARAM_TP_EQUATION_NUMS - 1 : GF_LEN * (PARAM_TP_EQUATION_NUMS - 1)] = 
                        {(GF_LEN){1'b0}};
*/
            /* Coefficient 1 */
/*
            assign  w_mode2_finSeledTPcoef1 = 
                        w_tp_selEn[0] ? 
                        in_tp_coef1[(GF_LEN * (0 + 1)) - 1 : (GF_LEN * (0 + 0))] : 
                        w_mode2_prior_coef1_TP_has[(GF_LEN * (0 + 1)) - 1 : (GF_LEN * (0 + 0))];

            for(i = 1; i < PARAM_TP_EQUATION_NUMS; i = i + 1) begin
                assign  w_mode2_prior_coef1_TP_has[(GF_LEN * (i + 0)) - 1 : (GF_LEN * (i - 1))] = 
                            w_tp_selEn[i] ? 
                            in_tp_coef1[(GF_LEN * (i + 1)) - 1 : (GF_LEN * (i + 0))] : 
                            w_mode2_prior_coef1_TP_has[(GF_LEN * (i + 1)) - 1 : (GF_LEN * (i + 0))];
            end

            assign  w_mode2_prior_coef1_TP_has[GF_LEN * PARAM_TP_EQUATION_NUMS - 1 : GF_LEN * (PARAM_TP_EQUATION_NUMS - 1)] = 
                        {(GF_LEN){1'b0}};
*/
            /* Coefficient 2 */
/*
            assign  w_mode2_finSeledTPcoef2 = 
                        w_tp_selEn[0] ? 
                        in_tp_coef2[(GF_LEN * (0 + 1)) - 1 : (GF_LEN * (0 + 0))] : 
                        w_mode2_prior_coef2_TP_has[(GF_LEN * (0 + 1)) - 1 : (GF_LEN * (0 + 0))];

            for(i = 1; i < PARAM_TP_EQUATION_NUMS; i = i + 1) begin
                assign  w_mode2_prior_coef2_TP_has[(GF_LEN * (i + 0)) - 1 : (GF_LEN * (i - 1))] = 
                            w_tp_selEn[i] ? 
                            in_tp_coef2[(GF_LEN * (i + 1)) - 1 : (GF_LEN * (i + 0))] : 
                            w_mode2_prior_coef2_TP_has[(GF_LEN * (i + 1)) - 1 : (GF_LEN * (i + 0))];
            end

            assign  w_mode2_prior_coef2_TP_has[GF_LEN * PARAM_TP_EQUATION_NUMS - 1 : GF_LEN * (PARAM_TP_EQUATION_NUMS - 1)] = 
                        {(GF_LEN){1'b0}};
*/
            /* Coefficient 3 */
/*
            assign  w_mode2_finSeledTPcoef3 = 
                        w_tp_selEn[0] ? 
                        in_tp_coef3[(GF_LEN * (0 + 1)) - 1 : (GF_LEN * (0 + 0))] : 
                        w_mode2_prior_coef3_TP_has[(GF_LEN * (0 + 1)) - 1 : (GF_LEN * (0 + 0))];

            for(i = 1; i < PARAM_TP_EQUATION_NUMS; i = i + 1) begin
                assign  w_mode2_prior_coef3_TP_has[(GF_LEN * (i + 0)) - 1 : (GF_LEN * (i - 1))] = 
                            w_tp_selEn[i] ? 
                            in_tp_coef3[(GF_LEN * (i + 1)) - 1 : (GF_LEN * (i + 0))] : 
                            w_mode2_prior_coef3_TP_has[(GF_LEN * (i + 1)) - 1 : (GF_LEN * (i + 0))];
            end

            assign  w_mode2_prior_coef3_TP_has[GF_LEN * PARAM_TP_EQUATION_NUMS - 1 : GF_LEN * (PARAM_TP_EQUATION_NUMS - 1)] = 
                        {(GF_LEN){1'b0}};
*/
            /* Coefficient selection based on w_tp_selEn signal end */
/*
            assign  w_mode2_shft_coef0 = w_mode2_finSeledTPcoef0;
            GF_Mult_2_10_comb uut_coef1_shft(.out(w_mode2_shft_coef1),.a(w_mode2_finSeledTPcoef1),.b(PARAM_MODE2_COEF1_SHFT));
            GF_Mult_2_10_comb uut_coef2_shft(.out(w_mode2_shft_coef2),.a(w_mode2_finSeledTPcoef2),.b(PARAM_MODE2_COEF2_SHFT));
            GF_Mult_2_10_comb uut_coef3_shft(.out(w_mode2_shft_coef3),.a(w_mode2_finSeledTPcoef3),.b(PARAM_MODE2_COEF3_SHFT));
*/
            assign  w_mode2_shft_coef[GF_LEN * (0 + 1) - 1 : GF_LEN * 0] = 
                        w_mode2_finSeledTPcoef[GF_LEN * (0 + 1) - 1 : GF_LEN * 0];
            GF_Mult_2_10_comb uut_coef1_shft
                (
                    .out(w_mode2_shft_coef[GF_LEN * (1 + 1) - 1 : GF_LEN * 1]),
                    .a(w_mode2_finSeledTPcoef[GF_LEN * (1 + 1) - 1 : GF_LEN * 1]),
                    .b(PARAM_MODE2_COEF1_SHFT)
                );
            GF_Mult_2_10_comb uut_coef2_shft
                (
                    .out(w_mode2_shft_coef[GF_LEN * (2 + 1) - 1 : GF_LEN * 2]),
                    .a(w_mode2_finSeledTPcoef[GF_LEN * (2 + 1) - 1 : GF_LEN * 2]),
                    .b(PARAM_MODE2_COEF2_SHFT)
                );
            GF_Mult_2_10_comb uut_coef3_shft
                (
                    .out(w_mode2_shft_coef[GF_LEN * (3 + 1) - 1 : GF_LEN * 3]),
                    .a(w_mode2_finSeledTPcoef[GF_LEN * (3 + 1) - 1 : GF_LEN * 3]),
                    .b(PARAM_MODE2_COEF3_SHFT)
                );

            chien_search_series
            #(
                .OUTTER_NAME(MODULE_NAME),
                .MODULE_NAME("uut_re_chienSearch"),

                .GF_LEN(GF_LEN),

                .PARALLELISM(PARALLELISM),

                .EQUAL_BUF_EN(0),

                .DONE_COEF_BUF_EN(0)

            )
            uut_re_chienSearch
            (
                .in_ctr_Srst(in_ctr_Srst),
                .in_ctr_en(in_ctr_en),
                .in_ctr_init(in_ctr_init),
                .in_ctr_done(1'b0),

                .clk(clk),

                .in_init_coef(w_mode2_shft_coef),
               
                .out_w_equal(w_mode2_seledTP_errLoc), 
                .out_equal(),       //not used

                .out_done_coef()      //not used
            );
        end
        else begin : outModeElseFifo
            fifo_multi_ch
            #(
             
                .FIFO_LEN(MODE1_FIFO_LEN),
                .FIFO_CH_WIDTH(PARAM_TP_EQUATION_NUMS * PARALLELISM)
            )
            uut_fifo_tp_equal
            (
                .clk(clk),

                .in_ctr_Srst(in_ctr_Srst),
                .in_ctr_en(in_ctr_en),

                .in(in_tp_equal),
                .out(w_mode1FIFOoutEqual)
            );
        end
        /* Chien Search process Regarding Decision mode end */

    endgenerate



    /* Select Test Pattern or coefficient of test pattern, it has priority start */
    generate

        if(DECISION_MODE == 1) begin : outMode1EqualProc
            initial $display("DU) mode1 canditate and LRB sum , %d-parallel", PARALLELISM);
            for(j = 0; j < PARALLELISM; j = j + 1) begin : gen_YP_DU_mode1
            /* Decision Out Mode 1 gen HD added SD errLoc start */
                for(i = 0; i < PARAM_TP_EQUATION_NUMS; i = i + 1) begin : genHD_add_SD_equal
                    assign  w_mode1_fin_errLocTpHas[j * PARAM_TP_EQUATION_NUMS + i] = 
                                w_mode1FIFOoutEqual[j] ^ w_SD_equal[j * PARAM_TP_EQUATION_NUMS +i];
                end
            /* Decision Out Mode 1 gen HD added SD errLoc end */


            /*Low side wire has a high priority.*/
                assign  w_tp_equal_finSeled[j] = 
                            w_tp_selEn[0] ? 
                            w_mode1_fin_errLocTpHas[j * PARAM_TP_EQUATION_NUMS + 0] : 
                            w_mode1_prior_errLoc[j * PARAM_TP_EQUATION_NUMS + 0];

                for(i = 1; i < PARAM_TP_EQUATION_NUMS; i = i + 1) begin : gen_YP_DU_mode1_prior_tpSel
                    assign  w_mode1_prior_errLoc[j * PARAM_TP_EQUATION_NUMS + i - 1] = 
                                w_tp_selEn[i] ? 
                                w_mode1_fin_errLocTpHas[j * PARAM_TP_EQUATION_NUMS + i] : 
                                w_mode1_prior_errLoc[j * PARAM_TP_EQUATION_NUMS + i];
                end

                assign  w_mode1_prior_errLoc
                            [j * PARAM_TP_EQUATION_NUMS + PARAM_TP_EQUATION_NUMS - 1] = 1'b0;
            end

        end
        else if(DECISION_MODE == 2) begin : outMode2EqualProc
            initial $display("DU) mode 2 canditate and LRB sum , %d-parallel", PARALLELISM);
            for(j = 0; j < PARALLELISM; j = j + 1) begin : gen_YP_DU_mode2 
                assign  w_tp_equal_finSeled[j] = 
                            w_mode2_seledTP_errLoc[j] ^ 
                                (   w_tp_selEnBuf[0] ? 
                                    w_SD_equal[j * PARAM_TP_EQUATION_NUMS + 0] : 
                                    w_mode2_prior_SD_equal[j * PARAM_TP_EQUATION_NUMS + 0]  );

                for(i = 1; i < PARAM_TP_EQUATION_NUMS; i = i + 1) begin : gen_YP_DU_mode2_prior_tpSel
                    assign  w_mode2_prior_SD_equal[j * PARAM_TP_EQUATION_NUMS + (i - 1)] = 
                                w_tp_selEnBuf[i] ? 
                                w_SD_equal[j * PARAM_TP_EQUATION_NUMS + i] : 
                                w_mode2_prior_SD_equal[j * PARAM_TP_EQUATION_NUMS + i];
                end

                assign  w_mode2_prior_SD_equal
                            [j * PARAM_TP_EQUATION_NUMS + PARAM_TP_EQUATION_NUMS - 1] = 1'b0;
            end

        end
        else begin : outModeElseEqualProc
            initial $display("DU) mode etc(=1) canditate and LRB sum , %d-parallel", PARALLELISM);
            for(j = 0; j < PARALLELISM; j = j + 1) begin : gen_YP_DU_mode1
            /* Decision Out Mode 1 gen HD added SD errLoc start */
                for(i = 0; i < PARAM_TP_EQUATION_NUMS; i = i + 1) begin : genHD_add_SD_equal
                    assign  w_mode1_fin_errLocTpHas[j * PARAM_TP_EQUATION_NUMS + i] = 
                                w_mode1FIFOoutEqual[j] ^ w_SD_equal[j * PARAM_TP_EQUATION_NUMS +i];
                end
            /* Decision Out Mode 1 gen HD added SD errLoc end */


                assign  w_tp_equal_finSeled[j] = 
                            w_tp_selEn[0] ? 
                            w_mode1_fin_errLocTpHas[j * PARAM_TP_EQUATION_NUMS + 0] : 
                            w_mode1_prior_errLoc[j * PARAM_TP_EQUATION_NUMS + 0];

                for(i = 1; i < PARAM_TP_EQUATION_NUMS; i = i + 1) begin : gen_YP_DU_mode1_prior_tpSel
                    assign  w_mode1_prior_errLoc[j * PARAM_TP_EQUATION_NUMS + i - 1] = 
                                w_tp_selEn[i] ? 
                                w_mode1_fin_errLocTpHas[j * PARAM_TP_EQUATION_NUMS + i] : 
                                w_mode1_prior_errLoc[j * PARAM_TP_EQUATION_NUMS + i];
                end

                assign  w_mode1_prior_errLoc
                            [j * PARAM_TP_EQUATION_NUMS + PARAM_TP_EQUATION_NUMS - 1] = 1'b0;
            end

        end
    endgenerate
    /* Select Test Pattern or coefficient of test pattern, it has priority end*/


    assign  out_sel_tp_equal = w_tp_equal_finSeled;


/*    generate
        if(DEBUG_MODE !=0) begin : debugCnt
            integer debug_clkCnt;

            always@(posedge clk) begin : debugCntFunc
                if(in_ctr_Srst)     debug_clkCnt = 0;
                else begin
                    if(in_ctr_en) begin
                        if(in_ctr_init)
                                    debug_clkCnt <= 1;
                        else if(in_ctr_done)    
                                    debug_clkCnt <= 0;
                        else        debug_clkCnt <= debug_clkCnt + 1;
                    end
                    else            debug_clkCnt <= debug_clkCnt;
                end
            end
        end

            if(DEBUG_MODE != 0) begin : debugModeEn
            reg [ERR_CNT_LEN - 1 : 0]   debug_errCnt[PARAM_TP_EQUATION_NUMS - 1 : 0];
            reg [SD_CORRECTABLE - 2 : 0]  debug_deg   [PARAM_TP_EQUATION_NUMS - 1 : 0];
            for(i = 0; i < PARAM_TP_EQUATION_NUMS; i = i + 1) begin : genDebug_errWire

                always@(*) begin
                    debug_errCnt[i] <= in_tp_errCnt[(ERR_CNT_LEN * i) + 1 : (ERR_CNT_LEN * i)];

                    debug_deg[i] <= in_tp_deg[(i *SD_CORRECTABLE ) + 1 : (i * SD_CORRECTABLE )];
                end


            end

        end
        else begin : debugModeDis
        end
    endgenerate*/


endmodule
`endif
