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
`ifndef CTR_FRONT
`define CTR_FRONT

`include "60_fifo_channel/fifo_multi_ch.v"
`include "71_controller/ctr_cnt/ctr_cnt.v"
`include "71_controller/ctr_enabler/ctr_enabler.v"
/* 
 * cycle starts from 1 and stops at EN_CLK_CYCLE
 */

module controller_front
(
    clk,
    in_ctr_Srst,
    in_ctr_en,
    in_ctr_init,

    out_ctr_bl1_init,
    out_ctr_bl1_done,
    out_ctr_bl1_en,
    out_ctr_bl1_proc_en,

    out_ctr_bl2_init,
    out_ctr_bl2_done,
    out_ctr_bl2_en,
    out_ctr_bl2_proc_en,

    out_ctr_ptl_init,
    out_ctr_ptl_last,
    
    out_ctr_done
);
    parameter   PARALLELISM     =   1;

    parameter   CNT_BIT_LEN     =   10;
    parameter   OP_CLK_CYCLE    =   1023;

    parameter   CLK_DISTANCE    =   1;

    localparam  QUOTIENT        =   OP_CLK_CYCLE / PARALLELISM;
    localparam  REDUNDANT       =   OP_CLK_CYCLE % QUOTIENT;

    localparam  EN_CLK_CYCLE    =   QUOTIENT + REDUNDANT;
    localparam  LAST_PARALLEL_EN_CLK_CYCLE  
                                =   (EN_CLK_CYCLE * PARALLELISM)
                                    + (OP_CLK_CYCLE % EN_CLK_CYCLE);


    /*Not confirmed Supporting Parallelism*/
    parameter   PARTIAL_PHASE_NUM       =   1;

    parameter   PARTIAL_CLK_DISTANCE    =   0;

    localparam  PARTIAL_QUOTIENT        =   OP_CLK_CYCLE / PARTIAL_PHASE_NUM;
    localparam  PARTIAL_REDUNDANT       =   OP_CLK_CYCLE % PARTIAL_PHASE_NUM;

    localparam  PARTIAL_EN_CLK_CYCLE    =
                                           PARTIAL_QUOTIENT + PARTIAL_REDUNDANT;
    localparam  LAST_PARTIAL_EN_CLK_CYCLE
                                        =   (PARTIAL_EN_CLK_CYCLE * PARTIAL_PHASE_NUM)
                                            +  OP_CLK_CYCLE % PARTIAL_EN_CLK_CYCLE;

    genvar i;        



    input           clk;

    input           in_ctr_Srst;
    input           in_ctr_en;
    input           in_ctr_init;

    output          out_ctr_bl1_init;
    output          out_ctr_bl1_done;
    output          out_ctr_bl1_en;
    output          out_ctr_bl1_proc_en;

    output          out_ctr_bl2_init;
    output          out_ctr_bl2_done;
    output          out_ctr_bl2_en;
    output          out_ctr_bl2_proc_en;
    
    output          out_ctr_ptl_init;
    output          out_ctr_ptl_last;

    output          out_ctr_done;

    /* CNT_BIT_LEN calculation */


    /************************************************************/
    /************************************************************/
    /************************************************************/
    /************************************************************/
    /************************************************************/
    /* Hard Decision module(Syndrome computation) */
    /* controller start */
    /* Syndrome computation initial signal */
    wire    ctr_bl1_init;
    assign  ctr_bl1_init = in_ctr_init;

    assign  out_ctr_bl1_init = ctr_bl1_init;
    /************************************************************/
    wire    ctr_bl1_done;
    assign  ctr_bl1_done = (cy_control_cnt == EN_CLK_CYCLE) ? 1 : 0;

    assign  out_ctr_bl1_done = ctr_bl1_done;
    /************************************************************/
    /* Syndrome computation enable signal */
    wire    ctr_bl1_en;
    //assign  ctr_bl1_en = in_ctr_en;
    assign  ctr_bl1_en = in_ctr_en & ctr_bl1_proc_en;

    assign  out_ctr_bl1_en = ctr_bl1_en;
    /************************************************************/
    reg    ctr_bl1_proc_en;
    //assign  ctr_bl1_proc_en = ((0 != cy_control_cnt) && (cy_control_cnt <= EN_CLK_CYCLE)) ? 1 : 0;

    assign  out_ctr_bl1_proc_en = ctr_bl1_proc_en;

    /************************************************************/
    always@(posedge clk)    begin
        if(in_ctr_Srst) ctr_bl1_proc_en <= 0;
        else    begin
            if(in_ctr_en)   begin
                if(ctr_bl1_init)        ctr_bl1_proc_en <= 1;
                else if(ctr_bl1_done)   ctr_bl1_proc_en <= 0;
                else                    ctr_bl1_proc_en <= ctr_bl1_proc_en;
            end
            else                        ctr_bl1_proc_en <= ctr_bl1_proc_en;
        end
    end
    /************************************************************/
    /*partial(TSC phase shifting) signal*/
    wire    ctr_ptl_cal_initWhCase;
    wire    ctr_ptl_cal_last;
    generate
        if((PARTIAL_PHASE_NUM - 1) == 0)    begin : ptlHas1Phase
            wire    ctr_ptl_cal_init;

            assign  ctr_ptl_cal_init = ctr_bl1_done; 
            assign  ctr_ptl_cal_initWhCase = ctr_ptl_cal_init;
        end
        else                                begin : ptlHas2orMorePhase
            wire    [PARTIAL_PHASE_NUM - 1 : 0] ctr_ptl_cal_init;

            assign  ctr_ptl_cal_init[0] = (cy_control_cnt == PARTIAL_EN_CLK_CYCLE) ? 1 : 0;
            for(i = 1; i < PARTIAL_PHASE_NUM; i = i + 1)    begin : ptl2orMP_wire
                assign  ctr_ptl_cal_init[i] = (cy_control_cnt == (PARTIAL_EN_CLK_CYCLE * (i + 1))) ? 1 : 0;
            end

            assign  ctr_ptl_cal_initWhCase = |ctr_ptl_cal_init;

            assign  ctr_ptl_cal_last = (cy_control_cnt >  LAST_PARTIAL_EN_CLK_CYCLE) ? 1 : 0;
        end
    endgenerate
    /************************************************************/
    reg [CNT_BIT_LEN - 1 : 0]           cy_control_cnt;

    always@(posedge clk)
    begin
        if(in_ctr_Srst)                cy_control_cnt <= {(CNT_BIT_LEN){1'b0}};
        else
        begin
            if(in_ctr_en)
            begin
                if(ctr_bl1_init)        cy_control_cnt <= {{(CNT_BIT_LEN - 1){1'b0}}, 1'b1};
                else if(cy_control_cnt == 0) cy_control_cnt <= cy_control_cnt;
                else if(!ctr_bl1_done)  cy_control_cnt <= cy_control_cnt + PARALLELISM;
                else                    cy_control_cnt <= {(CNT_BIT_LEN){1'b0}};
            end
            else                        cy_control_cnt <= cy_control_cnt;
        end
    end
    /************************************************************/
    /************************************************************/
    /*partial(TSC phase shifting) signal*/
    /*partial phase cnt*/
    //reg ...
    /************************************************************/
    /************************************************************/
    wire    ctr_bl2_init;


    assign  out_ctr_bl2_init = ctr_bl2_init;
    /************************************************************/
    wire    ctr_bl2_done;


    assign  out_ctr_bl2_done = ctr_bl2_done;

    /************************************************************/
    wire    ctr_bl2_en;

    assign  ctr_bl2_en = in_ctr_en & ctr_bl2_proc_en;

    assign  out_ctr_bl2_en = ctr_bl2_en;
    /************************************************************/
    wire    ctr_bl2_proc_en;

    assign  out_ctr_bl2_proc_en = ctr_bl2_proc_en;
    /************************************************************/
    /*partial(TSC phase shifting) signal*/
    /************************************************************/
    /************************************************************/
    /************************************************************/
    /*Generate FIfo*/    
    generate
        if(CLK_DISTANCE == 0) begin : bl1and2hasNoDelay
            assign  ctr_bl2_init = ctr_bl1_init;
            assign  ctr_bl2_done = ctr_bl1_done;
            assign  ctr_bl2_en   = ctr_bl1_en;
            assign  ctr_bl2_proc_en = ctr_bl1_proc_en; 
        end
        else begin : bl1and2hasDelay
            fifo_multi_ch
            //#(.FIFO_LEN(1), .FIFO_CH_WIDTH(4))
            //#(.FIFO_LEN(CLK_DISTANCE), .FIFO_CH_WIDTH(2))
            #(.FIFO_LEN(CLK_DISTANCE), .FIFO_CH_WIDTH(3))
            uut_B1B2CtrDelay
            (
                .clk(clk),

                .in_ctr_Srst(in_ctr_Srst),
                .in_ctr_en(in_ctr_en),

                //.in({ctr_bl1_init, ctr_bl1_done, ctr_bl1_en, ctr_bl1_proc_en}),
                //.in({ctr_bl1_init, ctr_bl1_done, ctr_bl1_en}),
                //.in({ctr_bl1_init, ctr_bl1_done}),
                .in({ctr_bl1_init, ctr_bl1_done, ctr_bl1_proc_en}),
                //.out({ctr_bl2_init, ctr_bl2_done, ctr_bl2_en, ctr_bl2_proc_en})
                //.out({ctr_bl2_init, ctr_bl2_done, ctr_bl2_en})
                //.out({ctr_bl2_init, ctr_bl2_done})
                .out({ctr_bl2_init, ctr_bl2_done, ctr_bl2_proc_en})
            );
        end

        if(PARTIAL_CLK_DISTANCE == 0)   begin : ptlHasNoDelay
            assign  out_ctr_ptl_init = ctr_ptl_cal_initWhCase;
        end
        else                            begin : ptlHasDelay
            fifo_multi_ch
            #(.FIFO_LEN(PARTIAL_CLK_DISTANCE), .FIFO_CH_WIDTH(2))
            uut_phsCtlDelay
            (
                .clk(clk),

                .in_ctr_Srst(in_ctr_Srst),
                .in_ctr_en(in_ctr_en),

                .in({ctr_ptl_cal_initWhCase, ctr_ptl_cal_last}),
                .out({out_ctr_ptl_init, out_ctr_ptl_last})
            );
            
        end


    endgenerate
    /* controller end */

    /* controling end signal */
    assign  out_ctr_done = ctr_bl2_done;

endmodule
`endif
