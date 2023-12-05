cmake_minimum_required(VERSION 3.13)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/tools")
include(arm-none-eabi-gcc)

set(PRJ_NAME wiznet-surf5-fw)


project(${PRJ_NAME}
  LANGUAGES ASM C CXX
)


set(EXECUTABLE ${PRJ_NAME}.elf)


file(GLOB SRC_FILES CONFIGURE_DEPENDS
  src/*.cpp
  src/*.c
  src/bsp/*.c
  src/bsp/device/*.c

  src/lib/W7500x_StdPeriph_Driver/src/*.c
)

file(GLOB_RECURSE SRC_FILES_RECURSE CONFIGURE_DEPENDS
  src/ap/*.cpp
  src/ap/*.c
  src/bsp/*.s
  src/common/*.c
  src/hw/*.c
)

add_executable(${EXECUTABLE} 
  ${SRC_FILES}
  ${SRC_FILES_RECURSE}
)



target_include_directories(${EXECUTABLE} PRIVATE 
  src  
  src/ap
  src/bsp
  src/bsp/device
  src/common 
  src/common/core
  src/common/hw/include 
  src/hw
  src/hw/driver
  src/lib

  src/lib/CMSIS/Include
  src/lib/CMSIS/Device/WIZnet/W7500/Include
  src/lib/W7500x_StdPeriph_Driver/inc
)

target_compile_definitions(${EXECUTABLE} PRIVATE
  -DW7500
  )

target_compile_options(${EXECUTABLE} PRIVATE
  -mcpu=cortex-m0
  -mthumb
  -mfloat-abi=soft

  -fdata-sections
  -ffunction-sections

  -Wall
  -Wno-comment
  -g3
  -Og
  )

target_link_options(${EXECUTABLE} PRIVATE
  -T../src/bsp/ldscript/W7500_FLASH.ld
  -mcpu=cortex-m0
  -mthumb
  -mfloat-abi=soft
  -specs=nano.specs
  -lc
  -lm
  -lstdc++
  -lsupc++

  -Wl,-Map=${PRJ_NAME}.map,--cref
  -Wl,--gc-sections
  -Xlinker -print-memory-usage -Xlinker
  )

add_custom_command(TARGET ${EXECUTABLE} 
  POST_BUILD
  COMMAND ${CMAKE_OBJCOPY} ARGS -O binary ${EXECUTABLE} ${PROJECT_NAME}.bin
  COMMENT "Invoking: Make Binary"
  )  