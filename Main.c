#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "file_system_header.h"
#include <time.h>
#include <unistd.h>
////////////Functions Prototype///////////////////////
void print_FAT(my_FAT *);
int make_disk(char *);
FILE mount();
void make_root_dir();
char * find_free_meta();
char * find_free_data();
char * find_free_fat_entry();
char * get_time();
int find_File(char * file_name);
void print_meta(my_Meta *);
void mainDriver();
void display_choices();
void list_files();
void create_file(char * f, char * d);
FILE mount();
void get_choice(char g []);
void file_shutdown();
void write_file(char * data, char * file);
void read_file(char * file);
void delete_file(char * file);
void display_info(char * file);
////////////Global Data Structure//////////////////////
FILE * file_pointer;
char buffer [32]; //use to convert int to string via sprintf
double dir_current;

/////////////////Main Program
int main(){
  int a = make_disk("VIRTUAL_DISK");
  //file_pointer = fopen(DISK, "r+");
  mount();
  mainDriver();
  //char * test = malloc(32 * sizeof(my_FAT*));
  //test = find_free_data();
  //my_FAT * fat_tab =  malloc(32 * sizeof(my_FAT*));
  /*strcpy (fat_tab->check_bit_valid, "1");
  strcpy (fat_tab->meta_data_pointer, "2");
  strcpy (fat_tab->next_pointer, "3");
  strcpy (fat_tab->data_pointer, "4");
  strcpy (fat_tab->file_name, "hel");
  print_FAT(fat_tab);*/
  //make_root_dir();
  //char * time = get_time();
  /*
  my_Meta * meta = malloc(32 * sizeof(my_Meta *));
  strcpy(meta->create_time, time);
  strcpy(meta->modify_time, time);
  print_meta(meta);
  */
  //printf("%s\n", test);
  //char * test = strdup(find_free_fat_entry());
  //printf("%s\n", test);
  
    
}
void mainDriver(){
  char get [3];
  puts("Welcome to my file system. Choose a number correspond to the comannd  below to use the file system!");
  while(1){
    display_choices();
    scanf("%3s", get);
    get_choice(get);
  }

}
void display_choices(){
  printf("1. Create a file\n");
  printf("2. Write a file\n");
  printf("3. Read a file\n");
  printf("4. Delete a file\n");
  printf("5. List files\n");
  printf("6. Create a directory\n");
  printf("7. Display info about a file\n");
  printf("8. Exit the file system!\n");
  
}
void get_choice(char choice [] ){
  char holder_file [15];
  char * data_written = malloc(SIZE_OF_BLOCK*sizeof(char));
  if(strcmp(choice, "1") == 0 ){
    puts("Create a file. Please name your file!");
    scanf("%15s", holder_file);
    create_file(holder_file, "");

  }
  else if(strcmp(choice, "2") == 0){ 
    puts("What is the name of the file you want to write to?");
    scanf("%15s", holder_file);
    puts("What do you want to write?");
    if(read(0, data_written, 512) != 0){
    }
    write_file(data_written, holder_file);
  }
  else if(strcmp(choice, "3") == 0){
    puts("What is the name of the file you want to read?");
    scanf("%15s", holder_file);
    read_file(holder_file);
  }
  else if(strcmp(choice, "4") == 0){
    puts("What is the name of the file you want to delete?");
    scanf("%15s", holder_file);
    delete_file(holder_file);
  }
  else if(strcmp(choice, "5") == 0){
    list_files();

  }else if(strcmp(choice, "6") == 0){
    puts("Create a Directory. Please name your file!");
    scanf("%15s", holder_file);
    create_file(holder_file, "DIR");
  }
  else if(strcmp(choice, "7") == 0){
    puts("Which file would you like to find more info about?");
    scanf("%15s", holder_file);
    display_info(holder_file);
  }
  else if(strcmp(choice, "8") == 0){
    file_shutdown();
  }
  else{
    puts("Invalid Command. Please enter a number!");
  }


}
FILE mount(){
  file_pointer = fopen(DISK, "r+");

  make_root_dir();
  
  return *file_pointer;


}
void print_FAT(my_FAT *  fat){ //use for debugging only
  printf("Bit: %s\n", fat->check_bit_valid); 
  printf("Meta: %s\n",fat->meta_data_pointer);
  printf("Next: %s\n",fat->next_pointer);
  printf("Data: %s\n",fat->data_pointer );
  printf("Name: %s\n",fat->file_name);
}

