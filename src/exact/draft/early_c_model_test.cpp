// Tests of cb/exact/draft/early_c_model.hpp: the early-C inverse model as the draft stage's InverseModel, on the function the model's own tests
// read (`f(a, b) { return a + b; }` as the C compiler of 1983-84 compiles it) and on the stage's own use of it.
#include "exact/draft/early_c_model.hpp"

#include "core/error.hpp"
#include "core/testing.hpp"

using namespace cb;
using namespace cb::exact;

namespace {

constexpr std::uint8_t kAdd[] = {0x55, 0x8b, 0xec, 0x8b, 0x46, 0x04, 0x03, 0x46, 0x06, 0x5d, 0xc3};   // push bp; mov bp,sp; mov ax,[bp+4]; add ax,[bp+6]; pop bp; ret

}  // namespace

CB_TEST("exact.draft.early_c_model.reads_a_function_both_ways", [] {
    const auto model = draft::early_c_model();
    const draft::ModelCapabilities c = model->capabilities();
    CB_CHECK(c.structured && c.table_switch && c.fold_ladders && c.fp_casts && c.unsigned_compares && c.frame_after_bp && c.regvars_eager);
    for (const bool structured : {false, true}) {
        const draft::Recovery r = structured ? model->recover_structured(kAdd, {}, {}) : model->recover(kAdd, {});
        CB_CHECK_EQ(r.frame_size, 0u);
        CB_CHECK(r.params == (std::vector<std::string>{"a", "b"}));
        CB_CHECK(r.stmts == (std::vector<std::string>{"return (a + b);"}));
        const std::string text = model->to_c("f", r);
        CB_CHECK(text.find("f(a, b)") != std::string::npos);
        CB_CHECK(text.find("int a, b;") != std::string::npos);
        CB_CHECK(text.find("return (a + b);") != std::string::npos);
    }
    CB_CHECK(model->last().regparam.empty());
    CB_CHECK(!model->last().fpret.has_value());
});

CB_TEST("exact.draft.early_c_model.marks_the_byte_wide_places_for_the_checker", [] {
    const auto model = draft::early_c_model();
    // mov bx,[bp+4]; mov al,[bx]; xor ah,ah: a byte read through a pointer
    constexpr std::uint8_t byte_read[] = {0x55, 0x8b, 0xec, 0x8b, 0x5e, 0x04, 0x8a, 0x07, 0x32, 0xe4, 0x5d, 0xc3};
    const std::optional<std::string> marked = model->marked_c(byte_read);
    CB_CHECK(marked.has_value());
    CB_CHECK(marked && marked->find("BYTE(") != std::string::npos);
});

CB_TEST("exact.draft.early_c_model.is_the_model_the_profile_names", [] {
    CB_CHECK(draft::inverse_model_named("early_c") != nullptr);
    CB_CHECK_THROWS_AS(UsageError, (void)draft::inverse_model_named("msc6"));
});
