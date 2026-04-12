################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
../031HTP/Core/Startup/startup_stm32f103rctx.s 

OBJS += \
./031HTP/Core/Startup/startup_stm32f103rctx.o 

S_DEPS += \
./031HTP/Core/Startup/startup_stm32f103rctx.d 


# Each subdirectory must supply rules for building sources it contributes
031HTP/Core/Startup/%.o: ../031HTP/Core/Startup/%.s 031HTP/Core/Startup/subdir.mk
	arm-none-eabi-gcc -mcpu=cortex-m3 -g3 -DDEBUG -c -x assembler-with-cpp -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@" "$<"

clean: clean-031HTP-2f-Core-2f-Startup

clean-031HTP-2f-Core-2f-Startup:
	-$(RM) ./031HTP/Core/Startup/startup_stm32f103rctx.d ./031HTP/Core/Startup/startup_stm32f103rctx.o

.PHONY: clean-031HTP-2f-Core-2f-Startup

