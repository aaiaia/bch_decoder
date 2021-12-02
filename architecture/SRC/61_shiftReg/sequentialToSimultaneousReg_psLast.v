/**********************************************************/
/*
*   Design Name : 
*   Date : YYYY.MM.DD
*/
/**********************************************************/

`ifndef SEQ_TO_SIM_REG_PS_LAST
`define SEQ_TO_SIM_REG_PS_LAST

`include "61_shiftReg/sequentialToSimultaneousReg.v"

module sequentialToSimultaneousReg_psLast(
    clk,

    in_ctr_Srst,
    in_ctr_en,
    in_ctr_last,

    in,
    out
);
/**********************************************************/
/**********************************************************/
    parameter OUTTER_NAME = "";
    parameter MODULE_NAME = "";
        localparam DESIGN_NAME = "sequentialToSimultaneousReg_psLast";
        localparam VCD_FILE = "sequentialToSimultaneousReg_psLast.vcd";
/**********************************************************/
/**********************************************************/
    parameter   DIRECTION   =   1;
    parameter   SHIFT_LEN   =   1;
    parameter   BIT_WIDTH   =   2;

    parameter   CLK_DISTANCE    =   1;
/**********************************************************/
/**********************************************************/
    input           clk;

    input           in_ctr_Srst;
    input           in_ctr_en;
    input           in_ctr_last;

    input   [BIT_WIDTH - 1 : 0] in;
    output  [(BIT_WIDTH * SHIFT_LEN) - 1 : 0] out;
//      reg clk;

/**********************************************************/
/**********************************************************/
generate
    if(DIRECTION > 0) begin : gen_sh_forward
        sequentialToSimultaneousReg
        #(
            .OUTTER_NAME(DESIGN_NAME),
            .MODULE_NAME("uut_seq2SimReg_F"),

            .DIRECTION(DIRECTION),
            .SHIFT_LEN(SHIFT_LEN - 1),
            .BIT_WIDTH(BIT_WIDTH),

            .CLK_DISTANCE(CLK_DISTANCE)
        )
        uut_seq2SimReg_F
        (
            .clk(clk),

            .in_ctr_Srst(in_ctr_Srst),
            .in_ctr_en(in_ctr_en&(~in_ctr_last)),

            .in(in),
            .out(out[(BIT_WIDTH * SHIFT_LEN) - 1 : BIT_WIDTH * 1])
        );

        assign out[BIT_WIDTH - 1 : 0] = in_ctr_last?in:{(BIT_WIDTH){1'b0}};
    end
    else begin : gen_sh_backward
        sequentialToSimultaneousReg
        #(
            .OUTTER_NAME(DESIGN_NAME),
            .MODULE_NAME("uut_seq2SimReg_B"),

            .DIRECTION(DIRECTION),
            .SHIFT_LEN(SHIFT_LEN - 1),
            .BIT_WIDTH(BIT_WIDTH),

            .CLK_DISTANCE(CLK_DISTANCE)
        )
        uut_seq2SimReg_B
        (
            .clk(clk),

            .in_ctr_Srst(in_ctr_Srst),
            .in_ctr_en(in_ctr_en&(~in_ctr_last)),

            .in(in),
            .out(out[(BIT_WIDTH * (SHIFT_LEN - 1)) - 1 : 0])
        );

        assign out[(BIT_WIDTH * SHIFT_LEN) - 1 : (BIT_WIDTH * (SHIFT_LEN - 1))] = in_ctr_last?in:{(BIT_WIDTH){1'b0}};
    end
endgenerate

/**********************************************************/
/**********************************************************/
    initial begin
//        $display("!!! %s is instanciate !!!", DESIGN_NAME);
//        $display("%s", OUTTER_NAME);
//        $display("%s", MODULE_NAME);
//        $display("%s", DESIGN_NAME);

//        $dumpfile("VCD_FILE");
//        $dumpvars;
//        $display("VCD FILES : %s", VCD_FILE);

//      clk <= 0;



//      #1000;
//      $finish;
    end
/**********************************************************/
/**********************************************************/
//      always #5 clk <= ~clk;
/**********************************************************/
/**********************************************************/
endmodule

`endif

