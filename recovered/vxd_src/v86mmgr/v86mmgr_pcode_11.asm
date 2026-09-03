; v86mmgr_pcode_11.asm -- V86MMGR object 11 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

loc_11_0000:
        pushad
        db  08Bh, 035h, 0FCh, 011h, 000h, 000h
        db  00Fh, 0B6h, 03Dh, 08Bh, 006h, 000h, 000h
        xor    eax, eax
        push   eax
        push   eax
        push   eax
        push   eax
        push   eax
        push   ebx
        push   0
        push   edi
        db  0CDh, 020h, 053h, 000h, 001h, 000h
        add    esp, 20h
        test   eax, eax
        je     SHORT loc_11_0070
        mov    dword ptr [esi + ebx + 10h], eax
        db  00Fh, 0B6h, 00Dh, 08Ah, 006h, 000h, 000h
        push   0
        push   0
        push   edi
        push   ecx
        push   ebx
        push   eax
        db  0CDh, 020h, 05Dh, 000h, 001h, 000h
        add    esp, 18h
        db  00Fh, 0B6h, 005h, 08Ah, 006h, 000h, 000h
        shl    eax, 8
        mov    word ptr [esi + ebx + 1eh], ax
        shl    eax, 4
        add    eax, dword ptr [ebx + 4]
        mov    dword ptr [esi + ebx + 20h], eax
        db  00Fh, 0B6h, 005h, 08Bh, 006h, 000h, 000h
        shl    eax, 0ch
        mov    dword ptr [esi + ebx + 14h], eax
        mov    dword ptr [esi + ebx + 18h], eax
        popad
        clc
        ret
loc_11_0070:
        stc
        popad
        ret
        db  000h
sub_11_0074:
        enter  0, 0
        pushad
        bts    word ptr [edi + 68eh], 5
        jb     NEAR PTR loc_11_019A
        mov    esi, eax
        db  0CDh, 020h, 0D5h, 000h, 001h, 000h
        inc    eax
        db  0A3h, 000h, 000h, 000h, 000h
        lea    edx, [edi + 4]
        mov    ecx, 40h
        xor    eax, eax
loc_11_00A0:
        test   byte ptr [edx + 2], 3
        jne    NEAR PTR loc_11_0180
        test   byte ptr [edi + 68eh], 4
        jne    SHORT loc_11_00BF
        db  03Bh, 005h, 000h, 000h, 000h, 000h
        jb     NEAR PTR loc_11_0180
loc_11_00BF:
        cmp    byte ptr [ebp - 8], 0
        jne    SHORT loc_11_00CF
        test   byte ptr [edx + 2], 10h
        je     NEAR PTR loc_11_0180
loc_11_00CF:
        push   eax
        test   byte ptr [ebp - 5], 80h
        jne    SHORT loc_11_00E8
        mov    eax, dword ptr [edx]
        cmp    eax, dword ptr [esi]
        jne    SHORT loc_11_00E8
        mov    eax, dword ptr [edx + 4]
        cmp    eax, dword ptr [esi + 4]
        je     NEAR PTR loc_11_017F
loc_11_00E8:
        push   ecx
        push   edx
        push   esi
        mov    eax, dword ptr [esi]
        mov    dword ptr [edx], eax
        mov    eax, dword ptr [esi + 4]
        mov    dword ptr [edx + 4], eax
        mov    esi, edx
        test   byte ptr [esi + 2], 20h
        jne    SHORT loc_11_0120
        movzx  eax, byte ptr [esi]
        mov    eax, dword ptr [edi + eax*4 + 280h]
        or     eax, eax
        je     SHORT loc_11_0120
        test   byte ptr [eax + 2ch], 4
        jne    SHORT loc_11_0120
        movzx  ecx, byte ptr [esi + 1]
        movzx  edx, word ptr [esi + 4]
        add    edx, ecx
        cmp    edx, dword ptr [eax + 28h]
        jbe    SHORT loc_11_013A
loc_11_0120:
        and    byte ptr [esi + 2], 13h
        or     byte ptr [esi + 2], 20h
        mov    byte ptr [esi], 0ffh
        mov    byte ptr [esi + 1], 4
        mov    word ptr [esi + 4], 0
        db  0B8h, 0F2h, 00Eh, 000h, 000h
loc_11_013A:
        movzx  ecx, byte ptr [esi + 1]
        movzx  edx, word ptr [esi + 4]
        mov    esi, dword ptr [esp + 0ch]
        test   byte ptr [eax + 2ch], 80h
        jne    SHORT loc_11_01AA
        test   byte ptr [eax + 2ch], 20h
        jne    SHORT loc_11_0166
        db  083h, 03Dh, 098h, 000h, 000h, 000h, 000h
        jne    SHORT loc_11_0166
        db  0E8h, 045h, 00Dh, 0F5h, 0FFh
        jae    SHORT loc_11_0166
        xor    eax, eax
        jmp    SHORT loc_11_0177
loc_11_0166:
        push   10h
        push   edx
        push   ecx
        push   esi
        push   ebx
        push   dword ptr [eax]
        db  0CDh, 020h, 05Dh, 000h, 001h, 000h
        add    esp, 18h
loc_11_0177:
        or     eax, eax
        pop    esi
        pop    edx
        pop    ecx
        stc
        je     SHORT loc_11_019D
loc_11_017F:
        pop    eax
loc_11_0180:
        add    esi, 8
        add    edx, 8
        add    eax, 4
        dec    ecx
        jne    NEAR PTR loc_11_00A0
        btr    word ptr [edi + 68eh], 5
        clc
loc_11_019A:
        popad
        leave
        ret
