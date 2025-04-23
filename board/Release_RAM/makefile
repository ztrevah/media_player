################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/subdir.mk
-include SDK/S32K144_SDK_4.0.1/platform/drivers/src/pins/subdir.mk
-include SDK/S32K144_SDK_4.0.1/platform/drivers/src/interrupt/subdir.mk
-include SDK/S32K144_SDK_4.0.1/platform/drivers/src/clock/S32K1xx/subdir.mk
-include SDK/S32K144_SDK_4.0.1/platform/devices/S32K144/startup/subdir.mk
-include SDK/S32K144_SDK_4.0.1/platform/devices/subdir.mk
-include Project_Settings/Startup_Code/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
SECONDARY_SIZE += \
Media_Player_UART.siz \


# All Target
all: Media_Player_UART.elf secondary-outputs

# Tool invocations
Media_Player_UART.elf: $(OBJS) E:/workspaceS32DS.3.5/Media_Player_UART/Project_Settings/Linker_Files/S32K144_64_ram.ld $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: Standard S32DS C Linker'
	arm-none-eabi-gcc -o "Media_Player_UART.elf" "@Media_Player_UART.args"  $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

Media_Player_UART.siz: Media_Player_UART.elf
	@echo 'Invoking: Standard S32DS Print Size'
	arm-none-eabi-size --format=berkeley Media_Player_UART.elf
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) ./*
	-@echo ' '

secondary-outputs: $(SECONDARY_SIZE)

.PHONY: all clean dependents

-include ../makefile.targets
