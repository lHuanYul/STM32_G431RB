################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/Users/HuanYu/Desktop/Codes/HY_MCU_MOD/STM32/Src/HY_MOD/fdcan/basic.c \
C:/Users/HuanYu/Desktop/Codes/HY_MCU_MOD/STM32/Src/HY_MOD/fdcan/callback.c \
C:/Users/HuanYu/Desktop/Codes/HY_MCU_MOD/STM32/Src/HY_MOD/fdcan/main.c \
C:/Users/HuanYu/Desktop/Codes/HY_MCU_MOD/STM32/Src/HY_MOD/fdcan/pkt_read.c \
C:/Users/HuanYu/Desktop/Codes/HY_MCU_MOD/STM32/Src/HY_MOD/fdcan/pkt_write.c \
C:/Users/HuanYu/Desktop/Codes/HY_MCU_MOD/STM32/Src/HY_MOD/fdcan/task.c 

OBJS += \
./STM32/Src/HY_MOD/fdcan/basic.o \
./STM32/Src/HY_MOD/fdcan/callback.o \
./STM32/Src/HY_MOD/fdcan/main.o \
./STM32/Src/HY_MOD/fdcan/pkt_read.o \
./STM32/Src/HY_MOD/fdcan/pkt_write.o \
./STM32/Src/HY_MOD/fdcan/task.o 

C_DEPS += \
./STM32/Src/HY_MOD/fdcan/basic.d \
./STM32/Src/HY_MOD/fdcan/callback.d \
./STM32/Src/HY_MOD/fdcan/main.d \
./STM32/Src/HY_MOD/fdcan/pkt_read.d \
./STM32/Src/HY_MOD/fdcan/pkt_write.d \
./STM32/Src/HY_MOD/fdcan/task.d 


# Each subdirectory must supply rules for building sources it contributes
STM32/Src/HY_MOD/fdcan/basic.o: C:/Users/HuanYu/Desktop/Codes/HY_MCU_MOD/STM32/Src/HY_MOD/fdcan/basic.c STM32/Src/HY_MOD/fdcan/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_NUCLEO_64 -DUSE_HAL_DRIVER -DSTM32G431xx -c -I../Core/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Drivers/BSP/STM32G4xx_Nucleo -I../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/ST/ARM/DSP/Inc -I"C:/Users/HuanYu/Desktop/Codes/HY_MCU_MOD/STM32/Inc" -I"C:/Users/HuanYu/Desktop/Codes/HY_MCU_MOD/ThirdParty" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
STM32/Src/HY_MOD/fdcan/callback.o: C:/Users/HuanYu/Desktop/Codes/HY_MCU_MOD/STM32/Src/HY_MOD/fdcan/callback.c STM32/Src/HY_MOD/fdcan/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_NUCLEO_64 -DUSE_HAL_DRIVER -DSTM32G431xx -c -I../Core/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Drivers/BSP/STM32G4xx_Nucleo -I../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/ST/ARM/DSP/Inc -I"C:/Users/HuanYu/Desktop/Codes/HY_MCU_MOD/STM32/Inc" -I"C:/Users/HuanYu/Desktop/Codes/HY_MCU_MOD/ThirdParty" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
STM32/Src/HY_MOD/fdcan/main.o: C:/Users/HuanYu/Desktop/Codes/HY_MCU_MOD/STM32/Src/HY_MOD/fdcan/main.c STM32/Src/HY_MOD/fdcan/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_NUCLEO_64 -DUSE_HAL_DRIVER -DSTM32G431xx -c -I../Core/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Drivers/BSP/STM32G4xx_Nucleo -I../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/ST/ARM/DSP/Inc -I"C:/Users/HuanYu/Desktop/Codes/HY_MCU_MOD/STM32/Inc" -I"C:/Users/HuanYu/Desktop/Codes/HY_MCU_MOD/ThirdParty" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
STM32/Src/HY_MOD/fdcan/pkt_read.o: C:/Users/HuanYu/Desktop/Codes/HY_MCU_MOD/STM32/Src/HY_MOD/fdcan/pkt_read.c STM32/Src/HY_MOD/fdcan/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_NUCLEO_64 -DUSE_HAL_DRIVER -DSTM32G431xx -c -I../Core/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Drivers/BSP/STM32G4xx_Nucleo -I../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/ST/ARM/DSP/Inc -I"C:/Users/HuanYu/Desktop/Codes/HY_MCU_MOD/STM32/Inc" -I"C:/Users/HuanYu/Desktop/Codes/HY_MCU_MOD/ThirdParty" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
STM32/Src/HY_MOD/fdcan/pkt_write.o: C:/Users/HuanYu/Desktop/Codes/HY_MCU_MOD/STM32/Src/HY_MOD/fdcan/pkt_write.c STM32/Src/HY_MOD/fdcan/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_NUCLEO_64 -DUSE_HAL_DRIVER -DSTM32G431xx -c -I../Core/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Drivers/BSP/STM32G4xx_Nucleo -I../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/ST/ARM/DSP/Inc -I"C:/Users/HuanYu/Desktop/Codes/HY_MCU_MOD/STM32/Inc" -I"C:/Users/HuanYu/Desktop/Codes/HY_MCU_MOD/ThirdParty" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
STM32/Src/HY_MOD/fdcan/task.o: C:/Users/HuanYu/Desktop/Codes/HY_MCU_MOD/STM32/Src/HY_MOD/fdcan/task.c STM32/Src/HY_MOD/fdcan/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_NUCLEO_64 -DUSE_HAL_DRIVER -DSTM32G431xx -c -I../Core/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Drivers/BSP/STM32G4xx_Nucleo -I../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/ST/ARM/DSP/Inc -I"C:/Users/HuanYu/Desktop/Codes/HY_MCU_MOD/STM32/Inc" -I"C:/Users/HuanYu/Desktop/Codes/HY_MCU_MOD/ThirdParty" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-STM32-2f-Src-2f-HY_MOD-2f-fdcan

