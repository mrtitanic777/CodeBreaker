; vmm_mcode_04.asm -- VMM object 4 segment _LMSGTABLE
.386p
_LMSGTABLE segment dword public use32 'MCODE'

loc_4_0000:
        pushad
        and    ebx, 0ffff7fffh
        mov    dword ptr [ebp + 1ch], 0
        movzx  eax, word ptr [ebp + 12h]
        movzx  ebx, word ptr [ebp + 10h]
        db  0E8h, 013h, 007h, 0F8h, 0FFh
        jecxz  loc_4_002B
        cmp    ebx, dword ptr [ecx + 34h]
        jae    SHORT loc_4_002B
        mov    edx, dword ptr [ecx + 30h]
        mov    edx, dword ptr [edx + ebx*4]
        mov    dword ptr [ebp + 1ch], edx
loc_4_002B:
        popad
        ret
        db  000h, 000h, 000h, 08Bh, 043h, 00Ch, 0F6h, 003h, 040h, 075h, 00Bh, 068h, 064h, 04Dh, 000h, 000h
        db  0CDh, 020h, 0F3h, 000h, 001h, 000h, 074h, 070h, 08Bh, 053h, 034h, 00Fh, 0B7h, 04Bh, 09Ah, 068h
        db  084h, 04Dh, 000h, 000h, 0CDh, 020h, 0F3h, 000h, 001h, 000h, 0CDh, 020h, 088h, 000h, 001h, 000h
        db  0FFh, 077h, 004h, 066h, 08Bh, 047h, 008h, 068h, 0B7h, 04Dh, 000h, 000h, 0CDh, 020h, 0F3h, 000h
        db  001h, 000h, 003h, 03Dh, 084h, 000h, 000h, 000h, 08Bh, 037h, 085h, 0F6h, 074h, 020h, 0CDh, 020h
        db  0A3h, 000h, 001h, 000h, 074h, 018h, 08Bh, 008h, 08Bh, 050h, 004h, 068h, 0CCh, 04Dh, 000h, 000h
        db  0CDh, 020h, 0F3h, 000h, 001h, 000h, 0CDh, 020h
_LMSGTABLE ends
        end
