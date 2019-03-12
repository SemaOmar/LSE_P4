################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/main.c \
../src/stm32f4xx_it.c \
../src/syscalls.c \
../src/system_stm32f4xx.c 

OBJS += \
./src/main.o \
./src/stm32f4xx_it.o \
./src/syscalls.o \
./src/system_stm32f4xx.o 

C_DEPS += \
./src/main.d \
./src/stm32f4xx_it.d \
./src/syscalls.d \
./src/system_stm32f4xx.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DSTM32 -DSTM32F4 -DSTM32F411VETx -DDEBUG -DSTM32F411xE -DUSE_HAL_DRIVER -DUSE_RTOS_SYSTICK -I"/home/alumno/workspace/p4/HAL_Driver/Inc/Legacy" -I"/home/alumno/workspace/p4/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F" -I"/home/alumno/workspace/p4/Middlewares/Third_Party/FreeRTOS/Source/include" -I"/home/alumno/workspace/p4/inc" -I"/home/alumno/workspace/p4/CMSIS/device" -I"/home/alumno/workspace/p4/CMSIS/core" -I"/home/alumno/workspace/p4/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS" -I"/home/alumno/workspace/p4/HAL_Driver/Inc" -I"/home/alumno/workspace/p4/BSP/STM32F411E-Discovery/" -I"/home/alumno/workspace/p4/BSP/Components" -O0 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


