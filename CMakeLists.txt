cmake_minimum_required(VERSION 3.16.3)
project(Atmega128Test VERSION 1.0.0)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_CXX_COMPILER avr-g++)
set(CMAKE_C_COMPILER avr-gcc)
set(CMAKE_ASM_COMPILER avr-gcc)
set(PROG_TYPE atmelice)
set(MCU atmega128a)
set(BAUD 9600)
set(F_CPU 7372800UL)
set(PRODUCT_NAME ann-drivers)
set(CMAKE_EXE_LINKER_FLAGS -mmcu=${MCU})
include_directories(FreeRTOS/Source/include src/HardwareDrivers/include FreeRTOS/PortInclude /usr/avr/include/)
set(E_FUSE 0xFF)
set(H_FUSE 0x90)
set(L_FUSE 0xFF)
set(LOCK_BIT 0xff)


add_definitions(
    -DF_CPU=${F_CPU}
    -DBAUD=${BAUD}
)

add_compile_options(
    -Wall # enable warnings
    -g
    -mmcu=${MCU} # MCU
)

FILE(GLOB SRC_FILES src/*.c FreeRTOS/Source/*.c src/HardwareDrivers/*.c)

add_executable(${PRODUCT_NAME} ${SRC_FILES})
set_target_properties(${PRODUCT_NAME} PROPERTIES OUTPUT_NAME ${PRODUCT_NAME}.elf)
#add_custom_target(strip ALL avr-strip ${PRODUCT_NAME}.elf DEPENDS ${PRODUCT_NAME})
#add_custom_target(hex ALL avr-objcopy -R .eeprom -O ihex ${PRODUCT_NAME}.elf ${PRODUCT_NAME}.hex DEPENDS strip)
add_custom_target(fuses avrdude -c ${PROG_TYPE} -p m128 -U lfuse:w:${L_FUSE}:m -U hfuse:w:${H_FUSE}:m -U efuse:w:${E_FUSE}:m -U lock:w:${LOCK_BIT}:m )
add_custom_target(upload avrdude -c ${PROG_TYPE} -p m128 -U flash:w:${PRODUCT_NAME}.elf DEPENDS ${PRODUCT_NAME})
add_custom_command(TARGET ${PRODUCT_NAME}
                   POST_BUILD
                   COMMAND avr-size --mcu=atmega128 -C ${PRODUCT_NAME}.elf)