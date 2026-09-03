; sb16_lcode_01_01.asm -- SB16 object 1 segment _LTEXT  (part 1/2)
.386p
_LTEXT segment dword public use32 'LCODE'

loc_1_0000:
        xor    byte ptr [ecx], dh
        xor    dh, byte ptr [ebx]
        xor    al, 35h
        aaa
        cmp    byte ptr [ecx], bh
        inc    ecx
        inc    edx
        inc    ebx
        inc    esp
        inc    ebp
        inc    esi
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0014:
        inc    ebx
        dec    edi
        push   eax
        pop    ecx
        push   edx
        dec    ecx
        inc    edi
        dec    eax
        push   esp
        and    byte ptr [eax], ch
        inc    ebx
        sub    dword ptr [eax], esp
        inc    ebx
        push   edx
        inc    ebp
        inc    ecx
        push   esp
        dec    ecx
        push   esi
        inc    ebp
        and    byte ptr [ebp + eax*2 + 43h], dl
        dec    eax
        dec    esi
        dec    edi
        dec    esp
        dec    edi
        inc    edi
        pop    ecx
        and    byte ptr [esp + edx*2 + 44h], cl
        sub    al, 20h
        xor    dword ptr [ecx], edi
        cmp    dword ptr [edx], esi
        add    byte ptr cs:[eax], al
        add    byte ptr [eax], al
loc_1_0044:
        push   edi
        popad
        db  074h, 063h
        push   4f44534dh
        push   ebx
        inc    ebp
        db  078h, 065h
        arpl   word ptr [eax], ax
        add    byte ptr [ebp + ecx*2 + 41h], al
        inc    edx
        db  075h, 066h
        db  066h, 065h, 072h, 053h
        db  069h, 07Ah, 065h, 000h, 000h, 000h
loc_1_0064:
        inc    ecx
        push   edi
        inc    ebp
        add    byte ptr [esi + 4dh], al
        add    byte ptr [eax], al
loc_1_006C:
        dec    ebp
        imul   esp, dword ptr [ecx + ebp*2], 44000000h
        dec    ebp
        inc    ecx
        add    byte ptr [eax + 44h], cl
        dec    ebp
        inc    ecx
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0080:
        imul   ebp, dword ptr [esi + 74h], 726f7000h
        je     SHORT loc_1_0089
loc_1_0089:
        add    byte ptr [eax], al
        add    byte ptr [ecx + ebp*2 + 73h], ah
        popad
        bound  ebp, qword ptr [ebp + 77h]
        popad
        db  072h, 06Eh
        imul   ebp, dword ptr [esi + 67h], 6e730000h
        bound  ebp, qword ptr fs:[ebx + esi*2 + 74h]
        xor    byte ptr cs:[eax], dh
        xor    byte ptr [eax], al
loc_1_00A8:
        db  073h, 06Eh
        bound  ebp, qword ptr fs:[ebx + esi*2 + 74h]
        db  02Eh, 064h, 072h, 076h
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax + eax], al
loc_1_00F4:
        add    byte ptr [eax], al
loc_1_00F6:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_00FC:
        add    byte ptr [eax], al
loc_1_00FE:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0108:
        inc    ebx
        push   esp
        xor    dword ptr [edi], esi
        xor    byte ptr [75746341h], dh
        popad
        insb
        inc    esp
        dec    ebp
        inc    ecx
        add    byte ptr [edx + esi*2 + 69h], al
        jbe    SHORT loc_1_0182
        db  072h, 073h
        add    byte ptr [edx + esi*2 + 42h], al
        insb
        popad
        db  073h, 074h
        jb     NEAR PTR loc_1_012A
loc_1_012A:
        add    byte ptr [eax], al
loc_1_012C:
        push   edx
        db  065h, 073h, 065h
        je     SHORT loc_1_017F
        dec    ecx
        inc    esp
        dec    ecx
        add    byte ptr [eax], al
        db  000h
loc_1_0138:
        push   ebx
        je     SHORT loc_1_019C
        db  074h, 075h
        jae    SHORT loc_1_013F
loc_1_013F:
        add    byte ptr [edx + 65h], dl
        db  073h, 065h
        je     SHORT loc_1_018A
        push   ebx
        push   eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_014C:
        inc    edx
        dec    esp
        inc    ecx
        push   ebx
        push   esp
        inc    ebp
        push   edx
        add    byte ptr [ebx + 53h], al
        dec    esi
        add    byte ptr [ebp + 69h], ch
        imul   ebx, dword ptr fs:[ebx + esi*2 + 62h], 6e733631h
        jb     NEAR PTR loc_1_01DE
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_016C:
        insd
        imul   esp, dword ptr [ecx + ebp*2 + 5ch], 6d666273h
        jb     NEAR PTR loc_1_01EF
        add    byte ptr [eax], al
        add    byte ptr [eax + 6eh], dl
        push   eax
loc_1_017F:
        inc    ebx
        db  072h, 065h
loc_1_0182:
        popad
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0188:
        inc    ebx
        push   esp
loc_1_018A:
        dec    esp
        push   ebx
        inc    edx
        xor    dword ptr [esi], esi
        pop    esp
        sub    al, byte ptr [ebx + 54h]
        dec    esp
        cmp    byte ptr [eax], dh
        xor    byte ptr [ecx], dh
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_019C:
        inc    ecx
        and    eax, 20583330h
        dec    ecx
        and    eax, 25442064h
        and    byte ptr fs:[eax + 25h], cl
        and    byte ptr fs:[esi + esi], dl
        add    byte ptr [eax], al
        db  000h
loc_1_01B4:
        inc    ecx
        and    eax, 20583330h
        dec    ecx
        and    eax, 25442064h
        and    byte ptr fs:[eax + 25h], cl
        and    byte ptr fs:[eax + 25h], dl
        xor    byte ptr [ebx], dh
        pop    eax
        and    byte ptr [esi + esi], dl
        add    byte ptr [ebp + 63h], cl
        db  075h, 056h
        jb     NEAR PTR loc_1_024A
        imul   ebp, dword ptr [edi + 6eh], 61430000h
loc_1_01DE:
        jb     SHORT loc_1_0244
        dec    ecx
        inc    esp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_01E8:
        add    dword ptr [edi + 100f4f2h], ecx
        push   es
loc_1_01EF:
        test   dword ptr [eax], 0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        adc    byte ptr [eax], al
        cmp    byte ptr [eax], al
        add    al, byte ptr [eax]
        add    dword ptr [ebx - 0eh], ecx
        hlt
        add    byte ptr [ecx], al
        add    dl, dh
        test   dword ptr [eax], 0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        adc    byte ptr [eax], al
        cmp    byte ptr [eax], al
        add    al, byte ptr [eax]
        add    dword ptr [ecx - 0eh], ecx
        hlt
        add    byte ptr [ecx], al
        add    dl, dh
        test   byte ptr [eax], 0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], dl
        add    byte ptr [eax], bh
        add    byte ptr [edx], al
        add    byte ptr [ecx + 0f7f212h], al
        inc    ecx
        add    dl, dh
loc_1_0244:
        test   dword ptr [eax], 0
loc_1_024A:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        adc    byte ptr [eax], al
        add    byte ptr ss:[edx], al
        add    byte ptr [ecx], al
        push   edi
        int1
        test   dword ptr [eax], 0f7f20001h
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], dl
        add    byte ptr [eax], dh
        add    byte ptr [edx], al
        add    byte ptr [ecx], al
        xchg   ebx, eax
        int1
        test   dword ptr [eax], 0f7f20001h
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], dl
        add    byte ptr [eax], dh
        add    byte ptr [edx], al
        add    byte ptr [ecx], al
        and    byte ptr [ecx + 0e1600f2h], 0f2h
        cmc
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], dl
        add    byte ptr [eax], bh
        add    byte ptr [edx], al
        add    byte ptr [ecx], al
        xchg   edx, eax
        ret    0f8h
        db  001h, 000h, 0C2h, 0F8h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 010h, 000h, 03Ah, 000h, 002h, 000h, 00Ch, 05Ch, 0F6h, 0F4h, 000h, 081h, 000h, 0F3h, 0F5h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 000h, 030h
        db  000h, 002h, 000h, 007h, 097h, 0F3h, 0F2h, 000h, 011h, 080h, 0F2h, 0F1h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 000h, 032h, 000h, 002h, 000h, 017h
        db  021h, 054h, 0F4h, 000h, 001h, 000h, 0F4h, 0F4h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 010h, 000h, 032h, 000h, 002h, 000h, 098h, 062h, 0F3h, 0F6h, 000h
        db  081h, 000h, 0F2h, 0F6h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 010h, 000h, 030h, 000h, 002h, 000h, 018h, 023h, 0F6h, 0F6h, 000h, 001h, 000h, 0E7h, 0F7h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 000h, 030h
        db  000h, 002h, 000h, 015h, 091h, 0F6h, 0F6h, 000h, 001h, 000h, 0F6h, 0F6h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 000h, 034h, 000h, 002h, 000h, 045h
        db  059h, 0D3h, 0F3h, 000h, 081h, 080h, 0A3h, 0F3h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 010h, 000h, 03Ch, 000h, 002h, 000h, 003h, 049h, 075h, 0F5h, 001h
        db  081h, 080h, 0B5h, 0F5h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 010h, 000h, 034h, 000h, 002h, 000h, 071h, 092h, 0F6h, 014h, 000h, 031h, 000h, 0F1h, 007h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 000h, 032h
        db  000h, 002h, 000h, 072h, 014h, 0C7h, 058h, 000h, 030h, 000h, 0C7h, 008h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 000h, 032h, 000h, 002h, 000h, 070h
        db  044h, 0AAh, 018h, 000h, 0B1h, 000h, 08Ah, 008h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 010h, 000h, 034h, 000h, 002h, 000h, 023h, 093h, 097h, 023h, 001h
        db  0B1h, 000h, 055h, 014h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 010h, 000h, 034h, 000h, 002h, 000h, 061h, 013h, 097h, 004h, 001h, 0B1h, 080h, 055h, 004h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 000h, 030h
        db  000h, 002h, 000h, 024h, 048h, 098h, 02Ah, 001h, 0B1h, 000h, 046h, 01Ah, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 000h, 03Ch, 000h, 002h, 000h, 061h
        db  013h, 091h, 006h, 001h, 021h, 000h, 061h, 007h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 010h, 000h, 03Ah, 000h, 002h, 000h, 021h, 013h, 071h, 006h, 000h
        db  0A1h, 089h, 061h, 007h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 010h, 000h, 036h, 000h, 002h, 000h, 002h, 09Ch, 0F3h, 094h, 001h, 041h, 080h, 0F3h, 0C8h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 000h, 03Ch
        db  000h, 002h, 000h, 003h, 054h, 0F3h, 09Ah, 001h, 011h, 000h, 0F1h, 0E7h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 000h, 03Ch, 000h, 002h, 000h, 023h
        db  05Fh, 0F1h, 03Ah, 000h, 021h, 000h, 0F2h, 0F8h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 010h, 000h, 030h, 000h, 002h, 000h, 003h, 087h, 0F6h, 022h, 001h
        db  021h, 080h, 0F3h, 0F8h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 010h, 000h, 036h, 000h, 002h, 000h, 003h, 047h, 0F9h, 054h, 000h, 021h, 000h, 0F6h, 03Ah
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 000h, 030h
        db  000h, 002h, 000h, 023h, 04Ah, 091h, 041h, 001h, 021h, 005h, 084h, 019h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 000h, 038h, 000h, 002h, 000h, 023h
        db  04Ah, 095h, 019h, 001h, 021h, 000h, 094h, 019h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 010h, 000h, 038h, 000h, 002h, 000h, 009h, 0A1h, 020h, 04Fh, 000h
        db  084h, 080h, 0D1h, 0F8h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 010h, 000h, 038h, 000h, 002h, 000h, 021h, 01Eh, 094h, 006h, 000h, 0A2h, 000h, 0C3h, 0A6h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 000h, 032h
        db  000h, 002h, 000h, 031h, 012h, 0F1h, 028h, 000h, 031h, 000h, 0F1h, 018h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 000h, 03Ah, 000h, 002h, 000h, 031h
        db  08Dh, 0F1h, 0E8h, 000h, 031h, 000h, 0F1h, 078h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 010h, 000h, 03Ah, 000h, 002h, 000h, 031h, 05Bh, 051h, 028h, 000h
        db  032h, 000h, 071h, 048h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 010h, 000h, 03Ch, 000h, 002h, 000h, 001h, 08Bh, 0A1h, 09Ah, 000h, 021h, 040h, 0F2h, 0DFh
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 000h, 038h
        db  000h, 002h, 000h, 021h, 08Bh, 0A2h, 016h, 000h, 021h, 008h, 0A1h, 0DFh, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 000h, 038h, 000h, 002h, 000h, 031h
        db  08Bh, 0F4h, 0E8h, 000h, 031h, 000h, 0F1h, 078h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 010h, 000h, 03Ah, 000h, 002h, 000h, 031h, 012h, 0F1h, 028h, 000h
        db  031h, 000h, 0F1h, 018h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 010h, 000h, 03Ah, 000h, 002h, 000h, 031h, 015h, 0DDh, 013h, 001h, 021h, 000h, 056h, 026h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 000h, 038h
        db  000h, 002h, 000h, 031h, 016h, 0DDh, 013h, 001h, 021h, 000h, 066h, 006h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 000h, 038h, 000h, 002h, 000h, 071h
        db  049h, 0D1h, 01Ch, 001h, 031h, 000h, 061h, 00Ch, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 010h, 000h, 038h, 000h, 002h, 000h, 021h, 04Dh, 071h, 012h, 001h
        db  023h, 080h, 072h, 006h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 010h, 000h, 032h, 000h, 002h, 000h, 0F1h, 040h, 0F1h, 021h, 001h, 0E1h, 000h, 06Fh, 016h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 000h, 032h
        db  000h, 002h, 000h, 002h, 01Ah, 0F5h, 075h, 001h, 001h, 080h, 085h, 035h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 000h, 030h, 000h, 002h, 000h, 002h
        db  01Dh, 0F5h, 075h, 001h, 001h, 080h, 0F3h, 0F4h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 010h, 000h, 030h, 000h, 002h, 000h, 010h, 041h, 0F5h, 005h, 001h
        db  011h, 000h, 0F2h, 0C3h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 010h, 000h, 032h, 000h, 002h, 000h, 021h, 09Bh, 0B1h, 025h, 001h, 0A2h, 001h, 072h, 008h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 000h, 03Eh
        db  000h, 002h, 000h, 0A1h, 098h, 07Fh, 003h, 001h, 021h, 000h, 03Fh, 007h, 001h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 000h, 030h, 000h, 002h, 000h, 0A1h
        db  093h, 0C1h, 012h, 000h, 061h, 000h, 04Fh, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 010h, 000h, 03Ah, 000h, 002h, 000h, 021h, 018h, 0C1h, 022h, 000h
        db  061h, 000h, 04Fh, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 010h, 000h, 03Ch, 000h, 002h, 000h, 031h, 05Bh, 0F4h, 015h, 000h, 072h, 083h, 08Ah, 005h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 000h, 030h
        db  000h, 002h, 000h, 0A1h, 090h, 074h, 039h, 000h, 061h, 000h, 071h, 067h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 000h, 030h, 000h, 002h, 000h, 071h
        db  057h, 054h, 005h, 000h, 072h, 000h, 07Ah, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 010h, 000h, 03Ch, 000h, 002h, 000h, 090h, 000h, 054h, 063h, 000h
        db  041h, 000h, 0A5h, 045h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 010h, 000h, 038h, 000h, 002h, 000h, 021h, 092h, 085h, 017h, 000h, 021h, 001h, 08Fh, 009h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 000h, 03Ch
        db  000h, 002h, 000h, 021h, 094h, 075h, 017h, 000h, 021h, 005h, 08Fh, 009h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 000h, 03Ch, 000h, 002h, 000h, 021h
        db  094h, 076h, 015h, 000h, 061h, 000h, 082h, 037h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 010h, 000h, 03Ch, 000h, 002h, 000h, 031h, 043h, 09Eh, 017h, 001h
        db  021h, 000h, 062h, 02Ch, 001h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 010h, 000h, 032h, 000h, 002h, 000h, 021h, 09Bh, 061h, 06Ah, 000h, 021h, 000h, 07Fh, 00Ah
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 000h, 032h
        db  000h, 002h, 000h, 061h, 08Ah, 075h, 01Fh, 000h, 022h, 006h, 074h, 00Fh, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 000h, 038h, 000h, 002h, 000h, 0A1h
        db  086h, 072h, 055h, 001h, 021h, 083h, 071h, 018h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 010h, 000h, 030h, 000h, 002h, 000h, 021h, 04Dh, 054h, 03Ch, 000h
        db  021h, 000h, 0A6h, 01Ch, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 010h, 000h, 038h, 000h, 002h, 000h, 031h, 08Fh, 093h, 002h, 001h, 061h, 000h, 072h, 00Bh
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 000h, 038h
        db  000h, 002h, 000h, 031h, 08Eh, 093h, 003h, 001h, 061h, 000h, 072h, 009h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 000h, 038h, 000h, 002h, 000h, 031h
        db  091h, 093h, 003h, 001h, 061h, 000h, 082h, 009h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 010h, 000h, 03Ah, 000h, 002h, 000h, 031h, 08Eh, 093h, 00Fh, 001h
        db  061h, 000h, 072h, 00Fh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 010h, 000h, 03Ah, 000h, 002h, 000h, 021h, 04Bh, 0AAh, 016h, 001h, 021h, 000h, 08Fh, 00Ah
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 000h, 038h
        db  000h, 002h, 000h, 031h, 090h, 07Eh, 017h, 001h, 021h, 000h, 08Bh, 00Ch, 001h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 000h, 036h, 000h, 002h, 000h, 031h
        db  081h, 075h, 019h, 001h, 032h, 000h, 061h, 019h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 010h, 000h, 030h, 000h, 002h, 000h, 032h, 090h, 09Bh, 021h, 000h
        db  021h, 000h, 072h, 017h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 010h, 000h, 034h, 000h, 002h, 000h, 0E1h, 01Fh, 085h, 05Fh, 000h, 0E1h, 000h, 065h, 01Ah
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 000h, 030h
        db  000h, 002h, 000h, 0E1h, 046h, 088h, 05Fh, 000h, 0E1h, 000h, 065h, 01Ah, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 000h, 030h, 000h, 002h, 000h, 0A1h
        db  09Ch, 075h, 01Fh, 000h, 021h, 000h, 075h, 00Ah, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 010h, 000h, 032h, 000h, 002h, 000h, 031h, 08Bh, 084h, 058h, 000h
        db  021h, 000h, 065h, 01Ah, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 010h, 000h, 030h, 000h, 002h, 000h, 0E1h, 04Ch, 066h, 056h, 000h, 0A1h, 000h, 065h, 026h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 000h, 030h
        db  000h, 002h, 000h, 062h, 0CBh, 076h, 046h, 000h, 0A1h, 000h, 055h, 036h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 000h, 030h, 000h, 002h, 000h, 062h
        db  099h, 057h, 007h, 000h, 0A1h, 000h, 056h, 007h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 010h, 000h, 03Bh, 000h, 002h, 000h, 062h, 093h, 077h, 007h, 000h
        db  0A1h, 000h, 076h, 007h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 010h, 000h, 03Bh, 000h, 002h, 000h, 022h, 059h, 0FFh, 003h, 002h, 021h, 000h, 0FFh, 00Fh
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 000h, 030h
        db  000h, 002h, 000h, 021h, 00Eh, 0FFh, 00Fh, 001h, 021h, 000h, 0FFh, 00Fh, 001h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 000h, 030h, 000h, 002h, 000h, 022h
        db  046h, 086h, 055h, 000h, 021h, 080h, 064h, 018h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 010h, 000h, 030h, 000h, 002h, 000h, 021h, 045h, 066h, 012h, 000h
        db  0A1h, 000h, 096h, 00Ah, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 010h, 000h, 030h, 000h, 002h, 000h, 021h, 08Bh, 092h, 02Ah, 001h, 022h, 000h, 091h, 02Ah
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 000h, 030h
        db  000h, 002h, 000h, 0A2h, 09Eh, 0DFh, 005h, 000h, 061h, 040h, 06Fh, 007h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 000h, 032h, 000h, 002h, 000h, 020h
        db  01Ah, 0EFh, 001h, 000h, 060h, 000h, 08Fh, 006h, 002h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 010h, 000h, 030h, 000h, 002h, 000h, 021h, 08Fh, 0F1h, 029h, 000h
        db  021h, 080h, 0F4h, 009h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 010h, 000h, 03Ah, 000h, 002h, 000h, 077h, 0A5h, 053h, 094h, 000h, 0A1h, 000h, 0A0h, 005h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 000h, 032h
        db  000h, 002h, 000h, 061h, 01Fh, 0A8h, 011h, 000h, 0B1h, 080h, 025h, 003h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 000h, 03Ah, 000h, 002h, 000h, 061h
        db  017h, 091h, 034h, 000h, 061h, 000h, 055h, 016h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 010h, 000h, 03Ch, 000h, 002h, 000h, 071h, 05Dh, 054h, 001h, 000h
        db  072h, 000h, 06Ah, 003h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 010h, 000h, 030h, 000h, 002h, 000h, 021h, 097h, 021h, 043h, 000h, 0A2h, 000h, 042h, 035h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 000h, 038h
        db  000h, 002h, 000h, 0A1h, 01Ch, 0A1h, 077h, 001h, 021h, 000h, 031h, 047h, 001h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 000h, 030h, 000h, 002h, 000h, 021h
        db  089h, 011h, 033h, 000h, 061h, 003h, 042h, 025h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 010h, 000h, 03Ah, 000h, 002h, 000h, 0A1h, 015h, 011h, 047h, 001h
        db  021h, 000h, 0CFh, 007h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 010h, 000h, 030h, 000h, 002h, 000h, 03Ah, 0CEh, 0F8h, 0F6h, 000h, 051h, 000h, 086h, 002h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 000h, 032h
        db  000h, 002h, 000h, 021h, 015h, 021h, 023h, 001h, 021h, 000h, 041h, 013h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 000h, 030h, 000h, 002h, 000h, 006h
        db  05Bh, 074h, 095h, 000h, 001h, 000h, 0A5h, 072h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 010h, 000h, 030h, 000h, 002h, 000h, 022h, 092h, 0B1h, 081h, 000h
        db  061h, 083h, 0F2h, 026h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 010h, 000h, 03Ch, 000h, 002h, 000h, 041h, 04Dh, 0F1h, 051h, 001h, 042h, 000h, 0F2h, 0F5h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 000h, 030h
        db  000h, 002h, 000h, 061h, 094h, 011h, 051h, 001h, 0A3h, 080h, 011h, 013h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 000h, 036h, 000h, 002h, 000h, 061h
        db  08Ch, 011h, 031h, 000h, 0A1h, 080h, 01Dh, 003h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 010h, 000h, 036h, 000h, 002h, 000h, 0A4h, 04Ch, 0F3h, 073h, 001h
        db  061h, 000h, 081h, 023h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 010h, 000h, 034h, 000h, 002h, 000h, 002h, 085h, 0D2h, 053h, 000h, 007h, 003h, 0F2h, 0F6h
        db  001h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 000h, 030h
        db  000h, 002h, 000h, 011h, 00Ch, 0A3h, 011h, 001h, 013h, 080h, 0A2h, 0E5h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 000h, 030h, 000h, 002h, 000h, 011h
        db  006h, 0F6h, 041h, 001h, 011h, 000h, 0F2h, 0E6h, 002h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 010h, 000h, 034h, 000h, 002h, 000h, 093h, 091h, 0D4h, 032h, 000h
        db  091h, 000h, 0EBh, 011h, 001h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 010h, 000h, 038h, 000h, 002h, 000h, 004h, 04Fh, 0FAh, 056h, 000h, 001h, 000h, 0C2h, 005h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 000h, 03Ch
        db  000h, 002h, 000h, 021h, 049h, 07Ch, 020h, 000h, 022h, 000h, 06Fh, 00Ch, 001h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 000h, 036h, 000h, 002h, 000h, 031h
        db  085h, 0DDh, 033h, 001h, 021h, 000h, 056h, 016h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 010h, 000h, 03Ah, 000h, 002h, 000h, 020h, 004h, 0DAh, 005h, 002h
        db  021h, 081h, 08Fh, 00Bh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 010h, 000h, 036h, 000h, 002h, 000h, 005h, 06Ah, 0F1h, 0E5h, 000h, 003h, 080h, 0C3h, 0E5h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 000h, 036h
        db  000h, 002h, 000h, 007h, 015h, 0ECh, 026h, 000h, 002h, 000h, 0F8h, 016h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 000h, 03Ah, 000h, 002h, 000h, 005h
        db  09Dh, 067h, 035h, 000h, 001h, 000h, 0DFh, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 010h, 000h, 038h, 000h, 002h, 000h, 018h, 096h, 0FAh, 028h, 000h
        db  012h, 000h, 0F8h, 0E5h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 010h, 000h, 03Ah, 000h, 002h, 000h, 010h, 086h, 0A8h, 007h, 000h, 000h, 003h, 0FAh, 003h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 000h, 036h
        db  000h, 002h, 000h, 011h, 041h, 0F8h, 047h, 002h, 010h, 003h, 0F3h, 003h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 000h, 034h, 000h, 002h, 000h, 001h
        db  08Eh, 0F1h, 006h, 002h, 010h, 000h, 0F3h, 002h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 010h, 000h, 03Eh, 000h, 002h, 000h, 00Eh, 000h, 01Fh, 000h, 000h
        db  0C0h, 000h, 01Fh, 0FFh, 003h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 010h, 000h, 03Eh, 000h, 002h, 000h, 006h, 080h, 0F8h, 024h, 000h, 003h, 088h, 056h, 084h
        db  002h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 000h, 03Eh
        db  000h, 002h, 000h, 00Eh, 000h, 0F8h, 000h, 000h, 0D0h, 005h, 034h, 004h, 003h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 000h, 03Eh, 000h, 002h, 000h, 00Eh
        db  000h, 0F6h, 000h, 000h, 0C0h, 000h, 01Fh, 002h, 003h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 010h, 000h, 03Eh, 000h, 002h, 000h, 0D5h, 095h, 037h, 0A3h, 000h
        db  0DAh, 040h, 056h, 037h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 010h, 000h, 030h, 000h, 002h, 000h, 035h, 05Ch, 0B2h, 061h, 002h, 014h, 008h, 0F4h, 015h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 000h, 03Ah
        db  000h, 002h, 000h, 00Eh, 000h, 0F6h, 000h, 000h, 0D0h, 000h, 04Fh, 0F5h, 003h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 000h, 03Eh, 000h, 002h, 000h, 026h
        db  000h, 0FFh, 001h, 000h, 0E4h, 000h, 012h, 016h, 001h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 010h, 000h, 03Eh, 000h, 002h, 000h, 000h, 000h, 0F3h, 0F0h, 000h
        db  000h, 000h, 0F6h, 0C9h, 002h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 010h, 000h, 03Eh, 000h, 002h, 000h, 010h, 044h, 0F8h, 077h, 002h, 011h, 000h, 0F3h, 006h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 000h, 038h
        db  000h, 002h, 000h, 010h, 044h, 0F8h, 077h, 002h, 011h, 000h, 0F3h, 006h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 000h, 038h, 000h, 002h, 000h, 002h
        db  007h, 0F9h, 0FFh, 000h, 011h, 000h, 0F8h, 0FFh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 010h, 000h, 038h, 000h, 002h, 000h, 000h, 000h, 0FCh, 005h, 002h
        db  000h, 000h, 0FAh, 017h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 010h, 000h, 03Eh, 000h, 002h, 000h, 000h, 002h, 0FFh, 007h, 000h, 001h, 000h, 0FFh, 008h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 000h, 030h
        db  000h, 002h, 000h, 000h, 000h, 0FCh, 005h, 002h, 000h, 000h, 0FAh, 017h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 000h, 03Eh, 000h, 002h, 000h, 000h
        db  000h, 0F6h, 00Ch, 000h, 000h, 000h, 0F6h, 006h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 010h, 000h, 034h, 000h, 002h, 000h, 00Ch, 000h, 0F6h, 008h, 000h
        db  012h, 000h, 0FBh, 047h, 002h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 010h, 000h, 03Ah, 000h, 002h, 000h, 000h, 003h, 0F8h, 02Ah, 000h, 000h, 000h, 0F6h, 045h
        db  001h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 000h, 034h
        db  000h, 002h, 000h, 00Ch, 000h, 0F6h, 008h, 000h, 012h, 005h, 07Bh, 047h, 002h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 000h, 03Ah, 000h, 002h, 000h, 000h
        db  003h, 0F8h, 02Ah, 000h, 000h, 000h, 0F6h, 045h, 001h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 010h, 000h, 034h, 000h, 002h, 000h, 00Ch, 000h, 0F6h, 002h, 000h
        db  012h, 000h, 0CBh, 043h, 002h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 010h, 000h, 03Ah, 000h, 002h, 000h, 000h, 003h, 0F8h, 02Ah, 000h, 000h, 000h, 0F6h, 045h
        db  001h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 000h, 034h
        db  000h, 002h, 000h, 000h, 003h, 0F8h, 02Ah, 000h, 000h, 000h, 0F6h, 045h, 001h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 000h, 034h, 000h, 002h, 000h, 00Eh
        db  000h, 0F6h, 000h, 000h, 0D0h, 000h, 09Fh, 002h, 003h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 010h, 000h, 03Eh, 000h, 002h, 000h, 000h, 003h, 0F8h, 02Ah, 000h
        db  000h, 000h, 0F6h, 045h, 001h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 010h, 000h, 034h, 000h, 002h, 000h, 00Eh, 008h, 0F8h, 042h, 000h, 007h, 04Ah, 0F4h, 0E4h
        db  003h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 000h, 03Eh
        db  000h, 002h, 000h, 00Eh, 000h, 0F5h, 030h, 000h, 0D0h, 00Ah, 09Fh, 002h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 000h, 03Eh, 000h, 002h, 000h, 00Eh
        db  00Ah, 0E4h, 0E4h, 003h, 007h, 05Dh, 0F5h, 0E5h, 001h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 010h, 000h, 036h, 000h, 002h, 000h, 002h, 003h, 0B4h, 004h, 000h
        db  005h, 00Ah, 097h, 0F7h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 010h, 000h, 03Eh, 000h, 002h, 000h, 04Eh, 000h, 0F6h, 000h, 000h, 09Eh, 000h, 09Fh, 002h
        db  003h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 000h, 03Eh
        db  000h, 002h, 000h, 011h, 045h, 0F8h, 037h, 002h, 010h, 008h, 0F3h, 005h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 000h, 038h, 000h, 002h, 000h, 00Eh
        db  000h, 0F6h, 000h, 000h, 0D0h, 000h, 09Fh, 002h, 003h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 010h, 000h, 03Eh, 000h, 002h, 000h, 080h, 000h, 0FFh, 003h, 003h
        db  010h, 00Dh, 0FFh, 014h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 010h, 000h, 03Ch, 000h, 002h, 000h, 00Eh, 008h, 0F8h, 042h, 000h, 007h, 051h, 0F4h, 0E4h
        db  003h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 000h, 03Eh
        db  000h, 002h, 000h, 006h, 00Bh, 0F5h, 00Ch, 000h, 002h, 000h, 0F5h, 008h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 000h, 036h, 000h, 002h, 000h, 001h
        db  000h, 0FAh, 0BFh, 000h, 002h, 000h, 0C8h, 097h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 010h, 000h, 037h, 000h, 002h, 000h, 001h, 051h, 0FAh, 087h, 000h
        db  001h, 000h, 0FAh, 0B7h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 010h, 000h, 036h, 000h, 002h, 000h, 001h, 054h, 0FAh, 08Dh, 000h, 002h, 000h, 0F8h, 0B8h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 000h, 036h
        db  000h, 002h, 000h, 001h, 059h, 0FAh, 088h, 000h, 002h, 000h, 0F8h, 0B6h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 000h, 036h, 000h, 002h, 000h, 001h
        db  000h, 0F9h, 00Ah, 003h, 000h, 000h, 0FAh, 006h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 010h, 000h, 03Eh, 000h, 002h, 000h, 000h, 080h, 0F9h, 089h, 003h
        db  000h, 000h, 0F6h, 06Ch, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 010h, 000h, 03Eh, 000h, 002h, 000h, 003h, 080h, 0F8h, 088h, 003h, 00Ch, 008h, 0F6h, 0B6h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 000h, 03Fh
        db  000h, 002h, 000h, 003h, 085h, 0F8h, 088h, 003h, 00Ch, 000h, 0F6h, 0B6h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 000h, 03Fh, 000h, 002h, 000h, 00Eh
        db  040h, 076h, 04Fh, 000h, 000h, 008h, 077h, 018h, 002h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 010h, 000h, 03Eh, 000h, 002h, 000h, 00Eh, 040h, 0C8h, 049h, 000h
        db  003h, 000h, 09Bh, 069h, 002h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 010h, 000h, 03Eh, 000h, 002h, 000h, 0D7h, 0DCh, 0ADh, 005h, 003h, 0C7h, 000h, 08Dh, 005h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 000h, 03Eh
        db  000h, 002h, 000h, 0D7h, 0DCh, 0A8h, 004h, 003h, 0C7h, 000h, 088h, 004h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 000h, 03Eh, 000h, 002h, 000h, 080h
        db  000h, 0F6h, 006h, 003h, 011h, 000h, 067h, 017h, 003h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 010h, 000h, 03Eh, 000h, 002h, 000h, 080h, 000h, 0F5h, 005h, 002h
        db  011h, 009h, 046h, 016h, 003h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 010h, 000h, 03Eh, 000h, 002h, 000h, 006h, 03Fh, 000h, 0F4h, 000h, 015h, 000h, 0F7h, 0F5h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 000h, 031h
        db  000h, 002h, 000h, 006h, 03Fh, 000h, 0F4h, 003h, 012h, 000h, 0F7h, 0F5h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 000h, 030h, 000h, 002h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_1DEA:
        add    eax, dword ptr [eax]
        add    dword ptr [eax], eax
        add    al, 0
        add    al, byte ptr [eax]
        add    eax, 0b000800h
        add    byte ptr [ecx], cl
        add    byte ptr [eax + eax], cl
        or     al, byte ptr [eax]
        or     eax, 13001000h
        add    byte ptr [ecx], dl
        add    byte ptr [eax + eax], dl
        adc    al, byte ptr [eax]
        adc    eax, 3010000h
        add    dword ptr [ecx], eax
        add    dword ptr [ecx + eax], eax
        add    al, byte ptr [ecx]
        add    eax, 0b010801h
        add    dword ptr [ecx], ecx
        add    dword ptr [ecx + eax], ecx
        or     al, byte ptr [ecx]
        or     eax, 13011001h
        add    dword ptr [ecx], edx
        add    dword ptr [ecx + eax], edx
        adc    al, byte ptr [ecx]
        adc    eax, 230001h
        and    eax, dword ptr [edx]
        xor    al, 3
        xor    byte ptr [edx + edi], al
        add    eax, 72f063ch
        sub    eax, dword ptr [esi]
        xor    dword ptr [ecx], ecx
        sub    eax, dword ptr [esi]
        xor    ecx, dword ptr [ebx]
        sub    eax, dword ptr [esi]
        push   es
        cmp    dword ptr [esi], ecx
        dec    eax
        push   es
        cmp    al, 10h
        dec    esp
        adc    dword ptr [edx + edx + 24h], edx
        adc    ecx, dword ptr [esp + edx + 54h]
        adc    eax, 17541653h
        sbb    byte ptr [eax], dl
        dec    ebp
        sbb    dword ptr [edx + ebx], edi
        inc    ecx
        sbb    edi, dword ptr [ebx]
        sbb    al, 33h
        sbb    eax, 1f471e2dh
        cmp    al, 20h
        cmp    ah, byte ptr [ecx]
        xor    eax, 47234022h
        and    al, 3dh
        and    eax, 2730263dh
        xor    byte ptr [eax], ch
        inc    ebp
        sub    dword ptr [edx + ebp + 3fh], eax
        sub    ecx, dword ptr [edx + 2ch]
        cmp    al, 2dh
        push   eax
        inc    eax
        das
        inc    ebp
        xor    byte ptr [ecx + 31h], cl
        dec    ebx
        xor    al, byte ptr [ebx + esi + 30h]
        xor    al, 35h
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1EA0:
        mov    bh, 0ah
        add    byte ptr [eax], al
        pop    edx
        or     eax, dword ptr [eax]
        add    byte ptr [edi], al
        or     al, 0
        add    byte ptr [esi - 7ffffff4h], bh
        or     eax, 0e4d0000h
        add    byte ptr [eax], al
        daa
        sldt   word ptr [eax]
        push   cs
        adc    byte ptr [eax], al
        add    byte ptr [edx], al
        adc    dword ptr [eax], eax
        add    byte ptr [18000012h], al
        adc    eax, dword ptr [eax]
        add    byte ptr [edx], bh
        adc    al, 0
        add    byte ptr [eax], ch
        and    al, 20h
        sbb    al, 17h
        adc    eax, 0e0f1113h
        or     eax, 90a0b0ch
        or     byte ptr [edi], al
        push   es
        add    eax, 3040405h
        add    eax, dword ptr [edx]
        add    al, byte ptr [ecx]
        add    dword ptr [ecx], eax
        add    byte ptr [eax], al
        db  000h
