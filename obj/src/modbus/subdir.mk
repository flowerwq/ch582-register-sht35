################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/modbus/coils.c \
../src/modbus/discrete_input.c \
../src/modbus/iregs.c \
../src/modbus/regs.c 

OBJS += \
./src/modbus/coils.o \
./src/modbus/discrete_input.o \
./src/modbus/iregs.o \
./src/modbus/regs.o 

C_DEPS += \
./src/modbus/coils.d \
./src/modbus/discrete_input.d \
./src/modbus/iregs.d \
./src/modbus/regs.d 


# Each subdirectory must supply rules for building sources it contributes
src/modbus/%.o: ../src/modbus/%.c
	@	@	riscv-none-embed-gcc -march=rv32imac -mabi=ilp32 -mcmodel=medany -msmall-data-limit=8 -mno-save-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g -DDEBUG=1 -I"D:\work\CH582M_register\src\include" -I"D:\work\CH582M_register\src\include\modbus" -I"D:\work\CH582M_register\src\include\utils" -I"../StdPeriphDriver/inc" -I"D:\work\CH582M_register\src\liblightmodbus-3.0\include" -I"../RVMSIS" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@

