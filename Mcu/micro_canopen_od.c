/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2023-09-06     koala.zhoubo       the first version
 */
#include "micro_canopen_od.h"



struct micro_canopen_od_items od_items[] = {
    // [索引]    [子索引]   [数据类型]               [访问权限]         [数据值]  [读取值]  [写入值]  [最小值]  [最大值]  [单位]  [参数名]           [参数描述]
    
    // 错误寄存器 0x1001
    {0x1001,     0,        OD_DATA_TYPE_UINT8,      OD_RW_ACCESS_RO,  0,      0,        0,        0,       0,        0,      "Error Register", ""},

    // 硬件版本 0x1009
    {0x1009,     0,        OD_DATA_TYPE_UINT16,     OD_RW_ACCESS_RO,  0,      0,        0,        0,       0,        0,      "Hardware Version", ""},

    // 软件版本 0x100A
    {0x100A,     0,        OD_DATA_TYPE_UINT16,     OD_RW_ACCESS_RO,  0,      0,        0,        0,       0,        0,      "Software Version", ""},

    // 生产者心跳间隔 0x1017 ms
    {0x1017,     0,        OD_DATA_TYPE_UINT16,    OD_RW_ACCESS_RW,  0,      100,      100,      0,       0xFFFF,  0,      "Producer Heartbeat Interval", ""},

    // 通讯掉线保护时间 0x2000 驱动器与主机通讯掉电时间设置,单位：ms
    {0x2000,     0,        OD_DATA_TYPE_UINT16,     OD_RW_ACCESS_RW,  0,      0,        0,        0,       0,        0,      "Communication Timeout", ""},

    // 反馈位置清零 0x2005
    {0x2005,     0,        OD_DATA_TYPE_UINT8,      OD_RW_ACCESS_WO,  0,      0,        0,        0,       0,        0,      "Feedback Position Clear", ""},

    // 设置绝对位置原点 0x2006
    {0x2006,     0,        OD_DATA_TYPE_UINT8,      OD_RW_ACCESS_WO,  0,      0,        0,        0,       0,        0,      "Set Absolute Position Origin", ""},

    // 上电锁轴方式 0x2007
    {0x2007,     0,        OD_DATA_TYPE_UINT8,      OD_RW_ACCESS_RW,  0,      0,        0,        0,       0,        0,      "Power On Lock Axis Mode", ""},

    // 电机最大转速 0x2008
    {0x2008,     0,        OD_DATA_TYPE_UINT32,     OD_RW_ACCESS_RW,  0,      0,        0,        0,       0,        0,      "Motor Max Speed", ""},

    // 寄存器参数设置 0x2009
    {0x2009,     0,        OD_DATA_TYPE_UINT8,      OD_RW_ACCESS_RW,  0,      0,        0,        0,       0,        0,      "Register Parameter Setting", ""},

    // CAN 自定义驱动器节点号 0x200A
    {0x200A,     0,        OD_DATA_TYPE_UINT8,      OD_RW_ACCESS_RW,  0,      0,        0,        0,       0,        0,      "CAN Custom Driver Node ID", ""},
    
    // CAN 自定义通讯波特率
    {0x200B,     0,        OD_DATA_TYPE_UINT32,     OD_RW_ACCESS_RW,  0,      0,        0,        0,       0,        0,      "CAN Custom Communication Baud Rate", ""},

    // 电机极对数(左) 0x200C
    {0x200C,     1,        OD_DATA_TYPE_UINT16,     OD_RW_ACCESS_RW,  0,      0,        0,        0,       0,        0,      "Motor Pole Pairs(Left)", ""},

    // 电机极对数(右)
    {0x200C,     2,        OD_DATA_TYPE_UINT16,     OD_RW_ACCESS_RW,  0,      0,        0,        0,       0,        0,      "Motor Pole Pairs(Right)", ""},

    // 起始速度(左) 0x200D
    {0x200D,     1,        OD_DATA_TYPE_UINT32,     OD_RW_ACCESS_RW,  0,      0,        0,        0,       0,        0,      "Start Speed(Left)", ""},

