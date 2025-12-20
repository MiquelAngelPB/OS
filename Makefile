MAKEFLAGS += -s
SHELL = /bin/bash
#Flags
Flags_C =  -m32 -ffreestanding -c -IAPI/include 
Flags_ASM = -f elf
Flags_LD = -r -m elf_i386
#Bootloader
Bootloader_ASM = Bootloader/src/Bootloader.asm
Bootloader_BIN = Bootloader/bin/Bootloader.bin
#API
API = ./API
API_O = API/bin/Utilities.o
#Kernel entry
KernelEntry_ASM = Kernel/src/KernelEntry.asm
KernelEntry_O = Kernel/bin/KernelEntry.o
#Kernel
Kernel_C = Kernel/src/Kernel.c
Kernel_O = Kernel/bin/Kernel.o
Kernel_ELF = Kernel/bin/Kernel.elf
Kernel_BIN = Kernel/bin/Kernel.bin
#Programs
Programs_SRC = $(wildcard Programs/src/**/*.c)
Programs_BIN = $(patsubst Programs/src/%.c, Programs/bin/%.o, $(Programs_SRC))
Programs_O = Programs/bin/Programs.o
#Others
LinkerScript = LinkerScript.ld
CrossCompiler = ../GCC-CrossCompiler/out/path/bin
Zeroes_ASM = Kernel/src/Zeroes.asm
Zeroes_BIN = Kernel/bin/Zeroes.bin
IMG = Img/os.img

Kernel_SECTORS = $(shell bytes=$$(stat -c%s $(Kernel_BIN)); echo $$(( (bytes + 511) / 512 )))

boot: $(Bootloader_ASM) linkkernel
	@echo Kernel sectors: $(Kernel_SECTORS)
	nasm $(Bootloader_ASM) -o $(Bootloader_BIN) -D KERNEL_SECTORS=$(Kernel_SECTORS)

kernelentry: $(KernelEntry_ASM)
	nasm $(Flags_ASM) $(KernelEntry_ASM) -o $(KernelEntry_O)

kernel: kernelentry
	#The cross compiler was built with https://github.com/andrewrobinson5/Cross-Compiler-Build-Script by andrewrobinson5
	#Thanks Andrew
	$(CrossCompiler)/i686-elf-gcc $(Flags_C) $(Kernel_C) -o $(Kernel_O)

linkkernel: $(LinkerScript) kernel API programs padding
	$(CrossCompiler)/i686-elf-ld -T $(LinkerScript) $(Flags_LD) $(KernelEntry_O) $(API_O) $(Programs_O) $(Kernel_O) -o $(Kernel_ELF)
	$(CrossCompiler)/i686-elf-objcopy -O binary $(Kernel_ELF) $(Kernel_BIN)
	cat $(Zeroes_BIN) >> $(Kernel_BIN)

API:
	nasm $(Flags_ASM) $(API)/src/*.asm -o $(API)/bin/API_ASM.o
	$(CrossCompiler)/i686-elf-gcc $(Flags_C) $(API)/src/*.c -o $(API)/bin/API_C.o

	$(CrossCompiler)/i686-elf-ld $(Flags_LD) $(API)/bin/API_ASM.o $(API)/bin/API_C.o -o $(API_O)

#Programs
Programs/bin/%.o: Programs/src/%.c
	@mkdir -p $(dir $@)
	$(CrossCompiler)/i686-elf-gcc $(Flags_C) $< -o $@

programs: $(Programs_BIN)
	@echo $(Programs_BIN)
	$(CrossCompiler)/i686-elf-ld $(Flags_LD) $(Programs_BIN) -o $(Programs_O)

#extras
padding: $(Zeroes_ASM)
	nasm $(Zeroes_ASM) -o $(Zeroes_BIN)

image: boot linkkernel
	dd if=$(Bootloader_BIN) of=$(IMG) bs=512 count=2 status=none
	dd if=$(Kernel_BIN) 	of=$(IMG) bs=512 seek=1 status=none

run: image
	qemu-system-x86_64 -drive format=raw,file=$(IMG) -monitor stdio -audiodev pa,id=speaker -machine pcspk-audiodev=speaker
	#qemu-system-i386

clean:
	rm -f $(Bootloader_BIN)
	rm -f $(KernelEntry_O)
	rm -f $(Kernel_O)
	rm -f $(Zeroes_BIN)
	rm -f $(IMG)