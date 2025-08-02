#ifndef MYSTDLIB_H
#define MYSTDLIB_H

#include <stdint.h>

#define TOTAL_SECTORS 256
#define BITMAP_SECTOR 2

void print(const char* str);
void iprint(uint16_t n);
void cprint(const char c);
void input(char *buffer);
int nextarg(char *buffer, int i, char *parse,char end);
void clear_screen();

void outb(uint16_t port, uint8_t val);
void outw(uint16_t port, uint16_t val);
uint8_t inb(uint16_t port);
uint16_t inw(uint16_t port);

void read_sector(uint32_t lba, uint16_t* buffer);
void write_sector(uint32_t lba, uint16_t* buffer);
void clean_sector(uint32_t sector);

void* mem(int count);
void mem_reset(void* ptr,int size);
void mem_cpy(void* dest, const void* src, uint16_t size);

int len(char *s);
int cpy(char *s1, char* s2,int i);
int cmp(const char *a, const char *b);
char* itoc(uint16_t n, char *buffer);

#endif
