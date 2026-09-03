; dosmgr_pcode_14.asm -- DOSMGR object 14 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

DOSMGR_VM_CRITICAL_INIT:
        db  0A1h, 00Ch, 000h, 000h, 000h
        mov    edx, dword ptr [ebx + 4]
        mov    dword ptr [edx + 8ch], eax
        call   sub_14_0054
        jb     SHORT loc_14_003D
loc_14_0015:
        db  08Bh, 00Dh, 04Ch, 000h, 000h, 000h
        jecxz  loc_14_002C
        mov    edx, dword ptr [ebx + 4]
        xchg   dword ptr [edx + 0a0h], ecx
        db  089h, 00Dh, 064h, 000h, 000h, 000h
loc_14_002C:
        mov    eax, dword ptr [ebx + 0ch]
        db  08Bh, 015h, 068h, 000h, 000h, 000h
        add    edx, dword ptr [ebx + 4]
        mov    word ptr [edx], ax
        clc
loc_14_003C:
        ret
loc_14_003D:
        mov    eax, 20008h
        db  08Dh, 015h, 0B0h, 001h, 000h, 000h
        xor    ecx, ecx
        inc    ecx
        db  0CDh, 020h, 0E0h, 000h, 001h, 000h
        stc
        jmp    SHORT loc_14_003C
sub_14_0054:
        pushad
        db  083h, 03Dh, 070h, 000h, 000h, 000h, 000h
        je     SHORT loc_14_0074
        db  08Bh, 035h, 06Ch, 000h, 000h, 000h
        add    esi, dword ptr [ebx + 4]
        cmp    word ptr [esi], -1
        jne    SHORT loc_14_0074
        db  0A1h, 078h, 000h, 000h, 000h
        mov    dword ptr [esi], eax
loc_14_0074:
        popad
        clc
        ret
        db  000h
_DOSMGR_Set_Exec_VM_Data:
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [esp + 10h]
        db  08Bh, 015h, 010h, 000h, 000h, 000h
        add    edx, dword ptr [ebx + 4]
        mov    word ptr [edx + 20h], 0
        cld
        mov    esi, dword ptr [esp + 1ch]
        lea    edi, [edx + 130h]
        mov    ecx, 44h
        stosb
        test   esi, esi
        je     SHORT loc_14_00AA
        dec    edi
loc_14_00A4:
        lodsb
        stosb
        or     al, al
        loopne loc_14_00A4
loc_14_00AA:
        mov    esi, dword ptr [esp + 14h]
        lea    edi, [edx + 2eh]
        mov    eax, edi
        sub    eax, dword ptr [ebx + 4]
        mov    ecx, 20h
        rep movsd
        shl    eax, 0ch
        shr    ax, 0ch
        mov    dword ptr [edx + 22h], eax
        lea    edi, [edx]
        lea    esi, [edi + 26h]
        db  068h, 0EDh, 000h, 000h, 000h
sub_14_00D1:
        mov    eax, edi
        sub    eax, dword ptr [ebx + 4]
        shl    eax, 0ch
        shr    ax, 0ch
        mov    dword ptr [esi], eax
        sub    eax, eax
        stosb
        mov    eax, 20202020h
        stosd
        stosd
        stosw
        stosb
        ret
loc_14_00ED:
        lea    edi, [edx + 10h]
        lea    esi, [edi + 1ah]
        call   sub_14_00D1
        mov    esi, dword ptr [esp + 18h]
        lea    edi, [edx + 0b0h]
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        mov    dword ptr [edx + ebx + 4ch], edi
        mov    ecx, 80h
loc_14_0111:
        lodsb
        stosb
        or     al, al
        loopne loc_14_0111
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_14_011B:
        enter  10h, 0
        push   esi
        push   edi
        db  08Bh, 035h, 000h, 000h, 000h, 000h
        bts    word ptr [esi + ebx], 0eh
        jb     NEAR PTR loc_14_0293
        cld
        mov    dword ptr [ebp - 4], edx
        mov    edi, edx
        movzx  edx, word ptr [edi + 54h]
        or     edx, edx
        jne    SHORT loc_14_0147
        db  08Bh, 015h, 0CAh, 000h, 000h, 000h
loc_14_0147:
        mov    dword ptr [ebp - 0ch], edx
        call   sub_14_02E9
        jb     NEAR PTR loc_14_0293
        mov    dword ptr [ebp - 8], eax
        mov    dword ptr [ebp - 10h], edx
        cmp    byte ptr [edi + 4], 0
        jne    SHORT loc_14_018A
        mov    edx, dword ptr [ebp - 0ch]
        or     edx, edx
        je     NEAR PTR loc_14_0293
        db  03Bh, 015h, 048h, 000h, 000h, 000h
        jbe    NEAR PTR loc_14_0293
        or     al, al
        jne    NEAR PTR loc_14_0228
        cmp    word ptr [edi + 54h], ax
        je     NEAR PTR loc_14_0293
loc_14_018A:
        and    al, 3
        cmp    al, 1
        je     NEAR PTR loc_14_0219
        mov    edx, dword ptr [ebp - 10h]
        mov    dword ptr [esi + ebx + 4ch], edx
        db  08Bh, 035h, 010h, 000h, 000h, 000h
        add    esi, dword ptr [ebx + 4]
        lea    edi, [esi + 0b0h]
        lea    edx, [edi - 1]
        add    esi, 2eh
        sub    eax, eax
        mov    ecx, 80h
        repne scasb
        dec    ecx
        jle    NEAR PTR loc_14_0293
        lodsb
        cmp    eax, ecx
        jae    NEAR PTR loc_14_0293
        push   esi
        mov    byte ptr [edi - 1], 20h
        cmp    byte ptr [esi], 20h
        jne    SHORT loc_14_01D5
        inc    esi
        dec    eax
loc_14_01D5:
        sub    ecx, eax
        xchg   ecx, eax
        rep movsb
        xchg   ecx, eax
        mov    al, 0dh
        stosb
        not    ecx
        add    ecx, 80h
        mov    byte ptr [edx], cl
        pop    edi
        dec    edi
        mov    word ptr [edi], 0d00h
        mov    esi, edx
        mov    edx, dword ptr [ebp - 4]
        cmp    byte ptr [edx + 4], 0
        jne    SHORT loc_14_020F
        inc    ecx
        inc    ecx
        rep movsb
        db  0BEh, 0B0h, 000h, 000h, 000h
        mov    ecx, 3
        call   sub_14_0297
        jmp    SHORT loc_14_0219
