#!/bin/env bun
/*
    Single script to perform all codegen task in one single step.
    Use `quick` as optional arg to avoid generating the meson file again.  
    This will speed up compilation considerably and is fine as long as you have not added or removed files in the schema folder.
*/

import { $ } from "bun"

/*
    Export the information from the package.json into a header file to be consumed during compilation.
    The json file is considered as authoritative for all metadata which is related to this project.
*/
import pkg from "@/package.json"

const version = (await Bun.file('./.version').text()).trim();
console.log(`Version is ${version}`);

// This code is now duplicated in the meson file, at some point deprecate this one in favour of the other by adding a simple wrapper.
// Split into [major, minor, rev] and optional [tag]
const [versionPart, tag = ''] = version.split('-');
const [major, minor, rev] = versionPart.split('.');

await Bun.write("./include/version.hpp",
`#pragma once
constexpr int vs_version_major=${major};
constexpr int vs_version_minor=${minor};
constexpr int vs_version_rev=${rev};
inline const char* vs_version_tag="${tag}";
inline const char* vs_version(){return "${version}";}
`);

// automatic codegen
import "./gen-widgets"
import "./gen-ns-strings"
import "./gen-policies"
import "./gen-enums"

// database construction
await $`bun drizzle-kit generate && bun drizzle-kit push`;