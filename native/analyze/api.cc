// api.cc -- cbanalyze.dll's C interface (cbanalyze.h): an image handed over once, then questions.
#include "cbanalyze.h"
#include "call_graph.h"
#include "conventions.h"
#include "image.h"
#include "json.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>

using namespace cba;

namespace {

struct Reader {
    const unsigned char *p, *end;
    bool ok = true;
    uint64_t take(int n) {
        if (end - p < n) {
            ok = false;
            return 0;
        }
        uint64_t v = 0;
        for (int k = 0; k < n; k++) v |= (uint64_t)p[k] << (8 * k);
        p += n;
        return v;
    }
    int64_t i64() { return (int64_t)take(8); }
    uint32_t u32() { return (uint32_t)take(4); }
    int u8() { return (int)take(1); }
};

void put_opt_str(std::string &o, const std::string &s) {
    if (s.empty()) o += "null";
    else put_str(o, s);
}

void put_op(std::string &o, const Op &op) {
    o += "[\"";
    o += op.kind;
    o += "\",";
    if (op.kind == 'r') put_str(o, op.reg), o += ',', put_int(o, op.size);
    else if (op.kind == 'i') put_int(o, op.value), o += ',', put_int(o, op.size);
    else {
        put_opt_str(o, op.base), o += ',';
        put_opt_str(o, op.index), o += ',';
        put_int(o, op.scale), o += ',';
        put_int(o, op.value), o += ',';
        put_opt_str(o, op.seg), o += ',';
        put_int(o, op.size);
    }
    o += ']';
}

void put_regs(std::string &o, const std::set<std::string> &rs) {
    o += '[';
    bool first = true;
    for (auto &r : rs) {
        if (!first) o += ',';
        first = false;
        put_str(o, r);
    }
    o += ']';
}

void put_function(std::string &o, const Function &fn) {
    o += '[';
    put_int(o, fn.start), o += ',', put_int(o, fn.limit), o += ',', put_int(o, fn.end), o += ",[";
    for (size_t k = 0; k < fn.insns.size(); k++) {
        const Ins &i = fn.insns[k];
        if (k) o += ',';
        o += '[';
        put_int(o, i.addr), o += ',', put_int(o, i.size), o += ',';
        put_str(o, i.mn), o += ',', put_str(o, i.text), o += ",[";
        for (size_t j = 0; j < i.ops.size(); j++) {
            if (j) o += ',';
            put_op(o, i.ops[j]);
        }
        o += "],";
        put_regs(o, i.rd), o += ',', put_regs(o, i.wr), o += ',';
        put_int(o, i.flow), o += ',';
        if (i.has_target) put_int(o, i.target);
        else o += "null";
        o += ']';
    }
    o += "],[";
    for (size_t k = 0; k < fn.tables.size(); k++) {
        const Table &t = fn.tables[k];
        if (k) o += ',';
        o += "{\"site\":", put_int(o, t.site), o += ",\"table\":", put_int(o, t.table), o += ",\"targets\":[";
        for (size_t j = 0; j < t.targets.size(); j++) {
            if (j) o += ',';
            put_int(o, t.targets[j]);
        }
        o += "],\"low\":", put_int(o, t.low), o += ",\"selector\":", put_str(o, t.selector), o += ",\"default\":";
        if (t.has_default) put_int(o, t.def);
        else o += "null";
        o += ",\"bytes\":";
        if (t.has_bytes) {
            o += '[';
            for (size_t j = 0; j < t.bytes.size(); j++) {
                if (j) o += ',';
                put_int(o, t.bytes[j]);
            }
            o += ']';
        } else o += "null";
        o += ",\"bounded\":";
        o += t.bounded ? "true" : "false";
        o += '}';
    }
    o += "]]";
}

int answer(const std::string &json, char **out, size_t *len) {
    char *m = (char *)std::malloc(json.size() + 1);
    if (!m) return 0;
    std::memcpy(m, json.data(), json.size());
    m[json.size()] = 0;
    *out = m;
    *len = json.size();
    return 1;
}

}  // namespace

extern "C" int cba_version(void) { return CBA_VERSION; }

extern "C" int cba_init(const wchar_t *capstone_dll, char *err, int errlen) {
    if (CS.open) return 0;
    return load_capstone(capstone_dll, err, (size_t)errlen) ? 0 : 1;
}

