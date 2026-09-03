; mrci2_icode_02.asm -- MRCI2 object 2 segment _ITEXT
.386p
_ITEXT segment dword public use32 'ICODE'

loc_2_0000:
        db  0B8h, 000h, 000h, 000h, 000h
        and    eax, 0fffff000h
        db  0BAh, 0F0h, 0AEh, 000h, 000h
        add    edx, 0fffh
        and    edx, 0fffff000h
        sub    edx, eax
        shr    eax, 0ch
        shr    edx, 0ch
        db  0A3h, 098h, 000h, 000h, 000h
        db  089h, 015h, 094h, 000h, 000h, 000h
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        sub    eax, eax
        db  0A3h, 028h, 000h, 000h, 000h
        db  0A3h, 0BAh, 000h, 000h, 000h
        call   sub_2_0076
        call   sub_2_01BB
        call   sub_2_032C
        db  0A1h, 028h, 000h, 000h, 000h
        db  00Bh, 005h, 0BAh, 000h, 000h, 000h
        cmp    eax, 1
        jb     SHORT loc_2_0075
        db  0BEh, 070h, 000h, 000h, 000h
        db  0CDh, 020h, 0EFh, 000h, 001h, 000h
        db  0BEh, 080h, 000h, 000h, 000h
        db  0CDh, 020h, 0EFh, 000h, 001h, 000h
loc_2_0075:
        ret
sub_2_0076:
        pushad
        db  0CDh, 020h, 011h, 001h, 001h, 000h
        cmp    eax, 20000000h
        ja     NEAR PTR loc_2_0155
        sub    esp, 6ch
        push   edi
        lea    edi, [esp + 4]
        db  0CDh, 020h, 08Dh, 000h, 001h, 000h
        pop    edi
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        mov    ebp, dword ptr [ebx + 8]
        db  0CDh, 020h, 083h, 000h, 001h, 000h
        push   edx
        push   0
        push   11h
        db  0CDh, 020h, 0A9h, 000h, 001h, 000h
        add    esp, 8
        pop    edx
        or     eax, eax
        je     NEAR PTR loc_2_0140
        mov    edi, eax
        mov    word ptr [ebp + 1ch], 4a11h
        mov    word ptr [ebp + 10h], 0
        mov    eax, 2fh
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        cmp    word ptr [ebp + 1ch], 0
        jne    SHORT loc_2_013A
        cmp    word ptr [ebp + 10h], 444dh
        jne    SHORT loc_2_013A
        movzx  ecx, byte ptr [ebp + 19h]
        movzx  edx, byte ptr [ebp + 18h]
        sub    dl, 41h
        add    ecx, edx
        sub    ecx, 1ah
        sbb    edx, edx
        and    ecx, edx
        add    ecx, 1ah
        xor    edx, edx
loc_2_00FE:
        mov    word ptr [edi], 444dh
        mov    byte ptr [edi + 2], 49h
        mov    word ptr [edi + 3], 3f3fh
        mov    word ptr [ebp + 1ch], 4404h
        inc    dl
        mov    byte ptr [ebp + 10h], dl
        mov    word ptr [ebp + 18h], 11h
        mov    eax, edi
        shr    eax, 4
        mov    word ptr [ebp + 3ch], ax
        mov    word ptr [ebp + 14h], 0
        mov    eax, 21h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        loop   loc_2_00FE
loc_2_013A:
        db  0CDh, 020h, 0AAh, 000h, 001h, 000h
loc_2_0140:
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        push   esi
        lea    esi, [esp + 4]
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        pop    esi
        add    esp, 6ch
loc_2_0155:
        popad
        ret
sub_2_0157:
        movzx  eax, word ptr [edi + 0ch]
        and    ah, 7fh
        cmp    eax, ebx
        jne    SHORT loc_2_0171
        cmp    word ptr [edi + 0eh], 0
        jne    SHORT loc_2_0171
        cmp    word ptr [edi + 10h], 8000h
        jbe    SHORT loc_2_019C
loc_2_0171:
        cmp    word ptr [edi + 10h], 8001h
        jae    SHORT loc_2_017F
        cmp    word ptr [edi + 6], 201h
