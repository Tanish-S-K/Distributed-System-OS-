#include "mystdlib.h"

static uint8_t sector_bitmap[TOTAL_SECTORS / 8];

void print(const char* str) {
    static int row = 0, col = 0;
    char* video = (char*)0xB8000;
    while (*str) {
        if (*str == '\n') {
            row++;
            col = 0;
        } else {
            int index = (row * 80 + col) * 2;
            video[index] = *str;
            video[index + 1] = 0x07;
            col++;
            if (col >= 80) {
                col = 0;
                row++;
            }
        }
        str++;
    }
}


void clear_screen() {
    char *video = (char *)0x0B8000;
    for (int i = 0; i < 80 * 25; i++) {
        *video++ = ' ';
        *video++ = 0x07;
    }
}

void outb(uint16_t port, uint8_t val) {
    __asm__ volatile ("outb %0, %1" : : "a"(val), "Nd"(port));
}

void outw(uint16_t port, uint16_t val) {
    __asm__ volatile ("outw %0, %1" : : "a"(val), "Nd"(port));
}

uint8_t inb(uint16_t port) {
    uint8_t ret;
    __asm__ volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

uint16_t inw(uint16_t port) {
    uint16_t ret;
    __asm__ volatile ("inw %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

void read_sector(uint32_t lba, uint16_t* buffer) {
    outb(0x1F2, 1);
    outb(0x1F3, (uint8_t)(lba));
    outb(0x1F4, (uint8_t)(lba >> 8));
    outb(0x1F5, (uint8_t)(lba >> 16));
    outb(0x1F6, 0xE0 | ((lba >> 24) & 0x0F));
    outb(0x1F7, 0x20);
    while ((inb(0x1F7) & 0x80));
    while (!(inb(0x1F7) & 0x08));
    for (int i = 0; i < 256; i++) {
        buffer[i] = inw(0x1F0);
    }
}

void write_sector(uint32_t lba, uint16_t* buffer) {
    outb(0x1F2, 1);
    outb(0x1F3, (uint8_t)(lba));
    outb(0x1F4, (uint8_t)(lba >> 8));
    outb(0x1F5, (uint8_t)(lba >> 16));
    outb(0x1F6, 0xE0 | ((lba >> 24) & 0x0F));
    outb(0x1F7, 0x30);
    while ((inb(0x1F7) & 0x80));
    while (!(inb(0x1F7) & 0x08));
    for (int i = 0; i < 256; i++) {
        outw(0x1F0, buffer[i]);
    }
    outb(0x1F7, 0xE7);
    while ((inb(0x1F7) & 0x80));
}


int len(char *s){
    int cnt = 0;
    while (*s) {
        s++;
        cnt++;}
    return cnt;
}


void cpy(char *s1, char *s2) {
    while (*s2) {
        *s1++ = *s2++;}
    *s1 = '\0';
}


int cmp(const char *a, const char *b) {
    while (*a && *b) {
        if (*a != *b) return 0;
        a++;
        b++;
    }
    return *a == *b;
}

static char memory_pool[512*100];

void* mem(int count){
    int size=count*sizeof(char);
    for(int i=0;i<=512*100-size;i++){
        int found=1;
        for(int j=0;j<size;j++){
            if(memory_pool[i+j]!=0){
                found=0;
                break;
            }
        }
        if(found){
            for(int j=0;j<size;j++){
                memory_pool[i+j]=1;
            }
            return &memory_pool[i];
        }
    }
    return 0;
}

void mem_reset(void* ptr,int size){
    char* p=(char*)ptr;
    if(p<memory_pool||p+size>memory_pool+512*100){
        return;
    }
    for(int i=0;i<size;i++){
        p[i]=0;
    }
}

