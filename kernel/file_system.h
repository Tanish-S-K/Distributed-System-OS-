#ifndef FILESYSTEM_H
#define FILESYSTEM_H

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
    int child_count;
    int* start;
    int size;
} Node;

extern Node* root;
extern Node* cur_dir;

void init_file_system();
void create_dir(char *name);
Node* find_sib(char *name);
void godir(char *name);
void list();
void goparent();
void displaydir();
void create_file(char *name);
Node* find_file(char *name);
Node* find(char *name);
void write_file(char *name, char* buffer);
void read_file(char *name, char *buffer);
void delete(char *name);
#endif
