MAKEFLAGS += -s
SHELL = /bin/bash
#Flags
Flags_C =  -m32 -ffreestanding -fno-pic -fno-pie -no-pie -c -IAPI/include 
Flags_ASM = -f elf
Flags_LD = -r -m elf_i386
#Bootloader
Bootloader_ASM = Bootloader/src/Bootloader.asm
Bootloader_BIN = Bootloader/bin/Bootloader.bin
#API
API_SRC_C = $(wildcard API/src/*.c)
API_BIN_C = $(patsubst API/src/%.c, API/bin/%.o, $(API_SRC_C))

API_SRC_ASM = $(wildcard API/src/*.asm)
API_BIN_ASM = $(patsubst API/src/%.asm, API/bin/%.o, $(API_SRC_ASM))

API_BIN = $(API_BIN_C) $(API_BIN_ASM)
API_O = API/bin/API.o
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
IMG = Img/os.img

boot: $(Bootloader_ASM) linkkernel
	bytes=$$(stat -c%s $(Kernel_BIN)); 					\
	#padding=$$(((512 - ($$bytes % 512)) % 512 )); 		\
	sectors=$$((($$bytes + 511) / 512 )); 	\
	echo "DEBUG -> bytes=$$bytes, padding=$$padding, sectors=$$sectors"; \
	nasm $(Bootloader_ASM) -o $(Bootloader_BIN) -D KERNEL_SECTORS=$$sectors

kernelentry: $(KernelEntry_ASM)
	nasm $(Flags_ASM) $(KernelEntry_ASM) -o $(KernelEntry_O)

kernel: kernelentry
	#The cross compiler was built with https://github.com/andrewrobinson5/Cross-Compiler-Build-Script by andrewrobinson5
	#Thanks Andrew
	$(CrossCompiler)/i686-elf-gcc $(Flags_C) $(Kernel_C) -o $(Kernel_O)

linkkernel: $(LinkerScript) kernel $(API_O)
	$(CrossCompiler)/i686-elf-ld -T $(LinkerScript) $(KernelEntry_O) $(Kernel_O) $(Programs_O) $(API_O) -o $(Kernel_ELF)
       	
	$(CrossCompiler)/i686-elf-objcopy -O binary $(Kernel_ELF) $(Kernel_BIN)

	#Add padding to reach 512 bytes multiple
	bytes=$$(stat -c%s $(Kernel_BIN)); 				\
	padding=$$(((512 - ($$bytes % 512)) % 512 )); 	\
	echo "Kernel size (without padding): $$(stat -c%s $(Kernel_BIN))"; \
	echo "Padding kernel with $$padding bytes"; 	\
	dd if=/dev/zero bs=1 count=$$padding >> $(Kernel_BIN); \
	echo "Kernel size (with padding): $$(stat -c%s $(Kernel_BIN))";

#API
API/bin/%.o: API/src/%.c
	$(CrossCompiler)/i686-elf-gcc $(Flags_C) $< -o $@

API/bin/%.o: API/src/%.asm
	nasm $(Flags_ASM) $< -o $@

$(API_O): $(API_BIN)
	$(CrossCompiler)/i686-elf-ld $(Flags_LD) $(API_BIN_C) $(API_BIN_ASM) -o $(API_O)

#Programs
Programs/bin/%.o: Programs/src/%.c
	@mkdir -p $(dir $@)
	$(CrossCompiler)/i686-elf-gcc $(Flags_C) $< -o $@

$(Programs_O): $(Programs_BIN)
	$(CrossCompiler)/i686-elf-ld $(Flags_LD) $(Programs_BIN) -o $(Programs_O)

#Final image
image: boot linkkernel
	dd if=$(Bootloader_BIN) of=$(IMG) bs=512 count=1 status=none
	dd if=$(Kernel_BIN) 	of=$(IMG) bs=512 seek=1 status=none

run: image
	qemu-system-x86_64 -drive format=raw,file=$(IMG) -monitor stdio -audiodev pa,id=speaker -machine pcspk-audiodev=speaker #Pause to use with gdb -S -s
	#qemu-system-i386

clean:
	find . -type f -name "*.o" -delete
	find . -type f -name "*.bin" -delete
	find . -type f -name "*.elf" -delete	