loc_11_019D:
        pop    eax
        btr    word ptr [edi + 68eh], 5
        stc
        jmp    SHORT loc_11_019A
loc_11_01AA:
        push   0
        push   edx
        push   ecx
        push   esi
        push   ebx
        push   dword ptr [eax]
        db  0E8h, 01Ch, 00Fh, 0F5h, 0FFh
        add    esp, 18h
        jmp    SHORT loc_11_0177
sub_11_01BC:
        enter  0ch, 0
        db  083h, 03Dh, 098h, 000h, 000h, 000h, 000h
        je     NEAR PTR loc_11_03C7
        mov    dword ptr [ebp - 0ch], eax
        db  08Bh, 015h, 0FCh, 011h, 000h, 000h
        mov    edi, dword ptr [edx + ebx + 5ch]
        mov    edi, dword ptr [edi + 280h]
        mov    dword ptr [ebp - 4], 0
        mov    dword ptr [ebp - 8], 0
        db  08Bh, 035h, 0FCh, 011h, 000h, 000h
        push   0
        push   ebx
        push   dword ptr [ebp + 8]
        db  0FFh, 035h, 008h, 012h, 000h, 000h
        db  0CDh, 020h, 072h, 000h, 001h, 000h
        add    esp, 10h
        db  08Bh, 00Dh, 008h, 012h, 000h, 000h
        and    ecx, 3
        je     SHORT loc_11_0260
        sub    ecx, 4
        neg    ecx
        mov    dword ptr [ebp - 4], ecx
        push   dword ptr [ebp - 0ch]
        push   0
        push   0
        push   0
        push   0
        push   ebx
        push   0
        push   ecx
        db  0CDh, 020h, 053h, 000h, 001h, 000h
        add    esp, 20h
        test   eax, eax
        je     NEAR PTR loc_11_03C9
        mov    dword ptr [esi + ebx + 4], eax
        push   0
        push   0
        push   dword ptr [ebp - 4]
        db  0FFh, 035h, 008h, 012h, 000h, 000h
        push   ebx
        push   eax
        db  0CDh, 020h, 05Dh, 000h, 001h, 000h
        add    esp, 18h
        test   eax, eax
        je     NEAR PTR loc_11_03C9
loc_11_0260:
        mov    ecx, dword ptr [ebp + 8]
        db  003h, 00Dh, 008h, 012h, 000h, 000h
        and    ecx, 3
        je     SHORT loc_11_02DF
        db  0A1h, 008h, 012h, 000h, 000h
        add    eax, dword ptr [ebp + 8]
        sub    eax, ecx
        mov    edx, dword ptr [esi + ebx + 5ch]
        lea    edx, [edx + 4]
        test   eax, 3
        stc
        jne    SHORT loc_11_028B
        shl    eax, 1
        add    eax, edx
loc_11_028B:
        test   byte ptr [eax + 2], 1
        je     SHORT loc_11_02DF
        mov    dword ptr [ebp - 8], ecx
        push   dword ptr [ebp - 0ch]
        push   0
        push   0
        push   0
        push   0
        push   ebx
        push   0
        push   ecx
        db  0CDh, 020h, 053h, 000h, 001h, 000h
        add    esp, 20h
        test   eax, eax
        je     NEAR PTR loc_11_03C9
        mov    dword ptr [esi + ebx + 8], eax
        db  08Bh, 00Dh, 008h, 012h, 000h, 000h
        add    ecx, dword ptr [ebp + 8]
        sub    ecx, dword ptr [ebp - 8]
        push   0
        push   0
        push   dword ptr [ebp - 8]
        push   ecx
        push   ebx
        push   eax
        db  0CDh, 020h, 05Dh, 000h, 001h, 000h
        add    esp, 18h
        test   eax, eax
        je     NEAR PTR loc_11_03C9
loc_11_02DF:
        mov    ecx, dword ptr [ebp + 8]
        sub    ecx, dword ptr [ebp - 4]
        jbe    NEAR PTR loc_11_03C4
        sub    ecx, dword ptr [ebp - 8]
        jbe    NEAR PTR loc_11_03C4
        push   ecx
        push   dword ptr [ebp - 0ch]
        push   0
        push   0
        push   0
        push   0
        push   ebx
        push   0
        push   ecx
        db  0CDh, 020h, 053h, 000h, 001h, 000h
        add    esp, 20h
        pop    ecx
        test   eax, eax
        je     NEAR PTR loc_11_03C9
        mov    dword ptr [esi + ebx], eax
        mov    dword ptr [edi], eax
        mov    dword ptr [edi + 4], edx
        mov    dword ptr [edi + 28h], ecx
        mov    edx, dword ptr [esi + ebx + 5ch]
        mov    dword ptr [edx + 67ch], ecx
        db  001h, 00Dh, 00Ch, 000h, 000h, 000h
        and    byte ptr [edi + 2ch], 0fbh
        or     byte ptr [edi + 2ch], 40h
        db  08Bh, 015h, 008h, 012h, 000h, 000h
        add    edx, dword ptr [ebp - 4]
        push   0
        push   0
        push   ecx
        push   edx
        push   ebx
        push   eax
        db  0CDh, 020h, 05Dh, 000h, 001h, 000h
        add    esp, 18h
        or     eax, eax
        je     SHORT loc_11_03C9
        db  08Bh, 015h, 008h, 012h, 000h, 000h
        add    edx, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 8]
        sub    ecx, dword ptr [ebp - 4]
        sub    ecx, dword ptr [ebp - 8]
        push   ebx
        mov    edi, dword ptr [esi + ebx + 5ch]
        mov    ebx, ecx
        mov    bh, cl
        mov    bl, 4
        cmp    bh, bl
        jae    SHORT loc_11_037A
        mov    bl, bh
