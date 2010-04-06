#include <stdio.h>
#include <glib.h>

#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"

#include "lua-plugin.h"


static const char * progname = "test-lua-plugin";
static lua_State * L = NULL;

static void l_message (const char *pname, const char *msg) {
  if (pname) fprintf(stderr, "%s: ", pname);
  fprintf(stderr, "%s\n", msg);
  fflush(stderr);
}

static int report (lua_State *L, int status) {
  if (status && !lua_isnil(L, -1)) {
    const char *msg = lua_tostring(L, -1);
    if (msg == NULL) msg = "(error object is not a string)";
    l_message(progname, msg);
    lua_pop(L, 1);
  }
  return status;
}

static int run_test(lua_State *L, const char * filename){
  int status = luaL_dofile(L, filename);
  fprintf(stderr, "%s done.\n", filename);
  return report(L, status);
}

int main(int argc, char * argv[]){
  printf("starting test...\n");

  /* initialize Lua */
  L = lua_open();

  lua_plugin_init(L);

  run_test(L, "test.lua");
  
  lua_plugin_fini(L);
  return 0;
}
