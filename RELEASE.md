## v0.1.3-alpha

This second alpha release is still not usable in real-world scenarios, yet some simple and somewhat useful demos should be achievable.

The main objectives for this release are:

- To improve the build system, moving it to a separate repo.
- Complete the revised code-generation of widgets
- Better serialization/de-serialization of XML props & revised getters/setters
- To introduce a wider range of fltk widgets.
- Documentation efforts.

### What's new?

This is what has been implemented so far (and some missing feature for context):

- [ ] Improvements to the build system
  - [ ] migration of distribution-related features to [external repo]()
  - [ ] fix linking to follow https://github.com/KaruroChori/vs-fltk/issues/63
  - [ ] debian and arch packages are now supported
  - [ ] install script can be now be safely used
  - [x] specific features of `vs` can be excluded from building.
- [ ] New widgets & improvements
  - [ ] flex
  - [ ] grid
- [ ] Updated code-gen
  - [x] Use the new schema
- [ ] Updated core features
  - [x] Removed most of the globals & reworked singletons
  - [x] Reworked `ui_tree` interface to be more generic and not only viable for xml trees.
  - [x] Added guards to decide which features to compile `vs.fltk` with.
  - [ ] Support for automatic search of files for components, based on a priority list.
  - [ ] Linking external libraries and exposing symbols to `vs`
- [ ] Documentation!
  - [x] Landing page for the project
  - [x] `doxygen` for C++ code
  - [x] `mkdocs` to distribute the main documentation
  - [ ] Documenting:
    - [ ] settings
    - [ ] paths/virtual paths
    - [ ] some basic information to use vs
