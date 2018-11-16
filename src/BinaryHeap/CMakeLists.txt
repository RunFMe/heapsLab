add_library(binaryHeap)

target_sources(
        binaryHeap
    PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/BinaryHeap.hpp
        ${CMAKE_CURRENT_LIST_DIR}/Vector.hpp
)

target_include_directories(
        binaryHeap
    PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}
)

set_target_properties(binaryHeap PROPERTIES LINKER_LANGUAGE CXX)