loc_11_037A:
        add    ecx, 3
        shr    ecx, 2
        lea    edi, [edi + 4]
        test   edx, 3
        stc
        jne    SHORT loc_11_0390
        shl    edx, 1
        add    edx, edi
loc_11_0390:
        xor    eax, eax
loc_11_0392:
        test   byte ptr [edx + 2], 3
        jne    SHORT loc_11_03A6
        mov    byte ptr [edx], 0
        mov    byte ptr [edx + 1], bl
        or     byte ptr [edx + 2], 40h
        mov    word ptr [edx + 4], ax
loc_11_03A6:
        add    edx, 8
        add    eax, 4
        sub    bh, 4
        jae    SHORT loc_11_03B6
        add    bh, 4
        mov    bl, bh
loc_11_03B6:
        loop   loc_11_0392
        pop    ebx
        mov    edi, dword ptr [esi + ebx + 5ch]
        or     byte ptr [edi + 68eh], 4
loc_11_03C4:
        xor    eax, eax
        inc    eax
loc_11_03C7:
        leave
        ret
loc_11_03C9:
        stc
        jmp    SHORT loc_11_03C7
loc_11_03CC:
        db  08Bh, 03Dh, 098h, 000h, 000h, 000h
        or     edi, edi
        je     NEAR PTR V86MMGR_VM_INIT
        db  0F6h, 005h, 004h, 012h, 000h, 000h, 004h
        jne    SHORT loc_11_041A
        lea    edx, [edi + 4]
        mov    ecx, 40h
loc_11_03EB:
        test   byte ptr [edx + 2], 1
        je     SHORT loc_11_03F8
        add    edx, 8
        loop   loc_11_03EB
        jmp    SHORT loc_11_041A
loc_11_03F8:
        lea    ecx, [edi + 4]
        sub    edx, ecx
        shr    edx, 1
        db  0CDh, 020h, 06Bh, 000h, 001h, 000h
        mov    ecx, eax
        and    ecx, 3
        je     SHORT loc_11_0413
        sub    ecx, 4
        neg    ecx
        add    eax, ecx
loc_11_0413:
        sub    eax, edx
        db  0A3h, 0A4h, 000h, 000h, 000h
loc_11_041A:
        db  0BEh, 0A8h, 000h, 000h, 000h
        lea    edx, [edi + 4]
        mov    ecx, 40h
        xor    eax, eax
loc_11_0429:
        test   byte ptr [edx + 2], 1
        jne    SHORT loc_11_0457
        test   byte ptr [edx + 2], 2
        jne    SHORT loc_11_0456
        push   eax
        push   ecx
        push   edx
        mov    ecx, edx
        lea    edx, [edi + 4]
        sub    ecx, edx
        shr    ecx, 1
        db  0CDh, 020h, 06Bh, 000h, 001h, 000h
        cmp    ecx, eax
        pop    edx
        pop    ecx
        pop    eax
        jae    SHORT loc_11_0456
        or     byte ptr [edx + 2], 2
        or     byte ptr [esi + 2], 2
loc_11_0456:
        inc    eax
loc_11_0457:
        add    edx, 8
        add    esi, 8
        loop   loc_11_0429
        mov    byte ptr [edi + 68fh], al
        mov    edx, eax
        shl    eax, 1
        add    eax, edx
        add    eax, 4
        mov    word ptr [edi + 688h], ax
V86MMGR_VM_INIT:
        db  08Bh, 035h, 0FCh, 011h, 000h, 000h
        mov    edi, dword ptr [esi + ebx + 5ch]
        or     edi, edi
        je     SHORT loc_11_04CB
        lea    edx, [esi + ebx + 64h]
        lea    eax, [edi + 4]
        mov    ecx, 40h
loc_11_048F:
        test   byte ptr [eax + 2], 3
        jne    SHORT loc_11_04C3
        test   byte ptr [edx + 1], 6
        jne    SHORT loc_11_04BF
        test   byte ptr [edx + 3], 6
        jne    SHORT loc_11_04BF
        test   byte ptr [edx + 5], 6
        jne    SHORT loc_11_04BF
        test   byte ptr [edx + 7], 6
        jne    SHORT loc_11_04BF
        or     byte ptr [edx + 1], 1
        or     byte ptr [edx + 3], 1
        or     byte ptr [edx + 5], 1
        or     byte ptr [edx + 7], 1
        jmp    SHORT loc_11_04C3
loc_11_04BF:
        or     byte ptr [eax + 2], 2
loc_11_04C3:
        add    eax, 8
        add    edx, 8
        loop   loc_11_048F
loc_11_04CB:
        clc
        ret
sub_11_04CD:
        db  08Bh, 035h, 098h, 000h, 000h, 000h
        or     esi, esi
        je     NEAR PTR loc_11_05BA
        db  08Bh, 03Dh, 0FCh, 011h, 000h, 000h
        push   101h
        push   692h
        db  0CDh, 020h, 04Fh, 000h, 001h, 000h
        pop    ecx
        pop    ecx
        or     eax, eax
        stc
        je     NEAR PTR loc_11_05BA
        mov    dword ptr [edi + ebx + 5ch], eax
        push   esi
        lea    edi, [eax + 240h]
        db  0BEh, 0A8h, 002h, 000h, 000h
        mov    ecx, 10h
        cld
        rep movsd
        lea    edi, [eax + 4]
        db  0BEh, 0A8h, 000h, 000h, 000h
        mov    ecx, 80h
        cld
        rep movsd
        mov    edi, eax
        db  066h, 0A1h, 0E8h, 002h, 000h, 000h
        mov    word ptr [edi + 68ah], ax
        pop    esi
        mov    eax, dword ptr [esi]
        mov    dword ptr [edi], eax
        mov    al, byte ptr [esi + 691h]
        mov    byte ptr [edi + 691h], al
        mov    ax, word ptr [esi + 688h]
        mov    word ptr [edi + 688h], ax
        mov    al, byte ptr [esi + 68fh]
        mov    byte ptr [edi + 68fh], al
        push   edi
        lea    esi, [esi + 280h]
        lea    edi, [edi + 280h]
        mov    ecx, 0ffh
