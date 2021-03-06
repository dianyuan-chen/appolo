/**
 * \addtogroup autosar
 * \{
 * \addtogroup SPI SPI驱动
 * \{
 */
#ifndef SPI_H_
#define SPI_H_

#include "Std_Types.h"
#include "Spi_Cfg.h"
#include "Spi_Types.h"

/* STD container : SpiGeneral
 * SPI_CANCEL_API                   1     Bool
 * SPI_CHANNEL_BUFFERS_ALLOWED      1     Int 0..2
 * SPI_DEV_ERROR_DETECT             1     Bool
 * SPI_HW_STATUS_API                1     Bool
 * SPI_INTERRUPTABLE_SEQ_ALLOWED    1     Bool
 * SPI_LEVEL_DELIVERED              1     Int 0..2
 * SPI_VERSION_INFO_API             1     Bool
 */

/* SPI container: SpiSequence
 * SpiInterruptableSequence         1     Bool
 * SpiSeqEndNotification            1     Func
 * SpiSequenceId                    1     Int     (name of the sequence)
 * JobAssignment                    1..*  Ref to SpiJob
 */

/* SPI container: SpiChannel
 * SpiChannelId                     1     Int ( name of the channel )
 * SpiChannelType                   1     enum IB, EB
 * SpiDataWidth                     1     Int, 1..32
 * SpiDefaultData                   1     Int
 * SpiEbMaxLength                   1     Int
 * SpiHwUnitSynchronous             0..1            [New in 4.0]
 * SpiIbNBuffers                    1     Int
 * SpiTransferStart                 1     Enum LSB, MSB
 */

/* SPI container: SpiJob
 * SpiHwUnit                        1     Enum, CSIB0,CSIB1,CSIB2,CSIB3
 * SpiJobEndNotification            1     Func
 * SpiJobId                         1     Int ( name of job )
 * SpiJobPriority                   1     Int 0..3
 * ChannelAssignment                1..*  Ref to channel
 * DeviceAssignment                 1     Ref to exteral device
 */

/* SPI container: SpiExternalDevice
 * SpiBaudrate                      1     float
 * SpiCsIdentifier                  1     String
 * SpiCsPolarity                    1     enum, HIGH, LOW
 * SpiDataShiftEdge                 1     enum LEADING, TRAILING
 * SpiEnableCs                      1     Bool
 * SpiShiftClockIdleLevel           1     Enum, HIGH, LOW
 * SpiTimeClk2Cs                    1     float
 */

/* SPI container: SpiDriver
 * SpiMaxChannel                    0..1  Int
 * SpiMaxJob                        0..1  Int
 * SpiMaxSequence                   0..1  Int
 * SpiChannel[c]                    1..*  Channel Data
 * SpiExternalDevice[c]             1..*  External device data
 * SpiJob[c]                        1..*  Job data
 * SpiSequence[c]                   1..*  Sequence data.
  */



/* --- Service IDs --- */
#define SPI_INIT_SERVICE_ID               0x00U
#define SPI_DEINIT_SERVICE_ID             0x01U
#define SPI_WRITEIB_SERVICE_ID            0x02U
#define SPI_ASYNCTRANSMIT_SERVICE_ID      0x03U
#define SPI_READIB_SERVICE_ID             0x04U
#define SPI_SETUPEB_SERVICE_ID            0x05U
#define SPI_GETSTATUS_SERVICE_ID          0x06U
#define SPI_GETJOBRESULT_SERVICE_ID       0x07U
#define SPI_GETSEQUENCERESULT_SERVICE_ID  0x08U
#define SPI_GETVERSIONINFO_SERVICE_ID     0x09U
#define SPI_SYNCTRANSMIT_SERVICE_ID       0x0AU
#define SPI_GETHWUNITSTATUS_SERVICE_ID    0x0BU
#define SPI_CANCEL_SERVICE_ID             0x0CU
#define SPI_SETASYNCMODE_SERVICE_ID       0x0DU

/* --- Error codes --- */
#define SPI_E_PARAM_CHANNEL               0x0AU
#define SPI_E_PARAM_JOB                   0x0BU
#define SPI_E_PARAM_SEQ                   0x0CU
#define SPI_E_PARAM_LENGTH                0x0DU
#define SPI_E_PARAM_UNIT                  0x0EU
#define SPI_E_UNINIT                      0x1AU
#define SPI_E_SEQ_PENDING                 0x2AU
#define SPI_E_SEQ_IN_PROCESS              0x3AU
#define SPI_E_ALREADY_INITIALIZED         0x4AU


/* Types according to standard */
#define SPI_SIMPLE         0U   /* Not implemented, NOT TESTED */
#define SPI_FIFO           1U
#define SPI_DMA            2U


