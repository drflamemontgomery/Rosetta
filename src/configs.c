#include "configs.h"
#include <string.h>
#include "fs.h"

config_holder error_config = {0x00, 0x00};

void read_from_file(uint8_t** src, uint8_t* dest, int len) {
  for(int i = 0; i < len; i++) {
    *dest++ = *(*src)++;
  }
}

config_holder load_config(int fd) {
  config_holder holder = {0x00, 0x00};

  uint8_t* file = (uint8_t*)(FS_START + fd * 256);

  if(*file++ != 'R') return error_config;
  if(*file++ != 'C') return error_config;
  if(*file++ != 'F') return error_config;
  if(*file++ != 'G') return error_config;

  struct __attribute__((__packed__)) {
    uint8_t num_of_configs;
    uint16_t config_length;
  } file_header;

  read_from_file(&file, (uint8_t*)&file_header, sizeof(file_header));
  holder.num_of_configs = file_header.num_of_configs;
  holder.configs = (config_header**)malloc(sizeof(config_header*)*file_header.num_of_configs);
  for(int i = 0; i < holder.num_of_configs; i++) {
    holder.configs[i] = NULL;
  }

  for(int i = 0; i < holder.num_of_configs; i++) {
    
    if(*file++ != 'B') goto file_fail;
    if(*file++ != 'C') goto file_fail;

    uint8_t length = *file++;
    config_header* header = (config_header*)malloc(length + 2);
    header->size = length + 1;
    header->controller_id = 0;

    read_from_file(&file, (uint8_t*)(header) + 2, length);

    holder.configs[i] = header;
  }

  if(*file++ != 'G') goto file_fail;
  if(*file++ != 'F') goto file_fail;
  if(*file++ != 'C') goto file_fail;
  if(*file++ != 'R') goto file_fail;

  return holder;

file_fail:
  unload_config(&holder);
  return error_config;
}

void unload_config(config_holder* config) {
  for(int i = 0; i < config->num_of_configs; i++) {
    if(config->configs[i] == NULL) continue;
    free(config->configs[i]);
  }
  free(config->configs);
  config->configs = NULL;
  config->num_of_configs = 0;
}
