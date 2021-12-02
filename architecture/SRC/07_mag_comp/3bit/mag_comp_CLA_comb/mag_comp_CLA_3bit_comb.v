`ifndef MAG_COMP_3BIT_COMB
`define MAG_COMP_3BIT_COMB
module mag_comp_CLA_3bit_comb(in_a, in_b, out_cp);

    /* if in_b is smaller than in_a, cp will out 1 */
    /* if cp is 1, min value is changed */
    input [2:0] in_a;
    input [2:0] in_b;

    output      out_cp;

    wire [2:0] w_b_inv;

    assign w_b_inv = ~in_b;

    assign out_cp = (in_a[2] & w_b_inv[2]) ^ (in_a[1] & w_b_inv[1] & (in_a[2] ^ w_b_inv[2])) ^ (in_a[0] & w_b_inv[0] & (in_a[1] ^ w_b_inv[1]) & (in_a[2] ^ w_b_inv[2]));

endmodule
`endif
