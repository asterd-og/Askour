#include <codegen/gens/string.h>
#include <codegen/gens/func.h>
#include <codegen/gen.h>
#include <codegen/utils.h>

namespace string {
    std::map<std::string, std::string> strs;
    //its probably not the best way to do this
    //but thats the only way i can think of rn

    int last_tstr=0;

    std::string _new(std::string value) {
        if (!exists(value)) {
            std::string old_fname=gen::get_current_func();
            std::string fname=utils::fmt("_$.TSTR%d", last_tstr);
            func::_new(fname.c_str(), true);
            gen::set_current_func(fname.c_str());

            gen::emit(utils::fmt("\t.string \"%s\"", value.c_str()));

            gen::set_current_func(old_fname);

            last_tstr++;

            strs[value]=fname;
        }
        return strs[value];
    }

    bool exists(std::string value) {
        return ( strs.find(value) != strs.end() );
    }
}