loc_11_0570:
        cld
        lodsd
        or     eax, eax
        je     SHORT loc_11_057F
        test   byte ptr [eax + 2ch], 80h
        je     SHORT loc_11_057F
        stosd
        jmp    SHORT loc_11_0582
loc_11_057F:
        add    edi, 4
loc_11_0582:
        loop   loc_11_0570
        pop    edi
        push   101h
        push   36h
        db  0CDh, 020h, 04Fh, 000h, 001h, 000h
        pop    ecx
        pop    ecx
        or     eax, eax
        je     SHORT loc_11_05BB
        mov    dword ptr [edi + 280h], eax
        xor    ecx, ecx
        mov    dword ptr [eax], ecx
        mov    dword ptr [eax + 28h], ecx
        dec    ecx
        mov    dword ptr [eax + 4], ecx
        mov    byte ptr [eax + 2ch], 44h
        lea    eax, [edi + 4]
        mov    ecx, 80000001h
        call   sub_11_0074
loc_11_05BA:
        ret
loc_11_05BB:
        db  08Bh, 03Dh, 0FCh, 011h, 000h, 000h
        xor    eax, eax
        xchg   dword ptr [edi + ebx + 5ch], eax
        push   0
        push   eax
        db  0CDh, 020h, 051h, 000h, 001h, 000h
        pop    ecx
        pop    ecx
        stc
        jmp    SHORT loc_11_05BA
        db  000h, 000h, 000h
sub_11_05D8:
        db  0CDh, 020h, 06Ah, 000h, 001h, 000h
        mov    edi, eax
        db  08Bh, 035h, 0B4h, 00Eh, 000h, 000h
loc_11_05E6:
        db  00Fh, 0B7h, 00Ch, 075h, 028h, 00Fh, 000h, 000h
        test   ch, 4
        je     SHORT loc_11_060A
        test   ch, 80h
        jne    SHORT loc_11_060A
        push   0
        push   0
        push   1
        push   esi
        push   ebx
        push   edi
        db  0CDh, 020h, 05Dh, 000h, 001h, 000h
        add    esp, 18h
loc_11_060A:
        inc    esi
        db  03Bh, 035h, 0B8h, 00Eh, 000h, 000h
        jbe    SHORT loc_11_05E6
        clc
        ret
        db  000h, 000h, 000h
V86MMGR_CREATE_VM:
        db  08Bh, 03Dh, 0FCh, 011h, 000h, 000h
        lea    edi, [edi + ebx + 64h]
        db  0BEh, 028h, 00Fh, 000h, 000h
        mov    ecx, 88h
        cld
        rep movsd
        db  08Bh, 03Dh, 0FCh, 011h, 000h, 000h
        or     byte ptr [edi + ebx + 2ch], 8
        call   sub_11_05D8
        jb     SHORT loc_11_065E
        call   0
        jb     SHORT loc_11_065E
        call   sub_11_04CD
        jb     SHORT loc_11_065E
        call   sub_11_0C9C
        jb     SHORT loc_11_065D
        call   sub_11_0CE9
        jb     SHORT loc_11_065E
loc_11_065D:
        ret
loc_11_065E:
        xor    ebx, ebx
        mov    eax, 20008h
        db  08Dh, 015h, 0B8h, 011h, 000h, 000h
        xor    ecx, ecx
        inc    ecx
        db  0CDh, 020h, 0E0h, 000h, 001h, 000h
        stc
        jmp    SHORT loc_11_065D
V86MMGR_Allocate_V86_Pages:
        enter  2ch, 0
        pushad
        mov    dword ptr [ebp - 4], 0
        db  0CDh, 020h, 004h, 000h, 001h, 000h
        jne    SHORT loc_11_069A
        pushad
        db  0E8h, 05Fh, 028h, 0F5h, 0FFh
        popad
        jae    SHORT loc_11_069A
        db  0CDh, 020h, 0BFh, 080h, 001h, 000h
loc_11_069A:
        db  0CDh, 020h, 06Bh, 000h, 001h, 000h
        db  0A3h, 008h, 012h, 000h, 000h
        db  0CDh, 020h, 0D5h, 000h, 001h, 000h
        db  0A3h, 00Ch, 012h, 000h, 000h
        inc    eax
        shl    eax, 2
        db  0A3h, 010h, 012h, 000h, 000h
        mov    ecx, esi
        xor    eax, eax
        db  0E8h, 04Ah, 0FFh, 0F4h, 0FFh
        mov    esi, ecx
        db  0A1h, 010h, 012h, 000h, 000h
        mov    ecx, edi
        cmp    ecx, eax
        ja     NEAR PTR loc_11_091A
        mov    eax, 1
        db  0E8h, 02Fh, 0FFh, 0F4h, 0FFh
        mov    edi, ecx
        lea    edx, [ebp - 2ch]
        push   0
        push   edx
        push   ebx
        db  0CDh, 020h, 074h, 000h, 001h, 000h
        add    esp, 0ch
        or     eax, eax
        je     NEAR PTR loc_11_0906
        push   edi
        push   esi
        lea    edx, [ebp - 2ch]
        db  08Bh, 035h, 008h, 012h, 000h, 000h
        mov    edi, esi
        mov    eax, esi
        and    esi, 1fh
        shr    edi, 5
        or     esi, esi
        je     SHORT loc_11_0722
        mov    ecx, esi
        sub    ecx, 20h
        neg    ecx
