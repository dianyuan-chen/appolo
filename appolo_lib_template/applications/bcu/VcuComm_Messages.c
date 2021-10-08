/**
 * \file VcuComm_messages.c
 * \copyright UDAN Co.,Ltd. *
 *
 * \brief 整车通信报文文件.
 *
 * * \par 修订历史:
 * | 版本号 | 修订日志 | 修改人 | 修订时间 |
 * | :--- | :--- | :--- | :--- |
 * | 0.1 | 初始版本, 完成讨论部分的定义. | UD00004 | 20170316 |
 */
#include "App_Types.h"
#include "Cpu.h"
#include "Det.h"
#include "VcuComm_Types.h"
#include "VcuComm_Messages.h"
#include "CurrentM.h"
#include "Statistic.h"
#include "Soc.h"
#include "Soh.h"
#include "Insu.h"
#include "ChargeConnectM.h"
#include "ChargerComm.h"
#include "HWDiagnosis.h"
#include "AppInfo.h"
#include "ParameterM.h"
#include "CellDataM.h"
#include "RelayM.h"
#include "RelayM_Lcfg.h"
#if ( VCUCOMM_DEV_ERROR_DETECT == STD_ON )
#include "Modules.h"
#endif

#define VCUCOMM_MESSAGES_E_PARAM_INVALID_PTR            0U

#define VCUCOMM_MESSAGES_API_ID_ReceiveCbk              0U
#define VCUCOMM_MESSAGES_API_ID_GetMsgData0x1801FFF4Cbk 1U
#define VCUCOMM_MESSAGES_API_ID_GetMsgData0x1802FFF4Cbk 2U
#define VCUCOMM_MESSAGES_API_ID_GetMsgData0x1803FFF4Cbk 3U
#define VCUCOMM_MESSAGES_API_ID_GetMsgData0x1804FFF4Cbk 4U
#define VCUCOMM_MESSAGES_API_ID_GetMsgData0x1807FFF4Cbk 5U
#define VCUCOMM_MESSAGES_API_ID_GetMsgData0x1801D2F4Cbk 6U
#define VCUCOMM_MESSAGES_API_ID_GetMsgData0x1802D2F4Cbk 7U
#define VCUCOMM_MESSAGES_API_ID_GetMsgData0x1803D2F4Cbk 8U
#define VCUCOMM_MESSAGES_API_ID_GetMsgData0x1804D2F4Cbk 9U
#define VCUCOMM_MESSAGES_API_ID_GetMsgData0x1805D2F4Cbk 10U
#define VCUCOMM_MESSAGES_API_ID_GetMsgData0x1806D2F4Cbk 11U
#define VCUCOMM_MESSAGES_API_ID_GetMsgData0x1807D2F4Cbk 12U
#define VCUCOMM_MESSAGES_API_ID_GetMsgData0x1808D2F4Cbk 13U
#define VCUCOMM_MESSAGES_API_ID_GetMsgData0x1809D2F4Cbk 14U
#define VCUCOMM_MESSAGES_API_ID_GetMsgData0x180AD2F4Cbk 15U
#define VCUCOMM_MESSAGES_API_ID_GetMsgData0x1850D2F4Cbk 16U


#if ( VCUCOMM_DEV_ERROR_DETECT == STD_ON )
#define VALIDATE_PTR(_ptr, _api) \
    if (_ptr == NULL) { \
        Det_ReportError(MODULE_ID_VCUCOMM_MESSAGES, 0U, _api, VCUCOMM_MESSAGES_E_PARAM_INVALID_PTR); \
        goto cleanup; \
    }
#else
#define VALIDATE_PTR(_ptr, _api)
#endif

#define VCUCOMM_CURRENT_OFFSET      (32000)
#define VCUCOMM_VOLT_4_DISPLAY(volt)    (VOLT_4_DISPLAY(volt) < 10000U ? VOLT_4_DISPLAY(volt) : 10000U)

VcuComm_MsgDataType VcuComm_MsgData;

static uint16 VcuComm_bms2VcuCurrent(Current_CurrentType current);

void VcuComm_MessageInit(Async_LooperType *looper)
{
    (void)looper;
    (void)VcuComm_SendIPduRegister(&VcuComm_IPduSendConfigInfo[0], VcuComm_SendIPduNum);
}

