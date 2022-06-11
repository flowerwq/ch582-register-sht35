################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/utils/subdir.mk
-include src/modbus/subdir.mk
-include src/subdir.mk
-include StdPeriphDriver/subdir.mk
-include Startup/subdir.mk
-include RVMSIS/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
SECONDARY_FLASH += \
CH582M_register.hex \

SECONDARY_LIST += \
CH582M_register.lst \

SECONDARY_SIZE += \
CH582M_register.siz \


# All Target
all: CH582M_register.elf secondary-outputs

# Tool invocations
CH582M_register.elf: $(OBJS) $(USER_OBJS)
	@	@	riscv-none-embed-gcc -march=rv32imac -mabi=ilp32 -mcmodel=medany -msmall-data-limit=8 -mno-save-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g -T "../Ld/Link.ld" -nostartfiles -Xlinker --gc-sections -L"../" -L"../StdPeriphDriver" -Xlinker --print-memory-usage -Wl,-Map,"CH582M_register.map" --specs=nano.specs -u _printf_float -u _scanf_float --specs=nosys.specs   -o "CH582M_register.elf" $(OBJS) $(USER_OBJS) $(LIBS) -lprintfloat -lprintf
	@	@
CH582M_register.hex: CH582M_register.elf
	@	riscv-none-embed-objcopy -O ihex "CH582M_register.elf"  "CH582M_register.hex"
	@	@
CH582M_register.lst: CH582M_register.elf
	@	riscv-none-embed-objdump --all-headers --demangle --disassemble "CH582M_register.elf" > "CH582M_register.lst"
	@	@
CH582M_register.siz: CH582M_register.elf
	@	riscv-none-embed-size --format=berkeley "CH582M_register.elf"
	@	@
# Other Targets
clean:
	-$(RM) $(ASM_UPPER_DEPS)$(OBJS)$(SECONDARY_FLASH)$(SECONDARY_LIST)$(SECONDARY_SIZE)$(ASM_DEPS)$(S_DEPS)$(S_UPPER_DEPS)$(C_DEPS) CH582M_register.elf
	-@
secondary-outputs: $(SECONDARY_FLASH) $(SECONDARY_LIST) $(SECONDARY_SIZE)

.PHONY: all clean dependents

-include ../makefile.targets
