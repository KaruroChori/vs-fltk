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
await Bun.write("./include/version.hpp",
    `#pragma once
constexpr int vs_version_major=${pkg.version.split('-')[0].split('.')[0]};
constexpr int vs_version_minor=${pkg.version.split('-')[0].split('.')[1]};
constexpr int vs_version_rev=${pkg.version.split('-')[0].split('.')[2]};
inline const char* vs_version_tag="${pkg.version.split('-')[1]}";
inline const char* vs_version(){return "${pkg.version}";}
`)

if (process.argv[2] != 'quick') {
    //Ideally I should run `meson rewrite kwargs set project / version ${version}` but the compiler detection breaks everything.
    const meson = await Bun.file('./meson.build').text();
    await Bun.write('./meson.build', meson.replace(/#<inject-version>[\s\S]*#<\/inject-version>/g,
        `#<inject-version>
    version: '${pkg.version}',
    #</inject-version>`
    ))
}

// Components automatic codegen
import "./gen-components"
import "./gen-ns-strings"
import "./gen-policies"


await $`bun drizzle-kit generate && bun drizzle-kit push`;