void VcuComm_GetMsgData0x1801FFF4Cbk(uint8 *Buffer, uint16 *Length)
{
    Current_CurrentType current;
    Diagnosis_LevelType level;
    uint16 index = 0U, temp;

    VALIDATE_PTR(Buffer, VCUCOMM_MESSAGES_API_ID_GetMsgData0x1801FFF4Cbk);
    VALIDATE_PTR(Length, VCUCOMM_MESSAGES_API_ID_GetMsgData0x1801FFF4Cbk);
    /**< 总压 */
    temp = Statistic_GetBcu100mvTotalVoltage();
    temp = VOLT_4_DISPLAY(temp);
    WRITE_LT_UINT16(Buffer, index, temp);
    /**< 总电流 */
    current = CurrentM_GetCurrentCalibrated(CURRENTM_CHANNEL_MAIN);
    temp = VcuComm_bms2VcuCurrent(current);
    WRITE_LT_UINT16(Buffer, index, temp);
    /**< SOC */
    WRITE_LT_UINT16(Buffer, index,  Soc_Get());
    temp = 0U;
    /**< 单体过充 */
    if (Diagnosis_GetLevel(DIAGNOSIS_ITEM_CHG_HV) >= DIAGNOSIS_LEVEL_SECOND)
    {
        temp |= ((uint16)1U << 0);
    }
    else if (Diagnosis_GetLevel(DIAGNOSIS_ITEM_DCHG_HV) >= DIAGNOSIS_LEVEL_SECOND)
    {
        temp |= ((uint16)1U << 0);
    }
    else
    {
    }
    /**< 单体过放 */
    if (Diagnosis_GetLevel(DIAGNOSIS_ITEM_CHG_LV) >= DIAGNOSIS_LEVEL_SECOND)
    {
        temp |= ((uint16)1U << 1);
    }
    else if (Diagnosis_GetLevel(DIAGNOSIS_ITEM_DCHG_LV) >= DIAGNOSIS_LEVEL_SECOND)
    {
        temp |= ((uint16)1U << 1);
    }
    else
    {
    }
    /**< 温度过高 */
    if (Diagnosis_GetLevel(DIAGNOSIS_ITEM_CHG_HT) >= DIAGNOSIS_LEVEL_SECOND)
    {
        temp |= ((uint16)1U << 2);
    }
    else if (Diagnosis_GetLevel(DIAGNOSIS_ITEM_DCHG_HT) >= DIAGNOSIS_LEVEL_SECOND)
    {
        temp |= ((uint16)1U << 2);
    }
    else
    {
    }
    /**< 放电过流 */
    if (Diagnosis_GetLevel(DIAGNOSIS_ITEM_DCHG_OC) >= DIAGNOSIS_LEVEL_SECOND)
    {
        temp |= ((uint16)1U << 3);
    }
    else if (Diagnosis_GetLevel(DIAGNOSIS_ITEM_SP_OC) >= DIAGNOSIS_LEVEL_SECOND)
    {
        temp |= ((uint16)1U << 3);
    }
    else
    {
    }

    level = Diagnosis_GetLevel(DIAGNOSIS_ITEM_LEAK);
    /**< 轻微漏电 */
    if (level == DIAGNOSIS_LEVEL_FIRST)
    {
        temp |= ((uint16)1U << 5);
    }
    /**< 严重漏电 */
    if (level >= DIAGNOSIS_LEVEL_SECOND)
    {
        temp |= ((uint16)1U << 6);
    }
    /**< 通信中断 */
    if (Diagnosis_GetLevel(DIAGNOSIS_ITEM_INTER_COMM) >= DIAGNOSIS_LEVEL_FIRST)
    {
        temp |= ((uint16)1U << 7);
    }
    /**< 总压过高 */
    if (Diagnosis_GetLevel(DIAGNOSIS_ITEM_CHG_HTV) >= DIAGNOSIS_LEVEL_SECOND)
    {
        temp |= ((uint16)1U << 8);
    }
    else if (Diagnosis_GetLevel(DIAGNOSIS_ITEM_DCHG_HTV) >= DIAGNOSIS_LEVEL_SECOND)
    {
        temp |= ((uint16)1U << 8);
    }
    else
    {
    }
    /**< 总压过低 */
    if (Diagnosis_GetLevel(DIAGNOSIS_ITEM_CHG_LTV) >= DIAGNOSIS_LEVEL_SECOND)
    {
        temp |= ((uint16)1U << 9);
    }
    else if (Diagnosis_GetLevel(DIAGNOSIS_ITEM_DCHG_LTV) >= DIAGNOSIS_LEVEL_SECOND)
    {
        temp |= ((uint16)1U << 9);
    }
    else
    {
    }
    /**< 充电过流 */
    if (Diagnosis_GetLevel(DIAGNOSIS_ITEM_AC_CHG_OC) >= DIAGNOSIS_LEVEL_SECOND)
    {
        temp |= ((uint16)1U << 10);
    }
    else if (Diagnosis_GetLevel(DIAGNOSIS_ITEM_DC_CHG_OC) >= DIAGNOSIS_LEVEL_SECOND)
    {
        temp |= ((uint16)1U << 10);
    }
    else
    {
    }
    /**< 温度过低 */
    if (Diagnosis_GetLevel(DIAGNOSIS_ITEM_CHG_LT) >= DIAGNOSIS_LEVEL_SECOND)
    {
        temp |= ((uint16)1U << 11);
    }
    else if (Diagnosis_GetLevel(DIAGNOSIS_ITEM_DCHG_LT) >= DIAGNOSIS_LEVEL_SECOND)
    {
        temp |= ((uint16)1U << 11);
    }
    else
    {
    }
    /**< 温差过大 */
    if (Diagnosis_GetLevel(DIAGNOSIS_ITEM_CHG_DT) >= DIAGNOSIS_LEVEL_FIRST)
    {
        temp |= ((uint16)1U << 12);
    }
    else if (Diagnosis_GetLevel(DIAGNOSIS_ITEM_DCHG_DT) >= DIAGNOSIS_LEVEL_FIRST)
    {
        temp |= ((uint16)1U << 12);
    }
    else
    {
    }
    /**< 压差过大 */
    if (Diagnosis_GetLevel(DIAGNOSIS_ITEM_CHG_DV) >= DIAGNOSIS_LEVEL_FIRST)
    {
        temp |= ((uint16)1U << 13);
    }
    else if (Diagnosis_GetLevel(DIAGNOSIS_ITEM_DCHG_DV) >= DIAGNOSIS_LEVEL_FIRST)
    {
        temp |= ((uint16)1U << 13);
    }
    else
    {
    }
    /**< 放电过流 */

    WRITE_LT_UINT16(Buffer, index, temp);

    *Length = index;
#if ( VCUCOMM_DEV_ERROR_DETECT == STD_ON )
cleanup:
#endif
    return;
}

