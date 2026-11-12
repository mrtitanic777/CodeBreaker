// codebreaker/exact/project_forward.hpp -- compat helper (docs/CPP_DESIGN.md 6.3) for the shims of the exact commands that
// add to what exact/modules reads of a project (exact/data, exact/library): a new-layer interface that derives from
// cb::exact::modules::Project and answers the modules part from an old-layer modules::Project. Conversion and
// forwarding only; deleted with the last of those shims.
#pragma once

#include "codebreaker/exact/modules.hpp"

namespace codebreaker::exact::modules::detail {

// `Base` is a new-layer interface derived from cb::exact::modules::Project; the members of that part are
// answered by the old project given.
template <class Base>
class ForwardedProject : public Base {
  public:
    explicit ForwardedProject(const codebreaker::exact::modules::Project& old) : inner_(old) {}

    std::filesystem::path src() const override { return inner_.src(); }
    std::filesystem::path path(std::string_view name) const override { return inner_.path(name); }
    std::uint32_t code_end() const override { return inner_.code_end(); }
    std::uint32_t own_end() const override { return inner_.own_end(); }
    std::optional<std::string> switches() const override { return inner_.switches(); }
    std::optional<std::string> config_text(std::string_view key) const override { return inner_.config_text(key); }
    std::vector<std::uint32_t> code_segments() const override { return inner_.code_segments(); }
    std::shared_ptr<const cb::exact::modules::Toolchain> toolchain() const override { return inner_.toolchain(); }
    std::map<std::string, cb::Bytes> headers() const override { return inner_.headers(); }
    std::vector<std::string> include() const override { return inner_.include(); }
    std::vector<std::shared_ptr<const cb::exact::modules::Project>> also_sets() const override { return inner_.also_sets(); }
    cb::ByteView image() const override { return inner_.image(); }
    cb::exact::SymbolTable symbols() const override { return inner_.symbols(); }
    cb::exact::compare::Program program() const override { return inner_.program(); }

  private:
    ProjectAdapter inner_;
};

}  // namespace codebreaker::exact::modules::detail