loc_2_017F:
        sbb    ecx, ecx
        and    ecx, ebx
        not    ecx
        and    ecx, eax
        mov    edx, ebx
        not    ecx
        and    edx, ecx
        je     SHORT loc_2_01B9
        and    ebx, eax
        cmp    eax, ebx
        jne    SHORT loc_2_019A
        cmp    word ptr [edi + 0eh], 0
loc_2_019A:
        clc
        ret
loc_2_019C:
        cmp    word ptr [edi + 6], 200h
        jb     SHORT loc_2_01B6
        ja     SHORT loc_2_01B9
        cmp    dword ptr [edi], 5446534dh
        jne    SHORT loc_2_01B9
        cmp    word ptr [edi + 4], 200h
        ja     SHORT loc_2_01B9
loc_2_01B6:
        cmp    edi, edi
        ret
loc_2_01B9:
        stc
        ret
sub_2_01BB:
        push   ebx
        sub    edi, edi
        mov    ebp, dword ptr [ebx + 8]
        db  0CDh, 020h, 083h, 000h, 001h, 000h
        mov    word ptr [ebp + 1ch], 4a12h
        mov    word ptr [ebp + 18h], 4d52h
        mov    word ptr [ebp + 14h], 4349h
        mov    eax, 2fh
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        db  0C7h, 005h, 028h, 000h, 000h, 000h, 0EBh, 002h, 000h, 000h
        cmp    word ptr [ebp + 18h], 4943h
        jne    SHORT loc_2_027B
        cmp    word ptr [ebp + 14h], 524dh
        jne    SHORT loc_2_027B
        movzx  edi, word ptr [ebp + 38h]
        db  066h, 089h, 03Dh, 036h, 000h, 000h, 000h
        shl    edi, 4
        movzx  edx, word ptr [ebp]
        db  066h, 089h, 015h, 034h, 000h, 000h, 000h
        add    edi, edx
        add    edi, dword ptr [ebx + 4]
        test   byte ptr [edi + 0dh], 80h
        je     SHORT loc_2_022F
        db  083h, 025h, 034h, 000h, 000h, 000h, 000h
loc_2_022F:
        mov    ebx, 2ebh
        call   sub_2_0157
        ja     SHORT loc_2_0248
        jb     NEAR PTR loc_2_0320
        mov    eax, 2ebh
        jmp    SHORT loc_2_027B
loc_2_0248:
        db  089h, 015h, 028h, 000h, 000h, 000h
        or     eax, 2ebh
        mov    ecx, dword ptr [edi + 8]
        db  089h, 00Dh, 02Ch, 000h, 000h, 000h
        mov    bx, word ptr [edi + 0eh]
        mov    cx, word ptr [edi + 10h]
        db  066h, 089h, 01Dh, 00Eh, 000h, 000h, 000h
        db  066h, 039h, 00Dh, 010h, 000h, 000h, 000h
        ja     SHORT loc_2_027B
        db  066h, 089h, 00Dh, 010h, 000h, 000h, 000h
loc_2_027B:
        push   eax
        db  0CDh, 020h, 011h, 001h, 001h, 000h
        cmp    eax, 20000000h
        mov    eax, 0
        ja     SHORT loc_2_029B
        push   1
        push   12h
        db  0CDh, 020h, 0A8h, 000h, 001h, 000h
        add    esp, 8
loc_2_029B:
        pop    ebx
        mov    edx, eax
        or     eax, eax
        je     SHORT loc_2_02B3
        pop    ecx
        push   ecx
        add    edx, dword ptr [ecx + 4]
        shl    eax, 0ch
        shr    ax, 0ch
        db  0A3h, 034h, 000h, 000h, 000h
loc_2_02B3:
        db  0BEh, 0A6h, 001h, 000h, 000h
        db  0CDh, 020h, 00Ch, 000h, 001h, 000h
        db  0A3h, 09Ch, 000h, 000h, 000h
        db  0BEh, 0A6h, 001h, 000h, 000h
        db  0CDh, 020h, 00Bh, 000h, 001h, 000h
        jb     SHORT loc_2_0320
        db  0BEh, 000h, 000h, 000h, 000h
        mov    dword ptr [esi + 8], eax
        or     edx, edx
        je     SHORT loc_2_02F1
        xchg   edx, edi
        mov    ecx, 12h
        cld
        push   esi
        rep movsb
        pop    esi
        sub    edi, 12h
        mov    word ptr [edi + 0ch], bx
        mov    edi, edx
