################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/LoRaWizard/LoRaConf.c \
../Drivers/LoRaWizard/LoRaEvents.c \
../Drivers/LoRaWizard/LoRaMessageHandler.c 

OBJS += \
./Drivers/LoRaWizard/LoRaConf.o \
./Drivers/LoRaWizard/LoRaEvents.o \
./Drivers/LoRaWizard/LoRaMessageHandler.o 

C_DEPS += \
./Drivers/LoRaWizard/LoRaConf.d \
./Drivers/LoRaWizard/LoRaEvents.d \
./Drivers/LoRaWizard/LoRaMessageHandler.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/LoRaWizard/%.o Drivers/LoRaWizard/%.su Drivers/LoRaWizard/%.cyclo: ../Drivers/LoRaWizard/%.c Drivers/LoRaWizard/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DCORE_CM4 -DUSE_HAL_DRIVER -DSTM32WL55xx -c -I../Core/Inc -I../Drivers/STM32WLxx_HAL_Driver/Inc -I../Drivers/STM32WLxx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32WLxx/Include -I../Drivers/CMSIS/Include -I"C:/Users/luizf/STM32CubeIDE/workspace_1.17.0/remote-emergency-system/Drivers/BSP" -I"C:/Users/luizf/STM32CubeIDE/workspace_1.17.0/remote-emergency-system/Drivers/Radio" -I"C:/Users/luizf/STM32CubeIDE/workspace_1.17.0/remote-emergency-system/Utils/conf" -I"C:/Users/luizf/STM32CubeIDE/workspace_1.17.0/remote-emergency-system/Utils/misc" -I"C:/Users/luizf/STM32CubeIDE/workspace_1.17.0/remote-emergency-system/Drivers/BSP/STM32WLxx_Nucleo/_htmresc" -I"C:/Users/luizf/STM32CubeIDE/workspace_1.17.0/remote-emergency-system/Drivers/BSP/STM32WLxx_Nucleo" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Drivers-2f-LoRaWizard

clean-Drivers-2f-LoRaWizard:
	-$(RM) ./Drivers/LoRaWizard/LoRaConf.cyclo ./Drivers/LoRaWizard/LoRaConf.d ./Drivers/LoRaWizard/LoRaConf.o ./Drivers/LoRaWizard/LoRaConf.su ./Drivers/LoRaWizard/LoRaEvents.cyclo ./Drivers/LoRaWizard/LoRaEvents.d ./Drivers/LoRaWizard/LoRaEvents.o ./Drivers/LoRaWizard/LoRaEvents.su ./Drivers/LoRaWizard/LoRaMessageHandler.cyclo ./Drivers/LoRaWizard/LoRaMessageHandler.d ./Drivers/LoRaWizard/LoRaMessageHandler.o ./Drivers/LoRaWizard/LoRaMessageHandler.su

.PHONY: clean-Drivers-2f-LoRaWizard

