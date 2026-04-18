################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

CG_TOOL_ROOT := C:/ti/ti_cgt_arm_llvm_4.0.2.LTS

GEN_OPTS__FLAG := @"syscfg/device.opt" 
GEN_CMDS__FLAG := -Wl,-l"syscfg/device_linker.cmd" 

ORDERED_OBJS += \
"./empty.o" \
"./syscfg/ti_msp_dl_config.o" \
"./startup_mspm0g350x_ticlang.o" \
"./BSP/Buzzer/buzzer.o" \
"./BSP/Eight_Tracking/ADC.o" \
"./BSP/Eight_Tracking/IIC.o" \
"./BSP/Eight_Tracking/No_Mcu_Ganv_Grayscale_Sensor.o" \
"./BSP/Eight_Tracking/app_irtracking.o" \
"./BSP/Key/key.o" \
"./BSP/LED/led.o" \
"./BSP/Motor/app_motor.o" \
"./BSP/Motor/app_motor_usart.o" \
"./BSP/Motor/bsp_motor_usart.o" \
"./BSP/Questions/questions.o" \
"./BSP/Servo/servo.o" \
"./BSP/Servo/servo_usart.o" \
"./BSP/TB6612/tb6612.o" \
"./BSP/Task/task.o" \
"./BSP/Timer/timer.o" \
"./BSP/delay.o" \
"./BSP/usart.o" \
$(GEN_CMDS__FLAG) \
-Wl,-ldevice.cmd.genlibs \
-Wl,-llibc.a \

-include ../makefile.init

