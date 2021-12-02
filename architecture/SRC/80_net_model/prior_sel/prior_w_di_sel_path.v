`ifndef PRIOR_W_DIRECT_SEL_PATH
`define PRIOR_W_DIRECT_SEL_PATH

module prior_w_di_sel_path(in, out, sel);
    /**************************************************************/
    /**************************************************************/
    function    integer valToBool;
        input   integer val;

        begin
            if(val) valToBool = 1;
            else    valToBool = 0;
        end
    endfunction
    /**************************************************************/
    function integer ifPosReturnZero;
        input   integer chkVal;
        input   integer returnVal;

        begin
            if(chkVal >= 0) ifPosReturnZero = 0;
            else            ifPosReturnZero = returnVal;
        end
    endfunction
    /**************************************************************/
    function integer ifNegReturnZero;
        input   integer chkVal;
        input   integer returnVal;

        begin
            if(chkVal < 0)  ifNegReturnZero = 0;
            else            ifNegReturnZero = returnVal;
        end
    endfunction
    /**************************************************************/
    function integer plusOneMinusOneConvert;
        input   integer value;
        begin
            if(value >= 0)  plusOneMinusOneConvert = 1;
            else            plusOneMinusOneConvert = -1;
        end
    endfunction
    /**************************************************************/
    /**************************************************************/
    genvar      i;

    localparam  DESIGN_NAME     =   "prior_w_di_sel_path";
    parameter   BIT_WIDTH      =   2;
    parameter   SEL_SIG_NUMS    =   8;
    
    /* PRIORITY is positive value, MSB has first priority
     * or negative, LSB has first priority */ 
    parameter   PRIORITY_DIRECTION      =   1;

