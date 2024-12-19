import { elements, attributes } from '../../schemas/vs-ns.json'

function toSymbol(str: string) {
    return str.toUpperCase().replaceAll('.', '_').replaceAll('-', '_');
}

export const ns_strings_struct =
    `struct ns_strings{
    private:
        char* data = nullptr;
    
    public:
        //TAGS
${elements.map(x =>
        `        const char *${toSymbol(x)}_TAG;`
    ).join('\n')
    }

        //PROPS
${attributes.map(x =>
        `        const char *${toSymbol(x)}_PROP;`
    ).join('\n')
    }

        void prepare(const char * ns_prefix);
        inline ~ns_strings(){if(data!=nullptr)delete[] data;}
};`

//TODO: Write code to write strings

export const ns_strings_impl =
    `
void ui_tree_xml::ns_strings::prepare(const char * ns_prefix){
#   define WRITE(name,value) name=data+count;memcpy(data+count,ns_prefix,ns_prefix_len);memcpy(data+count+ns_prefix_len,value,std::char_traits<char>::length(value));data[count+ns_prefix_len+std::char_traits<char>::length(value)]=0;count+=ns_prefix_len+std::char_traits<char>::length(value)+1;
#   define STRLEN(str) ns_prefix_len+std::char_traits<char>::length(str)+1

    size_t ns_prefix_len=strlen(ns_prefix);

    if(data!=nullptr)delete []data;
    int count = 0;

    data = new char[
${
    //elements.reduce((total,v:string)=> total+v.length+1,0) + attributes.reduce((total,v:string)=> total+v.length+2,0)
    [...elements, ...attributes].map(x => `STRLEN("${x}")`).join('+')
    }];
${elements.map(x =>
        `    WRITE(${toSymbol(x)}_TAG,"${x}")`
    ).join('\n')
    }

${attributes.map(x =>
        `    WRITE(${toSymbol(x)}_PROP,"${x}")`
    ).join('\n')
    }
#   undef WRITE
#   undef STRLEN
}
`
await Bun.write('./include/ui-tree.xml.ns.autofrag.hpp', ns_strings_struct);
await Bun.write('./src/ui-tree.xml.ns.autofrag.cpp', ns_strings_impl);

//Generate ui-tree.xml.ns.autofrag.hpp and 