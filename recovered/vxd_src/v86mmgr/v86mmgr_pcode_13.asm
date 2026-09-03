; v86mmgr_pcode_13.asm -- V86MMGR object 13 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

        db  060h, 08Bh, 035h, 0FCh, 011h, 000h, 000h, 0F6h, 044h, 01Eh, 02Ch, 020h, 074h, 05Fh, 08Bh, 07Ch
        db  01Eh, 044h, 00Bh, 0FFh, 074h, 01Ah, 06Ah, 000h, 057h, 0CDh, 020h, 058h, 000h, 001h, 000h, 059h
        db  059h, 06Ah, 000h, 06Ah, 000h, 050h, 057h, 0CDh, 020h, 057h, 000h, 001h, 000h, 083h, 0C4h, 010h
        db  08Bh, 07Ch, 01Eh, 05Ch, 00Bh, 0FFh, 074h, 035h, 0B9h, 0FFh, 000h, 000h, 000h, 08Dh, 0B7h, 080h
        db  002h, 000h, 000h, 0FCh, 0ADh, 00Bh, 0C0h, 074h, 022h, 081h, 0F9h, 0FFh, 000h, 000h, 000h, 074h
        db  01Ah, 0F6h, 040h, 02Ch, 084h, 075h, 014h, 051h, 06Ah, 000h, 06Ah, 000h, 0FFh, 070h, 028h, 0FFh
        db  030h, 0CDh, 020h, 057h, 000h, 001h, 000h, 083h, 0C4h, 010h, 059h, 0E2h, 0D6h, 061h, 0F8h, 0C3h
V86MMGR_VM_SUSPEND:
        call   sub_13_007C
        call   0
        clc
        ret
sub_13_007C:
        pushad
        db  08Bh, 035h, 0FCh, 011h, 000h, 000h
        test   byte ptr [esi + ebx + 2ch], 10h
        je     SHORT loc_13_00AC
        mov    edi, dword ptr [esi + ebx + 30h]
        or     edi, edi
        je     SHORT loc_13_00AC
        push   0
        push   edi
        db  0CDh, 020h, 058h, 000h, 001h, 000h
        pop    ecx
        pop    ecx
        push   0
        push   0
        push   eax
        push   edi
        db  0CDh, 020h, 057h, 000h, 001h, 000h
        add    esp, 10h
loc_13_00AC:
        mov    edi, dword ptr [esi + ebx + 60h]
        or     edi, edi
        je     SHORT loc_13_0129
        mov    ecx, dword ptr [edi + 18h]
        jecxz  loc_13_00DC
        test   byte ptr [esi + ebx + 2eh], 1
        je     SHORT loc_13_00DC
        push   0
        push   ecx
        db  0CDh, 020h, 058h, 000h, 001h, 000h
        pop    ecx
        pop    ecx
        push   0
        push   0
        push   eax
        push   dword ptr [edi + 18h]
        db  0CDh, 020h, 057h, 000h, 001h, 000h
        add    esp, 10h
loc_13_00DC:
        test   byte ptr [edi + 14h], 2
        je     SHORT loc_13_00F6
        push   100h
        push   0
        push   10h
        push   dword ptr [edi]
        db  0CDh, 020h, 057h, 000h, 001h, 000h
        add    esp, 10h
loc_13_00F6:
        test   byte ptr [esi + ebx + 2ch], 10h
        je     SHORT loc_13_0129
        lea    esi, [edi + 1ch]
        db  08Bh, 00Dh, 0ACh, 012h, 000h, 000h
        jecxz  loc_13_0129
loc_13_0108:
        cld
        lodsd
        or     eax, eax
        jns    SHORT loc_13_0127
        cmp    dword ptr [eax], 0
        je     SHORT loc_13_0127
        push   ecx
        push   0
        push   0
        push   dword ptr [eax + 8]
        push   dword ptr [eax]
        db  0CDh, 020h, 057h, 000h, 001h, 000h
        add    esp, 10h
        pop    ecx
loc_13_0127:
        loop   loc_13_0108
loc_13_0129:
        popad
        clc
        ret
_PTEXT ends
        end
