`ifndef FIFO_MULTI_CHANNEL
`define FIFO_MULTI_CHANNEL

module  fifo_multi_ch
(
    clk,

    in_ctr_Srst,
    in_ctr_en,

    in,
    out
);
    parameter   OUTTER_NAME = "";
    parameter   MODULE_NAME = "fifo_multi_ch";
    localparam  DESIGN_NAME = "fifo_multi_ch";


    parameter   FIFO_LEN    =   1;
    parameter   FIFO_CH_WIDTH   =   2;

    /******************************************/

    input           clk;

    input           in_ctr_Srst;
    input           in_ctr_en;

    input   [FIFO_CH_WIDTH - 1 : 0] in;
    output  [FIFO_CH_WIDTH - 1 : 0] out;



    genvar i;
    generate
        if(FIFO_LEN == 0) begin : fifoLengthIs0
            assign  in  =   out;
        end
        else if(FIFO_LEN ==  1) begin : fifoLengthIs1
            reg     [FIFO_CH_WIDTH - 1 : 0] fifo_ch;

            always@(posedge clk) begin
                if(in_ctr_Srst)         fifo_ch <= {(FIFO_CH_WIDTH){1'b0}};
                else if(in_ctr_en)      fifo_ch <= in;
                else                    fifo_ch <= fifo_ch;
            end

            assign  out = fifo_ch;
        end
        else begin : fifoLengthIs2orMore

            reg     [FIFO_CH_WIDTH - 1 : 0] fifo_ch [FIFO_LEN - 1 : 0];

            always@(posedge clk) begin
                if(in_ctr_Srst)         fifo_ch[0] <= {(FIFO_CH_WIDTH){1'b0}};
                else if(in_ctr_en)      fifo_ch[0] <= in;
                else                    fifo_ch[0] <= fifo_ch[0];
            end
            
            for(i = 1; i < FIFO_LEN; i = i + 1) begin : fifo_len_gen
                always@(posedge clk) begin
                    if(in_ctr_Srst)     fifo_ch[i] <= {(FIFO_CH_WIDTH){1'b0}};
                    else if(in_ctr_en)  fifo_ch[i] <= fifo_ch[i - 1];
                    else                fifo_ch[i] <= fifo_ch[i];
                end
            end

            assign  out = fifo_ch[FIFO_LEN - 1];
        end
    endgenerate

    /******************************************/
    initial begin
        $display("/**************************************************/");
        $display("Instanciating %s", DESIGN_NAME);

        $display("In %s," ,OUTTER_NAME);
        $display("MODULE_NAME : %s", MODULE_NAME);
        $display("DESIGN_NAME : %s", DESIGN_NAME);

        $display("FIFO_LEN=%d", FIFO_LEN);
        $display("FIFO_CH_WIDTH=%d", FIFO_CH_WIDTH);
        $display("/**************************************************/");
    end
endmodule

`endif
