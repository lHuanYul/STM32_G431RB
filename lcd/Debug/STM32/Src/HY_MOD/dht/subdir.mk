################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/Users/HuanYu/Desktop/Codes/HY_MCU_MOD/STM32/Src/HY_MOD/dht/basic.c \
C:/Users/HuanYu/Desktop/Codes/HY_MCU_MOD/STM32/Src/HY_MOD/dht/callback.c \
C:/Users/HuanYu/Desktop/Codes/HY_MCU_MOD/STM32/Src/HY_MOD/dht/main.c \
C:/Users/HuanYu/Desktop/Codes/HY_MCU_MOD/STM32/Src/HY_MOD/dht/task.c 

OBJS += \
./STM32/Src/HY_MOD/dht/basic.o \
./STM32/Src/HY_MOD/dht/callback.o \
./STM32/Src/HY_MOD/dht/main.o \
./STM32/Src/HY_MOD/dht/task.o 

C_DEPS += \
./STM32/Src/HY_MOD/dht/basic.d \
./STM32/Src/HY_MOD/dht/callback.d \
./STM32/Src/HY_MOD/dht/main.d \
./STM32/Src/HY_MOD/dht/task.d 


# Each subdirectory must supply rules for building sources it contributes
STM32/Src/HY_MOD/dht/basic.o: C:/Users/HuanYu/Desktop/Codes/HY_MCU_MOD/STM32/Src/HY_MOD/dht/basic.c STM32/Src/HY_MOD/dht/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_NUCLEO_64 -DUSE_HAL_DRIVER -DSTM32G431xx -c -I../Core/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Drivers/BSP/STM32G4xx_Nucleo -I../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/ST/ARM/DSP/Inc -I"C:/Users/HuanYu/Desktop/Codes/HY_MCU_MOD/STM32/Inc" -I"C:/Users/HuanYu/Desktop/Codes/HY_MCU_MOD/ThirdParty" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
STM32/Src/HY_MOD/dht/callback.o: C:/Users/HuanYu/Desktop/Codes/HY_MCU_MOD/STM32/Src/HY_MOD/dht/callback.c STM32/Src/HY_MOD/dht/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_NUCLEO_64 -DUSE_HAL_DRIVER -DSTM32G431xx -c -I../Core/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Drivers/BSP/STM32G4xx_Nucleo -I../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/ST/ARM/DSP/Inc -I"C:/Users/HuanYu/Desktop/Codes/HY_MCU_MOD/STM32/Inc" -I"C:/Users/HuanYu/Desktop/Codes/HY_MCU_MOD/ThirdParty" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
STM32/Src/HY_MOD/dht/main.o: C:/Users/HuanYu/Desktop/Codes/HY_MCU_MOD/STM32/Src/HY_MOD/dht/main.c STM32/Src/HY_MOD/dht/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_NUCLEO_64 -DUSE_HAL_DRIVER -DSTM32G431xx -c -I../Core/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Drivers/BSP/STM32G4xx_Nucleo -I../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/ST/ARM/DSP/Inc -I"C:/Users/HuanYu/Desktop/Codes/HY_MCU_MOD/STM32/Inc" -I"C:/Users/HuanYu/Desktop/Codes/HY_MCU_MOD/ThirdParty" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
STM32/Src/HY_MOD/dht/task.o: C:/Users/HuanYu/Desktop/Codes/HY_MCU_MOD/STM32/Src/HY_MOD/dht/task.c STM32/Src/HY_MOD/dht/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_NUCLEO_64 -DUSE_HAL_DRIVER -DSTM32G431xx -c -I../Core/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Drivers/BSP/STM32G4xx_Nucleo -I../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/ST/ARM/DSP/Inc -I"C:/Users/HuanYu/Desktop/Codes/HY_MCU_MOD/STM32/Inc" -I"C:/Users/HuanYu/Desktop/Codes/HY_MCU_MOD/ThirdParty" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-STM32-2f-Src-2f-HY_MOD-2f-dht

clean-STM32-2f-Src-2f-HY_MOD-2f-dht:
	-$(RM) ./STM32/Src/HY_MOD/dht/basic.cyclo ./STM32/Src/HY_MOD/dht/basic.d ./STM32/Src/HY_MOD/dht/basic.o ./STM32/Src/HY_MOD/dht/basic.su ./STM32/Src/HY_MOD/dht/callback.cyclo ./STM32/Src/HY_MOD/dht/callback.d ./STM32/Src/HY_MOD/dht/callback.o ./STM32/Src/HY_MOD/dht/callback.su ./STM32/Src/HY_MOD/dht/main.cyclo ./STM32/Src/HY_MOD/dht/main.d ./STM32/Src/HY_MOD/dht/main.o ./STM32/Src/HY_MOD/dht/main.su ./STM32/Src/HY_MOD/dht/task.cyclo ./STM32/Src/HY_MOD/dht/task.d ./STM32/Src/HY_MOD/dht/task.o ./STM32/Src/HY_MOD/dht/task.su

.PHONY: clean-STM32-2f-Src-2f-HY_MOD-2f-dht