#define CSIB0   0U
#define CSIB1   1U
#define CSIB2   2U
#define CSIB3   3U

typedef enum {
    SPI_UNINIT = 0, // The SPI Handler/Driver is not initialized or not usable.
    // SPI011: This shall be the default value after reset. This
    // status shall have the value 0.

    SPI_IDLE = 1,   // The SPI Handler/Driver is not currently transmitting any
    // Job.

    SPI_BUSY = 2,   // The SPI Handler/Driver is performing a SPI Job( transmit )
} Spi_StatusType;

typedef enum {
    SPI_JOB_OK = 0,     // The last transmission of the Job has been finished
    // successfully.
    // SPI012: This shall be the default value after reset.
    // This status shall have the value 0.

    SPI_JOB_PENDING = 1,    // The SPI Handler/Driver is performing a SPI Job.
    // The meaning of this status is equal to SPI_BUSY.

    SPI_JOB_FAILED = 2,     // The last transmission of the Job has failed.

    SPI_JOB_QUEUED = 3,     // Job accepted but not started.

} Spi_JobResultType;

typedef enum {

    SPI_SEQ_OK,     //  The last transmission of the Sequence has been
    //    finished successfully.
    //    SPI017: This shall be the default value after reset.
    //    This status shall have the value 0.

    SPI_SEQ_PENDING, // The SPI Handler/Driver is performing a SPI
    // Sequence. The meaning of this status is equal to
    // SPI_BUSY.

    SPI_SEQ_FAILED,     // The last transmission of the Sequence has failed.

    SPI_SEQ_CANCELLED,  // The last transmission of the Sequence has been
    //    cancelled by user.
} Spi_SeqResultType;


// Type for defining the number of data elements of the type Spi_DataType to
// send and / or receive by Channel


// Specifies the asynchronous mechanism mode for SPI busses handled
// asynchronously in LEVEL 2.
// SPI150: This type is available or not accordint to the pre compile time parameter:
// SPI_LEVEL_DELIVERED. This is only relevant for LEVEL 2.

typedef enum {

    //  The asynchronous mechanism is ensured by
    //    polling, so interrupts related to SPI busses
    //    handled asynchronously are disabled.
    SPI_POLLING_MODE,

    //    The asynchronous mechanism is ensured by
    //    interrupt, so interrupts related to SPI busses
    //    handled asynchronously are enabled.

    SPI_INTERRUPT_MODE,
} Spi_AsyncModeType;

typedef enum {
    SPI_EB = 0, // External Buffer
    SPI_IB = 1 // Internal Buffer
} Spi_BufferType;

typedef enum {
    SPI_TRANSFER_START_LSB,
    SPI_TRANSFER_START_MSB,
} Spi_TransferStartType;


typedef enum {
    SPI_EDGE_LEADING,
    SPI_EDGE_TRAILING
} Spi_EdgeType;


#if (SPI_IMPLEMENTATION==SPI_DMA)
#include "Dma.h"
#endif

// All data needed to configure one SPI-channel
typedef struct {
    // Symbolic name
    Spi_ChannelType SpiChannelId;
    // Buffer usage with EB/IB channel
    Spi_BufferType SpiChannelType;

    // This parameter is the width of a transmitted data unit.
    uint8 SpiDataWidth;
    // This parameter is the default value to transmit.
    uint8 SpiDefaultData;

    // This parameter contains the maximum size of data buffers in case of EB
    // Channels and only.
    Spi_NumberOfDataType SpiEbMaxLength;

    // This parameter contains the maximum number of data buffers in case of IB
    // Channels and only.
    Spi_NumberOfDataType SpiIbNBuffers;

    // This parameter defines the first starting bit for transmission.
    Spi_TransferStartType SpiTransferStart;

    //
    //  _Bool SpiDmaNoIncreaseSrc;

} Spi_ChannelConfigType;

// All data needed to configure one SPI-Job, amongst others the connection
// between the internal SPI unit and the special settings for an external de-
// vice is done.
typedef struct Spi_JobConfig {

    Spi_JobType SpiJobId;

    //    This parameter is the symbolic name to identify the HW SPI Hardware micro-
    //    controller peripheral allocated to this Job.
    uint32 SpiHwUnit;

    // This parameter is a reference to a notification function.
    void (*SpiJobEndNotification)();

    // Priority of the Job
    // range 0..3
    unsigned SpiJobPriority;

    // A job references several channels. TODO: Optimize this...
    Spi_ChannelType ChannelAssignment[SPI_MAX_CHANNEL + 1U];

    // Reference to the external device used by this job
    Spi_ExternalDeviceTypeType DeviceAssignment;

    //  unsigned    SPI_NUMBER_OF_CHANNELS;
    //  unsigned    SPI_LIST_OF_CHANNELS[SPI_MAX_CHANNEL];
} Spi_JobConfigType;

