#include <stdint.h>
#include "header/mystdlib.h"
#include "header/file_system.h"

int authenticate(int status){
	char buffer[512];
	int i=0;
	char name[100],pass[100];
	read_sector(255,(uint16_t *)buffer);
	
	if(status){
		print("Set Username >> ");
		input(name);
		print("\n");
		print("Set Password >> ");
		input(pass);
		print("\n");
		i = cpy(buffer,name,1);
		buffer[i] = ' ';
		i = cpy(buffer,pass,i+1);
		write_sector(255,(uint16_t *)buffer);
		return 1;
	}
	char username[100],password[100];
	print("Enter the Username : ");
	input(name);
	print("\n");
	print("Enter the Password : ");
	input(pass);
	print("\n");
	i = nextarg(buffer,1,username,' ');
	i = nextarg(buffer,i,password,'\0');
	
	
	return cmp(username,name)&cmp(password,pass);
}
