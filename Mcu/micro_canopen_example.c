/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2023-09-06     koala.zhoubo       the first version
 */

#include "micro_canopen.h"
#include "micro_canopen_od.h"
#include <rtthread.h>
#include <rtdevice.h>

#define USE_MICRO_CANOPEN_EXAMPLE
#ifdef USE_MICRO_CANOPEN_EXAMPLE
// rt thread 使用例程
struct micro_canopen_obj_t canopen_obj;

uint8_t user_can_send(uint16_t id, uint8_t *data, uint8_t len);
uint8_t user_can_recv(uint16_t *id, uint8_t *data, uint8_t *len);
int can_sample();

// 写入速度 加速度 减速度
void canopen_write_speed(int32_t left_speed, int32_t right_speed, int32_t left_acc, int32_t right_acc, int32_t left_dec, int32_t right_dec)
{
    // 通过写rpdo的方式写入速度
    micro_canopen_rpdo_wrie_value(&canopen_obj, RPOD_ID_TARGET_SPEED_LEFT_RIGHT, (uint32_t)(right_speed << 16 | left_speed));
    micro_canopen_rpdo_wrie_value(&canopen_obj, RPOD_ID_S_SHAPE_ACCELERATION_TIME_LEFT, (uint32_t)(left_acc));
    micro_canopen_rpdo_wrie_value(&canopen_obj, RPOD_ID_S_SHAPE_ACCELERATION_TIME_RIGHT, (uint32_t)(left_acc));
    micro_canopen_rpdo_wrie_value(&canopen_obj, RPOD_ID_S_SHAPE_DECELERATION_TIME_LEFT, (uint32_t)(left_dec));
    micro_canopen_rpdo_wrie_value(&canopen_obj, RPOD_ID_S_SHAPE_DECELERATION_TIME_RIGHT, (uint32_t)(left_dec));
};
void canopen_read_pdo_test()
{
    uint32_t read_val;

    // 通过读取rpdo的方式读取速度

    micro_canopen_tpdo_read_value(&canopen_obj, TPOD_ID_ACTUAL_SPEED_FEEDBACK_LEFT_AND_RIGHT, &read_val);

    rt_kprintf("TPOD_ID_ACTUAL_SPEED_FEEDBACK_LEFT_AND_RIGHT  l:%ld   r:%ld\n", (int16_t)(read_val & 0xffff), (int16_t)(read_val >> 16));

    micro_canopen_tpdo_read_value(&canopen_obj, TPOD_ID_DRIVER_LAST_FAULT_CODE, &read_val);
    rt_kprintf("TPOD_ID_DRIVER_LAST_FAULT_CODE: %ld\n", read_val);

    micro_canopen_tpdo_read_value(&canopen_obj, TPOD_ID_ACTUAL_POSITION_FEEDBACK_LEFT, &read_val);
    rt_kprintf("TPOD_ID_ACTUAL_POSITION_FEEDBACK_LEFT: %ld\n", read_val);

    micro_canopen_tpdo_read_value(&canopen_obj, TPOD_ID_ACTUAL_POSITION_FEEDBACK_RIGHT, &read_val);
    rt_kprintf("TPOD_ID_ACTUAL_POSITION_FEEDBACK_RIGHT: %ld\n", read_val);

    micro_canopen_tpdo_read_value(&canopen_obj, TPOD_ID_CONTROL_WORD, &read_val);
    rt_kprintf("TPOD_ID_CONTROL_WORD: %ld\n", read_val);
};

