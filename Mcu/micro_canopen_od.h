/*******************************************************************************
    CANopen Object Dictionary definition for CANopenNode V5

    This file was automatically generated by CANopenEditor v4.1-0-gb76e55f-dirty

    https://github.com/CANopenNode/CANopenNode
    https://github.com/CANopenNode/CANopenEditor

    DON'T EDIT THIS FILE MANUALLY !!!!
********************************************************************************

    File info:
        File Names:   micro_canopen_od.h; micro_canopen_od.c
        Project File: ZLAC8015D(1).xdd
        File Version: 0x01

        Created:      2021/1/25 13:35:00
        Created By:   LHY
        Modified:     2023/11/14 17:30:23
        Modified By:  LHY

    Device Info:
        Vendor Name:  CANopen
        Vendor ID:    0x00000100
        Product Name: ZLAC8015
        Product ID:   0x00000001

        Description:  1
*******************************************************************************/

#ifndef micro_canopen_od_H
#define micro_canopen_od_H

#include "micro_canopen.h"
enum 
{

    OD_ID_DEVICE_TYPE,                                                     //0x1000  设备类型
    OD_ID_ERROR_REGISTER,                                                  //0x1001  错误寄存器
    OD_ID_SYNC_COB_ID,                                                     //0x1005  同步COB-ID
    OD_ID_MANUFACTURER_HARDWARE_VERSION,                                   //0x1009  制造商硬件版本
    OD_ID_MANUFACTURER_SOFTWARE_VERSION,                                   //0x100A  制造商软件版本
    OD_ID_EMERGENCY_COB_ID,                                                //0x1014  紧急COB ID
    OD_ID_PRODUCER_HEARTBEAT_TIME,                                         //0x1017  生产者心跳时间
    OD_ID_VENDOR_ID,                                                       //0x1018  供应商Id
    OD_ID_PRODUCT_CODE,                                                    //0x1018  产品代码
    OD_ID_COB_ID_CLIENT_TO_SERVER,                                         //0x1200  COB ID客户端到服务器
    OD_ID_COB_ID_SERVER_TO_CLIENT,                                         //0x1200  COB ID服务器到客户端
    OD_ID_OFFLINE_COMMUNICATION_PARAMETER,                                 //0x2000  通讯掉线保护时间
    OD_ID_RS485_NODE_ID,                                                   //0x2001  RS485节点ID
    OD_ID_RS485_BAUDRATE,                                                  //0x2002  RS485波特率
    OD_ID_INPUT_SIGNAL_STATE,                                              //0x2003  输入信号状态
    OD_ID_OUTPUT_SIGNAL_STATE,                                             //0x2004  输出信号状态
    OD_ID_CLEAR_FEEDBACK_POSITION,                                         //0x2005  反馈位置清零
    OD_ID_SET_ORIGIN,                                                      //0x2006  设置绝对位置原点
    OD_ID_DRIVER_INITIAL_STATE,                                            //0x2007  上电锁轴方式
    OD_ID_MOTOR_MAX_SPEED,                                                 //0x2008  电机最大转速
    OD_ID_DRIVER_PARAMETER_CONFIG,                                         //0x2009  寄存器参数设置
    OD_ID_CAN_NODE_ID,                                                     //0x200A  CAN节点ID
    OD_ID_CAN_BAUDRATE,                                                    //0x200B  CAN波特率
    OD_ID_LEFT_MOTOR_POLE,                                                 //0x200C  电机极对数(左)
    OD_ID_RIGHT_MOTOR_POLE,                                                //0x200C  电机极对数(右)
    OD_ID_LEFT_MOTOR_INIT_SPEED,                                           //0x200D  起始速度(左)
    OD_ID_RIGHT_MOTOR_INIT_SPEED,                                          //0x200D  起始速度(右)
    OD_ID_LEFT_MOTOR_ENCODER_LINE,                                         //0x200E  编码器线束设置(左)
    OD_ID_RIGHT_MOTOR_ENCODER_LINE,                                        //0x200E  编码器线束设置(右)
    OD_ID_SYN_CONTROL,                                                     //0x200F  同步/异步控制标志位
    OD_ID_EEPROM_CONFIG,                                                   //0x2010  EEPROM配置
    OD_ID_LEFT_MOTOR_HALL_OFFSET_ANGLE,                                    //0x2011  电机与 Hall 的偏移角度(左)
    OD_ID_RIGHT_MOTOR_HALL_OFFSET_ANGLE,                                   //0x2011  电机与 Hall 的偏移角度(右)
    OD_ID_LEFT_MOTOR_OVERLOAD_FACTOR,                                      //0x2012  过载系数(左)
    OD_ID_RIGHT_MOTOR_OVERLOAD_FACTOR,                                     //0x2012  过载系数(右)
    OD_ID_LEFT_MOTOR_OVERHEAT_THRESHOLD,                                   //0x2013  电机温度保护阈值(左)
    OD_ID_RIGHT_MOTOR_OVERHEAT_THRESHOLD,                                  //0x2013  电机温度保护阈值(右)
    OD_ID_DRIVE_MOTOR_OVERHEAT_THRESHOLD,                                  //0x2013  驱动器温度保护阈值
    OD_ID_LEFT_MOTOR_RATED_CURRENT,                                        //0x2014  额定电流(左)
    OD_ID_RIGHT_MOTOR_RATED_CURRENT,                                       //0x2014  额定电流(右)
    OD_ID_LEFT_MOTOR_MAX_CURRENT,                                          //0x2015  最大电流(左)
    OD_ID_RIGHT_MOTOR_MAX_CURRENT,                                         //0x2015  最大电流(右)
    OD_ID_LEFT_MOTOR_OVERLOAD_PROTECTION_TIME,                             //0x2016  过载保护时间(左)
    OD_ID_RIGHT_MOTOR_OVERLOAD_PROTECTION_TIME,                            //0x2016  过载保护时间(右)
    OD_ID_LEFT_MOTOR_OVERPOSITION_THRESHOLD,                               //0x2017  超差报警阈值(左)
    OD_ID_RIGHT_MOTOR_OVERPOSITION_THRESHOLD,                              //0x2017  超差报警阈值(右)
    OD_ID_LEFT_MOTOR_VL_SMOOTH_FACTOR,                                     //0x2018  左电机VL平滑系数
    OD_ID_RIGHT_MOTOR_VL_SMOOTH_FACTOR,                                    //0x2018  右电机VL平滑系数
    OD_ID_LEFT_MOTOR_CP,                                                   //0x2019  左电机Cp
    OD_ID_RIGHT_MOTOR_CP,                                                  //0x2019  右电机Cp
    OD_ID_LEFT_MOTOR_CI,                                                   //0x201A  左电机Ci
    OD_ID_RIGHT_MOTOR_CI,                                                  //0x201A  右电机Ci
    OD_ID_LEFT_MOTOR_FF_SMOOTH_FACTOR,                                     //0x201B  左电机FF平滑系数
    OD_ID_RIGHT_MOTOR_FF_SMOOTH_FACTOR,                                    //0x201B  右电机FF平滑系数
    OD_ID_LEFT_MOTOR_TORQUE_SMOOTH_FACTOR,                                 //0x201C  左电机扭矩平滑系数
    OD_ID_RIGHT_MOTOR_TORQUE_SMOOTH_FACTOR,                                //0x201C  右电机扭矩平滑系数
    OD_ID_LEFT_MOTOR_VP,                                                   //0x201D  左电机Vp
    OD_ID_RIGHT_MOTOR_VP,                                                  //0x201D  右电机Vp
    OD_ID_LEFT_MOTOR_VI,                                                   //0x201E  左电机Vi
    OD_ID_RIGHT_MOTOR_VI,                                                  //0x201E  右电机Vi
    OD_ID_LEFT_MOTOR_VF,                                                   //0x201F  左电机Vf
    OD_ID_RIGHT_MOTOR_VF,                                                  //0x201F  右电机Vf
    OD_ID_LEFT_MOTOR_PP,                                                   //0x2020  左电机Pp
    OD_ID_RIGHT_MOTOR_PP,                                                  //0x2020  右电机Pp
    OD_ID_LEFT_MOTOR_PF,                                                   //0x2021  左电机Pf
    OD_ID_RIGHT_MOTOR_PF,                                                  //0x2021  右电机Pf
    OD_ID_LEFT_MOTOR_VELOCITY_OBSERVER_1,                                  //0x2022  左电机速度观测器1
    OD_ID_RIGHT_MOTOR_VELOCITY_OBSERVER_1,                                 //0x2022  右马达速度观测器1
    OD_ID_LEFT_MOTOR_VELOCITY_OBSERVER_2,                                  //0x2023  左马达速度观测器2
    OD_ID_RIGHT_MOTOR_VELOCITY_OBSERVER_2,                                 //0x2023  右马达速度观测器2
    OD_ID_LEFT_MOTOR_VELOCITY_OBSERVER_3,                                  //0x2024  左电机速度观测器3
    OD_ID_RIGHT_MOTOR_VELOCITY_OBSERVER_3,                                 //0x2024  右电机速度观测器3
    OD_ID_LEFT_MOTOR_VELOCITY_OBSERVER_4,                                  //0x2025  左电机速度观测器4
    OD_ID_RIGHT_MOTOR_VELOCITY_OBSERVER_4,                                 //0x2025  右电机速度观测器4
    OD_ID_ALARM_PWM_PROCESSING_METHOD,                                     //0x2026  报警 PWM 处理方式
    OD_ID_OVERLOAD_HANDLING_METHOD,                                        //0x2026  过载处理方式
    OD_ID_IO_EMERGENCY_STOP_PROCESSING_METHOD,                             //0x2026  I/O 急停处理方式
    OD_ID_PARKING_MODE,                                                    //0x2026  驻车模式
    OD_ID_GIVEN_SPEED_RESOLUTION,                                          //0x2026  给定速度分辨率
    OD_ID_INPUT_LEVLE_CONFIG,                                              //0x2030  输入Level配置
    OD_ID_X0_CONFIG,                                                       //0x2030  X0配置
    OD_ID_X1_CONFIG,                                                       //0x2030  X1配置
    OD_ID_OUTPUT_LEVEL_CONFIG,                                             //0x2030  输出电平配置
    OD_ID_Y0_CONFIG,                                                       //0x2030  Y0配置
    OD_ID_Y1_CONFIG,                                                       //0x2030  Y1配置
    OD_ID_BO_CONFIG,                                                       //0x2030  Bo配置
    OD_ID_B1_CONFIG,                                                       //0x2030  B1配置
    OD_ID_SOFTWARE_VERSION,                                                //0x2031  软件版本
    OD_ID_LEFT_MOTOR_TEMPERATURE,                                          //0x2032  电机温度(左)
    OD_ID_RIGHT_MOTOR_TEMPERATURE,                                         //0x2032  电机温度(右)
    OD_ID_DRIVE_TEMPERATURE,                                               //0x2032  驱动器温度
    OD_ID_LEFT_MOTOR_STATE,                                                //0x2033  电机状态寄存器(左)
    OD_ID_RIGHT__MOTOR_STATE,                                              //0x2033  右电机状态
    OD_ID_LEFT_MOTOR_HALL_STATE,                                           //0x2034  霍尔输入状态(左)
    OD_ID_RIGHT_MOTOR_HALL_STATE,                                          //0x2034  霍尔输入状态(右)
    OD_ID_BUS_VOLTAGE,                                                     //0x2035  母线电压
    OD_ID_ERROR_CODE,                                                      //0x603F  驱动器最近一次故障码
    OD_ID_CONTROLWORD,                                                     //0x6040  控制字
    OD_ID_STATUSWORD,                                                      //0x6041  状态字
    OD_ID_QUICK_STOP_OPTION_CODE,                                          //0x605A  快速停止代码
    OD_ID_SHUTDOWN_OPTION_CODE,                                            //0x605B  关闭操作代码
    OD_ID_DISABLE_OPERATION_OPTION_CODE,                                   //0x605C  禁用操作代码
    OD_ID_HALT_OPTION_CODE,                                                //0x605D  Halt 控制寄存器
    OD_ID_MODES_OF_OPERATION,                                              //0x6060  运行模式
    OD_ID_MODE_OF_OPERATION_DISPLAY,                                       //0x6061  运行模式状态
    OD_ID_LEFT_MOTOR_POSITION_ACTUAL_VALUE,                                //0x6064  实际位置反馈(左)
    OD_ID_RIGHT_MOTOR_POSITION_ACTUAL_VALUE,                               //0x6064  实际位置反馈(右)
    OD_ID_LEFT_MOTOR_VELOCITY_ACTUAL_VALUE,                                //0x606C  实际速度反馈(左)
    OD_ID_RIGHT_MOTOR_VELOCITY_ACTUAL_VALUE,                               //0x606C  实际速度反馈(右)
    OD_ID_VELOCITY_ACTUAL_SUB3,                                            //0x606C  左右实际速度组合
    OD_ID_LEFT_MOTOR_TARGET_TORQUE,                                        //0x6071  目标转矩(左)
    OD_ID_RIGHT_MOTOR_TARGET_TORQUE,                                       //0x6071  目标转矩(右)
    OD_ID_TARGET_TORQUE_SUB3,                                              //0x6071  左右目标转矩组合
    OD_ID_LEFT_MOTOR_TORQUE_ACTUAL_VALUE,                                  //0x6077  实时转矩反馈(左)
    OD_ID_RIGHT_MOTOR_TORQUE_ACTUAL_VALUE,                                 //0x6077  实时转矩反馈(右)
    OD_ID_TORQUE_ACTUAL_VALUE_SUB3,                                        //0x6077  左右实时转矩组合
    OD_ID_LEFT_MOTOR_TARGET_POSITION,                                      //0x607A  目标位置(左)
    OD_ID_RIGHT_MOTOR_TARGET_POSITION,                                     //0x607A  目标位置(右)
    OD_ID_LEFT_MOTOR_PROFILE_VELOCITY,                                     //0x6081  最大速度(左）
    OD_ID_RIGHT_MOTOR_PROFILE_VELOCITY,                                    //0x6081  最大速度(右）
    OD_ID_LEFT_MOTOR_END_VELOCITY,                                         //0x6082  位置模式启/停速度(左)
    OD_ID_RIGHT_MOTOR_END_VELOCITY,                                        //0x6082  位置模式启/停速度(右)
    OD_ID_LEFT_MOTOR_PROFILE_ACCELERATION,                                 //0x6083  S 形减速时间(左)
    OD_ID_RIGHT_MOTOR_PROFILE_ACCELERATION,                                //0x6083  S 形减速时间(右)
    OD_ID_LEFT_MOTOR_PROFILE_DECELERATION,                                 //0x6084  S 形减速时间(左)
    OD_ID_RIGHT_MOTOR_PROFILE_DECELERATION,                                //0x6084  S 形减速时间(右)
    OD_ID_LEFT_MOTOR_QUICK_STOP_DECELERATION,                              //0x6085  急停减速时间(左)
    OD_ID_RIGHT_MOTOR_QUICK_STOP_DECELERATION,                             //0x6085  急停减速时间(右)
    OD_ID_LEFT_MOTOR_TORQUE_SLOPE,                                         //0x6087  转矩斜率(左)
    OD_ID_RIGHT_MOTOR_TORQUE_SLOPE,                                        //0x6087  转矩斜率(右)
    OD_ID_LEFT_MOTOR_TARGET_VELOCITY,                                      //0x60FF  目标速度(左)
    OD_ID_RIGHT_MOTOR_TARGET_VELOCITY,                                     //0x60FF  目标速度(右)
    OD_ID_TARGET_VELOCITY_SUB3,                                            //0x60FF  左右电机目标速度
    OD_ID_MAX

};
/*--------------------------------------------- RPOD_ID -----------------------------------*/
enum{
    RPDO_ID_TARGET_VELOCITY_SUB3,
    RPDO_ID_LEFT_MOTOR_PROFILE_ACCELERATION,
    RPDO_ID_RIGHT_MOTOR_PROFILE_ACCELERATION,
    RPDO_ID_LEFT_MOTOR_PROFILE_DECELERATION,
    RPDO_ID_RIGHT_MOTOR_PROFILE_DECELERATION,
    RPDO_ID_CONTROLWORD,
};
/*--------------------------------------------- TPOD_ID -----------------------------------*/
enum{
    TPDO_ID_VELOCITY_ACTUAL_SUB3,
    TPDO_ID_ERROR_CODE,
    TPDO_ID_LEFT_MOTOR_POSITION_ACTUAL_VALUE,
    TPDO_ID_RIGHT_MOTOR_POSITION_ACTUAL_VALUE,
    TPDO_ID_CONTROLWORD,
    TPDO_ID_STATUSWORD,
};
#define RPDO_ITEMS_NUM  6
#define RPDO_CFG_NUM 4
#define TPDO_ITEMS_NUM  6
#define TPDO_CFG_NUM 4

extern struct micro_canopen_od_items od_items[];

extern struct micro_canopen_od_pdo_itmes_t tpdo_items[];
extern struct micro_canopen_pdo_cfg_t tpdo_cfg[];

extern struct micro_canopen_od_pdo_itmes_t rpdo_items[];
extern struct micro_canopen_pdo_cfg_t rpdo_cfg[];

#endif

