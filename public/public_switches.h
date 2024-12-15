/**********************************************************
filename   : public_switches.h
function   : ���ؿ��ƣ��������֣�
author     : л��
created    : 2022-11-23
comment    :
**********************************************************/
#ifndef _PUBLIC_SWITCHES_H_
#define _PUBLIC_SWITCHES_H_

#include "common.h"

enum SWITCH_TAG{
    SWITCH_OFF = 0,      // �ر�
    SWITCH_ON            // ��
};

// ����������
typedef struct
{
    ushort_t  uiRegIndex;         // �����ļĴ����±�
    ushort_t  uiBitSelector;      // λѡ����
    uchar_t ucInitStatus;         // ��ʼ״̬
} SWITCH_CONFIG;

// ����״ֵ̬
typedef struct
{
    uchar_t ucCurrentStatus;    // ��ǰ״̬
} SWITCH_STATUS;

// ��ȡ�������ص�״̬
uchar_t GetSwitchStatus(ushort_t id);
// ��ʼ�����п���
ushort_t InitAllSwitches(void);
// ��/�رյ�������
ushort_t SwitchTurnOn(ushort_t id);
ushort_t SwitchTurnOff(ushort_t id);
ushort_t SwitchSet(ushort_t id, uchar_t status);

// ͬʱ���� ��/�ر� �������
ushort_t SwitchSwitchTurnOn(ushort_t* id_arr, ushort_t num);
ushort_t SwitchSwitchTurnOff(ushort_t* id_arr, ushort_t num);

// ͬʱ���� ��/�ر� ���п���
ushort_t AllSwitchesTurnOn(void);
ushort_t AllSwitchesTurnOff(void);



#endif
