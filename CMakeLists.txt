# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Initialise pico_sdk from installed location
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.1.1)
set(toolchainVersion 14_2_Rel1)
set(picotoolVersion 2.1.1)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()

set(PICO_BOARD pico_w CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(Tarefa01 C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add source file
add_executable(Tarefa01 Tarefa01.c)

# Define nome e versão
pico_set_program_name(Tarefa01 "Tarefa01")
pico_set_program_version(Tarefa01 "0.1")

# Configurar stdio
pico_enable_stdio_uart(Tarefa01 0)
pico_enable_stdio_usb(Tarefa01 0)

# Incluir bibliotecas necessárias
target_link_libraries(Tarefa01
    pico_stdlib
    hardware_adc
    hardware_irq
    hardware_gpio
    hardware_pwm
    pico_multicore
)
# Incluir diretórios
target_include_directories(Tarefa01 PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
)

# Geração de arquivos bin/hex/uf2
pico_add_extra_outputs(Tarefa01)
