`ifndef LLR_MAGNITUDE_SEQ
`define LLR_MAGNITUDE_SEQ

module llr_magnitude_seq
(
    clk,

    in_ctr_Srst,
    in_ctr_en,

    in_llr,
    out_llr_mag
);
    parameter   OUTTER_NAME = "";
    parameter   MODULE_NAME = "llr_magnitude_seq";
        localparam  DESIGN_NAME = "llr_magnitude_seq";

    parameter   LLR_LEN  = 4;
    /*****************************************/

    input                   clk;

    input                   in_ctr_Srst;
    input                   in_ctr_en;

    input   [LLR_LEN - 1 : 0]   in_llr;

    output  [LLR_LEN - 2 : 0]   out_llr_mag;

    reg     [LLR_LEN - 2 : 0]   buf_llr;

    generate
        if (LLR_LEN <= 1)
        begin:llr_bit_len_less_then_2_NS
            assign in_llr = out_llr_mag;
        end
        else if(LLR_LEN > 1)
        begin:llr_bit_len_larger_then_2
            always@(posedge clk) begin
                if(in_ctr_Srst)     buf_llr <= {(LLR_LEN){1'b1}};
                else begin
                    if(!in_ctr_en) buf_llr <= buf_llr;
                    else
                    begin
                        if(!in_llr[LLR_LEN - 1]) buf_llr <= in_llr[LLR_LEN - 2 : 0];
                        else
                            if(in_llr[LLR_LEN - 2 : 0] != 0) buf_llr <= (~in_llr[LLR_LEN - 2 : 0]) + 1;
                            else buf_llr <= (~in_llr[LLR_LEN - 2 : 0]); 
                    end
                end
            end

        end
    endgenerate
    
    assign out_llr_mag = buf_llr;

    /*****************************************/
    initial begin
        $display("/**************************************************/");
        $display("Instanciating %s", DESIGN_NAME);

        $display("In %s," ,OUTTER_NAME);
        $display("MODULE_NAME : %s", MODULE_NAME);
        $display("DESIGN_NAME : %s", DESIGN_NAME);

        $display("LLR_LEN=%d",LLR_LEN);
        $display("/**************************************************/");
    end
endmodule
`endif
