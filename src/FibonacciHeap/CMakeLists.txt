add_library(fibbonachiHeap)

target_sources(
        fibbonachiHeap
        PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/FibbonachiHeap.hpp
)

target_include_directories(
        fibbonachiHeap
        PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}
)

set_target_properties(fibbonachiHeap PROPERTIES LINKER_LANGUAGE CXX)