loc_14_020F:
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        or     byte ptr [edx + ebx], 1
loc_14_0219:
        mov    edx, dword ptr [ebp - 0ch]
        or     edx, edx
        je     SHORT loc_14_025C
        db  03Bh, 015h, 048h, 000h, 000h, 000h
        jbe    SHORT loc_14_025C
loc_14_0228:
        test   dword ptr [ebp - 8], 4
        jne    SHORT loc_14_0293
        mov    ecx, 9
        call   sub_14_0297
        jb     SHORT loc_14_0293
        mov    ax, 452fh
        stosw
        mov    al, 3ah
        stosb
        mov    ecx, 5
loc_14_024B:
        xchg   edx, eax
        cdq
        db  0F7h, 034h, 08Dh, 0B2h, 000h, 000h, 000h
        add    al, 30h
        stosb
        loop   loc_14_024B
        mov    al, 20h
        stosb
loc_14_025C:
        mov    edx, dword ptr [ebp - 4]
        cmp    byte ptr [edx + 4], 0
        je     SHORT loc_14_0293
        db  0BEh, 0B3h, 000h, 000h, 000h
        mov    ecx, 103h
        call   sub_14_0297
        jb     SHORT loc_14_0293
        lea    esi, [edx + 4]
        mov    edi, esi
        sub    al, al
        mov    ecx, 50h
        repne scasb
        jne    SHORT loc_14_0293
        not    ecx
        add    ecx, 150h
        call   sub_14_0297
loc_14_0293:
        pop    edi
        pop    esi
        leave
        ret
sub_14_0297:
        push   edx
        db  08Bh, 03Dh, 010h, 000h, 000h, 000h
        add    edi, dword ptr [ebx + 4]
        add    edi, 2eh
        mov    al, byte ptr [edi]
        movzx  edx, al
        add    al, cl
        jb     SHORT loc_14_02D1
        cmp    al, 7fh
        cmc
        jb     SHORT loc_14_02D1
        stosb
        add    edi, edx
        inc    edx
        or     ch, ch
        movzx  ecx, cl
        je     SHORT loc_14_02C2
        mov    edx, 1
loc_14_02C2:
        mov    al, byte ptr [edi]
        mov    byte ptr [ecx + edi], al
        dec    edi
        dec    edx
        jne    SHORT loc_14_02C2
        inc    edi
        push   edi
        rep movsb
        pop    edi
        clc
loc_14_02D1:
        pop    edx
        ret
sub_14_02D3:
        mov    ecx, edx
        jmp    SHORT loc_14_02E2
loc_14_02D7:
        inc    edx
        cmp    al, 5ch
        je     SHORT loc_14_02E0
        cmp    al, 2fh
        jne    SHORT loc_14_02E2
loc_14_02E0:
        mov    ecx, edx
loc_14_02E2:
        mov    al, byte ptr [edx]
        cmp    al, 0
        jne    SHORT loc_14_02D7
        ret
sub_14_02E9:
        push   esi
        push   edi
        db  0BEh, 090h, 000h, 000h, 000h
        db  0CDh, 020h, 0B5h, 000h, 001h, 000h
        jb     SHORT loc_14_036C
        push   edx
        call   sub_14_02D3
        mov    esi, ecx
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        mov    edx, dword ptr [edx + ebx + 4ch]
        call   sub_14_02D3
        mov    edi, ecx
        sub    edx, edx
        mov    ecx, 80h
loc_14_0318:
        lodsb
        mov    ah, byte ptr [edi]
        inc    edi
        or     ax, ax
        je     SHORT loc_14_0333
        cmp    al, 41h
        jb     SHORT loc_14_032D
        cmp    al, 5ah
        ja     SHORT loc_14_032D
        or     ax, 2020h
loc_14_032D:
        cmp    al, ah
        loope  loc_14_0318
        jmp    SHORT loc_14_0369
loc_14_0333:
        inc    edx
        db  08Bh, 035h, 010h, 000h, 000h, 000h
        add    esi, dword ptr [ebx + 4]
        add    esi, 2eh
        lodsb
        movzx  ecx, al
        jecxz  loc_14_0369
loc_14_0346:
        lodsb
        cmp    al, 2fh
        jne    SHORT loc_14_0367
        mov    al, byte ptr [esi]
        or     al, 20h
        cmp    al, 65h
        jne    SHORT loc_14_0358
        add    edx, 4
        jmp    SHORT loc_14_0367
loc_14_0358:
        cmp    al, 63h
        jne    SHORT loc_14_035F
        inc    edx
        jmp    SHORT loc_14_0369
loc_14_035F:
        cmp    al, 6bh
        jne    SHORT loc_14_0367
        inc    edx
        inc    edx
        jmp    SHORT loc_14_0369
loc_14_0367:
        loop   loc_14_0346
loc_14_0369:
        xchg   edx, eax
        pop    edx
        clc
loc_14_036C:
        pop    edi
        pop    esi
        ret
sub_14_036F:
        pushad
        xor    esi, esi
        db  0CDh, 020h, 0B5h, 000h, 001h, 000h
        mov    edi, edx
        test   edi, edi
        je     SHORT loc_14_038B
        cld
        xor    eax, eax
        or     ecx, 0ffffffffh
loc_14_0384:
        repne scasb
        cmp    byte ptr [edi], al
        jne    SHORT loc_14_0384
        inc    edi
loc_14_038B:
        sub    edi, edx
        mov    dword ptr [esp + 1ch], edi
        popad
        ret
loc_14_0393:
        test   byte ptr [edx + 2], 4
        je     SHORT loc_14_03A2
        db  0A1h, 000h, 000h, 000h, 000h
        or     byte ptr [eax + ebx], 8
loc_14_03A2:
        ret
