################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/ADC.c \
../src/GPIO.c \
../src/LPUART.c \
../src/PWM.c \
../src/main.c \
../src/sys_clock.c \
../src/timer_interrupt.c 

OBJS += \
./src/ADC.o \
./src/GPIO.o \
./src/LPUART.o \
./src/PWM.o \
./src/main.o \
./src/sys_clock.o \
./src/timer_interrupt.o 

C_DEPS += \
./src/ADC.d \
./src/GPIO.d \
./src/LPUART.d \
./src/PWM.d \
./src/main.d \
./src/sys_clock.d \
./src/timer_interrupt.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	arm-none-eabi-gcc "@src/ADC.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


