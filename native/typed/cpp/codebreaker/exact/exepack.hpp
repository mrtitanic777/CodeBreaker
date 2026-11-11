// codebreaker/exact/exepack.hpp -- compat shim for cb/exact/exepack.hpp (docs/CPP_DESIGN.md 6.3). No logic: conversion
// and forwarding. Deleted with its last old-layer consumer.
#pragma once

#include "cb/compat/py_bridge.hpp"
#include "cb/exact/exepack.hpp"
#include "py.hpp"

namespace codebreaker::exact::exepack {

class Packed : public py::object {
public:
    int64_t ip = 0;
    int64_t cs = 0;
    int64_t mem = 0;
    int64_t size = 0;
    int64_t sp = 0;
    int64_t ss = 0;
    int64_t dest = 0;
    py::list<int64_t> relocs;
};

inline const py::bytes SIG = py::bytes("RB");
inline const py::bytes MSG = py::bytes("Packed file is corrupt");

// The packed program's header and relocations; null when the file is not EXEPACK-packed. Raises py::StructError
// (struct.error) when the relocation table runs off the end of the file.
inline py::ref<Packed> read(const py::bytes& b) {
    return cb::compat::guard([&]() -> py::ref<Packed> {
        const auto packed = cb::exact::exepack::read(cb::compat::from_py(b));
        if (!packed) return nullptr;
        py::ref<Packed> p = std::make_shared<Packed>();
        p->ip = packed->ip;
        p->cs = packed->cs;
        p->mem = packed->mem;
        p->size = packed->size;
        p->sp = packed->sp;
        p->ss = packed->ss;
        p->dest = packed->dest;
        p->relocs = cb::compat::to_py(packed->relocs);
        return p;
    });
}

}  // namespace codebreaker::exact::exepack
