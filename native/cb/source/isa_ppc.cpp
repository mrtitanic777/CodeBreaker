// cb-unity: alone (file-local names that other files in a batch also use)
// cb/source/isa_ppc.cpp -- the PowerPC 750 family for the source writers (isa_ppc.hpp).

#include "cb/source/isa_ppc.hpp"

#include "cb/decompile/ppc.hpp"

namespace cb::source::isa {

namespace {

namespace dp = decompile::ppc;
namespace dz = decompile::z80;

class PpcSyntax final : public build::Encoder {
public:
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
            return dp::assemble(line, static_cast<std::uint32_t>(pc), labels);
        } catch (const dz::UnknownLabel& e) {
            return Failure(build::UndefinedSymbolError(e.name()));
        } catch (const ParseError& e) {
            return Failure(e.clone());
        }
    }
};

}  // namespace

const build::Encoder& ppc_encoder() {
    static const PpcSyntax syntax;
    return syntax;
}

}  // namespace cb::source::isa
