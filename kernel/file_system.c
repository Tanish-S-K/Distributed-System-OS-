#include "mystdlib.h"

typedef enum {
    file_node,
    directory_node
} NodeType;

typedef struct Node {
    char name[16];
    NodeType type;
    struct Node* parent;
    struct Node* childs;
    struct Node* sib;
    struct Node* prev;
    int child_count;
    int *start;
    int size;
} Node;

Node* root;
Node* cur_dir;

void init_file_system() {
    root = (Node*)mem(sizeof(Node));
    cpy(root->name, "/");
    root->type = directory_node;
    root->parent = 0;
    root->childs = 0;
    root->sib = 0;
    root->prev = 0;
    cur_dir = root;
}

void displaydir(){
    print("Current Directory ");
    print(cur_dir->name);
    print("\n");
}

void create_dir(char *name){
    Node* dir = (Node*)mem(sizeof(Node));
    cpy(dir->name, name);
    dir->type = directory_node;
    dir->parent = cur_dir;
    dir->childs = 0;
    dir->sib = 0;
    dir->prev = 0;
    dir->start = 0;
    dir->size = 0;

    if (!cur_dir->childs) {
        cur_dir->childs = dir;
    } else {
        Node* ptr = cur_dir->childs;
        while (ptr->sib) {
            ptr = ptr->sib;
        }
        ptr->sib = dir;
        dir->prev = ptr;
    }
}

Node* find_sib(char *name) {
    Node* ptr = cur_dir->childs;
    while (ptr) {
        if (cmp(name, ptr->name)) return ptr;
        ptr = ptr->sib;
    }
    return 0;
}

void godir(char *name) {
    Node* ptr = find_sib(name);
    if (!ptr || ptr->type != directory_node) {
        print("Directory Does Not Exist!!!\n");
        return;
    }
    cur_dir = ptr;
    displaydir();
}

void goparent(){
    cur_dir = cur_dir->parent;
    displaydir();
}

void list() {
    Node* ptr = cur_dir->childs;
    while (ptr) {
        print(ptr->name);
        print(ptr->type == directory_node ? " [DIR]" : " [FILE]");
        print("\n");
        ptr = ptr->sib;
    }
}


// files management functions

void create_file(char *name){
    Node* dir = (Node*)mem(sizeof(Node));
    cpy(dir->name, name);
    dir->type = file_node;
    dir->parent = cur_dir;
    dir->childs = 0;
    dir->sib = 0;
    dir->prev = 0;
    dir->start = 0;
    dir->size = 0;

    if (!cur_dir->childs) {
        cur_dir->childs = dir;
    } else {
        Node* ptr = cur_dir->childs;
        while (ptr->sib) {
            ptr = ptr->sib;
        }
        ptr->sib = dir;
        dir->prev = ptr;
    }
}

Node* find_file(char *name) {
    Node* ptr = cur_dir->childs;
    while (ptr) {
        if (ptr->type == file_node && cmp(name, ptr->name)) {
            return ptr;
        }
        ptr = ptr->sib;
    }
    return 0;
}
Node* find(char *name) {
    Node* ptr = cur_dir->childs;
    while (ptr) {
        if (cmp(name, ptr->name)) {
            return ptr;
        }
        ptr = ptr->sib;
    }
    return 0;
}

void write_file(char *name, char* buffer) {
    Node* f = find_file(name);
    if (!f) {
        print("File does not exist\n");
        return;
    }
    int l = len(buffer);
    f->start = (int*)mem(l+ 1);
    f->size = l;

    char *ptr = (char*)f->start;
    for (int i = 0; i < l; i++) {
        ptr[i] = buffer[i];
    }
    ptr[l] = '\0';
}

void read_file(char *name, char *buffer) {
    Node* f = find_file(name);
    if (!f || !f->start) {
        print("File not found or empty\n");
        return;
    }
    char *src = (char*)f->start;
    for (int i = 0; src[i]; i++) {
        buffer[i] = src[i];
    }
    buffer[f->size] = '\0';
}

void delete(char *name){
    Node* f = find(name);
    if (!f){
        print("No Such File!!\n");
        return;
    }
    if (f == cur_dir->childs) {
        cur_dir->childs = f->sib;
        if (f->sib) {
            f->sib->prev = 0;
        }
    } else {
        if (f->prev) {
            f->prev->sib = f->sib;
        }
        if (f->sib) {
            f->sib->prev = f->prev;
        }
    }
    if (f->type == file_node && f->start) {
        mem_reset(f->start, f->size + 1);
        f->start = 0;
        f->size = 0;
    }

    mem_reset(f, sizeof(Node));
    print("Deleted\n");
}