//    localparam  LO_PARAM_START_LOC      =   ifPosReturnZero(PRIORITY_DIRECTION, SEL_SIG_NUMS);
//    localparam  LO_PARAM_END_LOC        =   ifNegReturnZero(PRIORITY_DIRECTION, SEL_SIG_NUMS);
//    localparam  LO_PARAM_INCREASEMENT   =   plusOneMinusOneConvert(PRIORITY_DIRECTION);

    /**************************************************************/
    /* If default_input_en has any value, default input option is enabled.
    * In in signal, this value always locates MSB side.
    * For example, under this option is enabled, .in({default value or wire,
    * evaluated value or wire}).
    */
    parameter   DEFAULT_INPUT_EN    =   0;
        localparam  LO_PARAM_DEFAULT_INPUT_EN    =   valToBool(DEFAULT_INPUT_EN);

    /*output buf is not complete*/
    parameter   INPUT_SEL_BUF_EN    =   0;
        localparam  LO_PA_INPUT_SEL_BUF_EN = valToBool(INPUT_SEL_BUF_EN);

    parameter   INPUT_VALUE_BUF_EN  =   0;
        localparam  LO_PA_INPUT_VALUE_BUF_EN = valToBool(INPUT_VALUE_BUF_EN);

    parameter   OUTPUT_VALUE_BUF_EN = 0;
        localparam  LO_PA_OUTPUT_VALUE_BUF_EN = valToBool(OUTPUT_VALUE_BUF_EN);
    /**************************************************************/
    /**************************************************************/

    input   [BIT_WIDTH * (SEL_SIG_NUMS + LO_PARAM_DEFAULT_INPUT_EN) - 1 : 0] in;
    output  [BIT_WIDTH - 1 : 0]                out;

    input   [SEL_SIG_NUMS - 1 : 0]             sel;

    wire    [BIT_WIDTH * (SEL_SIG_NUMS + LO_PARAM_DEFAULT_INPUT_EN) - 1 : 0] w_in;
    wire    [SEL_SIG_NUMS - 1 : 0]             w_sel;
    wire    [BIT_WIDTH - 1 : 0]                w_out;



    /**************************************************************/
    /**************************************************************/
    generate
        if(LO_PA_INPUT_VALUE_BUF_EN) begin : gen_in_val_buf
        end
        else begin : wiring_in_val
            assign w_in = in;
        end

        if(LO_PA_OUTPUT_VALUE_BUF_EN) begin : gen_out_val_buf
        end
        else begin : wiring_out_val
            assign out = w_out; 
        end

        if(LO_PA_INPUT_SEL_BUF_EN) begin : gen_in_sel_buf
        end
        else    begin : wiring_in_sel
            assign w_sel = sel; 
        end

    endgenerate
    /**************************************************************/
    /**************************************************************/
    generate
        wire    [BIT_WIDTH * SEL_SIG_NUMS - 1 : 0] w_seled_wire;

        if(PRIORITY_DIRECTION < 0) begin : gen_LSB_hasPriority
          
            initial $display("for(i = 0; i < SEL_SIG_NUMS - 1; i = i + 1) label : gen_LSB_wiring");

            for(i = 0; i < SEL_SIG_NUMS - 1; i = i + 1) begin : gen_LSB_wiring
                assign  w_seled_wire[BIT_WIDTH * (i + 1) - 1 : BIT_WIDTH * (i)] = 
                            w_sel[i] ? 
                                in[BIT_WIDTH * (i + 1) - 1 : BIT_WIDTH * (i)] : 
                                w_seled_wire[BIT_WIDTH * ((i + 1) + 1) - 1 : BIT_WIDTH * ((i) + 1)];

                initial begin
                    $display("w_seled_wire[%d : %d] = w_sel[%d] ? w_in[%d : %d] : w_seled_wire[%d : %d] w_in for", 
                            BIT_WIDTH * (i + 1) - 1, BIT_WIDTH * (i), 
                            i, 
                            BIT_WIDTH * (i + 1) - 1,       BIT_WIDTH * (i),
                            BIT_WIDTH * ((i + 1) + 1) - 1, BIT_WIDTH * ((i) + 1));
                end
            end

            if(LO_PARAM_DEFAULT_INPUT_EN) begin : default_inEN
                assign  w_seled_wire[BIT_WIDTH * (SEL_SIG_NUMS) - 1 : BIT_WIDTH * (SEL_SIG_NUMS - 1)] = 
                                        w_sel[SEL_SIG_NUMS - 1] ? 
                                            w_in[BIT_WIDTH * (SEL_SIG_NUMS) - 1 : BIT_WIDTH * (SEL_SIG_NUMS - 1)] :
                                            w_in[BIT_WIDTH * (SEL_SIG_NUMS + LO_PARAM_DEFAULT_INPUT_EN) - 1 : BIT_WIDTH * (SEL_SIG_NUMS + 0)];
                    initial begin
                        $display("w_seled_wire[%d : %d] = w_sel[%d] ? w_in[%d : %d] : w_in[%d : %d]", 
                                BIT_WIDTH * (SEL_SIG_NUMS) - 1, BIT_WIDTH * (SEL_SIG_NUMS - 1),
                                SEL_SIG_NUMS - 1, 
                                BIT_WIDTH * (SEL_SIG_NUMS) - 1, BIT_WIDTH * (SEL_SIG_NUMS - 1),
                                BIT_WIDTH * (SEL_SIG_NUMS + LO_PARAM_DEFAULT_INPUT_EN) - 1,
                                BIT_WIDTH * (SEL_SIG_NUMS + 0));
                    end
            end
            else begin : default_inDis
                assign  w_seled_wire[BIT_WIDTH * (SEL_SIG_NUMS) - 1 : BIT_WIDTH * (SEL_SIG_NUMS - 1)] = 
                                        w_sel[SEL_SIG_NUMS - 1] ? 
                                            w_in[BIT_WIDTH * (SEL_SIG_NUMS) - 1 : BIT_WIDTH * (SEL_SIG_NUMS - 1)] :
                                            {(BIT_WIDTH){1'b0}};
                    initial begin
                        $display("w_seled_wire[%d : %d] = w_sel[%d] ? w_in[%d : %d] : %d", 
                                BIT_WIDTH * (SEL_SIG_NUMS) - 1, BIT_WIDTH * (SEL_SIG_NUMS - 1),
                                SEL_SIG_NUMS - 1, 
                                BIT_WIDTH * (SEL_SIG_NUMS) - 1, BIT_WIDTH * (SEL_SIG_NUMS - 1),
                                {(BIT_WIDTH){1'b0}});
                    end
            end

            assign  w_out = w_seled_wire[BIT_WIDTH - 1 : 0];
                initial $display("w_out[%d : %d] = w_seled_wire[%d : %d]", BIT_WIDTH - 1, 0, BIT_WIDTH - 1, 0);

        end
        else begin : gen_MSB_hasPriority
            wire    [BIT_WIDTH * SEL_SIG_NUMS - 1 : 0] w_seled_wire;

            initial $display("for(i = 1; i < SEL_SIG_NUMS; i = i + 1) begin : gen_MSB_wiring");

            for(i = 1; i < SEL_SIG_NUMS; i = i + 1) begin : gen_MSB_wiring
                assign  w_seled_wire[BIT_WIDTH * (i + 1) - 1 : BIT_WIDTH * (i)] = 
                            w_sel[i] ? 
                                w_in[BIT_WIDTH * (i + 1) - 1 : BIT_WIDTH * (i)] : 
                                w_seled_wire[BIT_WIDTH * (i) - 1 : BIT_WIDTH * ((i - 1))];
                    initial begin
                        $display("w_seled_wire [%d : %d] = w_sel[%d] ? w_in[%d : %d] : w_seled_wire[%d : %d] w_in for",
                                BIT_WIDTH * (i + 1) - 1,   BIT_WIDTH * (i),
                                i, 
                                BIT_WIDTH * (i + 1) - 1,   BIT_WIDTH * (i),
                                BIT_WIDTH * (i) - 1,   BIT_WIDTH * ((i - 1)),
                                );
                    end
            end



            if(LO_PARAM_DEFAULT_INPUT_EN) begin : default_inEN
                assign  w_seled_wire[BIT_WIDTH - 1 : 0] = 
                            w_sel[0] ? 
                                w_in[BIT_WIDTH - 1 : 0] :
                                w_in[BIT_WIDTH * (SEL_SIG_NUMS + LO_PARAM_DEFAULT_INPUT_EN) - 1 : BIT_WIDTH * (SEL_SIG_NUMS + 0)];

                    initial begin
                        $display("w_seled_wire [%d : %d] = w_sel[%d] ? w_in[%d : %d] : w_in[%d : %d]",
                                BIT_WIDTH - 1,     0,
                                0,
                                BIT_WIDTH - 1,     0,
                                BIT_WIDTH * (SEL_SIG_NUMS + LO_PARAM_DEFAULT_INPUT_EN) - 1,
                                 BIT_WIDTH * (SEL_SIG_NUMS + 0)
                                );
                    end
            end
            else begin : default_inDis
                assign  w_seled_wire[BIT_WIDTH - 1 : 0] = 
                            w_sel[0] ? 
                                w_in[BIT_WIDTH - 1 : 0] :
                                {(BIT_WIDTH){1'b0}};

                    initial begin
                        $display("w_seled_wire [%d : %d] = w_sel[%d] ? w_in[%d : %d] : %d",
                                BIT_WIDTH - 1,     0,
                                0,
                                BIT_WIDTH - 1,     0,
                                {(BIT_WIDTH){1'b0}}
                                );
                    end
            end

                assign  w_seled_wire[BIT_WIDTH - 1 : 0] = 
                            w_sel[0] ? 
                                w_in[BIT_WIDTH - 1 : 0] :
                                {(BIT_WIDTH){1'b0}};

                    initial begin
                        $display("w_seled_wire [%d : %d] = w_sel[%d] ? w_in[%d : %d] : %d",
                                BIT_WIDTH - 1,     0,
                                0,
                                BIT_WIDTH - 1,     0,
                                {(BIT_WIDTH){1'b0}}
                                );
                    end


            assign  w_out = w_seled_wire[BIT_WIDTH * SEL_SIG_NUMS - 1 : BIT_WIDTH * (SEL_SIG_NUMS - 1)];
                initial $display("w_out[%d : %d] = w_seled_wire[%d : %d]", 
                            BIT_WIDTH - 1,                 0, 
                            BIT_WIDTH * SEL_SIG_NUMS - 1,  BIT_WIDTH * (SEL_SIG_NUMS - 1)
                            );


        end
    
    endgenerate

/*********************************************************************/
/*********************************************************************/
    initial begin
        $display("/**************************************************/");
        $display("Instanciating %s", DESIGN_NAME);

        $display("DESIGN_NAME : %s", DESIGN_NAME);

        $display("SEL_SIG_NUMS : %d",SEL_SIG_NUMS);
        $display("BIT_WIDTH : %d",BIT_WIDTH);
        $display("PRIORITY_DIRECTION : %d",PRIORITY_DIRECTION);
        $display("PRIORITY_DIRECTION : %s",PRIORITY_DIRECTION < 0 ? "LSB side" : "MSB side");
//        $display("LO_PARAM_START_LOC : %d",LO_PARAM_START_LOC);
//        $display("LO_PARAM_END_LOC : %d",LO_PARAM_END_LOC);
//        $display("LO_PARAM_INCREASEMENT : %d",LO_PARAM_INCREASEMENT);
        $display("DEFAULT_INPUT_EN = %d", DEFAULT_INPUT_EN);
        $display("LO_PARAM_DEFAULT_INPUT_EN = %s", LO_PARAM_DEFAULT_INPUT_EN ? "Enabled" : "Disabled");

        $display("INPUT_SEL_BUF_EN = %d", INPUT_SEL_BUF_EN);
        $display("LO_PA_INPUT_SEL_BUF_EN : %s",
            LO_PA_INPUT_SEL_BUF_EN ? "enabled" : "disabled");

        $display("INPUT_VALUE_BUF_EN = %d", INPUT_VALUE_BUF_EN);
        $display("LO_PA_INPUT_VALUE_BUF_EN : %s",
            LO_PA_INPUT_VALUE_BUF_EN ? "enabled" : "disabled");

        $display("OUTPUT_VALUE_BUF_EN = %d", OUTPUT_VALUE_BUF_EN);
        $display("LO_PA_OUTPUT_VALUE_BUF_EN : %s",
            LO_PA_OUTPUT_VALUE_BUF_EN ? "enabled" : "disabled");

    end

endmodule



`endif
