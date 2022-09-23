/*
 *                                                                          
 * Copyright (c)  :2022-03-14  AIIT Design Systems Inc.   /
 * Permission is hereby granted, free of charge, to any person obtaining  /
 * a copy of this software and associated documentation files (the   /
 * Software), to deal in the Software without restriction, including   /
 * without limitation the rights to use, copy, modify, merge, publish, 
 * distribute, sublicense, and/or sell copies of the Software, and to   /
 * permit persons to whom the Software is furnished to do so, subject to   /
 * the following conditions:  /
 * The above copyright notice and this permission notice shall be included   /
 * in all copies or substantial portions of the Software.  /
 *                                                                          
 * THE SOFTWARE IS PROVIDED AS IS, WITHOUT WARRANTY OF ANY KIND,  /
 * EXPRESS OR IPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF  /
 * ERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  /
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY   /
 * CLAI, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,  /
 * TORT OR OTHERWISE, ARISING FRO, OUT OF OR IN CONNECTION WITH THE   /
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.  /
 *  
 *
 */

#ifndef _AVS_REG__H
#define _AVS_REG__H

#include <string.h>

/* option */
int op_width = 320;
int op_height = 320;
int op_frame_rate = 50;

int op_ctu_size = 64;
int op_min_cu_size = 4;
int op_max_part_ratio = 8;
int op_max_split_times = 6;
int op_min_qt_size = 8;
int op_max_bt_size = 64;
int op_max_eqt_size = 64;
int op_max_dt_size = 64;

int op_i_period = 5;
int op_max_b_frames = 0;
int op_frames = 10;
int op_disable_hgop = 1;
int op_pb_frame_qp_offset = 1;
int op_skip_frames = 0;

int op_signature = 1;
int op_verbose = 1;
int op_input_bit_depth = 8;
int op_internal_bit_depth = 8;
int op_TemporalSubsampleRatio = 1;

int op_qp = 63;
int op_qp_add_frm = 0;
int op_qp_offset_cb = 0;
int op_qp_offset_cr = 0;
int op_qp_offset_adp = 1;
int op_lcu_delta_qp = 0;

int op_deblock = 1;
int op_sao = 1;
int op_alf = 0;

int op_ipf = 0;
int op_tspcm = 0;
int op_dt_intra = 0;
int op_pbt = 0;
int op_amvr = 1;
int op_hmvp = 8;
int op_emvr = 1;
int op_umve = 1;
int op_affine = 1;
int op_smvd = 1;
int op_libpic = 0;

int op_patch_stable = 1;
int op_cross_patch_loopfilter = 1;
int op_patch_ref_colocated = 0;
int op_patch_uniform = 1;
int op_patch_width_in_lcu = 0;
int op_patch_height_in_lcu = 0;

static int op_qp_offset_libpic = -6;

#if 0
int op_wq_enable;
int op_seq_wq_mode;
int op_seq_wq_user;
int op_pic_wq_data_idx;
int op_pic_wq_user;
int op_wq_param;
int op_wq_model;
int op_wq_param_detailed;
int op_wq_param_undetailed;
#endif



#if 0
#ifdef RISCV_REGISTER_RW
#define read_mreg32( addr )				*(volatile unsigned int *)(addr)
#define write_mreg32( addr, val)		*(volatile unsigned int *)(addr)= (volatile unsigned int)(val)
#define read_mreg16( addr )				*(volatile unsigned short *)(addr)
#define write_mreg16( addr, val)		*(volatile unsigned short *)(addr) = (volatile unsigned short)(val)
#define read_mreg8( addr )				*(volatile unsigned char *)(addr)
#define write_mreg8( addr, val)			*(volatile unsigned char *)(addr) = (volatile unsigned char)(val)
#define write_iram32( addr, val)		*(volatile unsigned int *)(addr) = (val)
#define write_iram16( addr,	val)		*(volatile unsigned short *)(addr) =(val)
#define read_iram32(addr)				*(volatile unsigned int *)(addr)
#define read_iram16(addr)				*(volatile unsigned short *)(addr)
#else
#define read_mreg32( addr )
#define write_mreg32( addr, val)
#define read_mreg16( addr )
#define write_mreg16( addr, val)
#define read_mreg8( addr )
#define write_mreg8( addr, val)
#define write_iram32( addr, val)
#define write_iram16( addr,	val)
#define read_iram32(addr)
#define read_iram16(addr)
#endif
#endif

/* write only */
#define IRQ_OPS_BIT ((1 << 0) & 0x0000000E)
/* read only */
#define IRQ_SYNC_BIT    ((1 << 15) & 0x00008000) 

