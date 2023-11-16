/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2023-09-05     koala.zhoubo       the first version
 */

#include "micro_canopen.h"
#include "micro_canopen_od.h"

#include <rtthread.h>

/*  ----------------------------------- 函数预定义 ----------------------------------- */

void micro_canopen_pack_sdo_data(struct micro_canopen_obj_t *obj, uint16_t index, uint8_t subindex, uint8_t val_type, int32_t val);
void micro_canopen_tpdo_write_to_can(struct micro_canopen_obj_t *obj);
uint8_t micro_canopen_data_type_to_pdo_data_len(uint8_t datatype);
uint8_t micro_canopen_fifo_pop(struct micro_canopen_obj_t *obj, struct can_fifo_obj_t *data);
uint8_t micro_canopen_fifo_push_into_txfifo(struct micro_canopen_obj_t *obj, uint16_t id, uint8_t *data, uint8_t len);
uint8_t micro_canopen_fifo_pop_from_txfifo(struct micro_canopen_obj_t *obj, struct can_fifo_obj_t *data);
void micro_canopen_tpdo_read_from_can(struct micro_canopen_obj_t *obj, uint16_t rpod_id, uint8_t *data,uint8_t len);

// 打印od 字典数据
void micro_canopen_print_od(struct micro_canopen_obj_t *obj , uint8_t od_id)
{
    if(od_id >= OD_ID_MAX) return;
    rt_kprintf("------------------OD------------------\r\n");
    rt_kprintf("name: %s\r\n", obj->od_items[od_id].name);
    rt_kprintf("index: %x\r\n", obj->od_items[od_id].index);
    rt_kprintf("subindex: %x\r\n", obj->od_items[od_id].subindex);
    rt_kprintf("data_type: %x\r\n", obj->od_items[od_id].data_type);
    rt_kprintf("value: %x\r\n", obj->node_val[od_id]);

    rt_kprintf("--------------------------------------\r\n");

}
// 通过INDEX 获取OD ID
uint8_t micro_canopen_get_od_id_by_index(struct micro_canopen_obj_t *obj, uint16_t index)
{
    uint8_t i = 0;
    for (i = 0; i < OD_ID_MAX; i++)
    {
        if (obj->od_items[i].index == index)
        {
            return i;
        }
    }
    return 0xff; // 未找到
};


// 获取obj的数据
uint32_t micro_canopen_obj_read_value(struct micro_canopen_obj_t *obj, uint16_t od_id)
{
    return obj->node_val[od_id];
};

// 更新字典中的数据
uint8_t micro_canopen_update_dict(struct micro_canopen_obj_t *obj, uint8_t enum_od_id)
{
    // 从字典中获取数据的index值

    struct can_fifo_obj_t can_fifo_obj_data;

    uint16_t index = obj->od_items[enum_od_id].index;
    uint8_t subindex = obj->od_items[enum_od_id].subindex;

    rt_memset(&can_fifo_obj_data, 0, sizeof(can_fifo_obj_data));

    // 存储 od 中的 id
    obj->recv_sdo.od_index = enum_od_id;

    can_fifo_obj_data.id = TSDO + obj->node_id;
    
    can_fifo_obj_data.data[0] = SDO_CMD_READ_REQ;

    // 索引
    can_fifo_obj_data.data[1] = index & 0xff;
    can_fifo_obj_data.data[2] = (index >> 8) & 0xff;

    // 子索引
    can_fifo_obj_data.data[3] = subindex;
    can_fifo_obj_data.len = 8;

    micro_canopen_fifo_push_into_txfifo(obj,  can_fifo_obj_data.id, can_fifo_obj_data.data, can_fifo_obj_data.len);

}


