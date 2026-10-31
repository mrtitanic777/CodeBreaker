#!/usr/bin/env python3
"""dpmi.py -- a DPMI host for the real-mode DOS executive (exact/emu/dos.py), so the period tools
that enter 32-bit protected mode through a DPMI host (the Win95 DDK's Phar Lap TNT ML.EXE; a DOS
-extended Microsoft C pass) run on CodeBreaker's own Unicorn CPU instead of DOSBox-X.

It plugs into dos.run() as `dpmi=True`: dos.run builds a Host, hands it the engine and the real
-mode INT 21h handler, and routes INT 2Fh/1687h, INT 31h and the mode-switch to it. Everything runs
in the SINGLE UC_MODE_16 engine dos.run already uses -- Unicorn (QEMU) honours the descriptor D-bit
in protected mode, so that one engine decodes real-mode code, the 16-bit protected-mode stub and the
32-bit protected-mode extender correctly; only the real->protected transition needs care, and that is
done the hardware way (a host-written trampoline runs `mov cr0` + a far jump), which is the one method
that leaves QEMU's cached segment state consistent.

What the host provides:
  * INT 2Fh AX=1687h   report a DPMI host; hand back the mode-switch entry (ES:DI), 32-bit support,
                       processor and version, and the host-data paragraph count.
  * the mode-switch     on the client's FAR CALL to the entry: build a GDT (host stub + flat data +
                       the LDT) and an LDT (selectors for the client's CS/DS/SS and its PSP), set
                       GDTR/LDTR, and run a trampoline that turns on CR0.PE and jumps into the client
                       now in protected mode.
  * INT 31h services    0000/0001 allocate/free LDT descriptors; 0003 selector increment; 0006/0007
                       get/set base; 0008 set limit; 0009 set access; 000A alias; 000B/000C get/set
                       descriptor; 0100/0101 allocate/free DOS memory; 0200-0205 real-mode vectors;
                       0300 simulate a real-mode interrupt (this is how a protected client reaches the
                       INT 21h file API -- it is routed straight back through dos.run's int21); 0400
                       version; 0500/0501/0502/0503 memory info/allocate/free/resize (extended memory
                       above 1 MB); 0600/0601 lock (a no-op here -- no paging); 0E00/0E01 the 80x87
                       state. A service not implemented raises, so the whole run falls back to DOSBox.

A selector is keyed by `sel & 0xFFFC` (RPL stripped, the LDT bit kept) in st["desc"] so dos.py's
lin() turns a protected-mode (selector:offset) address into the same linear byte the real-mode side
sees -- the identity the reflected INT 21h calls rely on.
"""
import struct

from unicorn import UcError, UC_PROT_ALL
from unicorn.x86_const import (
    UC_X86_REG_AX, UC_X86_REG_BX, UC_X86_REG_CX, UC_X86_REG_DX, UC_X86_REG_SI, UC_X86_REG_DI,
    UC_X86_REG_BP, UC_X86_REG_SP, UC_X86_REG_IP, UC_X86_REG_CS, UC_X86_REG_DS, UC_X86_REG_ES,
    UC_X86_REG_SS, UC_X86_REG_EAX, UC_X86_REG_EBX, UC_X86_REG_ECX, UC_X86_REG_EDX, UC_X86_REG_ESI,
    UC_X86_REG_EDI, UC_X86_REG_EBP, UC_X86_REG_GDTR, UC_X86_REG_LDTR, UC_X86_REG_EFLAGS)

# --- the host's memory, all linear (the real-mode program keeps 0..1 MB) -----------------------
ENTRY = 0x00000660          # the 2-byte INT-marker the mode-switch entry (ES:DI) points at
TRAMP = 0x00000680          # the real-mode -> protected-mode trampoline (hardware switch)
HOST = 0x00110000           # host tables, just above the real-mode megabyte
GDT = HOST + 0x0000         # global descriptor table
LDT = HOST + 0x0800         # local descriptor table (the DPMI descriptors)
STUB = HOST + 0x1000        # the protected-mode landing stub (loads the client's data selectors)
POOL = 0x00200000           # extended-memory pool (INT 31h 0501) base
POOLTOP = 0x01000000        # ... and top (16 MB)
HOSTTOP = POOLTOP           # map 0x110000..HOSTTOP as fresh host RAM

