; vnbt_icode_02.asm -- VNBT object 2 segment _ITEXT
.386p
_ITEXT segment dword public use32 'ICODE'

        db  0CDh, 020h, 000h, 000h, 088h, 004h, 072h, 014h, 0CDh, 020h, 000h, 000h, 040h, 000h, 072h, 00Ch
        db  0CDh, 020h, 00Ah, 000h, 088h, 004h, 0A3h, 068h, 02Eh, 001h, 000h, 0F8h, 0F5h, 01Bh, 0C0h, 0C3h
loc_2_0020:
        dec    ebp
        push   ebx
        push   esp
        inc    ebx
        push   eax
        add    byte ptr [eax], al
loc_2_0027:
        add    byte ptr [esi + 42h], cl
        push   esp
        db  000h
loc_2_002C:
        db  080h, 03Dh, 026h, 000h, 000h, 000h, 002h
        jne    SHORT loc_2_0038
        clc
        jmp    SHORT loc_2_007F
loc_2_0038:
        db  0C6h, 005h, 026h, 000h, 000h, 000h, 001h
        db  0BFh, 0A0h, 034h, 001h, 000h
        db  0B9h, 0A0h, 034h, 001h, 000h
        sub    ecx, edi
        shr    ecx, 2
        sub    eax, eax
        cld
        rep stosd
        db  0CDh, 020h, 000h, 000h, 088h, 004h
        jb     SHORT loc_2_007F
        db  0B8h, 020h, 000h, 000h, 000h
        push   eax
        db  0CDh, 020h, 008h, 000h, 088h, 004h
        add    esp, 4
        cmp    eax, 0
        jne    SHORT loc_2_0072
        stc
        jmp    SHORT loc_2_007F
loc_2_0072:
        db  0A3h, 094h, 034h, 001h, 000h
        call   sub_2_00B0
        cmp    eax, 1
loc_2_007F:
        jb     SHORT loc_2_009F
        db  080h, 03Dh, 026h, 000h, 000h, 000h, 001h
        je     SHORT loc_2_0097
        db  080h, 03Dh, 027h, 000h, 000h, 000h, 001h
        clc
        je     SHORT loc_2_009F
        stc
        jmp    SHORT loc_2_009F
loc_2_0097:
        clc
        db  0C6h, 005h, 027h, 000h, 000h, 000h, 001h
loc_2_009F:
        db  0C6h, 005h, 026h, 000h, 000h, 000h, 002h
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_2_00B0:
        push   esi
        push   edi
        sub    esi, esi
        call   0
        cmp    eax, esi
        je     NEAR PTR loc_2_01B4
        db  068h, 080h, 05Fh, 000h, 000h
        db  0E8h, 0E5h, 0FFh, 0FCh, 0FFh
        add    esp, 4
        db  0E8h, 0CDh, 0B8h, 0FDh, 0FFh
        db  0E8h, 07Ch, 0FFh, 0FCh, 0FFh
        db  08Bh, 015h, 080h, 02Fh, 001h, 000h
        mov    edi, edx
        sub    eax, eax
        mov    ecx, 49h
        rep stosd
        stosw
        push   edx
        call   sub_2_01C0
        add    esp, 4
        cmp    eax, esi
        je     SHORT loc_2_0103
        cmp    eax, 0ffh
        jne    NEAR PTR loc_2_01B4
loc_2_0103:
        db  08Bh, 00Dh, 080h, 02Fh, 001h, 000h
        lea    eax, [ecx + 70h]
        mov    dword ptr [ecx + 74h], eax
        mov    dword ptr [eax], eax
        lea    eax, [ecx + 78h]
        mov    dword ptr [ecx + 7ch], eax
        mov    dword ptr [eax], eax
        call   sub_2_03F0
        or     eax, eax
        je     SHORT loc_2_012D
        cmp    eax, 0ffh
        jne    NEAR PTR loc_2_01B4
loc_2_012D:
        call   sub_2_0664
        or     eax, eax
        je     SHORT loc_2_0150
        cmp    eax, 0ffh
        je     SHORT loc_2_0150
        call   sub_2_07D4
        db  0E8h, 099h, 0BAh, 0FDh, 0FFh
        mov    eax, esi
        pop    edi
        pop    esi
        ret
        db  08Dh, 064h, 024h, 000h