uint8_t micro_canopen_send_sdo(struct micro_canopen_obj_t *obj, uint16_t id, uint8_t *data, uint8_t len)
{
    // 发送  重试 超时
    uint8_t retry = 0;
    uint8_t timeout = 0;
    uint8_t ret = 0;

    obj->recv_sdo_flag = 0;

    while (timeout < CANOPEN_SDO_RETRY_TIMEOUT)
    {
        // 发送数据
        ret = obj->can_send(id, data, len);

        if (ret != 0) // 发送成功 等待应答
        {
            while (timeout < CANOPEN_SDO_RETRY_TIMEOUT)
            {
                if (obj->recv_sdo_flag != 0 && obj->recv_sdo.recv_sdo_cmd == SDO_CMD_SET_REQ)
                {
                    if (obj->recv_sdo.recv_sdo_index == (data[2] << 8 | data[1]))
                    {
                        // 应答成功
                        return RT_EOK;
                    }
                    else // 不是应答的该指令
                    {
                        break;
                        // 继续等待
                    }
                }
                else if (obj->recv_sdo_flag != 0 && obj->recv_sdo.recv_sdo_cmd == SDO_CMD_ERROR)
                {
                    // 应答失败
                    return RT_ERROR;
                }
                else if (retry >= CANOPEN_SDO_RETRY_TIMEOUT)
                {
                    // 超时
                    return RT_ERROR;
                }
                timeout += CANOPEN_SDO_RETRY_INTERVAL;
                rt_thread_mdelay(CANOPEN_SDO_RETRY_INTERVAL);
            }
        }

        rt_kprintf("send sdo %d\r\n", retry);
        if (retry == 0)
        {
            rt_kprintf("id %x data ", id);
            for (uint8_t i = 0; i < len; i++)
            {
                rt_kprintf("%0X ", data[i]);
            }
        }

        // 等待接收数据
        rt_thread_mdelay(CANOPEN_SDO_RETRY_INTERVAL);
        retry++;
        timeout += CANOPEN_SDO_RETRY_INTERVAL;
        // 打印数据
    }
    return RT_ERROR;
};

void micro_canopen_send_pdo(struct micro_canopen_obj_t *obj, uint16_t id, uint8_t *data, uint8_t len)
{
    // 发送  重试 超时
    uint8_t retry = 0;
    uint8_t timeout = 0;
    uint8_t ret = 0;
    while (timeout < CANOPEN_SDO_RETRY_TIMEOUT)
    {
        // 发送数据
        ret = obj->can_send(id, data, len);

        if (ret != 0)
            return ret;

        rt_kprintf("send sdo %d\r\n", retry);
        if (retry == 0)
        {
            rt_kprintf("id %x data ", id);
            for (uint8_t i = 0; i < len; i++)
            {
                rt_kprintf("%0X ", data[i]);
            }
        }

        // 等待接收数据
        rt_thread_mdelay(CANOPEN_SDO_RETRY_INTERVAL);
        retry++;
        timeout += CANOPEN_SDO_RETRY_INTERVAL;
        // 打印数据
    }
}

/**
 * @brief  激活节点
 * @note
 * @param  *obj: canopen对象
 * @retval None
 */
void micro_canopen_actvate_node(struct micro_canopen_obj_t *obj)
{
    micro_canopen_nmt(obj, NMT_START, obj->node_id);
};

void micro_canopen_nmt(struct micro_canopen_obj_t *obj, uint8_t cmd, uint8_t node_id)
{
    uint8_t data[2] = {0, 0};
    // 激活节点
    data[0] = cmd;
    data[1] = node_id;
    // obj->can_send(NMT_NET, data, 2);
    micro_canopen_fifo_push_into_txfifo(obj, NMT_NET, data, 2);
}

/**
 * @brief  发送同步指令
 * @note
 * @param  *obj: canopen对象
 * @retval None
 */
void micro_canopen_send_sync(struct micro_canopen_obj_t *obj)
{
    uint8_t data[8] = {0, 0};
    micro_canopen_fifo_push_into_txfifo(obj, SYNC, data, 2);
};

/**
 * @brief  初始化canopen
 * @note
 * @param  *obj: canopen对象
 * @retval None
 */
void micro_canopen_init(struct micro_canopen_obj_t *obj,uint8_t node_id,uint8_t od_count)
{

    // 配置文件
    obj->node_id = node_id;
    obj->node_val = rt_malloc(sizeof(uint32_t) * od_count);

    obj->od_items = &od_items[0];

    obj->tpdo_cfg = &tpdo_cfg[0];
    obj->tpdo_items = &tpdo_items[0];

    obj->rpdo_cfg = &rpdo_cfg[0];
    obj->rpdo_items = &rpdo_items[0];

    // 初始化 can fifo
    micro_canopen_fifo_init(obj);
};
void micro_canopen_data_send(struct micro_canopen_obj_t *obj)
{
    // 从fifo 中取出 data
    struct can_fifo_obj_t can_fifo_obj_data;

    while (micro_canopen_fifo_pop_from_txfifo(obj, &can_fifo_obj_data))
    {
        // id 如果是 sdo 则需要等待应答超时等

        // id 如果是 pdo 则直接发送只判断can收发成功与否
        if ((can_fifo_obj_data.id & TSDO) == TSDO)
        {
            micro_canopen_send_sdo(obj, can_fifo_obj_data.id, can_fifo_obj_data.data, can_fifo_obj_data.len);
        }
        else
        {
            micro_canopen_send_pdo(obj, can_fifo_obj_data.id, can_fifo_obj_data.data, can_fifo_obj_data.len);
        }
    }
}