loc_1_1EF0:
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
loc_1_1EF4:
        push   ebx
        inc    edx
        xor    dword ptr [esi], esi
        and    byte ptr [ebp + 75h], cl
        jae    SHORT loc_1_1F66
        arpl   word ptr [eax], sp
        push   ebx
        jns    SHORT loc_1_1F70
        db  074h, 068h
        jae    NEAR PTR loc_1_1F70
        db  07Ah, 065h
        jb     SHORT loc_1_1F0B
loc_1_1F0B:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    al, al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        inc    esi
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        rol    byte ptr [eax], 0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        inc    esi
        add    al, byte ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        rol    byte ptr [eax], 0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        inc    esi
        add    eax, dword ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        rol    byte ptr [eax], 0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        inc    esi
        add    al, 0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1F66:
        add    byte ptr [eax], al
        rol    byte ptr [eax], 0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        inc    esi
loc_1_1F70:
        add    eax, 0
        add    byte ptr [eax], al
        add    al, al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        inc    esi
        push   es
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    al, al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        inc    esi
        pop    es
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    al, al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        inc    esi
        or     byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        rol    byte ptr [eax], 0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        inc    esi
        or     dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        rol    byte ptr [eax], 0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        inc    esi
        or     al, byte ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        rol    byte ptr [eax], 0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        inc    esi
        or     eax, dword ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        rol    byte ptr [eax], 0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        inc    esi
        or     al, 0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        rol    byte ptr [eax], 0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        inc    esi
        or     eax, 0
        add    byte ptr [eax], al
        add    al, al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        inc    esi
        push   cs
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    al, al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        inc    esi
        sldt   word ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    al, al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        inc    esi
        adc    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        rol    byte ptr [eax], 0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        inc    esi
        adc    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        rol    byte ptr [eax], 0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        inc    esi
        adc    al, byte ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        rol    byte ptr [eax], 0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        inc    esi
        adc    eax, dword ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        rol    byte ptr [eax], 0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        inc    esi
        adc    al, 0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        rol    byte ptr [eax], 0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        inc    esi
        adc    eax, 0
        add    byte ptr [eax], al
        add    al, al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        inc    esi
        push   ss
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    al, al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        inc    esi
        pop    ss
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    al, al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        inc    esi
        sbb    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        rol    byte ptr [eax], 0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        inc    esi
        sbb    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        rol    byte ptr [eax], 0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        inc    esi
        inc    esi
        db  0D9h, 0D4h, 047h, 0E8h, 062h, 0CFh, 011h, 093h, 0BCh, 044h, 045h, 053h, 054h, 000h, 000h, 083h
        db  0FAh, 09Ah, 027h, 081h, 049h, 0CEh, 011h, 0A5h, 021h, 000h, 020h, 0AFh, 00Bh, 0E5h, 060h, 085h
        db  0FAh, 09Ah, 027h, 081h, 049h, 0CEh, 011h, 0A5h, 021h, 000h, 020h, 0AFh, 00Bh, 0E5h, 060h, 084h
        db  0FAh, 09Ah, 027h, 081h, 049h, 0CEh, 011h, 0A5h, 021h, 000h, 020h, 0AFh, 00Bh, 0E5h, 060h, 086h
        db  0FAh, 09Ah, 027h, 081h, 049h, 0CEh, 011h, 0A5h, 021h, 000h, 020h, 0AFh, 00Bh, 0E5h, 060h, 0C0h
        db  033h, 042h, 08Ch, 0CCh, 0B4h, 0CEh, 011h, 092h, 094h, 044h, 045h, 053h, 054h, 000h, 000h
loc_1_2120:
        sal    dword ptr [ebx], 42h
        mov    esp, cs
        mov    ah, 0ceh
        adc    dword ptr [edx + 53454494h], edx
        push   esp
        add    byte ptr [eax], al
sub_1_2130:
        mov    eax, dword ptr [esp + 4]
        test   byte ptr [eax], 1
        jne    SHORT loc_1_2140
        mov    eax, 1
        jmp    SHORT loc_1_214F
loc_1_2140:
        push   dword ptr [eax + 8]
        push   dword ptr [esp + 0ch]
        call   sub_1_936C
        add    esp, 8
loc_1_214F:
        ret
sub_1_2150:
        mov    eax, dword ptr [esp + 4]
        test   byte ptr [eax], 1
        jne    SHORT loc_1_2160
        mov    eax, 1
        jmp    SHORT loc_1_216B
loc_1_2160:
        push   dword ptr [eax + 8]
        call   sub_1_931C
        add    esp, 4
loc_1_216B:
        ret
sub_1_216C:
        mov    eax, dword ptr [esp + 4]
        xor    ecx, ecx
        mov    word ptr [eax + 6], cx
        mov    dword ptr [eax], ecx
        mov    word ptr [eax + 4], 6a0h
        mov    dword ptr [eax + 8], ecx
        xor    eax, eax
        ret
sub_1_2184:
        push   ebp
        movzx  ax, byte ptr [esp + 0ch]
        mov    ebp, esp
        sub    esp, 20h
        mov    word ptr [ebp - 20h], ax
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 10h]
        mov    word ptr [esi + 6], ax
        mov    dword ptr [ebp - 1ch], ecx
        xor    ecx, ecx
        mov    eax, dword ptr [ebp + 18h]
        mov    edx, dword ptr [ebp + 14h]
        mov    dword ptr [ebp - 18h], ecx
        mov    dword ptr [ebp - 14h], eax
        mov    dword ptr [ebp - 10h], edx
        lea    eax, [esi + 8]
        mov    dword ptr [ebp - 0ch], ecx
        mov    ecx, dword ptr [ebp + 1ch]
        push   eax
        lea    edx, [ebp - 20h]
        mov    dword ptr [ebp - 4], ecx
        mov    dword ptr [ebp - 8], 1f4h
        mov    word ptr [ebp - 1eh], 4
        push   edx
        call   sub_1_95AD
        add    esp, 8
        test   eax, eax
        je     SHORT loc_1_21E4
        mov    eax, 1
        jmp    SHORT loc_1_21FB
loc_1_21E4:
        push   esi
        or     byte ptr [esi], 1
        call   sub_1_2150
        add    esp, 4
        push   esi
        call   sub_1_221C
        add    esp, 4
        xor    eax, eax
loc_1_21FB:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
sub_1_2200:
        mov    eax, dword ptr [esp + 4]
        test   byte ptr [eax], 1
        jne    SHORT loc_1_2210
        mov    eax, 1
        jmp    SHORT loc_1_221B
loc_1_2210:
        push   dword ptr [eax + 8]
        call   sub_1_939E
        add    esp, 4
loc_1_221B:
        ret
sub_1_221C:
        mov    eax, dword ptr [esp + 4]
        test   byte ptr [eax], 1
        jne    SHORT loc_1_222C
        mov    eax, 1
        jmp    SHORT loc_1_2237
loc_1_222C:
        push   dword ptr [eax + 8]
        call   sub_1_93B2
        add    esp, 4
loc_1_2237:
        ret
sub_1_2238:
        push   esi
        mov    esi, dword ptr [esp + 8]
        push   esi
        call   sub_1_2200
        add    esp, 4
        push   esi
        call   sub_1_2150
        add    esp, 4
        push   dword ptr [esi + 8]
        call   sub_1_95D4
        add    esp, 4
        xor    eax, eax
        and    byte ptr [esi], 0feh
        pop    esi
        ret
sub_1_2261:
        mov    eax, dword ptr [esp + 4]
        xor    ecx, ecx
        mov    word ptr [eax + 6], cx
        and    byte ptr [eax], 0feh
        mov    word ptr [eax + 4], 0ebh
        mov    dword ptr [eax + 8], ecx
        xor    eax, eax
        ret
sub_1_227A:
        push   ebp
        mov    edx, 1
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    cl, byte ptr [ebp + 0ch]
        shl    edx, cl
        movzx  eax, word ptr [esi + 4]
        test   edx, eax
        je     SHORT loc_1_22B6
        movzx  cx, cl
        mov    word ptr [esi + 6], cx
        lea    eax, [esi + 8]
        push   eax
        push   dword ptr [ebp + 10h]
        push   ecx
        call   sub_1_9681
        add    esp, 0ch
        test   eax, eax
        jne    SHORT loc_1_22B6
        xor    eax, eax
        or     byte ptr [esi], 1
        jmp    SHORT loc_1_22BB
loc_1_22B6:
        mov    eax, 1
loc_1_22BB:
        pop    esi
        pop    ebp
        ret
sub_1_22BE:
        mov    eax, dword ptr [esp + 4]
        test   byte ptr [eax], 1
        jne    SHORT loc_1_22CE
        mov    eax, 3
        jmp    SHORT loc_1_22DB
loc_1_22CE:
        mov    eax, dword ptr [eax + 8]
        sub    eax, dword ptr [esp + 8]
        cmp    eax, 1
        sbb    eax, eax
        inc    eax
loc_1_22DB:
        ret
sub_1_22DC:
        push   ebp
        mov    eax, 3
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        test   byte ptr [esi], 1
        je     SHORT loc_1_2316
        push   dword ptr [ebp + 10h]
        push   dword ptr [ebp + 1ch]
        push   dword ptr [ebp + 14h]
        push   -1
        push   dword ptr [esi + 8]
        call   sub_1_96BF
        add    esp, 14h
        push   dword ptr [ebp + 18h]
        push   dword ptr [ebp + 0ch]
        push   dword ptr [esi + 8]
        call   sub_1_96EE
        add    esp, 0ch
        xor    eax, eax
loc_1_2316:
        pop    esi
        pop    ebp
        ret
sub_1_2319:
        mov    eax, dword ptr [esp + 4]
        test   byte ptr [eax], 1
        jne    SHORT loc_1_2329
        mov    eax, 3
        jmp    SHORT loc_1_233C
loc_1_2329:
        xor    ecx, ecx
        push   ecx
        push   ecx
        push   ecx
        push   ecx
        push   dword ptr [eax + 8]
        call   sub_1_96BF
        add    esp, 14h
        xor    eax, eax
loc_1_233C:
        ret
sub_1_233D:
        mov    eax, dword ptr [esp + 4]
        test   byte ptr [eax], 1
        jne    SHORT loc_1_234D
        mov    eax, 3
        jmp    SHORT loc_1_235C
loc_1_234D:
        push   dword ptr [esp + 8]
        push   dword ptr [eax + 8]
        call   sub_1_9757
        add    esp, 8
loc_1_235C:
        ret
sub_1_235D:
        mov    eax, dword ptr [esp + 4]
        test   byte ptr [eax], 1
        jne    SHORT loc_1_236D
        mov    eax, 3
        jmp    SHORT loc_1_237C
loc_1_236D:
        push   dword ptr [esp + 8]
        push   dword ptr [eax + 8]
        call   sub_1_9725
        add    esp, 8
loc_1_237C:
        ret
sub_1_237D:
        push   esi
        mov    eax, 3
        mov    esi, dword ptr [esp + 8]
        test   byte ptr [esi], 1
        je     SHORT loc_1_23AD
        xor    eax, eax
        push   eax
        push   eax
        push   eax
        push   eax
        push   dword ptr [esi + 8]
        call   sub_1_96BF
        add    esp, 14h
        push   dword ptr [esi + 8]
        call   sub_1_96AB
        add    esp, 4
        xor    eax, eax
        and    byte ptr [esi], 0feh
loc_1_23AD:
        pop    esi
        ret
sub_1_23AF:
        mov    edx, dword ptr [esp + 8]
        mov    ecx, dword ptr [esp + 4]
        mov    word ptr [edx], 0
        mov    eax, ecx
        inc    ecx
        cmp    byte ptr [eax], 0
        je     SHORT loc_1_23CF
loc_1_23C4:
        inc    word ptr [edx]
        mov    eax, ecx
        inc    ecx
        cmp    byte ptr [eax], 0
        jne    SHORT loc_1_23C4
loc_1_23CF:
        xor    eax, eax
        ret
sub_1_23D2:
        xor    dx, dx
        push   ebx
        mov    ecx, dword ptr [esp + 0ch]
        push   esi
        mov    esi, dword ptr [esp + 0ch]
        cmp    byte ptr [ecx], dl
        je     SHORT loc_1_23F7
loc_1_23E3:
        movzx  eax, dx
        inc    dx
        mov    bl, byte ptr [eax + ecx]
        mov    byte ptr [eax + esi], bl
        movzx  eax, dx
        cmp    byte ptr [eax + ecx], 0
        jne    SHORT loc_1_23E3
loc_1_23F7:
        movzx  eax, dx
        mov    byte ptr [eax + esi], 0
        xor    eax, eax
        pop    esi
        pop    ebx
        ret
sub_1_2403:
        push   ebp
        lea    ecx, [esp + 10h]
        mov    ebp, esp
        sub    esp, 1ch
        mov    word ptr [ebp - 2], 0
        push   ebx
        mov    eax, dword ptr [ebp + 0ch]
        push   esi
        mov    ebx, dword ptr [ebp + 8]
        push   edi
        cmp    byte ptr [eax], 0
        mov    word ptr [ebp - 4], 0
        je     NEAR PTR loc_1_25B3
loc_1_242B:
        mov    edx, dword ptr [ebp + 0ch]
        movzx  eax, word ptr [ebp - 4]
        mov    dl, byte ptr [eax + edx]
        cmp    dl, 25h
        jne    NEAR PTR loc_1_2593
        inc    word ptr [ebp - 4]
        mov    edx, dword ptr [ebp + 0ch]
        movzx  eax, word ptr [ebp - 4]
        mov    dl, byte ptr [eax + edx]
        movzx  eax, dl
        cmp    eax, 63h
        je     SHORT loc_1_2483
        cmp    eax, 64h
        je     SHORT loc_1_2498
        cmp    eax, 73h
        je     NEAR PTR loc_1_24FA
        cmp    eax, 78h
        je     NEAR PTR loc_1_2528
        movzx  eax, word ptr [ebp - 2]
        inc    word ptr [ebp - 2]
        mov    byte ptr [eax + ebx], 25h
        movzx  esi, word ptr [ebp - 2]
        mov    byte ptr [esi + ebx], dl
        jmp    NEAR PTR loc_1_259A
loc_1_2483:
        movzx  eax, word ptr [ebp - 2]
        inc    word ptr [ebp - 2]
        mov    dl, byte ptr [ecx]
        add    ecx, 4
        mov    byte ptr [eax + ebx], dl
        jmp    NEAR PTR loc_1_259E
loc_1_2498:
        mov    si, word ptr [ecx]
        mov    di, 1
        cmp    si, 0ah
        jbe    SHORT loc_1_24B8
loc_1_24A5:
        mov    ax, si
        imul   di, di, 0ah
        sub    dx, dx
        div    di
        cmp    ax, 0ah
        ja     SHORT loc_1_24A5
loc_1_24B8:
        mov    word ptr [ebp - 8], 0ah
        mov    ax, si
        sub    dx, dx
        div    di
        movzx  edx, word ptr [ebp - 2]
        inc    word ptr [ebp - 2]
        add    al, 30h
        mov    byte ptr [edx + ebx], al
        mov    ax, si
        sub    dx, dx
        div    di
        mov    si, dx
        mov    ax, di
        sub    dx, dx
        div    word ptr [ebp - 8]
        mov    di, ax
        test   di, di
        jne    SHORT loc_1_24B8
        add    ecx, 4
        jmp    NEAR PTR loc_1_259E
loc_1_24FA:
        xor    dx, dx
        mov    eax, dword ptr [ecx]
        cmp    byte ptr [eax], dl
        je     SHORT loc_1_2523
loc_1_2503:
        movzx  esi, dx
        inc    dx
        mov    eax, dword ptr [ecx]
        mov    al, byte ptr [esi + eax]
        movzx  edi, dx
        movzx  esi, word ptr [ebp - 2]
        inc    word ptr [ebp - 2]
        mov    byte ptr [esi + ebx], al
        mov    eax, dword ptr [ecx]
        cmp    byte ptr [edi + eax], 0
        jne    SHORT loc_1_2503
