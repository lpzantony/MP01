################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/core/Benchmark.cpp \
../src/core/IRCLIENT.cpp \
../src/core/Shell.cpp 

OBJS += \
./src/core/Benchmark.o \
./src/core/IRCLIENT.o \
./src/core/Shell.o 

CPP_DEPS += \
./src/core/Benchmark.d \
./src/core/IRCLIENT.d \
./src/core/Shell.d 


# Each subdirectory must supply rules for building sources it contributes
src/core/%.o: ../src/core/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++11 -I"/Users/edwardvergara/Documents/TPT/2A/MP01/IRCLIENT/src" -O3 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


