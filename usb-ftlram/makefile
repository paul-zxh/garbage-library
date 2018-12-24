# JetOS makefile

CC	:= sparc-elf-gcc
OBJDUMP := sparc-elf-objdump
RM 	:= rm

CFLAGS1	:= -O2 -msoft-float -Tlinkprom -N -L./ -nostdlib -I./inc
CFLAGS2	:= -O2 -msoft-float -I./inc 
OBJECTS := init0.o lb1spc.o lb2spc.o lb3spc.o main.o eflash.o vsprintf.o jtag_con.o mem.o onfi_run.o nflash.o usb.o ftl.o ftl_drv.o

OBJ	:= $(foreach temp, $(OBJECTS), ./out/obj/$(temp))

.PHONY: build clean tcm

./out/udisk.elf	: $(OBJ)
	@$(CC) $(CFLAGS1) -o $@ $(OBJ)
	@$(RM) -rf ./out/obj
	@echo $@ build done.
	

./out/obj/%.o		: ./asm/%.S
	@mkdir -p ./out/obj
	$(CC) -c $(CFLAGS2) -o $@ $<
./out/obj/%.o		: ./src/%.c	
	@mkdir -p ./out/obj
	$(CC) -c $(CFLAGS2) -o $@ $<
./out/obj/%.o		: ./src/crypt/%.c	
	$(CC) -c $(CFLAGS2) -o $@ $<
	
	

clean		:
	@rm -rf ./out/*


