#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "avs_reg.h"
const int com_tbl_log2[257] = {
	/* 0, 1 */
	-1, -1,
	/* 2, 3 */
	1, -1,
	/* 4 ~ 7 */
	2, -1, -1, -1,
	/* 8 ~ 15 */
	3, -1, -1, -1, -1, -1, -1, -1,
	/* 16 ~ 31 */
	4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	/* 31 ~ 63 */
	5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	/* 64 ~ 127 */
	6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	/* 128 ~ 255 */
	7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	/* 256 */
	8
};

static const int com_tbl_qp_chroma_adjust_enc[64] = {
	0,  1,  2,  3,  4,  5,  6,  7,  8,  9,
	10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
	20, 21, 22, 23, 24, 25, 26, 27, 28, 29,
	30, 31, 32, 33, 33, 34, 35, 36, 37, 38,
	39, 40, 41, 42, 43, 44, 44, 44, 45, 46,
	47, 48, 48, 49, 50, 51, 52, 53, 54, 55,
	56, 57, 58, 59
};

static const int com_tbl_qp_chroma_adjust[64] = {
	//this table aligned with spec
	0,  1,  2,  3,  4,  5,  6,  7,  8,  9,
	10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
	20, 21, 22, 23, 24, 25, 26, 27, 28, 29,
	30, 31, 32, 33, 34, 35, 36, 37, 38, 39,
	40, 41, 42, 42, 43, 43, 44, 44, 45, 45,
	46, 46, 47, 47, 48, 48, 48, 49, 49, 49,
	50, 50, 50, 51
};

#define MAX_CU_SIZE                      	(1 << (7))
#define CONV_LOG2(v)                  		(com_tbl_log2[v])
#define MIN_CU_LOG2                 		2
#define PIC_PAD_SIZE_L                     	(MAX_CU_SIZE + 16)
#define PIC_PAD_SIZE_C                     	(PIC_PAD_SIZE_L >> 1)

void Cal_Chroma_Quant_Param(AVS_CFG_T *avs_cfg)
{
	int qp_offset_bit_depth = (8 * (op_internal_bit_depth - 8));
	int qp = avs_cfg->ctu_ctrl_input_qp_y;
	int picture_qp;
	if (op_pb_frame_qp_offset) {
		if ((op_qp_add_frm == 0) && (op_disable_hgop == 1)) {
			qp += avs_cfg->fetch_ctx_io_ctx_info_pic_header_slice_type != SLICE_I ? op_pb_frame_qp_offset : 0;
		}
	}

	/* frame_qp_add = 0*/
	//qp = COM_CLIP3(MIN_QUANT, (MAX_QUANT_BASE + ctx->info.qp_offset_bit_depth), (ctx->param.frame_qp_add != 0 && (int)(ctx->ptr) >= ctx->param.frame_qp_add) ? qp + 1.0 : qp);
	/* disable_hgop = 1
	if (!ctx->param.disable_hgop) {
			double dqp_offset;
			int qp_offset;
			qp += qp_adapt_param[ctx->temporal_id].qp_offset_layer;
			dqp_offset = COM_MAX(0, qp - ctx->info.qp_offset_bit_depth) * qp_adapt_param[ctx->temporal_id].qp_offset_model_scale +
				qp_adapt_param[ctx->temporal_id].qp_offset_model_offset + 0.5;
			qp_offset = (int)floor(COM_CLIP3(0.0, 4.0, dqp_offset));
			qp += qp_offset;
	}*/

	picture_qp = COM_CLIP3(MIN_QUANT, (MAX_QUANT_BASE + qp_offset_bit_depth), qp);
	if (op_qp_offset_adp) {
		int adaptive_qp_offset_encoder;
		int target_chroma_qp = com_tbl_qp_chroma_adjust_enc[COM_CLIP(picture_qp - qp_offset_bit_depth + 1, 0, 63)];

		for (adaptive_qp_offset_encoder = -5; adaptive_qp_offset_encoder < 10; adaptive_qp_offset_encoder++) {
			if (target_chroma_qp == com_tbl_qp_chroma_adjust[COM_CLIP(picture_qp - qp_offset_bit_depth + adaptive_qp_offset_encoder, 0, 63)]) {
				break;
			}
		}

		if (picture_qp - qp_offset_bit_depth + adaptive_qp_offset_encoder > 63) {
			adaptive_qp_offset_encoder = 63 - (picture_qp - qp_offset_bit_depth);
		}
		avs_cfg->fetch_ctx_io_ctx_info_pic_header_chroma_quant_param_delta_cb = op_qp_offset_cb + adaptive_qp_offset_encoder;
		avs_cfg->fetch_ctx_io_ctx_info_pic_header_chroma_quant_param_delta_cr = op_qp_offset_cr + adaptive_qp_offset_encoder;
	} else {
		avs_cfg->fetch_ctx_io_ctx_info_pic_header_chroma_quant_param_delta_cb = op_qp_offset_cb ;
		avs_cfg->fetch_ctx_io_ctx_info_pic_header_chroma_quant_param_delta_cr = op_qp_offset_cr ;
	}

}