loc_1_2523:
        add    ecx, 4
        jmp    SHORT loc_1_259E
loc_1_2528:
        mov    ax, word ptr [ecx]
        db  0BEh, 000h, 000h, 000h, 000h
        mov    word ptr [ebp - 6], ax
        lea    edi, [ebp - 1ch]
        movsd
        movsd
        movsd
        movsd
        movsb
        cmp    word ptr [ebp - 6], 10h
        mov    si, 1
        jbe    SHORT loc_1_255B
loc_1_2547:
        shl    si, 4
        mov    ax, word ptr [ebp - 6]
        sub    dx, dx
        div    si
        cmp    ax, 10h
        ja     SHORT loc_1_2547
loc_1_255B:
        mov    ax, word ptr [ebp - 6]
        sub    dx, dx
        div    si
        movzx  eax, ax
        movzx  edx, word ptr [ebp - 2]
        inc    word ptr [ebp - 2]
        mov    al, byte ptr [ebp + eax - 1ch]
        mov    byte ptr [edx + ebx], al
        mov    ax, word ptr [ebp - 6]
        sub    dx, dx
        div    si
        shr    si, 4
        mov    word ptr [ebp - 6], dx
        test   si, si
        jne    SHORT loc_1_255B
        add    ecx, 4
        jmp    SHORT loc_1_259E
loc_1_2593:
        movzx  eax, word ptr [ebp - 2]
        mov    byte ptr [eax + ebx], dl
loc_1_259A:
        inc    word ptr [ebp - 2]
loc_1_259E:
        inc    word ptr [ebp - 4]
        mov    edx, dword ptr [ebp + 0ch]
        movzx  eax, word ptr [ebp - 4]
        cmp    byte ptr [eax + edx], 0
        jne    NEAR PTR loc_1_242B
loc_1_25B3:
        movzx  eax, word ptr [ebp - 2]
        pop    edi
        pop    esi
        mov    byte ptr [eax + ebx], 0
        xor    eax, eax
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_1_25C4:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        call   sub_1_301E
        test   eax, eax
        je     SHORT loc_1_25EE
        mov    esi, dword ptr [ebp + 14h]
loc_1_25D7:
        cmp    dword ptr [eax + 8], esi
        jne    SHORT loc_1_25E1
        test   byte ptr [eax], 1
        jne    SHORT loc_1_25F8
loc_1_25E1:
        push   eax
        call   sub_1_303E
        add    esp, 4
        test   eax, eax
        jne    SHORT loc_1_25D7
loc_1_25EE:
        mov    eax, 1
loc_1_25F3:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
loc_1_25F8:
        movzx  ecx, word ptr [ebp + 0ch]
        cmp    ecx, 1
        je     SHORT loc_1_263B
        cmp    ecx, 2
        je     SHORT loc_1_266D
        cmp    ecx, 4
        je     NEAR PTR loc_1_26BA
        cmp    ecx, 8
        je     NEAR PTR loc_1_2704
        cmp    ecx, 10h
        je     NEAR PTR loc_1_273B
        cmp    ecx, 80h
        je     NEAR PTR loc_1_2775
        cmp    ecx, 100h
        je     NEAR PTR loc_1_27AC
        jmp    SHORT loc_1_25EE
loc_1_263B:
        cmp    word ptr [ebp + 18h], 2
        jb     SHORT loc_1_2649
        mov    eax, 2
        jmp    SHORT loc_1_25F3
loc_1_2649:
        push   dword ptr [ebp + 1ch]
        mov    ecx, 0ech
        movzx  edx, word ptr [ebp + 18h]
        imul   edx, ecx
        add    edx, eax
        add    edx, ecx
        push   edx
        push   dword ptr [ebp + 10h]
        push   dword ptr [ebp + 8]
        call   sub_1_5FAC
        add    esp, 10h
        jmp    SHORT loc_1_25F3
loc_1_266D:
        cmp    word ptr [ebp + 18h], 1
        jb     SHORT loc_1_267E
        mov    eax, 2
        jmp    NEAR PTR loc_1_25F3
loc_1_267E:
        test   byte ptr [eax + 0d4h], 2
        jne    SHORT loc_1_2691
        mov    eax, 2
        jmp    NEAR PTR loc_1_25F3
loc_1_2691:
        push   dword ptr [ebp + 1ch]
        movzx  ecx, word ptr [ebp + 18h]
        imul   ecx, ecx, 0e0h
        add    ecx, eax
        add    ecx, 2eeh
        push   ecx
        push   dword ptr [ebp + 10h]
        push   dword ptr [ebp + 8]
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_26B2:
        add    esp, 10h
        jmp    NEAR PTR loc_1_25F3
loc_1_26BA:
        cmp    word ptr [ebp + 18h], 1
        jb     SHORT loc_1_26CB
        mov    eax, 2
        jmp    NEAR PTR loc_1_25F3
loc_1_26CB:
        test   byte ptr [eax + 0d4h], 2
        jne    SHORT loc_1_26DE
        mov    eax, 2
        jmp    NEAR PTR loc_1_25F3
loc_1_26DE:
        push   dword ptr [ebp + 1ch]
        movzx  ecx, word ptr [ebp + 18h]
        imul   ecx, ecx, 5ah
        add    ecx, eax
        add    ecx, 3ceh
        push   ecx
        push   dword ptr [ebp + 10h]
        push   dword ptr [ebp + 8]
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_26FC:
        add    esp, 10h
        jmp    NEAR PTR loc_1_25F3
loc_1_2704:
        cmp    word ptr [ebp + 18h], 1
        jb     SHORT loc_1_2715
        mov    eax, 2
        jmp    NEAR PTR loc_1_25F3
loc_1_2715:
        push   dword ptr [ebp + 1ch]
        movzx  ecx, word ptr [ebp + 18h]
        imul   ecx, ecx, 2ah
        add    ecx, eax
        add    ecx, 2c4h
        push   ecx
        push   dword ptr [ebp + 10h]
        push   dword ptr [ebp + 8]
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_2733:
        add    esp, 10h
        jmp    NEAR PTR loc_1_25F3
loc_1_273B:
        cmp    word ptr [ebp + 18h], 1
        jb     SHORT loc_1_274C
        mov    eax, 2
        jmp    NEAR PTR loc_1_25F3
loc_1_274C:
        push   dword ptr [ebp + 1ch]
        movzx  ecx, word ptr [ebp + 18h]
        imul   ecx, ecx, 229h
        add    ecx, eax
        add    ecx, 428h
        push   ecx
        push   dword ptr [ebp + 10h]
        push   dword ptr [ebp + 8]
        call   sub_1_8656
        add    esp, 10h
        jmp    NEAR PTR loc_1_25F3
loc_1_2775:
        cmp    word ptr [ebp + 18h], 1
        jb     SHORT loc_1_2786
        mov    eax, 2
        jmp    NEAR PTR loc_1_25F3
loc_1_2786:
        push   dword ptr [ebp + 1ch]
        movzx  ecx, word ptr [ebp + 18h]
        imul   ecx, ecx, 2ch
        add    ecx, eax
        add    ecx, 651h
        push   ecx
        push   dword ptr [ebp + 10h]
        push   dword ptr [ebp + 8]
        call   sub_1_87C6
        add    esp, 10h
        jmp    NEAR PTR loc_1_25F3
loc_1_27AC:
        movzx  ecx, word ptr [ebp + 10h]
        cmp    ecx, 1
        je     SHORT loc_1_27D6
        cmp    ecx, 2
        je     SHORT loc_1_27F6
        cmp    ecx, 3
        je     SHORT loc_1_2829
        cmp    ecx, 4
        je     NEAR PTR loc_1_2858
        cmp    ecx, 5
        je     NEAR PTR loc_1_2866
        jmp    NEAR PTR loc_1_25EE
loc_1_27D6:
        mov    ax, word ptr [eax + 681h]
        mov    esi, dword ptr [ebp + 1ch]
        push   eax
        call   sub_1_3B3A
        mov    word ptr [esi], ax
        add    esp, 4
        and    word ptr [esi], 0ffh
        jmp    NEAR PTR loc_1_2887
loc_1_27F6:
        cmp    dword ptr [eax + 0e4h], 0
        je     NEAR PTR loc_1_2887
        lea    esi, [eax + 67dh]
        lea    eax, [ebp - 1]
        push   eax
        push   7ch
        push   esi
        call   sub_1_40A6
        add    esp, 0ch
        or     byte ptr [ebp - 1], 80h
        push   dword ptr [ebp - 1]
        push   7ch
        push   esi
        call   sub_1_4076
        jmp    SHORT loc_1_2884
loc_1_2829:
        cmp    dword ptr [eax + 0e4h], 0
        je     SHORT loc_1_2887
        lea    esi, [eax + 67dh]
        lea    eax, [ebp - 1]
        push   eax
        push   7ch
        push   esi
        call   sub_1_40A6
        add    esp, 0ch
        and    byte ptr [ebp - 1], 7fh
        push   dword ptr [ebp - 1]
        push   7ch
        push   esi
        call   sub_1_4076
        jmp    SHORT loc_1_2884
loc_1_2858:
        mov    eax, dword ptr [eax + 0e8h]
        mov    ecx, dword ptr [ebp + 1ch]
        mov    word ptr [ecx], ax
        jmp    SHORT loc_1_2887
loc_1_2866:
        mov    ecx, dword ptr [ebp + 1ch]
        cmp    dword ptr [eax + 0e8h], 0
        mov    cl, byte ptr [ecx]
        je     SHORT loc_1_2887
        mov    ax, word ptr [eax + 681h]
        push   0
        push   eax
        push   ecx
        call   sub_1_3E36
loc_1_2884:
        add    esp, 0ch
loc_1_2887:
        xor    eax, eax
        jmp    NEAR PTR loc_1_25F3
sub_1_288E:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 10h]
        test   byte ptr [esi], 4
        je     SHORT loc_1_28B9
        push   dword ptr [esi + 0d8h]
        add    esi, 6a7h
        push   esi
        call   sub_1_2130
        add    esp, 8
        jmp    NEAR PTR loc_1_2A9B
loc_1_28B9:
        lea    edi, [esi + 67dh]
        lea    eax, [ebp - 1]
        push   eax
        push   82h
        push   edi
        call   sub_1_40A6
        add    esp, 0ch
        test   byte ptr [ebp - 1], 7
        je     NEAR PTR loc_1_2A64
        test   byte ptr [ebp - 1], 1
        je     NEAR PTR loc_1_2982
        lea    ebx, [esi + 0ech]
        push   8
        push   ebx
        call   sub_1_9DEB
        add    esp, 8
        test   eax, eax
        push   ebx
        jne    SHORT loc_1_296B
        call   sub_1_9E43
        add    esp, 4
        cmp    dword ptr [esi + 0f4h], 0
        je     SHORT loc_1_2963
        cmp    dword ptr [esi + 1e0h], 0
        je     SHORT loc_1_2963
        mov    byte ptr [ebp - 3], 1
        mov    byte ptr [ebp - 5], 1
loc_1_291D:
        cmp    byte ptr [ebp - 5], 6
        jae    SHORT loc_1_294B
        push   ebx
        call   sub_1_A0F0
        add    esp, 4
        lea    eax, [ebp - 3]
        push   eax
        push   82h
        push   edi
        call   sub_1_40A6
        add    esp, 0ch
        inc    byte ptr [ebp - 5]
        and    byte ptr [ebp - 3], 1
        cmp    byte ptr [ebp - 3], 0
        jne    SHORT loc_1_291D
loc_1_294B:
        mov    ax, word ptr [esi + 79fh]
        cmp    ah, 4
        jne    SHORT loc_1_2982
        cmp    al, 0fh
        jae    SHORT loc_1_2982
        push   esi
        call   sub_1_2AA4
        jmp    SHORT loc_1_297F
loc_1_2963:
        push   ebx
        call   sub_1_A0F0
        jmp    SHORT loc_1_297F
loc_1_296B:
        call   sub_1_A0F0
        add    esp, 4
        lea    eax, [esi + 6a7h]
        push   eax
        call   sub_1_2150
loc_1_297F:
        add    esp, 4
loc_1_2982:
        test   byte ptr [ebp - 1], 2
        je     NEAR PTR loc_1_2A38
        test   byte ptr [esi + 0d4h], 20h
        lea    ebx, [esi + 0ech]
        je     SHORT loc_1_29A1
        lea    ebx, [esi + 1d8h]
loc_1_29A1:
        push   10h
        push   ebx
        call   sub_1_9DEB
        add    esp, 8
        test   eax, eax
        push   ebx
        jne    SHORT loc_1_2A21
        call   sub_1_9E43
        add    esp, 4
        cmp    dword ptr [esi + 0f4h], 0
        je     SHORT loc_1_2A19
        cmp    dword ptr [esi + 1e0h], 0
        je     SHORT loc_1_2A19
        mov    byte ptr [ebp - 2], 1
        mov    byte ptr [ebp - 5], 1
loc_1_29D3:
        cmp    byte ptr [ebp - 5], 6
        jae    SHORT loc_1_2A01
        push   ebx
        call   sub_1_A0F0
        add    esp, 4
        lea    eax, [ebp - 2]
        push   eax
        push   82h
        push   edi
        call   sub_1_40A6
        add    esp, 0ch
        inc    byte ptr [ebp - 5]
        and    byte ptr [ebp - 2], 2
        cmp    byte ptr [ebp - 2], 0
        jne    SHORT loc_1_29D3
loc_1_2A01:
        mov    ax, word ptr [esi + 79fh]
        cmp    ah, 4
        jne    SHORT loc_1_2A38
        cmp    al, 0fh
        jae    SHORT loc_1_2A38
        push   esi
        call   sub_1_2AA4
        jmp    SHORT loc_1_2A35
loc_1_2A19:
        push   ebx
        call   sub_1_A0F0
        jmp    SHORT loc_1_2A35
loc_1_2A21:
        call   sub_1_A0F0
        add    esp, 4
        lea    eax, [esi + 6a7h]
        push   eax
        call   sub_1_2150
loc_1_2A35:
        add    esp, 4
loc_1_2A38:
        test   byte ptr [ebp - 1], 4
        je     SHORT loc_1_2A9B
        lea    ebx, [esi + 2eeh]
        push   ebx
        call   sub_1_A447
        add    esp, 4
        test   eax, eax
        jne    SHORT loc_1_2A59
        push   ebx
        call   sub_1_A309
        jmp    SHORT loc_1_2A5F
loc_1_2A59:
        push   edi
        call   sub_1_41D9
loc_1_2A5F:
        add    esp, 4
        jmp    SHORT loc_1_2A8C
loc_1_2A64:
        test   byte ptr [ebp - 1], 8
        je     SHORT loc_1_2A8C
        lea    eax, [ebp - 6]
        push   eax
        push   edi
        call   sub_1_4807
        mov    ax, word ptr [ebp - 6]
        add    esp, 8
        add    ax, 0dh
        lea    ecx, [ebp - 8]
        push   ecx
        push   eax
        call   sub_1_9CE4
        add    esp, 8
loc_1_2A8C:
        add    esi, 6a7h
        push   esi
        call   sub_1_2150
        add    esp, 4
loc_1_2A9B:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_1_2AA4:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        mov    byte ptr [ebp - 1], 0
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        mov    byte ptr [ebp - 2], 0
        push   dword ptr [ebx + 1a0h]
        push   dword ptr [ebx + 1c8h]
        lea    edi, [ebx + 0ech]
        lea    esi, [ebx + 1d8h]
        add    ebx, 67dh
        call   sub_1_233D
        add    esp, 8
        push   dword ptr [esi + 0b4h]
        push   dword ptr [esi + 0dch]
        call   sub_1_233D
        add    esp, 8
        lea    ecx, [ebp - 2]
        push   ecx
        push   82h
        push   ebx
        call   sub_1_40A6
        add    esp, 0ch
        and    byte ptr [ebp - 2], 3
        jne    NEAR PTR loc_1_2C03
        push   0fh
        push   ebx
        call   sub_1_433E
        add    esp, 8
        push   6
        push   ebx
        call   sub_1_433E
        add    esp, 8
        lea    eax, [ebp - 1]
        push   eax
        push   ebx
        call   sub_1_43BA
        add    esp, 8
        test   byte ptr [ebp - 1], 1
        je     SHORT loc_1_2B9A
        push   0eh
        push   ebx
        call   sub_1_433E
        add    esp, 8
        push   8
        push   ebx
        call   sub_1_433E
        add    esp, 8
        push   20h
        push   ebx
        call   sub_1_433E
        add    esp, 8
        push   0eh
        push   ebx
        call   sub_1_433E
        add    esp, 8
        push   8
        push   ebx
        call   sub_1_433E
        add    esp, 8
        push   4
        push   ebx
        call   sub_1_433E
        add    esp, 8
        push   0eh
        push   ebx
        call   sub_1_433E
        add    esp, 8
        push   8
        push   ebx
        call   sub_1_433E
        add    esp, 8
        push   0
        push   ebx
        call   sub_1_433E
        add    esp, 8
loc_1_2B9A:
        test   byte ptr [ebp - 1], 2
        je     SHORT loc_1_2C03
        push   0eh
        push   ebx
        call   sub_1_433E
        add    esp, 8
        push   10h
        push   ebx
        call   sub_1_433E
        add    esp, 8
        push   20h
        push   ebx
        call   sub_1_433E
        add    esp, 8
        push   0eh
        push   ebx
        call   sub_1_433E
        add    esp, 8
        push   10h
        push   ebx
        call   sub_1_433E
        add    esp, 8
        push   4
        push   ebx
        call   sub_1_433E
        add    esp, 8
        push   0eh
        push   ebx
        call   sub_1_433E
        add    esp, 8
        push   10h
        push   ebx
        call   sub_1_433E
        add    esp, 8
        push   0
        push   ebx
        call   sub_1_433E
        add    esp, 8
loc_1_2C03:
        push   dword ptr [edi + 0b4h]
        push   dword ptr [edi + 0dch]
        call   sub_1_235D
        add    esp, 8
        push   dword ptr [esi + 0b4h]
        push   dword ptr [esi + 0dch]
        call   sub_1_235D
        add    esp, 8
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_1_2C34:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   esi
        lea    eax, [ebp - 2]
        push   eax
        lea    ecx, [ebp - 8]
        push   ecx
        mov    esi, dword ptr [ebp + 0ch]
        push   esi
        push   2
        call   sub_1_2E0C
        add    esp, 10h
        test   eax, eax
        jne    SHORT loc_1_2C87
        mov    eax, dword ptr [ebp - 8]
        test   byte ptr [eax], 4
        je     SHORT loc_1_2C82
        mov    ecx, eax
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [ecx + 0d8h], eax
        jne    SHORT loc_1_2C87
        cmp    dword ptr [ebp + 10h], 0
        push   esi
        je     SHORT loc_1_2C78
        call   sub_1_939E
        jmp    SHORT loc_1_2C7D
loc_1_2C78:
        call   sub_1_93B2
loc_1_2C7D:
        add    esp, 4
        jmp    SHORT loc_1_2C87
loc_1_2C82:
        call   sub_1_A58C
loc_1_2C87:
        xor    eax, eax
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
sub_1_2C8E:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        lea    eax, [ebp - 2]
        push   esi
        lea    ecx, [ebp - 8]
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   eax
        push   ecx
        push   esi
        push   5
        call   sub_1_2E0C
        add    esp, 10h
        test   eax, eax
        jne    NEAR PTR loc_1_2DAC
        xor    di, di
        mov    eax, dword ptr [ebp - 8]
        mov    ebx, 0ech
        and    byte ptr [eax], 0fbh
loc_1_2CC3:
        movzx  eax, di
        imul   eax, ebx
        inc    di
        push   esi
        add    eax, dword ptr [ebp - 8]
        add    eax, ebx
        push   eax
        call   sub_1_A09E
        add    esp, 8
        cmp    di, 2
        jb     SHORT loc_1_2CC3
        xor    di, di
loc_1_2CE3:
        movzx  eax, di
        inc    di
        push   esi
        imul   eax, eax, 0e0h
        add    eax, dword ptr [ebp - 8]
        add    eax, 2eeh
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_2CFD:
        add    esp, 8
        cmp    di, 1
        jb     SHORT loc_1_2CE3
        xor    di, di
loc_1_2D09:
        movzx  eax, di
        inc    di
        push   esi
        imul   eax, eax, 5ah
        add    eax, dword ptr [ebp - 8]
        add    eax, 3ceh
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_2D20:
        add    esp, 8
        cmp    di, 1
        jb     SHORT loc_1_2D09
        push   1
        mov    eax, dword ptr [ebp - 8]
        push   esi
        add    eax, 67dh
        push   eax
        call   sub_1_4614
        add    esp, 0ch
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     SHORT loc_1_2D67
        db  066h, 08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp - 8]
        cmp    word ptr [eax + 683h], cx
        jne    SHORT loc_1_2D67
        push   0
        push   esi
        push   7
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        add    esp, 0ch
loc_1_2D67:
        mov    eax, dword ptr [ebp - 8]
        mov    edi, 6a7h
        add    eax, edi
        push   eax
        call   sub_1_2200
        add    esp, 4
        mov    eax, dword ptr [ebp - 8]
        add    eax, edi
        push   eax
        call   sub_1_2150
        add    esp, 4
        mov    eax, dword ptr [ebp - 8]
        add    eax, edi
        push   eax
        call   sub_1_221C
        add    esp, 4
        mov    eax, dword ptr [ebp - 8]
        add    eax, 67dh
        push   0ffffh
        push   eax
        call   sub_1_4288
        add    esp, 8
loc_1_2DAC:
        lea    eax, [ebp - 2]
        lea    ecx, [ebp - 8]
        push   eax
        push   ecx
        push   esi
        push   6
        call   sub_1_2E0C
        add    esp, 10h
        test   eax, eax
        jne    SHORT loc_1_2E03
        xor    di, di
        mov    eax, dword ptr [ebp - 8]
        and    byte ptr [eax], 0f7h
loc_1_2DCC:
        movzx  eax, di
        inc    di
        push   esi
        imul   eax, eax, 229h
        add    eax, dword ptr [ebp - 8]
        add    eax, 428h
        push   eax
        call   sub_1_6531
        add    esp, 8
        cmp    di, 1
        jb     SHORT loc_1_2DCC
        push   1
        mov    eax, dword ptr [ebp - 8]
        push   esi
        add    eax, 67dh
        push   eax
        call   sub_1_46AB
        add    esp, 0ch
loc_1_2E03:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_1_2E0C:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        cmp    eax, 1
        push   esi
        je     SHORT loc_1_2E4A
        cmp    eax, 2
        je     SHORT loc_1_2E92
        cmp    eax, 3
        je     NEAR PTR loc_1_2ECE
        cmp    eax, 4
        je     NEAR PTR loc_1_2F4A
        cmp    eax, 5
        je     NEAR PTR loc_1_2F8A
        cmp    eax, 6
        je     NEAR PTR loc_1_2FD4
loc_1_2E42:
        mov    eax, 1
loc_1_2E47:
        pop    esi
        pop    ebp
        ret
loc_1_2E4A:
        call   sub_1_301E
        mov    esi, eax
        test   esi, esi
        je     SHORT loc_1_2E42
loc_1_2E55:
        test   byte ptr [esi], 1
        je     SHORT loc_1_2E70
        push   dword ptr [ebp + 0ch]
        lea    ecx, [esi + 67dh]
        push   ecx
        call   sub_1_41F5
        add    esp, 8
        test   eax, eax
        je     SHORT loc_1_2E81
loc_1_2E70:
        push   esi
        call   sub_1_303E
        add    esp, 4
        mov    esi, eax
        test   esi, esi
        jne    SHORT loc_1_2E55
        jmp    SHORT loc_1_2E42
loc_1_2E81:
        mov    ecx, dword ptr [ebp + 10h]
        mov    eax, dword ptr [ebp + 14h]
        mov    dword ptr [ecx], esi
        mov    word ptr [eax], 0
        xor    eax, eax
        jmp    SHORT loc_1_2E47
loc_1_2E92:
        call   sub_1_301E
        test   eax, eax
        je     SHORT loc_1_2E42
loc_1_2E9B:
        test   byte ptr [eax], 1
        je     SHORT loc_1_2EAB
        mov    ecx, dword ptr [ebp + 0ch]
        cmp    dword ptr [eax + 6afh], ecx
        je     SHORT loc_1_2EBA
loc_1_2EAB:
        push   eax
        call   sub_1_303E
        add    esp, 4
        test   eax, eax
        jne    SHORT loc_1_2E9B
        jmp    SHORT loc_1_2E42
loc_1_2EBA:
        mov    ecx, dword ptr [ebp + 10h]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp + 14h]
        mov    word ptr [eax], 0
        xor    eax, eax
        jmp    NEAR PTR loc_1_2E47
loc_1_2ECE:
        call   sub_1_301E
        mov    esi, eax
        test   esi, esi
        je     NEAR PTR loc_1_2E42
loc_1_2EDD:
        test   byte ptr [esi], 1
        je     SHORT loc_1_2F0E
        push   dword ptr [ebp + 0ch]
        lea    ecx, [esi + 6b3h]
        push   ecx
        call   sub_1_22BE
        add    esp, 8
        test   eax, eax
        je     SHORT loc_1_2F22
        push   dword ptr [ebp + 0ch]
        lea    ecx, [esi + 6bfh]
        push   ecx
        call   sub_1_22BE
        add    esp, 8
        test   eax, eax
        je     SHORT loc_1_2F36
loc_1_2F0E:
        push   esi
        call   sub_1_303E
        add    esp, 4
        mov    esi, eax
        test   esi, esi
        jne    SHORT loc_1_2EDD
        jmp    NEAR PTR loc_1_2E42
loc_1_2F22:
        mov    ecx, dword ptr [ebp + 10h]
        mov    eax, dword ptr [ebp + 14h]
        mov    dword ptr [ecx], esi
        mov    word ptr [eax], 0
        xor    eax, eax
        jmp    NEAR PTR loc_1_2E47
loc_1_2F36:
        mov    ecx, dword ptr [ebp + 10h]
        mov    eax, dword ptr [ebp + 14h]
        mov    dword ptr [ecx], esi
        mov    word ptr [eax], 1
        xor    eax, eax
        jmp    NEAR PTR loc_1_2E47
loc_1_2F4A:
        call   sub_1_301E
        test   eax, eax
        je     NEAR PTR loc_1_2E42
