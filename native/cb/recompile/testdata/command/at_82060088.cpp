#include "rex_ppc_min.h"

extern PPC_FUNC(sub_82061438);
extern PPC_FUNC(sub_8208BD48);
extern PPC_FUNC(sub_822864A8);

PPC_FUNC_IMPL(sub_82060088) {
	PPC_FUNC_PROLOGUE();
	uint32_t ea{};
	PPCRegister temp{};
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12, -8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// stwu r1, -0x60(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// addi r3, r1, 0x50
	ctx.r3.s64 = ctx.r1.s64 + 80;
	// bl 0x82061438
	ctx.lr = 0x8206009C;
	sub_82061438(ctx, base);
	// lis r9, -0x7dd5
	ctx.r9.s64 = -2111111168;
	// ld r10, 0x50(r1)
	ctx.r10.u64 = PPC_LOAD_U64(ctx.r1.u32 + 80);
	// addi r8, r9, 0x7940
	ctx.r8.s64 = ctx.r9.s64 + 31040;
	// ld r11, 0x7940(r9)
	ctx.r11.u64 = PPC_LOAD_U64(ctx.r9.u32 + 31040);
	// std r10, 0x7940(r9)
	PPC_STORE_U64(ctx.r9.u32 + 31040, ctx.r10.u64);
	// subf r11, r11, r10
	ctx.r11.u64 = ctx.r10.u64 - ctx.r11.u64;
	// lfs f0, 0x18(r8)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r8.u32 + 24);
	ctx.f0.f64 = double(temp.f32);
	// std r11, 0x50(r1)
	PPC_STORE_U64(ctx.r1.u32 + 80, ctx.r11.u64);
	// lfd f13, 0x50(r1)
	ctx.fpscr.disableFlushMode();
	ctx.f13.u64 = PPC_LOAD_U64(ctx.r1.u32 + 80);
	// fcfid f12, f13
	ctx.fpscr.disableFlushMode();
	ctx.f12.f64 = double(ctx.f13.s64);
	// ld r9, 8(r8)
	ctx.r9.u64 = PPC_LOAD_U64(ctx.r8.u32 + 8);
	// frsp f12, f12
	ctx.fpscr.disableFlushMode();
	ctx.f12.f64 = double(float(ctx.f12.f64));
	// add r11, r9, r11
	ctx.r11.u64 = ctx.r9.u64 + ctx.r11.u64;
	// std r11, 8(r8)
	PPC_STORE_U64(ctx.r8.u32 + 8, ctx.r11.u64);
	// lfd f13, 8(r8)
	ctx.fpscr.disableFlushMode();
	ctx.f13.u64 = PPC_LOAD_U64(ctx.r8.u32 + 8);
	// fcfid f13, f13
	ctx.fpscr.disableFlushMode();
	ctx.f13.f64 = double(ctx.f13.s64);
	// frsp f11, f13
	ctx.fpscr.disableFlushMode();
	ctx.f11.f64 = double(float(ctx.f13.f64));
	// fmuls f13, f12, f0
	ctx.fpscr.disableFlushMode();
	ctx.f13.f64 = double(float(ctx.f12.f64 * ctx.f0.f64));
	// stfs f13, 0x14(r8)
	ctx.fpscr.disableFlushMode();
	temp.f32 = float(ctx.f13.f64);
	PPC_STORE_U32(ctx.r8.u32 + 20, temp.u32);
	// fmuls f0, f11, f0
	ctx.fpscr.disableFlushMode();
	ctx.f0.f64 = double(float(ctx.f11.f64 * ctx.f0.f64));
	// stfs f0, 0x10(r8)
	ctx.fpscr.disableFlushMode();
	temp.f32 = float(ctx.f0.f64);
	PPC_STORE_U32(ctx.r8.u32 + 16, temp.u32);
	// addi r1, r1, 0x60
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12, -8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// blr
	return;
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12, -8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31, -0x10(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// stwu r1, -0x60(r1)
	ea = -96 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// mr r31, r3
	ctx.r31.u64 = ctx.r3.u64;
	// lwz r3, 0(r3)
	ctx.r3.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0);
	// cmplwi cr6, r3, 0
	ctx.cr6.compare<uint32_t>(ctx.r3.u32, 0, ctx.xer);
	// beq cr6, 0x82060138
	if (ctx.cr6.eq) { goto loc_82060138; }
	// lwz r11, 0(r3)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r3.u32 + 0);
	// lwz r11, 8(r11)
	ctx.r11.u64 = PPC_LOAD_U32(ctx.r11.u32 + 8);
	// mtctr r11
	ctx.ctr.u64 = ctx.r11.u64;
	// bctrl
	ctx.lr = 0x82060130;
	PPC_CALL_INDIRECT_FUNC(ctx.ctr.u32);
	// li r11, 0
	ctx.r11.s64 = 0;
	// stw r11, 0(r31)
	PPC_STORE_U32(ctx.r31.u32 + 0, ctx.r11.u32);
loc_82060138:
	// addi r1, r1, 0x60
	ctx.r1.s64 = ctx.r1.s64 + 96;
	// lwz r12, -8(r1)
	ctx.r12.u64 = PPC_LOAD_U32(ctx.r1.u32 + -8);
	// mtlr r12
	ctx.lr = ctx.r12.u64;
	// ld r31, -0x10(r1)
	ctx.r31.u64 = PPC_LOAD_U64(ctx.r1.u32 + -16);
	// blr
	return;
	// .long 0x00000000
	PPC_UNIMPLEMENTED(".long 0x00000000");
	// mflr r12
	ctx.r12.u64 = ctx.lr;
	// stw r12, -8(r1)
	PPC_STORE_U32(ctx.r1.u32 + -8, ctx.r12.u32);
	// std r31, -0x10(r1)
	PPC_STORE_U64(ctx.r1.u32 + -16, ctx.r31.u64);
	// addi r12, r1, -0x10
	ctx.r12.s64 = ctx.r1.s64 + -16;
	// bl 0x822864a8
	ctx.lr = 0x82060164;
	sub_822864A8(ctx, base);
	// stwu r1, -0x140(r1)
	ea = -320 + ctx.r1.u32;
	PPC_STORE_U32(ea, ctx.r1.u32);
	ctx.r1.u32 = ea;
	// lis r11, -0x7dd5
	ctx.r11.s64 = -2111111168;
	// lis r10, -0x7e00
	ctx.r10.s64 = -2113929216;
	// addi r31, r11, 0x7940
	ctx.r31.s64 = ctx.r11.s64 + 31040;
	// addi r3, r1, 0xd0
	ctx.r3.s64 = ctx.r1.s64 + 208;
	// lfs f0, 0x17e8(r10)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r10.u32 + 6120);
	ctx.f0.f64 = double(temp.f32);
	// lfs f31, 0x10(r31)
	ctx.fpscr.disableFlushMode();
	temp.u32 = PPC_LOAD_U32(ctx.r31.u32 + 16);
	ctx.f31.f64 = double(temp.f32);
	// fmuls f1, f31, f0
	ctx.fpscr.disableFlushMode();
	ctx.f1.f64 = double(float(ctx.f31.f64 * ctx.f0.f64));
	// bl 0x8208bd48
	ctx.lr = 0x82060188;
	sub_8208BD48(ctx, base);
}
