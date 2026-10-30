// Structured exception dispatch (RtlRaiseException), following the NT RISC kernel the Xbox 360 kernel derives from.
//
// RtlRaiseException captures the caller's CONTEXT, then walks the guest call frames: each frame's function is looked
// up in .pdata; a function whose .pdata entry has the exception flag stores its language handler and handler data in
// the two words before its first instruction. The kernel's own __C_specific_handler (xboxkrnl ordinal 421, imported
// by the title) is the handler of every SEH function: it matches the scope table against the frame's control PC and
// runs the title's filter funclets. A filter returning EXCEPTION_CONTINUE_EXECUTION (-1) resumes after the raise
// with the (possibly modified) context; EXCEPTION_CONTINUE_SEARCH (0) keeps searching.
//
// RtlUnwind does the local unwind that leaves a __try early (its comment, at the end).
//
// Not implemented yet, and stopped with a message when reached: unwinding to an __except block (a filter returning
// EXCEPTION_EXECUTE_HANDLER), frames handled by other language handlers (C++ __CxxFrameHandler), modifications of
// the resume address or stack pointer by a filter, and RtlUnwind through more than one frame.
//
// Details established from the title's code:
// - Scope records are {Begin, End, Filter, JumpTarget} with absolute addresses; End is the return address of the
//   last call in the guarded range, so a call frame's control PC is its return address - 4.
// - Filter funclets take the EXCEPTION_POINTERS in r3 and rebuild their parent's frame from r12 (e.g. a parent with
//   `addi r31,r1,-320; stwu r1,-320(r1)` has a filter doing `addi r31,r12,-320`): r12 is the establisher frame,
//   the function's incoming stack pointer. As in NT, r4 also carries it.
// - Every non-leaf function saves LR either through __savegprlr_* (at incoming SP - 8) or with `stw r12,d(r1)`
//   in its prologue (.pdata gives the prologue length in instructions), and its stwu leaves the back chain at [SP].
//
// Layouts (XDK winnt.h): EXCEPTION_RECORD: code +0, flags +4, record +8, address +0xC, NumberParameters +0x10,
// ExceptionInformation[15] +0x14. CONTEXT (0xA40): ContextFlags +0, Msr +4, Iar +8, Lr +0xC, Ctr +0x10 (u64),
// Gpr0-31 +0x18 (u64), Cr +0x118, Xer +0x11C, Fpscr +0x120 (double), Fpr0-31 +0x128, UserModeControl +0x228,
// Vscr +0x230, Vr0-127 +0x240.

#include "kernel/kernel.h"
#include "modules.h"

#include <cstring>
#include <vector>

namespace
{
    // __savegprlr_14 .. _31: 18 std, stw r12, blr.
    constexpr uint32_t kSaveGprLrSize = 0x50;

    constexpr uint32_t kContextSize = 0xA40;
    constexpr uint32_t kContextFull = 0x17; // CONTROL | FLOATING_POINT | INTEGER | VECTOR
    constexpr uint32_t kExceptionNoncontinuable = 0x1;
    constexpr int32_t kExceptionContinueExecution = -1;

    struct FunctionEntry
    {
        uint32_t begin = 0;
        uint32_t end = 0;
        uint32_t prologInstructions = 0;
        bool hasHandler = false;
        uint32_t saveGprLr14 = 0; // the module's __savegprlr_14
    };

    bool LookupFunction(uint32_t pc, FunctionEntry& entry)
    {
        auto& mem = guest::g_memory;
        // Each recompiled module has its own .pdata.
        const modules::LoadedModule* module = modules::FindByCode(pc);
        if (module == nullptr)
            return false;
        const uint32_t pdata = module->pdata;
        uint32_t lo = 0, hi = module->pdataSize / 8;
        while (lo < hi) // .pdata is sorted by start address
        {
            const uint32_t mid = (lo + hi) / 2;
            const uint32_t begin = mem.Read32(pdata + mid * 8);
            const uint32_t packed = mem.Read32(pdata + mid * 8 + 4);
            const uint32_t end = begin + ((packed >> 8) & 0x3FFFFF) * 4;
            if (pc < begin)
                hi = mid;
            else if (pc >= end)
                lo = mid + 1;
            else
            {
                entry = { begin, end, packed & 0xFF, (packed >> 31) != 0, module->recompiled->saveGprLr14 };
                return true;
            }
        }
        return false;
    }

