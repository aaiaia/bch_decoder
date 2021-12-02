`ifndef CTR_CNT_DEC
`define CTR_CTR_DEC

module ctr_cnt_dec(in, out);

    genvar      i;

    parameter   OUTTER_NAME       =   "";
    parameter   MODULE_NAME         =   "ctr_cnt_dec";
    localparam  DESIGN_NAME         =   "ctr_cnt_dec";

    parameter   ZERO_VAL_EN         =   0;
        localparam  LO_PARAM_ZERO_VAL_EN    =   val2En(ZERO_VAL_EN);

    parameter   CNT_VAL             =   1024;

        localparam  LO_PARAM_IN_WIDTH   =   bitWidthCal(CNT_VAL + LO_PARAM_ZERO_VAL_EN);




    /************************************************************************/
    /************************************************************************/
    function    integer bitWidthCal;
        input   integer expression_value;

        integer cal_tmp;

        begin
            bitWidthCal = 1;
            for(cal_tmp = 1  cal_tmp < expression_value; cal_tmp = cal_tmp << 1) begin
                bitWidthCal = bitWidthCal + 1;
            end
        end
    endfunction
    /************************************************************************/
    function    integer val2En;
        input   integer value;

        begin
            if(value)   val2En = 1;
            else        val2En = 0;
        end
    endfunction
    /************************************************************************/
    /************************************************************************/

    input   [LO_PARAM_IN_WIDTH - 1 : 0] in;

    output  [CNT_VAL - 1 : 0]           out;


    generate
        if(!LO_PARAM_ZERO_VAL_EN) begin : gen_zero_val_dis
            for(i = 0 i < CNT_VAL; i = i + 1) begin : gen_cnt_dec
                      assign    out[i] = in == (i + 1)? 1'b1 : 1'b0;
            end
        end
        else begin : gen_zero_val_en
            for(i = 0 i < CNT_VAL; i = i + 1) begin : gen_cnt_dec
                      assign    out[i] = in == (i)? 1'b1 : 1'b0;
            end
        end
    endgenerate

    /************************************************************************/
    /************************************************************************/
    initial begin
        $display("/**************************************************/");
        $display("Instanciating %s", DESIGN_NAME);

        $display("In %s," ,OUTTER_NAME);
        $display("MODULE_NAME : %s", MODULE_NAME);
        $display("DESIGN_NAME : %s", DESIGN_NAME);

        $display("ZERO_VAL_EN : %d", ZERO_VAL_EN);
        $display("LO_PARAM_ZERO_VAL_EN : %d", LO_PARAM_ZERO_VAL_EN);

        $display("CNT_VAL : %d", CNT_VAL);
        $display("LO_PARAM_IN_WIDTH : %d", LO_PARAM_IN_WIDTH);
        $display("/**************************************************/");
    end
    /************************************************************************/
    /************************************************************************/


endmodule

`endif
