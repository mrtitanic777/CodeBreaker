// codebreaker/exact/emu/isis.hpp -- typed C++ port of codebreaker.exact.emu.isis.
//
// isis.py - run an Intel ISIS-II program (8080 code) : enough of the 8080 and of ISIS-II for the
// Intel development tools of 1978-82 - PL/M-86, LINK86, LOC86, LIB86, OH86.
//
//   python isis.py [-d0 DIR] [-d1 DIR] ... PROGRAM command tail
//   run(program, tail, drives) -> Result(console, status, steps)
//
// The tools are data here: 8080 object files interpreted by this module, nothing of them runs on the
// host. Drives :F0: .. :F9: are host directories (file names upper case, as ISIS has them).
// ISIS calls (CALL 40h, C = function, DE -> parameter block): OPEN CLOSE DELETE READ WRITE SEEK LOAD
// RENAME CONSOL EXIT ATTRIB RESCAN ERROR WHOCON SPATH. Monitor entries: CI CO MEMCK and the rest as
// no-ops.
//
// Notes on the port:
//   * the 64 KiB memory, the console and the console input are py::bytearray, as in the Python: a slice
//     assignment past the end grows the array there too;
//   * the eight registers are a std::array<int64_t, 8> (B C D E H L - A; slot 6 is unused);
//   * the Python builds a table of 256 closures (Machine._build / .ops) once per machine; here the same
//     instruction is decoded from its opcode when it is done (Machine::_exec), with the same effects;
//   * an open file is the Python's list [path, position, device, data, dirty]: the struct File, shared
//     by reference (py::ref) because the Python mutates the list in place;
//   * echo=True writes to sys.stdout; here to the std::ostream the caller hands over (default std::cout);
//   * the Python's __main__ block is main(argv, out) (the name `main` is kept as in xenix);
//   * ints are int64_t; memory addresses and registers never leave 16 / 8 bits.
#pragma once

#include <array>
#include <cstdint>
#include <iostream>
#include <optional>
#include <ostream>
#include <tuple>

#include "py.hpp"
#include "py_os.hpp"

namespace codebreaker::exact::emu::isis {

// class Result: console, status, steps
class Result {
  public:
    py::str console;
    int64_t status;
    int64_t steps;
    Result(const py::str& console_, int64_t status_, int64_t steps_) : console(console_), status(status_), steps(steps_) {}
};

// class Exit(Exception): the program ended (HLT or the EXIT call)
struct Exit : py::Exception {
    Exit() : py::Exception() {}
    const char* type() const override { return "Exit"; }
};

// next(...) of an exhausted generator: no free file number.
struct StopIteration : py::Exception {
    StopIteration() : py::Exception() {}
    const char* type() const override { return "StopIteration"; }
};

// PAR[i] = 1 when the byte i has an even number of one bits (the 8080's parity flag).
extern const py::list<int64_t> PAR;

// An open file or device: [path, position, device, data, dirty].
struct File {
    std::optional<py::str> path;
    int64_t pos;
    std::optional<py::str> dev;
    py::bytearray data;
    bool dirty;
};

// -> (host path or None, device name or None)
using PathResult = std::tuple<std::optional<py::str>, std::optional<py::str>>;

class Machine {
  public:
    py::bytearray m;                           // memory
    std::array<int64_t, 8> r;                  // B C D E H L - A
    int64_t sp, pc, fz, fs, fp, fc, fa;        // stack pointer, program counter, flags
    py::dict<int64_t, py::str> drives;
    py::dict<int64_t, py::ref<File>> files;
    py::bytearray console;
    bool echo;
    py::bytearray cin;
    int64_t cpos;
    std::optional<int64_t> status;
    int64_t steps;                             // set by run()
    std::ostream* out;                         // where echo goes (sys.stdout)

    Machine(const py::dict<int64_t, py::str>& drives, const py::str& tail, bool echo = false, std::ostream& out = std::cout);

    // ---- files
    PathResult path(const py::str& name);
    py::str str_at(int64_t a);
    int64_t w(int64_t a);
    void ww(int64_t a, int64_t v);
    int64_t load(const py::str& path, int64_t bias = 0);
    // -> True when control went to the program just loaded (LOAD with retsw 1), else None (false)
    bool isis();

    // ---- the 8080
    int64_t pop();
    // the instruction `op`, whose first byte is read and pc is already past
    void _exec(int64_t op);
    int64_t run(int64_t limit = 2000000000);

  private:
    int64_t rd(int64_t i);
    void wr(int64_t i, int64_t v);
    void szp(int64_t v);
    void add(int64_t v, int64_t c = 0);
    void sub(int64_t v, int64_t c = 0, bool keep = true);
    void logic(int64_t t, int64_t ac = 0);
    void push(int64_t v);
    int64_t imm8();
    int64_t imm16();
    int64_t psw();
    bool cond(int64_t k);
    int64_t rp(int64_t k);
    void setrp(int64_t k, int64_t v);
};

// program: host path of the ISIS object file; tail: the command line after the program name
Result run(const py::str& program, const py::str& tail, const py::dict<int64_t, py::str>& drives, bool echo = false,
           int64_t limit = 2000000000, std::ostream& out = std::cout);

// The command line: argv as sys.argv is (the program's name first). Writes to `out`.
void main(const py::list<py::str>& argv, std::ostream& out);

}  // namespace codebreaker::exact::emu::isis
