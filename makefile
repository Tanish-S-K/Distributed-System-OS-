all: os-image.bin

os-image.bin: boot/bootloader.asm kernel/entry.asm kernel/kernel.c kernel/file_system.c kernel/mystdlib.c
	mkdir -p build
	nasm boot/bootloader.asm -f bin -o build/boot.bin
	nasm kernel/entry.asm -f elf -o build/entry.o
	gcc -m32 -ffreestanding -fno-pic -fno-pie -c kernel/kernel.c -o build/kernel.o
	gcc -m32 -ffreestanding -fno-pic -fno-pie -c kernel/file_system.c -o build/file_system.o
	gcc -m32 -ffreestanding -fno-pic -fno-pie -c kernel/mystdlib.c -o build/mystdlib.o
	ld -m elf_i386 -Ttext 0x1000 build/entry.o build/kernel.o build/file_system.o build/mystdlib.o -o build/kernel.bin --oformat binary

os-image.img: os-image.bin
	dd if=/dev/zero of=os-image.img bs=512 count=256
	dd if=build/boot.bin of=os-image.img conv=notrunc
	dd if=build/kernel.bin of=os-image.img bs=512 seek=1 conv=notrunc

run: os-image.img
	qemu-system-i386 -hda os-image.img

clean:
	rm -rf build os-image.img os-image.bin