loc_2_02F1:
        or     edi, edi
        je     SHORT loc_2_0317
        test   byte ptr [edi + 0dh], 80h
        jne    SHORT loc_2_0317
        mov    ecx, 12h
        push   ecx
        push   esi
        push   edi
        mov    esi, edi
        db  089h, 035h, 024h, 000h, 000h, 000h
        db  0BFh, 012h, 000h, 000h, 000h
        rep movsb
        pop    edi
        pop    esi
        pop    ecx
        rep movsb
loc_2_0317:
        call   sub_2_03ED
        jb     SHORT loc_2_0320
        jmp    SHORT loc_2_032A
loc_2_0320:
        db  0C7h, 005h, 028h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_2_032A:
        pop    ebx
        ret
sub_2_032C:
        mov    eax, 4a12h
        mov    edx, 4d524349h
        push   2fh
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        db  0C7h, 005h, 0BAh, 000h, 000h, 000h, 0EBh, 003h, 000h, 000h
        cmp    edx, 4943524dh
        jne    SHORT loc_2_03C4
        mov    ebx, 3ebh
        call   sub_2_0157
        ja     SHORT loc_2_0378
        jb     NEAR PTR loc_2_03E2
        movzx  eax, word ptr [edi + 0ch]
        and    ah, 7fh
        db  0A3h, 044h, 004h, 000h, 000h
        mov    eax, dword ptr [edi + 8]
        db  0A3h, 048h, 004h, 000h, 000h
        jmp    SHORT loc_2_03B1
loc_2_0378:
        or     eax, 3ebh
        db  089h, 015h, 0BAh, 000h, 000h, 000h
        mov    ecx, dword ptr [edi + 8]
        db  089h, 00Dh, 030h, 000h, 000h, 000h
        mov    bx, word ptr [edi + 0eh]
        mov    cx, word ptr [edi + 10h]
        db  066h, 0A3h, 0B4h, 000h, 000h, 000h
        db  066h, 089h, 01Dh, 0B6h, 000h, 000h, 000h
        db  066h, 039h, 00Dh, 0B8h, 000h, 000h, 000h
        je     SHORT loc_2_03B1
        db  066h, 089h, 00Dh, 0B8h, 000h, 000h, 000h
loc_2_03B1:
        test   byte ptr [edi + 0dh], 80h
        jne    SHORT loc_2_03C4
        db  0BEh, 0A8h, 000h, 000h, 000h
        mov    ecx, 12h
        cld
        rep movsb
loc_2_03C4:
        call   sub_2_03ED
        jb     SHORT loc_2_03E2
        db  08Bh, 01Dh, 044h, 004h, 000h, 000h
        or     ebx, ebx
        je     SHORT loc_2_03EC
        mov    eax, 0ffffffffh
        db  0FFh, 015h, 048h, 004h, 000h, 000h
        jmp    SHORT loc_2_03EC
loc_2_03E2:
        db  0C7h, 005h, 0BAh, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_2_03EC:
        ret
sub_2_03ED:
        db  00Fh, 0BAh, 02Dh, 040h, 004h, 000h, 000h, 000h
        cmc
        jae    SHORT loc_2_043D
        mov    eax, 2fh
        db  0BEh, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 041h, 000h, 001h, 000h
        mov    eax, 2fh
        db  0CDh, 020h, 044h, 000h, 001h, 000h
        db  089h, 00Dh, 0A4h, 000h, 000h, 000h
        db  089h, 015h, 0A0h, 000h, 000h, 000h
        db  0BEh, 086h, 001h, 000h, 000h
        db  0CDh, 020h, 00Ch, 000h, 001h, 000h
        movzx  edx, ax
        mov    ecx, eax
        shr    ecx, 10h
        mov    eax, 2fh
        db  0CDh, 020h, 045h, 000h, 001h, 000h
loc_2_043D:
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_2_0448:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
_ITEXT ends
        end
