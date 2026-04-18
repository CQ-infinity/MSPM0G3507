################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
BSP/Task/%.o: ../BSP/Task/%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"D:/TI/CCS/ccs/tools/compiler/ti-cgt-armllvm_4.0.3.LTS/bin/tiarmclang.exe" -c @"syscfg/device.opt"  -march=thumbv6m -mcpu=cortex-m0plus -mfloat-abi=soft -mlittle-endian -mthumb -O2 -I"C:/Users/33253/workspace_ccstheia/Competition_415" -I"C:/Users/33253/workspace_ccstheia/Competition_415/Debug" -I"C:/TI/mspm0_sdk_2_05_00_05/source/third_party/CMSIS/Core/Include" -I"C:/TI/mspm0_sdk_2_05_00_05/source" -I"C:/Users/33253/workspace_ccstheia/Competition_415/BSP" -I"C:/Users/33253/workspace_ccstheia/Competition_415/BSP/Buzzer" -I"C:/Users/33253/workspace_ccstheia/Competition_415/BSP/Eight_Tracking" -I"C:/Users/33253/workspace_ccstheia/Competition_415/BSP/eMPL" -I"C:/Users/33253/workspace_ccstheia/Competition_415/BSP/Key" -I"C:/Users/33253/workspace_ccstheia/Competition_415/BSP/LED" -I"C:/Users/33253/workspace_ccstheia/Competition_415/BSP/Motor" -I"C:/Users/33253/workspace_ccstheia/Competition_415/BSP/MPU6050" -I"C:/Users/33253/workspace_ccstheia/Competition_415/BSP/Questions" -I"C:/Users/33253/workspace_ccstheia/Competition_415/BSP/TB6612" -I"C:/Users/33253/workspace_ccstheia/Competition_415/BSP/Task" -I"C:/Users/33253/workspace_ccstheia/Competition_415/BSP/Servo" -I"C:/Users/33253/workspace_ccstheia/Competition_415/BSP/Timer" -gdwarf-3 -MMD -MP -MF"BSP/Task/$(basename $(<F)).d_raw" -MT"$(@)" -I"C:/Users/33253/workspace_ccstheia/Competition_415/Debug/syscfg"  $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


