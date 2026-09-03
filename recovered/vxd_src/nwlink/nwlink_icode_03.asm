; nwlink_icode_03.asm -- NWLINK object 3 segment _ITEXT
.386p
_ITEXT segment dword public use32 'ICODE'

        db  083h, 0ECh, 004h, 06Ah, 042h, 0FFh, 074h, 024h, 00Ch, 08Dh, 044h, 024h, 008h, 068h, 014h, 058h
        db  000h, 000h, 050h, 0E8h, 092h, 024h, 0FDh, 0FFh, 083h, 07Ch, 024h, 000h, 001h, 01Bh, 0C0h, 040h
        db  083h, 0C4h, 004h, 0C3h
sub_3_0024:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        db  081h, 03Dh, 0A4h, 058h, 000h, 000h, 0A4h, 058h, 000h, 000h
        je     SHORT loc_3_0060
        db  08Bh, 035h, 0A4h, 058h, 000h, 000h
        mov    eax, esi
        mov    ecx, dword ptr [eax]
        db  0C7h, 041h, 004h, 0A4h, 058h, 000h, 000h
        mov    eax, dword ptr [eax]
        db  0A3h, 0A4h, 058h, 000h, 000h
        push   dword ptr [esi + 8]
        db  0E8h, 024h, 024h, 0FDh, 0FFh
        push   0
        push   0ch
        push   esi
        db  0E8h, 00Eh, 024h, 0FDh, 0FFh
loc_3_0060:
        db  081h, 03Dh, 0ACh, 058h, 000h, 000h, 0ACh, 058h, 000h, 000h
        je     SHORT loc_3_0096
        db  08Bh, 035h, 0ACh, 058h, 000h, 000h
        mov    eax, esi
        mov    ecx, dword ptr [eax]
        db  0C7h, 041h, 004h, 0ACh, 058h, 000h, 000h
        mov    eax, dword ptr [eax]
        db  0A3h, 0ACh, 058h, 000h, 000h
        push   dword ptr [esi + 8]
        db  0E8h, 000h, 024h, 0FDh, 0FFh
        push   0
        push   0ch
        push   esi
        db  0E8h, 0D8h, 023h, 0FDh, 0FFh
loc_3_0096:
        mov    esi, dword ptr [esp + 14h]
        or     esi, esi
        je     SHORT loc_3_00C2
        db  0BFh, 0CCh, 058h, 000h, 000h
        mov    ebx, 1
loc_3_00A8:
        mov    eax, dword ptr [edi]
        test   dword ptr [eax + 4], ebx
        je     SHORT loc_3_00BC
        push   dword ptr [eax + 0ch]
        lea    eax, [esp + 10h]
        push   eax
        db  0E8h, 000h, 024h, 0FDh, 0FFh
loc_3_00BC:
        add    edi, 4
        dec    esi
        jne    SHORT loc_3_00A8
loc_3_00C2:
        db  083h, 03Dh, 018h, 058h, 000h, 000h, 000h
        je     SHORT loc_3_00DE
        push   0
        db  0FFh, 035h, 01Ch, 058h, 000h, 000h
        db  0FFh, 035h, 018h, 058h, 000h, 000h
        db  0E8h, 090h, 023h, 0FDh, 0FFh
loc_3_00DE:
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
sub_3_00E5:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 10h]
        sub    ecx, ecx
        mov    word ptr [esi], 0
        mov    edx, dword ptr [esp + 0ch]
loc_3_00F6:
        mov    al, byte ptr [edx]
        inc    edx
        cmp    al, 30h
        jl     SHORT loc_3_0105
        cmp    al, 39h
        jg     SHORT loc_3_0105
        sub    al, 30h
        jmp    SHORT loc_3_011B
loc_3_0105:
        cmp    al, 41h
        jl     SHORT loc_3_0111
        cmp    al, 46h
        jg     SHORT loc_3_0111
        sub    al, 37h
        jmp    SHORT loc_3_011B
loc_3_0111:
        cmp    al, 61h
        jl     SHORT loc_3_0132
        cmp    al, 66h
        jg     SHORT loc_3_0132
        sub    al, 57h
loc_3_011B:
        mov    di, word ptr [esi]
        shl    di, 4
        movsx  ax, al
        add    di, ax
        mov    word ptr [esi], di
        inc    ecx
        cmp    ecx, 4
        jl     SHORT loc_3_00F6
loc_3_0132:
        or     ecx, ecx
        mov    eax, 0ffffffffh
        je     SHORT loc_3_013D
        sub    eax, eax
