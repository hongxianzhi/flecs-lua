#ifndef FLECS_LUA_H
#define FLECS_LUA_H

#include <flecs-lua/bake_config.h>

#ifdef __cplusplus
extern "C" {
#endif


typedef struct ecs_lua_ctx
{
    lua_State *L;
    int flags;

    int internal;
    int error;
    ecs_world_t *world;
}ecs_lua_ctx;

typedef ecs_lua_ctx EcsLuaHost;

typedef struct FlecsLua
{
    ECS_DECLARE_COMPONENT(EcsLuaHost);
}FlecsLua;

FLECS_LUA_EXPORT
int ecs_lua_init(ecs_lua_ctx *ctx);

FLECS_LUA_EXPORT
void ecs_lua_exit(lua_State *L);

FLECS_LUA_EXPORT
void FlecsLuaImport(ecs_world_t *w);

#define FlecsLuaImportHandles(handles)\
    ECS_IMPORT_COMPONENT(handles, EcsLuaHost);\

/* Get world pointer from registry */
FLECS_LUA_EXPORT
ecs_world_t *ecs_lua_get_world(lua_State *L);

FLECS_LUA_EXPORT
int luaopen_ecs(lua_State *L);

#ifdef __cplusplus
}
#endif

#endif /* FLECS_LUA_H */