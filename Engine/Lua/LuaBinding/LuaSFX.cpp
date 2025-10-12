/**
  * @file           : LuaSFX.cpp
  * @author         : Romi Brooks
  * @brief          :
  * @attention      :
  * @date           : 2025/10/12
  Copyright (c) 2025 Romi Brooks, All rights reserved.
**/

#include "../../Audio/Manager/SFXManager.hpp"
#include "../../Audio/SFX.hpp"
#include "lua.hpp"
#include <iostream>

#define GET_SFX() (engine::audio::SFX::GetInstance())

#define GET_SFX_MANAGER() (engine::audio::SFXManager::GetManager())

static int lua_SFX_Load(lua_State* L) {
    const char* sfx_id = luaL_checkstring(L, 1);
    const char* file_path = luaL_checkstring(L, 2);

    bool load_result = GET_SFX().Load(sfx_id, file_path);

    lua_pushboolean(L, load_result);
    return 1;
}

static int lua_SFX_Play(lua_State* L) {
    const char* sfx_id = luaL_checkstring(L, 1);
    GET_SFX().Play(sfx_id);
    return 0;
}

static int lua_SFX_Stop(lua_State* L) {
    const char* sfx_id = luaL_checkstring(L, 1);
    GET_SFX().Stop(sfx_id);
    return 0;
}

static int lua_SFX_StopAll(lua_State* L) {
    GET_SFX().StopAll();
    return 0;
}

static int lua_SFX_SetVolume(lua_State* L) {
    const char* sfx_id = luaL_checkstring(L, 1);
    float volume = static_cast<float>(luaL_checknumber(L, 2)); // 转换为float（Lua默认传double）

    GET_SFX().SetVolume(sfx_id, volume);
    return 0;
}

static int lua_SFX_SetGlobalVolume(lua_State* L) {
    float global_volume = static_cast<float>(luaL_checknumber(L, 1));
    engine::audio::SFX::SetGlobalVolume(global_volume); // 调用静态方法
    return 0;
}

static int lua_SFX_GetGlobalVolume(lua_State* L) {
    float current_volume = engine::audio::SFX::GetGlobalVolume();
    lua_pushnumber(L, current_volume);
    return 1;
}

static int lua_SFX_IsLoaded(lua_State* L) {
    const char* sfx_id = luaL_checkstring(L, 1);
    bool is_loaded = GET_SFX().IsLoaded(sfx_id);

    lua_pushboolean(L, is_loaded);
    return 1;
}

static int lua_SFX_Reset(lua_State* L) {
    GET_SFX().Reset();
    return 0;
}

static int lua_SFXManager_LoadSFXFiles(lua_State* L) {
    const char* buf_id = luaL_checkstring(L, 1);
    const char* file_path = luaL_checkstring(L, 2);

    bool load_result = GET_SFX_MANAGER().LoadSFXFiles(buf_id, file_path);
    lua_pushboolean(L, load_result);
    return 1;
}

static int lua_SFXManager_UnloadSFX(lua_State* L) {
    const char* buf_id = luaL_checkstring(L, 1);
    bool unload_result = GET_SFX_MANAGER().UnloadSFX(buf_id);

    lua_pushboolean(L, unload_result);
    return 1;
}

static int lua_SFXManager_UnloadAll(lua_State* L) {
    GET_SFX_MANAGER().UnloadAll();
    return 0;
}

static int lua_SFXManager_HasSFX(lua_State* L) {
    const char* buf_id = luaL_checkstring(L, 1);
    bool has_buf = GET_SFX_MANAGER().HasSFX(buf_id);

    lua_pushboolean(L, has_buf);
    return 1;
}

static int lua_SFXManager_GetLoadedCount(lua_State* L) {
    size_t count = GET_SFX_MANAGER().GetLoadedCount();
    lua_pushinteger(L, static_cast<lua_Integer>(count)); // 转换为Lua整数类型
    return 1;
}

void registerSFXToLua(lua_State* L) {
    lua_newtable(L);
    const luaL_Reg sfx_method_list[] = {
        {"Load",            lua_SFX_Load},
        {"Play",            lua_SFX_Play},
        {"Stop",            lua_SFX_Stop},
        {"StopAll",         lua_SFX_StopAll},
        {"SetVolume",       lua_SFX_SetVolume},
        {"SetGlobalVolume", lua_SFX_SetGlobalVolume},
        {"GetGlobalVolume", lua_SFX_GetGlobalVolume},
        {"IsLoaded",        lua_SFX_IsLoaded},
        {"Reset",           lua_SFX_Reset},
        {nullptr, nullptr}
    };

    luaL_setfuncs(L, sfx_method_list, 0);

    lua_setglobal(L, "SFX");

    lua_newtable(L);
    const luaL_Reg sfx_manager_method_list[] = {
        {"LoadSFXFiles",    lua_SFXManager_LoadSFXFiles},
        {"UnloadSFX",       lua_SFXManager_UnloadSFX},
        {"UnloadAll",       lua_SFXManager_UnloadAll},
        {"HasSFX",          lua_SFXManager_HasSFX},
        {"GetLoadedCount",  lua_SFXManager_GetLoadedCount},
        {nullptr, nullptr}
    };
    luaL_setfuncs(L, sfx_manager_method_list, 0);
    lua_setglobal(L, "SFXManager");

    std::cout << "[Lua Binding] SFX and SFXManager registered successfully." << std::endl;
}