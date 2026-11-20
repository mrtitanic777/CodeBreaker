// A deterministic stand-in for the translator's back end (emit/source.hpp BodySource), for the tests of the source emitter and of the
// converter: every answer is a function of the question's names, so a test can state what the output must be.
#pragma once

#include <optional>
#include <string>
#include <vector>

#include "core/text.hpp"
#include "java/emit/ownership.hpp"
#include "java/emit/source.hpp"

namespace cb::java::emit::source {

class FakeBodies : public BodySource {
public:
    std::optional<std::string> field_init(const ir::JClass&, const ir::Field& f) override {
        if (f.initializer->find("gap") != std::string::npos) return std::nullopt;
        return "INIT(" + std::string(text::trim(*f.initializer)) + ")";
    }
    std::optional<std::vector<std::string>> array_items(const ir::JClass&, const ir::Field& f) override {
        const auto body = ownership::aggregate_array_body(f);
        if (!body || body->find("gap") != std::string::npos) return std::nullopt;
        std::vector<std::string> items;
        for (const std::string_view item : text::split(*body, ',')) items.emplace_back(text::trim(item));
        return items;
    }
    std::optional<std::vector<std::string>> instance_inits(const ir::JClass& cls) override {
        if (cls.name.starts_with("Gap")) return std::nullopt;
        return std::vector<std::string>{"// inst " + cls.name};
    }
    std::optional<CtorBody> ctor(const ir::JClass&, const ir::Method* m, bool two_phase) override {
        const std::string suffix = two_phase ? "p" : "";
        if (m == nullptr) return CtorBody{.lines = {"// implicit ctor" + suffix}};
        for (const ir::Param& p : m->params) {
            if (p.type.name == "Gap") return std::nullopt;
        }
        const std::size_t n = m->params.size();
        const std::vector<std::string> lines = {"// ctor" + std::to_string(n) + suffix};
        if (n == 0) return CtorBody{.lines = lines};
        if (n == 1) return CtorBody{.call = CtorCall::Super, .super_args = "a0", .lines = lines};
        if (n == 2) return CtorBody{.call = CtorCall::This, .lines = lines};
        return CtorBody{.call = CtorCall::Super, .super_args = "", .lines = lines};
    }
    std::optional<std::vector<std::string>> enum_args(const ir::JClass&, const ir::EnumConst& c) override {
        if (c.name == "GAPC") return std::nullopt;
        return std::vector<std::string>{std::to_string(c.name.size()), "\"" + c.name + "\""};
    }
    std::optional<std::vector<std::vector<std::string>>> static_blocks(const ir::JClass& cls) override {
        std::size_t n = cls.static_inits.size();
        if (cls.name == "BadBlocks") n -= 1;
        std::vector<std::vector<std::string>> blocks;
        for (std::size_t i = 0; i < n; ++i) blocks.push_back({"// static block " + std::to_string(i)});
        return blocks;
    }
    std::optional<std::vector<std::string>> body(const ir::JClass& cls, const ir::Method& m) override {
        if (m.name.starts_with("gap")) return std::nullopt;
        if (m.name.starts_with("getInstance")) return std::vector<std::string>{"return s_instance;"};
        return std::vector<std::string>{"// body " + cls.name + "." + m.name + "/" + std::to_string(m.params.size())};
    }
};

}  // namespace cb::java::emit::source
