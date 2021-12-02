/* Front controller control LLR magnitude converter,
 * Syndrome computation and Test Syndrom Computation 
 * Block. Between LLR magnitude converter and Syndrom
 * Computation Blocks, 1 clk delay is inserted.
 * That is mean, F/Fs are inserted.*/
/*
 * Block1 is magnitude
 * Block2 is syndrome and test syndrome computation
*/
/* init signal
 * en sig   -----------------_________-------
 * bl1 init ____-_______...______-----_______
 * bl2 init _____-______..._______-----______
 * ctr init ____-_______...______-----_______
 * 
 * done signal
 * en sig   -----------------_________-------
 * bl1 done ____-_______...______-----_______
 * bl2 done _____-______..._______-----______
 * ctr done _____-______..._______-----______
*/
/************************************************************************/
/************************************************************************/
`ifndef CTR_GENERAL
`define CTR_GENERAL

`include "60_fifo_channel/fifo_multi_ch.v"
`include "60_fifo_channel/fifo_single_ch.v"
`include "71_ctr_time/ctr_cnt/ctr_cnt.v"
`include "71_ctr_time/ctr_enabler/ctr_enabler.v"
/* 
 * cycle starts from 1 and stops at EN_CLK_CYCLE
 */
/************************************************************************/
/************************************************************************/

module controller_general
(
    clk,
    in_ctr_Srst,
    in_ctr_en,
    in_ctr_init,

    out_bus_ctr_bl_init,
    out_bus_ctr_bl_done,
    out_bus_ctr_bl_last,
    out_bus_ctr_bl_en,

    out_ctr_ptl_init,
    out_ctr_ptl_last,
    
    out_ctr_done
);
/************************************************************************/
/************************************************************************/
    parameter   OUTTER_NAME   =   "";
    parameter   MODULE_NAME = "controller_general";
        localparam  DESIGN_NAME = "controller_general";
    parameter   INPUT_CTR_BUF       =   0;

    parameter   PARALLELISM     =   1;
        localparam PARAM_PARALLEL = neverBeingZero(PARALLELISM);

    parameter   OP_CLK_CYCLE    =   1023;
        /* Parallelism start */
        localparam  PARAM_QUOTIENT = OP_CLK_CYCLE / PARAM_PARALLEL;
        localparam  PARAM_REDUNDANT = OP_CLK_CYCLE % PARAM_PARALLEL;

        localparam  PARAM_LAST_P_LOC = last_p_loc(OP_CLK_CYCLE, PARAM_REDUNDANT, PARAM_PARALLEL);


        localparam  PARAM_OP_CLK_CYCLE  =   cal_op_clk(OP_CLK_CYCLE, PARAM_PARALLEL);


        localparam  PARAM_CLK_CNT_LEN     =   bitWidthCal(PARAM_OP_CLK_CYCLE);
        /* Parallelism end */

        localparam TMP_EXIST_LAST   =   PARAM_OP_CLK_CYCLE % PARALLELISM;

    parameter   HAVING_CLK_DISTANCE_BLK_NUMS    =   1;
    parameter   WORKING_CLK_DISTANCE_DIVIDER    =   1;
        localparam  LO_PARAM_CTR_BLK_NUMS = neverBeingZero(HAVING_CLK_DISTANCE_BLK_NUMS);
        localparam  LO_PARAM_CLK_DISTANCE = zeroOrOneRet_zero(HAVING_CLK_DISTANCE_BLK_NUMS, WORKING_CLK_DISTANCE_DIVIDER);

    /* Partial Processing control */
    parameter   PARTIAL_CTR_EN          =   0;
    parameter   PARTIAL_PHASE_NUM       =   1;
    parameter   PARTIAL_CLK_DISTANCE    =   0;
        localparam TMP_PARTIAL_EXITST_LAST  =   PARAM_OP_CLK_CYCLE % PARTIAL_PHASE_NUM;
        

    /*enabler*/
    parameter   MODULE_ENABLER_EN                   =   0;
        localparam  LO_PARAM_MODULE_ENABLER_EN      =   valueToPosBool(MODULE_ENABLER_EN);

//    parameter   CTR_ENABLER_EN                      =   0;
//        localparam  LO_PARAM_CTR_ENABLER_EN         =   valueToPosBool(CTR_ENABLER_EN);
/************************************************************************/
    genvar i;        

