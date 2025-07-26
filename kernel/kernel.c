#include "mystdlib.h"
#include "file_system.h"

void kernel_main() {
    clear_screen();
    init_file_system();
    create_dir("names");
    create_dir("address");
    create_dir("salary");
    create_file("files");
    list();
    delete("files");
    list();
    while (1) {}
}
