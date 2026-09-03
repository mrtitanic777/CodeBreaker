; vpicd_pcode_09.asm -- VPICD object 9 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

loc_9_0000:
        push   ebx
        push   esi
        push   edi
        mov    eax, 1
        mov    esi, dword ptr [esp + 10h]
        cmp    esi, 7
        jae    SHORT loc_9_001F
        mov    ecx, dword ptr [esp + 1ch]
        db  0FFh, 014h, 0B5h, 036h, 000h, 000h, 000h
loc_9_001F:
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_9_0023:
        mov    ebx, dword ptr [ecx]
        call   sub_9_035B
        call   sub_9_01DA
        call   sub_9_03EE
        mov    edi, dword ptr [ebx + 0ch]
        call   sub_9_01AC
        clc
        je     SHORT loc_9_004F
        push   -1
        call   sub_9_006C
        jae    SHORT loc_9_004F
        push   0
        call   sub_9_006C
loc_9_004F:
        db  0C7h, 005h, 052h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        jae    SHORT loc_9_0065
        db  0C7h, 005h, 052h, 000h, 000h, 000h, 013h, 000h, 000h, 000h
loc_9_0065:
        db  0A1h, 052h, 000h, 000h, 000h
        ret
        db  090h
sub_9_006C:
        enter  8, 0
        mov    eax, 0ffffffffh
        mov    dword ptr [ebp - 8], eax
        mov    edx, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 4], edx
loc_9_007E:
        mov    si, word ptr [edi + 4]
        db  066h, 023h, 015h, 014h, 027h, 000h, 000h
        lea    ecx, [esi - 1]
        not    edx
        test   cx, si
        je     NEAR PTR loc_9_0187
loc_9_0097:
        and    esi, edx
        test   word ptr [edi], 1
        db  00Fh, 0B7h, 005h, 004h, 000h, 000h, 000h
        not    eax
        je     SHORT loc_9_00C4
        db  066h, 08Bh, 00Dh, 018h, 000h, 000h, 000h
        test   word ptr [edi], 2
        jne    SHORT loc_9_00BA
        not    cx
loc_9_00BA:
        db  066h, 023h, 00Dh, 006h, 000h, 000h, 000h
        or     ax, cx
loc_9_00C4:
        and    esi, eax
        je     NEAR PTR loc_9_0158
loc_9_00CC:
        mov    cl, 0ffh
        push   esi
loc_9_00CF:
        bsr    ax, si
        je     SHORT loc_9_00F3
        movzx  eax, ax
        db  08Ah, 0A8h, 008h, 000h, 000h, 000h
        sub    cl, ch
        sbb    dh, dh
        and    cl, dh
        sub    dl, al
        and    dl, dh
        add    dl, al
        add    cl, ch
        je     SHORT loc_9_00F3
        btr    esi, eax
        jmp    SHORT loc_9_00CF
loc_9_00F3:
        pop    esi
        mov    al, dl
        db  0FEh, 080h, 008h, 000h, 000h, 000h
        db  066h, 00Fh, 0ABh, 005h, 004h, 000h, 000h, 000h
        test   word ptr [edi], 1
        je     SHORT loc_9_0113
        db  066h, 00Fh, 0ABh, 005h, 006h, 000h, 000h, 000h
loc_9_0113:
        test   word ptr [edi], 2
        je     SHORT loc_9_0122
        db  066h, 00Fh, 0ABh, 005h, 018h, 000h, 000h, 000h
loc_9_0122:
        pushad
        call   sub_9_01AC
        je     SHORT loc_9_017D
        push   dword ptr [ebp - 8]
        call   sub_9_006C
        jae    SHORT loc_9_017D
        popad
        db  0FEh, 088h, 008h, 000h, 000h, 000h
        jne    SHORT loc_9_014D
        db  066h, 00Fh, 0B3h, 005h, 004h, 000h, 000h, 000h
        db  066h, 00Fh, 0B3h, 005h, 006h, 000h, 000h, 000h
loc_9_014D:
        btr    esi, eax
        or     esi, esi
        jne    NEAR PTR loc_9_00CC
loc_9_0158:
        test   dword ptr [ebp + 8], 0ffffffffh
        jne    SHORT loc_9_0178
        xor    dword ptr [ebp - 4], 0ffffffffh
        mov    edx, dword ptr [ebp - 4]
        je     NEAR PTR loc_9_007E
        xor    dword ptr [ebp - 8], 0ffffffffh
        je     NEAR PTR loc_9_007E
loc_9_0178:
        leave
        stc
        ret    4
