/**
 * \file VcuComm_LCfg.c
 * \copyright UDAN Co.,Ltd. *
 *
 * \brief 整车通信链接配置文件.
 *
 * * \par 修订历史:
 * | 版本号 | 修订日志 | 修改人 | 修订时间 |
 * | :--- | :--- | :--- | :--- |
 * | 0.1 | 初始版本, 完成讨论部分的定义. | UD00004 | 20161209 |
 */

#include "VcuComm_Types.h"
#include "VcuComm.h"
#include "VcuComm_Messages.h"

const uint16 VcuComm_RxIPduStart = (uint16)VCUCOMM_RX_IPDU_START;
const uint16 VcuComm_RxIPduStop = (uint16)VCUCOMM_RX_IPDU_STOP;
const uint16 VcuComm_StageIdle = (uint16)VCUCOMM_STAGE_IDLE;
const uint16 VcuComm_StageStart = (uint16)VCUCOMM_STAGE_START;
const uint16 VcuComm_StageStop = (uint16)VCUCOMM_STAGE_STOP;
const uint16 VcuComm_StageMax = (uint16)VCUCOMM_STAGE_MAX;

#define VCUCOMM_SEND_IPDU_CFG_NUM           (((uint16)VCUCOMM_TX_IPDU_STOP - (uint16)VCUCOMM_TX_IPDU_START) + 1U)
#define VCUCOMM_REC_IPDU_CFG_NUM            (((uint16)VCUCOMM_RX_IPDU_STOP - (uint16)VCUCOMM_RX_IPDU_START) + 1U)