    // 起始速度(右)
    {0x200D,     2,        OD_DATA_TYPE_UINT32,     OD_RW_ACCESS_RW,  0,      0,        0,        0,       0,        0,      "Start Speed(Right)", ""},

    // 编码器线数设置(左) 0x200E
    {0x200E,     1,        OD_DATA_TYPE_UINT32,     OD_RW_ACCESS_RW,  0,      0,        0,        0,       0,        0,      "Encoder Line Number Setting(Left)", ""},
    
    // 编码器线数设置(右)
    {0x200E,     2,        OD_DATA_TYPE_UINT32,     OD_RW_ACCESS_RW,  0,      0,        0,        0,       0,        0,      "Encoder Line Number Setting(Right)", ""},

    // 同步/异步控制标志位
    {0x200F,     0,        OD_DATA_TYPE_UINT16,      OD_RW_ACCESS_RW,  0,      0,        0,        0,       0,        0,      "Synchronous/Asynchronous Control Flag", ""},

    // 是否保存参数于EEPROM
    {0x2010,     0,        OD_DATA_TYPE_UINT8,      OD_RW_ACCESS_RW,  0,      0,        0,        0,       0,        0,      "Whether to Save Parameters to EEPROM", ""},


    // 电机与 Hall 的偏移角度(左)
    {0x2011,     1,        OD_DATA_TYPE_UINT16,     OD_RW_ACCESS_RW,  0,      0,        0,        0,       360,      0,      "Motor and Hall Offset Angle(Left)", ""},

    // 电机与 Hall 的偏移角度(右)
    {0x2011,     2,        OD_DATA_TYPE_UINT16,     OD_RW_ACCESS_RW,  0,      0,        0,        0,       360,      0,      "Motor and Hall Offset Angle(Right)", ""},

    // 过载系数(左)
    {0x2012,     1,        OD_DATA_TYPE_UINT16,     OD_RW_ACCESS_RW,  0,      0,        0,        0,       1000,     0,      "Overload Coefficient(Left)", ""},

    // 过载系数(右)
    {0x2012,     2,        OD_DATA_TYPE_UINT16,     OD_RW_ACCESS_RW,  0,      0,        0,        0,       1000,     0,      "Overload Coefficient(Right)", ""},

    // 电机温度保护阈值(左)
    {0x2013,     1,        OD_DATA_TYPE_UINT16,     OD_RW_ACCESS_RW,  0,      0,        0,        0,       150,      0,      "Motor Temperature Protection Threshold(Left)", ""},

    // 电机温度保护阈值(右)
    {0x2013,     2,        OD_DATA_TYPE_UINT16,     OD_RW_ACCESS_RW,  0,      0,        0,        0,       150,      0,      "Motor Temperature Protection Threshold(Right)", ""},

    // 额定电流(左)
    {0x2014,     1,        OD_DATA_TYPE_UINT16,     OD_RW_ACCESS_RW,  0,      0,        0,        0,       1000,     0,      "Rated Current(Left)", ""},

    // 额定电流(右)
    {0x2014,     2,        OD_DATA_TYPE_UINT16,     OD_RW_ACCESS_RW,  0,      0,        0,        0,       1000,     0,      "Rated Current(Right)", ""},

    // 最大电流(左)
    {0x2015,     1,        OD_DATA_TYPE_UINT16,     OD_RW_ACCESS_RW,  0,      0,        0,        0,       1000,     0,      "Max Current(Left)", ""},

    // 最大电流(右)
    {0x2015,     2,        OD_DATA_TYPE_UINT16,     OD_RW_ACCESS_RW,  0,      0,        0,        0,       1000,     0,      "Max Current(Right)", ""},

    // 过载保护时间(左)
    {0x2016,     1,        OD_DATA_TYPE_UINT32,     OD_RW_ACCESS_RW,  0,      0,        0,        0,       10000,    0,      "Overload Protection Time(Left)", ""},

    // 过载保护时间(右)
    {0x2016,     2,        OD_DATA_TYPE_UINT32,     OD_RW_ACCESS_RW,  0,      0,        0,        0,       10000,    0,      "Overload Protection Time(Right)", ""},

