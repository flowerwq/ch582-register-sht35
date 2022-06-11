################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/utils/crc.c \
../src/utils/crc16.c \
../src/utils/log.c \
../src/utils/md5.c 

OBJS += \
./src/utils/crc.o \
./src/utils/crc16.o \
./src/utils/log.o \
./src/utils/md5.o 

C_DEPS += \
./src/utils/crc.d \
./src/utils/crc16.d \
./src/utils/log.d \
./src/utils/md5.d 


# Each subdirectory must supply rules for building sources it contributes
src/utils/%.o: ../src/utils/%.c
	@	@	riscv-none-embed-gcc -march=rv32imac -mabi=ilp32 -mcmodel=medany -msmall-data-limit=8 -mno-save-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g -DDEBUG=1 -I"D:\work\CH582M_register\src\include" -I"D:\work\CH582M_register\src\include\modbus" -I"D:\work\CH582M_register\src\include\utils" -I"../StdPeriphDriver/inc" -I"D:\work\CH582M_register\src\liblightmodbus-3.0\include" -I"../RVMSIS" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@

