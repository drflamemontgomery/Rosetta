#include "appdata.h"

const OutputReport DEFAULT_OUTPUT_REPORT = {
    .buttons = 0x00,
    .hat = 0x00,

    // 0x7f is the center of the Axis
    .axis_x = 0x7f,
    .axis_y = 0x7f,
    .axis_z = 0x7f,
    .axis_rz = 0x7f,
};

AppData AppData_default() {
  return (AppData){
      .outputs =
          {
              .ptr =
                  {
                      (UsbOutput){
                          .id = 0,
                          .report = DEFAULT_OUTPUT_REPORT,
                      },
                      (UsbOutput){
                          .id = 1,
                          .report = DEFAULT_OUTPUT_REPORT,
                      },
                      (UsbOutput){
                          .id = 2,
                          .report = DEFAULT_OUTPUT_REPORT,
                      },
                      (UsbOutput){
                          .id = 3,
                          .report = DEFAULT_OUTPUT_REPORT,
                      },

                  },
              .len = 4,
          },
  };
}
