# file CMakeLists.txt
# author Henrick Deschamps
# version 1.0.0
# date 2016-06-16

#-----------------------------------------------------------------------------------------------------------------------
# Project configuration

# Project name
project(seaplanes CXX)

# CMake minimum version required set by default, might be changed.
cmake_minimum_required(VERSION 3.8)

# Description summary of the project.
set(DESCRIPTION_SUMMARY "SEAPLANES")

# Application Binary Interface version for library/application interfacing.
set(ABI_VERSION_MAJOR "1")
set(ABI_VERSION_MINOR "1")
set(ABI_VERSION_PATCH "0")
set(ABI_VERSION "${ABI_VERSION_MAJOR}.${ABI_VERSION_MINOR}.${ABI_VERSION_PATCH}")

# Setting compilation options.
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wpedantic")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wextra")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Weffc++")

# CXX flags depending on the build type.
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -g")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -DNDEBUG")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} -O2 -g")
set(CMAKE_CXX_FLAGS_MINSIZEREL "${CMAKE_CXX_FLAGS_MINSIZEREL} -Os")

# Default build type if none provided.
if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE RelWithDebInfo)
endif ()

# Location for the SEAPlanes library build.
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY usr/lib)
set(CMAKE_INCLUDE_OUTPUT_DIRECTORY usr/include)

if (APPLE)
    set(CMAKE_MACOSX_RPATH ON)
    find_library(RTI_LIB RTI-NG ENV DYLD_LIBRARY_PATH)
    find_library(FEDTIME_LIB FedTimed ENV DYLD_LIBRARY_PATH)
    find_library(HLA_LIB HLAd ENV DYLD_LIBRARY_PATH)
    if (NOT RTI_LIB)
        find_library(RTI_LIB RTI-NGd ENV DYLD_LIBRARY_PATH)
    endif ()
    get_filename_component(RTI_LIB_PATH ${RTI_LIB} DIRECTORY)
    set(CMAKE_INSTALL_RPATH "")
else ()
    find_library(RTI_LIB RTI-NG ENV LIBRARY_PATH)
    find_library(FEDTIME_LIB FedTimed ENV LIBRARY_PATH)
    find_library(HLA_LIB HLAd ENV LIBRARY_PATH)
    if (NOT RTI_LIB)
        find_library(RTI_LIB RTI-NGd ENV LIBRARY_PATH)
    endif ()
endif ()
#-----------------------------------------------------------------------------------------------------------------------

#-----------------------------------------------------------------------------------------------------------------------
# Extra modules

# Allowing alternatives for compilations.
option(COVERAGE "Allow code coverage. (requires GCOV. Optionnaly LCOV and genhtml for reports)." OFF)

# Adding CMake extra modules for code linting/formatting.
set(CMAKE_EXTRA_MODULES_DIR ${CMAKE_SOURCE_DIR}/cmake-extra-modules)
set(CMAKE_EXTRA_MODULES clang-format clang-tidy)
foreach (CMAKE_EXTRA_MODULE ${CMAKE_EXTRA_MODULES})
    include(${CMAKE_EXTRA_MODULES_DIR}/${CMAKE_EXTRA_MODULE}.cmake)
endforeach ()
#-----------------------------------------------------------------------------------------------------------------------

#-----------------------------------------------------------------------------------------------------------------------
# Build

# Include directories
include_directories(
        ${CMAKE_SOURCE_DIR}/src
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/templates
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DFEDERATION_NAME='\"SEAPLANES\"'")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DFEDERATION_FILE='\"FOM_Seaplanes.fed\"'")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DFEDERATION_SYNC_POINT_NAME='\"syncPoint\"'")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DUSE_CERTI_MESSAGE_BUFFER='0'")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DRTI_USES_STD_FSTREAM='1'")

# Include files
set(SEAPLANES_INC
        ${CMAKE_SOURCE_DIR}/include/ProtoLogicalProcessor.h
        ${CMAKE_SOURCE_DIR}/include/TimeManagementPolicyFactory.h
        ${CMAKE_SOURCE_DIR}/include/ITimeManagementPolicy.h
        ${CMAKE_SOURCE_DIR}/include/TimeManagementPolicy.h
        ${CMAKE_SOURCE_DIR}/include/TimeManagementPolicyTimeStep.h
        ${CMAKE_SOURCE_DIR}/include/seaplanes.h
        ${CMAKE_SOURCE_DIR}/include/SeaplanesTime.h
        ${CMAKE_SOURCE_DIR}/include/LogicalProcessorCommon.h
        ${CMAKE_SOURCE_DIR}/include/LogicalProcessorAttribute.h
        ${CMAKE_SOURCE_DIR}/include/LogicalProcessorObjectClass.h
        ${CMAKE_SOURCE_DIR}/include/LogicalProcessorObjectInstance.h
        ${CMAKE_SOURCE_DIR}/include/LogicalProcessorObjectInstancePublished.h
        ${CMAKE_SOURCE_DIR}/include/LogicalProcessorObjectInstanceSubscribed.h
        ${CMAKE_SOURCE_DIR}/include/LogicalProcessorLogger.h
        ${CMAKE_SOURCE_DIR}/templates/TimeLiterals.tcc
        )