    // 超差报警阈值(左)
    {0x2017,     1,        OD_DATA_TYPE_UINT32,     OD_RW_ACCESS_RW,  0,      0,        0,        0,       10000,    0,      "Deviation Alarm Threshold(Left)", ""},

    // 超差报警阈值(右)
    {0x2017,     2,        OD_DATA_TYPE_UINT32,     OD_RW_ACCESS_RW,  0,      0,        0,        0,       10000,    0,      "Deviation Alarm Threshold(Right)", ""},

    // L 速度平滑系数
    {0x2018,     1,        OD_DATA_TYPE_UINT16,     OD_RW_ACCESS_RW,  0,      0,        0,        0,       1000,     0,      "L Speed Smooth Coefficient", ""},

    // R 速度平滑系数
    {0x2018,     2,        OD_DATA_TYPE_UINT16,     OD_RW_ACCESS_RW,  0,      0,        0,        0,       1000,     0,      "R Speed Smooth Coefficient", ""},

    // L 电流环比例系数
    {0x2019,     1,        OD_DATA_TYPE_UINT16,     OD_RW_ACCESS_RW,  0,      0,        0,        0,       10000,    0,      "L Current Loop Proportion Coefficient", ""},

    // R 电流环比例系数
    {0x2019,     2,        OD_DATA_TYPE_UINT16,     OD_RW_ACCESS_RW,  0,      0,        0,        0,       10000,    0,      "R Current Loop Proportion Coefficient", ""},

    // L 电流环积分增益
    {0x201A,     1,        OD_DATA_TYPE_UINT16,     OD_RW_ACCESS_RW,  0,      0,        0,        0,       10000,    0,      "L Current Loop Integral Gain", ""},

    // R 电流环积分增益
    {0x201A,     2,        OD_DATA_TYPE_UINT16,     OD_RW_ACCESS_RW,  0,      0,        0,        0,       10000,    0,      "R Current Loop Integral Gain", ""},

    // L 前馈输出平滑系数
    {0x201B,     1,        OD_DATA_TYPE_UINT16,     OD_RW_ACCESS_RW,  0,      0,        0,        0,       1000,     0,      "L Feedforward Output Smooth Coefficient", ""},

    // R 前馈输出平滑系数
    {0x201B,     2,        OD_DATA_TYPE_UINT16,     OD_RW_ACCESS_RW,  0,      0,        0,        0,       1000,     0,      "R Feedforward Output Smooth Coefficient", ""},

    // L 转矩输出平滑系数
    {0x201C,     1,        OD_DATA_TYPE_UINT16,     OD_RW_ACCESS_RW,  0,      0,        0,        0,       1000,     0,      "L Torque Output Smooth Coefficient", ""},

    // R 转矩输出平滑系数
    {0x201C,     2,        OD_DATA_TYPE_UINT16,     OD_RW_ACCESS_RW,  0,      0,        0,        0,       1000,     0,      "R Torque Output Smooth Coefficient", ""},

    // L 速度比例增益
    {0x201D,     1,        OD_DATA_TYPE_UINT16,     OD_RW_ACCESS_RW,  0,      0,        0,        1000,    10000,    0,      "L Speed Proportion Gain", ""},

    // R 速度比例增益
    {0x201D,     2,        OD_DATA_TYPE_UINT16,     OD_RW_ACCESS_RW,  0,      0,        0,        1000,    10000,    0,      "R Speed Proportion Gain", ""},

    // L 速度积分增益Ki
    {0x201E,     1,        OD_DATA_TYPE_UINT16,     OD_RW_ACCESS_RW,  0,      0,        0,        1000,    10000,    0,      "L Speed Integral Gain Ki", ""},

    // R 速度积分增益Ki
    {0x201E,     2,        OD_DATA_TYPE_UINT16,     OD_RW_ACCESS_RW,  0,      0,        0,        1000,    10000,    0,      "R Speed Integral Gain Ki", ""},

    // L 速度前馈增益 Kf
    {0x201F,     1,        OD_DATA_TYPE_UINT16,     OD_RW_ACCESS_RW,  0,      0,        0,        1000,    10000,    0,      "L Speed Feedforward Gain Kf", ""},

