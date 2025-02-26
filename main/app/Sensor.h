#ifndef _AS5600_TASK_H_
#define _AS5600_TASK_H_

#include "as5600.h"
#include "mt6701.h"
#include <cstdint>


#pragma pack(1)
typedef struct {
    uint8_t sof;       // 数据帧起始字节，固定值为 0xA5
    uint16_t data_len; // 数据帧中 data 的长度
    uint8_t seq;       // 包序号
    uint8_t crc8;      // 帧头 CRC8 校验
} frame_header_t;      // 5-bytes

// typedef struct {
//     int16_t joints[7]; //14-bytes
//     uint8_t push;
//     uint8_t state1;
//     uint8_t state2;
//     uint8_t data[13];
// } ctrl_data_t;

typedef struct {
    float joints[7]; // 28-bytes
    uint8_t push;
    uint8_t state1;
} ctrl_data_t; // 30-bytes

typedef struct {
    frame_header_t frame_header; // 5-bytes
    uint16_t cmd_id;             // 2-bytes
    ctrl_data_t ctrl_data;       // 30-bytes
    uint16_t frame_tail;         // 2-bytes 整包校验
} tx_data_t;                     // 39-bytes
#pragma pack()

float *get_angle();
void sensor_task(void *param);

#endif