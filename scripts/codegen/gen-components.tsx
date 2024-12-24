/*
    Code generator for code and derived specs from json schemas.
    Use `quick` as optional arg to avoid generating the meson file again.  
    This will speed up compilation considerably and is fine as long as you have not added or removed files in the schema folder.
*/

import type { Static } from '@sinclair/typebox';
import { $ } from 'bun'
import { Glob } from "bun";
import { parse } from "node:path"
import { widget_schema, type_schema } from './components.schema';
import { Value } from '@sinclair/typebox/value';

import { render, JSXXML } from 'jsx-xml'


function make_type_code(type: Static<typeof type_schema>, subtype: string, code: string) {
    if (false) { }
    else if (type === 'flag') return `bool computed; if((ok=field_types::h_flag(&computed,value,that))){${code}}`
    else if (type === 'enum') return `int computed  = field_types::${subtype}_i(value);if((ok=(computed!=-1))){${code}}`
    else if (type === 'raw') return code;
    else if (type === 'path') return `/*TODO*/`
    else if (type === 'color') return `uint32_t computed; if((ok=field_types::h_colour(&computed,value,that))){${code}}`
    else if (type === 'string') return code;
    else if (type === 'scalar-1') return `size_t computed[1]; if((ok = field_types::h_px(1,computed,value,that))){${code}}`
    else if (type === 'scalar-2') return `size_t computed[2]; if((ok = field_types::h_px(2,computed,value,that))){${code}}`
    else if (type === 'scalar-4') return `size_t computed[4]; if((ok = field_types::h_px(4,computed,value,that))){${code}}`
}

