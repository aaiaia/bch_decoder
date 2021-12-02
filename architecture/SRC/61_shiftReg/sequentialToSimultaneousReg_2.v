`ifndef SEQ_TO_SIM_REG_2
`define SEQ_TO_SIM_REG_2

module  sequentialToSimultaneousReg_2
(
    clk ,

    in_ctr_Srst ,
    in_ctr_en ,

    in_ctr_sh_en ,

    in ,
    out
);
    /************************************************************************/
    /************************************************************************/
    function    integer notBeingZero;
        input   integer value;

        begin
            if(!value)  notBeingZero = 0;
            else        notBeingZero = value;
        end
    endfunction
    /************************************************************************/
    function    integer bitWidthCal;
        input   integer expression_value;

        integer cal_tmp;

        begin
            bitWidthCal = 1;
            for(cal_tmp = 1;  cal_tmp < expression_value; cal_tmp = cal_tmp << 1) begin
                bitWidthCal = bitWidthCal + 1;
            end
        end
    endfunction
    /************************************************************************/
    /************************************************************************/
    genvar i;
    /************************************************************************/
    parameter OUTTER_NAME = "";
    parameter MODULE_NAME = "sequentialToSimultaneousReg_2";
        localparam DESIGN_NAME = "sequentialToSimultaneousReg_2";
    /************************************************************************/
    /************************************************************************/

    parameter   DIRECTION   =   1;
    parameter   SHIFT_LEN   =   1;
    parameter   BIT_WIDTH   =   2;

    parameter   CLK_DISTANCE    =   1;
        localparam  LO_PARAM_CLK_DISTANCE = notBeingZero(CLK_DISTANCE);
        localparam  LO_PARAM_CLK_DIS_BIT_LEN = bitWidthCal(LO_PARAM_CLK_DISTANCE);
    /************************************************************************/
    /************************************************************************/
    input           clk;

    input           in_ctr_Srst;
    input           in_ctr_en;

    input           in_ctr_sh_en ;

    input   [BIT_WIDTH - 1 : 0] in;
    output  [(BIT_WIDTH * SHIFT_LEN) - 1 : 0] out;

    /* clk distance register */
    wire                                        ctr_clk_dis_en;
    generate
        if(LO_PARAM_CLK_DISTANCE == 1) begin
            assign  ctr_clk_dis_en = 1'b1;
        end
        else begin
            initial $display("DESIGN_NAME : %s is not support LO_PARAM_CLK_DISTANCE = %d perpectly.", 
                DESIGN_NAME, LO_PARAM_CLK_DISTANCE);
            reg     [LO_PARAM_CLK_DIS_BIT_LEN - 1 : 0]  ctr_clk_dis;

            always@(posedge clk) begin
                if(in_ctr_Srst)         ctr_clk_dis <= {(LO_PARAM_CLK_DIS_BIT_LEN){1'b0}};
                else if(in_ctr_en & in_ctr_sh_en)      
                                        ctr_clk_dis <= ctr_clk_dis + 1;
                else if(ctr_clk_dis_en) ctr_clk_dis <= {(LO_PARAM_CLK_DIS_BIT_LEN){1'b0}};
                else                    ctr_clk_dis <= ctr_clk_dis;
            end
            assign  ctr_clk_dis_en = (ctr_clk_dis==LO_PARAM_CLK_DISTANCE-1)?(1'b1):(1'b0);
            
        end
    endgenerate



    generate
        if(SHIFT_LEN == 0) begin : shift_regIs0
            initial begin
                $display("WARNING!!!) %s has no SHIFT_LEN", DESIGN_NAME);
            end
        end
        else if(SHIFT_LEN ==  1) begin : shift_regIs1
            reg     [BIT_WIDTH - 1 : 0] shift_reg;

            always@(posedge clk) begin
                if(in_ctr_Srst)         shift_reg <= {(BIT_WIDTH){1'b0}};
                else if(in_ctr_en & ctr_clk_dis_en) begin
                    if(in_ctr_sh_en)    shift_reg <= in ;
                    else                shift_reg <= shift_reg ;
                end
                else                    shift_reg <= shift_reg ;
            end

            assign  out = shift_reg;
        end
        else begin : shift_regIs2orMore

            reg     [BIT_WIDTH - 1 : 0] shift_reg [SHIFT_LEN - 1 : 0];

            if(DIRECTION > 0) begin : sh_forward
                always@(posedge clk) begin
                    if(in_ctr_Srst)         shift_reg[0] <= {(BIT_WIDTH){1'b0}};
                    else if(in_ctr_en & ctr_clk_dis_en)
                        if(in_ctr_sh_en)    shift_reg[0] <= in;
                        else                shift_reg[0] <= shift_reg[0];

                    else                    shift_reg[0] <= shift_reg[0];
                end
                
                for(i = 1; i < SHIFT_LEN; i = i + 1) begin : shiForFunc 
                    always@(posedge clk) begin
                        if(in_ctr_Srst)     shift_reg[i] <= {(BIT_WIDTH){1'b0}};
                        else if(in_ctr_en & ctr_clk_dis_en)  
                            if(in_ctr_sh_en)shift_reg[i] <= shift_reg[i - 1];
                            else            shift_reg[i] <= shift_reg[i];
                        else                shift_reg[i] <= shift_reg[i];
                    end
                end
            end
            else begin : sh_backward
                always@(posedge clk) begin
                    if(in_ctr_Srst)     shift_reg[SHIFT_LEN - 1] <= {(BIT_WIDTH){1'b0}};
                    else if(in_ctr_en & ctr_clk_dis_en)  
                        if(in_ctr_sh_en)shift_reg[SHIFT_LEN - 1] <= in;
                        else            shift_reg[SHIFT_LEN - 1] <= shift_reg[SHIFT_LEN - 1];
                    else                shift_reg[SHIFT_LEN - 1] <= shift_reg[SHIFT_LEN - 1];
                end

                for(i = 0; i < SHIFT_LEN - 1; i = i + 1) begin : shiForFunc 
                    always@(posedge clk) begin
                        if(in_ctr_Srst)     shift_reg[i] <= {(BIT_WIDTH){1'b0}};
                        else if(in_ctr_en & ctr_clk_dis_en)
                            if(in_ctr_sh_en)shift_reg[i] <= shift_reg[i + 1];
                            else            shift_reg[i] <= shift_reg[i];
                        else                shift_reg[i] <= shift_reg[i];
                    end
                end
            end

            for(i = 0; i < SHIFT_LEN; i = i + 1) begin : wiring
                assign  out[BIT_WIDTH * (i + 1) - 1 : BIT_WIDTH * (i + 0)] = shift_reg[i];
            end
        end
    endgenerate


endmodule
`endif
