// resources.cc -- which resources the code loads and which dialog controls it touches (was the
// core of codebreaker/analyze/resources.py: _uses, _stem, _identifier). What the module carries
// is read by the format readers in Python, which also decides whether each load is present.
#include "resources.h"

#include <cstring>
#include <unordered_map>

#include "call_graph.h"
#include "json.h"
#include "strings.h"

namespace cba {

namespace {

const std::pair<int, const char *> TYPES[] = {{1, "CURSOR"}, {2, "BITMAP"}, {3, "ICON"}, {4, "MENU"}, {5, "DIALOG"},
                                              {6, "STRING"}, {9, "ACCELERATOR"}, {10, "RCDATA"}, {12, "GROUP_CURSOR"},
                                              {14, "GROUP_ICON"}, {16, "VERSION"}};
const int NONE = -1;
// function -> (resource type, 32-bit: argument number; 16-bit: words back to the identifier, to its segment)
struct Loader { const char *name, *kind; int pos32, pos16, seg16; };
const Loader LOADERS[] = {{"loadstring", "STRING", 1, 3, NONE}, {"loadicon", "GROUP_ICON", 1, 0, 1},
                          {"loadcursor", "GROUP_CURSOR", 1, 0, 1}, {"loadbitmap", "BITMAP", 1, 0, 1},
                          {"loadmenu", "MENU", 1, 0, 1}, {"loadaccelerators", "ACCELERATOR", 1, 0, 1},
                          {"dialogbox", "DIALOG", 1, 3, 4}, {"createdialog", "DIALOG", 1, 3, 4},
                          {"dialogboxparam", "DIALOG", 1, 5, 6}, {"createdialogparam", "DIALOG", 1, 5, 6},
                          {"findresource", nullptr, 1, 2, 3}, {"loadimage", nullptr, 1, NONE, NONE}};
// function -> (32-bit: argument number of the control identifier; 16-bit: words back to it)
struct Control { const char *name; int pos32, pos16; };
const Control CONTROLS[] = {{"getdlgitem", 1, 0}, {"setdlgitemtext", 1, 2}, {"getdlgitemtext", 1, 3},
                            {"setdlgitemint", 1, 2}, {"getdlgitemint", 1, 3}, {"senddlgitemmessage", 1, 4},
                            {"checkdlgbutton", 1, 1}, {"isdlgbuttonchecked", 1, 0}};

const Loader *loader(const std::string &s) {
    for (auto &l : LOADERS)
        if (s == l.name) return &l;
    return nullptr;
}

const Control *control(const std::string &s) {
    for (auto &c : CONTROLS)
        if (s == c.name) return &c;
    return nullptr;
}

// _stem: the import's name, lower case, without its A/W; "" when no name here can be it
std::string stem(const std::string &api) {
    size_t bang = api.rfind('!');
    std::string low = bang == std::string::npos ? api : api.substr(bang + 1);
    for (char &c : low) {
        if ((unsigned char)c >= 0x80) return std::string();   // lower-cases into none of the tables
        if (c >= 'A' && c <= 'Z') c = (char)(c - 'A' + 'a');
    }
    if (low.empty()) return low;
    std::string cut = low.substr(0, low.size() - 1);
    if ((low.back() == 'a' || low.back() == 'w') && (loader(cut) || control(cut))) return cut;
    return low;
}

// an identifier: none, an integer, or a name (upper case)
struct Ident {
    int kind = 0;           // 0 none, 1 integer, 2 name
    int64_t value = 0;
    std::string name;
};

Ident as_name(const Image &img, bool has, int64_t va, bool use_text) {
    Ident r;
    if (!has || !use_text) return r;
    const Image::Str *s = in_table(img, va);
    if (!s) return r;
    r.kind = 2;
    r.name = s->text;
    for (char &c : r.name)
        if (c >= 'a' && c <= 'z') c = (char)(c - 'a' + 'A');
    return r;
}

Ident identifier(const Image &img, const std::vector<Arg> &args, int at, int seg, bool use_text) {
    Ident r;
    if (at == NONE || at >= (int)args.size() || args[at].kind != 'i') return r;
    int64_t value = args[at].value;
    if (img.bits == 32) {
        if (value < 0x10000) {
            r.kind = 1, r.value = value;
            return r;
        }
        return as_name(img, true, value, use_text);
    }
    if (seg == NONE) {
        r.kind = 1, r.value = value;
        return r;
    }
    if (seg >= (int)args.size()) return r;
    const Arg &s = args[seg];
    if (s.kind == 'i' && s.value == 0) {
        r.kind = 1, r.value = value;
        return r;
    }
    return as_name(img, s.kind == 'r' && s.reg == "ds", img.data_segment + value, use_text);
}

void put_ident(std::string &o, const Ident &i) {
    if (i.kind == 1) put_int(o, i.value);
    else if (i.kind == 2) put_str(o, i.name);
    else o += "null";
}

std::string uses_json(Image &img) {
    if (!img.graph_built) call_graph(img);
    ensure_table(img);
    std::string loads, controls;
    for (int64_t f : std::vector<int64_t>(img.functions)) {
        const Function &fn = img.function(f);
        std::unordered_map<int64_t, int> index;
        for (size_t k = 0; k < fn.insns.size(); k++) index.emplace(fn.insns[k].addr, (int)k);
        for (const Image::Call &c : img.calls[f]) {
            if (!c.has_dest || !img.imports.count(c.dest)) continue;
            auto name = img.import_text.find(c.dest);
            std::string st = stem(name == img.import_text.end() ? std::string() : name->second);
            const Loader *l = loader(st);
            const Control *ctl = control(st);
            if (!l && !ctl) continue;
            auto at = index.find(c.site);
            auto args = arguments(fn, at == index.end() ? 0 : at->second, 8);
            if (ctl) {
                Ident id = identifier(img, args, img.bits == 32 ? ctl->pos32 : ctl->pos16, NONE, false);
                if (id.kind == 1 && id.value < 0x10000) {
                    if (!controls.empty()) controls += ',';
                    controls += '[', put_int(controls, f), controls += ',', put_int(controls, c.site), controls += ',';
                    put_str(controls, st), controls += ',', put_int(controls, id.value), controls += ']';
                }
                continue;
            }
            Ident id = identifier(img, args, img.bits == 32 ? l->pos32 : l->pos16, l->seg16, true);
            if (!id.kind) continue;
            std::string kind = l->kind ? l->kind : "";
            bool has_kind = l->kind != nullptr;
            if (st == "findresource") {                           // the type is an argument too
                Ident t = identifier(img, args, img.bits == 32 ? 2 : 0, 1, true);
                has_kind = t.kind != 0;
                if (t.kind == 1) {
                    kind.clear();
                    for (auto &ty : TYPES)
                        if (ty.first == t.value) kind = ty.second;
                    if (kind.empty()) {
                        char b[32];
                        snprintf(b, sizeof b, "#%lld", (long long)t.value);
                        kind = b;
                    }
                } else kind = t.name;
            }
            if (!loads.empty()) loads += ',';
            loads += '[', put_int(loads, f), loads += ',', put_int(loads, c.site), loads += ',', put_str(loads, st);
            loads += ',';
            if (has_kind) put_str(loads, kind);
            else loads += "null";
            loads += ',', put_ident(loads, id), loads += ']';
        }
    }
    return "{\"loads\":[" + loads + "],\"controls\":[" + controls + "]}";
}

}  // namespace

bool resources_answer(Image &img, const char *what, std::string &out) {
    if (!std::strcmp(what, "uses")) out = uses_json(img);
    else return false;
    return true;
}

}  // namespace cba
