; vpowerd_icode_04.asm -- VPOWERD object 4 segment _ITEXT
.386p
_ITEXT segment dword public use32 'ICODE'

loc_4_0000:
        db  089h, 015h, 030h, 006h, 000h, 000h
        db  08Dh, 035h, 028h, 000h, 000h, 000h
        db  0CDh, 020h, 03Ah, 000h, 001h, 000h
        ret
loc_4_0013:
        db  0A1h, 030h, 006h, 000h, 000h
        cmp    word ptr [eax], 101h
        jae    SHORT loc_4_0026
        db  0C6h, 005h, 02Ah, 006h, 000h, 000h, 000h
loc_4_0026:
        cmp    word ptr [eax], 102h
        jb     SHORT loc_4_0034
        db  0C6h, 005h, 02Bh, 006h, 000h, 000h, 001h
loc_4_0034:
        mov    ax, 5303h
        xor    ebx, ebx
        push   15h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        jb     NEAR PTR loc_4_02A8
        db  089h, 01Dh, 024h, 006h, 000h, 000h
        mov    esi, 0ffffh
        mov    edi, esi
        movzx  eax, ax
        shl    eax, 4
        movzx  ecx, cx
        shl    ecx, 4
        movzx  ebx, dx
        shl    ebx, 4
        db  080h, 03Dh, 02Ah, 006h, 000h, 000h, 000h
        je     SHORT loc_4_008C
        mov    edx, 100000h
        sub    edx, eax
        dec    edx
        cmp    edx, esi
        ja     SHORT loc_4_007E
        mov    esi, edx
loc_4_007E:
        mov    edx, 100000h
        sub    edx, ebx
        dec    edx
        cmp    edx, edi
        ja     SHORT loc_4_008C
        mov    edi, edx
loc_4_008C:
        push   ecx
        push   1
        push   40h
        push   9ah
        push   esi
        push   eax
        db  0CDh, 020h, 07Ah, 000h, 001h, 000h
        add    esp, 14h
        push   0
        push   eax
        push   edx
        db  0CDh, 020h, 076h, 000h, 001h, 000h
        add    esp, 0ch
        pop    ecx
        or     eax, eax
        je     NEAR PTR loc_4_025B
        db  066h, 0A3h, 028h, 006h, 000h, 000h
        db  080h, 03Dh, 02Ah, 006h, 000h, 000h, 000h
        je     SHORT loc_4_00D9
        mov    esi, 0ffffh
        mov    edx, 100000h
        sub    edx, ecx
        dec    edx
        cmp    edx, esi
        ja     SHORT loc_4_00D9
        mov    esi, edx
loc_4_00D9:
        push   1
        push   0
        push   9ah
        push   esi
        push   ecx
        db  0CDh, 020h, 07Ah, 000h, 001h, 000h
        add    esp, 14h
        push   0
        push   eax
        push   edx
        db  0CDh, 020h, 076h, 000h, 001h, 000h
        add    esp, 0ch
        or     eax, eax
        je     NEAR PTR loc_4_025B
        push   1
        push   0
        push   92h
        push   edi
        push   ebx
        db  0CDh, 020h, 07Ah, 000h, 001h, 000h
        add    esp, 14h
        push   0
        push   eax
        push   edx
        db  0CDh, 020h, 076h, 000h, 001h, 000h
        add    esp, 0ch
        or     eax, eax
        je     NEAR PTR loc_4_025B
        push   es
        db  066h, 08Eh, 005h, 028h, 006h, 000h, 000h
        db  08Bh, 03Dh, 024h, 006h, 000h, 000h
        cmp    dword ptr es:[edi], 6650669ch
        jne    SHORT loc_4_014A
        cmp    dword ptr es:[edi + 4], 0ec8b6655h
loc_4_014A:
        pop    es
        je     NEAR PTR loc_4_0272
        db  080h, 03Dh, 02Ah, 006h, 000h, 000h, 000h
        je     NEAR PTR loc_4_01E2
        mov    cx, 101h
        db  080h, 03Dh, 02Bh, 006h, 000h, 000h, 000h
        je     SHORT loc_4_016D
        inc    cx
