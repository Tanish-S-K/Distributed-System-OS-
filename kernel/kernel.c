#include "mystdlib.h"
#include "file_system.h"

void kernel_main() {
    clear_screen();
    init_file_system();
    create_dir("names");
    create_dir("random");
    create_file("file1");
    create_file("file2");
    write_file("file1","i am tanish");
    write_file("file2","i wrote this code");
    char buffer[100];
    read_file("file1",buffer);
    print("file1---\n");
    print(buffer);
    print("\n");
    read_file("file2",buffer);
    print("file2---\n");
    print(buffer);
    print("\n");
    list();
    delete_file("file1");
    godir("random");
    create_file("file1");
    list();
    goparent();
    curpos();
    list();

    while (1) {}
}
