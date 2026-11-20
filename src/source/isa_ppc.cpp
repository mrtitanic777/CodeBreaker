// cb-unity: alone (file-local names that other files in a batch also use)
// source/isa_ppc.cpp -- the PowerPC 750 family for the source writers (isa_ppc.hpp).

#include "source/isa_ppc.hpp"

#include "decompile/ppc.hpp"

namespace cb::source::isa {

namespace {

namespace dp = decompile::ppc;
namespace dz = decompile::z80;

class PpcSyntax final : public build::Encoder {
public:
    explicit PpcSyntax(bool wide = false) : wide_(wide) {}
    Result<std::int64_t> value(std::string_view expr, const build::Labels& labels, std::int64_t pc) const override {
        try {
            return dp::value(expr, labels, static_cast<std::uint32_t>(pc));
        } catch (const dz::UnknownLabel& e) {
            return Failure(build::UndefinedSymbolError(e.name()));
        } catch (const ParseError& e) {
            return Failure(e.clone());
        }
    }
    Result<Bytes> assemble(std::string_view line, std::int64_t pc, const build::Labels& labels) const override {
        try {
            return dp::assemble(line, static_cast<std::uint32_t>(pc), labels, wide_);
        } catch (const dz::UnknownLabel& e) {
            return Failure(build::UndefinedSymbolError(e.name()));
        } catch (const ParseError& e) {
            return Failure(e.clone());
        }
    }

private:
    bool wide_;
};

}  // namespace

const build::Encoder& ppc_encoder() {
    static const PpcSyntax syntax;
    return syntax;
}

const build::Encoder& ppc64_encoder() {
    static const PpcSyntax syntax(true);
    return syntax;
}

}  // namespace cb::source::isa
