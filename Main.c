#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "file_system_header.h"
#include <time.h>
////////////Functions Prototype///////////////////////
void print_FAT(my_FAT *);
int make_disk(char *);
FILE mount();
void make_root_dir();
char * find_free_meta();
char * find_free_data();
char * find_free_fat_entry();
char * get_time();
void print_meta(my_Meta *);
////////////Global Data Structure//////////////////////
FILE * file_pointer;
char buffer [32]; //use to convert int to string via sprintf
double dir_current;



/////////////////Main Program
int main(){
  int a = make_disk("VIRTUAL_DISK");
  //my_FAT * fat_tab =  malloc(32 * sizeof(my_FAT*));
  /*strcpy (fat_tab->check_bit_valid, "1");
  strcpy (fat_tab->meta_data_pointer, "2");
  strcpy (fat_tab->next_pointer, "3");
  strcpy (fat_tab->data_pointer, "4");
  strcpy (fat_tab->file_name, "hel");
  print_FAT(fat_tab);*/
  make_root_dir();
  char * time = get_time();
  /*
  my_Meta * meta = malloc(32 * sizeof(my_Meta *));
  strcpy(meta->create_time, time);
  strcpy(meta->modify_time, time);
  print_meta(meta);
  */
  printf("%s\n", time);
  
    
}

FILE mount(){
  file_pointer = fopen(DISK, "r+");
  return *file_pointer;


}
void print_FAT(my_FAT * fat){ //use for debugging only
  printf("Bit: %s\n", fat->check_bit_valid); 
  printf("Meta: %s\n",fat->meta_data_pointer);
  printf("Next: %s\n",fat->next_pointer);
  printf("Data: %s\n",fat->data_pointer );
  printf("Name: %s\n",fat->file_name);
}

void make_root_dir(){
  file_pointer = fopen(DISK, "r+");
  fseek(file_pointer, 0, SEEK_SET);//appropriate when load into a mounted disk
  
  /*            Initialize the first FAT in the linked list          */
  my_FAT * root = (my_FAT *)malloc(32 * sizeof(my_FAT*));
  
  strcpy(root->file_name, "/");
  printf("Root Name:%s\n", root->file_name);
  strcpy(root->check_bit_valid, "1");

  sprintf(buffer, "%u", SIZE_OF_BLOCK * meta_begin / 15 + 1 );
  strcpy(root->meta_data_pointer, buffer);

  sprintf(buffer, "%u", SIZE_OF_BLOCK * data_begin / 15 + 1 );
  strcpy(root->data_pointer, buffer);
  
  strcpy(root->next_pointer, "-1");

  fwrite(root, 32 * sizeof(my_FAT*), 1, file_pointer);
  
  print_FAT(root);

  /*          Initialize the first data area                 */
  fseek(file_pointer, SIZE_OF_BLOCK * data_begin, SEEK_SET);
  dir_current = data_begin * SIZE_OF_BLOCK;
  fputs(".", file_pointer);

  /*          Initialize the first meta area                */
  int check = fseek(file_pointer, SIZE_OF_BLOCK * meta_begin, SEEK_SET);
  my_Meta * root_meta = (my_Meta *) malloc(32 * sizeof(my_Meta*));
  char * current_time = get_time();
  char * mod_time = get_time();
  strcpy(root_meta->file_name, "/");
  strcpy(root_meta->extension, "rrr");
  strcpy(root_meta->file_size, "111");
  strcpy(root_meta->create_time, current_time);
  strcpy(root_meta->modify_time, mod_time);
  fwrite(root_meta, 32*sizeof(my_Meta *), 1, file_pointer);
  printf("Success created root meta\n");
  print_meta(root_meta);

}
char * find_free_fat_entry(){
  char valid_check[2];
  fseek(file_pointer, 0, SEEK_SET);
  unsigned int i;
  for(i = 0; i < SIZE_OF_BLOCK; i = i + FAT_ENTRY_SIZE){
    fseek(file_pointer, i, SEEK_SET);
    fread(valid_check, 1, 1, file_pointer);
    if(strcmp(valid_check, "") == 0){
      sprintf(buffer, "%u", (i / 16) + 1);
      return buffer;
    }

  }
  return "Error";
}
char * find_free_meta(){
  char valid_check [15];
  unsigned int i;
  fseek(file_pointer, meta_begin * SIZE_OF_BLOCK , SEEK_SET);
  for(i = 0; i < meta_begin * SIZE_OF_BLOCK; i = i + META_ENTRY_SIZE){
    fseek(file_pointer, i + (meta_begin * SIZE_OF_BLOCK), SEEK_SET);
    fread(valid_check, 15, 1, file_pointer);
    if(strcmp(valid_check, "") == 0){
      sprintf(buffer, "%u", (meta_begin * SIZE_OF_BLOCK / 16) + 1 + (i / META_ENTRY_SIZE * 4 ) );
      return buffer;
    }
  }
  return "Error";

}

char * find_free_data(){
  char valid_check [2];
  unsigned int i;
  fseek(file_pointer, data_begin * SIZE_OF_BLOCK, SEEK_SET );
  for(i = 0; i <= (NUM_BLOCKS - data_begin) * DATA_ENTRY_SIZE; i = i + DATA_ENTRY_SIZE){
    fseek(file_pointer, i + (data_begin * SIZE_OF_BLOCK), SEEK_SET);

    fread(valid_check, 1 , 1, file_pointer);

    if(strcmp(valid_check, "") == 0){
      sprintf(buffer, "%u", (SIZE_OF_BLOCK * data_begin / 16) + (i / SIZE_OF_BLOCK * 32) + 1 );
      return buffer;
    }
  }
  return "Error";

}

char * get_time(){
  char * holder = (char *)malloc(32 *sizeof(char));
  time_t current_time;
  struct tm * info_time;
  time(&current_time);
  info_time = localtime(&current_time);
  holder = asctime (info_time);
  return holder;
}

void print_meta(my_Meta * meta){ //print out meta information || use for Debugging only
  printf("Name: %s\n", meta->file_name); 
  printf("Extension: %s\n",meta->extension);
  printf("Size: %s\n", meta->file_size);
  printf("Created: %s\n",meta->create_time);
  printf("Modified:: %s\n",meta->modify_time);  
}