_DOSMGR_Exec_VM:
        enter  4, 0
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 4], 0
        mov    ebx, dword ptr [ebp + 8]
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp + 10h]
        mov    dword ptr [edx + ebx + 18h], eax
        mov    eax, dword ptr [ebp + 14h]
        mov    dword ptr [edx + ebx + 1ch], eax
        db  0CDh, 020h, 06Bh, 000h, 001h, 000h
        add    eax, dword ptr [ebp + 0ch]
        shl    eax, 2
        mov    esi, dword ptr [ebx + 4]
        mov    word ptr [esi + 413h], ax
        db  00Fh, 0B7h, 00Dh, 034h, 000h, 000h, 000h
        lea    edi, [ecx - 1]
        shl    edi, 4
        db  0F6h, 005h, 010h, 002h, 000h, 000h, 004h
        jne    SHORT loc_14_03FE
        cmp    byte ptr [edi + esi], 5ah
        je     NEAR PTR loc_14_0652
loc_14_03FE:
        db  080h, 03Dh, 068h, 001h, 000h, 000h, 000h
        je     NEAR PTR loc_14_05C5
        push   eax
        shr    eax, 2
        dec    eax
        push   eax
        db  0CDh, 020h, 0D5h, 000h, 001h, 000h
        cmp    eax, dword ptr [esp]
        je     NEAR PTR loc_14_0597
        jb     NEAR PTR loc_14_05C3
        push   80h
        push   0
        push   0
        push   0
        push   0
        push   ebx
        push   0
        push   1
        db  0CDh, 020h, 053h, 000h, 001h, 000h
        add    esp, 20h
        or     eax, eax
        je     NEAR PTR loc_14_0535
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        mov    dword ptr [edx + ebx + 48h], eax
        mov    edx, eax
        db  0CDh, 020h, 0D5h, 000h, 001h, 000h
        push   eax
        push   0
        push   0
        push   1
        push   eax
        push   ebx
        push   edx
        db  0CDh, 020h, 05Dh, 000h, 001h, 000h
        add    esp, 18h
        or     eax, eax
        pop    eax
        je     NEAR PTR loc_14_0519
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        test   byte ptr [edx + ebx + 1], 1
        jne    SHORT loc_14_04A0
        cmp    dword ptr [edx + ebx + 48h], 0
        je     SHORT loc_14_04A0
        push   eax
        push   0
        push   0
        push   1
        push   dword ptr [edx + ebx + 48h]
        db  0CDh, 020h, 057h, 000h, 001h, 000h
        add    esp, 10h
        pop    eax
loc_14_04A0:
        shl    eax, 0ch
        mov    byte ptr [eax + esi], 4dh
        mov    word ptr [eax + esi + 1], 0
        mov    word ptr [eax + esi + 3], 0feh
        add    eax, 0ff0h
        add    eax, esi
        xchg   edi, eax
        push   esi
        db  0BEh, 069h, 001h, 000h, 000h
        mov    ecx, 4
        cld
        rep movsd
        pop    esi
        xchg   edi, eax
        db  0CDh, 020h, 0D5h, 000h, 001h, 000h
        shl    eax, 8
        mov    ecx, eax
        pop    eax
        add    esp, 4
        shl    eax, 8
        or     al, 0ffh
        db  0F6h, 005h, 010h, 002h, 000h, 000h, 004h
        je     SHORT loc_14_04F4
        db  08Bh, 015h, 030h, 000h, 000h, 000h
        mov    word ptr [edx + esi], ax
loc_14_04F4:
        sub    ecx, eax
        dec    ecx
        shl    eax, 4
        mov    byte ptr [eax + esi], 4dh
        mov    word ptr [eax + esi + 1], 8
        mov    word ptr [eax + esi + 3], cx
        db  00Fh, 0B7h, 00Dh, 034h, 000h, 000h, 000h
        shr    eax, 4
        jmp    NEAR PTR loc_14_0685
loc_14_0519:
        xor    eax, eax
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        xchg   dword ptr [edx + ebx + 48h], eax
        or     eax, eax
        je     SHORT loc_14_0535
        push   0
        push   eax
        db  0CDh, 020h, 055h, 000h, 001h, 000h
        add    esp, 8
loc_14_0535:
        db  0CDh, 020h, 0D5h, 000h, 001h, 000h
        shl    eax, 8
        add    eax, 100h
        db  00Fh, 0B7h, 00Dh, 06Ch, 001h, 000h, 000h
        add    ecx, eax
        pop    eax
        add    esp, 4
        shl    eax, 8
        or     al, 0ffh
        db  0F6h, 005h, 010h, 002h, 000h, 000h, 004h
        je     SHORT loc_14_0568
        db  08Bh, 015h, 030h, 000h, 000h, 000h
        mov    word ptr [edx + esi], ax
loc_14_0568:
        sub    ecx, eax
        dec    ecx
        shl    eax, 4
        db  08Ah, 015h, 069h, 001h, 000h, 000h
        mov    byte ptr [eax + esi], dl
        db  066h, 08Bh, 015h, 06Ah, 001h, 000h, 000h
        mov    word ptr [eax + esi + 1], dx
        mov    word ptr [eax + esi + 3], cx
        db  00Fh, 0B7h, 00Dh, 034h, 000h, 000h, 000h
        shr    eax, 4
        jmp    NEAR PTR loc_14_0685
loc_14_0597:
        pop    eax
        add    esp, 4
        shl    eax, 0ch
        add    eax, 0ff0h
        add    eax, esi
        push   eax
        xchg   edi, eax
        push   esi
        push   ecx
        db  0BEh, 069h, 001h, 000h, 000h
        mov    ecx, 4
        rep movsd
        pop    ecx
        pop    esi
        xchg   edi, eax
        pop    eax
        sub    eax, esi
        shr    eax, 4
        jmp    NEAR PTR loc_14_0685
loc_14_05C3:
        pop    eax
        pop    eax
