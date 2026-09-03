; vmm_pcode_21.asm -- VMM object 21 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

VMM_Add_DDB:
        pushad
        mov    ax, word ptr [edi + 6]
        cmp    ax, 0
        je     SHORT loc_21_001D
        db  0B9h, 040h, 0C3h, 000h, 000h
loc_21_0010:
        mov    ecx, dword ptr [ecx]
        jecxz  loc_21_001D
        cmp    word ptr [ecx + 6], ax
        jne    SHORT loc_21_0010
        popad
        stc
        ret
loc_21_001D:
        mov    eax, dword ptr [edi + 14h]
        db  0BEh, 040h, 0C3h, 000h, 000h
loc_21_0025:
        mov    ecx, dword ptr [esi]
        jecxz  loc_21_0032
        cmp    dword ptr [ecx + 14h], eax
        ja     SHORT loc_21_0032
        mov    esi, ecx
        jmp    SHORT loc_21_0025
loc_21_0032:
        mov    dword ptr [edi], ecx
        mov    dword ptr [esi], edi
        cmp    word ptr [edi + 4], 400h
        jb     SHORT loc_21_005C
        mov    ecx, edi
loc_21_0040:
        mov    ecx, dword ptr [ecx]
        jecxz  loc_21_004E
        cmp    word ptr [ecx + 4], 400h
        jb     SHORT loc_21_0040
        jmp    SHORT loc_21_0053
loc_21_004E:
        db  0B9h, 080h, 0D9h, 000h, 000h
loc_21_0053:
        push   dword ptr [ecx + 3ch]
        pop    dword ptr [edi + 3ch]
        mov    dword ptr [ecx + 3ch], edi
loc_21_005C:
        popad
        clc
        ret
VMM_Remove_DDB:
        push   ecx
        test   byte ptr [edi + 0bh], 40h
        je     SHORT loc_21_007A
        movzx  ecx, word ptr [edi + 6]
        cmp    ecx, 40h
        jae    SHORT loc_21_007A
        db  0C7h, 004h, 08Dh, 0E0h, 0C5h, 000h, 000h, 000h, 000h, 000h, 000h
loc_21_007A:
        db  0B9h, 040h, 0C3h, 000h, 000h
loc_21_007F:
        mov    ecx, dword ptr [ecx]
        jecxz  loc_21_00A5
        cmp    dword ptr [ecx], edi
        jne    SHORT loc_21_007F
        push   dword ptr [edi]
        pop    dword ptr [ecx]
        db  0B9h, 080h, 0D9h, 000h, 000h
        jmp    SHORT loc_21_0097
loc_21_0092:
        mov    ecx, dword ptr [ecx + 3ch]
        jecxz  loc_21_00A2
loc_21_0097:
        cmp    dword ptr [ecx + 3ch], edi
        jne    SHORT loc_21_0092
        push   dword ptr [edi + 3ch]
        pop    dword ptr [ecx + 3ch]
loc_21_00A2:
        pop    ecx
        clc
        ret
loc_21_00A5:
        pop    ecx
        stc
        ret
OpenFile:
        enter  14h, 0
        pushad
        sub    esp, 6ch
        push   edi
        lea    edi, [esp + 4]
        db  0CDh, 020h, 08Dh, 000h, 001h, 000h
        pop    edi
        db  0CDh, 020h, 082h, 000h, 001h, 000h
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        mov    eax, dword ptr [ebx + 8]
        mov    dword ptr [ebp - 0ch], eax
        and    dword ptr [ebp - 10h], 0
        mov    dword ptr [ebp - 14h], 80h
        db  081h, 03Dh, 0DCh, 0C5h, 000h, 000h, 000h, 000h, 000h, 040h
        jb     SHORT loc_21_00EC
        mov    dword ptr [ebp - 14h], 104h
loc_21_00EC:
        db  081h, 03Dh, 0DCh, 0C5h, 000h, 000h, 000h, 000h, 000h, 010h
        ja     SHORT loc_21_00FF
        db  0E8h, 09Eh, 012h, 0EBh, 0FFh
        jmp    SHORT loc_21_010A
loc_21_00FF:
        call   sub_21_03C9
        jb     NEAR PTR loc_21_037F
loc_21_010A:
        mov    dword ptr [ebp - 4], eax
        or     eax, eax
        je     NEAR PTR loc_21_037F
        mov    esi, dword ptr [ebx + 4]
        add    dword ptr [ebp - 4], esi
        push   eax
        shr    eax, 4
        mov    word ptr [ebp - 6], ax
        pop    eax
        and    eax, 0fh
        mov    word ptr [ebp - 8], ax
        mov    esi, dword ptr [ebp - 20h]
        cmp    byte ptr [esi + 1], 3ah
        je     NEAR PTR loc_21_032E
        mov    ecx, 80h
        cld
loc_21_013E:
        lodsb
        or     al, al
        je     SHORT loc_21_0155
        cmp    al, 5ch
        je     NEAR PTR loc_21_032E
        cmp    al, 2fh
        je     NEAR PTR loc_21_032E
        loop   loc_21_013E
loc_21_0155:
        db  0BEh, 0B8h, 002h, 000h, 000h
        db  0CDh, 020h, 0B5h, 000h, 001h, 000h
        jb     SHORT loc_21_01A5
        mov    esi, edx
        mov    edi, dword ptr [ebp - 34h]
        xor    ecx, ecx
        cmp    byte ptr [esi], 0
        je     SHORT loc_21_01A5
        cld
loc_21_016F:
        lodsb
        cmp    al, 2fh
        jne    SHORT loc_21_0176
        mov    al, 5ch
loc_21_0176:
        inc    ecx
        cmp    dword ptr [ebp - 14h], ecx
        jb     SHORT loc_21_01A5
        stosb
        or     al, al
        jne    SHORT loc_21_016F
        dec    edi
        mov    al, 5ch
        inc    ecx
        cmp    dword ptr [ebp - 14h], ecx
        jb     SHORT loc_21_01A5
        stosb
        mov    esi, dword ptr [ebp - 20h]
loc_21_018E:
        lodsb
        inc    ecx
        cmp    dword ptr [ebp - 14h], ecx
        jb     SHORT loc_21_01A5
        stosb
        or     al, al
        jne    SHORT loc_21_018E
        call   sub_21_040F
        jae    NEAR PTR loc_21_03A3
loc_21_01A5:
        db  0CDh, 020h, 0B6h, 000h, 001h, 000h
        mov    esi, edx
        mov    edi, dword ptr [ebp - 34h]
        mov    ebx, ecx
        xor    ecx, ecx
        cld
loc_21_01B5:
        lodsb
        cmp    al, 2fh
        jne    SHORT loc_21_01BC
        mov    al, 5ch
loc_21_01BC:
        inc    ecx
        cmp    dword ptr [ebp - 14h], ecx
        jb     SHORT loc_21_01F6
        stosb
        dec    ebx
        jne    SHORT loc_21_01B5
        cmp    byte ptr [edi - 1], 3ah
        je     SHORT loc_21_01DB
        cmp    byte ptr [edi - 1], 5ch
        je     SHORT loc_21_01DB
        mov    al, 5ch
        inc    ecx
        cmp    dword ptr [ebp - 14h], ecx
        jb     SHORT loc_21_01F6
        stosb
loc_21_01DB:
        mov    esi, dword ptr [ebp - 20h]
        cld
loc_21_01DF:
        lodsb
        inc    ecx
        cmp    dword ptr [ebp - 14h], ecx
        jb     SHORT loc_21_01F6
        stosb
        or     al, al
        jne    SHORT loc_21_01DF
        call   sub_21_040F
        jae    NEAR PTR loc_21_03A3