    // R 速度前馈增益 Kf
    {0x201F,     2,        OD_DATA_TYPE_UINT16,     OD_RW_ACCESS_RW,  0,      0,        0,        1000,    10000,    0,      "R Speed Feedforward Gain Kf", ""},

    // L 位置比例增益 Kp
    {0x2020,     1,        OD_DATA_TYPE_UINT16,     OD_RW_ACCESS_RW,  0,      0,        0,        1000,    10000,    0,      "L Position Proportion Gain Kp", ""},

    // R 位置比例增益 Kp
    {0x2020,     2,        OD_DATA_TYPE_UINT16,     OD_RW_ACCESS_RW,  0,      0,        0,        1000,    10000,    0,      "R Position Proportion Gain Kp", ""},

    // L 位置前馈增益 Kf
    {0x2021,     1,        OD_DATA_TYPE_UINT16,     OD_RW_ACCESS_RW,  0,      0,        0,        1000,    10000,    0,      "L Position Feedforward Gain Kf", ""},

    // R 位置前馈增益 Kf
    {0x2021,     2,        OD_DATA_TYPE_UINT16,     OD_RW_ACCESS_RW,  0,      0,        0,        1000,    10000,    0,      "R Position Feedforward Gain Kf", ""},

    // L 速度观测器系数 1
    {0x2022,     1,        OD_DATA_TYPE_UINT32,     OD_RW_ACCESS_RW,  0,      0,        0,        1000,    100000,   0,      "L Speed Observer Coefficient 1", ""},

    // R 速度观测器系数 1
    {0x2022,     2,        OD_DATA_TYPE_UINT32,     OD_RW_ACCESS_RW,  0,      0,        0,        1000,    100000,   0,      "R Speed Observer Coefficient 1", ""},

    // L 速度观测器系数 2
    {0x2023,     1,        OD_DATA_TYPE_UINT32,     OD_RW_ACCESS_RW,  0,      0,        0,        1000,    100000,   0,      "L Speed Observer Coefficient 2", ""},

    // R 速度观测器系数 2
    {0x2023,     2,        OD_DATA_TYPE_UINT32,     OD_RW_ACCESS_RW,  0,      0,        0,        1000,    100000,   0,      "R Speed Observer Coefficient 2", ""},

    // L 速度观测器系数 3
    {0x2024,     1,        OD_DATA_TYPE_UINT32,     OD_RW_ACCESS_RW,  0,      0,        0,        1000,    100000,   0,      "L Speed Observer Coefficient 3", ""},

    // R 速度观测器系数 3
    {0x2024,     2,        OD_DATA_TYPE_UINT32,     OD_RW_ACCESS_RW,  0,      0,        0,        1000,    100000,   0,      "R Speed Observer Coefficient 3", ""},

    // L 速度观测器系数 4
    {0x2025,     1,        OD_DATA_TYPE_UINT32,     OD_RW_ACCESS_RW,  0,      0,        0,        1000,    100000,   0,      "L Speed Observer Coefficient 4", ""},

    // R 速度观测器系数 4
    {0x2025,     2,        OD_DATA_TYPE_UINT32,     OD_RW_ACCESS_RW,  0,      0,        0,        1000,    100000,   0,      "R Speed Observer Coefficient 4", ""},

    // 报警 PWM 处理方式
    {0x2026,     0,        OD_DATA_TYPE_UINT8,      OD_RW_ACCESS_RW,  0,      0,        0,        0,       2,        0,      "Alarm PWM Processing Mode", ""},

    // 过载处理方式
    {0x2027,     0,        OD_DATA_TYPE_UINT8,      OD_RW_ACCESS_RW,  0,      0,        0,        0,       2,        0,      "Overload Processing Mode", ""},

    // 电机温度(左) 0x2032
    {0x2032,     1,        OD_DATA_TYPE_UINT16,     OD_RW_ACCESS_RO,  0,      0,        0,        0,       150,      0,      "Motor Temperature(Left)", ""},

    // 电机温度(右)
    {0x2032,     2,        OD_DATA_TYPE_UINT16,     OD_RW_ACCESS_RO,  0,      0,        0,        0,       150,      0,      "Motor Temperature(Right)", ""},