loc_1_2F57:
        mov    ecx, dword ptr [ebp + 0ch]
        cmp    dword ptr [eax + 8], ecx
        jne    SHORT loc_1_2F64
        test   byte ptr [eax], 1
        jne    SHORT loc_1_2F76
loc_1_2F64:
        push   eax
        call   sub_1_303E
        add    esp, 4
        test   eax, eax
        jne    SHORT loc_1_2F57
        jmp    NEAR PTR loc_1_2E42
loc_1_2F76:
        mov    ecx, dword ptr [ebp + 10h]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp + 14h]
        mov    word ptr [eax], 0
        xor    eax, eax
        jmp    NEAR PTR loc_1_2E47
loc_1_2F8A:
        call   sub_1_301E
        test   eax, eax
        je     NEAR PTR loc_1_2E42
loc_1_2F97:
        mov    ecx, dword ptr [eax]
        test   cl, 1
        je     SHORT loc_1_2FAE
        test   cl, 4
        je     SHORT loc_1_2FAE
        mov    ecx, dword ptr [ebp + 0ch]
        cmp    dword ptr [eax + 0d8h], ecx
        je     SHORT loc_1_2FC0
loc_1_2FAE:
        push   eax
        call   sub_1_303E
        add    esp, 4
        test   eax, eax
        jne    SHORT loc_1_2F97
        jmp    NEAR PTR loc_1_2E42
loc_1_2FC0:
        mov    ecx, dword ptr [ebp + 10h]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp + 14h]
        mov    word ptr [eax], 0
        xor    eax, eax
        jmp    NEAR PTR loc_1_2E47
loc_1_2FD4:
        call   sub_1_301E
        test   eax, eax
        je     NEAR PTR loc_1_2E42
loc_1_2FE1:
        mov    ecx, dword ptr [eax]
        test   cl, 1
        je     SHORT loc_1_2FF8
        test   cl, 8
        je     SHORT loc_1_2FF8
        mov    ecx, dword ptr [ebp + 0ch]
        cmp    dword ptr [eax + 0dch], ecx
        je     SHORT loc_1_300A
loc_1_2FF8:
        push   eax
        call   sub_1_303E
        add    esp, 4
        test   eax, eax
        jne    SHORT loc_1_2FE1
        jmp    NEAR PTR loc_1_2E42
loc_1_300A:
        mov    ecx, dword ptr [ebp + 10h]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp + 14h]
        mov    word ptr [eax], 0
        xor    eax, eax
        jmp    NEAR PTR loc_1_2E47
sub_1_301E:
        push   esi
        xor    esi, esi
        db  039h, 035h, 00Eh, 000h, 000h, 000h
        je     SHORT loc_1_303A
        db  0A1h, 00Eh, 000h, 000h, 000h
        push   eax
        call   sub_1_A598
        test   eax, eax
        je     SHORT loc_1_303A
        mov    esi, dword ptr [eax]
loc_1_303A:
        mov    eax, esi
        pop    esi
        ret
sub_1_303E:
        mov    eax, dword ptr [esp + 4]
        push   esi
        xor    esi, esi
        push   dword ptr [eax + 4]
        db  0FFh, 035h, 00Eh, 000h, 000h, 000h
        call   sub_1_A5A8
        test   eax, eax
        je     SHORT loc_1_3059
        mov    esi, dword ptr [eax]
loc_1_3059:
        mov    eax, esi
        pop    esi
        ret
sub_1_305D:
        mov    eax, dword ptr [esp + 4]
        mov    eax, dword ptr [eax + 2ch]
        add    eax, 2c4h
        ret
sub_1_306A:
        mov    eax, dword ptr [esp + 4]
        mov    eax, dword ptr [eax + 24h]
        ret
sub_1_3072:
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_9614
        add    esp, 4
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_98D2
        add    esp, 4
        xor    eax, eax
        ret
sub_1_308F:
        db  066h, 083h, 03Dh, 004h, 000h, 000h, 000h, 000h
        jne    SHORT loc_1_30CA
        db  066h, 083h, 03Dh, 006h, 000h, 000h, 000h, 000h
        jne    SHORT loc_1_30CA
        db  066h, 0FFh, 005h, 006h, 000h, 000h, 000h
        call   sub_1_3072
        test   eax, eax
        jne    SHORT loc_1_30CA
        db  068h, 00Eh, 000h, 000h, 000h
        push   4
        call   sub_1_9828
        add    esp, 8
        cmp    eax, 1
        sbb    eax, eax
        inc    eax
        jmp    SHORT loc_1_30CF
loc_1_30CA:
        mov    eax, 1
loc_1_30CF:
        ret
sub_1_30D0:
        xor    ecx, ecx
        db  0A1h, 000h, 000h, 000h, 000h
        db  066h, 089h, 00Dh, 000h, 000h, 000h, 000h
        cmp    eax, ecx
        je     SHORT loc_1_30EB
        push   ecx
        push   ecx
        push   1
        call   eax
        add    esp, 0ch
loc_1_30EB:
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_98F5
        add    esp, 4
        db  0A1h, 00Eh, 000h, 000h, 000h
        push   eax
        call   sub_1_9854
        add    esp, 4
        xor    eax, eax
        ret
sub_1_3109:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        push   ebx
        push   esi
        push   edi
        mov    esi, 1
        je     SHORT loc_1_3158
        mov    ax, word ptr [ebp + 0ch]
        db  066h, 039h, 005h, 000h, 000h, 000h, 000h
        je     SHORT loc_1_313F
        db  00Fh, 0B7h, 005h, 000h, 000h, 000h, 000h
        movzx  ecx, word ptr [ebp + 0ch]
        sub    eax, ecx
        cmp    eax, -1
        jne    SHORT loc_1_3158
loc_1_313F:
        push   dword ptr [ebp + 14h]
        push   dword ptr [ebp + 10h]
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp + 8]
        call   sub_1_92B5
        add    esp, 10h
        jmp    NEAR PTR loc_1_3384
loc_1_3158:
        lea    eax, [ebp - 2]
        lea    ecx, [ebp - 8]
        movzx  edi, word ptr [ebp + 0ch]
        push   eax
        push   ecx
        push   edi
        push   1
        call   sub_1_2E0C
        add    esp, 10h
        test   eax, eax
        je     SHORT loc_1_317D
        mov    eax, 1
        jmp    NEAR PTR loc_1_3386
loc_1_317D:
        mov    eax, dword ptr [ebp - 8]
        test   byte ptr [eax], 4
        je     NEAR PTR loc_1_320F
        mov    ecx, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0d8h], ecx
        jne    NEAR PTR loc_1_3384
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     SHORT loc_1_31F5
        mov    ax, word ptr [ebp + 0ch]
        db  066h, 039h, 005h, 000h, 000h, 000h, 000h
        je     SHORT loc_1_31BC
        db  00Fh, 0B7h, 005h, 000h, 000h, 000h, 000h
        sub    eax, edi
        cmp    eax, -1
        jne    SHORT loc_1_31F5
loc_1_31BC:
        db  00Fh, 0B7h, 015h, 000h, 000h, 000h, 000h
        inc    edx
        mov    al, 3fh
        out    dx, al
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_1_95FE
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        add    esp, 8
        inc    ax
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_1_95FE
        db  066h, 08Bh, 015h, 000h, 000h, 000h, 000h
        add    esp, 8
        in     al, dx
loc_1_31F5:
        cmp    dword ptr [ebp + 14h], 0
        je     NEAR PTR loc_1_3373
        push   dword ptr [ebp + 10h]
        push   dword ptr [ebp + 0ch]
        call   sub_1_9CE4
        jmp    NEAR PTR loc_1_3381
loc_1_320F:
        call   sub_1_A58C
        cmp    eax, dword ptr [ebp + 8]
        jne    SHORT loc_1_3233
        cmp    dword ptr [ebp + 14h], 0
        je     NEAR PTR loc_1_3373
        push   dword ptr [ebp + 10h]
        push   dword ptr [ebp + 0ch]
        call   sub_1_9CE4
        jmp    NEAR PTR loc_1_3381
loc_1_3233:
        xor    di, di
        mov    ebx, 0ech
loc_1_323B:
        push   dword ptr [ebp + 8]
        movzx  eax, di
        imul   eax, ebx
        add    eax, dword ptr [ebp - 8]
        add    eax, ebx
        push   eax
        call   sub_1_A04E
        add    esp, 8
        cmp    eax, 4
        jne    SHORT loc_1_3259
        xor    esi, esi
loc_1_3259:
        inc    di
        cmp    di, 2
        jb     SHORT loc_1_323B
        xor    di, di
loc_1_3264:
        movzx  eax, di
        imul   eax, eax, 0e0h
        push   0
        add    eax, dword ptr [ebp - 8]
        push   dword ptr [ebp + 8]
        add    eax, 2eeh
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_3280:
        add    esp, 0ch
        cmp    eax, 4
        jne    SHORT loc_1_328A
        xor    esi, esi
loc_1_328A:
        inc    di
        cmp    di, 1
        jb     SHORT loc_1_3264
        xor    di, di
loc_1_3295:
        movzx  eax, di
        imul   eax, eax, 5ah
        push   0
        add    eax, dword ptr [ebp - 8]
        push   dword ptr [ebp + 8]
        add    eax, 3ceh
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_32AE:
        add    esp, 0ch
        cmp    eax, 4
        jne    SHORT loc_1_32B8
        xor    esi, esi
loc_1_32B8:
        inc    di
        cmp    di, 1
        jb     SHORT loc_1_3295
        test   esi, esi
        jne    SHORT loc_1_333A
        xor    si, si
        mov    edi, 0ech
loc_1_32CC:
        push   dword ptr [ebp + 8]
        movzx  eax, si
        imul   eax, edi
        inc    si
        add    eax, dword ptr [ebp - 8]
        add    eax, edi
        push   eax
        call   sub_1_A09E
        add    esp, 8
        cmp    si, 2
        jb     SHORT loc_1_32CC
        xor    si, si
loc_1_32EE:
        push   dword ptr [ebp + 8]
        movzx  eax, si
        inc    si
        imul   eax, eax, 0e0h
        add    eax, dword ptr [ebp - 8]
        add    eax, 2eeh
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_330A:
        add    esp, 8
        cmp    si, 1
        jb     SHORT loc_1_32EE
        xor    si, si
loc_1_3316:
        push   dword ptr [ebp + 8]
        movzx  eax, si
        inc    si
        imul   eax, eax, 5ah
        add    eax, dword ptr [ebp - 8]
        add    eax, 3ceh
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_332F:
        add    esp, 8
        cmp    si, 1
        jb     SHORT loc_1_3316
        jmp    SHORT loc_1_3384
loc_1_333A:
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp + 8]
        or     byte ptr [eax], 4
        mov    eax, dword ptr [ebp - 8]
        push   2
        push   ecx
        mov    dword ptr [eax + 0d8h], ecx
        mov    eax, dword ptr [ebp - 8]
        add    eax, 67dh
        push   eax
        call   sub_1_4614
        add    esp, 0ch
        cmp    dword ptr [ebp + 14h], 0
        je     SHORT loc_1_3373
        push   dword ptr [ebp + 10h]
        push   dword ptr [ebp + 0ch]
        call   sub_1_9CE4
        jmp    SHORT loc_1_3381
loc_1_3373:
        mov    eax, dword ptr [ebp + 10h]
        mov    al, byte ptr [eax]
        push   eax
        push   dword ptr [ebp + 0ch]
        call   sub_1_9CF5
loc_1_3381:
        add    esp, 8
loc_1_3384:
        xor    eax, eax
loc_1_3386:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_1_338D:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        lea    eax, [ebp - 2]
        push   esi
        lea    ecx, [ebp - 8]
        push   edi
        mov    esi, 1
        movzx  edx, word ptr [ebp + 0ch]
        push   eax
        push   ecx
        push   edx
        push   esi
        call   sub_1_2E0C
        add    esp, 10h
        test   eax, eax
        je     SHORT loc_1_33BC
        mov    eax, esi
        jmp    NEAR PTR loc_1_34B6
loc_1_33BC:
        mov    eax, dword ptr [ebp - 8]
        test   byte ptr [eax], 8
        je     SHORT loc_1_33ED
        mov    edi, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0dch], edi
        jne    NEAR PTR loc_1_34B4
        cmp    dword ptr [ebp + 14h], 0
        je     NEAR PTR loc_1_34A3
        push   dword ptr [ebp + 10h]
        push   dword ptr [ebp + 0ch]
        call   sub_1_9CE4
        jmp    NEAR PTR loc_1_34B1
loc_1_33ED:
        call   sub_1_A58C
        mov    edi, dword ptr [ebp + 8]
        cmp    eax, edi
        jne    SHORT loc_1_3413
        cmp    dword ptr [ebp + 14h], 0
        je     NEAR PTR loc_1_34A3
        push   dword ptr [ebp + 10h]
        push   dword ptr [ebp + 0ch]
        call   sub_1_9CE4
        jmp    NEAR PTR loc_1_34B1
loc_1_3413:
        xor    bx, bx
loc_1_3416:
        movzx  eax, bx
        imul   eax, eax, 229h
        push   0
        add    eax, dword ptr [ebp - 8]
        push   edi
        add    eax, 428h
        push   eax
        call   sub_1_63E1
        add    esp, 0ch
        test   eax, eax
        je     SHORT loc_1_3439
        xor    esi, esi
loc_1_3439:
        inc    bx
        cmp    bx, 1
        jb     SHORT loc_1_3416
        test   esi, esi
        jne    SHORT loc_1_346D
        xor    si, si
loc_1_3448:
        movzx  eax, si
        inc    si
        push   edi
        imul   eax, eax, 229h
        add    eax, dword ptr [ebp - 8]
        add    eax, 428h
        push   eax
        call   sub_1_6531
        add    esp, 8
        cmp    si, 1
        jb     SHORT loc_1_3448
        jmp    SHORT loc_1_34B4
loc_1_346D:
        mov    eax, dword ptr [ebp - 8]
        push   2
        push   edi
        or     byte ptr [eax], 8
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax + 0dch], edi
        mov    eax, dword ptr [ebp - 8]
        add    eax, 67dh
        push   eax
        call   sub_1_46AB
        add    esp, 0ch
        cmp    dword ptr [ebp + 14h], 0
        je     SHORT loc_1_34A3
        push   dword ptr [ebp + 10h]
        push   dword ptr [ebp + 0ch]
        call   sub_1_9CE4
        jmp    SHORT loc_1_34B1
loc_1_34A3:
        mov    eax, dword ptr [ebp + 10h]
        mov    al, byte ptr [eax]
        push   eax
        push   dword ptr [ebp + 0ch]
        call   sub_1_9CF5
loc_1_34B1:
        add    esp, 8
loc_1_34B4:
        xor    eax, eax
loc_1_34B6:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_1_34BD:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        test   byte ptr [edi + 4], 1
        je     NEAR PTR loc_1_3744
        lea    esi, [edi + 0d5h]
        push   1
        mov    ax, word ptr [esi]
        add    ax, 6
        push   eax
        call   sub_1_9CF5
        xor    ax, ax
        add    esp, 8
loc_1_34EE:
        inc    ax
        cmp    ax, 0ffffh
        jb     SHORT loc_1_34EE
        mov    ax, word ptr [esi]
        push   0
        add    ax, 6
        xor    bx, bx
        push   eax
        call   sub_1_9CF5
        add    esp, 8
loc_1_350B:
        lea    eax, [ebp - 1]
        push   eax
        mov    ax, word ptr [esi]
        add    ax, 0eh
        push   eax
        call   sub_1_9CE4
        add    esp, 8
        test   byte ptr [ebp - 1], 80h
        je     SHORT loc_1_353F
        lea    eax, [ebp - 1]
        push   eax
        mov    ax, word ptr [esi]
        add    ax, 0ah
        push   eax
        call   sub_1_9CE4
        add    esp, 8
        cmp    byte ptr [ebp - 1], 0aah
        je     SHORT loc_1_3578
loc_1_353F:
        inc    bx
        cmp    bx, 0ffffh
        jb     SHORT loc_1_350B
        cmp    byte ptr [ebp - 1], 0aah
        je     SHORT loc_1_3578
        push   1
        lea    eax, [ebp - 4]
        mov    word ptr [ebp - 4], 0
        push   2
        push   eax
        push   3
        db  068h, 000h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        push   dword ptr [edi + 8]
        call   sub_1_A5B8
        add    esp, 1ch
        jmp    NEAR PTR loc_1_3744
loc_1_3578:
        test   byte ptr [edi + 4], 2
        je     NEAR PTR loc_1_3631
        mov    dword ptr [ebp - 8], 9
        lea    ebx, [edi + 0d7h]
loc_1_358F:
        mov    ax, word ptr [ebx]
        push   0ffh
        inc    ax
        push   eax
        call   sub_1_9CF5
        mov    word ptr [ebp - 4], 0
        add    esp, 8
loc_1_35A8:
        lea    eax, [ebp - 1]
        push   eax
        mov    ax, word ptr [ebx]
        inc    ax
        push   eax
        call   sub_1_9CE4
        add    esp, 8
        test   byte ptr [ebp - 1], 40h
        je     SHORT loc_1_35CC
        inc    word ptr [ebp - 4]
        cmp    word ptr [ebp - 4], 0ffffh
        jb     SHORT loc_1_35A8
loc_1_35CC:
        mov    ax, word ptr [ebx]
        push   0ffh
        inc    ax
        push   eax
        call   sub_1_9CF5
        mov    word ptr [ebp - 4], 0
        add    esp, 8
loc_1_35E5:
        lea    eax, [ebp - 1]
        push   eax
        mov    ax, word ptr [ebx]
        inc    ax
        push   eax
        call   sub_1_9CE4
        add    esp, 8
        test   byte ptr [ebp - 1], 80h
        je     SHORT loc_1_3609
        inc    word ptr [ebp - 4]
        cmp    word ptr [ebp - 4], 0ffffh
        jb     SHORT loc_1_35E5
loc_1_3609:
        mov    cx, word ptr [ebx]
        lea    eax, [ebp - 1]
        push   eax
        push   ecx
        call   sub_1_9CE4
        add    esp, 8
        cmp    byte ptr [ebp - 1], 0feh
        je     SHORT loc_1_3631
        and    byte ptr [edi + 4], 0fdh
        mov    eax, dword ptr [ebp - 8]
        dec    dword ptr [ebp - 8]
        test   eax, eax
        jne    NEAR PTR loc_1_358F
loc_1_3631:
        test   byte ptr [edi + 4], 2
        mov    word ptr [ebp - 4], 0
        jne    SHORT loc_1_365C
        push   1
        lea    eax, [ebp - 4]
        push   2
        push   eax
        push   3
        db  068h, 000h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        push   dword ptr [edi + 8]
        call   sub_1_A5B8
        add    esp, 1ch
loc_1_365C:
        test   byte ptr [edi], 2
        je     SHORT loc_1_36AD
        mov    ax, word ptr [esi]
        push   eax
        call   sub_1_3C93
        add    esp, 4
        test   eax, eax
        je     NEAR PTR loc_1_3740
        test   byte ptr [edi + 4], 20h
        mov    ax, word ptr [esi]
        push   eax
        jne    SHORT loc_1_368C
        push   dword ptr [edi + 8]
        call   sub_1_3EB9
        jmp    NEAR PTR loc_1_373D
loc_1_368C:
        call   sub_1_3F76
        add    esp, 4
        test   eax, eax
        je     NEAR PTR loc_1_3740
        mov    ax, word ptr [esi]
        push   eax
        push   dword ptr [edi + 8]
        call   sub_1_3FE6
        jmp    NEAR PTR loc_1_373D
loc_1_36AD:
        movzx  eax, byte ptr [edi + 0dfh]
        cmp    eax, 5
        je     SHORT loc_1_36CA
        cmp    eax, 7
        je     SHORT loc_1_36CE
        cmp    eax, 9
        je     SHORT loc_1_36D2
        cmp    eax, 0ah
        je     SHORT loc_1_36D6
        jmp    SHORT loc_1_3744
loc_1_36CA:
        mov    bl, 2
        jmp    SHORT loc_1_36D8
loc_1_36CE:
        mov    bl, 4
        jmp    SHORT loc_1_36D8
loc_1_36D2:
        mov    bl, 1
        jmp    SHORT loc_1_36D8
loc_1_36D6:
        mov    bl, 8
loc_1_36D8:
        mov    ax, word ptr [esi]
        push   80h
        add    ax, 4
        push   eax
        call   sub_1_9CF5
        mov    ax, word ptr [esi]
        add    esp, 8
        add    ax, 5
        push   ebx
        push   eax
        mov    bl, 1
        call   sub_1_9CF5
        add    esp, 8
        mov    cl, byte ptr [edi + 0ddh]
        shl    bl, cl
        test   byte ptr [edi + 4], 20h
        je     SHORT loc_1_371A
        mov    al, 1
        mov    cl, byte ptr [edi + 0deh]
        shl    al, cl
        or     bl, al
loc_1_371A:
        mov    ax, word ptr [esi]
        push   81h
        add    ax, 4
        push   eax
        call   sub_1_9CF5
        mov    ax, word ptr [esi]
        add    esp, 8
        add    ax, 5
        push   ebx
        push   eax
        call   sub_1_9CF5
loc_1_373D:
        add    esp, 8
loc_1_3740:
        xor    eax, eax
        jmp    SHORT loc_1_3749
loc_1_3744:
        mov    eax, 1
loc_1_3749:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_1_3750:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   edi
        mov    si, word ptr [eax + 0d5h]
        push   0e0h
        add    si, 800h
        lea    edi, [esi + 402h]
        push   edi
        call   sub_1_9D15
        add    esp, 8
        lea    eax, [ebp - 2]
        push   eax
        mov    ax, si
        add    ax, 400h
        push   eax
        call   sub_1_9D02
        add    esp, 8
        test   byte ptr [ebp - 2], 0ch
        jne    SHORT loc_1_379D
        mov    eax, 1
        jmp    SHORT loc_1_3805
loc_1_379D:
        push   3dh
        push   edi
        call   sub_1_9D15
        add    esp, 8
        lea    eax, [ebp - 2]
        push   eax
        push   esi
        call   sub_1_9D02
        add    esp, 8
        test   byte ptr [ebp - 2], 58h
        jne    SHORT loc_1_37C2
        mov    eax, 1
        jmp    SHORT loc_1_3805
loc_1_37C2:
        push   3eh
        push   edi
        call   sub_1_9D15
        add    esp, 8
        lea    eax, [ebp - 2]
        push   eax
        push   esi
        call   sub_1_9D02
        add    esp, 8
        mov    eax, 1
        test   byte ptr [ebp - 2], 3
        je     SHORT loc_1_3805
        push   3fh
        push   edi
        call   sub_1_9D15
        add    esp, 8
        lea    eax, [ebp - 2]
        push   eax
        push   esi
        call   sub_1_9D02
        add    esp, 8
        cmp    word ptr [ebp - 2], 1
        sbb    eax, eax
        inc    eax
loc_1_3805:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
sub_1_380B:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        lea    eax, [ebp - 2]
        lea    ecx, [ebp - 8]
        push   eax
        push   ecx
        push   dword ptr [ebp + 8]
        push   4
        call   sub_1_2E0C
        add    esp, 10h
        test   eax, eax
        mov    eax, 1
        jne    SHORT loc_1_3842
        xor    eax, eax
        mov    ecx, dword ptr [ebp - 8]
        cmp    eax, dword ptr [ecx + 0e0h]
        mov    eax, 1
        adc    eax, -1
loc_1_3842:
        mov    esp, ebp
        pop    ebp
        ret
sub_1_3846:
        push   dword ptr [esp + 0ch]
        push   dword ptr [esp + 0ch]
        push   dword ptr [esp + 0ch]
        call   sub_1_5856
        add    esp, 0ch
        ret
sub_1_385B:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        db  066h, 083h, 03Dh, 00Ch, 000h, 000h, 000h, 004h
        jae    SHORT loc_1_38DA
        lea    eax, [ebp - 8]
        push   eax
        db  0FFh, 035h, 00Eh, 000h, 000h, 000h
        call   sub_1_97E9
        add    esp, 8
        test   eax, eax
        jne    SHORT loc_1_38DA
        push   dword ptr [ebp - 8]
        db  0FFh, 035h, 00Eh, 000h, 000h, 000h
        call   sub_1_9812
        add    esp, 8
        lea    ecx, [ebp - 4]
        push   ecx
        push   7abh
        call   sub_1_9890
        add    esp, 8
        test   eax, eax
        jne    SHORT loc_1_38DA
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 4]
        mov    edx, dword ptr [ebp - 8]
        mov    dword ptr [eax + 4], edx
        mov    edx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx], 0
        mov    eax, dword ptr [ebp - 4]
        or     byte ptr [eax], 1
        xor    eax, eax
        db  066h, 0FFh, 005h, 00Ch, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [edx], ecx
        jmp    SHORT loc_1_38DF
loc_1_38DA:
        mov    eax, 1
loc_1_38DF:
        mov    esp, ebp
        pop    ebp
        ret
sub_1_38E3:
        push   esi
        mov    esi, dword ptr [esp + 8]
        push   dword ptr [esi + 4]
        db  0FFh, 035h, 00Eh, 000h, 000h, 000h
        call   sub_1_987A
        add    esp, 8
        push   dword ptr [esi + 4]
        db  0FFh, 035h, 00Eh, 000h, 000h, 000h
        call   sub_1_9864
        db  066h, 08Bh, 00Dh, 000h, 000h, 000h, 000h
        add    esp, 8
        cmp    word ptr [esi + 683h], cx
        jne    SHORT loc_1_3937
        xor    eax, eax
        db  066h, 0A3h, 000h, 000h, 000h, 000h
        db  039h, 005h, 000h, 000h, 000h, 000h
        je     SHORT loc_1_3937
        push   eax
        push   eax
        push   6
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        add    esp, 0ch
loc_1_3937:
        push   esi
        call   sub_1_98BE
        db  066h, 0FFh, 00Dh, 00Ch, 000h, 000h, 000h
        add    esp, 4
        xor    eax, eax
        pop    esi
        ret
sub_1_394B:
        mov    eax, dword ptr [esp + 8]
        cmp    eax, 2
        je     SHORT loc_1_395B
        cmp    eax, 3
        je     SHORT loc_1_3967
        jmp    SHORT loc_1_397E
loc_1_395B:
        mov    ecx, dword ptr [esp + 4]
        inc    dword ptr [ecx + 0e0h]
        jmp    SHORT loc_1_397A
loc_1_3967:
        mov    ecx, dword ptr [esp + 4]
        add    ecx, 0e0h
        mov    eax, dword ptr [ecx]
        test   eax, eax
        je     SHORT loc_1_397E
        dec    eax
        mov    dword ptr [ecx], eax
