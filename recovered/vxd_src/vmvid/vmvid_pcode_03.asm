; vmvid_pcode_03.asm -- VMVID object 3 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

loc_3_0000:
        mov    eax, 15000eh
        db  0BEh, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 01Ch, 001h, 001h, 000h
        clc
        ret
loc_3_0012:
        xchg   esi, eax
        mov    ax, 3430h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        xchg   esi, eax
        push   dword ptr [ebp + 3ch]
        push   dword ptr [ebp + 14h]
        mov    eax, dword ptr [esi + 4]
        mov    word ptr [ebp + 14h], ax
        shr    eax, 10h
        mov    word ptr [ebp + 3ch], ax
        mov    ax, 3c14h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        mov    dword ptr [esi + 4], eax
        mov    eax, dword ptr [esi + 8]
        mov    word ptr [ebp + 14h], ax
        shr    eax, 10h
        mov    word ptr [ebp + 3ch], ax
        mov    ax, 3c14h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        mov    dword ptr [esi + 8], eax
        pop    dword ptr [ebp + 14h]
        pop    dword ptr [ebp + 3ch]
        mov    eax, dword ptr [esi]
        db  0B8h, 000h, 000h, 000h, 000h
        push   dword ptr [esi + 8]
        push   dword ptr [esi + 4]
        push   eax
        push   dword ptr [esi]
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_0075:
        add    esp, 10h
        or     eax, eax
        je     SHORT loc_3_0089
        mov    word ptr [ebp + 1ch], ax
        shr    eax, 10h
        mov    word ptr [ebp + 14h], ax
        clc
        ret
loc_3_0089:
        mov    word ptr [ebp + 1ch], 0
        mov    word ptr [ebp + 14h], 0
        stc
        ret
loc_3_0097:
        xchg   esi, eax
        mov    ax, 3430h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        xchg   esi, eax
        mov    eax, dword ptr [esi]
        db  0B8h, 000h, 000h, 000h, 000h
        db  08Dh, 005h, 000h, 000h, 000h, 000h
        push   dword ptr [esi + 4]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_00B9:
        add    esp, 8
        mov    word ptr [ebp + 1ch], 0
        clc
        ret
loc_3_00C4:
        movzx  edx, word ptr [ebp + 1ch]
        or     edx, edx
        jb     SHORT loc_3_00DD
        call   sub_3_01B4
        jb     SHORT loc_3_00D5
        clc
        ret
loc_3_00D5:
        mov    word ptr [ebp + 1ch], 2
        jmp    SHORT loc_3_00E3
loc_3_00DD:
        mov    word ptr [ebp + 1ch], 1
loc_3_00E3:
        stc
        ret
loc_3_00E5:
        movzx  edx, word ptr [ebp + 1ch]
        or     edx, edx
        je     SHORT loc_3_011E
        db  039h, 01Dh, 000h, 000h, 000h, 000h
        je     SHORT loc_3_00FD
        mov    word ptr [ebp + 1ch], 3
        jmp    SHORT loc_3_0124
loc_3_00FD:
        call   sub_3_0244
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        mov    word ptr [ebp + 1ch], 0
        clc
        ret
loc_3_011E:
        mov    word ptr [ebp + 1ch], 1
loc_3_0124:
        stc
        ret
loc_3_0126:
        pushfd
        mov    ax, word ptr [ebp + 1ch]
        cmp    ax, 1
        jne    SHORT loc_3_0173
        mov    ecx, dword ptr [ebp + 18h]
        db  03Bh, 00Dh, 000h, 000h, 000h, 000h
        jne    SHORT loc_3_0173
        xchg   edi, eax
        mov    ax, 3810h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        xchg   edi, eax
        cmp    edi, -1
        je     SHORT loc_3_0173
        mov    ecx, dword ptr [edi]
        or     ecx, ecx
        je     SHORT loc_3_0173
        cmp    ecx, 1ch
        ja     SHORT loc_3_0173
        mov    word ptr [edi + 4], 1
        mov    word ptr [edi + 6], 388h
        mov    eax, dword ptr [ebp + 18h]
        mov    dword ptr [edi + 0ch], eax
        mov    word ptr [ebp + 1ch], 1
        popfd
        clc
        ret
loc_3_0173:
        mov    word ptr [ebp + 1ch], 0
        popfd
        stc
        ret
sub_3_017C:
        push   edi
        db  0BFh, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 095h, 000h, 001h, 000h
        pop    edi
        ret
