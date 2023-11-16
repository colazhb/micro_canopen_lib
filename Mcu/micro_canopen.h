/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2023-09-05     koala.zhoubo       the first version
 */
#ifndef APPLICATIONS_MICRO_CANOPEN_MICRO_CANOPEN_H_
#define APPLICATIONS_MICRO_CANOPEN_MICRO_CANOPEN_H_

#include <stdint.h>

#define CANOPEN_SDO_RETRY_INTERVAL 10  // sdo 重试间隔 ms
#define CANOPEN_SDO_RETRY_TIMEOUT  100 // sdo 超时时间 ms
/* --------------------------- CAN FIFO ------------------------------- */
#define CAN_FIFO_SIZE  128
struct  can_fifo_obj_t
{
    uint32_t id;
    uint8_t data[8];
    uint8_t len;
};

struct can_fifo_t
{
    struct can_fifo_obj_t buff[CAN_FIFO_SIZE];
    uint8_t front;
    uint8_t rear;
    uint8_t count;
};

/*-------------------------- 字典部分 ------------------------------*/
// 字典值
enum
{
    OD_DATA_TYPE_BOOLEAN = 0x01,
    OD_DATA_TYPE_INT8 = 0x02,
    OD_DATA_TYPE_INT16 = 0x03,
    OD_DATA_TYPE_INT32 = 0x04,
    OD_DATA_TYPE_UINT8 = 0x05,
    OD_DATA_TYPE_UINT16 = 0x06,
    OD_DATA_TYPE_UINT32 = 0x07,
    OD_DATA_TYPE_REAL32 = 0x08,
    OD_DATA_TYPE_UNKNOWN = 0xff,
};
// 读写类型
enum
{
    OD_RW_ACCESS_RW,
    OD_RW_ACCESS_RO,
    OD_RW_ACCESS_WO,
    OD_RW_ACCESS_RWR,
    OD_RW_ACCESS_RWW,
    OD_RW_ACCESS_C,
};

// 序号,索引,子索引,数据类型,数据长度,参数名
struct micro_canopen_od_items
{
    uint16_t index;
    uint8_t subindex;
    uint8_t data_type;
    uint8_t data_len;
    // uint8_t access;
    // uint32_t value;
    // uint32_t read_value;
    // uint32_t write_value;
    // uint32_t min;
    // uint32_t max;
    // uint32_t unit;
    char *name;
    // char *desc;
};

/*-------------------------- 字典部分结束 ------------------------------*/

enum
{    
    SDO_CMD_SET_1BYTE = 0x2F,
    SDO_CMD_SET_2BYTE = 0x2B,
    SDO_CMD_SET_3BYTE = 0x27,
    SDO_CMD_SET_4BYTE = 0x23,

    SDO_CMD_SET_REQ   = 0x60,
    SDO_CMD_READ_REQ  = 0x40,

    SDO_CMD_GET_1BYTE = 0x4F,
    SDO_CMD_GET_2BYTE = 0x4B,
    SDO_CMD_GET_3BYTE = 0x47,
    SDO_CMD_GET_4BYTE = 0x43,

    SDO_CMD_ERROR = 0x80,
};

// NMT 指令
enum
{
    NMT_START     = 0x01,
    NMT_STOP      = 0x02,
    NMT_PREOP     = 0x80,
    NMT_RST_NODE  = 0x81,
    NMT_RST_COMM  = 0x82,

};

enum
{
    NMT_NET = 0x000,
    SYNC    = 0x080,
    EMCY    = 0x080,  // 080+Node-ID
    TPDO0   = 0x180,  // 180+Node-ID
    RPDO0   = 0x200,  // 200+Node-ID
    TPDO1   = 0x280,  // 280+Node-ID
    RPDO1   = 0x300,  // 300+Node-ID
    TPDO2   = 0x380,  // 380+Node-ID
    RPDO2   = 0x400,  // 400+Node-ID
    TPDO3   = 0x480,  // 480+Node-ID
    RPDO3   = 0x500,  // 500+Node-ID
    RSDO    = 0x580,  // 580+Node-ID
    TSDO    = 0x600,  // 600+Node-ID
    HEART_NMT_ERR   = 0x700
    //NMT_ERR = 0x700,  // 700+Node-ID

};

enum
{
    PDO_TYPE_TPDO = 0x01,
    PDO_TYPE_RPDO = 0x02,
};

enum
{
    RPDO_ID_0,
    RPDO_ID_1,
    RPDO_ID_2,
    RPDO_ID_3,
};
enum
{
    TPDO_ID_0,
    TPDO_ID_1,
    TPDO_ID_2,
    TPDO_ID_3,
};

/*

micro_canopen_od_pdo_itmes_t 结构体解释
index: 索引值 例如: 0x60FF
pdo_sub_data_id: 在items数组中的序号 例如:struct micro_canopen_od_pdo_itmes_t tpdo_items[x] 中的x的值
pos: 在数组的起始位置
data_type: 数据类型

TX PDO

ID  COB  INDEX      BYTE0   BYTE1   BYTE2   BYTE3   BYTE4   BYTE5   BYTE6   BYTE7
1   180  0x1800     [                                                           ]
2   280  0x1801     [                                                           ]
3   380  0x1802     [                                                           ]
4   480  0x1803     [                                                           ]


RX PDO

ID  COB  INDEX      BYTE0   BYTE1   BYTE2   BYTE3   BYTE4   BYTE5   BYTE6   BYTE7
1   200  0x1400     [                                                           ]
2   300  0x1401     [                                                           ]
3   400  0x1402     [                                                           ]
4   500  0x1403     [                                                           ]
*/


