#include "private.h"

static ECS_COMPONENT_DECLARE(EcsLuaHost);

static const int ecs_lua__key;

#define ECS_LUA_CONTEXT (&ecs_lua__key)

#define ECS_LUA__KEEPOPEN 1

ecs_lua_ctx *ecs_lua_get_context(lua_State *L)
{
    lua_rawgetp(L, LUA_REGISTRYINDEX, ECS_LUA_CONTEXT);
    ecs_lua_ctx *p = lua_touserdata(L, -1);
    lua_pop(L, 1);
    return p;
}

ecs_world_t *ecs_lua_get_world(lua_State *L)
{
    ecs_lua_ctx *p = ecs_lua_get_context(L);
    return p->world;
}

void ecs_lua_progress(lua_State *L)
{
    ecs_lua__prolog(L);
    ecs_lua_ctx *ctx = ecs_lua_get_context(L);

    ecs_assert(ctx->progress_ref != LUA_NOREF, ECS_INVALID_PARAMETER, "progress callback is not set");

    if(ctx->progress_ref == LUA_NOREF) return;

    lua_rawgeti(L, LUA_REGISTRYINDEX, ctx->progress_ref);

    int type = lua_type(L, -1);
    ecs_assert(type == LUA_TFUNCTION, ECS_INTERNAL_ERROR, NULL);

    if(type != LUA_TFUNCTION) return;

    int ret = lua_pcall(L, 0, 0, 0);

    if(ret)
    {
        const char *err = lua_tostring(L, lua_gettop(L));
        ecs_os_err("progress() cb error (%d): %s", ret, err);
        lua_pop(L, 1);
    }

    ecs_assert(!ret, ECS_INTERNAL_ERROR, NULL);

    ecs_lua__epilog(L);
}


/* Entity */
int new_entity(lua_State *L);
int delete_entity(lua_State *L);
int new_tag(lua_State *L);
int entity_name(lua_State *L);
int entity_symbol(lua_State *L);
int entity_fullpath(lua_State *L);
int lookup_entity(lua_State *L);
int lookup_child(lua_State *L);
int lookup_path(lua_State *L);
int lookup_fullpath(lua_State *L);
int lookup_symbol(lua_State *L);
int use_alias(lua_State *L);
int entity_has(lua_State *L);
int has_role(lua_State *L);
int is_alive(lua_State *L);
int exists(lua_State *L);
int entity_add(lua_State *L);
int entity_remove(lua_State *L);
int clear_entity(lua_State *L);
int delete_children(lua_State *L);
int new_type(lua_State *L);
int get_type(lua_State *L);
int get_typeid(lua_State *L);
int get_parent(lua_State *L);

int add_trait(lua_State *L);
int remove_trait(lua_State *L);
int has_trait(lua_State *L);
int set_trait(lua_State *L);
int set_trait_tag(lua_State *L);
int get_trait(lua_State *L);
int get_trait_tag(lua_State *L);
int get_case(lua_State *L);

int new_enum(lua_State *L);
int new_bitmask(lua_State *L);
int new_array(lua_State *L);
int new_struct(lua_State *L);
int new_alias(lua_State *L);

int get_func(lua_State *L);
int get_mut(lua_State *L);

int mutable_modified(lua_State *L);
int set_func(lua_State *L);

int new_ref(lua_State *L);
int get_ref(lua_State *L);

int singleton_get(lua_State *L);
int singleton_get_mut(lua_State *L);
int singleton_modified(lua_State *L);
int singleton_set(lua_State *L);

int new_prefab(lua_State *L);

/* Hierarchy */
int get_child_count(lua_State *L);
int scope_iter(lua_State *L);
int scope_next(lua_State *L);
int set_scope(lua_State *L);
int get_scope(lua_State *L);
int set_name_prefix(lua_State *L);

/* Bulk */
int bulk_new(lua_State *L);
int bulk_delete(lua_State *L);

/* Iterator */
int column(lua_State *L);
int columns(lua_State *L);
int is_owned(lua_State *L);
int column_entity(lua_State *L);