loc_11_0717:
        bt     dword ptr [edx + edi*4], esi
        jb     SHORT loc_11_0738
        inc    esi
        inc    eax
        loop   loc_11_0717
        inc    edi
loc_11_0722:
        db  03Bh, 005h, 00Ch, 012h, 000h, 000h
        ja     SHORT loc_11_0738
        bsf    esi, dword ptr [edx + edi*4]
        jne    SHORT loc_11_0736
        add    eax, 20h
        inc    edi
        jmp    SHORT loc_11_0722
loc_11_0736:
        add    eax, esi
loc_11_0738:
        db  08Bh, 015h, 00Ch, 012h, 000h, 000h
        inc    edx
        cmp    eax, edx
        jbe    SHORT loc_11_0745
        mov    eax, edx
loc_11_0745:
        mov    dword ptr [ebp - 8], eax
        db  02Bh, 005h, 008h, 012h, 000h, 000h
        pop    esi
        pop    edi
        cmp    eax, edi
        jb     NEAR PTR loc_11_091A
        cmp    eax, esi
        jae    SHORT loc_11_075E
        mov    esi, eax
loc_11_075E:
        push   0
        db  0CDh, 020h, 059h, 000h, 001h, 000h
        pop    ecx
        sub    edx, 14h
        jae    SHORT loc_11_076E
        xor    edx, edx
loc_11_076E:
        mov    ecx, esi
        test   byte ptr [esp + 18h], 1
        jne    SHORT loc_11_0782
        db  080h, 03Dh, 015h, 012h, 000h, 000h, 003h
        jne    SHORT loc_11_0782
        mov    edx, eax
loc_11_0782:
        cmp    ecx, edx
        jbe    SHORT loc_11_0790
        cmp    edi, edx
        ja     NEAR PTR loc_11_0913
        mov    ecx, edx
loc_11_0790:
        xor    eax, eax
        test   byte ptr [esp + 18h], 1
        je     SHORT loc_11_07A4
        db  08Bh, 015h, 0FCh, 011h, 000h, 000h
        or     byte ptr [edx + ebx + 2eh], 1
loc_11_07A4:
        mov    dword ptr [ebp - 4], ecx
        db  08Bh, 015h, 0FCh, 011h, 000h, 000h
        mov    dword ptr [edx + ebx + 0ch], ecx
        push   ecx
        call   sub_11_01BC
        pop    edx
        jb     NEAR PTR loc_11_0913
        jne    SHORT loc_11_081E
        push   eax
        push   0
        push   0
        push   0
        push   0
        push   ebx
        push   0
        push   ecx
        db  0CDh, 020h, 053h, 000h, 001h, 000h
        add    esp, 20h
        test   eax, eax
        je     NEAR PTR loc_11_0913
        db  08Bh, 00Dh, 0FCh, 011h, 000h, 000h
        mov    dword ptr [ecx + ebx], eax
        mov    edi, eax
        push   0
        push   ebx
        push   dword ptr [ebp - 4]
        db  0FFh, 035h, 008h, 012h, 000h, 000h
        db  0CDh, 020h, 072h, 000h, 001h, 000h
        add    esp, 10h
        push   0
        push   0
        push   dword ptr [ebp - 4]
        db  0FFh, 035h, 008h, 012h, 000h, 000h
        push   ebx
        push   edi
        db  0CDh, 020h, 05Dh, 000h, 001h, 000h
        add    esp, 18h
        or     eax, eax
        je     NEAR PTR loc_11_0913
loc_11_081E:
        db  08Bh, 015h, 0FCh, 011h, 000h, 000h
        test   byte ptr [edx + ebx + 2eh], 1
        je     SHORT loc_11_083E
        push   800h
        push   0
        push   0
        push   ebx
        db  0CDh, 020h, 065h, 000h, 001h, 000h
        add    esp, 10h
loc_11_083E:
        db  0A1h, 008h, 012h, 000h, 000h
        mov    ecx, dword ptr [ebp - 4]
        lea    eax, [ebx + eax*2 + 64h]
        db  003h, 005h, 0FCh, 011h, 000h, 000h
loc_11_0850:
        or     word ptr [eax], 1010h
        add    eax, 2
        loop   loc_11_0850
        mov    esi, dword ptr [ebp - 4]
        db  003h, 035h, 008h, 012h, 000h, 000h
        mov    edi, dword ptr [ebp - 8]
        sub    edi, esi
        jbe    SHORT loc_11_08DA
        push   0
        push   0
        push   edi
        push   esi
        push   ebx
        db  0FFh, 035h, 000h, 012h, 000h, 000h
        db  0CDh, 020h, 05Dh, 000h, 001h, 000h
        add    esp, 18h
        db  083h, 03Dh, 098h, 000h, 000h, 000h, 000h
        je     SHORT loc_11_08DA
        db  08Bh, 015h, 0FCh, 011h, 000h, 000h
        mov    edx, dword ptr [edx + ebx + 5ch]
        lea    edx, [edx + 4]
        mov    eax, esi
        and    eax, 3
        je     SHORT loc_11_08A2
        sub    eax, 4
        neg    eax