loc_14_05C5:
        shl    eax, 6
        db  03Bh, 005h, 054h, 000h, 000h, 000h
        je     NEAR PTR loc_14_0685
        shr    eax, 8
        dec    eax
        shl    eax, 0ch
        add    eax, 0ff0h
        mov    byte ptr [eax + esi], 4dh
        mov    word ptr [eax + esi + 1], 8
        db  08Bh, 015h, 054h, 000h, 000h, 000h
        shr    eax, 4
        push   ecx
        xor    ecx, ecx
        db  0F6h, 005h, 010h, 002h, 000h, 000h, 004h
        je     SHORT loc_14_063C
        push   edx
        db  08Bh, 015h, 030h, 000h, 000h, 000h
        mov    ecx, edi
        shr    ecx, 4
        inc    ecx
        add    cx, word ptr [edi + esi + 3]
        cmp    word ptr [edx + esi], cx
        mov    ecx, 0
        jne    SHORT loc_14_063B
        movzx  ecx, ax
        xchg   word ptr [edx + esi], cx
        shl    ecx, 4
        mov    cx, word ptr [ecx + esi + 3]
        shl    eax, 4
        mov    word ptr [eax + esi + 8], 4353h
        shr    eax, 4
        inc    cx
loc_14_063B:
        pop    edx
loc_14_063C:
        sub    edx, eax
        dec    edx
        shl    eax, 4
        mov    word ptr [eax + esi + 3], dx
        add    word ptr [eax + esi + 3], cx
        shr    eax, 4
        pop    ecx
        jmp    SHORT loc_14_0685
loc_14_0652:
        shl    eax, 6
        push   ecx
        push   eax
        db  0CDh, 020h, 000h, 000h, 012h, 000h
loc_14_065D:
        jb     SHORT loc_14_0683
        cmp    eax, 200h
        jb     SHORT loc_14_0683
        db  0CDh, 020h, 001h, 000h, 012h, 000h
        jb     SHORT loc_14_0683
        jecxz  loc_14_0683
        pushfd
        cmp    eax, dword ptr [esp + 4]
        jne    SHORT loc_14_0682
        popfd
        je     SHORT loc_14_067F
        add    dword ptr [esp], ecx
        jmp    SHORT loc_14_0683
loc_14_067F:
        sub    esp, 4
loc_14_0682:
        pop    eax
loc_14_0683:
        pop    eax
        pop    ecx
loc_14_0685:
        sub    eax, ecx
        mov    word ptr [edi + esi + 3], ax
        db  0CDh, 020h, 083h, 000h, 001h, 000h
        mov    esi, dword ptr [ebx + 8]
        db  0A1h, 010h, 000h, 000h, 000h
        add    eax, dword ptr [ebx + 4]
        lea    edi, [eax + 130h]
        mov    byte ptr [esi + 1dh], 0eh
        mov    dl, byte ptr [edi]
        sub    dl, 41h
        mov    byte ptr [esi + 14h], dl
        mov    eax, 21h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        mov    byte ptr [esi + 1dh], 3bh
        shl    edi, 0ch
        shr    di, 0ch
        mov    word ptr [esi + 14h], di
        shr    edi, 10h
        mov    word ptr [esi + 3ch], di
        mov    eax, 21h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        xor    ecx, ecx
        db  0CDh, 020h, 004h, 000h, 001h, 000h
        jne    SHORT loc_14_06F1
        db  0E8h, 048h, 0FDh, 0F1h, 0FFh
        db  066h, 08Bh, 00Dh, 0CEh, 000h, 000h, 000h
loc_14_06F1:
        db  0CDh, 020h, 000h, 000h, 01Ah, 000h
        test   eax, eax
        je     SHORT loc_14_070E
        db  0CDh, 020h, 002h, 000h, 01Ah, 000h
        jb     SHORT loc_14_070A
        or     eax, eax
        jne    SHORT loc_14_0722
        inc    eax
        jmp    SHORT loc_14_070E
loc_14_070A:
        or     eax, eax
        jne    SHORT loc_14_0773
loc_14_070E:
        db  080h, 03Dh, 01Fh, 000h, 000h, 000h, 002h
        jae    SHORT loc_14_071E
        db  00Fh, 0B6h, 005h, 01Fh, 000h, 000h, 000h
loc_14_071E:
        or     eax, eax
        je     SHORT loc_14_0773
loc_14_0722:
        mov    eax, dword ptr [ebx + 0ch]
        dec    eax
        db  0F7h, 025h, 038h, 000h, 000h, 000h
        add    ecx, eax
        db  0CDh, 020h, 06Bh, 000h, 001h, 000h
        shl    eax, 8
        add    eax, ecx
        inc    eax
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        mov    edi, dword ptr [edx + ebx + 4ch]
        dec    edi
        mov    edx, edi
loc_14_0747:
        inc    edx
        cmp    byte ptr [edx], 0
        jne    SHORT loc_14_0747
        sub    edx, edi
        push   eax
        call   sub_14_036F
        mov    dword ptr [ebp - 4], eax
        add    edx, eax
        pop    eax
        add    edx, 11h
        shr    edx, 4
        add    eax, edx
        inc    eax
        db  0CDh, 020h, 004h, 000h, 001h, 000h
        je     SHORT loc_14_076E
        xor    al, 1
loc_14_076E:
        shr    eax, 1
        adc    ecx, 0
loc_14_0773:
        db  0CDh, 020h, 06Bh, 000h, 001h, 000h
        shl    eax, 8
        add    eax, ecx
        db  066h, 08Bh, 015h, 034h, 000h, 000h, 000h
        sub    eax, edx
        mov    word ptr [esi + 38h], dx
        mov    word ptr [esi + 10h], ax
        mov    word ptr [esi + 1ch], 4a00h
        mov    eax, 21h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        or     byte ptr [edx + ebx], 2
        db  0CDh, 020h, 004h, 000h, 001h, 000h
        jne    NEAR PTR loc_14_0844
        db  08Bh, 03Dh, 03Ch, 000h, 000h, 000h
        or     edi, edi
        je     SHORT loc_14_0844
        db  08Bh, 035h, 010h, 000h, 000h, 000h
        add    esi, dword ptr [ebx + 4]
        mov    ecx, 174h
        rep movsb
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        and    byte ptr [edx + ebx], 0fdh
        db  08Bh, 03Dh, 010h, 000h, 000h, 000h
        add    edi, dword ptr [ebx + 4]
        add    edi, 0b0h
        mov    dword ptr [edx + ebx + 4ch], edi
        db  08Bh, 035h, 03Ch, 000h, 000h, 000h
        lea    esi, [esi + 1f4h]
        mov    ecx, 80h
        rep movsb
        db  08Bh, 03Dh, 010h, 000h, 000h, 000h
        add    edi, dword ptr [ebx + 4]
        add    edi, 2fh
        db  08Bh, 035h, 03Ch, 000h, 000h, 000h
        add    esi, 174h
        mov    eax, 2f20642fh
        stosd
        mov    ax, 2063h
        stosw
        push   7ah
        push   esi
        push   edi
        db  0CDh, 020h, 07Bh, 001h, 001h, 000h
        add    esp, 0ch
        mov    byte ptr [edx], 0dh
        sub    edi, 6
        sub    edx, edi
        mov    byte ptr [edi - 1], dl
        jmp    SHORT loc_14_085C