/**
 * @brief  canopen处理函数
 * @note
 * @param  *obj: canopen对象
 * @retval None
 */
void micro_canopen_process(struct micro_canopen_obj_t *obj)
{
    obj->sync_time_count += obj->main_period_ms;
    obj->heartbeat_time_count += obj->main_period_ms;

    // 发送同步指令
    if (obj->sync_time_count >= obj->sync_time)
    {
        micro_canopen_send_sync(obj);
        obj->sync_time_count = 0;
    };

    // 发送RPDO
    micro_canopen_rpdo_write_to_can(obj);

    // 从 txfifo 取出 can 数据 并且发送出去
    micro_canopen_data_send(obj);

    // 心跳检测
    if (obj->heartbeat_time_count >= obj->heartbeat_timeout)
    {
        obj->heartbeat_time_count = 0;

        // 打印超时信息
        rt_kprintf("canopen heartbeat timeout\r\n");
        // 节点失效
        // obj->node_id = 0;
    };
};

/**
 * @brief  初始化PDO
 * @note
 * @param  *obj: canopen对象
 * @param  *pdo_cfg_obj: pdo配置对象
 * @retval None
 */
void micro_canopen_pdo_init(struct micro_canopen_obj_t *obj, uint8_t pdo_type)
{
    struct micro_canopen_pdo_cfg_t *p_cfg;
    struct micro_canopen_od_pdo_itmes_t *p_items;
    struct micro_canopen_od_items *od;

    int i = 0;
    int j = 0;

    uint16_t pdo_cp_index; // PDO communication parameter rpdo:0x1400 tpdo:0x1800
    uint16_t pdo_mp_index; // PDO mapping parameter       rpdo:0x1600 tpdo:0x1a00
    uint32_t index_pack;
    uint8_t sub_start;
    uint8_t sub_count;
    uint8_t count;
    uint8_t cfg_max;
    uint8_t items_max;
    uint8_t temp;

    // uint8_t pdo_cfg_size = sizeof(*p_cfg) / sizeof(struct micro_canopen_pdo_cfg_t);
    // uint8_t pdo_items_size = sizeof(*p_items) / sizeof(struct micro_canopen_od_pdo_itmes_t);
    od = obj->od_items;
    if (pdo_type == PDO_TYPE_RPDO)
    {
        // 配置RPDO
        // 0x1400 + node_id
        pdo_cp_index = 0x1400;
        pdo_mp_index = 0x1600;
        cfg_max = RPDO_CFG_NUM;
        items_max = RPDO_ITEMS_NUM;
        p_cfg = &obj->rpdo_cfg[0];
        p_items = &obj->rpdo_items[0];
    }
    else
    {
        // 配置TPDO 使用
        pdo_cp_index = 0x1800;
        pdo_mp_index = 0x1A00;
        cfg_max = TPDO_CFG_NUM;
        items_max = TPDO_ITEMS_NUM;
        p_cfg = &obj->tpdo_cfg[0];
        p_items = &obj->tpdo_items[0];
    }

    sub_count = 0;
    j = 0;
    sub_start = 0;
    for (i = 0; i < cfg_max; i++)
    {

        sub_count = 0;

        // 清空PDO映射
        micro_canopen_pack_sdo_data(obj, pdo_mp_index, 0, SDO_CMD_SET_1BYTE, 0);

        // 映射
        while (p_items[j].pdo_sub_data_id == sub_start && j < items_max)
        {
            // 组装要map的数据  数据索引+子索引+长度

            // 数据长度  bit 个数
            temp = micro_canopen_data_type_to_pdo_data_len(od[p_items[j].od_id].data_type);

            // 组装后的数据
            index_pack = 0;
            index_pack = (uint32_t)(od[p_items[j].od_id].index) << 16;
            index_pack |= (uint32_t)(od[p_items[j].od_id].subindex) << 8;
            index_pack |= temp;

            micro_canopen_pack_sdo_data(obj, pdo_mp_index, sub_count + 1, SDO_CMD_SET_4BYTE, index_pack);

            j++;
            sub_count++;
        }
        if (sub_count == 0)
        {
            // 关闭PDO
            micro_canopen_pack_sdo_data(obj, pdo_cp_index, 5, SDO_CMD_SET_2BYTE, 0);
            pdo_cp_index++;
            pdo_mp_index++;
            continue;
        }
        sub_start++;
        
        if(pdo_type != PDO_TYPE_RPDO)
        {
            /*
            传输类型
            异步传输——由事件触发传输，包括数据改变触发、周期性事件定时器触发；
            同步传输——网络中同步帧有关。

            1）当 TPDO 传输类型为 0 时，如果映射对象的数据发生改变，且接收到一个同步帧，则发送该 TPDO；
            2）当 TPDO 的传输类型为 1~240 时，接收到相应个数的同步帧时，发送该 TPDO。
            3）当 TPDO 的传输类型是 254 或 255 时，映射数据发生改变或者事件计时器到达则发送该 TPDO。
            4）当 RPDO 的传输类型为 0~240 时，只要接收到一个同步帧则将该 RPDO 最新的数据更新到应用；当 RPDO 的传输类型为 254 或者 255 时，将接收到的数据直接更新到应用。
            [00] 非循环同步
            [01] 循环同步
            [FC] 远程同步
            [FD] 远程异步
            [FE] 异步(制造商特定事件)
            [FF] 同步(制造商特定事件)
            */
            micro_canopen_pack_sdo_data(obj, pdo_cp_index, 2, SDO_CMD_SET_1BYTE, p_cfg->transmission_type);

            /*
            禁止时间
            针对 TPDO 设置了禁止时间，存放在通信参数(1800h~1803h) 的子索引 03 上，防止CAN 网络被优先级较高的 PDO 持续占有。
            该参数的单位是 100us，设置数值后，同一个TPDO 传输间隔减不得小于该参数对应的时间。
            例如：TPDO2 的禁止时间为 300，则 TPDO 的传输间隔不会小于 30ms。
            */
            micro_canopen_pack_sdo_data(obj, pdo_cp_index, 3, SDO_CMD_SET_2BYTE, p_cfg->inhibit_time);     

            /*
            事件计时器
            针对异步传输( 传输类型为 254 或 255) 的 TPDO，定义事件计时器，位于通信参数(1800h~1803h) 的子索引 05 上，单位为 500us。
            事件计时器也可以看做是一种触发事件，它也会触发相应的 TPDO 传输。
            如果在计时器运行周期内出现了数据改变等其它事件，TPDO也会触发，且事件计数器会被立即复位。

            如果这个时间为 0，则这个 PDO 为事件改变发送。
            */   
            micro_canopen_pack_sdo_data(obj, pdo_cp_index, 5, SDO_CMD_SET_2BYTE, p_cfg->event_timer);

            /*
            SYNC start value 
            同步起始值：同步传输的 PDO，收到诺干个同步包后，才进行发送，这个同步起始值就是同步包数量。
            比如设置为 2，即收到 2 个同步包后才进行发送。
            */
            micro_canopen_pack_sdo_data(obj, pdo_cp_index, 6, SDO_CMD_SET_1BYTE, p_cfg->sync_start);
        }

        // 开启映射
        micro_canopen_pack_sdo_data(obj, pdo_mp_index, 0, SDO_CMD_SET_1BYTE, p_cfg->count);

        pdo_cp_index++;
        pdo_mp_index++;
    }
}

