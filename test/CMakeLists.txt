cmake_minimum_required(VERSION 3.9)

add_executable(
        unit_tests
        binomial_heap_test.cpp
        main.cpp)

target_link_libraries(
        unit_tests
        gtest_main
        binomialHeap
)

add_test(
        NAME
        unit
        COMMAND
        ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR}/unit_tests
)
