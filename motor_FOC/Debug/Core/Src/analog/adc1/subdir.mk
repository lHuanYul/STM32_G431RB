################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/analog/adc1/main.c 

OBJS += \
./Core/Src/analog/adc1/main.o 

C_DEPS += \
./Core/Src/analog/adc1/main.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/analog/adc1/%.o Core/Src/analog/adc1/%.su Core/Src/analog/adc1/%.cyclo: ../Core/Src/analog/adc1/%.c Core/Src/analog/adc1/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_NUCLEO_64 -DUSE_HAL_DRIVER -DSTM32G431xx -c -I../Core/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../Drivers/BSP/STM32G4xx_Nucleo -I../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-analog-2f-adc1

clean-Core-2f-Src-2f-analog-2f-adc1:
	-$(RM) ./Core/Src/analog/adc1/main.cyclo ./Core/Src/analog/adc1/main.d ./Core/Src/analog/adc1/main.o ./Core/Src/analog/adc1/main.su

.PHONY: clean-Core-2f-Src-2f-analog-2f-adc1

