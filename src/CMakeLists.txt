add_executable(tester
    tester.cpp
    netDefine.cpp
    netSaveRestore.cpp
    netOperate.cpp
    netTrain.cpp
    netMetric.cpp
)

target_link_libraries(tester gate)

target_include_directories(tester PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../common)
