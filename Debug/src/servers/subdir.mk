################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/servers/http_client.c \
../src/servers/http_server.c 

OBJS += \
./src/servers/http_client.o \
./src/servers/http_server.o 

C_DEPS += \
./src/servers/http_client.d \
./src/servers/http_server.d 


# Each subdirectory must supply rules for building sources it contributes
src/servers/%.o: ../src/servers/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


