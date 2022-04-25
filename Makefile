# ARM GCC COMPILER CALL

# Toolchain To Use
TOOLCHAIN	:= arm-none-eabi-
CC		    := $(TOOLCHAIN)gcc		# c compiler
AS			:= $(TOOLCHAIN)as		# assembler
LD 			:= $(TOOLCHAIN)ld 		# linker
OBJ 		:= $(TOOLCHAIN)objcopy	# Object Copy
READELF		:= $(TOOLCHAIN)readelf	# Object Copy

# -Os				Optimize for Size
# -mcpu=cortex-m4	Compile for the ARM M4 Processor
# mthumb			Target the MTHUMB Instruction Set
# -ftlo				Set Linker Time Optimisations
ARCH 		:= m33
TARGET_ARCH := -mcpu=cortex-$(ARCH)
THUMB		:= -mthumb
WALL		:= -Wall -Wextra
LINKTIME	:= -flto
OPTFLAGS	:= -Os
CFLAGS	  	:= $(OPTFLAGS) $(TARGET_ARCH) $(THUMB) #$(LINKTIME)
ASFLAGS		:= $(TARGET_ARCH) $(THUMB)
LDFLAGS 	:= -T 
OBJFLAGS	:= -O binary
ELFFLAGS	:= -a

INC_DIR   	:= include
SRC_DIR   	:= src
START_DIR 	:= startup
LINK_DIR  	:= linker
OBJ_DIR	  	:= obj
BIN_DIR	  	:= bin
HAL_DIR   	:= hal
BOARD_DIR   := board
MAIN_DIR	:= main
LIB_DIR		:= lib
HAL_DIR		:= hal

#ONLY ONE
STARTUP		:= startup_stm32l552ze.s

#ONLY ONE
LINKER		:= linker_stm32l552ze.ld

OBJS 		:=	$(OBJ_DIR)/main.o \
				$(OBJ_DIR)/pointer.o \
				$(OBJ_DIR)/rcc.o \
				$(OBJ_DIR)/gpio.o \
				$(OBJ_DIR)/timer.o \
				$(OBJ_DIR)/fdcan.o
#	EXAMPLE OF AUTOMATIC VARIABLES
#	%.o: %.c %.h common.h
#		$(CC) $(CFLAGS) -c $<
#
#	$@ Looks at the target
#	(Target)
#	%.o: 			%.c %.h common.h
#	
#	$< Looks at the first source
#			(First Source)
#	%.o: 	%.c 					%.h common.h
#		$(CC) $(CFLAGS) -c $<
#	$^ Looks at the first source
#			(All Source)
#	%.o: 	%.c %.h common.h
#		$(CC) $(CFLAGS) -c $^
release: $(BIN_DIR)/main.bin

# Build An ELF 
$(BIN_DIR)/main.bin: $(BIN_DIR)/main.elf
	$(OBJ) $(OBJFLAGS) $^ $@

# Build An ELF 
$(BIN_DIR)/main.elf: $(LINK_DIR)/$(LINKER) $(OBJS) $(BIN_DIR)/startup.o
	$(LD) $(OPTFLAGS) -s $(LDFLAGS) $^ -o $@

# Build Dependances
$(BIN_DIR)/startup.o: $(START_DIR)/$(STARTUP)
	$(AS) $< $(ASFLAGS) -o $@

$(OBJ_DIR)/%.o: $(MAIN_DIR)/$(SRC_DIR)/%.c
	$(CC) -I $(MAIN_DIR)/$(INC_DIR) -I $(LIB_DIR)/$(INC_DIR) -I $(BOARD_DIR)/$(INC_DIR) -I $(HAL_DIR)/$(INC_DIR) $(CFLAGS) -c  $< -o $@

$(OBJ_DIR)/%.o: $(HAL_DIR)/$(SRC_DIR)/%.c
	$(CC) -I $(HAL_DIR)/$(INC_DIR) -I $(LIB_DIR)/$(INC_DIR) $(CFLAGS) -c  $< -o $@

# Build Library
$(OBJ_DIR)/%.o: $(LIB_DIR)/$(SRC_DIR)/%.c
	$(CC) -I $(LIB_DIR)/$(INC_DIR) $(CFLAGS) -c  $< -o $@

readelf:
	$(READELF) $(ELFFLAGS) $(BIN_DIR)/main.elf

clean:
	rm -f $(OBJ_DIR)/*.o
	rm -f $(BIN_DIR)/*.o
	rm -f $(BIN_DIR)/*.elf
	rm -f $(BIN_DIR)/*.bin

flash:
	STM32_Programmer_CLI -c port=SWD -w $(BIN_DIR)/main.bin 0x08000000

info:
	STM32_Programmer_CLI -c port=SWD

reset:
	STM32_Programmer_CLI -c port=SWD -rst

hard_reset:
	STM32_Programmer_CLI -c port=SWD -hardRst

setup:
	mkdir obj
	mkdir bin

# To Move The Rules
#sudo cp ./rules/*.rules /etc/udev/rules.d/