/*! @brief  [AIIT]:	avs register base space    */
#define AVS_REG_BASE										(0xA5060000)
#define AVS_REG_FETCH1_BASE									(AVS_REG_BASE + 0x1000)
#define AVS_REG_FETCH2_BASE									(AVS_REG_BASE + 0x2000)
#define AVS_REG_SAO_BASE									(AVS_REG_BASE + 0x3000)
#define AVS_REG_BS_BASE										(AVS_REG_BASE + 0x4010)
#define DDR_BASE1											(0x80000000)
#define DDR_BASE2											(0x20000000)

#define AVS_REG_CAP	(4096)

/*! @brief  [AIIT]:	avs FETCH1 register base space    */
#define FETCH_CTX_IO_CTX_PINTER_REFP_PTR_OFFSET 			(AVS_REG_FETCH1_BASE + 0x10)
#define REFP_MAP_MV_OFFSET									(AVS_REG_FETCH1_BASE + 0x18)
#define REFP_MAP_REFI_OFFSET								(AVS_REG_FETCH1_BASE + 0x20)
#define REFP_LIST_PTR_OFFSET								(AVS_REG_FETCH1_BASE + 0x28)
#define MAP_SCU_OFFSET										(AVS_REG_FETCH1_BASE + 0x30)
#define MAP_MV_OFFSET										(AVS_REG_FETCH1_BASE + 0x38)
#define MAP_REFI_OFFSET										(AVS_REG_FETCH1_BASE + 0x40)
#define MAP_CU_MODE_OFFSET									(AVS_REG_FETCH1_BASE + 0x48)
#define CTU_CTRL_INPUT_LAMBDA_OFFSET						(AVS_REG_FETCH1_BASE + 0x50)
#define CTU_CTRL_INPUT_SQRT_LAMBDA_OFFSET					(AVS_REG_FETCH1_BASE + 0x58)
#define CTU_CTRL_INPUT_DIST_CHROMA_WEIGHT_OFFSET			(AVS_REG_FETCH1_BASE + 0x60)
#define IMGY_ORG_OFFSET										(AVS_REG_FETCH1_BASE + 0x68)
#define IMGU_ORG_OFFSET										(AVS_REG_FETCH1_BASE + 0x70)
#define IMGV_ORG_OFFSET										(AVS_REG_FETCH1_BASE + 0x78)
#define REFBUFY_OFFSET										(AVS_REG_FETCH1_BASE + 0x80)
#define REFBUFU_OFFSET										(AVS_REG_FETCH1_BASE + 0x88)
#define REFBUFV_OFFSET										(AVS_REG_FETCH1_BASE + 0x90)