loc_4_016D:
        mov    ax, 530eh
        xor    ebx, ebx
        db  0FFh, 01Dh, 024h, 006h, 000h, 000h
        jb     NEAR PTR loc_4_0272
        db  080h, 03Dh, 02Bh, 006h, 000h, 000h, 000h
        je     SHORT loc_4_01E2
        mov    ax, 5310h
        xor    bx, bx
        db  0FFh, 01Dh, 024h, 006h, 000h, 000h
        db  08Bh, 03Dh, 030h, 006h, 000h, 000h
        mov    word ptr [edi + 4], cx
        mov    byte ptr [edi + 6], bl
        jae    SHORT loc_4_01C6
        db  0C6h, 005h, 02Bh, 006h, 000h, 000h, 000h
        mov    word ptr [edi], 101h
        mov    cx, 101h
        mov    ax, 530eh
        xor    ebx, ebx
        db  0FFh, 01Dh, 024h, 006h, 000h, 000h
        jb     NEAR PTR loc_4_0272
loc_4_01C6:
        db  0A1h, 030h, 006h, 000h, 000h
        test   dword ptr [eax + 3], 10h
        je     SHORT loc_4_01DC
        db  0CDh, 020h, 010h, 000h, 026h, 000h
        jmp    SHORT loc_4_01E2
loc_4_01DC:
        db  0CDh, 020h, 00Fh, 000h, 026h, 000h
loc_4_01E2:
        db  0C6h, 005h, 02Ch, 006h, 000h, 000h, 0FFh
        mov    eax, 0
        db  0BEh, 0D4h, 002h, 000h, 000h
        db  0BFh, 0DEh, 002h, 000h, 000h
        db  0CDh, 020h, 0ABh, 000h, 001h, 000h
        push   eax
        db  0CDh, 020h, 003h, 000h, 026h, 000h
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_4_0272
        push   40000000h
        db  068h, 08Ch, 006h, 000h, 000h
        db  0CDh, 020h, 00Ah, 000h, 026h, 000h
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_4_0272
        db  08Bh, 03Dh, 030h, 006h, 000h, 000h
        bt     dword ptr [edi + 3], 7
        jae    SHORT loc_4_0243
        mov    eax, 2fh
        db  08Dh, 035h, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 041h, 000h, 001h, 000h
        jb     SHORT loc_4_0272
loc_4_0243:
        push   0
        push   0
        db  068h, 0A2h, 000h, 000h, 000h
        db  0CDh, 020h, 011h, 000h, 017h, 000h
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_4_0261
        clc
        ret
loc_4_025B:
        db  0CDh, 020h, 0BFh, 080h, 001h, 000h
loc_4_0261:
        mov    eax, 2fh
        db  08Dh, 035h, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 018h, 001h, 001h, 000h
loc_4_0272:
        db  0E8h, 001h, 0FEh, 0FBh, 0FFh
        db  00Fh, 0B7h, 035h, 028h, 006h, 000h, 000h
        push   0
        push   esi
        db  0CDh, 020h, 077h, 000h, 001h, 000h
        add    esp, 8
        add    esi, 8
        push   0
        push   esi
        db  0CDh, 020h, 077h, 000h, 001h, 000h
        add    esp, 8
        add    esi, 8
        push   0
        push   esi
        db  0CDh, 020h, 077h, 000h, 001h, 000h
        add    esp, 8
loc_4_02A8:
        db  08Dh, 035h, 028h, 000h, 000h, 000h
        db  0CDh, 020h, 02Bh, 001h, 001h, 000h
        db  08Dh, 035h, 088h, 006h, 000h, 000h
        mov    edi, dword ptr [esi]
loc_4_02BC:
        cmp    esi, edi
        je     SHORT loc_4_02D2
        mov    eax, edi
        mov    edi, dword ptr [edi]
        push   0
        push   eax
        db  0CDh, 020h, 051h, 000h, 001h, 000h
        add    esp, 8
        jmp    SHORT loc_4_02BC
loc_4_02D2:
        stc
        ret
loc_4_02D4:
        push   eax
        dec    edi
        push   edi
        inc    ebp
        push   edx
        inc    esp
        push   edx
        push   esi
        db  000h
loc_4_02DE:
        push   eax
        outsd
        db  077h, 065h
        db  072h, 053h
        db  074h, 061h
        db  074h, 075h
        jae    SHORT loc_4_02EA
loc_4_02EA:
        add    byte ptr [eax], al
_ITEXT ends
        end