loc_11_08A2:
        add    esi, eax
        sub    edi, eax
        jbe    SHORT loc_11_08DA
        shr    edi, 2
        or     edi, edi
        je     SHORT loc_11_08DA
        mov    ecx, edi
        test   esi, 3
        stc
        jne    SHORT loc_11_08BE
        shl    esi, 1
        add    esi, edx
loc_11_08BE:
        test   byte ptr [esi + 2], 3
        jne    SHORT loc_11_08D5
        mov    byte ptr [esi], 0ffh
        mov    byte ptr [esi + 1], 4
        or     byte ptr [esi + 2], 20h
        mov    word ptr [esi + 4], 0
loc_11_08D5:
        add    esi, 8
        loop   loc_11_08BE
loc_11_08DA:
        mov    eax, dword ptr [ebp - 8]
        db  08Bh, 015h, 00Ch, 012h, 000h, 000h
        inc    edx
        cmp    eax, edx
        jae    SHORT loc_11_08FF
        mov    ecx, edx
        sub    ecx, eax
        lea    eax, [ebx + eax*2 + 64h]
        db  003h, 005h, 0FCh, 011h, 000h, 000h
loc_11_08F6:
        or     byte ptr [eax + 1], 8
        add    eax, 2
        loop   loc_11_08F6
loc_11_08FF:
        clc
loc_11_0900:
        popad
        mov    eax, dword ptr [ebp - 4]
        leave
        ret
loc_11_0906:
        mov    eax, 20008h
        db  08Dh, 015h, 0B8h, 011h, 000h, 000h
        jmp    SHORT loc_11_0921
loc_11_0913:
        mov    eax, 20001h
        jmp    SHORT loc_11_091F
loc_11_091A:
        mov    eax, 20002h
loc_11_091F:
        xor    edx, edx
loc_11_0921:
        push   ebx
        test   byte ptr [ebx], 4
        je     SHORT loc_11_0929
        xor    ebx, ebx
loc_11_0929:
        xor    ecx, ecx
        inc    ecx
        db  0CDh, 020h, 0E0h, 000h, 001h, 000h
        pop    ebx
        db  08Bh, 035h, 0FCh, 011h, 000h, 000h
        xor    ecx, ecx
        xchg   dword ptr [esi + ebx + 4], ecx
        jecxz  loc_11_094C
        push   0
        push   ecx
        db  0CDh, 020h, 055h, 000h, 001h, 000h
        pop    ecx
        pop    ecx
loc_11_094C:
        xor    ecx, ecx
        xchg   dword ptr [esi + ebx], ecx
        jecxz  loc_11_095E
        push   0
        push   ecx
        db  0CDh, 020h, 055h, 000h, 001h, 000h
        pop    ecx
        pop    ecx
loc_11_095E:
        xor    ecx, ecx
        xchg   dword ptr [esi + ebx + 8], ecx
        jecxz  loc_11_0971
        push   0
        push   ecx
        db  0CDh, 020h, 055h, 000h, 001h, 000h
        pop    ecx
        pop    ecx
loc_11_0971:
        and    dword ptr [ebp - 4], 0
        stc
        jmp    SHORT loc_11_0900
V86MMGR_ResetBasePages:
        pushad
        or     ecx, ecx
        je     NEAR PTR loc_11_0AAE
        db  08Bh, 035h, 0FCh, 011h, 000h, 000h
        mov    edx, dword ptr [esi + ebx]
        or     edx, dword ptr [esi + ebx + 4]
        or     edx, dword ptr [esi + ebx + 8]
        je     NEAR PTR loc_11_0AAE
        cmp    dword ptr [esi + ebx + 5ch], 0
        je     NEAR PTR loc_11_0AAE
        mov    edx, eax
        add    ecx, edx
        dec    ecx
        db  0CDh, 020h, 0D5h, 000h, 001h, 000h
        cmp    edx, eax
        ja     NEAR PTR loc_11_0AAE
        cmp    ecx, eax
        ja     NEAR PTR loc_11_0AAE
        db  0CDh, 020h, 06Bh, 000h, 001h, 000h
        cmp    edx, eax
        jb     NEAR PTR loc_11_0AAE
        mov    edi, edx
        sub    edx, eax
        mov    ecx, dword ptr [esp + 18h]
        add    ecx, edx
        dec    ecx
        cmp    edx, dword ptr [esi + ebx + 0ch]
        jae    NEAR PTR loc_11_0AAE
        cmp    ecx, dword ptr [esi + ebx + 0ch]
        jae    NEAR PTR loc_11_0AAE
        mov    ecx, dword ptr [esp + 18h]
        mov    eax, dword ptr [esi + ebx + 4]
        or     eax, eax
        je     SHORT loc_11_0A34
        push   edx
        push   ecx
        push   0
        push   eax
        db  0CDh, 020h, 058h, 000h, 001h, 000h
        pop    ecx
        pop    ecx
        pop    ecx
        pop    edx
        cmp    eax, edx
        jbe    SHORT loc_11_0A32
        sub    eax, edx
        cmp    eax, ecx
        jbe    SHORT loc_11_0A12
        mov    eax, ecx
loc_11_0A12:
        push   ecx
        push   eax
        push   0
        push   edi
        push   eax
        push   edx
        push   dword ptr [esi + ebx + 4]
        db  0CDh, 020h, 0D3h, 000h, 001h, 000h
        add    esp, 14h
        pop    eax
        pop    ecx
        sub    ecx, eax
        jbe    SHORT loc_11_0AAB
        xor    edx, edx
        add    edi, eax
        jmp    SHORT loc_11_0A34
