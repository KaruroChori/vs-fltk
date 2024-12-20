#include <iostream>

#include <ui-tree.hpp>
#include <ui-tree.xml.hpp>
#include <loader.hpp>


namespace vs {
loader_t::loader_t(global_ctx_t& globals, const char *profile, const char *path) {

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

int loader_t::test() {
  return root->runtime_testsuite();
  // TODO
}

int loader_t::run() {
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

loader_t::~loader_t() {
  if (root != nullptr)
    delete root;
}

} // namespace vs