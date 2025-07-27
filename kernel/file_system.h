#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include "mystdlib.h"

#define total_sectors 256

typedef struct {
    char name[16];
    uint8_t type;
    uint16_t parent;
    uint16_t child;
    uint16_t next_sib;
    uint16_t prev_sib;
    uint16_t data_sector;
    uint16_t size;
} DiskNode;

extern uint16_t cur_sec;
void init_file_system();

uint16_t create_dir(char *name);
uint16_t create_file(char* name);
void write_file(char* name, char* content);
void read_file(char* name,char *data);
void delete_file(char* name);
void godir(char *name);
void curpos();
void goparent();

void list();


#endif