loc_2_0150:
        db  0E8h, 04Bh, 0A6h, 0FDh, 0FFh
        push   1
        db  068h, 0F0h, 04Eh, 000h, 000h
        db  0E8h, 05Bh, 0C7h, 0FDh, 0FFh
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_2_0177
        call   sub_2_07D4
        db  0E8h, 06Eh, 0BAh, 0FDh, 0FFh
        mov    eax, esi
        pop    edi
        pop    esi
        ret
loc_2_0177:
        call   sub_2_02B0
        push   0
        push   0
        db  0E8h, 05Bh, 095h, 0FDh, 0FFh
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_2_01A0
        call   sub_2_07D4
        db  0E8h, 04Ah, 0BAh, 0FDh, 0FFh
        mov    eax, esi
        pop    edi
        pop    esi
        ret
        db  005h, 000h, 000h, 000h, 000h
loc_2_01A0:
        db  0C7h, 005h, 0CCh, 02Eh, 001h, 000h, 000h, 000h, 000h, 000h
        db  0E8h, 0A5h, 0FEh, 0FCh, 0FFh
        mov    esi, 1
loc_2_01B4:
        db  0E8h, 027h, 0BAh, 0FDh, 0FFh
        mov    eax, esi
        pop    edi
        pop    esi
        ret
        db  08Bh, 0FFh
sub_2_01C0:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0ch]
        mov    edi, esi
        sub    eax, eax
        mov    ecx, 49h
        rep stosd
        stosw
        push   eax
        push   esi
        call   sub_2_0828
        add    esp, 8
        mov    eax, dword ptr [esi + 0f3h]
        or     eax, eax
        je     SHORT loc_2_0210
        push   104h
        push   eax
        db  0E8h, 04Eh, 007h, 0FDh, 0FFh
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_2_0201
        mov    eax, 1
        pop    edi
        pop    esi
        ret
loc_2_0201:
        mov    byte ptr [esi + 120h], 1
        jmp    SHORT loc_2_0217
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_2_0210:
        mov    byte ptr [esi + 120h], 0
loc_2_0217:
        mov    word ptr [esi + 0a2h], 0eh
        xor    eax, eax
        db  066h, 0A1h, 064h, 033h, 001h, 000h
        cmp    eax, 1
        je     SHORT loc_2_0240
        cmp    eax, 2
        je     SHORT loc_2_0250
        cmp    eax, 4
        je     SHORT loc_2_0260
        cmp    eax, 8
        je     SHORT loc_2_0270
        jmp    SHORT loc_2_0279
        db  08Bh, 0FFh
loc_2_0240:
        mov    word ptr [esi + 0d8h], 0
        jmp    SHORT loc_2_0279
        db  005h, 000h, 000h, 000h, 000h
loc_2_0250:
        mov    word ptr [esi + 0d8h], 2000h
        jmp    SHORT loc_2_0279
        db  005h, 000h, 000h, 000h, 000h
loc_2_0260:
        mov    word ptr [esi + 0d8h], 4000h
        jmp    SHORT loc_2_0279
        db  005h, 000h, 000h, 000h, 000h
loc_2_0270:
        mov    word ptr [esi + 0d8h], 6000h
