#include "mystdlib.h"

#define total_sectors 256

typedef struct {
    char name[19];
    uint8_t type;
    uint16_t parent;
    uint16_t child;
    uint16_t next_sib;
    uint16_t prev_sib;
    uint16_t data_sector;
    uint16_t size;
} DiskNode;

uint16_t bitmap[total_sectors];
uint16_t cur_sec;

void read_node(uint16_t sector, DiskNode* node) {
    uint16_t buffer[256];
    read_sector(sector, buffer);
    mem_cpy(node, buffer, sizeof(DiskNode));
}

void write_node(uint16_t sector, DiskNode* node) {
    uint16_t buffer[256] = {0};
    mem_cpy(buffer, node, sizeof(DiskNode));
    write_sector(sector, buffer);
}

void set_bit(uint16_t sector,uint16_t n){
    bitmap[sector] = n;
    write_sector(2,bitmap);
}

uint16_t salloc() {
    read_sector(2, bitmap);

    for (uint16_t i = 3; i<total_sectors; i++) {
        if (bitmap[i] != 1) {
            bitmap[i] = 1;
            write_sector(2, bitmap);
            return i;
        }
    }
    return 0;
}

void print_bit(){
    read_sector(2,bitmap);
    for (int i=0;i<256;i++){
        iprint(bitmap[i]);
    }
}
void init_file_system() {
    read_sector(2, bitmap);
    if (bitmap[3] == 1) {
        print("File system already exists\n");
        return;
    }
    for (int i=0;i<256;i++){
        bitmap[i] = 0;
    }
    set_bit(3,1);
    
    DiskNode root;
    cpy(root.name, "/");
    root.type = 1;
    root.parent = 0;
    root.child = 0;
    root.next_sib = 0;
    root.prev_sib = 0;
    root.data_sector = 0;
    root.size = 0;
    cur_sec = 3;

    write_node(3, &root);
    print("Initialized new file system\n");
}

uint16_t create_dir(char *name) {
    uint16_t sec = salloc();
    if (!sec) {
        print("Disk full!\n");
        return -1;
    }

    DiskNode dir;
    cpy(dir.name, name);
    dir.type = 1;
    dir.parent = cur_sec;
    dir.child = 0;
    dir.next_sib = 0;
    dir.prev_sib = 0;
    dir.data_sector = 0;
    dir.size = 0;

    DiskNode parent;
    read_node(cur_sec, &parent);
    if (parent.child == 0) {
        parent.child = sec;
        write_node(cur_sec, &parent);
    } else {
        DiskNode ptr;
        uint16_t ptr_sector = parent.child;
        read_node(ptr_sector, &ptr);
        while (ptr.next_sib) {
            ptr_sector = ptr.next_sib;
            read_node(ptr_sector, &ptr);
        }
        ptr.next_sib = sec;
        write_node(ptr_sector, &ptr);
        dir.prev_sib = ptr_sector;
    }
    write_node(sec, &dir);
    set_bit(sec,1);
    return sec;
}

void list() {
    DiskNode current;
    read_node(cur_sec, &current);
    if (current.child == 0) {
        print("No files or directories\n");
        return;
    }
    uint16_t ptr_sector = current.child;
    DiskNode ptr;
    read_node(ptr_sector, &ptr);

    while (1) {
        print(ptr.name);
        print(ptr.type == 1 ? " [DIR]\n" : " [FILE]\n");
        if (ptr.next_sib == 0) break;
        ptr_sector = ptr.next_sib;
        read_node(ptr_sector, &ptr);
    }
}



// file functions 