loc_3_018A:
        push   eax
        db  0A1h, 000h, 000h, 000h, 000h
        cmp    eax, ebx
        je     SHORT loc_3_019B
        call   sub_3_01B4
        jb     SHORT loc_3_01AE
loc_3_019B:
        pop    eax
        cmp    ecx, 4
        jbe    SHORT loc_3_01A7
        db  0CDh, 020h, 094h, 080h, 001h, 000h
loc_3_01A7:
        je     SHORT loc_3_01AC
        in     al, dx
        jmp    SHORT loc_3_01AD
loc_3_01AC:
        out    dx, al
loc_3_01AD:
        ret
loc_3_01AE:
        pop    eax
        xor    eax, eax
        dec    eax
        ret
        db  090h
sub_3_01B4:
        push   eax
        db  0A1h, 000h, 000h, 000h, 000h
        or     eax, eax
        je     SHORT loc_3_01C4
        cmp    ebx, eax
        je     SHORT loc_3_01F9
        jmp    SHORT loc_3_01FC
loc_3_01C4:
        db  03Bh, 01Dh, 000h, 000h, 000h, 000h
        je     SHORT loc_3_01D1
        call   sub_3_0265
loc_3_01D1:
        db  087h, 01Dh, 000h, 000h, 000h, 000h
        db  089h, 01Dh, 000h, 000h, 000h, 000h
        or     ebx, ebx
        je     SHORT loc_3_01EE
        db  0CDh, 020h, 005h, 000h, 001h, 000h
        jb     SHORT loc_3_01EE
        call   sub_3_0244
loc_3_01EE:
        db  08Bh, 01Dh, 000h, 000h, 000h, 000h
        call   sub_3_0223
loc_3_01F9:
        pop    eax
        clc
        ret
loc_3_01FC:
        push   esi
        mov    esi, ebx
        jmp    SHORT loc_3_021F
        db  060h, 00Fh, 0B7h, 00Dh, 00Ch, 000h, 000h, 000h, 003h, 00Dh, 004h, 000h, 000h, 000h, 0B8h, 030h
        db  000h, 000h, 000h, 033h, 0F6h, 033h, 0FFh, 0CDh, 020h, 004h, 000h, 017h, 000h, 061h
loc_3_021F:
        pop    esi
        pop    eax
        stc
        ret
sub_3_0223:
        push   esi
        push   ecx
        push   edx
        db  0BEh, 000h, 000h, 000h, 000h
        movzx  ecx, word ptr [esi]
        add    esi, 2
loc_3_0231:
        movzx  edx, word ptr [esi]
        add    esi, 6
        db  0CDh, 020h, 09Ah, 000h, 001h, 000h
        loop   loc_3_0231
        pop    edx
        pop    ecx
        pop    esi
        clc
        ret
sub_3_0244:
        push   esi
        push   ecx
        push   edx
        db  0BEh, 000h, 000h, 000h, 000h
        movzx  ecx, word ptr [esi]
        add    esi, 2
loc_3_0252:
        movzx  edx, word ptr [esi]
        add    esi, 6
        db  0CDh, 020h, 098h, 000h, 001h, 000h
        loop   loc_3_0252
        pop    edx
        pop    ecx
        pop    esi
        clc
        ret
sub_3_0265:
        push   eax
        push   edx
        push   ebx
        push   ecx
        mov    edx, 388h
        mov    ebx, 105h
        mov    al, 1
        call   sub_3_0312
        mov    ebx, 4
        mov    al, 60h
        call   sub_3_0312
        mov    ebx, 104h
        mov    al, 3fh
        call   sub_3_0312
        mov    ebx, 8
        xor    al, al
        call   sub_3_0312
        mov    ebx, 0bdh
        mov    al, 0c0h
        call   sub_3_0312
        mov    ecx, 15h
        mov    ebx, 40h
        mov    al, 3fh
loc_3_02B6:
        xor    bh, bh
        call   sub_3_0312
        mov    bh, 1
        call   sub_3_0312
        inc    bl
        loop   loc_3_02B6
        mov    ecx, 8
        mov    ebx, 0b0h
        xor    eax, eax
loc_3_02D4:
        xor    bh, bh
        call   sub_3_0312
        mov    bh, 1
        call   sub_3_0312
        inc    bl
        loop   loc_3_02D4
        mov    ebx, 104h
        xor    eax, eax
        call   sub_3_0312
        xor    eax, eax
        mov    ebx, 105h
        call   sub_3_0312
        call   sub_3_032F
        call   sub_3_032F
        call   sub_3_032F
        pop    ecx
        pop    ebx
        pop    edx
        pop    eax
        ret