loc_3_013D:
        pop    edi
        pop    esi
        ret
sub_3_0140:
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edx, dword ptr [esp + 2ch]
        mov    edi, edx
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        mov    word ptr [esp + 1eh], cx
        mov    ax, cx
        mov    word ptr [esp + 1ch], ax
        mov    dword ptr [esp + 20h], edx
        lea    eax, [esp + 1ch]
        push   2
        push   eax
        lea    eax, [esp + 1ch]
        push   dword ptr [esp + 30h]
        push   eax
        lea    eax, [esp + 28h]
        push   eax
        db  0E8h, 01Bh, 023h, 0FDh, 0FFh
        cmp    dword ptr [esp + 18h], 0
        jne    NEAR PTR loc_3_0229
        mov    eax, dword ptr [esp + 14h]
        mov    edi, dword ptr [eax + 8]
        movzx  eax, word ptr [eax + 4]
        or     edi, edi
        je     NEAR PTR loc_3_0229
        sub    ebx, ebx
        sub    ecx, ecx
        cmp    eax, ecx
        jle    SHORT loc_3_01B5
loc_3_01A9:
        cmp    byte ptr [edi + ecx], 2ch
        jne    SHORT loc_3_01B0
        inc    ebx
loc_3_01B0:
        inc    ecx
        cmp    ecx, eax
        jl     SHORT loc_3_01A9
loc_3_01B5:
        push   -1
        push   0
        lea    eax, [ebx*2 + 2]
        push   eax
        inc    ebx
        push   dword ptr [esp + 40h]
        db  0E8h, 09Dh, 022h, 0FDh, 0FFh
        mov    eax, dword ptr [esp + 34h]
        cmp    dword ptr [eax], 0
        je     SHORT loc_3_0229
        sub    ebp, ebp
        cmp    ebx, ebp
        jle    SHORT loc_3_0229
        mov    esi, dword ptr [esp + 30h]
loc_3_01DE:
        cmp    byte ptr [edi], 2ch
        je     SHORT loc_3_0223
        lea    eax, [esp + 12h]
        push   eax
        push   edi
        call   sub_3_00E5
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_3_0229
        mov    ah, byte ptr [esp + 12h]
        mov    al, byte ptr [esp + 13h]
        mov    word ptr [esp + 12h], ax
        mov    cx, ax
        mov    eax, dword ptr [esp + 34h]
        mov    edx, dword ptr [esi]
        mov    eax, dword ptr [eax]
        mov    word ptr [eax + edx*2], cx
        inc    dword ptr [esi]
        cmp    byte ptr [edi], 2ch
        je     SHORT loc_3_0223
loc_3_0218:
        cmp    byte ptr [edi], 0
        je     SHORT loc_3_0223
        inc    edi
        cmp    byte ptr [edi], 2ch
        jne    SHORT loc_3_0218
loc_3_0223:
        inc    ebp
        inc    edi
        cmp    ebp, ebx
        jl     SHORT loc_3_01DE
loc_3_0229:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 14h
        ret
sub_3_0231:
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        lea    eax, [esp + 14h]
        db  068h, 00Ch, 07Ch, 000h, 000h
        push   eax
        lea    eax, [esp + 14h]
        push   eax
        db  0E8h, 07Dh, 022h, 0FDh, 0FFh
        cmp    dword ptr [esp + 0ch], 0
        je     SHORT loc_3_025C
        mov    eax, 1
        jmp    NEAR PTR loc_3_0342
loc_3_025C:
        db  0BEh, 014h, 07Bh, 000h, 000h
        db  083h, 03Dh, 014h, 07Bh, 000h, 000h, 000h
        je     SHORT loc_3_02DE
        sub    ebx, ebx
loc_3_026C:
        mov    edi, dword ptr [esi]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        mov    word ptr [esp + 1ah], cx
        mov    ax, cx
        mov    word ptr [esp + 18h], ax
        mov    eax, dword ptr [esi]
        mov    dword ptr [esp + 1ch], eax
        mov    ecx, dword ptr [esi + 10h]
        mov    eax, dword ptr [esi + 0ch]
        mov    dword ptr [ecx], eax
        lea    eax, [esp + 18h]
        push   ebx
        push   eax
        lea    eax, [esp + 18h]
        push   dword ptr [esp + 1ch]
        push   eax
        lea    eax, [esp + 1ch]
        push   eax
        db  0E8h, 0F1h, 021h, 0FDh, 0FFh
        cmp    dword ptr [esp + 0ch], ebx
        jne    SHORT loc_3_02D7
        mov    eax, dword ptr [esp + 10h]
        db  081h, 0FEh, 014h, 07Bh, 000h, 000h
        mov    eax, dword ptr [eax + 4]
        jne    SHORT loc_3_02C8
        add    eax, 3
        and    eax, 0fffffffch