void micro_canopen_dev_init()
{
    canopen_obj.node_id = 0x01;           // 节点ID
    canopen_obj.bandrate = 500;           // 500k
    canopen_obj.main_period_ms = 1;       // 1ms
    canopen_obj.sync_time = 500;          // 500ms 同步一次
    canopen_obj.heartbeat_timeout = 200; // 1s 心跳超时时间

    // CAN 回调
    canopen_obj.can_send = user_can_send;

    // 初始化canopen
    micro_canopen_init(&canopen_obj);

    // 预操作
    micro_canopen_nmt(&canopen_obj, NMT_PREOP, 0);

    // TPDO RPDO 初始化
    micro_canopen_pdo_init(&canopen_obj, PDO_TYPE_TPDO);
    micro_canopen_pdo_init(&canopen_obj, PDO_TYPE_RPDO);

    // 电机模式操作
    micro_canopen_od_wrtie_value(&canopen_obj, OD_ID_OPERATION_MODE, 0x03); // 速度模式

    // 异常清除
    micro_canopen_od_wrtie_value(&canopen_obj, OD_ID_CONTROL_WORD, 0x80); // 清除故障

    // 反馈位置清零
    micro_canopen_od_wrtie_value(&canopen_obj, OD_ID_FEEDBACK_POSITION_CLEAR, 3); // 反馈位置清零    

    // 6040 控制字操作  00->06->07->0F
    micro_canopen_od_wrtie_value(&canopen_obj, OD_ID_CONTROL_WORD, 0x00);
    micro_canopen_od_wrtie_value(&canopen_obj, OD_ID_CONTROL_WORD, 0x06);
    micro_canopen_od_wrtie_value(&canopen_obj, OD_ID_CONTROL_WORD, 0x07);
    micro_canopen_od_wrtie_value(&canopen_obj, OD_ID_CONTROL_WORD, 0x0F);

    // 设置同步模式
    micro_canopen_od_wrtie_value(&canopen_obj, OD_ID_SYNCHRONOUS_ASYNCHRONOUS_CONTROL_FLAG, 0x01); // 同步模式

    // 设置心跳时间 0.5ms 单位
    micro_canopen_od_wrtie_value(&canopen_obj, OD_ID_PRODUCER_HEARTBEAT_TIME, 200); // 100ms 心跳时间

    // 通讯断开保护时间
    micro_canopen_od_wrtie_value(&canopen_obj, OD_ID_COMMUNICATION_TIMEOUT, 200); // 200ms 保护

    

    // 节点激活 同  micro_canopen_actvate_node(&canopen_obj);
    micro_canopen_nmt(&canopen_obj, NMT_START, 0);
};

void micro_canopen_thread_entry(void *parameter)
{
    // 先初始化CAN 设备
    can_sample();

    // 初始化canopen
    micro_canopen_dev_init();

    while (1)
    {
        micro_canopen_process(&canopen_obj);
        rt_thread_mdelay(canopen_obj.main_period_ms);
    }
};

void micro_canopen_thread_init()
{
    rt_thread_t tid;
    tid = rt_thread_create("m_can", micro_canopen_thread_entry, RT_NULL, 4096, 25, 10);
    if (tid != RT_NULL)
    {
        rt_thread_startup(tid);
    }
};
MSH_CMD_EXPORT(micro_canopen_thread_init, micro canopen thread init);

// Canopen 字符串类型index 转 整形 ,例如 "1A00"  ->  6656
uint16_t canopen_str_index_to_int(uint8_t *str)
{
    uint16_t index = 0;
    // 字符串16进制 转 整数
    if(strlen(str) == 4)
    {
        for(int i =0;i <4;i++)
        {
            // 字符串转16进制
            if(str[i] >= '0' && str[i] <= '9')
            {
                index += (str[i] - '0') << (4*(3-i));
            }
            else if(str[i] >= 'A' && str[i] <= 'F')
            {
                index += (str[i] - 'A' + 10) << (4*(3-i));
            }
            else if(str[i] >= 'a' && str[i] <= 'f')
            {
                index += (str[i] - 'a' + 10) << (4*(3-i));
            }
        }       
       
    }
    return index;
}


