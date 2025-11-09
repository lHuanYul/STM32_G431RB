################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/motor/basic.c \
../Core/Src/motor/clarke.c \
../Core/Src/motor/ctrl_deg.c \
../Core/Src/motor/ctrl_foc.c \
../Core/Src/motor/main.c \
../Core/Src/motor/park.c \
../Core/Src/motor/pi.c \
../Core/Src/motor/svgendq.c \
../Core/Src/motor/trigonometric.c 

OBJS += \
./Core/Src/motor/basic.o \
./Core/Src/motor/clarke.o \
./Core/Src/motor/ctrl_deg.o \
./Core/Src/motor/ctrl_foc.o \
./Core/Src/motor/main.o \
./Core/Src/motor/park.o \
./Core/Src/motor/pi.o \
./Core/Src/motor/svgendq.o \
./Core/Src/motor/trigonometric.o 

C_DEPS += \
./Core/Src/motor/basic.d \
./Core/Src/motor/clarke.d \
./Core/Src/motor/ctrl_deg.d \
./Core/Src/motor/ctrl_foc.d \
./Core/Src/motor/main.d \
./Core/Src/motor/park.d \
./Core/Src/motor/pi.d \
./Core/Src/motor/svgendq.d \
./Core/Src/motor/trigonometric.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/motor/%.o Core/Src/motor/%.su Core/Src/motor/%.cyclo: ../Core/Src/motor/%.c Core/Src/motor/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_NUCLEO_64 -DUSE_HAL_DRIVER -DSTM32G431xx -c -I../Core/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../Drivers/BSP/STM32G4xx_Nucleo -I../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Middlewares/ST/ARM/DSP/Inc -I"../Middlewares/ST/ARM/DSP/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-motor

clean-Core-2f-Src-2f-motor:
	-$(RM) ./Core/Src/motor/basic.cyclo ./Core/Src/motor/basic.d ./Core/Src/motor/basic.o ./Core/Src/motor/basic.su ./Core/Src/motor/clarke.cyclo ./Core/Src/motor/clarke.d ./Core/Src/motor/clarke.o ./Core/Src/motor/clarke.su ./Core/Src/motor/ctrl_deg.cyclo ./Core/Src/motor/ctrl_deg.d ./Core/Src/motor/ctrl_deg.o ./Core/Src/motor/ctrl_deg.su ./Core/Src/motor/ctrl_foc.cyclo ./Core/Src/motor/ctrl_foc.d ./Core/Src/motor/ctrl_foc.o ./Core/Src/motor/ctrl_foc.su ./Core/Src/motor/main.cyclo ./Core/Src/motor/main.d ./Core/Src/motor/main.o ./Core/Src/motor/main.su ./Core/Src/motor/park.cyclo ./Core/Src/motor/park.d ./Core/Src/motor/park.o ./Core/Src/motor/park.su ./Core/Src/motor/pi.cyclo ./Core/Src/motor/pi.d ./Core/Src/motor/pi.o ./Core/Src/motor/pi.su ./Core/Src/motor/svgendq.cyclo ./Core/Src/motor/svgendq.d ./Core/Src/motor/svgendq.o ./Core/Src/motor/svgendq.su ./Core/Src/motor/trigonometric.cyclo ./Core/Src/motor/trigonometric.d ./Core/Src/motor/trigonometric.o ./Core/Src/motor/trigonometric.su

.PHONY: clean-Core-2f-Src-2f-motor