/* Query */
int query_gc(lua_State *L);
int query_new(lua_State *L);
int subquery_new(lua_State *L);
int query_iter(lua_State *L);
int query_next(lua_State *L);
int query_changed(lua_State *L);
int each_func(lua_State *L);

/* Snapshot */
int snapshot_take(lua_State *L);
int snapshot_restore(lua_State *L);
int snapshot_iter(lua_State *L);
int snapshot_next(lua_State *L);
int snapshot_gc(lua_State *L);

/* System */
int new_system(lua_State *L);
int new_trigger(lua_State *L);
int run_system(lua_State *L);
int set_system_context(lua_State *L);

/* Module */
int new_module(lua_State *L);

/* Log */
int print_log(lua_State *L);
int print_err(lua_State *L);
int print_dbg(lua_State *L);
int print_warn(lua_State *L);
int tracing_enable(lua_State *L);

/* Misc */
int assert_func(lua_State *L);
int sizeof_component(lua_State *L);
int createtable(lua_State *L);

/* Time */
int get_time(lua_State *L);
int time_measure(lua_State *L);
int time__tostring(lua_State *L);

/* Timer */
int set_timeout(lua_State *L);
int get_timeout(lua_State *L);
int set_interval(lua_State *L);
int get_interval(lua_State *L);
int start_timer(lua_State *L);
int stop_timer(lua_State *L);
int set_rate_filter(lua_State *L);
int set_tick_source(lua_State *L);

/* Pipeline */
int new_pipeline(lua_State *L);
int set_pipeline(lua_State *L);
int get_pipeline(lua_State *L);
int progress(lua_State *L);
int progress_cb(lua_State *L);
int set_target_fps(lua_State *L);
int set_time_scale(lua_State *L);
int reset_clock(lua_State *L);
int lquit(lua_State *L);
int deactivate_systems(lua_State *L);
int set_threads(lua_State *L);
int get_threads(lua_State *L);
int get_thread_index(lua_State *L);

/* World */
int world_info(lua_State *L);
int world_stats(lua_State *L);
int dim(lua_State *L);
int dim_type(lua_State *L);

/* EmmyLua */
int emmy_class(lua_State *L);

