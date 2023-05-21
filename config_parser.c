#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

void pa_pm_button(int file, unsigned char byte_size) {
  unsigned char* bytes = (unsigned char*)malloc(byte_size);
  read(file, bytes, byte_size);
  printf("Button Mask: 0x");
  for(unsigned char i = byte_size; i > 0; i--) {
    printf("%02X", bytes[i - 1]);
  }
  free(bytes);
  putchar('\n');
}

void pa_hat(int file) {
  printf("Hat Direction: ");
  unsigned char direction;
  read(file, &direction, 1);
  switch(direction) {
    case 0x00:
      printf("N");
      break;
    case 0x01:
      printf("NE");
      break;
    case 0x02:
      printf("E");
      break;
    case 0x03:
      printf("SE");
      break;
    case 0x04:
      printf("S");
      break;
    case 0x05:
      printf("SW");
      break;
    case 0x06:
      printf("W");
      break;
    case 0x07:
      printf("NW");
      break;
    case 0x0f:
      printf("Center");
      break;
  }
  putchar('\n');
}

void pm_hat(int file) {
  printf("Hat Direction: ");
  unsigned char direction;
  read(file, &direction, 1);
  if(direction & 0x8) putchar('U');
  if(direction & 0x4) putchar('D');
  if(direction & 0x2) putchar('L');
  if(direction & 0x1) putchar('R');
  putchar('\n');
}

void pa_axis(int file, unsigned char byte_size) {
  unsigned char* bytes = (unsigned char*)malloc(byte_size);
  read(file, bytes, byte_size);
  printf("Axis Threshold: 0x");
  
  for(unsigned char i = byte_size; i > 0; i--) {
    printf("%02X", bytes[i - 1]);
  }
  free(bytes);
  putchar('\n');
}

void pm_axis(int file) {
  unsigned char axis_num;
  char axis_value;

  read(file, &axis_num, 1);
  read(file, &axis_value, 1);

  printf("Axis Num: %d\n", axis_num);
  printf("Axis Value: %d\n", axis_value);
}

void pm_user_defined(int file) {
  long func_addr;
  read(file, &func_addr, 4);
  printf("User Defined Function Address: %ld\n", func_addr);
}

void print_activation_config(int file, unsigned char type, unsigned char byte_size) {
  printf("\n\n-- Activation --\n\n");
  switch(type) {
    case 0x00:
      printf("Pass Through Button\n");
      break;
    case 0x10:
      printf("Pass Through Hat\n");
      break;
    case 0x20:
      printf("Pass Through Axis\n");
      break;

    case 0x01:
      pa_pm_button(file, byte_size);
      break;

    case 0x11:
      pa_hat(file);
      break;
      
    case 0x21:
      pa_axis(file, byte_size);
      break;

    case 0xff:
      printf("User Defined\n");
      break;
  }
}

void print_mapping_config(int file, unsigned char type) {
  printf("\n\n-- Mapping --\n\n");
  switch(type) {
    case 0x00:
      printf("Pass Through Button\n");
      break;
    case 0x10:
      printf("Pass Through Hat\n");
      break;
    case 0x20:
      unsigned char axis_num;
      read(file, &axis_num, 1);
      printf("Pass Through Axis %d\n", axis_num);
      break;

    case 0x01:
      pa_pm_button(file, 2);
      break;

    case 0x11:
      pm_hat(file);
      break;
      
    case 0x21:
      pm_axis(file);
      break;

    case 0xff:
      pm_user_defined(file);
      break;
  }

}

void print_config_type(unsigned char type) {
  switch(type) {
    case 0x00:
      printf("Button Pass Through");
      break;
    case 0x01:
      printf("Button");
      break;

    case 0x10:
      printf("Hat Pass Through");
      break;
    case 0x11:
      printf("Hat");
      break;

    case 0x20:
      printf("Axis Pass Through");
      break;
    case 0x21:
      printf("Axis");
      break;
    
    case 0xff:
      printf("User Defined");
      break;
    default:
      printf("Unknown");
      break;
  }

}

void print_file_data(char* filename) {

  printf("\n\n--- file report for '%s' ---\n\n", filename);

  int file = open(filename, O_RDONLY);
  
  struct __attribute__((__packed__)) {
    char checksum[4];
    unsigned char num_of_fields;
    unsigned short byte_config_total_size;
  } header;

  read(file, &header, sizeof(header));
  
  if(header.checksum[0] != 'R') goto exit_print;
  if(header.checksum[1] != 'C') goto exit_print;
  if(header.checksum[2] != 'F') goto exit_print;
  if(header.checksum[3] != 'G') goto exit_print;

  printf("-- Header --\n\n");
  printf("Checksum: %c%c%c%c\n", header.checksum[0], header.checksum[1], header.checksum[2], header.checksum[3]);
  printf("Number of Fields: 0x%02X\n", header.num_of_fields);
  printf("Config Total Size: 0x%04X\n", header.byte_config_total_size);

  for(int i = 0; i < header.num_of_fields; i++) {
    struct __attribute__((__packed__)) {
      char checksum[2];
      unsigned char length;
      unsigned char start_byte;
      unsigned char byte_size;
      unsigned char activation_type;
      unsigned char mapping_type;
    } metadata;

    read(file, &metadata, sizeof(metadata));

    if(metadata.checksum[0] != 'B') continue;
    if(metadata.checksum[1] != 'C') continue;

    printf("\n\n-- Field %d --\n\n", i);
    printf("Checksum: %c%c\n", metadata.checksum[0], metadata.checksum[1]);
    printf("Length: 0x%02X\n", metadata.length);
    printf("Start Byte: 0x%02X\n", metadata.start_byte);
    printf("Byte Size: 0x%02X\n", metadata.byte_size);
    printf("Activation Type: ");
    print_config_type(metadata.activation_type);
    printf("\nMapping Type: ");
    print_config_type(metadata.mapping_type);
    putchar('\n');

    print_activation_config(file, metadata.activation_type, metadata.byte_size);
    print_mapping_config(file, metadata.mapping_type);
  }

  unsigned char footer_checksum[4];
  read(file, footer_checksum, 4);
  if(footer_checksum[0] != 'G') goto exit_print;
  if(footer_checksum[1] != 'F') goto exit_print;
  if(footer_checksum[2] != 'C') goto exit_print;
  if(footer_checksum[3] != 'R') goto exit_print;

  printf("\n\n-- Footer --\n\n");
  printf("Checksum: GFCR\n\n");


exit_print:

  close(file);

}

int main(int argc, char* argv[]) {
  
  if(argc == 1) {
    printf("usage: %s files...\n", argv[0]);
    return -1;
  }
  
  for(int i = 1; i < argc; i++) {
    print_file_data(argv[i]);
  }
  

  return 0;
}
