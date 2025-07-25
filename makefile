
all: os-image.bin

os-image.bin: boot/bootloader.asm kernel/entry.asm kernel/kernel.c
	mkdir -p build
	nasm boot/bootloader.asm -f bin -o build/boot.bin
	nasm kernel/entry.asm -f elf -o build/entry.o
	gcc -m32 -ffreestanding -fno-pic -fno-pie -c kernel/kernel.c -o build/kernel.o
	ld -m elf_i386 -Ttext 0x1000 build/entry.o build/kernel.o -o build/kernel.bin --oformat binary
os-image.img: os-image.bin	
	dd if=/dev/zero of=os-image.img bs=512 count=100
	dd if=build/boot.bin of=os-image.img conv=notrunc
	dd if=build/kernel.bin of=os-image.img bs=512 seek=1 conv=notrunc

run: os-image.img
	qemu-system-i386 -hda os-image.img

clean:
	rm -rf build