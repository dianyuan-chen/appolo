#ifndef ANALAG_INPUT_CFG_H
#define ANALAG_INPUT_CFG_H

#define ANALOGINPUT_DEV_ERROR_DETECT STD_ON

#define ANALOGINPUT_BCU_CHANNEL_NUM         1

#define ANALOGINPUT_SINGLE_BMU_CHANNEL_NUM  1
#define ANALOGINPUT_BMU_CHANNEL_NUM         (ANALOGINPUT_SINGLE_BMU_CHANNEL_NUM * BCU_SLAVE_NUM_MAX)

#define ANALOGINPUT_BCU_ANALOGINPUT1        0

#if ANALOGINPUT_SINGLE_BMU_CHANNEL_NUM > 1
#define ANALOGINPUT_BMU_ANALOGINPUT_1(bmu)  ((ANALOGINPUT_BCU_CHANNEL_NUM + (bmu * ANALOGINPUT_SINGLE_BMU_CHANNEL_NUM)))
#else
#define ANALOGINPUT_BMU_ANALOGINPUT_1(bmu)  ((ANALOGINPUT_BCU_CHANNEL_NUM + bmu))
#endif

#define ANALOGINPUT_CHANNEL_NUM             (ANALOGINPUT_BCU_CHANNEL_NUM + ANALOGINPUT_BMU_CHANNEL_NUM)


#endif