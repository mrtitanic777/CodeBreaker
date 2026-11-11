// api.cc -- cbf_call: a question routed to its reader, the answer (or the exception) handed back.
#include "cbformats.h"

#include <exception>
#include <string>

#include "cb/core/error.hpp"
#include "core/questions.h"

namespace cbf {
std::map<std::string, Answer> &registry() {
    static std::map<std::string, Answer> r;
    return r;
}
}  // namespace cbf

namespace {
thread_local std::string answer;   // valid until this thread's next call

// The Python exception a cb::Error stands for (cb/compat/py_bridge.hpp's table): by meaning, never by message.
const char *python_kind(const cb::Error &e) {
    if (dynamic_cast<const cb::TruncatedError *>(&e)) return "struct.error";   // a short read
    if (dynamic_cast<const cb::NotFoundError *>(&e)) return "FileNotFoundError";
    if (dynamic_cast<const cb::AccessError *>(&e)) return "PermissionError";
    if (dynamic_cast<const cb::TimeoutError *>(&e)) return "TimeoutError";
    if (dynamic_cast<const cb::IoError *>(&e)) return "OSError";
    if (dynamic_cast<const cb::UnsupportedError *>(&e)) return "NotImplementedError";
    if (dynamic_cast<const cb::FormatError *>(&e)) return "ValueError";
    if (dynamic_cast<const cb::ParseError *>(&e)) return "ValueError";
    if (dynamic_cast<const cb::UsageError *>(&e)) return "ValueError";
    if (dynamic_cast<const cb::InternalError *>(&e)) return "AssertionError";
    return "RuntimeError";   // ToolError, EmulationError and the rest
}

int hand_back(int status, const std::string &s, const uint8_t **out, size_t *out_size) {
    answer = s;
    *out = reinterpret_cast<const uint8_t *>(answer.data());
    *out_size = answer.size();
    return status;
}
}  // namespace

extern "C" CBF_API int cbf_call(const char *question, const uint8_t *data, size_t size, const uint8_t *args,
                                size_t args_size, const uint8_t **out, size_t *out_size) {
    using namespace cbf;
    auto &r = registry();
    auto it = r.find(question ? question : "");
    if (it == r.end()) return hand_back(CBF_NO_SUCH, question ? question : "", out, out_size);
    Out o;
    try {
        Value a = read_pickle(args, args_size);
        Call call{Bytes(data, data ? size : 0), a, o};
        it->second(call);
        return hand_back(CBF_OK, o.finish(), out, out_size);
    } catch (const PyError &e) {
        Out err;
        err.begin_tuple();
        err.text(e.kind);
        err.text(e.message);
        err.end_tuple();
        return hand_back(CBF_RAISED, err.finish(), out, out_size);
    } catch (const Fallback &f) {
        return hand_back(CBF_FALLBACK, f.reason, out, out_size);
    } catch (const cb::Error &e) {               // a cb::formats reader: the Python exception that means the same
        Out err;
        err.begin_tuple();
        err.text(python_kind(e));
        err.text(e.what());
        err.end_tuple();
        return hand_back(CBF_RAISED, err.finish(), out, out_size);
    } catch (const std::bad_alloc &) {
        Out err;
        err.begin_tuple();
        err.text("MemoryError");
        err.text("");
        err.end_tuple();
        return hand_back(CBF_RAISED, err.finish(), out, out_size);
    } catch (const std::exception &e) {           // a fault of the C++ itself: raised, never hidden
        Out err;
        err.begin_tuple();
        err.text("RuntimeError");
        err.text(std::string("formats.dll: ") + e.what());
        err.end_tuple();
        return hand_back(CBF_RAISED, err.finish(), out, out_size);
    }
}

extern "C" CBF_API int cbf_version(void) { return CBF_VERSION; }

extern "C" CBF_API int cbf_unicorn(const wchar_t *, char *err, size_t errlen) {   // not used (cbformats.h)
    if (err && errlen) err[0] = 0;
    return 0;
}

extern "C" CBF_API const char *cbf_questions(void) {
    static std::string list;
    if (list.empty())
        for (auto &kv : cbf::registry()) list += kv.first + "\n";
    return list.c_str();
}
