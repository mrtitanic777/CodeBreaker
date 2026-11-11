// Tests of the DLL loader and of the three prebuilt libraries through their API tables: each is asked to do one
// small real job, so a wrong header/DLL pairing fails here and not in the middle of an emulation.
#include "cb/core/dynlib.hpp"

#include "cb/core/capstone_api.hpp"
#include "cb/core/error.hpp"
#include "cb/core/testing.hpp"
#include "cb/core/unicorn_api.hpp"
#include "cb/core/z3_api.hpp"

using namespace cb;

CB_TEST("core.dynlib.search_dirs_and_errors", [] {
    CB_CHECK(!DynLib::search_dirs().empty());
    CB_CHECK_THROWS_AS(NotFoundError, DynLib::load_beside_exe("no-such-library.dll"));
    CB_CHECK_THROWS_AS(NotFoundError, DynLib::load("C:/definitely/not/here/x.dll"));
    try {
        (void)DynLib::load_beside_exe("no-such-library.dll");
    } catch (const NotFoundError& e) {
        CB_CHECK(std::string(e.what()).find("looked in") != std::string::npos);
    }
    // a system DLL loads and its functions are found by name
    DynLib k32 = DynLib::load("kernel32.dll");
    using GetTick = unsigned long (__stdcall*)();
    CB_CHECK(k32.function<GetTick>("GetTickCount") != nullptr);
    CB_CHECK_THROWS_AS(NotFoundError, k32.function<GetTick>("NoSuchFunctionAtAll"));
    DynLib moved = std::move(k32);   // ownership moves; the old object is empty
    CB_CHECK(moved.function<GetTick>("GetTickCount64") != nullptr);
});

CB_TEST("core.unicorn.runs_x86_code", [] {
    const auto& uc = emu::unicorn::api();
    CB_CHECK(!emu::unicorn::version_string().empty());
    CB_CHECK(uc.uc_arch_supported(UC_ARCH_X86));
    uc_engine* engine = nullptr;
    emu::unicorn::check(uc.uc_open(UC_ARCH_X86, UC_MODE_32, &engine), "uc_open");
    // mov eax, 5 ; add eax, 0x10 ; ret-less: stop at the end of the code
    const std::uint8_t code[] = {0xB8, 0x05, 0x00, 0x00, 0x00, 0x83, 0xC0, 0x10};
    emu::unicorn::check(uc.uc_mem_map(engine, 0x1000, 0x1000, UC_PROT_ALL), "uc_mem_map");
    emu::unicorn::check(uc.uc_mem_write(engine, 0x1000, code, sizeof(code)), "uc_mem_write");
    emu::unicorn::check(uc.uc_emu_start(engine, 0x1000, 0x1000 + sizeof(code), 0, 0), "uc_emu_start");
    std::uint32_t eax = 0;
    emu::unicorn::check(uc.uc_reg_read(engine, UC_X86_REG_EAX, &eax), "uc_reg_read");
    CB_CHECK_EQ(eax, 0x15u);
    // an unmapped read is reported as an error code that check() turns into an EmulationError
    std::uint8_t byte = 0;
    CB_CHECK_THROWS_AS(EmulationError, emu::unicorn::check(uc.uc_mem_read(engine, 0x9000, &byte, 1), "uc_mem_read"));
    emu::unicorn::check(uc.uc_close(engine), "uc_close");
});

CB_TEST("core.capstone.disassembles_x86", [] {
    const auto& cs = disasm::capstone::api();
    CB_CHECK(!disasm::capstone::version_string().empty());
    csh handle = 0;
    disasm::capstone::check(cs.cs_open(CS_ARCH_X86, CS_MODE_32, &handle), "cs_open");
    const std::uint8_t code[] = {0xB8, 0x05, 0x00, 0x00, 0x00, 0x83, 0xC0, 0x10, 0xC3};
    cs_insn* insn = nullptr;
    const std::size_t count = cs.cs_disasm(handle, code, sizeof(code), 0x1000, 0, &insn);
    CB_CHECK_EQ(count, 3u);
    if (count == 3) {
        CB_CHECK_EQ(std::string(insn[0].mnemonic), "mov");
        CB_CHECK_EQ(std::string(insn[0].op_str), "eax, 5");
        CB_CHECK_EQ(insn[1].address, 0x1005u);
        CB_CHECK_EQ(std::string(insn[2].mnemonic), "ret");
        cs.cs_free(insn, count);
    }
    csh unused = 0;
    CB_CHECK_THROWS_AS(Error, disasm::capstone::check(cs.cs_open(static_cast<cs_arch>(99), CS_MODE_32, &unused), "cs_open"));
    disasm::capstone::check(cs.cs_close(&handle), "cs_close");
});

CB_TEST("core.z3.solves_a_bitvector_equation", [] {
    const auto& z = z3::api();
    CB_CHECK(!z3::version_string().empty());
    Z3_config cfg = z.Z3_mk_config();
    Z3_context ctx = z.Z3_mk_context(cfg);
    z.Z3_del_config(cfg);
    Z3_sort bv32 = z.Z3_mk_bv_sort(ctx, 32);
    Z3_ast x = z.Z3_mk_const(ctx, z.Z3_mk_string_symbol(ctx, "x"), bv32);
    Z3_ast one = z.Z3_mk_unsigned_int64(ctx, 1, bv32);
    Z3_ast five = z.Z3_mk_unsigned_int64(ctx, 5, bv32);
    Z3_solver solver = z.Z3_mk_solver(ctx);
    z.Z3_solver_inc_ref(ctx, solver);
    z.Z3_solver_assert(ctx, solver, z.Z3_mk_eq(ctx, z.Z3_mk_bvadd(ctx, x, one), five));   // x + 1 == 5
    CB_CHECK_EQ(static_cast<int>(z.Z3_solver_check(ctx, solver)), static_cast<int>(Z3_L_TRUE));
    Z3_model model = z.Z3_solver_get_model(ctx, solver);
    z.Z3_model_inc_ref(ctx, model);
    Z3_ast value = nullptr;
    CB_CHECK(z.Z3_model_eval(ctx, model, x, true, &value));
    std::uint64_t result = 0;
    CB_CHECK(z.Z3_get_numeral_uint64(ctx, value, &result));
    CB_CHECK_EQ(result, 4u);
    // adding a contradiction makes it unsatisfiable
    z.Z3_solver_assert(ctx, solver, z.Z3_mk_bvugt(ctx, x, five));
    CB_CHECK_EQ(static_cast<int>(z.Z3_solver_check(ctx, solver)), static_cast<int>(Z3_L_FALSE));
    z.Z3_model_dec_ref(ctx, model);
    z.Z3_solver_dec_ref(ctx, solver);
    z.Z3_del_context(ctx);
});