    // 驱动器温度
    {0x2032,     3,        OD_DATA_TYPE_UINT16,     OD_RW_ACCESS_RO,  0,      0,        0,        0,       150,      0,      "Driver Temperature", ""},

    // 电机状态寄存器(左)
    {0x2033,     1,        OD_DATA_TYPE_UINT16,     OD_RW_ACCESS_RO,  0,      0,        0,        0,       0xFFFF,   0,      "Motor Status Register(Left)", ""},

    // 电机状态寄存器(右)
    {0x2033,     2,        OD_DATA_TYPE_UINT16,     OD_RW_ACCESS_RO,  0,      0,        0,        0,       0xFFFF,   0,      "Motor Status Register(Right)", ""},

    // 霍尔输入状态(左)
    {0x2034,     1,        OD_DATA_TYPE_UINT8,      OD_RW_ACCESS_RO,  0,      0,        0,        0,       0xFF,     0,      "Hall Input Status(Left)", ""},

    // 霍尔输入状态(右)
    {0x2034,     2,        OD_DATA_TYPE_UINT8,      OD_RW_ACCESS_RO,  0,      0,        0,        0,       0xFF,     0,      "Hall Input Status(Right)", ""},

    // 母线电压
    {0x2035,     0,        OD_DATA_TYPE_UINT16,     OD_RW_ACCESS_RO,  0,      0,        0,        0,       10000,    0,      "Bus Voltage", ""},

    // 驱动器最近一次故障码 0x603f
    {0x603F,     0,        OD_DATA_TYPE_UINT32,     OD_RW_ACCESS_RO,  0,      0,        0,        0,       0xFFFFFFFF,0,      "Driver Last Fault Code", ""},

    // 控制字
    {0x6040,     0,        OD_DATA_TYPE_UINT16,     OD_RW_ACCESS_RW,  0,      0,        0,        0,       0xFFFF,   0,      "Control Word", ""},

    // 状态字
    {0x6041,     0,        OD_DATA_TYPE_UINT16,     OD_RW_ACCESS_RO,  0,      0,        0,        0,       0xFFFF,   0,      "Status Word", ""},

    // 快速停止代码
    {0x605A,     0,        OD_DATA_TYPE_UINT16,     OD_RW_ACCESS_RW,  0,      0,        0,        0,       7,        0,      "Quick Stop Code", ""},

    // 关闭操作代码
    {0x605B,     0,        OD_DATA_TYPE_UINT16,     OD_RW_ACCESS_RW,  0,      0,        0,        0,       7,        0,      "Shutdown Code", ""},

    // 禁用操作代码
    {0x605C,     0,        OD_DATA_TYPE_UINT16,     OD_RW_ACCESS_RW,  0,      0,        0,        0,       7,        0,      "Disable Code", ""},

    // Halt 控制寄存器
    {0x605D,     0,        OD_DATA_TYPE_UINT16,     OD_RW_ACCESS_RW,  0,      0,        0,        0,       7,        0,      "Halt Control Register", ""},

    // 运行模式
    {0x6060,     0,        OD_DATA_TYPE_UINT8,      OD_RW_ACCESS_RW,  0,      0,        0,        0,       8,        0,      "Operation Mode", ""},

    // 运行模式状态
    {0x6061,     0,        OD_DATA_TYPE_UINT8,      OD_RW_ACCESS_RO,  0,      0,        0,        0,       8,        0,      "Operation Mode Status", ""},

    // 实际位置反馈(左)
    {0x6064,     1,        OD_DATA_TYPE_INT32,      OD_RW_ACCESS_RO,  0,      0,        0,        0,       0xFFFFFFFF,0,      "Actual Position Feedback(Left)", ""},

    // 实际位置反馈(右)
    {0x6064,     2,        OD_DATA_TYPE_INT32,      OD_RW_ACCESS_RO,  0,      0,        0,        0,       0xFFFFFFFF,0,      "Actual Position Feedback(Right)", ""},

    // 实际速度反馈(左)
    {0x606C,     1,        OD_DATA_TYPE_INT32,      OD_RW_ACCESS_RO,  0,      0,        0,        0,       0xFFFFFFFF,0,      "Actual Speed Feedback(Left)", ""},

