/**
 * \file VcuComm_messages.h
 * \copyright UDAN Co.,Ltd. *
 *
 * \brief 整车通信报文头文件.
 *
 * * \par 修订历史:
 * | 版本号 | 修订日志 | 修改人 | 修订时间 |
 * | :--- | :--- | :--- | :--- |
 * | 0.1 | 初始版本, 完成讨论部分的定义. | UD00004 | 20170316 |
 */
#ifndef VCUCOMM_MESSAGE_H_
#define VCUCOMM_MESSAGE_H_

#include "VcuComm.h"


/**
 * \brief 整车通信高压上下电命令类型定义
 */
typedef enum{
    VCUCOMM_POWER_OFF, /**< 高压下电命令 */
    VCUCOMM_POWER_ON /**< 高压上电命令 */
}VcuComm_PowerCmdType;

/**
 * \整车通信报文数据
 */
typedef struct{
    VcuComm_PowerCmdType PowerCommand; /**< 高压上下电命令 */
}VcuComm_MsgDataType;


extern VcuComm_MsgDataType VcuComm_MsgData;

/**
 * \brief 整车通信报文初始化
 */
void VcuComm_MessageInit(Async_LooperType *looper);

/**
 * \brief 获取整车通信发送报文的数据信息
 *
 * \param Buffer 数据缓存
 * \param Length 数据长度
 */
void VcuComm_GetMsgData0x1801FFF4Cbk(uint8 *Buffer, uint16 *Length);
void VcuComm_GetMsgData0x1802FFF4Cbk(uint8 *Buffer, uint16 *Length);
void VcuComm_GetMsgData0x1803FFF4Cbk(uint8 *Buffer, uint16 *Length);
void VcuComm_GetMsgData0x1804FFF4Cbk(uint8 *Buffer, uint16 *Length);
void VcuComm_GetMsgData0x1807FFF4Cbk(uint8 *Buffer, uint16 *Length);
void VcuComm_GetMsgData0x1801D2F4Cbk(uint8 *Buffer, uint16 *Length);
void VcuComm_GetMsgData0x1802D2F4Cbk(uint8 *Buffer, uint16 *Length);
void VcuComm_GetMsgData0x1803D2F4Cbk(uint8 *Buffer, uint16 *Length);
void VcuComm_GetMsgData0x1804D2F4Cbk(uint8 *Buffer, uint16 *Length);
void VcuComm_GetMsgData0x1805D2F4Cbk(uint8 *Buffer, uint16 *Length);
void VcuComm_GetMsgData0x1806D2F4Cbk(uint8 *Buffer, uint16 *Length);
void VcuComm_GetMsgData0x1807D2F4Cbk(uint8 *Buffer, uint16 *Length);
void VcuComm_GetMsgData0x1808D2F4Cbk(uint8 *Buffer, uint16 *Length);
void VcuComm_GetMsgData0x1809D2F4Cbk(uint8 *Buffer, uint16 *Length);
void VcuComm_GetMsgData0x180AD2F4Cbk(uint8 *Buffer, uint16 *Length);
void VcuComm_GetMsgData0x1850D2F4Cbk(uint8 *Buffer, uint16 *Length);


/**
 * \brief 接收超时处理回调函数
 */
void VcuComm_RecTimeoutCbk(void);


/**
 * \brief 发送整车报文1条件检查
 *
 * \return E_OK: 允许发送 E_NOT_OK: 不允许发送
 */
Std_ReturnType VcuComm_SendConditionCheck(uint16 IPdu);

/**
 * \brief 接收整车报文条件检查
 *
 * \return E_OK: 允许接收 E_NOT_OK: 不允许接收
 */
Std_ReturnType VcuComm_RecConditionCheck(void);

/**
 * \brief 整车接收报文处理
 *
 * \param Buffer 数据缓存
 * \param Length 数据长度
 */
void VcuComm_ReceiveCbk(uint8 *Buffer, uint16 Length);



#endif