// 判断cmd 是否是读取应答
uint8_t micro_canopen_cmd_is_read_req( uint8_t cmd)
{
    if(cmd == SDO_CMD_GET_1BYTE || cmd == SDO_CMD_GET_2BYTE || cmd == SDO_CMD_GET_3BYTE || cmd == SDO_CMD_GET_4BYTE)
    {
        return RT_EOK;
    }
    return RT_ERROR;

}
void micro_canopen_sdo_data_decode(struct micro_canopen_obj_t *obj,uint8_t *data)
{

    
    obj->recv_sdo.recv_sdo_cmd = data[0];
    obj->recv_sdo.recv_sdo_index = data[2] << 8 | data[1];
    obj->recv_sdo.recv_sdo_subindex = data[3];
    obj->recv_sdo.recv_sdo_data = data[4] | data[5] << 8 | data[6] << 16 | data[7] << 24;

    if(micro_canopen_cmd_is_read_req(data[0]) == RT_EOK)
    {
        // 从字典中取出数据
        uint8_t od_index = obj->recv_sdo.od_index;
        if(od_index >= OD_ID_MAX) return;
        
        uint16_t index = obj->od_items[od_index].index;
        uint8_t subindex = obj->od_items[od_index].subindex;
        if(index == obj->recv_sdo.recv_sdo_index && subindex == obj->recv_sdo.recv_sdo_subindex)
        {
            obj->node_val[od_index] = obj->recv_sdo.recv_sdo_data;
        } 

    }
    obj->recv_sdo_flag = 1;

}
uint16_t micro_canopen_canid_to_cobid(uint32_t canid)
{
    return (uint16_t)(canid & 0xff80);
}
uint8_t micro_canopen_canid_to_nodeid(uint32_t canid)
{
    return (uint8_t)(canid & 0x7f);
}
/**
 * @brief  将CAN数据解析成OD数据
 * @note
 * @param  *obj: canopen对象
 * @param  index: 索引
 * @param  data: 数据
 * @retval None
 */
