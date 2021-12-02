`ifndef FIFO_SINGLE_CHANNEL
`define FIFO_SINGLE_CHANNEL

module  fifo_single_ch
(
    clk,

    in_ctr_Srst,
    in_ctr_en,

    in,
    out
);
    parameter   OUTTER_NAME =   "";
    parameter   MODULE_NAME =   "fifo_single_ch";
        localparam  DESIGN_NAME =   "fifo_single_ch";

    parameter   FIFO_LEN    =   1;

    /*********************************************/

    input           clk;

    input           in_ctr_Srst;
    input           in_ctr_en;


    input   in;
    output  out;

    genvar i;
    generate
        if(FIFO_LEN == 0) begin : fifoLengthIs0
            assign  in  =   out;
        end
        else if(FIFO_LEN ==  1) begin : fifoLengthIs1
            reg                         fifo_single_ch;

            always@(posedge clk) begin
                if(in_ctr_Srst)         fifo_single_ch <= 1'b0;
                else if(in_ctr_en)      fifo_single_ch <= in;
                else                    fifo_single_ch <= fifo_single_ch;
            end

            assign  out = fifo_single_ch;
        end
        else begin : fifoLengthIs2orMore    
            reg                         fifo_single_ch [FIFO_LEN - 1 : 0];

            always@(posedge clk) begin
                if(in_ctr_Srst)         fifo_single_ch[0] <= 1'b0;
                else if(in_ctr_en)      fifo_single_ch[0] <= in;
                else                    fifo_single_ch[0] <= fifo_single_ch[0];
            end
            
            for(i = 1; i < FIFO_LEN; i = i + 1) begin : fifo_len_gen
                always@(posedge clk) begin
                    if(in_ctr_Srst)     fifo_single_ch[i] <= 1'b0;
                    else if(in_ctr_en)  fifo_single_ch[i] <= fifo_single_ch[i - 1];
                    else                fifo_single_ch[i] <= fifo_single_ch[i];
                end
            end

            assign  out = fifo_single_ch[FIFO_LEN - 1];
        end
    endgenerate

    /*********************************************/
    initial begin
        $display("/**************************************************/");
        $display("Instanciating %s", DESIGN_NAME);

        $display("In %s," ,OUTTER_NAME);
        $display("MODULE_NAME : %s", MODULE_NAME);
        $display("DESIGN_NAME : %s", DESIGN_NAME);

        $display("FIFO_LEN = %d", FIFO_LEN);
        $display("/**************************************************/");
    end
endmodule

`endif
