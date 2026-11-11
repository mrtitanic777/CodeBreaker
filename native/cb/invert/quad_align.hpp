// cb/invert/quad_align.hpp -- align a Lattice C quad file with its compiled object, function by function, giving the
// ground-truth (input, output) pairs of the MC2 code generator: the quad records of a function and the bytes the
// compiler made of them. The foundation for the forward quad-to-bytes model (early_c_forward).
//
// The functions of the quad file are the runs of records that start at FUNC-BEGIN; the functions of the object are its
// public symbols in address order, each running to the next public. The two lists are paired by position.
#pragma once

#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

#include "cb/core/bytes.hpp"
#include "cb/invert/quad_dump.hpp"

namespace cb::invert::quad_align {

struct Quad {
    std::uint8_t opcode = 0;
    Bytes payload;
};
using QuadFunction = std::vector<Quad>;   // one function: FUNC-BEGIN first

// The functions of a quad file, in file order. Records before the first FUNC-BEGIN belong to no function and are
// dropped. A file too short to hold records gives none. TruncatedError if a record promises more payload than the
// file holds.
std::vector<QuadFunction> parse_quads(ByteView quad_file);

// A public symbol of the object with the code that follows it.
struct ObjectFunction {
    std::string name;
    std::uint32_t offset = 0;   // in the PROG segment
    Bytes code;                 // up to the next public (the end of the segment for the last)
};

// The PROG-segment functions of an OMF object (16-bit or 32-bit records), in address order (equal offsets keep the
// order of the PUBDEF records). Data symbols are included: they are publics too (align() filters them).
// FormatError/TruncatedError for a damaged object.
std::vector<ObjectFunction> parse_obj_funcs(ByteView object);

struct Aligned {
    std::string name;
    std::uint32_t offset = 0;
    Bytes code;
    QuadFunction quads;   // empty when the object has more functions than the quad file
};

// Pair the object's functions with the quad file's by position. Publics whose bytes do not start like a function
// (push bp, push bx, mov r,r/m, mov ax,imm: 55 53 8b b8) are data symbols and are left out.
std::vector<Aligned> align(const std::vector<QuadFunction>& quads, const std::vector<ObjectFunction>& functions);

// The alignment as the tool prints it: the two counts, then each function's name, size, quad opcodes and code bytes.
// `quad_functions` is how many functions the quad file had, `aligned` what align() made of them.
std::string format_alignment(std::size_t quad_functions, const std::vector<Aligned>& aligned);

// All of it from the two files' bytes.
std::string report(ByteView quad_file, ByteView object);

}  // namespace cb::invert::quad_align
