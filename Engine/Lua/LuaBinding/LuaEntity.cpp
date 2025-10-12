/**
  * @file           : Entity.cpp
  * @author         : Romi Brooks
  * @brief          :
  * @attention      :
  * @date           : 2025/10/12
  Copyright (c) 2025 Romi Brooks, All rights reserved.
**/

#include <iostream>
#include "../../Entities/Entity.hpp"
#include "../lua.hpp"

using Entity = engine::entity::Entity;
// 检查Lua栈中的userdata是否为Entity类型
#define CHECK_ENTITY(L) \
    Entity* entity = *static_cast<Entity**>(luaL_checkudata(L, 1, "EntityMetaTable")); \
    if (!entity) { \
        return luaL_error(L, "invalid Entity object"); \
    }

// Lua绑定：获取血量
static int lua_Entity_GetHP(lua_State* L) {
    CHECK_ENTITY(L);
    lua_pushinteger(L, entity->GetHP());
    return 1; // 返回1个值（血量）
}

// Lua绑定：设置血量（热修复可能需要动态调整血量）
static int lua_Entity_SetHP(lua_State* L) {
    CHECK_ENTITY(L);
    unsigned int hp = luaL_checkinteger(L, 2); // 第二个参数为新血量
    entity->SetBloody(hp);
    return 0; // 无返回值
}

// Lua绑定：受到伤害
static int lua_Entity_Damage(lua_State* L) {
    CHECK_ENTITY(L);
    unsigned int damage = luaL_checkinteger(L, 2);
    bool result = entity->Damage(damage);
    lua_pushboolean(L, result); // 返回是否成功造成伤害
    return 1;
}

// Lua绑定：攻击其他实体
static int lua_Entity_Attack(lua_State* L) {
    CHECK_ENTITY(L);
    // 第二个参数必须是另一个Entity对象
    Entity* target = *static_cast<Entity**>(luaL_checkudata(L, 2, "EntityMetaTable"));
    if (!target) {
        return luaL_error(L, "invalid target Entity");
    }
    bool result = entity->Attack(*target);
    lua_pushboolean(L, result);
    return 1;
}

// Lua绑定：设置位置
static int lua_Entity_SetPosition(lua_State* L) {
    CHECK_ENTITY(L);
    float x = luaL_checknumber(L, 2);
    float y = luaL_checknumber(L, 3);
    entity->SetPosition(x, y);
    return 0;
}

// Lua绑定：获取位置（返回Lua表 {x=..., y=...}）
static int lua_Entity_GetPosition(lua_State* L) {
    CHECK_ENTITY(L);
    sf::Vector2f pos = entity->GetPosition();
    lua_newtable(L); // 创建表
    lua_pushnumber(L, pos.x);
    lua_setfield(L, -2, "x"); // 表.x = pos.x
    lua_pushnumber(L, pos.y);
    lua_setfield(L, -2, "y"); // 表.y = pos.y
    return 1; // 返回位置表
}

// Lua绑定：移动（根据Movement信号）
static int lua_Entity_Move(lua_State* L) {
    CHECK_ENTITY(L);
    // 假设Movement是整数枚举（如0=上，1=下等）
    int signal = luaL_checkinteger(L, 2);
    entity->Move(static_cast<engine::config::movements::Movement>(signal));
    return 0;
}

// 注册Entity元表和方法到Lua环境
void registerEntityToLua(lua_State* L) {
    // 创建元表（用于标识Entity类型）
    luaL_newmetatable(L, "EntityMetaTable");

    // 元表的__index指向自身（方便调用方法）
    lua_pushvalue(L, -1);
    lua_setfield(L, -2, "__index");

    // 注册Entity的Lua方法
    const luaL_Reg entityMethods[] = {
        {"GetHP", lua_Entity_GetHP},
        {"SetHP", lua_Entity_SetHP},
        {"Damage", lua_Entity_Damage},
        {"Attack", lua_Entity_Attack},
        {"SetPosition", lua_Entity_SetPosition},
        {"GetPosition", lua_Entity_GetPosition},
        {"Move", lua_Entity_Move},
        {nullptr, nullptr} // 结束标记
    };
    luaL_setfuncs(L, entityMethods, 0);

    // 弹出元表（清理栈）
    lua_pop(L, 1);
}

// 将C++的Entity对象推入Lua环境（作为全局变量）
// 将C++的Entity对象推入Lua环境（实现）
// 将C++的Entity对象推入Lua环境（实现）
void pushEntityToLua(lua_State* L, Entity* entity, const std::string& luaVarName) {
	if (!L || !entity) return;

	// 1. 创建userdata存储Entity指针
	// 分配足够的内存存储Entity*
	Entity**udata = static_cast<Entity**>(lua_newuserdata(L, sizeof(Entity*)));
	*udata = entity; // 存储指针

	// 2. 绑定元表（确保Lua知道这是Entity类型）
	luaL_getmetatable(L, "EntityMetaTable");
	lua_setmetatable(L, -2);

	// 3. 将userdata设置为Lua全局变量
	lua_setglobal(L, luaVarName.c_str());
}