void VcuComm_GetMsgData0x1802FFF4Cbk(uint8 *Buffer, uint16 *Length)
{
    uint16 temp;
    uint16 index = 0U;

    VALIDATE_PTR(Buffer, VCUCOMM_MESSAGES_API_ID_GetMsgData0x1802FFF4Cbk);
    VALIDATE_PTR(Length, VCUCOMM_MESSAGES_API_ID_GetMsgData0x1802FFF4Cbk);
    /**< 最高单体电压 */
    temp = Statistic_GetBcuHvMax();
    temp = VCUCOMM_VOLT_4_DISPLAY(temp);
    WRITE_LT_UINT16(Buffer, index, temp);
    /**< 最高单体电压串号 */
    temp = CellDataM_VoltageIndexToLogic(Statistic_GetBcuHvSlaveNum(0U), Statistic_GetBcuHvIndex(0U)) + 1U;
    if (temp == 0xFFFFU)
    {
        temp = 0U;
    }
    WRITE_LT_UINT16(Buffer, index, temp);
    /**< 最低单体电压 */
    temp = Statistic_GetBcuLvMax();
    temp = VCUCOMM_VOLT_4_DISPLAY(temp);
    WRITE_LT_UINT16(Buffer, index, temp);
    /**< 最低单体电压串号 */
    temp = CellDataM_VoltageIndexToLogic(Statistic_GetBcuLvSlaveNum(0U), Statistic_GetBcuLvIndex(0U)) + 1U;
    if (temp == 0xFFFFU)
    {
        temp = 0U;
    }
    WRITE_LT_UINT16(Buffer, index, temp);

    *Length = index;
#if ( VCUCOMM_DEV_ERROR_DETECT == STD_ON )
cleanup:
#endif
    return;
}

