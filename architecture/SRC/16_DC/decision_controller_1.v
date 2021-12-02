`ifndef DECISION_CTR_1
`define DECISION_CTR_1

`include "16_DC/decision_indicator.v"
`include "65_ctr_info/ctr_infoLoad_ind/ctr_infoLoaders_independent.v"
`include "80_net_model/prior_sel/prior_w_di_sel_path.v"
module decision_controller_1
(
    clk ,
    in_ctr_Srst ,
    in_ctr_en ,

    in_ctr_sig ,

    in_tp_deg ,
    in_tp_erCnt ,

    out_tp_sel 
);
    /***********************************************************************/
    /***********************************************************************/
    function integer    notBeingZero;
        input   integer value;

        begin
            if(value > 0)   notBeingZero = value;
            else            notBeingZero = 1;
        end
    endfunction
    /***********************************************************************/
    function integer    valueToBool;
        input   integer value;

        begin
            if(value)   valueToBool = 1;
            else        valueToBool = 0;
        end
    endfunction
    /***********************************************************************/
    function integer    valueToNotBool;
        input   integer value;

        begin
            if(!value)   valueToNotBool = 1;
            else        valueToNotBool = 0;
        end
    endfunction
    /***********************************************************************/
    function integer    minValChker;
        input   integer minVal;
        input   integer value;

        begin
            if(value < minVal)
            begin
                    minValChker = minVal;
                    $display("ERROR!!!) in ctr_infoLoaders_independent, value(%d) has to larger or equal then minVal(%d).", value, minVal);
            end
            else    minValChker = value;
        end
    endfunction
    /***********************************************************************/
    /***********************************************************************/
    /****************************************
    * Offset is started at -1.
        *
        * Values are concluded below sequence.
        * 1. LO_IDX_CTR_SIG_EN_FIR (MODULE 0)
        * 2. LO_IDX_CTR_SIG_EN_LAS (MODULE LAST)
        * 3. LO_IDX_CTR_SIG_PASS_LAST (MODULE 0)
        * 4. LO_IDX_CTR_SIG_CTR_SIG_START (MODULE 1 ~ LAST - 1)
        * 5. LO_IDX_CTR_SIG_CTR_SIG_END (MODULE 1 ~ LAST - 1)
        * 6. LO_IDX_CTR_SIG_OUT_EN(MODULE LAST)
        * ****************************************/
       /****************************************
       * 1. LO_IDX_CTR_SIG_EN_FIR (MODULE 0)
       * ****************************************/
//      function integer first_en_index;
//          input integer startVal;
//          input integer in_n_bus_en;
//
//          begin
//              first_en_index = in_n_bus_en + startVal;
//          end
//
//      endfunction
      /******************************/

      /****************************************
      * 2. LO_IDX_CTR_SIG_EN_LAS (MODULE LAST)
      * ****************************************/
//     function integer last_en_index;
//         input integer startVal;
//         input integer out_n_bus_en;
//
//         begin
//             last_en_index = out_n_bus_en + startVal;
//         end
//
//     endfunction
     /******************************/

     /****************************************
     * 3. LO_IDX_CTR_SIG_PASS_LAST (MODULE 0)
     * ****************************************/
//    function integer passLast_index;
//        input integer startVal;
//        input integer passLast_en;
//
//        begin
//            passLast_index = passLast_en + startVal;
//        end
//
//    endfunction
    /****************************************
    * 4. LO_IDX_CTR_SIG_CTR_SIG_START (MODULE 1 ~ LAST - 1)
    * ****************************************/
    function integer cal_bus_index_start;
        input integer startVal;
        input integer bus_width;

        integer bus_valid_chk;

        begin
            if(bus_width)   bus_valid_chk = 1;
            else            bus_valid_chk = 0;

            cal_bus_index_start = startVal + bus_valid_chk;
        end

    endfunction
    /****************************************
    * 5. LO_IDX_CTR_SIG_CTR_SIG_END (MODULE 1 ~ LAST - 1)
    * ****************************************/
    function integer cal_bus_index_end;
        input integer startVal;
        input integer bus_width;

        integer bus_valid_chk;
        begin
            if(bus_width)   bus_valid_chk = 1;
            else            bus_valid_chk = 0;

            cal_bus_index_end = startVal + (bus_width - 1) * bus_valid_chk;
        end

    endfunction
    /****************************************
    * 6. LO_IDX_CTR_SIG_OUT_EN(MODULE LAST)
    * ****************************************/
//    function integer out_en_index;
//        input integer startVal;
//        input integer out_en;
//
//        begin
//            out_en_index = out_en + startVal;
//        end
//
//    endfunction
    /***********************************************************************/
    /***********************************************************************/
    function    integer degInfoLoaderEn;
        input   integer decision_mode;

        begin
            if(decision_mode == 0)      $display("Not support decision mode = %s", decision_mode);
            else if(decision_mode == 1) degInfoLoaderEn = 1;
            else if(decision_mode == 2) degInfoLoaderEn = 1;
            else if(decision_mode == 3) degInfoLoaderEn = 1;
            else                        degInfoLoaderEn = 0;
        end
    endfunction
    /***********************************************************************/
    function    integer deg_input_bus_en;
        input   integer decision_mode;

        begin
            if(decision_mode == 0)  deg_input_bus_en = 1;
            else if(decision_mode == 1)  deg_input_bus_en = 1;
            else if(decision_mode == 2)  deg_input_bus_en = 1;
            else if(decision_mode == 3)  deg_input_bus_en = 0;
            else begin
                $display("in decision_controller_1) decision_mode has wrong value(%d)", decision_mode);
                deg_input_bus_en = 0;
            end
        end
    endfunction
    /***********************************************************************/
    function    integer deg_output_bus_en;
        input   integer decision_mode;

        begin
            if(decision_mode == 0)  deg_output_bus_en = 1;
            else if(decision_mode == 1)  deg_output_bus_en = 1;
            else if(decision_mode == 2)  deg_output_bus_en = 1;
            else if(decision_mode == 3)  deg_output_bus_en = 0;
            else begin
                $display("in decision_controller_1) decision_mode has wrong value(%d)", decision_mode);
                deg_output_bus_en = 0;
            end
        end
    endfunction
    /***********************************************************************/
    function    integer deg_seq_dist_cali;
        input   integer decision_mode;
        input   integer deg_seq_distance;

        begin
                if(decision_mode == 1)      deg_seq_dist_cali = deg_seq_distance;
                else if(decision_mode == 2) deg_seq_dist_cali = deg_seq_distance;
                else if(decision_mode == 3) deg_seq_dist_cali = deg_seq_distance + 1;
                else                        deg_seq_dist_cali = deg_seq_distance;
        end
    endfunction
    /***********************************************************************/
    function    integer deg_passLast_en;
        input   integer decision_mode;

        begin
            if(decision_mode == 1)      deg_passLast_en = 0;
            else if(decision_mode == 2) deg_passLast_en = 0;
            else if(decision_mode == 3) deg_passLast_en = 1;
            else                        deg_passLast_en = 0;
        end
    endfunction
    /***********************************************************************/
    /***********************************************************************/
    function    integer errCntInfoLoaderEn;
        input   integer decision_mode;

        begin
            if(decision_mode == 1)          errCntInfoLoaderEn = 1;
            else if(decision_mode == 2)     errCntInfoLoaderEn = 1;
            else if(decision_mode == 3)     errCntInfoLoaderEn = 0;
            else                            errCntInfoLoaderEn = 1;
        end
    endfunction
    /***********************************************************************/
    function    integer errCntPriorSelPathEn;
        input   integer decision_mode;

        begin
            if(decision_mode == 1)          errCntPriorSelPathEn = 0;
            else if(decision_mode == 2)     errCntPriorSelPathEn = 0;
            else if(decision_mode == 3)     errCntPriorSelPathEn = 1;
            else                            errCntPriorSelPathEn = 0;
        end
    endfunction
    /***********************************************************************/
    function    integer errCnt_input_bus_en;
        input   integer decision_mode;

        begin
            if(decision_mode == 0)  errCnt_input_bus_en = 1;
            else if(decision_mode == 1)  errCnt_input_bus_en = 1;
            else if(decision_mode == 2)  errCnt_input_bus_en = 1;
            else if(decision_mode == 3)  errCnt_input_bus_en = 1;
            else begin
                $display("in decision_controller_1) decision_mode has wrong value(%d)", decision_mode);
                errCnt_input_bus_en = 0;
            end
        end
    endfunction
    /***********************************************************************/
    function    integer errCnt_infoLoader_in_bus_en;
        input   integer errCntPriorSelPathEn;
        input   integer errCntInBusEn;

        begin
            if(errCntPriorSelPathEn)    errCnt_infoLoader_in_bus_en = 0;
            else                        errCnt_infoLoader_in_bus_en = errCntInBusEn;
        end
    endfunction
    /***********************************************************************/
    function    integer errCnt_output_bus_en;
        input   integer decision_mode;

        begin
            if(decision_mode == 0)  errCnt_output_bus_en = 1;
            else if(decision_mode == 1)  errCnt_output_bus_en = 1;
            else if(decision_mode == 2)  errCnt_output_bus_en = 1;
            else if(decision_mode == 3)  errCnt_output_bus_en = 0;
            else begin
                $display("in decision_controller_1) decision_mode has wrong value(%d)", decision_mode);
                errCnt_output_bus_en = 0;
            end
        end
    endfunction
    /***********************************************************************/
    function    integer errCnt_seq_dist_cali;
        input   integer decision_mode;
        input   integer errCnt_seq_distance;

        begin
                if(decision_mode == 1)      errCnt_seq_dist_cali = errCnt_seq_distance;
                else if(decision_mode == 2) errCnt_seq_dist_cali = errCnt_seq_distance;
                else if(decision_mode == 3) errCnt_seq_dist_cali = errCnt_seq_distance;
                else                        errCnt_seq_dist_cali = errCnt_seq_distance;
        end
    endfunction
    /***********************************************************************/
    function    integer errCnt_passLast_en;
        input   integer decision_mode;

        begin
            if(decision_mode == 1)      errCnt_passLast_en = 0;
            else if(decision_mode == 2) errCnt_passLast_en = 0;
            else if(decision_mode == 3) errCnt_passLast_en = 0;
            else                        errCnt_passLast_en = 0;
        end
    endfunction
    /***********************************************************************/
    /***********************************************************************/
    function    integer useIndicatorParallelBuf;
        input   integer decision_mode;

        begin
            if(decision_mode == 1)      useIndicatorParallelBuf = 1;
            else if(decision_mode == 2) useIndicatorParallelBuf = 1;
            else if(decision_mode == 3) useIndicatorParallelBuf = 0;
            else                    useIndicatorParallelBuf = 1;
        end
    endfunction
    /***********************************************************************/
    function    integer useIndcatorSer2SimBufEn;
        input   integer decision_mode;

        begin
            if(decision_mode == 1)      useIndcatorSer2SimBufEn = 0;
            else if(decision_mode == 2) useIndcatorSer2SimBufEn = 0;
            else if(decision_mode == 3) useIndcatorSer2SimBufEn = 1;
            else                        useIndcatorSer2SimBufEn = 0;
        end
    endfunction
    /***********************************************************************/
    /***********************************************************************/
    genvar i;

    parameter   OUTTER_NAME   = "";
    parameter   MODULE_NAME     = "decision_controller_1";
        localparam  DESIGN_NAME = "decision_controller_1";



    /* input / output buffer start */
    parameter   IN_BUF_LEN  =   0;
    parameter   OUT_BUF_LEN =   0;
    /* input / output buffer end */

    /*
    * Decision Mode
    * 0 : Save Error vector to fifo channel
    *       Sequentially get error polynomial, which needs 2^p cycle, they are
    *       saved in FIFO channel in mSBS, out simultaneously. After
    *       this process, simulataneously do Chien search and metric check.
    *       The results are saved FIFO channel in Chien search and metric
    *       block.
    *       Then decide error vector to correct.
    *       
    *       Degree infomation has simultaneous process. 
    *       Metric infomation has simultaneous process. 
    *
    *
    * 1 : Do simultaneous processing that pre-Chien search(and metric check) 
    *       and Chien search(Polynomial Decision)
    *       
    *       Degree infomation has simultaneous process.
    *       Metric infomation has simultaneous process. 
    *
    *
    * 2 : Do sequential processing that pre-Chien search(and metric check)
    *       and Chien search(Polynomial Decision)
    *
    *       Getting error polynomial needs 2^p cycle and pre-Chien search
    *       blocks are initialized by out value of this module.
    *       Metric values out sequentially and decision also has sequential 
    *       process.
    *       
    *       Degree infomation has sequential process.
    *       Metric infomation has sequential process. 
    */
    /***********************************************************************/
    /***********************************************************************/
    parameter   DECISION_MODE   =   3;




    parameter   DECISION_NUMS                           =   8;
        localparam  LO_PA_DECISION_NUMS              =   notBeingZero(DECISION_NUMS);



        localparam  LO_PA_INFO_LOADER_EN_DEG         =   degInfoLoaderEn(DECISION_MODE);
    parameter   CLK_DISTANCE_DEG                        =   1;
        localparam  LO_PA_CLK_DISTANCE_DEG               =   notBeingZero(CLK_DISTANCE_DEG);;

    parameter   SEQ_DISTANCE_DEG                        =   2;
        localparam  LO_PA_SEQ_DISTANCE_DEG           =   
            deg_seq_dist_cali(DECISION_MODE, minValChker(2, SEQ_DISTANCE_DEG));
        localparam  LO_PA_GLUE_BLK_DEG               =   
            LO_PA_SEQ_DISTANCE_DEG != 0 ? LO_PA_SEQ_DISTANCE_DEG - 2 : 0;

    parameter   BIT_WIDTH_DEG                           =   2;
        localparam  LO_PA_BIT_WIDTH_DEG              =   notBeingZero(BIT_WIDTH_DEG);

        localparam  LO_PA_IN_BUS_EN_DEG              =   deg_input_bus_en(DECISION_MODE);
            localparam  LO_PA_IN_N_BUS_EN_DEG        =   valueToNotBool(LO_PA_IN_BUS_EN_DEG);
        localparam  LO_PA_OUT_BUS_EN_DEG             =   deg_output_bus_en(DECISION_MODE);
            localparam  LO_PA_OUT_N_BUS_EN_DEG       =   valueToNotBool(LO_PA_OUT_BUS_EN_DEG);

        localparam  LO_PA_IN_WIDTH_DEG       =   
            ((LO_PA_DECISION_NUMS - 1) * LO_PA_IN_BUS_EN_DEG + 1) * LO_PA_BIT_WIDTH_DEG;
        localparam  LO_PA_OUT_WIDTH_DEG      =   
            ((LO_PA_DECISION_NUMS - 1) * LO_PA_OUT_BUS_EN_DEG + 1) * LO_PA_BIT_WIDTH_DEG;

        localparam  LO_PA_PASSING_LAST_EN_DEG         =   deg_passLast_en(DECISION_MODE);

        localparam  LO_PA_USE_INIT_SIG_FIR_DEG       =   LO_PA_IN_BUS_EN_DEG;
        
        localparam  LO_PA_OUT_SIG_EN_DEG             =   0;

    /* Controlling signal indexing start */
    localparam  LO_IDX_CTR_SIG_CTR_DEG_START  = -1;
        localparam  LO_IDX_CTR_SIG_INFOLOAD_EN_FIR_DEG         = 
            LO_IDX_CTR_SIG_CTR_DEG_START + 
            LO_PA_INFO_LOADER_EN_DEG * LO_PA_IN_N_BUS_EN_DEG;
        localparam  LO_IDX_CTR_SIG_INFOLOAD_EN_LAS_DEG         = 
            LO_IDX_CTR_SIG_INFOLOAD_EN_FIR_DEG + 
            LO_PA_INFO_LOADER_EN_DEG * LO_PA_OUT_N_BUS_EN_DEG;
        localparam  LO_IDX_CTR_SIG_INFOLOAD_PASS_LAST_DEG      = 
            LO_IDX_CTR_SIG_INFOLOAD_EN_LAS_DEG + 
            LO_PA_INFO_LOADER_EN_DEG * LO_PA_PASSING_LAST_EN_DEG;
        
        localparam  LO_IDX_CTR_SIG_INFOLOAD_INIT_FIR_DEG           = 
            LO_IDX_CTR_SIG_INFOLOAD_PASS_LAST_DEG + 
            LO_PA_INFO_LOADER_EN_DEG * LO_PA_USE_INIT_SIG_FIR_DEG;
            
        localparam  LO_IDX_CTR_SIG_INFOLOAD_INIT_BUS_MID_START_DEG  = 
            cal_bus_index_start(LO_IDX_CTR_SIG_INFOLOAD_INIT_FIR_DEG, 
            LO_PA_INFO_LOADER_EN_DEG * LO_PA_GLUE_BLK_DEG);
        localparam  LO_IDX_CTR_SIG_INFOLOAD_INIT_BUS_MID_END_DEG    = 
            cal_bus_index_end(LO_IDX_CTR_SIG_INFOLOAD_INIT_BUS_MID_START_DEG, 
            LO_PA_INFO_LOADER_EN_DEG * LO_PA_GLUE_BLK_DEG);
        localparam  LO_IDX_CTR_SIG_INFOLOAD_INIT_SIG_LAS_DEG               = 
            LO_IDX_CTR_SIG_INFOLOAD_INIT_BUS_MID_END_DEG + LO_PA_INFO_LOADER_EN_DEG * 1; 
        localparam  LO_IDX_CTR_SIG_INFOLOAD_OUT_EN_DEG         = 
            LO_IDX_CTR_SIG_INFOLOAD_INIT_SIG_LAS_DEG + LO_PA_INFO_LOADER_EN_DEG * LO_PA_OUT_SIG_EN_DEG;
    localparam  LO_IDX_CTR_SIG_CTR_DEG_END    = LO_IDX_CTR_SIG_INFOLOAD_OUT_EN_DEG;
    /* Controlling signal indexing end */

        localparam  LO_PA_CTR_INFO_BUS_WIDTH_DEG     = 
            LO_PA_INFO_LOADER_EN_DEG * (LO_IDX_CTR_SIG_CTR_DEG_END + 1);


            localparam  LO_PA_EN_INFO_LOADER_FOR_ERR_CNT   =   errCntInfoLoaderEn(DECISION_MODE);
    parameter   CLK_DISTANCE_ERR_CNT            =   1;
        localparam  LO_PA_CLK_DISTANCE_ERR_CNT   =   notBeingZero(CLK_DISTANCE_ERR_CNT);

    parameter   SEQ_DISTANCE_ERR_CNT            =   1;
        localparam  LO_PA_SEQ_DISTANCE_ERR_CNT   =   
            errCnt_seq_dist_cali(DECISION_MODE, minValChker(2, SEQ_DISTANCE_ERR_CNT));
        localparam  LO_PA_GLUE_BLK_ERR_CNT   =   
            (LO_PA_SEQ_DISTANCE_ERR_CNT != 0) ? LO_PA_SEQ_DISTANCE_ERR_CNT - 2 : 0;

    parameter   BIT_WIDTH_ERR_CNT               =   2;
        localparam  LO_PA_BIT_WIDTH_ERR_CNT  =   notBeingZero(BIT_WIDTH_ERR_CNT);

        localparam  LO_PA_IN_BUS_EN_ERR_CNT  =   errCnt_input_bus_en(DECISION_MODE);
            localparam  LO_PA_IN_N_BUS_EN_ERR_CNT  =   valueToNotBool(LO_PA_IN_BUS_EN_ERR_CNT);
        localparam  LO_PA_OUT_BUS_EN_ERR_CNT =   errCnt_output_bus_en(DECISION_MODE);
            localparam  LO_PA_OUT_N_BUS_EN_ERR_CNT =   valueToNotBool(LO_PA_OUT_BUS_EN_ERR_CNT);

            localparam  LO_PA_EN_PRIOR_SEL_PATH_FOR_INFOLOAD_ERCNT =   errCntPriorSelPathEn(DECISION_MODE); 

            localparam  LO_PA_PRIOR_SEL_PATH_NUMS_FOR_INFOLOAD_ERCNT = 
                LO_PA_EN_PRIOR_SEL_PATH_FOR_INFOLOAD_ERCNT * LO_PA_DECISION_NUMS;
            localparam  LO_PA_PRIOR_SEL_PATH_BIT_WIDTH_FOR_INFOLOAD_ERCNT    =   
                LO_PA_PRIOR_SEL_PATH_NUMS_FOR_INFOLOAD_ERCNT * LO_PA_BIT_WIDTH_ERR_CNT;

            localparam  LO_PA_INFOLOAD_IN_BUS_EN_ERCNT  =  
                errCnt_infoLoader_in_bus_en(LO_PA_EN_PRIOR_SEL_PATH_FOR_INFOLOAD_ERCNT, 
                LO_PA_IN_BUS_EN_ERR_CNT); 
            localparam  LO_PA_INFOLOAD_IN_N_BUS_EN_ERCNT    =   valueToNotBool(LO_PA_INFOLOAD_IN_BUS_EN_ERCNT);
                
            localparam  LO_PA_INFOLOAD_OUT_BUS_EN_ERCNT  =  LO_PA_OUT_N_BUS_EN_ERR_CNT;
            localparam  LO_PA_INFOLOAD_OUT_N_BUS_EN_ERCNT    =   valueToNotBool(LO_PA_INFOLOAD_OUT_BUS_EN_ERCNT);
                

        localparam  LO_PA_IN_WIDTH_ERR_CNT   =   
            ((LO_PA_DECISION_NUMS - 1) * LO_PA_IN_BUS_EN_ERR_CNT + 1) * LO_PA_BIT_WIDTH_ERR_CNT;
        localparam  LO_PA_OUT_WIDTH_ERR_CNT  =   
            ((LO_PA_DECISION_NUMS - 1) * LO_PA_OUT_BUS_EN_ERR_CNT + 1) * LO_PA_BIT_WIDTH_ERR_CNT;

        localparam  LO_PA_INFOLOAD_IN_WIDTH_ERR_CNT =   
            ((LO_PA_DECISION_NUMS - 1) * LO_PA_INFOLOAD_IN_BUS_EN_ERCNT + 1) * LO_PA_BIT_WIDTH_ERR_CNT;
        localparam  LO_PA_INFOLOAD_OUT_WIDTH_ERR_CNT =   
            ((LO_PA_DECISION_NUMS - 1) * LO_PA_INFOLOAD_OUT_BUS_EN_ERCNT + 1) * LO_PA_BIT_WIDTH_ERR_CNT;

        localparam  LO_PA_PASSING_LAST_EN_ERR_CNT    =  errCnt_passLast_en(DECISION_MODE);

        localparam  LO_PA_USE_INIT_SIG_FIR_ERR_CNT       =   LO_PA_IN_BUS_EN_ERR_CNT;

        localparam  LO_PA_OUT_SIG_EN_ERR_CNT         =   0;

    /* Controlling signal indexing start */
    localparam  LO_IDX_CTR_SIG_INFOLOAD_CTR_ERCNT_START  = LO_IDX_CTR_SIG_CTR_DEG_END;
        localparam  LO_IDX_CTR_SIG_INFOLOAD_EN_FIR_ERCNT         = 
            LO_IDX_CTR_SIG_INFOLOAD_CTR_ERCNT_START + 
            LO_PA_EN_INFO_LOADER_FOR_ERR_CNT * LO_PA_INFOLOAD_IN_N_BUS_EN_ERCNT;
        localparam  LO_IDX_CTR_SIG_INFOLOAD_EN_LAS_ERCNT         = 
            LO_IDX_CTR_SIG_INFOLOAD_EN_FIR_ERCNT + 
            LO_PA_EN_INFO_LOADER_FOR_ERR_CNT * LO_PA_INFOLOAD_OUT_N_BUS_EN_ERCNT;
        localparam  LO_IDX_CTR_SIG_INFOLOAD_PSLAST_ERCNT      = 
            LO_IDX_CTR_SIG_INFOLOAD_EN_LAS_ERCNT + 
            LO_PA_EN_INFO_LOADER_FOR_ERR_CNT * LO_PA_PASSING_LAST_EN_ERR_CNT;

        localparam  LO_IDX_CTR_SIG_INFOLOAD_INIT_FIR_ERCNT           =   
            LO_IDX_CTR_SIG_INFOLOAD_PSLAST_ERCNT + 
            LO_PA_EN_INFO_LOADER_FOR_ERR_CNT * LO_PA_USE_INIT_SIG_FIR_ERR_CNT;

        localparam  LO_IDX_CTR_SIG_INFOLOAD_INIT_MID_START_ERCNT  = 
            cal_bus_index_start(LO_IDX_CTR_SIG_INFOLOAD_INIT_FIR_ERCNT, 
            LO_PA_EN_INFO_LOADER_FOR_ERR_CNT * LO_PA_GLUE_BLK_ERR_CNT);
        localparam  LO_IDX_CTR_SIG_INFOLOAD_INIT_MID_END_ERCNT    = 
            cal_bus_index_end(LO_IDX_CTR_SIG_INFOLOAD_INIT_MID_START_ERCNT,   
            LO_PA_EN_INFO_LOADER_FOR_ERR_CNT * LO_PA_GLUE_BLK_ERR_CNT);
        localparam  LO_IDX_CTR_SIG_INFOLOAD_INIT_LAS_ERCNT       =
            LO_IDX_CTR_SIG_INFOLOAD_INIT_MID_END_ERCNT + LO_PA_EN_INFO_LOADER_FOR_ERR_CNT * 1;
        localparam  LO_IDX_CTR_SIG_INFOLOAD_OUT_EN_ERCNT         = 
            LO_IDX_CTR_SIG_INFOLOAD_INIT_LAS_ERCNT + LO_PA_EN_INFO_LOADER_FOR_ERR_CNT * LO_PA_OUT_SIG_EN_ERR_CNT;
    localparam  LO_IDX_CTR_SIG_INFOLOAD_CTR_ERCNT_END        = LO_IDX_CTR_SIG_INFOLOAD_OUT_EN_ERCNT;
    /* Controlling signal indexing end */

        localparam  LO_PA_CTR_SIG_WIDTH_FOR_ERCNT     = 
            LO_PA_EN_INFO_LOADER_FOR_ERR_CNT * (LO_IDX_CTR_SIG_INFOLOAD_OUT_EN_ERCNT + 1);


    /* Controlling signal indexing start */
    localparam  LO_PA_PRIOR_SEL_NUMS = 
        LO_PA_DECISION_NUMS * LO_PA_EN_PRIOR_SEL_PATH_FOR_INFOLOAD_ERCNT;


    localparam  LO_IDX_CTR_SIG_PRIOR_SEL_START = LO_IDX_CTR_SIG_INFOLOAD_CTR_ERCNT_END;

    localparam  LO_IDX_CTR_SIG_PRIOR_SEL_BUS_START = cal_bus_index_start(LO_IDX_CTR_SIG_PRIOR_SEL_START,
        LO_PA_EN_PRIOR_SEL_PATH_FOR_INFOLOAD_ERCNT * LO_PA_PRIOR_SEL_NUMS);
    localparam  LO_IDX_CTR_SIG_PRIOR_SEL_BUS_END = cal_bus_index_end(LO_IDX_CTR_SIG_PRIOR_SEL_BUS_START,
        LO_PA_EN_PRIOR_SEL_PATH_FOR_INFOLOAD_ERCNT * LO_PA_PRIOR_SEL_NUMS);

    localparam  LO_IDX_CTR_SIG_PRIOR_SEL_END = LO_IDX_CTR_SIG_PRIOR_SEL_BUS_END;

    localparam  LO_PA_CTR_SIG_WIDTH_FOR_PRIOR_SEL = 
        LO_PA_EN_PRIOR_SEL_PATH_FOR_INFOLOAD_ERCNT * (LO_IDX_CTR_SIG_PRIOR_SEL_END + 1);
    /* Controlling signal indexing end */



    /* About indicator */
        localparam  LO_PA_INDICATOR_OUT_BUF_EN   =   useIndicatorParallelBuf(DECISION_MODE);
        localparam  LO_PA_USED_SER2SIM_OUT_BUF_VALID_SIG_EN  =   useIndcatorSer2SimBufEn(DECISION_MODE); 
    /* Controll signal Index of indicator*/
            localparam  LO_IDX_CTR_SIG_CTR_TO_INDIC_START   = LO_IDX_CTR_SIG_PRIOR_SEL_END;
        localparam  LO_IDX_CTR_SIG_INDIC_PARALLEL_OUT_INIT =
            LO_IDX_CTR_SIG_CTR_TO_INDIC_START + LO_PA_INDICATOR_OUT_BUF_EN;
        localparam  LO_IDX_CTR_SIG_SEQ2SIM_EN =   
            LO_IDX_CTR_SIG_INDIC_PARALLEL_OUT_INIT + LO_PA_USED_SER2SIM_OUT_BUF_VALID_SIG_EN;

            localparam  LO_IDX_CTR_SIG_CTR_TO_INDIC_END     = LO_IDX_CTR_SIG_SEQ2SIM_EN;
                
            
        localparam  LO_PA_CTR_INFO_BUS_WIDTH =   LO_IDX_CTR_SIG_CTR_TO_INDIC_END + 1;

    /* out_tp_sel bus index calculation*/
    /* out decision controll signal out_tp_sel bus width calculaion start*/
        localparam  LO_PA_OUT_SEL_TP_BUS_WIDTH    =   LO_PA_DECISION_NUMS;

    localparam  LO_IDX_OUT_DEC_CTR_START      = -1; 
        localparam  LO_IDX_BUS_OUT_SEL_TP_SIG_START  = LO_IDX_OUT_DEC_CTR_START + 1;
        localparam  LO_IDX_BUS_OUT_SEL_TP_SIG_END    =   
            LO_IDX_BUS_OUT_SEL_TP_SIG_START + LO_PA_OUT_SEL_TP_BUS_WIDTH - 1;

        localparam  LO_IDX_OUT_USED_INDIC_SER2SIM_OUT_BUF_AND_VALID_SIG = 
                        LO_IDX_BUS_OUT_SEL_TP_SIG_END + 
                        LO_PA_USED_SER2SIM_OUT_BUF_VALID_SIG_EN;
        localparam  LO_IDX_OUT_DEC_CTR_END    = 
                        LO_IDX_OUT_USED_INDIC_SER2SIM_OUT_BUF_AND_VALID_SIG;

    localparam  LO_PA_OUT_DEC_CTR_SIG_WIDTH  = LO_IDX_OUT_DEC_CTR_END + 1;
    /* out decision controll signal out_tp_sel bus width calculaion end*/

    /***********************************************************************/
    /***********************************************************************/
    input   clk;
    input   in_ctr_Srst;
    input   in_ctr_en;
    /***********************************************************************/
    input   [LO_PA_CTR_INFO_BUS_WIDTH - 1 : 0]   in_ctr_sig;
    /***********************************************************************/

    input   [LO_PA_IN_WIDTH_DEG - 1 : 0]         in_tp_deg;

    input   [LO_PA_IN_WIDTH_ERR_CNT - 1 : 0]     in_tp_erCnt;
    /***********************************************************************/
    wire    [LO_PA_IN_WIDTH_DEG - 1 : 0]         w_in_tp_deg;

    wire    [LO_PA_INFOLOAD_IN_WIDTH_ERR_CNT - 1 : 0]     w_tp_erCnt_to_indi;
    /***********************************************************************/

    /***********************************************************************/
    /***********************************************************************/
    output  [LO_PA_OUT_DEC_CTR_SIG_WIDTH - 1 : 0]        out_tp_sel;
    /***********************************************************************/
    /***********************************************************************/
    wire                                            w_indic_outBufShEn;
    /***********************************************************************/
    /***********************************************************************/

    /*Degree Infomation Loader start*/
    generate
        if(LO_PA_INFO_LOADER_EN_DEG) begin : gen_infoLoader_deg
            ctr_infoLoaders_independent
            #(
                .OUTTER_NAME("decision_controller_1"),
                .MODULE_NAME("uut_ctr_infoLoader_deg"),

                .IN_BUF_LEN(IN_BUF_LEN),
                .OUT_BUF_LEN(OUT_BUF_LEN),

                .IND_DISTANCE_INFO(LO_PA_SEQ_DISTANCE_DEG),
                .CLK_DISTANCE_INFO(LO_PA_CLK_DISTANCE_DEG),

                .PASSING_LAST_EN(LO_PA_PASSING_LAST_EN_DEG),
                .OUT_SIG_EN(LO_PA_OUT_SIG_EN_DEG),

                .INPUT_BUS_EN(LO_PA_IN_BUS_EN_DEG),
                .OUTPUT_BUS_EN(LO_PA_OUT_BUS_EN_DEG),

                .INFO_NUMS(LO_PA_DECISION_NUMS),
                .BIT_WIDTH_INFO(LO_PA_BIT_WIDTH_DEG)
            )
            uut_ctr_infoLoader_deg
            (
                .clk(clk),
                .in_ctr_Srst(in_ctr_Srst),
                .in_ctr_en(in_ctr_en),

                .in_ctr_sig(in_ctr_sig[LO_IDX_CTR_SIG_CTR_DEG_END : LO_IDX_CTR_SIG_CTR_DEG_START + 1]),
                .in_info(in_tp_deg),

                .out_info(w_in_tp_deg)
            );
        end
        else begin : gen_wiring_deg
                initial $display("WARRNING!!!) in %s, label : gen_wiring_deg", DESIGN_NAME);
                assign w_in_tp_deg = in_tp_deg;
        end
    endgenerate
    /*Degree Infomation Loader end*/


    /*Error count Infomation Loader Start*/
    generate
        if(LO_PA_EN_INFO_LOADER_FOR_ERR_CNT) begin : gen_infoLoader_errCnt
            wire    [LO_PA_INFOLOAD_IN_WIDTH_ERR_CNT - 1 : 0]    w_to_infoL_erCnt;
            
            if(LO_PA_EN_PRIOR_SEL_PATH_FOR_INFOLOAD_ERCNT) begin : priorSelErCnt
                wire    [LO_PA_DECISION_NUMS - 1 : 0]  w_selPriorErCnt;
                assign  w_selPriorErCnt = in_ctr_sig[LO_IDX_CTR_SIG_PRIOR_SEL_BUS_END : LO_IDX_CTR_SIG_PRIOR_SEL_BUS_START];

                prior_w_di_sel_path
                #(
                    .BIT_WIDTH(LO_PA_BIT_WIDTH_DEG),
                    .SEL_SIG_NUMS(LO_PA_DECISION_NUMS),
                    .PRIORITY_DIRECTION(-1),
                    .DEFAULT_INPUT_EN(1)
                )
                uut_priorSelErCnt
                (
                    .in({(LO_PA_BIT_WIDTH_DEG){1'b0}}), 
                    .out(w_to_infoL_erCnt), 
                    .sel(w_selPriorErCnt)
                );
            end
            else begin : wiring_in_tp_erCnt_to_infoloader_ForErCnt
                assign  w_to_infoL_erCnt = in_tp_erCnt;
            end

            ctr_infoLoaders_independent
            #(
                .OUTTER_NAME("decision_controller_1"),
                .MODULE_NAME("uut_ctr_infoLoader_errCnt"),

                .IN_BUF_LEN(IN_BUF_LEN),
                .OUT_BUF_LEN(OUT_BUF_LEN),

                .IND_DISTANCE_INFO(LO_PA_SEQ_DISTANCE_ERR_CNT),
                .CLK_DISTANCE_INFO(LO_PA_CLK_DISTANCE_ERR_CNT),

                .PASSING_LAST_EN(LO_PA_PASSING_LAST_EN_ERR_CNT),
                .OUT_SIG_EN(LO_PA_OUT_SIG_EN_ERR_CNT),

                .INPUT_BUS_EN(LO_PA_INFOLOAD_IN_BUS_EN_ERCNT),
                .OUTPUT_BUS_EN(LO_PA_INFOLOAD_OUT_BUS_EN_ERCNT),

                .INFO_NUMS(LO_PA_SEQ_DISTANCE_ERR_CNT),
                .BIT_WIDTH_INFO(LO_PA_BIT_WIDTH_ERR_CNT)
            )
            uut_ctr_infoLoader_errCnt
            (
                .clk(clk),
                .in_ctr_Srst(in_ctr_Srst),
                .in_ctr_en(in_ctr_en),

                .in_ctr_sig(in_ctr_sig[LO_IDX_CTR_SIG_INFOLOAD_CTR_ERCNT_END : LO_IDX_CTR_SIG_INFOLOAD_CTR_ERCNT_START + 1]),
                .in_info(w_to_infoL_erCnt),
                .out_info(w_tp_erCnt_to_indi)
            );
        end
        else begin : gen_wiring_errCnt
            assign w_in_tp_deg = in_tp_deg; 
        end
    endgenerate
    /*Error count Infomation Loader End*/

generate
    if(LO_PA_INDICATOR_OUT_BUF_EN) begin : gen_indic_outBufEn

        assign  w_indic_outBufShEn = in_ctr_sig[LO_IDX_CTR_SIG_INDIC_PARALLEL_OUT_INIT];
    end

    if(LO_PA_USED_SER2SIM_OUT_BUF_VALID_SIG_EN) begin : gen_seq2simOutBufShEn
        assign w_indic_outBufShEn =   in_ctr_sig[LO_IDX_CTR_SIG_SEQ2SIM_EN];
    end
endgenerate
    decision_indicator
    #(
        .OUTTER_NAME(DESIGN_NAME),
        .MODULE_NAME("uut_decision_indocator"),

        .DECISION_MODE(DECISION_MODE),

        .DECISION_NUMS(LO_PA_DECISION_NUMS),

        .BIT_WIDTH_DEG(LO_PA_BIT_WIDTH_DEG),

        .BIT_WIDTH_ERR_CNT(LO_PA_BIT_WIDTH_ERR_CNT),

        .USED_INDIC_PARALLEL_OUT_BUF_EN(LO_PA_INDICATOR_OUT_BUF_EN),
        .USED_INDIC_SER2SIM_OUT_BUF_AND_VALID_SIG_EN(LO_PA_USED_SER2SIM_OUT_BUF_VALID_SIG_EN)
    )
    uut_decision_indocator
    (
        .clk(clk),
        .in_ctr_Srst(in_ctr_Srst),
        .in_ctr_en(in_ctr_en),
        .in_ctr_shEnOutBuf(w_indic_outBufShEn),

        .in_tp_deg(w_in_tp_deg),
        .in_tp_errCnt(w_tp_erCnt_to_indi),

        .out_indicTP(out_tp_sel)
    );

    /***********************************************************************/
    /***********************************************************************/

    initial begin
        $display("/**************************************************/");
        $display("Instanciating %s", DESIGN_NAME);

        $display("In %s," , OUTTER_NAME);
        $display("MODULE_NAME : %s", MODULE_NAME);
        $display("DESIGN_NAME : %s", DESIGN_NAME);

        if(DECISION_MODE != 3) begin
            $display("WARRNING!!!) decision_controller_1 block only support decision mode 3.");
            $display("Now, this block is setted to decision mode %d", DECISION_MODE);
        end

        $display("IN_BUF_LEN = %d", IN_BUF_LEN);
        $display("OUT_BUF_LEN = %d", OUT_BUF_LEN);

        $display("DECISION_MODE = %d", DECISION_MODE);
        $display("DECISION_NUMS = %d", DECISION_NUMS);
        $display("LO_PA_DECISION_NUMS = %d", LO_PA_DECISION_NUMS);

    $display("=====> About degree.");
        $display("LO_PA_INFO_LOADER_EN_DEG : %s", 
            LO_PA_INFO_LOADER_EN_DEG ? "enabled" : "disabled");

        $display("CLK_DISTANCE_DEG = %d", CLK_DISTANCE_DEG);
        $display("LO_PA_CLK_DISTANCE_DEG = %d", LO_PA_CLK_DISTANCE_DEG);

        $display("SEQ_DISTANCE_DEG = %d", SEQ_DISTANCE_DEG);
        $display("LO_PA_SEQ_DISTANCE_DEG = %d", LO_PA_SEQ_DISTANCE_DEG);
            $display("LO_PA_GLUE_BLK_DEG = %d", LO_PA_GLUE_BLK_DEG);

        $display("BIT_WIDTH_DEG = %d", BIT_WIDTH_DEG);
            $display("LO_PA_BIT_WIDTH_DEG = %d", LO_PA_BIT_WIDTH_DEG);

        $display("LO_PA_IN_BUS_EN_DEG : %s", 
            LO_PA_IN_BUS_EN_DEG ? "enabled" : "disabled");
            $display("LO_PA_IN_N_BUS_EN_DEG : %s", 
                LO_PA_IN_N_BUS_EN_DEG ? "enabled" : "disabled");
        $display("LO_PA_OUT_BUS_EN_DEG : %s",
            LO_PA_OUT_BUS_EN_DEG ? "enabled" : "disabled");
            $display("LO_PA_OUT_N_BUS_EN_DEG : %s", 
                LO_PA_OUT_N_BUS_EN_DEG ? "enabled" : "disabled");

        $display("LO_PA_IN_WIDTH_DEG = %d", LO_PA_IN_WIDTH_DEG);
        $display("LO_PA_OUT_WIDTH_DEG = %d", LO_PA_OUT_WIDTH_DEG);

        $display("LO_PA_PASSING_LAST_EN_DEG : %s", 
            LO_PA_PASSING_LAST_EN_DEG ? "enabled" : "disabled");

        $display("LO_PA_USE_INIT_SIG_FIR_DEG : %s", 
            LO_PA_USE_INIT_SIG_FIR_DEG ? "enabled" : "disabled");

        $display("LO_PA_OUT_SIG_EN_DEG : %s", 
            LO_PA_OUT_SIG_EN_DEG ? "enabled" : "disalbed");

    $display("LO_IDX_CTR_SIG_CTR_DEG_START = %d", LO_IDX_CTR_SIG_CTR_DEG_START);
        $display("LO_IDX_CTR_SIG_INFOLOAD_EN_FIR_DEG = %d", LO_IDX_CTR_SIG_INFOLOAD_EN_FIR_DEG);
        $display("LO_IDX_CTR_SIG_INFOLOAD_EN_LAS_DEG = %d", LO_IDX_CTR_SIG_INFOLOAD_EN_LAS_DEG);
        $display("LO_IDX_CTR_SIG_INFOLOAD_PASS_LAST_DEG = %d", LO_IDX_CTR_SIG_INFOLOAD_PASS_LAST_DEG);
        $display("LO_IDX_CTR_SIG_INFOLOAD_INIT_FIR_DEG = %d", LO_IDX_CTR_SIG_INFOLOAD_INIT_FIR_DEG);
        $display("LO_IDX_CTR_SIG_INFOLOAD_INIT_BUS_MID_START_DEG = %d", LO_IDX_CTR_SIG_INFOLOAD_INIT_BUS_MID_START_DEG);
        $display("LO_IDX_CTR_SIG_INFOLOAD_INIT_BUS_MID_END_DEG = %d", LO_IDX_CTR_SIG_INFOLOAD_INIT_BUS_MID_END_DEG);
        $display("LO_IDX_CTR_SIG_INFOLOAD_INIT_SIG_LAS_DEG = %d", LO_IDX_CTR_SIG_INFOLOAD_INIT_SIG_LAS_DEG);
        $display("LO_IDX_CTR_SIG_INFOLOAD_OUT_EN_DEG = %d", LO_IDX_CTR_SIG_INFOLOAD_OUT_EN_DEG);
    $display("LO_IDX_CTR_SIG_CTR_DEG_END = %d", LO_IDX_CTR_SIG_CTR_DEG_END);

        $display("LO_PA_CTR_INFO_BUS_WIDTH_DEG = %d", LO_PA_CTR_INFO_BUS_WIDTH_DEG);



    $display("=====> About error count.");
            $display("LO_PA_EN_INFO_LOADER_FOR_ERR_CNT = %s", 
                LO_PA_EN_INFO_LOADER_FOR_ERR_CNT ? "enabled" : "disabled");
        $display("CLK_DISTANCE_ERR_CNT = %d", CLK_DISTANCE_ERR_CNT);
        $display("LO_PA_CLK_DISTANCE_ERR_CNT = %d", LO_PA_CLK_DISTANCE_ERR_CNT);

        $display("SEQ_DISTANCE_ERR_CNT = %d", SEQ_DISTANCE_ERR_CNT);
            $display("LO_PA_SEQ_DISTANCE_ERR_CNT = %d", LO_PA_SEQ_DISTANCE_ERR_CNT);
            $display("LO_PA_GLUE_BLK_ERR_CNT = %d", LO_PA_GLUE_BLK_ERR_CNT);

        $display("BIT_WIDTH_ERR_CNT = %d", BIT_WIDTH_ERR_CNT);
            $display("LO_PA_BIT_WIDTH_ERR_CNT = %d", LO_PA_BIT_WIDTH_ERR_CNT);

        $display("LO_PA_IN_BUS_EN_ERR_CNT : %s", 
            LO_PA_IN_BUS_EN_ERR_CNT ? "enabled" : "disabled");
            $display("LO_PA_IN_N_BUS_EN_ERR_CNT : %s", 
                LO_PA_IN_N_BUS_EN_ERR_CNT ? "enabled" : "disabled");
        $display("LO_PA_OUT_BUS_EN_ERR_CNT : %s", 
            LO_PA_OUT_BUS_EN_ERR_CNT ? "enabled" : "disabled");
            $display("LO_PA_OUT_N_BUS_EN_ERR_CNT : %s", 
                LO_PA_OUT_N_BUS_EN_ERR_CNT ? "enabled" : "disabled");

        $display("LO_PA_EN_PRIOR_SEL_PATH_FOR_INFOLOAD_ERCNT : %s", 
            LO_PA_EN_PRIOR_SEL_PATH_FOR_INFOLOAD_ERCNT ? "enabled" : "disabled");

        $display("LO_PA_PRIOR_SEL_PATH_NUMS_FOR_INFOLOAD_ERCNT = %d", LO_PA_PRIOR_SEL_PATH_NUMS_FOR_INFOLOAD_ERCNT);
        $display("LO_PA_PRIOR_SEL_PATH_BIT_WIDTH_FOR_INFOLOAD_ERCNT = %d", LO_PA_PRIOR_SEL_PATH_BIT_WIDTH_FOR_INFOLOAD_ERCNT);

        $display("LO_PA_INFOLOAD_IN_BUS_EN_ERCNT : %s", 
            LO_PA_INFOLOAD_IN_BUS_EN_ERCNT ? "enabled" : "disabled");
        $display("LO_PA_INFOLOAD_IN_N_BUS_EN_ERCNT : %s", 
            LO_PA_INFOLOAD_IN_N_BUS_EN_ERCNT ? "enabled" : "disabled");

        $display("LO_PA_INFOLOAD_OUT_BUS_EN_ERCNT : %s", 
            LO_PA_INFOLOAD_OUT_BUS_EN_ERCNT ? "enabled" : "disabled");
        $display("LO_PA_INFOLOAD_OUT_N_BUS_EN_ERCNT : %s", 
            LO_PA_INFOLOAD_OUT_N_BUS_EN_ERCNT ? "enabled" : "disabled");

        $display("LO_PA_IN_WIDTH_ERR_CNT = %d", LO_PA_IN_WIDTH_ERR_CNT);
        $display("LO_PA_OUT_WIDTH_ERR_CNT = %d", LO_PA_OUT_WIDTH_ERR_CNT);

        $display("LO_PA_INFOLOAD_IN_WIDTH_ERR_CNT = %d", LO_PA_INFOLOAD_IN_WIDTH_ERR_CNT);
        $display("LO_PA_INFOLOAD_OUT_WIDTH_ERR_CNT = %d", LO_PA_INFOLOAD_OUT_WIDTH_ERR_CNT);

        $display("LO_PA_PASSING_LAST_EN_ERR_CNT : %s", 
            LO_PA_PASSING_LAST_EN_ERR_CNT ? "enabled" : "disabled");

        $display("LO_PA_USE_INIT_SIG_FIR_ERR_CNT : %s", 
            LO_PA_USE_INIT_SIG_FIR_ERR_CNT ? "enabled" : "disabled");

        $display("LO_PA_OUT_SIG_EN_ERR_CNT : %s", 
            LO_PA_OUT_SIG_EN_ERR_CNT ? "enabled" : "disabled");

        /* Controlling signal indexing start */
    $display("LO_IDX_CTR_SIG_INFOLOAD_CTR_ERCNT_START = %d", LO_IDX_CTR_SIG_INFOLOAD_CTR_ERCNT_START);
        $display("LO_IDX_CTR_SIG_INFOLOAD_EN_FIR_ERCNT = %d", LO_IDX_CTR_SIG_INFOLOAD_EN_FIR_ERCNT);
        $display("LO_IDX_CTR_SIG_INFOLOAD_EN_LAS_ERCNT = %d", LO_IDX_CTR_SIG_INFOLOAD_EN_LAS_ERCNT);
        $display("LO_IDX_CTR_SIG_INFOLOAD_PSLAST_ERCNT = %d", LO_IDX_CTR_SIG_INFOLOAD_PSLAST_ERCNT);
        $display("LO_IDX_CTR_SIG_INFOLOAD_INIT_FIR_ERCNT = %d", LO_IDX_CTR_SIG_INFOLOAD_INIT_FIR_ERCNT);
        $display("LO_IDX_CTR_SIG_INFOLOAD_INIT_MID_START_ERCNT = %d", LO_IDX_CTR_SIG_INFOLOAD_INIT_MID_START_ERCNT);
        $display("LO_IDX_CTR_SIG_INFOLOAD_INIT_MID_END_ERCNT = %d", LO_IDX_CTR_SIG_INFOLOAD_INIT_MID_END_ERCNT);
        $display("LO_IDX_CTR_SIG_INFOLOAD_INIT_LAS_ERCNT = %d", LO_IDX_CTR_SIG_INFOLOAD_INIT_LAS_ERCNT);
        $display("LO_IDX_CTR_SIG_INFOLOAD_OUT_EN_ERCNT = %d", LO_IDX_CTR_SIG_INFOLOAD_OUT_EN_ERCNT);
    $display("LO_IDX_CTR_SIG_INFOLOAD_CTR_ERCNT_END = %d", LO_IDX_CTR_SIG_INFOLOAD_CTR_ERCNT_END);

        $display("LO_PA_CTR_SIG_WIDTH_FOR_ERCNT = %d", LO_PA_CTR_SIG_WIDTH_FOR_ERCNT);


    $display("=====> About prior sel for error count.");
        $display("LO_PA_PRIOR_SEL_NUMS = %d", LO_PA_PRIOR_SEL_NUMS);

        $display("LO_IDX_CTR_SIG_PRIOR_SEL_START = %d", LO_IDX_CTR_SIG_PRIOR_SEL_START);

        $display("LO_IDX_CTR_SIG_PRIOR_SEL_BUS_START = %d", LO_IDX_CTR_SIG_PRIOR_SEL_BUS_START);
        $display("LO_IDX_CTR_SIG_PRIOR_SEL_BUS_END = %d", LO_IDX_CTR_SIG_PRIOR_SEL_BUS_END);
        $display("LO_IDX_CTR_SIG_PRIOR_SEL_END = %d", LO_IDX_CTR_SIG_PRIOR_SEL_END);

        $display("LO_PA_CTR_SIG_WIDTH_FOR_PRIOR_SEL = %d", LO_PA_CTR_SIG_WIDTH_FOR_PRIOR_SEL);


    $display("=====> About indicator.");
        $display("LO_PA_INDICATOR_OUT_BUF_EN : %s", 
            LO_PA_INDICATOR_OUT_BUF_EN ? "enabled" : "disabled" );
        $display("LO_PA_USED_SER2SIM_OUT_BUF_VALID_SIG_EN : %s", 
            LO_PA_USED_SER2SIM_OUT_BUF_VALID_SIG_EN ? "enabled" : "disabled");
        $display("LO_IDX_CTR_SIG_CTR_TO_INDIC_START = %d", LO_IDX_CTR_SIG_CTR_TO_INDIC_START);
        $display("LO_IDX_CTR_SIG_INDIC_PARALLEL_OUT_INIT = %d", LO_IDX_CTR_SIG_INDIC_PARALLEL_OUT_INIT);
        $display("LO_IDX_CTR_SIG_SEQ2SIM_EN = %d", LO_IDX_CTR_SIG_SEQ2SIM_EN);
        $display("LO_IDX_CTR_SIG_CTR_TO_INDIC_END = %d", LO_IDX_CTR_SIG_CTR_TO_INDIC_END);

        $display("LO_PA_CTR_INFO_BUS_WIDTH = %d, define to in_ctr_sig[%d - 1 : 0]]", 
            LO_PA_CTR_INFO_BUS_WIDTH, LO_PA_CTR_INFO_BUS_WIDTH);

        $display("LO_PA_OUT_SEL_TP_BUS_WIDTH = %d", LO_PA_OUT_SEL_TP_BUS_WIDTH);
        $display("LO_IDX_OUT_DEC_CTR_START = %d", LO_IDX_OUT_DEC_CTR_START);
        $display("LO_IDX_BUS_OUT_SEL_TP_SIG_START = %d", LO_IDX_BUS_OUT_SEL_TP_SIG_START);
        $display("LO_IDX_BUS_OUT_SEL_TP_SIG_END = %d", LO_IDX_BUS_OUT_SEL_TP_SIG_END);
        $display("LO_IDX_OUT_USED_INDIC_SER2SIM_OUT_BUF_AND_VALID_SIG = %d", LO_IDX_OUT_USED_INDIC_SER2SIM_OUT_BUF_AND_VALID_SIG);
        $display("LO_IDX_OUT_DEC_CTR_END = %d", LO_IDX_OUT_DEC_CTR_END);

        $display("LO_PA_OUT_DEC_CTR_SIG_WIDTH = %d", LO_PA_OUT_DEC_CTR_SIG_WIDTH);
        $display("/**************************************************/");
    end

endmodule

`endif