loc_3_02C8:
        cmp    dword ptr [esi + 4], eax
        ja     SHORT loc_3_02D7
        cmp    dword ptr [esi + 8], eax
        jb     SHORT loc_3_02D7
        mov    ecx, dword ptr [esi + 10h]
        mov    dword ptr [ecx], eax
loc_3_02D7:
        add    esi, 14h
        cmp    dword ptr [esi], ebx
        jne    SHORT loc_3_026C
loc_3_02DE:
        db  068h, 0E8h, 056h, 000h, 000h
        db  068h, 0E0h, 056h, 000h, 000h
        db  068h, 014h, 07Ch, 000h, 000h
        push   dword ptr [esp + 20h]
        call   sub_3_0140
        add    esp, 10h
        db  083h, 03Dh, 0E0h, 056h, 000h, 000h, 000h
        jne    SHORT loc_3_032F
        db  068h, 0ECh, 056h, 000h, 000h
        db  068h, 0E4h, 056h, 000h, 000h
        db  068h, 028h, 07Ch, 000h, 000h
        push   dword ptr [esp + 20h]
        call   sub_3_0140
        add    esp, 10h
        db  083h, 03Dh, 0E0h, 056h, 000h, 000h, 000h
        jne    SHORT loc_3_032F
        db  083h, 03Dh, 0E4h, 056h, 000h, 000h, 000h
        je     SHORT loc_3_0339
loc_3_032F:
        db  081h, 00Dh, 0F8h, 058h, 000h, 000h, 000h, 000h, 001h, 000h
loc_3_0339:
        push   dword ptr [esp + 14h]
        db  0E8h, 062h, 021h, 0FDh, 0FFh
loc_3_0342:
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 14h
        ret
        db  000h, 000h, 000h, 00Bh, 0D2h, 074h, 00Ah, 00Fh, 0B7h, 0C2h, 032h, 0F6h, 0C1h, 0EAh, 00Ch, 003h
        db  0D0h, 089h, 015h, 018h, 059h, 000h, 000h, 081h, 00Dh, 0F8h, 058h, 000h, 000h, 000h, 001h, 000h
        db  000h, 0CDh, 020h, 000h, 000h, 028h, 000h, 0C3h
loc_3_0371:
        db  0F7h, 005h, 0F8h, 058h, 000h, 000h, 001h, 001h, 000h, 000h
        jne    NEAR PTR loc_3_0487
        db  081h, 00Dh, 0F8h, 058h, 000h, 000h, 000h, 001h, 000h, 000h
        call   sub_3_07DB
        rol    eax, 11h
        db  009h, 005h, 0F8h, 058h, 000h, 000h
        call   sub_3_06F9
        rol    eax, 11h
        db  009h, 005h, 0F8h, 058h, 000h, 000h
        call   sub_3_0698
        or     eax, eax
        je     SHORT loc_3_03C0
        db  0F7h, 005h, 0F8h, 058h, 000h, 000h, 000h, 000h, 002h, 000h
        je     NEAR PTR loc_3_049A
loc_3_03C0:
        db  0CDh, 020h, 000h, 000h, 028h, 000h
        jb     NEAR PTR loc_3_049A
        db  0CDh, 020h, 000h, 000h, 092h, 004h
        jb     SHORT loc_3_03DE
        db  081h, 00Dh, 0F8h, 058h, 000h, 000h, 000h, 002h, 000h, 000h