void VcuComm_GetMsgData0x1803FFF4Cbk(uint8 *Buffer, uint16 *Length)
{
    uint16 temp;
    uint32 resistor;
    uint16 index = 0U;

    VALIDATE_PTR(Buffer, VCUCOMM_MESSAGES_API_ID_GetMsgData0x1803FFF4Cbk);
    VALIDATE_PTR(Length, VCUCOMM_MESSAGES_API_ID_GetMsgData0x1803FFF4Cbk);

    /**< 最高温度 */
    temp = Statistic_GetBcuHtMax();
    temp = TEMP_TO_40_OFFSET(temp);
    temp = TEMP_4_DISPLAY(temp);
    WRITE_LT_UINT8(Buffer, index, temp);
    /**< 最高温度串号 */
    temp = CellDataM_TemperatureIndexToLogic(Statistic_GetBcuHtSlaveNum(0U), Statistic_GetBcuHtIndex(0U)) + 1U;
    if (temp == 0xFFFFU)
    {
        temp = 0U;
    }
    WRITE_LT_UINT16(Buffer, index, temp);
    /**< 最低温度 */
    temp = Statistic_GetBcuLtMax();
    temp = TEMP_TO_40_OFFSET(temp);
    temp = TEMP_4_DISPLAY(temp);
    WRITE_LT_UINT8(Buffer, index, temp);
    /**< 最低温度串号 */
    temp = CellDataM_TemperatureIndexToLogic(Statistic_GetBcuLtSlaveNum(0U), Statistic_GetBcuLtIndex(0U)) + 1U;
    if (temp == 0xFFFFU)
    {
        temp = 0U;
    }
    WRITE_LT_UINT16(Buffer, index, temp);
    /**< 正极绝缘阻值 */
    temp = Insu_GetPositive();
    if (Insu_ResIsValid(temp) == FALSE)
    {
        temp = 0xFFFFU;
    }
    else
    {
        resistor = (uint32)temp * 10U;
        if (resistor > 0xFFFFU)
        {
            temp = 0xFFFFU;
        }
        else
        {
            temp = (uint16)resistor;
        }
    }
    WRITE_LT_UINT16(Buffer, index, temp);

    *Length = index;
#if ( VCUCOMM_DEV_ERROR_DETECT == STD_ON )
cleanup:
#endif
    return;
}