uint16_t create_file(char* name) {
    uint16_t sec = salloc();
    if (!sec) {
        print("Disk full!\n");
        return 0;
    }

    DiskNode file;
    cpy(file.name, name);
    file.type = 0;
    file.parent = cur_sec;
    file.child = 0;
    file.next_sib = 0;
    file.prev_sib = 0;
    file.data_sector = 0;
    file.size = 0;

    DiskNode parent;
    read_node(cur_sec, &parent);
    if (parent.child == 0) {
        parent.child = sec;
        write_node(cur_sec, &parent);
    } else {
        DiskNode ptr;
        uint16_t ptr_sector = parent.child;
        read_node(ptr_sector, &ptr);
        while (ptr.next_sib) {
            ptr_sector = ptr.next_sib;
            read_node(ptr_sector, &ptr);
        }
        ptr.next_sib = sec;
        write_node(ptr_sector, &ptr);
        file.prev_sib = ptr_sector;
    }

    write_node(sec, &file);
    set_bit(sec,1);
    return sec;
}

uint16_t find_file(char* name){
    DiskNode cur;
    for(int i=4;i<256;i++){
        if (bitmap[i]){
            read_node(i,&cur);
            if(cur.type==0 && cmp(cur.name,name)){
                return i;
            }
        }
    }
    return 0;
}

uint16_t find_dir(char* name){
    DiskNode cur;
    for(int i=4;i<256;i++){
        if (bitmap[i]){
            read_node(i,&cur);
            if(cur.type==1 && cmp(cur.name,name)){
                return i;
            }
        }
    }
    return 0;
}

void write_file(char* name, uint16_t* content) {
    uint16_t file_sec = find_file(name);
    if (!file_sec) {
        print("No Such File Found!!\n");
        return;
    }
    DiskNode file;
    read_node(file_sec,&file);

    for(int i=50;i<256;i++){
        if (!bitmap[i]){
            bitmap[i] = 1;
            char buffer[512];
            write_sector(i,content);
            file.data_sector = i;
            write_node(file_sec,&file);
            return;
        }
    }
}

void read_file(char* name,char *data) {
    uint16_t file_sec = find_file(name);
    if (!file_sec) {
        print("No Such File Found!!\n");
        return;
    }
    DiskNode file;
    read_node(file_sec,&file);
    int data_sec = file.data_sector;
    if (data_sec){
        read_sector(data_sec,(uint16_t*)data);
    }
    else{
        print("Empty!!\n");
    }
}

void delete_file(char* name) {
    uint16_t file_sec = find_file(name);
    if (!file_sec) {
        print("No Such File Exist!!!\n");
        return;
    }

    DiskNode file;
    read_node(file_sec, &file);

    DiskNode parent;
    read_node(file.parent, &parent);

    if (parent.child == file_sec) {
        parent.child = file.next_sib;
        write_node(file.parent, &parent);
        if (file.next_sib) {
            DiskNode sib;
            read_node(file.next_sib, &sib);
            sib.prev_sib = 0;
            write_node(file.next_sib, &sib);
        }
    } else {
        if (file.prev_sib) {
            DiskNode prev;
            read_node(file.prev_sib, &prev);
            prev.next_sib = file.next_sib;
            write_node(file.prev_sib, &prev);
        }
        if (file.next_sib) {
            DiskNode next;
            read_node(file.next_sib, &next);
            next.prev_sib = file.prev_sib;
            write_node(file.next_sib, &next);
        }
    }
    set_bit(file_sec,0);
    clean_sector(file_sec);
    if (file.data_sector) {
        set_bit(file.data_sector,0);
        clean_sector(file.data_sector);
    }

    print("File deleted successfully\n");
}

void godir(char *name){
    uint16_t sec= find_dir(name);
    if (!sec){
        print("No Such Directory Exits!!!!\n");
        return;
    }
    cur_sec = sec;
}

void curpos(){
    DiskNode node;
    read_node(cur_sec,&node);
    print(node.name);
    print("\n");
}

void goparent(){
    DiskNode node;
    read_node(cur_sec,&node);
    if (!node.parent){
        print("NO Parent Exist!!!\n");
    }
    cur_sec = node.parent;
}
