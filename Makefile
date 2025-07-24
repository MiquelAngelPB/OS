Bootloader_ASM = Bootloader/src/Bootloader.asm
Bootloader_BIN = Bootloader/bin/Bootloader.bin
KernelEntry_ASM = Kernel/src/KernelEntry.asm
KernelEntry_O = Kernel/bin/KernelEntry.o
Kernel_C = Kernel/src/Kernel.c
Kernel_O = Kernel/bin/Kernel.o
Kernel_ELF = Kernel/bin/Kernel.elf
Kernel_BIN = Kernel/bin/Kernel.bin
LinkerScript = LinkerScript.ld
CrossCompiler = ../GCC-CrossCompiler/out/path/bin
IMG = img/os.img

hello:
	echo "Hello World"

boot: $(Bootloader_ASM)
	nasm $(Bootloader_ASM) -o $(Bootloader_BIN)

kernelentry: $(KernelEntry_ASM)
	nasm -f elf $(KernelEntry_ASM) -o $(KernelEntry_O)

kernel: kernelentry
	#The cross compiler was built with https://github.com/andrewrobinson5/Cross-Compiler-Build-Script by andrewrobinson5
	#Thank you Andrew
	$(CrossCompiler)/i686-elf-gcc -m32 -ffreestanding -c $(Kernel_C) -o $(Kernel_O)
	$(CrossCompiler)/i686-elf-ld -T $(LinkerScript) -m elf_i386 $(KernelEntry_O) $(Kernel_O) -o $(Kernel_ELF)
	$(CrossCompiler)/i686-elf-objcopy -O binary $(Kernel_ELF) $(Kernel_BIN)


image: boot kernel
	dd if=$(Bootloader_BIN) of=$(IMG) bs=512 count=2
	dd if=$(Kernel_BIN) of=$(IMG) bs=512 count=2 seek=1

run: image
	qemu-system-x86_64 -drive format=raw,file=$(IMG) -monitor stdio
	#qemu-system-i386

clean:
	rm -f $(Bootloader_BIN)
	rm -f $(KernelEntry_O)
	rm -f $(Kenel_O)
	rm -f $(IMG)