SEL_STUBCODE = 0x08         # GDT: 16-bit code, base = STUB (the landing stub)
SEL_FLATDATA = 0x10         # GDT: flat 32-bit data (host use / client flat access)
SEL_LDT = 0x18              # GDT: the LDT descriptor -> LDTR
LDT_FIRST = 1               # first usable LDT index (selector (idx<<3)|4)
LDT_COUNT = 512             # LDT entries


def _desc(base, limit, access, gran):
    """one 8-byte descriptor; limit > 1 MB is taken as byte-granular and scaled to 4 KB pages."""
    if limit > 0xFFFFF:
        limit >>= 12
        gran |= 0x80
    return struct.pack("<Q", (limit & 0xFFFF) | ((base & 0xFFFFFF) << 16) | ((access & 0xFF) << 40)
                       | (((limit >> 16) & 0xF) << 48) | ((gran & 0xF0) << 48)
                       | (((base >> 24) & 0xFF) << 56))


class Host(object):
    def __init__(self):
        self.ctx = None
        self.pm = False
        self.ldt_used = {}                       # LDT index -> (base, limit, access)
        self.next_ldt = LDT_FIRST
        self.vectors = {}                         # real-mode vector overrides set via INT 31h 0200/0201
        self.mem = {}                             # handle -> (linear base, bytes) of 0501 allocations
        self.pool = POOL
        self.next_handle = 1
        self.scratch_ds = None                    # two reusable selectors for INT 31h 0300 reflection
        self.scratch_es = None

    # -- wiring from dos.run ---------------------------------------------------------------------
    def attach(self, ctx):
        """ctx exposes the engine and the real-mode INT 21h handler: .mu .st .int21 .R .W .carry
        .alloc .largest .load .reg (the module's int-21 register readers/writers)."""
        self.ctx = ctx
        mu = ctx.mu
        try:
            mu.mem_map(HOST, HOSTTOP - HOST, UC_PROT_ALL)     # host tables + extended pool
        except UcError:
            pass                                               # already mapped (a re-attach)
        mu.mem_write(ENTRY, b"\xCD\xFF")                       # the mode-switch entry: INT FFh (we trap it)
        ctx.st["desc"] = ctx.st.get("desc", {})

    # -- the interrupt surface dos.run routes here -----------------------------------------------
    def handle(self, intno):
        """True if this interrupt is a DPMI one and was serviced."""
        if intno == 0xFF:                                      # the mode-switch entry (our INT marker)
            self._switch()
            return True
        R = self.ctx.R
        if intno == 0x2F and (R(UC_X86_REG_AX) & 0xFFFF) == 0x1687:
            self._int2f_1687()
            return True
        if intno == 0x31:
            self._int31()
            return True
        return False

    # -- INT 2Fh AX=1687h: here is a DPMI host --------------------------------------------------
    def _int2f_1687(self):
        W = self.ctx.W
        W(UC_X86_REG_AX, 0x0000)        # 0: DPMI available
        W(UC_X86_REG_BX, 0x0001)        # bit 0: 32-bit programs supported
        W(UC_X86_REG_CX, 0x0003)        # 80386
        W(UC_X86_REG_DX, 0x005A)        # DPMI version 0.90
        W(UC_X86_REG_SI, 0x0000)        # paragraphs of host private data the client must provide: none
        W(UC_X86_REG_ES, ENTRY >> 4)    # ES:DI = the mode-switch entry
        W(UC_X86_REG_DI, ENTRY & 0xF)

    # -- the real-mode -> protected-mode switch (the client FAR CALLs the entry) -----------------
    def _switch(self):
        mu, st, R, W = self.ctx.mu, self.ctx.st, self.ctx.R, self.ctx.W
        ss, sp = R(UC_X86_REG_SS), R(UC_X86_REG_SP)
        ret_ip, ret_cs = struct.unpack_from("<HH", mu.mem_read(ss * 16 + sp, 4))   # the FAR-CALL frame
        ds = R(UC_X86_REG_DS)       # ES at entry is the host-data block (DPMI); the PSP is the executive's
        # the client is a 16-bit DPMI client: AX bit 0 = 0. (A 32-bit client sets bit 0; the extender
        # it loads then runs 32-bit, decoded via its own D=1 code descriptors -- handled the same way.)
        # Build the GDT (host stub + flat data + the LDT descriptor).
        gdt = bytearray(b"\x00" * 8)
        gdt += _desc(STUB, 0xFFFF, 0x9A, 0x00)                 # SEL_STUBCODE 0x08: 16-bit code at STUB
        gdt += _desc(0, 0xFFFFFFFF, 0x92, 0xC0)                # SEL_FLATDATA 0x10: flat 32-bit data
        gdt += _desc(LDT, LDT_COUNT * 8 - 1, 0x82, 0x00)       # SEL_LDT 0x18: the LDT
        mu.mem_write(GDT, bytes(gdt))
        mu.mem_write(LDT, b"\x00" * (LDT_COUNT * 8))
        self.ldt_used = {}
        self.next_ldt = LDT_FIRST
        # the client's initial selectors: code, data, stack, PSP -- each base = its real-mode seg<<4.
        sel_cs = self._make_ldt(ret_cs * 16, 0xFFFF, 0x9A)     # 16-bit code
        sel_ds = self._make_ldt(ds * 16, 0xFFFF, 0x92)
        sel_ss = self._make_ldt(ss * 16, 0xFFFF, 0x92)
        sel_psp = self._make_ldt(self.ctx.psp * 16, 0xFF, 0x92)
        mu.reg_write(UC_X86_REG_GDTR, (0, GDT, len(gdt) - 1, 0))
        mu.reg_write(UC_X86_REG_LDTR, (SEL_LDT, LDT, LDT_COUNT * 8 - 1, 0x82))
        # the protected-mode landing stub (at STUB, reached through SEL_STUBCODE): load the client's
        # data selectors, set the stack to just past the far-call frame, far-jump to the client.
        stub = (b"\x66\xB8" + struct.pack("<H", sel_ds) + b"\x00\x00" + b"\x8E\xD8"        # mov eax,ds_sel; mov ds,ax
                + b"\x66\xB8" + struct.pack("<H", sel_ss) + b"\x00\x00" + b"\x8E\xD0"      # mov eax,ss_sel; mov ss,ax
                + b"\xBC" + struct.pack("<H", (sp + 4) & 0xFFFF)                           # mov sp, retsp
                + b"\x66\xB8" + struct.pack("<H", sel_psp) + b"\x00\x00" + b"\x8E\xC0"     # mov eax,psp_sel; mov es,ax
                + b"\xEA" + struct.pack("<H", ret_ip) + struct.pack("<H", sel_cs))         # jmp far sel_cs:ret_ip
        mu.mem_write(STUB, stub)
        # the real-mode trampoline: cli ; set CR0.PE ; far-jump into the stub (the hardware switch).
        tramp = (b"\xFA" + b"\x0F\x20\xC0" + b"\x0C\x01" + b"\x0F\x22\xC0"
                 + b"\xEA" + struct.pack("<H", 0) + struct.pack("<H", SEL_STUBCODE))
        mu.mem_write(TRAMP, tramp)
        self.pm = True
        st["pm"] = True
        # run the trampoline in real mode; it performs the switch itself.
        W(UC_X86_REG_CS, TRAMP >> 4)
        W(UC_X86_REG_IP, TRAMP & 0xF)
        st["again"] = True
        mu.emu_stop()

    # -- LDT descriptor bookkeeping --------------------------------------------------------------
    def _make_ldt(self, base, limit, access, index=None):
        """write an LDT descriptor, record it in st["desc"] (so lin() can resolve it), return the
        selector (index<<3 | TI=4)."""
        if index is None:
            index = self.next_ldt
            while index in self.ldt_used:
                index += 1
            self.next_ldt = index + 1
        self.ldt_used[index] = (base, limit, access)
        gran = 0x80 if limit > 0xFFFFF else 0x00
        if access & 0x08 and limit > 0xFFFF:                   # a wide code selector is 32-bit (D=1)
            gran |= 0x40
        self.ctx.mu.mem_write(LDT + index * 8, _desc(base, limit, access, gran | (0x40 if (gran & 0x80) else 0)))
        sel = (index << 3) | 4
        self.ctx.st["desc"][sel & 0xFFFC] = (base, limit, access)
        return sel

    def _ldt_index(self, sel):
        return (sel & 0xFFF8) >> 3

    # -- INT 31h ---------------------------------------------------------------------------------
    def _int31(self):
        R, W, carry = self.ctx.R, self.ctx.W, self.ctx.carry
        ax = R(UC_X86_REG_AX) & 0xFFFF
        fn = ax
        bx, cx, dx = R(UC_X86_REG_BX), R(UC_X86_REG_CX), R(UC_X86_REG_DX)
        si, di = R(UC_X86_REG_SI), R(UC_X86_REG_DI)
        mu = self.ctx.mu

        if fn == 0x0000:                                       # allocate CX descriptors (LDT)
            n = cx or 1
            base_idx = self.next_ldt
            for k in range(n):
                self._make_ldt(0, 0, 0x92, index=base_idx + k)
            self.next_ldt = base_idx + n
            W(UC_X86_REG_AX, (base_idx << 3) | 4)
            carry(False)
            return
        if fn == 0x0001:                                       # free descriptor BX
            self.ldt_used.pop(self._ldt_index(bx), None)
            self.ctx.st["desc"].pop(bx & 0xFFFC, None)
            carry(False)
            return
        if fn == 0x0003:                                       # selector increment value
            W(UC_X86_REG_AX, 8)
            carry(False)
            return
        if fn == 0x0006:                                       # get base of selector BX -> CX:DX
            base = self._base(bx)
            W(UC_X86_REG_CX, (base >> 16) & 0xFFFF)
            W(UC_X86_REG_DX, base & 0xFFFF)
            carry(False)
            return
        if fn == 0x0007:                                       # set base of selector BX = CX:DX
            self._set(bx, base=(cx << 16) | dx)
            carry(False)
            return
        if fn == 0x0008:                                       # set limit of selector BX = CX:DX (bytes)
            self._set(bx, limit=(cx << 16) | dx)
            carry(False)
            return
        if fn == 0x0009:                                       # set access rights of selector BX = CL/CH
            self._set(bx, access=cx & 0xFF)
            carry(False)
            return
        if fn in (0x000A,):                                    # create a data alias of code selector BX
            base, limit, _acc = self._get(bx)
            W(UC_X86_REG_AX, self._make_ldt(base, limit, 0x92))
            carry(False)
            return
        if fn == 0x000B:                                       # get descriptor BX into ES:EDI (8 bytes)
            idx = self._ldt_index(bx)
            mu.mem_write(self._lin_esedi(), self.ctx.mu.mem_read(LDT + idx * 8, 8))
            carry(False)
            return
        if fn == 0x000C:                                       # set descriptor BX from ES:EDI (8 bytes)
            raw = bytes(mu.mem_read(self._lin_esedi(), 8))
            idx = self._ldt_index(bx)
            mu.mem_write(LDT + idx * 8, raw)
            e = struct.unpack("<Q", raw)[0]
            base = ((e >> 16) & 0xFFFFFF) | (((e >> 56) & 0xFF) << 24)
            limit = (e & 0xFFFF) | (((e >> 48) & 0xF) << 16)
            if (e >> 55) & 1:
                limit = (limit << 12) | 0xFFF
            self.ldt_used[idx] = (base, limit, (e >> 40) & 0xFF)
            self.ctx.st["desc"][bx & 0xFFFC] = (base, limit, (e >> 40) & 0xFF)
            carry(False)
            return
        if fn == 0x0100:                                       # allocate BX paragraphs of DOS memory
            seg = self.ctx.alloc(bx)
            if seg is None:
                W(UC_X86_REG_AX, 8)
                W(UC_X86_REG_BX, self.ctx.largest())
                carry(True)
                return
            sel = self._make_ldt(seg * 16, bx * 16 - 1 if bx else 0, 0x92)
            W(UC_X86_REG_AX, seg)                              # real-mode segment
            W(UC_X86_REG_DX, sel)                              # protected-mode selector
            carry(False)
            return
        if fn == 0x0101:                                       # free DOS memory (selector DX)
            carry(False)
            return
        if fn in (0x0200, 0x0201, 0x0202, 0x0203, 0x0204, 0x0205):
            self._vectors(fn, bx, cx, dx)
            return
        if fn == 0x0300:                                       # simulate a real-mode interrupt
            self._simulate_rm_int()
            return
        if fn == 0x0400:                                       # get DPMI version
            W(UC_X86_REG_AX, 0x005A)                           # 0.90
            W(UC_X86_REG_BX, 0x0003)                           # 32-bit, V86... (bit0 = paged? report plain)
            W(UC_X86_REG_CX, 0x0003)                           # 80386
            W(UC_X86_REG_DX, 0x0000)
            carry(False)
            return
        if fn == 0x0500:                                       # get free memory information (0x30 bytes at ES:EDI)
            self._mem_info()
            carry(False)
            return
        if fn == 0x0501:                                       # allocate a memory block (BX:CX bytes)
            size = (bx << 16) | cx
            base = self._pool_alloc(size)
            if base is None:
                carry(True)
                return
            h = self.next_handle
            self.next_handle += 1
            self.mem[h] = (base, size)
            W(UC_X86_REG_BX, (base >> 16) & 0xFFFF)
            W(UC_X86_REG_CX, base & 0xFFFF)
            W(UC_X86_REG_SI, (h >> 16) & 0xFFFF)
            W(UC_X86_REG_DI, h & 0xFFFF)
            carry(False)
            return
        if fn == 0x0502:                                       # free memory block (handle SI:DI)
            self.mem.pop((si << 16) | di, None)
            carry(False)
            return
        if fn == 0x0503:                                       # resize: keep the base, accept new size
            h = (si << 16) | di
            if h in self.mem:
                base, _ = self.mem[h]
                self.mem[h] = (base, (bx << 16) | cx)
                W(UC_X86_REG_BX, (base >> 16) & 0xFFFF)
                W(UC_X86_REG_CX, base & 0xFFFF)
                carry(False)
            else:
                carry(True)
            return
        if fn in (0x0600, 0x0601, 0x0602, 0x0603):             # lock/unlock linear region: no paging here
            carry(False)
            return
        if fn in (0x0E00, 0x0E01):                             # 80x87: report present, accept set
            if fn == 0x0E00:
                W(UC_X86_REG_AX, 0x0045)
            carry(False)
            return
        raise RuntimeError("DPMI INT 31h AX=%04X not implemented" % fn)

    # -- descriptor helpers ----------------------------------------------------------------------
    def _get(self, sel):
        return self.ldt_used.get(self._ldt_index(sel), (0, 0, 0x92))

    def _base(self, sel):
        return self._get(sel)[0]

    def _set(self, sel, base=None, limit=None, access=None):
        idx = self._ldt_index(sel)
        b, l, a = self.ldt_used.get(idx, (0, 0, 0x92))
        if base is not None:
            b = base & 0xFFFFFFFF
        if limit is not None:
            l = limit & 0xFFFFFFFF
        if access is not None:
            a = access & 0xFF
        gran = 0x00
        wl = l
        if l > 0xFFFFF:
            gran = 0x80
        if a & 0x08 and l > 0xFFFF:
            gran |= 0x40                                       # a 32-bit code selector (D=1)
        self.ldt_used[idx] = (b, l, a)
        self.ctx.st["desc"][sel & 0xFFFC] = (b, l, a)
        self.ctx.mu.mem_write(LDT + idx * 8, _desc(b, l, a, gran))

    def _lin_esedi(self):
        es = self.ctx.R(UC_X86_REG_ES)
        edi = self.ctx.R(UC_X86_REG_EDI)
        return self.ctx.st["desc"].get(es & 0xFFFC, (es * 16, 0, 0))[0] + edi

    def _pool_alloc(self, size):
        size = (size + 0xFFF) & ~0xFFF                          # page-align
        if self.pool + size > POOLTOP:
            return None
        base = self.pool
        self.pool += size
        return base

    def _mem_info(self):
        free = POOLTOP - self.pool
        info = bytearray(0x30)
        struct.pack_into("<I", info, 0x00, free)                # largest available block (bytes)
        struct.pack_into("<I", info, 0x04, free >> 12)          # max unlocked pages
        struct.pack_into("<I", info, 0x08, free >> 12)          # max locked pages
        struct.pack_into("<I", info, 0x0C, (POOLTOP - POOL) >> 12)
        struct.pack_into("<I", info, 0x10, free >> 12)
        self.ctx.mu.mem_write(self._lin_esedi(), bytes(info))

    def _vectors(self, fn, bx, cx, dx):
        R, W, carry = self.ctx.R, self.ctx.W, self.ctx.carry
        bl = bx & 0xFF
        if fn == 0x0200:                                        # get real-mode interrupt vector BL -> CX:DX
            seg, off = self.ctx.st["vec"].get(bl, (0, 0))
            W(UC_X86_REG_CX, seg)
            W(UC_X86_REG_DX, off)
            carry(False)
            return
        if fn == 0x0201:                                        # set real-mode interrupt vector BL = CX:DX
            self.ctx.st["vec"][bl] = (cx, dx)
            carry(False)
            return
        # 0202-0205 protected-mode exception/interrupt handlers: accept (we never raise them)
        carry(False)

    # -- INT 31h 0300: simulate a real-mode interrupt -> dos.run's int21 -------------------------
    def _simulate_rm_int(self):
        """BL = interrupt, ES:(E)DI -> the DPMI real-mode call structure. Load its real-mode register
        image and run dos.run's int21(). The structure's DS/ES are REAL-MODE segments; we point two
        scratch selectors (base = seg<<4) at them and leave the CPU in protected mode, so int21()'s
        lin() resolves exactly the linear byte the client's DOS transfer buffer occupies -- without
        ever loading a raw real-mode segment value into a protected-mode segment register."""
        mu, st, R, W, carry = self.ctx.mu, self.ctx.st, self.ctx.R, self.ctx.W, self.ctx.carry
        bl = R(UC_X86_REG_BX) & 0xFF
        rmcs = self._lin_esedi()                                 # the call structure (ES:EDI)
        s = bytes(mu.mem_read(rmcs, 0x32))
        (r_edi, r_esi, r_ebp, _res, r_ebx, r_edx, r_ecx, r_eax) = struct.unpack_from("<8I", s, 0)
        (r_flags, r_es, r_ds, r_fs, r_gs, r_ip, r_cs, r_sp, r_ss) = struct.unpack_from("<9H", s, 0x20)
        if self.scratch_ds is None:                              # two reusable scratch data selectors
            self.scratch_ds = self._make_ldt(0, 0xFFFF, 0x92)
            self.scratch_es = self._make_ldt(0, 0xFFFF, 0x92)
        self._set(self.scratch_ds, base=r_ds * 16)
        self._set(self.scratch_es, base=r_es * 16)
        saved = {r: R(r) for r in (UC_X86_REG_EAX, UC_X86_REG_EBX, UC_X86_REG_ECX, UC_X86_REG_EDX,
                                   UC_X86_REG_ESI, UC_X86_REG_EDI, UC_X86_REG_EBP, UC_X86_REG_EFLAGS)}
        st["ov_ds"] = self.scratch_ds               # int21 reads ds/es from these (lin() base = seg<<4),
        st["ov_es"] = self.scratch_es               # NOT from the CPU segment registers (left untouched)
        try:
            mu.reg_write(UC_X86_REG_EAX, r_eax)
            mu.reg_write(UC_X86_REG_EBX, r_ebx)
            mu.reg_write(UC_X86_REG_ECX, r_ecx)
            mu.reg_write(UC_X86_REG_EDX, r_edx)
            mu.reg_write(UC_X86_REG_ESI, r_esi)
            mu.reg_write(UC_X86_REG_EDI, r_edi)
            mu.reg_write(UC_X86_REG_EBP, r_ebp)
            if bl == 0x21:
                self.ctx.int21()
            else:
                raise RuntimeError("DPMI 0300 reflect of INT %02Xh not implemented" % bl)
            out_ax = R(UC_X86_REG_EAX) & 0xFFFF
            out_bx = R(UC_X86_REG_EBX) & 0xFFFF
            out_cx = R(UC_X86_REG_ECX) & 0xFFFF
            out_dx = R(UC_X86_REG_EDX) & 0xFFFF
            out_si = R(UC_X86_REG_ESI) & 0xFFFF
            out_di = R(UC_X86_REG_EDI) & 0xFFFF
            out_fl = R(UC_X86_REG_EFLAGS) & 0xFFFF
        finally:
            for r, v in saved.items():
                mu.reg_write(r, v)
            st["ov_ds"] = None
            st["ov_es"] = None
        # write the results back into the real-mode call structure
        rs = bytearray(s)
        struct.pack_into("<I", rs, 0x1C, out_ax)
        struct.pack_into("<I", rs, 0x10, out_bx)
        struct.pack_into("<I", rs, 0x18, out_cx)
        struct.pack_into("<I", rs, 0x14, out_dx)
        struct.pack_into("<I", rs, 0x04, out_si)
        struct.pack_into("<I", rs, 0x00, out_di)
        struct.pack_into("<H", rs, 0x20, out_fl)
        mu.mem_write(rmcs, bytes(rs))
        carry(False)
