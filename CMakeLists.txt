cmake_minimum_required(VERSION 3.31)
project(Homeless)

set(CMAKE_CXX_STANDARD 23)

# set lib
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

# Add lib
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
        # Libs
        "${IMGUI_DIR}/imgui.cpp"
        "${IMGUI_DIR}/imgui_draw.cpp"
        "${IMGUI_DIR}/imgui_tables.cpp"
        "${IMGUI_DIR}/imgui_widgets.cpp"
        "${IMGUI_SFML_DIR}/imgui-SFML.cpp"

        # Temp: Window
        Stage/MainWindow.cpp
        Stage/MainWindow.hpp

        # Engine: Entities
        Engine/Entities/Entity.cpp
        Engine/Entities/Entity.hpp
        Engine/Entities/NPC.cpp
        Engine/Entities/NPC.hpp
        Engine/Entities/Player.cpp
        Engine/Entities/Player.hpp

        # Engine: Audio
        Engine/Audio/SFXManager.cpp
        Engine/Audio/SFXManager.hpp
        Engine/Audio/MusicFade.cpp
        Engine/Audio/MusicFade.hpp
        Engine/Audio/Music.cpp
        Engine/Audio/Music.hpp
        Engine/Audio/SFX.cpp
        Engine/Audio/SFX.hpp

        # Engine: Physics
        Engine/Physics/Edge/EdgeDetect.hpp

        # Engine: Actions
        Engine/Actions/Generation/Generate.cpp
        Engine/Actions/Generation/Generate.hpp

        # Engine: Toolkits
        Engine/Utilities/Random.cpp
        Engine/Utilities/Random.hpp

        # Engine: Window
        Engine/Windows/Manager.cpp
        Engine/Windows/Manager.hpp
        Engine/Windows/Windows.cpp
        Engine/Windows/Windows.hpp

        # Engine: Log
        Engine/Log/LogSystem.cpp
        Engine/Log/LogSystem.hpp

        # Config Toolkits
        Engine/Config/Entities/NPC.hpp
        Engine/Config/Entities/Player.hpp
        Engine/Config/Utilities/Transer.hpp
        Engine/Config/Events/MoveEvent.hpp
        Engine/Config/Audios/Music.hpp
        Engine/Audio/VolumeManager.cpp
        Engine/Audio/VolumeManager.hpp

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