# Change some names.
set(SEAPLANES_LIB ${PROJECT_NAME})
set(SRC_SEAPLANES_LIB
        ${SEAPLANES_INC}
        ${CMAKE_SOURCE_DIR}/src/ProtoLogicalProcessor.cpp
        ${CMAKE_SOURCE_DIR}/src/TimeManagementPolicyFactory.cpp
        ${CMAKE_SOURCE_DIR}/src/TimeManagementPolicy.cpp
        ${CMAKE_SOURCE_DIR}/src/TimeManagementPolicyTimeStep.cpp
        ${CMAKE_SOURCE_DIR}/src/SeaplanesTime.cpp
        ${CMAKE_SOURCE_DIR}/src/LogicalProcessorAttribute.cpp
        ${CMAKE_SOURCE_DIR}/src/LogicalProcessorObjectClass.cpp
        ${CMAKE_SOURCE_DIR}/src/LogicalProcessorObjectInstance.cpp
        ${CMAKE_SOURCE_DIR}/src/LogicalProcessorObjectInstancePublished.cpp
        ${CMAKE_SOURCE_DIR}/src/LogicalProcessorObjectInstanceSubscribed.cpp
        ${CMAKE_SOURCE_DIR}/src/LogicalProcessorLogger.cpp)

add_library(${SEAPLANES_LIB} SHARED ${SRC_SEAPLANES_LIB})
set_target_properties(${SEAPLANES_LIB} PROPERTIES VERSION ${ABI_VERSION} SOVERSION ${ABI_VERSION_MAJOR})
target_link_libraries(${SEAPLANES_LIB})

if (APPLE)
    set(CMAKE_MACOSX_RPATH ON)
    target_link_libraries(${SEAPLANES_LIB} ${RTI_LIB} ${FEDTIME_LIB} ${HLA_LIB})
endif ()
#-----------------------------------------------------------------------------------------------------------------------

#-----------------------------------------------------------------------------------------------------------------------
# Installation

# Path configuration for SEAPlanes.
set(SEAPLANES_PATHS_CONF seaplanespathsrc)
file(WRITE ${CMAKE_BINARY_DIR}/${SEAPLANES_PATHS_CONF}
        "export LIBRARY_PATH=$LIBRARY_PATH:${CMAKE_INSTALL_PREFIX}/${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/${PROJECT_NAME}/\n"
        "export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:${CMAKE_INSTALL_PREFIX}/${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/${PROJECT_NAME}/\n"
        "export C_PATH=$C_PATH:${CMAKE_INSTALL_PREFIX}/${CMAKE_INCLUDE_OUTPUT_DIRECTORY}/${PROJECT_NAME}/\n"
        "export CPLUS_INCLUDE_PATH=$CPLUS_INCLUDE_PATH:${CMAKE_INSTALL_PREFIX}/${CMAKE_INCLUDE_OUTPUT_DIRECTORY}/${PROJECT_NAME}/\n"
        )
if (APPLE)
    file(APPEND ${CMAKE_BINARY_DIR}/${SEAPLANES_PATHS_CONF} "export DYLD_LIBRARY_PATH=$DYLD_LIBRARY_PATH:${CMAKE_INSTALL_PREFIX}/${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/${PROJECT_NAME}/\n")
endif ()


# Configurations to install.
set(SEAPLANES_CONFS ${CMAKE_BINARY_DIR}/${SEAPLANES_PATHS_CONF})

# Installation recipes.
# The SEAPlanes library.
set(LIBRARY_INSTALL_DIR ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/${PROJECT_NAME}/)
install(TARGETS ${SEAPLANES_LIB} PUBLIC_HEADER LIBRARY DESTINATION ${LIBRARY_INSTALL_DIR})

# The SEAPlanes library includes.
set(INCLUDE_INSTALL_DIR ${CMAKE_INCLUDE_OUTPUT_DIRECTORY}/${PROJECT_NAME}/)
install(FILES ${SEAPLANES_INC} DESTINATION ${INCLUDE_INSTALL_DIR})

# The SEAPlanes library configuration files.
set(CONFS_INSTALL_DIR etc/${PROJECT_NAME})
install(FILES ${SEAPLANES_CONFS} DESTINATION ${CONFS_INSTALL_DIR}/)


