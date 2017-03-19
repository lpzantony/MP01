################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/messages/Msg.cpp 

OBJS += \
./src/messages/Msg.o 

CPP_DEPS += \
./src/messages/Msg.d 


# Each subdirectory must supply rules for building sources it contributes
src/messages/%.o: ../src/messages/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++11 -I"/Users/edwardvergara/Documents/TPT/2A/MP01/IRCLIENT/src" -O3 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