    // Where the function saved its return address, relative to its incoming stack pointer.
    bool SavedLrOffset(const FunctionEntry& f, int32_t& offset)
    {
        auto& mem = guest::g_memory;
        for (uint32_t i = 0; i < f.prologInstructions; i++)
        {
            const uint32_t a = f.begin + i * 4;
            const uint32_t w = mem.Read32(a);
            if ((w & 0xFFFF0000) == 0x91810000) // stw r12,d(r1)
            {
                offset = int16_t(w & 0xFFFF);
                return true;
            }
            if ((w & 0xFC000003) == 0x48000001) // bl
            {
                int32_t disp = int32_t(w & 0x03FFFFFC);
                if (disp & 0x02000000)
                    disp -= 0x04000000;
                const uint32_t target = a + disp;
                if (f.saveGprLr14 != 0 && target >= f.saveGprLr14 && target < f.saveGprLr14 + kSaveGprLrSize)
                {
                    offset = -8;
                    return true;
                }
            }
        }
        return false;
    }

    // CTR, XER, CR and MSR are locals of the recompiled code (ppc_shared/ppc_config.h): a CONTEXT shows
    // them as 0 and changes a filter makes to them are not applied. r14-r31 / f14-f31 / v14+ are in the context for
    // the functions that use SEH (XenonRecomp FindNonVolatileInContext), which are the ones that raise and filter.
    PPCRegister& Gpr(PPCContext& c, int i)
    {
        PPCRegister* r[] = { &c.r0, &c.r1, &c.r2, &c.r3, &c.r4, &c.r5, &c.r6, &c.r7, &c.r8, &c.r9, &c.r10, &c.r11,
            &c.r12, &c.r13, &c.r14, &c.r15, &c.r16, &c.r17, &c.r18, &c.r19, &c.r20, &c.r21, &c.r22, &c.r23, &c.r24,
            &c.r25, &c.r26, &c.r27, &c.r28, &c.r29, &c.r30, &c.r31 };
        return *r[i];
    }

    PPCRegister& Fpr(PPCContext& c, int i)
    {
        PPCRegister* f[] = { &c.f0, &c.f1, &c.f2, &c.f3, &c.f4, &c.f5, &c.f6, &c.f7, &c.f8, &c.f9, &c.f10, &c.f11,
            &c.f12, &c.f13, &c.f14, &c.f15, &c.f16, &c.f17, &c.f18, &c.f19, &c.f20, &c.f21, &c.f22, &c.f23, &c.f24,
            &c.f25, &c.f26, &c.f27, &c.f28, &c.f29, &c.f30, &c.f31 };
        return *f[i];
    }

    PPCVRegister& Vr(PPCContext& c, int i)
    {
        return (&c.v0)[i];
    }

    // Host vector registers hold the guest's bytes in reverse order (as the recompiled lvx/stvx do).
    void StoreVector(uint32_t address, const PPCVRegister& v)
    {
        for (int b = 0; b < 16; b++)
            guest::g_memory.Write8(address + b, v.u8[15 - b]);
    }

    void LoadVector(uint32_t address, PPCVRegister& v)
    {
        for (int b = 0; b < 16; b++)
            v.u8[15 - b] = guest::g_memory.Read8(address + b);
    }

