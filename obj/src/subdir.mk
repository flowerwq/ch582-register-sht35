################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/Main.c \
../src/appinfo.c \
../src/configtool.c \
../src/display.c \
../src/liblightmodbus-impl.c \
../src/modbus.c \
../src/oled.c \
../src/sensor.c \
../src/sht35.c \
../src/storage.c \
../src/uid.c \
../src/version.c \
../src/worktime.c 

OBJS += \
./src/Main.o \
./src/appinfo.o \
./src/configtool.o \
./src/display.o \
./src/liblightmodbus-impl.o \
./src/modbus.o \
./src/oled.o \
./src/sensor.o \
./src/sht35.o \
./src/storage.o \
./src/uid.o \
./src/version.o \
./src/worktime.o 

C_DEPS += \
./src/Main.d \
./src/appinfo.d \
./src/configtool.d \
./src/display.d \
./src/liblightmodbus-impl.d \
./src/modbus.d \
./src/oled.d \
./src/sensor.d \
./src/sht35.d \
./src/storage.d \
./src/uid.d \
./src/version.d \
./src/worktime.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@	@	riscv-none-embed-gcc -march=rv32imac -mabi=ilp32 -mcmodel=medany -msmall-data-limit=8 -mno-save-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g -DDEBUG=1 -I"D:\work\CH582M_register\src\include" -I"D:\work\CH582M_register\src\include\modbus" -I"D:\work\CH582M_register\src\include\utils" -I"../StdPeriphDriver/inc" -I"D:\work\CH582M_register\src\liblightmodbus-3.0\include" -I"../RVMSIS" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@

