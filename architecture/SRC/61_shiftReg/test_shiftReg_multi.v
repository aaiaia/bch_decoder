`timescale 10us/1us

`include "61_shiftReg/shiftReg.v"

module test_shift_reg();

reg     clk;

reg     ctr_Srst;
reg     ctr_en;

reg     [4 - 1 : 0] ctr_value;

reg     buf_Srst;
always@(posedge clk)    buf_Srst <= ctr_Srst;
reg     buf_en;
always@(posedge clk)    buf_en <= ctr_en;

reg     [4 - 1 : 0] buf_value;
always@(posedge clk)    buf_value <= ctr_value;

wire    [16 * 4 - 1 : 0]w_forward;
wire    [16 * 4 - 1 : 0]w_reverse;


initial begin
    $dumpfile("61_shiftReg.vcd");
    $dumpvars;
    clk <= 0;
    ctr_Srst <= 0;
    ctr_en <= 0;
    ctr_value <= 0;
#5;

    ctr_Srst <= 1;
#10;

    ctr_Srst <= 0;
    ctr_en <= 1;

    ctr_value <= 4'h0; 
#10;

    ctr_value <= 4'h1;
#10;

    ctr_value <= 4'h2;
#10;

    ctr_value <= 4'h3;
#10;

    ctr_value <= 4'h4;
#10;

    ctr_value <= 4'h5;
#10;

    ctr_value <= 4'h6;
#10;

    ctr_value <= 4'h7;
#10;

    ctr_value <= 4'h8;
#10;

    ctr_value <= 4'h9;
#10;

    ctr_value <= 4'hA;
#10;

    ctr_value <= 4'hB;
#10;

    ctr_value <= 4'hC;
#10;

    ctr_value <= 4'hD;
#10;

    ctr_value <= 4'hE;
#10;

    ctr_value <= 4'hF;
#10;

    ctr_value <= 4'h0;
#10;

    ctr_value <= 4'h1;
#10;

    ctr_value <= 4'h2;
#10;

    ctr_value <= 4'h3;
#10;

$finish;
end

always #5 clk <= ~clk;

shift_reg
#(
    .DIRECTION("forward"),
    .SHIFT_LEN(16),
    .BIT_WIDTH(4)
)
forwardReg
(
    .clk(clk),

    .in_ctr_Srst(buf_Srst),
    .in_ctr_en(buf_en),

    .in(buf_value),
    .out(w_forward)
);

shift_reg
#(
    .DIRECTION("reverse"),
    .SHIFT_LEN(16),
    .BIT_WIDTH(4)
)
reverseReg
(
    .clk(clk),

    .in_ctr_Srst(buf_Srst),
    .in_ctr_en(buf_en),

    .in(buf_value),
    .out(w_reverse)
);



endmodule