# If in debug, coverage report can be generated with <cmake> ... -DCOVERAGE=ON and <make> coverage
if (CMAKE_BUILD_TYPE MATCHES "Debug")
    if (COVERAGE)
        find_program(MAKE_DIR mkdir)
        find_program(LCOV lcov)
        find_program(GEN_HTML genhtml)
        find_program(PRINT echo)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage")
        if ("${CMAKE_C_COMPILER_ID}" STREQUAL "Clang")
            set_property(TARGET ${PROJECT_NAME} APPEND_STRING PROPERTY LINK_FLAGS "--coverage ")
        endif ()
        target_link_libraries(${PROJECT_NAME} gcov)
        set(COVERAGE_DIR ${CMAKE_BINARY_DIR}/coverage)
        set(CMAKE_COV
                ${MAKE_DIR} -p ${COVERAGE_DIR}
                && ${LCOV} -c
                -b ${CMAKE_BINARY_DIR}/CMakeFiles/${PROJECT_NAME}.dir/src
                -d ${CMAKE_BINARY_DIR}/CMakeFiles/${PROJECT_NAME}.dir/src
                -o ${COVERAGE_DIR}/coverage.run
                && ${GEN_HTML} -o ${COVERAGE_DIR} ${COVERAGE_DIR}/coverage.run
                && ${PRINT} "Coverage report generated. Please open ${COVERAGE_DIR}/index.html with a browser.")
        add_custom_target(coverage COMMAND ${CMAKE_COV})
    endif ()
else ()
    if (COVERAGE)
        message(WARNING
                "coverage is only available with Debug build type."
                )
    endif ()
endif ()

# Special install_seaplanes target for certain IDE.
add_custom_target(install_${PROJECT_NAME}
        "${CMAKE_COMMAND}" --build "${CMAKE_BINARY_DIR}" --target install
        DEPENDS ${PROJECT_NAME}
        COMMENT "Installing ${PROJECT_NAME} in ${CMAKE_INSTALL_PREFIX}"
        )
#-----------------------------------------------------------------------------------------------------------------------

#-----------------------------------------------------------------------------------------------------------------------
# Documentation, with Doxygen
find_package(Doxygen)
option(BUILD_DOCUMENTATION "Create and install the HTML based API documentation (requires Doxygen)" ${DOXYGEN_FOUND})

if (BUILD_DOCUMENTATION)
    set(doxyfile_in ${CMAKE_CURRENT_SOURCE_DIR}/doc/Doxyfile.in)
    set(doxyfile ${CMAKE_CURRENT_BINARY_DIR}/doc/Doxyfile)

    configure_file(${doxyfile_in} ${doxyfile} @ONLY)

    add_custom_target(doc
            COMMAND ${DOXYGEN_EXECUTABLE} ${doxyfile}
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
            COMMENT "Generating API documentation with Doxygen"
            VERBATIM)
endif ()
#-----------------------------------------------------------------------------------------------------------------------

#-----------------------------------------------------------------------------------------------------------------------
# Project summary for CMake run

string(TOUPPER "${CMAKE_BUILD_TYPE}" BUILD_TYPE)

message(STATUS "+------------------------------------")
message(STATUS "|   ${PROJECT_NAME} built summary")
message(STATUS "+------------------------------------")
message(STATUS "| Description:")
message(STATUS "| ${DESCRIPTION_SUMMARY}")
message(STATUS "+------------------------------------")
message(STATUS "| ")
message(STATUS "| library ABI version: ${ABI_VERSION}")
message(STATUS "| generator:           ${CMAKE_GENERATOR}")
message(STATUS "| build type:          ${CMAKE_BUILD_TYPE}")
message(STATUS "| ")
message(STATUS "| CXX:                 ${CMAKE_CXX_COMPILER}")
message(STATUS "| CXXFLAGS:            ${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_${BUILD_TYPE}}")
message(STATUS "| LDFLAGS:             ${LDFLAGS}")
message(STATUS "| ")
message(STATUS "| source directory:    ${CMAKE_CURRENT_SOURCE_DIR}")
message(STATUS "| build directory:     ${CMAKE_CURRENT_BINARY_DIR}")
message(STATUS "| install prefix:      ${CMAKE_INSTALL_PREFIX}")
message(STATUS "| include directory:   ${CMAKE_INSTALL_PREFIX}/${INCLUDE_INSTALL_DIR}")
message(STATUS "| library directory:   ${CMAKE_INSTALL_PREFIX}/${LIBRARY_INSTALL_DIR}")
message(STATUS "| confs directory:     ${CMAKE_INSTALL_PREFIX}/${CONFS_INSTALL_DIR}")
message(STATUS "| ")
if (CMAKE_BUILD_TYPE MATCHES "Debug")
    message(STATUS "| coverage:            ${COVERAGE}")
endif ()
if (APPLE)
    message(STATUS "| rpath:               ${CMAKE_INSTALL_RPATH}")
endif ()
message(STATUS "+------------------------------------")
#-----------------------------------------------------------------------------------------------------------------------