loc_2_0279:
        push   0
        push   0ffh
        db  068h, 014h, 02Fh, 001h, 000h
        push   0
        db  0E8h, 034h, 0B6h, 0FDh, 0FFh
        add    esp, 10h
        db  0A2h, 0F4h, 033h, 001h, 000h
        db  0BFh, 0C4h, 033h, 001h, 000h
        sub    eax, eax
        mov    ecx, 8
        rep stosd
        pop    edi
        pop    esi
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_02B0:
        sub    esp, 4
        push   esi
        push   edi
        db  0C7h, 005h, 02Fh, 033h, 001h, 000h, 000h, 000h, 000h, 000h
        db  0E8h, 078h, 0C7h, 0FDh, 0FFh
        mov    esi, eax
        or     esi, esi
        je     NEAR PTR loc_2_03B3
        mov    edi, esi
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        db  0BFh, 020h, 02Fh, 001h, 000h
        lea    edx, [ecx - 1]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        lea    eax, [edx + ecx]
        push   eax
        db  0E8h, 0E9h, 068h, 0FDh, 0FFh
        add    esp, 4
        mov    edx, eax
        or     edx, edx
        je     NEAR PTR loc_2_03B3
        mov    edi, esi
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        mov    esi, edi
        mov    edi, edx
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        db  0BEh, 020h, 02Fh, 001h, 000h
        mov    edi, edx
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        mov    eax, dword ptr [esi]
        mov    dword ptr [edi - 1], eax
        mov    ax, word ptr [esi + 4]
        mov    word ptr [edi + 3], ax
        dec    edi
        db  089h, 015h, 02Fh, 033h, 001h, 000h
        db  0C7h, 005h, 033h, 033h, 001h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 037h, 033h, 001h, 000h, 000h, 000h, 000h, 000h
        lea    eax, [esp + 8]
        push   eax
        db  068h, 028h, 02Fh, 001h, 000h
        db  0E8h, 0D4h, 0B4h, 0FDh, 0FFh
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_2_037C
        mov    eax, dword ptr [esp + 8]
        db  0A3h, 033h, 033h, 001h, 000h
loc_2_037C:
        lea    eax, [esp + 8]
        push   eax
        db  068h, 030h, 02Fh, 001h, 000h
        db  0E8h, 0B5h, 0B4h, 0FDh, 0FFh
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_2_03B3
        mov    eax, dword ptr [esp + 8]
        cmp    byte ptr [eax], 0
        je     SHORT loc_2_03A6
        db  0A3h, 037h, 033h, 001h, 000h
        pop    edi
        pop    esi
        add    esp, 4
        ret
loc_2_03A6:
        mov    eax, dword ptr [esp + 8]
        push   eax
        db  0E8h, 098h, 0FCh, 0FCh, 0FFh
        add    esp, 4
loc_2_03B3:
        pop    edi
        pop    esi
        add    esp, 4
        ret
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_03C0:
        push   esi
        mov    ecx, dword ptr [esp + 8]
        sub    eax, eax
        cmp    byte ptr [ecx], al
        je     SHORT loc_2_03DF
        mov    edx, dword ptr [esp + 0ch]
        nop
loc_2_03D0:
        movsx  esi, byte ptr [ecx]
        cmp    esi, edx
        jne    SHORT loc_2_03D9
        mov    eax, ecx
loc_2_03D9:
        inc    ecx
        cmp    byte ptr [ecx], 0
        jne    SHORT loc_2_03D0
loc_2_03DF:
        pop    esi
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_03F0:
        push   edi
        db  0C6h, 005h, 04Fh, 033h, 001h, 000h, 000h
        db  0C6h, 005h, 050h, 033h, 001h, 000h, 000h
        db  0C6h, 005h, 051h, 033h, 001h, 000h, 000h
        db  0BFh, 0F8h, 033h, 001h, 000h
        sub    eax, eax
        mov    ecx, 25h
        rep stosd
        db  08Bh, 00Dh, 080h, 02Fh, 001h, 000h
        lea    eax, [ecx + 8]
        mov    dword ptr [ecx + 0ch], eax
        mov    dword ptr [eax], eax
        db  0C7h, 005h, 078h, 032h, 001h, 000h, 074h, 032h, 001h, 000h
        db  0C7h, 005h, 074h, 032h, 001h, 000h, 074h, 032h, 001h, 000h
        db  0C7h, 005h, 080h, 032h, 001h, 000h, 07Ch, 032h, 001h, 000h
        db  0C7h, 005h, 07Ch, 032h, 001h, 000h, 07Ch, 032h, 001h, 000h
        db  0C6h, 005h, 01Bh, 033h, 001h, 000h, 040h
        db  0C6h, 005h, 016h, 033h, 001h, 000h, 001h
        db  0C7h, 005h, 0A4h, 033h, 001h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 084h, 033h, 001h, 000h, 080h, 033h, 001h, 000h
        db  0C7h, 005h, 080h, 033h, 001h, 000h, 080h, 033h, 001h, 000h
        db  0C7h, 005h, 060h, 033h, 001h, 000h, 05Ch, 033h, 001h, 000h
        db  0C7h, 005h, 05Ch, 033h, 001h, 000h, 05Ch, 033h, 001h, 000h
        push   0
        movzx  eax, word ptr [ecx + 90h]
        push   eax
        add    ecx, 80h
        push   ecx
        db  0E8h, 0E0h, 0C5h, 0FDh, 0FFh
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_2_04B2
        cmp    eax, 0ffh
        jne    NEAR PTR loc_2_0662