    // CONTEXT of the caller of RtlRaiseException: it resumes at the return address.
    void CaptureContext(PPCContext& c, uint32_t context)
    {
        auto& mem = guest::g_memory;
        std::memset(mem.Translate(context), 0, kContextSize);
        mem.Write32(context + 0x00, kContextFull);
        mem.Write32(context + 0x04, 0x200A000); // user-mode MSR (not tracked: skip_msr)
        mem.Write32(context + 0x08, uint32_t(c.lr));
        mem.Write32(context + 0x0C, uint32_t(c.lr));
        for (int i = 0; i < 32; i++)
            mem.Write64(context + 0x18 + i * 8, Gpr(c, i).u64);
        mem.Write64(context + 0x120, c.fpscr.loadFromHost());
        for (int i = 0; i < 32; i++)
            mem.Write64(context + 0x128 + i * 8, Fpr(c, i).u64);
        for (int i = 0; i < 128; i++)
            StoreVector(context + 0x240 + i * 16, Vr(c, i));
    }

    // NtContinue with the captured context: the registers a filter may have changed take effect.
    void RestoreContext(PPCContext& c, uint32_t context)
    {
        auto& mem = guest::g_memory;
        for (int i = 0; i < 32; i++)
            Gpr(c, i).u64 = mem.Read64(context + 0x18 + i * 8);
        c.lr = mem.Read32(context + 0x0C);
        c.fpscr.storeFromGuest(uint32_t(mem.Read64(context + 0x120)));
        for (int i = 0; i < 32; i++)
            Fpr(c, i).u64 = mem.Read64(context + 0x128 + i * 8);
        for (int i = 0; i < 128; i++)
            LoadVector(context + 0x240 + i * 16, Vr(c, i));
    }

    // __C_specific_handler while dispatching: returns the first filter verdict that is not "continue search".
    // pointers: guest EXCEPTION_POINTERS; stack: stack pointer for calling filters.
    int32_t SpecificHandler(PPCContext& ctx, const FunctionEntry& f, uint32_t controlPc, uint32_t establisher,
        uint32_t pointers, uint32_t stack, uint32_t& target)
    {
        auto& mem = guest::g_memory;
        const uint32_t scopes = mem.Read32(f.begin - 4);
        const uint32_t count = mem.Read32(scopes);
        for (uint32_t i = 0; i < count; i++)
        {
            const uint32_t record = scopes + 4 + i * 16;
            const uint32_t begin = mem.Read32(record), end = mem.Read32(record + 4);
            const uint32_t filter = mem.Read32(record + 8), jumpTarget = mem.Read32(record + 12);
            if (controlPc < begin || controlPc >= end || jumpTarget == 0) // jumpTarget 0: a __finally block
                continue;
            int32_t value = 1; // EXCEPTION_EXECUTE_HANDLER may be stored instead of a filter
            if (filter != 1)
            {
                PPCContext call = ctx;
                call.r1.u64 = stack;
                call.r3.u64 = pointers;
                call.r4.u64 = establisher;
                call.r12.u64 = establisher;
                kernel::CallGuest(call, filter);
                value = int32_t(call.r3.u32);
            }
            if (value != 0)
            {
                target = jumpTarget;
                return value;
            }
        }
        return 0;
    }

    // __C_specific_handler while unwinding: the frame's __finally blocks to run, innermost first (scope records whose
    // JumpTarget is 0; the Filter word is the termination handler) that hold controlPc and, in the target frame, not
    // targetIp (pass 0 for a frame being unwound through).
    std::vector<uint32_t> TerminationHandlers(const FunctionEntry& f, uint32_t controlPc, uint32_t targetIp)
    {
        auto& mem = guest::g_memory;
        std::vector<uint32_t> handlers;
        if (!f.hasHandler)
            return handlers;
        const uint32_t scopes = mem.Read32(f.begin - 4);
        const uint32_t count = mem.Read32(scopes);
        for (uint32_t i = 0; i < count; i++)
        {
            const uint32_t record = scopes + 4 + i * 16;
            const uint32_t begin = mem.Read32(record), end = mem.Read32(record + 4);
            if (controlPc < begin || controlPc >= end || mem.Read32(record + 12) != 0)
                continue;
            if (targetIp >= begin && targetIp < end) // the target is inside this __try: it is not left
                continue;
            handlers.push_back(mem.Read32(record + 8));
        }
        return handlers;
    }

