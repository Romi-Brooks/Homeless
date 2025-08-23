cmake_minimum_required(VERSION 3.31)
project(Homeless)

set(CMAKE_CXX_STANDARD 23)

# 使用相对路径指向项目内的SFML库
set(SFML_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Lib/SFML-3.0.0")
set(IMGUI_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Lib/ImGUI")
set(IMGUI_SFML_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Lib/ImGUI-SFML")

# 检查路径是否存在
if(NOT EXISTS ${SFML_DIR})
    message(FATAL_ERROR "SFML directory not found: ${SFML_DIR}")
endif()
if(NOT EXISTS ${IMGUI_DIR})
    message(FATAL_ERROR "IMGUI directory not found: ${IMGUI_DIR}")
endif()
if(NOT EXISTS ${IMGUI_SFML_DIR})
    message(FATAL_ERROR "ImGui-SFML directory not found: ${IMGUI_SFML_DIR}")
endif()

# 添加包含目录
include_directories(
        ${SFML_DIR}/include
        ${IMGUI_DIR}
        ${IMGUI_SFML_DIR}
)

# 添加库目录 - 使用绝对路径确保可靠性
link_directories("${SFML_DIR}/lib")

# 打印调试信息
message(STATUS "SFML include path: ${SFML_DIR}/include")
message(STATUS "SFML library path: ${SFML_DIR}/lib")
message(STATUS "ImGui include path: ${IMGUI_DIR}")
message(STATUS "ImGui-SFML include path: ${IMGUI_SFML_DIR}")

# 查找库文件的实际名称
file(GLOB SFML_LIBS "${SFML_DIR}/lib/*")
message(STATUS "Found SFML libraries: ${SFML_LIBS}")

add_executable(Homeless Homeless.cpp
        "${IMGUI_DIR}/imgui.cpp"
        "${IMGUI_DIR}/imgui_draw.cpp"
        "${IMGUI_DIR}/imgui_tables.cpp"
        "${IMGUI_DIR}/imgui_widgets.cpp"
        "${IMGUI_SFML_DIR}/imgui-SFML.cpp"

        Window/MainWindow.cpp
        Window/MainWindow.hpp

        components/Entity.cpp
        components/Entity.hpp
        components/NPC.cpp
        components/NPC.hpp
        components/Player.cpp
        components/Player.hpp

        Log/LogSystem.cpp
        Log/LogSystem.hpp
        Type/Type.hpp
        Engine/Movement.cpp
        Engine/Movement.hpp
        Engine/Attack.cpp
        Engine/Attack.hpp
)

# 链接SFML库 - 使用实际找到的库文件名
target_link_libraries(Homeless PRIVATE
        sfml-graphics-d
        sfml-window-d
        sfml-system-d
        sfml-audio-d
)

target_include_directories(Homeless PRIVATE
        ${IMGUI_DIR}
        ${IMGUI_SFML_DIR}
)

if(MINGW)
    target_link_libraries(Homeless PRIVATE -lstdc++exp)
endif()

if (WIN32)
    target_link_libraries(Homeless PRIVATE opengl32)
endif()

# 复制DLL文件到输出目录
file(GLOB SFML_DLLS "${SFML_DIR}/bin/*.dll")
if(SFML_DLLS)
    # 创建目录并复制DLL
    add_custom_command(TARGET Homeless POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_BINARY_DIR}/"
            COMMAND ${CMAKE_COMMAND} -E copy ${SFML_DLLS} "${CMAKE_BINARY_DIR}/"
            COMMENT "Copying SFML DLLs to output directory"
    )
    message(STATUS "SFML DLLs to copy: ${SFML_DLLS}")
else()
    message(WARNING "No DLLs found in ${SFML_DIR}/bin")
endif()