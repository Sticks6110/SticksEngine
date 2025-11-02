add_library(Engine STATIC
        src/Renderer.cpp
)
target_include_directories(Engine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)