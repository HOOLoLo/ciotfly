cmake_minimum_required(VERSION 3.8)
project(ciotfly)

set(CMAKE_CXX_STANDARD 11)

LINK_LIBRARIES(-lpthread -lm -ldl)
set(SOURCE_FILES
        src/analy.c
        src/analy.h
        src/ancom.c
        src/ancom.h
        src/cetc_sock.c
        src/cetc_sock.h
        src/client_tcp.c
        src/client_tcp.h
        src/iotfly.c
        src/log.c
        src/log.h
        src/point_s.c
        src/point_s.h
        src/serial_usb_com.c
        src/serial_usb_com.h
        src/server_sock.c
        src/server_sock.h
        src/signal.c
        src/signal.h
        src/threadi.c
        src/threadi.h
        src/uart.h
        src/unidimensional.c
        src/unidimensional.h
        Location.so
        src/app_user.h
        src/app_user.c
        src/get_light.cpp
        src/get_light.h
        src/test.cpp)

add_executable(ciotfly ${SOURCE_FILES})