void canopen_read_index(int argc, char *argv[])
{   
    uint16_t index = 0;
    uint8_t od_id = 0;
    
    if (argc < 2)
    {
        rt_kprintf("Usage: canopen_read_index 1A00\n");
        return;
    }
    else
    {
        index = canopen_str_index_to_int(argv[1]);

        od_id = micro_canopen_get_od_id_by_index(&canopen_obj, index);

        if(od_id >= OD_ID_MAX) return;
              
        micro_canopen_update_dict(&canopen_obj, od_id);

        rt_thread_mdelay(50);

        // 打印字典
        // 注释 index subindex value 
        //rt_kprintf("name : %s index: %d  subindex: %d  value: %ld\n", canopen_obj.od_items[od_id].name, index, canopen_obj.od_items[od_id].subindex, canopen_obj.od_items[od_id].value);
        micro_canopen_print_od(&canopen_obj, od_id);

    }

}
MSH_CMD_EXPORT(canopen_read_index, micro canopen read index);

void canopen_write_index(int argc, char *argv[])
{
    uint16_t index = 0;
    uint32_t value = 0;
    uint8_t od_id = 0;
    if (argc < 3)
    {
        rt_kprintf("Usage: canopen_write_index 1A00 1234\n");
        return;
    }
    else
    {
        index = canopen_str_index_to_int(argv[1]);
        value = atoi(argv[2]);
        od_id = micro_canopen_get_od_id_by_index(&canopen_obj, index);
        if(od_id >= OD_ID_MAX) return;
        micro_canopen_od_wrtie_value(&canopen_obj, od_id, value);
    }
};
MSH_CMD_EXPORT(canopen_write_index, micro canopen write index);

void canopen_read_speed(int argc, char *argv[])
{
    canopen_read_pdo_test();
}
MSH_CMD_EXPORT(canopen_read_speed, micro canopen read speed);
void canopen_set_speed(int argc, char *argv[])
{
    int32_t left_speed, right_speed;
    int32_t left_acc;
    int32_t left_dec;
    if (argc < 2)
    {
        rt_kprintf("Usage: micro_canopen_example speed [left_speed] [right_speed]\n");
        return;
    }
    else
    {

        left_speed = atoi(argv[1]);
        right_speed = atoi(argv[2]);
        // left_dec = atoi(argv[4]);
        left_acc = 100;
        left_dec = 100;

        canopen_write_speed(left_speed, right_speed, left_acc, left_acc, left_dec, left_dec);
    }
};
MSH_CMD_EXPORT(canopen_set_speed, micro canopen example);
#endif

/* ----------------------------------------------- CAN 部分 -----------------------------------------------------*/

/*
 * 程序清单：这是一个 CAN 设备使用例程
 * 例程导出了 can_sample 命令到控制终端
 * 命令调用格式：can_sample can1
 * 命令解释：命令第二个参数是要使用的 CAN 设备名称，为空则使用默认的 CAN 设备
 * 程序功能：通过 CAN 设备发送一帧，并创建一个线程接收数据然后打印输出。
 */
#define CAN_DEV_NAME "can1" /* CAN 设备名称 */

static struct rt_semaphore rx_sem; /* 用于接收消息的信号量 */
static rt_device_t can_dev;        /* CAN 设备句柄 */

/* 接收数据回调函数 */
static rt_err_t can_rx_call(rt_device_t dev, rt_size_t size)
{
    /* CAN 接收到数据后产生中断，调用此回调函数，然后发送接收信号量 */
    rt_sem_release(&rx_sem);

    return RT_EOK;
}