void Cal_Ctu_Ctrl(AVS_CFG_T *avs_cfg, unsigned int dist_chroma_weight[2][2], unsigned int lambda[3][2], unsigned int sqrt_lambda[3][2])
{
	int adj_qp_cb, adj_qp_cr;
	int qp_offset_bit_depth = (8 * (op_internal_bit_depth - 8));
	avs_cfg->ctu_ctrl_input_pic_width_in_scu = (op_width + ((1 << MIN_CU_LOG2) - 1)) >> MIN_CU_LOG2;
	avs_cfg->ctu_ctrl_input_pic_height_in_scu = (op_height + ((1 << MIN_CU_LOG2) - 1)) >> MIN_CU_LOG2;
	avs_cfg->ctu_ctrl_input_f_scu = avs_cfg->ctu_ctrl_input_pic_width_in_scu * avs_cfg->ctu_ctrl_input_pic_height_in_scu;
	avs_cfg->ctu_ctrl_input_log2_max_cuwh = CONV_LOG2(op_ctu_size);

	adj_qp_cb = avs_cfg->ctu_ctrl_input_qp_y + avs_cfg->fetch_ctx_io_ctx_info_pic_header_chroma_quant_param_delta_cb - qp_offset_bit_depth;
	adj_qp_cr = avs_cfg->ctu_ctrl_input_qp_y + avs_cfg->fetch_ctx_io_ctx_info_pic_header_chroma_quant_param_delta_cr - qp_offset_bit_depth;
	adj_qp_cb = COM_CLIP(adj_qp_cb, MIN_QUANT - 16, MAX_QUANT_BASE);
	adj_qp_cr = COM_CLIP(adj_qp_cr, MIN_QUANT - 16, MAX_QUANT_BASE);
	if (adj_qp_cb >= 0) {
		adj_qp_cb = com_tbl_qp_chroma_adjust[COM_MIN(MAX_QUANT_BASE, adj_qp_cb)];
	}
	if (adj_qp_cr >= 0) {
		adj_qp_cr = com_tbl_qp_chroma_adjust[COM_MIN(MAX_QUANT_BASE, adj_qp_cr)];
	}
	avs_cfg->ctu_ctrl_input_qp_u = COM_CLIP(adj_qp_cb + qp_offset_bit_depth, MIN_QUANT, MAX_QUANT_BASE + qp_offset_bit_depth);
	avs_cfg->ctu_ctrl_input_qp_v = COM_CLIP(adj_qp_cr + qp_offset_bit_depth, MIN_QUANT, MAX_QUANT_BASE + qp_offset_bit_depth);

	double sub_lambda[3];
	double sub_dist_chroma_weight[2];
	
	sub_dist_chroma_weight[0] = pow(2.0, (avs_cfg->ctu_ctrl_input_qp_y - adj_qp_cb) / 4.0);
	sub_dist_chroma_weight[1] = pow(2.0, (avs_cfg->ctu_ctrl_input_qp_y - adj_qp_cr) / 4.0);

	sub_lambda[0] = 1.43631 * pow(2.0, (avs_cfg->ctu_ctrl_input_qp_y - 16.0) / 4.0);
	sub_lambda[1] = sub_lambda[0] / sub_dist_chroma_weight[0];
	sub_lambda[2] = sub_lambda[0] / sub_dist_chroma_weight[1];

	long ctx_sqrt_lambda[3];
	long ctx_lambda[3];
	long ctx_dist_chroma_weight[2];
	
	ctx_sqrt_lambda[0] = (long)(sqrt(sub_lambda[0]) * FIXED_SQRT_LAMBDA_NUM);
	ctx_sqrt_lambda[1] = (long)(sqrt(sub_lambda[1]) * FIXED_SQRT_LAMBDA_NUM);
	ctx_sqrt_lambda[2] = (long)(sqrt(sub_lambda[2]) * FIXED_SQRT_LAMBDA_NUM);

	ctx_lambda[0] = (long)(FIXED_LAMBDA_NUM * sub_lambda[0]);
	ctx_lambda[1] = (long)(FIXED_LAMBDA_NUM * sub_lambda[1]);
	ctx_lambda[2] = (long)(FIXED_LAMBDA_NUM * sub_lambda[2]);

	ctx_dist_chroma_weight[0] = (long)(sub_dist_chroma_weight[0] * FIXED_LAMBDA_NUM);
	ctx_dist_chroma_weight[1] = (long)(sub_dist_chroma_weight[1] * FIXED_LAMBDA_NUM);

	dist_chroma_weight[0][0] = ctx_dist_chroma_weight[0] & 0xffffffff;
	dist_chroma_weight[0][1] = (ctx_dist_chroma_weight[0] >> 32) & 0xffffffff;
	dist_chroma_weight[1][0] = ctx_dist_chroma_weight[1] & 0xffffffff;
	dist_chroma_weight[1][1] = (ctx_dist_chroma_weight[1] >> 32) & 0xffffffff;

	lambda[0][0] = ctx_lambda[0] & 0xffffffff;
	lambda[0][1] = (ctx_lambda[0] >> 32) & 0xffffffff;
	lambda[1][0] = ctx_lambda[1] & 0xffffffff;
	lambda[1][1] = (ctx_lambda[1] >> 32) & 0xffffffff;
	lambda[2][0] = ctx_lambda[2] & 0xffffffff;
	lambda[2][1] = (ctx_lambda[2] >> 32) & 0xffffffff;

	sqrt_lambda[0][0] = ctx_sqrt_lambda[0] & 0xffffffff;
	sqrt_lambda[0][1] = (ctx_sqrt_lambda[0] >> 32)& 0xffffffff;
	sqrt_lambda[1][0] = ctx_sqrt_lambda[1] & 0xffffffff;
	sqrt_lambda[1][1] = (ctx_sqrt_lambda[1] >> 32)& 0xffffffff;
	sqrt_lambda[2][0] = ctx_sqrt_lambda[2] & 0xffffffff;
	sqrt_lambda[2][1] = (ctx_sqrt_lambda[2] >> 32)& 0xffffffff;

	avs_cfg->ctu_ctrl_input_lambda_mv = (unsigned int)ctx_sqrt_lambda[0];
}

void DDR_pingpong1(AVS_CFG_T *avs_cfg)
{
	
	avs_cfg->RefbufU_offset = avs_cfg->u_rec_offset;
	avs_cfg->RefbufV_offset = avs_cfg->v_rec_offset;
	avs_cfg->RefbufY_offset = avs_cfg->y_rec_offset;
	
	avs_cfg->ctu_ctrl_input_dist_chroma_weight_offset = DDR_BASE1;
	avs_cfg->ctu_ctrl_input_lambda_offset = DDR_BASE1 + 16;
	avs_cfg->ctu_ctrl_input_sqrt_lambda_offset = avs_cfg->ctu_ctrl_input_lambda_offset + 24;
	avs_cfg->fetch_ctx_io_ctx_pinter_refp_ptr_offset = avs_cfg->ctu_ctrl_input_sqrt_lambda_offset + 24;

	avs_cfg->imgU_org_offset = avs_cfg->fetch_ctx_io_ctx_pinter_refp_ptr_offset + 136;
	avs_cfg->imgV_org_offset = avs_cfg->imgU_org_offset + op_width * op_height / 2;
	avs_cfg->imgY_org_offset = avs_cfg->imgV_org_offset + op_width * op_height / 2;

	avs_cfg->map_cu_mode_offset = avs_cfg->imgY_org_offset + op_width * op_height * 2;
	avs_cfg->map_mv_offset = avs_cfg->map_cu_mode_offset + avs_cfg->ctu_ctrl_input_f_scu * 4;
	avs_cfg->map_refi_offset = avs_cfg->map_mv_offset + avs_cfg->ctu_ctrl_input_f_scu * 8;
	avs_cfg->map_scu_offset = avs_cfg->map_refi_offset + avs_cfg->ctu_ctrl_input_f_scu * 2;

	avs_cfg->refp_list_ptr_offset = avs_cfg->map_scu_offset + avs_cfg->ctu_ctrl_input_f_scu * 4 + 2;
	avs_cfg->refp_map_mv_offset = avs_cfg->refp_list_ptr_offset + 17 * 4;
	avs_cfg->refp_map_refi_offset = avs_cfg->refp_map_mv_offset + avs_cfg->ctu_ctrl_input_f_scu * 8;

	avs_cfg->ctx_map_cu_mode_offset = avs_cfg->map_cu_mode_offset;
	avs_cfg->ctx_map_mv_offset = avs_cfg->map_mv_offset;
	avs_cfg->ctx_map_refi_offset = avs_cfg->map_refi_offset;
	avs_cfg->ctx_map_scu_offset = avs_cfg->map_scu_offset;

	avs_cfg->u_rec_offset = avs_cfg->refp_map_refi_offset + avs_cfg->ctu_ctrl_input_f_scu * 20;
	avs_cfg->v_rec_offset = avs_cfg->u_rec_offset + avs_cfg->fetch_ctx_io_stride_chroma * op_height;
	avs_cfg->y_rec_offset = avs_cfg->v_rec_offset + avs_cfg->fetch_ctx_io_stride_chroma * op_height;
	
	avs_cfg->bs_data_offset = avs_cfg->y_rec_offset + avs_cfg->fetch_ctx_io_stride_chroma * op_height * 4;
	
}

