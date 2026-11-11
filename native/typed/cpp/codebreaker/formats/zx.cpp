// codebreaker/formats/zx.cpp -- see zx.hpp.

#include "codebreaker/formats/zx.hpp"

#include "codebreaker/formats/native_ask.hpp"

namespace codebreaker::formats::zx {

const std::tuple<py::str, py::str, py::str> EXTENSIONS{py::str(".sna"), py::str(".z80"), py::str(".tap")};

bool is_zx(const py::str& path) {
    py::str low = path.lower();
    if (!low.endswith(EXTENSIONS)) return false;
    try {
        read(path);
        return true;                              // (read() never gives None)
    } catch (const py::OSError&) {
        return false;
    } catch (const py::ValueError&) {
        return false;
    } catch (const py::StructError&) {
        return false;
    } catch (const py::IndexError&) {
        return false;
    }
}

Program _program(const Program& answer) { return answer; }

Program read(const py::str& path) {
    namespace na = native_ask;
    // os.fspath(path), then the question about no data
    return _program(na::ask<Program>("zx.read", py::bytes(), na::no_own, path));
}

}  // namespace codebreaker::formats::zx