static const luaL_Reg ecs_lib[] =
{
    { "new", new_entity },
    { "delete", delete_entity },
    { "tag", new_tag },
    { "name", entity_name },
    { "symbol", entity_symbol },
    { "fullpath", entity_fullpath },
    { "lookup", lookup_entity },
    { "lookup_child", lookup_child },
    { "lookup_path", lookup_path },
    { "lookup_fullpath", lookup_fullpath },
    { "lookup_symbol", lookup_symbol },
    { "use", use_alias },
    { "has", entity_has },
    { "has_role", has_role },
    { "is_alive", is_alive },
    { "exists", exists },
    { "add", entity_add },
    { "remove", entity_remove },
    { "clear", clear_entity },
    { "delete_children", delete_children },
    { "get_parent", get_parent },
    { "type", new_type },
    { "get_type", get_type },
    { "get_typeid", get_typeid },

    { "add_trait", add_trait },
    { "remove_trait", remove_trait },
    { "has_trait", has_trait },
    { "set_trait", set_trait },
    { "set_trait_tag", set_trait_tag },
    { "get_trait", get_trait },
    { "get_trait_tag", get_trait_tag },
    { "get_case", get_case },

    { "enum", new_enum },
    { "bitmask", new_bitmask},
    { "array", new_array },
    { "struct", new_struct },
    { "alias", new_alias },

    { "get", get_func },
    { "get_mut", get_mut },
    { "modified", mutable_modified },
    { "set", set_func },
    { "ref", new_ref },
    { "get_ref", get_ref },

    { "singleton_get", singleton_get },
    { "singleton_get_mut", singleton_get_mut },
    { "singleton_modified", singleton_modified },
    { "singleton_set", singleton_set },

    { "prefab", new_prefab },

    { "get_child_count", get_child_count },
    { "scope_iter", scope_iter },
    { "scope_next", scope_next },
    { "set_scope", set_scope },
    { "get_scope", get_scope },
    { "set_name_prefix", set_name_prefix },

    { "bulk_new", bulk_new },
    { "bulk_delete", bulk_delete },

    { "column", column },
    { "columns", columns },
    { "is_owned", is_owned },
    { "column_entity", column_entity },

    { "query", query_new },
    { "subquery", subquery_new },
    { "query_iter", query_iter },
    { "query_next", query_next },
    { "query_changed", query_changed },
    { "each", each_func },

    { "system", new_system },
    { "trigger", new_trigger },
    { "run", run_system },
    { "set_system_context", set_system_context },

    { "snapshot", snapshot_take },
    { "snapshot_restore", snapshot_restore },
    { "snapshot_iter", snapshot_iter },
    { "snapshot_next", snapshot_next },

    { "module", new_module },

    { "log", print_log },
    { "err", print_err },
    { "dbg", print_dbg },
    { "warn", print_warn },
    { "tracing_enable", tracing_enable },

    { "assert", assert_func },
    { "sizeof", sizeof_component },
    { "createtable", createtable },

    { "get_time", get_time },
    { "time_measure", time_measure },

    { "set_timeout", set_timeout },
    { "get_timeout", get_timeout },
    { "set_interval", set_interval },
    { "get_interval", get_interval },
    { "start_timer", start_timer },
    { "stop_timer", stop_timer },
    { "set_rate_filter", set_rate_filter },
    { "set_tick_source", set_tick_source },

    { "pipeline", new_pipeline },
    { "set_pipeline", set_pipeline },
    { "get_pipeline", get_pipeline },
    { "progress", progress },
    { "progress_cb", progress_cb },
    { "set_target_fps", set_target_fps },
    { "set_time_scale", set_time_scale },
    { "reset_clock", reset_clock },
    { "quit", lquit },
    { "deactivate_systems", deactivate_systems },
    { "set_threads", set_threads },
    { "get_threads", get_threads },
    { "get_thread_index", get_thread_index },

    { "world_info", world_info },
    { "world_stats", world_stats },
    { "dim", dim },
    { "dim_type", dim_type },

    { "emmy_class", emmy_class },

#define XX(const) {#const, NULL },
    ECS_LUA_ENUMS(XX)
    ECS_LUA_MACROS(XX)
#undef XX
    { NULL, NULL }
};

int luaopen_ecs(lua_State *L)
{
    luaL_checkversion(L);
    luaL_newlibtable(L, ecs_lib);
    lua_pushlightuserdata(L, ecs_lua_get_world(L));
    luaL_setfuncs(L, ecs_lib, 1);

    luaL_newmetatable(L, "ecs_type_t");
    lua_pop(L, 1);

    luaL_newmetatable(L, "ecs_ref_t");
    lua_pop(L, 1);

    luaL_newmetatable(L, "ecs_readonly");
    lua_pushcfunction(L, ecs_lua__readonly);
    lua_setfield(L, -2, "__newindex");
    lua_pushcfunction(L, ecs_lua__readonly);
    lua_setfield(L, -2, "__usedindex");
    lua_pushboolean(L, false);
    lua_setfield(L, -2, "__metatable");
    lua_pop(L, 1);

    luaL_newmetatable(L, "ecs_query_t");
    lua_pushcfunction(L, query_gc);
    lua_setfield(L, -2, "__gc");
    lua_pop(L, 1);

    luaL_newmetatable(L, "ecs_snapshot_t");
    lua_pushcfunction(L, snapshot_gc);
    lua_setfield(L, -2, "__gc");
    lua_pop(L, 1);

    luaL_newmetatable(L, "ecs_time_t");
    lua_pushcfunction(L, time__tostring);
    lua_setfield(L, -2, "__tostring");
    lua_pop(L, 1);

    lua_createtable(L, 128, 0);
    lua_rawsetp(L, LUA_REGISTRYINDEX, ECS_LUA_CURSORS);

    lua_createtable(L, 128, 0);
    lua_rawsetp(L, LUA_REGISTRYINDEX, ECS_LUA_TYPES);

#define XX(const) lua_pushinteger(L, Ecs##const); lua_setfield(L, -2, #const);
    ECS_LUA_ENUMS(XX)
#undef XX
#define XX(const) lua_pushinteger(L, ECS_##const); lua_setfield(L, -2, #const);
    ECS_LUA_MACROS(XX)
#undef XX
    return 1;
}

