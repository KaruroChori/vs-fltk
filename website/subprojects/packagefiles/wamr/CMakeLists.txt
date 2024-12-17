cmake_minimum_required(VERSION 3.21)
project(wamr LANGUAGES C)
set (WAMR_ROOT_DIR .)

include(${WAMR_ROOT_DIR}/build-scripts/runtime_lib.cmake)
add_library(wamr ${WAMR_RUNTIME_LIB_SOURCE})