function gen_cpp(data: Static<typeof widget_schema>) {
    const cextends = data.extends == null ? null : data.extends.split(':');
    const cppname = `${data.ns}_${data.name?.replaceAll('.', '_')}`;

    let class_decl = data.use_main_header === null ? `
#pragma once

#include <ui.hpp>
#include <ui-fields.hpp>
${cextends ? `#include <components/autogen/${cextends[0]}/${cextends[1]}.hpp>\n` : ``}
${data.headers ? data.headers.map(x => `#include <${x}>\n`).join('\n') : ``}

namespace vs{

class ${cppname} : public ${data.codegen.extends}{
    public:
        template<typename... Args>
        ${cppname}(ui_base* p,Args ...w):${data.codegen.extends}(p,w...){}
        virtual frame_type_t default_frame_type() override {return frame_type_t::LEAF;}
        virtual const char* class_name() override{return "${data.ns}:${data.name}";}

        virtual ~${cppname}(){}

        virtual int apply_prop(const char* prop, const char* value) override {return ${cppname}::_apply_prop(this,prop,value);}
        virtual int get_computed(const char* prop, const char ** value) override {return ${cppname}::_get_computed(this,prop,value);};

    public:
        static int _apply_prop(${cppname}* that, const char* prop, const char* value);
        static int _get_computed(${cppname}* that, const char* prop, const char** value);
};

}
    `:
        `#pragma once
#include <${data.use_main_header}>
`;

    let class_impl = `
#include <cstdint>
#include <components/autogen/${data.ns}/${data.name}.hpp>

namespace vs{

int ${cppname}::_apply_prop(${cppname}* that, const char* prop, const char* value){
    auto& w = that->widget();
    bool ok = true;
    if(false){}
    ${(data.skip_fields ?? []).map(x => {
        if (x[x.length - 1] == '*') return `else if (strncmp(prop, "${x}", ${x.length - 1})==0){}`
        else return `else if(strcmp(prop,"${x}")==0){}`
    }).join('\n\t')}
    ${Object.entries(data.fields).filter(x => x[1].setter != undefined).map(x => {
        x[1].alias = x[1].alias ?? []
        x[1].alias.push(x[0]);
        return x[1].alias.map(y => `
    ${x[1].description ? `//${x[1].description}` : ``}
    else if(strcmp(prop,"${y}")==0){
        ${make_type_code(x[1].type, x[1].subtype ?? "", x[1].setter ?? "")}
    }`).join('\n')
    }
    ).join('\n')
        }
    ${cextends === null ? `else {return 2;}\nreturn ok?0:1;` : `if(!ok) return 1; else return ${data.codegen.set_tail ?? `${cextends[0]}_${cextends[1]}`}::_apply_prop((${data.codegen.set_tail ?? `${cextends[0]}_${cextends[1]}`}*)that,prop,value);`}
}

int ${cppname}:: _get_computed(${cppname} * that, const char* prop, const char** value) {
    auto& w = that->widget();
    bool ok = true;
    if(false){}
    ${Object.entries(data.fields).filter(x => x[1].getter != undefined).map(x => {
            x[1].alias?.push(x[0]);
            return x[1].alias?.map(y => `
    ${x[1].description ? `//${x[1].description}` : ``}
    else if(strcmp(prop,"${y}")==0){
        ${x[1].getter}
    }`).join('\n')
        }
        ).join('\n')
        }
    ${cextends === null ? `else {return 2;}\nreturn ok?0:1;` : `if(!ok) return 1; else return ${data.codegen.get_tail ?? `${cextends[0]}_${cextends[1]}`}::_get_computed((${data.codegen.get_tail ?? `${cextends[0]}_${cextends[1]}`}*)that,prop,value);`}
}
    
}
`;

    let parser_selector =
        data.exposed != false ?
            data.type === 'leaf' ? `mkNSLeafWidget(${data.ns}, ${data.name}, ${cppname})` :
                data.type === 'node' ? `mkNSNodeWidget(${data.ns}, ${data.name}, ${cppname}) ` :
                    data.type === 'container' ? `mkNSContainerWidget(${data.ns}, ${data.name}, ${cppname})` :
                        data.type === 'slot' ? `mkNSSlotWidget(${data.ns}, ${data.name}, ${cppname})` :
                            data.type === 'slot-contaiener' ? `mkNSSlotContainerWidget(${data.ns}, ${data.name}, ${cppname})` :
                                `` : ``;
    return [class_decl, class_impl, parser_selector]
}


function gen_d_ts(data: Static<typeof widget_schema>) { }

function gen_xsd(data: Static<typeof widget_schema>) { }

function gen_xml_editor(data: Static<typeof widget_schema>) {
    return <component ns={data.ns} name={data.name} type={data.type} extends={data.extends} >
        <description>{data.description ?? "No description provided"}</description>
        <fields>{Object.entries(data.fields).map(x =>
            <item
                name={x[0]} alias={x[1].alias}
                type={x[1].type} subtype={x[1].subtype}
                semantic={x[1].semantic}
                is-prop={x[1].setter != null} is-computed={x[1].getter != null}
            >{x[1].description ?? 'No description provided'}</item>)}
        </fields>
    </component>
}

if (process.argv[2] != 'quick') {
    await $`rm -rf ./src/components/autogen/`
}
await $`mkdir -p ./src/components/autogen/`

await $`rm -rf ./include/components/autogen/`
await $`mkdir -p ./include/components/autogen/`

await $`rm -rf ./commons/schemas/components/`
await $`mkdir -p ./commons/schemas/components/`

//Save the schema, so that our json files can all be validated in the editor while writing them.
await Bun.write('./commons/schemas/json-component.schema.json', JSON.stringify(widget_schema, null, 4))

const glob = new Glob("./schemas/components/**/*.json");


const cpp_files = []
const h_files = []
const parser_entries = []

for await (const file of glob.scan(".")) {
    const tmp = Value.Default(widget_schema, (await Bun.file(file).json()))
    if (!Value.Check(widget_schema, tmp)) {
        console.warn(`[SKIP] ${file} since its syntax is not comformant`);
        for (const i of [...Value.Errors(widget_schema, tmp)]) {
            console.error(i.path, i.message);
        }
    }
    else {
        const namepieces = parse(file).name.split(':');
        tmp.ns ??= namepieces[0]
        tmp.name ??= namepieces[1]

        //Cpp code
        cpp_files.push(`./src/components/autogen/${tmp.ns}/${tmp.name}.cpp`)
        h_files.push(`components/autogen/${tmp.ns}/${tmp.name}.hpp`)
        {
            const ret = gen_cpp(tmp);

            await $`mkdir -p ./include/components/autogen/${tmp.ns}/`
            await Bun.write(`./include/components/autogen/${tmp.ns}/${tmp.name}.hpp`, ret[0])

            await $`mkdir -p ./src/components/autogen/${tmp.ns}/`
            await Bun.write(`./src/components/autogen/${tmp.ns}/${tmp.name}.cpp`, ret[1])
            parser_entries.push(ret[2])
        }

        //XML Model for the Editor
        {
            const ret = gen_xml_editor(tmp);
            await $`mkdir -p ./commons/schemas/components/${tmp.ns}/`
            await Bun.write(`./commons/schemas/components/${tmp.ns}/${tmp.name}.xml`, render(ret))
        }

        //XML Schema for aid when writing code
        //TODO:

        //Typescript types
        //TODO:

        console.log(`[DONE] ${parse(file).base}`)
    }
}

await Bun.write('./include/components/autogen/index.hpp', h_files.map(x => `#include <${x}>`).join('\n'))
await Bun.write('./src/ui.xml-widgets.autogen.cpp', parser_entries.join('\n'))

//TODO
await Bun.write('./include/cbindings/components.autogen.h', '')
//TODO
await Bun.write('./src/cbindings/components.autogen.cpp', '')

if (process.argv[2] != 'quick') {
    await Bun.write('./src/components/autogen/meson.build', `autogen_components = [${cpp_files.map(x => `'${x}'`)}]`)
}