    // A termination handler takes AbnormalTermination (TRUE) in r3 and the establisher frame in r4 and r12.
    void RunTerminationHandlers(PPCContext& ctx, const std::vector<uint32_t>& handlers, uint32_t establisher,
        uint32_t stack)
    {
        for (const uint32_t handler : handlers)
        {
            PPCContext call = ctx;
            call.r1.u64 = stack;
            call.r3.u64 = 1;
            call.r4.u64 = establisher;
            call.r12.u64 = establisher;
            kernel::CallGuest(call, handler);
        }
    }
}

// VOID RtlCaptureContext(PCONTEXT ContextRecord): the caller's registers; it resumes at the return address.
GUEST_IMPORT(RtlCaptureContext)
{
    CaptureContext(ctx, ctx.r3.u32);
}

// VOID RtlRaiseException(PEXCEPTION_RECORD ExceptionRecord)
GUEST_IMPORT(RtlRaiseException)
{
    auto& mem = guest::g_memory;
    const uint32_t record = ctx.r3.u32;
    const uint32_t code = mem.Read32(record);
    const kernel::GuestThread* thread = kernel::CurrentThread();

    // Kernel frame on the guest stack: CONTEXT, EXCEPTION_POINTERS, then room for calling filters below it.
    const uint32_t sp = ctx.r1.u32;
    const uint32_t context = (sp - kContextSize - 0x20) & ~0xFu;
    const uint32_t pointers = context + kContextSize;
    const uint32_t stack = context - 0x100;
    if (stack < thread->stackLimit)
        Fatal("RtlRaiseException {:08X}: no stack left for dispatch", code);
    CaptureContext(ctx, context);
    mem.Write32(stack, sp); // back chain
    mem.Write32(pointers + 0, record);
    mem.Write32(pointers + 4, context);

    const uint32_t handlerThunk = kernel::LookupImportThunk("xboxkrnl.exe", 421); // __C_specific_handler
    uint32_t controlPc = uint32_t(ctx.lr) - 4;
    uint32_t frame = sp; // stack pointer while that frame's function runs
    for (;;)
    {
        FunctionEntry f;
        if (frame < thread->stackLimit || frame >= thread->stackBase || !LookupFunction(controlPc, f))
            Fatal("Unhandled exception {:08X} (flags {:08X}, address {:08X}): no handler on the stack (last pc {:08X})",
                code, mem.Read32(record + 4), mem.Read32(record + 0xC), controlPc);
        const uint32_t establisher = mem.Read32(frame); // back chain: the function's incoming stack pointer

        if (f.hasHandler)
        {
            const uint32_t handler = mem.Read32(f.begin - 8);
            if (handler != handlerThunk)
                Fatal("Exception {:08X} reached frame {:08X} (pc {:08X}) handled by {:08X}: only __C_specific_handler "
                      "frames are supported", code, f.begin, controlPc, handler);
            uint32_t target = 0;
            const int32_t verdict = SpecificHandler(ctx, f, controlPc, establisher, pointers, stack, target);
            if (verdict < 0)
            {
                if (mem.Read32(record + 4) & kExceptionNoncontinuable)
                    Fatal("Exception {:08X}: a filter continued a noncontinuable exception", code);
                if (mem.Read32(context + 0x08) != uint32_t(ctx.lr) || uint32_t(mem.Read64(context + 0x18 + 8)) != sp)
                    Fatal("Exception {:08X}: a filter changed the resume address or stack pointer", code);
                RestoreContext(ctx, context);
                return;
            }
            if (verdict > 0)
                Fatal("Exception {:08X}: unwinding to the __except block at {:08X} (frame {:08X}) is not implemented",
                    code, target, f.begin);
        }

        int32_t lrOffset;
        if (!SavedLrOffset(f, lrOffset))
            Fatal("Exception {:08X}: cannot unwind through {:08X} (no saved return address in its prologue)", code, f.begin);
        controlPc = mem.Read32(establisher + lrOffset) - 4;
        frame = establisher;
    }
}

