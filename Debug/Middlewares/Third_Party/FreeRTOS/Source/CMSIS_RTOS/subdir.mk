################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS/cmsis_os.c 

OBJS += \
./Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS/cmsis_os.o 

C_DEPS += \
./Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS/cmsis_os.d 


# Each subdirectory must supply rules for building sources it contributes
Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS/%.o: ../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DSTM32 -DSTM32F4 -DSTM32F411VETx -DDEBUG -DSTM32F411xE -DUSE_HAL_DRIVER -DUSE_RTOS_SYSTICK -I"/home/alumno/workspace/p4/HAL_Driver/Inc/Legacy" -I"/home/alumno/workspace/p4/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F" -I"/home/alumno/workspace/p4/Middlewares/Third_Party/FreeRTOS/Source/include" -I"/home/alumno/workspace/p4/inc" -I"/home/alumno/workspace/p4/CMSIS/device" -I"/home/alumno/workspace/p4/CMSIS/core" -I"/home/alumno/workspace/p4/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS" -I"/home/alumno/workspace/p4/HAL_Driver/Inc" -I"/home/alumno/workspace/p4/BSP/STM32F411E-Discovery/" -I"/home/alumno/workspace/p4/BSP/Components" -O0 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


