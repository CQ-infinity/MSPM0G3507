################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
BSP/TB6612/%.o: ../BSP/TB6612/%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Arm Compiler - building file: "$<"'
	"C:/ti/ti_cgt_arm_llvm_4.0.2.LTS/bin/tiarmclang.exe" -c @"syscfg/device.opt"  -march=thumbv6m -mcpu=cortex-m0plus -mfloat-abi=soft -mlittle-endian -mthumb -O2 -I"C:/Users/xxh/workspace_ccstheia/Competition_415" -I"C:/Users/xxh/workspace_ccstheia/Competition_415/Debug" -I"C:/ti/mspm0_sdk_2_10_00_04/source/third_party/CMSIS/Core/Include" -I"C:/ti/mspm0_sdk_2_10_00_04/source" -I"C:/Users/xxh/workspace_ccstheia/Competition_415/BSP" -I"C:/Users/xxh/workspace_ccstheia/Competition_415/BSP/Buzzer" -I"C:/Users/xxh/workspace_ccstheia/Competition_415/BSP/Eight_Tracking" -I"C:/Users/xxh/workspace_ccstheia/Competition_415/BSP/eMPL" -I"C:/Users/xxh/workspace_ccstheia/Competition_415/BSP/Key" -I"C:/Users/xxh/workspace_ccstheia/Competition_415/BSP/LED" -I"C:/Users/xxh/workspace_ccstheia/Competition_415/BSP/Motor" -I"C:/Users/xxh/workspace_ccstheia/Competition_415/BSP/MPU6050" -I"C:/Users/xxh/workspace_ccstheia/Competition_415/BSP/Questions" -I"C:/Users/xxh/workspace_ccstheia/Competition_415/BSP/TB6612" -I"C:/Users/xxh/workspace_ccstheia/Competition_415/BSP/Task" -I"C:/Users/xxh/workspace_ccstheia/Competition_415/BSP/Servo" -I"C:/Users/xxh/workspace_ccstheia/Competition_415/BSP/Timer" -gdwarf-3 -MMD -MP -MF"BSP/TB6612/$(basename $(<F)).d_raw" -MT"$(@)" -I"C:/Users/xxh/workspace_ccstheia/Competition_415/Debug/syscfg"  $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


