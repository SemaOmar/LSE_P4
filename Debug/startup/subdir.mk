################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
../startup/startup_stm32.s 

OBJS += \
./startup/startup_stm32.o 


# Each subdirectory must supply rules for building sources it contributes
startup/%.o: ../startup/%.s
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Assembler'
	@echo $(PWD)
	arm-none-eabi-as -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -I"/home/alumno/workspace/p4/HAL_Driver/Inc/Legacy" -I"/home/alumno/workspace/p4/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F" -I"/home/alumno/workspace/p4/Middlewares/Third_Party/FreeRTOS/Source/include" -I"/home/alumno/workspace/p4/inc" -I"/home/alumno/workspace/p4/CMSIS/device" -I"/home/alumno/workspace/p4/CMSIS/core" -I"/home/alumno/workspace/p4/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS" -I"/home/alumno/workspace/p4/HAL_Driver/Inc" -I"/home/alumno/workspace/p4/BSP/STM32F411E-Discovery/" -I"/home/alumno/workspace/p4/BSP/Components" -g -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


