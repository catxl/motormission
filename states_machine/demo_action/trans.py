#!/usr/bin/python3
#coding=utf-8
from datetime import datetime
import sys
import os
import json

#from graphviz import Digraph
#from transitions import Machine
#from transitions.extensions import GraphMachine

def LoadJson(fn):
    f = open(fn, 'r', encoding='utf-8')
    obj = json.load(f)
    f.close()
    return obj

SELF_VAR_PREFIX = "$"

def TransToC(srcobj, missname):
    # vars
    myvars = srcobj['vars']
    myvars_info = {}
    my_var_arg_idx = 0
    for v in myvars:
        myvars_info[v[0]] = my_var_arg_idx
        my_var_arg_idx += 1

    # states
    states = srcobj['states']
    states_names = states.keys()
    states_type = []
    for st in states:
        if("ENTRY"==srcobj['states'][st]):
            states_type.append("NODE_TYPE_ENTRY")
        elif("NORMAL"==srcobj['states'][st]):
            states_type.append("NODE_TYPE_NORMAL")
        elif("FINAL"==srcobj['states'][st]):
            states_type.append("NODE_TYPE_FINAL")

    # actions
    actions = srcobj['actions']
    action_names = actions.keys()
    action_tips = []
    for nm in action_names :
        for i in range(len(actions[nm])-1):
            actions[nm][i].append("0")
            for j in range(len(actions[nm][i][1])):
                if(SELF_VAR_PREFIX == actions[nm][i][1][j][0]):
                    actions[nm][i][1][j] = str(myvars_info[actions[nm][i][1][j]])
                    if("0"==actions[nm][i][2]):
                        actions[nm][i][2] = "(1<<"+str(j)+")"
                    else:
                        actions[nm][i][2] += "|(1<<"+str(j)+")"
            actions[nm][i][1] += ['0','0','0','0','0','0']
    
    # indicators
    indicators = srcobj['indicators']
    ind_names = indicators.keys()
    for nm in ind_names :
        indicators[nm].append("0")
        for j in range(len(indicators[nm][1])):
            if(SELF_VAR_PREFIX == indicators[nm][1][j][0]):
                indicators[nm][1][j] = str(myvars_info[indicators[nm][1][j]])
                if("0"==indicators[nm][3]):
                    indicators[nm][3] = "(1<<"+str(j)+")"
                else:
                    indicators[nm][3] += "|(1<<"+str(j)+")"
        indicators[nm][1] += ['0','0','0','0','0','0']
    
    # transformers
    transformers = srcobj['transformers']
    
    # instance_number
    instance_number = 1
    if("instance_number" in srcobj):
        if(srcobj["instance_number"]>1):
            instance_number = srcobj["instance_number"]
    
    out_c_file_paht = "mission_" + missname + ".c"
    f = open(out_c_file_paht, 'w', encoding='utf-8')
    f.write('''/**********************************************************
filename   : mission_''' + missname + '''.c
function   : 任务: ''' + missname + '''
**********************************************************/
#include "mission_inc.h"

// 任务参数说明
''')
    i = 0
    for v in myvars:
        f.write("//    args[{}] : {} : {} \n".format(i, v[0], v[2]))
        i+=1
    f.write('''
//=========================================================
// 1. 配置状态节点
//=========================================================
// 状态节点ID定义
enum STATUS_NODE_IDS{
    ''')
    f.write(", ".join(states_names) + ", NODE_NUM\n") # ST_00, ST_01, ST_02, ST_03, ST_FINAL, NODE_NUM
    f.write('''};
// 状态节点配置数据
static StatusNodeConf g_cfg_status_node[NODE_NUM] = {
    {''')
    f.write("}, {".join(states_type) + "}\n")   # {NODE_TYPE_NORMAL},{NODE_TYPE_NORMAL},{NODE_TYPE_NORMAL},{NODE_TYPE_NORMAL},{NODE_TYPE_FINAL}
    f.write('''};
static const uint_t g_cfg_status_node_num = NODE_NUM;
    

//=========================================================
// 2. 配置动作
//=========================================================
/*
 * 执行动作
 * 动作参数(参考：public_actions.c)
 *    伐操作，参数序号: 0(on/off)，1(阀的数量)，2-5阀的ID
 *    开关操作，参数序号: 0(on/off)，1(开关的ID)
 *    启动倒计时，参数序号: 0(计时器的ID)，1(超时ms)
 *    步进电机转动，参数序号: 0(步进电机的ID)，1(方向)，2(速度)，3(步数)
 *    混匀电机转动，参数序号: 0(电机ID)，1(速度)
 *    修改节点状态，参数序号: 0(任务ID)，1(节点ID)，2(状态值)
 *    任务完成，参数序号: 0(任务ID)
 *    任务失败，参数序号: 0(任务ID)，1(错误码)
 *    自定义执行函数，参数序号: 0(函数入口地址)
*/
''')
    #// A0: 离开光藕
    #static Action A_00[] = {
    #    {0, ACT_MOTOR_MOVE, {CLEAN_PLATE_ROTATE_M0, 1, 1, 0x1000, 0, 0}, (1<<0)}
    #};
    #static const int A_00_NUM = sizeof(A_00)/sizeof(Action);
    for nm in action_names :
        f.write("// " + nm + ": " + actions[nm][len(actions[nm])-1] + "\n")
        f.write("static Action " + nm + "[] = {\n")
        for i in range(len(actions[nm])-1):
            fmtstr = "{:<5}{:<30}{:<30}{}".format(str(i)+",", actions[nm][i][0]+",",  "{" + ", ".join(actions[nm][i][1][0:6]) + "},", actions[nm][i][2])
            #import pdb; pdb.set_trace()
            f.write("    {" + fmtstr + "},\n")
        f.write("};\n")
        f.write("static const int " + nm + "_NUM = sizeof(" + nm + ")/sizeof(Action);\n")
    f.write('''
//=========================================================
// 3. 配置指示器
//=========================================================
/*
 * 任务的指示器配置
 * 1. 请确保指示器(Indicator)的配置顺序：0~N
*/
// 指示器ID定义
enum INDCT_IDS{
    ''')
    f.write(", ".join(ind_names) + ", INDCT_IDS_NUM\n") # INDCT_00, INDCT_01, INDCT_02, INDCT_IDS_NUM
    
    f.write('''};
static Indicator g_cfg_mission_idincator[INDCT_IDS_NUM] = {
//   id,                           type,                               args[6],                                                    argTypeSelector
//   ID,                           指示器类型                          指示器的配置参数，由指示器类型而定                          参数值选择器
''')
    #{INDCT_00,  INDCT_SENSOR_ACTIVE,      {CLEAN_PLATE_ROTATE_POS0_S0, 0,0,0,0,0}, (1<<0)},      // 00: S0传感器激活
    for nm in ind_names :
        fmtstr = "{:<30}{:<36}{:<60} {}" .format(nm+",", indicators[nm][0]+",", "{"+", ".join(indicators[nm][1][0:6])+"},", indicators[nm][3] + "},")
        f.write("    {" + fmtstr + " //" + nm + ": " + indicators[nm][2] + "\n")
    f.write('''};

//=========================================================
// 4. 配置状态机
//=========================================================
/*
 * 任务状态转换器配置 = StatusNode + Indicators + Actions + NextNode
*/
static TransformerConf g_cfg_mission_transformer[] = {
//   node_id,           indct_id,                     pActArr,            act_num,                      next_node_num,  next_node[6]
''')
    #{ST_00,             INDCT_00,  A_00,     A_00_NUM,  (next_node_number),   {ST_01}},
    for trans in transformers:
        fmtstr = "{:<20}{:<30}{:<20}{:<30}{:<16}{:<10}".format("{"+trans[0]+",", trans[1]+",", trans[2]+",", trans[2]+"_NUM,", str(len(trans[3]))+",",  "{"+",".join(trans[3])+"}},")
        f.write("    "+fmtstr+"\n")

    f.write('''};
static const uint_t g_cfg_missions_trans_num = sizeof(g_cfg_mission_transformer)/sizeof(TransformerConf);


//=========================================================
// 5. 配置任务（此处配完之后，还需在all_missions.h/.c中配置）
//=========================================================
// 定义任务 = 状态 + 指示器 + 转换器 + 初始激活节点
static Mission g_cfg_mission = {
    g_cfg_status_node,                      // 【配置】状态节点
    g_cfg_status_node_num,                  // 【配置】状态节点数量
    g_cfg_mission_idincator,                // 【配置】指示器
    INDCT_IDS_NUM,                          // 【配置】指示器数量
    g_cfg_mission_transformer,              // 【配置】转换器
    g_cfg_missions_trans_num,               // 【配置】转换器数量
};
// 记录任务内部运行状态的内存变量
static NodeStatus g_mission_node_status[''' + str(instance_number) + '''][NODE_NUM] = {0};
''')
    f.write('static uint_t g_miss_inst_args[{}][{}]'.format(instance_number, len(myvars)) + " = {0};")
    f.write('''
/*
 * 对外暴露的任务对象
*/
MissionInstance g_mission_inst_''' + missname + "[" + str(instance_number) + '''] = {
''')
    for i in range(instance_number):
        f.write('''    {
        0,
        &g_cfg_mission,                         // 任务定义的配置
        &(g_mission_node_status[''' + str(i) + '''][0]),         // 存储状态节点运行状态
        MISSION_INITIALIZED,                    // 任务执行状态
        0, 0, 0, 0,                             // 任务携带的参数
        &(g_miss_inst_args[''' + str(i) + '''][0])
    },''')
    f.write("\n};\n")
    f.close()