loc_1_397A:
        xor    eax, eax
        jmp    SHORT loc_1_3983
loc_1_397E:
        mov    eax, 1
loc_1_3983:
        ret
sub_1_3984:
        mov    eax, dword ptr [esp + 4]
        mov    edx, dword ptr [esp + 8]
        mov    cx, word ptr [eax + 7a5h]
        xor    eax, eax
        mov    word ptr [edx], cx
        ret
sub_1_3999:
        push   dword ptr [esp + 8]
        mov    eax, dword ptr [esp + 8]
        add    eax, 67dh
        push   eax
        call   sub_1_4807
        add    esp, 8
        ret
sub_1_39B0:
        mov    eax, dword ptr [esp + 4]
        mov    ecx, dword ptr [esp + 8]
        add    eax, 713h
        mov    dword ptr [ecx], eax
        ret
sub_1_39C0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        mov    si, word ptr [ebp + 8]
        push   edi
        mov    ax, si
        push   0e3h
        add    ax, 0ch
        xor    di, di
        push   eax
        call   sub_1_9CF5
        add    esp, 8
loc_1_39E5:
        xor    bx, bx
loc_1_39E8:
        lea    eax, [ebp - 1]
        lea    ecx, [esi + 0eh]
        push   eax
        push   ecx
        call   sub_1_9CE4
        add    esp, 8
        test   byte ptr [ebp - 1], 80h
        jne    SHORT loc_1_3A0D
        inc    bx
        cmp    bx, 200h
        jb     SHORT loc_1_39E8
        test   byte ptr [ebp - 1], 80h
        je     SHORT loc_1_3A52
loc_1_3A0D:
        lea    eax, [ebp - 1]
        push   eax
        mov    ax, si
        add    ax, 0ah
        push   eax
        call   sub_1_9CE4
        add    esp, 8
        cmp    byte ptr [ebp - 1], 0
        je     SHORT loc_1_3A49
        cmp    di, 28h
        jae    SHORT loc_1_3A41
        db  0A1h, 030h, 000h, 000h, 000h
        movzx  ecx, di
        movsx  edx, byte ptr [ecx + eax]
        movzx  ecx, byte ptr [ebp - 1]
        cmp    edx, ecx
        jne    SHORT loc_1_3A59
loc_1_3A41:
        inc    di
        cmp    di, 2dh
        jb     SHORT loc_1_39E5
loc_1_3A49:
        xor    eax, eax
loc_1_3A4B:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_1_3A52:
        mov    eax, 1
        jmp    SHORT loc_1_3A4B
loc_1_3A59:
        mov    eax, 1
        jmp    SHORT loc_1_3A4B
sub_1_3A60:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 4
        test   eax, eax
        push   esi
        je     SHORT loc_1_3ACC
        cmp    eax, 2
        jb     SHORT loc_1_3A85
        cmp    eax, 3
        jbe    SHORT loc_1_3ACC
        cmp    eax, 1d4048h
        je     SHORT loc_1_3A8C
        cmp    eax, -1
        je     SHORT loc_1_3ACC
loc_1_3A85:
        mov    eax, 1
        jmp    SHORT loc_1_3ACE
loc_1_3A8C:
        lea    eax, [ebp - 2]
        push   eax
        db  0FFh, 035h, 030h, 000h, 000h, 000h
        call   sub_1_23AF
        add    esp, 8
        mov    esi, dword ptr [ebp + 14h]
        movzx  ecx, word ptr [ebp - 2]
        cmp    dword ptr [esi + 1ch], ecx
        jae    SHORT loc_1_3AB1
        mov    eax, 1
        jmp    SHORT loc_1_3ACE
loc_1_3AB1:
        db  0A1h, 030h, 000h, 000h, 000h
        push   eax
        push   dword ptr [esi + 18h]
        call   sub_1_23D2
        mov    cx, word ptr [ebp - 2]
        add    esp, 8
        mov    edx, dword ptr [esi + 20h]
        mov    word ptr [edx], cx
loc_1_3ACC:
        xor    eax, eax
loc_1_3ACE:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
sub_1_3AD3:
        push   ebp
        mov    ax, word ptr [esp + 8]
        mov    ebp, esp
        push   dword ptr [esp + 0ch]
        add    ax, 4
        push   eax
        call   sub_1_9CF5
        mov    ax, word ptr [ebp + 8]
        add    esp, 8
        push   dword ptr [ebp + 10h]
        add    ax, 5
        push   eax
        call   sub_1_9CF5
        mov    esp, ebp
        pop    ebp
        ret
sub_1_3B01:
        push   ebp
        mov    ax, word ptr [esp + 8]
        mov    ebp, esp
        add    ax, 4
        sub    esp, 4
        push   dword ptr [ebp + 0ch]
        push   eax
        call   sub_1_9CF5
        mov    ax, word ptr [ebp + 8]
        add    esp, 8
        add    ax, 5
        lea    ecx, [ebp - 1]
        push   ecx
        push   eax
        call   sub_1_9CE4
        movzx  ax, byte ptr [ebp - 1]
        add    esp, 8
        mov    esp, ebp
        pop    ebp
        ret
sub_1_3B3A:
        sub    esp, 4
        push   ebx
        push   esi
        xor    bx, bx
        push   edi
        mov    di, word ptr [esp + 14h]
        push   ebp
        push   82h
        push   edi
        call   sub_1_3B01
        and    ax, 0f0h
        add    esp, 8
        cmp    ax, 10h
        mov    si, ax
        je     NEAR PTR loc_1_3C60
        cmp    si, 20h
        je     NEAR PTR loc_1_3C60
        cmp    si, 80h
        je     NEAR PTR loc_1_3C60
        cmp    si, 0a0h
        je     NEAR PTR loc_1_3C5B
        cmp    si, 0b0h
        je     NEAR PTR loc_1_3C5B
        cmp    si, 0c0h
        je     NEAR PTR loc_1_3C5B
        push   edi
        call   sub_1_3C93
        mov    dword ptr [esp + 14h], eax
        add    esp, 4
        test   eax, eax
        je     SHORT loc_1_3BBF
        push   14h
        push   81h
        push   edi
        call   sub_1_3AD3
        add    esp, 0ch
loc_1_3BBF:
        cmp    si, 40h
        jne    SHORT loc_1_3C01
        mov    si, 4
        mov    ebp, 0ffh
loc_1_3BCE:
        push   esi
        push   ebp
        push   edi
        call   sub_1_3AD3
        add    esp, 0ch
        push   ebp
        push   edi
        call   sub_1_3B01
        add    esp, 8
        test   ax, ax
        je     SHORT loc_1_3BED
        cmp    ax, bp
        jne    SHORT loc_1_3BF7
loc_1_3BED:
        inc    si
        cmp    si, 5
        jbe    SHORT loc_1_3BCE
        jmp    SHORT loc_1_3C01
loc_1_3BF7:
        shl    ax, 8
        or     ax, si
        mov    bx, ax
loc_1_3C01:
        test   bx, bx
        jne    SHORT loc_1_3C37
        push   0feh
        push   edi
        call   sub_1_3B01
        add    esp, 8
        test   ax, ax
        je     SHORT loc_1_3C37
        cmp    ax, 0ffh
        je     SHORT loc_1_3C37
        mov    bx, ax
        push   0fdh
        push   edi
        call   sub_1_3B01
        shl    ax, 8
        add    esp, 8
        or     bx, ax
loc_1_3C37:
        cmp    dword ptr [esp + 10h], 0
        je     SHORT loc_1_3C88
        mov    esi, 81h
        push   esi
        push   edi
        call   sub_1_3B01
        add    esp, 8
        push   1
        push   esi
        push   edi
        call   sub_1_3AD3
        add    esp, 0ch
        jmp    SHORT loc_1_3C88
loc_1_3C5B:
        xor    ax, ax
        jmp    SHORT loc_1_3C8B
loc_1_3C60:
        push   3
        mov    esi, 0ffh
        push   esi
        push   edi
        call   sub_1_3AD3
        add    esp, 0ch
        push   esi
        push   edi
        call   sub_1_3B01
        add    esp, 8
        cmp    ax, 60h
        jne    SHORT loc_1_3C88
        mov    bx, ax
        mov    bh, bl
        mov    bl, 3
loc_1_3C88:
        mov    ax, bx
loc_1_3C8B:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
sub_1_3C93:
        push   ebx
        push   esi
        push   edi
        mov    esi, 0ffh
        mov    di, word ptr [esp + 10h]
        push   ebp
        push   20h
        push   esi
        push   edi
        call   sub_1_3AD3
        add    esp, 0ch
        push   esi
        push   edi
        call   sub_1_3B01
        add    esp, 8
        cmp    ax, 60h
        jne    SHORT loc_1_3D01
        mov    ebp, 81h
        push   ebp
        push   edi
        call   sub_1_3B01
        mov    si, ax
        add    esp, 8
        push   14h
        push   ebp
        push   edi
        call   sub_1_3AD3
        add    esp, 0ch
        xor    ebx, ebx
        push   ebp
        push   edi
        call   sub_1_3B01
        mov    bx, ax
        add    esp, 8
        and    bl, 0ebh
        push   esi
        push   ebp
        push   edi
        call   sub_1_3AD3
        add    esp, 0ch
        mov    eax, 1
        test   bl, bl
        jne    SHORT loc_1_3D03
loc_1_3D01:
        xor    eax, eax
loc_1_3D03:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_1_3D08:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   edi
        mov    si, word ptr [ebp + 8]
        push   84h
        lea    edi, [esi + 4]
        add    si, 5
        push   edi
        call   sub_1_9CF5
        add    esp, 8
        lea    eax, [ebp - 1]
        push   eax
        push   esi
        call   sub_1_9CE4
        add    esp, 8
        cmp    word ptr [ebp + 0ch], 300h
        jne    SHORT loc_1_3D48
        and    byte ptr [ebp - 1], 0fdh
        or     byte ptr [ebp - 1], 4
        jmp    SHORT loc_1_3D54
loc_1_3D48:
        cmp    word ptr [ebp + 0ch], 330h
        jne    SHORT loc_1_3D54
        and    byte ptr [ebp - 1], 0f9h
loc_1_3D54:
        push   84h
        push   edi
        call   sub_1_9CF5
        add    esp, 8
        push   dword ptr [ebp - 1]
        push   esi
        call   sub_1_9CF5
        add    esp, 8
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
sub_1_3D74:
        push   ebp
        mov    ebp, esp
        sub    esp, 14h
        mov    word ptr [ebp - 14h], 0ffffh
        mov    word ptr [ebp - 12h], 0
        mov    word ptr [ebp - 10h], 0ff00h
        mov    word ptr [ebp - 0eh], 100h
        mov    word ptr [ebp - 0ch], 0fe00h
        mov    word ptr [ebp - 0ah], 200h
        mov    word ptr [ebp - 8], 0ffffh
        mov    word ptr [ebp - 6], 420h
        push   esi
        push   edi
        mov    esi, 5
        mov    eax, 1
        mov    word ptr [ebp - 4], 0fffeh
        movzx  cx, byte ptr [ebp + 0ch]
        mov    word ptr [ebp - 2], 520h
        movzx  di, byte ptr [ebp + 8]
        shl    di, 8
        or     di, cx
        lea    ecx, [ebp - 4]
loc_1_3DD6:
        mov    dx, word ptr [ecx]
        and    dx, di
        cmp    dx, word ptr [ecx + 2]
        je     SHORT loc_1_3DEC
        sub    ecx, 4
        dec    esi
        test   esi, esi
        jg     SHORT loc_1_3DD6
        jmp    SHORT loc_1_3DEE
loc_1_3DEC:
        xor    eax, eax
loc_1_3DEE:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
sub_1_3DF4:
        push   ebx
        push   esi
        push   edi
        mov    ebx, 90h
        mov    di, word ptr [esp + 10h]
        push   ebx
        push   edi
        call   sub_1_3B01
        mov    si, ax
        add    esp, 8
        xor    si, 1
        push   esi
        push   ebx
        push   edi
        call   sub_1_3AD3
        add    esp, 0ch
        push   ebx
        push   edi
        call   sub_1_3B01
        sub    si, ax
        add    esp, 8
        cmp    si, 1
        pop    edi
        sbb    eax, eax
        pop    esi
        neg    eax
        pop    ebx
        ret
sub_1_3E36:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        mov    si, word ptr [ebp + 10h]
        test   si, si
        je     SHORT loc_1_3E76
        lea    eax, [ebp - 1]
        push   eax
        push   esi
        call   sub_1_9CE4
        add    esp, 8
        mov    al, byte ptr [ebp + 8]
        test   al, al
        jne    SHORT loc_1_3E60
        and    byte ptr [ebp - 1], 0feh
        jmp    SHORT loc_1_3E68
loc_1_3E60:
        cmp    al, 1
        jne    SHORT loc_1_3E68
        or     byte ptr [ebp - 1], 1
loc_1_3E68:
        push   dword ptr [ebp - 1]
        push   esi
        call   sub_1_9CF5
        add    esp, 8
        jmp    SHORT loc_1_3EB4
loc_1_3E76:
        push   90h
        push   dword ptr [ebp + 0ch]
        call   sub_1_3B01
        add    esp, 8
        mov    byte ptr [ebp - 1], al
        mov    al, byte ptr [ebp + 8]
        test   al, al
        jne    SHORT loc_1_3E96
        and    byte ptr [ebp - 1], 0feh
        jmp    SHORT loc_1_3E9E
loc_1_3E96:
        cmp    al, 1
        jne    SHORT loc_1_3E9E
        or     byte ptr [ebp - 1], 1
loc_1_3E9E:
        movzx  ax, byte ptr [ebp - 1]
        push   eax
        push   90h
        push   dword ptr [ebp + 0ch]
        call   sub_1_3AD3
        add    esp, 0ch
loc_1_3EB4:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
sub_1_3EB9:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        lea    eax, [ebp - 8]
        push   esi
        push   edi
        push   eax
        mov    edi, 81h
        call   sub_1_9B4D
        mov    si, word ptr [ebp + 0ch]
        add    esp, 4
        push   edi
        push   esi
        call   sub_1_3B01
        mov    bx, ax
        add    esp, 8
        push   14h
        push   edi
        push   esi
        call   sub_1_3AD3
        add    esp, 0ch
        push   edi
        push   esi
        call   sub_1_3B01
        mov    word ptr [ebp - 2], ax
        add    esp, 8
        push   ebx
        push   edi
        push   esi
        call   sub_1_3AD3
        add    esp, 0ch
        test   byte ptr [ebp - 2], 0ebh
        je     SHORT loc_1_3F45
        push   14h
        push   edi
        push   esi
        call   sub_1_3AD3
        mov    ax, word ptr [ebp - 2]
        add    esp, 0ch
        and    al, 0bh
        movzx  ax, al
        push   eax
        push   edi
        push   esi
        call   sub_1_3AD3
        add    esp, 0ch
        push   edi
        push   esi
        call   sub_1_3B01
        add    esp, 8
        push   eax
        push   edi
        push   esi
        call   sub_1_3AD3
        add    esp, 0ch
loc_1_3F45:
        push   dword ptr [ebp - 8]
        call   sub_1_9B5B
        add    esp, 4
        lea    eax, [ebp - 2]
        push   1
        push   1
        push   eax
        push   3
        db  068h, 000h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        push   dword ptr [ebp + 8]
        call   sub_1_A5B8
        add    esp, 1ch
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_1_3F76:
        sub    esp, 4
        lea    eax, [esp]
        push   ebx
        push   esi
        push   edi
        push   ebp
        push   eax
        mov    ebp, 81h
        call   sub_1_9B4D
        mov    si, word ptr [esp + 1ch]
        add    esp, 4
        push   ebp
        push   esi
        call   sub_1_3B01
        mov    di, ax
        add    esp, 8
        push   14h
        push   ebp
        push   esi
        call   sub_1_3AD3
        add    esp, 0ch
        push   ebp
        push   esi
        call   sub_1_3B01
        mov    bx, ax
        add    esp, 8
        push   edi
        push   ebp
        and    bx, 0e0h
        push   esi
        call   sub_1_3AD3
        add    esp, 0ch
        push   dword ptr [esp + 10h]
        call   sub_1_9B5B
        add    esp, 4
        cmp    bx, 1
        pop    ebp
        sbb    eax, eax
        pop    edi
        neg    eax
        pop    esi
        pop    ebx
        add    esp, 4
        ret
sub_1_3FE6:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        mov    word ptr [ebp - 2], 0
        push   esi
        lea    eax, [ebp - 8]
        mov    dword ptr [ebp - 0ch], 1
        push   edi
        push   eax
        call   sub_1_9B4D
        add    esp, 4
        lea    eax, [ebp - 0ch]
        lea    ecx, [ebp - 2]
        push   1
        push   eax
        push   ecx
        push   3
        db  068h, 000h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        mov    edi, 81h
        push   dword ptr [ebp + 8]
        call   sub_1_A5C0
        mov    si, word ptr [ebp + 0ch]
        add    esp, 1ch
        push   14h
        push   edi
        push   esi
        call   sub_1_3AD3
        add    esp, 0ch
        push   dword ptr [ebp - 2]
        push   edi
        push   esi
        call   sub_1_3AD3
        add    esp, 0ch
        push   edi
        push   esi
        call   sub_1_3B01
        mov    word ptr [ebp - 2], ax
        add    esp, 8
        push   dword ptr [ebp - 2]
        push   edi
        push   esi
        call   sub_1_3AD3
        add    esp, 0ch
        push   dword ptr [ebp - 8]
        call   sub_1_9B5B
        add    esp, 4
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
sub_1_4076:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    cx, word ptr [eax + 18h]
        mov    ebp, esp
        mov    ax, word ptr [eax + 1ah]
        sub    esp, 4
        mov    word ptr [ebp - 4], cx
        mov    word ptr [ebp - 2], ax
        mov    dx, word ptr [ebp - 4]
        mov    al, byte ptr [ebp + 0ch]
        out    dx, al
        mov    al, byte ptr [ebp + 10h]
        mov    dx, word ptr [ebp - 2]
        out    dx, al
        xor    eax, eax
        mov    esp, ebp
        pop    ebp
        ret
sub_1_40A6:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    cx, word ptr [eax + 18h]
        mov    ebp, esp
        mov    ax, word ptr [eax + 1ah]
        sub    esp, 4
        mov    word ptr [ebp - 4], cx
        push   esi
        mov    word ptr [ebp - 2], ax
        mov    dx, word ptr [ebp - 4]
        mov    al, byte ptr [ebp + 0ch]
        out    dx, al
        mov    esi, dword ptr [ebp + 10h]
        mov    dx, word ptr [ebp - 2]
        in     al, dx
        mov    byte ptr [esi], al
        xor    eax, eax
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
sub_1_40DA:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        xor    si, si
        push   edi
        mov    edi, dword ptr [ebp + 8]
loc_1_40E9:
        mov    ax, word ptr [edi + 22h]
        push   dword ptr [ebp + 0ch]
        xor    bx, bx
        push   eax
        call   sub_1_9CF5
        add    esp, 8
loc_1_40FC:
        mov    cx, word ptr [edi + 20h]
        lea    eax, [ebp - 1]
        push   eax
        push   ecx
        call   sub_1_9CE4
        add    esp, 8
        test   byte ptr [ebp - 1], 80h
        jne    SHORT loc_1_412A
        mov    cx, word ptr [edi + 1eh]
        lea    eax, [ebp - 1]
        push   eax
        push   ecx
        call   sub_1_9CE4
        add    esp, 8
        cmp    byte ptr [ebp - 1], 0feh
        je     SHORT loc_1_4147
loc_1_412A:
        inc    bx
        cmp    bx, 0ffffh
        jb     SHORT loc_1_40FC
        inc    si
        cmp    si, 0ah
        jb     SHORT loc_1_40E9
        mov    eax, 1
loc_1_4140:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_1_4147:
        xor    eax, eax
        jmp    SHORT loc_1_4140
sub_1_414B:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        xor    si, si
        push   edi
        mov    bx, word ptr [ebp + 10h]
        cmp    bx, si
        je     SHORT loc_1_4181
        mov    edi, dword ptr [ebp + 8]
loc_1_4163:
        mov    cx, word ptr [edi + 20h]
        lea    eax, [ebp - 1]
        push   eax
        push   ecx
        call   sub_1_9CE4
        add    esp, 8
        test   byte ptr [ebp - 1], 80h
        je     SHORT loc_1_418D
        inc    si
        cmp    si, bx
        jb     SHORT loc_1_4163
loc_1_4181:
        mov    eax, 1
loc_1_4186:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_1_418D:
        push   dword ptr [ebp + 0ch]
        mov    cx, word ptr [edi + 1eh]
        push   ecx
        call   sub_1_9CE4
        add    esp, 8
        xor    eax, eax
        jmp    SHORT loc_1_4186
sub_1_41A1:
        mov    eax, dword ptr [esp + 4]
        sub    esp, 4
        mov    ax, word ptr [eax + 14h]
        mov    word ptr [esp + 2], ax
        mov    dx, word ptr [esp + 2]
        in     al, dx
        xor    eax, eax
        add    esp, 4
        ret
sub_1_41BD:
        mov    eax, dword ptr [esp + 4]
        sub    esp, 4
        mov    ax, word ptr [eax + 16h]
        mov    word ptr [esp + 2], ax
        mov    dx, word ptr [esp + 2]
        in     al, dx
        xor    eax, eax
        add    esp, 4
        ret
sub_1_41D9:
        mov    eax, dword ptr [esp + 4]
        sub    esp, 4
        mov    ax, word ptr [eax + 1ch]
        mov    word ptr [esp + 2], ax
        mov    dx, word ptr [esp + 2]
        in     al, dx
        xor    eax, eax
        add    esp, 4
        ret
sub_1_41F5:
        push   ebp
        xor    eax, eax
        mov    dx, word ptr [esp + 0ch]
        mov    ebp, esp
        mov    ax, dx
        and    eax, 0f00h
        cmp    eax, 200h
        je     SHORT loc_1_421C
        cmp    eax, 300h
        je     SHORT loc_1_423A
        mov    eax, 1
        jmp    SHORT loc_1_4286
loc_1_421C:
        mov    ecx, dword ptr [ebp + 8]
        mov    ax, word ptr [ecx + 4]
        xor    ax, dx
        test   al, 0f0h
        jne    SHORT loc_1_4233
        test   byte ptr [ecx], 2
        je     SHORT loc_1_4233
        xor    eax, eax
        jmp    SHORT loc_1_4286
loc_1_4233:
        mov    eax, 1
        jmp    SHORT loc_1_4286
loc_1_423A:
        xor    eax, eax
        mov    ax, dx
        and    eax, 0f0h
        je     SHORT loc_1_4259
        cmp    eax, 30h
        je     SHORT loc_1_4259
        cmp    eax, 80h
        je     SHORT loc_1_4277
        mov    eax, 1
        jmp    SHORT loc_1_4286
loc_1_4259:
        mov    ecx, dword ptr [ebp + 8]
        mov    ax, word ptr [ecx + 6]
        xor    ax, dx
        test   al, 0f0h
        jne    SHORT loc_1_4270
        test   byte ptr [ecx], 4
        je     SHORT loc_1_4270
        xor    eax, eax
        jmp    SHORT loc_1_4286
loc_1_4270:
        mov    eax, 1
        jmp    SHORT loc_1_4286
loc_1_4277:
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx]
        and    eax, 8
        cmp    eax, 1
        sbb    eax, eax
        neg    eax
loc_1_4286:
        pop    ebp
        ret
sub_1_4288:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    byte ptr [ebp - 2], 80h
        mov    ax, word ptr [esi + 0ah]
        mov    cx, word ptr [esi + 12h]
        mov    dx, word ptr [esi + 10h]
        mov    word ptr [ebp - 8], ax
        mov    word ptr [ebp - 6], cx
        mov    word ptr [ebp - 4], dx
        mov    dx, word ptr [ebp - 8]
        mov    al, 1
        out    dx, al
        mov    ecx, 0ffffh
loc_1_42BB:
        loop   loc_1_42BB
        mov    al, 0
        out    dx, al
        mov    cx, word ptr [ebp + 0ch]
        mov    dx, word ptr [ebp - 6]
loc_1_42C8:
        in     al, dx
        and    al, byte ptr [ebp - 2]
        jne    SHORT loc_1_42D0
        loop   loc_1_42C8
loc_1_42D0:
        mov    dx, word ptr [ebp - 4]
        in     al, dx
        mov    byte ptr [ebp - 1], al
        cmp    byte ptr [ebp - 1], 0aah
        mov    eax, 1
        jne    SHORT loc_1_4338
        mov    edi, 0ffffh
        push   edi
        push   0fh
        push   esi
        call   sub_1_436F
        add    esp, 0ch
        push   edi
        push   5
        push   esi
        call   sub_1_436F
        add    esp, 0ch
        lea    eax, [ebp - 1]
        push   edi
        push   eax
        push   esi
        call   sub_1_43EF
        add    esp, 0ch
        push   edi
        push   0eh
        push   esi
        call   sub_1_436F
        add    esp, 0ch
        push   edi
        push   5
        push   esi
        call   sub_1_436F
        add    esp, 0ch
        mov    al, byte ptr [ebp - 1]
        or     al, 3
        push   edi
        push   eax
        push   esi
        call   sub_1_436F
        add    esp, 0ch
        xor    eax, eax
loc_1_4338:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
sub_1_433E:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    cx, word ptr [eax + 0eh]
        mov    ebp, esp
        mov    ax, word ptr [eax + 0ch]
        sub    esp, 4
        mov    word ptr [ebp - 4], cx
        mov    word ptr [ebp - 2], ax
        mov    dx, word ptr [ebp - 4]
loc_1_435C:
        in     al, dx
        or     al, al
        js     SHORT loc_1_435C
        mov    dx, word ptr [ebp - 2]
        mov    al, byte ptr [ebp + 0ch]
        out    dx, al
        xor    eax, eax
        mov    esp, ebp
        pop    ebp
        ret
sub_1_436F:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    cx, word ptr [eax + 0eh]
        mov    ebp, esp
        mov    ax, word ptr [eax + 0ch]
        sub    esp, 8
        mov    word ptr [ebp - 2], 0
        mov    word ptr [ebp - 6], cx
        mov    word ptr [ebp - 4], ax
        mov    dx, word ptr [ebp - 6]
        mov    cx, word ptr [ebp + 10h]
loc_1_4397:
        in     al, dx
        or     al, al
        jns    SHORT loc_1_43A6
        loop   loc_1_4397
        mov    word ptr [ebp - 2], 1
        jmp    SHORT loc_1_43AE
loc_1_43A6:
        mov    dx, word ptr [ebp - 4]
        mov    al, byte ptr [ebp + 0ch]
        out    dx, al