struct micro_canopen_od_pdo_itmes_t
{
    uint8_t od_id;              // 对应 od 字典中的 id 序号
    uint8_t pdo_sub_data_id;    // 在items中的序号
    uint8_t pos;                // 在数组的起始位置
    //uint8_t pdo_index;          // 在pdo中的子序号
    
};
struct micro_canopen_od_pdo_data_t
{
    uint8_t pdo_can_data[8];
    /* data */
};

struct micro_canopen_od_pdo_map_t
{
    uint32_t change_flag;    // 是否数据有变动,变动的数据才进行发送,或者接收 ， MAP 数组共4个 每组可以表示8个bit 所以要用32位,因为最大ID 可能有32个

    struct micro_canopen_od_pdo_data_t  pdo_data[4];
};

/* pdo 配置结构体 */
struct micro_canopen_pdo_cfg_t
{
    // //   [Index]  [Mapping]   [COB]  [transmissiontype] [inhibit] [eventtimer] [syncstart]  [Count]
    uint16_t index;
    uint16_t mapping;
    uint32_t cob_id;
    uint8_t transmission_type;
    uint16_t inhibit_time;
    uint16_t event_timer;
    uint8_t sync_start;
    uint8_t count;  
};



struct  micro_canopen_can_data
{
    /* data */
    uint32_t index;
    uint8_t  data_len;
    uint8_t data[8];

};

struct micro_canoepn_sdo_t
{    
    uint8_t  od_index;    // 准备读取的字典 index 的索引

    uint8_t recv_sdo_cmd;
    uint32_t recv_sdo_index;
    uint8_t recv_sdo_subindex;
    uint32_t recv_sdo_data;
};

struct micro_canopen_obj_t
{
    /* data */
    uint32_t *node_val;

    uint8_t node_id;               // 节点ID
    uint32_t bandrate;              // 波特率
    uint32_t sync_time;             // 同步时间
    uint32_t main_period_ms;        // 主进程延时 ms
    
    uint32_t sync_time_count;      // 上一次同步时间
    uint32_t heartbeat_time_count; // 心跳计数器
    uint32_t heartbeat_timeout;    // 心跳超时时间

    // 用于接收的临时 can 数据
    struct micro_canopen_can_data can_pack_data;

    // 字典
    struct micro_canopen_od_items *od_items;

    // TPDO RPDO 配置文件

    struct micro_canopen_od_pdo_itmes_t *tpdo_items;
    struct micro_canopen_od_pdo_itmes_t *rpdo_items;

    struct micro_canopen_pdo_cfg_t *tpdo_cfg;
    struct micro_canopen_pdo_cfg_t *rpdo_cfg;


    // raw 数据 直接通过can 存储
    struct micro_canopen_od_pdo_map_t tpod_map_raw_data;
    struct micro_canopen_od_pdo_map_t rpod_map_raw_data;

    struct can_fifo_t tx_fifo;

    uint8_t (*can_send)(uint16_t id, uint8_t *data, uint8_t len);
    uint8_t (*can_recv)(uint16_t* id, uint8_t *data, uint8_t *len);

    // CAN 接收到数据临时解析
   struct micro_canoepn_sdo_t recv_sdo;
   uint8_t recv_sdo_flag;

};


void micro_canopen_init(struct micro_canopen_obj_t *obj,uint8_t node_id,uint8_t od_count);
void micro_canopen_nmt(struct micro_canopen_obj_t *obj,uint8_t cmd ,uint8_t node_id);
void micro_canopen_process(struct micro_canopen_obj_t *obj);

void micro_canopen_pdo_init(struct micro_canopen_obj_t *obj,uint8_t pdo_type);

void micro_canopen_tpdo_read_value(struct micro_canopen_obj_t *obj, uint16_t rpod_id, uint32_t *val);
void micro_canopen_rpdo_wrie_value(struct micro_canopen_obj_t *obj, uint16_t rpod_id, uint32_t val);

uint8_t micro_canopen_can_data_push(struct micro_canopen_obj_t *obj, uint32_t id, uint8_t *data,uint8_t len);

uint8_t micro_canopen_update_dict(struct micro_canopen_obj_t *obj, uint8_t enum_od_id);

uint32_t micro_canopen_od_read_value(struct micro_canopen_obj_t *obj, uint16_t od_id);
void micro_canopen_od_wrtie_value(struct micro_canopen_obj_t *obj, uint16_t od_id,uint32_t val);

uint8_t micro_canopen_get_od_id_by_index(struct micro_canopen_obj_t *obj, uint16_t index);

uint16_t micro_canopen_canid_to_cobid(uint32_t canid);
uint8_t  micro_canopen_canid_to_nodeid(uint32_t canid);

#endif /* APPLICATIONS_MICRO_CANOPEN_MICRO_CANOPEN_H_ */
