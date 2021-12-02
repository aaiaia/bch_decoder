/**********************************************************/
/*
*   Design Name : 
*   Date : YYYY.MM.DD
*/
/**********************************************************/

`ifndef SEQ_TO_SIM_REG_PS_LAST_OPT
`define SEQ_TO_SIM_REG_PS_LAST_OPT

`include "61_shiftReg/sequentialToSimultaneousReg.v"
`include "61_shiftReg/sequentialToSimultaneousReg_psLast.v"
module sequentialToSimultaneousReg_psLastOpt(
    clk,

    in_ctr_Srst,
    in_ctr_en,
    in_ctr_sig,

    in,
    out
);
/**********************************************************/
/**********************************************************/
    function integer val2Bool;
        input   integer val;

        begin
            if(val) val2Bool = 1;
            else    val2Bool = 0;
        end
    endfunction
/**********************************************************/
    function integer val2notBool;
        input   integer val;

        begin
            if(val) val2notBool = 1;
            else    val2notBool = 0;
        end
    endfunction
/**********************************************************/
    function integer indexCal;
        input integer interVal;
        input integer initVal;

        begin
            indexCal = initVal + interVal;
        end
    endfunction
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

    parameter   PASSING_LAST    =   0;
        localparam  LO_PARAM_PASSING_LAST_Y = val2Bool(PASSING_LAST);
        localparam  LO_PARAM_PASSING_LAST_N = val2notBool(PASSING_LAST);
/* Indexing of control signal */
/*
* default index is 0, which is used to enable to shift enabled signal
* index 1 is only valid when PASSING_LAST has any value, except 0.
*/
        localparam  LO_INDEX_CTR_SH_EN = 0;
        localparam  LO_INDEX_CTR_LAST = indexCal(LO_PARAM_PASSING_LAST_Y, LO_INDEX_CTR_SH_EN);

        localparam  LO_INDEX_CTR_WIDTH = LO_INDEX_CTR_LAST + 1;
/**********************************************************/
/**********************************************************/
    input           clk;

    input           in_ctr_Srst;
    input           in_ctr_en;
    input   [LO_INDEX_CTR_WIDTH - 1 : 0]        in_ctr_sig;

    input   [BIT_WIDTH - 1 : 0]                 in;
    output  [(BIT_WIDTH * SHIFT_LEN) - 1 : 0]   out;


    wire    w_belong2Opt_sh_en;
//      reg clk;

/**********************************************************/
/**********************************************************/
generate
    /*shift enable signal definition*/
    if(LO_PARAM_PASSING_LAST_Y) begin : gen_whenY_PSL_shEnSig
        assign w_belong2Opt_sh_en = 
            in_ctr_en & in_ctr_sig[LO_INDEX_CTR_SH_EN] & (~in_ctr_sig[LO_INDEX_CTR_LAST]);
    end
    else begin : gen_whenN_PSL_shEnSig 
        assign w_belong2Opt_sh_en = 
            in_ctr_en & in_ctr_sig[LO_INDEX_CTR_SH_EN];
    end

    /*Sequential to simultaneous register*/
    if(DIRECTION > 0) begin : gen_sh_FW
            sequentialToSimultaneousReg
            #(
                .OUTTER_NAME(DESIGN_NAME),
                .MODULE_NAME("uut_seq2SimReg_F"),

                .DIRECTION(DIRECTION),
                .SHIFT_LEN(SHIFT_LEN - LO_PARAM_PASSING_LAST_Y),
                .BIT_WIDTH(BIT_WIDTH),

                .CLK_DISTANCE(CLK_DISTANCE)
            )
            uut_seq2SimReg_F
            (
                .clk(clk),

                .in_ctr_Srst(in_ctr_Srst),
                .in_ctr_en(w_belong2Opt_sh_en),

                .in(in),
                .out(out[(BIT_WIDTH * SHIFT_LEN) - 1 : BIT_WIDTH * (1 - LO_PARAM_PASSING_LAST_N)])
            );

    end
    else begin : gen_sh_BW
            sequentialToSimultaneousReg
            #(
                .OUTTER_NAME(DESIGN_NAME),
                .MODULE_NAME("uut_seq2SimReg_B"),

                .DIRECTION(DIRECTION),
                .SHIFT_LEN(SHIFT_LEN - LO_PARAM_PASSING_LAST_Y),
                .BIT_WIDTH(BIT_WIDTH),

                .CLK_DISTANCE(CLK_DISTANCE)
            )
            uut_seq2SimReg_B
            (
                .clk(clk),

                .in_ctr_Srst(in_ctr_Srst),
                .in_ctr_en(w_belong2Opt_sh_en),

                .in(in),
                .out(out[(BIT_WIDTH * (SHIFT_LEN - LO_PARAM_PASSING_LAST_Y)) - 1 : 0])
            );

    end

    /*Belong to passing last enabling options output, output port connection
    * is changed.*/
    if(LO_PARAM_PASSING_LAST_Y) begin : gen_out_belong2PSL_wiring 
        if(DIRECTION > 0) begin : forward
            assign out[BIT_WIDTH - 1 : 0] = 
                in_ctr_sig[LO_INDEX_CTR_LAST] ? in : {(BIT_WIDTH){1'b0}};
        end
        else begin : backward
            assign out[(BIT_WIDTH * SHIFT_LEN) - 1 : (BIT_WIDTH * (SHIFT_LEN - 1))] = 
                in_ctr_sig[LO_INDEX_CTR_LAST] ? in : {(BIT_WIDTH){1'b0}};
        end
    end
endgenerate

/**********************************************************/
/**********************************************************/
    initial begin
        $display("/**************************************************/");
        $display("Instanciating %s", DESIGN_NAME);

        $display("In %s," ,OUTTER_NAME);
        $display("MODULE_NAME : %s", MODULE_NAME);
        $display("DESIGN_NAME : %s", DESIGN_NAME);

        $display("DIRECTION = %d(%s)", DIRECTION, (DIRECTION > 0) ? "forward" : "backward");
        $display("SHIFT_LEN = %d", SHIFT_LEN);
        $display("BIT_WIDTH = %d", BIT_WIDTH);

        $display("CLK_DISTANCE = %d", CLK_DISTANCE);

        $display("PASSING_LAST = %d", PASSING_LAST);
        $display("LO_PARAM_PASSING_LAST_Y = %s", LO_PARAM_PASSING_LAST_Y ? "enabled" : "disabled");

        $display("LO_INDEX_CTR_SH_EN = %d", LO_INDEX_CTR_SH_EN);
        $display("LO_INDEX_CTR_LAST = %d", LO_INDEX_CTR_LAST);
        $display("LO_INDEX_CTR_WIDTH = %d", LO_INDEX_CTR_WIDTH);
//        $dumpfile("VCD_FILE");
//        $dumpvars;
//        $display("VCD FILES : %s", VCD_FILE);

//      clk <= 0;



//      #1000;
//      $finish;
        $display("/**************************************************/");
    end
/**********************************************************/
/**********************************************************/
//      always #5 clk <= ~clk;
/**********************************************************/
/**********************************************************/
endmodule

`endif