loc_1_43AE:
        cmp    word ptr [ebp - 2], 1
        sbb    eax, eax
        mov    esp, ebp
        inc    eax
        pop    ebp
        ret
sub_1_43BA:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    cx, word ptr [eax + 12h]
        mov    ebp, esp
        mov    ax, word ptr [eax + 10h]
        sub    esp, 4
        mov    word ptr [ebp - 4], cx
        push   esi
        mov    word ptr [ebp - 2], ax
        mov    dx, word ptr [ebp - 4]
loc_1_43D9:
        in     al, dx
        or     al, al
        jns    SHORT loc_1_43D9
        mov    dx, word ptr [ebp - 2]
        in     al, dx
        mov    esi, dword ptr [ebp + 0ch]
        mov    byte ptr [esi], al
        xor    eax, eax
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
sub_1_43EF:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    cx, word ptr [eax + 12h]
        mov    ebp, esp
        mov    ax, word ptr [eax + 10h]
        sub    esp, 8
        mov    word ptr [ebp - 6], cx
        push   esi
        mov    word ptr [ebp - 2], 0
        mov    word ptr [ebp - 4], ax
        mov    dx, word ptr [ebp - 6]
        mov    cx, word ptr [ebp + 10h]
loc_1_4418:
        in     al, dx
        or     al, al
        js     SHORT loc_1_4427
        loop   loc_1_4418
        mov    word ptr [ebp - 2], 1
        jmp    SHORT loc_1_4431
loc_1_4427:
        mov    dx, word ptr [ebp - 4]
        in     al, dx
        mov    esi, dword ptr [ebp + 0ch]
        mov    byte ptr [esi], al
loc_1_4431:
        cmp    word ptr [ebp - 2], 1
        sbb    eax, eax
        pop    esi
        inc    eax
        mov    esp, ebp
        pop    ebp
        ret
sub_1_443E:
        mov    eax, dword ptr [esp + 4]
        mov    word ptr [eax + 4], 220h
        mov    word ptr [eax + 6], 330h
        mov    dword ptr [eax], 0
        xor    eax, eax
        ret
sub_1_4457:
        push   ebp
        mov    ebp, esp
        push   ebx
        mov    cx, word ptr [ebp + 0ch]
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        lea    eax, [ecx + 6]
        lea    edi, [esi + 4]
        mov    word ptr [edi], cx
        lea    edx, [ecx + 0ah]
        mov    word ptr [esi + 0ah], ax
        or     byte ptr [esi], 1
        mov    word ptr [esi + 10h], dx
        lea    eax, [ecx + 0ch]
        mov    word ptr [esi + 0ch], ax
        lea    ebx, [ecx + 0fh]
        mov    word ptr [esi + 0eh], ax
        lea    edx, [ecx + 4]
        mov    word ptr [esi + 16h], bx
        lea    eax, [ecx + 0eh]
        add    cx, 5
        mov    bx, 4
        mov    word ptr [esi + 12h], ax
        mov    word ptr [esi + 14h], ax
        mov    word ptr [esi + 18h], dx
        mov    word ptr [esi + 1ah], cx
loc_1_44AB:
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        movzx  ax, bl
        add    ax, word ptr [edi]
        push   eax
        call   sub_1_9540
        add    esp, 8
        test   eax, eax
        jne    NEAR PTR loc_1_455A
        inc    bx
        cmp    bx, 7
        jbe    SHORT loc_1_44AB
        mov    bx, 0ah
loc_1_44D3:
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        movzx  ax, bl
        add    ax, word ptr [edi]
        push   eax
        call   sub_1_9540
        add    esp, 8
        test   eax, eax
        jne    SHORT loc_1_4561
        inc    bx
        cmp    bx, 0fh
        jbe    SHORT loc_1_44D3
        mov    cx, word ptr [edi]
        xor    bx, bx
        lea    eax, [ecx + 8]
        add    cx, 9
        mov    word ptr [esi + 24h], ax
        mov    word ptr [esi + 26h], ax
        mov    word ptr [esi + 28h], cx
loc_1_450C:
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        movzx  ax, bl
        add    ax, word ptr [edi]
        push   eax
        call   sub_1_9540
        add    esp, 8
        test   eax, eax
        jne    SHORT loc_1_4568
        inc    bx
        cmp    bx, 3
        jbe    SHORT loc_1_450C
        mov    bx, 8
loc_1_4530:
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        movzx  ax, bl
        add    ax, word ptr [edi]
        push   eax
        call   sub_1_9540
        add    esp, 8
        test   eax, eax
        jne    SHORT loc_1_456F
        inc    bx
        cmp    bx, 9
        jbe    SHORT loc_1_4530
        xor    eax, eax
        or     byte ptr [esi], 2
loc_1_4555:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
loc_1_455A:
        mov    eax, 1
        jmp    SHORT loc_1_4555
loc_1_4561:
        mov    eax, 1
        jmp    SHORT loc_1_4555
loc_1_4568:
        mov    eax, 1
        jmp    SHORT loc_1_4555
loc_1_456F:
        mov    eax, 1
        jmp    SHORT loc_1_4555
sub_1_4576:
        mov    ax, word ptr [esp + 8]
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 0ch]
        xor    bx, bx
        mov    word ptr [esi + 6], ax
        mov    word ptr [esi + 1ch], ax
        mov    word ptr [esi + 1eh], ax
        inc    ax
        mov    word ptr [esi + 20h], ax
        mov    word ptr [esi + 22h], ax
loc_1_459A:
        mov    eax, dword ptr [esp + 14h]
        push   eax
        movzx  ax, bl
        add    ax, word ptr [esi + 6]
        push   eax
        call   sub_1_9540
        add    esp, 8
        test   eax, eax
        jne    SHORT loc_1_45C4
        inc    bx
        cmp    bx, 1
        jbe    SHORT loc_1_459A
        xor    eax, eax
        or     byte ptr [esi], 4
loc_1_45C1:
        pop    esi
        pop    ebx
        ret
loc_1_45C4:
        mov    eax, 1
        jmp    SHORT loc_1_45C1
sub_1_45CB:
        mov    ax, word ptr [esp + 8]
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 0ch]
        mov    word ptr [esi + 8], ax
        cmp    word ptr [esi + 4], ax
        je     SHORT loc_1_4605
        xor    bx, bx
loc_1_45E3:
        mov    eax, dword ptr [esp + 14h]
        push   eax
        movzx  ax, bl
        add    ax, word ptr [esi + 8]
        push   eax
        call   sub_1_9540
        add    esp, 8
        test   eax, eax
        jne    SHORT loc_1_460D
        inc    bx
        cmp    bx, 3
        jbe    SHORT loc_1_45E3
loc_1_4605:
        xor    eax, eax
        or     byte ptr [esi], 8
loc_1_460A:
        pop    esi
        pop    ebx
        ret
loc_1_460D:
        mov    eax, 1
        jmp    SHORT loc_1_460A
sub_1_4614:
        push   ebp
        mov    eax, dword ptr [esp + 10h]
        mov    ebp, esp
        cmp    eax, 1
        push   ebx
        push   esi
        push   edi
        je     SHORT loc_1_462F
        cmp    eax, 2
        je     SHORT loc_1_4636
        mov    eax, 1
        jmp    SHORT loc_1_46A6
loc_1_462F:
        db  0BFh, 000h, 000h, 000h, 000h
        jmp    SHORT loc_1_463B
loc_1_4636:
        db  0BFh, 000h, 000h, 000h, 000h
loc_1_463B:
        mov    bx, 4
        mov    esi, dword ptr [ebp + 8]
loc_1_4642:
        movzx  ax, bl
        add    ax, word ptr [esi + 4]
        inc    bx
        push   eax
        push   dword ptr [ebp + 0ch]
        call   edi
        add    esp, 8
        cmp    bx, 7
        jbe    SHORT loc_1_4642
        mov    bx, 0ah
loc_1_465F:
        movzx  ax, bl
        add    ax, word ptr [esi + 4]
        inc    bx
        push   eax
        push   dword ptr [ebp + 0ch]
        call   edi
        add    esp, 8
        cmp    bx, 0fh
        jbe    SHORT loc_1_465F
        db  066h, 083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     SHORT loc_1_4688
        cmp    dword ptr [ebp + 10h], 1
        jne    SHORT loc_1_46A4
loc_1_4688:
        xor    bx, bx
loc_1_468B:
        movzx  ax, bl
        add    ax, word ptr [esi + 6]
        inc    bx
        push   eax
        push   dword ptr [ebp + 0ch]
        call   edi
        add    esp, 8
        cmp    bx, 1
        jbe    SHORT loc_1_468B
loc_1_46A4:
        xor    eax, eax
loc_1_46A6:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
sub_1_46AB:
        mov    eax, dword ptr [esp + 0ch]
        push   ebx
        push   esi
        cmp    eax, 1
        push   edi
        push   ebp
        je     SHORT loc_1_46C4
        cmp    eax, 2
        je     SHORT loc_1_46CB
        mov    eax, 1
        jmp    SHORT loc_1_4729
loc_1_46C4:
        db  0BEh, 000h, 000h, 000h, 000h
        jmp    SHORT loc_1_46D0
loc_1_46CB:
        db  0BEh, 000h, 000h, 000h, 000h
loc_1_46D0:
        xor    bx, bx
        mov    edi, dword ptr [esp + 14h]
        mov    ebp, dword ptr [esp + 18h]
loc_1_46DB:
        movzx  ax, bl
        add    ax, word ptr [edi + 4]
        inc    bx
        push   eax
        push   ebp
        call   esi
        add    esp, 8
        cmp    bx, 3
        jbe    SHORT loc_1_46DB
        mov    bx, 8
loc_1_46F6:
        movzx  ax, bl
        add    ax, word ptr [edi + 4]
        inc    bx
        push   eax
        push   ebp
        call   esi
        add    esp, 8
        cmp    bx, 9
        jbe    SHORT loc_1_46F6
        xor    bx, bx
loc_1_4710:
        movzx  ax, bl
        add    ax, word ptr [edi + 8]
        inc    bx
        push   eax
        push   ebp
        call   esi
        add    esp, 8
        cmp    bx, 3
        jbe    SHORT loc_1_4710
        xor    eax, eax
loc_1_4729:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_1_472E:
        push   ebx
        push   esi
        mov    bx, 4
        push   edi
        mov    esi, dword ptr [esp + 10h]
        lea    edi, [esi + 4]
        and    byte ptr [esi], 0feh
loc_1_473F:
        movzx  ax, bl
        add    ax, word ptr [edi]
        inc    bx
        push   eax
        call   sub_1_9564
        add    esp, 4
        cmp    bx, 7
        jbe    SHORT loc_1_473F
        mov    bx, 0ah
loc_1_475B:
        movzx  ax, bl
        add    ax, word ptr [edi]
        inc    bx
        push   eax
        call   sub_1_9564
        add    esp, 4
        cmp    bx, 0fh
        jbe    SHORT loc_1_475B
        xor    bx, bx
loc_1_4776:
        movzx  ax, bl
        add    ax, word ptr [edi]
        inc    bx
        push   eax
        call   sub_1_9564
        add    esp, 4
        cmp    bx, 3
        jbe    SHORT loc_1_4776
        mov    bx, 8
loc_1_4792:
        movzx  ax, bl
        add    ax, word ptr [edi]
        inc    bx
        push   eax
        call   sub_1_9564
        add    esp, 4
        cmp    bx, 9
        jbe    SHORT loc_1_4792
        xor    eax, eax
        pop    edi
        and    byte ptr [esi], 0fdh
        pop    esi
        pop    ebx
        ret
sub_1_47B3:
        push   ebx
        push   esi
        xor    bx, bx
        mov    esi, dword ptr [esp + 0ch]
loc_1_47BC:
        movzx  ax, bl
        add    ax, word ptr [esi + 6]
        inc    bx
        push   eax
        call   sub_1_9564
        add    esp, 4
        cmp    bx, 1
        jbe    SHORT loc_1_47BC
        xor    eax, eax
        and    byte ptr [esi], 0fbh
        pop    esi
        pop    ebx
        ret
sub_1_47DD:
        push   ebx
        push   esi
        xor    bx, bx
        mov    esi, dword ptr [esp + 0ch]
loc_1_47E6:
        movzx  ax, bl
        add    ax, word ptr [esi + 8]
        inc    bx
        push   eax
        call   sub_1_9564
        add    esp, 4
        cmp    bx, 3
        jbe    SHORT loc_1_47E6
        xor    eax, eax
        and    byte ptr [esi], 0f7h
        pop    esi
        pop    ebx
        ret
sub_1_4807:
        mov    eax, dword ptr [esp + 4]
        test   byte ptr [eax], 1
        je     SHORT loc_1_481F
        mov    cx, word ptr [eax + 4]
        mov    eax, dword ptr [esp + 8]
        mov    word ptr [eax], cx
        xor    eax, eax
        jmp    SHORT loc_1_4824
loc_1_481F:
        mov    eax, 1
loc_1_4824:
        ret
sub_1_4825:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   82h
        mov    esi, dword ptr [ebp + 8]
        mov    ax, word ptr [esi + 18h]
        push   eax
        call   sub_1_9CF5
        mov    cx, word ptr [esi + 1ah]
        add    esp, 8
        lea    eax, [ebp - 1]
        push   eax
        push   ecx
        call   sub_1_9CE4
        add    esp, 8
        mov    al, byte ptr [ebp - 1]
        shr    al, 4
        mov    edx, dword ptr [ebp + 0ch]
        movzx  cx, al
        mov    word ptr [edx], cx
        xor    eax, eax
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
loc_1_4869:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 310h
        test   eax, eax
        push   ebx
        push   esi
        push   edi
        je     SHORT loc_1_48AB
        cmp    eax, 1
        je     NEAR PTR loc_1_4A1F
        cmp    eax, 2
        je     NEAR PTR loc_1_4ECD
        cmp    eax, 3
        je     NEAR PTR loc_1_4F27
        cmp    eax, 4
        je     NEAR PTR loc_1_4ECD
        mov    eax, 1
        jmp    NEAR PTR loc_1_4F4B
loc_1_48AB:
        xor    edi, edi
        lea    eax, [ebp - 54h]
        push   edi
        lea    ecx, [ebp - 1ch]
        push   edi
        mov    esi, dword ptr [ebp + 10h]
        push   eax
        mov    dword ptr [ebp - 54h], edi
        push   ecx
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_48C3:
        add    esp, 14h
        test   eax, eax
        je     SHORT loc_1_48CD
        mov    dword ptr [ebp - 1ch], edi
loc_1_48CD:
        cmp    dword ptr [ebp - 1ch], 10h
        je     NEAR PTR loc_1_4A18
        cmp    dword ptr [ebp - 1ch], 40h
        je     NEAR PTR loc_1_4A18
        push   3
        lea    eax, [ebp - 10h]
        push   esi
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_48ED:
        add    esp, 0ch
        test   eax, eax
        je     SHORT loc_1_48FB
        xor    eax, eax
        jmp    NEAR PTR loc_1_4F4B
loc_1_48FB:
        mov    eax, dword ptr [ebp - 10h]
        push   0
        lea    ecx, [ebp - 18h]
        lea    edx, [ebp - 0ch]
        push   ecx
        mov    dword ptr [ebp - 0ch], eax
        xor    di, di
        push   2
        push   eax
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_4916:
        add    esp, 14h
        test   eax, eax
        jne    SHORT loc_1_494D
loc_1_491D:
        mov    eax, dword ptr [ebp - 0ch]
        mov    ax, word ptr [eax + 4]
        mov    cx, ax
        and    ch, 0fh
        cmp    ch, 2
        jne    SHORT loc_1_4932
        mov    di, ax
loc_1_4932:
        push   0
        lea    eax, [ebp - 18h]
        push   eax
        lea    edx, [ebp - 0ch]
        push   2
        push   dword ptr [ebp - 0ch]
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_4946:
        add    esp, 14h
        test   eax, eax
        je     SHORT loc_1_491D
loc_1_494D:
        cmp    eax, 0fh
        je     SHORT loc_1_4959
        xor    eax, eax
        jmp    NEAR PTR loc_1_4F4B
loc_1_4959:
        test   di, di
        jne    SHORT loc_1_4965
        xor    eax, eax
        jmp    NEAR PTR loc_1_4F4B
loc_1_4965:
        push   edi
        call   sub_1_3B3A
        add    esp, 4
        cmp    al, 5
        je     SHORT loc_1_4979
        xor    eax, eax
        jmp    NEAR PTR loc_1_4F4B
loc_1_4979:
        push   1
        lea    eax, [ebp - 10h]
        push   esi
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_4985:
        add    esp, 0ch
        test   eax, eax
        je     SHORT loc_1_4993
        xor    eax, eax
        jmp    NEAR PTR loc_1_4F4B
loc_1_4993:
        mov    eax, dword ptr [ebp - 10h]
        push   0
        lea    ecx, [ebp - 18h]
        lea    edx, [ebp - 0ch]
        push   ecx
        mov    dword ptr [ebp - 0ch], eax
        push   3
        xor    edi, edi
        push   eax
        xor    ebx, ebx
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_49AF:
        add    esp, 14h
        test   eax, eax
        jne    SHORT loc_1_49E3
loc_1_49B6:
        mov    ecx, dword ptr [ebp - 0ch]
        mov    al, byte ptr [ecx + 2]
        test   al, 0fh
        je     SHORT loc_1_49C2
        mov    edi, ecx
loc_1_49C2:
        test   al, 0f0h
        je     SHORT loc_1_49C8
        mov    ebx, ecx
loc_1_49C8:
        push   0
        lea    eax, [ebp - 18h]
        push   eax
        lea    edx, [ebp - 0ch]
        push   3
        push   dword ptr [ebp - 0ch]
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_49DC:
        add    esp, 14h
        test   eax, eax
        je     SHORT loc_1_49B6
loc_1_49E3:
        cmp    eax, 0fh
        jne    NEAR PTR loc_1_4F4B
        test   edi, edi
        je     SHORT loc_1_49F4
        and    byte ptr [edi + 2], 0feh
loc_1_49F4:
        test   ebx, ebx
        je     SHORT loc_1_49FC
        and    byte ptr [ebx + 2], 0bfh
loc_1_49FC:
        push   0
        lea    ecx, [ebp - 10h]
        push   dword ptr [ebp - 10h]
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_4A0A:
        add    esp, 0ch
        test   eax, eax
        je     SHORT loc_1_4993
        xor    eax, eax
        jmp    NEAR PTR loc_1_4F4B
loc_1_4A18:
        xor    eax, eax
        jmp    NEAR PTR loc_1_4F4B
loc_1_4A1F:
        cmp    dword ptr [ebp + 0ch], 1
        jbe    SHORT loc_1_4A2F
        mov    eax, 1
        jmp    NEAR PTR loc_1_4F4B
loc_1_4A2F:
        xor    eax, eax
        mov    esi, dword ptr [ebp + 10h]
        mov    dword ptr [ebp - 50h], eax
        mov    dword ptr [ebp - 210h], eax
        push   eax
        lea    ecx, [ebp - 50h]
        push   eax
        mov    dword ptr [ebp - 20ch], eax
        push   ecx
        mov    byte ptr [ebp - 204h], al
        lea    eax, [ebp - 24h]
        mov    dword ptr [ebp - 208h], esi
        push   eax
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_4A5F:
        add    esp, 14h
        test   eax, eax
        je     SHORT loc_1_4A70
        mov    eax, 13h
        jmp    NEAR PTR loc_1_4F4B
loc_1_4A70:
        mov    eax, dword ptr [ebp - 24h]
        cmp    eax, 10h
        je     SHORT loc_1_4A86
        cmp    eax, 40h
        je     SHORT loc_1_4A86
        and    byte ptr [ebp - 210h], 0fdh
        jmp    SHORT loc_1_4A8D
loc_1_4A86:
        or     byte ptr [ebp - 210h], 2
loc_1_4A8D:
        push   0
        lea    eax, [ebp - 12ch]
        push   esi
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_4A9C:
        add    esp, 0ch
        test   eax, eax
        jne    NEAR PTR loc_1_4F4B
        push   0
        lea    eax, [ebp - 204h]
        push   0c8h
        push   eax
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_4ABB:
        add    esp, 10h
        test   eax, eax
        jne    NEAR PTR loc_1_4F4B
        xor    dx, dx
        cmp    word ptr [ebp - 0d0h], dx
        jbe    NEAR PTR loc_1_4B97
loc_1_4AD6:
        movzx  eax, dx
        xor    ecx, ecx
        lea    edi, [eax*2]
        mov    ax, word ptr [ebp + edi - 0ceh]
        mov    cx, ax
        and    ecx, 0f00h
        cmp    ecx, 100h
        je     SHORT loc_1_4B15
        cmp    ecx, 200h
        je     SHORT loc_1_4B15
        cmp    ecx, 300h
        je     SHORT loc_1_4B49
        mov    eax, 13h
        jmp    NEAR PTR loc_1_4F4B
loc_1_4B15:
        cmp    word ptr [ebp + edi - 0a6h], 10h
        jb     NEAR PTR loc_1_4BBB
        or     byte ptr [ebp - 20ch], 1
        mov    word ptr [ebp - 13bh], ax
        or     byte ptr [ebp - 20ch], 80h
        mov    word ptr [ebp - 137h], ax
        or     byte ptr [ebp - 20ch], 4
        jmp    SHORT loc_1_4B88
loc_1_4B49:
        mov    cx, ax
        and    cl, 0f0h
        cmp    cl, 70h
        jbe    SHORT loc_1_4B6F
        cmp    word ptr [ebp + edi - 0a6h], 2
        jb     SHORT loc_1_4B88
        or     byte ptr [ebp - 20ch], 4
        mov    word ptr [ebp - 137h], ax
        jmp    SHORT loc_1_4B88
loc_1_4B6F:
        cmp    word ptr [ebp + edi - 0a6h], 2
        jb     SHORT loc_1_4B88
        or     byte ptr [ebp - 20ch], 2
        mov    word ptr [ebp - 139h], ax
loc_1_4B88:
        inc    dx
        cmp    dx, word ptr [ebp - 0d0h]
        jb     NEAR PTR loc_1_4AD6
loc_1_4B97:
        xor    ax, ax
        cmp    word ptr [ebp - 6eh], ax
        jbe    SHORT loc_1_4BDA
loc_1_4BA0:
        movzx  ecx, ax
        mov    cl, byte ptr [ebp + ecx - 6ch]
        cmp    cl, 4
        jae    SHORT loc_1_4BC5
        or     byte ptr [ebp - 20ch], 10h
        mov    byte ptr [ebp - 133h], cl
        jmp    SHORT loc_1_4BD2
loc_1_4BBB:
        mov    eax, 13h
        jmp    NEAR PTR loc_1_4F4B
loc_1_4BC5:
        or     byte ptr [ebp - 20ch], 20h
        mov    byte ptr [ebp - 132h], cl
loc_1_4BD2:
        inc    ax
        cmp    ax, word ptr [ebp - 6eh]
        jb     SHORT loc_1_4BA0
loc_1_4BDA:
        xor    ax, ax
        test   byte ptr [ebp - 20ch], 1
        je     SHORT loc_1_4BF8
        push   dword ptr [ebp - 13bh]
        call   sub_1_3B3A
        and    ax, 0ffh
        add    esp, 4
loc_1_4BF8:
        cmp    ax, 5
        jne    SHORT loc_1_4C35
        test   byte ptr [ebp - 210h], 2
        jne    SHORT loc_1_4C35
        cmp    byte ptr [ebp - 133h], 0
        je     NEAR PTR loc_1_4CCB
        cmp    byte ptr [ebp - 132h], 6
        je     NEAR PTR loc_1_4CCB
        push   dword ptr [ebp - 139h]
        push   dword ptr [ebp - 13bh]
        call   sub_1_3D08
        add    esp, 8
loc_1_4C35:
        cmp    word ptr [ebp - 7eh], 0
        jbe    NEAR PTR loc_1_4EC6
        cmp    byte ptr [ebp - 7ch], 2
        jne    SHORT loc_1_4C4A
        mov    byte ptr [ebp - 7ch], 9
loc_1_4C4A:
        mov    al, byte ptr [ebp - 7ch]
        push   1
        push   100h
        lea    ecx, [ebp - 310h]
        push   ecx
        mov    byte ptr [ebp - 131h], al
        or     byte ptr [ebp - 20ch], 40h
        push   0
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_4C70:
        add    esp, 14h
        test   eax, eax
        jne    SHORT loc_1_4CAE
        lea    eax, [ebp - 28h]
        lea    ecx, [ebp - 310h]
        push   eax
        push   ecx
        push   80000002h
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_4C8C:
        add    esp, 0ch
        test   eax, eax
        jne    SHORT loc_1_4CAE
        db  068h, 000h, 000h, 000h, 000h
        push   dword ptr [ebp - 28h]
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_4CA0:
        add    esp, 8
        push   dword ptr [ebp - 28h]
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_4CAB:
        add    esp, 4
loc_1_4CAE:
        lea    eax, [ebp - 210h]
        push   eax
        call   sub_1_34BD
        add    esp, 4
        test   eax, eax
        je     SHORT loc_1_4CD5
        mov    eax, 24h
        jmp    NEAR PTR loc_1_4F4B
loc_1_4CCB:
        mov    eax, 13h
        jmp    NEAR PTR loc_1_4F4B
loc_1_4CD5:
        push   dword ptr [ebp - 13bh]
        call   sub_1_39C0
        add    esp, 4
        mov    edi, eax
        test   edi, edi
        je     SHORT loc_1_4CF3
        mov    eax, 24h
        jmp    NEAR PTR loc_1_4F4B
loc_1_4CF3:
        mov    byte ptr [ebp - 2], 1
        test   byte ptr [ebp - 210h], 2
        mov    byte ptr [ebp - 1], 1
        jne    SHORT loc_1_4D08
        mov    byte ptr [ebp - 1], 0
loc_1_4D08:
        push   1
        lea    eax, [ebp - 2]
        push   2
        push   eax
        push   3
        db  068h, 000h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_4D22:
        add    esp, 1ch
        lea    ecx, [ebp - 14h]
        push   ecx
        call   sub_1_385B
        add    esp, 4
        test   eax, eax
        je     SHORT loc_1_4D3F
        mov    eax, 13h
        jmp    NEAR PTR loc_1_4F4B