def TransToPng(srcobj, missname):
    out_c_file_paht = missname + ".dot"
    f = open(out_c_file_paht, 'w', encoding='utf-8')
    # states
    states = srcobj['states']
    states_names = [nm for nm in states.keys()]
    
    f.write('''digraph G{
    label = "''' + missname + '''"
    node [style = filled, color = Green, fontcolor = Red, fontsize = 12, shape = ellipse];
    {
''')
#        ST_00[label = "ST_00"];
    for st in states:
        if("ENTRY"==srcobj['states'][st]):
            f.write("        "+st+"[label = \"" + st + "\"];\n")
    f.write('''    }
    node [style = filled, color = Gray, fontcolor = Red, fontsize = 12, shape = ellipse];
    {
''')
#        ST_00[label = "ST_00"];
    for st in states:
        if("NORMAL"==srcobj['states'][st]):
            f.write("        "+st+"[label = \"" + st + "\"];\n")
    f.write('''    }
    node [style = filled, color = RED, fontcolor = Black, fontsize = 12, shape = doublecircle];
    {
''')
#        ST_00[label = "ST_00"];
    for st in states:
        if("FINAL"==srcobj['states'][st]):
            f.write("        "+st+"[label = \"" + st + "\"];\n")

    # indicators
    f.write('''    }

    node [fontname="FangSong", style = filled, color = Yellow, fontcolor = Red, fontsize = 12, shape = diamond];
    {
''')
    indct_map = {}
    indicators = srcobj['indicators']
    ind_names = indicators.keys()
    for nm in ind_names :
        indicators[nm][1] += ['0','0','0','0','0','0']