loc_9_017D:
        popad
        mov    word ptr [edi + 2], ax
        leave
        clc
        ret    4
loc_9_0187:
        db  066h, 085h, 035h, 014h, 027h, 000h, 000h
        jne    NEAR PTR loc_9_0097
        or     si, si
        je     SHORT loc_9_0158
        db  066h, 009h, 035h, 014h, 027h, 000h, 000h
        db  080h, 00Dh, 0F8h, 026h, 000h, 000h, 001h
        jmp    NEAR PTR loc_9_0097
sub_9_01AC:
        push   0
        push   0
        push   4
        push   edi
        db  068h, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 01Fh, 000h, 033h, 000h
        add    esp, 14h
        cmp    eax, 0
        jne    SHORT loc_9_01CF
        db  08Bh, 03Dh, 000h, 000h, 000h, 000h
        or     edi, edi
loc_9_01CE:
        ret
loc_9_01CF:
        mov    ebx, dword ptr [ebx]
        or     ebx, ebx
        je     SHORT loc_9_01CE
        mov    edi, dword ptr [ebx + 0ch]
        jmp    SHORT sub_9_01AC
sub_9_01DA:
        pushad
        cld
        db  0BEh, 0FAh, 026h, 000h, 000h
        db  0BFh, 004h, 000h, 000h, 000h
        mov    ecx, 6
        rep movsd
        db  066h, 0A1h, 012h, 027h, 000h, 000h
        db  066h, 0A3h, 034h, 000h, 000h, 000h
        popad
        ret
loc_9_01FB:
        db  066h, 00Fh, 0BAh, 035h, 0F8h, 026h, 000h, 000h, 002h
loc_9_0204:
        mov    ebx, dword ptr [ecx]
        call   sub_9_01DA
        call   sub_9_03EE
        mov    edi, dword ptr [ebx + 0ch]
loc_9_0213:
        call   sub_9_01AC
        je     NEAR PTR loc_9_02B2
        movzx  eax, word ptr [edi + 2]
        cmp    eax, 0fh
        ja     NEAR PTR loc_9_02C8
        db  00Fh, 0A3h, 005h, 034h, 000h, 000h, 000h
        jae    SHORT loc_9_0251
        db  0FEh, 088h, 008h, 000h, 000h, 000h
        jne    SHORT loc_9_0251
        db  00Fh, 0B3h, 005h, 034h, 000h, 000h, 000h
        db  00Fh, 0B3h, 005h, 006h, 000h, 000h, 000h
        db  00Fh, 0B3h, 005h, 004h, 000h, 000h, 000h
loc_9_0251:
        db  00Fh, 0ABh, 005h, 004h, 000h, 000h, 000h
        jae    SHORT loc_9_0280
        test   word ptr [edi], 1
        je     SHORT loc_9_02BD
        db  00Fh, 0A3h, 005h, 006h, 000h, 000h, 000h
        jae    SHORT loc_9_02BD
        xor    cl, cl
        bt     word ptr [edi], 1
        adc    cl, 0
        db  00Fh, 0A3h, 005h, 018h, 000h, 000h, 000h
        sbb    cl, 0
        jne    SHORT loc_9_02BD
loc_9_0280:
        db  0FEh, 080h, 008h, 000h, 000h, 000h
        test   word ptr [edi], 1
        je     SHORT loc_9_0213
        db  00Fh, 0ABh, 005h, 006h, 000h, 000h, 000h
        db  00Fh, 0ABh, 005h, 018h, 000h, 000h, 000h
        test   word ptr [edi], 2
        jne    NEAR PTR loc_9_0213
        db  00Fh, 0B3h, 005h, 018h, 000h, 000h, 000h
        jmp    NEAR PTR loc_9_0213
loc_9_02B2:
        mov    eax, 0
loc_9_02B7:
        db  0A3h, 052h, 000h, 000h, 000h
        ret
loc_9_02BD:
        db  066h, 00Fh, 0BAh, 025h, 0F8h, 026h, 000h, 000h, 002h
        jb     SHORT loc_9_0280
loc_9_02C8:
        mov    eax, 13h
        jmp    SHORT loc_9_02B7
loc_9_02CF:
        db  066h, 00Fh, 0BAh, 02Dh, 0F8h, 026h, 000h, 000h, 002h
        jmp    NEAR PTR loc_9_0204
