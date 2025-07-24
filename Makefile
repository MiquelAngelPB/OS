Bootloader_ASM = Bootloader/src/Bootloader.asm
Bootloader_BIN = Bootloader/bin/Bootloader.bin
KernelEntry_ASM = Kernel/src/KernelEntry.asm
KernelEntry_BIN = Kernel/bin/KernelEntry.bin
IMG = img/os.img

hello:
	echo "Hello World"

boot: $(Bootloader_ASM)
	nasm $(Bootloader_ASM) -o $(Bootloader_BIN)

kernelentry: $(KernelEntry_ASM)
	nasm $(KernelEntry_ASM) -o $(KernelEntry_BIN)

kernel:
	echo "Here is where the kernel is compiled and linked with the kernel entry"

image: boot kernelentry
	dd if=$(Bootloader_BIN) of=$(IMG) bs=512 count=2
	dd if=$(KernelEntry_BIN) of=$(IMG) bs=512 count=2 seek=1

run: image
	qemu-system-x86_64 -drive format=raw,file=$(IMG) -monitor stdio
	#qemu-system-i386

clean:
	rm -f $(Bootloader_BIN)
	rm -f $(KernelEntry_BIN)
	rm -f $(IMG)