loc_3_03DE:
        db  068h, 087h, 008h, 000h, 000h
        call   0
        pop    edx
        or     eax, eax
        jne    NEAR PTR loc_3_049A
        call   sub_3_0231
        or     eax, eax
        jne    NEAR PTR loc_3_0495
        db  0E8h, 0FDh, 0FBh, 0FCh, 0FFh
        or     eax, eax
        jne    NEAR PTR loc_3_0495
        call   sub_3_0586
        push   0
        push   0ch
        db  0CDh, 020h, 0A7h, 000h, 001h, 000h
        add    esp, 8
        or     eax, eax
        je     SHORT loc_3_0489
        db  0A3h, 020h, 058h, 000h, 000h
        call   sub_3_04C2
        push   4
        push   8
        db  0CDh, 020h, 0A8h, 000h, 001h, 000h
        add    esp, 8
        or     eax, eax
        je     SHORT loc_3_0495
        mov    dword ptr [eax + 1], 0eah
        db  08Bh, 03Dh, 0FCh, 058h, 000h, 000h
        mov    dword ptr [eax + 2], edi
        mov    word ptr [eax + 6], 1e03h
        shl    eax, 0ch
        inc    eax
        db  0A3h, 0FCh, 058h, 000h, 000h
        add    eax, 5
        db  0A3h, 030h, 055h, 000h, 000h
        call   sub_3_049C
        db  083h, 03Dh, 018h, 059h, 000h, 000h, 000h
        je     SHORT loc_3_0478
        call   sub_3_063B
        jb     SHORT loc_3_0489
loc_3_0478:
        db  0E8h, 047h, 04Ah, 0FDh, 0FFh
        db  081h, 00Dh, 0F8h, 058h, 000h, 000h, 000h, 020h, 000h, 000h
loc_3_0487:
        clc
        ret
loc_3_0489:
        db  0FFh, 035h, 034h, 058h, 000h, 000h
        call   sub_3_0024
        pop    eax
loc_3_0495:
        db  0E8h, 0C9h, 00Ch, 0FDh, 0FFh
loc_3_049A:
        stc
        ret
sub_3_049C:
        xor    eax, eax
        db  0C7h, 005h, 0C0h, 058h, 000h, 000h, 037h, 000h, 000h, 000h
        db  0A3h, 090h, 058h, 000h, 000h
        db  0B8h, 094h, 058h, 000h, 000h
        mov    dword ptr [eax], eax
        mov    dword ptr [eax + 4], eax
        db  0B8h, 09Ch, 058h, 000h, 000h
        mov    dword ptr [eax], eax
        mov    dword ptr [eax + 4], eax
        ret
sub_3_04C2:
        mov    eax, 2fh
        db  0BEh, 06Fh, 038h, 000h, 000h
        db  0CDh, 020h, 041h, 000h, 001h, 000h
        mov    eax, 7ah
        db  0BEh, 0A8h, 037h, 000h, 000h
        db  0CDh, 020h, 041h, 000h, 001h, 000h
        db  0BEh, 0A2h, 037h, 000h, 000h
        db  0CDh, 020h, 00Bh, 000h, 001h, 000h
        db  0A3h, 0FCh, 058h, 000h, 000h
        mov    eax, 2fh
        db  0CDh, 020h, 044h, 000h, 001h, 000h
        db  089h, 00Dh, 004h, 059h, 000h, 000h
        db  089h, 015h, 008h, 059h, 000h, 000h
        db  0BEh, 05Ch, 037h, 000h, 000h
        db  0CDh, 020h, 00Ch, 000h, 001h, 000h
        mov    ecx, eax
        movzx  edx, cx
        shr    ecx, 10h
        mov    eax, 2fh
        db  0CDh, 020h, 045h, 000h, 001h, 000h
        mov    eax, 7ah
        db  0CDh, 020h, 044h, 000h, 001h, 000h
        db  089h, 00Dh, 00Ch, 059h, 000h, 000h
        db  089h, 015h, 010h, 059h, 000h, 000h
        db  0BEh, 093h, 036h, 000h, 000h
        db  0CDh, 020h, 00Ch, 000h, 001h, 000h
        mov    ecx, eax
        movzx  edx, cx
        shr    ecx, 10h
        mov    eax, 7ah
        db  0CDh, 020h, 045h, 000h, 001h, 000h
        db  0BEh, 08Dh, 036h, 000h, 000h
        db  0CDh, 020h, 00Ch, 000h, 001h, 000h
        db  0A3h, 000h, 059h, 000h, 000h
        mov    eax, 400024h
        db  0BEh, 0CAh, 053h, 000h, 000h
        db  0CDh, 020h, 090h, 000h, 001h, 000h
        jb     SHORT loc_3_0584
        db  089h, 035h, 084h, 058h, 000h, 000h
loc_3_0584:
        clc
        ret