/*! @brief  [AIIT]:	avs FETCH2 register base space    */
#define FETCH_CTX_IO_CTX_PTR								(AVS_REG_FETCH2_BASE + 0x10)
#define FETCH_CTX_IO_CTX_RPM_NUM_REFP						(AVS_REG_FETCH2_BASE + 0x18)
#define FETCH_CTX_IO_CTX_PINTER_REFP_PIC_HEIGHT_CHROMA		(AVS_REG_FETCH2_BASE + 0x20)
#define FETCH_CTX_IO_CTX_PINTER_REFP_PIC_HEIGHT_LUMA		(AVS_REG_FETCH2_BASE + 0x28)
#define FETCH_CTX_IO_CTX_PINTER_REFP_PIC_STRIDE_CHROMA		(AVS_REG_FETCH2_BASE + 0x30)
#define FETCH_CTX_IO_CTX_PINTER_REFP_PIC_STRIDE_LUMA		(AVS_REG_FETCH2_BASE + 0x38)
#define FETCH_CTX_IO_CTX_PINTER_REFP_PIC_WIDTH_CHROMA		(AVS_REG_FETCH2_BASE + 0x40)
#define FETCH_CTX_IO_CTX_PINTER_REFP_PIC_WIDTH_LUMA			(AVS_REG_FETCH2_BASE + 0x48)
#define FETCH_CTX_IO_CTX_INFO_PIC_WIDTH						(AVS_REG_FETCH2_BASE + 0x50)
#define FETCH_CTX_IO_CTX_INFO_PIC_HEIGHT					(AVS_REG_FETCH2_BASE + 0x58)
#define FETCH_CTX_IO_CTX_INFO_LOG2_MAX_CUWH					(AVS_REG_FETCH2_BASE + 0x60)
#define FETCH_CTX_IO_CTX_INFO_PIC_WIDTH_IN_LCU				(AVS_REG_FETCH2_BASE + 0x68)
#define FETCH_CTX_IO_CTX_INFO_PIC_HEIGHT_IN_LCU				(AVS_REG_FETCH2_BASE + 0x70)
#define FETCH_CTX_IO_CTX_INFO_PIC_WIDTH_IN_SCU				(AVS_REG_FETCH2_BASE + 0x78)
#define FETCH_CTX_IO_CTX_INFO_PIC_HEIGHT_IN_SCU				(AVS_REG_FETCH2_BASE + 0x80)
#define FETCH_CTX_IO_CTX_INFO_PIC_HEADER_CHROMA_QUANT_PARAM_DELTA_CB	(AVS_REG_FETCH2_BASE + 0x88)
#define FETCH_CTX_IO_CTX_INFO_PIC_HEADER_CHROMA_QUANT_PARAM_DELTA_CR	(AVS_REG_FETCH2_BASE + 0x90)
#define FETCH_CTX_IO_CTX_INFO_PIC_HEADER_SLICE_TYPE			(AVS_REG_FETCH2_BASE + 0x98)
#define FETCH_CTX_IO_SAMPLE_ADAPTIVE_OFFSET_ENABLE_FLAG		(AVS_REG_FETCH2_BASE + 0xa0)
#define FETCH_CTX_IO_TREE_STATUS							(AVS_REG_FETCH2_BASE + 0xa8)
#define FETCH_CTX_IO_STRIDE_LUMA							(AVS_REG_FETCH2_BASE + 0xb0)
#define FETCH_CTX_IO_STRIDE_CHROMA							(AVS_REG_FETCH2_BASE + 0xb8)
#define FETCH_CTX_IO_LCU_CNT								(AVS_REG_FETCH2_BASE + 0xc0)
#define CTX_LCU_CNT											(AVS_REG_FETCH2_BASE + 0xc8)
#define CTX_LCU_CNT_CTRL									(AVS_REG_FETCH2_BASE + 0xcc)		//no valid
#define CTU_CTRL_INPUT_WIDTH								(AVS_REG_FETCH2_BASE + 0xd0)
#define CTU_CTRL_INPUT_HEIGHT								(AVS_REG_FETCH2_BASE + 0xd8)
#define CTU_CTRL_INPUT_F_LCU								(AVS_REG_FETCH2_BASE + 0xe0)
#define CTU_CTRL_INPUT_SLICE_ENABLE_I						(AVS_REG_FETCH2_BASE + 0xe8)
#define CTU_CTRL_INPUT_SLICE_ENABLE_O						(AVS_REG_FETCH2_BASE + 0xf0)		//no valid
#define CTU_CTRL_INPUT_SLICE_ENABLE_O_CTRL					(AVS_REG_FETCH2_BASE + 0xf4)		//no valid
#define CTU_CTRL_INPUT_QP_Y									(AVS_REG_FETCH2_BASE + 0xf8)
#define CTU_CTRL_INPUT_QP_U									(AVS_REG_FETCH2_BASE + 0x100)
#define CTU_CTRL_INPUT_QP_V									(AVS_REG_FETCH2_BASE + 0x108)
#define CTU_CTRL_INPUT_LAMBDA_MV							(AVS_REG_FETCH2_BASE + 0x110)
#define CTU_CTRL_INPUT_PTR									(AVS_REG_FETCH2_BASE + 0x118)
#define CTU_CTRL_INPUT_GOP_SIZE								(AVS_REG_FETCH2_BASE + 0x120)
#define CTU_CTRL_INPUT_SLICE_TYPE							(AVS_REG_FETCH2_BASE + 0x128)
#define CTU_CTRL_INPUT_NUM_OF_HMVP_CAND						(AVS_REG_FETCH2_BASE + 0x130)
#define CTU_CTRL_INPUT_BIT_DEPTH_INTERNAL					(AVS_REG_FETCH2_BASE + 0x138)
#define CTU_CTRL_INPUT_IPF_ENABLE_FLAG						(AVS_REG_FETCH2_BASE + 0x140)
#define CTU_CTRL_INPUT_MAX_PART_RATIO						(AVS_REG_FETCH2_BASE + 0x148)
#define CTU_CTRL_INPUT_PIC_WIDTH_IN_SCU						(AVS_REG_FETCH2_BASE + 0x150)
#define CTU_CTRL_INPUT_PIC_HEIGHT_IN_SCU					(AVS_REG_FETCH2_BASE + 0x158)
#define CTU_CTRL_INPUT_F_SCU								(AVS_REG_FETCH2_BASE + 0x160)
#define CTU_CTRL_INPUT_LOG2_MAX_CUWH						(AVS_REG_FETCH2_BASE + 0x168)
#define CTU_CTRL_INPUT_MIN_CU_SIZE							(AVS_REG_FETCH2_BASE + 0x170)
#define CTU_CTRL_INPUT_MAX_SPLIT_TIMES						(AVS_REG_FETCH2_BASE + 0x178)
#define CTU_CTRL_INPUT_MIN_QT_SIZE							(AVS_REG_FETCH2_BASE + 0x180)
#define CTU_CTRL_INPUT_MAX_BT_SIZE							(AVS_REG_FETCH2_BASE + 0x188)
#define CTU_CTRL_INPUT_MAX_EQT_SIZE							(AVS_REG_FETCH2_BASE + 0x190)
#define CTU_CTRL_INPUT_AFFINE_SUBBLOCK_SIZE_IDX				(AVS_REG_FETCH2_BASE + 0x198)
#define CTU_CTRL_INPUT_IPCM_ENABLE_FLAG						(AVS_REG_FETCH2_BASE + 0x1a0)
#define CTU_CTRL_INPUT_TSCPM_ENABLE_FLAG					(AVS_REG_FETCH2_BASE + 0x1a8)
#define CTU_CTRL_INPUT_AMVR_ENABLE_FLAG						(AVS_REG_FETCH2_BASE + 0x1b0)
#define CTU_CTRL_INPUT_AMVR_ENABLE_FLAG_CTRL				(AVS_REG_FETCH2_BASE + 0x1b4)		//no valid
#define CTU_CTRL_INPUT_EMVR_ENABLE_FLAG						(AVS_REG_FETCH2_BASE + 0x1b8)
#define CTU_CTRL_INPUT_EMVR_ENABLE_FLAG_CTRL				(AVS_REG_FETCH2_BASE + 0x1bc)		//no valid
#define CTU_CTRL_INPUT_AFFINE_ENABLE_FLAG					(AVS_REG_FETCH2_BASE + 0x1c0)
#define CTU_CTRL_INPUT_AFFINE_ENABLE_FLAG_CTRL				(AVS_REG_FETCH2_BASE + 0x1c4)		//no valid