RM := DEL /F
RMDIR := RMDIR /S/Q

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir_vars.mk
-include BSP/Buzzer/subdir_vars.mk
-include BSP/Eight_Tracking/subdir_vars.mk
-include BSP/Key/subdir_vars.mk
-include BSP/LED/subdir_vars.mk
-include BSP/Motor/subdir_vars.mk
-include BSP/Questions/subdir_vars.mk
-include BSP/Servo/subdir_vars.mk
-include BSP/TB6612/subdir_vars.mk
-include BSP/Task/subdir_vars.mk
-include BSP/Timer/subdir_vars.mk
-include BSP/subdir_vars.mk
-include subdir_rules.mk
-include BSP/Buzzer/subdir_rules.mk
-include BSP/Eight_Tracking/subdir_rules.mk
-include BSP/Key/subdir_rules.mk
-include BSP/LED/subdir_rules.mk
-include BSP/Motor/subdir_rules.mk
-include BSP/Questions/subdir_rules.mk
-include BSP/Servo/subdir_rules.mk
-include BSP/TB6612/subdir_rules.mk
-include BSP/Task/subdir_rules.mk
-include BSP/Timer/subdir_rules.mk
-include BSP/subdir_rules.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C55_DEPS)),)
-include $(C55_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(S67_DEPS)),)
-include $(S67_DEPS)
endif
ifneq ($(strip $(S62_DEPS)),)
-include $(S62_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(OPT_DEPS)),)
-include $(OPT_DEPS)
endif
ifneq ($(strip $(C??_DEPS)),)
-include $(C??_DEPS)
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(S??_DEPS)),)
-include $(S??_DEPS)
endif
ifneq ($(strip $(C64_DEPS)),)
-include $(C64_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(S64_DEPS)),)
-include $(S64_DEPS)
endif
ifneq ($(strip $(CLA_DEPS)),)
-include $(CLA_DEPS)
endif
ifneq ($(strip $(S55_DEPS)),)
-include $(S55_DEPS)
endif
ifneq ($(strip $(SV7A_DEPS)),)
-include $(SV7A_DEPS)
endif
ifneq ($(strip $(C62_DEPS)),)
-include $(C62_DEPS)
endif
ifneq ($(strip $(C67_DEPS)),)
-include $(C67_DEPS)
endif
ifneq ($(strip $(K_DEPS)),)
-include $(K_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C43_DEPS)),)
-include $(C43_DEPS)
endif
ifneq ($(strip $(S43_DEPS)),)
-include $(S43_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(SA_DEPS)),)
-include $(SA_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXE_OUTPUTS += \
Competition_415.out 

EXE_OUTPUTS__QUOTED += \
"Competition_415.out" 

BIN_OUTPUTS += \
Competition_415.hex 

BIN_OUTPUTS__QUOTED += \
"Competition_415.hex" 


# All Target
all: $(OBJS) $(GEN_CMDS)
	@$(MAKE) --no-print-directory -Onone "Competition_415.out" secondary-outputs

# Tool invocations
Competition_415.out: $(OBJS) $(GEN_CMDS)
	@echo 'Arm Linker - building target: "$@"'
	"C:/ti/ti_cgt_arm_llvm_4.0.2.LTS/bin/tiarmclang.exe" @"syscfg/device.opt"  -march=thumbv6m -mcpu=cortex-m0plus -mfloat-abi=soft -mlittle-endian -mthumb -O2 -gdwarf-3 -Wl,-m"Competition_415.map" -Wl,-i"C:/ti/mspm0_sdk_2_10_00_04/source" -Wl,-i"C:/Users/xxh/workspace_ccstheia/Competition_415/Debug/syscfg" -Wl,-i"C:/ti/ti_cgt_arm_llvm_4.0.2.LTS/lib" -Wl,--diag_wrap=off -Wl,--display_error_number -Wl,--warn_sections -Wl,--xml_link_info="Competition_415_linkInfo.xml" -Wl,--rom_model -o "Competition_415.out" $(ORDERED_OBJS)
	@echo 'Finished building target: "$@"'
	@echo ' '

Competition_415.hex: $(EXE_OUTPUTS)
	@echo 'Arm Hex Utility - building secondary target: "$@"'
	"C:/ti/ti_cgt_arm_llvm_4.0.2.LTS/bin/tiarmhex.exe" --memwidth=8 --romwidth=8 --intel -o "Competition_415.hex" $(EXE_OUTPUTS__QUOTED)
	@echo 'Finished building secondary target: "$@"'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(GEN_MISC_FILES__QUOTED)$(BIN_OUTPUTS__QUOTED)$(GEN_FILES__QUOTED)$(EXE_OUTPUTS__QUOTED)
	-$(RM) "empty.o" "syscfg\ti_msp_dl_config.o" "startup_mspm0g350x_ticlang.o" "BSP\Buzzer\buzzer.o" "BSP\Eight_Tracking\ADC.o" "BSP\Eight_Tracking\IIC.o" "BSP\Eight_Tracking\No_Mcu_Ganv_Grayscale_Sensor.o" "BSP\Eight_Tracking\app_irtracking.o" "BSP\Key\key.o" "BSP\LED\led.o" "BSP\Motor\app_motor.o" "BSP\Motor\app_motor_usart.o" "BSP\Motor\bsp_motor_usart.o" "BSP\Questions\questions.o" "BSP\Servo\servo.o" "BSP\Servo\servo_usart.o" "BSP\TB6612\tb6612.o" "BSP\Task\task.o" "BSP\Timer\timer.o" "BSP\delay.o" "BSP\usart.o" 
	-$(RM) "empty.d" "syscfg\ti_msp_dl_config.d" "startup_mspm0g350x_ticlang.d" "BSP\Buzzer\buzzer.d" "BSP\Eight_Tracking\ADC.d" "BSP\Eight_Tracking\IIC.d" "BSP\Eight_Tracking\No_Mcu_Ganv_Grayscale_Sensor.d" "BSP\Eight_Tracking\app_irtracking.d" "BSP\Key\key.d" "BSP\LED\led.d" "BSP\Motor\app_motor.d" "BSP\Motor\app_motor_usart.d" "BSP\Motor\bsp_motor_usart.d" "BSP\Questions\questions.d" "BSP\Servo\servo.d" "BSP\Servo\servo_usart.d" "BSP\TB6612\tb6612.d" "BSP\Task\task.d" "BSP\Timer\timer.d" "BSP\delay.d" "BSP\usart.d" 
	-$(RMDIR) $(GEN_MISC_DIRS__QUOTED)
	-@echo ' '

secondary-outputs: $(BIN_OUTPUTS)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets

