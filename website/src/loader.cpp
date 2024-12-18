#include <iostream>

#ifdef HAS_CURL
#include <curl/curl.h>
#endif


#include <ui-tree.hpp>
#include <ui-tree.xml.hpp>
#include <loader.hpp>

//TODO:Remove

namespace vs {
app_loader::app_loader(global_ctx_t& globals, const char *profile, const char *path) {
  
#ifdef HAS_CURL
  curl_global_init(CURL_GLOBAL_ALL);
#endif
  pugi::xml_document doc;

  static constexpr const char *embedded_profile = R"(<root></root>)";

  std::string profile_path =
      profile != nullptr
          ? (globals.path_env.userdata_path.as_string() + profile + ".xml")
          : (globals.path_env.userdata_path.as_string() + "default.xml");
  pugi::xml_parse_result result = doc.load_file(profile_path.c_str());
  if (!result) {
    std::cout << "XML [" << profile_path
              << "] parsed with errors, attr value: ["
              << doc.child("node").attribute("attr").value() << "]\n";
    std::cout << "Error description: " << result.description() << "\n";
    std::cout << "Error offset: " << result.offset << "\n\n";
    std::cout << "Loading embedded profile instead\n\n";
  } else {
    doc.load_string(embedded_profile);
  }

  root = new ui_tree_xml(nullptr,nullptr,nullptr);
  root->globals=&globals;

  if (root->load(path, ui_tree::type_t::APP) != 0) {
    throw "Unable to process file";
  } else {
    root->build();
    // vs::theme_cute::apply();
  }
}

int app_loader::test() {
  return root->runtime_testsuite();
  // TODO
}

int app_loader::run() {
  root->globals->mem_storage.cleanup();
  root->cleanup();
  if (!root->globals->env.computed_policies.headless) {
    auto t = Fl::run();
    delete root;
    root = nullptr;
    return t;
  } else {
    // for(;;);
    return 0;
  }
}

app_loader::~app_loader() {
#ifdef HAS_CURL
  curl_global_cleanup();
#endif
  if (root != nullptr)
    delete root;
}

} // namespace vs