add_library(SRC STATIC
        src/Test.cpp
)

target_include_directories(SRC PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(SRC PRIVATE Engine)