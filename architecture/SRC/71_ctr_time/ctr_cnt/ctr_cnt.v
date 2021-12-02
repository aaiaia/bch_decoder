`ifndef CTR_CNT
`define CTR_CNT

module ctr_cnt
(
    clk,

    in_Srst,
    in_en,

    in_init,

    out_cy_cnt,

    out_done,
    out_last,

    out_ptl_init,
    out_ptl_last
);

    parameter   OUTTER_NAME   =   "";
    parameter   MODULE_NAME     =   "ctr_cnt";
        localparam  DESIGN_NAME =   "ctr_cnt";

    parameter   RST_VAL         =   0;
    parameter   INIT_VAL        =   1;
    parameter   INCREASEMENT    =   1;
    parameter   OP_CLK_CYCLE         =   32'b1111_1111_1111_1111_1111_1111_1111_1111;
    parameter   PARALLELISM     =   1;

        /* Counter Bit Width */
        parameter   LO_PARAM_CNT_WIDTH      =   bitWidthCal(OP_CLK_CYCLE);


        localparam  TMP_QUOTIENT    =   OP_CLK_CYCLE / PARALLELISM;
        localparam  TMP_REDUNDANT   =   OP_CLK_CYCLE % TMP_QUOTIENT;
        localparam  EN_CLK_CYCLE    =   TMP_QUOTIENT + TMP_REDUNDANT;
        localparam  LAST_PARALLEL_EN_CLK_CYCLE  =   (EN_CLK_CYCLE * PARALLELISM)
                                            +   (OP_CLK_CYCLE % EN_CLK_CYCLE);

    parameter   PARTIAL_CTR_EN      =   0;
    parameter   PARTIAL_PHASE_NUM   =   1;
        localparam  PARTIAL_TMP_QUOTIENT        =   EN_CLK_CYCLE / PARTIAL_PHASE_NUM;
        localparam  PARTIAL_TMP_REDUNDANT       =   EN_CLK_CYCLE % PARTIAL_PHASE_NUM;
        localparam  PARTIAL_EN_CLK_CYCLE    =   PARTIAL_TMP_QUOTIENT + PARTIAL_TMP_REDUNDANT;
        localparam  LAST_PARTIAL_EN_CLK_CYCLE   =   (PARTIAL_EN_CLK_CYCLE * PARTIAL_PHASE_NUM)
                                            +   (OP_CLK_CYCLE % PARTIAL_EN_CLK_CYCLE);



    /************************************************************************/
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
    /************************************************************************/


    input   clk;

    input   in_Srst;
    input   in_en;

    input   in_init;

    output  out_done;
    output  out_last;

    output  [LO_PARAM_CNT_WIDTH - 1 : 0]    out_cy_cnt;

    output  out_ptl_init;
    output  out_ptl_last;

    reg     [LO_PARAM_CNT_WIDTH - 1 : 0]    cy_cnt;

    genvar i;

    always@(posedge clk)    begin
        if(in_Srst)                             cy_cnt <= RST_VAL;
        else    begin
            if(in_en)   begin
                if(in_init)                     cy_cnt <= INIT_VAL;
                else if(cy_cnt == RST_VAL)      cy_cnt <= cy_cnt;
                else if(cy_cnt == EN_CLK_CYCLE) cy_cnt <= RST_VAL;
                else                            cy_cnt <= cy_cnt + INCREASEMENT;
            end
            else                                cy_cnt <= cy_cnt;
        end
    end

    assign  out_cy_cnt = cy_cnt;
    assign  out_done = (cy_cnt == EN_CLK_CYCLE) ? 1 : 0;

    generate
        if(PARALLELISM > 1) begin : prlLastSig
            assign  out_last = (cy_cnt > LAST_PARALLEL_EN_CLK_CYCLE) ? 1 : 0;
        end
        else                begin : noPrlLastSig
            assign  out_last = 0;
        end
    endgenerate

    generate
        if(PARTIAL_CTR_EN != 0) begin : ptlGen
            if((PARTIAL_PHASE_NUM - 1) == 0)    begin : noPtlPhase
                assign  out_ptl_init = 0;
                assign  out_ptl_last = 0;
            end
            else                                begin : ptlPhase
                wire    [PARTIAL_PHASE_NUM - 1 : 0] ctr_ptl_cal_init;


                for(i = 0; i < PARTIAL_PHASE_NUM; i = i + 1)    begin : ptlCalInitGen
                    assign  ctr_ptl_cal_init[i] = 
                                (cy_cnt == (PARTIAL_EN_CLK_CYCLE * (i + 1))) ? 1 : 0;
                end

                assign  out_ptl_init = |ctr_ptl_cal_init;

                assign  out_ptl_last = (cy_cnt > LAST_PARTIAL_EN_CLK_CYCLE) ? 1 : 0;
            end
        end
        else                    begin : noPtlGen
            assign  out_ptl_init = 0;
            assign  out_ptl_last = 0;
        end
    endgenerate

    /************************************************************************/
    /************************************************************************/
    initial
    begin
        //$display("=%d",);
        $display("/**************************************************/");
        $display("Instanciating %s", DESIGN_NAME);

        $display("In %s," ,OUTTER_NAME);
        $display("MODULE_NAME : %s", MODULE_NAME);
        $display("DESIGN_NAME : %s", DESIGN_NAME);

        $display("LO_PARAM_CNT_WIDTH=%d",LO_PARAM_CNT_WIDTH);
        $display("RST_VAL=%d",RST_VAL);
        $display("INIT_VAL=%d",INIT_VAL);
        $display("INCREASEMENT=%d",INCREASEMENT);
        $display("OP_CLK_CYCLE=%d",OP_CLK_CYCLE);


        $display("PARALLELISM(INCREASEMENT)=%d",PARALLELISM);
        $display("TMP_QUOTIENT=%d",TMP_QUOTIENT);
        $display("TMP_REDUNDANT=%d",TMP_REDUNDANT);
        $display("EN_CLK_CYCLE=%d",EN_CLK_CYCLE);
        $display("LAST_PARALLEL_EN_CLK_CYCLE=%d",LAST_PARALLEL_EN_CLK_CYCLE);

        $display("PARTIAL_CTR_EN=%d",PARTIAL_CTR_EN);
        $display("PARTIAL_PHASE_NUM=%d",PARTIAL_PHASE_NUM);
        $display("PARTIAL_TMP_QUOTIENT=%d",PARTIAL_TMP_QUOTIENT);
        $display("PARTIAL_TMP_REDUNDANT=%d",PARTIAL_TMP_REDUNDANT);
        $display("PARTIAL_EN_CLK_CYCLE=%d",PARTIAL_EN_CLK_CYCLE);
        $display("LAST_PARTIAL_EN_CLK_CYCLE=%d",LAST_PARTIAL_EN_CLK_CYCLE);
        $display("/**************************************************/");
    end
endmodule

`endif