extern "C" void *cba_open(const unsigned char *blob, size_t n, char *err, int errlen) {
    Reader r{blob, blob + n};
    auto *img = new Image();
    img->kind = (char)r.u8();
    img->bits = r.u8();
    img->flat = r.u8() != 0;
    img->has_pointers = r.u8() != 0;
    uint32_t ns = r.u32();
    for (uint32_t k = 0; k < ns && r.ok; k++) {
        Span s;
        s.va = r.i64();
        uint32_t len = r.u32();
        s.exec = r.u8() != 0;
        s.writable = r.u8() != 0;
        s.init = r.i64();
        if ((size_t)(r.end - r.p) < len) {
            r.ok = false;
            break;
        }
        s.data.assign((const char *)r.p, len);
        r.p += len;
        img->spans.push_back(std::move(s));
    }
    uint32_t nf = r.u32();
    for (uint32_t k = 0; k < nf && r.ok; k++) {
        int64_t start = r.i64(), limit = r.i64();
        img->functions.push_back(start);
        img->limits[start] = limit;
    }
    uint32_t ni = r.u32();
    for (uint32_t k = 0; k < ni && r.ok; k++) img->imports.insert(r.i64());
    uint32_t np = r.u32();
    for (uint32_t k = 0; k < np && r.ok; k++) {
        int64_t site = r.i64();
        Pointer p;
        p.kind = r.u8();
        p.has_target = r.u8() != 0;
        p.target = r.i64();
        img->pointer_list.emplace_back(site, p);
    }
    img->data_segment = r.i64();
    uint32_t nr = r.u32();
    for (uint32_t k = 0; k < nr && r.ok; k++) img->roots.push_back(r.i64());
    std::string why;
    if (!r.ok) why = "the image blob is short";
    else if (!img->open_decoder(why)) {
    }
    if (!why.empty()) {
        snprintf(err, (size_t)errlen, "%s", why.c_str());
        delete img;
        return nullptr;
    }
    img->finish();
    return img;
}

extern "C" int cba_ask(void *handle, const char *question, long long arg, char **out, size_t *len) {
    *out = nullptr;
    *len = 0;
    Image *img = (Image *)handle;
    std::string o;
    if (!std::strcmp(question, "image.discover")) {
        img->discover();
        o += "{\"discovered\":[";
        bool first = true;
        for (int64_t v : img->order) {
            if (!first) o += ',';
            first = false;
            put_int(o, v);
        }
        o += "],\"functions\":[";
        for (size_t k = 0; k < img->functions.size(); k++) {
            int64_t f = img->functions[k];
            if (k) o += ',';
            o += '[', put_int(o, f), o += ',', put_int(o, img->limits[f]), o += ']';
        }
        o += "],\"decoded\":[";
        first = true;
        for (auto &kv : img->decoded()) {
            if (!first) o += ',';
            first = false;
            put_function(o, *kv.second);
        }
        o += "]}";
    } else if (!std::strcmp(question, "conventions.functions")) {
        o = conventions(*img);
    } else if (!std::strcmp(question, "call_graph.build")) {
        o = call_graph(*img);
    } else if (!std::strcmp(question, "image.linear")) {   // arg: the address, and the count above bit 40
        put_function(o, img->linear((int64_t)(arg & 0xFFFFFFFFFFll), (int)(arg >> 40)));
    } else if (!std::strcmp(question, "image.function")) {
        put_function(o, img->function((int64_t)arg));
    } else return -1;
    return answer(o, out, len);
}

extern "C" int cba_names(void *handle, const unsigned char *blob, size_t n) {
    Image *img = (Image *)handle;
    Reader r{blob, blob + n};
    for (auto *m : {&img->name_text, &img->import_text}) {
        m->clear();
        uint32_t count = r.u32();
        for (uint32_t k = 0; k < count && r.ok; k++) {
            int64_t va = r.i64();
            uint32_t len = r.u32();
            if ((size_t)(r.end - r.p) < len) {
                r.ok = false;
                break;
            }
            (*m)[va].assign((const char *)r.p, len);
            r.p += len;
        }
    }
    return r.ok ? 0 : 1;
}

extern "C" void cba_free(char *answer) { std::free(answer); }

extern "C" void cba_close(void *handle) { delete (Image *)handle; }