loc_21_01F6:
        or     byte ptr [ebp - 10h], 2
        mov    edi, dword ptr [ebp - 34h]
        xor    ecx, ecx
        mov    esi, dword ptr [ebp - 4]
        db  0CDh, 020h, 016h, 000h, 02Ah, 000h
        or     eax, eax
        je     SHORT loc_21_0265
        and    byte ptr [ebp - 10h], 0fdh
        mov    ecx, dword ptr [ebp - 0ch]
        mov    word ptr [ecx + 1ch], 1900h
        mov    eax, 21h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        mov    al, byte ptr [ecx + 1ch]
        movzx  edx, al
        inc    dl
        add    al, 41h
        cld
        stosb
        mov    ax, 5c3ah
        stosw
        mov    ax, word ptr [ebp - 6]
        mov    word ptr [ecx + 3ch], ax
        mov    ax, word ptr [ebp - 8]
        mov    word ptr [ecx + 4], ax
        mov    byte ptr [ecx + 14h], dl
        mov    word ptr [ecx + 1ch], 4700h
        mov    eax, 21h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        test   byte ptr [ecx + 2ch], 1
        jne    SHORT loc_21_02A5
        mov    ecx, 3
loc_21_0265:
        mov    esi, dword ptr [ebp - 4]
        cld
loc_21_0269:
        lodsb
        inc    ecx
        cmp    dword ptr [ebp - 14h], ecx
        jb     SHORT loc_21_02A5
        stosb
        or     al, al
        jne    SHORT loc_21_0269
        dec    edi
        cmp    byte ptr [edi - 1], 3ah
        je     SHORT loc_21_028B
        cmp    byte ptr [edi - 1], 5ch
        je     SHORT loc_21_028B
        mov    al, 5ch
        inc    ecx
        cmp    dword ptr [ebp - 14h], ecx
        jb     SHORT loc_21_02A5
        stosb
loc_21_028B:
        mov    esi, dword ptr [ebp - 20h]
loc_21_028E:
        lodsb
        inc    ecx
        cmp    dword ptr [ebp - 14h], ecx
        jb     SHORT loc_21_02A5
        stosb
        or     al, al
        jne    SHORT loc_21_028E
        call   sub_21_040F
        jae    NEAR PTR loc_21_03A3
loc_21_02A5:
        db  0BEh, 0BFh, 002h, 000h, 000h
        db  0CDh, 020h, 0B5h, 000h, 001h, 000h
        jb     SHORT loc_21_0325
        mov    ebx, edx
loc_21_02B4:
        mov    esi, ebx
        mov    edi, dword ptr [ebp - 34h]
        xor    ecx, ecx
        cmp    byte ptr [esi], 0
        je     SHORT loc_21_0325
        cmp    byte ptr [esi], 9
        je     SHORT loc_21_0325
        cmp    byte ptr [esi], 20h
        je     SHORT loc_21_0325
        cld
loc_21_02CB:
        lodsb
        cmp    al, 2fh
        jne    SHORT loc_21_02D2
        mov    al, 5ch
loc_21_02D2:
        inc    ecx
        cmp    dword ptr [ebp - 14h], ecx
        jb     SHORT loc_21_0316
        stosb
        cmp    al, 3bh
        je     SHORT loc_21_02E2
        or     al, al
        jne    SHORT loc_21_02CB
        dec    esi
loc_21_02E2:
        dec    edi
        cmp    byte ptr [edi - 1], 3ah
        je     SHORT loc_21_02F8
        cmp    byte ptr [edi - 1], 5ch
        je     SHORT loc_21_02F8
        mov    al, 5ch
        inc    ecx
        cmp    dword ptr [ebp - 14h], ecx
        jb     SHORT loc_21_0316
        stosb
loc_21_02F8:
        mov    ebx, esi
        mov    esi, dword ptr [ebp - 20h]
loc_21_02FD:
        lodsb
        inc    ecx
        cmp    dword ptr [ebp - 14h], ecx
        jb     SHORT loc_21_0316
        stosb
        or     al, al
        jne    SHORT loc_21_02FD
        call   sub_21_040F
        jae    NEAR PTR loc_21_03A3
        jmp    SHORT loc_21_02B4
loc_21_0316:
        dec    esi
        mov    ebx, esi
loc_21_0319:
        lodsb
        or     al, al
        je     SHORT loc_21_02B4
        inc    ebx
        cmp    al, 3bh
        je     SHORT loc_21_02B4
        jmp    SHORT loc_21_0319
loc_21_0325:
        db  080h, 03Dh, 0A5h, 002h, 000h, 000h, 002h
        jb     SHORT loc_21_037F
loc_21_032E:
        mov    edi, dword ptr [ebp - 34h]
        xor    ecx, ecx
        mov    esi, dword ptr [ebp - 20h]
        cld
loc_21_0337:
        lodsb
        inc    ecx
        cmp    dword ptr [ebp - 14h], ecx
        jb     SHORT loc_21_037F
        stosb
        or     al, al
        jne    SHORT loc_21_0337
        mov    esi, dword ptr [ebp - 34h]
        mov    edi, dword ptr [ebp - 4]
        mov    ecx, 20h
        cld
        rep movsd
        mov    ecx, dword ptr [ebp - 0ch]
        mov    ax, word ptr [ebp - 6]
        mov    word ptr [ecx + 3ch], ax
        mov    ax, word ptr [ebp - 8]
        mov    word ptr [ecx + 14h], ax
        mov    word ptr [ecx + 1ch], 3da0h
        mov    eax, 21h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        movzx  eax, word ptr [ecx + 1ch]
        test   byte ptr [ecx + 2ch], 1
        je     SHORT loc_21_03A3
loc_21_037F:
        cmp    dword ptr [ebp - 4], 0
        je     SHORT loc_21_038A
        call   sub_21_03EA
loc_21_038A:
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        push   esi
        lea    esi, [esp + 4]
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        pop    esi
        add    esp, 6ch
        stc
loc_21_03A0:
        popad
        leave
        ret
loc_21_03A3:
        mov    dword ptr [ebp - 18h], eax
        cmp    dword ptr [ebp - 4], 0
        je     SHORT loc_21_03B1
        call   sub_21_03EA
loc_21_03B1:
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        push   esi
        lea    esi, [esp + 4]
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        pop    esi
        add    esp, 6ch
        clc
        jmp    SHORT loc_21_03A0
sub_21_03C9:
        push   ebp
        mov    eax, dword ptr [ebp - 0ch]
        mov    ebp, eax
        mov    ecx, 104h
        clc
        db  0CDh, 020h, 008h, 000h, 006h, 000h
        pop    ebp
        jb     SHORT loc_21_03E9
        movzx  eax, di
        shr    edi, 10h
        shl    edi, 4
        add    eax, edi
loc_21_03E9:
        ret
sub_21_03EA:
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        test   byte ptr [ebp - 10h], 1
        jne    SHORT loc_21_0408
        push   ebp
        mov    ebp, dword ptr [ebp - 0ch]
        mov    ecx, 104h
        clc
        db  0CDh, 020h, 009h, 000h, 006h, 000h
        pop    ebp
        ret
loc_21_0408:
        db  0CDh, 020h, 0AAh, 000h, 001h, 000h
        ret
sub_21_040F:
        push   esi
        push   edi
        push   ecx
        db  080h, 03Dh, 0A5h, 002h, 000h, 000h, 000h
        jne    SHORT loc_21_0441
        mov    ecx, dword ptr [ebp - 0ch]
        mov    word ptr [ecx + 1ch], 0dc00h
        mov    eax, 21h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        db  0FEh, 005h, 0A5h, 002h, 000h, 000h
        cmp    byte ptr [ecx + 1ch], 0
        je     SHORT loc_21_0441
        db  0FEh, 005h, 0A5h, 002h, 000h, 000h
loc_21_0441:
        mov    esi, dword ptr [ebp - 34h]
        mov    edi, dword ptr [ebp - 4]
        mov    ecx, 20h
        test   byte ptr [ebp - 10h], 2
        je     SHORT loc_21_0457
        mov    ecx, 41h
