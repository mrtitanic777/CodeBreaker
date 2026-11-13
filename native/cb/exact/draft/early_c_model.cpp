#include "cb/exact/draft/early_c_model.hpp"

#include <format>

#include "cb/core/error.hpp"
#include "cb/invert/early_c.hpp"

namespace cb::exact::draft {

namespace {

namespace ec = cb::invert::early_c;

// What the draft stage reads of a recovery besides the statements: the model's own return type and locals, handed back to `to_c`.
struct EarlyState {
    std::string return_type;
    std::vector<cb::invert::early_c_frame::Offset> locals;
};

class EarlyCImpl final : public EarlyCModel {
public:
    ModelCapabilities capabilities() const override {
        ModelCapabilities c;
        c.frame_after_bp = c.regvars_eager = c.structured = true;
        c.table_switch = c.fp_casts = c.unsigned_compares = c.fold_ladders = true;
        return c;
    }

    Recovery recover(ByteView code, const Reading& reading) override { return made(ec::recover(code, 0, {}, options_for(reading))); }

    Recovery recover_structured(ByteView code, const Reading& reading, const Structuring& how) override {
        ec::Options o = options_for(reading);
        o.compare_switches = how.compare_switches;
        o.fp_casts = how.fp_casts;
        o.unsigned_compares = how.unsigned_compares;
        if (how.table_switch) {
            const SwitchHook& hook = how.table_switch;
            o.hooks.switch_targets = [hook](std::uint32_t address) -> std::optional<std::vector<std::uint32_t>> {
                const auto where = hook(address);
                if (!where) return std::nullopt;
                std::vector<std::uint32_t> out;
                out.reserve(where->size());
                for (const std::int64_t w : *where) out.push_back(static_cast<std::uint32_t>(w));
                return out;
            };
        }
        return made(ec::recover_cfg(code, 0, {}, o));
    }

    std::string to_c(std::string_view name, const Recovery& r) override {
        const auto* state = std::any_cast<EarlyState>(&r.model_state);
        if (state == nullptr) throw UsageError("the early-C model was handed a recovery another model made");
        return ec::to_c(name, r.frame_size, r.params, r.stmts, state->return_type, state->locals);
    }

    std::vector<std::string> fold_ladders(const std::vector<std::string>& stmts) override { return ec::fold_ladders(stmts, false); }

    ModelLast last() const override { return last_; }

    // The checker mode: the text of the linear walk with the byte-wide places marked. The stage's own settings (3.00's frames and
    // register variables) are in force, as they were left by the recoveries before it.
    std::optional<std::string> marked_c(ByteView code) const override {
        ec::Options o = ec::Options::from_environment();
        o.mark = true;
        o.frame_after_bp = o.eager_register_variables = true;
        try {
            const ec::Recovered r = ec::recover(code, 0, {}, o);
            return ec::to_c("f", r.frame_size, r.params, r.statements, r.return_type, r.locals);
        } catch (const Error&) {
            return std::nullopt;   // (the model declines: no marks)
        }
    }

private:
    static ec::Options options_for(const Reading& reading) {
        ec::Options o = ec::Options::from_environment();
        o.frame_after_bp = reading.frame_after_bp;
        o.eager_register_variables = reading.regvars_eager;
        return o;
    }

    Recovery made(ec::Recovered&& r) {
        last_ = ModelLast{};
        for (const auto& [reg, index] : r.findings.register_params) last_.regparam.insert_or_assign(reg, static_cast<int>(index));
        for (const auto& [place, type] : r.findings.fp_types) last_.fptypes[place] = type;
        if (!r.findings.fp_return.empty()) last_.fpret = r.findings.fp_return;
        last_.unsigned_longs = r.findings.unsigned_longs;
        Recovery out;
        out.frame_size = r.frame_size;
        out.params = std::move(r.params);
        out.stmts = std::move(r.statements);
        out.model_state = EarlyState{std::move(r.return_type), std::move(r.locals)};
        return out;
    }

    ModelLast last_;
};

}  // namespace

std::shared_ptr<EarlyCModel> early_c_model() { return std::make_shared<EarlyCImpl>(); }

std::shared_ptr<InverseModel> inverse_model_named(std::string_view name) {
    if (name == "early_c") return early_c_model();
    throw UsageError(std::format("no inverse model '{}' in this build (have: early_c)", name));
}

}  // namespace cb::exact::draft
