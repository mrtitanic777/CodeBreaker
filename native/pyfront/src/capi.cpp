// capi.cpp -- the C ABI over pyfront::parse / pyfront::dump.  See pyfront/pyfront.h.
#include "pyfront/pyfront.h"
#include "pyfront/pyfront.hpp"

struct pyfront_tree {
    pyfront::ParseResult result;
    std::string dump_text;
    bool dumped = false;
    int dumped_attrs = -1;
};

extern "C" {

pyfront_tree* pyfront_parse(const char* source, size_t len, const char* filename) {
    auto* t = new pyfront_tree;
    t->result = pyfront::parse(std::string_view(source, len), filename ? filename : "<unknown>");
    return t;
}

int pyfront_ok(const pyfront_tree* t) { return t && t->result.module ? 1 : 0; }

const char* pyfront_dump(pyfront_tree* t, int include_attributes, size_t* len) {
    if (!t || !t->result.module) return nullptr;
    if (!t->dumped || t->dumped_attrs != include_attributes) {
        t->dump_text = pyfront::dump(*t->result.module, include_attributes != 0);
        t->dumped = true;
        t->dumped_attrs = include_attributes;
    }
    if (len) *len = t->dump_text.size();
    return t->dump_text.c_str();
}

const char* pyfront_error_msg(const pyfront_tree* t) { return t && t->result.error ? t->result.error->msg.c_str() : nullptr; }
int pyfront_error_lineno(const pyfront_tree* t) { return t && t->result.error ? t->result.error->lineno : 0; }
int pyfront_error_offset(const pyfront_tree* t) { return t && t->result.error ? t->result.error->offset : 0; }
int pyfront_error_end_lineno(const pyfront_tree* t) { return t && t->result.error ? t->result.error->end_lineno : 0; }
int pyfront_error_end_offset(const pyfront_tree* t) { return t && t->result.error ? t->result.error->end_offset : 0; }
void* pyfront_module(pyfront_tree* t) { return t ? t->result.module.get() : nullptr; }
void pyfront_free(pyfront_tree* t) { delete t; }

}