loc_21_0457:
        cld
        rep movsd
        mov    ecx, dword ptr [ebp - 0ch]
        mov    ax, word ptr [ebp - 6]
        mov    word ptr [ecx + 3ch], ax
        mov    word ptr [ecx + 38h], ax
        mov    ax, word ptr [ebp - 8]
        mov    word ptr [ecx + 14h], ax
        test   byte ptr [ebp - 10h], 2
        je     SHORT loc_21_049C
        mov    word ptr [ecx + 4], ax
        mov    word ptr [ecx], ax
        mov    word ptr [ecx + 18h], 1
        mov    word ptr [ecx + 1ch], 7160h
        mov    eax, 21h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        test   byte ptr [ecx + 2ch], 1
        stc
        jne    SHORT loc_21_04DA
loc_21_049C:
        db  080h, 03Dh, 0A5h, 002h, 000h, 000h, 002h
        jb     SHORT loc_21_04BD
        mov    word ptr [ecx + 1ch], 4300h
        mov    eax, 21h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        test   byte ptr [ecx + 2ch], 1
        stc
        jne    SHORT loc_21_04DA
loc_21_04BD:
        mov    word ptr [ecx + 1ch], 3da0h
        mov    eax, 21h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        test   byte ptr [ecx + 2ch], 1
        stc
        jne    SHORT loc_21_04DA
        movzx  eax, word ptr [ecx + 1ch]
        clc
loc_21_04DA:
        pop    ecx
        pop    edi
        pop    esi
        ret
Get_Exec_Path:
        db  08Bh, 015h, 058h, 001h, 000h, 000h
        db  08Bh, 00Dh, 05Ch, 001h, 000h, 000h
        ret
        db  000h
loc_21_04EC:
        push   ebx
        push   esi
        push   edi
        mov    ecx, dword ptr [esp + 10h]
        mov    eax, 1
        cmp    ecx, 7
        jae    SHORT loc_21_050B
        mov    esi, dword ptr [esp + 1ch]
        db  0FFh, 014h, 08Dh, 0FCh, 002h, 000h, 000h
loc_21_050B:
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  090h
sub_21_0510:
        push   ebx
        push   esi
        push   edi
        xor    bl, bl
        mov    bh, cl
        mov    esi, eax
        mov    edi, edx
        push   0
        db  0FFh, 035h, 0E4h, 002h, 000h, 000h
        push   edi
        push   esi
        db  0CDh, 020h, 026h, 000h, 033h, 000h
        add    esp, 10h
        or     eax, eax
        jne    SHORT loc_21_057B
        cmp    bh, 0ffh
        je     SHORT loc_21_057B
        push   0
        db  0FFh, 035h, 0DCh, 002h, 000h, 000h
        push   edi
        push   esi
        db  0CDh, 020h, 026h, 000h, 033h, 000h
        add    esp, 10h
        or     eax, eax
        jne    SHORT loc_21_057B
        cmp    esi, 100h
        jb     SHORT loc_21_0579
        or     bh, bh
        je     SHORT loc_21_057B
loc_21_055A:
        add    si, bx
        jb     SHORT loc_21_0579
        add    di, bx
        push   0
        db  0FFh, 035h, 0DCh, 002h, 000h, 000h
        push   edi
        push   esi
        db  0CDh, 020h, 026h, 000h, 033h, 000h
        add    esp, 10h
        or     eax, eax
        je     SHORT loc_21_055A
loc_21_0579:
        or     eax, eax
loc_21_057B:
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_21_057F:
        push   0
        db  0FFh, 035h, 0DCh, 002h, 000h, 000h
        db  0FFh, 035h, 0D0h, 002h, 000h, 000h
        db  0FFh, 035h, 0D8h, 002h, 000h, 000h
        db  0CDh, 020h, 062h, 000h, 033h, 000h
        add    esp, 10h
        or     eax, eax
        jne    NEAR PTR loc_21_067E
        push   0
        db  0FFh, 035h, 0E4h, 002h, 000h, 000h
        db  0FFh, 035h, 0D0h, 002h, 000h, 000h
        db  0FFh, 035h, 0E0h, 002h, 000h, 000h
        db  0CDh, 020h, 062h, 000h, 033h, 000h
        add    esp, 10h
        or     eax, eax
        jne    NEAR PTR loc_21_067E
        call   sub_21_0AC7
        jne    NEAR PTR loc_21_067E
        push   0
        db  0FFh, 035h, 0C4h, 002h, 000h, 000h
        db  0FFh, 035h, 0DCh, 002h, 000h, 000h
        db  0FFh, 035h, 0E4h, 002h, 000h, 000h
        db  0CDh, 020h, 062h, 000h, 033h, 000h
        add    esp, 10h
        or     eax, eax
        jne    NEAR PTR loc_21_067E
        call   sub_21_0B45
        db  0C6h, 005h, 01Ch, 003h, 000h, 000h, 000h
        call   sub_21_0680
        jb     SHORT loc_21_067E
        push   0
        db  0FFh, 035h, 0E8h, 002h, 000h, 000h
        db  0FFh, 035h, 0D0h, 002h, 000h, 000h
        db  0FFh, 035h, 0DCh, 002h, 000h, 000h
        db  0CDh, 020h, 063h, 000h, 033h, 000h
        add    esp, 10h
        or     eax, eax
        jne    SHORT loc_21_067E
        push   0
        db  0FFh, 035h, 0ECh, 002h, 000h, 000h
        db  0FFh, 035h, 0D0h, 002h, 000h, 000h
        db  0FFh, 035h, 0E4h, 002h, 000h, 000h
        db  0CDh, 020h, 063h, 000h, 033h, 000h
        add    esp, 10h
        or     eax, eax
        jne    SHORT loc_21_067E
        db  0FFh, 035h, 0E8h, 002h, 000h, 000h
        db  0FFh, 035h, 0DCh, 002h, 000h, 000h
        db  08Fh, 005h, 0E8h, 002h, 000h, 000h
        db  08Fh, 005h, 0DCh, 002h, 000h, 000h
        db  0FFh, 035h, 0ECh, 002h, 000h, 000h
        db  0FFh, 035h, 0E4h, 002h, 000h, 000h
        db  08Fh, 005h, 0ECh, 002h, 000h, 000h
        db  08Fh, 005h, 0E4h, 002h, 000h, 000h
loc_21_067E:
        ret
        db  090h
sub_21_0680:
        enter  18h, 0
        push   ebx
        push   esi
        push   edi
        call   sub_21_0A94
        je     NEAR PTR loc_21_0926
        mov    word ptr [ebp - 4], 0
        mov    word ptr [ebp - 8], 0
loc_21_069E:
        lea    esi, [edi + 0ch]
        movzx  eax, word ptr [edi]
        or     eax, eax
        je     NEAR PTR loc_21_0900
        mov    dword ptr [ebp - 14h], eax
loc_21_06AF:
        movzx  edx, word ptr [esi + 4]
        mov    ecx, 100h
        sub    edx, ecx
        cmc
        sbb    eax, eax
        and    edx, eax
        add    edx, ecx
        db  0A1h, 0E4h, 002h, 000h, 000h
        cmp    byte ptr [esi + 0ah], 0ffh
        je     SHORT loc_21_06D1
        db  0A1h, 0C4h, 002h, 000h, 000h
loc_21_06D1:
        mov    dword ptr [ebp - 18h], eax
        mov    dword ptr [ebp - 0ch], edx
