// build/x86cc/l2body.hpp -- function bodies given to x86cc as calls rather than as C text (docs/L2_COMPILER.md, seam 1).
//
// A compile of a C unit whose function bodies are empty (`int add(int a, int b) {}`) can take a body from its caller: when the
// front end reaches a body the caller gives, it builds the function's statements by the calls of a BodyBuilder instead of
// reading them. Each call is one of the front end's own semantic actions, the one its parser makes for the same C, so a body
// built here gives the same tree, and so the same object, as the text it stands for. The caller (build/l2cc: L2 lowered from
// translate::ir) knows nothing of OpenWatcom's types, and the front end nothing of L2's.
//
//     struct Add : x86cc::BodySource {
//         bool gives(std::string_view f) const override { return f == "add"; }   // others: the text's body is compiled
//         void build(std::string_view, x86cc::BodyBuilder& b) override {
//             b.return_value(b.binary(x86cc::BinaryOp::Add, b.name("a"), b.name("b")), true);
//         }
//     } bodies;
//     x86cc::Run r = x86cc::compile(x86cc::Target::i386, {"-bt=nt", "add.c"}, folder, {}, &bodies);
//
// A builder's values are the front end's trees, only good inside the call that is building the body. A name that does not
// resolve, or a call the front end refuses, makes the builder fail (`failed()`, with why); what it built is still compiled, so
// the compile's object is not to be used then.
#pragma once

#include <cstdint>
#include <string>
#include <string_view>

namespace cb::build::x86cc {

// The binary operators, by their C meaning (each is the token the front end's semantic action takes).
enum class BinaryOp : std::uint8_t { Add, Sub, Mul, Div, Rem, Shl, Shr, And, Or, Xor, Eq, Ne, Lt, Le, Gt, Ge };

// The type of an integer constant, as C gives it to a literal (`1`, `1u`, `1L`, `1uL`, `1LL`, `1uLL`).
enum class IntType : std::uint8_t { Int, UInt, Long, ULong, LongLong, ULongLong };

class BodyBuilder {
public:
    struct Value {
        void* tree = nullptr;   // the front end's expression tree
    };

    virtual ~BodyBuilder() = default;

    // -- expressions
    virtual Value name(std::string_view id) = 0;   // a parameter, local or global, by its name in the unit
    virtual Value integer(std::uint64_t bits, IntType type) = 0;
    virtual Value binary(BinaryOp op, Value left, Value right) = 0;

    // -- statements
    virtual void expression(Value value) = 0;   // an expression statement
    // `last`: the function's final statement, at its outermost block (the text's `return x; }`): no jump to the end is made.
    virtual void return_value(Value value, bool last) = 0;
    virtual void return_void(bool last) = 0;

    virtual bool failed() const = 0;
    virtual const std::string& why() const = 0;
};

// The bodies a compile takes from its caller. For each function the text defines, the front end asks `gives` by the function's
// name: if it does, the text's body is `{}` and `build` makes the statements; if not, the text's body is compiled.
class BodySource {
public:
    virtual ~BodySource() = default;
    virtual bool gives(std::string_view function) const = 0;
    virtual void build(std::string_view function, BodyBuilder& builder) = 0;
};

}  // namespace cb::build::x86cc
