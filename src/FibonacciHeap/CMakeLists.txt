add_library(fibonacciHeap)

target_sources(
        fibonacciHeap
        PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/FibonacciHeap.hpp
)

target_include_directories(
        fibonacciHeap
        PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}
)

set_target_properties(fibonacciHeap PROPERTIES LINKER_LANGUAGE CXX)