void micro_canopen_data_unpack(struct micro_canopen_obj_t *obj, uint32_t index, uint8_t *data, uint8_t len)
{
    uint16_t fun_id = 0;
    uint8_t node_id = 0;    
    fun_id =  micro_canopen_canid_to_cobid(index);
    node_id = micro_canopen_canid_to_nodeid(index);

    // 将CAN数据解析成OD数据
    switch (fun_id)
    {
    case NMT_NET:
        break;
    case SYNC:
        // 同步数据
        if (node_id == 0)
        {
        }
        // 紧急数据
        {
        }
        break;
    case TPDO0:
    case TPDO1:
    case TPDO2:
    case TPDO3:
        micro_canopen_tpdo_read_from_can(obj, fun_id, data ,len);
        // 从CAN数据解析成OD数据
        break;
    case RPDO0:
    case RPDO1:
    case RPDO2:
    case RPDO3:

        break;
    case RSDO: // 从站返回
        micro_canopen_sdo_data_decode(obj,data);

        break;
    case TSDO:
        break;
    case HEART_NMT_ERR:
        // 心跳
        if(node_id == obj->node_id)
        {
            obj->heartbeat_time_count = 0;
        }
        break;

    default:
        break;
    }
}

/**
 * @brief  将数据打包成CAN数据
 * @note
 * @param  *obj: canopen对象
 * @param  index: 索引
 * @param  subindex: 子索引
 * @param  val_type: 数据类型
 * @param  val: 数据
 * @retval None
 */

void micro_canopen_pack_sdo_data(struct micro_canopen_obj_t *obj, uint16_t index, uint8_t subindex, uint8_t val_type, int32_t val)
{
    uint8_t temp[8];

    // type
    temp[0] = val_type;

    // index
    temp[1] = index & 0xff;
    temp[2] = (index >> 8) & 0xff;

    // subindex
    temp[3] = subindex;

    // value
    temp[4] = val & 0xff;
    temp[5] = (val >> 8) & 0xff;
    temp[6] = (val >> 16) & 0xff;
    temp[7] = (val >> 24) & 0xff;

    // 将数据打包成CAN数据
    // obj->can_send(TSDO + obj->node_id, temp, 8);
    micro_canopen_fifo_push_into_txfifo(obj, TSDO + obj->node_id, temp, 8);
}

uint8_t micro_canopen_data_type_to_pdo_data_len(uint8_t datatype)
{
    switch (datatype)
    {
    case OD_DATA_TYPE_BOOLEAN:
        return 1;
    case OD_DATA_TYPE_INT8:
        return 8;
    case OD_DATA_TYPE_INT16:
        return 16;
    case OD_DATA_TYPE_INT32:
        return 32;
    case OD_DATA_TYPE_UINT8:
        return 8;
    case OD_DATA_TYPE_UINT16:
        return 16;
    case OD_DATA_TYPE_UINT32:
        return 32;
    case OD_DATA_TYPE_REAL32:
        return 32;
    default:
        break;
    }
}

/** 将数据类型转换成数据长度
 * @brief  将数据类型转换成数据长度
 * @note
 * @param  datatype: 数据类型
 * @retval None
 */
