`ifndef GRAY_TO_DIFF_DECODER_COMB
`define GRAY_TO_DIFF_DECODER_COMB
module gray_to_diff_decoder_comb
(
    in_GC,
    out_sel_TP
);
    parameter GC_LEN = 2;

    parameter SEL_LENGTH = 3;

    input   [GC_LEN - 1 : 0]        in_GC;
    output  [SEL_LENGTH - 1 : 0]    out_sel_TP;

    assign out_sel_TP[0] = ~(out_sel_TP[1] + out_sel_TP[2]);

    assign out_sel_TP[1] = in_GC[0]&(~in_GC[1]);
    assign out_sel_TP[2] = (~in_GC[0])&in_GC[1];


endmodule
`endif
