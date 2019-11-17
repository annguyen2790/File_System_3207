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
int  find_free_fat_entry(char *);
char * get_time();
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
  //make_root_dir();
  char * time = get_time();
  printf("%s\n", time);
    
}

FILE mount(){
  file_pointer = fopen(DISK, "r+");
  return *file_pointer;


}
void print_FAT(my_FAT * fat){
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
  //printf("%s\n", root->file_name);
  strcpy(root->check_bit_valid, "1");

  sprintf(buffer, "%u", SIZE_OF_BLOCK * meta_begin / 16 + 1 );
  strcpy(root->meta_data_pointer, buffer);

  sprintf(buffer, "%u", SIZE_OF_BLOCK * data_begin / 16 + 1 );
  strcpy(root->data_pointer, buffer);
  
  strcpy(root->next_pointer, "-1");

  fwrite(root, 32 * sizeof(my_FAT*), 1, file_pointer);
  
  print_FAT(root);

  /*          Initialize the first data area                 */
  fseek(file_pointer, SIZE_OF_BLOCK * data_begin, SEEK_SET);
  dir_current = data_begin * SIZE_OF_BLOCK;
  fputs(".", file_pointer);

  /*          Initialize the first meta area                */
  fseek(file_pointer, SIZE_OF_BLOCK * meta_begin, SEEK_SET);
  

}
char * find_free_meta(){
  return NULL;

}

char * find_free_data(){
  return NULL;

}
int find_free_fat_entry(char * file_name){
  return -1;
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