/************************************************************************/
/************************************************************************/
    input           clk;

    input           in_ctr_Srst;
    input           in_ctr_en;
    input           in_ctr_init;

    output  [LO_PARAM_CTR_BLK_NUMS - 1 : 0] out_bus_ctr_bl_init;
    output  [LO_PARAM_CTR_BLK_NUMS - 1 : 0] out_bus_ctr_bl_done;
    output  [LO_PARAM_CTR_BLK_NUMS - 1 : 0] out_bus_ctr_bl_last;
    output  [LO_PARAM_CTR_BLK_NUMS - 1 : 0] out_bus_ctr_bl_en;
   
    output          out_ctr_ptl_init;
    output          out_ctr_ptl_last;

    output          out_ctr_done;

    /************************************************************/
    /************************************************************/
    /************************************************************/
    /************************************************************/
    /************************************************************/
    wire w_ctr_mainCnt_Srst;
    //wire w_ctr_mainCnt_en;
    wire w_ctr_mainCnt_init;

    /************************************************************/
    /************************************************************/
    /************************************************************/
    /************************************************************/
    /************************************************************/
    /************************************************************************/
    /************************************************************************/
    function    integer bitWidthCal;
        input   integer expression_value;

        integer cal_tmp;

        begin
            bitWidthCal = 1;
            for(cal_tmp = 1;  cal_tmp < expression_value; cal_tmp = cal_tmp << 1) begin
                bitWidthCal = bitWidthCal + 1;
            end
        end
    endfunction
    /************************************************************************/
    /************************************************************************/
    /************************************************************************/
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
            function integer log2;
                input [31:0] value;

                begin
                    for(log2 = 0; value > 0; log2 = log2 + 1) begin
                        value = (value >> 1);
                    end
                end
            endfunction
    /************************************************************/
            function integer neverBeingZero;
                input integer value;

                begin
                    if(!value)  neverBeingZero = 1;
                    else        neverBeingZero = value;
                end

            endfunction
    /************************************************************/
            function integer last_p_loc;
                input integer length;
                input integer redundant;
                input integer parallelism;

                integer i;

                begin
                    if(redundant == 0) begin
                        last_p_loc = parallelism;
                    end
                    else begin
                        last_p_loc = redundant;
                    end
                end

            endfunction
    /************************************************************/
    /************************************************************/
            function    integer valueToPosBool;
                input   integer chkValue;

                begin
                    if(chkValue)    valueToPosBool = 1;
                    else            valueToPosBool = 0;
                end
            endfunction
    /************************************************************/
    /************************************************************/
            function    integer valueToNegBool;
                input   integer chkValue;

                begin
                    if(chkValue)    valueToNegBool = 0;
                    else            valueToNegBool = 1;
                end
            endfunction
    /************************************************************/
    /************************************************************/
            function    integer zeroOrOneRet_zero;
                input   integer chkValue;
                input   integer in_returnVal;

                begin
                    if(chkValue == 0)       zeroOrOneRet_zero = 0;
                    else if(chkValue == 1)  zeroOrOneRet_zero = 0;
                    else                    zeroOrOneRet_zero = in_returnVal;
                end
            endfunction
    /************************************************************/
    /************************************************************/
    /************************************************************/
    /************************************************************/
    /************************************************************/
    /************************************************************/
    /************************************************************/

    generate
        if(INPUT_CTR_BUF == 0) begin : noInCtrBuf
            assign w_ctr_mainCnt_Srst = in_ctr_Srst;
//            assign w_ctr_mainCnt_en = in_ctr_en;
            assign w_ctr_mainCnt_init = in_ctr_init;
        end
        else begin : yesInCtrBuf
            assign w_ctr_mainCnt_Srst = in_ctr_Srst;
            
            fifo_multi_ch
