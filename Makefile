MAKEFLAGS += -s
SHELL = /bin/bash
#Flags
Flags_C =  -m32 -ffreestanding -c -IAPI/src
Flags_ASM = -f elf
#Bootloader
Bootloader_ASM = Bootloader/src/Bootloader.asm
Bootloader_BIN = Bootloader/bin/Bootloader.bin
#API
API_SRC = API/src
API_BIN = API/bin
API_O = API/bin/Utilities.o
API_H = API/src/API.h
#Kernel entry
KernelEntry_ASM = Kernel/src/KernelEntry.asm
KernelEntry_O = Kernel/bin/KernelEntry.o
#Kernel
Kernel_C = Kernel/src/Kernel.c
Kernel_O = Kernel/bin/Kernel.o
Kernel_ELF = Kernel/bin/Kernel.elf
Kernel_BIN = Kernel/bin/Kernel.bin
#Programs
Console_C = Programs/src/Console.c
Raycaster_C = Programs/src/Raycaster.c
Programs_BIN = Programs/bin
Programs_O = Programs/bin/Programs.o
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

linkkernel: $(LinkerScript) kernel API programs
	$(CrossCompiler)/i686-elf-ld -T $(LinkerScript) -m elf_i386 $(KernelEntry_O) $(API_O) $(Programs_O) $(Kernel_O) -o $(Kernel_ELF)
	$(CrossCompiler)/i686-elf-objcopy -O binary $(Kernel_ELF) $(Kernel_BIN)

API: $(API_SRC)/Utilities.asm $(API_SRC)/Utilities.c
	nasm $(Flags_ASM) $(API_SRC)/Utilities.asm -o $(API_BIN)/Utilities_asm.o
	$(CrossCompiler)/i686-elf-gcc $(Flags_C) $(API_SRC)/Utilities.c -o $(API_BIN)/Utilities_c.o
	$(CrossCompiler)/i686-elf-gcc $(Flags_C) $(API_SRC)/Utilities_VGA.c -o $(API_BIN)/Utilities_VGA.o
	$(CrossCompiler)/i686-elf-gcc $(Flags_C) $(API_SRC)/Utilities_Audio.c -o $(API_BIN)/Utilities_Audio.o
	
	$(CrossCompiler)/i686-elf-gcc $(Flags_C) $(API_SRC)/Math.c -o $(API_BIN)/Math.o

	$(CrossCompiler)/i686-elf-ld -r -m elf_i386 $(API_BIN)/Utilities_asm.o $(API_BIN)/Utilities_c.o $(API_BIN)/Utilities_VGA.o $(API_BIN)/Utilities_Audio.o $(API_BIN)/Math.o -o $(API_O)

programs:
	$(CrossCompiler)/i686-elf-gcc $(Flags_C) $(Console_C) -o $(Programs_BIN)/Console.o
	$(CrossCompiler)/i686-elf-gcc $(Flags_C) $(Raycaster_C) -o $(Programs_BIN)/Raycaster.o

	$(CrossCompiler)/i686-elf-ld -r -m elf_i386 $(Programs_BIN)/Console.o $(Programs_BIN)/Raycaster.o -o $(Programs_O)

padding: $(Zeroes_ASM)
	nasm $(Zeroes_ASM) -o $(Zeroes_BIN)

image: boot padding
	dd if=$(Bootloader_BIN) of=$(IMG) bs=512 count=2 status=none
	dd if=$(Kernel_BIN) 	of=$(IMG) bs=512 seek=1 status=none
	cat $(Zeroes_BIN) >> $(IMG)

run: image
	qemu-system-x86_64 -drive format=raw,file=$(IMG) -monitor stdio -audiodev pa,id=speaker -machine pcspk-audiodev=speaker
	#qemu-system-i386

clean:
	rm -f $(Bootloader_BIN)
	rm -f $(KernelEntry_O)
	rm -f $(Kernel_O)
	rm -f $(Zeroes_BIN)
	rm -f $(IMG)