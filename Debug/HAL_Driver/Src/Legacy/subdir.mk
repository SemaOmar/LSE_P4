################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../HAL_Driver/Src/Legacy/stm32f4xx_hal_can.c 

OBJS += \
./HAL_Driver/Src/Legacy/stm32f4xx_hal_can.o 

C_DEPS += \
./HAL_Driver/Src/Legacy/stm32f4xx_hal_can.d 


# Each subdirectory must supply rules for building sources it contributes
HAL_Driver/Src/Legacy/%.o: ../HAL_Driver/Src/Legacy/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DSTM32 -DSTM32F4 -DSTM32F411VETx -DDEBUG -DSTM32F411xE -DUSE_HAL_DRIVER -DUSE_RTOS_SYSTICK -I"/home/alumno/workspace/p4/HAL_Driver/Inc/Legacy" -I"/home/alumno/workspace/p4/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F" -I"/home/alumno/workspace/p4/Middlewares/Third_Party/FreeRTOS/Source/include" -I"/home/alumno/workspace/p4/inc" -I"/home/alumno/workspace/p4/CMSIS/device" -I"/home/alumno/workspace/p4/CMSIS/core" -I"/home/alumno/workspace/p4/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS" -I"/home/alumno/workspace/p4/HAL_Driver/Inc" -I"/home/alumno/workspace/p4/BSP/STM32F411E-Discovery/" -I"/home/alumno/workspace/p4/BSP/Components" -O0 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