loc_14_0844:
        xor    eax, eax
        db  087h, 005h, 03Ch, 000h, 000h, 000h
        or     eax, eax
        je     SHORT loc_14_085C
        push   0
        push   eax
        db  0CDh, 020h, 051h, 000h, 001h, 000h
        add    esp, 8
loc_14_085C:
        call   sub_14_086C
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_14_086C:
        db  0CDh, 020h, 083h, 000h, 001h, 000h
        mov    esi, dword ptr [ebx + 8]
        db  0A1h, 010h, 000h, 000h, 000h
        add    eax, dword ptr [ebx + 4]
        mov    ecx, dword ptr [eax + 26h]
        mov    word ptr [esi], cx
        shr    ecx, 10h
        mov    word ptr [esi + 38h], cx
        lea    ecx, [eax + 2fh]
        sub    ecx, dword ptr [ebx + 4]
        ror    ecx, 4
        mov    word ptr [esi + 3ch], cx
        shr    ecx, 1ch
        mov    word ptr [esi + 4], cx
        mov    word ptr [esi + 1ch], 2901h
        push   eax
        mov    eax, 21h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        pop    eax
        mov    ecx, dword ptr [eax + 2ah]
        mov    word ptr [esi], cx
        shr    ecx, 10h
        mov    word ptr [esi + 38h], cx
        mov    word ptr [esi + 1ch], 2901h
        mov    eax, 21h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        db  00Fh, 0B7h, 005h, 034h, 000h, 000h, 000h
        dec    eax
        shl    eax, 4
loc_14_08E0:
        mov    ecx, eax
        movzx  eax, word ptr [ecx + 3]
        inc    eax
        shl    eax, 4
        add    eax, ecx
        cmp    word ptr [ecx + 1], 0
        jne    SHORT loc_14_08E0
        cmp    eax, 50h
        jbe    SHORT loc_14_08E0
        mov    ecx, eax
        xor    eax, eax
        mov    dword ptr [ecx - 10h], eax
        mov    al, 10h
        db  0CDh, 020h, 004h, 000h, 001h, 000h
        jne    SHORT loc_14_0915
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        test   byte ptr [edx + ebx], 2
        jne    SHORT loc_14_093E
loc_14_0915:
        mov    eax, dword ptr [ebp - 4]
        test   eax, eax
        jne    SHORT loc_14_0921
        call   sub_14_036F
loc_14_0921:
        add    eax, 0fh
        and    al, 0f0h
        mov    edi, ecx
        sub    edi, eax
        push   ecx
        mov    ecx, eax
        shr    ecx, 2
        xor    esi, esi
        db  0CDh, 020h, 0B5h, 000h, 001h, 000h
        mov    esi, edx
        cld
        rep movsd
        pop    ecx
loc_14_093E:
        sub    ecx, eax
        db  0A1h, 010h, 000h, 000h, 000h
        lea    edx, [eax + 20h]
        add    eax, dword ptr [ebx + 4]
        shr    ecx, 4
        mov    word ptr [eax + 20h], cx
        db  08Bh, 035h, 000h, 000h, 000h, 000h
        test   byte ptr [esi + ebx], 8
        je     SHORT loc_14_0963
        db  0E8h, 0B0h, 0F8h, 0F1h, 0FFh
loc_14_0963:
        mov    esi, dword ptr [ebx + 8]
        shl    edx, 0ch
        shr    dx, 0ch
        mov    word ptr [esi + 10h], dx
        shr    edx, 10h
        mov    word ptr [esi + 38h], dx
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [edx + ebx + 4ch]
        sub    ecx, dword ptr [ebx + 4]
        cmp    ecx, 0ffff0h
        jb     SHORT loc_14_09B7
        push   esi
        db  0BEh, 090h, 000h, 000h, 000h
        db  0CDh, 020h, 0B5h, 000h, 001h, 000h
        xchg   edx, eax
        xor    esi, esi
        db  0CDh, 020h, 0B5h, 000h, 001h, 000h
        sub    eax, edx
        db  08Bh, 00Dh, 010h, 000h, 000h, 000h
        add    ecx, dword ptr [ebx + 4]
        movzx  ecx, word ptr [ecx + 20h]
        shl    ecx, 4
        add    ecx, eax
        pop    esi
loc_14_09B7:
        shl    ecx, 0ch
        shr    cx, 0ch
        mov    word ptr [esi + 14h], cx
        shr    ecx, 10h
        mov    word ptr [esi + 3ch], cx
        mov    word ptr [esi + 1ch], 4b00h
        mov    dx, word ptr [esi + 34h]
        shl    edx, 10h
        mov    dx, word ptr [esi + 30h]
        xor    eax, eax
        db  0BEh, 05Ch, 001h, 000h, 000h
        db  0CDh, 020h, 00Dh, 000h, 001h, 000h
        mov    al, 21h
        db  0CDh, 020h, 046h, 080h, 001h, 000h
DOSMGR_Copy_VM_Drive_State:
        pushad
        mov    esi, dword ptr [esi + 4]
        db  08Bh, 03Dh, 020h, 000h, 000h, 000h
        add    esi, edi
        add    edi, dword ptr [ebx + 4]
        sub    esp, 400h
        mov    ebp, esp
        db  08Bh, 015h, 02Ch, 000h, 000h, 000h
