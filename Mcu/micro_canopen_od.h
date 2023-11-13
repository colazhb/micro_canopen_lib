/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2023-09-06     koala.zhoubo       the first version
 */
#ifndef APPLICATIONS_MICRO_CANOPEN_MICRO_CANOPEN_OD_H_
#define APPLICATIONS_MICRO_CANOPEN_MICRO_CANOPEN_OD_H_

#include "micro_canopen.h"


enum
{
    OD_ID_ERROR_REGISTER,                                       // 0x1001 错误寄存器
    OD_ID_HARDWARE_VERSION,                                     // 0x1009 硬件版本
    OD_ID_SOFTWARE_VERSION,                                     // 0x100A 软件版本    
    OD_ID_PRODUCER_HEARTBEAT_TIME,                              // 0x1017 生产者心跳间隔
    OD_ID_COMMUNICATION_TIMEOUT,                                // 0x2000 通信超时
    OD_ID_FEEDBACK_POSITION_CLEAR,                              // 0x2005 反馈位置清零
    OD_ID_SET_ABSOLUTE_POSITION_ORIGIN,                         // 0x2006 设置绝对位置原点
    OD_ID_POWER_ON_LOCK_AXIS_MODE,                              // 0x2007 上电锁轴模式
    OD_ID_MOTOR_MAX_SPEED,                                      // 0x2008 电机最大速度
    OD_ID_REGISTER_PARAMETER_SETTING,                           // 0x2009 寄存器参数设置
    OD_ID_CAN_CUSTOM_DRIVER_NODE_ID,                            // 0x200A CAN自定义驱动器节点ID
    OD_ID_CAN_CUSTOM_COMMUNICATION_BAUD_RATE,                   // 0x200B CAN自定义通信波特率
    OD_ID_MOTOR_POLE_PAIRS_LEFT,                                // 0x200C 电机极对数左
    OD_ID_MOTOR_POLE_PAIRS_RIGHT,                               // 0x200C 电机极对数右
    OD_ID_START_SPEED_LEFT,                                     // 0x200D 启动速度左
    OD_ID_START_SPEED_RIGHT,                                    // 0x200D 启动速度右
    OD_ID_ENCODER_LINE_NUMBER_SETTING_LEFT,                     // 0x200E 编码器线数设置左
    OD_ID_ENCODER_LINE_NUMBER_SETTING_RIGHT,                    // 0x200E 编码器线数设置右
    OD_ID_SYNCHRONOUS_ASYNCHRONOUS_CONTROL_FLAG,                // 0x200F 同步/异步控制标志
    OD_ID_WHETHER_TO_SAVE_PARAMETERS_TO_EEPROM,                 // 0x2010 是否将参数保存到EEPROM
    OD_ID_MOTOR_AND_HALL_OFFSET_ANGLE_LEFT,                     // 0x2011 电机和霍尔偏移角度左
    OD_ID_MOTOR_AND_HALL_OFFSET_ANGLE_RIGHT,                    // 0x2011 电机和霍尔偏移角度右
    OD_ID_OVERLOAD_COEFFICIENT_LEFT,                            // 0x2012 过载系数左
    OD_ID_OVERLOAD_COEFFICIENT_RIGHT,                           // 0x2012 过载系数右
    OD_ID_MOTOR_TEMPERATURE_PROTECTION_THRESHOLD_LEFT,          // 0x2013 电机温度保护阈值左
    OD_ID_MOTOR_TEMPERATURE_PROTECTION_THRESHOLD_RIGHT,         // 0x2013 电机温度保护阈值右
    OD_ID_RATED_CURRENT_LEFT,                                   // 0x2014 额定电流左
    OD_ID_RATED_CURRENT_RIGHT,                                  // 0x2014 额定电流右
    OD_ID_MAX_CURRENT_LEFT,                                     // 0x2015 最大电流左
    OD_ID_MAX_CURRENT_RIGHT,                                    // 0x2015 最大电流右
    OD_ID_OVERLOAD_PROTECTION_TIME_LEFT,                        // 0x2016 过载保护时间左
    OD_ID_OVERLOAD_PROTECTION_TIME_RIGHT,                       // 0x2016 过载保护时间右
    OD_ID_DEVIATION_ALARM_THRESHOLD_LEFT,                       // 0x2017 偏差报警阈值左
    OD_ID_DEVIATION_ALARM_THRESHOLD_RIGHT,                      // 0x2017 偏差报警阈值右
    OD_ID_L_SPEED_SMOOTH_COEFFICIENT,                           // 0x2018 L速度平滑系数
    OD_ID_R_SPEED_SMOOTH_COEFFICIENT,                           // 0x2018 R速度平滑系数
    OD_ID_L_CURRENT_LOOP_PROPORTION_COEFFICIENT,                // 0x2019 L电流环比例系数
    OD_ID_R_CURRENT_LOOP_PROPORTION_COEFFICIENT,                // 0x2019 R电流环比例系数
    OD_ID_L_CURRENT_LOOP_INTEGRAL_GAIN,                         // 0x201A L电流环积分增益
    OD_ID_R_CURRENT_LOOP_INTEGRAL_GAIN,                         // 0x201A R电流环积分增益
    OD_ID_L_FEEDFORWARD_OUTPUT_SMOOTH_COEFFICIENT,              // 0x201B L前馈输出平滑系数
    OD_ID_R_FEEDFORWARD_OUTPUT_SMOOTH_COEFFICIENT,              // 0x201B R前馈输出平滑系数
    OD_ID_L_TORQUE_OUTPUT_SMOOTH_COEFFICIENT,                   // 0x201C L力矩输出平滑系数
    OD_ID_R_TORQUE_OUTPUT_SMOOTH_COEFFICIENT,                   // 0x201C R力矩输出平滑系数
    OD_ID_L_SPEED_PROPORTION_GAIN,                              // 0x201D L速度比例增益
    OD_ID_R_SPEED_PROPORTION_GAIN,                              // 0x201D R速度比例增益
    OD_ID_L_SPEED_INTEGRAL_GAIN_KI,                             // 0x201E L速度积分增益Ki
    OD_ID_R_SPEED_INTEGRAL_GAIN_KI,                             // 0x201E R速度积分增益Ki
    OD_ID_L_SPEED_FEEDFORWARD_GAIN_KF,                          // 0x201F L速度前馈增益Kf
    OD_ID_R_SPEED_FEEDFORWARD_GAIN_KF,                          // 0x201F R速度前馈增益Kf
    OD_ID_L_POSITION_PROPORTION_GAIN_KP,                        // 0x2020 L位置比例增益Kp
    OD_ID_R_POSITION_PROPORTION_GAIN_KP,                        // 0x2020 R位置比例增益Kp
    OD_ID_L_POSITION_FEEDFORWARD_GAIN_KF,                       // 0x2021 L位置前馈增益Kf
    OD_ID_R_POSITION_FEEDFORWARD_GAIN_KF,                       // 0x2021 R位置前馈增益Kf
    OD_ID_L_SPEED_OBSERVER_COEFFICIENT_1,                       // 0x2022 L速度观测器系数1
    OD_ID_R_SPEED_OBSERVER_COEFFICIENT_1,                       // 0x2022 R速度观测器系数1
    OD_ID_L_SPEED_OBSERVER_COEFFICIENT_2,                       // 0x2023 L速度观测器系数2
    OD_ID_R_SPEED_OBSERVER_COEFFICIENT_2,                       // 0x2023 R速度观测器系数2
    OD_ID_L_SPEED_OBSERVER_COEFFICIENT_3,                       // 0x2024 L速度观测器系数3
    OD_ID_R_SPEED_OBSERVER_COEFFICIENT_3,                       // 0x2024 R速度观测器系数3
    OD_ID_L_SPEED_OBSERVER_COEFFICIENT_4,                       // 0x2025 L速度观测器系数4
    OD_ID_R_SPEED_OBSERVER_COEFFICIENT_4,                       // 0x2025 R速度观测器系数4
    OD_ID_ALARM_PWM_PROCESSING_MODE,                            // 0x2026 报警PWM处理模式
    OD_ID_OVERLOAD_PROCESSING_MODE,                             // 0x2027 过载处理模式
    OD_ID_MOTOR_TEMPERATURE_LEFT,                               // 0x2032 电机温度左
    OD_ID_MOTOR_TEMPERATURE_RIGHT,                              // 0x2032 电机温度右
    OD_ID_DRIVER_TEMPERATURE,                                   // 0x2032 驱动器温度
    OD_ID_MOTOR_STATUS_REGISTER_LEFT,                           // 0x2033 电机状态寄存器左
    OD_ID_MOTOR_STATUS_REGISTER_RIGHT,                          // 0x2033 电机状态寄存器右
    OD_ID_HALL_INPUT_STATUS_LEFT,                               // 0x2034 霍尔输入状态左
    OD_ID_HALL_INPUT_STATUS_RIGHT,                              // 0x2034 霍尔输入状态右
    OD_ID_BUS_VOLTAGE,                                          // 0x2035 母线电压
    OD_ID_DRIVER_LAST_FAULT_CODE,                               // 0x603F 驱动器最后一次故障代码
    OD_ID_CONTROL_WORD,                                         // 0x6040 控制字
    OD_ID_STATUS_WORD,                                          // 0x6041 状态字
    OD_ID_QUICK_STOP_CODE,                                      // 0x605A 快速停止代码
    OD_ID_SHUTDOWN_CODE,                                        // 0x605B 关机代码
    OD_ID_DISABLE_CODE,                                         // 0x605C 失能代码
    OD_ID_HALT_CONTROL_REGISTER,                                // 0x605D 停止控制寄存器
    OD_ID_OPERATION_MODE,                                       // 0x6060 运行模式
    OD_ID_OPERATION_MODE_STATUS,                                // 0x6061 运行模式状态
    OD_ID_ACTUAL_POSITION_FEEDBACK_LEFT,                        // 0x6064 实际位置反馈左
    OD_ID_ACTUAL_POSITION_FEEDBACK_RIGHT,                       // 0x6064 实际位置反馈右
    OD_ID_ACTUAL_SPEED_FEEDBACK_LEFT,                           // 0x606C 实际速度反馈左
    OD_ID_ACTUAL_SPEED_FEEDBACK_RIGHT,                          // 0x606C 实际速度反馈右
    OD_ID_ACTUAL_SPEED_FEEDBACK_LEFT_AND_RIGHT,                 // 0x606C 实际速度反馈左和右
    OD_ID_TARGET_TORQUE_LEFT,                                   // 0x6071 目标力矩左
    OD_ID_TARGET_TORQUE_RIGHT,                                  // 0x6071 目标力矩右
    OD_ID_TARGET_TORQUE_LEFT_AND_RIGHT,                         // 0x6071 目标力矩左和右
    OD_ID_REAL_TIME_TORQUE_FEEDBACK_LEFT,                       // 0x6077 实时力矩反馈左
    OD_ID_REAL_TIME_TORQUE_FEEDBACK_RIGHT,                      // 0x6077 实时力矩反馈右
    OD_ID_REAL_TIME_TORQUE_FEEDBACK_LEFT_AND_RIGHT,             // 0x6077 实时力矩反馈左和右
    OD_ID_TARGET_POSITION_LEFT,                                 // 0x607A 目标位置左
    OD_ID_TARGET_POSITION_RIGHT,                                // 0x607A 目标位置右
    OD_ID_MAX_SPEED_LEFT,                                       // 0x6081 最大速度左
    OD_ID_MAX_SPEED_RIGHT,                                      // 0x6081 最大速度右
    OD_ID_POSITION_MODE_START_STOP_SPEED_LEFT,                  // 0x6082 位置模式启停速度左
    OD_ID_POSITION_MODE_START_STOP_SPEED_RIGHT,                 // 0x6082 位置模式启停速度右
    OD_ID_S_SHAPE_ACCELERATION_TIME_LEFT,                       // 0x6083 S形加速时间左
    OD_ID_S_SHAPE_ACCELERATION_TIME_RIGHT,                      // 0x6083 S形加速时间右
    OD_ID_S_SHAPE_DECELERATION_TIME_LEFT,                       // 0x6084 S形减速时间左
    OD_ID_S_SHAPE_DECELERATION_TIME_RIGHT,                      // 0x6084 S形减速时间右
    OD_ID_EMERGENCY_STOP_DECELERATION_TIME_LEFT,                // 0x6085 急停减速时间左
    OD_ID_EMERGENCY_STOP_DECELERATION_TIME_RIGHT,               // 0x6085 急停减速时间右
    OD_ID_TORQUE_SLOPE_LEFT,                                    // 0x6087 力矩斜率左
    OD_ID_TORQUE_SLOPE_RIGHT,                                   // 0x6087 力矩斜率右
    OD_ID_TARGET_SPEED_LEFT,                                    // 0x60FF 目标速度左
    OD_ID_TARGET_SPEED_RIGHT,                                   // 0x60FF 目标速度右
    OD_ID_TARGET_SPEED_LEFT_RIGHT,                              // 0x60FF 目标速度左+右
    OD_ID_MAX,
};