// The communication settings of an external device. Closely linked to Spi-
// Job.
typedef struct {

    // This parameter is the communication baudrate - This parameter allows
    // using a range of values, from the point of view of configuration tools, from
    // Hz up to MHz.
    // Note! Float in config case, not here
    uint32 SpiBaudrate;

    // Symbolic name to identify the CS used for this job
    uint8 SpiCsIdentifier;

    // This parameter defines the active polarity of Chip Select.
    // STD_HIGH or STD_LOW
    uint8 SpiCsPolarity;

    // This parameter defines the SPI data shift edge.
    Spi_EdgeType SpiDataShiftEdge;

    // This parameter enables or not the Chip Select handling functions.
    uint8 SpiEnableCs;

    // This parameter defines the SPI shift clock idle level.
    uint8 SpiShiftClockIdleLevel;

    // Timing between clock and chip select - This parameter allows to use a
    // range of values from 0 up to 100 microSec. the real configuration-value
    // used in software BSW-SPI is calculated out of this by the generator-tools
    // Note! Float in config case, not here. Unit ns
    uint32 SpiTimeClk2Cs;

    // Timing between PCS and first edge of SCK. Unit ns.
    uint32 SpiTimeCs2Clk;

    // ArcCore extension...
    // The controller ID(0..3)
    //uint32 SpiControllerId;
    void (*SpiCsCallback)(int data);

} Spi_ExternalDeviceType;

// All data needed to configure one SPI-sequence
typedef struct Spi_SequenceConfig {
    // This parameter allows or not this Sequence to be suspended by another
    // one.
    unsigned SpiInterruptibleSequence;
    // This parameter is a reference to a notification function.
    void (*SpiSeqEndNotification)(void);
    //
    Spi_SequenceType SpiSequenceId;
    //    unsigned            SPI_NUMBER_OF_JOBS;
    // A sequence references several jobs, which are executed during a commu-
    // nication sequence
    Spi_JobType JobAssignment[SPI_MAX_JOB + 1U];
} Spi_SequenceConfigType;


typedef struct {
    //    This parameter contains the number of Channels configured. It will be
    //    gathered by tools during the configuration stage.
    uint8 SpiMaxChannel;

    uint8 SpiMaxJob;

    uint8 SpiMaxSequence;

    // All data needed to configure one SPI-channel
    const Spi_ChannelConfigType *SpiChannelConfig;

    // The communication settings of an external device. Closely
    // linked to SpiJob.
    const Spi_ExternalDeviceType *SpiExternalDevice;

    //    All data needed to configure one SPI-Job, amongst others the
    //    connection between the internal SPI unit and the special set-
    //    tings for an external device is done.
    const struct Spi_JobConfig *SpiJobConfig;

    // All data needed to configure one SPI-sequence
    const struct Spi_SequenceConfig *SpiSequenceConfig;

    const struct Spi_HwConfig *SpiHwConfig;
} Spi_ConfigType;

void Spi_Init(const Spi_ConfigType *ConfigPtr);
Std_ReturnType Spi_DeInit(void);
Std_ReturnType Spi_WriteIB(Spi_ChannelType Channel, const Spi_DataType *DataBufferPtr);
Std_ReturnType Spi_AsyncTransmit(Spi_SequenceType Sequence );
Std_ReturnType Spi_ReadIB(Spi_ChannelType Channel, Spi_DataType *const DataBufferPtr);
Std_ReturnType Spi_SetupEB(Spi_ChannelType Channel, const Spi_DataType  *SrcDataBufferPtr, Spi_DataType *DesDataBufferPtr, Spi_NumberOfDataType Length);
Spi_StatusType Spi_GetStatus(void);
Spi_JobResultType Spi_GetJobResult(Spi_JobType job);
Spi_SeqResultType Spi_GetSequenceResult(Spi_SequenceType Sequence);
Std_ReturnType Spi_SyncTransmit(Spi_SequenceType Sequence);
Spi_StatusType Spi_GetHWUnitStatus(Spi_HWUnitType HWUnit);
void Spi_Cancel(Spi_SequenceType Sequence );
Std_ReturnType Spi_SetAsyncMode(Spi_AsyncModeType Mode);
//void Spi_MainFunction_Handling(void);
//void Spi_MainFunction_Driving(void);

extern const Spi_ConfigType SpiConfigData;

#endif /*SPI_H_*/

/**
 * \}
 * \}
 */
