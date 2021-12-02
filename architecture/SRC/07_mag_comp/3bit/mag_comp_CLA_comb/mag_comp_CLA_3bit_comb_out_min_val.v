`ifndef MAG_COMP_3BIT_COMB_OUT_MIN_VAL
`define MAG_COMP_3BIT_COMB_OUT_MIN_VAL
module mag_comp_CLA_3bit_comb_out_min_val(in_a, in_b, out);
    parameter   OUTTER_NAME = "";
    parameter   MODULE_NAME = "mag_comp_CLA_3bit_comb_out_min_val";
        localparam  DESIGN_NAME = "mag_comp_CLA_3bit_comb_out_min_val";

    parameter   ADDITIONAL_PASSING_VAL_WIDTH   =   0;
    /*************************************************/
    /* if in_b is smaller than in_a, cp will out 1 */
    /* if cp is 1, min value is changed */
    input [2 + ADDITIONAL_PASSING_VAL_WIDTH : 0] in_a;
    input [2 + ADDITIONAL_PASSING_VAL_WIDTH : 0] in_b;

    output  [2 + ADDITIONAL_PASSING_VAL_WIDTH : 0] out;

    wire cp;

    wire [2 : 0] w_b_inv;

    assign w_b_inv[2 : 0] = ~in_b[2 : 0];

    assign cp = (in_a[2] & w_b_inv[2]) ^ (in_a[1] & w_b_inv[1] & (in_a[2] ^ w_b_inv[2])) ^ (in_a[0] & w_b_inv[0] & (in_a[1] ^ w_b_inv[1]) & (in_a[2] ^ w_b_inv[2]));
    //if cp is 1 in_b, it is smaller than the other.
    //Or not, in_a is smaller than in_b.
    assign  out = (cp ? in_b : in_a);

    /*************************************************/
    initial begin
        $display("/**************************************************/");
        $display("Instanciating %s", DESIGN_NAME);

        $display("In %s," ,OUTTER_NAME);
        $display("MODULE_NAME : %s", MODULE_NAME);
        $display("DESIGN_NAME : %s", DESIGN_NAME);

        $display("ADDITIONAL_PASSING_VAL_WIDTH=%d",ADDITIONAL_PASSING_VAL_WIDTH);
        $display("/**************************************************/");
    end
endmodule
`endif
