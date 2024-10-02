# groups.cmake

# group Application/MDK-ARM
add_library(Group_Application_MDK-ARM OBJECT
  "${SOLUTION_ROOT}/startup_stm32f103xb.s"
)
target_include_directories(Group_Application_MDK-ARM PUBLIC
  $<TARGET_PROPERTY:${CONTEXT},INTERFACE_INCLUDE_DIRECTORIES>
)
target_compile_definitions(Group_Application_MDK-ARM PUBLIC
  $<TARGET_PROPERTY:${CONTEXT},INTERFACE_COMPILE_DEFINITIONS>
)
target_compile_options(Group_Application_MDK-ARM PUBLIC
  $<TARGET_PROPERTY:${CONTEXT},INTERFACE_COMPILE_OPTIONS>
)
target_link_libraries(Group_Application_MDK-ARM PUBLIC
  ${CONTEXT}_ABSTRACTIONS
)
set(COMPILE_DEFINITIONS
  STM32F10X_MD
  _RTE_
)
cbuild_set_defines(AS_ARM COMPILE_DEFINITIONS)
set_source_files_properties("${SOLUTION_ROOT}/startup_stm32f103xb.s" PROPERTIES
  COMPILE_FLAGS "${COMPILE_DEFINITIONS}"
)

# group Application/User/Core
add_library(Group_Application_User_Core OBJECT
  "${SOLUTION_ROOT}/../Core/Src/main.c"
  "${SOLUTION_ROOT}/../Core/Src/gpio.c"
  "${SOLUTION_ROOT}/../Core/Src/usart.c"
  "${SOLUTION_ROOT}/../Core/Src/stm32f1xx_it.c"
  "${SOLUTION_ROOT}/../Core/Src/stm32f1xx_hal_msp.c"
  "${SOLUTION_ROOT}/../Core/Src/BC260Y.c"
  "${SOLUTION_ROOT}/../Core/Src/COD.c"
)
target_include_directories(Group_Application_User_Core PUBLIC
  $<TARGET_PROPERTY:${CONTEXT},INTERFACE_INCLUDE_DIRECTORIES>
)
target_compile_definitions(Group_Application_User_Core PUBLIC
  $<TARGET_PROPERTY:${CONTEXT},INTERFACE_COMPILE_DEFINITIONS>
)
target_compile_options(Group_Application_User_Core PUBLIC
  $<TARGET_PROPERTY:${CONTEXT},INTERFACE_COMPILE_OPTIONS>
)
target_link_libraries(Group_Application_User_Core PUBLIC
  ${CONTEXT}_ABSTRACTIONS
)

# group Drivers/STM32F1xx_HAL_Driver
add_library(Group_Drivers_STM32F1xx_HAL_Driver OBJECT
  "${SOLUTION_ROOT}/../Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_gpio_ex.c"
  "${SOLUTION_ROOT}/../Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_tim.c"
  "${SOLUTION_ROOT}/../Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_tim_ex.c"
  "${SOLUTION_ROOT}/../Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_uart.c"
  "${SOLUTION_ROOT}/../Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal.c"
  "${SOLUTION_ROOT}/../Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_rcc.c"
  "${SOLUTION_ROOT}/../Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_rcc_ex.c"
  "${SOLUTION_ROOT}/../Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_gpio.c"
  "${SOLUTION_ROOT}/../Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_dma.c"
  "${SOLUTION_ROOT}/../Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_cortex.c"
  "${SOLUTION_ROOT}/../Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_pwr.c"
  "${SOLUTION_ROOT}/../Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_flash.c"
  "${SOLUTION_ROOT}/../Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_flash_ex.c"
  "${SOLUTION_ROOT}/../Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_exti.c"
)
target_include_directories(Group_Drivers_STM32F1xx_HAL_Driver PUBLIC
  $<TARGET_PROPERTY:${CONTEXT},INTERFACE_INCLUDE_DIRECTORIES>
)
target_compile_definitions(Group_Drivers_STM32F1xx_HAL_Driver PUBLIC
  $<TARGET_PROPERTY:${CONTEXT},INTERFACE_COMPILE_DEFINITIONS>
)
target_compile_options(Group_Drivers_STM32F1xx_HAL_Driver PUBLIC
  $<TARGET_PROPERTY:${CONTEXT},INTERFACE_COMPILE_OPTIONS>
)
target_link_libraries(Group_Drivers_STM32F1xx_HAL_Driver PUBLIC
  ${CONTEXT}_ABSTRACTIONS
)

# group Drivers/CMSIS
add_library(Group_Drivers_CMSIS OBJECT
  "${SOLUTION_ROOT}/../Core/Src/system_stm32f1xx.c"
)
target_include_directories(Group_Drivers_CMSIS PUBLIC
  $<TARGET_PROPERTY:${CONTEXT},INTERFACE_INCLUDE_DIRECTORIES>
)
target_compile_definitions(Group_Drivers_CMSIS PUBLIC
  $<TARGET_PROPERTY:${CONTEXT},INTERFACE_COMPILE_DEFINITIONS>
)
target_compile_options(Group_Drivers_CMSIS PUBLIC
  $<TARGET_PROPERTY:${CONTEXT},INTERFACE_COMPILE_OPTIONS>
)
target_link_libraries(Group_Drivers_CMSIS PUBLIC
  ${CONTEXT}_ABSTRACTIONS
)