loc_2_04B2:
        db  08Bh, 00Dh, 080h, 02Fh, 001h, 000h
        movzx  eax, word ptr [ecx + 8ch]
        push   eax
        movzx  eax, word ptr [ecx + 8eh]
        push   eax
        push   ecx
        db  0E8h, 072h, 0C6h, 0FDh, 0FFh
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_2_04E0
        cmp    eax, 0ffh
        jne    NEAR PTR loc_2_0662
loc_2_04E0:
        db  0C7h, 005h, 070h, 032h, 001h, 000h, 06Ch, 032h, 001h, 000h
        db  0C7h, 005h, 06Ch, 032h, 001h, 000h, 06Ch, 032h, 001h, 000h
        db  0C7h, 005h, 068h, 032h, 001h, 000h, 064h, 032h, 001h, 000h
        db  0C7h, 005h, 064h, 032h, 001h, 000h, 064h, 032h, 001h, 000h
        db  0C7h, 005h, 068h, 033h, 001h, 000h, 010h, 027h, 000h, 000h
        db  0C7h, 005h, 088h, 032h, 001h, 000h, 084h, 032h, 001h, 000h
        db  0C7h, 005h, 084h, 032h, 001h, 000h, 084h, 032h, 001h, 000h
        db  0C7h, 005h, 090h, 032h, 001h, 000h, 08Ch, 032h, 001h, 000h
        db  0C7h, 005h, 08Ch, 032h, 001h, 000h, 08Ch, 032h, 001h, 000h
        db  0C7h, 005h, 098h, 032h, 001h, 000h, 094h, 032h, 001h, 000h
        db  0C7h, 005h, 094h, 032h, 001h, 000h, 094h, 032h, 001h, 000h
        db  0C7h, 005h, 0A0h, 032h, 001h, 000h, 09Ch, 032h, 001h, 000h
        db  0C7h, 005h, 09Ch, 032h, 001h, 000h, 09Ch, 032h, 001h, 000h
        db  066h, 0C7h, 005h, 05Ch, 032h, 001h, 000h, 008h, 000h
        db  066h, 0C7h, 005h, 05Eh, 032h, 001h, 000h, 048h, 000h
        db  066h, 0C7h, 005h, 060h, 032h, 001h, 000h, 037h, 000h
        db  066h, 0C7h, 005h, 048h, 032h, 001h, 000h, 002h, 000h
        push   2
        push   8
        db  068h, 08Ch, 032h, 001h, 000h
        db  0E8h, 034h, 0F6h, 0FEh, 0FFh
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_2_05A6
        cmp    eax, 0ffh
        jne    NEAR PTR loc_2_0662
loc_2_05A6:
        push   2
        db  066h, 0C7h, 005h, 04Ah, 032h, 001h, 000h, 002h, 000h
        push   48h
        db  068h, 094h, 032h, 001h, 000h
        db  0E8h, 00Bh, 0F6h, 0FEh, 0FFh
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_2_05CF
        cmp    eax, 0ffh
        jne    NEAR PTR loc_2_0662
loc_2_05CF:
        push   2
        db  066h, 0C7h, 005h, 04Ch, 032h, 001h, 000h, 002h, 000h
        push   37h
        db  068h, 09Ch, 032h, 001h, 000h
        db  0E8h, 0E2h, 0F5h, 0FEh, 0FFh
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_2_05F4
        cmp    eax, 0ffh
        jne    SHORT loc_2_0662
