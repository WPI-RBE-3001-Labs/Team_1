################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Lab\ 1/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
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
LSS += \
RBE3001_Group.lss \

FLASH_IMAGE += \
RBE3001_Group.hex \

EEPROM_IMAGE += \
RBE3001_Group.eep \

SIZEDUMMY += \
sizedummy \


# All Target
all: RBE3001_Group.elf secondary-outputs

# Tool invocations
RBE3001_Group.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: AVR C Linker'
	avr-gcc -Wl,-Map,RBE3001_Group.map -Wl,-u,vfprintf -lprintf_flt -lm -L"C:\Users\djcupo\git\rbelib\RBELib\Release" -mmcu=atmega644p -o "RBE3001_Group.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

RBE3001_Group.lss: RBE3001_Group.elf
	@echo 'Invoking: AVR Create Extended Listing'
	-avr-objdump -h -S RBE3001_Group.elf  >"RBE3001_Group.lss"
	@echo 'Finished building: $@'
	@echo ' '

RBE3001_Group.hex: RBE3001_Group.elf
	@echo 'Create Flash image (ihex format)'
	-avr-objcopy -R .eeprom -R .fuse -R .lock -R .signature -O ihex RBE3001_Group.elf  "RBE3001_Group.hex"
	@echo 'Finished building: $@'
	@echo ' '

RBE3001_Group.eep: RBE3001_Group.elf
	@echo 'Create eeprom image (ihex format)'
	-avr-objcopy -j .eeprom --no-change-warnings --change-section-lma .eeprom=0 -O ihex RBE3001_Group.elf  "RBE3001_Group.eep"
	@echo 'Finished building: $@'
	@echo ' '

sizedummy: RBE3001_Group.elf
	@echo 'Invoking: Print Size'
	-avr-size --format=avr --mcu=atmega644p RBE3001_Group.elf
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(FLASH_IMAGE)$(ELFS)$(OBJS)$(ASM_DEPS)$(EEPROM_IMAGE)$(S_DEPS)$(SIZEDUMMY)$(S_UPPER_DEPS)$(LSS)$(C_DEPS) RBE3001_Group.elf
	-@echo ' '

secondary-outputs: $(LSS) $(FLASH_IMAGE) $(EEPROM_IMAGE) $(SIZEDUMMY)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
