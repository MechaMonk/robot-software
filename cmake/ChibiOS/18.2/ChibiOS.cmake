IF(NOT ChibiOS_FIND_COMPONENTS)
    SET(ChibiOS_FIND_COMPONENTS nil hal)
    MESSAGE(STATUS "No ChibiOS components specified, using default: ${ChibiOS_FIND_COMPONENTS}")
ENDIF()

SET (CHIBIOS_COMPONENTS nil rt hal streams shell cppwrappers evtimer)

LIST(FIND ChibiOS_FIND_COMPONENTS nil ChibiOS_FIND_COMPONENTS_nil)
LIST(FIND ChibiOS_FIND_COMPONENTS rt ChibiOS_FIND_COMPONENTS_rt)
LIST(FIND ChibiOS_FIND_COMPONENTS hal ChibiOS_FIND_COMPONENTS_hal)

IF((${ChibiOS_FIND_COMPONENTS_nil} LESS 0) AND (${ChibiOS_FIND_COMPONENTS_rt} LESS 0))
  MESSAGE(STATUS "No kernel component selected, using Nil kernel")
  LIST(APPEND ChibiOS_FIND_COMPONENTS nil)
  SET(CHIBIOS_KERNEL nil)
ELSE()
  IF((NOT (${ChibiOS_FIND_COMPONENTS_nil} LESS 0)) AND (NOT (${ChibiOS_FIND_COMPONENTS_rt} LESS 0)))
    MESSAGE(FATAL_ERROR "Cannot use RT and Nil kernel at the same time")
  ENDIF()
  IF(NOT (${ChibiOS_FIND_COMPONENTS_nil} LESS 0))
    SET(CHIBIOS_KERNEL nil)
  ELSE()
    SET(CHIBIOS_KERNEL rt)
  ENDIF()
ENDIF()

IF(${ChibiOS_FIND_COMPONENTS_hal} LESS 0)
  LIST(APPEND ChibiOS_FIND_COMPONENTS hal)
ENDIF()

IF(NOT CHIBIOS_HALCONF_FILE)
    MESSAGE(FATAL_ERROR "Cannot find halconf.h, please specify it using CHIBIOS_HALCONF_FILE variable")
ENDIF()
    
IF(NOT CHIBIOS_CHCONF_FILE)
    MESSAGE(FATAL_ERROR "Cannot find chconf.h, please specify it using CHIBIOS_CHCONF_FILE variable")
ENDIF()

FILE(STRINGS ${CHIBIOS_CHCONF_FILE} CHCONF_LINES REGEX "#define CH_CFG_USE_([a-zA-Z_0-9]+) +TRUE")
FOREACH(LINE ${CHCONF_LINES})
    STRING(REGEX REPLACE "#define CH_CFG_USE_([a-zA-Z_0-9]+) +TRUE" "\\1" COMP ${LINE})
    LIST(APPEND CHIBIOS_RTOS_COMPONENTS ${COMP})
ENDFOREACH()

MESSAGE(STATUS "Detected ChibiOS RTOS components:")
FOREACH(COMP ${CHIBIOS_RTOS_COMPONENTS})
    MESSAGE(STATUS "\t${COMP}")
ENDFOREACH()

FILE(STRINGS ${CHIBIOS_HALCONF_FILE} HALCONF_LINES REGEX "#define HAL_USE_([a-zA-Z_0-9]+) +TRUE")
FOREACH(LINE ${HALCONF_LINES})
    STRING(REGEX REPLACE "#define HAL_USE_([a-zA-Z_0-9]+) +TRUE" "\\1" COMP ${LINE})
    LIST(APPEND CHIBIOS_HAL_COMPONENTS ${COMP})
ENDFOREACH()

MESSAGE(STATUS "Detected ChibiOS HAL components:")
FOREACH(COMP ${CHIBIOS_HAL_COMPONENTS})
    MESSAGE(STATUS "\t${COMP}")
ENDFOREACH()

INCLUDE(ChibiOS/18.2/ChibiOS_RTOS)
INCLUDE(ChibiOS/18.2/ChibiOS_HAL)

SET (CHIBIOS_INCLUDES_streams
    os/hal/lib/streams
)

SET (CHIBIOS_SOURCES_streams
    os/hal/lib/streams/nullstreams.c
    os/hal/lib/streams/chprintf.c
    os/hal/lib/streams/memstreams.c
)

SET (CHIBIOS_INCLUDES_shell
    os/various/shell/
)

set (CHIBIOS_SOURCES_shell
    os/various/shell/shell.c
    os/various/shell/shell_cmd.c
)

SET (CHIBIOS_INCLUDES_cppwrappers
    os/various/cpp_wrappers
)

set (CHIBIOS_SOURCES_cppwrappers
    os/various/cpp_wrappers/ch.cpp
)

SET (CHIBIOS_INCLUDES_evtimer
    os/various/
)

set (CHIBIOS_SOURCES_evtimer
    os/various/evtimer.c
)


MESSAGE(STATUS "RTOS sources: ")
FOREACH(SOURCE ${CHIBIOS_SOURCES_${CHIBIOS_KERNEL}})
    MESSAGE(STATUS "\t${SOURCE}")
ENDFOREACH()

MESSAGE(STATUS "HAL sources: ")
FOREACH(SOURCE ${CHIBIOS_SOURCES_hal})
    MESSAGE(STATUS "\t${SOURCE}")
ENDFOREACH()

IF(NOT ChibiOS_LINKER_SCRIPT)
    MESSAGE(STATUS "ChibiOS doesn't have linker script for your chip, please specify it directly using ChibiOS_LINKER_SCRIPT variable.")
ENDIF()

FOREACH(comp ${ChibiOS_FIND_COMPONENTS})
    LIST(FIND CHIBIOS_COMPONENTS ${comp} INDEX)
    IF(INDEX EQUAL -1)
        MESSAGE(FATAL_ERROR "Unknown ChibiOS component: ${comp}\nSupported ChibiOS components: ${CHIBIOS_COMPONENTS}")
    ENDIF()
    FOREACH(source ${CHIBIOS_SOURCES_${comp}})
        FIND_FILE(CHIBIOS_${comp}_${source} NAMES ${source} PATHS ${CHIBIOS_ROOT} NO_DEFAULT_PATH CMAKE_FIND_ROOT_PATH_BOTH)
        LIST(APPEND ChibiOS_SOURCES ${CHIBIOS_${comp}_${source}})
    ENDFOREACH()
    FOREACH(incl ${CHIBIOS_INCLUDES_${comp}})
        LIST(APPEND ChibiOS_INCLUDE_DIRS ${CHIBIOS_ROOT}/${incl})
    ENDFOREACH()
ENDFOREACH()

