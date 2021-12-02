`ifndef DECISION_UNIT
`define DECISION_UNIT

`include "00_GF_mult/gf_10/comb/GF_Mult_2_10_comb.v"
`include "60_fifo_channel/fifo_multi_ch.v"

/* Chien Search */
`include "05_CS/order_3/sequential/chien_search_series.v"
module decision_unit
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

    in_tp_coef0,
    in_tp_coef1,
    in_tp_coef2,
    in_tp_coef3,

    out_sel_tp_num,
    out_sel_tp_equal

);
    parameter   GF_LEN          =   10;
    parameter   EQUATION_NUM    =   8;
        parameter   EQUATION_NUM_LEN    =   3;
    parameter   EQUATION_DEG    =   3;



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
    parameter   MODE2_COEF1_SHFT    =   10'd16;
    parameter   MODE2_COEF2_SHFT    =   10'd256;
    parameter   MODE2_COEF3_SHFT    =   10'd36;

    parameter   MODE2_RECHIEN_AL_POW0 = 10'd1;
    parameter   MODE2_RECHIEN_AL_POW1 = 10'd2;
    parameter   MODE2_RECHIEN_AL_POW2 = 10'd4;
    parameter   MODE2_RECHIEN_AL_POW3 = 10'd8;


    parameter   DEBUG_MODE      =   0;

    genvar i;

    input   clk;

    input   in_ctr_Srst;
    input   in_ctr_en;

    input   in_ctr_init;
    input   in_ctr_done;

    input   in_ctr_getDeg;
    input   [EQUATION_NUM * (EQUATION_DEG - 1) - 1 : 0] in_tp_deg;
    input   [EQUATION_DEG - 1: 0]                   in_alpha_equal;

    input   [EQUATION_NUM - 1 : 0]   in_tp_equal;
    input   [EQUATION_NUM * ERR_CNT_LEN - 1 : 0]    in_tp_errCnt;

    input   [GF_LEN * EQUATION_NUM - 1 : 0]         in_tp_coef0;
    input   [GF_LEN * EQUATION_NUM - 1 : 0]         in_tp_coef1;
    input   [GF_LEN * EQUATION_NUM - 1 : 0]         in_tp_coef2;
    input   [GF_LEN * EQUATION_NUM - 1 : 0]         in_tp_coef3;


    output  [ERR_CNT_LEN - 1 : 0]                   out_sel_tp_num;
    output  out_sel_tp_equal;

    wire    [EQUATION_NUM - 1 : 0]                  w_degLess2;
    wire    [EQUATION_NUM - 1 : 0]                  w_degViolation;
    wire    [EQUATION_NUM - 1 : 0]                  w_tp_selEn;
        reg [EQUATION_NUM - 1 : 0]                  w_tp_selEnBuf;

    wire    [EQUATION_NUM - 1: 0]                   w_SD_equal;

    wire                                            w_tp_equal_finSeled;


            wire    [EQUATION_NUM - 1 : 0]                  w_mode1_prior_errLoc;
            wire    [EQUATION_NUM - 1 : 0]                  w_mode1_fin_errLocTpHas;
            wire    [EQUATION_NUM - 1 : 0]                  w_mode1FIFOoutEqual;

            wire    [GF_LEN * EQUATION_NUM - 1 : 0]         w_mode2_prior_coef0_TP_has;
            wire    [GF_LEN * EQUATION_NUM - 1 : 0]         w_mode2_prior_coef1_TP_has;
            wire    [GF_LEN * EQUATION_NUM - 1 : 0]         w_mode2_prior_coef2_TP_has;
            wire    [GF_LEN * EQUATION_NUM - 1 : 0]         w_mode2_prior_coef3_TP_has;
            wire    [GF_LEN - 1 : 0]                        w_mode2_finSeledTPcoef0;
            wire    [GF_LEN - 1 : 0]                        w_mode2_finSeledTPcoef1;
            wire    [GF_LEN - 1 : 0]                        w_mode2_finSeledTPcoef2;
            wire    [GF_LEN - 1 : 0]                        w_mode2_finSeledTPcoef3;

            wire    [GF_LEN - 1 : 0]                        w_mode2_shft_coef0;
            wire    [GF_LEN - 1 : 0]                        w_mode2_shft_coef1;
            wire    [GF_LEN - 1 : 0]                        w_mode2_shft_coef2;
            wire    [GF_LEN - 1 : 0]                        w_mode2_shft_coef3;


            wire                                            w_mode2_seledTP_errLoc;

            wire    [EQUATION_NUM - 1: 0]                   w_mode2_prior_SD_equal;





    generate
        /* Generate enable signal to select Correctable Codeword start */
        for(i = 0; i < EQUATION_NUM; i = i + 1) begin : tpEnSignalGen 
            assign  w_degLess2[i] = ~(in_tp_deg[EQUATION_NUM + i] | in_tp_deg[i]);

            assign  w_degViolation[i] = 
                    (
                        w_degLess2[i] ? 
                        1'b0 : 
                        |(in_tp_errCnt[(i * 2) + 1 : (i * 2)]  ^ {in_tp_deg[EQUATION_NUM + i], in_tp_deg[i]})
                    );

            assign  w_tp_selEn[i] = ~w_degViolation[i];
        end

        always@(posedge clk) begin : tp_selEnBuf
            if(in_ctr_Srst)     w_tp_selEnBuf <= {(EQUATION_NUM){1'b0}};
            else if(in_ctr_en) begin
                if(in_ctr_init) w_tp_selEnBuf <= w_tp_selEn;
                else            w_tp_selEnBuf <= w_tp_selEnBuf;
            end
            else                w_tp_selEnBuf <= w_tp_selEnBuf;
            
        end

        /* Generate enable signal to select Correctable Codeword end */


        /* Test Syndrome Chien Search start */
        if(EQUATION_NUM == 8) begin : tpNumIs8_SD_dec
            assign  w_SD_equal[0] = 1'b0;
            assign  w_SD_equal[1] = in_alpha_equal[1];
            assign  w_SD_equal[2] = in_alpha_equal[0] ^ in_alpha_equal[1]; 
            assign  w_SD_equal[3] = in_alpha_equal[0] ^ in_alpha_equal[1] ^ in_alpha_equal[2];
            assign  w_SD_equal[4] = in_alpha_equal[1] ^ in_alpha_equal[2];
            assign  w_SD_equal[5] = in_alpha_equal[2];
            assign  w_SD_equal[6] = in_alpha_equal[0] ^ in_alpha_equal[2];
            assign  w_SD_equal[7] = in_alpha_equal[0];
        end
        else begin : notSupYetTPnumIs8_SD_dec
            for(i = 0; i < EQUATION_NUM; i = i + 1) begin : assi_w_tp_eqWire
                assign  w_SD_equal[i] = 1'b0;
            end
        end
        /* Test Syndrome Chien Search end*/


        /* Chien Search process Regarding Decision mode start */
        if(DECISION_MODE == 1) begin : outMode1Fifo
            fifo_multi_ch
            #(
             
                .FIFO_LEN(MODE1_FIFO_LEN),
                .FIFO_CH_WIDTH(EQUATION_NUM)
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

            /* Coefficient selection based on w_tp_selEn signal start */
            /* coefficient 0 */
            assign  w_mode2_finSeledTPcoef0 = 
                        w_tp_selEn[0] ? 
                        in_tp_coef0[(GF_LEN * (0 + 1)) - 1 : (GF_LEN * (0 + 0))] : 
                        w_mode2_prior_coef0_TP_has[(GF_LEN * (0 + 1)) - 1 : (GF_LEN * (0 + 0))];

            for(i = 1; i < EQUATION_NUM; i = i + 1) begin
                assign  w_mode2_prior_coef0_TP_has[(GF_LEN * (i + 0)) - 1 : (GF_LEN * (i - 1))] = 
                            w_tp_selEn[i] ? 
                            in_tp_coef0[(GF_LEN * (i + 1)) - 1 : (GF_LEN * (i + 0))] : 
                            w_mode2_prior_coef0_TP_has[(GF_LEN * (i + 1)) - 1 : (GF_LEN * (i + 0))];
            end

            assign  w_mode2_prior_coef0_TP_has[GF_LEN * EQUATION_NUM - 1 : GF_LEN * (EQUATION_NUM - 1)] = 
                        {(GF_LEN){1'b0}};

            /* Coefficient 1 */
            assign  w_mode2_finSeledTPcoef1 = 
                        w_tp_selEn[0] ? 
                        in_tp_coef1[(GF_LEN * (0 + 1)) - 1 : (GF_LEN * (0 + 0))] : 
                        w_mode2_prior_coef1_TP_has[(GF_LEN * (0 + 1)) - 1 : (GF_LEN * (0 + 0))];

            for(i = 1; i < EQUATION_NUM; i = i + 1) begin
                assign  w_mode2_prior_coef1_TP_has[(GF_LEN * (i + 0)) - 1 : (GF_LEN * (i - 1))] = 
                            w_tp_selEn[i] ? 
                            in_tp_coef1[(GF_LEN * (i + 1)) - 1 : (GF_LEN * (i + 0))] : 
                            w_mode2_prior_coef1_TP_has[(GF_LEN * (i + 1)) - 1 : (GF_LEN * (i + 0))];
            end

            assign  w_mode2_prior_coef1_TP_has[GF_LEN * EQUATION_NUM - 1 : GF_LEN * (EQUATION_NUM - 1)] = 
                        {(GF_LEN){1'b0}};

            /* Coefficient 2 */
            assign  w_mode2_finSeledTPcoef2 = 
                        w_tp_selEn[0] ? 
                        in_tp_coef2[(GF_LEN * (0 + 1)) - 1 : (GF_LEN * (0 + 0))] : 
                        w_mode2_prior_coef2_TP_has[(GF_LEN * (0 + 1)) - 1 : (GF_LEN * (0 + 0))];

            for(i = 1; i < EQUATION_NUM; i = i + 1) begin
                assign  w_mode2_prior_coef2_TP_has[(GF_LEN * (i + 0)) - 1 : (GF_LEN * (i - 1))] = 
                            w_tp_selEn[i] ? 
                            in_tp_coef2[(GF_LEN * (i + 1)) - 1 : (GF_LEN * (i + 0))] : 
                            w_mode2_prior_coef2_TP_has[(GF_LEN * (i + 1)) - 1 : (GF_LEN * (i + 0))];
            end

            assign  w_mode2_prior_coef2_TP_has[GF_LEN * EQUATION_NUM - 1 : GF_LEN * (EQUATION_NUM - 1)] = 
                        {(GF_LEN){1'b0}};

            /* Coefficient 3 */
            assign  w_mode2_finSeledTPcoef3 = 
                        w_tp_selEn[0] ? 
                        in_tp_coef3[(GF_LEN * (0 + 1)) - 1 : (GF_LEN * (0 + 0))] : 
                        w_mode2_prior_coef3_TP_has[(GF_LEN * (0 + 1)) - 1 : (GF_LEN * (0 + 0))];

            for(i = 1; i < EQUATION_NUM; i = i + 1) begin
                assign  w_mode2_prior_coef3_TP_has[(GF_LEN * (i + 0)) - 1 : (GF_LEN * (i - 1))] = 
                            w_tp_selEn[i] ? 
                            in_tp_coef3[(GF_LEN * (i + 1)) - 1 : (GF_LEN * (i + 0))] : 
                            w_mode2_prior_coef3_TP_has[(GF_LEN * (i + 1)) - 1 : (GF_LEN * (i + 0))];
            end

            assign  w_mode2_prior_coef3_TP_has[GF_LEN * EQUATION_NUM - 1 : GF_LEN * (EQUATION_NUM - 1)] = 
                        {(GF_LEN){1'b0}};

            /* Coefficient selection based on w_tp_selEn signal end */
            assign  w_mode2_shft_coef0 = w_mode2_finSeledTPcoef0;
            GF_Mult_2_10_comb uut_coef1_shft(.out(w_mode2_shft_coef1),.a(w_mode2_finSeledTPcoef1),.b(MODE2_COEF1_SHFT));
            GF_Mult_2_10_comb uut_coef2_shft(.out(w_mode2_shft_coef2),.a(w_mode2_finSeledTPcoef2),.b(MODE2_COEF2_SHFT));
            GF_Mult_2_10_comb uut_coef3_shft(.out(w_mode2_shft_coef3),.a(w_mode2_finSeledTPcoef3),.b(MODE2_COEF3_SHFT));

            chien_search_series
            #(
                .GF_LEN(GF_LEN),

                .ALPHA_POW_0(MODE2_RECHIEN_AL_POW0),
                .ALPHA_POW_1(MODE2_RECHIEN_AL_POW1),
                .ALPHA_POW_2(MODE2_RECHIEN_AL_POW2),
                .ALPHA_POW_3(MODE2_RECHIEN_AL_POW3),

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

                .in_init_coef0(w_mode2_shft_coef0),
                .in_init_coef1(w_mode2_shft_coef1),
                .in_init_coef2(w_mode2_shft_coef2),
                .in_init_coef3(w_mode2_shft_coef3),
               
                .out_w_equal(w_mode2_seledTP_errLoc), 
                .out_equal(),       //not used

                .out_done_coef0(),      //not used
                .out_done_coef1(),      //not used
                .out_done_coef2(),      //not used
                .out_done_coef3()       //not used
            );
        end
        else begin : outModeElseFifo
            fifo_multi_ch
            #(
             
                .FIFO_LEN(MODE1_FIFO_LEN),
                .FIFO_CH_WIDTH(EQUATION_NUM)
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
            /* Decision Out Mode 1 gen HD added SD errLoc start */
            for(i = 0; i < EQUATION_NUM; i = i + 1) begin : genHD_add_SD_equal
                assign  w_mode1_fin_errLocTpHas[i] = w_mode1FIFOoutEqual[i] ^ w_SD_equal[i];
            end
            /* Decision Out Mode 1 gen HD added SD errLoc end */


            assign  w_tp_equal_finSeled = w_tp_selEn[0] ? w_mode1_fin_errLocTpHas[0] : w_mode1_prior_errLoc[0];

            for(i = 1; i < EQUATION_NUM; i = i + 1) begin
                assign  w_mode1_prior_errLoc[i - 1] = w_tp_selEn[i] ? w_mode1_fin_errLocTpHas[i] : w_mode1_prior_errLoc[i];
            end

            assign  w_mode1_prior_errLoc[EQUATION_NUM - 1] = 1'b0;

        end
        else if(DECISION_MODE == 2) begin : outMode2EqualProc
            assign  w_tp_equal_finSeled = 
                        w_mode2_seledTP_errLoc ^ (w_tp_selEnBuf[0] ? w_SD_equal[0] : w_mode2_prior_SD_equal[0]);

            for(i = 1; i < EQUATION_NUM; i = i + 1) begin
                assign  w_mode2_prior_SD_equal[i - 1] = w_tp_selEnBuf[i] ? w_SD_equal[i] : w_mode2_prior_SD_equal[i];
            end

            assign  w_mode2_prior_SD_equal[EQUATION_NUM - 1] = 1'b0;

        end
        else begin : outModeElseEqualProc
            /* Decision Out Mode 1 gen HD added SD errLoc start */
            for(i = 0; i < EQUATION_NUM; i = i + 1) begin : genHD_add_SD_equal
                assign  w_mode1_fin_errLocTpHas[i] = w_mode1FIFOoutEqual[i] ^ w_SD_equal[i];
            end
            /* Decision Out Mode 1 gen HD added SD errLoc end */


            assign  w_tp_equal_finSeled = w_tp_selEn[0] ? w_mode1_fin_errLocTpHas[0] : w_mode1_prior_errLoc[0];

            for(i = 1; i < EQUATION_NUM; i = i + 1) begin
                assign  w_mode1_prior_errLoc[i - 1] = w_tp_selEn[i] ? w_mode1_fin_errLocTpHas[i] : w_mode1_prior_errLoc[i];
            end

            assign  w_mode1_prior_errLoc[EQUATION_NUM - 1] = 1'b0;

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
            reg [ERR_CNT_LEN - 1 : 0]   debug_errCnt[EQUATION_NUM - 1 : 0];
            reg [EQUATION_DEG - 2 : 0]  debug_deg   [EQUATION_NUM - 1 : 0];
            for(i = 0; i < EQUATION_NUM; i = i + 1) begin : genDebug_errWire

                always@(*) begin
                    debug_errCnt[i] <= in_tp_errCnt[(ERR_CNT_LEN * i) + 1 : (ERR_CNT_LEN * i)];

                    debug_deg[i] <= in_tp_deg[(i *EQUATION_DEG ) + 1 : (i * EQUATION_DEG )];
                end


            end

        end
        else begin : debugModeDis
        end
    endgenerate*/


endmodule
`endif
