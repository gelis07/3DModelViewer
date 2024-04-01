cmake_minimum_required(VERSION 3.5)

project(Test)

# Set the C++ standard (change as needed)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add the executable target
add_executable(Test
    src/main.cpp
    src/Camera.cpp
    src/Mesh.cpp
    src/Model.cpp
    src/Material.cpp
    src/Light.cpp
    src/Editor/Editor.cpp
    src/Scenes.cpp
    src/GlAbst/Framebuffer.cpp
    src/GlAbst/Shader.cpp
    src/GlAbst/Texture.cpp
    src/GlAbst/VertexPackage.cpp
    include/imgui/imgui_draw.cpp
    include/imgui/imgui_impl_glfw.cpp
    include/imgui/imgui_stdlib.cpp
    include/imgui/imgui_impl_opengl3.cpp
    include/imgui/imgui.cpp
    include/imgui/imgui_demo.cpp
    include/imgui/imgui_tables.cpp
    include/imgui/imgui_widgets.cpp
)

# Add include directories
target_include_directories(Test PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include/
    ${CMAKE_CURRENT_SOURCE_DIR}/src/
)

# Add library directories
target_link_directories(Test PRIVATE
    ${CMAKE_SOURCE_DIR}/lib
)

# Add library dependencies
target_link_libraries(Test PRIVATE
    stdc++fs
    glfw3dll
    glew32s
    libassimp-5
    opengl32
)

# Set additional compiler options and flags
target_compile_options(Test PRIVATE
    -fdiagnostics-color=always
    -g
    -std=c++17
)

# Set the output directory for the executable
set_target_properties(Test PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin"
)

# Specify the CMake generator for GCC
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-dev")
endif()