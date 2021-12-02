`ifndef CTR_ENABLER
`define CTR_ENABLER

module ctr_enabler
(
    clk,

    in_Srst,
    in_en,

    in_init,
    in_done,

    out_module_proc_en,
    out_module_en
);
    input   clk;

    input   in_Srst;
    input   in_en;

    input   in_init;
    input   in_done;

    output  out_module_proc_en;
    output  out_module_en;

    reg     ctr_mod_proc_en;

    always@(posedge clk)    begin
        if(in_Srst)             ctr_mod_proc_en <= 0;
        else    begin
            if(in_init)         ctr_mod_proc_en <= 1;
            else if(in_done)    ctr_mod_proc_en <= 0;
            else                ctr_mod_proc_en <= ctr_mod_proc_en;
        end
    end

    assign  out_module_proc_en = ctr_mod_proc_en;

    assign  out_module_en = out_module_proc_en & in_en;

endmodule
`endif
