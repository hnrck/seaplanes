# Additional target for code linting with clang-tidy.

set(SRCS_DIR ${CMAKE_SOURCE_DIR}/src/)
set(INCS_DIR ${CMAKE_SOURCE_DIR}/include/)
set(TEMPLATES_DIR ${CMAKE_SOURCE_DIR}/templates/)

file(GLOB_RECURSE SRCS ${SRCS_DIR}/*.cpp)
file(GLOB_RECURSE INCS ${INCS_DIR}/*.h)
file(GLOB_RECURSE TEMPLATES ${TEMPLATES_DIR}/*.tcc)

find_program(CLANG-TIDY clang-tidy)

if (CLANG-TIDY)
    add_custom_target(lint)

    set(CLANG_TIDY_CHECKS "*,-clang-analyzer-alpha.*,-llvm-header-guard,-fuchsia-*")
    set(CLANG_TIDY_CXX_STD c++14)

    foreach (SRC ${SRCS})
        add_custom_command(TARGET lint COMMAND ${CLANG-TIDY} -checks=${CLANG_TIDY_CHECKS} ${SRC} -- -std=${CLANG_TIDY_CXX_STD} -I${INCS_DIR} -I${TEMPLATES_DIR} -x c++)
    endforeach (SRC ${SRCS})

    foreach (INC ${INCS})
        add_custom_command(TARGET lint COMMAND ${CLANG-TIDY} -checks=${CLANG_TIDY_CHECKS} ${INC} -- -std=${CLANG_TIDY_CXX_STD} -I${INCS_DIR} -I${TEMPLATES_DIR} -x c++)
    endforeach (INC ${INCS})

    foreach (TEMPLATE ${TEMPLATES})
        add_custom_command(TARGET lint COMMAND ${CLANG-TIDY} -checks=${CLANG_TIDY_CHECKS} ${TEMPLATE} -- -std=${CLANG_TIDY_CXX_STD} -I${TEMPLATES_DIR} -I${TEMPLATES_DIR} -x c++)
    endforeach (TEMPLATE ${TEMPLATES})
endif (CLANG-TIDY)
