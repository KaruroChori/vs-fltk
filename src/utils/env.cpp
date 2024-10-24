
#include "FL/Enumerations.H"
#include "version.hpp"
#include <unistd.h>
#include <pwd.h>

#include <SQLiteCpp/Database.h>
#include <sqlite3.h>
#include <quickjs.h>
#include "subprojects/libtcc/config.h"
#include "subprojects/wamr/core/version.h"

#include <uv.h>
#ifdef HAS_CURL
#include <curl/curl.h>
#endif

#include <utils/env.hpp>

namespace vs{

policies_t global_policy;

//To be set in the main of the application of before any ui_tree facility is used.
path_env_t global_path_env;

//TODO: For now this is linux only. I will need to be expanded to support more os
path_env_t mk_env(const char* arg0,const char* arg1){
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

  if(arg0[0]=='/'){main_env.app_path={rpath_type_t::FS,resolve_path::normalizer("",arg0,false).second};main_env.app_path=main_env.app_path.base_dir();}
  else {main_env.app_path={rpath_type_t::FS,resolve_path::normalizer(buffer,arg0,true).second};main_env.app_path=main_env.app_path.base_dir();}

  //TODO: At the moment only local files with this design
  main_env.root={rpath_type_t::FS,resolve_path::normalizer(buffer,arg1,true, true).second};

  const char *homedir = "";
  if ((homedir = getenv("HOME")) == NULL) {
      homedir = getpwuid(getuid())->pw_dir;
  }
  std::string _homedir = homedir;

  //In theory homedir should have the `/` at the end, and the normalizer is not expecting this format with child having the trailing '/'. 
  main_env.packages_path = {rpath_type_t::FS,resolve_path::normalizer(_homedir.c_str(),"/.vs-fltk/packages",true).second + "/"};
  main_env.appdata_path = {rpath_type_t::FS,resolve_path::normalizer(_homedir.c_str(),"/.vs-fltk",true).second  + "/"};

  //TODO: add random subpath
  main_env.tmp_path={rpath_type_t::FS,"/tmp/"};

  return main_env;
}


js_rt_t::js_rt_t(){
    auto tmp=JS_NewRuntime();
    //TODO define limits somewhere
    //JS_SetMemoryLimit(tmp, 80 * 1024);
    //JS_SetMaxStackSize(tmp, 10 * 1024);
    rt=tmp;
}
js_rt_t::~js_rt_t(){JS_FreeRuntime((JSRuntime*)rt);}
void* js_rt_t::operator()(){return rt;}


js_rt_t global_js_rt;


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
#   ifdef HAS_CURL
        tmp.curl=curl_version();
#   else
        tmp.curl="Not installed";
#   endif
    tmp.fltk=std::to_string(FL_API_VERSION);
    tmp.libuv=uv_version_string();
    tmp.sqlite=sqlite3_libversion();
    tmp.tcc= TCC_VERSION;
    tmp.quickjs=JS_GetVersion();
    tmp.vs=vs_version();
    tmp.wamr= WAMR_VERSION;
    return tmp;
}

#undef WAMR_VERSION
#undef str
#undef str_helper

}

