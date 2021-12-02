`ifndef PRIOR_BUFFER_1
`define PRIOR_BUFFER_1

module prior_buffer_1(
    clk,
    in_ctr_Srst,
    in_ctr_en,

    in_ctr_init,
    in_ctr_done,

    in_ctr_buf_en,
    in_ctr_valid,

    in,
    out
);
/*********************************************************/
/*********************************************************/
function    integer valToBool ;
    input   integer val;

    begin
        if(val) valToBool = 1;
        else    valToBool = 0;
    end
endfunction
/*********************************************************/
/*********************************************************/
/*********************************************************/
    parameter   OUTTER_NAME = "";
    parameter   MODULE_NAME = "prior_buffer_1";
    localparam  DESIGN_NAME = "prior_buffer_1";
/*********************************************************/
    parameter   WIDTH   =   1;
    localparam  LO_PA_WIDTH = valToBool(WIDTH);

/*********************************************************/
function    [WIDTH - 1 : 0] bitLimitVal;
    input   integer val;

    begin
        bitLimitVal = val;
    end
endfunction
/*********************************************************/
    parameter   RST_VAL = 0;
        localparam  LO_PA_RST_VAL = bitLimitVal(RST_VAL);
/*********************************************************/
/*********************************************************/
    input   clk ;
    input   in_ctr_Srst ;
    input   in_ctr_en ;

    input   in_ctr_init ;
    input   in_ctr_done ;

    input   in_ctr_buf_en ;
    input   in_ctr_valid ;


    input   [WIDTH - 1 : 0] in ;
    output  [WIDTH - 1 : 0] out ;

    reg     [WIDTH - 1 : 0] r_ps_buf ;


    generate
            reg flag_notSelYet ;

            always@(posedge clk) begin : def_flag
                if(in_ctr_Srst) flag_notSelYet <= 1'b1;
                else begin
                    if(!in_ctr_en) flag_notSelYet <= flag_notSelYet;
                    else if(in_ctr_init)    flag_notSelYet <= 1'b1;
                    else if(!in_ctr_buf_en) flag_notSelYet <= flag_notSelYet;
                    else if(in_ctr_valid)   flag_notSelYet <= 1'b0;
                    else                    flag_notSelYet <= flag_notSelYet;
                end
            end

            always@(posedge clk)
            begin : def_buffer
                if(in_ctr_Srst) r_ps_buf <= LO_PA_RST_VAL ;  
                else
                begin
                    if(in_ctr_en)
                    begin
                        if(in_ctr_init)             r_ps_buf <= in;
                        else if(in_ctr_buf_en) 
                        begin
                            if(flag_notSelYet) 
                            begin
                                if(in_ctr_valid)    r_ps_buf <= r_ps_buf;
                                else if(in_ctr_done)r_ps_buf <= LO_PA_RST_VAL;
                                else                r_ps_buf <= r_ps_buf;
                            end
                            else                    r_ps_buf <= r_ps_buf;
                        end
                        else                        r_ps_buf <= r_ps_buf;
                    end
                    else                            r_ps_buf <= r_ps_buf;
                end
            end

            assign  out = flag_notSelYet ? 
                (in_ctr_valid ? r_ps_buf : LO_PA_RST_VAL ) : r_ps_buf;

    endgenerate
endmodule
`endif