loc_1_4D3F:
        call   sub_1_9C82
        push   dword ptr [ebp - 14h]
        call   sub_1_5368
        add    esp, 4
        lea    ecx, [ebp - 210h]
        push   dword ptr [ebp - 14h]
        push   ecx
        call   sub_1_5496
        add    esp, 8
        test   eax, eax
        je     SHORT loc_1_4D85
        push   dword ptr [ebp - 14h]
        call   sub_1_59AD
        add    esp, 4
        push   dword ptr [ebp - 14h]
        call   sub_1_38E3
        add    esp, 4
        mov    eax, 13h
        jmp    NEAR PTR loc_1_4F4B
loc_1_4D85:
        lea    eax, [ebp - 210h]
        push   eax
        call   sub_1_508A
        add    esp, 4
        mov    eax, dword ptr [ebp - 208h]
        mov    dword ptr [ebp - 4ch], 1
        db  0C7h, 045h, 0B8h, 000h, 000h, 000h, 000h
        mov    dword ptr [ebp - 44h], eax
        push   dword ptr [ebp - 4ch]
        push   dword ptr [ebp - 48h]
        push   dword ptr [ebp - 44h]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 001h, 000h, 04Ah, 004h
        add    esp, 0ch
        mov    eax, dword ptr [ebp - 20ch]
        mov    ecx, dword ptr [ebp - 208h]
        and    eax, 2
        mov    dword ptr [ebp - 38h], ecx
        db  0C7h, 045h, 0C4h, 000h, 000h, 000h, 000h
        shr    eax, 1
        mov    dword ptr [ebp - 40h], eax
        push   dword ptr [ebp - 40h]
        push   dword ptr [ebp - 3ch]
        push   dword ptr [ebp - 38h]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 001h, 000h, 04Ah, 004h
        add    esp, 0ch
        test   byte ptr [ebp - 210h], 2
        jne    SHORT loc_1_4E27
        lea    eax, [ebp - 210h]
        push   eax
        call   sub_1_3750
        add    esp, 4
        mov    edi, eax
        test   edi, edi
        jne    SHORT loc_1_4E27
        push   1
        db  068h, 000h, 000h, 000h, 000h
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_4E24:
        add    esp, 0ch
loc_1_4E27:
        mov    word ptr [ebp - 4], 1
        mov    dword ptr [ebp - 34h], 4
        test   byte ptr [ebp - 210h], 2
        mov    word ptr [ebp - 6], 0
        jne    SHORT loc_1_4E70
        mov    word ptr [ebp - 4], 1
        test   edi, edi
        je     SHORT loc_1_4E53
        mov    word ptr [ebp - 4], 0
loc_1_4E53:
        push   1
        lea    eax, [ebp - 4]
        push   2
        push   eax
        push   3
        db  068h, 000h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_4E6D:
        add    esp, 1ch
loc_1_4E70:
        push   0
        lea    eax, [ebp - 30h]
        push   eax
        lea    ecx, [ebp - 34h]
        push   ecx
        lea    edx, [ebp - 24h]
        push   edx
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_4E84:
        add    esp, 14h
        test   eax, eax
        jne    SHORT loc_1_4EB1
        movzx  ax, byte ptr [ebp - 30h]
        mov    word ptr [ebp - 6], ax
        push   1
        push   2
        lea    eax, [ebp - 6]
        push   eax
        push   3
        db  068h, 000h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_4EAE:
        add    esp, 1ch
loc_1_4EB1:
        push   dword ptr [ebp - 208h]
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_4EBC:
        add    esp, 4
        xor    eax, eax
        jmp    NEAR PTR loc_1_4F4B
loc_1_4EC6:
        mov    eax, 13h
        jmp    SHORT loc_1_4F4B
loc_1_4ECD:
        mov    esi, dword ptr [ebp + 10h]
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_4ED6:
        add    esp, 4
        lea    eax, [ebp - 8]
        lea    ecx, [ebp - 20h]
        push   eax
        push   ecx
        push   esi
        push   4
        call   sub_1_2E0C
        add    esp, 10h
        test   eax, eax
        je     SHORT loc_1_4EF4
        xor    eax, eax
        jmp    SHORT loc_1_4F4B
loc_1_4EF4:
        push   dword ptr [ebp - 20h]
        call   sub_1_59AD
        add    esp, 4
        push   dword ptr [ebp - 20h]
        call   sub_1_38E3
        db  0C7h, 045h, 0D4h, 000h, 000h, 000h, 000h
        add    esp, 4
        push   dword ptr [ebp - 2ch]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 004h, 000h, 04Ah, 004h
        add    esp, 4
        xor    eax, eax
        jmp    SHORT loc_1_4F4B
loc_1_4F27:
        cmp    dword ptr [ebp + 0ch], 1
        mov    eax, 1
        ja     SHORT loc_1_4F4B
        push   dword ptr [ebp + 10h]
        call   sub_1_380B
        add    esp, 4
        cmp    eax, 1
        mov    eax, 0
        adc    eax, -1
        and    eax, 13h
loc_1_4F4B:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_1_4F52:
        push   ebp
        mov    ebp, esp
        sub    esp, 2ch
        cmp    dword ptr [ebp + 8], 5
        push   ebx
        push   esi
        push   edi
        je     SHORT loc_1_4F6B
        mov    eax, 1
        jmp    NEAR PTR loc_1_5083
loc_1_4F6B:
        push   0
        mov    esi, dword ptr [ebp + 10h]
        push   esi
        lea    eax, [ebp - 8]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_4F7A:
        add    esp, 0ch
        test   eax, eax
        jne    SHORT loc_1_4F8B
        mov    eax, 1
        jmp    NEAR PTR loc_1_5083
loc_1_4F8B:
        lea    eax, [ebp - 4]
        lea    ecx, [ebp - 28h]
        push   eax
        push   ecx
        push   esi
        push   4
        call   sub_1_2E0C
        add    esp, 10h
        test   eax, eax
        je     SHORT loc_1_4FAC
        mov    eax, 13h
        jmp    NEAR PTR loc_1_5083
loc_1_4FAC:
        lea    eax, [ebp - 2]
        push   eax
        push   dword ptr [ebp - 28h]
        call   sub_1_3999
        add    esp, 8
        test   eax, eax
        je     SHORT loc_1_4FC9
        mov    eax, 13h
        jmp    NEAR PTR loc_1_5083
loc_1_4FC9:
        push   0
        lea    eax, [ebp - 8]
        push   esi
        db  068h, 000h, 000h, 000h, 000h
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_4FDA:
        add    esp, 10h
        test   eax, eax
        jne    NEAR PTR loc_1_5083
        push   0
        lea    ecx, [ebp - 24h]
        push   3000h
        push   dword ptr [ebp - 8]
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_4FF8:
        add    esp, 10h
        test   eax, eax
        jne    NEAR PTR loc_1_5083
        xor    di, di
        xor    ebx, ebx
        mov    si, 400h
loc_1_500C:
        mov    ax, word ptr [ebp - 2]
        mov    byte ptr [ebp - 16h], bl
        add    ax, si
        mov    byte ptr [ebp - 15h], bl
        mov    byte ptr [ebp - 0ah], 0ffh
        mov    byte ptr [ebp - 9], 0fh
        mov    word ptr [ebp - 1ch], bx
        push   ebx
        mov    word ptr [ebp - 1ah], bx
        push   18h
        mov    word ptr [ebp - 18h], bx
        lea    ecx, [ebp - 20h]
        mov    word ptr [ebp - 10h], ax
        push   ecx
        add    ax, 3
        push   2
        mov    word ptr [ebp - 20h], 1
        push   dword ptr [ebp - 24h]
        mov    word ptr [ebp - 1eh], 0ch
        mov    word ptr [ebp - 14h], 0ffffh
        mov    word ptr [ebp - 12h], 4
        mov    word ptr [ebp - 0eh], ax
        lea    eax, [ebp - 2ch]
        mov    word ptr [ebp - 0ch], bx
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_506A:
        add    esp, 18h
        test   eax, eax
        jne    SHORT loc_1_5083
        movzx  esi, si
        add    si, 400h
        inc    di
        cmp    di, 3
        jb     SHORT loc_1_500C
        xor    eax, eax
loc_1_5083:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_1_508A:
        push   ebp
        mov    edx, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 94h
        mov    eax, dword ptr [edx + 4]
        push   ebx
        test   al, 20h
        je     SHORT loc_1_50A7
        mov    bl, byte ptr [edx + 0deh]
        jmp    SHORT loc_1_50AD
loc_1_50A7:
        mov    bl, byte ptr [edx + 0ddh]
loc_1_50AD:
        mov    cl, byte ptr [edx + 0dfh]
        cmp    cl, 9
        jne    SHORT loc_1_50BF
        mov    ecx, 2
        jmp    SHORT loc_1_50C2
loc_1_50BF:
        movzx  ecx, cl
loc_1_50C2:
        test   al, 2
        je     SHORT loc_1_50F6
        movzx  eax, word ptr [edx + 0d7h]
        movzx  ebx, bl
        push   eax
        push   ebx
        movzx  eax, byte ptr [edx + 0ddh]
        movzx  edx, word ptr [edx + 0d5h]
        push   eax
        push   ecx
        push   edx
        lea    eax, [ebp - 54h]
        db  068h, 000h, 000h, 000h, 000h
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_50F1:
        add    esp, 1ch
        jmp    SHORT loc_1_511C
loc_1_50F6:
        movzx  eax, bl
        movzx  ebx, byte ptr [edx + 0ddh]
        push   eax
        push   ebx
        movzx  eax, word ptr [edx + 0d5h]
        push   ecx
        lea    edx, [ebp - 54h]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_5119:
        add    esp, 18h
loc_1_511C:
        mov    dword ptr [ebp - 14h], 40h
        db  0C7h, 045h, 0F4h, 000h, 000h, 000h, 000h
        lea    eax, [ebp - 94h]
        mov    dword ptr [ebp - 10h], eax
        push   dword ptr [ebp - 14h]
        push   dword ptr [ebp - 10h]
        push   dword ptr [ebp - 0ch]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 003h, 000h, 04Ah, 004h
        add    esp, 0ch
        lea    eax, [ebp - 94h]
        push   eax
        push   45h
        call   sub_1_51D7
        add    esp, 8
        test   eax, eax
        jne    SHORT loc_1_51B0
        xor    eax, eax
        xor    ecx, ecx
        cmp    byte ptr [ebp - 54h], al
        je     SHORT loc_1_5176
loc_1_5169:
        cmp    eax, 3fh
        jge    SHORT loc_1_51B0
        inc    eax
        cmp    byte ptr [ebp + eax - 54h], 0
        jne    SHORT loc_1_5169
loc_1_5176:
        cmp    eax, 3fh
        jge    SHORT loc_1_51B0
        mov    byte ptr [ebp + eax - 54h], 20h
        inc    eax
        cmp    byte ptr [ebp - 94h], 0
        je     SHORT loc_1_51AB
loc_1_518A:
        mov    dl, byte ptr [ebp + ecx - 94h]
        cmp    dl, 20h
        je     SHORT loc_1_51AB
        cmp    eax, 3fh
        jge    SHORT loc_1_51AB
        mov    byte ptr [ebp + eax - 54h], dl
        inc    ecx
        inc    eax
        cmp    byte ptr [ebp + ecx - 94h], 0
        jne    SHORT loc_1_518A
loc_1_51AB:
        mov    byte ptr [ebp + eax - 54h], 0
loc_1_51B0:
        db  0C7h, 045h, 0FCh, 000h, 000h, 000h, 000h
        lea    eax, [ebp - 54h]
        mov    dword ptr [ebp - 8], eax
        push   dword ptr [ebp - 8]
        push   dword ptr [ebp - 4]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 002h, 000h, 04Ah, 004h
        add    esp, 8
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_1_51D7:
        mov    ecx, dword ptr [esp + 8]
        push   esi
        push   edi
        mov    al, byte ptr [ecx]
        test   al, al
        je     SHORT loc_1_5227
        xor    esi, esi
        cmp    byte ptr [ecx + esi], 0
        je     SHORT loc_1_51F8
loc_1_51EB:
        cmp    byte ptr [ecx + esi], 20h
        jne    SHORT loc_1_51F8
        inc    esi
        cmp    byte ptr [ecx + esi], 0
        jne    SHORT loc_1_51EB
loc_1_51F8:
        mov    dl, byte ptr [ecx + esi]
        cmp    byte ptr [esp + 0ch], dl
        je     SHORT loc_1_522F
        movsx  eax, dl
        movsx  edi, byte ptr [esp + 0ch]
        sub    eax, edi
        cmp    eax, 20h
        je     SHORT loc_1_522F
        test   dl, dl
        je     SHORT loc_1_5221
loc_1_5214:
        cmp    byte ptr [ecx + esi], 20h
        je     SHORT loc_1_5221
        inc    esi
        cmp    byte ptr [ecx + esi], 0
        jne    SHORT loc_1_5214
loc_1_5221:
        cmp    byte ptr [ecx + esi], 0
        jne    SHORT loc_1_51EB
loc_1_5227:
        mov    eax, 1
loc_1_522C:
        pop    edi
        pop    esi
        ret
loc_1_522F:
        mov    edi, esi
        mov    dl, byte ptr [ecx + esi]
        test   dl, dl
        je     SHORT loc_1_5245
loc_1_5238:
        cmp    byte ptr [ecx + edi], 20h
        je     SHORT loc_1_5245
        inc    edi
        cmp    byte ptr [ecx + edi], 0
        jne    SHORT loc_1_5238
loc_1_5245:
        xor    eax, eax
        test   dl, dl
        je     SHORT loc_1_525D
loc_1_524B:
        cmp    esi, edi
        je     SHORT loc_1_525D
        mov    dl, byte ptr [ecx + esi]
        inc    esi
        mov    byte ptr [ecx + eax], dl
        inc    eax
        cmp    byte ptr [ecx + esi], 0
        jne    SHORT loc_1_524B
loc_1_525D:
        mov    byte ptr [ecx + eax], 0
        xor    eax, eax
        jmp    SHORT loc_1_522C
sub_1_5265:
        mov    eax, dword ptr [esp + 4]
        mov    dword ptr [eax], 0
        xor    eax, eax
        ret
sub_1_5272:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 0ch]
        cmp    ebx, 40h
        jbe    SHORT loc_1_528D
        mov    eax, 1
        jmp    NEAR PTR loc_1_5334
loc_1_528D:
        add    ebx, 3
        mov    esi, dword ptr [ebp + 8]
        shr    ebx, 2
        test   ebx, ebx
        je     SHORT loc_1_52EB
        lea    edi, [esi + 8]
loc_1_529D:
        mov    word ptr [ebp - 4], bx
        push   ax
        push   cx
        mov    ax, word ptr [ebp - 4]
        dec    ax
        bsr    cx, ax
        inc    cl
        mov    ax, 1
        shl    ax, cl
        dec    ax
        and    ax, 1fh
        mov    word ptr [ebp - 2], ax
        pop    cx
        pop    ax
        movzx  ecx, word ptr [ebp - 2]
        lea    eax, [esi + 14h]
        push   eax
        push   edi
        lea    eax, [esi + 0ch]
        push   eax
        lea    eax, [esi + 10h]
        push   eax
        push   ebx
        push   ecx
        call   sub_1_9440
        add    esp, 18h
        test   eax, eax
        je     SHORT loc_1_52EB
        shr    ebx, 1
        test   ebx, ebx
        jne    SHORT loc_1_529D
loc_1_52EB:
        test   ebx, ebx
        jne    SHORT loc_1_52F6
        mov    eax, 1
        jmp    SHORT loc_1_5334
loc_1_52F6:
        lea    eax, [ebp - 0ch]
        lea    ecx, [ebp - 8]
        push   eax
        push   ecx
        push   dword ptr [esi + 8]
        push   dword ptr [esi + 14h]
        call   sub_1_94B8
        add    esp, 10h
        lea    ecx, [esi + 4]
        push   ecx
        push   dword ptr [ebp - 0ch]
        push   dword ptr [ebp - 8]
        call   sub_1_93F9
        add    esp, 0ch
        test   eax, eax
        je     SHORT loc_1_5329
        mov    eax, 1
        jmp    SHORT loc_1_5334
loc_1_5329:
        xor    eax, eax
        or     byte ptr [esi], 1
        mov    word ptr [esi + 6], 0
loc_1_5334:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_1_533B:
        push   esi
        mov    esi, dword ptr [esp + 8]
        test   byte ptr [esi], 1
        jne    SHORT loc_1_534C
        mov    eax, 3
        jmp    SHORT loc_1_5366
loc_1_534C:
        mov    ax, word ptr [esi + 4]
        push   eax
        call   sub_1_942B
        add    esp, 4
        push   dword ptr [esi + 10h]
        call   sub_1_94A4
        add    esp, 4
        xor    eax, eax
loc_1_5366:
        pop    esi
        ret
sub_1_5368:
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 0ch]
        push   edi
        xor    edi, edi
        lea    eax, [esi + 67dh]
        push   eax
        mov    dword ptr [esi + 0d4h], edi
        call   sub_1_443E
        add    esp, 4
        lea    eax, [esi + 6a7h]
        push   eax
        call   sub_1_216C
        add    esp, 4
loc_1_5395:
        movzx  eax, di
        inc    di
        imul   eax, eax, 0ch
        add    eax, esi
        add    eax, 6b3h
        push   eax
        call   sub_1_2261
        add    esp, 4
        cmp    di, 2
        jb     SHORT loc_1_5395
        xor    di, di
        lea    eax, [esi + 6fbh]
        push   eax
        mov    ebx, 0ech
        call   sub_1_5265
        add    esp, 4
loc_1_53CA:
        movzx  eax, di
        imul   eax, ebx
        inc    di
        add    eax, esi
        add    eax, ebx
        push   eax
        call   sub_1_9EC9
        add    esp, 4
        cmp    di, 2
        jb     SHORT loc_1_53CA
        xor    di, di
loc_1_53E8:
        movzx  eax, di
        inc    di
        imul   eax, eax, 2ah
        add    eax, esi
        add    eax, 2c4h
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_53FD:
        add    esp, 4
        cmp    di, 1
        jb     SHORT loc_1_53E8
        xor    di, di
loc_1_5409:
        movzx  eax, di
        inc    di
        imul   eax, eax, 0e0h
        add    eax, esi
        add    eax, 2eeh
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_5421:
        add    esp, 4
        cmp    di, 1
        jb     SHORT loc_1_5409
        xor    di, di
loc_1_542D:
        movzx  eax, di
        inc    di
        imul   eax, eax, 5ah
        add    eax, esi
        add    eax, 3ceh
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_5442:
        add    esp, 4
        cmp    di, 1
        jb     SHORT loc_1_542D
        xor    di, di
loc_1_544E:
        movzx  eax, di
        inc    di
        imul   eax, eax, 229h
        add    eax, esi
        add    eax, 428h
        push   eax
        call   sub_1_639C
        add    esp, 4
        cmp    di, 1
        jb     SHORT loc_1_544E
        xor    di, di
loc_1_5472:
        movzx  eax, di
        inc    di
        imul   eax, eax, 2ch
        add    eax, esi
        add    eax, 651h
        push   eax
        call   sub_1_8EA8
        add    esp, 4
        cmp    di, 1
        jb     SHORT loc_1_5472
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_1_5496:
        sub    esp, 8
        mov    dword ptr [esp + 4], 1
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 14h]
        push   edi
        mov    edi, dword ptr [esp + 1ch]
        push   ebp
        mov    eax, dword ptr [esi + 8]
        lea    ecx, [esi + 0ch]
        push   ecx
        lea    edx, [edi + 0ch]
        push   edx
        mov    dword ptr [edi + 8], eax
        lea    ebp, [edi + 0d4h]
        call   sub_1_23D2
        add    esp, 8
        xor    eax, eax
        mov    dword ptr [edi + 0d8h], eax
        mov    dword ptr [edi + 0dch], eax
        mov    ecx, dword ptr [esi + 4]
        mov    dword ptr [ebp], ecx
        mov    dword ptr [edi + 0e0h], eax
        test   byte ptr [esi], 2
        je     SHORT loc_1_54ED
        or     byte ptr [edi], 20h
        jmp    SHORT loc_1_54F0
loc_1_54ED:
        and    byte ptr [edi], 0dfh
loc_1_54F0:
        xor    eax, eax
        mov    dword ptr [edi + 713h], eax
        mov    dword ptr [edi + 717h], eax
        mov    dword ptr [edi + 71bh], eax
        test   byte ptr [ebp], 1
        je     SHORT loc_1_5533
        mov    cx, word ptr [esi + 0d5h]
        lea    eax, [edi + 67dh]
        db  068h, 000h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        push   ecx
        push   eax
        call   sub_1_4457
        add    esp, 10h
        test   eax, eax
        je     SHORT loc_1_5533
        and    byte ptr [ebp], 0feh
loc_1_5533:
        test   byte ptr [ebp], 2
        je     SHORT loc_1_555D
        mov    cx, word ptr [esi + 0d7h]
        lea    eax, [edi + 67dh]
        db  068h, 000h, 000h, 000h, 000h
        push   ecx
        push   eax
        call   sub_1_4576
        add    esp, 0ch
        test   eax, eax
        je     SHORT loc_1_555D
        and    byte ptr [ebp], 0fdh
loc_1_555D:
        test   byte ptr [ebp], 4
        je     SHORT loc_1_5587
        mov    cx, word ptr [esi + 0d9h]
        lea    eax, [edi + 67dh]
        db  068h, 000h, 000h, 000h, 000h
        push   ecx
        push   eax
        call   sub_1_45CB
        add    esp, 0ch
        test   eax, eax
        je     SHORT loc_1_5587
        and    byte ptr [ebp], 0fbh
loc_1_5587:
        lea    ebx, [edi + 67dh]
        lea    eax, [edi + 7a5h]
        push   eax
        push   ebx
        call   sub_1_4825
        add    esp, 8
        test   byte ptr [ebp], 40h
        je     SHORT loc_1_55D1
        push   edi
        mov    al, byte ptr [esi + 0dfh]
        db  068h, 000h, 000h, 000h, 000h
        lea    ecx, [edi + 6a7h]
        db  068h, 000h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        push   eax
        push   ecx
        call   sub_1_2184
        add    esp, 18h
        test   eax, eax
        je     SHORT loc_1_55D1
        and    byte ptr [ebp], 0bfh
loc_1_55D1:
        test   byte ptr [ebp], 10h
        je     SHORT loc_1_5634
        db  068h, 000h, 000h, 000h, 000h
        mov    al, byte ptr [esi + 0ddh]
        push   eax
        lea    eax, [edi + 6b3h]
        push   eax
        call   sub_1_227A
        mov    dword ptr [esp + 1ch], eax
        add    esp, 0ch
        lea    eax, [edi + 6cbh]
        push   20h
        push   eax
        call   sub_1_5272
        add    esp, 8
        cmp    dword ptr [esp + 10h], 0
        jne    SHORT loc_1_5612
        test   eax, eax
        je     SHORT loc_1_5634
loc_1_5612:
        lea    eax, [edi + 6b3h]
        push   eax
        call   sub_1_237D
        add    esp, 4
        lea    eax, [edi + 6cbh]
        push   eax
        call   sub_1_533B
        add    esp, 4
        and    byte ptr [ebp], 0efh
loc_1_5634:
        test   byte ptr [ebp], 20h
        je     SHORT loc_1_5691
        db  068h, 000h, 000h, 000h, 000h
        mov    al, byte ptr [esi + 0deh]
        push   eax
        lea    esi, [edi + 6e3h]
        lea    eax, [edi + 6bfh]
        push   eax
        call   sub_1_227A
        mov    dword ptr [esp + 1ch], eax
        add    esp, 0ch
        push   20h
        push   esi
        call   sub_1_5272
        add    esp, 8
        cmp    dword ptr [esp + 10h], 0
        jne    SHORT loc_1_5675
        test   eax, eax
        je     SHORT loc_1_5691
loc_1_5675:
        lea    eax, [edi + 6bfh]
        push   eax
        call   sub_1_237D
        add    esp, 4
        push   esi
        call   sub_1_533B
        add    esp, 4
        and    byte ptr [ebp], 0dfh
loc_1_5691:
        mov    eax, dword ptr [ebp]
        test   al, 10h
        je     SHORT loc_1_56A4
        test   al, 20h
        je     SHORT loc_1_56A4
        mov    dword ptr [esp + 14h], 0
loc_1_56A4:
        test   al, 80h
        je     SHORT loc_1_56C8
        lea    eax, [edi + 6fbh]
        lea    ecx, [edi + 651h]
        push   eax
        push   ebx
        push   ecx
        push   edi
        call   sub_1_8EB5
        add    esp, 10h
        test   eax, eax
        je     SHORT loc_1_56C8
        and    byte ptr [ebp], 7fh
loc_1_56C8:
        mov    eax, dword ptr [ebp]
        test   al, 1
        je     SHORT loc_1_56FA
        test   al, 10h
        je     SHORT loc_1_56FA
        test   al, 40h
        je     SHORT loc_1_56FA
        lea    ecx, [edi + 6cbh]
        lea    edx, [edi + 6b3h]
        push   dword ptr [esp + 14h]
        push   ecx
        push   edx
        lea    eax, [edi + 0ech]
        push   ebx
        push   eax
        push   edi
        call   sub_1_9ED7
        add    esp, 18h
loc_1_56FA:
        mov    eax, dword ptr [ebp]
        test   al, 1
        je     SHORT loc_1_572C
        test   al, 20h
        je     SHORT loc_1_572C
        test   al, 40h
        je     SHORT loc_1_572C
        lea    ecx, [edi + 6e3h]
        lea    edx, [edi + 6bfh]
        push   dword ptr [esp + 14h]
        push   ecx
        push   edx
        lea    eax, [edi + 1d8h]
        push   ebx
        push   eax
        push   edi
        call   sub_1_9ED7
        add    esp, 18h
loc_1_572C:
        test   byte ptr [ebp], 1
        je     SHORT loc_1_5743
        push   ebx
        lea    eax, [edi + 2c4h]
        push   eax
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_5740:
        add    esp, 0ch
loc_1_5743:
        mov    eax, dword ptr [ebp]
        test   al, 1
        je     SHORT loc_1_5763
        test   al, 40h
        je     SHORT loc_1_5763
        test   al, 2
        je     SHORT loc_1_5763
        push   ebx
        lea    eax, [edi + 2eeh]
        push   eax
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_5760:
        add    esp, 0ch
loc_1_5763:
        test   byte ptr [ebp], 2
        je     SHORT loc_1_577A
        push   ebx
        lea    eax, [edi + 3ceh]
        push   eax
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_5777:
        add    esp, 0ch
loc_1_577A:
        test   byte ptr [ebp], 1
        je     SHORT loc_1_5791
        push   ebx
        lea    eax, [edi + 428h]
        push   eax
        push   edi
        call   sub_1_63B7
        add    esp, 0ch
