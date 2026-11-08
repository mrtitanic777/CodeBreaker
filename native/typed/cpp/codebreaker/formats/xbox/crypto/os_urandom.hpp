// codebreaker/formats/xbox/crypto/os_urandom.hpp -- os.urandom(n) for the Xbox 360 crypto scripts.
//
// urandom(n) draws n bytes from the operating system's random source (std::random_device). The scripts that
// draw random keys / IVs (xekeys_signer, exp_signer) call it through here; set_urandom installs another
// source (a test harness uses it to make a run reproducible; the Python harness replaces the module's
// `urandom` name the same way). set_urandom with an empty function restores the operating system's.
//
// (A helper the runtime does not have: export_keys.cpp keeps its own copy of the plain function.)
#pragma once

#include <cstdint>
#include <functional>
#include <random>

#include "py.hpp"

namespace codebreaker::formats::xbox::crypto::os_urandom {

using Source = std::function<py::bytes(int64_t)>;

inline Source& source_() {
    static Source s;
    return s;
}

inline void set_urandom(const Source& source) { source_() = source; }

// os.urandom(n)
inline py::bytes urandom(int64_t n) {
    if (source_()) return source_()(n);
    if (n < 0) throw py::ValueError("negative argument not allowed");
    std::random_device rd;
    py::bytearray out = py::bytearray::zeros(n);
    for (int64_t i = 0; i < n; ++i) out[i] = static_cast<int64_t>(rd() & 0xFF);
    return py::bytes(out);
}

}  // namespace codebreaker::formats::xbox::crypto::os_urandom