    // 实际速度反馈(右)
    {0x606C,     2,        OD_DATA_TYPE_INT32,      OD_RW_ACCESS_RO,  0,      0,        0,        0,       0xFFFFFFFF,0,      "Actual Speed Feedback(Right)", ""},

    // 左右实际速度组合
    {0x606C,     3,        OD_DATA_TYPE_INT32,      OD_RW_ACCESS_RO,  0,      0,        0,        0,       0xFFFFFFFF,0,      "Actual Speed Feedback(Left and Right)", ""},

    // 目标转矩(左)
    {0x6071,     1,        OD_DATA_TYPE_INT16,      OD_RW_ACCESS_RW,  0,      0,        0,        -1000,   1000,     0,      "Target Torque(Left)", ""},

    // 目标转矩(右)
    {0x6071,     2,        OD_DATA_TYPE_INT16,      OD_RW_ACCESS_RW,  0,      0,        0,        -1000,   1000,     0,      "Target Torque(Right)", ""},

    // 左右目标转矩组合
    {0x6071,     3,        OD_DATA_TYPE_INT32,      OD_RW_ACCESS_RW,  0,      0,        0,        -1000,   1000,     0,      "Target Torque(Left and Right)", ""},

    // 实时转矩反馈(左)
    {0x6077,     1,        OD_DATA_TYPE_INT16,      OD_RW_ACCESS_RO,  0,      0,        0,        -1000,   1000,     0,      "Real Time Torque Feedback(Left)", ""},

    // 实时转矩反馈(右)
    {0x6077,     2,        OD_DATA_TYPE_INT16,      OD_RW_ACCESS_RO,  0,      0,        0,        -1000,   1000,     0,      "Real Time Torque Feedback(Right)", ""},

    // 左右实时转矩组合
    {0x6077,     3,        OD_DATA_TYPE_INT32,      OD_RW_ACCESS_RO,  0,      0,        0,        -1000,   1000,     0,      "Real Time Torque Feedback(Left and Right)", ""},

    // 目标位置(左)
    {0x607A,     1,        OD_DATA_TYPE_INT32,      OD_RW_ACCESS_RW,  0,      0,        0,        -2147483648,2147483647,0,      "Target Position(Left)", ""},
    
    // 目标位置(右)
    {0x607A,     2,        OD_DATA_TYPE_INT32,      OD_RW_ACCESS_RW,  0,      0,        0,        -2147483648,2147483647,0,      "Target Position(Right)", ""},

    // 最大速度(左)
    {0x6081,     1,        OD_DATA_TYPE_UINT32,     OD_RW_ACCESS_RW,  0,      0,        0,        0,       100000,   0,      "Max Speed(Left)", ""},

    // 最大速度(右)
    {0x6081,     2,        OD_DATA_TYPE_UINT32,     OD_RW_ACCESS_RW,  0,      0,        0,        0,       100000,   0,      "Max Speed(Right)", ""},

    // 位置模式启/停速度(左)
    {0x6082,     1,        OD_DATA_TYPE_UINT32,     OD_RW_ACCESS_RW,  0,      0,        0,        0,       100000,   0,      "Position Mode Start/Stop Speed(Left)", ""},

    // 位置模式启/停速度(右)
    {0x6082,     2,        OD_DATA_TYPE_UINT32,     OD_RW_ACCESS_RW,  0,      0,        0,        0,       100000,   0,      "Position Mode Start/Stop Speed(Right)", ""},

    // S 形加速时间(左)
    {0x6083,     1,        OD_DATA_TYPE_UINT32,     OD_RW_ACCESS_RW,  0,      0,        0,        1,       100000,   0,      "S Shape Acceleration Time(Left)", ""},

    // S 形加速时间(右)
    {0x6083,     2,        OD_DATA_TYPE_UINT32,     OD_RW_ACCESS_RW,  0,      0,        0,        1,       100000,   0,      "S Shape Acceleration Time(Right)", ""},

    // S 形减速时间(左)
    {0x6084,     1,        OD_DATA_TYPE_UINT32,     OD_RW_ACCESS_RW,  0,      0,        0,        1,       100000,   0,      "S Shape Deceleration Time(Left)", ""},

