// api.cc -- cbsymbols.dll's C interface (cbsymbols.h).
#include "cbsymbols.h"
#include "symbols.h"

#include <cstdlib>
#include <cstring>

extern "C" int cbs_version(void) { return CBS_VERSION; }

extern "C" int cbs_ask(const char* question, const unsigned* text, size_t n, char** answer, size_t* len) {
    std::u32string s(text, text + n);
    std::string json;
    bool ok;
    *answer = nullptr;
    *len = 0;
    if (!std::strcmp(question, "demangle.parse")) ok = cbs::demangle_parse(s, json);
    else if (!std::strcmp(question, "demangle.type_name")) ok = cbs::demangle_type_name(s, json);
    else if (!std::strcmp(question, "prototypes.build")) ok = cbs::prototypes_build(s, json);
    else return -1;
    if (!ok) return 0;
    char* out = (char*)std::malloc(json.size() + 1);
    if (!out) return 0;
    std::memcpy(out, json.data(), json.size());
    out[json.size()] = 0;
    *answer = out;
    *len = json.size();
    return 1;
}

extern "C" void cbs_free(char* answer) { std::free(answer); }
