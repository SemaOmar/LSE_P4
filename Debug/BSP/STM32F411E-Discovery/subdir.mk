################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../BSP/STM32F411E-Discovery/stm32f411e_discovery.c \
../BSP/STM32F411E-Discovery/stm32f411e_discovery_accelerometer.c \
../BSP/STM32F411E-Discovery/stm32f411e_discovery_gyroscope.c 

OBJS += \
./BSP/STM32F411E-Discovery/stm32f411e_discovery.o \
./BSP/STM32F411E-Discovery/stm32f411e_discovery_accelerometer.o \
./BSP/STM32F411E-Discovery/stm32f411e_discovery_gyroscope.o 

C_DEPS += \
./BSP/STM32F411E-Discovery/stm32f411e_discovery.d \
./BSP/STM32F411E-Discovery/stm32f411e_discovery_accelerometer.d \
./BSP/STM32F411E-Discovery/stm32f411e_discovery_gyroscope.d 


# Each subdirectory must supply rules for building sources it contributes
BSP/STM32F411E-Discovery/%.o: ../BSP/STM32F411E-Discovery/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DSTM32 -DSTM32F4 -DSTM32F411VETx -DDEBUG -DSTM32F411xE -DUSE_HAL_DRIVER -DUSE_RTOS_SYSTICK -I"/home/alumno/workspace/p4/HAL_Driver/Inc/Legacy" -I"/home/alumno/workspace/p4/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F" -I"/home/alumno/workspace/p4/Middlewares/Third_Party/FreeRTOS/Source/include" -I"/home/alumno/workspace/p4/inc" -I"/home/alumno/workspace/p4/CMSIS/device" -I"/home/alumno/workspace/p4/CMSIS/core" -I"/home/alumno/workspace/p4/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS" -I"/home/alumno/workspace/p4/HAL_Driver/Inc" -I"/home/alumno/workspace/p4/BSP/STM32F411E-Discovery/" -I"/home/alumno/workspace/p4/BSP/Components" -O0 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