    // S 形减速时间(右)
    {0x6084,     2,        OD_DATA_TYPE_UINT32,     OD_RW_ACCESS_RW,  0,      0,        0,        1,       100000,   0,      "S Shape Deceleration Time(Right)", ""},

    // 急停减速时间(左)
    {0x6085,     1,        OD_DATA_TYPE_UINT32,     OD_RW_ACCESS_RW,  0,      0,        0,        1,       100000,   0,      "Emergency Stop Deceleration Time(Left)", ""},

    // 急停减速时间(右)
    {0x6085,     2,        OD_DATA_TYPE_UINT32,     OD_RW_ACCESS_RW,  0,      0,        0,        1,       100000,   0,      "Emergency Stop Deceleration Time(Right)", ""},

    // 转矩斜率(左)
    {0x6087,     1,        OD_DATA_TYPE_UINT16,     OD_RW_ACCESS_RW,  0,      0,        0,        1,       1000,     0,      "Torque Slope(Left)", ""},

    // 转矩斜率(右)
    {0x6087,     2,        OD_DATA_TYPE_UINT16,     OD_RW_ACCESS_RW,  0,      0,        0,        1,       1000,     0,      "Torque Slope(Right)", ""},

    // 目标速度(左)
    {0x60FF,     1,        OD_DATA_TYPE_INT32,      OD_RW_ACCESS_RW,  0,      0,        0,        -100000, 100000,   0,      "Target Speed(Left)", ""},

    // 目标速度(右)
    {0x60FF,     2,        OD_DATA_TYPE_INT32,      OD_RW_ACCESS_RW,  0,      0,        0,        -100000, 100000,   0,      "Target Speed(Right)", ""},
    // 目标速度(左+右)
    {0x60FF,     3,        OD_DATA_TYPE_INT32,      OD_RW_ACCESS_RW,  0,      0,        0,        -100000, 100000,   0,      "Target Speed(Left+Right)", ""},


};

/*--------------------------------------------- TPDO RPDO -----------------------------------*/
struct micro_canopen_od_pdo_itmes_t rpdo_items[]= {
    // [索引] [在MAP数组的下标] [在数据数组的起始位置]

    // ---------- RPDO ----------
     {OD_ID_TARGET_SPEED_LEFT_RIGHT,0,0},

    // Left Motor Acc
    {OD_ID_S_SHAPE_ACCELERATION_TIME_LEFT,1,0},

    // Right Motor Acc
    {OD_ID_S_SHAPE_ACCELERATION_TIME_RIGHT,1,4},

    // Left Motor Dec
    {OD_ID_S_SHAPE_DECELERATION_TIME_LEFT,2,0},

    // Right Motor Dec
    {OD_ID_S_SHAPE_DECELERATION_TIME_RIGHT,2,4},

    // Controlword
    {OD_ID_CONTROL_WORD,3,0}
};

struct micro_canopen_pdo_cfg_t rpdo_cfg[] = {
  
        //[0: 定时触发 1: 行为触发]    [定时触发时间]    [行为触发]    [0: 不保存 1: 保存]

        // RPDO  主机下发数据
        {0,1000,0,1},  // 0x1600
        {0,1000,0,1},  // 0x1601
        {0,1000,0,1},  // 0x1602
        {0,1000,0,1},  // 0x1603
};

struct micro_canopen_od_pdo_itmes_t tpdo_items[]= {
    // ---------- TPDO ----------
    // 
    {OD_ID_ACTUAL_SPEED_FEEDBACK_LEFT_AND_RIGHT,0,0},

    {OD_ID_DRIVER_LAST_FAULT_CODE,0,4},    

    {OD_ID_ACTUAL_POSITION_FEEDBACK_LEFT,1,0},

    {OD_ID_ACTUAL_POSITION_FEEDBACK_RIGHT,1,4},

    {OD_ID_CONTROL_WORD,2,0},

};

struct micro_canopen_pdo_cfg_t tpdo_cfg[] = {

        // TPDO  主机接收从机的数据
        {0,1000,0,1},  // 0x1800
        {0,1000,0,1},  // 0x1801
        {0,1000,0,1},  // 0x1802
        {1,1000,0,1},  // 0x1803
};
