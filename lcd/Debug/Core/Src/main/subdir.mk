################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/main/lcd_1inch47.c \
../Core/Src/main/main.c \
../Core/Src/main/spi_json.c 

OBJS += \
./Core/Src/main/lcd_1inch47.o \
./Core/Src/main/main.o \
./Core/Src/main/spi_json.o 

C_DEPS += \
./Core/Src/main/lcd_1inch47.d \
./Core/Src/main/main.d \
./Core/Src/main/spi_json.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/main/%.o Core/Src/main/%.su Core/Src/main/%.cyclo: ../Core/Src/main/%.c Core/Src/main/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_NUCLEO_64 -DUSE_HAL_DRIVER -DSTM32G431xx -c -I../Core/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Drivers/BSP/STM32G4xx_Nucleo -I../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/ST/ARM/DSP/Inc -I"C:/Users/HuanYu/Desktop/Codes/HY_MCU_MOD/STM32/Inc" -I"C:/Users/HuanYu/Desktop/Codes/HY_MCU_MOD/ThirdParty" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-main

clean-Core-2f-Src-2f-main:
	-$(RM) ./Core/Src/main/lcd_1inch47.cyclo ./Core/Src/main/lcd_1inch47.d ./Core/Src/main/lcd_1inch47.o ./Core/Src/main/lcd_1inch47.su ./Core/Src/main/main.cyclo ./Core/Src/main/main.d ./Core/Src/main/main.o ./Core/Src/main/main.su ./Core/Src/main/spi_json.cyclo ./Core/Src/main/spi_json.d ./Core/Src/main/spi_json.o ./Core/Src/main/spi_json.su

.PHONY: clean-Core-2f-Src-2f-main