sub_3_0312:
        push   ebx
        push   edx
        push   eax
        mov    eax, ebx
        shl    ah, 1
        add    dl, ah
        out    dx, al
        sub    dl, ah
        call   sub_3_032F
        pop    eax
        inc    dl
        out    dx, al
        call   sub_3_032F
        pop    edx
        pop    ebx
        ret
sub_3_032F:
        push   eax
        push   ecx
        inc    edx
        mov    ecx, 32h
loc_3_0337:
        in     al, dx
        loop   loc_3_0337
        dec    edx
        pop    ecx
        pop    eax
        ret
loc_3_033E:
        push   eax
        db  0A1h, 000h, 000h, 000h, 000h
        cmp    eax, ebx
        je     SHORT loc_3_0354
        mov    eax, 1
        call   sub_3_0447
        jb     SHORT loc_3_0367
loc_3_0354:
        pop    eax
        cmp    ecx, 4
        jbe    SHORT loc_3_0360
        db  0CDh, 020h, 094h, 080h, 001h, 000h
loc_3_0360:
        je     SHORT loc_3_0365
        in     al, dx
        jmp    SHORT loc_3_0366
loc_3_0365:
        out    dx, al
loc_3_0366:
        ret
loc_3_0367:
        pop    eax
        xor    eax, eax
        dec    eax
        ret
sub_3_036C:
        push   edi
        push   eax
        push   ecx
        db  0BFh, 000h, 000h, 000h, 000h
        movzx  ecx, word ptr [edi]
        add    edi, 2
        db  066h, 0A1h, 000h, 000h, 000h, 000h
loc_3_0380:
        mov    word ptr [edi], ax
        add    edi, 6
        inc    ax
        loop   loc_3_0380
        db  0BFh, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 095h, 000h, 001h, 000h
        pop    ecx
        pop    eax
        pop    edi
        ret
loc_3_0399:
        movzx  edx, word ptr [ebp + 1ch]
        or     edx, edx
        je     SHORT loc_3_03BC
        mov    word ptr [ebp + 1ch], 0
        mov    eax, 1
        call   sub_3_0447
        jb     SHORT loc_3_03B4
        ret
loc_3_03B4:
        mov    word ptr [ebp + 1ch], 1
        jmp    SHORT loc_3_03C2
loc_3_03BC:
        mov    word ptr [ebp + 1ch], 1
loc_3_03C2:
        stc
        ret
loc_3_03C4:
        movzx  edx, word ptr [ebp + 1ch]
        or     edx, edx
        je     SHORT loc_3_03F0
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        cmp    ecx, ebx
        je     SHORT loc_3_03DE
        mov    word ptr [ebp + 1ch], 1
        jmp    SHORT loc_3_03F6
loc_3_03DE:
        mov    eax, 1
        call   sub_3_0534
        mov    word ptr [ebp + 1ch], 0
        clc
        ret
loc_3_03F0:
        mov    word ptr [ebp + 1ch], 1
loc_3_03F6:
        stc
        ret
loc_3_03F8:
        pushfd
        mov    ax, word ptr [ebp + 1ch]
        cmp    ax, 1
        jne    SHORT loc_3_043E
        mov    ecx, dword ptr [ebp + 18h]
        xchg   edi, eax
        mov    ax, 3810h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        xchg   edi, eax
        mov    word ptr [edi + 4], 0
        db  00Fh, 0B7h, 005h, 000h, 000h, 000h, 000h
        mov    word ptr [edi + 6], ax
        db  00Fh, 0B7h, 005h, 000h, 000h, 000h, 000h
        mov    byte ptr [edi + 8], al
        db  0A1h, 000h, 000h, 000h, 000h
        mov    dword ptr [edi + 10h], eax
        mov    word ptr [ebp + 1ch], 1
        popfd
        clc
        ret
loc_3_043E:
        mov    word ptr [ebp + 1ch], 0
        popfd
        stc
        ret
sub_3_0447:
        push   edx
        push   ecx
        push   eax
        test   eax, 1
        je     SHORT loc_3_0471
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     SHORT loc_3_0464
        db  039h, 01Dh, 000h, 000h, 000h, 000h
        je     SHORT loc_3_0495
        jmp    SHORT loc_3_0471
