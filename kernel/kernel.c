#include "header/mystdlib.h"
#include "header/file_system.h"
#include "header/auth.h"
#include "header/cli.h"


void kernel_main() {
    clear_screen();
    int i =init_file_system();

    int status;
    while (1){
        status = authenticate(i);

        if (status){
            print("Welcome, User!!!\n");
            break;
        } else{
            clear_screen();
            print("\nWrong Credentials,Give OS to the Owner!!!\n");
        }
    }
    print("Entered the kernal-tan\n");
    cli();

    while (1) {}
}