uint8_t micro_canopen_data_type_to_data_len(uint8_t datatype)
{
    switch (datatype)
    {
    case OD_DATA_TYPE_BOOLEAN:
        return 1;
    case OD_DATA_TYPE_INT8:
        return 1;
    case OD_DATA_TYPE_INT16:
        return 2;
    case OD_DATA_TYPE_INT32:
        return 4;
    case OD_DATA_TYPE_UINT8:
        return 1;
    case OD_DATA_TYPE_UINT16:
        return 2;
    case OD_DATA_TYPE_UINT32:
        return 4;
    case OD_DATA_TYPE_REAL32:
        return 4;
    default:
        break;
    }
}

/** 将数据类型转换成SDO命令头
 * @brief  将数据类型转换成SDO命令
 * @note
 * @param  datatype: 数据类型
 * @retval None
 */
uint8_t micro_canopen_data_type_to_sdo_cmd(uint8_t datatype)
{
    switch (datatype)
    {
    case OD_DATA_TYPE_BOOLEAN:
        return SDO_CMD_SET_1BYTE;
    case OD_DATA_TYPE_INT8:
        return SDO_CMD_SET_1BYTE;
    case OD_DATA_TYPE_INT16:
        return SDO_CMD_SET_2BYTE;
    case OD_DATA_TYPE_INT32:
        return SDO_CMD_SET_4BYTE;
    case OD_DATA_TYPE_UINT8:
        return SDO_CMD_SET_1BYTE;
    case OD_DATA_TYPE_UINT16:
        return SDO_CMD_SET_2BYTE;
    case OD_DATA_TYPE_UINT32:
        return SDO_CMD_SET_4BYTE;
    case OD_DATA_TYPE_REAL32:
        return SDO_CMD_SET_4BYTE;
    default:
        break;
    }
}

/**
 * @brief  写OD数据
 * @note
 * @param  *items: od数据
 * @param  od_id: od索引
 * @param  val: 数据
 * @retval None
 */
void micro_canopen_od_wrtie_value(struct micro_canopen_obj_t *obj, uint16_t od_id, uint32_t val)
{
    struct micro_canopen_od_items *items;
    uint8_t data_buf[8];
    items = &obj->od_items[od_id];
    //items->write_value = val;
    uint8_t cmd = micro_canopen_data_type_to_sdo_cmd(items->data_type);

    // 数据类型
    data_buf[0] = cmd;

    // 字典索引
    data_buf[1] = items->index & 0xff;
    data_buf[2] = (items->index >> 8) & 0xff;

    // 子索引
    data_buf[3] = items->subindex;

    // 数据
    data_buf[4] = val & 0xff;
    data_buf[5] = (val >> 8) & 0xff;
    data_buf[6] = (val >> 16) & 0xff;
    data_buf[7] = (val >> 24) & 0xff;

    // 发送数据
    micro_canopen_fifo_push_into_txfifo(obj, TSDO + obj->node_id, data_buf, 8);
};

void micro_canopen_tpdo_read_from_can(struct micro_canopen_obj_t *obj, uint16_t rpod_id, uint8_t *data ,uint8_t len)
{
    // 将数据复制到 raw map 数组
    uint8_t id = (rpod_id - 0x180) / 0x100;
    memcpy(obj->tpod_map_raw_data.pdo_data[id].pdo_can_data, data, len);

    // 更新数据到结构体
};

void micro_canopen_rpdo_write_to_can(struct micro_canopen_obj_t *obj)
{
    uint8_t i = 0;
    uint8_t change_flag = obj->rpod_map_raw_data.change_flag;
    if (change_flag == 0)
        return;

    for (i = 0; i < 4; i++)
    {
        if (change_flag & (1 << i))
        {
            // 发送数据
            // obj->can_send(TPDO0 + (0x100 * i) + obj->node_id, obj->tpod_map_raw_data.pdo_data[i].pdo_can_data, 8);
            micro_canopen_fifo_push_into_txfifo(obj, RPDO0 + (0x100 * i) + obj->node_id, obj->rpod_map_raw_data.pdo_data[i].pdo_can_data, 8);

            // 将 flag 清除
            obj->rpod_map_raw_data.change_flag &= ~(1 << i);
        }
    }
}
/**
 * @brief  写RPOD数据
 * @note
 * @param  *items: pod数据
 * @param  tpod_id: pod索引
 * @param  val: 数据
 * @retval None
 */
