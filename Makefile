Bootloader_ASM = Bootloader/src/Bootloader.asm
Bootloader_BIN = Bootloader/bin/Bootloader.bin
Kernel_ASM = Kernel/src/Kernel.asm
Kernel_BIN = Kernel/bin/Kernel.bin
IMG = img/os.img

hello:
	echo "Hello World"

boot: $(Bootloader_ASM)
	nasm $(Bootloader_ASM) -o $(Bootloader_BIN)

kernel: $(Kernel_ASM)
	nasm $(Kernel_ASM) -o $(Kernel_BIN)

image: boot kernel
	dd if=$(Bootloader_BIN) of=$(IMG) bs=512 count=2
	dd if=$(Kernel_BIN) of=$(IMG) bs=512 count=2 seek=1

	#cat $(Bootloader_BIN) $(Kernel_BIN) > $(IMG)

run: image
	qemu-system-x86_64 -drive format=raw,file=$(IMG)
	#qemu-system-i386

clean:
	rm -f $(Bootloader_BIN)
	rm -f $(Kernel_BIN)
	rm -f $(IMG)