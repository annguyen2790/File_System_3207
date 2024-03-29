#define DISK "VIRTUAL_DISK"
#define NUM_BLOCKS 245
#define SIZE_OF_BLOCK 512
#define meta_begin 1
#define data_begin 3
#define FAT_ENTRY_SIZE 32
#define META_ENTRY_SIZE 96
#define DATA_ENTRY_SIZE 512


typedef struct FAT {
  char check_bit_valid[1];
  char file_name[15];
  char meta_data_pointer[5];
  char next_pointer [5];
  char data_pointer [5];
  
}my_FAT;

typedef struct meta_data {
  char file_name [15];
  char extension [3];
  char file_size [4];
  char create_time [27];
  char modify_time [27];
  
}my_Meta;

typedef struct data {
  
  char raw_data [512];
  
}my_Data;
