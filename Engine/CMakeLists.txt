find_package(glad CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(SDL3 CONFIG REQUIRED)
find_package(Stb REQUIRED)

add_library(Engine STATIC
        external/imgui/imgui.cpp
        external/imgui/imgui_demo.cpp
        external/imgui/imgui_draw.cpp
        external/imgui/imgui_impl_opengl3.cpp
        external/imgui/imgui_impl_sdl3.cpp
        external/imgui/imgui_tables.cpp
        external/imgui/imgui_widgets.cpp
        external/imgui/imgui_stdlib.h
        external/imgui/imgui_stdlib.cpp
        external/ImReflect.hpp
        src/Shader.cpp
        include/SticksEngine/Render/Shader.h
        src/STB.cpp
        src/Renderer.cpp
        include/SticksEngine/Render/Renderer.h
        include/SticksEngine/Behaviour.h
        include/SticksEngine/GameObject.h
        src/GameObject.cpp
        src/Scene.cpp
        include/SticksEngine/Scene.h
)

target_link_libraries(Engine PUBLIC glad::glad)
target_link_libraries(Engine PUBLIC glm::glm)
target_link_libraries(Engine PUBLIC SDL3::SDL3)

target_include_directories(Engine PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/external/imgui
        ${CMAKE_CURRENT_SOURCE_DIR}/external
        ${Stb_INCLUDE_DIR}
)