loc_11_0A32:
        sub    edx, eax
loc_11_0A34:
        mov    eax, dword ptr [esi + ebx]
        or     eax, eax
        je     SHORT loc_11_0A77
        push   edx
        push   ecx
        push   0
        push   eax
        db  0CDh, 020h, 058h, 000h, 001h, 000h
        pop    ecx
        pop    ecx
        pop    ecx
        pop    edx
        cmp    eax, edx
        jbe    SHORT loc_11_0A75
        sub    eax, edx
        cmp    eax, ecx
        jbe    SHORT loc_11_0A56
        mov    eax, ecx
loc_11_0A56:
        push   ecx
        push   eax
        push   0
        push   edi
        push   eax
        push   edx
        push   dword ptr [esi + ebx]
        db  0CDh, 020h, 0D3h, 000h, 001h, 000h
        add    esp, 14h
        pop    eax
        pop    ecx
        sub    ecx, eax
        jbe    SHORT loc_11_0AAB
        xor    edx, edx
        add    edi, eax
        jmp    SHORT loc_11_0A77
loc_11_0A75:
        sub    edx, eax
loc_11_0A77:
        mov    eax, dword ptr [esi + ebx + 8]
        or     eax, eax
        je     SHORT loc_11_0AAB
        push   edx
        push   ecx
        push   0
        push   eax
        db  0CDh, 020h, 058h, 000h, 001h, 000h
        pop    ecx
        pop    ecx
        pop    ecx
        pop    edx
        cmp    eax, edx
        jbe    SHORT loc_11_0AAB
        sub    eax, edx
        cmp    eax, ecx
        jbe    SHORT loc_11_0A9A
        mov    eax, ecx
loc_11_0A9A:
        push   0
        push   edi
        push   eax
        push   edx
        push   dword ptr [esi + ebx]
        db  0CDh, 020h, 0D3h, 000h, 001h, 000h
        add    esp, 14h
loc_11_0AAB:
        clc
loc_11_0AAC:
        popad
        ret
loc_11_0AAE:
        stc
        jmp    SHORT loc_11_0AAC
V86MMGR_Set_EMS_XMS_Limits:
        enter  0, 0
        pushad
        pushfd
        and    byte ptr [ebp - 24h], 0feh
        db  08Bh, 00Dh, 0FCh, 011h, 000h, 000h
        cmp    eax, edx
        jne    SHORT loc_11_0ACF
        inc    eax
        je     NEAR PTR loc_11_0B65
        dec    eax
        je     SHORT loc_11_0B29
loc_11_0ACF:
        add    eax, 3
        shr    eax, 2
        cmp    edx, -1
        je     SHORT loc_11_0AE0
        add    edx, 3
        shr    edx, 2
loc_11_0AE0:
        test   byte ptr [ecx + ebx + 2fh], 80h
        jne    SHORT loc_11_0B29
        or     eax, eax
        je     SHORT loc_11_0B29
        push   eax
        push   edx
        push   ecx
        mov    dword ptr [ecx + ebx + 48h], eax
        mov    dword ptr [ecx + ebx + 4ch], eax
        xor    ecx, ecx
        test   byte ptr [ebp - 8], 8
        je     SHORT loc_11_0B04
        or     ecx, 80h
loc_11_0B04:
        push   ecx
        push   0
        push   0
        push   0
        push   0
        push   ebx
        push   0
        push   eax
        db  0CDh, 020h, 053h, 000h, 001h, 000h
        add    esp, 20h
        or     eax, eax
        pop    ecx
        mov    dword ptr [ecx + ebx + 44h], eax
        pop    edx
        pop    eax
        je     NEAR PTR loc_11_0C7D
loc_11_0B29:
        test   byte ptr [ecx + ebx + 2fh], 80h
        jne    SHORT loc_11_0B65
        mov    dword ptr [ecx + ebx + 50h], eax
        mov    dword ptr [ecx + ebx + 54h], edx
        mov    edx, dword ptr [ecx + ebx + 54h]
        or     edx, dword ptr [ecx + ebx + 50h]
        mov    eax, 80000000h
        jne    SHORT loc_11_0B4A
        or     eax, 2
loc_11_0B4A:
        db  0F6h, 005h, 004h, 012h, 000h, 000h, 008h
        jne    SHORT loc_11_0B59
        test   byte ptr [ebp - 8], 8
        je     SHORT loc_11_0B5C
loc_11_0B59:
        or     eax, 20h
loc_11_0B5C:
        and    byte ptr [ecx + ebx + 2ch], 0ddh
        or     dword ptr [ecx + ebx + 2ch], eax
loc_11_0B65:
        cmp    esi, edi
        jne    SHORT loc_11_0B73
        inc    esi
        je     NEAR PTR loc_11_0C05
        dec    esi
        je     SHORT loc_11_0BC9
loc_11_0B73:
        add    esi, 3
        shr    esi, 2
        cmp    edi, -1
        je     SHORT loc_11_0B84
        add    edi, 3
        shr    edi, 2
loc_11_0B84:
        test   byte ptr [ecx + ebx + 2fh], 40h
        jne    SHORT loc_11_0BC9
        or     esi, esi
        je     SHORT loc_11_0BC9
        push   ecx
        mov    dword ptr [ecx + ebx + 34h], esi
        mov    dword ptr [ecx + ebx + 38h], esi
        xor    ecx, ecx
        test   byte ptr [ebp - 8], 4
        je     SHORT loc_11_0BA6
        or     ecx, 80h