void make_root_dir(){
  //file_pointer = fopen(DISK, "r+");
  fseek(file_pointer, 0, SEEK_SET);//appropriate when load into a mounted disk

  /*            Initialize the first FAT in the linked list          */
  my_FAT * root = (my_FAT *)malloc(32 * sizeof(my_FAT*));

  strcpy(root->file_name, "ROOT");
  //printf("Root Name:%s\n", root->file_name);
  strcpy(root->check_bit_valid, "1");

  sprintf(buffer, "%u", SIZE_OF_BLOCK * meta_begin / 15 + 1 );
  strcpy(root->meta_data_pointer, buffer);

  sprintf(buffer, "%u", SIZE_OF_BLOCK * data_begin / 15 + 1 );
  strcpy(root->data_pointer, buffer);

  strcpy(root->next_pointer, "-1");

  fwrite(root, 32 * sizeof(my_FAT*), 1, file_pointer);

  //print_FAT(root);

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
  strcpy(root_meta->extension, "DIR");
  strcpy(root_meta->file_size, "000");
  strcpy(root_meta->create_time, current_time);
  strcpy(root_meta->modify_time, mod_time);
  fwrite(root_meta, 32*sizeof(my_Meta *), 1, file_pointer);
  //printf("Success created root meta\n");
  //print_meta(root_meta);

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
      sprintf(buffer, "%u", (meta_begin * SIZE_OF_BLOCK / 16) + 1 + (i / META_ENTRY_SIZE) );
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
      sprintf(buffer, "%u", (SIZE_OF_BLOCK * data_begin / 16) + (i / SIZE_OF_BLOCK * 32) + 1 ); //allocate enough space to write :P
      return buffer;
    }
  }
  return "Error";

}
int find_File(char * file_name){
  char valid_check[12];
  unsigned int return_index = -1;
  fseek(file_pointer, 0, SEEK_SET);
  for(int i = 0; i < (meta_begin * SIZE_OF_BLOCK); i = i + FAT_ENTRY_SIZE){
    fseek(file_pointer, i + 1, SEEK_SET);
    fread(valid_check, 12, 1, file_pointer);
    if(strcmp(valid_check, file_name) == 0){
      return_index = (i / 16) + 1;
      return return_index; //return the index inside the FAT
    }

  }

  return return_index; //in case, file cannot be found

}
char * get_time(){ //get the current time including the date
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

void create_file(char * file_name, char * dir){
  
  my_FAT * new_fat_entry = malloc(32 * sizeof(my_FAT));
  my_Meta * new_meta_entry = malloc(32 * sizeof(my_Meta));
  
  char * meta_index = strdup(find_free_meta());
  char * data_index = strdup(find_free_data());
  char * FAT_index = strdup(find_free_fat_entry());
  
  if(strcmp(meta_index, "Error") == 0){
    puts("Failed to get a free spot in meta section");
  }
  if(strcmp(data_index, "Error") == 0){
    puts("Failed to get a free spot in data section");
  }
  if(strcmp(FAT_index, "Error") == 0){
    puts("Failed to get a free spot in the FAT");
  }
  //if file is a directory
  if (strcmp(dir, "DIR") == 0){
    int current_data = atoi(data_index) - 1;
    dir_current = 16 *current_data;
  }
  //
  double meta_i = atol(meta_index) - 1;
  double data_i = atol(data_index) - 1;
  double FAT_i = atol(FAT_index) - 1;
  /*Fat ENTRY*/
  fseek(file_pointer, 16 * FAT_i, SEEK_SET);
  strcpy(new_fat_entry->check_bit_valid, "1");
  strcpy(new_fat_entry->file_name, file_name);
  strcpy(new_fat_entry->meta_data_pointer, meta_index);
  strcpy(new_fat_entry->data_pointer, data_index);
  strcpy(new_fat_entry->next_pointer, "-1");
  fwrite(new_fat_entry, 512, 1, file_pointer);
  print_FAT(new_fat_entry);
  //Meta ENTRY
  
  fseek(file_pointer, 16 * meta_i, SEEK_SET);
  strcpy(new_meta_entry->file_name, file_name);
  strcpy(new_meta_entry->extension, "");
  if(strcmp(dir, "DIR") == 0){
    strcpy(new_meta_entry->extension, "DIR");
  }
  strcpy(new_meta_entry->file_size, "512");
  strcpy(new_meta_entry->create_time, get_time());
  strcpy(new_meta_entry->modify_time, get_time());
  fwrite(new_meta_entry, 512, 1, file_pointer);
  print_meta(new_meta_entry);
  printf("FILE CREATED SUCCESS!\n");

  size_t i = 0;
  char valid_read[1];
  for( i = 0; i < 32; i++){
    fseek(file_pointer, dir_current + (i * 16), SEEK_SET);
    fread(valid_read, 1, 1, file_pointer);
    if(strcmp(valid_read, "") == 0){
      fseek(file_pointer, -1, SEEK_CUR);
      fwrite(file_name, 16, 1, file_pointer);
      break;
    }
  }

  

}
void write_file(char * data, char * file_name){
  //find the FAT index of the file
  //check length of written data and make sure it is not over the size of block
  //data_begin + 16 th place next to the FAT index of the file
  int data_length = strlen(data);
  if(data_length >= SIZE_OF_BLOCK){
    printf("Exceeding the permitted amount\n");
    return;

  }
  int FAT_index = find_File(file_name);
  
  if(FAT_index == -1){
    printf("File index not found from WRITE\n");
    return;
  }
  char * data_pointer = malloc(9*sizeof(char));
  int data_offset;

  fseek(file_pointer, 16 * (FAT_index - 1), SEEK_SET); //go from begin to current FAT_index

  fseek(file_pointer, data_begin + 16, SEEK_CUR); //go to data section

  fread(data_pointer, 1, 9, file_pointer);

  data_offset = atoi(data_pointer);

  fseek(file_pointer, 16 * (data_offset - 1), SEEK_SET);

  fwrite(data, 1, SIZE_OF_BLOCK, file_pointer);

}
void read_file(char * file_name){
  
  int FAT_index = find_File(file_name);
  
  if(FAT_index == -1){
    printf("Unable to find FAT index from READ\n");
    return;
  }
  char data_pointer[9];
  
  int data_offset;

  fseek(file_pointer, 16 * (FAT_index - 1), SEEK_SET); //go from begin to current FAT_index

  fseek(file_pointer, data_begin + 16, SEEK_CUR); //go to data section

  fread(data_pointer, 1, 9, file_pointer);

  data_offset = atoi(data_pointer);

  fseek(file_pointer, 16 *(data_offset -1), SEEK_SET);

  char holder[SIZE_OF_BLOCK];

  fread(holder, 1, SIZE_OF_BLOCK, file_pointer);
  //printf("%c", holder[1]);
  //printf("%c", holder[2]);
  //printf("%c", holder[3]);
  puts("------------------------------------");
  for(int i = 0; i < strlen(holder); i++){
    printf("%c", holder[i]);
    //printf("WHAAAAAAAAA\n");
    
  } 
  printf("\n");
  puts("------------------------------------");
  

}
void list_files(){
  char file_name[15] = {""};
  size_t i;
  printf("Here is a list of file on this virtual disk:\n");
  for(i = 0; i < 32; i++){
    fseek(file_pointer, dir_current + (i * 16), SEEK_SET);
    fread(file_name, 1, 15, file_pointer);
    printf("%s    ", file_name);
  }
  printf("\n");

}
void file_shutdown(){
  puts("Exited the file system. All created files on this virtual disk is destroyed. Good bye!");
  fclose(file_pointer);
  exit(0);

}
void delete_file(char * file_name){

  my_Data * clean_data = malloc(DATA_ENTRY_SIZE * sizeof(char)); //get an empty 512 blocks

  my_Meta * clean_meta = malloc(META_ENTRY_SIZE * sizeof(char));

  my_FAT * clean_FAT_entry = malloc(FAT_ENTRY_SIZE * sizeof(char));

  int FAT_index = find_File(file_name); //get the FAT index of this file
  
  if(FAT_index == -1){
    printf("No such file existed on this disk\n");
    return;
  }
  /*Delete the FAT entry of the file*/
  fseek(file_pointer, 16 * (FAT_index - 1), SEEK_SET);
  fwrite(clean_FAT_entry, sizeof(my_FAT),1, file_pointer);

  /*Delete the data of this file*/
  
  char * data_pointer = malloc(9 * sizeof(char));
  
  fseek(file_pointer, 16 * (FAT_index - 1), SEEK_SET);       //move to the current file FAT index
  fseek(file_pointer, 16 + data_begin, SEEK_CUR);         //move to the current data section of this FAT index by 3 offset
  fread(data_pointer, 1, 9, file_pointer);              //read and give free space to data pointer
  int offset = atoi(data_pointer) - 1;
  fseek(file_pointer, offset * 16, SEEK_SET );         //translate the amount of space from data pointer and move from beginning of file to the position
  fwrite(clean_data, sizeof(my_Data),1, file_pointer); //write an empty blocks to it

  /*Delete the meta data of this file*/
  char * meta_pointer = malloc(9 * sizeof(char));

  fseek(file_pointer, 16 * (FAT_index - 1), SEEK_SET);
  fseek(file_pointer, 16 - data_begin, SEEK_CUR);
  fread(meta_pointer, 1, 9, file_pointer);
  int meta_offset = atoi(meta_pointer) - 1;
  fseek(file_pointer, meta_offset * 16, SEEK_SET);
  fwrite(clean_meta, sizeof(my_Meta), 1, file_pointer);

  /*Undisplay the deleted file in the dir section*/
  char buffer [32] = {""};
  char delete [16] = {""};
  size_t i = 0;
  while(i < 32){
    fseek(file_pointer, dir_current + (16 * i), SEEK_SET); //go through the current directory in the file sys
    fread(buffer, 1, 15, file_pointer); //read in the contents of the directory
    if(strcmp(buffer, file_name) == 0){ //if there is the name of the file you want to delete
      fseek(file_pointer, dir_current + (16 * i), SEEK_SET); //move the file pointer to it
      fwrite(delete, sizeof(16), 1, file_pointer);
    }
    i++;
  }
}
void display_info(char * file_name){
  char name [15] = "";
  char extension[3] = "";
  char time_created[27] = "";
  char time_modified[27] = "";
  
  int FAT_index = find_File(file_name);
  
  if(FAT_index == -1){
    printf("FAT index not found from DISPLAY INFO\n");
    return;

  }
  fseek(file_pointer, 16 * (FAT_index - 1), SEEK_SET);
  fseek(file_pointer, 13, SEEK_CUR);
  char meta_pointer [10];
  int holder;
  fread(meta_pointer, 1, 10, file_pointer);
  holder = atoi(meta_pointer) - 1;

  fseek(file_pointer, 16 * holder, SEEK_SET);
  
  fread(name, 1, 15, file_pointer); //Name
  fread(extension, 1, 3, file_pointer); //Extension
  fread(time_created, 1, 27, file_pointer); //Created time
  fread(time_modified, 1, 27, file_pointer); //Modified time

  printf("Time Created:%s\n", time_created);
  printf("Time Modified:%s\n", time_modified);
}
void change_directory(char * dir_name){
  //to be implemented
}
