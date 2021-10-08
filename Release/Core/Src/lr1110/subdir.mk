################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/lr1110/lr1110_bootloader.c \
../Core/Src/lr1110/lr1110_crypto_engine.c \
../Core/Src/lr1110/lr1110_driver_version.c \
../Core/Src/lr1110/lr1110_gnss.c \
../Core/Src/lr1110/lr1110_hal.c \
../Core/Src/lr1110/lr1110_radio.c \
../Core/Src/lr1110/lr1110_regmem.c \
../Core/Src/lr1110/lr1110_system.c \
../Core/Src/lr1110/lr1110_wifi.c \
../Core/Src/lr1110/system.c \
../Core/Src/lr1110/system_gpio.c \
../Core/Src/lr1110/system_spi.c 

OBJS += \
./Core/Src/lr1110/lr1110_bootloader.o \
./Core/Src/lr1110/lr1110_crypto_engine.o \
./Core/Src/lr1110/lr1110_driver_version.o \
./Core/Src/lr1110/lr1110_gnss.o \
./Core/Src/lr1110/lr1110_hal.o \
./Core/Src/lr1110/lr1110_radio.o \
./Core/Src/lr1110/lr1110_regmem.o \
./Core/Src/lr1110/lr1110_system.o \
./Core/Src/lr1110/lr1110_wifi.o \
./Core/Src/lr1110/system.o \
./Core/Src/lr1110/system_gpio.o \
./Core/Src/lr1110/system_spi.o 

C_DEPS += \
./Core/Src/lr1110/lr1110_bootloader.d \
./Core/Src/lr1110/lr1110_crypto_engine.d \
./Core/Src/lr1110/lr1110_driver_version.d \
./Core/Src/lr1110/lr1110_gnss.d \
./Core/Src/lr1110/lr1110_hal.d \
./Core/Src/lr1110/lr1110_radio.d \
./Core/Src/lr1110/lr1110_regmem.d \
./Core/Src/lr1110/lr1110_system.d \
./Core/Src/lr1110/lr1110_wifi.d \
./Core/Src/lr1110/system.d \
./Core/Src/lr1110/system_gpio.d \
./Core/Src/lr1110/system_spi.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/lr1110/%.o: ../Core/Src/lr1110/%.c Core/Src/lr1110/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -DUSE_HAL_DRIVER -DSTM32WB55xx -c -I../Core/Inc -I../Drivers/STM32WBxx_HAL_Driver/Inc -I../Drivers/STM32WBxx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32WBxx/Include -I../Drivers/CMSIS/Include -I../USB_Device/App -I../USB_Device/Target -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -I"C:/Users/RLK/STM32CubeIDE/workspace_1.7.0/Test/Core/Inc/lr1110" -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

