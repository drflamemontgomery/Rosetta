#include "configs.h"


config_t left_half;
config_t right_half;

config_t left_risk_of_rain;
config_t right_risk_of_rain;

bool initialized = false;

void init_configs() {
  if(initialized) {return;}
  initialized = true;

  left_half.num_of_elems = 8;
  right_half.num_of_elems = 11;
  left_risk_of_rain.num_of_elems = 12;
  right_risk_of_rain.num_of_elems = 7;

  left_half.elems = malloc(sizeof(config_elem)*8);
  right_half.elems = malloc(sizeof(config_elem)*11);
  left_risk_of_rain.elems = malloc(sizeof(config_elem)*12);
  right_risk_of_rain.elems = malloc(sizeof(config_elem)*7);
  

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
}
