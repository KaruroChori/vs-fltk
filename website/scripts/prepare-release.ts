#!/bin/env bun
//TODO: Write the helper script to simplify the releasing process.
//For reference: @/docs/developers/releasing.md
/*
Stage I (Before tag in github)
- [ ] Make sure tests are all passing with the current commit (locally).
- [ ] Update the version number in `package.json` to the next **odd** revision.
- [ ] Clear content in `TODO.md`.
- [ ] Make sure `RELEASE.md` is up to date. Copy it in `./docs/releases/` using the version name as filename.
- [ ] Run the full codegen step.
- [ ] Commit & push.

Stage II (after tag in github)
- [ ] Update the version number in `meson.build` and `package.json` to the next **even** revision.
- [ ] Clear content in `RELEASE.md`
*/

//Just to trigger the normal workflows