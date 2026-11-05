// exc.cc -- Python's exceptions: the builtin hierarchy, args / str / repr as CPython prints them,
// OSError's errno and filename, KeyError's repr, StopIteration's value, SystemExit's code, the
// context chaining an except block gives (__context__), raise from (__cause__).
#include <cerrno>
#include <cstring>

#include "rt.hpp"

namespace py {

TypeObj *E_BaseException, *E_Exception, *E_TypeError, *E_ValueError, *E_KeyError, *E_IndexError,
    *E_AttributeError, *E_StopIteration, *E_ZeroDivisionError, *E_OverflowError, *E_NameError,
    *E_UnboundLocalError, *E_RuntimeError, *E_NotImplementedError, *E_AssertionError, *E_OSError,
    *E_FileNotFoundError, *E_FileExistsError, *E_PermissionError, *E_IsADirectoryError,
    *E_NotADirectoryError, *E_ImportError, *E_ModuleNotFoundError, *E_UnicodeDecodeError,
    *E_UnicodeEncodeError, *E_LookupError, *E_GeneratorExit, *E_SystemExit, *E_RecursionError,
    *E_MemoryError, *E_TimeoutError, *E_EOFError, *E_KeyboardInterrupt, *E_UnicodeError, *E_SyntaxError;

static thread_local std::vector<var>* t_handling = nullptr;   // exceptions being handled (innermost last)

static std::vector<var>& handling_stack() {
    if (!t_handling) t_handling = new std::vector<var>();
    return *t_handling;
}

var make_exc_args(TypeObj* cls, std::vector<var> args) {
    CallArgs a;
    a.pos = std::move(args);
    return call_type(cls, a);
}
var make_exc(TypeObj* cls, const std::string& msg) { return make_exc_args(cls, {new_str(msg)}); }

static void set_context(const var& exc) {
    auto& st = handling_stack();
    if (st.empty() || !exc.is_ref()) return;
    auto* e = static_cast<ExcObj*>(exc.ref());
    if (e->context.is_unbound() && !is(st.back(), exc)) e->context = st.back();
}

std::string call_stack_();
[[noreturn]] void raise(const var& x) {
    static bool debug = getenv("PYRT_DEBUG") != nullptr;
    if (debug) {
        std::string s;
        try { s = repr_(x); } catch (...) {}
        fprintf(stderr, "pyrt: raise %.300s\n%s", s.c_str(), call_stack_().c_str());
    }
    var e = x;
    if (x.is_ref() && x.ref()->type->layout == Layout::Type) {
        TypeObj* t = static_cast<TypeObj*>(x.ref());
        if (!is_subtype(t, E_BaseException)) raise(E_TypeError, "exceptions must derive from BaseException");
        e = call_type(t, CallArgs());
    } else if (!isinstance_(x, E_BaseException)) {
        raise(E_TypeError, "exceptions must derive from BaseException");
    }
    set_context(e);
    throw error(e);
}
[[noreturn]] void raise_from(const var& x, const var& cause) {
    var e = x;
    if (x.is_ref() && x.ref()->type->layout == Layout::Type) e = call_type(static_cast<TypeObj*>(x.ref()), CallArgs());
    var c = cause;
    if (c.is_ref() && c.ref()->type->layout == Layout::Type) c = call_type(static_cast<TypeObj*>(c.ref()), CallArgs());
    auto* eo = static_cast<ExcObj*>(e.ref());
    eo->cause = c;
    eo->suppress_context = true;
    set_context(e);
    throw error(e);
}
std::string call_stack_();
[[noreturn]] void raise(TypeObj* cls, const std::string& msg) {
    static bool debug = getenv("PYRT_DEBUG") != nullptr;
    if (debug) fprintf(stderr, "pyrt: %s: %s\n%s", cls->name.c_str(), msg.c_str(), call_stack_().c_str());
    var e = make_exc(cls, msg);
    set_context(e);
    throw error(e);
}
[[noreturn]] void raise_type(const char* fmt_msg, const var& a) {
    std::string m = fmt_msg;
    auto p = m.find("%s");
    if (p != std::string::npos) m.replace(p, 2, tname(a));
    raise(E_TypeError, m);
}
[[noreturn]] void raise_stop(const var& value) {
    var e = value.is_unbound() || value.is_none() ? make_exc_args(E_StopIteration, {}) : make_exc_args(E_StopIteration, {value});
    throw error(e);
}
[[noreturn]] void reraise() {
    auto& st = handling_stack();
    if (st.empty()) raise(E_RuntimeError, "No active exception to reraise");
    throw error(st.back());
}
var current_exception() {
    auto& st = handling_stack();
    return st.empty() ? var(None) : st.back();
}
handling_::handling_(const error& e) { handling_stack().push_back(e.value); }
handling_::~handling_() { handling_stack().pop_back(); }

void assert_(const var& test, const var& msg) {
    if (test) return;
    if (msg.is_none() || msg.is_unbound()) throw error(make_exc_args(E_AssertionError, {}));
    throw error(make_exc_args(E_AssertionError, {msg}));
}
[[noreturn]] void name_error(const char* name) { raise(E_NameError, std::string("name '") + name + "' is not defined"); }

const char* error::what() const noexcept {
    try {
        what_ = tname(value) + ": " + str_(value);
    } catch (...) {
        what_ = "exception";
    }
    return what_.c_str();
}
bool error::matches(const var& cls) const {
    if (cls.is_ref() && cls.ref()->type->layout == Layout::Type) return isinstance_(value, static_cast<TypeObj*>(cls.ref()));
    if (is_tuple(cls)) {
        for (auto& c : tuple_items(cls))
            if (matches(c)) return true;
        return false;
    }
    raise(E_TypeError, "catching classes that do not inherit from BaseException is not allowed");
}

// ------------------------------------------------------------------------------------------ the types
static var exc_new(const CallArgs& a) {
    TypeObj* cls = T(a.pos.at(0));
    auto* e = new ExcObj();
    e->type = cls;
    incref(cls);
    std::vector<var> args(a.pos.begin() + 1, a.pos.end());
    e->args = new_tuple(args);
    return var(static_cast<Obj*>(e));
}
static var exc_init(const CallArgs& a) {
    auto* e = static_cast<ExcObj*>(a.pos[0].ref());
    std::vector<var> args(a.pos.begin() + 1, a.pos.end());
    e->args = new_tuple(args);
    return None;
}
static std::string exc_str(const var& self) {
    auto* e = static_cast<ExcObj*>(self.ref());
    const auto& args = tuple_items(e->args);
    if (args.empty()) return "";
    if (args.size() == 1) return str_(args[0]);
    return repr_(e->args);
}

static void exc_dealloc_guard() {}

void init_exceptions() {
    E_BaseException = new_type("BaseException", {T_object}, Layout::Exception);
    E_BaseException->is_exception = true;
    add_static(E_BaseException, "__new__", exc_new);
    add_method(E_BaseException, "__init__", exc_init);
    add_method(E_BaseException, "__str__", [](const CallArgs& a) { return new_str(exc_str(a.pos[0])); });
    add_method(E_BaseException, "__repr__", [](const CallArgs& a) {
        auto* e = static_cast<ExcObj*>(a.pos[0].ref());
        const auto& args = tuple_items(e->args);
        std::string r = type_of(a.pos[0])->name + "(";
        for (size_t i = 0; i < args.size(); i++) r += (i ? ", " : "") + repr_(args[i]);
        return new_str(r + ")");
    });
    add_method(E_BaseException, "with_traceback", [](const CallArgs& a) { return a.pos[0]; });
    add_method(E_BaseException, "add_note", [](const CallArgs& a) {
        var notes = getattr(a.pos[0], interned("__notes__"), None);
        if (notes.is_none()) { notes = new_list(); a.pos[0].setattr("__notes__", notes); }
        notes.call("append", a.pos.at(1));
        return None;
    });
    auto mk = [](const char* n, TypeObj* base) { return new_type(n, {base}, Layout::Exception); };
    E_Exception = mk("Exception", E_BaseException);
    E_SystemExit = mk("SystemExit", E_BaseException);
    add_getter(E_SystemExit, "code", [](const var& s) {
        auto& args = tuple_items(static_cast<ExcObj*>(s.ref())->args);
        return args.empty() ? var(None) : args.size() == 1 ? args[0] : static_cast<ExcObj*>(s.ref())->args;
    });
    E_KeyboardInterrupt = mk("KeyboardInterrupt", E_BaseException);
    E_GeneratorExit = mk("GeneratorExit", E_BaseException);
    E_StopIteration = mk("StopIteration", E_Exception);
    add_getter(E_StopIteration, "value", [](const var& s) {
        auto& args = tuple_items(static_cast<ExcObj*>(s.ref())->args);
        return args.empty() ? var(None) : args[0];
    });
    TypeObj* E_ArithmeticError = mk("ArithmeticError", E_Exception);
    E_ZeroDivisionError = mk("ZeroDivisionError", E_ArithmeticError);
    E_OverflowError = mk("OverflowError", E_ArithmeticError);
    TypeObj* E_FloatingPointError = mk("FloatingPointError", E_ArithmeticError);
    E_AssertionError = mk("AssertionError", E_Exception);
    E_AttributeError = mk("AttributeError", E_Exception);
    TypeObj* E_BufferError = mk("BufferError", E_Exception);
    E_EOFError = mk("EOFError", E_Exception);
    E_ImportError = mk("ImportError", E_Exception);
    E_ModuleNotFoundError = mk("ModuleNotFoundError", E_ImportError);
    E_LookupError = mk("LookupError", E_Exception);
    E_IndexError = mk("IndexError", E_LookupError);
    E_KeyError = mk("KeyError", E_LookupError);
    add_method(E_KeyError, "__str__", [](const CallArgs& a) {
        auto& args = tuple_items(static_cast<ExcObj*>(a.pos[0].ref())->args);
        if (args.size() == 1) return new_str(repr_(args[0]));
        return new_str(exc_str(a.pos[0]));
    });
    E_MemoryError = mk("MemoryError", E_Exception);
    E_NameError = mk("NameError", E_Exception);
    E_UnboundLocalError = mk("UnboundLocalError", E_NameError);
    E_OSError = mk("OSError", E_Exception);
    // OSError(errno, strerror[, filename]): its attributes and "[Errno 2] No such file ...: 'x'"
    add_method(E_OSError, "__init__", [](const CallArgs& a) {
        auto* e = static_cast<ExcObj*>(a.pos[0].ref());
        std::vector<var> args(a.pos.begin() + 1, a.pos.end());
        e->args = new_tuple(args);
        var self = a.pos[0];
        if (args.size() >= 2 && args.size() <= 5) {
            self.setattr("errno", args[0]);
            self.setattr("strerror", args[1]);
            self.setattr("filename", args.size() >= 3 ? args[2] : var(None));
            self.setattr("filename2", args.size() >= 5 ? args[4] : var(None));
            if (args.size() >= 3) e->args = new_tuple({args[0], args[1]});
        } else {
            self.setattr("errno", None);
            self.setattr("strerror", None);
            self.setattr("filename", None);
            self.setattr("filename2", None);
        }
        return None;
    });
    add_method(E_OSError, "__str__", [](const CallArgs& a) {
        var s = a.pos[0];
        var fn = getattr(s, interned("filename"), None), en = getattr(s, interned("errno"), None);
        var se = getattr(s, interned("strerror"), None);
        if (!en.is_none() && !se.is_none()) {
            std::string r = "[Errno " + str_(en) + "] " + str_(se);
            if (!fn.is_none()) r += ": " + repr_(fn);
            var fn2 = getattr(s, interned("filename2"), None);
            if (!fn2.is_none()) r += " -> " + repr_(fn2);
            return new_str(r);
        }
        return new_str(exc_str(s));
    });
    E_FileNotFoundError = mk("FileNotFoundError", E_OSError);
    E_FileExistsError = mk("FileExistsError", E_OSError);
    E_PermissionError = mk("PermissionError", E_OSError);
    E_IsADirectoryError = mk("IsADirectoryError", E_OSError);
    E_NotADirectoryError = mk("NotADirectoryError", E_OSError);
    E_TimeoutError = mk("TimeoutError", E_OSError);
    TypeObj* E_InterruptedError = mk("InterruptedError", E_OSError);
    TypeObj* E_ChildProcessError = mk("ChildProcessError", E_OSError);
    TypeObj* E_ConnectionError = mk("ConnectionError", E_OSError);
    TypeObj* E_BrokenPipeError = mk("BrokenPipeError", E_ConnectionError);
    TypeObj* E_ProcessLookupError = mk("ProcessLookupError", E_OSError);
    TypeObj* E_BlockingIOError = mk("BlockingIOError", E_OSError);
    TypeObj* E_ReferenceError = mk("ReferenceError", E_Exception);
    E_RuntimeError = mk("RuntimeError", E_Exception);
    E_NotImplementedError = mk("NotImplementedError", E_RuntimeError);
    E_RecursionError = mk("RecursionError", E_RuntimeError);
    E_SyntaxError = mk("SyntaxError", E_Exception);
    TypeObj* E_SystemError = mk("SystemError", E_Exception);
    E_TypeError = mk("TypeError", E_Exception);
    E_ValueError = mk("ValueError", E_Exception);
    E_UnicodeError = mk("UnicodeError", E_ValueError);
    E_UnicodeDecodeError = mk("UnicodeDecodeError", E_UnicodeError);
    E_UnicodeEncodeError = mk("UnicodeEncodeError", E_UnicodeError);
    // UnicodeDecodeError(encoding, object, start, end, reason)
    for (TypeObj* u : {E_UnicodeDecodeError, E_UnicodeEncodeError}) {
        bool dec = u == E_UnicodeDecodeError;
        add_method(u, "__str__", [dec](const CallArgs& a) {
            auto& args = tuple_items(static_cast<ExcObj*>(a.pos[0].ref())->args);
            if (args.size() != 5) return new_str(exc_str(a.pos[0]));
            std::string enc = str_(args[0]), reason = str_(args[4]);
            int64_t st = args[2].as_int64(), en = args[3].as_int64();
            char buf[64];
            if (dec) {
                const std::string& raw = raw_of(args[1]);
                if (en == st + 1) {
                    snprintf(buf, sizeof buf, "0x%02x", (unsigned char)raw[(size_t)st]);
                    return new_str("'" + enc + "' codec can't decode byte " + buf + " in position " + std::to_string(st) + ": " + reason);
                }
                return new_str("'" + enc + "' codec can't decode bytes in position " + std::to_string(st) + "-" + std::to_string(en - 1) + ": " + reason);
            }
            auto cps = to_u32(str_utf8(args[1]));
            if (en == st + 1) {
                char32_t c = cps[(size_t)st];
                if (c < 0x100) snprintf(buf, sizeof buf, "'\\x%02x'", (unsigned)c);
                else if (c < 0x10000) snprintf(buf, sizeof buf, "'\\u%04x'", (unsigned)c);
                else snprintf(buf, sizeof buf, "'\\U%08x'", (unsigned)c);
                return new_str("'" + enc + "' codec can't encode character " + buf + " in position " + std::to_string(st) + ": " + reason);
            }
            return new_str("'" + enc + "' codec can't encode characters in position " + std::to_string(st) + "-" + std::to_string(en - 1) + ": " + reason);
        });
        add_getter(u, "start", [](const var& s) { return tuple_items(static_cast<ExcObj*>(s.ref())->args).at(2); });
        add_getter(u, "end", [](const var& s) { return tuple_items(static_cast<ExcObj*>(s.ref())->args).at(3); });
        add_getter(u, "reason", [](const var& s) { return tuple_items(static_cast<ExcObj*>(s.ref())->args).at(4); });
        add_getter(u, "object", [](const var& s) { return tuple_items(static_cast<ExcObj*>(s.ref())->args).at(1); });
        add_getter(u, "encoding", [](const var& s) { return tuple_items(static_cast<ExcObj*>(s.ref())->args).at(0); });
    }
    TypeObj* E_Warning = mk("Warning", E_Exception);
    TypeObj* E_UserWarning = mk("UserWarning", E_Warning);
    TypeObj* E_DeprecationWarning = mk("DeprecationWarning", E_Warning);
    TypeObj* E_RuntimeWarning = mk("RuntimeWarning", E_Warning);
    TypeObj* E_StopAsyncIteration = mk("StopAsyncIteration", E_Exception);

    t::BaseException = tv(E_BaseException); t::Exception = tv(E_Exception); t::SystemExit = tv(E_SystemExit);
    t::KeyboardInterrupt = tv(E_KeyboardInterrupt); t::GeneratorExit = tv(E_GeneratorExit);
    t::StopIteration = tv(E_StopIteration); t::ArithmeticError = tv(E_ArithmeticError);
    t::ZeroDivisionError = tv(E_ZeroDivisionError); t::OverflowError = tv(E_OverflowError);
    t::FloatingPointError = tv(E_FloatingPointError); t::AssertionError = tv(E_AssertionError);
    t::AttributeError = tv(E_AttributeError); t::BufferError = tv(E_BufferError); t::EOFError = tv(E_EOFError);
    t::ImportError = tv(E_ImportError); t::ModuleNotFoundError = tv(E_ModuleNotFoundError);
    t::LookupError = tv(E_LookupError); t::IndexError = tv(E_IndexError); t::KeyError = tv(E_KeyError);
    t::MemoryError = tv(E_MemoryError); t::NameError = tv(E_NameError); t::UnboundLocalError = tv(E_UnboundLocalError);
    t::OSError = tv(E_OSError); t::FileNotFoundError = tv(E_FileNotFoundError); t::FileExistsError = tv(E_FileExistsError);
    t::PermissionError = tv(E_PermissionError); t::IsADirectoryError = tv(E_IsADirectoryError);
    t::NotADirectoryError = tv(E_NotADirectoryError); t::TimeoutError = tv(E_TimeoutError);
    t::InterruptedError = tv(E_InterruptedError); t::ChildProcessError = tv(E_ChildProcessError);
    t::ConnectionError = tv(E_ConnectionError); t::BrokenPipeError = tv(E_BrokenPipeError);
    t::ProcessLookupError = tv(E_ProcessLookupError); t::BlockingIOError = tv(E_BlockingIOError);
    t::ReferenceError = tv(E_ReferenceError); t::RuntimeError = tv(E_RuntimeError);
    t::NotImplementedError = tv(E_NotImplementedError); t::RecursionError = tv(E_RecursionError);
    t::SyntaxError = tv(E_SyntaxError); t::SystemError = tv(E_SystemError); t::TypeError = tv(E_TypeError);
    t::ValueError = tv(E_ValueError); t::UnicodeError = tv(E_UnicodeError);
    t::UnicodeDecodeError = tv(E_UnicodeDecodeError); t::UnicodeEncodeError = tv(E_UnicodeEncodeError);
    t::Warning = tv(E_Warning); t::UserWarning = tv(E_UserWarning); t::DeprecationWarning = tv(E_DeprecationWarning);
    t::RuntimeWarning = tv(E_RuntimeWarning); t::StopAsyncIteration = tv(E_StopAsyncIteration);
    (void)exc_dealloc_guard;
}

// native/l2's pyre and pystr raise by the exception's name (value.hpp's interface)
[[noreturn]] void raise(const char* type, const std::string& msg) {
    std::string t = type;
    if (t == "re.error" || t == "error") raise(make_exc_args(T(import("re").getattr("error")), {new_str(msg)}));
    var b = getattr(import("builtins"), new_str(t), None);
    if (b.is_ref() && b.ref()->type->layout == Layout::Type) raise(T(b), msg);
    raise(E_RuntimeError, t + ": " + msg);
}

// an OSError of the C library's errno (or a Windows error), the subclass Python picks for it
var os_error(int err, const std::string& filename, const std::string& filename2) {
    TypeObj* cls = E_OSError;
    switch (err) {
    case ENOENT: cls = E_FileNotFoundError; break;
    case EEXIST: cls = E_FileExistsError; break;
    case EACCES: case EPERM: cls = E_PermissionError; break;
    case EISDIR: cls = E_IsADirectoryError; break;
    case ENOTDIR: cls = E_NotADirectoryError; break;
    default: break;
    }
    std::string msg;
    switch (err) {   // the messages CPython prints on Windows
    case ENOENT: msg = "No such file or directory"; break;
    case EEXIST: msg = "File exists"; break;
    case EACCES: msg = "Permission denied"; break;
    case EISDIR: msg = "Is a directory"; break;
    case ENOTDIR: msg = "Not a directory"; break;
    case EINVAL: msg = "Invalid argument"; break;
    case ENOTEMPTY: msg = "Directory not empty"; break;
    default: msg = std::strerror(err); break;
    }
    std::vector<var> args{var(err), new_str(msg)};
    if (!filename.empty() || !filename2.empty()) args.push_back(new_str(filename));
    if (!filename2.empty()) { args.push_back(None); args.push_back(new_str(filename2)); }
    return make_exc_args(cls, args);
}

}  // namespace py