loc_21_06D7:
        push   0
        push   dword ptr [ebp - 18h]
        movzx  eax, word ptr [esi + 6]
        push   eax
        movsx  eax, word ptr [esi]
        push   eax
        movzx  eax, word ptr [esi + 2]
        push   eax
        push   dword ptr [ebp - 0ch]
        lea    eax, [ebp - 0ch]
        push   eax
        db  0CDh, 020h, 056h, 000h, 033h, 000h
        add    esp, 1ch
        or     eax, eax
        jne    NEAR PTR loc_21_08CF
        mov    eax, dword ptr [ebp - 0ch]
        movzx  edx, word ptr [esi + 2]
        dec    edx
        add    edx, eax
        mov    dword ptr [ebp - 10h], edx
        cmp    dx, word ptr [esi + 6]
        jb     SHORT loc_21_0743
        cmp    ax, word ptr [esi + 4]
        ja     SHORT loc_21_0743
        cmp    word ptr [edi], 1
        ja     SHORT loc_21_0743
        push   0
        db  0FFh, 035h, 0CCh, 002h, 000h, 000h
        push   edx
        push   eax
        db  0CDh, 020h, 025h, 000h, 033h, 000h
        add    esp, 10h
        or     eax, eax
        jne    NEAR PTR loc_21_08F7
        mov    ecx, 0ffffffffh
        jmp    SHORT loc_21_075B
loc_21_0743:
        push   0
        db  0FFh, 035h, 0CCh, 002h, 000h, 000h
        push   edx
        push   eax
        db  0CDh, 020h, 027h, 000h, 033h, 000h
        add    esp, 10h
        cmp    eax, 1
        sbb    ecx, ecx
loc_21_075B:
        xor    cx, word ptr [ebp - 4]
        je     NEAR PTR loc_21_08EF
        movzx  ecx, word ptr [esi + 0ah]
        cmp    cl, 0ffh
        je     NEAR PTR loc_21_082A
        or     cl, cl
        je     NEAR PTR loc_21_082A
        dec    cl
        cmp    cl, ch
        jne    NEAR PTR loc_21_08CF
        inc    cl
        test   ch, cl
        jne    NEAR PTR loc_21_08CF
        mov    ch, cl
        xor    cl, cl
        cmp    cx, word ptr [ebp - 10h]
        jbe    NEAR PTR loc_21_08CF
        db  066h, 089h, 00Dh, 01Dh, 003h, 000h, 000h
loc_21_07A3:
        add    word ptr [ebp - 0ch], cx
        add    word ptr [ebp - 10h], cx
        jb     SHORT loc_21_07DA
        push   0
        db  0FFh, 035h, 0DCh, 002h, 000h, 000h
        push   dword ptr [ebp - 10h]
        push   dword ptr [ebp - 0ch]
        db  0CDh, 020h, 027h, 000h, 033h, 000h
        add    esp, 10h
        or     eax, eax
        db  066h, 08Bh, 00Dh, 01Dh, 003h, 000h, 000h
        je     SHORT loc_21_07A3
        dec    cx
        and    word ptr [ebp - 0ch], cx
        jmp    NEAR PTR loc_21_08C4
loc_21_07DA:
        add    word ptr [ebp - 0ch], cx
        add    word ptr [ebp - 10h], cx
        jb     SHORT loc_21_082A
        push   1
        db  0FFh, 035h, 0DCh, 002h, 000h, 000h
        push   dword ptr [ebp - 10h]
        push   dword ptr [ebp - 0ch]
        db  0CDh, 020h, 025h, 000h, 033h, 000h
        add    esp, 10h
        or     eax, eax
        jne    NEAR PTR loc_21_08F7
        push   0
        db  0FFh, 035h, 0C4h, 002h, 000h, 000h
        push   dword ptr [ebp - 10h]
        push   dword ptr [ebp - 0ch]
        db  0CDh, 020h, 025h, 000h, 033h, 000h
        add    esp, 10h
        or     eax, eax
        db  066h, 08Bh, 00Dh, 01Dh, 003h, 000h, 000h
        je     SHORT loc_21_07DA
        jmp    NEAR PTR loc_21_08F7
loc_21_082A:
        push   0
        db  0FFh, 035h, 0DCh, 002h, 000h, 000h
        push   dword ptr [ebp - 10h]
        push   dword ptr [ebp - 0ch]
        db  0CDh, 020h, 025h, 000h, 033h, 000h
        add    esp, 10h
        or     eax, eax
        jne    NEAR PTR loc_21_08F7
        push   0
        db  0FFh, 035h, 0C4h, 002h, 000h, 000h
        push   dword ptr [ebp - 10h]
        push   dword ptr [ebp - 0ch]
        db  0CDh, 020h, 025h, 000h, 033h, 000h
        add    esp, 10h
        or     eax, eax
        jne    NEAR PTR loc_21_08F7
        push   0
        db  0FFh, 035h, 0E4h, 002h, 000h, 000h
        push   dword ptr [ebp - 10h]
        push   dword ptr [ebp - 0ch]
        db  0CDh, 020h, 025h, 000h, 033h, 000h
        add    esp, 10h
        or     eax, eax
        jne    SHORT loc_21_08F7
        call   sub_21_0680
        jae    SHORT loc_21_0908
        db  080h, 03Dh, 01Ch, 003h, 000h, 000h, 000h
        jne    SHORT loc_21_0905
        mov    eax, dword ptr [ebp - 0ch]
        mov    edx, dword ptr [ebp - 10h]
        mov    cl, byte ptr [esi + 0ah]
        call   sub_21_0510
        jne    SHORT loc_21_08F7
        push   0
        db  0FFh, 035h, 0C4h, 002h, 000h, 000h
        db  0FFh, 035h, 0DCh, 002h, 000h, 000h
        db  0FFh, 035h, 0E4h, 002h, 000h, 000h
        db  0CDh, 020h, 062h, 000h, 033h, 000h
        add    esp, 10h
        or     eax, eax
        jne    SHORT loc_21_08F7
loc_21_08C4:
        inc    dword ptr [ebp - 0ch]
        jne    NEAR PTR loc_21_06D7
        jmp    SHORT loc_21_08F7
loc_21_08CF:
        add    esi, 0ch
        dec    dword ptr [ebp - 14h]
        jne    NEAR PTR loc_21_06AF
        cmp    word ptr [ebp - 8], 0
        je     SHORT loc_21_0900
        xor    word ptr [ebp - 4], 0ffffh
        jne    NEAR PTR loc_21_069E
        jmp    SHORT loc_21_0900
loc_21_08EF:
        mov    word ptr [ebp - 8], 1
        jmp    SHORT loc_21_08C4
loc_21_08F7:
        db  0C6h, 005h, 01Ch, 003h, 000h, 000h, 0FFh
        jmp    SHORT loc_21_0905
loc_21_0900:
        mov    eax, 13h
loc_21_0905:
        stc
        jmp    SHORT loc_21_0927
loc_21_0908:
        mov    eax, dword ptr [ebp - 0ch]
        mov    word ptr [edi + 4], ax
        mov    eax, dword ptr [ebp - 10h]
        mov    word ptr [edi + 6], ax
        mov    ax, word ptr [esi + 0ah]
        mov    word ptr [edi + 0ah], ax
        mov    ax, word ptr [esi + 8]
        mov    word ptr [edi + 8], ax
loc_21_0926:
        clc
loc_21_0927:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_21_092C:
        db  0C7h, 005h, 018h, 003h, 000h, 000h, 001h, 000h, 000h, 000h
loc_21_0936:
        push   0
        db  0FFh, 035h, 0DCh, 002h, 000h, 000h
        db  0FFh, 035h, 0D8h, 002h, 000h, 000h
        db  0CDh, 020h, 028h, 000h, 033h, 000h
        add    esp, 0ch
        or     eax, eax
        jne    SHORT loc_21_09CA
        push   0
        db  0FFh, 035h, 0E4h, 002h, 000h, 000h
        db  0FFh, 035h, 0E0h, 002h, 000h, 000h
        db  0CDh, 020h, 028h, 000h, 033h, 000h
        add    esp, 0ch
        or     eax, eax
        jne    SHORT loc_21_09CA
        call   sub_21_0AC7
        jne    SHORT loc_21_09CA
