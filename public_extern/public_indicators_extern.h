/**********************************************************
filename   : public_indicators_extern.h
function   : 指示器接口定义（扩展部分）
author     : 谢令
created    : 2023-01-06
comment    :
**********************************************************/
#ifndef _PUBLIC_INDICATORS_EXTERN_H_
#define _PUBLIC_INDICATORS_EXTERN_H_

#include "common.h"
#include "public_indicators.h"

/*
 * 指示器类型
*/
enum INDICATOR_EXTERN_TYPE {
    INDICATOR_EXTERN_TYPE_BEGIN = INDICATOR_TYPE_NUM - 1 ,  // <--!!这行不能删
    // TODO: 添加扩展类型
    INDICATOR_EXTERN_TYPE_NUM
};

/*
 * 指示器输出指引: 判断指定的条件是否触发
 *          返回：0(未触发)1(已触发)
*/
int IndicatorExternShow(MissionInstance * pMission, Indicator* pIndctArr, int id);

#endif
