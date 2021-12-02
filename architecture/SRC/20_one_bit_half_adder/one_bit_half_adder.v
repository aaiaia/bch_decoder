`ifndef ONE_BIT_HALF_ADDER
`define ONE_BIT_HALF_ADDER
module one_bit_half_adder
(out, a b);

input a;
input b;

output [1:0] out;

assign out[0] = a^b;
assign out[1] = a&b;
    
endmodule
`endif