void VcuComm_GetMsgData0x1804FFF4Cbk(uint8 *Buffer, uint16 *Length)
{
    uint16 temp;
    uint32 resistor;
    uint16 index = 0U;

    VALIDATE_PTR(Buffer, VCUCOMM_MESSAGES_API_ID_GetMsgData0x1804FFF4Cbk);
    VALIDATE_PTR(Length, VCUCOMM_MESSAGES_API_ID_GetMsgData0x1804FFF4Cbk);

    /**< 负极绝缘阻值 */
    temp = Insu_GetNegative();
    if (Insu_ResIsValid(temp) == FALSE)
    {
        temp = 0xFFFFU;
    }
    else
    {
        resistor = (uint32)temp * 10U;
        if (resistor > 0xFFFFU)
        {
            temp = 0xFFFFU;
        }
        else
        {
            temp = (uint16)resistor;
        }
    }
    WRITE_LT_UINT16(Buffer, index, temp);
    /**< SOH */
    temp = Soh_Get();
    WRITE_LT_UINT16(Buffer, index, temp);
    temp = 0U;
#ifdef RELAYM_FN_POSITIVE_MAIN
    /**< 放电继电器状态 */
    if (RelayM_GetActualStatus(RELAYM_FN_POSITIVE_MAIN) == RELAYM_ACTUAL_ON)
    {
        temp |= ((uint16)1U << 0);
    }
#endif
    /**< 充电继电器状态 */
#ifdef RELAYM_FN_POSITIVE_AC_CHARGE
    if (RelayM_GetActualStatus(RELAYM_FN_POSITIVE_AC_CHARGE) == RELAYM_ACTUAL_ON)
    {
        temp |= ((uint16)1U << 1);
    }
#endif
#ifdef RELAYM_FN_POSITIVE_DC_CHARGE
    if (RelayM_GetActualStatus(RELAYM_FN_POSITIVE_DC_CHARGE) == RELAYM_ACTUAL_ON)
    {
        temp |= ((uint16)1U << 1);
    }
#endif
    /**< 预充继电器状态 */
#ifdef RELAYM_FN_PRECHARGE
    if (RelayM_GetActualStatus(RELAYM_FN_PRECHARGE) == RELAYM_ACTUAL_ON)
    {
        temp |= ((uint16)1U << 2);
    }
#endif
    /**< 充电连接状态 */
    if (CHARGECONNECTM_IS_CONNECT())
    {
        temp |= ((uint16)1U << 3);
        /**< 充电状态 */
        if (ChargerComm_GetChargingStatus() == TRUE)
        {
            /**< 总电流 */
            uint16 current = CurrentM_GetMainDiagChgCurrent();
            if (CurrentM_DiagIsValidCurrent(current) &&
                current >= CURRENT_100MA_FROM_A(2U))
            {
                temp |= ((uint16)1U << 4);
            }
        }
    }
    /**< BMS系统故障 */
    if (HWDiagnosis_IsHwFault() == TRUE)
    {
        temp |= ((uint16)1U << 5);
    }
    else if (Diagnosis_GetLevel(DIAGNOSIS_ITEM_VOLT_LINE) >= DIAGNOSIS_LEVEL_FIRST)
    {
        temp |= ((uint16)1U << 5);
    }
    else if (Diagnosis_GetLevel(DIAGNOSIS_ITEM_TEMP_LINE) >= DIAGNOSIS_LEVEL_FIRST)
    {
        temp |= ((uint16)1U << 5);
    }
    else if (Diagnosis_GetLevel(DIAGNOSIS_ITEM_CHG_DTV) >= DIAGNOSIS_LEVEL_SECOND)
    {
        temp |= ((uint16)1U << 5);
    }
    else if (Diagnosis_GetLevel(DIAGNOSIS_ITEM_DCHG_DTV) >= DIAGNOSIS_LEVEL_SECOND)
    {
        temp |= ((uint16)1U << 5);
    }
    else
    {
    }
    /**< 继电器粘连 */

    WRITE_LT_UINT8(Buffer, index, temp);
    /**< 充放电次数 */
    temp = 0U;
    (void)ParameterM_EeepRead(PARAMETERM_EEEP_DISCHARGE_CNT_INDEX, &temp);
    WRITE_LT_UINT16(Buffer, index, temp);
    /**< 保留 */
    WRITE_LT_UINT8(Buffer, index, 0xFFU);

    *Length = index;
#if ( VCUCOMM_DEV_ERROR_DETECT == STD_ON )
cleanup:
#endif
    return;
}

void VcuComm_GetMsgData0x1807FFF4Cbk(uint8 *Buffer, uint16 *Length)
{
    uint16 temp;
    uint16 index = 0U;

    VALIDATE_PTR(Buffer, VCUCOMM_MESSAGES_API_ID_GetMsgData0x1807FFF4Cbk);
    VALIDATE_PTR(Length, VCUCOMM_MESSAGES_API_ID_GetMsgData0x1807FFF4Cbk);

    /**< 车型 */
    WRITE_LT_UINT8(Buffer, index,  0x02U);
    /**< 生产厂商 */
    WRITE_LT_UINT8(Buffer, index,  0x01U);
    /**< 硬件号 */
    WRITE_LT_UINT8(Buffer, index,  0x01U);
    /**< 软件号 */
    WRITE_LT_UINT8(Buffer, index,  0x01U);
    /**< 年 */
#if APP_INFO_BUILD_DATE_Y >= 2000U
    temp = APP_INFO_BUILD_DATE_Y - 2000U;
#else
    temp = 0U;
#endif
    WRITE_LT_UINT8(Buffer, index,  temp);
    /**< 月 */
    WRITE_LT_UINT8(Buffer, index,  APP_INFO_BUILD_DATE_M);
    /**< 日 */
    WRITE_LT_UINT8(Buffer, index,  APP_INFO_BUILD_DATE_D);
    /**< 备注：1-加热 */
    WRITE_LT_UINT8(Buffer, index,  0x01U);

    *Length = index;
#if ( VCUCOMM_DEV_ERROR_DETECT == STD_ON )
cleanup:
#endif
    return;
}