loc_21_0973:
        call   sub_21_0A94
        je     SHORT loc_21_09CA
        mov    cx, word ptr [edi + 0ah]
        movzx  eax, word ptr [edi + 4]
        movzx  edx, word ptr [edi + 6]
        cmp    cl, 0ffh
        je     SHORT loc_21_09D1
        db  0FFh, 035h, 0E4h, 002h, 000h, 000h
        db  0FFh, 035h, 0DCh, 002h, 000h, 000h
        call   sub_21_09F0
        jne    SHORT loc_21_09CA
        cmp    ah, 1
        jb     SHORT loc_21_0973
        or     cl, cl
        je     SHORT loc_21_0973
        dec    cl
        cmp    cl, ch
        inc    cl
        test   ch, cl
        jne    SHORT loc_21_09CB
        cmp    cl, dh
        jbe    SHORT loc_21_0973
loc_21_09B5:
        add    ah, cl
        jb     SHORT loc_21_0973
        add    dh, cl
        push   0
        db  0FFh, 035h, 0DCh, 002h, 000h, 000h
        call   sub_21_09F0
        je     SHORT loc_21_09B5
loc_21_09CA:
        ret
loc_21_09CB:
        mov    eax, 13h
        ret
loc_21_09D1:
        push   0
        db  0FFh, 035h, 0E4h, 002h, 000h, 000h
        call   sub_21_09F0
        je     SHORT loc_21_0973
        ret
loc_21_09E1:
        db  0C7h, 005h, 018h, 003h, 000h, 000h, 000h, 000h, 000h, 000h
        jmp    NEAR PTR loc_21_0936
sub_21_09F0:
        push   eax
        push   ecx
        push   edx
        push   edi
        mov    esi, eax
        mov    edi, edx
        db  0FFh, 035h, 018h, 003h, 000h, 000h
        push   dword ptr [esp + 18h]
        push   edi
        push   esi
        db  0CDh, 020h, 025h, 000h, 033h, 000h
        add    esp, 10h
        or     eax, eax
        jne    SHORT loc_21_0A89
        push   0
        db  0FFh, 035h, 0D4h, 002h, 000h, 000h
        push   edi
        push   esi
        db  0CDh, 020h, 026h, 000h, 033h, 000h
        add    esp, 10h
        or     eax, eax
        jne    SHORT loc_21_0A89
        cmp    dword ptr [esp + 18h], 0
        je     SHORT loc_21_0A89
        db  0FFh, 035h, 018h, 003h, 000h, 000h
        push   dword ptr [esp + 1ch]
        push   edi
        push   esi
        db  0CDh, 020h, 025h, 000h, 033h, 000h
        add    esp, 10h
        or     eax, eax
        jne    SHORT loc_21_0A89
        push   3
        push   dword ptr [ebx + 8]
        push   0
        db  0CDh, 020h, 01Ah, 000h, 033h, 000h
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_21_0A74
        push   4
        push   dword ptr [ebx + 8]
        push   0
        db  0CDh, 020h, 01Ah, 000h, 033h, 000h
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_21_0A74
        xor    eax, eax
        jmp    SHORT loc_21_0A89
loc_21_0A74:
        push   0
        db  0FFh, 035h, 0CCh, 002h, 000h, 000h
        push   edi
        push   esi
        db  0CDh, 020h, 025h, 000h, 033h, 000h
        add    esp, 10h
        or     eax, eax
loc_21_0A89:
        pop    edi
        pop    edx
        pop    ecx
        pop    esi
        jne    SHORT loc_21_0A91
        mov    eax, esi
loc_21_0A91:
        ret    8
sub_21_0A94:
        push   0
        push   0
        push   2
        push   edi
        db  068h, 0F0h, 002h, 000h, 000h
        db  0CDh, 020h, 01Fh, 000h, 033h, 000h
        add    esp, 14h
        cmp    eax, 0
        jne    SHORT loc_21_0AB7
        db  08Bh, 03Dh, 0F0h, 002h, 000h, 000h
        or     edi, edi
loc_21_0AB6:
        ret
loc_21_0AB7:
        mov    ebx, dword ptr [ebx]
        or     ebx, ebx
        mov    eax, 0
        je     SHORT loc_21_0AB6
        mov    edi, dword ptr [ebx + 0ch]
        jmp    SHORT sub_21_0A94
sub_21_0AC7:
        mov    ebx, dword ptr [esi]
        mov    edi, dword ptr [ebx + 0ch]
        pushad
        push   0
        db  0FFh, 035h, 0D4h, 002h, 000h, 000h
        db  0FFh, 035h, 0D0h, 002h, 000h, 000h
        db  0CDh, 020h, 028h, 000h, 033h, 000h
        add    esp, 0ch
        or     eax, eax
        jne    SHORT loc_21_0B3F
loc_21_0AE8:
        push   2
        push   dword ptr [ebx + 8]
        db  068h, 0F0h, 002h, 000h, 000h
        db  0CDh, 020h, 01Ah, 000h, 033h, 000h
        add    esp, 0ch
        or     eax, eax
        jne    SHORT loc_21_0B37
loc_21_0AFF:
        push   0
        push   0
        push   2
        db  0FFh, 035h, 0F0h, 002h, 000h, 000h
        db  068h, 0F0h, 002h, 000h, 000h
        db  0CDh, 020h, 01Fh, 000h, 033h, 000h
        add    esp, 14h
        or     eax, eax
        jne    SHORT loc_21_0B37
        db  08Bh, 03Dh, 0F0h, 002h, 000h, 000h
        movzx  eax, word ptr [edi + 4]
        movzx  edx, word ptr [edi + 6]
        mov    cl, byte ptr [edi + 0ah]
        call   sub_21_0510
        je     SHORT loc_21_0AFF
        jmp    SHORT loc_21_0B3F
loc_21_0B37:
        mov    ebx, dword ptr [ebx]
        or     ebx, ebx
        jne    SHORT loc_21_0AE8
        xor    eax, eax
loc_21_0B3F:
        mov    dword ptr [esp + 1ch], eax
        popad
        ret
sub_21_0B45:
        cmp    dword ptr [ebx], 0
        jne    SHORT loc_21_0B4B
        ret
loc_21_0B4B:
        push   esi
loc_21_0B4C:
        xor    eax, eax
        db  0A3h, 0F4h, 002h, 000h, 000h
        db  0A3h, 0F8h, 002h, 000h, 000h
        mov    edi, dword ptr [ebx + 0ch]
loc_21_0B5B:
        push   0
        push   0
        push   2
        push   edi
        db  068h, 0F0h, 002h, 000h, 000h
        db  0CDh, 020h, 01Fh, 000h, 033h, 000h
        add    esp, 14h
        cmp    eax, 0
        jne    SHORT loc_21_0BAD
        db  08Bh, 03Dh, 0F0h, 002h, 000h, 000h
        lea    esi, [edi + 0ch]
        movzx  ecx, word ptr [edi]
        jecxz  loc_21_0BAD
        db  0FFh, 005h, 0F8h, 002h, 000h, 000h
loc_21_0B89:
        movzx  eax, word ptr [esi + 6]
        sub    ax, word ptr [esi + 4]
        inc    ax
        xor    dx, dx
        cmp    dx, word ptr [esi + 2]
        je     SHORT loc_21_0BA6
        div    word ptr [esi + 2]
        db  001h, 005h, 0F4h, 002h, 000h, 000h
loc_21_0BA6:
        add    esi, 0ch
        loop   loc_21_0B89
        jmp    SHORT loc_21_0B5B
loc_21_0BAD:
        db  0A1h, 0F4h, 002h, 000h, 000h
        db  08Bh, 00Dh, 0F8h, 002h, 000h, 000h
        jecxz  loc_21_0BBE
        xor    edx, edx
        div    ecx
loc_21_0BBE:
        mov    dword ptr [ebx + 10h], eax
        mov    ebx, dword ptr [ebx]
        or     ebx, ebx
        jne    SHORT loc_21_0B4C
        pop    esi
        push   0
        push   esi
        db  0CDh, 020h, 014h, 000h, 033h, 000h
        pop    ecx
        pop    ecx
        mov    ebx, dword ptr [esi]
        mov    edi, dword ptr [ebx + 0ch]
        ret
