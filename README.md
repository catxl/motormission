# motormission
A state machine for stepmotor missions

# 概况
本项目主要为实时嵌入式控制系统，实现了一个利用电机、光耦等接口控制电机执行更复杂运动的状态机。
可通过json配置状态机，并由python脚本(trans.py)将其自动生成流程图(png)和对应的c程序配置。
任务状态机的基本运行逻辑是：在某个状态(state)下，当指示器(indicator)被激发，则执行一些动作(action)，并进入下一状态(state)。
状态机从入口(ENTRY)状态开始执行，执行到终结(FINAL)状态停止。

# Mission配置方法
## "vars"
定义任务的参数，类似于函数参数，配置变量名、默认值、说明文字
## "states" 
配置流程中的状态和该状态的类型，可定义的类型包括："ENTRY"/"NORMAL"/"FINAL"
## "indicators" 
配置流程中的指示器(indicator)，每个 indicator 都由一个独立的名字，其对应的配置内容是：指示器类型、参数、说明文字，其中指示器类型和参数与 public_indicators.h 中的实现有关。说明文字主要用于在流程图中显示。
## "actions" 
配置流程中的动作(action)，每个 action 都由一个独立的名字，其对应的配置内容是：动作类型、参数、说明文字，其中动作类型和参数与 public_actions.h 中的实现有关。动作类型和参数可配置多个，这些动作是同时执行的。说明文字必须配置在最后一行，主要用于在流程图中显示。
## "transformers"
状态机的定义，每一条标识一个状态转换关系：开始状态 在 指示器(indicator)被激发（即满足指定的条件）时，执行指定的动作(action)，并进入其他状态(next-state)，其中next-state可定义多个。
## "instance_number"
定义任务实例数。例如当你的系统中有6个电机都要执行本Mission定义的流程，那么可以将此项配置成6。


# 增加新任务(Mission)的流程
## 1. 复制任务定义目录
在 state_machine 目录下复制 demo_action 子目录，粘贴在同一级目录(state_machine)下，新目录命名为你想要的名字(符合C语言变量命名规则即可)，例如"example"，并将该目录(state_machine/example)下的 demo_action.json 重命名为 "example.json"。
## 2. 修改定义任务的json文件
按照你的设计，在example.json中定义任务。
## 3. 生成任务配置
执行该目录下的 gen_and_cp.bat 脚本，它将根据你的配置(example.json)生成任务的执行流程图(example.png)和任务配置代码(example.c)，并同时将 example.c 复制到了..\..\mission\目录下。
## 4. 将新任务引入你的工程
### 在 mission/all_missions.h 中，为新定义的任务增加一个ID，例如 MISSION_EXAMPLE。
### 在 mission/all_missions.c 中，声明外部变量 extern MissionInstance g_mission_inst_example[]; 并将其配置在 g_cfg_mission 中与前面的ID(MISSION_EXAMPLE)对应的位置上。
### 在你的程序中（比如main.c中"while(1)"的前面），执行 "MissionInstanceStart(0, MISSION_EXAMPLE);" 即可。
