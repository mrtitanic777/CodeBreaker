// translate/backends/cpp03_translator.cpp -- the C++03 / C++17 back end as the converter's BodyTranslator (see cpp03.hpp).

#include "translate/backends/cpp03.hpp"

namespace cb::translate::backends::cpp03 {

namespace transpile = cb::java::transpile;

namespace {
namespace translator_impl {

// The bodies the converter reads later: they hold the arena the units were lowered into, the context and the body source. The
// converter's own objects (resolver, classes, ledger, project) are those of the request, which outlive the conversion.
class OwnedBodies : public cb::java::emit::source::BodySource {
public:
    OwnedBodies(const transpile::TranslateRequest& request, const cb::java::runtime::Runtime& runtime, const Lowering& lowering,
                Dialect dialect, cb::java::projects::Patched& patched)
        : ctx_(request.resolver, request.type_profile, runtime, request.classes) {
        ctx_.ledger = &request.ledger;
        ctx_.char_map = request.project.char_map;
        Lowered lowered = lowering(arena_, request);
        patched = std::move(lowered.patched);
        bodies_ = std::make_unique<Bodies>(ctx_, arena_, lowered.units, dialect);
    }

    const Bodies& bodies() const { return *bodies_; }

    std::optional<std::string> field_init(const jir::JClass& cls, const jir::Field& field) override { return bodies_->field_init(cls, field); }
    std::optional<std::vector<std::string>> array_items(const jir::JClass& cls, const jir::Field& field) override {
        return bodies_->array_items(cls, field);
    }
    std::optional<std::vector<std::string>> instance_inits(const jir::JClass& cls) override { return bodies_->instance_inits(cls); }
    std::optional<cb::java::emit::source::CtorBody> ctor(const jir::JClass& cls, const jir::Method* ctor, bool two_phase) override {
        return bodies_->ctor(cls, ctor, two_phase);
    }
    std::optional<std::vector<std::string>> enum_args(const jir::JClass& cls, const jir::EnumConst& constant) override {
        return bodies_->enum_args(cls, constant);
    }
    std::optional<std::vector<std::vector<std::string>>> static_blocks(const jir::JClass& cls) override { return bodies_->static_blocks(cls); }
    std::optional<std::vector<std::string>> body(const jir::JClass& cls, const jir::Method& method) override { return bodies_->body(cls, method); }

private:
    Arena arena_;   // (declared before what refers into it)
    Context ctx_;
    std::unique_ptr<Bodies> bodies_;
};

}  // namespace translator_impl
}  // namespace

transpile::TranslatedBodies Translator::translate(const transpile::TranslateRequest& request) {
    transpile::TranslatedBodies out;
    const Dialect dialect = request.cpp_year >= 2017 ? Dialect::Cpp17 : Dialect::Cpp03;
    auto owned = std::make_unique<translator_impl::OwnedBodies>(request, runtime_, lowering_, dialect, out.patched);
    const Bodies& bodies = owned->bodies();
    out.classes_lowered = bodies.class_count();
    out.methods_written = bodies.written();
    for (const auto& [reason, count] : bodies.declined()) out.declined.emplace_back(reason, count);
    out.bodies = std::move(owned);
    return out;
}

}  // namespace cb::translate::backends::cpp03
