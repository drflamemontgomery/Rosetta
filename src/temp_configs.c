#include "structs.h"

const int faceoff_1_config_length = 14;
const int faceoff_2_config_length = 14;
const uint8_t faceoff_1_config[] = {
  //length
  7, 0x00, 0x03, 0x01, CH_AXIS_PASS_THROUGH, CH_AXIS_PASS_THROUGH, 0x00,
  7, 0x00, 0x06, 0x01, CH_AXIS_PASS_THROUGH, CH_AXIS_PASS_THROUGH, 0x03,
};
const uint8_t faceoff_2_config[] = { 
  7, 0x00, 0x04, 0x01, CH_AXIS_PASS_THROUGH, CH_AXIS_PASS_THROUGH, 0x01,
  7, 0x00, 0x05, 0x01, CH_AXIS_PASS_THROUGH, CH_AXIS_PASS_THROUGH, 0x02,
};

