#include "cb/exact/hooks_lattice.hpp"

namespace cb::exact::hooks_lattice {
namespace {

constexpr std::uint32_t kLeftNode = 0x1C, kRightNode = 0x1E;   // the two operand nodes, in the frame
constexpr std::uint32_t kOperandKind = 6;                       // in the frame: what is being compared
constexpr std::uint16_t kLongKind = 4;
constexpr std::uint32_t kNodeFlags = 2;                         // in a node: its type flags
constexpr std::uint16_t kViaPointer = 0x2000;

}  // namespace

std::uint32_t cmp_test(const WordReader& read_word, std::uint32_t bp, std::uint32_t ss, std::uint32_t ds) {
    const std::uint32_t n = read_word(ss, bp + kLeftNode);
    const std::uint32_t m = read_word(ss, bp + kRightNode);
    const bool eager = read_word(ss, bp + kOperandKind) == kLongKind && !(read_word(ds, n + kNodeFlags) & kViaPointer) &&
                       !(read_word(ds, m + kNodeFlags) & kViaPointer);
    return eager ? kLoadBoth : kSkipLoad;
}

}  // namespace cb::exact::hooks_lattice