loc_21_0BD9:
        db  0A1h, 0D4h, 002h, 000h, 000h
        db  087h, 005h, 0D0h, 002h, 000h, 000h
        db  0A3h, 0D4h, 002h, 000h, 000h
        db  0A1h, 0DCh, 002h, 000h, 000h
        db  087h, 005h, 0D8h, 002h, 000h, 000h
        db  0A3h, 0DCh, 002h, 000h, 000h
        db  0A1h, 0E4h, 002h, 000h, 000h
        db  087h, 005h, 0E0h, 002h, 000h, 000h
        db  0A3h, 0E4h, 002h, 000h, 000h
loc_21_0C09:
        push   0
        db  0FFh, 035h, 0C4h, 002h, 000h, 000h
        push   -1
        push   0
        db  0CDh, 020h, 026h, 000h, 033h, 000h
        add    esp, 10h
        push   0
        db  0FFh, 035h, 0D4h, 002h, 000h, 000h
        push   -1
        push   0
        db  0CDh, 020h, 026h, 000h, 033h, 000h
        add    esp, 10h
        push   0
        db  0FFh, 035h, 0DCh, 002h, 000h, 000h
        push   -1
        push   0
        db  0CDh, 020h, 026h, 000h, 033h, 000h
        add    esp, 10h
        push   0
        db  0FFh, 035h, 0E4h, 002h, 000h, 000h
        push   -1
        push   0
        db  0CDh, 020h, 026h, 000h, 033h, 000h
        add    esp, 10h
        push   0
        db  0FFh, 035h, 0E8h, 002h, 000h, 000h
        push   -1
        push   0
        db  0CDh, 020h, 026h, 000h, 033h, 000h
        add    esp, 10h
        push   0
        db  0FFh, 035h, 0ECh, 002h, 000h, 000h
        push   -1
        push   0
        db  0CDh, 020h, 026h, 000h, 033h, 000h
        add    esp, 10h
        xor    eax, eax
        ret
loc_21_0C8A:
        push   0
        db  0FFh, 035h, 0E8h, 002h, 000h, 000h
        db  0FFh, 035h, 0E0h, 002h, 000h, 000h
        db  0FFh, 035h, 0D8h, 002h, 000h, 000h
        db  0CDh, 020h, 062h, 000h, 033h, 000h
        add    esp, 10h
        or     eax, eax
        jne    SHORT loc_21_0CEE
        push   0
        db  0FFh, 035h, 0C8h, 002h, 000h, 000h
        db  0FFh, 035h, 0D0h, 002h, 000h, 000h
        db  0FFh, 035h, 0E8h, 002h, 000h, 000h
        db  0CDh, 020h, 062h, 000h, 033h, 000h
        add    esp, 10h
        or     eax, eax
        jne    SHORT loc_21_0CEE
        push   0
        db  0FFh, 035h, 0E8h, 002h, 000h, 000h
        push   -1
        push   0
        db  0CDh, 020h, 026h, 000h, 033h, 000h
        add    esp, 10h
        db  0C7h, 006h, 0C8h, 002h, 000h, 000h
        mov    dword ptr [esi + 4], 4
loc_21_0CEE:
        ret
loc_21_0CEF:
        xor    eax, eax
        ret
sub_21_0CF2:
        cmp    dh, 1
        jb     SHORT loc_21_0D44
        pushad
        mov    edi, edx
        db  0CDh, 020h, 000h, 000h, 033h, 000h
        jb     SHORT loc_21_0D43
        push   0
        db  0FFh, 035h, 0D8h, 002h, 000h, 000h
        push   edi
        push   edi
        db  0CDh, 020h, 027h, 000h, 033h, 000h
        add    esp, 10h
        or     eax, eax
        jne    SHORT loc_21_0D43
        push   0
        db  0FFh, 035h, 0E0h, 002h, 000h, 000h
        push   edi
        push   edi
        db  0CDh, 020h, 027h, 000h, 033h, 000h
        add    esp, 10h
        or     eax, eax
        jne    SHORT loc_21_0D43
        push   0
        db  0FFh, 035h, 0D0h, 002h, 000h, 000h
        push   edi
        push   edi
        db  0CDh, 020h, 025h, 000h, 033h, 000h
        add    esp, 10h
loc_21_0D43:
        popad
loc_21_0D44:
        ret
sub_21_0D45:
        pushad
        mov    edi, edx
        db  0CDh, 020h, 000h, 000h, 033h, 000h
        jb     SHORT loc_21_0D63
        push   0
        db  0FFh, 035h, 0D0h, 002h, 000h, 000h
        push   edi
        push   edi
        db  0CDh, 020h, 026h, 000h, 033h, 000h
        add    esp, 10h
loc_21_0D63:
        popad
        ret
loc_21_0D65:
        cmp    dh, 1
        jb     SHORT loc_21_0DC8
        movzx  esi, dx
        shr    edx, 10h
        mov    edi, esi
        cmp    edx, 4
        jbe    SHORT loc_21_0D7F
        inc    edi
        cmp    edx, 0ch
        jbe    SHORT loc_21_0D7F
        inc    edi
        inc    edi
loc_21_0D7F:
        db  0CDh, 020h, 000h, 000h, 033h, 000h
        jb     SHORT loc_21_0DC8
        push   0
        db  0FFh, 035h, 0D8h, 002h, 000h, 000h
        push   edi
        push   esi
        db  0CDh, 020h, 027h, 000h, 033h, 000h
        add    esp, 10h
        or     eax, eax
        jne    SHORT loc_21_0DC8
        push   0
        db  0FFh, 035h, 0E0h, 002h, 000h, 000h
        push   edi
        push   esi
        db  0CDh, 020h, 027h, 000h, 033h, 000h
        add    esp, 10h
        or     eax, eax
        jne    SHORT loc_21_0DC8
        push   0
        db  0FFh, 035h, 0D0h, 002h, 000h, 000h
        push   edi
        push   esi
        db  0CDh, 020h, 025h, 000h, 033h, 000h
        add    esp, 10h
loc_21_0DC8:
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_21_0DD0:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   esi
        cmp    dword ptr [ebp + 8], 218h
        jne    SHORT loc_21_0E33
        cmp    dword ptr [ebp + 0ch], 0ah
        jne    SHORT loc_21_0E33
        mov    al, byte ptr [ebp + 10h]
        or     al, al
        je     SHORT loc_21_0E13
        cmp    al, 2
        je     SHORT loc_21_0E13
        db  080h, 03Dh, 064h, 003h, 000h, 000h, 000h
        jne    SHORT loc_21_0E33
        db  0C7h, 045h, 0F8h, 043h, 055h, 000h, 000h
        mov    esi, dword ptr [ebp - 8]
        db  0CDh, 020h, 03Ah, 000h, 001h, 000h
        db  0C6h, 005h, 064h, 003h, 000h, 000h, 001h
        jmp    SHORT loc_21_0E33
loc_21_0E13:
        db  080h, 03Dh, 064h, 003h, 000h, 000h, 000h
        je     SHORT loc_21_0E33
        db  0C7h, 045h, 0FCh, 043h, 055h, 000h, 000h
        mov    esi, dword ptr [ebp - 4]
        db  0CDh, 020h, 02Bh, 001h, 001h, 000h
        db  0C6h, 005h, 064h, 003h, 000h, 000h, 000h
loc_21_0E33:
        mov    eax, 1
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
        db  000h, 000h, 000h
loc_21_0E40:
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 10h]
        mov    eax, 1
        cmp    esi, 7
        ja     SHORT loc_21_0E5F
        mov    ecx, dword ptr [esp + 1ch]
        db  0FFh, 014h, 0B5h, 0A4h, 003h, 000h, 000h
