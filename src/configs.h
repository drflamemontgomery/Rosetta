#ifndef ROSSETTA_CONFIGS
#define ROSSETTA_CONFIGS

#include "hid_dev.h"

config_holder load_config(int fd);
void unload_config(config_holder* config);

#endif