loc_2_05F4:
        db  0A1h, 080h, 02Fh, 001h, 000h
        mov    word ptr [eax + 24h], 56h
        db  066h, 0C7h, 005h, 044h, 032h, 001h, 000h, 000h, 000h
        push   2
        db  068h, 064h, 032h, 001h, 000h
        db  0E8h, 04Ch, 0F6h, 0FEh, 0FFh
        add    esp, 8
        mov    ecx, eax
        or     ecx, ecx
        je     SHORT loc_2_062C
        cmp    ecx, 0ffh
        je     SHORT loc_2_062C
        mov    eax, ecx
        pop    edi
        ret
        db  08Dh, 049h, 000h
loc_2_062C:
        sub    eax, eax
        db  0BFh, 06Ch, 033h, 001h, 000h
        stosd
        stosd
        stosd
        stosd
        stosb
        db  0C7h, 005h, 090h, 034h, 001h, 000h, 08Ch, 034h, 001h, 000h
        db  0C7h, 005h, 08Ch, 034h, 001h, 000h, 08Ch, 034h, 001h, 000h
        db  0C7h, 005h, 070h, 033h, 001h, 000h, 06Ch, 033h, 001h, 000h
        db  0C7h, 005h, 06Ch, 033h, 001h, 000h, 06Ch, 033h, 001h, 000h
        mov    eax, ecx
loc_2_0662:
        pop    edi
        ret
sub_2_0664:
        sub    esp, 4
        push   esi
        push   2
        db  0E8h, 0C9h, 009h, 0FFh, 0FFh
        add    esp, 4
        db  066h, 0C7h, 005h, 05Ah, 032h, 001h, 000h, 041h, 000h
        db  066h, 0C7h, 005h, 046h, 032h, 001h, 000h, 002h, 000h
        db  0C7h, 005h, 0F0h, 032h, 001h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 0F8h, 032h, 001h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 0F4h, 032h, 001h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 0E4h, 032h, 001h, 000h, 008h, 000h, 000h, 000h
        or     eax, eax
        je     SHORT loc_2_06BB
        cmp    eax, 0ffh
        jne    NEAR PTR loc_2_07CF
loc_2_06BB:
        db  0F6h, 005h, 064h, 033h, 001h, 000h, 001h
        jne    SHORT loc_2_0714
        db  0A1h, 0FCh, 032h, 001h, 000h
        db  0A3h, 0E0h, 032h, 001h, 000h
        db  066h, 0C7h, 005h, 014h, 033h, 001h, 000h, 000h, 000h
        lea    eax, [esp + 4]
        push   eax
        push   0
        push   0
        push   0
        db  068h, 0C0h, 0F5h, 000h, 000h
        push   0
        push   0
        db  0A1h, 0FCh, 032h, 001h, 000h
        shr    eax, 3
        push   eax
        db  0E8h, 077h, 00Ch, 0FFh, 0FFh
        add    esp, 20h
        or     eax, eax
        je     SHORT loc_2_070B
        cmp    eax, 0ffh
        jne    NEAR PTR loc_2_07CF
loc_2_070B:
        mov    eax, dword ptr [esp + 4]
        db  0A3h, 0F0h, 032h, 001h, 000h
loc_2_0714:
        sub    edx, edx
        mov    ecx, 0ea60h
        db  0A1h, 0E8h, 032h, 001h, 000h
        div    ecx
        db  066h, 0A3h, 01Ch, 033h, 001h, 000h
        or     ax, ax
        jne    SHORT loc_2_0736
        db  066h, 0C7h, 005h, 01Ch, 033h, 001h, 000h, 001h, 000h
loc_2_0736:
        lea    eax, [esp + 4]
        push   eax
        push   0
        push   0
        push   0
        db  068h, 0B8h, 0F7h, 000h, 000h
        push   0
        push   0
        push   0ea60h
        db  0E8h, 01Ch, 00Ch, 0FFh, 0FFh
        add    esp, 20h
        mov    esi, eax
        or     esi, esi
        je     SHORT loc_2_0774
        cmp    esi, 0ffh
        je     SHORT loc_2_0774
        call   sub_2_07D4
        mov    eax, esi
        pop    esi
        add    esp, 4
        ret
        db  08Dh, 049h, 000h
