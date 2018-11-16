add_library(binomialHeap)

target_sources(
        binomialHeap
        PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/BinomialHeap.hpp
)

target_include_directories(
        binomialHeap
        PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}
)

set_target_properties(binomialHeap PROPERTIES LINKER_LANGUAGE CXX)