`ifndef SIM_TO_SEQ_REG_PS_FIR_2
`define SIM_TO_SEQ_REG_PS_FIR_2

`include "simultaneousToSequantialReg_2.v"

module  simultaneousToSequantialReg_psFirs_2
(
    clk,

    in_ctr_Srst,
    in_ctr_init,
    in_ctr_en,
    in_ctr_sft_en,

    in,
    out
);
    /************************************************************************/
    /************************************************************************/
    function    integer notBeingZero;
        input   integer value;

        begin
            if(!value)  notBeingZero = 0;
            else        notBeingZero = value;
        end
    endfunction
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
    function    integer valToBool;
        input   integer val;

        begin
            if(val) valToBool = 1;
            else    valToBool = 0;
        end
    endfunction
    /************************************************************************/
    /************************************************************************/
    genvar i;
    /************************************************************************/
    parameter   DIRECTION   =   1;
    parameter   SHIFT_LEN   =   1;
    parameter   BIT_WIDTH   =   2;

    parameter   CLK_DISTANCE    =   1;
        localparam  LO_PARAM_CLK_DISTANCE = notBeingZero(CLK_DISTANCE);
        localparam  LO_PARAM_CLK_DIS_BIT_LEN = bitWidthCal(LO_PARAM_CLK_DISTANCE);

    parameter   PASSING_FIR =   0;
        localparam  LO_PA_PASSING_FIR = valToBool(PASSING_FIR);

    /************************************************************************/
    /************************************************************************/
    input           clk;

    input           in_ctr_Srst;
    input           in_ctr_init;
    input           in_ctr_en;
    input           in_ctr_sft_en;

    input   [(BIT_WIDTH * SHIFT_LEN) - 1 : 0]   in;
    output  [BIT_WIDTH - 1 : 0]                 out;

    generate
        if(LO_PA_PASSING_FIR) begin : y_ps_first
            wire    [BIT_WIDTH - 1 : 0] w_ypsFir_out;


            simultaneousToSequantialReg_2
            #(
                .DIRECTION(DIRECTION),
                .SHIFT_LEN(SHIFT_LEN - 1),
                .BIT_WIDTH(BIT_WIDTH),
                .CLK_DISTANCE(CLK_DISTANCE)
            )
            uut_simToSeq_y_psFir_2
            (
                .clk(clk),

                .in_ctr_Srst(in_ctr_Srst),
                .in_ctr_init(in_ctr_init),
                .in_ctr_en(in_ctr_en),
                .in_ctr_sft_en(in_ctr_sft_en),

                .in(in[BIT_WIDTH * SHIFT_LEN) - 1 : BIT_WIDTH * (1))]),
                .out(w_ypsFir_out)
            );

            assign out = in_ctr_init ? in[BIT_WIDTH - 1 : 0] : w_ypsFir_out;
        end
        else begin : n_ps_first
            simultaneousToSequantialReg_2
            #(
                .DIRECTION(DIRECTION),
                .SHIFT_LEN(SHIFT_LEN),
                .BIT_WIDTH(BIT_WIDTH),
                .CLK_DISTANCE(CLK_DISTANCE)
            )
            uut_simToSeq_y_psFir_2
            (
                .clk(clk),

                .in_ctr_Srst(in_ctr_Srst),
                .in_ctr_init(in_ctr_init),
                .in_ctr_en(in_ctr_en),
                .in_ctr_sft_en(in_ctr_sft_en),

                .in(in),
                .out(out)
            );
        end
    endgenerate


endmodule
`endif
