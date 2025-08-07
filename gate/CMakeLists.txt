add_library(gate STATIC gate.cpp)

target_include_directories(gate PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../common)