loc_21_0E5F:
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_21_0E63:
        mov    ebx, dword ptr [ecx]
        push   ecx
        call   sub_21_12BB
        push   0
        db  0FFh, 035h, 088h, 003h, 000h, 000h
        db  0FFh, 035h, 084h, 003h, 000h, 000h
        db  0CDh, 020h, 028h, 000h, 033h, 000h
        add    esp, 0ch
        cmp    eax, 0
        jne    SHORT loc_21_0ED0
        push   0
        db  0FFh, 035h, 098h, 003h, 000h, 000h
        db  0FFh, 035h, 094h, 003h, 000h, 000h
        db  0CDh, 020h, 028h, 000h, 033h, 000h
        add    esp, 0ch
        cmp    eax, 0
        jne    SHORT loc_21_0ED0
        pop    ecx
        call   sub_21_11FC
        cmp    eax, 0
        jne    SHORT loc_21_0ED1
        mov    edi, dword ptr [ebx + 0ch]
        call   sub_21_11C8
        mov    eax, 0
        je     SHORT loc_21_0ED1
        pushad
        push   -1
        call   sub_21_0ED8
        pop    ecx
        popad
        jae    SHORT loc_21_0ED1
        push   0
        call   sub_21_0ED8
loc_21_0ED0:
        pop    ecx
loc_21_0ED1:
        db  0A3h, 0C0h, 003h, 000h, 000h
        ret
        db  090h
sub_21_0ED8:
        enter  14h, 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 4], eax
        mov    eax, 0ffffffffh
        mov    dword ptr [ebp - 8], eax
loc_21_0EEA:
        lea    esi, [edi + 10h]
        movzx  ecx, word ptr [edi]
        jecxz  loc_21_0F2B
        mov    dword ptr [ebp - 14h], ecx
loc_21_0EF5:
        mov    eax, dword ptr [esi + 8]
        mov    dword ptr [ebp - 0ch], eax
loc_21_0EFB:
        lea    eax, [ebp - 0ch]
        push   0
        db  0FFh, 035h, 088h, 003h, 000h, 000h
        push   dword ptr [esi + 0ch]
        push   dword ptr [esi]
        push   dword ptr [esi + 4]
        push   dword ptr [ebp - 0ch]
        push   eax
        db  0CDh, 020h, 056h, 000h, 033h, 000h
        add    esp, 1ch
        cmp    eax, 0
        je     SHORT loc_21_0F33
        cmp    eax, 13h
        jne    NEAR PTR loc_21_0FDE
        jmp    SHORT loc_21_0F67
loc_21_0F2B:
        leave
        mov    eax, 13h
        stc
        ret
loc_21_0F33:
        mov    eax, dword ptr [ebp - 0ch]
        mov    edx, dword ptr [esi + 4]
        dec    edx
        add    edx, eax
        mov    dword ptr [ebp - 10h], edx
        call   sub_21_126B
        jb     SHORT loc_21_0F60
        push   1
        db  0FFh, 035h, 088h, 003h, 000h, 000h
        push   edx
        push   eax
        db  0CDh, 020h, 025h, 000h, 033h, 000h
        add    esp, 10h
        cmp    eax, 0
        je     SHORT loc_21_0FA2
        jmp    SHORT loc_21_0FDE
loc_21_0F60:
        inc    dword ptr [ebp - 0ch]
        jne    SHORT loc_21_0EFB
        jmp    SHORT loc_21_0F67
loc_21_0F67:
        add    esi, 14h
        dec    dword ptr [ebp - 14h]
        jne    SHORT loc_21_0EF5
        sub    esi, 14h
        test   dword ptr [ebp + 8], 0ffffffffh
        jne    SHORT loc_21_0F2B
        xor    dword ptr [ebp - 4], 0ffffffffh
        je     SHORT loc_21_0F87
        xor    dword ptr [ebp - 8], 0ffffffffh
        jne    SHORT loc_21_0F2B
loc_21_0F87:
        mov    eax, dword ptr [esi + 8]
        add    eax, dword ptr [esi + 4]
        cmp    eax, dword ptr [esi + 0ch]
        jbe    NEAR PTR loc_21_0EEA
        cmp    word ptr [esi], 1
        ja     NEAR PTR loc_21_0EEA
        jmp    SHORT loc_21_0F2B
loc_21_0FA2:
        push   ebx
        push   esi
        push   edi
        call   sub_21_11C8
        je     SHORT loc_21_0FE1
        push   dword ptr [ebp - 8]
        call   sub_21_0ED8
        lea    esp, [esp + 4]
        jae    SHORT loc_21_0FE1
        pop    edi
        pop    esi
        pop    ebx
        cmp    eax, 13h
        jne    SHORT loc_21_0FDE
        push   0
        db  0FFh, 035h, 088h, 003h, 000h, 000h
        push   dword ptr [ebp - 10h]
        push   dword ptr [ebp - 0ch]
        db  0CDh, 020h, 026h, 000h, 033h, 000h
        add    esp, 10h
        cmp    eax, 0
        je     SHORT loc_21_0F60
loc_21_0FDE:
        leave
        stc
        ret
loc_21_0FE1:
        pop    edi
        pop    esi
        pop    ebx
        mov    eax, dword ptr [ebp - 0ch]
        call   sub_21_11B3
        leave
        mov    eax, 0
        clc
        ret
loc_21_0FF4:
        db  0C7h, 005h, 0C4h, 003h, 000h, 000h, 000h, 000h, 000h, 000h
loc_21_0FFE:
        mov    ebx, dword ptr [ecx]
        push   0
        db  0FFh, 035h, 088h, 003h, 000h, 000h
        db  0FFh, 035h, 084h, 003h, 000h, 000h
        db  0CDh, 020h, 028h, 000h, 033h, 000h
        add    esp, 0ch
        cmp    eax, 0
        jne    NEAR PTR loc_21_119E
        push   0
        db  0FFh, 035h, 098h, 003h, 000h, 000h
        db  0FFh, 035h, 094h, 003h, 000h, 000h
        db  0CDh, 020h, 028h, 000h, 033h, 000h
        add    esp, 0ch
        cmp    eax, 0
        jne    NEAR PTR loc_21_119E
        call   sub_21_11FC
        cmp    eax, 0
        jne    NEAR PTR loc_21_119E
        mov    edi, dword ptr [ebx + 0ch]
loc_21_1051:
        call   sub_21_11C8
        je     NEAR PTR loc_21_1199
        push   0
        db  0FFh, 035h, 090h, 003h, 000h, 000h
        push   dword ptr [edi + 8]
        push   dword ptr [edi + 4]
        db  0CDh, 020h, 025h, 000h, 033h, 000h
        add    esp, 10h
        push   0
        db  0FFh, 035h, 088h, 003h, 000h, 000h
        push   dword ptr [edi + 8]
        push   dword ptr [edi + 4]
        db  0CDh, 020h, 027h, 000h, 033h, 000h
        add    esp, 10h
        cmp    eax, 0
        je     NEAR PTR loc_21_1160
        cmp    eax, 13h
        jne    NEAR PTR loc_21_119E
        db  083h, 03Dh, 0C4h, 003h, 000h, 000h, 000h
        jne    NEAR PTR loc_21_1160
        push   0
        db  0FFh, 035h, 09Ch, 003h, 000h, 000h
        push   -1
        push   0
        db  0CDh, 020h, 026h, 000h, 033h, 000h
        add    esp, 10h
        push   0
        db  0FFh, 035h, 09Ch, 003h, 000h, 000h
        push   dword ptr [edi + 8]
        push   dword ptr [edi + 4]
        db  0CDh, 020h, 025h, 000h, 033h, 000h
        add    esp, 10h
        cmp    eax, 0
        jne    NEAR PTR loc_21_119E
        push   0
        db  0FFh, 035h, 0A0h, 003h, 000h, 000h
        db  0FFh, 035h, 098h, 003h, 000h, 000h
        db  0FFh, 035h, 09Ch, 003h, 000h, 000h
        db  0CDh, 020h, 02Bh, 000h, 033h, 000h
        add    esp, 10h
        cmp    eax, 0
        jne    NEAR PTR loc_21_119E
        push   0
        push   -1
        db  0FFh, 035h, 09Ch, 003h, 000h, 000h
        db  0FFh, 035h, 0A0h, 003h, 000h, 000h
        db  0CDh, 020h, 02Ah, 000h, 033h, 000h
        add    esp, 10h
        cmp    eax, 0
        jne    SHORT loc_21_119E
        push   0
        db  0FFh, 035h, 0A0h, 003h, 000h, 000h
        db  0FFh, 035h, 088h, 003h, 000h, 000h
        db  0FFh, 035h, 09Ch, 003h, 000h, 000h
        db  0CDh, 020h, 02Bh, 000h, 033h, 000h
        add    esp, 10h
        cmp    eax, 0
        jne    SHORT loc_21_119E
        push   0
        db  0FFh, 035h, 0A0h, 003h, 000h, 000h
        push   dword ptr [edi + 8]
        push   dword ptr [edi + 4]
        db  0CDh, 020h, 027h, 000h, 033h, 000h
        add    esp, 10h
        cmp    eax, 0
        jne    SHORT loc_21_119E
