
#include "FL/Enumerations.H"
#include "utils/paths.hpp"
#include "version.hpp"
#include <cstdio>
#include <unistd.h>
#include <pwd.h>

#include <SQLiteCpp/Database.h>
#include <sqlite3.h>

#ifdef VS_USE_QJS
#include <quickjs.h>
#endif 

#ifdef VS_USE_TCC
#include "subprojects/libtcc/config.h"
#endif 

#ifdef VS_USE_WAMR
#include "subprojects/wamr/core/version.h"
#endif 

#ifdef VS_USE_VM_RISCV
#include <libriscv_settings.h>

#define STRINGIFY_(x) #x
#define STRINGIFY(x)  STRINGIFY_(x)

constexpr const char* riscv_version(){return STRINGIFY(RISCV_VERSION_MAJOR) "." STRINGIFY(RISCV_VERSION_MINOR); }
#undef STRINGIFY 
#undef STRINGIFY_
#endif 

//#include <uv.h>
#ifdef VS_USE_NETWORKING_CURL
#include <curl/curl.h>
#endif

#include <globals.hpp>
#include <utils/env.hpp>

namespace vs{



//TODO: For now this is linux only. I will need to be expanded to support more os
path_env_t mk_env(global_ctx_t& globals, const char* arg0,const char* arg1){
  path_env_t main_env;
  static char buffer[1024];
  if(getcwd(buffer,1023)==nullptr){throw "Unable to get CWD";}

  { //Add a trailing /
    int i=0;
    for(;buffer[i]!=0 && i<1024-1;i++);
    buffer[i]='/';
    buffer[i+1]='\0';
  }

  main_env.cwd={rpath_type_t::FS,buffer};

  //TODO: At the moment only local files with this design
  {
    auto t = getenv("VS_COMMONS_DIR");
    if(t!=nullptr) main_env.root={rpath_type_t::FS,resolve_path::normalizer(t,"",true, true).second};
    else{
      main_env.root={rpath_type_t::FS,resolve_path::normalizer(VS_COMMONS_DIR,"",true, true).second};
    }
  }

  const char *homedir = "";
  if ((homedir = getenv("HOME")) == NULL) {
      homedir = getpwuid(getuid())->pw_dir;
  }
  std::string _homedir = homedir;

  //In theory homedir should have the `/` at the end, and the normalizer is not expecting this format with child having the trailing '/'. 
  main_env.packages_path = {rpath_type_t::FS,resolve_path::normalizer(_homedir.c_str(),"/.vs-fltk/packages",true).second + "/"};
  main_env.userdata_path = {rpath_type_t::FS,resolve_path::normalizer(_homedir.c_str(),"/.vs-fltk",true).second  + "/"};

  //TODO: add random subpath
  main_env.tmp_path={rpath_type_t::FS,"/tmp/"};

  //Finally compute path for the requested file
  resolve_path resolver(globals.env.computed_policies,main_env,main_env.cwd);
  {
    auto t = resolver(resolve_path::from_t::NATIVE_CODE,arg1);
    if(t.first!=resolve_path::reason_t::OK)exit(2); //TODO: Handle case
    else main_env.app_path=t.second;
  }
  return main_env;
}


js_rt_t::js_rt_t(){
  #if VS_USE_QJS
    auto tmp=JS_NewRuntime();
    //TODO define limits somewhere
    //JS_SetMemoryLimit(tmp, 80 * 1024);
    //JS_SetMaxStackSize(tmp, 10 * 1024);
    rt=tmp;
  #endif
}

js_rt_t::~js_rt_t(){
  #if VS_USE_QJS
    JS_FreeRuntime((JSRuntime*)rt);
  #endif
}

void* js_rt_t::operator()(){return rt;}


void prepare_db(){

    try
    {
        // Open a database file
        SQLite::Database    db("example.db3");
        
        // Compile a SQL query, containing one parameter (index 1)
        SQLite::Statement   query(db, "SELECT * FROM test WHERE size > ?");
        
        // Bind the integer value 6 to the first parameter of the SQL query
        query.bind(1, 6);
        
        // Loop to execute the query step by step, to get rows of result
        while (query.executeStep())
        {
            // Demonstrate how to get some typed column value
            int         id      = query.getColumn(0);
            const char* value   = query.getColumn(1);
            int         size    = query.getColumn(2);
            
            //std::cout << "row: " << id << ", " << value << ", " << size << std::endl;
        }
    }
    catch (std::exception& e)
    {
        //std::cout << "exception: " << e.what() << std::endl;
    }
}

#define str_helper(x) #x
#define str(x) str_helper(x)
#define WAMR_VERSION str(WAMR_VERSION_MAJOR) "." str(WAMR_VERSION_MINOR) "." str(WAMR_VERSION_PATCH)


versions_t get_versions(){
    versions_t tmp;
    tmp.vs=vs_version();
#   ifdef VS_USE_NETWORKING_CURL
        tmp.curl = curl_version();
#   else
        tmp.curl = "Not installed";
#   endif
    tmp.fltk = std::to_string(FL_API_VERSION);
    tmp.libuv = "Not installed";//uv_version_string();
    tmp.sqlite = sqlite3_libversion();
#   if VS_USE_TCC
      tmp.tcc = TCC_VERSION;
#   else
      tmp.tcc = "Not installed";
#   endif 
#   if VS_USE_QJS
      tmp.quickjs=JS_GetVersion();
#   else
      tmp.quickjs = "Not installed";
#   endif
#   if VS_USE_WAMR
      tmp.wamr= WAMR_VERSION;
#   else
      tmp.wamr = "Not installed";
#   endif
#   if VS_USE_VM_RISCV
      tmp.libriscv= riscv_version();
#   else
      tmp.libriscv = "Not installed";
#   endif
    return tmp;
}

#undef WAMR_VERSION
#undef str
#undef str_helper


}