/*! @brief  [AIIT]:	avs sao register base space    */
#define CTX_MAP_SCU_OFFSET									(AVS_REG_SAO_BASE + 0x10)
#define CTX_MAP_MV_OFFSET									(AVS_REG_SAO_BASE + 0x18)
#define CTX_MAP_REFI_OFFSET									(AVS_REG_SAO_BASE + 0x20)
#define CTX_MAP_CU_MODE_OFFSET								(AVS_REG_SAO_BASE + 0x28)
#define Y_REC_OFFSET										(AVS_REG_SAO_BASE + 0x30)
#define U_REC_OFFSET										(AVS_REG_SAO_BASE + 0x38)
#define V_REC_OFFSET										(AVS_REG_SAO_BASE + 0x40)

/*! @brief  [AIIT]:	AVS yuv data base address    */
#define DRAM_BASE   (0x80000000)
#define DRAM_CAP    (100*SZ_1M)

#define SLICE_I                         	(1)
#define SLICE_P                         	(2)
#define SLICE_B								(3)

/* Max. and avs3_min. Quantization parameter */
#define MIN_QUANT                          0
#define MAX_QUANT_BASE                     63
#define FIXED_LAMBDA_NUM                   1000//ludd
#define FIXED_SQRT_LAMBDA_NUM              65535//ludd


#define COM_MAX(a,b)                   (((a) > (b)) ? (a) : (b))
#define COM_MIN(a,b)                   (((a) < (b)) ? (a) : (b))
#define COM_CLIP(n,avs3_min,avs3_max)            (((n)>(avs3_max))? (avs3_max) : (((n)<(avs3_min))? (avs3_min) : (n)))
#define COM_CLIP3(min_x, max_x, value)   COM_MAX((min_x), COM_MIN((max_x), (value)))

