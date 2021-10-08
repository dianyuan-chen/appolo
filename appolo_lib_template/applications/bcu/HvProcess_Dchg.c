/**
 * \file HvProcess_Dchg.c
 * \copyright UDAN Co.,Ltd. *
 *
 * \brief 高压放电流程控制文件.
 *
 * * \par 修订历史:
 * | 版本号 | 修订日志 | 修改人 | 修订时间 |
 * | :--- | :--- | :--- | :--- |
 * | 0.1 | 初始版本, 完成讨论部分的定义. | UD00004 | 20170320 |
 */
#include "Cpu.h"
#include "HvProcess_Dchg.h"
#include "HvProcess_Chg.h"
#include "ChargeConnectM.h"
#include "ChargeM.h"
#include "DischargeM.h"
#include "RelayM.h"
#include "RelayM_Lcfg.h"
#include "PrechargeM.h"

static HvProcess_DchgInnerDataType HvProcess_DchgInnerData;

void HvProcess_DchgInit(Async_LooperType *looper)
{
    (void)looper;
    HvProcess_DchgInnerData.State = HVPROCESS_DCHG_START;
}

HvProcess_DchgStateType HvProcess_GetDchgState(void)
{
    HvProcess_DchgStateType state;
    imask_t mask;

    Irq_Save(mask);
    state = HvProcess_DchgInnerData.State;
    Irq_Restore(mask);

    return state;
}

void HvProcess_SetDchgState(HvProcess_DchgStateType state)
{
    imask_t mask;

    Irq_Save(mask);
    HvProcess_DchgInnerData.State = state;
    Irq_Restore(mask);
}

void HvProcess_DchgPoll(void)
{

}

boolean HvProcess_DchgStateStartCond(void)
{
    boolean res = FALSE;
    Std_ReturnType allow;
    HvProcess_ChgStateType chgState;

    chgState = HvProcess_GetChgState();
    allow = DischargeM_DischargeIsAllowed();

    if ((!CHARGECONNECTM_IS_CONNECT()) &&
        chgState == HVPROCESS_CHG_START &&
        allow == E_OK)
    {
        res = TRUE;
    }
    return res;
}

void HvProcess_DchgStateStartAction(void)
{
    PrechargeM_Start();
}

boolean HvProcess_DchgStatePrechargeCond(void)
{
    boolean res = FALSE;

    if (PrechargeM_IsFinish() == TRUE)
    {
        res = TRUE;
    }
    return res;
}

void HvProcess_DchgStatePrechargeAction(void)
{
    (void)RelayM_Control(RELAYM_FN_POSITIVE_MAIN, RELAYM_CONTROL_ON);
}

boolean HvProcess_DchgChargeConnectionCond(void)
{
    boolean res = FALSE;

    if (CHARGECONNECTM_IS_CONNECT())
    {
        res = TRUE;
    }
    return res;
}

void HvProcess_DchgChargeConnectionAction(void)
{
    HvProcess_DchgInnerData.RelayOffTick = OSTimeGet();
    PrechargeM_Stop();
}

boolean HvProcess_DchgFaultCond(void)
{
    boolean res = FALSE;

    if (DischargeM_DischargeIsFault() == E_OK)
    {
        res = TRUE;
    }
    return res;
}

void HvProcess_DchgFaultAction(void)
{
    HvProcess_DchgInnerData.RelayOffTick = OSTimeGet();
    PrechargeM_Stop();
}

boolean HvProcess_DchgRelayOffDelayCond(void)
{
    boolean res = FALSE;
    uint32 nowTime = OSTimeGet();

    if (MS_GET_INTERNAL(HvProcess_DchgInnerData.RelayOffTick, nowTime) >= 2000U)
    {
        res = TRUE;
        HvProcess_DchgInnerData.RelayOffTick = nowTime;
    }
    return res;
}

void HvProcess_DchgRelayOffDelayAction(void)
{
    (void)RelayM_Control(RELAYM_FN_POSITIVE_MAIN, RELAYM_CONTROL_OFF);
}
