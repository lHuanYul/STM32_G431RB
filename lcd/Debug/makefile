################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include ThirdParty/CoreJSON/subdir.mk
-include STM32/Src/HY_MOD/voice_ysm3/subdir.mk
-include STM32/Src/HY_MOD/vehicle/subdir.mk
-include STM32/Src/HY_MOD/us_sensor/subdir.mk
-include STM32/Src/HY_MOD/spi_json/subdir.mk
-include STM32/Src/HY_MOD/spi/subdir.mk
-include STM32/Src/HY_MOD/sdmmc/subdir.mk
-include STM32/Src/HY_MOD/robotic_arm/subdir.mk
-include STM32/Src/HY_MOD/rfid/subdir.mk
-include STM32/Src/HY_MOD/packet/subdir.mk
-include STM32/Src/HY_MOD/main/subdir.mk
-include STM32/Src/HY_MOD/light/subdir.mk
-include STM32/Src/HY_MOD/lcd_1inch47/subdir.mk
-include STM32/Src/HY_MOD/fdcan/subdir.mk
-include STM32/Src/HY_MOD/dht/subdir.mk
-include STM32/Src/HY_MOD/adc_hall/subdir.mk
-include STM32/Src/HY_MOD/adc/subdir.mk
-include Middlewares/Third_Party/FreeRTOS/Source/portable/MemMang/subdir.mk
-include Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F/subdir.mk
-include Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2/subdir.mk
-include Middlewares/Third_Party/FreeRTOS/Source/subdir.mk
-include Drivers/STM32G4xx_HAL_Driver/Src/subdir.mk
-include Drivers/BSP/STM32G4xx_Nucleo/subdir.mk
-include Core/Startup/subdir.mk
-include Core/Src/main/subdir.mk
-include Core/Src/subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

OPTIONAL_TOOL_DEPS := \
$(wildcard ../makefile.defs) \
$(wildcard ../makefile.init) \
$(wildcard ../makefile.targets) \


BUILD_ARTIFACT_NAME := lcd
BUILD_ARTIFACT_EXTENSION := elf
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
lcd.elf \

MAP_FILES += \
lcd.map \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
lcd.list \


# All Target
all: main-build

# Main-build Target
main-build: lcd.elf secondary-outputs

# Tool invocations
lcd.elf lcd.map: $(OBJS) $(USER_OBJS) C:\Users\HuanYu\Desktop\Codes\STM32_G431RB\lcd\STM32G431RBTX_FLASH.ld makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-gcc -o "lcd.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m4 -T"C:\Users\HuanYu\Desktop\Codes\STM32_G431RB\lcd\STM32G431RBTX_FLASH.ld" --specs=nosys.specs -Wl,-Map="lcd.map" -Wl,--gc-sections -static -L../Middlewares/ST/ARM/DSP/Lib --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

lcd.list: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "lcd.list"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) default.size.stdout lcd.elf lcd.list lcd.map
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST)

fail-specified-linker-script-missing:
	@echo 'Error: Cannot find the specified linker script. Check the linker settings in the build configuration.'
	@exit 2

warn-no-linker-script-specified:
	@echo 'Warning: No linker script specified. Check the linker settings in the build configuration.'

.PHONY: all clean dependents main-build fail-specified-linker-script-missing warn-no-linker-script-specified

-include ../makefile.targets
