add_executable(Editor src/main.cpp
        ../src/src/Test.cpp
        ../src/include/Test.h
        src/EditorWindow.h
        src/Windows/SceneWindow.cpp
        src/Windows/SceneWindow.h
)
target_link_libraries(Editor PRIVATE Engine)
target_link_libraries(Editor PRIVATE SRC)

add_custom_command(TARGET Editor PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets
        $<TARGET_FILE_DIR:Editor>/assets
)