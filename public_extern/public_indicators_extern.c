/**********************************************************
filename   : public_indicators_extern.c
function   : 指示器接口定义（扩展部分）
author     : 谢令
created    : 2023-01-06
comment    :
**********************************************************/
#include "public_missions.h"
#include "public_indicators_extern.h"

/*
 * 指示器输出指引: 判断指定的条件是否触发
 *          返回：0(未触发)1(已触发)
*/
int IndicatorExternShow(MissionInstance* pMission, Indicator* pIndctArr, int id){
    switch(pIndctArr[id].type){
        default:{
            break;
        }
    }
    return 0;
}