loc_3_0464:
        db  089h, 01Dh, 000h, 000h, 000h, 000h
        call   sub_3_049A
        jmp    SHORT loc_3_0474
loc_3_0471:
        stc
        jmp    SHORT loc_3_0496
loc_3_0474:
        db  0A1h, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 005h, 000h, 003h, 000h
        test   ecx, 10h
        je     SHORT loc_3_048F
        db  0CDh, 020h, 008h, 000h, 003h, 000h
        jmp    SHORT loc_3_0495
loc_3_048F:
        db  0CDh, 020h, 009h, 000h, 003h, 000h
loc_3_0495:
        clc
loc_3_0496:
        pop    eax
        pop    ecx
        pop    edx
        ret
sub_3_049A:
        push   esi
        push   eax
        push   ecx
        push   edx
        mov    si, 1
        xor    ecx, ecx
        db  00Fh, 0B7h, 015h, 000h, 000h, 000h, 000h
loc_3_04AB:
        db  0CDh, 020h, 09Ah, 000h, 001h, 000h
        inc    dx
        cmp    cx, si
        je     SHORT loc_3_04BC
        inc    cx
        jmp    SHORT loc_3_04AB
loc_3_04BC:
        pop    edx
        pop    ecx
        pop    eax
        pop    esi
        clc
        ret
        db  08Bh, 0C0h
loc_3_04C4:
        push   edi
        push   edx
        push   ebx
        db  00Fh, 0BFh, 015h, 000h, 000h, 000h, 000h
        or     edx, edx
        js     SHORT loc_3_04EF
        db  08Bh, 01Dh, 000h, 000h, 000h, 000h
        or     ebx, ebx
        jne    SHORT loc_3_04F7
        push   eax
        push   ecx
        mov    ecx, 32h
loc_3_04E3:
        inc    edx
        in     al, dx
        or     al, al
        js     SHORT loc_3_04ED
        dec    edx
        in     al, dx
        loop   loc_3_04E3
loc_3_04ED:
        pop    ecx
        pop    eax
loc_3_04EF:
        db  0CDh, 020h, 004h, 000h, 003h, 000h
        jmp    SHORT loc_3_04FD
loc_3_04F7:
        db  0CDh, 020h, 002h, 000h, 003h, 000h
loc_3_04FD:
        pop    ebx
        pop    edx
        pop    edi
        clc
        ret
        db  08Bh, 0C0h
loc_3_0504:
        db  0CDh, 020h, 003h, 000h, 003h, 000h
        db  0CDh, 020h, 004h, 000h, 003h, 000h
        clc
        ret
        db  08Bh, 0C0h
loc_3_0514:
        push   edi
        db  039h, 01Dh, 000h, 000h, 000h, 000h
        jne    SHORT loc_3_052F
        jecxz  loc_3_0527
loc_3_051F:
        db  0CDh, 020h, 008h, 000h, 003h, 000h
        jmp    SHORT loc_3_0531
loc_3_0527:
        db  0CDh, 020h, 009h, 000h, 003h, 000h
        jmp    SHORT loc_3_0531
loc_3_052F:
        jmp    SHORT loc_3_051F
loc_3_0531:
        pop    edi
        clc
        ret
sub_3_0534:
        push   eax
        push   edx
        push   esi
        test   eax, 1
        je     SHORT loc_3_0575
        db  039h, 01Dh, 000h, 000h, 000h, 000h
        jne    SHORT loc_3_0575
        call   sub_3_057A
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0A1h, 000h, 000h, 000h, 000h
        or     eax, eax
        jne    SHORT loc_3_0575
        db  0A1h, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 008h, 000h, 003h, 000h
        db  0CDh, 020h, 004h, 000h, 003h, 000h
        db  0CDh, 020h, 003h, 000h, 003h, 000h
loc_3_0575:
        pop    esi
        pop    edx
        pop    eax
        clc
        ret
sub_3_057A:
        push   esi
        push   eax
        push   ecx
        mov    si, 1
        xor    ecx, ecx
        db  00Fh, 0B7h, 015h, 000h, 000h, 000h, 000h
loc_3_058A:
        db  0CDh, 020h, 098h, 000h, 001h, 000h
        inc    dx
        cmp    cx, si
        je     SHORT loc_3_059B
        inc    cx
        jmp    SHORT loc_3_058A
loc_3_059B:
        pop    ecx
        pop    eax
        pop    esi
        clc
        ret
_PTEXT ends
        end
