SET(TARGET_ELF ${CMAKE_CURRENT_BINARY_DIR}/app_bcu.elf)




ADD_SUBDIRECTORY (dcm)
ADD_SUBDIRECTORY (arch)
ADD_SUBDIRECTORY (boards)
ADD_SUBDIRECTORY (drivers)
ADD_SUBDIRECTORY (include)
ADD_SUBDIRECTORY (ucos)
ADD_SUBDIRECTORY (libs)
ADD_SUBDIRECTORY (com)
ADD_SUBDIRECTORY (diagnostic)
ADD_SUBDIRECTORY (bms_rte)
ADD_SUBDIRECTORY (applications/bcu)
ADD_SUBDIRECTORY (config)

IF ("1" STREQUAL "${BUILD_FROM_SRC}")
ELSE ("1" STREQUAL "${BUILD_FROM_SRC}")
SET (INCUDE_DIRS ${INCUDE_DIRS} ${TOPDIR}/libappolo/${TARGET_BOARD})
SET (CMAKE_EXE_LINKER_FLAGS "${TARGET_EXE_LINKER_FLAGS} ${CMAKE_EXE_LINKER_FLAGS} -Add{${TOPDIR}/libappolo/${TARGET_BOARD}/appolo.lib}")
ENDIF ("1" STREQUAL "${BUILD_FROM_SRC}")

ADD_EXECUTABLE(app_bcu ${SOURCE_FILES})
INCLUDE_DIRECTORIES (${INCUDE_DIRS})

ADD_PC_LINT(app_bcu ${SOURCE_FILES})

ADD_CUSTOM_COMMAND (
    TARGET app_bcu
    COMMAND cp -v app_bcu.S19 ${RELASE_FILE_NAME}.S19
    COMMAND cp -v app_bcu.S19 applications/bcu/app_bcu.S19
    COMMAND cp -v app_bcu.S19 applications/bcu/${RELASE_FILE_NAME}.S19
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)

#ADD_PC_LINT(app_bcu ${THIS_DIR_SRC})
