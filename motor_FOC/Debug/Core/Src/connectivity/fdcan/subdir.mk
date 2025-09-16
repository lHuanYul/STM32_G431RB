################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/connectivity/fdcan/basic.c \
../Core/Src/connectivity/fdcan/callback.c \
../Core/Src/connectivity/fdcan/main.c \
../Core/Src/connectivity/fdcan/pkt_write.c 

OBJS += \
./Core/Src/connectivity/fdcan/basic.o \
./Core/Src/connectivity/fdcan/callback.o \
./Core/Src/connectivity/fdcan/main.o \
./Core/Src/connectivity/fdcan/pkt_write.o 

C_DEPS += \
./Core/Src/connectivity/fdcan/basic.d \
./Core/Src/connectivity/fdcan/callback.d \
./Core/Src/connectivity/fdcan/main.d \
./Core/Src/connectivity/fdcan/pkt_write.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/connectivity/fdcan/%.o Core/Src/connectivity/fdcan/%.su Core/Src/connectivity/fdcan/%.cyclo: ../Core/Src/connectivity/fdcan/%.c Core/Src/connectivity/fdcan/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_NUCLEO_64 -DUSE_HAL_DRIVER -DSTM32G431xx -c -I../Core/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../Drivers/BSP/STM32G4xx_Nucleo -I../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-connectivity-2f-fdcan

clean-Core-2f-Src-2f-connectivity-2f-fdcan:
	-$(RM) ./Core/Src/connectivity/fdcan/basic.cyclo ./Core/Src/connectivity/fdcan/basic.d ./Core/Src/connectivity/fdcan/basic.o ./Core/Src/connectivity/fdcan/basic.su ./Core/Src/connectivity/fdcan/callback.cyclo ./Core/Src/connectivity/fdcan/callback.d ./Core/Src/connectivity/fdcan/callback.o ./Core/Src/connectivity/fdcan/callback.su ./Core/Src/connectivity/fdcan/main.cyclo ./Core/Src/connectivity/fdcan/main.d ./Core/Src/connectivity/fdcan/main.o ./Core/Src/connectivity/fdcan/main.su ./Core/Src/connectivity/fdcan/pkt_write.cyclo ./Core/Src/connectivity/fdcan/pkt_write.d ./Core/Src/connectivity/fdcan/pkt_write.o ./Core/Src/connectivity/fdcan/pkt_write.su

.PHONY: clean-Core-2f-Src-2f-connectivity-2f-fdcan