loc_21_1160:
        push   0
        db  0FFh, 035h, 098h, 003h, 000h, 000h
        push   dword ptr [edi + 8]
        push   dword ptr [edi + 4]
        db  0CDh, 020h, 026h, 000h, 033h, 000h
        add    esp, 10h
        push   0
        db  0FFh, 035h, 088h, 003h, 000h, 000h
        push   dword ptr [edi + 8]
        push   dword ptr [edi + 4]
        db  0CDh, 020h, 025h, 000h, 033h, 000h
        add    esp, 10h
        cmp    eax, 0
        je     NEAR PTR loc_21_1051
        jmp    SHORT loc_21_119E
loc_21_1199:
        mov    eax, 0
loc_21_119E:
        db  0A3h, 0C0h, 003h, 000h, 000h
        ret
loc_21_11A4:
        db  0C7h, 005h, 0C4h, 003h, 000h, 000h, 0FFh, 0FFh, 0FFh, 0FFh
        jmp    NEAR PTR loc_21_0FFE
sub_21_11B3:
        mov    dword ptr [edi + 4], eax
        add    eax, dword ptr [esi + 4]
        dec    eax
        mov    dword ptr [edi + 8], eax
        mov    ax, word ptr [esi + 10h]
        mov    word ptr [edi + 0ch], ax
        ret
        db  08Bh, 0C0h
sub_21_11C8:
        push   0
        push   0
        push   1
        push   edi
        db  068h, 07Ch, 003h, 000h, 000h
        db  0CDh, 020h, 01Fh, 000h, 033h, 000h
        add    esp, 14h
        cmp    eax, 0
        jne    SHORT loc_21_11F1
        db  08Bh, 03Dh, 07Ch, 003h, 000h, 000h
        lea    esi, [edi + 10h]
        movzx  ecx, word ptr [edi]
        or     edi, edi
loc_21_11F0:
        ret
loc_21_11F1:
        mov    ebx, dword ptr [ebx]
        or     ebx, ebx
        je     SHORT loc_21_11F0
        mov    edi, dword ptr [ebx + 0ch]
        jmp    SHORT sub_21_11C8
sub_21_11FC:
        pushad
loc_21_11FD:
        push   2
        push   dword ptr [ebx + 8]
        db  068h, 07Ch, 003h, 000h, 000h
        db  0CDh, 020h, 01Ah, 000h, 033h, 000h
        add    esp, 0ch
        cmp    eax, 0
        jne    SHORT loc_21_125A
loc_21_1215:
        db  0A1h, 07Ch, 003h, 000h, 000h
        push   0
        push   0
        push   1
        push   eax
        db  068h, 07Ch, 003h, 000h, 000h
        db  0CDh, 020h, 01Fh, 000h, 033h, 000h
        add    esp, 14h
        cmp    eax, 0
        jne    SHORT loc_21_125A
        db  08Bh, 03Dh, 07Ch, 003h, 000h, 000h
        mov    ecx, dword ptr [edi + 4]
        mov    edx, dword ptr [edi + 8]
        push   0
        db  0FFh, 035h, 088h, 003h, 000h, 000h
        push   edx
        push   ecx
        db  0CDh, 020h, 026h, 000h, 033h, 000h
        add    esp, 10h
        cmp    eax, 0
        jne    SHORT loc_21_1265
        jmp    SHORT loc_21_1215
loc_21_125A:
        mov    ebx, dword ptr [ebx]
        or     ebx, ebx
        jne    SHORT loc_21_11FD
        mov    eax, 0
loc_21_1265:
        mov    dword ptr [esp + 1ch], eax
        popad
        ret
sub_21_126B:
        pushad
        cmp    word ptr [edi], 1
        ja     SHORT loc_21_127C
        cmp    edx, dword ptr [esi + 0ch]
        jb     SHORT loc_21_127C
        cmp    eax, dword ptr [esi + 8]
        je     SHORT loc_21_12A6
loc_21_127C:
        push   0
        db  0FFh, 035h, 090h, 003h, 000h, 000h
        push   edx
        push   eax
        db  0CDh, 020h, 027h, 000h, 033h, 000h
        add    esp, 10h
        cmp    eax, 13h
        jne    SHORT loc_21_129D
        cmp    dword ptr [ebp - 4], 0
        je     SHORT loc_21_12A3
loc_21_129A:
        popad
        stc
        ret
loc_21_129D:
        cmp    dword ptr [ebp - 4], 0
        je     SHORT loc_21_129A
loc_21_12A3:
        popad
        clc
        ret
loc_21_12A6:
        push   0
        db  0FFh, 035h, 090h, 003h, 000h, 000h
        push   edx
        push   eax
        db  0CDh, 020h, 025h, 000h, 033h, 000h
        add    esp, 10h
        jmp    SHORT loc_21_12A3
sub_21_12BB:
        ret
loc_21_12BC:
        db  0A1h, 0C0h, 003h, 000h, 000h
        cmp    eax, 0
        jne    SHORT loc_21_12F2
        db  0A1h, 088h, 003h, 000h, 000h
        db  087h, 005h, 084h, 003h, 000h, 000h
        db  0A3h, 088h, 003h, 000h, 000h
        db  0A1h, 098h, 003h, 000h, 000h
        db  087h, 005h, 094h, 003h, 000h, 000h
        db  0A3h, 098h, 003h, 000h, 000h
        jmp    SHORT loc_21_12F2
loc_21_12E8:
        db  0C7h, 005h, 0C0h, 003h, 000h, 000h, 000h, 000h, 000h, 000h
loc_21_12F2:
        push   0
        db  0FFh, 035h, 088h, 003h, 000h, 000h
        push   -1
        push   0
        db  0CDh, 020h, 026h, 000h, 033h, 000h
        add    esp, 10h
        push   0
        db  0FFh, 035h, 098h, 003h, 000h, 000h
        push   -1
        push   0
        db  0CDh, 020h, 026h, 000h, 033h, 000h
        add    esp, 10h
        mov    eax, 13h
        db  087h, 005h, 0C0h, 003h, 000h, 000h
        ret
loc_21_1328:
        push   ecx
        push   0
        db  0FFh, 035h, 08Ch, 003h, 000h, 000h
        db  0FFh, 035h, 084h, 003h, 000h, 000h
        db  0CDh, 020h, 028h, 000h, 033h, 000h
        add    esp, 0ch
        pop    ecx
        cmp    eax, 0
        jne    SHORT loc_21_1359
        db  0B8h, 08Ch, 003h, 000h, 000h
        mov    dword ptr [ecx], eax
        mov    dword ptr [ecx + 4], 4
        mov    eax, 0
loc_21_1359:
        ret
loc_21_135A:
        mov    eax, 0
        ret
_PTEXT ends
        end