loc_14_0A0C:
        mov    eax, 400h
        cmp    edx, eax
        jae    SHORT loc_14_0A17
        mov    eax, edx
loc_14_0A17:
        mov    ebx, edi
        mov    edi, ebp
        mov    ecx, eax
        cld
        rep movsb
        push   esi
        mov    edi, ebx
        mov    esi, ebp
        mov    ecx, eax
        rep movsb
        pop    esi
        sub    edx, eax
        jne    SHORT loc_14_0A0C
        add    esp, 400h
        popad
        ret
        db  000h, 000h
sub_14_0A38:
        db  080h, 03Dh, 014h, 001h, 000h, 000h, 002h
        jne    SHORT loc_14_0A45
        mov    al, 20h
        jmp    SHORT loc_14_0A57
loc_14_0A45:
        db  080h, 03Dh, 014h, 001h, 000h, 000h, 001h
        jne    SHORT loc_14_0A52
        mov    al, 10h
        jmp    SHORT loc_14_0A57
loc_14_0A52:
        ret
loc_14_0A53:
        mov    ax, word ptr [edx + 2]
loc_14_0A57:
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        and    al, 30h
        mov    ah, al
        xor    ah, byte ptr [edx + ebx + 1]
        je     SHORT loc_14_0A8C
        and    byte ptr [edx + ebx + 1], 0dfh
        test   al, 20h
        je     SHORT loc_14_0A7C
        or     byte ptr [edx + ebx + 1], 20h
        db  0E8h, 016h, 0F7h, 0F1h, 0FFh
        or     al, 10h
loc_14_0A7C:
        test   al, 10h
        je     SHORT loc_14_0A87
        call   sub_14_0A8D
        jmp    SHORT loc_14_0A8C
loc_14_0A87:
        db  0E8h, 004h, 0F7h, 0F1h, 0FFh
loc_14_0A8C:
        ret
sub_14_0A8D:
        push   esi
        push   edi
        db  083h, 03Dh, 0FCh, 000h, 000h, 000h, 000h
        jne    SHORT loc_14_0ABF
        db  0BEh, 088h, 002h, 000h, 000h
        db  0CDh, 020h, 00Bh, 000h, 001h, 000h
        jb     NEAR PTR loc_14_0BC9
        db  0A3h, 0FCh, 000h, 000h, 000h
        db  0BEh, 011h, 003h, 000h, 000h
        db  0CDh, 020h, 0E9h, 000h, 001h, 000h
        jb     NEAR PTR loc_14_0BC9
loc_14_0ABF:
        db  083h, 03Dh, 004h, 001h, 000h, 000h, 000h
        jne    NEAR PTR loc_14_0B88
        mov    eax, 110h
        push   eax
        shl    eax, 2
        push   100h
        push   eax
        db  0CDh, 020h, 04Fh, 000h, 001h, 000h
        add    esp, 8
        pop    ecx
        or     eax, eax
        je     NEAR PTR loc_14_0BC9
        db  089h, 00Dh, 004h, 001h, 000h, 000h
        db  0A3h, 008h, 001h, 000h, 000h
        mov    edi, eax
        mov    eax, 80000000h
loc_14_0AFF:
        db  0BEh, 044h, 003h, 000h, 000h
        push   eax
        push   ebx
        push   ecx
        and    eax, 1ffh
        xchg   ecx, eax
        db  0CDh, 020h, 06Bh, 000h, 001h, 000h
        cmp    ecx, eax
        jb     SHORT loc_14_0B25
        db  0CDh, 020h, 0D5h, 000h, 001h, 000h
        cmp    ecx, eax
        ja     SHORT loc_14_0B25
        sub    edx, edx
        jmp    SHORT loc_14_0B3E
loc_14_0B25:
        sub    ebx, ebx
        db  0CDh, 020h, 010h, 000h, 006h, 000h
        mov    edx, eax
        and    eax, 1200h
        cmp    eax, 1000h
        jne    SHORT loc_14_0B3E
        or     edx, 2
loc_14_0B3E:
        pop    ecx
        pop    ebx
        pop    eax
        test   dl, 3
        je     SHORT loc_14_0B7B
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        test   byte ptr [edx + ebx + 1], 20h
        jne    SHORT loc_14_0B6D
        push   eax
        push   ecx
        shl    eax, 0ch
        push   1000h
        push   eax
        db  0CDh, 020h, 039h, 001h, 001h, 000h
        add    esp, 8
        cmp    al, 2
        pop    ecx
        pop    eax
        je     SHORT loc_14_0B7B
loc_14_0B6D:
        push   edi
        db  0CDh, 020h, 071h, 000h, 001h, 000h
        db  089h, 03Dh, 010h, 001h, 000h, 000h
        pop    edi
loc_14_0B7B:
        mov    dword ptr [edi], esi
        add    edi, 4
        inc    eax
        dec    ecx
        jne    NEAR PTR loc_14_0AFF
loc_14_0B88:
        db  08Bh, 03Dh, 000h, 000h, 000h, 000h
        cmp    dword ptr [edi + ebx + 54h], 0
        jne    SHORT loc_14_0BC2
        db  0A1h, 004h, 001h, 000h, 000h
        add    eax, 1fh
        shr    eax, 3
        push   101h
        push   eax
        db  0CDh, 020h, 04Fh, 000h, 001h, 000h
        add    esp, 8
        test   eax, eax
        je     SHORT loc_14_0BC9
        mov    dword ptr [edi + ebx + 54h], eax
        db  0FFh, 005h, 000h, 001h, 000h, 000h
        or     byte ptr [edi + ebx + 1], 10h
loc_14_0BC2:
        sub    eax, eax
        db  0E8h, 0D8h, 0F6h, 0F1h, 0FFh
loc_14_0BC9:
        pop    edi
        pop    esi
        ret
