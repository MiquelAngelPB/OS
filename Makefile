MAKEFLAGS += -s
SHELL = /bin/bash
#Flags
Flags_C =  -m32 -ffreestanding -c -IAPI/src
Flags_ASM = -f elf
#Bootloader
Bootloader_ASM = Bootloader/src/Bootloader.asm
Bootloader_BIN = Bootloader/bin/Bootloader.bin
#API
Utilities_ASM = API/src/Utilities.asm
Utilities_C = API/src/Utilities.c
Utilities_ASM_O = API/bin/Utilities_asm.o
Utilities_C_O = API/bin/Utilities_c.o
Utilities_O = API/bin/Utilities.o
API_H = API/src/API.h
#Kernel entry
KernelEntry_ASM = Kernel/src/KernelEntry.asm
KernelEntry_O = Kernel/bin/KernelEntry.o
#Kernel
Kernel_C = Kernel/src/Kernel.c
Kernel_O = Kernel/bin/Kernel.o
Kernel_ELF = Kernel/bin/Kernel.elf
Kernel_BIN = Kernel/bin/Kernel.bin
#Others
LinkerScript = LinkerScript.ld
CrossCompiler = ../GCC-CrossCompiler/out/path/bin
Zeroes_ASM = Kernel/src/Zeroes.asm
Zeroes_BIN = Kernel/bin/Zeroes.bin
IMG = img/os.img

Kernel_SECTORS = $(shell bytes=$$(stat -c%s $(Kernel_BIN)); echo $$(( (bytes + 511) / 512 )))

boot: $(Bootloader_ASM) linkkernel
	nasm $(Bootloader_ASM) -o $(Bootloader_BIN) -D KERNEL_SECTORS=$(Kernel_SECTORS)

kernelentry: $(KernelEntry_ASM)
	nasm $(Flags_ASM) $(KernelEntry_ASM) -o $(KernelEntry_O)

kernel: kernelentry
	#The cross compiler was built with https://github.com/andrewrobinson5/Cross-Compiler-Build-Script by andrewrobinson5
	#Thanks Andrew
	$(CrossCompiler)/i686-elf-gcc $(Flags_C) $(Kernel_C) -o $(Kernel_O)

linkkernel: $(LinkerScript) kernel API
	$(CrossCompiler)/i686-elf-ld -T $(LinkerScript) -m elf_i386 $(KernelEntry_O) $(Utilities_O) $(Kernel_O) -o $(Kernel_ELF)
	$(CrossCompiler)/i686-elf-objcopy -O binary $(Kernel_ELF) $(Kernel_BIN)

API: $(Utilities_ASM) $(Utilities_C)
	nasm $(Flags_ASM) $(Utilities_ASM) -o $(Utilities_ASM_O)
	$(CrossCompiler)/i686-elf-gcc $(Flags_C) $(Utilities_C) -o $(Utilities_C_O)

	$(CrossCompiler)/i686-elf-ld -r -m elf_i386 $(Utilities_ASM_O) $(Utilities_C_O) -o $(Utilities_O)

padding: $(Zeroes_ASM)
	nasm $(Zeroes_ASM) -o $(Zeroes_BIN)

image: boot padding
	dd if=$(Bootloader_BIN) of=$(IMG) bs=512 count=2 status=none
	dd if=$(Kernel_BIN) 	of=$(IMG) bs=512 seek=1 status=none
	cat $(Zeroes_BIN) >> $(IMG)

run: image
	qemu-system-x86_64 -drive format=raw,file=$(IMG) -monitor stdio
	#qemu-system-i386

clean:
	rm -f $(Bootloader_BIN)
	rm -f $(KernelEntry_O)
	rm -f $(Kernel_O)
	rm -f $(Zeroes_BIN)
	rm -f $(IMG)