static void can_rx_thread(void *parameter)
{
    int i;
    rt_err_t res;
    struct rt_can_msg rxmsg = {0};

    /* 设置接收回调函数 */
    rt_device_set_rx_indicate(can_dev, can_rx_call);

    struct can_fifo_obj_t recv_data;

    while (1)
    {
        /* hdr 值为 - 1，表示直接从 uselist 链表读取数据 */
        rxmsg.hdr = -1;
        /* 阻塞等待接收信号量 */
        rt_sem_take(&rx_sem, RT_WAITING_FOREVER);
        /* 从 CAN 读取一帧数据 */
        rt_device_read(can_dev, 0, &rxmsg, sizeof(rxmsg));
        /* 打印数据 ID 及内容 */
        // rt_kprintf("ID:%x", rxmsg.id);
        // for (i = 0; i < 8; i++)
        // {
        //     rt_kprintf("%2x ", rxmsg.data[i]);
        // }

        // rt_kprintf("\n");
        micro_canopen_can_data_push(&canopen_obj, rxmsg.id, rxmsg.data, rxmsg.len);
    }
}

int can_sample()
{
    // rt_pin_mode(GET_PIN(B, 0) ,PIN_MODE_OUTPUT);
    // rt_pin_write(GET_PIN(B, 0) ,PIN_LOW);

    struct rt_can_msg msg = {0};
    rt_err_t res;
    rt_size_t size;
    rt_thread_t thread;
    char can_name[RT_NAME_MAX];

    rt_strncpy(can_name, CAN_DEV_NAME, RT_NAME_MAX);

    /* 查找 CAN 设备 */
    can_dev = rt_device_find(can_name);
    if (!can_dev)
    {
        rt_kprintf("find %s failed!\n", can_name);
        return RT_ERROR;
    }

    /* 初始化 CAN 接收信号量 */
    rt_sem_init(&rx_sem, "rx_sem", 0, RT_IPC_FLAG_FIFO);

    /* 以中断接收及发送方式打开 CAN 设备 */
    res = rt_device_open(can_dev, RT_DEVICE_FLAG_INT_TX | RT_DEVICE_FLAG_INT_RX);

    res = rt_device_control(can_dev, RT_CAN_CMD_SET_BAUD, (void *)CAN500kBaud);

    RT_ASSERT(res == RT_EOK);
    /* 创建数据接收线程 */
    thread = rt_thread_create("can_rx", can_rx_thread, RT_NULL, 1024, 25, 10);
    if (thread != RT_NULL)
    {
        rt_thread_startup(thread);
    }
    else
    {
        rt_kprintf("create can_rx thread failed!\n");
    }
};

void can_sample_send()
{
    rt_size_t size;

    struct rt_can_msg msg = {0};
    msg.id = 0x78;          /* ID 为 0x78 */
    msg.ide = RT_CAN_STDID; /* 标准格式 */
    msg.rtr = RT_CAN_DTR;   /* 数据帧 */
    msg.len = 8;            /* 数据长度为 8 */
    /* 待发送的 8 字节数据 */
    msg.data[0] = 0x00;
    msg.data[1] = 0x11;
    msg.data[2] = 0x22;
    msg.data[3] = 0x33;
    msg.data[4] = 0x44;
    msg.data[5] = 0x55;
    msg.data[6] = 0x66;
    msg.data[7] = 0x77;
    /* 发送一帧 CAN 数据 */
    size = rt_device_write(can_dev, 0, &msg, sizeof(msg));
    if (size == 0)
    {
        rt_kprintf("can dev write data failed!\n");
    }
}

uint8_t user_can_send(uint16_t id, uint8_t *data, uint8_t len)
{
    rt_size_t size;
    struct rt_can_msg msg = {0};

    if (can_dev == RT_NULL)
    {
        return 0;
    }

    msg.id = id;
    msg.ide = RT_CAN_STDID; /* 标准格式 */
    msg.rtr = RT_CAN_DTR;   /* 数据帧 */
    msg.len = len;          /* 数据长度为 8 */

    rt_memcpy(msg.data, data, len);

    size = rt_device_write(can_dev, 0, &msg, sizeof(msg));
    if (size == 0)
    {
        rt_kprintf("can dev write data failed!\n");
        return 0;
    }
};