#        INDCT_00[label = "INDCT_00: S0传感器激活"];
    for nm in ind_names :
        indct_map[nm]= '[label = "' + nm + ': ' + indicators[nm][2] + '"]'
    for tr in srcobj['transformers']:
        f.write("        " + "_".join(tr[0:2]) + indct_map[tr[1]] + ';\n')

    # actions
    f.write('''    }
    
    node [fontname="FangSong", style = filled, color = lightskyblue, fontcolor = Black, fontsize = 12, shape = box];
    {
''')
    act_map = {}
    actions = srcobj['actions']
    action_names = actions.keys()
    action_tips = []
#        A_00[label = "A_00: 离开光藕"]
    for nm in action_names :
        act_map[nm] = '[label = "' + nm + ": " + actions[nm][-1] + '"]'
    for tr in srcobj['transformers']:
        f.write('        ' + "_".join(tr[0:3]) + act_map[tr[2]] +';\n')
    f.write('''    }

''')

    # transformers
    transformers = srcobj['transformers']
#    ST_00 -> INDCT_00 -> A_00 -> ST_01
    tr_col = ["", "", ""]
    for tr in transformers:
        tr_col[0] = tr[0]
        tr_col[1] = "_".join(tr[0:2])
        tr_col[2] = "_".join(tr[0:3])
        f.write("    " + " -> ".join(tr_col) + "\n")
        for nxt in tr[3]:
            f.write("    " + tr_col[2] + " -> " + nxt + "\n")
        #f.write("    " + " -> ".join(tr) + "\n")
    f.write('''}\n''')
    f.close()

    os.system('dot -Tpng "' + missname + '.dot" -o "' + missname +'.png"')
    os.system('"'+ missname + '.png"')


# python trans.py "m0_init"
if __name__ == '__main__':
    mission_name = sys.argv[1]
    srcobj = LoadJson(mission_name + ".json")
    TransToC(srcobj, mission_name)
    TransToPng(srcobj, mission_name)

