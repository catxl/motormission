/**********************************************************
filename   : public_utils.c
function   : 
author     : 谢令
created    : 2022-12-06
comment    :
**********************************************************/
#include "public_utils.h"
#include "public_step_motors.h"


// FORWARD=1（顺指针），BACKWARD=0（逆时针）
//circle_poionts_num：总的孔位数
//out_dir:计算出要转动方向
//out_steps:计算出要转动的步数
uint_t get_circle_move_dir_steps(uint_t src_pos, uint_t dest_pos, uint_t circle_poionts_num, uint_t* circle_points_steps, 
                                 uint_t* out_dir, uint_t* out_steps){
    uint_t src_step = circle_points_steps[src_pos];//当前位置到0号孔的脉冲数
    uint_t dst_step = circle_points_steps[dest_pos];//目标位置到0号孔的脉冲数
    *out_dir = FORWARD;
    if (dest_pos > src_pos) {                                            // 时钟: 1点 -> N点
        if ((dest_pos - src_pos) * 2 > circle_poionts_num) {             //       1->9, 反向
            *out_dir = BACKWARD;
            *out_steps = circle_points_steps[circle_poionts_num] + src_step - dst_step;//转一圈的步数-目标位到0的步数+要转位置到0的步数
        } else {                                                         //       1->6, 正向
            *out_steps = dst_step - src_step;
        }
    } else {                                                             // 时钟: 9点 -> N点
        if ((src_pos - dest_pos) * 2 > circle_poionts_num) {             //       9->1, 正向
            *out_steps = circle_points_steps[circle_poionts_num] + dst_step - src_step;//转一圈的步数-要转位置到0的步数+目标位到0的步数
        } else {                                                         //       9->6, 反向
            *out_dir = BACKWARD;
            *out_steps = src_step - dst_step;
        }
    }
    return 0;
}

//FORWARD=1（逆时针），BACKWARD=0（顺指针）
//poionts_num(等距孔位)
//circle_steps：整圈总步数
//out_dir:计算出要转动方向
//out_steps:计算出要转动的步数
//out_pos_num:计算出要转动的步数
uint_t get_circle_isometry_pos_move_dir_steps(uint_t src_pos, uint_t dest_pos, uint_t poionts_num, uint_t circle_steps, 
                                 uint_t* out_dir, uint_t* out_steps, uint_t* out_pos_num){
    uint_t src_step = circle_steps*src_pos/poionts_num;//当前位置到0号孔的脉冲数
    uint_t dst_step = circle_steps*dest_pos/poionts_num;//目标位置到0号孔的脉冲数
    *out_dir = FORWARD;
    if (dst_step > src_step) {                                            // 时钟: 1点 -> N点
        if ((dst_step - src_step) * 2 > circle_steps) {                   //       1->9, 逆时针
            *out_dir = BACKWARD;
            *out_steps = circle_steps + src_step - dst_step;              //转一圈的步数-目标位到0的步数+要转位置到0的步数
            *out_pos_num = poionts_num + src_pos - dest_pos;
        } else {                                                          //       1->6, 正向
            *out_steps = dst_step - src_step;
            *out_pos_num = dest_pos - src_pos;
        }
    } else {                                                              // 时钟: 9点 -> N点
        if ((src_step - dst_step) * 2 > circle_steps) {                   //       9->1, 正向
            *out_steps = circle_steps + dst_step - src_step;              //转一圈的步数-要转位置到0的步数+目标位到0的步数
            *out_pos_num = poionts_num + dest_pos - src_pos;
        } else {                                                          //       9->6, 反向
            *out_dir = BACKWARD;
            *out_steps = src_step - dst_step;
            *out_pos_num = src_pos - dest_pos;
        }
    }
    return 0;
}

/*
* 根据步数查速度: pArr = [number, [step, speed]], 按step从小到大排列配置的速度
*/
ushort_t get_speed_by_steps(ushort_t steps, uint_t* pArr){
    ushort_t i = 0;
    ushort_t n = 0;
    ushort_t s = 0;
    if(0 == pArr){
        return 0;
    }
    n = pArr[0] & 0xF;      // 最多16速
    for(i=0; i<n; i++){
        if(steps<=pArr[2*i+1]){
            s = (ushort_t)pArr[2*i+2];
        }
        else{
            break;
        }
    }
    return s;
}

/*
* 根据位置查步数和速度: pArr = [[step, speed]], 按位置编号(pos_no)从小到大排列配置
*/
ushort_t get_steps_speed_by_pos(ushort_t src_pos_no, ushort_t dst_pos_no, uint_t* pArr, 
                                ushort_t* out_dir, ushort_t* out_speed, ushort_t* out_steps){
    ushort_t src_step = (ushort_t) pArr[src_pos_no<<1];
    ushort_t dst_step = (ushort_t) pArr[dst_pos_no<<1];
    *out_speed = (ushort_t) pArr[(dst_pos_no<<1) + 1];
    *out_dir = FORWARD;
    if(src_step == dst_step){
        *out_steps = 0;
    }
    else if(src_step > dst_step){
        *out_dir = BACKWARD;
        *out_steps = src_step - dst_step;
    }
    else if(src_step < dst_step){
        *out_steps = dst_step - src_step;
    }
    return 0;
}