void VcuComm_GetMsgData0x1801D2F4Cbk(uint8 *Buffer, uint16 *Length)
{
    CellDataM_CellLogicIndexType start = 0U, stop = 3U;
    App_VoltageType volt;
    uint16 index = 0U;

    VALIDATE_PTR(Buffer, VCUCOMM_MESSAGES_API_ID_GetMsgData0x1801D2F4Cbk);
    VALIDATE_PTR(Length, VCUCOMM_MESSAGES_API_ID_GetMsgData0x1801D2F4Cbk);

    /**< 单体电压1~4 */
    for (; start <= stop; start++)
    {
        volt = CellDataM_GetVoltage(start);
        volt = VCUCOMM_VOLT_4_DISPLAY(volt);
        WRITE_LT_UINT16(Buffer, index, volt);
    }

    *Length = index;
#if ( VCUCOMM_DEV_ERROR_DETECT == STD_ON )
cleanup:
#endif
    return;
}

void VcuComm_GetMsgData0x1802D2F4Cbk(uint8 *Buffer, uint16 *Length)
{
    CellDataM_CellLogicIndexType start = 4U, stop = 7U;
    App_VoltageType volt;
    uint16 index = 0U;

    VALIDATE_PTR(Buffer, VCUCOMM_MESSAGES_API_ID_GetMsgData0x1802D2F4Cbk);
    VALIDATE_PTR(Length, VCUCOMM_MESSAGES_API_ID_GetMsgData0x1802D2F4Cbk);

    /**< 单体电压5~8 */
    for (; start <= stop; start++)
    {
        volt = CellDataM_GetVoltage(start);
        volt = VCUCOMM_VOLT_4_DISPLAY(volt);
        WRITE_LT_UINT16(Buffer, index, volt);
    }

    *Length = index;
#if ( VCUCOMM_DEV_ERROR_DETECT == STD_ON )
cleanup:
#endif
    return;
}

void VcuComm_GetMsgData0x1803D2F4Cbk(uint8 *Buffer, uint16 *Length)
{
    CellDataM_CellLogicIndexType start = 8U, stop = 11U;
    App_VoltageType volt;
    uint16 index = 0U;

    VALIDATE_PTR(Buffer, VCUCOMM_MESSAGES_API_ID_GetMsgData0x1803D2F4Cbk);
    VALIDATE_PTR(Length, VCUCOMM_MESSAGES_API_ID_GetMsgData0x1803D2F4Cbk);

    /**< 单体电压9~12 */
    for (; start <= stop; start++)
    {
        volt = CellDataM_GetVoltage(start);
        volt = VCUCOMM_VOLT_4_DISPLAY(volt);
        WRITE_LT_UINT16(Buffer, index, volt);
    }

    *Length = index;
#if ( VCUCOMM_DEV_ERROR_DETECT == STD_ON )
cleanup:
#endif
    return;
}

void VcuComm_GetMsgData0x1804D2F4Cbk(uint8 *Buffer, uint16 *Length)
{
    CellDataM_CellLogicIndexType start = 12U, stop = 15U;
    App_VoltageType volt;
    uint16 index = 0U;

    VALIDATE_PTR(Buffer, VCUCOMM_MESSAGES_API_ID_GetMsgData0x1804D2F4Cbk);
    VALIDATE_PTR(Length, VCUCOMM_MESSAGES_API_ID_GetMsgData0x1804D2F4Cbk);

    /**< 单体电压13~16 */
    for (; start <= stop; start++)
    {
        volt = CellDataM_GetVoltage(start);
        volt = VCUCOMM_VOLT_4_DISPLAY(volt);
        WRITE_LT_UINT16(Buffer, index, volt);
    }

    *Length = index;
#if ( VCUCOMM_DEV_ERROR_DETECT == STD_ON )
cleanup:
#endif
    return;
}

