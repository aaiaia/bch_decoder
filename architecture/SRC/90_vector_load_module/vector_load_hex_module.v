`ifndef VECTOR_HEX_LOAD_MODULE
`define VECTOR_HEX_LOAD_MODULE

module vector_load_hex_module
(
    clk,

    in_Srst,
    in_en,

    out_vector
);
    localparam  DESIGN_NAME = "vector_load_hex_module";
    genvar i;

    parameter PATH = "";
    parameter BIT_LEN = 3;

    parameter PARALLELISM = 1;
        localparam PARAM_PARALLEL = zeroChker(PARALLELISM);

    parameter WORD_LEN = 1023; 
    parameter WORD_NUM = 10;

        localparam PARAM_QUOTIENT = WORD_LEN / PARAM_PARALLEL;
        localparam PARAM_REDUNDANT = WORD_LEN % PARAM_PARALLEL;

        localparam PARAM_LAST_P_LOC = last_p_loc(WORD_LEN, PARAM_REDUNDANT, PARAM_PARALLEL);

    input   clk;
    input   in_Srst;
    input   in_en;

    output  [PARAM_PARALLEL * BIT_LEN - 1 : 0]   out_vector;

    reg [BIT_LEN-1:0]   buf_vec   [WORD_NUM-1:0][WORD_LEN-1:0];

    integer p_loc;
    integer p_word_num;

    function integer zeroChker;
        input integer value;

        if(!value)  zeroChker = 1;
        else        zeroChker = value;

    endfunction

    function integer last_p_loc;
        input integer length;
        input integer redundant;
        input integer parallelism;

        integer i;

        if(redundant == 0) begin
            last_p_loc = parallelism;
        end
        else begin
            last_p_loc = redundant;
        end

    endfunction


    initial begin
        $display("%s", DESIGN_NAME);
        $display("Vector Load Module(HEX), BIT WIDTH=%d, PARALLEL=%d\n", BIT_LEN, PARAM_PARALLEL);
    end


    generate
        if(PARAM_PARALLEL == 1) begin : gen_outVec_noParallel
            assign  out_vector = in_en ? buf_vec[p_word_num][p_loc] : {(BIT_LEN){1'b0}};
        end
        else begin : gen_outVec_yesParallel
            for(i = 0; i < PARAM_PARALLEL; i = i + 1) begin : gen_outVec_assgin
                assign  out_vector[(i + 1) * BIT_LEN - 1 : (i) * BIT_LEN] = 
                    in_en ? (
                            (p_loc + i) < WORD_LEN ?
                                buf_vec[p_word_num][p_loc + i] : 
                                {(BIT_LEN){1'b0}}
                            ) : 
                        //{(BIT_LEN){1'bx}}
                        {(BIT_LEN){1'b0}}
                    ;
            end
        end

    endgenerate



    

    always@(posedge clk)
    begin
        if(in_Srst) p_loc <= 0;
        else
        begin
            if(in_en)
            begin
                if(p_loc < (WORD_LEN - PARAM_LAST_P_LOC)) p_loc <= p_loc + PARAM_PARALLEL;
                else                    p_loc <= 0;
            end
            else                        p_loc <= p_loc;
        end
    end

    always@(posedge clk)
    begin
        if(in_Srst)                     p_word_num <= 0;
        else
        begin
            if(in_en)
            begin
                if(p_loc + PARALLELISM > (WORD_LEN - PARAM_REDUNDANT - 1)) p_word_num <= p_word_num + 1;
                else                    p_word_num <= p_word_num;
            end
            else                        p_word_num <= p_word_num;
        end
    end

    initial
    begin
        $readmemh(PATH, buf_vec);
    end 

endmodule

`endif