loc_1_5791:
        lea    ebp, [edi + 681h]
        mov    ax, word ptr [ebp]
        push   eax
        call   sub_1_3B3A
        mov    word ptr [esp + 14h], ax
        add    esp, 4
        shr    ax, 8
        push   eax
        push   dword ptr [esp + 14h]
        call   sub_1_3D74
        lea    ecx, [esp + 18h]
        add    esp, 8
        db  0BEh, 000h, 000h, 000h, 000h
        mov    dword ptr [edi + 0e4h], eax
        push   1
        push   2
        push   ecx
        push   3
        db  068h, 000h, 000h, 000h, 000h
        push   esi
        push   dword ptr [edi + 8]
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_57DE:
        mov    ax, word ptr [ebp]
        add    esp, 1ch
        mov    ebp, 0ffffh
        push   eax
        call   sub_1_3DF4
        add    esp, 4
        mov    dword ptr [edi + 0e8h], eax
        push   ebp
        push   0e1h
        push   ebx
        call   sub_1_436F
        lea    eax, [esp + 1dh]
        add    esp, 0ch
        push   ebp
        push   eax
        push   ebx
        call   sub_1_43EF
        lea    ecx, [esp + 1ch]
        add    esp, 0ch
        push   ebp
        push   ecx
        push   ebx
        call   sub_1_43EF
        mov    cx, word ptr [esp + 1ch]
        add    esp, 0ch
        lea    eax, [edi + 79fh]
        push   1
        mov    word ptr [eax], cx
        push   2
        push   eax
        push   3
        db  068h, 000h, 000h, 000h, 000h
        push   esi
        push   dword ptr [edi + 8]
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_5849:
        add    esp, 1ch
        xor    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
sub_1_5856:
        push   ebp
        movzx  eax, word ptr [esp + 0ch]
        mov    ebp, esp
        cmp    eax, 1
        je     SHORT loc_1_589C
        cmp    eax, 2
        je     SHORT loc_1_58E2
        cmp    eax, 4
        je     NEAR PTR loc_1_58FF
        cmp    eax, 8
        je     NEAR PTR loc_1_5913
        cmp    eax, 10h
        je     NEAR PTR loc_1_5927
        cmp    eax, 20h
        je     NEAR PTR loc_1_593B
        cmp    eax, 80h
        je     NEAR PTR loc_1_5954
        jmp    NEAR PTR loc_1_5976
loc_1_589C:
        mov    ecx, dword ptr [ebp + 10h]
        mov    edx, dword ptr [ebp + 8]
        mov    word ptr [ecx], 0
        add    edx, 0d4h
        mov    eax, dword ptr [edx]
        test   al, 1
        je     SHORT loc_1_58C0
        test   al, 10h
        je     SHORT loc_1_58C0
        test   al, 40h
        je     SHORT loc_1_58C0
        mov    word ptr [ecx], 1
loc_1_58C0:
        mov    eax, dword ptr [edx]
        test   al, 1
        je     NEAR PTR loc_1_5976
        test   al, 20h
        je     NEAR PTR loc_1_5976
        test   al, 40h
        je     NEAR PTR loc_1_5976
        inc    word ptr [ecx]
        jmp    NEAR PTR loc_1_5976
loc_1_58E2:
        mov    ecx, dword ptr [ebp + 10h]
        mov    edx, dword ptr [ebp + 8]
        mov    word ptr [ecx], 0
        mov    eax, dword ptr [edx + 0d4h]
        test   al, 1
        je     SHORT loc_1_5976
        test   al, 40h
        je     SHORT loc_1_5976
        test   al, 2
        jmp    SHORT loc_1_596F
loc_1_58FF:
        mov    ecx, dword ptr [ebp + 10h]
        mov    edx, dword ptr [ebp + 8]
        mov    word ptr [ecx], 0
        test   byte ptr [edx + 0d4h], 2
        jmp    SHORT loc_1_596F
loc_1_5913:
        mov    ecx, dword ptr [ebp + 10h]
        mov    edx, dword ptr [ebp + 8]
        mov    word ptr [ecx], 0
        test   byte ptr [edx + 0d4h], 1
        jmp    SHORT loc_1_596F
loc_1_5927:
        mov    ecx, dword ptr [ebp + 10h]
        mov    edx, dword ptr [ebp + 8]
        mov    word ptr [ecx], 0
        test   byte ptr [edx + 0d4h], 1
        jmp    SHORT loc_1_596F
loc_1_593B:
        mov    ecx, dword ptr [ebp + 10h]
        mov    edx, dword ptr [ebp + 8]
        mov    word ptr [ecx], 0
        mov    eax, dword ptr [edx + 0d4h]
        test   al, 1
        je     SHORT loc_1_5976
        test   al, 8
        jmp    SHORT loc_1_596F
loc_1_5954:
        mov    ecx, dword ptr [ebp + 10h]
        mov    edx, dword ptr [ebp + 8]
        mov    word ptr [ecx], 0
        mov    eax, dword ptr [edx + 0d4h]
        test   al, 1
        je     SHORT loc_1_5976
        test   al, 10h
        je     SHORT loc_1_5976
        test   al, 40h
loc_1_596F:
        je     SHORT loc_1_5976
        mov    word ptr [ecx], 1
loc_1_5976:
        xor    eax, eax
        pop    ebp
        ret
sub_1_597A:
        push   esi
        push   edi
        xor    esi, esi
        mov    edi, dword ptr [esp + 0ch]
        add    edi, 0ech
loc_1_5988:
        push   edi
        call   sub_1_9DDD
        add    esp, 4
        test   eax, eax
        jne    SHORT loc_1_59A6
        add    edi, 0ech
        inc    esi
        cmp    esi, 2
        jl     SHORT loc_1_5988
        xor    eax, eax
loc_1_59A3:
        pop    edi
        pop    esi
        ret
loc_1_59A6:
        mov    eax, 1
        jmp    SHORT loc_1_59A3
sub_1_59AD:
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 0ch]
        push   edi
        lea    edi, [esi + 0d4h]
        test   byte ptr [edi], 1
        je     SHORT loc_1_59CE
        lea    eax, [esi + 67dh]
        push   eax
        call   sub_1_472E
        add    esp, 4
loc_1_59CE:
        test   byte ptr [edi], 2
        je     SHORT loc_1_59E2
        lea    eax, [esi + 67dh]
        push   eax
        call   sub_1_47B3
        add    esp, 4
loc_1_59E2:
        test   byte ptr [edi], 4
        je     SHORT loc_1_59F6
        lea    eax, [esi + 67dh]
        push   eax
        call   sub_1_47DD
        add    esp, 4
loc_1_59F6:
        xor    di, di
        lea    eax, [esi + 6a7h]
        push   eax
        call   sub_1_2238
        add    esp, 4
loc_1_5A08:
        movzx  eax, di
        inc    di
        imul   eax, eax, 0ch
        add    eax, esi
        add    eax, 6b3h
        push   eax
        call   sub_1_237D
        add    esp, 4
        cmp    di, 2
        jb     SHORT loc_1_5A08
        xor    di, di
loc_1_5A29:
        movzx  eax, di
        inc    di
        imul   eax, eax, 18h
        add    eax, esi
        add    eax, 6cbh
        push   eax
        call   sub_1_533B
        add    esp, 4
        cmp    di, 2
        jb     SHORT loc_1_5A29
        xor    di, di
        lea    eax, [esi + 6fbh]
        push   eax
        mov    ebx, 0ech
        call   sub_1_533B
        add    esp, 4
loc_1_5A5E:
        movzx  eax, di
        imul   eax, ebx
        inc    di
        add    eax, esi
        add    eax, ebx
        push   eax
        call   sub_1_A033
        add    esp, 4
        cmp    di, 2
        jb     SHORT loc_1_5A5E
        xor    di, di
loc_1_5A7C:
        movzx  eax, di
        inc    di
        imul   eax, eax, 2ah
        add    eax, esi
        add    eax, 2c4h
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_5A91:
        add    esp, 4
        cmp    di, 1
        jb     SHORT loc_1_5A7C
        xor    di, di
loc_1_5A9D:
        movzx  eax, di
        inc    di
        imul   eax, eax, 0e0h
        add    eax, esi
        add    eax, 2eeh
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_5AB5:
        add    esp, 4
        cmp    di, 1
        jb     SHORT loc_1_5A9D
        xor    di, di
loc_1_5AC1:
        movzx  eax, di
        inc    di
        imul   eax, eax, 5ah
        add    eax, esi
        add    eax, 3ceh
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_5AD6:
        add    esp, 4
        cmp    di, 1
        jb     SHORT loc_1_5AC1
        xor    di, di
loc_1_5AE2:
        movzx  eax, di
        inc    di
        imul   eax, eax, 229h
        add    eax, esi
        add    eax, 428h
        push   eax
        call   sub_1_660A
        add    esp, 4
        cmp    di, 1
        jb     SHORT loc_1_5AE2
        xor    di, di
loc_1_5B06:
        movzx  eax, di
        inc    di
        imul   eax, eax, 2ch
        add    eax, esi
        add    eax, 651h
        push   eax
        call   sub_1_912F
        add    esp, 4
        cmp    di, 1
        jb     SHORT loc_1_5B06
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_1_5B2A:
        xor    dx, dx
        mov    eax, dword ptr [esp + 4]
        mov    ecx, dword ptr [eax + 8]
loc_1_5B34:
        movzx  eax, dx
        imul   eax, eax, 0ech
        lea    eax, [eax + ecx + 0ech]
        test   byte ptr [eax + 4], 1
        je     SHORT loc_1_5B57
        mov    eax, dword ptr [eax + 8]
        cmp    eax, 1
        je     SHORT loc_1_5B62
        cmp    eax, 2
        je     SHORT loc_1_5B62
loc_1_5B57:
        inc    dx
        cmp    dx, 2
        jb     SHORT loc_1_5B34
        xor    eax, eax
loc_1_5B61:
        ret
loc_1_5B62:
        mov    eax, 1
        jmp    SHORT loc_1_5B61
sub_1_5B69:
        sub    esp, 4
        push   esi
        mov    esi, dword ptr [esp + 0ch]
        add    esi, 0e8h
        mov    eax, dword ptr [esi]
        mov    eax, dword ptr [eax + 651h]
        test   al, 1
        je     SHORT loc_1_5BB2
        test   al, 4
        je     SHORT loc_1_5BB2
        lea    eax, [esp + 4]
        push   eax
        call   sub_1_9B4D
        add    esp, 4
        mov    eax, dword ptr [esi]
        add    eax, 651h
        push   eax
        call   sub_1_8E1E
        add    esp, 4
        push   dword ptr [esp + 4]
        call   sub_1_9B5B
        add    esp, 4
        jmp    SHORT loc_1_5BB7
loc_1_5BB2:
        mov    eax, 1
loc_1_5BB7:
        pop    esi
        add    esp, 4
        ret
sub_1_5BBC:
        sub    esp, 4
        push   esi
        mov    esi, dword ptr [esp + 0ch]
        add    esi, 0e8h
        mov    eax, dword ptr [esi]
        mov    eax, dword ptr [eax + 651h]
        test   al, 1
        je     SHORT loc_1_5C05
        test   al, 4
        je     SHORT loc_1_5C05
        lea    eax, [esp + 4]
        push   eax
        call   sub_1_9B4D
        add    esp, 4
        mov    eax, dword ptr [esi]
        add    eax, 651h
        push   eax
        call   sub_1_8E3B
        add    esp, 4
        push   dword ptr [esp + 4]
        call   sub_1_9B5B
        add    esp, 4
        jmp    SHORT loc_1_5C0A
loc_1_5C05:
        mov    eax, 1
loc_1_5C0A:
        pop    esi
        add    esp, 4
        ret
sub_1_5C0F:
        sub    esp, 4
        push   esi
        mov    esi, dword ptr [esp + 0ch]
        add    esi, 0e8h
        mov    eax, dword ptr [esi]
        mov    eax, dword ptr [eax + 651h]
        test   al, 1
        je     SHORT loc_1_5C58
        test   al, 4
        je     SHORT loc_1_5C58
        lea    eax, [esp + 4]
        push   eax
        call   sub_1_9B4D
        add    esp, 4
        mov    eax, dword ptr [esi]
        add    eax, 651h
        push   eax
        call   sub_1_8E54
        add    esp, 4
        push   dword ptr [esp + 4]
        call   sub_1_9B5B
        add    esp, 4
        jmp    SHORT loc_1_5C5D
loc_1_5C58:
        mov    eax, 1
loc_1_5C5D:
        pop    esi
        add    esp, 4
        ret
sub_1_5C62:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 0ch], 0
        mov    edi, dword ptr [esi + 24h]
        add    edi, 2eeh
loc_1_5C7E:
        test   byte ptr [edi], 8
        je     SHORT loc_1_5CEA
        lea    eax, [ebp - 1]
        mov    ecx, dword ptr [esi + 8]
        mov    dx, word ptr [ecx + 20h]
        push   eax
        push   edx
        call   sub_1_9CE4
        add    esp, 8
        test   byte ptr [ebp - 1], 40h
        je     NEAR PTR loc_1_5D7F
        test   byte ptr [ebp - 1], 80h
        jne    NEAR PTR loc_1_5D63
        lea    eax, [ebp - 2]
        mov    ecx, dword ptr [edi + 8]
        mov    dx, word ptr [ecx + 1eh]
        push   eax
        push   edx
        call   sub_1_9CE4
        add    esp, 8
        lea    edx, [edi + 0d8h]
        mov    ax, word ptr [edx]
        cmp    ax, 40h
        jae    NEAR PTR loc_1_5D63
        movzx  ebx, word ptr [edi + 0dah]
        movzx  eax, ax
        mov    cl, byte ptr [ebp - 2]
        add    ebx, eax
        mov    byte ptr [ebx + edi + 58h], cl
        inc    word ptr [edx]
        jmp    SHORT loc_1_5D63
loc_1_5CEA:
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_9B4D
        add    esp, 4
        lea    eax, [ebp - 1]
        mov    ecx, dword ptr [esi + 8]
        push   eax
        mov    dx, word ptr [ecx + 20h]
        push   edx
        call   sub_1_9CE4
        add    esp, 8
        test   byte ptr [ebp - 1], 40h
        je     NEAR PTR loc_1_5D96
        test   byte ptr [ebp - 1], 80h
        jne    SHORT loc_1_5D58
        lea    eax, [ebp - 2]
        mov    ecx, dword ptr [edi + 8]
        mov    dx, word ptr [ecx + 1eh]
        push   eax
        push   edx
        call   sub_1_9CE4
        add    esp, 8
        test   byte ptr [edi], 4
        je     SHORT loc_1_5D58
        lea    eax, [edi + 0d8h]
        mov    dx, word ptr [eax]
        cmp    dx, 40h
        jae    SHORT loc_1_5D58
        movzx  ebx, word ptr [edi + 0dah]
        movzx  edx, dx
        mov    cl, byte ptr [ebp - 2]
        add    ebx, edx
        mov    byte ptr [ebx + edi + 58h], cl
        inc    word ptr [eax]
loc_1_5D58:
        push   dword ptr [ebp - 8]
        call   sub_1_9B5B
        add    esp, 4
loc_1_5D63:
        inc    dword ptr [ebp - 0ch]
        cmp    dword ptr [ebp - 0ch], 0ffffh
        jb     NEAR PTR loc_1_5C7E
        mov    eax, 1
loc_1_5D78:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_1_5D7F:
        mov    eax, dword ptr [esi + 8]
        push   dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax + 1eh]
        push   eax
        call   sub_1_9CF5
        add    esp, 8
        xor    eax, eax
        jmp    SHORT loc_1_5D78
loc_1_5D96:
        mov    eax, dword ptr [esi + 8]
        push   dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax + 1eh]
        push   eax
        call   sub_1_9CF5
        add    esp, 8
        push   dword ptr [ebp - 8]
        call   sub_1_9B5B
        add    esp, 4
        xor    eax, eax
        jmp    SHORT loc_1_5D78
sub_1_5DB8:
        mov    eax, dword ptr [esp + 4]
        mov    ecx, dword ptr [eax + 24h]
        mov    eax, dword ptr [ecx + 2eeh]
        and    eax, 2
        cmp    eax, 1
        sbb    eax, eax
        neg    eax
        ret
sub_1_5DD0:
        mov    eax, dword ptr [esp + 4]
        mov    ecx, dword ptr [eax + 54h]
        mov    eax, dword ptr [ecx + 3ceh]
        and    eax, 2
        cmp    eax, 1
        sbb    eax, eax
        neg    eax
        ret
sub_1_5DE8:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebp + 0ch]
        movzx  eax, word ptr [edi + 0ch]
        mov    dword ptr [ebp - 8], eax
        cmp    eax, 1
        sbb    al, al
        mov    ecx, dword ptr [edi + 12h]
        and    al, 4
        lea    ebx, [esi + 0e4h]
        add    al, 4
        mov    dword ptr [ebp - 0ch], ecx
        or     al, 51h
        cmp    dword ptr [ebx], 0
        mov    byte ptr [ebp - 1], al
        jne    SHORT loc_1_5E31
        push   dword ptr [esi + 0b4h]
        push   dword ptr [esi + 0dch]
        call   sub_1_233D
        add    esp, 8
loc_1_5E31:
        push   dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [esi + 0e0h]
        push   dword ptr [ebp - 1]
        push   dword ptr [ecx + 14h]
        push   dword ptr [ecx + 0ch]
        push   dword ptr [esi + 0b4h]
        push   dword ptr [esi + 0dch]
        call   sub_1_22DC
        add    esp, 18h
        mov    ecx, dword ptr [edi + 0eh]
        mov    dword ptr [ebp - 4], ecx
        cmp    dword ptr [ebx], 0
        jne    SHORT loc_1_5E73
        movzx  eax, word ptr [edi + 8]
        push   eax
        push   dword ptr [ebp - 8]
        push   esi
        call   sub_1_A2BD
        add    esp, 0ch
loc_1_5E73:
        push   edi
        xor    edi, edi
        push   dword ptr [ebp - 4]
        push   edi
        push   esi
        call   sub_1_A13B
        add    esp, 10h
        cmp    dword ptr [ebx], edi
        je     SHORT loc_1_5E89
        mov    dword ptr [ebx], edi
loc_1_5E89:
        push   esi
        call   sub_1_5B69
        add    esp, 4
        push   dword ptr [esi + 0b4h]
        push   dword ptr [esi + 0dch]
        call   sub_1_235D
        add    esp, 8
        push   esi
        call   sub_1_5BBC
        add    esp, 4
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_1_5EB8:
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 0ch]
        push   edi
        mov    edi, dword ptr [esi + 0e8h]
        cmp    dword ptr [edi + 0f4h], 0
        je     SHORT loc_1_5EF7
        cmp    dword ptr [edi + 1e0h], 0
        je     SHORT loc_1_5EF7
        push   dword ptr [esi + 0b4h]
        push   dword ptr [esi + 0dch]
        call   sub_1_233D
        mov    dword ptr [esi + 0e4h], 1
        add    esp, 8
        jmp    SHORT loc_1_5F5B
loc_1_5EF7:
        push   esi
        call   sub_1_A1BE
        add    esp, 4
        push   esi
        call   sub_1_5C0F
        add    esp, 4
        xor    ebx, ebx
        push   dword ptr [esi + 0dch]
        call   sub_1_2319
        add    esp, 4
        lea    esi, [edi + 1d0h]
        cmp    dword ptr [esi], ebx
        je     SHORT loc_1_5F39
        push   dword ptr [edi + 1a0h]
        push   dword ptr [edi + 1c8h]
        call   sub_1_235D
        add    esp, 8
        mov    dword ptr [esi], ebx
loc_1_5F39:
        lea    esi, [edi + 2bch]
        xor    ebx, ebx
        cmp    dword ptr [esi], ebx
        je     SHORT loc_1_5F5B
        push   dword ptr [edi + 28ch]
        push   dword ptr [edi + 2b4h]
        call   sub_1_235D
        add    esp, 8
        mov    dword ptr [esi], ebx
loc_1_5F5B:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_1_5F61:
        mov    eax, dword ptr [esp + 4]
        push   dword ptr [eax + 0b4h]
        push   dword ptr [eax + 0dch]
        call   sub_1_233D
        add    esp, 8
        xor    eax, eax
        ret
sub_1_5F7C:
        mov    eax, dword ptr [esp + 4]
        push   dword ptr [eax + 0b4h]
        push   dword ptr [eax + 0dch]
        call   sub_1_235D
        add    esp, 8
        xor    eax, eax
        ret
sub_1_5F97:
        push   dword ptr [esp + 0ch]
        push   dword ptr [esp + 0ch]
        push   dword ptr [esp + 0ch]
        call   sub_1_A2BD
        add    esp, 0ch
        ret
sub_1_5FAC:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 10h]
        mov    eax, dword ptr [ebx + 4]
        test   al, 1
        jne    SHORT loc_1_5FC9
        mov    eax, 3
        jmp    NEAR PTR loc_1_61A8
loc_1_5FC9:
        movzx  ecx, word ptr [ebp + 0ch]
        cmp    ecx, 0ah
        ja     SHORT loc_1_5FD9
        db  0FFh, 024h, 08Dh, 000h, 000h, 000h, 000h
loc_1_5FD9:
        mov    eax, 0ch
        jmp    NEAR PTR loc_1_61A8
loc_1_5FE3:
        push   dword ptr [ebp + 14h]
        push   ebx
        call   sub_1_61DB
        add    esp, 8
        jmp    NEAR PTR loc_1_6155
loc_1_5FF4:
        push   dword ptr [ebp + 14h]
        push   ebx
        call   sub_1_61F6
        add    esp, 8
        jmp    NEAR PTR loc_1_6155
loc_1_6005:
        push   dword ptr [ebp + 14h]
        push   dword ptr [ebp + 8]
        push   ebx
        call   sub_1_6217
        jmp    NEAR PTR loc_1_61A5
loc_1_6016:
        test   al, 2
        jne    SHORT loc_1_6024
        mov    eax, 5
        jmp    NEAR PTR loc_1_61A8
loc_1_6024:
        push   dword ptr [ebp + 8]
        push   ebx
        call   sub_1_62FB
        add    esp, 8
        jmp    NEAR PTR loc_1_61A8
loc_1_6035:
        test   al, 2
        jne    SHORT loc_1_6043
        mov    eax, 5
        jmp    NEAR PTR loc_1_61A8
loc_1_6043:
        push   dword ptr [ebp + 14h]
        push   ebx
        call   sub_1_634F
        add    esp, 8
        jmp    NEAR PTR loc_1_61A8
loc_1_6054:
        mov    edx, dword ptr [ebp + 14h]
        test   al, 2
        jne    SHORT loc_1_6065
        mov    eax, 5
        jmp    NEAR PTR loc_1_61A8
loc_1_6065:
        cmp    word ptr [edx + 16h], 1
        jne    SHORT loc_1_6073
        xor    eax, eax
        jmp    NEAR PTR loc_1_61A8
loc_1_6073:
        cmp    dword ptr [ebx + 8], 0
        je     SHORT loc_1_6083
        mov    eax, 1
        jmp    NEAR PTR loc_1_61A8
loc_1_6083:
        lea    edi, [ebx + 0ch]
        mov    esi, edx
        mov    ecx, 6
        push   edx
        rep movsd
        mov    dword ptr [ebx + 8], 1
        push   ebx
        call   sub_1_5DE8
        add    esp, 8
        jmp    NEAR PTR loc_1_6155
loc_1_60A5:
        test   al, 2
        jne    SHORT loc_1_60B3
        mov    eax, 5
        jmp    NEAR PTR loc_1_61A8
loc_1_60B3:
        push   dword ptr [ebp + 14h]
        push   ebx
        call   sub_1_636A
        add    esp, 8
        jmp    NEAR PTR loc_1_6155
loc_1_60C4:
        test   al, 2
        jne    SHORT loc_1_60D2
        mov    eax, 5
        jmp    NEAR PTR loc_1_61A8
loc_1_60D2:
        mov    eax, dword ptr [ebp + 14h]
        mov    eax, dword ptr [eax]
        cmp    eax, 1
        je     SHORT loc_1_60E8
        cmp    eax, 2
        je     SHORT loc_1_610E
        cmp    eax, 3
        je     SHORT loc_1_6134
        jmp    SHORT loc_1_6155
loc_1_60E8:
        cmp    dword ptr [ebx + 8], 1
        je     SHORT loc_1_60F8
        mov    eax, 1
        jmp    NEAR PTR loc_1_61A8
loc_1_60F8:
        push   ebx
        call   sub_1_5F61
        add    esp, 4
        test   eax, eax
        jne    SHORT loc_1_6155
        mov    dword ptr [ebx + 8], 2
        jmp    SHORT loc_1_6155
loc_1_610E:
        cmp    dword ptr [ebx + 8], 2
        je     SHORT loc_1_611E
        mov    eax, 1
        jmp    NEAR PTR loc_1_61A8
loc_1_611E:
        push   ebx
        call   sub_1_5F7C
        add    esp, 4
        test   eax, eax
        jne    SHORT loc_1_6155
        mov    dword ptr [ebx + 8], 1
        jmp    SHORT loc_1_6155
loc_1_6134:
        cmp    dword ptr [ebx + 8], 0
        jne    SHORT loc_1_6141
        mov    eax, 1
        jmp    SHORT loc_1_61A8
loc_1_6141:
        push   ebx
        call   sub_1_5EB8
        add    esp, 4
        test   eax, eax
        jne    SHORT loc_1_6155
        mov    dword ptr [ebx + 8], 0
loc_1_6155:
        xor    eax, eax
        jmp    SHORT loc_1_61A8
loc_1_6159:
        mov    esi, dword ptr [ebx + 0dch]
        mov    edi, dword ptr [ebp + 14h]
        test   byte ptr [esi], 1
        jne    SHORT loc_1_616E
        mov    eax, 3
        jmp    SHORT loc_1_61A8
loc_1_616E:
        lea    eax, [ebp - 2]
        push   eax
        push   dword ptr [ebp + 8]
        push   dword ptr [esi + 8]
        call   sub_1_97C6
        add    esp, 0ch
        test   eax, eax
        jne    SHORT loc_1_61A8
        mov    cx, word ptr [ebp - 2]
        inc    cx
        movzx  ecx, cx
        cmp    word ptr [esi + 6], 4
        jb     SHORT loc_1_6196
        add    ecx, ecx
loc_1_6196:
        mov    dword ptr [edi], ecx
        jmp    SHORT loc_1_61A8
loc_1_619A:
        push   dword ptr [ebp + 14h]
        push   0
        push   ebx
        call   sub_1_5F97
loc_1_61A5:
        add    esp, 0ch
loc_1_61A8:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
_LTEXT ends
        end
