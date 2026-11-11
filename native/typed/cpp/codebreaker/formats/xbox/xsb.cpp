// codebreaker/formats/xbox/xsb.cpp -- see xsb.hpp.

#include "codebreaker/formats/xbox/xsb.hpp"

#include "codebreaker/formats/native_ask.hpp"

namespace codebreaker::formats::xbox::xsb {

Bank read(const py::str& path) {
    namespace na = native_ask;
    // @native.answered("xsb.read", data_arg=None): the question is about no data, the path its argument
    return na::ask<Bank>("xsb.read", py::bytes(), na::no_own, path);
}

}  // namespace codebreaker::formats::xbox::xsb