void micro_canopen_rpdo_wrie_value(struct micro_canopen_obj_t *obj, uint16_t rpod_id, uint32_t val)
{

    struct micro_canopen_od_pdo_itmes_t *items;
    struct micro_canopen_od_pdo_map_t *map;
    uint8_t index;
    uint8_t start_index = 0;

    uint8_t i, j;

    // RPDO RAW MAP 数据
    map = &obj->rpod_map_raw_data;

    // 获取 items 列表
    items = &obj->rpdo_items[rpod_id];
    // 获取 所在位置
    index = items->pdo_sub_data_id;

    // 标志数据有变动
    map->change_flag |= (1 << index);

    // 获取 PDO 长度
    uint32_t od_id = items->od_id;
    uint8_t data_type = obj->od_items[od_id].data_type;

    // 写入值
    //obj->od_items[od_id].write_value = val;

    // 写入 raw map 数据
    uint8_t data_len = micro_canopen_data_type_to_data_len(data_type);
    start_index = items->pos;

    // j = 0;
    for (i = start_index, j = 0; i < start_index + data_len; i++, j++)
    {
        map->pdo_data[index].pdo_can_data[i] = val >> (j * 8) & 0xff;
    }
};

// 读TPDO数据
void micro_canopen_tpdo_read_value(struct micro_canopen_obj_t *obj, uint16_t rpod_id, uint32_t *val)
{
    struct micro_canopen_od_pdo_itmes_t *items;
    struct micro_canopen_od_pdo_map_t *map;
    uint8_t index;
    uint8_t start_index = 0;

    uint8_t i, j;
    uint32_t temp = 0;

    // TPDO RAW MAP 数据
    map = &obj->tpod_map_raw_data;

    items = &obj->tpdo_items[rpod_id];
    index = items->pdo_sub_data_id;

    // 获取 PDO 长度
    uint32_t od_id = items->od_id;
    uint8_t data_type = obj->od_items[od_id].data_type;

    // 读取 raw map 数据
    uint8_t data_len = micro_canopen_data_type_to_data_len(data_type);

    if(data_len>4)
    {
        rt_kprintf("data_len > 4\r\n");
        return;
    }

    index = items->pdo_sub_data_id;
    start_index = items->pos;
    // j = 0;
    for (i = start_index, j = 0; i < start_index + data_len; i++, j++)
    {
        //*val |= (uint64_t)(map->pdo_data[index].pdo_can_data[i] << (j * 8));
        temp |= (uint64_t)(map->pdo_data[index].pdo_can_data[i] << (j * 8));
    }
    *val = temp;
};

/* --------------------------- CAN FIFO ------------------------------- */

void micro_canopen_fifo_init(struct micro_canopen_obj_t *obj)
{
    obj->tx_fifo.count = 0;
    obj->tx_fifo.front = 0;
    obj->tx_fifo.rear = 0;
}
uint8_t micro_canopen_fifo_push_into_txfifo(struct micro_canopen_obj_t *obj, uint16_t id, uint8_t *data, uint8_t len)
{
    // 判断是否满
    struct can_fifo_t *fifo = &obj->tx_fifo;

    if (fifo->count >= CAN_FIFO_SIZE)
    {
        return 0;
    }

    rt_memcpy(fifo->buff[fifo->rear].data, data, len);
    fifo->buff[fifo->rear].id = id;
    fifo->buff[fifo->rear].len = len;
    fifo->rear = (fifo->rear + 1) % CAN_FIFO_SIZE;
    fifo->count++;
    return 1;
}

// 从TXFIFO中取出
uint8_t micro_canopen_fifo_pop_from_txfifo(struct micro_canopen_obj_t *obj, struct can_fifo_obj_t *data)
{
    // 判断是否空
    struct can_fifo_t *fifo = &obj->tx_fifo;

    if (fifo->count == 0)
    {
        return 0;
    }
    rt_memcpy(data, &fifo->buff[fifo->front], sizeof(struct can_fifo_obj_t));
    fifo->front = (fifo->front + 1) % CAN_FIFO_SIZE;
    fifo->count--;
    return 1;
};
// -------------------------------- 针对用户部分---------------------------------------------//
// 从CAN 读取数据接口
uint8_t micro_canopen_can_data_push(struct micro_canopen_obj_t *obj, uint32_t id, uint8_t *data, uint8_t len)
{
    // 解析CAN数据
    micro_canopen_data_unpack(obj, id, data, len);
};