//            #(.FIFO_LEN(INPUT_CTR_BUF), .FIFO_CH_WIDTH(2))
            #(.FIFO_LEN(INPUT_CTR_BUF), .FIFO_CH_WIDTH(1))
            uut_inCtrBuf
            (
                .clk(clk),

                .in_ctr_Srst(in_ctr_Srst),
                .in_ctr_en(in_ctr_en),

//                .in({in_ctr_en, in_ctr_init}),
                .in(in_ctr_init),
//                .out({w_ctr_mainCnt_en, w_ctr_mainCnt_init})
                .out(w_ctr_mainCnt_init)
            );
        end
    endgenerate

    wire w_ctr_mainCnt_done;
    wire w_ctr_mainCnt_last;

    wire w_ctr_mainCnt_ptl_init;
    wire w_ctr_mainCnt_ptl_last;

    /* main clock counting conrtoller */
    ctr_cnt
    #(
        //.CNT_LENGTH(PARAM_CLK_CNT_LEN), 
        .RST_VAL({(PARAM_CLK_CNT_LEN){1'b0}}), 
        .INIT_VAL({{(PARAM_CLK_CNT_LEN - 1){1'b0}}, 1'b1}),
        .INCREASEMENT(1),
        .OP_CLK_CYCLE(OP_CLK_CYCLE),
        .PARALLELISM(PARALLELISM),

        .PARTIAL_CTR_EN(PARTIAL_CTR_EN),
        .PARTIAL_PHASE_NUM(PARTIAL_PHASE_NUM)
    )
    ctr_main_cnt
    (
        .clk(clk),

        .in_Srst(w_ctr_mainCnt_Srst),
//        .in_en(w_ctr_mainCnt_en),
        .in_en(in_ctr_en),

        .in_init(w_ctr_mainCnt_init),

        .out_cy_cnt(),

        .out_done(w_ctr_mainCnt_done),
        .out_last(w_ctr_mainCnt_last),

        .out_ptl_init(w_ctr_mainCnt_ptl_init),
        .out_ptl_last(w_ctr_mainCnt_ptl_last)
    );
    /************************************************************/
    /************************************************************/
    /************************************************************/
    /***** Module Enabler start *****/
    wire w_ctr_mainModuleEnabler_Srst;
    wire w_ctr_mainModuleEnabler_en;
    wire w_ctr_mainModuleEnabler_init;
    wire w_ctr_mainModuleEnabler_done;

    wire w_ctr_mainEnabler_proc_en;
    wire w_ctr_mainEnabler_module_en;

    generate
        if(LO_PARAM_MODULE_ENABLER_EN != 0) begin :  genEnabler

            assign w_ctr_mainModuleEnabler_Srst = in_ctr_Srst;
            assign w_ctr_mainModuleEnabler_en = in_ctr_en;
            assign w_ctr_mainModuleEnabler_init = w_ctr_mainCnt_init;
            assign w_ctr_mainModuleEnabler_done = w_ctr_mainCnt_done;


            ctr_enabler
            ctr_mainModuleEnabler
            (
                .clk(clk),

                .in_Srst(w_ctr_mainModuleEnabler_Srst),
                .in_en(w_ctr_mainModuleEnabler_en),

                .in_init(w_ctr_mainModuleEnabler_init),
                .in_done(w_ctr_mainModuleEnabler_done),

                .out_module_proc_en(w_ctr_mainEnabler_proc_en),
                .out_module_en(w_ctr_mainEnabler_module_en)
            );
        end
        else begin : notGenEnabler
        end
    endgenerate
    /***** Module Enabler end *****/
    /************************************************************/
    generate
        if(PARTIAL_CLK_DISTANCE == 0)   begin : ptlHasNoDelay
            assign  out_ctr_ptl_init = w_ctr_mainCnt_ptl_init;
            assign  out_ctr_ptl_last = w_ctr_mainCnt_ptl_last;
        end
        else begin : ptlHasDelay
            if(TMP_PARTIAL_EXITST_LAST == 0)    begin : ptlNotHasLastProc
                fifo_single_ch
                #(.FIFO_LEN(PARTIAL_CLK_DISTANCE))
                uut_phsCtlDelay
                (
                    .clk(clk),

                    .in_ctr_Srst(in_ctr_Srst),
                    .in_ctr_en(in_ctr_en),

                    .in(w_ctr_mainCnt_ptl_init),
                    .out(out_ctr_ptl_init)
                );

                assign  out_ctr_ptl_last = w_ctr_mainCnt_ptl_last;
            end
            else begin : ptlHasLastProc
                fifo_multi_ch
                #(.FIFO_LEN(PARTIAL_CLK_DISTANCE), .FIFO_CH_WIDTH(2))
                uut_phsCtlDelay
                (
                    .clk(clk),

                    .in_ctr_Srst(in_ctr_Srst),
                    .in_ctr_en(in_ctr_en),

                    .in({w_ctr_mainCnt_ptl_init, w_ctr_mainCnt_ptl_last}),
                    .out({out_ctr_ptl_init, out_ctr_ptl_last})
                );
            end
        end
    endgenerate
    /************************************************************/
    /************************************************************/
    /************************************************************/
    /************************************************************/
    /************************************************************/
    /* Hard Decision module(Syndrome computation) */
    /* Syndrome computation initial signal */
    wire   [LO_PARAM_CTR_BLK_NUMS - 1 : 0] bus_ctr_bl1_init;
    /*first blk signal assign start*/
    assign  bus_ctr_bl1_init[0] = w_ctr_mainCnt_init;
    /*first blk signal assign end*/
    /*secont ~ last blk signal fifo delay start*/
    /*secont ~ last blk signal fifo delay end*/

    assign  out_bus_ctr_bl_init = bus_ctr_bl1_init;
    /************************************************************/
    wire   [LO_PARAM_CTR_BLK_NUMS - 1 : 0] bus_ctr_bl1_done;
    /*first blk signal assign start*/
    assign  bus_ctr_bl1_done[0] = w_ctr_mainCnt_done;
    /*first blk signal assign end*/
    /*secont ~ last blk signal fifo delay start*/
    /*secont ~ last blk signal fifo delay end*/
    
    assign  out_bus_ctr_bl_done = bus_ctr_bl1_done;
    /************************************************************/
    wire   [LO_PARAM_CTR_BLK_NUMS - 1 : 0] bus_ctr_bl1_last;
    /*first blk signal assign start*/
    assign  bus_ctr_bl1_last[0] = w_ctr_mainCnt_last;
    /*first blk signal assign end*/
    /*secont ~ last blk signal fifo delay start*/
    /*secont ~ last blk signal fifo delay end*/
    
    assign  out_bus_ctr_bl_last = bus_ctr_bl1_last;
    /************************************************************/
    /* Syndrome computation enable signal */
    wire   [LO_PARAM_CTR_BLK_NUMS - 1 : 0] bus_ctr_bl1_en;
    generate
        if(LO_PARAM_MODULE_ENABLER_EN != 0) begin : genBl1EnablerWire
            /*first blk signal assign start*/
            assign  bus_ctr_bl1_en[0] = w_ctr_mainEnabler_module_en;
            /*first blk signal assign end*/
            /*secont ~ last blk signal fifo delay start*/
            /*secont ~ last blk signal fifo delay end*/

            assign  out_bus_ctr_bl_en = bus_ctr_bl1_en;
        end
        else begin : notGenBl1EnablerWire
        end
    endgenerate
    /************************************************************/
    /************************************************************/
    /*control signal delay start*/
    generate
        if(!LO_PARAM_MODULE_ENABLER_EN) begin : gen_blkCtrDelay_Nenabler
            //if(LO_PARAM_CTR_BLK_NUMS - 1 > 0) begin : gen_hasBitWise
/*                shift_reg
                #(
                    .DIRECTION(1),
                    .SHIFT_LEN(1),
                    .BIT_WIDTH(3),

                    .WORKING_CLK_DISTANCE_DIVIDER(LO_PARAM_CLK_DISTANCE)
                )
                uut_shift_reg_clkCtrDelay_NE_0th
                (
                    .clk(clk),

                    .in_ctr_Srst(in_ctr_Srst),
                    .in_ctr_en(in_ctr_en),

                    .in({bus_ctr_bl1_init[0], bus_ctr_bl1_done[0], bus_ctr_bl1_last[0]}
                    .out(out_coef)
                );*/

                for(i=0; i<LO_PARAM_CTR_BLK_NUMS-1; i=i+1) begin:gen_blkCtrFifo_NE
                    fifo_multi_ch
                    #(.FIFO_LEN(LO_PARAM_CLK_DISTANCE), .FIFO_CH_WIDTH(3))
                    fifo_clkCtrDelay_NE
                    (
                        .clk(clk),
                        .in_ctr_Srst(in_ctr_Srst),
                        .in_ctr_en(in_ctr_en),

                        .in({bus_ctr_bl1_init[i], bus_ctr_bl1_done[i], bus_ctr_bl1_last[i]}),
                        .out({bus_ctr_bl1_init[i + 1], bus_ctr_bl1_done[i + 1], bus_ctr_bl1_last[i + 1]})
                     );
/*                    fifo_single_ch
                    #(.FIFO_LEN(LO_PARAM_CLK_DISTANCE))
                    fifi_clkCtrDelay_NE_INIT_SIG
                    (
                        .clk(clk),
                        .in_ctr_Srst(in_ctr_Srst),
                        .in_ctr_en(in_ctr_en),
                        .in(bus_ctr_bl1_init[i]),
                        .out(bus_ctr_bl1_init[i + 1])
                    );

                    fifo_single_ch
                    #(.FIFO_LEN(LO_PARAM_CLK_DISTANCE))
                    fifi_clkCtrDelay_NE_DONE_SIG
                    (
                        .clk(clk),
                        .in_ctr_Srst(in_ctr_Srst),
                        .in_ctr_en(in_ctr_en),
                        .in(bus_ctr_bl1_done[i]),
                        .out(bus_ctr_bl1_done[i + 1])
                    );

                    fifo_single_ch
                    #(.FIFO_LEN(LO_PARAM_CLK_DISTANCE))
                    fifi_clkCtrDelay_NE_LAST_SIG
                    (
                        .clk(clk),
                        .in_ctr_Srst(in_ctr_Srst),
                        .in_ctr_en(in_ctr_en),
                        .in(bus_ctr_bl1_last[i]),
                        .out(bus_ctr_bl1_last[i + 1])
                    );*/

                end
            //end
            //else begin
            //end
        end
        else begin : gen_blkCtrDelay_Yenabler
            if(LO_PARAM_CTR_BLK_NUMS - 1 != 0) begin : gen_hasBitWise

                for(i=0; i<LO_PARAM_CTR_BLK_NUMS-1; i=i+1) begin:gen_blkCtrFifo_YE
                    fifo_multi_ch
                    #(.FIFO_LEN(LO_PARAM_CLK_DISTANCE), .FIFO_CH_WIDTH(3+1))
                    fifo_clkCtrDelay_YE
                    (
                        .clk(clk),
                        .in_ctr_Srst(in_ctr_Srst),
                        .in_ctr_en(in_ctr_en),

                        .in({bus_ctr_bl1_init[i], bus_ctr_bl1_done[i], bus_ctr_bl1_last[i], bus_ctr_bl1_en[i]}),
                        .out({bus_ctr_bl1_init[i + 1], bus_ctr_bl1_done[i + 1], bus_ctr_bl1_last[i + 1], bus_ctr_bl1_en[i + 1]})
                     );
                end
            end
            else begin
            end
        end
    endgenerate
    /*control signal delay end*/
    /************************************************************/
    /************************************************************/
    /************************************************************/
    /************************************************************/
    /************************************************************/

    /* controling end signal */
    assign  out_ctr_done = bus_ctr_bl1_done[LO_PARAM_CTR_BLK_NUMS-1];

/************************************************************************/
/************************************************************************/
    initial begin
        $display("/**************************************************/");
        $display("Instanciating %s", DESIGN_NAME);

        $display("In %s," ,OUTTER_NAME);
        $display("MODULE_NAME : %s", MODULE_NAME);
        $display("DESIGN_NAME : %s", DESIGN_NAME);

        $display("INPUT_CTR_BUF=%d",INPUT_CTR_BUF);
        $display("PARALLELISM=%d",PARALLELISM);
        $display("PARAM_PARALLEL=%d",PARAM_PARALLEL);

        $display("OP_CLK_CYCLE=%d",OP_CLK_CYCLE);
        $display("PARAM_QUOTIENT=%d",PARAM_QUOTIENT);
        $display("PARAM_REDUNDANT=%d",PARAM_REDUNDANT);
        $display("PARAM_LAST_P_LOC=%d",PARAM_LAST_P_LOC);
        $display("PARAM_OP_CLK_CYCLE=%d",PARAM_OP_CLK_CYCLE);
        $display("PARAM_CLK_CNT_LEN=%d",PARAM_CLK_CNT_LEN);

        $display("HAVING_CLK_DISTANCE_BLK_NUMS=%d",HAVING_CLK_DISTANCE_BLK_NUMS);
        $display("WORKING_CLK_DISTANCE_DIVIDER=%d",WORKING_CLK_DISTANCE_DIVIDER);
        $display("LO_PARAM_CTR_BLK_NUMS=%d",LO_PARAM_CTR_BLK_NUMS);
        $display("LO_PARAM_CLK_DISTANCE=%d",LO_PARAM_CLK_DISTANCE);

        $display("PARTIAL_CTR_EN=%d",PARTIAL_CTR_EN);
        $display("PARTIAL_PHASE_NUM=%d",PARTIAL_PHASE_NUM);
        $display("PARTIAL_CLK_DISTANCE=%d",PARTIAL_CLK_DISTANCE);
        $display("TMP_PARTIAL_EXITST_LAST=%d",TMP_PARTIAL_EXITST_LAST);

        $display("MODULE_ENABLER_EN=%d",MODULE_ENABLER_EN);
        $display("LO_PARAM_MODULE_ENABLER_EN=%d",MODULE_ENABLER_EN);
//        $display("CTR_ENABLER_EN=%d",CTR_ENABLER_EN);
//        $display("LO_PARAM_CTR_ENABLER_EN=%d",LO_PARAM_CTR_ENABLER_EN);
        $display("/**************************************************/");
    end
endmodule
`endif