enum
{
    TPOD_ID_ACTUAL_SPEED_FEEDBACK_LEFT_AND_RIGHT,
    TPOD_ID_DRIVER_LAST_FAULT_CODE,    
    TPOD_ID_ACTUAL_POSITION_FEEDBACK_LEFT,
    TPOD_ID_ACTUAL_POSITION_FEEDBACK_RIGHT,
    TPOD_ID_CONTROL_WORD,
};

enum
{
    RPOD_ID_TARGET_SPEED_LEFT_RIGHT,
    RPOD_ID_S_SHAPE_ACCELERATION_TIME_LEFT,
    RPOD_ID_S_SHAPE_ACCELERATION_TIME_RIGHT,
    RPOD_ID_S_SHAPE_DECELERATION_TIME_LEFT,
    RPOD_ID_S_SHAPE_DECELERATION_TIME_RIGHT,
    RPOD_ID_CONTROL_WORD,
};

#define RPDO_ITEMS_NUM 6
#define RPDO_CFG_NUM 4

#define TPDO_ITEMS_NUM 5
#define TPDO_CFG_NUM 4

extern struct micro_canopen_od_items od_items[];

extern struct micro_canopen_od_pdo_itmes_t tpdo_items[];
extern struct micro_canopen_pdo_cfg_t tpdo_cfg[];

extern struct micro_canopen_od_pdo_itmes_t rpdo_items[];
extern struct micro_canopen_pdo_cfg_t rpdo_cfg[];

#endif /* APPLICATIONS_MICRO_CANOPEN_MICRO_CANOPEN_OD_H_ */