const uint8 VcuComm_SendIPduNum = (uint8)VCUCOMM_SEND_IPDU_CFG_NUM;
const VcuComm_SendIPduCfgType VcuComm_IPduSendConfigInfo[VCUCOMM_SEND_IPDU_CFG_NUM] = {
    {
        (uint16)VCUCOMM_TX_IPDU_MSG_0x1801FFF4, /**< IPdu */
        8U, /**< Length */
        100U, /**< Cycle */
        NULL, /**< CondFunc */
        VcuComm_GetMsgData0x1801FFF4Cbk, /**< Func */
    },
    {
        (uint16)VCUCOMM_TX_IPDU_MSG_0x1802FFF4, /**< IPdu */
        8U, /**< Length */
        100U, /**< Cycle */
        NULL, /**< CondFunc */
        VcuComm_GetMsgData0x1802FFF4Cbk, /**< Func */
    },
    {
        (uint16)VCUCOMM_TX_IPDU_MSG_0x1803FFF4, /**< IPdu */
        8U, /**< Length */
        100U, /**< Cycle */
        NULL, /**< CondFunc */
        VcuComm_GetMsgData0x1803FFF4Cbk, /**< Func */
    },
    {
        (uint16)VCUCOMM_TX_IPDU_MSG_0x1804FFF4, /**< IPdu */
        8U, /**< Length */
        100U, /**< Cycle */
        NULL, /**< CondFunc */
        VcuComm_GetMsgData0x1804FFF4Cbk, /**< Func */
    },
    {
        (uint16)VCUCOMM_TX_IPDU_MSG_0x1807FFF4, /**< IPdu */
        8U, /**< Length */
        100U, /**< Cycle */
        NULL, /**< CondFunc */
        VcuComm_GetMsgData0x1807FFF4Cbk, /**< Func */
    },
    {
        (uint16)VCUCOMM_TX_IPDU_MSG_0x1801D2F4, /**< IPdu */
        8U, /**< Length */
        1000U, /**< Cycle */
        NULL, /**< CondFunc */
        VcuComm_GetMsgData0x1801D2F4Cbk, /**< Func */
    },
    {
        (uint16)VCUCOMM_TX_IPDU_MSG_0x1802D2F4, /**< IPdu */
        8U, /**< Length */
        1000U, /**< Cycle */
        NULL, /**< CondFunc */
        VcuComm_GetMsgData0x1802D2F4Cbk, /**< Func */
    },
    {
        (uint16)VCUCOMM_TX_IPDU_MSG_0x1803D2F4, /**< IPdu */
        8U, /**< Length */
        1000U, /**< Cycle */
        NULL, /**< CondFunc */
        VcuComm_GetMsgData0x1803D2F4Cbk, /**< Func */
    },
    {
        (uint16)VCUCOMM_TX_IPDU_MSG_0x1804D2F4, /**< IPdu */
        8U, /**< Length */
        1000U, /**< Cycle */
        NULL, /**< CondFunc */
        VcuComm_GetMsgData0x1804D2F4Cbk, /**< Func */
    },
    {
        (uint16)VCUCOMM_TX_IPDU_MSG_0x1805D2F4, /**< IPdu */
        8U, /**< Length */
        1000U, /**< Cycle */
        NULL, /**< CondFunc */
        VcuComm_GetMsgData0x1805D2F4Cbk, /**< Func */
    },
    {
        (uint16)VCUCOMM_TX_IPDU_MSG_0x1806D2F4, /**< IPdu */
        8U, /**< Length */
        1000U, /**< Cycle */
        NULL, /**< CondFunc */
        VcuComm_GetMsgData0x1806D2F4Cbk, /**< Func */
    },
    {
        (uint16)VCUCOMM_TX_IPDU_MSG_0x1807D2F4, /**< IPdu */
        8U, /**< Length */
        1000U, /**< Cycle */
        NULL, /**< CondFunc */
        VcuComm_GetMsgData0x1807D2F4Cbk, /**< Func */
    },
    {
        (uint16)VCUCOMM_TX_IPDU_MSG_0x1808D2F4, /**< IPdu */
        8U, /**< Length */
        1000U, /**< Cycle */
        NULL, /**< CondFunc */
        VcuComm_GetMsgData0x1808D2F4Cbk, /**< Func */
    },
    {
        (uint16)VCUCOMM_TX_IPDU_MSG_0x1809D2F4, /**< IPdu */
        8U, /**< Length */
        1000U, /**< Cycle */
        NULL, /**< CondFunc */
        VcuComm_GetMsgData0x1809D2F4Cbk, /**< Func */
    },
    {
        (uint16)VCUCOMM_TX_IPDU_MSG_0x180AD2F4, /**< IPdu */
        8U, /**< Length */
        1000U, /**< Cycle */
        NULL, /**< CondFunc */
        VcuComm_GetMsgData0x180AD2F4Cbk, /**< Func */
    },
    {
        (uint16)VCUCOMM_TX_IPDU_MSG_0x1850D2F4, /**< IPdu */
        8U, /**< Length */
        1000U, /**< Cycle */
        NULL, /**< CondFunc */
        VcuComm_GetMsgData0x1850D2F4Cbk, /**< Func */
    },
};

static const VcuComm_RecIPduInfoType VcuComm_Stage1RecItem[] = {
    {
        (uint16)VCUCOMM_RX_IPDU_MSG_0x12345678, /**< IPdu */
        8U,
        VCUCOMM_REC_TIMEOUT_DEFAULT, /**< Timeout: 接收超时时间 */
        NULL, /**< TimeoutFunc: 获取IPdu接收超时时间回调函数 */
        NULL, /**< RecFunc: 数据接收回调函数 */
        NULL, /**< TimeoutEventFunc: 数据接收超时事件回调函数 */
    },
};


/**
 * \brief 整车通信接收报文配置
 */
const VcuComm_RecIPduCfgType VcuComm_IPduRecConfigInfo[VCUCOMM_STAGE_MAX] = {
    {
        (uint8)ARRAY_SIZE(VcuComm_Stage1RecItem), /**< IPduNum: 报文个数 */
        VcuComm_Stage1RecItem, /**< IPduInfo: 各IPdu配置信息 */
    },
    {
        (uint8)ARRAY_SIZE(VcuComm_Stage1RecItem), /**< IPduNum: 报文个数 */
        VcuComm_Stage1RecItem, /**< IPduInfo: 各IPdu配置信息 */
    },
};


