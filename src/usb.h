#ifndef _USB_H
#define _USB_H

#include "pico/stdio.h"

enum ControllerButtons { A };

typedef union OutputReport {
  struct {
    union {
      struct {
        unsigned char west : 1;
        unsigned char south : 1;
        unsigned char east : 1;
        unsigned char north : 1;
        unsigned char lb : 1;
        unsigned char rb : 1;
        unsigned char lt : 1;
        unsigned char rt : 1;
        unsigned char select : 1;
        unsigned char start : 1;
        unsigned char thumbl : 1;
        unsigned char thumbr : 1;
        unsigned char mode : 1;
        unsigned char special : 1;
      };
      uint16_t buttons;
    };

    union {
      struct {
        unsigned char left : 1;
        unsigned char right : 1;
        unsigned char up : 1;
        unsigned char down : 1;
      };
      uint8_t hat;
    };
    uint8_t axis_x;
    uint8_t axis_y;
    uint8_t axis_z;
    uint8_t axis_rz;
  };
  uint8_t bytes[8];
} OutputReport;

typedef struct UsbOutput {
  OutputReport report;
  int id;
} UsbOutput;

void UsbOutput_sendReport(const UsbOutput *output);

#endif //_USB_H