typedef struct _AVS_CFG_T_ {
	int fetch_ctx_io_ctx_pinter_refp_ptr_offset;
	int refp_map_mv_offset;
	int refp_map_refi_offset;
	int refp_list_ptr_offset;
	int map_scu_offset;
	int map_mv_offset;
	int map_refi_offset;
	int map_cu_mode_offset;
	int ctu_ctrl_input_lambda_offset;
	int ctu_ctrl_input_sqrt_lambda_offset;
	int ctu_ctrl_input_dist_chroma_weight_offset;
	int imgY_org_offset;
	int imgU_org_offset;
	int imgV_org_offset;
	int RefbufY_offset;
	int RefbufU_offset;
	int RefbufV_offset;

	int fetch_ctx_io_ctx_ptr;
	int fetch_ctx_io_ctx_rpm_num_refp;
	int fetch_ctx_io_ctx_pinter_refp_pic_height_chroma;
	int fetch_ctx_io_ctx_pinter_refp_pic_height_luma;
	int fetch_ctx_io_ctx_pinter_refp_pic_stride_chroma;
	int fetch_ctx_io_ctx_pinter_refp_pic_stride_luma;
	int fetch_ctx_io_ctx_pinter_refp_pic_width_chroma;
	int fetch_ctx_io_ctx_pinter_refp_pic_width_luma;
	int fetch_ctx_io_ctx_info_pic_width;
	int fetch_ctx_io_ctx_info_pic_height;
	int fetch_ctx_io_ctx_info_log2_max_cuwh;
	int fetch_ctx_io_ctx_info_pic_width_in_lcu;
	int fetch_ctx_io_ctx_info_pic_height_in_lcu;
	int fetch_ctx_io_ctx_info_pic_width_in_scu;
	int fetch_ctx_io_ctx_info_pic_height_in_scu;
	int fetch_ctx_io_ctx_info_pic_header_chroma_quant_param_delta_cb;
	int fetch_ctx_io_ctx_info_pic_header_chroma_quant_param_delta_cr;
	int fetch_ctx_io_ctx_info_pic_header_slice_type;
	int fetch_ctx_io_sample_adaptive_offset_enable_flag;
	int fetch_ctx_io_tree_status;
	int fetch_ctx_io_stride_luma;
	int fetch_ctx_io_stride_chroma;
	int fetch_ctx_io_lcu_cnt;
	int ctx_lcu_cnt;
	int ctu_ctrl_input_width;
	int ctu_ctrl_input_height;
	int ctu_ctrl_input_f_lcu;
	int ctu_ctrl_input_slice_enable_i;
	int ctu_ctrl_input_qp_y;
	int ctu_ctrl_input_qp_u;
	int ctu_ctrl_input_qp_v;
	int ctu_ctrl_input_lambda_mv;
	int ctu_ctrl_input_ptr;
	int ctu_ctrl_input_gop_size;
	int ctu_ctrl_input_slice_type;
	int ctu_ctrl_input_num_of_hmvp_cand;
	int ctu_ctrl_input_bit_depth_internal;
	int ctu_ctrl_input_ipf_enable_flag;
	int ctu_ctrl_input_max_part_ratio;
	int ctu_ctrl_input_pic_width_in_scu;
	int ctu_ctrl_input_pic_height_in_scu;
	int ctu_ctrl_input_f_scu;
	int ctu_ctrl_input_log2_max_cuwh;
	int ctu_ctrl_input_min_cu_size;
	int ctu_ctrl_input_max_split_times;
	int ctu_ctrl_input_min_qt_size;
	int ctu_ctrl_input_max_bt_size;
	int ctu_ctrl_input_max_eqt_size;
	int ctu_ctrl_input_affine_subblock_size_idx;
	int ctu_ctrl_input_ipcm_enable_flag;
	int ctu_ctrl_input_tscpm_enable_flag;
	int ctu_ctrl_input_amvr_enable_flag;
	int ctu_ctrl_input_emvr_enable_flag;
	int ctu_ctrl_input_affine_enable_flag;

	int ctx_map_scu_offset;
	int ctx_map_mv_offset;
	int ctx_map_refi_offset;
	int ctx_map_cu_mode_offset;
	int y_rec_offset;
	int u_rec_offset;
	int v_rec_offset;
	
	int bs_data_offset;
}AVS_CFG_T;

/* input YUV DRAM address */

inline int write_mem(void *addr1, void *addr2, size_t size)
{
	memcpy(addr1, addr2, size);
	return 0;
}

inline int wait_irq_pos(int val)
{
#ifdef RISCV_REGISTER_RW
	uint32_t reg = 0;
    uint32_t i = 0;

    while(1){
        reg = read_mreg32(sync_start);
        if(val == 0){
            if(((uint16_t)(reg >> 16)) == 0x0001)
                break;
        }
		else{
            if(i++ > 100000)
                break;
            else if (i % 20000 == 0)
            {
                //pr_log(DEBUG, "Delay time,wait process finish ......\n");
                printf("Delay time,wait process finish ......\n");
            }
        }
    }
#endif
	return 0;
}


#endif /* _AVS_REG__H */
