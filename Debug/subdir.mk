################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../FIFO_GEN.c \
../TERMINAL.c \
../noyau.c \
../noyau_file.c \
../noyau_test.c \
../priosys.c \
../serialio.c \
../utils.c \
../versatile_interrupt.c \
../versatile_serial.c \
../versatile_timer.c 

S_UPPER_SRCS += \
../crt0.S 

OBJS += \
./FIFO_GEN.o \
./TERMINAL.o \
./crt0.o \
./noyau.o \
./noyau_file.o \
./noyau_test.o \
./priosys.o \
./serialio.o \
./utils.o \
./versatile_interrupt.o \
./versatile_serial.o \
./versatile_timer.o 

C_DEPS += \
./FIFO_GEN.d \
./TERMINAL.d \
./noyau.d \
./noyau_file.d \
./noyau_test.d \
./priosys.d \
./serialio.d \
./utils.d \
./versatile_interrupt.d \
./versatile_serial.d \
./versatile_timer.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	arm-none-eabi-gcc -Igcc -O0 -g3 -Wall -c -fmessage-length=0 -ffreestanding -mcpu=arm926ej-s -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

%.o: ../%.S
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Assembler'
	arm-none-eabi-as -ggdb -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