loc_11_0BA6:
        push   ecx
        push   0
        push   0
        push   0
        push   0
        push   ebx
        push   0
        push   esi
        db  0CDh, 020h, 053h, 000h, 001h, 000h
        add    esp, 20h
        or     eax, eax
        pop    ecx
        mov    dword ptr [ecx + ebx + 30h], eax
        je     NEAR PTR loc_11_0C76
loc_11_0BC9:
        test   byte ptr [ecx + ebx + 2fh], 40h
        jne    SHORT loc_11_0C05
        mov    dword ptr [ecx + ebx + 3ch], esi
        mov    dword ptr [ecx + ebx + 40h], edi
        mov    edx, dword ptr [ecx + ebx + 40h]
        or     edx, dword ptr [ecx + ebx + 3ch]
        mov    eax, 40000000h
        jne    SHORT loc_11_0BEA
        or     eax, 1
loc_11_0BEA:
        db  0F6h, 005h, 004h, 012h, 000h, 000h, 010h
        jne    SHORT loc_11_0BF9
        test   byte ptr [ebp - 8], 4
        je     SHORT loc_11_0BFC
loc_11_0BF9:
        or     eax, 10h
loc_11_0BFC:
        and    byte ptr [ecx + ebx + 2ch], 0eeh
        or     dword ptr [ecx + ebx + 2ch], eax
loc_11_0C05:
        test   byte ptr [ebp - 8], 3
        je     SHORT loc_11_0C19
        xor    eax, eax
        test   byte ptr [ebp - 8], 1
        jne    SHORT loc_11_0C14
        inc    eax
loc_11_0C14:
        db  0E8h, 01Eh, 0F5h, 0F4h, 0FFh
loc_11_0C19:
        test   byte ptr [ebx], 8
        je     SHORT loc_11_0C72
        mov    edi, dword ptr [ecx + ebx + 44h]
        or     edi, edi
        je     SHORT loc_11_0C48
        test   byte ptr [ebp - 8], 8
        je     SHORT loc_11_0C48
        push   ecx
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
        pop    ecx
loc_11_0C48:
        mov    edi, dword ptr [ecx + ebx + 30h]
        or     edi, edi
        je     SHORT loc_11_0C72
        test   byte ptr [ebp - 8], 4
        je     SHORT loc_11_0C72
        push   ecx
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
        pop    ecx
loc_11_0C72:
        popfd
        popad
        leave
        ret
loc_11_0C76:
        mov    eax, 20003h
        jmp    SHORT loc_11_0C82
loc_11_0C7D:
        mov    eax, 20004h
loc_11_0C82:
        test   byte ptr [ebx], 4
        je     SHORT loc_11_0C89
        xor    ebx, ebx
loc_11_0C89:
        xor    edx, edx
        xor    ecx, ecx
        inc    ecx
        db  0CDh, 020h, 0E0h, 000h, 001h, 000h
        or     byte ptr [ebp - 24h], 1
        jmp    SHORT loc_11_0C72
        db  000h, 000h
sub_11_0C9C:
        pushad
        db  083h, 03Dh, 030h, 013h, 000h, 000h, 0FFh
        je     SHORT loc_11_0CD3
        db  083h, 03Dh, 030h, 013h, 000h, 000h, 000h
        mov    eax, 4
        je     SHORT loc_11_0CC5
        mov    eax, 2
        db  0F6h, 005h, 004h, 012h, 000h, 000h, 020h
        jne    SHORT loc_11_0CC5
        or     eax, 1
loc_11_0CC5:
        push   eax
        push   ebx
        db  0CDh, 020h, 07Dh, 000h, 001h, 000h
        pop    ecx
        pop    ecx
        or     eax, eax
        je     SHORT loc_11_0CD6
loc_11_0CD3:
        clc
loc_11_0CD4:
        popad
        ret
loc_11_0CD6:
        mov    eax, 20005h
        xor    edx, edx
        xor    ecx, ecx
        inc    ecx
        db  0CDh, 020h, 0E0h, 000h, 001h, 000h
        stc
        jmp    SHORT loc_11_0CD4
sub_11_0CE9:
        db  08Bh, 035h, 0B0h, 012h, 000h, 000h
        db  0CDh, 020h, 09Dh, 000h, 001h, 000h
        db  08Bh, 035h, 0FCh, 011h, 000h, 000h
        db  00Fh, 082h, 0DCh, 0F3h, 0F4h, 0FFh
        xchg   edi, eax
        push   edi
        db  08Bh, 00Dh, 0ACh, 012h, 000h, 000h
        add    ecx, 7
        xor    eax, eax
        cld
        rep stosd
        pop    edi
        mov    dword ptr [esi + ebx + 60h], edi
        db  08Bh, 015h, 030h, 013h, 000h, 000h
        mov    word ptr [edi + 16h], dx
        db  0A1h, 000h, 012h, 000h, 000h
        mov    dword ptr [edi], eax
        lea    esi, [edi + 0ch]
        mov    dword ptr [esi + 4], 7fffffffh
        mov    dword ptr [esi], esi
        push   edi
        lea    edi, [edi + 1ch]
        db  08Bh, 035h, 0BCh, 012h, 000h, 000h
        db  08Bh, 00Dh, 0ACh, 012h, 000h, 000h
        jecxz  loc_11_0D53
loc_11_0D45:
        lodsw
        cmp    ax, -1
        je     SHORT loc_11_0D51
        movzx  eax, ax
        stosd
loc_11_0D51:
        loop   loc_11_0D45
loc_11_0D53:
        pop    edi
        db  08Bh, 035h, 0FCh, 011h, 000h, 000h
        clc
        ret
_PTEXT ends
        end
