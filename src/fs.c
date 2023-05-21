#include <stdlib.h>
#include <string.h>

#include "settings.h"
#include "structs.h"

void unload_config(config_holder_t *config);

void read_from_file(uint8_t **src, uint8_t *dst, int len) {
  while(len > 0) {
    *dst++ = *(*src)++;
    len--;
  }
}

bool compare_string(uint8_t **src, const char *str, int len) {
  while(len > 0) {
    if(*(*src)++ != *str++) return false;
    len--;
  }

  return true;
}

static const config_holder_t error_config = {0x00, NULL};

config_holder_t load_config(int fd) {
  config_holder_t holder = {0x00, NULL};

  uint8_t *file = (uint8_t*)(FS_START + fd * 256);
  if(!compare_string(&file, "RCFG", 4)) return error_config;

  struct __attribute__((__packed__)) {
    uint8_t num_of_configs;
    uint16_t config_length;
  } file_header;

  read_from_file(&file, (uint8_t*)&file_header, sizeof(file_header));

  holder.num_of_configs = file_header.num_of_configs;
  holder.configs = (config_header_t**)malloc(sizeof(config_header_t*) * file_header.num_of_configs);

  for(int i = 0; i < holder.num_of_configs; i++) {
    
    if(!compare_string(&file, "BC", 2)) goto file_fail;

    uint8_t length = *file++;
    config_header_t *header = (config_header_t*)malloc(length + 2);
    header->size = length + 1;
    header->controller_id = 0;

    read_from_file(&file, (uint8_t*)(header) + 2, length);

    holder.configs[i] = header;
  }

  if(!compare_string(&file, "GFCR", 4)) goto file_fail;

  return holder;
file_fail:

  unload_config(&holder);
  return error_config;
}

void unload_config(config_holder_t *config) {
  for(int i = 0; i < config->num_of_configs; i++) {
    if(config->configs[i] == NULL) continue;
    free(config->configs[i]);
  }
  free(config->configs);
  config->configs = NULL;
  config->num_of_configs = 0;
}
