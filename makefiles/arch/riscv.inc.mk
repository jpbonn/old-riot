# Target architecture for the build. Use msp430 if you are unsure.
export TARGET_ARCH ?= riscv64-unknown-elf

# define build specific options
ASFLAGS_CPU   = -march=rv32imac -mabi=ilp32 
CFLAGS_CPU   = $(ASFLAGS_CPU) -mcmodel=medany -std=gnu99
CFLAGS_LINK  = -ffunction-sections -fdata-sections -fno-builtin
CFLAGS_DBG  ?= -gdwarf-2
CFLAGS_OPT  ?= -Os
# export compiler flags
export CFLAGS += $(CFLAGS_CPU) $(CFLAGS_LINK) $(CFLAGS_DBG) $(CFLAGS_OPT)
# export assmebly flags
export ASFLAGS += $(ASFLAGS_CPU) $(CFLAGS_DBG)
# export linker flags
export LINKFLAGS += $(CFLAGS_CPU) $(CFLAGS_DBG) $(CFLAGS_OPT) 
export LINKFLAGS += -Wl,--gc-sections -static -lgcc -nostartfiles -static

# setup linker script
export LINKFLAGS += -L$(RIOTCPU)/$(CPU)/ldscripts
export LINKER_SCRIPT ?= $(CPU_MODEL).ld
export LINKFLAGS += -T$(LINKER_SCRIPT) -Wl,--fatal-warnings

# This CPU implementation is using the new core/CPU interface:
export CFLAGS += -DCOREIF_NG=1