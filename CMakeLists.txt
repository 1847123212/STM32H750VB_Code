cmake_minimum_required (VERSION 3.6)

set(CMAKE_TRY_COMPILE_TARGET_TYPE "STATIC_LIBRARY")

set(PROJ "firmware")

project (${PROJ} C ASM)

set(CMAKE_VERBOSE_MAKEFILE True)
set(SOURCE_DIR ${CMAKE_SOURCE_DIR}/Core )
set(CMSIS_DIR ${CMAKE_SOURCE_DIR}/Drivers/CMSIS)
set(DEVICE_DIR ${CMAKE_SOURCE_DIR}/Drivers/CMSIS/Device/ST/STM32H7xx)
set(HAL_DIR ${CMAKE_SOURCE_DIR}/Drivers/STM32H7xx_HAL_Driver)
set(RTOS_DIR ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/FreeRTOS/Source)
set(CMSIS5_DIR ${CMAKE_SOURCE_DIR}/CMSIS5/ )
set(DSP_DIR ${CMSIS5_DIR}/CMSIS/DSP/)



include_directories(${SOURCE_DIR}/Inc)
include_directories(${HAL_DIR}/Inc)
include_directories(${HAL_DIR}/Inc/Legacy)
include_directories(${RTOS_DIR}/include)
include_directories(${RTOS_DIR}/CMSIS_RTOS_V2)
include_directories(${RTOS_DIR}/portable/GCC/ARM_CM4F)
include_directories(${DEVICE_DIR}/Include)
include_directories(${CMSIS_DIR}/Include)
include_directories(${CMSIS5_DIR}/CMSIS/DSP/Include)

set(CPU "-mcpu=cortex-m7")
set(FPU "-mfpu=fpv5-d16")
set(FLOAT_ABI "-mfloat-abi=hard") 
set(MCU "${CPU} -mthumb ${FPU} ${FLOAT-ABI}")

add_definitions(-DUSE_HAL_DRIVER 
                -DSTM32H750xx)

 
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_ASM_COMPILER "arm-none-eabi-gcc")
set(CMAKE_C_FLAGS "-O0 -g -Wall -Werror -Wno-pointer-sign -ffunction-sections -fdata-sections")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mfloat-abi=hard -mcpu=cortex-m7 -mfpu=fpv5-d16 -mthumb")
# et(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -MMD -MP -MF'$(@:%.o=%.d)' ")

set (LDSCRIPT "STM32H750VBTx_FLASH.ld")

SET (LIBS  "-lc -lm -lnosys" )
SET (LIBDIR )
set (CMAKE_EXE_LINKER_FLAGS "${MCU} -specs=nano.specs -T../${LDSCRIPT} ${LIBDIR} ${LIBS} -Wl,-Map=./${Proj}.map,--cref -Wl,--gc-sections")
# set (CMAKE_EXE_LINKER_FLAGS "-specs=nano.specs -T../${LDSCRIPT} ${LIBDIR} ${LIBS} -Wl, --cref -Wl, --gc-sections")


FILE(GLOB_RECURSE CORE_SOURCE ${SOURCE_DIR}/*.c)


SET(HAL_SOURCE Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal.c)
SET(HAL_FEATURES cortex rcc flash gpio hsem dma mdma pwr i2c exti spi tim)
foreach(X ${HAL_FEATURES})
    FILE(GLOB_RECURSE T_SOURCE  ${HAL_DIR}/stm32h7xx_hal_${X}.c)
    FILE(GLOB_RECURSE T_EX_SOURCE  ${HAL_DIR}/stm32h7xx_hal_${X}_ex.c)
    List(APPEND HAL_SOURCE ${T_SOURCE} ${T_EX_SOURCE})
endforeach(X ${HAL_FEATURES})



FILE(GLOB RTOS_SOURCE ${RTOS_DIR}/*.c)
List(APPEND RTOS_SOURCE 
            Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2/cmsis_os2.c 
            Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2/cmsis_os1.c 
            Middlewares/Third_Party/FreeRTOS/Source/portable/MemMang/heap_4.c 
            Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F/port.c )


SET(CMAKE_ASM_FLAGS "${CFLAGS} " )
SET(ASM_OPTIONS "-x assembler-with-cpp")

list(APPEND CMAKE_MODULE_PATH ${DSP_DIR})

FILE(GLOB_RECURSE DSP_SOURCE ${DSP_DIR}/Source/*.c)


set(ARM_CPU "cortex-m7")
add_subdirectory(${DSP_DIR}/Source)

ADD_EXECUTABLE(${PROJ}.elf 
                ${CORE_SOURCE}
                ${HAL_SOURCE}
                ${RTOS_SOURCE}
                ./startup_stm32h750xx.s
                )
            
target_link_libraries(${PROJ}.elf PRIVATE CMSISDSP)

# ADD_CUSTOM_COMMAND(
#   TARGET ${PROJ}.elf
#   COMMAND "arm-none-eabi-objdump"
#   ARGS "-S" "${PROJ}.elf" ">>" "${PROJ}.lst")

ADD_CUSTOM_COMMAND(
  TARGET ${PROJ}.elf
  COMMAND "arm-none-eabi-size"
  ARGS  "${PROJ}.elf")

ADD_CUSTOM_COMMAND(
  TARGET ${PROJ}.elf
  COMMAND "arm-none-eabi-objcopy"
  ARGS  "-O" "binary" "-S" "${PROJ}.elf" "${PROJ}.bin")

ADD_CUSTOM_COMMAND(
  TARGET ${PROJ}.elf
  COMMAND "arm-none-eabi-objcopy"
  ARGS  "-O" "ihex" "${PROJ}.elf" "${PROJ}.hex")