void VcuComm_GetMsgData0x1805D2F4Cbk(uint8 *Buffer, uint16 *Length)
{
    CellDataM_CellLogicIndexType start = 16U, stop = 19U;
    App_VoltageType volt;
    uint16 index = 0U;

    VALIDATE_PTR(Buffer, VCUCOMM_MESSAGES_API_ID_GetMsgData0x1805D2F4Cbk);
    VALIDATE_PTR(Length, VCUCOMM_MESSAGES_API_ID_GetMsgData0x1805D2F4Cbk);

    /**< 单体电压17~20 */
    for (; start <= stop; start++)
    {
        volt = CellDataM_GetVoltage(start);
        volt = VCUCOMM_VOLT_4_DISPLAY(volt);
        WRITE_LT_UINT16(Buffer, index, volt);
    }

    *Length = index;
#if ( VCUCOMM_DEV_ERROR_DETECT == STD_ON )
cleanup:
#endif
    return;
}

void VcuComm_GetMsgData0x1806D2F4Cbk(uint8 *Buffer, uint16 *Length)
{
    CellDataM_CellLogicIndexType start = 20U, stop = 23U;
    App_VoltageType volt;
    uint16 index = 0U;

    VALIDATE_PTR(Buffer, VCUCOMM_MESSAGES_API_ID_GetMsgData0x1806D2F4Cbk);
    VALIDATE_PTR(Length, VCUCOMM_MESSAGES_API_ID_GetMsgData0x1806D2F4Cbk);

    /**< 单体电压21~24 */
    for (; start <= stop; start++)
    {
        volt = CellDataM_GetVoltage(start);
        volt = VCUCOMM_VOLT_4_DISPLAY(volt);
        WRITE_LT_UINT16(Buffer, index, volt);
    }

    *Length = index;
#if ( VCUCOMM_DEV_ERROR_DETECT == STD_ON )
cleanup:
#endif
    return;
}

void VcuComm_GetMsgData0x1807D2F4Cbk(uint8 *Buffer, uint16 *Length)
{
    CellDataM_CellLogicIndexType start = 24U, stop = 27U;
    App_VoltageType volt;
    uint16 index = 0U;

    VALIDATE_PTR(Buffer, VCUCOMM_MESSAGES_API_ID_GetMsgData0x1807D2F4Cbk);
    VALIDATE_PTR(Length, VCUCOMM_MESSAGES_API_ID_GetMsgData0x1807D2F4Cbk);

    /**< 单体电压25~28 */
    for (; start <= stop; start++)
    {
        volt = CellDataM_GetVoltage(start);
        volt = VCUCOMM_VOLT_4_DISPLAY(volt);
        WRITE_LT_UINT16(Buffer, index, volt);
    }

    *Length = index;
#if ( VCUCOMM_DEV_ERROR_DETECT == STD_ON )
cleanup:
#endif
    return;
}

void VcuComm_GetMsgData0x1808D2F4Cbk(uint8 *Buffer, uint16 *Length)
{
    CellDataM_CellLogicIndexType start = 28U, stop = 31U;
    App_VoltageType volt;
    uint16 index = 0U;

    VALIDATE_PTR(Buffer, VCUCOMM_MESSAGES_API_ID_GetMsgData0x1808D2F4Cbk);
    VALIDATE_PTR(Length, VCUCOMM_MESSAGES_API_ID_GetMsgData0x1808D2F4Cbk);

    /**< 单体电压29~32 */
    for (; start <= stop; start++)
    {
        volt = CellDataM_GetVoltage(start);
        volt = VCUCOMM_VOLT_4_DISPLAY(volt);
        WRITE_LT_UINT16(Buffer, index, volt);
    }

    *Length = index;
#if ( VCUCOMM_DEV_ERROR_DETECT == STD_ON )
cleanup:
#endif
    return;
}

void VcuComm_GetMsgData0x1809D2F4Cbk(uint8 *Buffer, uint16 *Length)
{
    CellDataM_CellLogicIndexType start = 32U, stop = 35U;
    App_VoltageType volt;
    uint16 index = 0U;

    VALIDATE_PTR(Buffer, VCUCOMM_MESSAGES_API_ID_GetMsgData0x1809D2F4Cbk);
    VALIDATE_PTR(Length, VCUCOMM_MESSAGES_API_ID_GetMsgData0x1809D2F4Cbk);

    /**< 单体电压33~36 */
    for (; start <= stop; start++)
    {
        volt = CellDataM_GetVoltage(start);
        volt = VCUCOMM_VOLT_4_DISPLAY(volt);
        WRITE_LT_UINT16(Buffer, index, volt);
    }

    *Length = index;
#if ( VCUCOMM_DEV_ERROR_DETECT == STD_ON )
cleanup:
#endif
    return;
}

