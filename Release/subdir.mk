################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Final\ Lab.c \
../Lab2A.c \
../Lab3.c \
../main.c 

OBJS += \
./Final\ Lab.o \
./Lab2A.o \
./Lab3.o \
./main.o 

C_DEPS += \
./Final\ Lab.d \
./Lab2A.d \
./Lab3.d \
./main.d 


# Each subdirectory must supply rules for building sources it contributes
Final\ Lab.o: ../Final\ Lab.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -I"C:\Users\Mike\Documents\Robotics 3001\Workspace\RBELib\include" -Wall -Os -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega644p -DF_CPU=18432000UL -MMD -MP -MF"Final Lab.d" -MT"Final\ Lab.d" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -I"C:\Users\Mike\Documents\Robotics 3001\Workspace\RBELib\include" -Wall -Os -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega644p -DF_CPU=18432000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


