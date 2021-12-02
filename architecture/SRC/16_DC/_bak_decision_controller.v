`ifndef DECISION_CTR
`define DECISION_CTR

`include "60_fifo_channel/fifo_multi_ch.v"
`include "60_fifo_channel/fifo_single_ch.v"
`include "16_DC/decision_infoLoader.v"

module decision_ctr
(
    clk,
    in_ctr_Srst,
    in_ctr_en,

    in_ctr_getVal_deg,
    in_tp_deg,
    in_ctr_outVel_deg,
    out_tp_deg,

    in_ctr_getVal_erCnt,
    in_tp_erCnt,
    in_ctr_outVal_erCnt,
    out_tp_erCnt
);
    /***********************************************************************/
    /***********************************************************************/
    function integer    notBeingZero;
        input   integer value;

        begin
            if(value > 0)   notBeingZero = value;
            else            notBeingZero = 1;
        end
    endfunction
    /***********************************************************************/
    function integer    valueToBool;
        input   integer value;

        begin
            if(value)   valueToBool = 1;
            else        valueToBool = 0;
        end
    endfunction
    /***********************************************************************/
    /***********************************************************************/
    genvar i;

    parameter   OUTTER_MODULE   = "";
    parameter   MODULE_NAME     = "decision_ctr";
        localparam  DESIGN_NAME = "decision_ctr";



    /* input / output buffer start */
    parameter   IN_BUF_LEN              =   0;
        localparam  LO_PARAM_IN_BUF_EN  = valueToBool(IN_BUF_LEN);
    parameter   OUT_BUF_LEN             =   0;
        localparam  LO_PARAM_IN_BUF_EN  = valueToBool(OUT_BUF_LEN);
    /* input / output buffer end */

    /*
    * Decision Mode
    * 0 : Save Error vector to fifo channel
    *       Sequentially get error polynomial, which needs 2^p cycle, they are
    *       saved in FIFO channel in mSBS, out simultaneously. After
    *       this process, simulataneously do Chien search and metric check.
    *       The results are saved FIFO channel in Chien search and metric
    *       block.
    *       Then decide error vector to correct.
    *       
    *       Degree infomation has simultaneous process. 
    *       Metric infomation has simultaneous process. 
    *
    *
    * 1 : Do simultaneous processing that pre-Chien search(and metric check) 
    *       and Chien search(Polynomial Decision)
    *       
    *       Degree infomation has simultaneous process.
    *       Metric infomation has simultaneous process. 
    *
    *
    * 2 : Do sequential processing that pre-Chien search(and metric check)
    *       and Chien search(Polynomial Decision)
    *
    *       Getting error polynomial needs 2^p cycle and pre-Chien search
    *       blocks are initialized by out value of this module.
    *       Metric values out sequentially and decision also has sequential 
    *       process.
    *       
    *       Degree infomation has sequential process.
    *       Metric infomation has sequential process. 
    */
    /***********************************************************************/
    /***********************************************************************/
    function    integer deg_input_bus_en;
        input   integer decision_mode;

        begin
            if(decision_mode == 0)  deg_input_bus_en <= 1;
            else if(decision_mode == 1)  deg_input_bus_en <= 1;
            else if(decision_mode == 2)  deg_input_bus_en <= 0;
            else begin
                $display("in decision_ctr) decision_mode has wrong value(%d)", decision_ctr);
                deg_input_bus_en <= 0;
            end
        end
    endfunction
    /***********************************************************************/
    function    integer deg_output_bus_en;
        input   integer decision_mode;

        begin
            if(decision_mode == 0)  deg_output_bus_en <= 1;
            else if(decision_mode == 1)  deg_output_bus_en <= 1;
            else if(decision_mode == 2)  deg_output_bus_en <= 0;
            else begin
                $display("in decision_ctr) decision_mode has wrong value(%d)", decision_ctr);
                deg_output_bus_en <= 0;
            end
        end
    endfunction
    /***********************************************************************/
    function    integer errCnt_input_bus_en;
        input   integer decision_mode;

        begin
            if(decision_mode == 0)  errCnt_input_bus_en <= 1;
            else if(decision_mode == 1)  errCnt_input_bus_en <= 1;
            else if(decision_mode == 2)  errCnt_input_bus_en <= 0;
            else begin
                $display("in decision_ctr) decision_mode has wrong value(%d)", decision_ctr);
                errCnt_input_bus_en <= 0;
            end
        end
    endfunction
    /***********************************************************************/
    function    integer errCnt_output_bus_en;
        input   integer decision_mode;

        begin
            if(decision_mode == 0)  errCnt_output_bus_en <= 1;
            else if(decision_mode == 1)  errCnt_output_bus_en <= 1;
            else if(decision_mode == 2)  errCnt_output_bus_en <= 0;
            else begin
                $display("in decision_ctr) decision_mode has wrong value(%d)", decision_ctr);
                errCnt_output_bus_en <= 0;
            end
        end
    endfunction
    /***********************************************************************/
    /***********************************************************************/
    parameter   DECISION_MODE   =   1;



    /* input bus enable */
    parameter   INPUT_BUS_EN                =   0;
        localparam  LO_PARAM_IN_BUS_EN      =   valueToBool(INPUT_BUS_EN);
    /* input bus enable */

    /* output bus enable */
    parameter   OUTPUT_BUS_EN               =   0;
        localparam  LO_PARAM_OUT_BUS_EN     =   valueToBool(OUTPUT_BUS_EN);
    /* output bus enable */

    parameter   DECISION_NUMS                           =   8;
        localparam  LO_PARAM_DECISION_NUMS              =   notBeingZero(DECISION_NUMS);


    parameter   CLK_DISRANCE_DEG                =   2;
    parameter   BIT_WIDTH_DEG                   =   2;
        localparam  LO_PARAM_IN_BUS_EN_DEG      =   deg_input_bus_en(DECISION_MODE);
        localparam  LO_PARAM_OUT_BUS_EN_DEG     =   deg_output_bus_en(DECISION_MODE);
        localparam  LO_PARAM_BIT_WIDTH_DEG      =   notBeingZero(BIT_WIDTH_DEG);

        localparam  LO_PARAM_IN_WIDTH_DEG       =   
            ((LO_PARAM_DECISION_NUMS - 1) * LO_PARAM_IN_BUS_EN_DEG + 1) * BIT_WIDTH_DEG;
        localparam  LO_PARAM_OUT_WIDTH_DEG      =   
            ((LO_PARAM_DECISION_NUMS - 1) * LO_PARAM_OUT_BUS_EN_DEG + 1) * BIT_WIDTH_DEG;
        

    parameter   CLK_DISTANCE_ERR_CNT            =   1;
    parameter   BIT_WITDH_ERR_CNT               =   2;
        localparam  LO_PARAM_CLK_DISRANCE_ERR_CNT   =   
        localparam  LO_PARAM_IN_BUS_EN_ERR_CNT  =   errCnt_input_bus_en(DECISION_MODE);
        localparam  LO_PARAM_OUT_BUS_EN_ERR_CNT =   errCnt_output_bus_en(DECISION_MODE);
        localparam  LO_PARAM_BIT_WIDTH_ERR_CNT  =   notBeingZero(BIT_WIDTH_ERR_CNT);

        localparam  LO_PARAM_IN_WIDTH_ERR_CNT   =   
            ((LO_PARAM_DECISION_NUMS - 1) * LO_PARAM_IN_BUS_EN_ERR_CNT + 1) * BIT_WITDH_ERR_CNT;
        localparam  LO_PARAM_OUT_WIDTH_ERR_CNT  =   
            ((LO_PARAM_DECISION_NUMS - 1) * LO_PARAM_OUT_BUS_EN_ERR_CNT + 1) * BIT_WITDH_ERR_CNT;



    /***********************************************************************/
    /***********************************************************************/
    input   clk;
    input   in_ctr_Srst;
    input   in_ctr_en;
    /***********************************************************************/
    input   in_ctr_getVal_deg;
    input   in_ctr_outVel_deg;
    input   [LO_PARAM_IN_WIDTH_DEG - 1 : 0]        in_tp_deg;

    output  [LO_PARAM_OUT_WIDTH_DEG - 1 : 0]        out_tp_deg;
    /***********************************************************************/
    wire    w_in_ctr_getVal_deg;
    wire    w_in_ctr_outVel_deg;
    wire    [LO_PARAM_IN_WIDTH_DEG - 1 : 0]        w_in_tp_deg;

    wire    [LO_PARAM_OUT_WIDTH_DEG - 1 : 0]        w_out_tp_deg;
    /***********************************************************************/
    input   in_ctr_getVal_erCnt;
    input   in_ctr_outVal_erCnt;
    input   [LO_PARAM_IN_WIDTH_ERR_CNT - 1 : 0]    in_tp_erCnt;

    output  [LO_PARAM_OUT_WIDTH_ERR_CNT - 1 : 0]    out_tp_erCnt;
    /***********************************************************************/
    wire    w_in_ctr_getVal_erCnt;
    wire    w_in_ctr_outVal_erCnt;
    wire    [LO_PARAM_IN_WIDTH_ERR_CNT - 1 : 0]    w_in_tp_erCnt;

    wire    [LO_PARAM_OUT_WIDTH_ERR_CNT - 1 : 0]    w_out_tp_erCnt;
    /***********************************************************************/
    /***********************************************************************/


    /***********************************************************************/
    /***********************************************************************/
    generate
    /* input buffer */
        /*input buffer degree*/
        if(LO_PARAM_IN_BUF_EN) begin : gen_deg_in_buf_enabled
            fifo_multi_ch
            #(
                .OUTTER_NAME("decision_ctr"),
                .MODULE_NAME("uut_in_buf_tp_deg"),

                .FIFO_LEN(IN_BUF_LEN),
                .FIFO_CH_WIDTH(LO_PARAM_IN_WIDTH_DEG + 2)
            )
            uut_in_buf_tp_deg
            (
                .clk(clk),

                .in_ctr_Srst(in_ctr_Srst),
                .in_ctr_en(in_ctr_en),

                .in({in_ctr_getVal_deg, in_ctr_outVel_deg, in_tp_deg}),
                .out({w_in_ctr_getVal_deg, w_in_ctr_outVel_deg, w_in_tp_deg})
            );
        end
        else begin : gen_deg_in_buf_disabled
            assign  w_in_ctr_getVal_deg     = in_ctr_getVal_deg;
            assign  w_in_ctr_outVel_deg    = in_ctr_outVel_deg;

            assign  w_in_tp_deg         = in_tp_deg;
        end
        /*input buffer degree*/

        /*input buffer error count*/
        if(LO_PARAM_IN_BUF_EN) begin : gen_errCnt_in_buf_enabled
            fifo_multi_ch
            #(
                .OUTTER_NAME("decision_ctr"),
                .MODULE_NAME("uut_in_buf_tp_errCnt"),

                .FIFO_LEN(IN_BUF_LEN),
                .FIFO_CH_WIDTH(LO_PARAM_IN_WIDTH_ERR_CNT + 2)
            )
            uut_in_buf_tp_errCnt
            (
                .clk(clk),

                .in_ctr_Srst(in_ctr_Srst),
                .in_ctr_en(in_ctr_en),

                .in({in_ctr_getVal_erCnt, in_ctr_outVal_erCnt, in_tp_erCnt}),
                .out({w_in_ctr_getVal_erCnt, w_in_ctr_outVal_erCnt, w_in_tp_erCnt})
            );
        end
        else begin : gen_errCnt_in_buf_disabled
            assign  w_in_ctr_getVal_erCnt   = in_ctr_getVal_erCnt;
            assign  w_in_ctr_outVal_erCnt  = in_ctr_outVal_erCnt;

            assign  w_in_tp_erCnt       = in_tp_erCnt;
        end
        /*input buffer error count*/
    /* input buffer */



    /* output buffer */
        /* output buffer degree*/
        if(LO_PARAM_IN_BUF_EN) begin : gen_deg_out_buf_enabled
            fifo_multi_ch
            #(
                .OUTTER_NAME("decision_ctr"),
                .MODULE_NAME("uut_out_buf_tp_deg"),

                .FIFO_LEN(OUT_BUF_LEN),
                .FIFO_CH_WIDTH(LO_PARAM_OUT_WIDTH_DEG)
            )
            uut_out_buf_tp_deg
            (
                .clk(clk),

                .in_ctr_Srst(in_ctr_Srst),
                .in_ctr_en(in_ctr_en),

                .in(w_out_tp_deg),
                .out(out_tp_deg)
            );
        end
        else begin : gen_deg_out_buf_disabled
            assign  out_tp_deg = w_out_tp_deg;
        end
        /* output buffer degree*/

        /* output buffer error count*/
        if(LO_PARAM_IN_BUF_EN) begin : gen_errCnt_out_buf_enabled
            fifo_multi_ch
            #(
                .OUTTER_NAME("decision_ctr"),
                .MODULE_NAME("uut_out_buf_tp_errCnt"),

                .FIFO_LEN(OUT_BUF_LEN),
                .FIFO_CH_WIDTH(LO_PARAM_OUT_WIDTH_ERR_CNT)
            )
            uut_out_buf_tp_errCnt
            (
                .clk(clk),

                .in_ctr_Srst(in_ctr_Srst),
                .in_ctr_en(in_ctr_en),

                .in(w_out_tp_erCnt),
                .out(out_tp_erCnt)
            );
        end
        else begin : gen_errCnt_out_buf_disabled
            assign out_tp_erCnt = w_out_tp_erCnt;
        end
        /* output buffer error count*/
    /* output buffer */
    endgenerate 

    /* Degree Infomation start */
    generate
        if(DECISION_MODE == 1) begin : gen_DM_1_deg
            wire    [LO_PARAM_IN_WIDTH_DEG - 1 : 0] w_DM_1_deg_0to1;

            ctr_info
            #(
                .IN_BUF_LEN(0),
                .OUT_BUF_LEN(0),
                .OUTTER_MODULE(DESIGN_NAME),
                .MODULE_NAME("uut_DM_1_deg_0"),    
                .INPUT_BUS_EN(LO_PARAM_IN_BUS_EN_DEG),
                .OUTPUT_BUS_EN(LO_PARAM_OUT_BUS_EN_DEG),
                .INFO_NUMS(DECISION_NUMS),
                .BIT_WIDTH_INFO(BIT_WIDTH_DEG),
                .CLK_DISRANCE_INFO(1)
            )
            uut_DM_1_deg_0
            (
                .clk(clk),
                .in_ctr_Srst(in_ctr_Srst),
                .in_ctr_en(in_ctr_en),

                .in_ctr_get_info(w_in_ctr_getVal_deg),
                .in_info(w_in_tp_deg),
                .in_ctr_out_info(w_in_ctr_outVel_deg),
                .out_info(w_DM_1_deg_0to1)
            );

            ctr_info
            #(
                .IN_BUF_LEN(0),
                .OUT_BUF_LEN(0),
                .OUTTER_MODULE(DESIGN_NAME),
                .MODULE_NAME("uut_DM_1_deg_1"),    
                .INPUT_BUS_EN(LO_PARAM_IN_BUS_EN_DEG
                .OUTPUT_BUS_EN(LO_PARAM_OUT_BUS_EN_DEG),
                .INFO_NUMS(DECISION_NUMS),
                .BIT_WIDTH_INFO(BIT_WIDTH_DEG),
                .CLK_DISRANCE_INFO(1)
            )
            uut_DM_1_deg_1
            (
                .clk(clk),
                .in_ctr_Srst(in_ctr_Srst),
                .in_ctr_en(in_ctr_en),

                .in_ctr_get_info(w_DM_1_interCon_deg),
                .in_info(w_DM_1_deg_0to1),
                .in_ctr_out_info(w_in_ctr_outVel_deg),
                .out_info(w_out_tp_deg)
            );

        end
        else if(DECISION_MODE == 2) begin : gen_DM_2_deg
            wire    [LO_PARAM_IN_WIDTH_DEG - 1 : 0] w_DM_2_deg_0to1;

            ctr_info
            #(
                .IN_BUF_LEN(0),
                .OUT_BUF_LEN(0),
                .OUTTER_MODULE(DESIGN_NAME),
                .MODULE_NAME("uut_DM_2_deg_0"),    
                .INPUT_BUS_EN(LO_PARAM_IN_BUS_EN_DEG),
                .OUTPUT_BUS_EN(LO_PARAM_OUT_BUS_EN_DEG),
                .INFO_NUMS(DECISION_NUMS),
                .BIT_WIDTH_INFO(BIT_WIDTH_DEG),
                .CLK_DISRANCE_INFO(1)
            )
            uut_DM_2_deg_0
            (
                .clk(clk),
                .in_ctr_Srst(in_ctr_Srst),
                .in_ctr_en(in_ctr_en),

                .in_ctr_get_info(w_in_ctr_getVal_deg),
                .in_info(w_in_tp_deg),
                .in_ctr_out_info(w_in_ctr_outVel_deg),
                .out_info(w_DM_2_deg_0to1)
            );

            ctr_info
            #(
                .IN_BUF_LEN(0),
                .OUT_BUF_LEN(0),
                .OUTTER_MODULE(DESIGN_NAME),
                .MODULE_NAME("uut_DM_2_deg_1"),    
                .INPUT_BUS_EN(LO_PARAM_IN_BUS_EN_DEG),
                .OUTPUT_BUS_EN(LO_PARAM_OUT_BUS_EN_DEG),
                .INFO_NUMS(DECISION_NUMS),
                .BIT_WIDTH_INFO(BIT_WIDTH_DEG),
                .CLK_DISRANCE_INFO(1)
            )
            uut_DM_2_deg_1
            (
                .clk(clk),
                .in_ctr_Srst(in_ctr_Srst),
                .in_ctr_en(in_ctr_en),

                .in_ctr_get_info(w_DM_1_interCon_deg),
                .in_info(w_DM_2_deg_0to1),
                .in_ctr_out_info(w_in_ctr_outVel_deg),
                .out_info(w_out_tp_deg)
            );

        end
        else if(DECISION_MODE == 3) begin : gen_DM_3_deg
            wire    [LO_PARAM_OUT_WIDTH_DEG - 1 : 0]    w_DM_3_deg_0to1;

            ctr_info
            #(
                .IN_BUF_LEN(0),
                .OUT_BUF_LEN(0),
                .OUTTER_MODULE(DESIGN_NAME),
                .MODULE_NAME("uut_DM_3_deg_0"),    
                .INPUT_BUS_EN(LO_PARAM_IN_BUS_EN_DEG),
                .OUTPUT_BUS_EN(LO_PARAM_OUT_BUS_EN_DEG),
                .INFO_NUMS(DECISION_NUMS),
                .BIT_WIDTH_INFO(BIT_WIDTH_DEG),
                .CLK_DISRANCE_INFO(1)
            )
            uut_DM_3_deg_0
            (
                .clk(clk),
                .in_ctr_Srst(in_ctr_Srst),
                .in_ctr_en(in_ctr_en),

                .in_ctr_get_info(w_in_ctr_getVal_deg),
                .in_info(w_in_tp_deg),
                .in_ctr_out_info(w_in_ctr_outVel_deg),
                .out_info(w_DM_3_deg_0to1)
            );

            ctr_info
            #(
                .IN_BUF_LEN(0),
                .OUT_BUF_LEN(0),
                .OUTTER_MODULE(DESIGN_NAME),
                .MODULE_NAME("uut_DM_3_deg_1"),    
                .INPUT_BUS_EN(LO_PARAM_IN_BUS_EN_DEG),
                .OUTPUT_BUS_EN(LO_PARAM_OUT_BUS_EN_DEG),
                .INFO_NUMS(DECISION_NUMS),
                .BIT_WIDTH_INFO(BIT_WIDTH_DEG),
                .CLK_DISRANCE_INFO(1)
            )
            uut_DM_3_deg_1
            (
                .clk(clk),
                .in_ctr_Srst(in_ctr_Srst),
                .in_ctr_en(in_ctr_en),

                .in_ctr_get_info(w_DM_1_interCon_deg),
                .in_info(w_DM_3_deg_0to1),
                .in_ctr_out_info(w_in_ctr_outVel_deg),
                .out_info(w_out_tp_deg)
            );

        end
    endgenerate
    /* Degree Infomation end */
endmodule

`endif

