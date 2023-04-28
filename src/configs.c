#include "configs.h"
#include <string.h>

void xbox_pass_func(struct struct_config_elem* elem, uint8_t* data);
void ps1_racing_wheel_pass_func(struct struct_config_elem* elem, uint8_t* data);
void pass_through_func(struct struct_config_elem* elem, uint8_t* data);

config_t left_half;
config_t right_half;

config_t left_risk_of_rain;
config_t right_risk_of_rain;

config_t pass_through_1;
config_t pass_through_2;

config_t xbox_pass_1;
config_t xbox_pass_2;

config_t ps1_racing_wheel_pass;

bool initialized = false;

void init_configs() {
  if(initialized) {return;}
  initialized = true;

  left_half.num_of_elems = 8;
  right_half.num_of_elems = 11;
  left_risk_of_rain.num_of_elems = 12;
  right_risk_of_rain.num_of_elems = 7;
  pass_through_1.num_of_elems = 1;
  pass_through_2.num_of_elems = 1;
  xbox_pass_1.num_of_elems = 1;
  xbox_pass_2.num_of_elems = 1;
  ps1_racing_wheel_pass.num_of_elems = 1;

  left_half.elems = malloc(sizeof(config_elem)*8);
  right_half.elems = malloc(sizeof(config_elem)*11);
  left_risk_of_rain.elems = malloc(sizeof(config_elem)*12);
  right_risk_of_rain.elems = malloc(sizeof(config_elem)*7);
  pass_through_1.elems = malloc(sizeof(config_elem)*1);
  pass_through_2.elems = malloc(sizeof(config_elem)*1);
  xbox_pass_1.elems = malloc(sizeof(config_elem)*1);
  xbox_pass_2.elems = malloc(sizeof(config_elem)*1);
  ps1_racing_wheel_pass.elems = malloc(sizeof(config_elem)*1);
  

  {
    static const uint8_t push_back[9] = {0, 1, 2, 4, 8, 9, 10, 12, 13};
    for(int i = 0; i < 9; i++) {
      left_risk_of_rain.elems[i].type = BUTTON;
      left_risk_of_rain.elems[i].output = 0;
      left_risk_of_rain.elems[i].byte = i < 4 ? 0 : 1;
      left_risk_of_rain.elems[i].btn.mask_from = 1<<(push_back[i]%8);
      left_risk_of_rain.elems[i].btn.mask_to = 1<<push_back[i];
    }
    for(int i = 0; i < 2; i++) {
      left_risk_of_rain.elems[i+9].type = AXIS;
      left_risk_of_rain.elems[i+9].output = 0;
      left_risk_of_rain.elems[i+9].byte = i+3;
      left_risk_of_rain.elems[i+9].axis.axis = i;
      left_risk_of_rain.elems[i+9].axis.reversed = 0;
    }
    left_risk_of_rain.elems[11].type = HAT;
    left_risk_of_rain.elems[11].output = 0;
    left_risk_of_rain.elems[11].byte = 2;
  }

  {
    static const uint8_t push_back[5] = {3, 5, 6, 7, 11};
    for(int i = 0; i < 5; i++) {
      right_risk_of_rain.elems[i].type = BUTTON;
      right_risk_of_rain.elems[i].output = 0;
      right_risk_of_rain.elems[i].byte = i < 4 ? 0 : 1;
      right_risk_of_rain.elems[i].btn.mask_from = 1<<(push_back[i]%8);
      right_risk_of_rain.elems[i].btn.mask_to = 1<<push_back[i];
    }
    for(int i = 0; i < 2; i++) {
      right_risk_of_rain.elems[i+5].type = AXIS;
      right_risk_of_rain.elems[i+5].output = 0;
      right_risk_of_rain.elems[i+5].byte = i+5;
      right_risk_of_rain.elems[i+5].axis.axis = i+2;
      right_risk_of_rain.elems[i+5].axis.reversed = 0;
    }
  }

  // Non Risk Of Rain

  {
    static const uint8_t push_back[5] = {4, 6, 8, 10, 13};
    for(int i = 0; i < 5; i++) {
      left_half.elems[i].type = BUTTON;
      left_half.elems[i].output = 0;
      left_half.elems[i].byte = i < 2 ? 0 : 1;
      left_half.elems[i].btn.mask_from = 1<<(push_back[i]%8);
      left_half.elems[i].btn.mask_to = 1<<push_back[i];
    }
    for(int i = 0; i < 2; i++) {
      left_half.elems[i+5].type = AXIS;
      left_half.elems[i+5].output = 0;
      left_half.elems[i+5].byte = i+3;
      left_half.elems[i+5].axis.axis = i;
      left_half.elems[i+5].axis.reversed = 0;
    }
    left_half.elems[7].type = HAT;
    left_half.elems[7].output = 0;
    left_half.elems[7].byte = 2;
  }

  {
    static const uint8_t push_back[9] = {0, 1, 2, 3, 5, 7, 9, 11, 12};
    for(int i = 0; i < 9; i++) {
      right_half.elems[i].type = BUTTON;
      right_half.elems[i].output = 0;
      right_half.elems[i].byte = i < 4 ? 0 : 1;
      right_half.elems[i].btn.mask_from = 1<<(push_back[i]%8);
      right_half.elems[i].btn.mask_to = 1<<push_back[i];
    }
    for(int i = 0; i < 2; i++) {
      right_half.elems[i+9].type = AXIS;
      right_half.elems[i+9].output = 0;
      right_half.elems[i+9].byte = i+5;
      right_half.elems[i+9].axis.axis = i+2;
      right_half.elems[i+9].axis.reversed = 0;
    }
  }

  // XBOX TEST CONFIG
  
  {
    xbox_pass_1.elems[0].type = USER_DEFINED;
    xbox_pass_1.elems[0].output = 2;
    xbox_pass_1.elems[0].byte = 0;
    xbox_pass_1.elems[0].user.run_config = xbox_pass_func;

    xbox_pass_2.elems[0].type = USER_DEFINED;
    xbox_pass_2.elems[0].output = 3;
    xbox_pass_2.elems[0].byte = 0;
    xbox_pass_2.elems[0].user.run_config = xbox_pass_func;
  }
  
  {
    pass_through_1.elems[0].type = USER_DEFINED;
    pass_through_1.elems[0].output = 0;
    pass_through_1.elems[0].byte = 0;
    pass_through_1.elems[0].user.run_config = pass_through_func;

    pass_through_2.elems[0].type = USER_DEFINED;
    pass_through_2.elems[0].output = 1;
    pass_through_2.elems[0].byte = 0;
    pass_through_2.elems[0].user.run_config = pass_through_func;
  }

  {
    ps1_racing_wheel_pass.elems[0].type = USER_DEFINED;
    ps1_racing_wheel_pass.elems[0].output = 2;
    ps1_racing_wheel_pass.elems[0].byte = 0;
    ps1_racing_wheel_pass.elems[0].user.run_config = ps1_racing_wheel_pass_func;
  }
}

