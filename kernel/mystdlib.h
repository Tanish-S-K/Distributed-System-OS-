#ifndef MYSTDLIB_H
#define MYSTDLIB_H

#include <stdint.h>

#define TOTAL_SECTORS 256
#define BITMAP_SECTOR 2

void print(const char* str);
void clear_screen();

void outb(uint16_t port, uint8_t val);
void outw(uint16_t port, uint16_t val);
uint8_t inb(uint16_t port);
uint16_t inw(uint16_t port);

void read_sector(uint32_t lba, uint16_t* buffer);
void write_sector(uint32_t lba, uint16_t* buffer);

void* mem(int count);
void mem_reset(void* ptr,int size);

int len(char *s);
void cpy(char *s1, char* s2);
int cmp(const char *a, const char *b);

#endif