loc_2_0774:
        mov    eax, dword ptr [esp + 4]
        db  0A3h, 0F8h, 032h, 001h, 000h
        db  083h, 03Dh, 000h, 033h, 001h, 000h, 0FFh
        je     SHORT loc_2_07CD
        lea    eax, [esp + 4]
        push   eax
        push   0
        push   0
        push   0
        db  068h, 038h, 0F9h, 000h, 000h
        push   0
        push   0
        db  0FFh, 035h, 000h, 033h, 001h, 000h
        db  0E8h, 0CBh, 00Bh, 0FFh, 0FFh
        add    esp, 20h
        mov    esi, eax
        or     esi, esi
        je     SHORT loc_2_07C4
        cmp    esi, 0ffh
        je     SHORT loc_2_07C4
        call   sub_2_07D4
        mov    eax, esi
        pop    esi
        add    esp, 4
        ret
        db  08Bh, 0FFh
loc_2_07C4:
        mov    eax, dword ptr [esp + 4]
        db  0A3h, 0F4h, 032h, 001h, 000h
loc_2_07CD:
        sub    eax, eax
loc_2_07CF:
        pop    esi
        add    esp, 4
        ret
sub_2_07D4:
        db  083h, 03Dh, 0F0h, 032h, 001h, 000h, 000h
        je     SHORT loc_2_07EF
        push   0
        push   0
        db  0FFh, 035h, 0F0h, 032h, 001h, 000h
        db  0E8h, 0ECh, 00Dh, 0FFh, 0FFh
        add    esp, 0ch
loc_2_07EF:
        db  083h, 03Dh, 0F4h, 032h, 001h, 000h, 000h
        je     SHORT loc_2_080A
        push   0
        push   0
        db  0FFh, 035h, 0F4h, 032h, 001h, 000h
        db  0E8h, 0D1h, 00Dh, 0FFh, 0FFh
        add    esp, 0ch
loc_2_080A:
        db  083h, 03Dh, 0F8h, 032h, 001h, 000h, 000h
        je     SHORT loc_2_0825
        push   0
        push   0
        db  0FFh, 035h, 0F8h, 032h, 001h, 000h
        db  0E8h, 0B6h, 00Dh, 0FFh, 0FFh
        add    esp, 0ch
loc_2_0825:
        sub    eax, eax
        ret
