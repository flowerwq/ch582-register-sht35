################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/liblightmodbus-3.0/test/codeql/impl.c 

OBJS += \
./src/liblightmodbus-3.0/test/codeql/impl.o 

C_DEPS += \
./src/liblightmodbus-3.0/test/codeql/impl.d 


# Each subdirectory must supply rules for building sources it contributes
src/liblightmodbus-3.0/test/codeql/%.o: ../src/liblightmodbus-3.0/test/codeql/%.c
	@	@	riscv-none-embed-gcc -march=rv32imac -mabi=ilp32 -mcmodel=medany -msmall-data-limit=8 -mno-save-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g -DDEBUG=1 -I"../StdPeriphDriver/inc" -I"../RVMSIS" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@

