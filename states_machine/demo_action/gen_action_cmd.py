#!/usr/bin/python3
#coding=utf-8
from datetime import datetime
import sys
import os

def ToHex(n, fixlen=1):
    # 将short值转换为无符号int类型
    num = n & ((1<<(fixlen*8)) - 1)
    # 将无符号int类型转换为十六进制字符串并添加前导零至4位长度
    hex_str = hex(num)[2:]
    if len(hex_str) < fixlen*2:
        hex_str = '0' * (fixlen*2 - len(hex_str)) + hex_str
    s = hex_str.upper()
    return [s[i:i+2] for i in range(0, len(s), 2)]

def GetPackHex(msg):
    #return ' '.join([hex(c).replace('0x', '') for c in msg])
    return ' '.join(['%02X' % ch for ch in msg])

# python trans.py "m0_init"
if __name__ == '__main__':
                       # 32: ACT_CLEAN_PROC_SUBSTRATE_SPIT,       // CleanProcessor: 注入底物
    action_type = 32   # 33: ACT_CLEAN_PROC_SUBSTRATE_INIT 
                       # 34: ACT_CLEAN_PROC_SUBSTRATE_TASK_STOP,  // CleanProcessor: 底物动作停止
    args = [12, 34, 56, 0x1234, 0x8, 0]

    msg = bytes.fromhex("68 1d 11 00 05 90 06")
    msg += action_type.to_bytes(1, 'big')
    for i in range(6):
        msg += args[i].to_bytes(2, 'big')
    checkSum = 0
    for ch in msg:
        checkSum += ch
    checkSum -= (0x68 + 0x1d + 0x11)
    checkSum &= 0xFF
    msg += checkSum.to_bytes(1, 'big') + bytes.fromhex("16")
    
    print(GetPackHex(msg))
    