static ecs_lua_ctx *ctx_init(ecs_lua_ctx ctx)
{
    lua_State *L = ctx.L;
    ecs_world_t *world = ctx.world;

    ecs_lua_ctx *lctx = lua_newuserdata(L, sizeof(ecs_lua_ctx));
    lua_rawsetp(L, LUA_REGISTRYINDEX, ECS_LUA_CONTEXT);

    memcpy(lctx, &ctx, sizeof(ecs_lua_ctx));

    lctx->error = 0;
    lctx->progress_ref = LUA_NOREF;

    ecs_entity_t MetaTypeSerializer = ecs_lookup_fullpath(world, "flecs.meta.MetaTypeSerializer");
    ecs_assert(MetaTypeSerializer != 0, ECS_INTERNAL_ERROR, NULL);

    lctx->serializer_id = MetaTypeSerializer;

    lctx->metatype_id = ecs_lookup_fullpath(world, "flecs.meta.MetaTypeSerializer");
    ecs_assert(lctx->metatype_id != 0, ECS_INTERNAL_ERROR, NULL);

    lua_pushinteger(L, MetaTypeSerializer);
    lua_rawsetp(L, LUA_REGISTRYINDEX, ECS_LUA_SERIALIZER);

    luaL_requiref(L, "ecs", luaopen_ecs, 1);
    lua_pop(L, 1);

    return lctx;
}

static void ecs_lua_exit(lua_State *L)
{
    ecs_lua_ctx *ctx = ecs_lua_get_context(L);

    if( !(ctx->internal & ECS_LUA__KEEPOPEN) ) lua_close(L);
}

lua_State *ecs_lua_get_state(ecs_world_t *world)
{
    const EcsLuaHost *host = ecs_singleton_get(world, EcsLuaHost);

    ecs_assert(host != NULL, ECS_INTERNAL_ERROR, NULL);
    ecs_assert(host->L != NULL, ECS_INTERNAL_ERROR, NULL);

    return host->L;
}

int ecs_lua_set_state(ecs_world_t *world, lua_State *L)
{
    ecs_assert(L != NULL, ECS_INVALID_PARAMETER, NULL);

    EcsLuaHost *host = ecs_singleton_get_mut(world, EcsLuaHost);

    ecs_lua_exit(host->L);

    ecs_lua_ctx param = { .L = L, .world = world, .internal = ECS_LUA__KEEPOPEN };

    host->L = L;
    host->ctx = ctx_init(param);

    ecs_singleton_modified(world, EcsLuaHost);

    return 0;
}

static void *Allocf(void *ud, void *ptr, size_t osize, size_t nsize)
{
    if(!nsize)
    {
        ecs_os_free(ptr);
        return NULL;
    }

    return ecs_os_realloc(ptr, nsize);
}


/* Should only be called on ecs_fini() */
ECS_DTOR(EcsLuaHost, ptr,
{
    lua_close(ptr->L);
    ptr->L = NULL;
});

void FlecsLuaImport(ecs_world_t *w)
{
    ECS_MODULE(w, FlecsLua);

    ECS_IMPORT(w, FlecsMeta);

    ecs_set_name_prefix(w, "Ecs");

    ECS_COMPONENT_DEFINE(w, EcsLuaHost);

    ECS_META(w, EcsLuaWorldInfo);
    ECS_META(w, EcsLuaWorldStats);

    ECS_EXPORT_COMPONENT(EcsLuaHost);

    lua_State *L = lua_newstate(Allocf, NULL);

    ecs_lua_ctx param = { .L = L, .world = w};
    ecs_lua_ctx *ctx = ctx_init(param);

    ecs_singleton_set(w, EcsLuaHost, { .L = L, .ctx = ctx });

    ecs_set_component_actions(w, EcsLuaHost,
    {
        .dtor = ecs_dtor(EcsLuaHost)
    });
}
