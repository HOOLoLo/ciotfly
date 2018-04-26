################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/analy.c \
../src/ancom.c \
../src/cetc_sock.c \
../src/client_tcp.c \
../src/iotfly.c \
../src/log.c \
../src/point_s.c \
../src/serial_usb_com.c \
../src/server_sock.c \
../src/signal.c \
../src/threadi.c \
../src/uart.c \
../src/unidimensional.c 

OBJS += \
./src/analy.o \
./src/ancom.o \
./src/cetc_sock.o \
./src/client_tcp.o \
./src/iotfly.o \
./src/log.o \
./src/point_s.o \
./src/serial_usb_com.o \
./src/server_sock.o \
./src/signal.o \
./src/threadi.o \
./src/uart.o \
./src/unidimensional.o 

C_DEPS += \
./src/analy.d \
./src/ancom.d \
./src/cetc_sock.d \
./src/client_tcp.d \
./src/iotfly.d \
./src/log.d \
./src/point_s.d \
./src/serial_usb_com.d \
./src/server_sock.d \
./src/signal.d \
./src/threadi.d \
./src/uart.d \
./src/unidimensional.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	arm-fsl-linux-gnueabi-gcc -I/home/cetc/workspace/IOT_MAINBOARD_final/IOT_MAINBOARD/tools/gcc-4.6.2-glibc-2.13-linaro-multilib-2011.12/fsl-linaro-toolchain/arm-fsl-linux-gnueabi/include -I/home/cetc/workspace/IOT_MAINBOARD_final/IOT_MAINBOARD/app/user/app_user/include -I/home/cetc/workspace/IOT_MAINBOARD_final/IOT_MAINBOARD/tools/gcc-4.6.2-glibc-2.13-linaro-multilib-2011.12/fsl-linaro-toolchain/arm-fsl-linux-gnueabi/multi-libs/usr/include -I/home/cetc/workspace/IOT_MAINBOARD_final/IOT_MAINBOARD/tools/gcc-4.6.2-glibc-2.13-linaro-multilib-2011.12/fsl-linaro-toolchain/lib/gcc/arm-fsl-linux-gnueabi/4.6.2/include -I/home/cetc/workspace/IOT_MAINBOARD_final/IOT_MAINBOARD/tools/gcc-4.6.2-glibc-2.13-linaro-multilib-2011.12/fsl-linaro-toolchain/lib/gcc/arm-fsl-linux-gnueabi/4.6.2/include-fixed -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