clean-STM32-2f-Src-2f-HY_MOD-2f-fdcan:
	-$(RM) ./STM32/Src/HY_MOD/fdcan/basic.cyclo ./STM32/Src/HY_MOD/fdcan/basic.d ./STM32/Src/HY_MOD/fdcan/basic.o ./STM32/Src/HY_MOD/fdcan/basic.su ./STM32/Src/HY_MOD/fdcan/callback.cyclo ./STM32/Src/HY_MOD/fdcan/callback.d ./STM32/Src/HY_MOD/fdcan/callback.o ./STM32/Src/HY_MOD/fdcan/callback.su ./STM32/Src/HY_MOD/fdcan/main.cyclo ./STM32/Src/HY_MOD/fdcan/main.d ./STM32/Src/HY_MOD/fdcan/main.o ./STM32/Src/HY_MOD/fdcan/main.su ./STM32/Src/HY_MOD/fdcan/pkt_read.cyclo ./STM32/Src/HY_MOD/fdcan/pkt_read.d ./STM32/Src/HY_MOD/fdcan/pkt_read.o ./STM32/Src/HY_MOD/fdcan/pkt_read.su ./STM32/Src/HY_MOD/fdcan/pkt_write.cyclo ./STM32/Src/HY_MOD/fdcan/pkt_write.d ./STM32/Src/HY_MOD/fdcan/pkt_write.o ./STM32/Src/HY_MOD/fdcan/pkt_write.su ./STM32/Src/HY_MOD/fdcan/task.cyclo ./STM32/Src/HY_MOD/fdcan/task.d ./STM32/Src/HY_MOD/fdcan/task.o ./STM32/Src/HY_MOD/fdcan/task.su

.PHONY: clean-STM32-2f-Src-2f-HY_MOD-2f-fdcan

