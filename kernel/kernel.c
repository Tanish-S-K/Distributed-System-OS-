#include <stdint.h>

void print(const char* str) {
    char *video = (char *)0x0B8000;
    while (*str) {
        *video++ = *str++;
        *video++ = 0x07;
    }
}

void clear_screen(){
    char *video = (char *)0x0B8000;
    for(int i=0;i<80*25;i++){
        *video++ = ' ';
        *video++ = 0x07;
    }
}

void outb(unsigned short port,unsigned char val){
    __asm__ volatile ("outb %0, %1": :"a"(val),"Nd"(port));
}

void outw(unsigned short port,unsigned short val){
    __asm__ volatile ("outw %0, %1": :"a"(val),"Nd"(port));
}

unsigned inb(unsigned short port){
    unsigned char ret;
    __asm__ volatile ("inb %1, %0": "=a"(ret):"Nd"(port));
    return ret;
}

unsigned short inw(unsigned short port){
    unsigned short ret;
    __asm__ volatile ("inw %1, %0":"=a"(ret):"Nd"(port));
    return ret;
}

void read_sector(uint32_t lba,uint16_t* buffer){
    outb(0x1F2,1);
    outb(0x1F3, (uint8_t)(lba));
    outb(0x1F4, (uint8_t)(lba >> 8));
    outb(0x1F5,(uint8_t)(lba>>16));
    outb(0x1F6,0xE0|((lba>>24)&0x0F));
    outb(0x1F7,0x20);
    while ((inb(0x1F7) & 0x80));
    while (!(inb(0x1F7) & 0x08));
    for(int i=0;i<256;i++){
        buffer[i] = inw(0x1F0);
    }
}

void write_sector(uint32_t lba,uint16_t* buffer){
    outb(0x1F2,1);
    outb(0x1F3,(uint8_t)(lba));
    outb(0x1F4,(uint8_t)(lba>>8));
    outb(0x1F5,(uint8_t)(lba>>16));
    outb(0x1F6,0xE0|((lba>>24)&0x0F));
    outb(0x1F7,0x30);


    while ((inb(0x1F7) & 0x80));
    while (!(inb(0x1F7) & 0x08));
    for(int i=0;i<256;i++){
        outw(0x1F0,buffer[i]);
    }
    outb(0x1F7, 0xE7);
    while ((inb(0x1F7) & 0x80));
}

void kernel_main() {
    clear_screen();
    char buffer[512] = "my name is sk tanish";
    write_sector(20,(uint16_t *)buffer);
    read_sector(20,(uint16_t *)buffer);
    buffer[511] = '\0';
    print((char *)buffer);
    while (1) {}
}
