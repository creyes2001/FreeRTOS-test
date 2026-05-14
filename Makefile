#====================Directories===================================
STM_ROOT_DIR = /home/reyes/embedded/STM32
STM_STARTUP_DIR = $(STM_ROOT_DIR)/startup
STM_INCLUDE_DIR = $(STM_ROOT_DIR)/include
LDSCRIPT = $(STM_ROOT_DIR)/STM32F446RETx_FLASH.ld

BUILD_DIR = build
OBJ_DIR = $(BUILD_DIR)/obj
BIN_DIR = $(BUILD_DIR)/bin

INCLUDE_DIRS = $(STM_INCLUDE_DIR)\
			   ./inc

#=====================TOOLCHAIN=================================
CC = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy
SIZE = arm-none-eabi-size


#=====================Files======================================
TARGET  = FreeTest

SOURCES = main.c\
		  src/gpio.c

ASM_SRCS = $(STM_STARTUP_DIR)/startup_stm32f446xx.s


C_OBJS = $(patsubst %.c,$(OBJ_DIR)/%.o,$(notdir $(SOURCES)))
ASM_OBJS = $(patsubst %.s,$(OBJ_DIR)/%.o,$(notdir $(ASM_SRCS)))
OBJECTS = $(C_OBJS) $(ASM_OBJS)


#=====================Flags==================================
MCU = -mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard
WFLAGS = -Wall -Wextra -Werror -Wshadow
CFLAGS = $(MCU) -O0 -g $(WFLAGS) -DSTM32F446xx $(addprefix -I,$(INCLUDE_DIRS))
LDFLAGS = $(MCU) -T $(LDSCRIPT) --specs=nosys.specs --specs=nano.specs
LDFLAGS += -Wl,--gc-sections


#Tell Make where to find .c and .s source files
VPATH = src:$(STM_STARTUP_DIR)

all: $(BUILD_DIR)/$(TARGET).elf $(BIN_DIR)/$(TARGET).bin 

#Binary
$(BIN_DIR)/$(TARGET).bin: $(BUILD_DIR)/$(TARGET).elf
	@mkdir -p $(dir $@)
	$(OBJCOPY) -O binary $< $@

##Link
$(BUILD_DIR)/$(TARGET).elf: $(OBJECTS)
	@mkdir -p $(dir $@)
	$(CC)  $(LDFLAGS) $^ -o $@

##Compile c
$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c -o $@ $^

#Assemble
$(OBJ_DIR)/%.o: %.s 
	@mkdir -p $(dir $@)
	$(CC) $(MCU) -c $< -o $@
	

size:
	$(SIZE) $(BUILD_DIR)/$(TARGET).elf

flash: $(BIN_DIR)/$(TARGET).bin
	openocd -f interface/stlink.cfg -f target/stm32f4x.cfg \
		-c "program $(BIN_DIR)/$(TARGET).bin 0x08000000 verify reset exit"


clean:
	rm -rf $(BUILD_DIR)
