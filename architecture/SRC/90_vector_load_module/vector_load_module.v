`ifndef VECTOR_LOAD_MODULE
`define VECTOR_LOAD_MODULE

module vector_load_bin_module
(
    clk,

    in_Srst,
    in_en,

    out_vector
);
    parameter PATH = ""
    parameter BIT_LEN = 3;
    parameter WORD_LEN = 10;

    input   clk;
    input   in_Srst;
    input   in_en;

    output  [BIT_LEN-1:0]   out_vector;

    reg [BIT_LEN-1:0] buf_vec[WORD_LEN-1:0];

    integer word_indicator;

    assign  out_vector = buf_vec[word_indicator];

    always@(posedge clk)
    begin
        if(in_Srst) word_indicator <= 0;
        else
        begin
            if(in_en)
            begin
                if(word_indicator != WORD_LEN-1)    word_indicator <= word_indicator + 1;
                else                                word_indicator <= 0;
            end
            else        word_indicator <= word_indicator;
        end
    end

    initial
    begin
        $readmemb(PATH, buf_vec);
    end 

endmodule

`endif