DOSMGR_CREATE_VM:
        sub    eax, eax
        db  08Bh, 035h, 000h, 000h, 000h, 000h
        db  039h, 005h, 014h, 003h, 000h, 000h
        je     SHORT loc_14_0C21
        db  038h, 005h, 020h, 003h, 000h, 000h
        je     SHORT loc_14_0C21
        db  08Bh, 00Dh, 018h, 003h, 000h, 000h
        jecxz  loc_14_0C21
        push   100h
        push   eax
        push   eax
        push   eax
        push   eax
        push   ebx
        push   0
        push   ecx
        db  0CDh, 020h, 053h, 000h, 001h, 000h
        add    esp, 20h
        or     eax, eax
        je     SHORT loc_14_0C23
        mov    dword ptr [esi + ebx + 20h], eax
        push   100h
        push   0
        db  0FFh, 035h, 018h, 003h, 000h, 000h
        push   eax
        db  0CDh, 020h, 057h, 000h, 001h, 000h
        add    esp, 10h
loc_14_0C21:
        clc
loc_14_0C22:
        ret
loc_14_0C23:
        mov    eax, 20005h
        xor    edx, edx
        xor    ecx, ecx
        xor    ebx, ebx
        inc    ecx
        db  0CDh, 020h, 0E0h, 000h, 001h, 000h
        stc
        jmp    SHORT loc_14_0C22
DOSMGR_VM_INIT:
        db  083h, 03Dh, 014h, 003h, 000h, 000h, 000h
        je     NEAR PTR loc_14_0DAC
        db  080h, 03Dh, 020h, 003h, 000h, 000h, 000h
        je     NEAR PTR loc_14_0DAC
        db  0CDh, 020h, 004h, 000h, 001h, 000h
        je     NEAR PTR loc_14_0E11
        db  083h, 03Dh, 018h, 003h, 000h, 000h, 000h
        je     NEAR PTR loc_14_0DAC
        db  0CDh, 020h, 000h, 000h, 017h, 000h
        cmp    eax, 30ah
        jb     SHORT loc_14_0C94
        db  0CDh, 020h, 005h, 000h, 017h, 000h
        test   eax, 40000h
        je     SHORT loc_14_0C94
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        or     byte ptr [edx + ebx + 1], 1
        xor    eax, eax
        jmp    SHORT loc_14_0CA8
loc_14_0C94:
        db  0F6h, 005h, 010h, 002h, 000h, 000h, 002h
        je     SHORT loc_14_0CC6
        mov    eax, 100h
        db  08Bh, 015h, 000h, 000h, 000h, 000h
loc_14_0CA8:
        push   eax
        push   0
        db  0FFh, 035h, 018h, 003h, 000h, 000h
        push   dword ptr [edx + ebx + 20h]
        db  0CDh, 020h, 056h, 000h, 001h, 000h
        add    esp, 10h
        or     eax, eax
        je     NEAR PTR loc_14_0E16
loc_14_0CC6:
        db  0BEh, 014h, 002h, 000h, 000h
        db  08Bh, 00Dh, 014h, 003h, 000h, 000h
loc_14_0CD1:
        push   ecx
        movzx  eax, word ptr [esi]
        dec    eax
        shr    eax, 8
        movzx  ecx, word ptr [esi + 2]
        inc    ecx
        test   byte ptr [esi + 4], 2
        je     SHORT loc_14_0CE5
        inc    ecx
loc_14_0CE5:
        shr    ecx, 8
        push   ecx
        push   eax
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        test   byte ptr [esi + 5], 1
        jne    SHORT loc_14_0D0E
        test   byte ptr [edx + ebx + 1], 1
        jne    SHORT loc_14_0D0E
        push   200h
        push   ecx
        push   eax
        push   ebx
        db  0CDh, 020h, 065h, 000h, 001h, 000h
        add    esp, 10h
loc_14_0D0E:
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        push   0
        push   dword ptr [edx + ebx + 20h]
        db  0CDh, 020h, 058h, 000h, 001h, 000h
        add    esp, 8
        or     eax, eax
        je     NEAR PTR loc_14_0E13
        mov    edi, edx
        mov    eax, dword ptr [esi + 0ch]
        shl    eax, 0ch
        add    edi, eax
        mov    al, 4dh
        test   byte ptr [esi + 4], 1
        je     SHORT loc_14_0D3F
        mov    al, 5ah
loc_14_0D3F:
        mov    byte ptr [edi], al
        test   byte ptr [esi + 5], 1
        je     SHORT loc_14_0D4F
        mov    word ptr [edi + 1], 8
        jmp    SHORT loc_14_0D55
loc_14_0D4F:
        mov    word ptr [edi + 1], 0
loc_14_0D55:
        movzx  eax, word ptr [esi + 2]
        mov    word ptr [edi + 3], ax
        test   byte ptr [esi + 4], 2
        je     SHORT loc_14_0D80
        mov    eax, dword ptr [esp + 4]
        shl    eax, 0ch
        sub    eax, 10h
        add    edi, eax
        push   esi
        mov    esi, dword ptr [esi + 8]
        or     esi, esi
        je     SHORT loc_14_0D7F
        mov    ecx, 4
        cld
        rep movsd
loc_14_0D7F:
        pop    esi
loc_14_0D80:
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        pop    eax
        pop    ecx
        push   0
        push   dword ptr [esi + 0ch]
        push   ecx
        push   eax
        push   ebx
        push   dword ptr [edx + ebx + 20h]
        db  0CDh, 020h, 05Dh, 000h, 001h, 000h
        add    esp, 18h
        pop    ecx
        or     eax, eax
        je     SHORT loc_14_0E16
        add    esi, 10h
        dec    ecx
        jne    NEAR PTR loc_14_0CD1
loc_14_0DAC:
        db  0F6h, 005h, 010h, 002h, 000h, 000h, 004h
        je     SHORT loc_14_0E11
        sub    esp, 6ch
        push   edi
        lea    edi, [esp + 4]
        db  0CDh, 020h, 08Dh, 000h, 001h, 000h
        pop    edi
        db  0CDh, 020h, 083h, 000h, 001h, 000h
        mov    ebp, dword ptr [ebx + 8]
        mov    word ptr [ebp + 1ch], 5803h
        mov    word ptr [ebp + 10h], 0
        db  0F6h, 005h, 010h, 002h, 000h, 000h, 002h
        je     SHORT loc_14_0DF1
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        or     byte ptr [edx + ebx + 1], 4
        inc    word ptr [ebp + 10h]