void press_button(int dev_id, int btn_idx, int out_idx, uint16_t data) {
  if((data & (1<<btn_idx)) == (1<<btn_idx)) {
      hid_device_out[dev_id].buttons |= (1<<out_idx);
  }
}

extern uint64_t blink_interval_ms;

void xbox_pass_func(struct struct_config_elem* elem, uint8_t* data) {
  struct struct_xbox_controller* controller = (struct struct_xbox_controller*)data;
  hid_device_out[elem->output].axis_x = controller->axis_x;
  hid_device_out[elem->output].axis_y = 0xff-controller->axis_y;
  hid_device_out[elem->output].axis_z = controller->axis_z;
  hid_device_out[elem->output].axis_rz = 0xff-controller->axis_rz;

  press_button(elem->output, 8, 4, controller->buttons);
  press_button(elem->output, 9, 5, controller->buttons);
  press_button(elem->output, 10, 12, controller->buttons);
  press_button(elem->output, 12, 1, controller->buttons);
  press_button(elem->output, 13, 2, controller->buttons);
  press_button(elem->output, 14, 0, controller->buttons);
  press_button(elem->output, 15, 3, controller->buttons);
  press_button(elem->output, 4, 9, controller->buttons);
  press_button(elem->output, 5, 8, controller->buttons);
  press_button(elem->output, 6, 10, controller->buttons);
  press_button(elem->output, 7, 11, controller->buttons);
  if(controller->lt > 0x7f) { hid_device_out[elem->output].buttons |= (1<<6); }
  if(controller->rt > 0x7f) { hid_device_out[elem->output].buttons |= (1<<7); }
  blink_interval_ms = 2000;
}