loc_9_02DD:
        db  083h, 03Dh, 052h, 000h, 000h, 000h, 000h
        mov    eax, 13h
        db  0A3h, 052h, 000h, 000h, 000h
        jne    SHORT loc_9_0315
        pushad
        cld
        db  0BEh, 004h, 000h, 000h, 000h
        db  0BFh, 0FAh, 026h, 000h, 000h
        mov    ecx, 6
        rep movsd
        db  066h, 0A1h, 034h, 000h, 000h, 000h
        db  066h, 0A3h, 012h, 027h, 000h, 000h
        popad
        mov    eax, 0
loc_9_0315:
        ret
loc_9_0316:
        db  0C7h, 005h, 052h, 000h, 000h, 000h, 013h, 000h, 000h, 000h
        mov    eax, 0
        ret
loc_9_0326:
        pushad
        db  0BFh, 01Ch, 000h, 000h, 000h
        db  0BEh, 0FAh, 026h, 000h, 000h
        cld
        lodsw
        not    ax
        stosw
        mov    ecx, 16h
        cld
        rep movsb
        popad
        db  0C7h, 001h, 01Ch, 000h, 000h, 000h
        mov    dword ptr [ecx + 4], 18h
        mov    eax, 0
        ret
loc_9_0355:
        mov    eax, 0
        ret
sub_9_035B:
        cmp    dword ptr [ebx], 0
        jne    SHORT loc_9_0361
        ret
loc_9_0361:
        pushad
loc_9_0362:
        mov    edx, dword ptr [ebx + 0ch]
        xor    esi, esi
        xor    edi, edi
loc_9_0369:
        push   0
        push   0
        push   4
        push   edx
        db  068h, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 01Fh, 000h, 033h, 000h
        add    esp, 14h
        cmp    eax, 0
        jne    SHORT loc_9_03C9
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        movzx  eax, word ptr [edx + 4]
        mov    cx, 0aaaah
        and    cx, ax
        xor    ax, cx
        add    cx, cx
        adc    ax, cx
        mov    cx, 0cccch
        and    cx, ax
        xor    ax, cx
        shr    cx, 2
        add    ax, cx
        add    al, ah
        mov    cl, al
        shr    cl, 4
        and    ax, 0fh
        add    al, cl
        test   word ptr [edx], 1
        je     SHORT loc_9_03C4
        shl    eax, 1
loc_9_03C4:
        add    esi, eax
        inc    edi
        jmp    SHORT loc_9_0369
loc_9_03C9:
        mov    eax, esi
        mov    ecx, edi
        jecxz  loc_9_03D3
        xor    edx, edx
        div    ecx
loc_9_03D3:
        mov    dword ptr [ebx + 10h], eax
        mov    ebx, dword ptr [ebx]
        or     ebx, ebx
        jne    SHORT loc_9_0362
        mov    eax, dword ptr [esp + 18h]
        push   0
        push   eax
        db  0CDh, 020h, 014h, 000h, 033h, 000h
        add    esp, 8
        popad
        ret
sub_9_03EE:
        pushad
loc_9_03EF:
        push   2
        push   dword ptr [ebx + 8]
        db  068h, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 01Ah, 000h, 033h, 000h
        add    esp, 0ch
        cmp    eax, 0
        jne    SHORT loc_9_044A
loc_9_0407:
        db  0A1h, 000h, 000h, 000h, 000h
        push   0
        push   0
        push   4
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 01Fh, 000h, 033h, 000h
        add    esp, 14h
        cmp    eax, 0
        jne    SHORT loc_9_044A
        db  08Bh, 03Dh, 000h, 000h, 000h, 000h
        movzx  eax, word ptr [edi + 2]
        db  0FEh, 088h, 008h, 000h, 000h, 000h
        jne    SHORT loc_9_0448
        db  066h, 00Fh, 0B3h, 005h, 004h, 000h, 000h, 000h
        db  066h, 00Fh, 0B3h, 005h, 006h, 000h, 000h, 000h
loc_9_0448:
        jmp    SHORT loc_9_0407
loc_9_044A:
        mov    ebx, dword ptr [ebx]
        or     ebx, ebx
        jne    SHORT loc_9_03EF
        popad
        ret
VPICD_Service_26:
        db  066h, 0A1h, 00Eh, 027h, 000h, 000h
        ret
VPICD_Service_27:
        mov    ax, word ptr [esp + 4]
        db  066h, 0A3h, 00Eh, 027h, 000h, 000h
        not    ax
        db  066h, 023h, 005h, 0FCh, 026h, 000h, 000h
        db  066h, 0A3h, 0F4h, 025h, 000h, 000h
        ret
        db  023h, 056h, 050h
_PTEXT ends
        end