void VcuComm_GetMsgData0x180AD2F4Cbk(uint8 *Buffer, uint16 *Length)
{
    CellDataM_CellLogicIndexType start = 36U, stop = 39U;
    App_VoltageType volt;
    uint16 index = 0U;

    VALIDATE_PTR(Buffer, VCUCOMM_MESSAGES_API_ID_GetMsgData0x180AD2F4Cbk);
    VALIDATE_PTR(Length, VCUCOMM_MESSAGES_API_ID_GetMsgData0x180AD2F4Cbk);

    /**< 单体电压37~40 */
    for (; start <= stop; start++)
    {
        volt = CellDataM_GetVoltage(start);
        volt = VCUCOMM_VOLT_4_DISPLAY(volt);
        WRITE_LT_UINT16(Buffer, index, volt);
    }

    *Length = index;
#if ( VCUCOMM_DEV_ERROR_DETECT == STD_ON )
cleanup:
#endif
    return;
}

void VcuComm_GetMsgData0x1850D2F4Cbk(uint8 *Buffer, uint16 *Length)
{
    CellDataM_CellLogicIndexType start = 0U, stop = 7U;
    App_TemperatureType temp;
    uint16 index = 0U;

    VALIDATE_PTR(Buffer, VCUCOMM_MESSAGES_API_ID_GetMsgData0x1850D2F4Cbk);
    VALIDATE_PTR(Length, VCUCOMM_MESSAGES_API_ID_GetMsgData0x1850D2F4Cbk);

    /**< 单体温度1~8 */
    for (; start <= stop; start++)
    {
        temp = CellDataM_GetTemperature(start);
        temp = (App_TemperatureType)TEMP_TO_40_OFFSET(temp);
        temp = TEMP_4_DISPLAY(temp);
        WRITE_LT_UINT8(Buffer, index, temp);
    }

    *Length = index;
#if ( VCUCOMM_DEV_ERROR_DETECT == STD_ON )
cleanup:
#endif
    return;
}


void VcuComm_ReceiveCbk(uint8 *Buffer, uint16 Length)
{
    VALIDATE_PTR(Buffer, VCUCOMM_MESSAGES_API_ID_ReceiveCbk);

    VcuComm_SetCommunicationStatus(TRUE);
    VcuComm_ClrCommAbortMessageFlag((uint16)VCUCOMM_RX_IPDU_MSG_0x12345678);
    (void)Buffer;
    (void)Length;

#if ( VCUCOMM_DEV_ERROR_DETECT == STD_ON )
cleanup:
#endif
    return;
}

void VcuComm_RecTimeoutCbk(void)
{
    VcuComm_MsgData.PowerCommand = VCUCOMM_POWER_OFF;
    VcuComm_SetCommunicationStatus(FALSE);
    VcuComm_SetCommAbortMessageFlag((uint16)VCUCOMM_RX_IPDU_MSG_0x12345678);
}



// Condition Check function define
Std_ReturnType VcuComm_SendConditionCheck(uint16 IPdu)
{
    Std_ReturnType res = E_NOT_OK;

    (void)IPdu;
    if (VcuComm_GetCurrentRecStage() == (uint16)VCUCOMM_STAGE_STAGE1)
    {
        res = E_OK;
    }
    return res;
}

Std_ReturnType VcuComm_RecConditionCheck(void)
{
    Std_ReturnType res = E_NOT_OK;
    uint16 stage;

    stage = VcuComm_GetCurrentRecStage();
    if (stage == (uint16)VCUCOMM_STAGE_STAGE1)
    {
        res = E_OK;
    }
    return res;
}

static uint16 VcuComm_bms2VcuCurrent(Current_CurrentType current)
{
    uint16 vcu_current;
    Current_CurrentType cur_offset = VCUCOMM_CURRENT_OFFSET;

    if (!CurrentM_IsValidCurrent(current))
    {
        current = 0;
    }
    if (current < (-1) * cur_offset)
    {
        vcu_current = 0U;
    }
    else if (current > cur_offset)
    {
        vcu_current = (uint16)cur_offset * 2U;
    }
    else
    {
        vcu_current = (uint16)current + (uint16)cur_offset;
    }
    return vcu_current;
}

