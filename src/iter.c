#include "private.h"


ecs_iter_t *ecs_lua__checkiter(lua_State *L, int arg)
{
    if(luaL_getmetafield(L, arg, "__ecs_iter") != LUA_TTABLE) luaL_argerror(L, arg, "table is not an iterator");

    lua_rawgeti(L, -1, 1);
    ecs_iter_t *it = lua_touserdata(L, -1);
    lua_pop(L, 2);

    return it;
}

ecs_iter_t *get_iter_columns(lua_State *L)
{
    ecs_world_t *w = ecs_lua_get_world(L);

    ecs_iter_t *it = ecs_lua__checkiter(L, 1);

    luaL_getsubtable(L, 1, "columns");

    return it;
}

int column(lua_State *L)
{
    ecs_iter_t *it = get_iter_columns(L);

    lua_Integer i = luaL_checkinteger(L, 2);

    if(i < 1 || i > it->column_count) luaL_argerror(L, 2, "invalid column index");

    lua_geti(L, -1, i);

    return 1;
}

int columns(lua_State *L)
{
    ecs_iter_t *it = get_iter_columns(L);

    int i;
    for(i=1; i <= it->column_count; i++)
    {
        lua_geti(L, 2, i);
    }

    return it->column_count;
}

int is_owned(lua_State *L)
{
    ecs_iter_t *it = ecs_lua__checkiter(L, 1);
    lua_Integer col = luaL_checkinteger(L, 2);

    int b = ecs_is_owned(it, col);

    lua_pushboolean(L, b);

    return 1;
}