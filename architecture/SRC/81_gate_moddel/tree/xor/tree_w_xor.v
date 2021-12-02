`ifndef TREE_W_XOR
`define TREE_W_XOR

module tree_w_xor(in, out);
    parameter   OUTTER_NAME = "";
    parameter   MODULE_NAME = "tree_w_xor";
    localparam  DESIGN_NAME = "tree_w_xor";

    genvar      var_input_exception;

    genvar      var_tree_depth;
    genvar      var_tree_length;

    parameter   BIT_WIDTH   =   10;
    parameter   PARALLELISM =   16;

    /* GF_SUM_PARAMETERS */

    localparam  TREE_PARALLELISM = find_regul_two_pow(PARALLELISM);
    localparam  TREE_DEPTH = log2(TREE_PARALLELISM);
    localparam  TREE_INNER_CONNECTION = log2_sum(TREE_PARALLELISM);


    initial begin
        $display("OUTTER_NAME : %s", OUTTER_NAME);
        $display("MODULE_NAME : %s", MODULE_NAME);
        $display("DESIGN_NAME : %s", DESIGN_NAME);

        $display("BIT_WIDTH : %d", BIT_WIDTH);
        $display("PARALLELISM : %d", PARALLELISM);

        $display("TREE_PARALLELISM:%d, TREE_DEPTH:%d, TREE_IN_CON:%d\n",
            TREE_PARALLELISM,
            TREE_DEPTH,
            TREE_INNER_CONNECTION);
    end

   

   input    [BIT_WIDTH * PARALLELISM - 1 : 0]   in;
   output   [BIT_WIDTH - 1 : 0]                 out;



    /******************************************************************/
    /******************************************************************/
    function    integer find_regul_two_pow;
        input   integer parallelism;

        integer tmp;
        begin
            for(tmp = 1; tmp < parallelism; tmp = tmp << 1);

            find_regul_two_pow = tmp;
        end
    endfunction
    /******************************************************************/
    function integer two_pow;
        input integer parallel;
        
        integer i;

        begin
            if(parallel % 2) begin
                for(i = 1; i < parallel; i = (i << 1));

                two_pow = i;
            end
            else begin
                two_pow = parallel;
            end
        end
    endfunction
    /******************************************************************/
    function integer log2;
        input [31:0] value;

        begin
            if(value != 0) begin
                for(log2 = 0; value > 0; log2 = log2 + 1) begin
                    value = (value>>1);
                end
            end
            else begin
                log2 = 0;
            end
        end
    endfunction
    /******************************************************************/
    function integer log2_sum;
        input [31:0] value;

        begin
            for(log2_sum = 0; value > 0; log2_sum = log2_sum) begin
                log2_sum = value + log2_sum;
                value = value >> 1;
            end
        end
    endfunction
    /******************************************************************/
    /******************************************************************/
    /******************************************************************/
    /******************************************************************/
    /******************************************************************/
    /******************************************************************/
    /******************************************************************/
    /******************************************************************/


    generate


    wire    [TREE_INNER_CONNECTION * BIT_WIDTH - 1 : 0]   w_tree_wire;

    /* Input exception start*/
    assign w_tree_wire
        [
            (TREE_INNER_CONNECTION) * BIT_WIDTH - 1 : 
            (TREE_INNER_CONNECTION - TREE_PARALLELISM) * BIT_WIDTH
        ] = {{in[BIT_WIDTH * PARALLELISM - 1 : 0]},{((TREE_PARALLELISM - PARALLELISM) * BIT_WIDTH){1'b0}}};

    initial begin
        $display("w_tree_wire[%d:%d] = in[%d:%d],{(%d){1'b0}}",
            (TREE_INNER_CONNECTION) * BIT_WIDTH - 1,
            (TREE_INNER_CONNECTION - TREE_PARALLELISM) * BIT_WIDTH,
            BIT_WIDTH * PARALLELISM - 1,
            0,
            (TREE_PARALLELISM - PARALLELISM) * BIT_WIDTH
            );

        $display("w_tree_wire width : %d, in width : %d + {(%d){1'b0}}",
            ((TREE_INNER_CONNECTION) * BIT_WIDTH - 1) - ((TREE_INNER_CONNECTION - TREE_PARALLELISM) * BIT_WIDTH) + 1,
            (BIT_WIDTH * PARALLELISM - 1) - 0 + 1,
            (TREE_PARALLELISM - PARALLELISM) * BIT_WIDTH
            );
    end

    
    /* Input exception end*/

    for(var_tree_depth = TREE_PARALLELISM; var_tree_depth != 1; var_tree_depth = (var_tree_depth >> 1)) begin : gen_tree_depth

        for(    var_tree_length = log2_sum(var_tree_depth); 
                var_tree_length > log2_sum(var_tree_depth >> 1); 
                var_tree_length = var_tree_length - 2) 
        begin : gen_tree_length
            /* sample wiring index
            assign w_tree_wire[(((((var_tree_length) - 1) >> 1) - 1) + 1) * BIT_WIDTH - 1 : (((((var_tree_length) - 1) >> 1) - 1) + 0) * BIT_WIDTH] = 
                w_tree_wire[(((var_tree_length) - 1) + 1) * BIT_WIDTH - 1 : (((var_tree_length) - 1) + 0) * BIT_WIDTH] ^
                w_tree_wire[(((var_tree_length) - 2) + 1) * BIT_WIDTH - 1 : (((var_tree_length) - 2) + 0) * BIT_WIDTH];

             * */
            initial $display("depth:%d,length:%d, [%d:%d] = [%d:%d] ^ [%d:%d]\n",
                    var_tree_depth,
                    var_tree_length,
                    (((((var_tree_length) - 1) >> 1) - 1) + 1) * BIT_WIDTH - 1,   (((((var_tree_length) - 1) >> 1) - 1) + 0) * BIT_WIDTH,
                    (((var_tree_length) - 1) + 1) * BIT_WIDTH - 1,                (((var_tree_length) - 1) + 0) * BIT_WIDTH,
                    (((var_tree_length) - 2) + 1) * BIT_WIDTH - 1,                (((var_tree_length) - 2) + 0) * BIT_WIDTH
            );

            
                assign
                w_tree_wire[(((((var_tree_length) - 1) >> 1) - 1) + 1) * BIT_WIDTH - 1 : (((((var_tree_length) - 1) >> 1) - 1) + 0) * BIT_WIDTH] = 
                w_tree_wire[(((var_tree_length) - 2) + 1) * BIT_WIDTH - 1 : (((var_tree_length) - 2) + 0) * BIT_WIDTH] ^ 
                w_tree_wire[(((var_tree_length) - 1) + 1) * BIT_WIDTH - 1 : (((var_tree_length) - 1) + 0) * BIT_WIDTH] ;
        end

    end

    assign out = w_tree_wire[BIT_WIDTH - 1 : 0];

    endgenerate

endmodule

`endif
