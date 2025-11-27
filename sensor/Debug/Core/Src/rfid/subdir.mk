################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/rfid/MFRC522_STM32.c \
../Core/Src/rfid/main.c \
../Core/Src/rfid/trcv_buffer.c 

OBJS += \
./Core/Src/rfid/MFRC522_STM32.o \
./Core/Src/rfid/main.o \
./Core/Src/rfid/trcv_buffer.o 

C_DEPS += \
./Core/Src/rfid/MFRC522_STM32.d \
./Core/Src/rfid/main.d \
./Core/Src/rfid/trcv_buffer.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/rfid/%.o Core/Src/rfid/%.su Core/Src/rfid/%.cyclo: ../Core/Src/rfid/%.c Core/Src/rfid/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_NUCLEO_64 -DUSE_HAL_DRIVER -DSTM32G431xx -c -I../Core/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Drivers/BSP/STM32G4xx_Nucleo -I../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/ST/ARM/DSP/Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-rfid

clean-Core-2f-Src-2f-rfid:
	-$(RM) ./Core/Src/rfid/MFRC522_STM32.cyclo ./Core/Src/rfid/MFRC522_STM32.d ./Core/Src/rfid/MFRC522_STM32.o ./Core/Src/rfid/MFRC522_STM32.su ./Core/Src/rfid/main.cyclo ./Core/Src/rfid/main.d ./Core/Src/rfid/main.o ./Core/Src/rfid/main.su ./Core/Src/rfid/trcv_buffer.cyclo ./Core/Src/rfid/trcv_buffer.d ./Core/Src/rfid/trcv_buffer.o ./Core/Src/rfid/trcv_buffer.su

.PHONY: clean-Core-2f-Src-2f-rfid

