/**********************************************************
filename   : public_utils.h
function   : 
author     : 谢令
created    : 2022-12-06
comment    :
**********************************************************/
#ifndef _PUBLIC_UTILS_H_
#define _PUBLIC_UTILS_H_

#include "common.h"

/*
// FORWARD=1（顺指针），BACKWARD=0（逆时针）
//circle_poionts_num：总的孔位数
//out_dir:计算出要转动方向
//out_steps:计算出要转动的步数
*/
uint_t get_circle_move_dir_steps(uint_t src_pos, uint_t dest_pos, uint_t circle_poionts_num, uint_t* circle_points_steps, 
                                 uint_t* out_dir, uint_t* out_steps);

/*
//FORWARD=1（逆时针），BACKWARD=0（顺指针）
//poionts_num(等距孔位)
//circle_steps：整圈总步数
//out_dir:计算出要转动方向
//out_steps:计算出要转动的步数
//out_pos_num:计算出要转动的步数
*/
uint_t get_circle_isometry_pos_move_dir_steps(uint_t src_pos, uint_t dest_pos, uint_t poionts_num, uint_t circle_steps, 
                                 uint_t* out_dir, uint_t* out_steps, uint_t* out_pos_num);

/*
* 根据步数查速度: pArr = [number, [step, speed]], 按step从小到大排列配置的速度
*/
ushort_t get_speed_by_steps(ushort_t steps, uint_t* pArr);

/*
* 根据位置查步数和速度: pArr = [[step, speed]], 按位置编号(pos_no)从小到大排列配置
*/
ushort_t get_steps_speed_by_pos(ushort_t src_pos_no, ushort_t dst_pos_no, uint_t* pArr, 
                                ushort_t* out_dir, ushort_t* out_speed, ushort_t* out_steps);
#endif
