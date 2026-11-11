// codebreaker/formats/native_ask.hpp -- typed C++ of the questions codebreaker.formats.native asks formats.dll.
//
// The Python modules zx, cab, fonts, gfx, xnb, xsb, xbe, xdbf, xzp, xpr, xex_edit and ciso are their APIs over
// native.call / @native.answered: the reader is native/formats (formats.dll).  Here the same reader sources are linked
// in (native/formats/api.cc is the one entry point, cbf_call) and each question is answered in typed values by
// py::native (py_answers.hpp).  What the reader raises is raised as native._exception makes it: struct.error,
// the module's own exception (`own`: CabError, XprError ...) or the Python builtin of that name.
#pragma once

#include <cstdint>
#include <functional>
#include <string>
#include <type_traits>

#include "py.hpp"
#include "py_answers.hpp"

// the reader's one entry point (native/formats/api.cc, linked in)
extern "C" int cbf_call(const char* question, const uint8_t* data, size_t size, const uint8_t* args, size_t args_size,
                        const uint8_t** out, size_t* out_size);

namespace codebreaker::formats::native_ask {

// A module's own exception: throws it for `kind` and returns true, or returns false when `kind` is none of its own.
using Own = std::function<bool(const std::string& kind, const std::string& message)>;

inline const Own no_own = [](const std::string&, const std::string&) { return false; };

// native._exception(kind, message), raised
[[noreturn]] inline void raise_as(const std::string& name, std::string msg, const Own& own) {
    if (name == "struct.error") throw py::StructError(msg);
    if (own(name, msg)) throw py::RuntimeError(msg);   // (own throws; not reached)
    if (name == "UnicodeDecodeError") throw py::UnicodeDecodeError("'ascii' codec can't decode bytes in position 0-0: " + msg);
    if (name == "UnicodeEncodeError") throw py::UnicodeEncodeError("'latin-1' codec can't encode characters in position 0-0: " + msg);
    if (name == "KeyError") {   // KeyError(message.strip("'")): str() of it is the repr
        size_t a = msg.find_first_not_of('\'');
        size_t b = msg.find_last_not_of('\'');
        msg = a == std::string::npos ? std::string() : msg.substr(a, b - a + 1);
        throw py::KeyError(py::repr(py::str(msg)).utf8());
    }
    if (name == "ValueError") throw py::ValueError(msg);
    if (name == "IndexError") throw py::IndexError(msg);
    if (name == "TypeError") throw py::TypeError(msg);
    if (name == "MemoryError") throw py::MemoryError(msg);
    if (name == "ZeroDivisionError") throw py::ZeroDivisionError(msg);
    if (name == "OverflowError") throw py::OverflowError(msg);
    if (name == "AttributeError") throw py::AttributeError(msg);
    if (name == "OSError") throw py::OSError(msg);
    if (name == "FileNotFoundError") throw py::FileNotFoundError(msg);
    if (name == "PermissionError") throw py::PermissionError(msg);
    if (name == "AssertionError") throw py::AssertionError(msg);
    if (name == "RecursionError") throw py::RecursionError(msg);
    throw py::RuntimeError(msg);
}

// native.call(question, data, *args): the answer as it came (a pickle's value), for a caller that reads it by its shape
template <class... A>
py::native::Val ask_val(const char* question, const py::bytes& data, const Own& own, const A&... args) {
    namespace nat = py::native;
    nat::Args a;
    (a.put(args), ...);
    const std::string blob = a.done();
    const uint8_t* out = nullptr;
    size_t n = 0;
    const std::string& raw = data.raw();
    int status = ::cbf_call(question, reinterpret_cast<const uint8_t*>(raw.data()), raw.size(),
                            reinterpret_cast<const uint8_t*>(blob.data()), blob.size(), &out, &n);
    if (status != 0 && status != 1) throw py::RuntimeError(std::string("formats.dll does not answer ") + question);
    nat::Val v = nat::read_pickle(out, n);
    if (status == 1) {
        std::string name = v.items.size() > 0 ? v.items[0].s : "RuntimeError", msg = v.items.size() > 1 ? v.items[1].s : "";
        raise_as(name, msg, own);
    }
    return v;
}

// native.call(question, data, *args): the answer read as R
template <class R, class... A>
R ask(const char* question, const py::bytes& data, const Own& own, const A&... args) {
    py::native::Val v = ask_val(question, data, own, args...);
    if constexpr (std::is_void_v<R>) return;
    else return py::native::read<R>(v);
}

// a module's exception of `Type` for the reader's name `kind`
template <class Type> Own raises(const char* kind) {
    return [kind](const std::string& k, const std::string& m) -> bool {
        if (k != kind) return false;
        throw Type(m);
    };
}

}  // namespace codebreaker::formats::native_ask