loc_14_0DF1:
        mov    eax, 21h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        push   esi
        lea    esi, [esp + 4]
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        pop    esi
        add    esp, 6ch
loc_14_0E11:
        clc
loc_14_0E12:
        ret
loc_14_0E13:
        add    esp, 0ch
loc_14_0E16:
        mov    eax, 20005h
        xor    edx, edx
        xor    ecx, ecx
        inc    ecx
        db  0CDh, 020h, 0E0h, 000h, 001h, 000h
        stc
        jmp    SHORT loc_14_0E12
        db  000h, 000h, 000h
loc_14_0E2C:
        db  0BAh, 010h, 003h, 000h, 000h
        cmp    byte ptr [ebp + 1ch], 0ah
        jne    SHORT loc_14_0E3C
        db  0BAh, 012h, 003h, 000h, 000h
loc_14_0E3C:
        db  0CDh, 020h, 006h, 080h, 006h, 000h
DOSMGR_BEGIN_PM_APP:
        db  003h, 03Dh, 004h, 000h, 000h, 000h
        xor    esi, esi
        test   dl, 1
        je     SHORT loc_14_0E52
        or     esi, 40h
loc_14_0E52:
        sub    esp, 6ch
        push   edi
        lea    edi, [esp + 4]
        db  0CDh, 020h, 08Dh, 000h, 001h, 000h
        pop    edi
        db  0CDh, 020h, 082h, 000h, 001h, 000h
        mov    byte ptr [ebp + 1dh], 62h
        mov    eax, 21h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        movzx  edx, word ptr [ebp + 10h]
        mov    word ptr [edi + 18h], dx
        mov    eax, edx
        shl    eax, 4
        mov    ecx, 0ffh
        call   sub_14_0FF2
        jb     NEAR PTR loc_14_0FDB
        push   eax
        mov    eax, edx
        shl    eax, 4
        movzx  eax, word ptr [eax + 2ch]
        shl    eax, 4
        db  0CDh, 020h, 004h, 000h, 001h, 000h
        jne    SHORT loc_14_0EDD
        cmp    word ptr [eax], 0
        jne    SHORT loc_14_0EDD
        pushad
        xor    esi, esi
        db  0CDh, 020h, 0B5h, 000h, 001h, 000h
        mov    dword ptr [esp + 1ch], edx
        mov    edi, edx
        lea    esi, [eax + 4]
        or     ecx, 0ffffffffh
        xor    eax, eax
        cld
loc_14_0EC6:
        repne scasb
        cmp    byte ptr [edi], al
        jne    SHORT loc_14_0EC6
        add    edi, 3
        push   -1
        push   esi
        push   edi
        db  0CDh, 020h, 07Bh, 001h, 001h, 000h
        add    esp, 0ch
        popad
loc_14_0EDD:
        movzx  ecx, word ptr [eax - 0dh]
        shl    ecx, 4
        dec    ecx
        call   sub_14_0FF2
        jb     NEAR PTR loc_14_0FCD
        push   eax
        mov    byte ptr [ebp + 1dh], 2fh
        mov    eax, 21h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        cmp    dx, word ptr [ebp + 38h]
        jne    SHORT loc_14_0F14
        cmp    word ptr [ebp + 10h], 80h
        jne    SHORT loc_14_0F14
        mov    eax, dword ptr [esp + 4]
        jmp    SHORT loc_14_0F2B
loc_14_0F14:
        movzx  eax, word ptr [ebp + 38h]
        shl    eax, 4
        mov    ecx, 0ffffh
        call   sub_14_0FF2
        jb     NEAR PTR loc_14_0FBF
loc_14_0F2B:
        db  08Bh, 035h, 000h, 000h, 000h, 000h
        add    esi, ebx
        xchg   word ptr [esi + 36h], ax
        movzx  ecx, word ptr [ebp + 10h]
        xchg   dword ptr [esi + 38h], ecx
        mov    word ptr [edi + 14h], ax
        mov    dword ptr [edi + 10h], ecx
        xor    eax, eax
        mov    dword ptr [esi + 3ch], eax
        xor    eax, eax
        mov    al, 23h
        db  0CDh, 020h, 044h, 000h, 001h, 000h
        mov    word ptr [edi + 8], cx
        mov    dword ptr [edi + 4], edx
        xor    eax, eax
        mov    al, 24h
        db  0CDh, 020h, 044h, 000h, 001h, 000h
        mov    word ptr [edi + 0eh], cx
        mov    dword ptr [edi + 0ah], edx
        pop    eax
        pop    edx
        push   fs
        mov    fs, dx
        mov    word ptr fs:[2ch], ax
        pop    fs
        mov    esi, edi
        db  0CDh, 020h, 008h, 001h, 001h, 000h
        xchg   edi, esi
        mov    eax, edx
        xchg   word ptr [esi + 2ah], ax
        mov    word ptr [edi + 16h], ax
        db  0A1h, 040h, 000h, 000h, 000h
        mov    dword ptr [8ch], eax
        db  0A1h, 044h, 000h, 000h, 000h
        mov    dword ptr [90h], eax
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        push   esi
        lea    esi, [esp + 4]
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        pop    esi
        add    esp, 6ch
        mov    word ptr [ebp + 38h], dx
        clc
        ret
loc_14_0FBF:
        pop    eax
        push   0
        push   eax
        push   ebx
        db  0CDh, 020h, 079h, 000h, 001h, 000h
        add    esp, 0ch
loc_14_0FCD:
        pop    eax
        push   0
        push   eax
        push   ebx
        db  0CDh, 020h, 079h, 000h, 001h, 000h
        add    esp, 0ch
loc_14_0FDB:
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        push   esi
        lea    esi, [esp + 4]
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        pop    esi
        add    esp, 6ch
        stc
        ret
sub_14_0FF2:
        push   edx
        push   0
        push   esi
        push   92h
        push   ecx
        push   eax
        db  0CDh, 020h, 07Ah, 000h, 001h, 000h
        add    esp, 14h
        push   0
        push   1
        push   eax
        push   edx
        push   ebx
        db  0CDh, 020h, 078h, 000h, 001h, 000h
        add    esp, 14h
        pop    edx
        cmp    eax, 1
        ret
_PTEXT ends
        end