sub_3_0586:
        sub    esp, 6ch
        push   edi
        lea    edi, [esp + 4]
        db  0CDh, 020h, 08Dh, 000h, 001h, 000h
        pop    edi
        db  0CDh, 020h, 082h, 000h, 001h, 000h
        mov    word ptr [ebp + 1ch], 7a00h
        mov    eax, 2fh
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        mov    al, byte ptr [ebp + 1ch]
        inc    al
        jne    SHORT loc_3_062F
        mov    ax, word ptr [ebp + 38h]
        shl    eax, 10h
        mov    ax, word ptr [ebp]
        db  0A3h, 014h, 059h, 000h, 000h
        db  083h, 00Dh, 0F8h, 058h, 000h, 000h, 008h
        push   0
        push   0ah
        db  0CDh, 020h, 0A9h, 000h, 001h, 000h
        add    esp, 8
        or     eax, eax
        je     SHORT loc_3_062F
        push   eax
        shr    eax, 4
        mov    word ptr [ebp + 38h], ax
        mov    word ptr [ebp + 4], 0
        mov    word ptr [ebp + 10h], 9
        db  08Bh, 00Dh, 014h, 059h, 000h, 000h
        movzx  edx, cx
        shr    ecx, 10h
        db  0CDh, 020h, 048h, 000h, 001h, 000h
        db  0CDh, 020h, 085h, 000h, 001h, 000h
        pop    eax
        mov    ecx, dword ptr [eax]
        db  08Bh, 035h, 0DCh, 058h, 000h, 000h
        mov    dword ptr [esi + 18h], ecx
        db  0CDh, 020h, 0AAh, 000h, 001h, 000h
loc_3_0619:
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        push   esi
        lea    esi, [esp + 4]
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        pop    esi
        add    esp, 6ch
        ret
loc_3_062F:
        db  0C7h, 005h, 018h, 059h, 000h, 000h, 000h, 000h, 000h, 000h
        jmp    SHORT loc_3_0619
sub_3_063B:
        db  08Bh, 03Dh, 024h, 058h, 000h, 000h
        lea    ecx, [edi + edi*2]
        push   2
        push   ecx
        db  0CDh, 020h, 0A8h, 000h, 001h, 000h
        add    esp, 8
        or     eax, eax
        je     SHORT loc_3_0696
        db  08Bh, 035h, 018h, 059h, 000h, 000h
        lea    edi, [eax + edi*2]
        db  0A3h, 05Ch, 058h, 000h, 000h
        db  089h, 03Dh, 064h, 058h, 000h, 000h
        shl    eax, 0ch
        shr    ax, 0ch
        shl    edi, 0ch
        shr    di, 0ch
        mov    dword ptr [esi + 32h], eax
        mov    dword ptr [esi + 36h], edi
        mov    dword ptr [esi + 3ah], 0
        lea    eax, [esi + 3ah]
        db  0A3h, 040h, 058h, 000h, 000h
        db  0A1h, 0D8h, 055h, 000h, 000h
        mov    word ptr [esi + 42h], ax
        clc
        ret
loc_3_0696:
        stc
        ret
sub_3_0698:
        push   ebp
        sub    esp, 6ch
        push   edi
        lea    edi, [esp + 4]
        db  0CDh, 020h, 08Dh, 000h, 001h, 000h
        pop    edi
        db  0CDh, 020h, 082h, 000h, 001h, 000h
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        mov    ebp, dword ptr [ebx + 8]
        mov    word ptr [ebp + 14h], 0
        mov    word ptr [ebp + 1ch], 7a00h
        mov    eax, 2fh
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        mov    cl, byte ptr [ebp + 1ch]
        xor    eax, eax
        inc    cl
        jne    SHORT loc_3_06D9
        inc    al
loc_3_06D9:
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        push   esi
        lea    esi, [esp + 4]
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        pop    esi
        add    esp, 6ch
        pop    ebp
        ret
loc_3_06F0:
        dec    esi
        inc    esp
        dec    ecx
        push   ebx
        dec    eax
        dec    esp
        push   eax
        and    al, 0
