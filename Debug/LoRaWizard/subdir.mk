################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../LoRaWizard/LoRaConf.c \
../LoRaWizard/LoRaEvents.c \
../LoRaWizard/LoRaMessageHandler.c \
../LoRaWizard/mainLoRa.c 

OBJS += \
./LoRaWizard/LoRaConf.o \
./LoRaWizard/LoRaEvents.o \
./LoRaWizard/LoRaMessageHandler.o \
./LoRaWizard/mainLoRa.o 

C_DEPS += \
./LoRaWizard/LoRaConf.d \
./LoRaWizard/LoRaEvents.d \
./LoRaWizard/LoRaMessageHandler.d \
./LoRaWizard/mainLoRa.d 


# Each subdirectory must supply rules for building sources it contributes
LoRaWizard/%.o LoRaWizard/%.su LoRaWizard/%.cyclo: ../LoRaWizard/%.c LoRaWizard/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DCORE_CM4 -DUSE_HAL_DRIVER -DSTM32WL55xx -c -I../Core/Inc -I../Drivers/STM32WLxx_HAL_Driver/Inc -I../Drivers/STM32WLxx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32WLxx/Include -I../Drivers/CMSIS/Include -I"C:/Users/luizf/STM32CubeIDE/workspace_1.17.0/remote-emergency-system/Drivers/BSP" -I"C:/Users/luizf/STM32CubeIDE/workspace_1.17.0/remote-emergency-system/Drivers/Radio" -I"C:/Users/luizf/STM32CubeIDE/workspace_1.17.0/remote-emergency-system/Utils/conf" -I"C:/Users/luizf/STM32CubeIDE/workspace_1.17.0/remote-emergency-system/Utils/misc" -I"C:/Users/luizf/STM32CubeIDE/workspace_1.17.0/remote-emergency-system/Drivers/BSP/STM32WLxx_Nucleo/_htmresc" -I"C:/Users/luizf/STM32CubeIDE/workspace_1.17.0/remote-emergency-system/Drivers/BSP/STM32WLxx_Nucleo" -I"C:/Users/luizf/STM32CubeIDE/workspace_1.17.0/remote-emergency-system/LoRaWizard" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-LoRaWizard

clean-LoRaWizard:
	-$(RM) ./LoRaWizard/LoRaConf.cyclo ./LoRaWizard/LoRaConf.d ./LoRaWizard/LoRaConf.o ./LoRaWizard/LoRaConf.su ./LoRaWizard/LoRaEvents.cyclo ./LoRaWizard/LoRaEvents.d ./LoRaWizard/LoRaEvents.o ./LoRaWizard/LoRaEvents.su ./LoRaWizard/LoRaMessageHandler.cyclo ./LoRaWizard/LoRaMessageHandler.d ./LoRaWizard/LoRaMessageHandler.o ./LoRaWizard/LoRaMessageHandler.su ./LoRaWizard/mainLoRa.cyclo ./LoRaWizard/mainLoRa.d ./LoRaWizard/mainLoRa.o ./LoRaWizard/mainLoRa.su

.PHONY: clean-LoRaWizard

