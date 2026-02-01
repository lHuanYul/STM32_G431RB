################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/Users/HuanYu/Desktop/Codes/HY_MCU_MOD/STM32/Src/HY_MOD/spi_json/basic.c \
C:/Users/HuanYu/Desktop/Codes/HY_MCU_MOD/STM32/Src/HY_MOD/spi_json/main.c \
C:/Users/HuanYu/Desktop/Codes/HY_MCU_MOD/STM32/Src/HY_MOD/spi_json/task.c 

OBJS += \
./STM32/Src/HY_MOD/spi_json/basic.o \
./STM32/Src/HY_MOD/spi_json/main.o \
./STM32/Src/HY_MOD/spi_json/task.o 

C_DEPS += \
./STM32/Src/HY_MOD/spi_json/basic.d \
./STM32/Src/HY_MOD/spi_json/main.d \
./STM32/Src/HY_MOD/spi_json/task.d 


# Each subdirectory must supply rules for building sources it contributes
STM32/Src/HY_MOD/spi_json/basic.o: C:/Users/HuanYu/Desktop/Codes/HY_MCU_MOD/STM32/Src/HY_MOD/spi_json/basic.c STM32/Src/HY_MOD/spi_json/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_NUCLEO_64 -DUSE_HAL_DRIVER -DSTM32G431xx -c -I../Core/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Drivers/BSP/STM32G4xx_Nucleo -I../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/ST/ARM/DSP/Inc -I"C:/Users/HuanYu/Desktop/Codes/HY_MCU_MOD/STM32/Inc" -I"C:/Users/HuanYu/Desktop/Codes/HY_MCU_MOD/ThirdParty" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
STM32/Src/HY_MOD/spi_json/main.o: C:/Users/HuanYu/Desktop/Codes/HY_MCU_MOD/STM32/Src/HY_MOD/spi_json/main.c STM32/Src/HY_MOD/spi_json/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_NUCLEO_64 -DUSE_HAL_DRIVER -DSTM32G431xx -c -I../Core/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Drivers/BSP/STM32G4xx_Nucleo -I../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/ST/ARM/DSP/Inc -I"C:/Users/HuanYu/Desktop/Codes/HY_MCU_MOD/STM32/Inc" -I"C:/Users/HuanYu/Desktop/Codes/HY_MCU_MOD/ThirdParty" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
STM32/Src/HY_MOD/spi_json/task.o: C:/Users/HuanYu/Desktop/Codes/HY_MCU_MOD/STM32/Src/HY_MOD/spi_json/task.c STM32/Src/HY_MOD/spi_json/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_NUCLEO_64 -DUSE_HAL_DRIVER -DSTM32G431xx -c -I../Core/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Drivers/BSP/STM32G4xx_Nucleo -I../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/ST/ARM/DSP/Inc -I"C:/Users/HuanYu/Desktop/Codes/HY_MCU_MOD/STM32/Inc" -I"C:/Users/HuanYu/Desktop/Codes/HY_MCU_MOD/ThirdParty" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-STM32-2f-Src-2f-HY_MOD-2f-spi_json

clean-STM32-2f-Src-2f-HY_MOD-2f-spi_json:
	-$(RM) ./STM32/Src/HY_MOD/spi_json/basic.cyclo ./STM32/Src/HY_MOD/spi_json/basic.d ./STM32/Src/HY_MOD/spi_json/basic.o ./STM32/Src/HY_MOD/spi_json/basic.su ./STM32/Src/HY_MOD/spi_json/main.cyclo ./STM32/Src/HY_MOD/spi_json/main.d ./STM32/Src/HY_MOD/spi_json/main.o ./STM32/Src/HY_MOD/spi_json/main.su ./STM32/Src/HY_MOD/spi_json/task.cyclo ./STM32/Src/HY_MOD/spi_json/task.d ./STM32/Src/HY_MOD/spi_json/task.o ./STM32/Src/HY_MOD/spi_json/task.su

.PHONY: clean-STM32-2f-Src-2f-HY_MOD-2f-spi_json