sub_3_06F9:
        push   ebp
        push   esi
        push   edi
        sub    esp, 6ch
        push   edi
        lea    edi, [esp + 4]
        db  0CDh, 020h, 08Dh, 000h, 001h, 000h
        pop    edi
        db  0CDh, 020h, 082h, 000h, 001h, 000h
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        mov    ebp, dword ptr [ebx + 8]
        push   0
        push   80h
        db  0CDh, 020h, 0A9h, 000h, 001h, 000h
        add    esp, 8
        or     eax, eax
        je     NEAR PTR loc_3_07B4
        mov    edi, eax
        mov    edx, eax
        db  0BEh, 0F0h, 006h, 000h, 000h
        mov    ecx, 9
        rep movsb
        mov    eax, edx
        shr    eax, 4
        mov    word ptr [ebp + 3ch], ax
        mov    ax, dx
        and    ax, 0fh
        mov    word ptr [ebp + 14h], ax
        mov    word ptr [ebp + 1ch], 3d40h
        mov    ax, 21h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        mov    eax, 0
        test   dword ptr [ebp + 2ch], 1
        jne    SHORT loc_3_07AE
        mov    ax, word ptr [ebp + 1ch]
        mov    word ptr [ebp + 10h], ax
        mov    byte ptr [ebp + 1dh], 3eh
        mov    ax, 21h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        db  0CDh, 020h, 0AAh, 000h, 001h, 000h
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        push   esi
        lea    esi, [esp + 4]
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        pop    esi
        add    esp, 6ch
        pop    edi
        pop    esi
        pop    ebp
        mov    eax, 1
        ret
loc_3_07AE:
        db  0CDh, 020h, 0AAh, 000h, 001h, 000h
loc_3_07B4:
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        push   esi
        lea    esi, [esp + 4]
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        pop    esi
        add    esp, 6ch
        pop    edi
        pop    esi
        pop    ebp
        xor    eax, eax
        ret
loc_3_07CF:
        dec    edi
        inc    esp
        dec    ecx
        and    al, 48h
        dec    esp
        push   eax
        and    al, 0
loc_3_07D8:
        xor    eax, eax
        ret
sub_3_07DB:
        db  0CDh, 020h, 000h, 000h, 097h, 004h
        jb     SHORT loc_3_07D8
        push   ebp
        push   edi
        push   esi
        push   edx
        sub    esp, 6ch
        push   edi
        lea    edi, [esp + 4]
        db  0CDh, 020h, 08Dh, 000h, 001h, 000h
        pop    edi
        db  0CDh, 020h, 082h, 000h, 001h, 000h
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        mov    ebp, dword ptr [ebx + 8]
        mov    ebx, dword ptr [ebx + 4]
        mov    dx, 5100h
        mov    word ptr [ebp + 1ch], 5100h
loc_3_0812:
        mov    word ptr [ebp + 1ch], dx
        mov    eax, 2fh
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        cmp    byte ptr [ebp + 1ch], 0ffh
        je     SHORT loc_3_082F
loc_3_0827:
        inc    dh
        jne    SHORT loc_3_0812
        xor    eax, eax
        jmp    SHORT loc_3_0863
loc_3_082F:
        movzx  eax, word ptr [ebp + 38h]
        shl    eax, 4
        movzx  ecx, word ptr [ebp + 4]
        add    eax, ecx
        add    eax, ebx
        mov    edi, eax
        db  0BEh, 0CFh, 007h, 000h, 000h
        cld
        cmpsd
        jne    SHORT loc_3_0827
        cmpsd
        jne    SHORT loc_3_0827
        movzx  eax, word ptr [ebp + 14h]
        shl    eax, 4
        movzx  ecx, word ptr [ebp + 10h]
        add    eax, ecx
        db  0A3h, 018h, 059h, 000h, 000h
        mov    eax, 1
loc_3_0863:
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        push   esi
        lea    esi, [esp + 4]
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        pop    esi
        add    esp, 6ch
        pop    edx
        pop    esi
        pop    edi
        pop    ebp
        ret
        db  000h, 000h, 000h
loc_3_0880:
        dec    esi
        push   edi
        dec    esp
        dec    ecx
        dec    esi
        dec    ebx
        db  000h
loc_3_0887:
        add    ecx, dword ptr [edx]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        or     al, byte ptr [eax]
        db  000h, 07Eh, 011h
        add    byte ptr [eax], al
        db  0B9h, 021h, 000h, 000h, 01Bh
        and    al, byte ptr [eax]
        db  000h, 068h, 01Bh
        add    byte ptr [eax], al
        push   eax
        and    al, 0
        db  000h, 06Bh, 01Bh
        add    byte ptr [eax], al
        db  07Bh, 021h
        add    byte ptr [eax], al
        db  0D1h, 00Dh, 000h, 000h, 08Fh, 022h
        add    byte ptr [eax], al
        int3
        pop    ss
        add    byte ptr [eax], al
        inc    esi
        adc    eax, dword ptr [eax]
        db  000h, 0D7h
        sbb    al, byte ptr [eax]
        add    byte ptr [esi], al
        add    byte ptr [esi], al
        db  000h, 080h, 008h, 000h, 000h, 000h
        add    byte ptr [eax], al
_ITEXT ends
        end
