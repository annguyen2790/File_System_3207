//hello there
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
  char create_time [8];
  char modify_time [8]; 
}my_Meta;

typedef struct data {
  
  char raw_data [512];
  
}my_Data;
