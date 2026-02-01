################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/Users/HuanYu/Desktop/Codes/HY_MCU_MOD/STM32/Src/HY_MOD/main/fn_state.c \
C:/Users/HuanYu/Desktop/Codes/HY_MCU_MOD/STM32/Src/HY_MOD/main/tim.c \
C:/Users/HuanYu/Desktop/Codes/HY_MCU_MOD/STM32/Src/HY_MOD/main/variable_cal.c 

OBJS += \
./STM32/Src/HY_MOD/main/fn_state.o \
./STM32/Src/HY_MOD/main/tim.o \
./STM32/Src/HY_MOD/main/variable_cal.o 

C_DEPS += \
./STM32/Src/HY_MOD/main/fn_state.d \
./STM32/Src/HY_MOD/main/tim.d \
./STM32/Src/HY_MOD/main/variable_cal.d 


# Each subdirectory must supply rules for building sources it contributes
STM32/Src/HY_MOD/main/fn_state.o: C:/Users/HuanYu/Desktop/Codes/HY_MCU_MOD/STM32/Src/HY_MOD/main/fn_state.c STM32/Src/HY_MOD/main/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_NUCLEO_64 -DUSE_HAL_DRIVER -DSTM32G431xx -c -I../Core/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Drivers/BSP/STM32G4xx_Nucleo -I../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/ST/ARM/DSP/Inc -I"C:/Users/HuanYu/Desktop/Codes/HY_MCU_MOD/STM32/Inc" -I"C:/Users/HuanYu/Desktop/Codes/HY_MCU_MOD/ThirdParty" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
STM32/Src/HY_MOD/main/tim.o: C:/Users/HuanYu/Desktop/Codes/HY_MCU_MOD/STM32/Src/HY_MOD/main/tim.c STM32/Src/HY_MOD/main/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_NUCLEO_64 -DUSE_HAL_DRIVER -DSTM32G431xx -c -I../Core/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Drivers/BSP/STM32G4xx_Nucleo -I../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/ST/ARM/DSP/Inc -I"C:/Users/HuanYu/Desktop/Codes/HY_MCU_MOD/STM32/Inc" -I"C:/Users/HuanYu/Desktop/Codes/HY_MCU_MOD/ThirdParty" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
STM32/Src/HY_MOD/main/variable_cal.o: C:/Users/HuanYu/Desktop/Codes/HY_MCU_MOD/STM32/Src/HY_MOD/main/variable_cal.c STM32/Src/HY_MOD/main/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_NUCLEO_64 -DUSE_HAL_DRIVER -DSTM32G431xx -c -I../Core/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Drivers/BSP/STM32G4xx_Nucleo -I../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/ST/ARM/DSP/Inc -I"C:/Users/HuanYu/Desktop/Codes/HY_MCU_MOD/STM32/Inc" -I"C:/Users/HuanYu/Desktop/Codes/HY_MCU_MOD/ThirdParty" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-STM32-2f-Src-2f-HY_MOD-2f-main

clean-STM32-2f-Src-2f-HY_MOD-2f-main:
	-$(RM) ./STM32/Src/HY_MOD/main/fn_state.cyclo ./STM32/Src/HY_MOD/main/fn_state.d ./STM32/Src/HY_MOD/main/fn_state.o ./STM32/Src/HY_MOD/main/fn_state.su ./STM32/Src/HY_MOD/main/tim.cyclo ./STM32/Src/HY_MOD/main/tim.d ./STM32/Src/HY_MOD/main/tim.o ./STM32/Src/HY_MOD/main/tim.su ./STM32/Src/HY_MOD/main/variable_cal.cyclo ./STM32/Src/HY_MOD/main/variable_cal.d ./STM32/Src/HY_MOD/main/variable_cal.o ./STM32/Src/HY_MOD/main/variable_cal.su

.PHONY: clean-STM32-2f-Src-2f-HY_MOD-2f-main