sub_2_0828:
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 10h]
        push   edi
        mov    esi, dword ptr [esp + 10h]
        push   esi
        db  0E8h, 067h, 001h, 0FDh, 0FFh
        add    esp, 8
        push   0
        push   89h
        db  068h, 084h, 02Fh, 001h, 000h
        push   edi
        db  0E8h, 072h, 0B0h, 0FDh, 0FFh
        add    esp, 10h
        mov    word ptr [esi + 0dch], ax
        push   0
        push   35h
        db  068h, 094h, 02Fh, 001h, 000h
        push   edi
        db  0E8h, 059h, 0B0h, 0FDh, 0FFh
        add    esp, 10h
        mov    word ptr [esi + 0deh], ax
        push   1
        push   11h
        db  068h, 0A4h, 02Fh, 001h, 000h
        push   0
        db  0E8h, 03Fh, 0B0h, 0FDh, 0FFh
        add    esp, 10h
        mov    word ptr [esi + 117h], ax
        push   1
        push   0ffh
        db  068h, 0B4h, 02Fh, 001h, 000h
        push   0
        db  0E8h, 022h, 0B0h, 0FDh, 0FFh
        add    esp, 10h
        mov    word ptr [esi + 119h], ax
        push   0ea60h
        push   57e40h
        db  068h, 0C8h, 02Fh, 001h, 000h
        push   edi
        db  0E8h, 003h, 0B0h, 0FDh, 0FFh
        add    esp, 10h
        mov    dword ptr [esi + 0b4h], eax
        push   0ea600h
        push   0ea600h
        db  068h, 0D8h, 02Fh, 001h, 000h
        push   edi
        db  0E8h, 0E5h, 0AFh, 0FDh, 0FFh
        add    esp, 10h
        mov    dword ptr [esi + 0c8h], eax
        push   1
        push   3
        db  068h, 0ECh, 02Fh, 001h, 000h
        push   edi
        db  0E8h, 0CDh, 0AFh, 0FDh, 0FFh
        add    esp, 10h
        mov    word ptr [esi + 9eh], ax
        push   64h
        push   2eeh
        db  068h, 000h, 030h, 001h, 000h
        push   edi
        db  0E8h, 0B1h, 0AFh, 0FDh, 0FFh
        add    esp, 10h
        mov    dword ptr [esi + 94h], eax
        push   1
        push   3
        db  068h, 014h, 030h, 001h, 000h
        push   edi
        db  0E8h, 099h, 0AFh, 0FDh, 0FFh
        add    esp, 10h
        mov    word ptr [esi + 9ch], ax
        push   64h
        push   5dch
        db  068h, 028h, 030h, 001h, 000h
        push   edi
        db  0E8h, 07Dh, 0AFh, 0FDh, 0FFh
        add    esp, 10h
        mov    dword ptr [esi + 98h], eax
        push   0ea60h
        push   -1
        db  068h, 03Ch, 030h, 001h, 000h
        push   edi
        db  0E8h, 062h, 0AFh, 0FDh, 0FFh
        add    esp, 10h
        mov    dword ptr [esi + 0cch], eax
        push   0
        push   0
        db  068h, 050h, 030h, 001h, 000h
        push   edi
        db  0E8h, 04Ah, 0AFh, 0FDh, 0FFh
        add    esp, 10h
        mov    byte ptr [esi + 11eh], al
        push   0
        push   0
        db  068h, 060h, 030h, 001h, 000h
        push   edi
        db  0E8h, 032h, 0AFh, 0FDh, 0FFh
        add    esp, 10h
        mov    byte ptr [esi + 11dh], al
        push   0
        push   0
        db  068h, 070h, 030h, 001h, 000h
        push   edi
        db  0E8h, 01Ah, 0AFh, 0FDh, 0FFh
        add    esp, 10h
        mov    byte ptr [esi + 11fh], al
        push   3e8h
        push   1770h
        db  068h, 07Ch, 030h, 001h, 000h
        push   edi
        db  0E8h, 0FCh, 0AEh, 0FDh, 0FFh
        add    esp, 10h
        mov    dword ptr [esi + 0efh], eax
        push   20000h
        push   20000h
        db  068h, 08Ch, 030h, 001h, 000h
        push   edi
        db  0E8h, 0DEh, 0AEh, 0FDh, 0FFh
        add    esp, 10h
        mov    dword ptr [esi + 0ebh], eax
        push   0
        push   0
        db  068h, 0A0h, 030h, 001h, 000h
        push   edi
        db  0E8h, 0C6h, 0AEh, 0FDh, 0FFh
        add    esp, 10h
        mov    byte ptr [esi + 121h], al
        push   3e8h
        push   3a98h
        db  068h, 0B4h, 030h, 001h, 000h
        push   edi
        db  0E8h, 0A8h, 0AEh, 0FDh, 0FFh
        add    esp, 10h
        mov    dword ptr [esi + 0b8h], eax
        push   8
        push   8
        db  068h, 0C4h, 030h, 001h, 000h
        push   edi
        db  0E8h, 090h, 0AEh, 0FDh, 0FFh
        add    esp, 10h
        cmp    eax, 9
        mov    word ptr [esi + 124h], 48h
        je     SHORT loc_2_0A4A
        mov    word ptr [esi + 124h], 40h
loc_2_0A4A:
        push   0
        push   0
        db  068h, 0D4h, 030h, 001h, 000h
        push   edi
        db  0E8h, 067h, 0AEh, 0FDh, 0FFh
        add    esp, 10h
        or     eax, eax
        je     SHORT loc_2_0A6D
        cmp    eax, 30h
        jae    SHORT loc_2_0A6D
        db  066h, 083h, 00Dh, 064h, 033h, 001h, 000h, 010h
