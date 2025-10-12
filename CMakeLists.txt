cmake_minimum_required(VERSION 3.31)
project(Homeless)

# ============================================================================
# 全局设置
# ============================================================================
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ============================================================================
# 目录设置
# ============================================================================
set(SFML_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Lib/SFML-3.0.0")
set(IMGUI_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Lib/ImGUI")
set(IMGUI_SFML_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Lib/ImGUI-SFML")
set(LUA_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Lib/Lua")
set(TAGLIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Lib/taglib")

# ============================================================================
# 路径检查
# ============================================================================
foreach(DIR IN ITEMS ${SFML_DIR} ${IMGUI_DIR} ${IMGUI_SFML_DIR} ${LUA_SRC_DIR} ${TAGLIB_DIR})
    if(NOT EXISTS ${DIR})
        message(FATAL_ERROR "Required directory not found: ${DIR}")
    endif()
endforeach()

# ============================================================================
# 第三方库配置
# ============================================================================
# 包含目录
include_directories(
        ${SFML_DIR}/include
        ${IMGUI_DIR}
        ${IMGUI_SFML_DIR}
        ${TAGLIB_DIR}/include
)

# 链接目录
link_directories(
        "${SFML_DIR}/lib"
        "${TAGLIB_DIR}/lib"
)

# 1.SFML 聚合库
    add_library(sfml_graphics INTERFACE)
    target_link_libraries(sfml_graphics INTERFACE sfml-graphics-d)
    target_include_directories(sfml_graphics INTERFACE ${SFML_DIR}/include)

    add_library(sfml_window INTERFACE)
    target_link_libraries(sfml_window INTERFACE sfml-window-d)
    target_include_directories(sfml_window INTERFACE ${SFML_DIR}/include)

    add_library(sfml_system INTERFACE)
    target_link_libraries(sfml_system INTERFACE sfml-system-d)
    target_include_directories(sfml_system INTERFACE ${SFML_DIR}/include)

    add_library(sfml_audio INTERFACE)
    target_link_libraries(sfml_audio INTERFACE sfml-audio-d)
    target_include_directories(sfml_audio INTERFACE ${SFML_DIR}/include)

    add_library(sfml_all INTERFACE)
    target_link_libraries(sfml_all INTERFACE
            sfml_graphics
            sfml_window
            sfml_system
            sfml_audio
    )

# 2.Lua 库
    file(GLOB LUA_SOURCES
            ${LUA_SRC_DIR}/*.c
    )
    list(FILTER LUA_SOURCES EXCLUDE REGEX ".*/lua\\.c$")
    list(FILTER LUA_SOURCES EXCLUDE REGEX ".*/luac\\.c$")

    add_library(lua_lib STATIC ${LUA_SOURCES})
    target_include_directories(lua_lib PUBLIC ${LUA_SRC_DIR})

# 3.ImGui 库
    add_library(imgui_lib STATIC
            "${IMGUI_DIR}/imgui.cpp"
            "${IMGUI_DIR}/imgui_draw.cpp"
            "${IMGUI_DIR}/imgui_tables.cpp"
            "${IMGUI_DIR}/imgui_widgets.cpp"
    )
    target_include_directories(imgui_lib PUBLIC ${IMGUI_DIR})

# 4.ImGui-SFML 库
    add_library(imgui_sfml_lib STATIC
            "${IMGUI_SFML_DIR}/imgui-SFML.cpp"
    )
    target_include_directories(imgui_sfml_lib PUBLIC ${IMGUI_SFML_DIR})
    target_link_libraries(imgui_sfml_lib PUBLIC imgui_lib sfml_all)

# 5.TagLib 库
    # 检查头文件目录
    set(TAGLIB_INCLUDE_DIR "${TAGLIB_DIR}/include")
    include_directories(${TAGLIB_INCLUDE_DIR})

    # Set .a static lib
    set(TAGLIB_LIB_FILE "${TAGLIB_DIR}/lib/libtag.a")

    # 定义静态库目标
    add_library(taglib STATIC IMPORTED GLOBAL)
    set_target_properties(taglib PROPERTIES
            IMPORTED_LOCATION "${TAGLIB_LIB_FILE}"
            INTERFACE_INCLUDE_DIRECTORIES "${TAGLIB_INCLUDE_DIR}"
            INTERFACE_COMPILE_DEFINITIONS "TAGLIB_STATIC"  # 强制定义静态库宏
    )

# ============================================================================
# 引擎库 For Engine
# ============================================================================

# 1. 日志库（先编译，其他库依赖它）
add_library(engine_log STATIC
        Engine/Log/LogSystem.cpp
        Engine/Log/LogSystem.hpp
)
target_include_directories(engine_log PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Engine)

# 2. 实体库
add_library(engine_entities STATIC
        Engine/Entities/Entity.cpp
        Engine/Entities/Entity.hpp
        Engine/Entities/NPC.cpp
        Engine/Entities/NPC.hpp
        Engine/Entities/Player.cpp
        Engine/Entities/Player.hpp
)
# 实体库暂无外部依赖，仅需头文件

# 3. 音频库（关键：链接sfml_audio和engine_log）
add_library(engine_audio STATIC
        Engine/Audio/Metadata/MetadataReader.cpp
        Engine/Audio/Metadata/MetadataReader.hpp
        Engine/Audio/Manager/SFXManager.cpp
        Engine/Audio/Manager/SFXManager.hpp
        Engine/Audio/Manager/VolumeManager.cpp
        Engine/Audio/Manager/VolumeManager.hpp
        Engine/Audio/Plug/MusicFade.cpp
        Engine/Audio/Plug/MusicFade.hpp
        Engine/Audio/Music.cpp
        Engine/Audio/Music.hpp
        Engine/Audio/SFX.cpp
        Engine/Audio/SFX.hpp
)
target_include_directories(engine_audio PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Engine)
# 音频库依赖：SFML音频模块 + 日志系统（SFX/Music用LOG_*函数）
target_link_libraries(engine_audio PUBLIC sfml_all engine_log)

# 4. 工具库
add_library(engine_utilities STATIC
        Engine/Utilities/Random/Random.cpp
        Engine/Utilities/Random/Random.hpp
        Engine/Utilities/Packager/Unpackage.cpp
        Engine/Utilities/Packager/Unpackage.hpp
        Engine/Utilities/Packager/Package.hpp
        Engine/Utilities/Packager/Package.cpp
)

# 5. 文件系统库
add_library(engine_filesystem STATIC
        Engine/FileSystem/Encoder.cpp
        Engine/FileSystem/Encoder.hpp
)

# 6. 窗口管理库（依赖SFML）
add_library(engine_windows STATIC
        Engine/Windows/RenderWindow.cpp
        Engine/Windows/RenderWindow.hpp
        Engine/Windows/Screen.cpp
        Engine/Windows/Screen.hpp
        Engine/Windows/Manager/ScreenManager.cpp
        Engine/Windows/Manager/ScreenManager.hpp
)
target_link_libraries(engine_windows PUBLIC sfml_all)  # 窗口依赖SFML

# 7. Lua绑定库（关键：链接engine_audio，获取SFX/SFXManager实现）
add_library(engine_lua STATIC
        Engine/Lua/LuaLoader.hpp
        Engine/Lua/LuaLoader.cpp
        Engine/Lua/LuaBinding/LuaMusic.cpp
        Engine/Lua/LuaBinding/LuaSFX.cpp
        Engine/Lua/LuaBinding/LuaEntity.cpp
)
target_include_directories(engine_lua PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Engine)
# Lua库依赖：基础lua_lib + 音频库（LuaSFX/LuaMusic调用engine_audio） + 实体库（LuaEntity调用）
target_link_libraries(engine_lua PUBLIC lua_lib engine_audio engine_entities)

# 8. 生成器库
add_library(engine_generation STATIC
        Engine/Actions/Generation/NPCGenerator.cpp
        Engine/Actions/Generation/NPCGenerator.hpp
        Engine/Actions/Generation/PlayerGenerator.cpp
        Engine/Actions/Generation/PlayerGenerator.hpp
)
target_link_libraries(engine_generation PUBLIC engine_entities)  # 生成实体依赖实体库

# 9. 主引擎库（聚合所有引擎组件，依赖顺序无关）
add_library(engine_core INTERFACE)
target_link_libraries(engine_core INTERFACE
        engine_entities
        engine_audio
        engine_utilities
        engine_filesystem
        engine_windows
        engine_log
        engine_lua
        engine_generation
)
# ============================================================================
# 游戏库 For Game
# ============================================================================
# 1. 舞台/场景库（依赖窗口库和ImGui）
add_library(game_stage STATIC
        Game/Stage/Debugger.cpp
        Game/Stage/Debugger.hpp
        Game/Stage/Start.cpp
        Game/Stage/Start.hpp
        Game/Stage/HUD.cpp
        Game/Stage/HUD.hpp
)
target_link_libraries(game_stage PUBLIC engine_windows imgui_sfml_lib)  # HUD可能用ImGui

# 2. NPC实体库（依赖引擎实体库）
add_library(game_npc STATIC
        Game/Entities/NPC/DeadBody.cpp
        Game/Entities/NPC/DeadBody.hpp
)
target_link_libraries(game_npc PUBLIC engine_entities)

# 3. 配置库（仅头文件，无需链接）
add_library(game_config INTERFACE)

# 4. 主游戏库（聚合游戏组件）
add_library(game_core INTERFACE)
target_link_libraries(game_core INTERFACE
        game_stage
        game_npc
        game_config
)

# ============================================================================
# 主可执行文件
# ============================================================================
add_executable(Homeless Homeless.cpp)

# 链接所有依赖（按“可执行文件 ← 游戏库 ← 引擎库 ← 第三方库”顺序）
target_link_libraries(Homeless PRIVATE
        game_core        # 游戏层
        engine_core      # 引擎层（自动包含engine_lua/engine_audio等）
        imgui_sfml_lib   # ImGui相关
        taglib           # 音频元数据
        sfml_all         # SFML（冗余但安全，确保未遗漏）
)

# Windows特定配置（修复MinGW和OpenGL链接）
if(MINGW)
    target_link_libraries(Homeless PRIVATE -lstdc++exp -lws2_32)  # MinGW额外依赖
endif()
if(WIN32)
    target_link_libraries(Homeless PRIVATE opengl32 gdi32)  # OpenGL和GDI依赖
endif()

# ============================================================================
# 平台特定配置
# ============================================================================
if(MINGW)
    target_link_libraries(Homeless PRIVATE -lstdc++exp)
endif()

if(WIN32)
    target_link_libraries(Homeless PRIVATE opengl32)
endif()

# ============================================================================
# 后期构建步骤
# ============================================================================
# For SFML
if(WIN32)
    set(SFML_BIN_DIR "${SFML_DIR}/bin")
    if(EXISTS ${SFML_BIN_DIR})
        file(GLOB SFML_DLLS "${SFML_BIN_DIR}/*.dll")
        if(SFML_DLLS)
            add_custom_command(TARGET Homeless POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:Homeless>"
                    COMMAND ${CMAKE_COMMAND} -E copy ${SFML_DLLS} "$<TARGET_FILE_DIR:Homeless>"
                    COMMENT "Copying SFML DLLs to output directory"
            )
            message(STATUS "SFML DLLs found: ${SFML_DLLS}")
        else()
            message(WARNING "No DLLs found in ${SFML_BIN_DIR}")
        endif()
    else()
        message(WARNING "SFML bin directory not found: ${SFML_BIN_DIR}")
    endif()
endif()

# ============================================================================
# 调试信息
# ============================================================================
message(STATUS "SFML include path: ${SFML_DIR}/include")
message(STATUS "SFML library path: ${SFML_DIR}/lib")
message(STATUS "ImGui include path: ${IMGUI_DIR}")
message(STATUS "ImGui-SFML include path: ${IMGUI_SFML_DIR}")
message(STATUS "Lua source path: ${LUA_SRC_DIR}")
message(STATUS "TagLib include path: ${TAGLIB_DIR}/include")
message(STATUS "TagLib library path: ${TAGLIB_DIR}/lib")

# 找到的源文件数量
file(GLOB_RECURSE ALL_SOURCES "*.cpp" "*.c" "*.hpp" "*.h")
list(LENGTH ALL_SOURCES TOTAL_SOURCES)
message(STATUS "Total source files: ${TOTAL_SOURCES}")