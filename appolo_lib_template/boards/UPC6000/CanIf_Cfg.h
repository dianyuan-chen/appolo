#ifndef _CANIF_CFG_H_
#define _CANIF_CFG_H_

#include "Can.h"
#include "ComStack_Types.h"
#include "Can_Cfg.h"

#define CANIF_FILTER_TYPE_INDEX        2
#define CANIF_FILTER_TYPE_LINEAR       3

/* CanIfDispatchCfg */
#define CANIF_DISPATCH_UL_CDD          0
#define CANIF_DISPATCH_UL_CAN_SM       1
#define CANIF_DISPATCH_UL_ECUM         2

/* CanIf Dlc Check Type*/
#define CANIF_DLC_CHECK_AUTOSAR        0
#define CANIF_DLC_CHECK_UNEQUAL        1
#define CANIF_DLC_CHECK_USER           2


extern const uint8 CanIfTotalRxPduId;
extern const uint8 CanifTotalTxPduid;

#define CANIF_PRIVATE_DLC_CHECK             STD_OFF
#define CANIF_DLC_CHECK_TYPE                CANIF_DLC_CHECK_AUTOSAR
#define CANIF_DLC_CHECK_FAIL_INDICATION     STD_OFF
#define CANIF_PRIVATE_SOFTWARE_FILTER_TYPE             CANIF_FILTER_TYPE_INDEX

#define CANIF_PUBLIC_DEV_ERROR_DETECT             STD_OFF
#define CANIF_PUBLIC_TX_BUFFERING                 STD_ON
#define CANIF_PUBLIC_CANCEL_TRANSMIT_SUPPORT      STD_ON
#define CANIF_PUBLIC_SETDYNAMICTXID_API           STD_OFF
#define CANIF_PUBLIC_READRXPDU_DATA_API           STD_OFF
#define CANIF_PUBLIC_READRXPDU_NOTIFY_STATUS_API         STD_OFF
#define CANIF_PUBLIC_READTXPDU_NOTIFY_STATUS_API         STD_OFF
#define CANIF_PUBLIC_VERSION_INFO_API                    STD_OFF
#define CANIF_USED_CONTROLLER_NUM         CAN_USED_CONTROLLER_NUM
#define CANIF_UPPER_LAYER_HANDLE          STD_ON
#define CANIF_PDU_CHANNEL_MODE_CONTROL    STD_ON
#define CANIF_RX_PDU_RANGE_CANID          STD_OFF
#define CANIF_HRH_RANGE_FILTER            STD_OFF

#define CANIF_VENDOR_ID_VALUE           ((uint16)26)
#define CANIF_MODULE_ID_VALUE           ((uint16)60)
#define CANIF_INSTANCE_ID_VALUE_0       ((uint8)0)
#define CANIF_SW_MAJOR_VERSION_VALUE    (2)
#define CANIF_SW_MINOR_VERSION_VALUE    (3)
#define CANIF_SW_PATCH_VERSION_VALUE    (0)

#define CANIF_SUPPORT_OSEK_NM          STD_OFF
#define CANIF_DISPATCH_USERCTRLBUSOFF_UL         CANIF_DISPATCH_UL_CDD
#define CANIF_DISPATCH_USERSETWAKEUP_UL     CANIF_DISPATCH_UL_CDD
#define CANIF_DISPATCH_USERCTRLBUSOFF_DEFINED_NAME

#define CANIF_DISPATCH_USERSETWAKEUP_DEFINED_NAME

#endif
