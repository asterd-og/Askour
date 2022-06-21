#include <codegen/gens/func.h>
#include <codegen/gen.h>
#include <stdbool.h>

namespace func {
    std::map<std::string, bool> fs;

    void _new(std::string name, bool str) {
        fs[name] = str;
        gen::fasm[name] = "";
    }

    bool exists(std::string name) {
        return ( fs.find(name) != fs.end() );
    }

    bool is_str(std::string name) {
        return fs[name];
    }

    void def(parser::node node, int index, std::vector<parser::node> ast) {
        std::string name=node.middle.data;

        if (exists(name)) {
            gen::error("Function %s already exists", name.c_str());
        }

        _new(name, false);

        gen::set_current_func(name);

        gen::emit("\tpush rbp");
        gen::emit("\tmov rbp, rsp");

        if ( node.inner_nodes.size() > 0 ) {
            for ( int i = 0; i < node.inner_nodes.size(); i++ ) {
                gen::peek(i, node.inner_nodes);
            }
        }
    }
}