// VOID RtlUnwind(PVOID TargetFrame, PVOID TargetIp, PEXCEPTION_RECORD ExceptionRecord, PVOID ReturnValue)
//
// The local unwind a title's code makes to leave a __try early (the CRT's _local_unwind: RtlUnwind(establisher,
// continuation, NULL, NULL), e.g. 1888's 0x92821050): the __finally blocks between the call and TargetIp run, then
// the target function continues at TargetIp with ReturnValue in r3. Recompiled code cannot be resumed at an
// arbitrary address, so the unwind returns: through RtlUnwind's caller (a wrapper with no __finally of its own),
// back into the target function, whose code after the call goes on to TargetIp. Deeper unwinds (frames between the
// wrapper and the target, as a C++ catch or an __except reached by dispatch needs) stop with a message.
GUEST_IMPORT(RtlUnwind)
{
    auto& mem = guest::g_memory;
    const uint32_t targetFrame = ctx.r3.u32, targetIp = ctx.r4.u32, returnValue = ctx.r6.u32;
    const kernel::GuestThread* thread = kernel::CurrentThread();
    const uint32_t handlerThunk = kernel::LookupImportThunk("xboxkrnl.exe", 421); // __C_specific_handler

    const uint32_t sp = ctx.r1.u32;
    const uint32_t stack = (sp - 0x100) & ~0xFu; // room for calling termination handlers
    if (stack < thread->stackLimit)
        Fatal("RtlUnwind to {:08X}: no stack left", targetIp);
    mem.Write32(stack, sp); // back chain

    FunctionEntry caller, target;
    const uint32_t controlPc = uint32_t(ctx.lr) - 4;
    if (!LookupFunction(controlPc, caller) || !LookupFunction(targetIp, target))
        Fatal("RtlUnwind from {:08X} to {:08X}: no .pdata entry", controlPc, targetIp);
    for (const FunctionEntry* f : { &caller, &target })
        if (f->hasHandler && mem.Read32(f->begin - 8) != handlerThunk)
            Fatal("RtlUnwind to {:08X}: frame {:08X} is not a __C_specific_handler frame", targetIp, f->begin);

    const uint32_t callerEstablisher = mem.Read32(sp);
    if (callerEstablisher == targetFrame) // called by the target function itself
    {
        if (caller.begin != target.begin)
            Fatal("RtlUnwind to {:08X}: frame {:08X} is not the target's function", targetIp, targetFrame);
        RunTerminationHandlers(ctx, TerminationHandlers(target, controlPc, targetIp), targetFrame, stack);
        ctx.r3.u64 = returnValue;
        return;
    }
    if (mem.Read32(callerEstablisher) != targetFrame)
        Fatal("RtlUnwind from {:08X} to {:08X} (frame {:08X}): unwinding through more than one frame is not "
              "implemented", controlPc, targetIp, targetFrame);
    if (!TerminationHandlers(caller, controlPc, 0).empty())
        Fatal("RtlUnwind from {:08X}: the wrapper {:08X} has __finally blocks of its own", controlPc, caller.begin);

    // The wrapper keeps its return address in a register (mflr r31), not in its frame, so the call into it is found
    // in the target function's code: every `bl wrapper` must leave the same __finally blocks.
    std::vector<uint32_t> handlers;
    bool found = false;
    for (uint32_t a = target.begin; a < target.end; a += 4)
    {
        const uint32_t w = mem.Read32(a);
        if ((w & 0xFC000003) != 0x48000001)
            continue;
        int32_t disp = int32_t(w & 0x03FFFFFC);
        if (disp & 0x02000000)
            disp -= 0x04000000;
        if (a + disp != caller.begin)
            continue;
        std::vector<uint32_t> these = TerminationHandlers(target, a, targetIp);
        if (found && these != handlers)
            Fatal("RtlUnwind to {:08X}: the calls to {:08X} in {:08X} leave different __finally blocks", targetIp,
                caller.begin, target.begin);
        handlers = std::move(these);
        found = true;
    }
    if (!found)
        Fatal("RtlUnwind to {:08X}: {:08X} does not call {:08X}", targetIp, target.begin, caller.begin);
    RunTerminationHandlers(ctx, handlers, targetFrame, stack);
    ctx.r3.u64 = returnValue;
}