int main(int argc, char **argv)
{
	AVS_CFG_T cfg;
	unsigned int ctu_ctrl_dist_chroma_weight[2][2];
	unsigned int ctu_ctrl_lambda[3][2];
	unsigned int ctu_ctrl_sqrt_lambda[3][2];
	char output_file[50];
	FILE *fp;
	int i, count = 0, frame_count = 1, first_lcu_flag = 0, i_period_count, ret;
	
	cfg.fetch_ctx_io_ctx_info_pic_width = cfg.ctu_ctrl_input_width = op_width;
	cfg.fetch_ctx_io_ctx_info_pic_height = cfg.ctu_ctrl_input_height = op_height;
	cfg.fetch_ctx_io_ctx_info_pic_width_in_scu = (op_width + ((1 << MIN_CU_LOG2) - 1)) >> MIN_CU_LOG2;
	cfg.fetch_ctx_io_ctx_info_pic_height_in_scu = (op_height + ((1 << MIN_CU_LOG2) - 1)) >> MIN_CU_LOG2;
	cfg.fetch_ctx_io_ctx_info_log2_max_cuwh = CONV_LOG2(op_ctu_size);								// 6
	cfg.fetch_ctx_io_ctx_info_pic_width_in_lcu = (cfg.fetch_ctx_io_ctx_info_pic_width + op_ctu_size - 1) >> cfg.fetch_ctx_io_ctx_info_log2_max_cuwh;
	cfg.fetch_ctx_io_ctx_info_pic_height_in_lcu = (cfg.fetch_ctx_io_ctx_info_pic_height + op_ctu_size - 1) >> cfg.fetch_ctx_io_ctx_info_log2_max_cuwh;

	cfg.fetch_ctx_io_sample_adaptive_offset_enable_flag = op_sao;
	cfg.fetch_ctx_io_tree_status = 0x0;
	cfg.fetch_ctx_io_stride_luma = op_width + 144 * 2;
	cfg.fetch_ctx_io_stride_chroma = cfg.fetch_ctx_io_stride_luma / 2;
	cfg.ctu_ctrl_input_f_lcu = cfg.fetch_ctx_io_lcu_cnt = ((op_width + 63) / 64) * ((op_height + 63) / 64);
	cfg.ctu_ctrl_input_gop_size = op_disable_hgop;
	cfg.ctu_ctrl_input_num_of_hmvp_cand = op_hmvp;
	cfg.ctu_ctrl_input_bit_depth_internal = op_internal_bit_depth;
	cfg.ctu_ctrl_input_max_part_ratio = op_max_part_ratio;
	cfg.ctu_ctrl_input_min_cu_size = op_min_cu_size;
	cfg.ctu_ctrl_input_max_split_times = op_max_split_times;
	cfg.ctu_ctrl_input_min_qt_size = op_min_qt_size;
	cfg.ctu_ctrl_input_max_bt_size = op_max_bt_size;
	cfg.ctu_ctrl_input_max_eqt_size = op_max_eqt_size;
	cfg.ctu_ctrl_input_affine_subblock_size_idx = 0x0;
	cfg.ctu_ctrl_input_ipf_enable_flag = op_ipf;
	cfg.ctu_ctrl_input_tscpm_enable_flag = op_tspcm;
				
	if (op_libpic) {
		cfg.fetch_ctx_io_ctx_ptr = cfg.ctu_ctrl_input_ptr = 0x0;
		cfg.fetch_ctx_io_ctx_rpm_num_refp = 0x0;
		cfg.fetch_ctx_io_ctx_pinter_refp_pic_height_chroma = 0x0;
		cfg.fetch_ctx_io_ctx_pinter_refp_pic_height_luma = 0x0;
		cfg.fetch_ctx_io_ctx_pinter_refp_pic_stride_chroma = 0x0;
		cfg.fetch_ctx_io_ctx_pinter_refp_pic_stride_luma = 0x0;
		cfg.fetch_ctx_io_ctx_pinter_refp_pic_width_chroma = 0x0;
		cfg.fetch_ctx_io_ctx_pinter_refp_pic_width_luma = 0x0;

		cfg.ctu_ctrl_input_qp_y = op_qp + op_qp_offset_libpic;
		Cal_Chroma_Quant_Param(&cfg);
		cfg.fetch_ctx_io_sample_adaptive_offset_enable_flag = op_sao;
		cfg.fetch_ctx_io_tree_status = 0x0;
		cfg.fetch_ctx_io_stride_luma = op_width + 144 * 2;
		cfg.fetch_ctx_io_stride_chroma = cfg.fetch_ctx_io_stride_luma / 2;
		cfg.ctu_ctrl_input_f_lcu = cfg.fetch_ctx_io_lcu_cnt = ((op_width + 63) / 64) * ((op_height + 63) / 64);
		cfg.ctu_ctrl_input_gop_size = op_disable_hgop;
		cfg.ctu_ctrl_input_num_of_hmvp_cand = op_hmvp;
		cfg.ctu_ctrl_input_bit_depth_internal = op_internal_bit_depth;
		cfg.ctu_ctrl_input_max_part_ratio = op_max_part_ratio;
		cfg.ctu_ctrl_input_min_cu_size = op_min_cu_size;
		cfg.ctu_ctrl_input_max_split_times = op_max_split_times;
		cfg.ctu_ctrl_input_min_qt_size = op_min_qt_size;
		cfg.ctu_ctrl_input_max_bt_size = op_max_bt_size;
		cfg.ctu_ctrl_input_max_eqt_size = op_max_eqt_size;
		cfg.ctu_ctrl_input_affine_subblock_size_idx = 0x0;
		cfg.ctu_ctrl_input_ipf_enable_flag = op_ipf;
		cfg.ctu_ctrl_input_tscpm_enable_flag = op_tspcm;
		cfg.ctu_ctrl_input_amvr_enable_flag = 0x0;
		cfg.ctu_ctrl_input_emvr_enable_flag = 0x0;
		cfg.ctu_ctrl_input_affine_enable_flag = 0x0;
		
		Cal_Ctu_Ctrl(&cfg, ctu_ctrl_dist_chroma_weight, ctu_ctrl_lambda, ctu_ctrl_sqrt_lambda);
		cfg.ctx_lcu_cnt = cfg.ctu_ctrl_input_f_lcu;
		cfg.ctu_ctrl_input_slice_enable_i = 0x1;
		cfg.u_rec_offset = 0x800672c8;
		cfg.v_rec_offset = 0x800672c8;
		cfg.y_rec_offset = 0x800672c8;
		DDR_pingpong1(&cfg);
		fp = fopen("320x320_reg_0.txt", "w");
		fprintf(fp, "Write_reg(fetch_ctx_io_ctx_ptr, 0x%x);\n", cfg.fetch_ctx_io_ctx_ptr);
		fprintf(fp, "Write_reg(fetch_ctx_io_ctx_rpm_num_refp, 0x%x);\n", cfg.fetch_ctx_io_ctx_rpm_num_refp);
		fprintf(fp, "Write_reg(fetch_ctx_io_ctx_pinter_refp_pic_height_chroma, 0x%x);\n", cfg.fetch_ctx_io_ctx_pinter_refp_pic_height_chroma);
		fprintf(fp, "Write_reg(fetch_ctx_io_ctx_pinter_refp_pic_height_luma, 0x%x);\n", cfg.fetch_ctx_io_ctx_pinter_refp_pic_height_luma);
		fprintf(fp, "Write_reg(fetch_ctx_io_ctx_pinter_refp_pic_stride_chroma, 0x%x);\n", cfg.fetch_ctx_io_ctx_pinter_refp_pic_stride_chroma);
		fprintf(fp, "Write_reg(fetch_ctx_io_ctx_pinter_refp_pic_stride_luma, 0x%x);\n", cfg.fetch_ctx_io_ctx_pinter_refp_pic_stride_luma);
		fprintf(fp, "Write_reg(fetch_ctx_io_ctx_pinter_refp_pic_width_chroma, 0x%x);\n", cfg.fetch_ctx_io_ctx_pinter_refp_pic_width_chroma);
		fprintf(fp, "Write_reg(fetch_ctx_io_ctx_pinter_refp_pic_width_luma, 0x%x);\n", cfg.fetch_ctx_io_ctx_pinter_refp_pic_width_luma);
		fprintf(fp, "Write_reg(fetch_ctx_io_ctx_info_pic_width, 0x%x);\n", cfg.fetch_ctx_io_ctx_info_pic_width);
		fprintf(fp, "Write_reg(fetch_ctx_io_ctx_info_pic_height, 0x%x);\n", cfg.fetch_ctx_io_ctx_info_pic_height);	
		fprintf(fp, "Write_reg(fetch_ctx_io_ctx_info_pic_width_in_lcu, 0x%x);\n", cfg.fetch_ctx_io_ctx_info_pic_width_in_lcu);
		fprintf(fp, "Write_reg(fetch_ctx_io_ctx_info_pic_height_in_lcu, 0x%x);\n", cfg.fetch_ctx_io_ctx_info_pic_height_in_lcu);
		fprintf(fp, "Write_reg(fetch_ctx_io_ctx_info_pic_width_in_scu, 0x%x);\n", cfg.fetch_ctx_io_ctx_info_pic_width_in_scu);
		fprintf(fp, "Write_reg(fetch_ctx_io_ctx_info_pic_height_in_scu, 0x%x);\n", cfg.fetch_ctx_io_ctx_info_pic_height_in_scu);
		fprintf(fp, "Write_reg(fetch_ctx_io_ctx_info_log2_max_cuwh, 0x%x);\n", cfg.fetch_ctx_io_ctx_info_log2_max_cuwh);
		fprintf(fp, "Write_reg(fetch_ctx_io_ctx_info_pic_header_chroma_quant_param_delta_cb, 0x%x);\n", cfg.fetch_ctx_io_ctx_info_pic_header_chroma_quant_param_delta_cb);
		fprintf(fp, "Write_reg(fetch_ctx_io_ctx_info_pic_header_chroma_quant_param_delta_cr, 0x%x);\n", cfg.fetch_ctx_io_ctx_info_pic_header_chroma_quant_param_delta_cr);
		fprintf(fp, "Write_reg(fetch_ctx_io_ctx_info_pic_header_slice_type, 0x%x);\n", cfg.fetch_ctx_io_ctx_info_pic_header_slice_type);
		fprintf(fp, "Write_reg(fetch_ctx_io_sample_adaptive_offset_enable_flag, 0x%x);\n", cfg.fetch_ctx_io_sample_adaptive_offset_enable_flag);
		fprintf(fp, "Write_reg(fetch_ctx_io_tree_status, 0x%x);\n", cfg.fetch_ctx_io_tree_status);
		fprintf(fp, "Write_reg(fetch_ctx_io_stride_luma, 0x%x);\n", cfg.fetch_ctx_io_stride_luma);
		fprintf(fp, "Write_reg(fetch_ctx_io_stride_chroma, 0x%x);\n", cfg.fetch_ctx_io_stride_chroma);
		fprintf(fp, "Write_reg(fetch_ctx_io_lcu_cnt, 0x%x);\n", cfg.fetch_ctx_io_lcu_cnt);
		fprintf(fp, "Write_reg(ctx_lcu_cnt, 0x%x);\n", cfg.ctx_lcu_cnt);
		
		fprintf(fp, "Write_reg(ctu_ctrl_input_width, 0x%x);\n", cfg.ctu_ctrl_input_width);
		fprintf(fp, "Write_reg(ctu_ctrl_input_height, 0x%x);\n", cfg.ctu_ctrl_input_height);
		fprintf(fp, "Write_reg(ctu_ctrl_input_f_lcu, 0x%x);\n", cfg.ctu_ctrl_input_f_lcu);
		fprintf(fp, "Write_reg(ctu_ctrl_input_slice_enable_i, 0x%x);\n", cfg.ctu_ctrl_input_slice_enable_i);
		fprintf(fp, "Write_reg(ctu_ctrl_input_qp_y, 0x%x);\n", cfg.ctu_ctrl_input_qp_y);
		fprintf(fp, "Write_reg(ctu_ctrl_input_qp_u, 0x%x);\n", cfg.ctu_ctrl_input_qp_u);
		fprintf(fp, "Write_reg(ctu_ctrl_input_qp_v, 0x%x);\n", cfg.ctu_ctrl_input_qp_v);
		fprintf(fp, "Write_reg(ctu_ctrl_input_lambda_mv, 0x%x);\n", cfg.ctu_ctrl_input_lambda_mv);
		fprintf(fp, "Write_reg(ctu_ctrl_input_ptr, 0x%x);\n", cfg.ctu_ctrl_input_ptr);
		fprintf(fp, "Write_reg(ctu_ctrl_input_gop_size, 0x%x);\n", cfg.ctu_ctrl_input_gop_size);
		fprintf(fp, "Write_reg(ctu_ctrl_input_slice_type, 0x%x);\n", cfg.ctu_ctrl_input_slice_type);
		fprintf(fp, "Write_reg(ctu_ctrl_input_num_of_hmvp_cand, 0x%x);\n", cfg.ctu_ctrl_input_num_of_hmvp_cand);
		fprintf(fp, "Write_reg(ctu_ctrl_input_bit_depth_internal, 0x%x);\n", cfg.ctu_ctrl_input_bit_depth_internal);
		fprintf(fp, "Write_reg(ctu_ctrl_input_ipf_enable_flag, 0x%x);\n", cfg.ctu_ctrl_input_ipf_enable_flag);
		fprintf(fp, "Write_reg(ctu_ctrl_input_max_part_ratio, 0x%x);\n", cfg.ctu_ctrl_input_max_part_ratio);
		fprintf(fp, "Write_reg(ctu_ctrl_input_pic_width_in_scu, 0x%x);\n", cfg.ctu_ctrl_input_pic_width_in_scu);
		fprintf(fp, "Write_reg(ctu_ctrl_input_pic_height_in_scu, 0x%x);\n", cfg.ctu_ctrl_input_pic_height_in_scu);
		fprintf(fp, "Write_reg(ctu_ctrl_input_f_scu, 0x%x);\n", cfg.ctu_ctrl_input_f_scu);
		fprintf(fp, "Write_reg(ctu_ctrl_input_log2_max_cuwh, 0x%x);\n", cfg.ctu_ctrl_input_log2_max_cuwh);
		fprintf(fp, "Write_reg(ctu_ctrl_input_min_cu_size, 0x%x);\n", cfg.ctu_ctrl_input_min_cu_size);
		fprintf(fp, "Write_reg(ctu_ctrl_input_max_split_times, 0x%x);\n", cfg.ctu_ctrl_input_max_split_times);
		fprintf(fp, "Write_reg(ctu_ctrl_input_min_qt_size, 0x%x);\n", cfg.ctu_ctrl_input_min_qt_size);
		fprintf(fp, "Write_reg(ctu_ctrl_input_max_bt_size, 0x%x);\n", cfg.ctu_ctrl_input_max_bt_size);
		fprintf(fp, "Write_reg(ctu_ctrl_input_max_eqt_size, 0x%x);\n", cfg.ctu_ctrl_input_max_eqt_size);
		fprintf(fp, "Write_reg(ctu_ctrl_input_affine_subblock_size_idx, 0x%x);\n", cfg.ctu_ctrl_input_affine_subblock_size_idx);
		fprintf(fp, "Write_reg(ctu_ctrl_input_ipf_enable_flag, 0x%x);\n", cfg.ctu_ctrl_input_ipf_enable_flag);
		fprintf(fp, "Write_reg(ctu_ctrl_input_tscpm_enable_flag, 0x%x);\n", cfg.ctu_ctrl_input_tscpm_enable_flag);
		fprintf(fp, "Write_reg(ctu_ctrl_input_amvr_enable_flag, 0x%x);\n", cfg.ctu_ctrl_input_amvr_enable_flag);
		fprintf(fp, "Write_reg(ctu_ctrl_input_emvr_enable_flag, 0x%x);\n", cfg.ctu_ctrl_input_emvr_enable_flag);
		fprintf(fp, "Write_reg(ctu_ctrl_input_affine_enable_flag, 0x%x);\n", cfg.ctu_ctrl_input_affine_enable_flag);

		fprintf(fp, "Write_reg(ctu_ctrl_input_dist_chroma_weight_offset, 0x%x);\n", cfg.ctu_ctrl_input_dist_chroma_weight_offset);
		fprintf(fp, "Write_reg(ctu_ctrl_input_lambda_offset, 0x%x);\n", cfg.ctu_ctrl_input_lambda_offset);
		fprintf(fp, "Write_reg(ctu_ctrl_input_sqrt_lambda_offset, 0x%x);\n", cfg.ctu_ctrl_input_sqrt_lambda_offset);
		fprintf(fp, "Write_reg(fetch_ctx_io_ctx_pinter_refp_ptr_offset, 0x%x);\n", cfg.fetch_ctx_io_ctx_pinter_refp_ptr_offset);

		fprintf(fp, "Write_reg(imgU_org_offset, 0x%x);\n", cfg.imgU_org_offset);
		fprintf(fp, "Write_reg(imgV_org_offset, 0x%x);\n", cfg.imgV_org_offset);
		fprintf(fp, "Write_reg(imgY_org_offset, 0x%x);\n", cfg.imgY_org_offset);
		
		fprintf(fp, "Write_reg(map_cu_mode_offset, 0x%x);\n", cfg.map_cu_mode_offset);
		fprintf(fp, "Write_reg(map_mv_offset, 0x%x);\n", cfg.map_mv_offset);
		fprintf(fp, "Write_reg(map_refi_offset, 0x%x);\n", cfg.map_refi_offset);
		fprintf(fp, "Write_reg(map_scu_offset, 0x%x);\n", cfg.map_scu_offset);
		
		fprintf(fp, "Write_reg(RefbufU_offset, 0x%x);\n", cfg.RefbufU_offset);
		fprintf(fp, "Write_reg(RefbufV_offset, 0x%x);\n", cfg.RefbufV_offset);
		fprintf(fp, "Write_reg(RefbufY_offset, 0x%x);\n", cfg.RefbufY_offset);
		
		fprintf(fp, "Write_reg(refp_list_ptr_offset, 0x%x);\n", cfg.refp_list_ptr_offset);
		fprintf(fp, "Write_reg(refp_map_mv_offset, 0x%x);\n", cfg.refp_map_mv_offset);
		fprintf(fp, "Write_reg(refp_map_refi_offset, 0x%x);\n", cfg.refp_map_refi_offset);

		fprintf(fp, "Write_reg(ctx_map_cu_mode_offset, 0x%x);\n", cfg.ctx_map_cu_mode_offset);
		fprintf(fp, "Write_reg(ctx_map_mv_offset, 0x%x);\n", cfg.ctx_map_mv_offset);
		fprintf(fp, "Write_reg(ctx_map_refi_offset, 0x%x);\n", cfg.ctx_map_refi_offset);
		fprintf(fp, "Write_reg(ctx_map_scu_offset, 0x%x);\n", cfg.ctx_map_scu_offset);
		
		fprintf(fp, "Write_reg(u_rec_offset, 0x%x);\n", cfg.u_rec_offset);
		fprintf(fp, "Write_reg(v_rec_offset, 0x%x);\n", cfg.v_rec_offset);
		fprintf(fp, "Write_reg(y_rec_offset, 0x%x);\n", cfg.y_rec_offset);

		fprintf(fp, "Write_reg(bs_data_offset, 0x%x);\n", cfg.bs_data_offset);
		for ( ; cfg.ctx_lcu_cnt > 1; ) {
			cfg.ctx_lcu_cnt--;
			fprintf(fp, "\n");
			fprintf(fp, "Write_reg(ctx_lcu_cnt, 0x%x);\n", cfg.ctx_lcu_cnt);
		
			if (cfg.ctu_ctrl_input_slice_enable_i == 1) {
				cfg.ctu_ctrl_input_slice_enable_i = 0x0;
				cfg.ctu_ctrl_input_amvr_enable_flag = op_amvr;
				cfg.ctu_ctrl_input_emvr_enable_flag = op_emvr;
				cfg.ctu_ctrl_input_affine_enable_flag = op_affine;
				fprintf(fp, "Write_reg(ctu_ctrl_input_slice_enable_i, 0x%x);\n", cfg.ctu_ctrl_input_slice_enable_i);
				fprintf(fp, "Write_reg(ctu_ctrl_input_amvr_enable_flag, 0x%x);\n", cfg.ctu_ctrl_input_amvr_enable_flag);
				fprintf(fp, "Write_reg(ctu_ctrl_input_emvr_enable_flag, 0x%x);\n", cfg.ctu_ctrl_input_emvr_enable_flag);
				fprintf(fp, "Write_reg(ctu_ctrl_input_affine_enable_flag, 0x%x);\n", cfg.ctu_ctrl_input_affine_enable_flag);
			}
		}
	} else {
		if (op_frames % op_i_period)
			count = op_frames / op_i_period + 1;
		else
			count = op_frames / op_i_period;

		cfg.ctu_ctrl_input_qp_y = op_qp;
		Cal_Chroma_Quant_Param(&cfg);
		Cal_Ctu_Ctrl(&cfg, ctu_ctrl_dist_chroma_weight, ctu_ctrl_lambda, ctu_ctrl_sqrt_lambda);

		for (i = 0; i < count; i++) {
			for (i_period_count = 0; i_period_count < op_i_period; i_period_count++) {
				cfg.fetch_ctx_io_ctx_ptr = cfg.ctu_ctrl_input_ptr = i_period_count;
				cfg.ctu_ctrl_input_slice_enable_i = 0x1;
				cfg.ctx_lcu_cnt = cfg.ctu_ctrl_input_f_lcu;
				if (!i_period_count) {
					
					cfg.fetch_ctx_io_ctx_rpm_num_refp = 0x0;
					cfg.fetch_ctx_io_ctx_pinter_refp_pic_height_chroma = 0x0;
					cfg.fetch_ctx_io_ctx_pinter_refp_pic_height_luma = 0x0;
					cfg.fetch_ctx_io_ctx_pinter_refp_pic_stride_chroma = 0x0;
					cfg.fetch_ctx_io_ctx_pinter_refp_pic_stride_luma = 0x0;
					cfg.fetch_ctx_io_ctx_pinter_refp_pic_width_chroma = 0x0;
					cfg.fetch_ctx_io_ctx_pinter_refp_pic_width_luma = 0x0;

					cfg.fetch_ctx_io_ctx_info_pic_header_slice_type = cfg.ctu_ctrl_input_slice_type = SLICE_I;

					cfg.u_rec_offset = 0x800672c8;
					cfg.v_rec_offset = 0x800672c8;
					cfg.y_rec_offset = 0x800672c8;
					DDR_pingpong1(&cfg);

					if (!first_lcu_flag) {
						cfg.ctu_ctrl_input_amvr_enable_flag = 0x0;
						cfg.ctu_ctrl_input_emvr_enable_flag = 0x0;
						cfg.ctu_ctrl_input_affine_enable_flag = 0x0;
						first_lcu_flag = 1;
					}
				}else if (i_period_count == 1) {
					cfg.fetch_ctx_io_ctx_rpm_num_refp = 0x1;
					cfg.fetch_ctx_io_ctx_pinter_refp_pic_height_chroma = op_height / 2;
					cfg.fetch_ctx_io_ctx_pinter_refp_pic_height_luma = op_height;
					cfg.fetch_ctx_io_ctx_pinter_refp_pic_stride_chroma = (op_width + 288) / 2;
					cfg.fetch_ctx_io_ctx_pinter_refp_pic_stride_luma = (op_width + 288);
					cfg.fetch_ctx_io_ctx_pinter_refp_pic_width_chroma = op_width / 2;
					cfg.fetch_ctx_io_ctx_pinter_refp_pic_width_luma = op_width;

					cfg.fetch_ctx_io_ctx_info_pic_header_slice_type = cfg.ctu_ctrl_input_slice_type = SLICE_P;
				}
				
				printf("frame count = %d  i_period = %d  count = %d \n", frame_count, i_period_count, i);
				
				ret = sprintf(output_file, "320x320_reg_%d.txt", frame_count);
				fp = fopen(output_file, "w");
				fprintf(fp, "Write_reg(fetch_ctx_io_ctx_ptr, 0x%x);\n", cfg.fetch_ctx_io_ctx_ptr);
				fprintf(fp, "Write_reg(fetch_ctx_io_ctx_rpm_num_refp, 0x%x);\n", cfg.fetch_ctx_io_ctx_rpm_num_refp);
				fprintf(fp, "Write_reg(fetch_ctx_io_ctx_pinter_refp_pic_height_chroma, 0x%x);\n", cfg.fetch_ctx_io_ctx_pinter_refp_pic_height_chroma);
				fprintf(fp, "Write_reg(fetch_ctx_io_ctx_pinter_refp_pic_height_luma, 0x%x);\n", cfg.fetch_ctx_io_ctx_pinter_refp_pic_height_luma);
				fprintf(fp, "Write_reg(fetch_ctx_io_ctx_pinter_refp_pic_stride_chroma, 0x%x);\n", cfg.fetch_ctx_io_ctx_pinter_refp_pic_stride_chroma);
				fprintf(fp, "Write_reg(fetch_ctx_io_ctx_pinter_refp_pic_stride_luma, 0x%x);\n", cfg.fetch_ctx_io_ctx_pinter_refp_pic_stride_luma);
				fprintf(fp, "Write_reg(fetch_ctx_io_ctx_pinter_refp_pic_width_chroma, 0x%x);\n", cfg.fetch_ctx_io_ctx_pinter_refp_pic_width_chroma);
				fprintf(fp, "Write_reg(fetch_ctx_io_ctx_pinter_refp_pic_width_luma, 0x%x);\n", cfg.fetch_ctx_io_ctx_pinter_refp_pic_width_luma);
				fprintf(fp, "Write_reg(fetch_ctx_io_ctx_info_pic_width, 0x%x);\n", cfg.fetch_ctx_io_ctx_info_pic_width);
				fprintf(fp, "Write_reg(fetch_ctx_io_ctx_info_pic_height, 0x%x);\n", cfg.fetch_ctx_io_ctx_info_pic_height);	
				fprintf(fp, "Write_reg(fetch_ctx_io_ctx_info_pic_width_in_lcu, 0x%x);\n", cfg.fetch_ctx_io_ctx_info_pic_width_in_lcu);
				fprintf(fp, "Write_reg(fetch_ctx_io_ctx_info_pic_height_in_lcu, 0x%x);\n", cfg.fetch_ctx_io_ctx_info_pic_height_in_lcu);
				fprintf(fp, "Write_reg(fetch_ctx_io_ctx_info_pic_width_in_scu, 0x%x);\n", cfg.fetch_ctx_io_ctx_info_pic_width_in_scu);
				fprintf(fp, "Write_reg(fetch_ctx_io_ctx_info_pic_height_in_scu, 0x%x);\n", cfg.fetch_ctx_io_ctx_info_pic_height_in_scu);
				fprintf(fp, "Write_reg(fetch_ctx_io_ctx_info_log2_max_cuwh, 0x%x);\n", cfg.fetch_ctx_io_ctx_info_log2_max_cuwh);
				fprintf(fp, "Write_reg(fetch_ctx_io_ctx_info_pic_header_chroma_quant_param_delta_cb, 0x%x);\n", cfg.fetch_ctx_io_ctx_info_pic_header_chroma_quant_param_delta_cb);
				fprintf(fp, "Write_reg(fetch_ctx_io_ctx_info_pic_header_chroma_quant_param_delta_cr, 0x%x);\n", cfg.fetch_ctx_io_ctx_info_pic_header_chroma_quant_param_delta_cr);
				fprintf(fp, "Write_reg(fetch_ctx_io_ctx_info_pic_header_slice_type, 0x%x);\n", cfg.fetch_ctx_io_ctx_info_pic_header_slice_type);
				fprintf(fp, "Write_reg(fetch_ctx_io_sample_adaptive_offset_enable_flag, 0x%x);\n", cfg.fetch_ctx_io_sample_adaptive_offset_enable_flag);
				fprintf(fp, "Write_reg(fetch_ctx_io_tree_status, 0x%x);\n", cfg.fetch_ctx_io_tree_status);
				fprintf(fp, "Write_reg(fetch_ctx_io_stride_luma, 0x%x);\n", cfg.fetch_ctx_io_stride_luma);
				fprintf(fp, "Write_reg(fetch_ctx_io_stride_chroma, 0x%x);\n", cfg.fetch_ctx_io_stride_chroma);
				fprintf(fp, "Write_reg(fetch_ctx_io_lcu_cnt, 0x%x);\n", cfg.fetch_ctx_io_lcu_cnt);
				fprintf(fp, "Write_reg(ctx_lcu_cnt, 0x%x);\n", cfg.ctx_lcu_cnt);
				
				fprintf(fp, "Write_reg(ctu_ctrl_input_width, 0x%x);\n", cfg.ctu_ctrl_input_width);
				fprintf(fp, "Write_reg(ctu_ctrl_input_height, 0x%x);\n", cfg.ctu_ctrl_input_height);
				fprintf(fp, "Write_reg(ctu_ctrl_input_f_lcu, 0x%x);\n", cfg.ctu_ctrl_input_f_lcu);
				fprintf(fp, "Write_reg(ctu_ctrl_input_slice_enable_i, 0x%x);\n", cfg.ctu_ctrl_input_slice_enable_i);
				fprintf(fp, "Write_reg(ctu_ctrl_input_qp_y, 0x%x);\n", cfg.ctu_ctrl_input_qp_y);
				fprintf(fp, "Write_reg(ctu_ctrl_input_qp_u, 0x%x);\n", cfg.ctu_ctrl_input_qp_u);
				fprintf(fp, "Write_reg(ctu_ctrl_input_qp_v, 0x%x);\n", cfg.ctu_ctrl_input_qp_v);
				fprintf(fp, "Write_reg(ctu_ctrl_input_lambda_mv, 0x%x);\n", cfg.ctu_ctrl_input_lambda_mv);
				fprintf(fp, "Write_reg(ctu_ctrl_input_ptr, 0x%x);\n", cfg.ctu_ctrl_input_ptr);
				fprintf(fp, "Write_reg(ctu_ctrl_input_gop_size, 0x%x);\n", cfg.ctu_ctrl_input_gop_size);
				fprintf(fp, "Write_reg(ctu_ctrl_input_slice_type, 0x%x);\n", cfg.ctu_ctrl_input_slice_type);
				fprintf(fp, "Write_reg(ctu_ctrl_input_num_of_hmvp_cand, 0x%x);\n", cfg.ctu_ctrl_input_num_of_hmvp_cand);
				fprintf(fp, "Write_reg(ctu_ctrl_input_bit_depth_internal, 0x%x);\n", cfg.ctu_ctrl_input_bit_depth_internal);
				fprintf(fp, "Write_reg(ctu_ctrl_input_ipf_enable_flag, 0x%x);\n", cfg.ctu_ctrl_input_ipf_enable_flag);
				fprintf(fp, "Write_reg(ctu_ctrl_input_max_part_ratio, 0x%x);\n", cfg.ctu_ctrl_input_max_part_ratio);
				fprintf(fp, "Write_reg(ctu_ctrl_input_pic_width_in_scu, 0x%x);\n", cfg.ctu_ctrl_input_pic_width_in_scu);
				fprintf(fp, "Write_reg(ctu_ctrl_input_pic_height_in_scu, 0x%x);\n", cfg.ctu_ctrl_input_pic_height_in_scu);
				fprintf(fp, "Write_reg(ctu_ctrl_input_f_scu, 0x%x);\n", cfg.ctu_ctrl_input_f_scu);
				fprintf(fp, "Write_reg(ctu_ctrl_input_log2_max_cuwh, 0x%x);\n", cfg.ctu_ctrl_input_log2_max_cuwh);
				fprintf(fp, "Write_reg(ctu_ctrl_input_min_cu_size, 0x%x);\n", cfg.ctu_ctrl_input_min_cu_size);
				fprintf(fp, "Write_reg(ctu_ctrl_input_max_split_times, 0x%x);\n", cfg.ctu_ctrl_input_max_split_times);
				fprintf(fp, "Write_reg(ctu_ctrl_input_min_qt_size, 0x%x);\n", cfg.ctu_ctrl_input_min_qt_size);
				fprintf(fp, "Write_reg(ctu_ctrl_input_max_bt_size, 0x%x);\n", cfg.ctu_ctrl_input_max_bt_size);
				fprintf(fp, "Write_reg(ctu_ctrl_input_max_eqt_size, 0x%x);\n", cfg.ctu_ctrl_input_max_eqt_size);
				fprintf(fp, "Write_reg(ctu_ctrl_input_affine_subblock_size_idx, 0x%x);\n", cfg.ctu_ctrl_input_affine_subblock_size_idx);
				fprintf(fp, "Write_reg(ctu_ctrl_input_ipf_enable_flag, 0x%x);\n", cfg.ctu_ctrl_input_ipf_enable_flag);
				fprintf(fp, "Write_reg(ctu_ctrl_input_tscpm_enable_flag, 0x%x);\n", cfg.ctu_ctrl_input_tscpm_enable_flag);
				fprintf(fp, "Write_reg(ctu_ctrl_input_amvr_enable_flag, 0x%x);\n", cfg.ctu_ctrl_input_amvr_enable_flag);
				fprintf(fp, "Write_reg(ctu_ctrl_input_emvr_enable_flag, 0x%x);\n", cfg.ctu_ctrl_input_emvr_enable_flag);
				fprintf(fp, "Write_reg(ctu_ctrl_input_affine_enable_flag, 0x%x);\n", cfg.ctu_ctrl_input_affine_enable_flag);

				fprintf(fp, "Write_reg(ctu_ctrl_input_dist_chroma_weight_offset, 0x%x);\n", cfg.ctu_ctrl_input_dist_chroma_weight_offset);
				fprintf(fp, "Write_reg(ctu_ctrl_input_lambda_offset, 0x%x);\n", cfg.ctu_ctrl_input_lambda_offset);
				fprintf(fp, "Write_reg(ctu_ctrl_input_sqrt_lambda_offset, 0x%x);\n", cfg.ctu_ctrl_input_sqrt_lambda_offset);
				fprintf(fp, "Write_reg(fetch_ctx_io_ctx_pinter_refp_ptr_offset, 0x%x);\n", cfg.fetch_ctx_io_ctx_pinter_refp_ptr_offset);

				fprintf(fp, "Write_reg(imgU_org_offset, 0x%x);\n", cfg.imgU_org_offset);
				fprintf(fp, "Write_reg(imgV_org_offset, 0x%x);\n", cfg.imgV_org_offset);
				fprintf(fp, "Write_reg(imgY_org_offset, 0x%x);\n", cfg.imgY_org_offset);
				
				fprintf(fp, "Write_reg(map_cu_mode_offset, 0x%x);\n", cfg.map_cu_mode_offset);
				fprintf(fp, "Write_reg(map_mv_offset, 0x%x);\n", cfg.map_mv_offset);
				fprintf(fp, "Write_reg(map_refi_offset, 0x%x);\n", cfg.map_refi_offset);
				fprintf(fp, "Write_reg(map_scu_offset, 0x%x);\n", cfg.map_scu_offset);
				
				fprintf(fp, "Write_reg(RefbufU_offset, 0x%x);\n", cfg.RefbufU_offset);
				fprintf(fp, "Write_reg(RefbufV_offset, 0x%x);\n", cfg.RefbufV_offset);
				fprintf(fp, "Write_reg(RefbufY_offset, 0x%x);\n", cfg.RefbufY_offset);
				
				fprintf(fp, "Write_reg(refp_list_ptr_offset, 0x%x);\n", cfg.refp_list_ptr_offset);
				fprintf(fp, "Write_reg(refp_map_mv_offset, 0x%x);\n", cfg.refp_map_mv_offset);
				fprintf(fp, "Write_reg(refp_map_refi_offset, 0x%x);\n", cfg.refp_map_refi_offset);

				fprintf(fp, "Write_reg(ctx_map_cu_mode_offset, 0x%x);\n", cfg.ctx_map_cu_mode_offset);
				fprintf(fp, "Write_reg(ctx_map_mv_offset, 0x%x);\n", cfg.ctx_map_mv_offset);
				fprintf(fp, "Write_reg(ctx_map_refi_offset, 0x%x);\n", cfg.ctx_map_refi_offset);
				fprintf(fp, "Write_reg(ctx_map_scu_offset, 0x%x);\n", cfg.ctx_map_scu_offset);
				
				fprintf(fp, "Write_reg(u_rec_offset, 0x%x);\n", cfg.u_rec_offset);
				fprintf(fp, "Write_reg(v_rec_offset, 0x%x);\n", cfg.v_rec_offset);
				fprintf(fp, "Write_reg(y_rec_offset, 0x%x);\n", cfg.y_rec_offset);

				fprintf(fp, "Write_reg(bs_data_offset, 0x%x);\n", cfg.bs_data_offset);
				for ( ; cfg.ctx_lcu_cnt > 1; ) {
					cfg.ctx_lcu_cnt--;
					fprintf(fp, "\n");
					fprintf(fp, "Write_reg(ctx_lcu_cnt, 0x%x);\n", cfg.ctx_lcu_cnt);
				
					if (cfg.ctu_ctrl_input_slice_enable_i == 1) {
						cfg.ctu_ctrl_input_slice_enable_i = 0x0;
						cfg.ctu_ctrl_input_amvr_enable_flag = op_amvr;
						cfg.ctu_ctrl_input_emvr_enable_flag = op_emvr;
						cfg.ctu_ctrl_input_affine_enable_flag = op_affine;
						fprintf(fp, "Write_reg(ctu_ctrl_input_slice_enable_i, 0x%x);\n", cfg.ctu_ctrl_input_slice_enable_i);
						fprintf(fp, "Write_reg(ctu_ctrl_input_amvr_enable_flag, 0x%x);\n", cfg.ctu_ctrl_input_amvr_enable_flag);
						fprintf(fp, "Write_reg(ctu_ctrl_input_emvr_enable_flag, 0x%x);\n", cfg.ctu_ctrl_input_emvr_enable_flag);
						fprintf(fp, "Write_reg(ctu_ctrl_input_affine_enable_flag, 0x%x);\n", cfg.ctu_ctrl_input_affine_enable_flag);
					}
				}
				fclose(fp);
				
				frame_count++;
				
			}
		}
	}
	return 0;
}



