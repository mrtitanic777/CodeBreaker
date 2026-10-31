// questions.h -- how a reader offers its answers through cbf_call.
//
// Each reader keeps its parsing in plain C++ (pe/pe.h ...) and, beside it, a short file of
// questions: a function per question that reads the call's arguments, asks the reader, and
// writes the answer in the shape the Python reader gave it (the same dict keys in the same
// order, tuples where it had tuples).  The registry maps "pe.exports" to that function.
#pragma once

#include <map>
#include <string>

#include "bytes.h"
#include "pyvalue.h"

namespace cbf {

struct Call {
    Bytes data;          // the file's bytes (borrowed for the call)
    const Value &args;   // the arguments: a tuple
    Out &out;            // the answer

    size_t argc() const { return args.items.size(); }
    const Value &arg(size_t k) const {
        static const Value none;
        return k < args.items.size() ? args.items[k] : none;
    }
    int64_t int_arg(size_t k, int64_t dflt = 0) const { return k < argc() && !arg(k).is_none() ? arg(k).as_int() : dflt; }
    bool bool_arg(size_t k, bool dflt = false) const { return k < argc() ? arg(k).truthy() : dflt; }
    std::string str_arg(size_t k, const std::string &dflt = "") const { return k < argc() && !arg(k).is_none() ? arg(k).as_str() : dflt; }
};

using Answer = void (*)(Call &);

std::map<std::string, Answer> &registry();

// one reader's questions, registered at load: CBF_QUESTIONS(pe) { add("pe.exports", exports); ... }
struct Registrar {
    explicit Registrar(void (*fill)()) { fill(); }
};
inline void add(const char *question, Answer a) { registry()[question] = a; }

#define CBF_QUESTIONS(reader)                                       \
    static void cbf_register_##reader();                            \
    static ::cbf::Registrar cbf_registrar_##reader(cbf_register_##reader); \
    static void cbf_register_##reader()

}  // namespace cbf