const uint8_t button_to_dpad[16] = {
  0x0f, // center
  0x00, // top
  0x02, // right
  0x01, // top, right
  0x04, // bottom
  0x0f, // bottom, top
  0x03, // bottom, right
  0x0f, // bottom, top, right
  0x06, // left
  0x07, // left, top
  0x0f, // left, right
  0x0f, // left, top, right
  0x05, // left, bottom,
  0x0f, // left, bottom, top,
  0x0f, // left, bottom, right
  0x0f, // left, bottom, top, right
};

#define MAX(a,b) (a > b ? a : b)
#define MIN(a,b) (a < b ? a : b)

void ps1_racing_wheel_pass_func(struct struct_config_elem* elem, uint8_t* data) {
  struct struct_ps1_racing_wheel* controller = (struct struct_ps1_racing_wheel*)data;
  const int sensitivity = 5;
  if(controller->axis_x > 0x85) {
    controller->axis_x = MIN(0xff, 0x80 + (controller->axis_x - 0x80)*sensitivity);
  }
  if(controller->axis_x < 0x7b) {
    controller->axis_x = MAX(0x00, 0x80 - (0x80 - controller->axis_x)*sensitivity);
  }
  hid_device_out[elem->output].axis_x = controller->axis_x;
  hid_device_out[elem->output].axis_y = controller->axis_y;


  hid_device_out[elem->output].hat = button_to_dpad[(uint8_t)(controller->buttons>>12)];
  
  press_button(elem->output, 2, 1, controller->buttons); // B
  press_button(elem->output, 3, 4, controller->buttons); // L
  press_button(elem->output, 5, 2, controller->buttons); // A
  press_button(elem->output, 4, 5, controller->buttons); // R
  press_button(elem->output, 6, 0, controller->buttons); // Y
  press_button(elem->output, 7, 3, controller->buttons); // X
  press_button(elem->output, 8, 8, controller->buttons); // -
  press_button(elem->output, 9, 9, controller->buttons); // +


  /*press_button(elem->output, 8, 4, controller->buttons);
  press_button(elem->output, 9, 5, controller->buttons);
  press_button(elem->output, 10, 12, controller->buttons);
  press_button(elem->output, 12, 1, controller->buttons);
  press_button(elem->output, 13, 2, controller->buttons);
  press_button(elem->output, 14, 0, controller->buttons);
  press_button(elem->output, 15, 3, controller->buttons);
  press_button(elem->output, 4, 9, controller->buttons);
  press_button(elem->output, 5, 8, controller->buttons);
  press_button(elem->output, 6, 10, controller->buttons);
  press_button(elem->output, 7, 11, controller->buttons);*/
  blink_interval_ms = 5000;
}


void pass_through_func(struct struct_config_elem* elem, uint8_t* data) {
  struct faceoff_pro_controller_data* c_dat = (struct faceoff_pro_controller_data*)data;
  hid_device_out[elem->output].buttons = c_dat->buttons;
  hid_device_out[elem->output].hat = c_dat->hat;
  hid_device_out[elem->output].axis_x = c_dat->axis_x;
  hid_device_out[elem->output].axis_y = c_dat->axis_y;
  hid_device_out[elem->output].axis_z = c_dat->axis_z;
  hid_device_out[elem->output].axis_rz = c_dat->axis_rz;
}
