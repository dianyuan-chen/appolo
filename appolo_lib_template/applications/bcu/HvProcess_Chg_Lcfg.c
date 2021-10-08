/**
 * \file HvProcess_Chg.c
 * \copyright UDAN Co.,Ltd. *
 *
 * \brief 高压充电流程控制配置文件.
 *
 * * \par 修订历史:
 * | 版本号 | 修订日志 | 修改人 | 修订时间 |
 * | :--- | :--- | :--- | :--- |
 * | 0.1 | 初始版本, 完成讨论部分的定义. | UD00004 | 20170320 |
 */
#include "HvProcess_Types.h"
#include "HvProcess_Chg.h"

const HvProcess_StateInfoType HvProcess_ChgStartInfoConfig[] = {
    {
        HvProcess_ChgStateStartCond, /**< Cond: 条件函数指针 */
        HvProcess_ChgStateStartAction, /**< Action: 动作函数指针 */
        (uint16)HVPROCESS_CHG_HV_ON, /**< Next: 下一状态 */
    },
};

const HvProcess_StateInfoType HvProcess_ChgHvOnInfoConfig[] = {
    {
        HvProcess_ChgChargeConnectionCond, /**< Cond: 条件函数指针 */
        HvProcess_ChgChargeConnectionAction, /**< Action: 动作函数指针 */
        (uint16)HVPROCESS_CHG_RELAY_OFF_DELAY, /**< Next: 下一状态 */
    },
    {
        HvProcess_ChgFaultCond, /**< Cond: 条件函数指针 */
        HvProcess_ChgFaultAction, /**< Action: 动作函数指针 */
        (uint16)HVPROCESS_CHG_RELAY_OFF_DELAY, /**< Next: 下一状态 */
    },
};

const HvProcess_StateInfoType HvProcess_ChgRelayOffDelayInfoConfig[] = {
    {
        HvProcess_ChgRelayOffDelayCond, /**< Cond: 条件函数指针 */
        HvProcess_ChgRelayOffDelayAction, /**< Action: 动作函数指针 */
        (uint16)HVPROCESS_CHG_START, /**< Next: 下一状态 */
    },
};

const HvProcess_StateConfigType HvProcess_ChgStateConfig[HVPROCESS_CHG_STATE_MAX] = {
    {
        (uint8)ARRAY_SIZE(HvProcess_ChgStartInfoConfig), /**< Num: 状态配置个数 */
        HvProcess_ChgStartInfoConfig, /**< State: 状态信息指针 */
    },
    {
        (uint8)ARRAY_SIZE(HvProcess_ChgHvOnInfoConfig), /**< Num: 状态配置个数 */
        HvProcess_ChgHvOnInfoConfig, /**< State: 状态信息指针 */
    },
    {
        (uint8)ARRAY_SIZE(HvProcess_ChgRelayOffDelayInfoConfig), /**< Num: 状态配置个数 */
        HvProcess_ChgRelayOffDelayInfoConfig, /**< State: 状态信息指针 */
    },
};