loc_2_0A6D:
        db  066h, 0A1h, 064h, 033h, 001h, 000h
        and    ax, 10h
        cmp    ax, 1
        sbb    eax, eax
        and    eax, 0fffffffeh
        add    eax, 3
        push   eax
        push   eax
        db  068h, 0E0h, 030h, 001h, 000h
        push   edi
        db  0E8h, 030h, 0AEh, 0FDh, 0FFh
        add    esp, 10h
        cmp    eax, 2
        je     SHORT loc_2_0AE8
        cmp    eax, 3
        je     NEAR PTR loc_2_0B2C
        mov    word ptr [esi + 8ah], 0ah
        mov    word ptr [esi + 8ch], 0ah
        mov    word ptr [esi + 90h], 10h
        mov    word ptr [esi + 8eh], 10h
        mov    word ptr [esi + 1ah], 0ffffh
        mov    word ptr [esi + 1ch], 14h
        mov    word ptr [esi + 1eh], 0c8h
        mov    word ptr [esi + 20h], 0c8h
        mov    word ptr [esi + 22h], 0c8h
        jmp    NEAR PTR loc_2_0B6E
loc_2_0AE8:
        mov    word ptr [esi + 8ah], 14h
        mov    word ptr [esi + 8ch], 64h
        mov    word ptr [esi + 90h], 80h
        mov    word ptr [esi + 8eh], 80h
        mov    word ptr [esi + 1ah], 3e8h
        mov    word ptr [esi + 1ch], 3e8h
        mov    word ptr [esi + 1eh], 3e8h
        mov    word ptr [esi + 20h], 3e8h
        mov    word ptr [esi + 22h], 3e8h
        jmp    SHORT loc_2_0B6E
loc_2_0B2C:
        mov    word ptr [esi + 8ah], 0ffffh
        mov    word ptr [esi + 8ch], 0ffh
        mov    word ptr [esi + 90h], 0ffh
        mov    word ptr [esi + 8eh], 0ffh
        mov    word ptr [esi + 1ah], 0ffffh
        mov    word ptr [esi + 1ch], 0ffffh
        mov    word ptr [esi + 1eh], 0ffffh
        mov    word ptr [esi + 20h], 0ffffh
        mov    word ptr [esi + 22h], 0ffffh
loc_2_0B6E:
        push   edi
        push   esi
        call   sub_2_0B7C
        add    esp, 8
        pop    edi
        pop    esi
        ret
        db  090h
sub_2_0B7C:
        sub    esp, 4
        mov    eax, dword ptr [esp + 8]
        mov    eax, dword ptr [eax + 0f3h]
        or     eax, eax
        je     SHORT loc_2_0B96
        push   eax
        db  0E8h, 0B5h, 0F4h, 0FCh, 0FFh
        add    esp, 4
loc_2_0B96:
        lea    eax, [esp]
        push   eax
        db  068h, 034h, 031h, 001h, 000h
        db  0E8h, 09Bh, 0ACh, 0FDh, 0FFh
        add    esp, 8
        or     eax, eax
        je     SHORT loc_2_0BCC
        cmp    eax, 0ffh
        je     SHORT loc_2_0BCC
        db  0C7h, 005h, 027h, 033h, 001h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 02Bh, 033h, 001h, 000h, 000h, 000h, 000h, 000h
        add    esp, 4
        ret
        db  090h
loc_2_0BCC:
        push   5ch
        mov    eax, dword ptr [esp + 4]
        db  0A3h, 027h, 033h, 001h, 000h
        push   dword ptr [esp + 4]
        call   sub_2_03C0
        add    esp, 8
        or     eax, eax
        je     SHORT loc_2_0BF8
        sub    eax, dword ptr [esp]
        inc    eax
        db  0A3h, 02Bh, 033h, 001h, 000h
        add    esp, 4
        ret
        db  08Dh, 049h, 000h
loc_2_0BF8:
        add    esp, 4
        db  0C7h, 005h, 027h, 033h, 001h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 02Bh, 033h, 001h, 000h, 000h, 000h, 000h, 000h
        ret
_ITEXT ends
        end
