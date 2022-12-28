#include "configs.h"
#include <string.h>

void xbox_run_test_config(struct struct_config_elem* elem, uint8_t* data);
void pass_through_func(struct struct_config_elem* elem, uint8_t* data);

config_t left_half;
config_t right_half;

config_t left_risk_of_rain;
config_t right_risk_of_rain;

config_t pass_through_1;
config_t pass_through_2;

config_t xbox_test_config;

bool initialized = false;

void init_configs() {
  if(initialized) {return;}
  initialized = true;

  left_half.num_of_elems = 8;
  right_half.num_of_elems = 11;
  left_risk_of_rain.num_of_elems = 12;
  right_risk_of_rain.num_of_elems = 7;
  xbox_test_config.num_of_elems = 1;
  pass_through_1.num_of_elems = 1;
  pass_through_2.num_of_elems = 1;

  left_half.elems = malloc(sizeof(config_elem)*8);
  right_half.elems = malloc(sizeof(config_elem)*11);
  left_risk_of_rain.elems = malloc(sizeof(config_elem)*12);
  right_risk_of_rain.elems = malloc(sizeof(config_elem)*7);
  xbox_test_config.elems = malloc(sizeof(config_elem)*1);
  pass_through_1.elems = malloc(sizeof(config_elem)*1);
  pass_through_2.elems = malloc(sizeof(config_elem)*1);
  

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
    xbox_test_config.elems[0].type = USER_DEFINED;
    xbox_test_config.elems[0].output = 0;
    xbox_test_config.elems[0].byte = 0;
    xbox_test_config.elems[0].user.run_config = xbox_run_test_config;
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
}

void press_button(int dev_id, int btn_idx, int out_idx, uint16_t data) {
  if((data & (1<<btn_idx)) == (1<<btn_idx)) {
      hid_device_out[dev_id].buttons |= (1<<out_idx);
  }
}

void xbox_run_test_config(struct struct_config_elem* elem, uint8_t* data) {
  for(int i = 0; i < 2; i++) {
    struct struct_xbox_controller* controller = &(((struct struct_xbox_controller*)data)[i]);
    hid_device_out[2+i].axis_x = controller->axis_x;
    hid_device_out[2+i].axis_y = 0xff-controller->axis_y;
    hid_device_out[2+i].axis_z = controller->axis_z;
    hid_device_out[2+i].axis_rz = 0xff-controller->axis_rz;
    
    press_button(2+i, 8, 4, controller->buttons);
    press_button(2+i, 9, 5, controller->buttons);
    press_button(2+i, 10, 12, controller->buttons);
    press_button(2+i, 12, 1, controller->buttons);
    press_button(2+i, 13, 2, controller->buttons);
    press_button(2+i, 14, 0, controller->buttons);
    press_button(2+i, 15, 3, controller->buttons);
    press_button(2+i, 4, 9, controller->buttons);
    press_button(2+i, 5, 8, controller->buttons);
    press_button(2+i, 6, 10, controller->buttons);
    press_button(2+i, 7, 11, controller->buttons);
    if(controller->lt > 0x7f) { hid_device_out[2+i].buttons |= (1<<6); }
    if(controller->rt > 0x7f) { hid_device_out[2+i].buttons |= (1<<7); }
  }   
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
