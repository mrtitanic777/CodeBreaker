; deccore_pcode_02.asm -- DECCORE object 2 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

loc_2_0000:
        cmp    eax, 0
        jne    SHORT loc_2_000A
        db  0E9h, 0F6h, 0FFh, 0FEh, 0FFh
loc_2_000A:
        cmp    eax, 1
        jne    SHORT loc_2_0014
        db  0E9h, 0F3h, 0FFh, 0FEh, 0FFh
loc_2_0014:
        cmp    eax, 2
        jne    SHORT loc_2_001E
        db  0E9h, 022h, 000h, 0FFh, 0FFh
loc_2_001E:
        cmp    eax, 23h
        jne    SHORT loc_2_0028
        jmp    NEAR PTR loc_2_0257
loc_2_0028:
        cmp    eax, 4
        jne    SHORT loc_2_002F
        jmp    SHORT loc_2_0038
loc_2_002F:
        cmp    eax, 0bh
        jne    SHORT loc_2_0036
        jmp    SHORT loc_2_0038
loc_2_0036:
        clc
        ret
loc_2_0038:
        db  03Bh, 01Dh, 0DCh, 00Fh, 002h, 000h
        jne    SHORT loc_2_004A
        db  081h, 00Dh, 0D8h, 00Fh, 002h, 000h, 000h, 001h, 000h, 000h
loc_2_004A:
        ret
loc_2_004B:
        xor    ecx, ecx
        mov    eax, 1019h
        db  00Fh, 0BAh, 025h, 0D8h, 00Fh, 002h, 000h, 001h
        jae    SHORT loc_2_005D
        ret
loc_2_005D:
        db  0B9h, 018h, 002h, 002h, 000h
        ret
loc_2_0063:
        movzx  eax, word ptr [ebp + 1ch]
        test   eax, eax
        jne    SHORT loc_2_0096
        mov    word ptr [ebp + 1ch], ax
        and    word ptr [ebp + 2ch], 0fffeh
        mov    word ptr [ebp + 10h], 1019h
        db  08Bh, 015h, 018h, 012h, 002h, 000h
        mov    word ptr [ebp + 18h], dx
        db  08Bh, 015h, 0B4h, 010h, 002h, 000h
        mov    word ptr [ebp + 4], dx
        shr    edx, 10h
        mov    word ptr [ebp], dx
        ret
loc_2_0096:
        cmp    eax, 6
        ja     SHORT loc_2_00A0
        jmp    NEAR PTR loc_2_0657
loc_2_00A0:
        cmp    eax, 7
        ja     SHORT loc_2_00AA
        jmp    NEAR PTR loc_2_0DFC
loc_2_00AA:
        or     word ptr [ebp + 2ch], 1
        ret
loc_2_00B0:
        cmp    cl, 1
        jb     SHORT loc_2_00CC
        cmp    cl, 2
        je     NEAR PTR loc_2_01EF
        ja     SHORT loc_2_00E1
        db  0CDh, 020h, 0CFh, 000h, 001h, 000h
        mov    ecx, dword ptr [ebp + 0ch]
        mov    dword ptr [ecx], eax
        ret
loc_2_00CC:
        mov    ecx, 0ah
        db  0BEh, 004h, 010h, 002h, 000h
loc_2_00D6:
        cmp    dword ptr [esi + 8], 0
        je     SHORT loc_2_00E9
        add    esi, 0ch
        loop   loc_2_00D6
loc_2_00E1:
        mov    dword ptr [ebp - 4], 0ffffff90h
        ret
loc_2_00E9:
        mov    ecx, 37h
        mov    eax, dword ptr [ebp + 10h]
        xor    edx, edx
        div    ecx
        cmp    edx, 1bh
        jl     SHORT loc_2_00FB
        inc    eax
loc_2_00FB:
        mov    dword ptr [esi + 4], eax
        mov    dword ptr [esi], eax
        mov    edx, dword ptr [ebp + 0ch]
        mov    dword ptr [esi + 8], edx
        ret
        db  090h, 090h, 090h
loc_2_010A:
        mov    eax, 37h
        db  003h, 00Dh, 0FCh, 00Fh, 002h, 000h
        sub    ecx, 37h
        neg    ecx
        add    eax, ecx
        db  0A3h, 0FCh, 00Fh, 002h, 000h
        cmp    eax, 14h
        jl     SHORT loc_2_0190
loc_2_0126:
        call   sub_2_01E1
        db  00Fh, 0BAh, 02Dh, 0D8h, 00Fh, 002h, 000h, 002h
        jb     SHORT loc_2_0183
loc_2_0135:
        db  0BEh, 004h, 010h, 002h, 000h
        mov    ecx, 0ah
loc_2_013F:
        cmp    dword ptr [esi + 8], 0
        je     SHORT loc_2_0162
        dec    dword ptr [esi]
        jne    SHORT loc_2_0162
        mov    edx, dword ptr [esi + 4]
        cmp    eax, 4fh
        jl     SHORT loc_2_0157
        cmp    edx, 4
        jl     SHORT loc_2_0157
        dec    edx
loc_2_0157:
        mov    dword ptr [esi], edx
        pushad
        xor    ebx, ebx
        mov    ebp, esp
        call   dword ptr [esi + 8]
        popad
loc_2_0162:
        add    esi, 0ch
        loop   loc_2_013F
        db  00Fh, 0BAh, 035h, 0D8h, 00Fh, 002h, 000h, 003h
        jb     SHORT loc_2_0135
        db  00Fh, 0BAh, 035h, 0D8h, 00Fh, 002h, 000h, 006h
        jb     SHORT loc_2_0135
        db  083h, 025h, 0D8h, 00Fh, 002h, 000h, 0FBh
        ret
loc_2_0183:
        db  08Bh, 015h, 0FCh, 00Fh, 002h, 000h
        db  001h, 015h, 000h, 010h, 002h, 000h
        ret
loc_2_0190:
        add    eax, 37h
        db  083h, 00Dh, 0D8h, 00Fh, 002h, 000h, 008h
        db  0C7h, 005h, 0FCh, 00Fh, 002h, 000h, 037h, 000h, 000h, 000h
        cmp    eax, 14h
        jge    NEAR PTR loc_2_0126
        add    eax, 37h
        db  083h, 00Dh, 0D8h, 00Fh, 002h, 000h, 040h
        cmp    eax, 14h
        jge    NEAR PTR loc_2_0126
        mov    edx, 14h
        sub    edx, eax
        je     SHORT loc_2_01CD
        jns    SHORT loc_2_01D0
        neg    edx
loc_2_01CD:
        add    edx, 14h
loc_2_01D0:
        db  001h, 015h, 000h, 010h, 002h, 000h
        mov    eax, 37h
        jmp    NEAR PTR loc_2_0126
        db  090h
sub_2_01E1:
        xor    edx, edx
        db  0BEh, 00Ah, 001h, 002h, 000h
        db  0CDh, 020h, 03Ch, 000h, 001h, 000h
        ret
loc_2_01EF:
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, 0ah
        db  0BEh, 004h, 010h, 002h, 000h
loc_2_01FC:
        cmp    dword ptr [esi + 8], eax
        je     SHORT loc_2_020B
        add    esi, 0ch
        loop   loc_2_01FC
        jmp    NEAR PTR loc_2_00E1
loc_2_020B:
        xor    ecx, ecx
        mov    dword ptr [esi + 8], ecx
        mov    dword ptr [esi + 4], ecx
        mov    dword ptr [esi], ecx
        ret
        db  000h, 000h
sub_2_0218:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   ebx
        push   edi
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 4], 0fffffdabh
        cmp    byte ptr [ebp + 9], 2
        je     SHORT loc_2_023B
        db  00Fh, 0BAh, 025h, 0D8h, 00Fh, 002h, 000h, 001h
        jb     SHORT loc_2_0250
loc_2_023B:
        movzx  eax, byte ptr [ebp + 9]
        cld
        mov    dword ptr [ebp - 4], 0
        shl    eax, 2
        db  0FFh, 090h, 088h, 010h, 002h, 000h
loc_2_0250:
        cld
        pop    edi
        pop    ebx
        pop    esi
        pop    eax
        pop    ebp
        ret
loc_2_0257:
        db  08Bh, 01Dh, 0DCh, 00Fh, 002h, 000h
        mov    ebp, dword ptr [ebx + 8]
        mov    ecx, dword ptr [esi + 0ch]
        jecxz  loc_2_02A1
        cmp    ecx, -1
        je     SHORT loc_2_02A6
        cmp    ecx, 201h
        je     SHORT loc_2_027F
        mov    eax, 16h
        cmp    ecx, 200h
        jne    SHORT loc_2_02A8
loc_2_027F:
        mov    eax, 57h
        mov    edx, dword ptr [esi + 10h]
        cmp    edx, 400000h
        jb     SHORT loc_2_02A8
        cmp    dword ptr [esi + 24h], 0
        jne    SHORT loc_2_02A8
        push   edx
        push   ecx
        call   sub_2_0218
        add    esp, 8
        clc
        ret
loc_2_02A1:
        mov    ecx, 1019h
loc_2_02A6:
        xor    eax, eax
loc_2_02A8:
        clc
        ret
        db  000h, 000h
loc_2_02AC:
        mov    edx, dword ptr [ebp + 0ch]
        cmp    cl, 1
        jb     SHORT loc_2_02BE
        je     SHORT loc_2_02DC
loc_2_02B6:
        mov    dword ptr [ebp - 4], 0ffffff95h
        ret
loc_2_02BE:
        mov    edi, edx
        push   1
        push   edx
        db  0CDh, 020h, 04Fh, 000h, 001h, 000h
        add    esp, 8
        or     eax, eax
        je     SHORT loc_2_02B6
        mov    edx, dword ptr [ebp + 10h]
        mov    dword ptr [edx], eax
        db  001h, 03Dh, 010h, 011h, 002h, 000h
        ret
loc_2_02DC:
        mov    ecx, dword ptr [edx]
        push   ecx
        push   0
        push   ecx
        db  0CDh, 020h, 052h, 000h, 001h, 000h
        add    esp, 8
        pop    ecx
        je     SHORT loc_2_02B6
        db  029h, 005h, 010h, 011h, 002h, 000h
        push   0
        push   ecx
        db  0CDh, 020h, 051h, 000h, 001h, 000h
        add    esp, 8
        or     eax, eax
        je     SHORT loc_2_02B6
        ret
loc_2_0305:
        cmp    cl, 1
        ja     NEAR PTR loc_2_0D5F
        db  00Fh, 084h, 078h, 0FEh, 0FEh, 0FFh
        mov    edx, dword ptr [ebp + 0ch]
        db  0C7h, 002h, 0B0h, 010h, 002h, 000h
        mov    eax, dword ptr [ebp + 10h]
        xor    edx, edx
        mov    ecx, 0ah
        div    ecx
        cmp    eax, 1
        je     SHORT loc_2_0350
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        mov    eax, 80000030h
        or     eax, 40001000h
        db  0B9h, 02Bh, 011h, 002h, 000h
        db  0BFh, 014h, 011h, 002h, 000h
        db  0CDh, 020h, 003h, 000h, 017h, 000h
        jmp    SHORT loc_2_0351
loc_2_0350:
        ret
loc_2_0351:
        mov    dword ptr [ebp - 4], 0ffffff9bh
        ret
        db  000h, 000h, 000h
loc_2_035C:
        mov    edx, ecx
        db  0B8h, 0B0h, 010h, 002h, 000h
        mov    ecx, dword ptr [eax + 0ch]
        dec    ecx
        mov    edi, dword ptr [eax + 8]
        xor    eax, eax
        cmp    dl, 1
        jae    SHORT loc_2_03C9
loc_2_0371:
        cmp    dword ptr [edi], eax
        je     SHORT loc_2_037C
        add    edi, 24h
        loop   loc_2_0371
        jmp    SHORT loc_2_03C1
loc_2_037C:
        mov    esi, dword ptr [ebp + 0ch]
        mov    ecx, 24h
        rep movsb
        mov    esi, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [esi + 14h]
        mov    eax, dword ptr [esi + 18h]
        cmp    eax, 10100h
        jne    SHORT loc_2_039C
        db  089h, 00Dh, 0A4h, 011h, 002h, 000h
loc_2_039C:
        cmp    eax, 40100h
        jne    SHORT loc_2_03AB
        mov    edi, esi
        stc
        call   sub_2_046F
loc_2_03AB:
        mov    edx, eax
        shr    edx, 8
        cmp    edx, 402h
        jne    SHORT loc_2_03C0
        mov    edi, dword ptr [esi + 1ch]
        call   sub_2_0CB3
loc_2_03C0:
        ret
loc_2_03C1:
        mov    dword ptr [ebp - 4], 0ffffff99h
        ret
loc_2_03C9:
        cmp    dl, 2
        ja     SHORT loc_2_042B
        cld
loc_2_03CF:
        mov    esi, dword ptr [ebp + 0ch]
        push   ecx
        push   edi
        mov    ecx, 8
        repe cmpsb
        pop    edi
        pop    ecx
        je     SHORT loc_2_03E6
        add    edi, 24h
        loop   loc_2_03CF
        jmp    SHORT loc_2_03F8
loc_2_03E6:
        mov    ecx, 24h
        cmp    dl, 2
        je     SHORT loc_2_0400
        mov    esi, dword ptr [ebp + 0ch]
        xchg   edi, esi
        rep movsb
        ret
loc_2_03F8:
        mov    dword ptr [ebp - 4], 0ffffffffh
        ret
loc_2_0400:
        mov    edx, dword ptr [edi + 18h]
        cmp    edx, 40100h
        jne    SHORT loc_2_0411
        clc
        call   sub_2_046F
loc_2_0411:
        mov    esi, dword ptr [edi + 14h]
        xor    al, al
        rep stosb
        mov    eax, edx
        shr    edx, 8
        cmp    edx, 402h
        jne    SHORT loc_2_042A
        call   sub_2_0D1F
loc_2_042A:
        ret
loc_2_042B:
        cmp    dl, 3
        ja     SHORT loc_2_03C1
loc_2_0430:
        mov    eax, dword ptr [edi + 18h]
        shr    eax, 10h
        cmp    al, 4
        jne    SHORT loc_2_0469
        push   edi
        push   ecx
        cld
        lea    esi, [edi + 8]
        db  0BFh, 0A8h, 011h, 002h, 000h
        mov    ecx, 8
        rep movsb
loc_2_044C:
        dec    edi
        cmp    byte ptr [edi], 20h
        jne    SHORT loc_2_0457
        mov    byte ptr [edi], 0
        jmp    SHORT loc_2_044C
loc_2_0457:
        mov    ebx, 0ffffffffh
        db  0BAh, 0A8h, 011h, 002h, 000h
        db  0CDh, 020h, 002h, 000h, 027h, 000h
        pop    ecx
        pop    edi
loc_2_0469:
        add    edi, 24h
        loop   loc_2_0430
        ret
sub_2_046F:
        pushad
        db  08Bh, 00Dh, 0A4h, 011h, 002h, 000h
        jecxz  loc_2_047C
        call   ecx
loc_2_047A:
        popad
        ret
loc_2_047C:
        jae    SHORT loc_2_047A
        db  066h, 0C7h, 005h, 0F0h, 00Fh, 002h, 000h, 045h, 020h
        db  0C7h, 005h, 0F4h, 00Fh, 002h, 000h, 002h, 000h, 000h, 000h
        db  0C7h, 005h, 0F8h, 00Fh, 002h, 000h, 0B4h, 011h, 002h, 000h
        db  068h, 0E8h, 00Fh, 002h, 000h
        push   201h
        call   sub_2_0218
        add    esp, 8
        jmp    SHORT loc_2_047A
        db  000h
loc_2_04B0:
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        cmp    cl, 1
        jb     SHORT sub_2_04EF
        je     NEAR PTR sub_2_061D
        db  00Fh, 0BAh, 025h, 0B4h, 010h, 002h, 000h, 002h
        jae    SHORT loc_2_04D1
        call   sub_2_08D0
        ret
loc_2_04D1:
        mov    esi, dword ptr [ebp + 0ch]
        or     esi, esi
        jne    SHORT loc_2_04DF
        db  0C7h, 045h, 00Ch, 03Bh, 012h, 002h, 000h
loc_2_04DF:
        call   sub_2_061D
        mov    esi, dword ptr [ebp + 0ch]
        xor    eax, eax
        db  0CDh, 020h, 0BEh, 000h, 001h, 000h
sub_2_04EF:
        cld
        push   ebx
        xor    ecx, ecx
        db  039h, 00Dh, 00Ch, 012h, 002h, 000h
        je     NEAR PTR loc_2_0612
        mov    ebx, dword ptr [ebp + 0ch]
        cmp    dword ptr [ebx], 1
        jne    NEAR PTR loc_2_0612
        cmp    dword ptr [ebx + 4], ecx
        je     NEAR PTR loc_2_0612
        cmp    dword ptr [ebx + 0ch], 270fh
        jg     NEAR PTR loc_2_0612
        mov    edi, dword ptr [ebx + 10h]
        test   edi, edi
        je     SHORT loc_2_0552
        xor    al, al
        mov    ecx, 200h
        repne scasb
        jne    NEAR PTR loc_2_0612
        sub    edi, dword ptr [ebx + 10h]
        mov    ecx, edi
        db  0A1h, 014h, 012h, 002h, 000h
        db  02Bh, 005h, 010h, 012h, 002h, 000h
        sub    eax, 0eh
        cmp    eax, ecx
        jl     NEAR PTR loc_2_060C
loc_2_0552:
        db  08Bh, 03Dh, 00Ch, 012h, 002h, 000h
        db  003h, 03Dh, 010h, 012h, 002h, 000h
        mov    dx, word ptr [ebx + 8]
        cmp    dh, 20h
        je     SHORT loc_2_0570
        cmp    dh, 2ah
        jne    NEAR PTR loc_2_0612
loc_2_0570:
        push   edi
        mov    word ptr [edi], dx
        db  083h, 005h, 010h, 012h, 002h, 000h, 002h
        add    edi, 2
        push   ecx
        mov    esi, dword ptr [ebx + 4]
        mov    ecx, 4
loc_2_0587:
        lodsb
        cmp    al, 20h
        jae    SHORT loc_2_058E
        mov    al, 20h
loc_2_058E:
        stosb
        loop   loc_2_0587
        mov    al, 20h
        stosb
        db  083h, 005h, 010h, 012h, 002h, 000h, 00Ah
        mov    ecx, 4
        mov    al, 30h
        rep stosb
        mov    al, 20h
        stosb
        push   edi
        sub    edi, 2
        mov    eax, dword ptr [ebx + 0ch]
        mov    ecx, 0ah
loc_2_05B3:
        xor    edx, edx
        or     eax, eax
        je     SHORT loc_2_05C3
        div    ecx
        add    dl, 30h
        mov    byte ptr [edi], dl
        dec    edi
        jmp    SHORT loc_2_05B3
loc_2_05C3:
        pop    edi
        pop    ecx
        jecxz  loc_2_05E4
        mov    esi, dword ptr [ebx + 10h]
        db  001h, 00Dh, 010h, 012h, 002h, 000h
        push   ecx
        rep movsb
        mov    dword ptr [edi], ecx
        pop    ecx
        add    ecx, 0ch
        sub    edi, 3
        cmp    byte ptr [edi], 0dh
        je     SHORT loc_2_05F4
        add    edi, 2
loc_2_05E4:
        add    ecx, 2
        mov    dword ptr [edi], 0a0dh
        db  083h, 005h, 010h, 012h, 002h, 000h, 002h
loc_2_05F4:
        pop    edi
        pop    ebx
        push   edi
        db  0F6h, 005h, 028h, 012h, 002h, 000h, 0FFh
        je     SHORT loc_2_0605
        db  0E8h, 04Bh, 0FCh, 0FEh, 0FFh
loc_2_0605:
        call   sub_2_077C
        pop    edi
        ret
loc_2_060C:
        db  0FFh, 005h, 018h, 012h, 002h, 000h
loc_2_0612:
        pop    ebx
        xor    edi, edi
        mov    dword ptr [ebp - 4], 0ffffff97h
        ret
sub_2_061D:
        mov    ecx, dword ptr [ebp + 0ch]
        mov    byte ptr [ecx + 9], 2ah
        call   sub_2_04EF
        mov    ecx, edi
        jecxz  loc_2_064F
        db  083h, 03Dh, 01Ch, 012h, 002h, 000h, 000h
        jne    SHORT loc_2_064E
        add    ecx, 2
        mov    eax, 80000030h
        or     eax, 40001000h
        db  0BFh, 029h, 012h, 002h, 000h
        db  0CDh, 020h, 003h, 000h, 017h, 000h
loc_2_064E:
        ret
loc_2_064F:
        mov    dword ptr [ebp - 4], 0ffffff8ch
        ret
loc_2_0657:
        cmp    ax, 2
        ja     SHORT loc_2_06C0
        push   eax
        mov    ax, 3800h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        mov    edi, eax
        pop    eax
        cmp    edi, -1
        je     SHORT loc_2_06B2
        db  08Bh, 035h, 00Ch, 012h, 002h, 000h
        db  08Bh, 00Dh, 010h, 012h, 002h, 000h
        jecxz  loc_2_06AC
        movzx  edx, word ptr [ebp + 18h]
        cmp    edx, ecx
        jbe    SHORT loc_2_06B2
        mov    word ptr [ebp + 18h], cx
        rep movsb
        cmp    ax, 2
        je     SHORT loc_2_06A0
        db  089h, 00Dh, 010h, 012h, 002h, 000h
        db  08Bh, 035h, 00Ch, 012h, 002h, 000h
        mov    dword ptr [esi], ecx
loc_2_06A0:
        db  08Bh, 015h, 018h, 012h, 002h, 000h
        mov    word ptr [ebp + 10h], dx
        jmp    SHORT loc_2_0712
loc_2_06AC:
        mov    word ptr [ebp + 18h], cx
        jmp    SHORT loc_2_06A0
loc_2_06B2:
        mov    word ptr [ebp + 18h], cx
        mov    word ptr [ebp + 10h], cx
        or     word ptr [ebp + 2ch], 1
        ret
loc_2_06C0:
        cmp    ax, 5
        jge    NEAR PTR loc_2_07CA
        test   dword ptr [ebx], 20h
        je     SHORT loc_2_06B2
        db  03Bh, 01Dh, 0DCh, 00Fh, 002h, 000h
        jne    SHORT loc_2_06B2
        mov    di, word ptr [ebp + 38h]
        shl    edi, 10h
        mov    di, word ptr [ebp]
        db  08Bh, 00Dh, 01Ch, 012h, 002h, 000h
        cmp    ax, 3
        je     SHORT loc_2_06FD
        xor    ecx, edi
        jne    SHORT loc_2_06B2
        db  089h, 00Dh, 01Ch, 012h, 002h, 000h
        jmp    SHORT loc_2_0712
loc_2_06FD:
        db  089h, 03Dh, 01Ch, 012h, 002h, 000h
        movzx  ecx, word ptr [ebp + 10h]
        db  089h, 00Dh, 024h, 012h, 002h, 000h
        call   sub_2_077C
loc_2_0712:
        xor    eax, eax
        mov    word ptr [ebp + 1ch], ax
        and    word ptr [ebp + 2ch], 0fffeh
        ret
loc_2_071E:
        db  089h, 015h, 020h, 012h, 002h, 000h
        db  08Bh, 03Dh, 01Ch, 012h, 002h, 000h
        sub    esp, 6ch
        push   edi
        lea    edi, [esp + 4]
        db  0CDh, 020h, 08Dh, 000h, 001h, 000h
        pop    edi
        db  0CDh, 020h, 083h, 000h, 001h, 000h
        db  00Fh, 0BAh, 035h, 024h, 012h, 002h, 000h, 01Fh
        db  0A1h, 024h, 012h, 002h, 000h
        db  0CDh, 020h, 04Dh, 000h, 001h, 000h
        mov    ecx, edi
        shr    ecx, 10h
        movzx  edx, di
        db  0CDh, 020h, 048h, 000h, 001h, 000h
        db  0CDh, 020h, 085h, 000h, 001h, 000h
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        push   esi
        lea    esi, [esp + 4]
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        pop    esi
        add    esp, 6ch
        ret
sub_2_077C:
        db  083h, 03Dh, 010h, 012h, 002h, 000h, 002h
        jb     SHORT loc_2_07C9
        db  08Bh, 00Dh, 01Ch, 012h, 002h, 000h
        jecxz  loc_2_07C9
        db  00Fh, 0BAh, 025h, 0D8h, 00Fh, 002h, 000h, 008h
        jb     SHORT loc_2_07C9
        db  00Fh, 0BAh, 02Dh, 024h, 012h, 002h, 000h, 01Fh
        jb     SHORT loc_2_07C9
        mov    eax, 10h
        db  08Bh, 01Dh, 0DCh, 00Fh, 002h, 000h
        mov    ecx, 0bh
        xor    edx, edx
        db  0BEh, 01Eh, 007h, 002h, 000h
        mov    edi, 7fffffffh
        db  0CDh, 020h, 014h, 000h, 001h, 000h
        db  089h, 035h, 020h, 012h, 002h, 000h
loc_2_07C9:
        ret
loc_2_07CA:
        movzx  ecx, word ptr [ebp + 38h]
        shl    ecx, 10h
        mov    cx, word ptr [ebp + 10h]
        db  089h, 00Dh, 0A4h, 012h, 002h, 000h
        db  0C7h, 005h, 0ACh, 012h, 002h, 000h, 014h, 000h, 000h, 000h
        db  0BEh, 0A4h, 012h, 002h, 000h
        call   sub_2_0EFE
        jb     NEAR PTR loc_2_08A3
        db  0BFh, 090h, 012h, 002h, 000h
        db  08Bh, 035h, 0A8h, 012h, 002h, 000h
        mov    ecx, 14h
        rep movsb
        db  08Bh, 00Dh, 094h, 012h, 002h, 000h
        db  089h, 00Dh, 0B8h, 012h, 002h, 000h
        db  0C7h, 005h, 0C0h, 012h, 002h, 000h, 009h, 000h, 000h, 000h
        db  0BEh, 0B8h, 012h, 002h, 000h
        call   sub_2_0EFE
        jb     SHORT loc_2_0899
        db  08Bh, 00Dh, 0BCh, 012h, 002h, 000h
        db  089h, 00Dh, 094h, 012h, 002h, 000h
        db  08Bh, 00Dh, 0A0h, 012h, 002h, 000h
        db  089h, 00Dh, 0CCh, 012h, 002h, 000h
        db  0C7h, 005h, 0D4h, 012h, 002h, 000h, 000h, 002h, 000h, 000h
        db  0BEh, 0CCh, 012h, 002h, 000h
        call   sub_2_0EFE
        jb     SHORT loc_2_088F
        db  08Bh, 00Dh, 0D0h, 012h, 002h, 000h
        db  089h, 00Dh, 0A0h, 012h, 002h, 000h
        db  068h, 090h, 012h, 002h, 000h
        mov    ecx, 200h
        cmp    eax, 6
        jne    SHORT loc_2_0877
        mov    ecx, 201h
loc_2_0877:
        push   ecx
        call   sub_2_0218
        add    esp, 8
        db  0A3h, 0E0h, 012h, 002h, 000h
        db  0BEh, 0CCh, 012h, 002h, 000h
        call   sub_2_0F7B
loc_2_088F:
        db  0BEh, 0B8h, 012h, 002h, 000h
        call   sub_2_0F7B
loc_2_0899:
        db  0BEh, 0A4h, 012h, 002h, 000h
        call   sub_2_0F7B
loc_2_08A3:
        db  0A1h, 0E0h, 012h, 002h, 000h
        mov    word ptr [ebp + 1ch], ax
        ret
        db  000h, 000h, 000h
loc_2_08B0:
        cmp    cl, 2
        jae    SHORT loc_2_08C8
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        movzx  eax, byte ptr [ebp + 8]
        shl    eax, 2
        db  0FFh, 0A0h, 0E4h, 012h, 002h, 000h
loc_2_08C8:
        mov    dword ptr [ebp - 4], 0ffffff8fh
        ret
sub_2_08D0:
        pushfd
        pushad
        mov    esi, dword ptr [ebp + 0ch]
        or     esi, esi
        jne    SHORT loc_2_08DE
        db  0BEh, 0ECh, 012h, 002h, 000h
loc_2_08DE:
        db  0CDh, 020h, 0C2h, 000h, 001h, 000h
        popad
        popfd
        int    1
        ret
loc_2_08E9:
        pushfd
        pushad
        mov    esi, dword ptr [ebp + 0ch]
        db  0CDh, 020h, 0C2h, 000h, 001h, 000h
        popad
        popfd
        ret
loc_2_08F7:
        push   0
        push   0
        mov    esi, dword ptr [ebp + 0ch]
        db  0CDh, 020h, 0CAh, 000h, 001h, 000h
        ret
        db  000h, 000h, 000h
loc_2_0908:
        movzx  eax, cl
        shl    eax, 2
        db  0FFh, 0A0h, 024h, 013h, 002h, 000h
loc_2_0914:
        db  0FFh, 005h, 018h, 013h, 002h, 000h
        db  0A1h, 018h, 013h, 002h, 000h
        mov    dword ptr [ebp - 4], eax
        ret
        db  090h
loc_2_0924:
        mov    ecx, dword ptr [ebp + 0ch]
        db  0FFh, 00Dh, 018h, 013h, 002h, 000h
        ret
        db  090h, 090h
loc_2_0930:
        db  083h, 03Dh, 010h, 013h, 002h, 000h, 000h
        jg     SHORT loc_2_0949
        pushfd
        pop    eax
        test   eax, 200h
        jne    SHORT loc_2_0949
        db  083h, 00Dh, 0D8h, 00Fh, 002h, 000h, 020h
loc_2_0949:
        cli
        db  0FFh, 005h, 010h, 013h, 002h, 000h
        ret
        db  090h, 090h, 090h
loc_2_0954:
        db  0FFh, 00Dh, 010h, 013h, 002h, 000h
        jne    SHORT loc_2_0967
        db  00Fh, 0BAh, 035h, 0D8h, 00Fh, 002h, 000h, 005h
        jb     SHORT loc_2_0967
        sti
loc_2_0967:
        ret
loc_2_0968:
        db  00Fh, 0BCh, 005h, 020h, 013h, 002h, 000h
        je     SHORT loc_2_097B
        db  00Fh, 0B3h, 005h, 020h, 013h, 002h, 000h
        mov    byte ptr [ebp - 4], al
loc_2_097B:
        ret
loc_2_097C:
        mov    eax, dword ptr [ebp + 0ch]
        ret
        db  090h, 090h
loc_2_0982:
        mov    ecx, dword ptr [ebp + 0ch]
        db  00Fh, 0ABh, 00Dh, 01Ch, 013h, 002h, 000h
        setb   byte ptr [ebp - 4]
        ret
        db  090h
loc_2_0992:
        mov    ecx, dword ptr [ebp + 0ch]
        db  00Fh, 0B3h, 00Dh, 01Ch, 013h, 002h, 000h
        setb   byte ptr [ebp - 4]
        ret
loc_2_09A1:
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        db  0CDh, 020h, 006h, 000h, 001h, 000h
        mov    ecx, dword ptr [ebp + 0ch]
        db  0CDh, 020h, 083h, 000h, 001h, 000h
loc_2_09B6:
        db  0CDh, 020h, 085h, 000h, 001h, 000h
        db  0CDh, 020h, 038h, 000h, 001h, 000h
        jecxz  loc_2_09CD
        db  00Fh, 0A3h, 00Dh, 01Ch, 013h, 002h, 000h
        jb     SHORT loc_2_09B6
loc_2_09CD:
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        ret
loc_2_09D4:
        db  0CDh, 020h, 006h, 000h, 001h, 000h
        db  00Fh, 0BAh, 025h, 0D8h, 00Fh, 002h, 000h, 002h
        jae    SHORT loc_2_09E5
        inc    ecx
loc_2_09E5:
        mov    dword ptr [ebp - 4], ecx
        ret
        db  090h
loc_2_09EA:
        cmp    cl, 1
        je     SHORT loc_2_09D4
        db  0BEh, 04Ch, 013h, 002h, 000h
        mov    edx, dword ptr [ebp + 0ch]
        pushfd
        cli
        db  083h, 03Dh, 048h, 013h, 002h, 000h, 000h
        je     SHORT loc_2_0A27
        mov    ecx, 0ah
loc_2_0A07:
        cmp    dword ptr [esi], edx
        je     SHORT loc_2_0A49
        add    esi, 4
        loop   loc_2_0A07
        mov    ecx, 0ah
        db  0BEh, 04Ch, 013h, 002h, 000h
loc_2_0A1A:
        cmp    dword ptr [esi], 0
        je     SHORT loc_2_0A27
        add    esi, 4
        loop   loc_2_0A1A
        popfd
        jmp    SHORT loc_2_0A4B
loc_2_0A27:
        db  0FFh, 005h, 048h, 013h, 002h, 000h
        mov    dword ptr [esi], edx
        db  0BEh, 055h, 00Ah, 002h, 000h
        popfd
        cmp    dword ptr [ebp + 10h], 1
        je     SHORT loc_2_0A42
        db  0CDh, 020h, 00Eh, 000h, 001h, 000h
        ret
loc_2_0A42:
        db  0CDh, 020h, 010h, 000h, 001h, 000h
        ret
loc_2_0A49:
        popfd
        ret
loc_2_0A4B:
        mov    dword ptr [ebp - 4], 0ffffff8eh
        ret
        db  090h, 090h
loc_2_0A55:
        mov    ecx, 0ah
        db  0BEh, 04Ch, 013h, 002h, 000h
loc_2_0A5F:
        cmp    dword ptr [esi], edx
        je     SHORT loc_2_0A68
        add    esi, 4
        loop   loc_2_0A5F
loc_2_0A68:
        mov    dword ptr [esi], 0
        db  0FFh, 00Dh, 048h, 013h, 002h, 000h
        push   ebp
        mov    ebp, esp
        xor    ebx, ebx
        call   edx
        pop    ebp
        ret
        db  000h, 000h, 000h
sub_2_0A80:
        db  0BFh, 07Ch, 013h, 002h, 000h
        mov    esi, ebp
        add    esi, 8
        mov    ecx, 4
        rep movsd
        db  089h, 00Dh, 08Ch, 013h, 002h, 000h
        push   ebp
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        mov    ebp, dword ptr [ebx + 8]
        call   sub_2_0C2F
        jb     SHORT loc_2_0B21
        sub    esp, 6ch
        push   edi
        lea    edi, [esp + 4]
        db  0CDh, 020h, 08Dh, 000h, 001h, 000h
        pop    edi
        db  0CDh, 020h, 082h, 000h, 001h, 000h
        db  0A1h, 078h, 013h, 002h, 000h
        mov    edx, eax
        shr    edx, 4
        mov    word ptr [ebp + 3ch], dx
        shl    edx, 4
        sub    eax, edx
        mov    word ptr [ebp + 14h], ax
        db  0A1h, 080h, 013h, 002h, 000h
        mov    word ptr [ebp + 10h], ax
        db  08Bh, 00Dh, 07Ch, 013h, 002h, 000h
        movzx  eax, cl
        shl    eax, 2
        db  0FFh, 090h, 0ACh, 013h, 002h, 000h
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        push   esi
        lea    esi, [esp + 4]
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        pop    esi
        add    esp, 6ch
        db  081h, 025h, 0D8h, 00Fh, 002h, 000h, 0FFh, 0EFh, 0FFh, 0FFh
        pop    ebp
        db  0A1h, 08Ch, 013h, 002h, 000h
        db  0F6h, 005h, 0A7h, 013h, 002h, 000h, 0FFh
        je     SHORT loc_2_0B20
        mov    dword ptr [ebp - 4], eax
loc_2_0B20:
        ret
loc_2_0B21:
        pop    ebp
        mov    eax, 0ffffff38h
        sub    eax, edi
        db  0F6h, 005h, 0A7h, 013h, 002h, 000h, 0FFh
        je     SHORT loc_2_0B20
        mov    dword ptr [ebp - 4], eax
        ret
loc_2_0B36:
        mov    word ptr [ebp + 1ch], 3d02h
loc_2_0B3C:
        db  08Bh, 035h, 080h, 013h, 002h, 000h
        db  08Bh, 03Dh, 078h, 013h, 002h, 000h
        mov    ecx, 200h
        rep movsb
        call   sub_2_0C0E
        db  08Bh, 035h, 084h, 013h, 002h, 000h
        mov    dword ptr [esi], eax
        ret
loc_2_0B5D:
        mov    word ptr [ebp + 18h], 0
        mov    word ptr [ebp + 1ch], 3c00h
        jmp    SHORT loc_2_0B3C
loc_2_0B6B:
        db  08Bh, 00Dh, 084h, 013h, 002h, 000h
        mov    word ptr [ebp + 18h], cx
        mov    word ptr [ebp + 1ch], 3f00h
        call   sub_2_0C0E
        db  0A3h, 08Ch, 013h, 002h, 000h
        db  08Bh, 035h, 078h, 013h, 002h, 000h
        db  08Bh, 03Dh, 088h, 013h, 002h, 000h
        mov    ecx, eax
        rep movsb
        ret
        db  0C7h, 005h, 08Ch, 013h, 002h, 000h, 08Dh, 0FFh, 0FFh, 0FFh, 0C3h
loc_2_0BA1:
        db  08Bh, 00Dh, 084h, 013h, 002h, 000h
        mov    word ptr [ebp + 14h], cx
        shr    ecx, 10h
        mov    word ptr [ebp + 18h], cx
        db  0A1h, 088h, 013h, 002h, 000h
        mov    ah, 42h
        mov    word ptr [ebp + 1ch], ax
        call   sub_2_0C0E
        mov    ax, word ptr [ebp + 14h]
        shl    eax, 10h
        mov    ax, word ptr [ebp + 1ch]
        db  0A3h, 08Ch, 013h, 002h, 000h
        ret
loc_2_0BD3:
        db  08Bh, 00Dh, 084h, 013h, 002h, 000h
        db  08Bh, 03Dh, 078h, 013h, 002h, 000h
        db  08Bh, 035h, 088h, 013h, 002h, 000h
        rep movsb
        db  08Bh, 00Dh, 084h, 013h, 002h, 000h
        mov    word ptr [ebp + 18h], cx
        mov    word ptr [ebp + 1ch], 4000h
        call   sub_2_0C0E
        db  0A3h, 08Ch, 013h, 002h, 000h
        ret
loc_2_0C02:
        mov    word ptr [ebp + 1ch], 3e00h
        call   sub_2_0C0E
        ret
sub_2_0C0E:
        mov    eax, 21h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        movzx  eax, word ptr [ebp + 1ch]
        test   word ptr [ebp + 2ch], 1
        jne    SHORT loc_2_0C26
        ret
loc_2_0C26:
        neg    eax
        db  0A3h, 08Ch, 013h, 002h, 000h
        pop    eax
        ret
sub_2_0C2F:
        mov    edi, 9
        db  00Fh, 0BAh, 02Dh, 0D8h, 00Fh, 002h, 000h, 00Ch
        jb     SHORT loc_2_0C60
        dec    edi
        db  00Fh, 0BAh, 025h, 0D8h, 00Fh, 002h, 000h, 000h
        jb     SHORT loc_2_0C5E
        db  0CDh, 020h, 008h, 000h, 015h, 000h
        jne    SHORT loc_2_0C60
        dec    edi
        db  0CDh, 020h, 004h, 000h, 015h, 000h
        cmp    word ptr [eax], 0
        jne    SHORT loc_2_0C60
loc_2_0C5E:
        clc
        ret
loc_2_0C60:
        db  081h, 025h, 0D8h, 00Fh, 002h, 000h, 0FFh, 0EFh, 0FFh, 0FFh
        stc
        ret
loc_2_0C6C:
        mov    ebx, dword ptr [esp + 4]
        db  0CDh, 020h, 002h, 000h, 014h, 000h
        cld
        ret
loc_2_0C78:
        mov    eax, 4
        jmp    SHORT loc_2_0C81
loc_2_0C7F:
        xor    eax, eax
loc_2_0C81:
        db  08Bh, 088h, 0C8h, 013h, 002h, 000h
        jecxz  loc_2_0C91
        cld
        push   ebx
        call   ecx
        add    esp, 4
        ret
loc_2_0C91:
        mov    al, 23h
        mov    byte ptr [ebx + 1], al
        mov    byte ptr [ebx + 31h], al
        ret
loc_2_0C9A:
        mov    edx, ebx
        mov    ecx, dword ptr [esp + 4]
        mov    ebx, dword ptr [esp + 8]
        mov    eax, dword ptr [esp + 0ch]
        push   ebp
        push   edx
        push   esi
        push   edi
        call   ecx
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
sub_2_0CB3:
        db  0F7h, 005h, 0D8h, 00Fh, 002h, 000h, 000h, 002h, 000h, 000h
        je     SHORT loc_2_0D08
        xor    ebx, ebx
        db  0BAh, 07Fh, 00Ch, 002h, 000h
        db  0BEh, 0C8h, 013h, 002h, 000h
        cmp    dword ptr [esi], ebx
        je     SHORT loc_2_0CDB
        add    esi, 4
        cmp    dword ptr [esi], ebx
        jne    SHORT loc_2_0D09
        db  0BAh, 078h, 00Ch, 002h, 000h
loc_2_0CDB:
        mov    dword ptr [esi], ecx
        movzx  eax, al
        db  00Fh, 0ABh, 005h, 0D0h, 013h, 002h, 000h
        jae    SHORT loc_2_0D08
        push   esi
        db  066h, 081h, 03Dh, 0B2h, 010h, 002h, 000h, 000h, 004h
        jb     SHORT loc_2_0CFF
        mov    ecx, edi
        db  0CDh, 020h, 005h, 000h, 014h, 000h
        jmp    SHORT loc_2_0D05
loc_2_0CFF:
        db  0CDh, 020h, 001h, 000h, 014h, 000h
loc_2_0D05:
        pop    esi
        jb     SHORT loc_2_0D11
loc_2_0D08:
        ret
loc_2_0D09:
        mov    dword ptr [ebp - 4], 0ffffff98h
        ret
loc_2_0D11:
        mov    dword ptr [esi], 0
        mov    dword ptr [ebp - 4], 0ffffff8dh
        ret
sub_2_0D1F:
        db  0F7h, 005h, 0D8h, 00Fh, 002h, 000h, 000h, 002h, 000h, 000h
        je     SHORT loc_2_0D3B
        db  0BFh, 0C8h, 013h, 002h, 000h
        cmp    esi, dword ptr [edi]
        je     SHORT loc_2_0D3C
        add    edi, 4
        cmp    esi, dword ptr [edi]
        je     SHORT loc_2_0D3C
loc_2_0D3B:
        ret
loc_2_0D3C:
        mov    dword ptr [edi], 0
        db  066h, 081h, 03Dh, 0B2h, 010h, 002h, 000h, 000h, 004h
        jb     SHORT loc_2_0D54
        db  0CDh, 020h, 004h, 000h, 014h, 000h
        ret
loc_2_0D54:
        movzx  eax, al
        db  00Fh, 0B3h, 005h, 0D0h, 013h, 002h, 000h
        ret
loc_2_0D5F:
        cmp    cl, 2
        ja     SHORT loc_2_0D92
        db  0F7h, 005h, 0D8h, 00Fh, 002h, 000h, 000h, 002h, 000h, 000h
        je     SHORT loc_2_0D8A
        db  0CDh, 020h, 003h, 000h, 014h, 000h
        mov    ecx, dword ptr [ebp + 0ch]
        mov    dword ptr [ecx + 4], eax
        mov    dword ptr [ecx + 8], edx
        or     eax, edx
        db  023h, 005h, 0D0h, 013h, 002h, 000h
        mov    dword ptr [ecx], eax
        ret
loc_2_0D8A:
        mov    dword ptr [ebp - 4], 0fffffffeh
        ret
loc_2_0D92:
        mov    dword ptr [ebp - 4], 0ffffff8bh
        ret
        db  000h, 000h
loc_2_0D9C:
        mov    eax, 0ffffff8bh
        db  066h, 081h, 03Dh, 0B2h, 010h, 002h, 000h, 000h, 004h
        jb     SHORT loc_2_0DF5
        movzx  edx, cl
        shl    edx, 2
        db  0FFh, 0A2h, 0D4h, 013h, 002h, 000h
loc_2_0DB8:
        push   dword ptr [ebp + 14h]
        push   dword ptr [ebp + 10h]
        push   dword ptr [ebp + 0ch]
        db  0CDh, 020h, 048h, 001h, 001h, 000h
        add    esp, 0ch
        jmp    SHORT loc_2_0DF5
loc_2_0DCC:
        push   dword ptr [ebp + 20h]
        push   dword ptr [ebp + 1ch]
        push   dword ptr [ebp + 18h]
        push   dword ptr [ebp + 14h]
        push   dword ptr [ebp + 10h]
        push   dword ptr [ebp + 0ch]
        db  0CDh, 020h, 051h, 001h, 001h, 000h
        add    esp, 18h
        jmp    SHORT loc_2_0DF5
loc_2_0DE9:
        push   dword ptr [ebp + 0ch]
        db  0CDh, 020h, 049h, 001h, 001h, 000h
        add    esp, 4
loc_2_0DF5:
        mov    dword ptr [ebp - 4], eax
        ret
        db  000h, 000h, 000h
loc_2_0DFC:
        mov    ecx, 1
        db  0CDh, 020h, 01Fh, 000h, 001h, 000h
        push   eax
        mov    ax, 3810h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        mov    edx, eax
        pop    eax
        cmp    edx, -1
        je     NEAR PTR loc_2_0EE4
        db  0FFh, 00Dh, 0E0h, 013h, 002h, 000h
        cmp    dword ptr [edx], 1
        jne    NEAR PTR loc_2_0EE4
        db  0FFh, 00Dh, 0E0h, 013h, 002h, 000h
        db  0B8h, 0B0h, 010h, 002h, 000h
        mov    edi, dword ptr [eax + 8]
        mov    ecx, dword ptr [eax + 0ch]
        lea    esi, [edx + 4]
loc_2_0E41:
        push   ecx
        push   esi
        push   edi
        mov    ecx, 8
        repe cmpsb
        pop    edi
        pop    esi
        pop    ecx
        je     SHORT loc_2_0E5A
        add    edi, 24h
        loop   loc_2_0E41
        jmp    NEAR PTR loc_2_0EE4
loc_2_0E5A:
        db  0FFh, 00Dh, 0E0h, 013h, 002h, 000h
        test   dword ptr [edi + 18h], 200000h
        je     SHORT loc_2_0EE4
        db  0FFh, 00Dh, 0E0h, 013h, 002h, 000h
        movzx  ecx, word ptr [ebp + 38h]
        shl    ecx, 10h
        mov    cx, word ptr [ebp + 10h]
        db  089h, 00Dh, 0E4h, 013h, 002h, 000h
        mov    ecx, dword ptr [edx + 0ch]
        db  089h, 00Dh, 0ECh, 013h, 002h, 000h
        db  0FFh, 00Dh, 0E0h, 013h, 002h, 000h
        db  0BEh, 0E4h, 013h, 002h, 000h
        call   sub_2_0EFE
        jb     SHORT loc_2_0EE4
        db  0FFh, 00Dh, 0E0h, 013h, 002h, 000h
        lea    esi, [edx + 28h]
        call   sub_2_0EFE
        jb     SHORT loc_2_0EDA
        db  0FFh, 00Dh, 0E0h, 013h, 002h, 000h
        lea    esi, [edx + 3ch]
        call   sub_2_0EFE
        jb     SHORT loc_2_0ED2
        db  0FFh, 00Dh, 0E0h, 013h, 002h, 000h
        pushad
        push   edx
        call   dword ptr [edi + 14h]
        add    esp, 4
        popad
        lea    esi, [edx + 3ch]
        call   sub_2_0F7B
loc_2_0ED2:
        lea    esi, [edx + 28h]
        call   sub_2_0F7B
loc_2_0EDA:
        db  0BEh, 0E4h, 013h, 002h, 000h
        call   sub_2_0F7B
loc_2_0EE4:
        db  0CDh, 020h, 020h, 000h, 001h, 000h
        db  0A1h, 0E0h, 013h, 002h, 000h
        mov    word ptr [ebp + 1ch], ax
        db  0C7h, 005h, 0E0h, 013h, 002h, 000h, 008h, 000h, 000h, 000h
        ret
sub_2_0EFE:
        pushad
        xor    eax, eax
        mov    dword ptr [esi + 4], eax
        mov    dword ptr [esi + 0ch], eax
        mov    ecx, dword ptr [esi]
        jecxz  loc_2_0F75
        cmp    dword ptr [esi + 8], eax
        je     SHORT loc_2_0F75
        test   dword ptr [ebx], 20h
        jne    SHORT loc_2_0F28
        movzx  edx, cx
        shr    ecx, 10h
        shl    ecx, 4
        add    ecx, edx
        add    ecx, dword ptr [ebx + 4]
        jmp    SHORT loc_2_0F43
loc_2_0F28:
        movzx  edi, cx
        shr    ecx, 10h
        push   0
        push   ecx
        push   ebx
        db  0CDh, 020h, 06Eh, 000h, 001h, 000h
        add    esp, 0ch
        cmp    eax, -1
        je     SHORT loc_2_0F78
        lea    ecx, [edi + eax]
loc_2_0F43:
        mov    dword ptr [esi + 4], ecx
        mov    edi, ecx
        and    edi, 0fffh
        mov    ecx, dword ptr [esi + 8]
        add    ecx, edi
        dec    ecx
        shr    ecx, 0ch
        inc    ecx
        mov    dword ptr [esi + 10h], ecx
        mov    edi, dword ptr [esi + 4]
        shr    edi, 0ch
        mov    dword ptr [esi + 0ch], edi
        push   0
        push   ecx
        push   edi
        db  0CDh, 020h, 063h, 000h, 001h, 000h
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_2_0F78
loc_2_0F75:
        popad
        clc
        ret
loc_2_0F78:
        popad
        stc
        ret
sub_2_0F7B:
        pushad
        mov    edi, dword ptr [esi + 0ch]
        test   edi, edi
        je     SHORT loc_2_0F93
        mov    ecx, dword ptr [esi + 10h]
        push   0
        push   ecx
        push   edi
        db  0CDh, 020h, 064h, 000h, 001h, 000h
        add    esp, 0ch
loc_2_0F93:
        popad
        ret
        db  000h, 000h, 000h
loc_2_0F98:
        dec    ebx
        add    byte ptr [edx], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ebx], cl
        add    ebp, dword ptr [edx + 0a0333h]
        add    byte ptr [ebp + eax*2 + 43h], al
        inc    ebx
        dec    edi
        push   edx
        inc    ebp
        and    bl, bh
        pop    ds
        db  000h, 054h, 000h, 000h
        add    al, byte ptr [eax]
        arpl   word ptr [eax], ax
        add    al, byte ptr [eax]
        arpl   word ptr [eax], ax
        add    al, byte ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        cwde
        lar    eax, word ptr [eax]
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_2_0FD8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_2_0FDC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_2_0FE0:
        inc    ebx
        dec    edi
        push   edx
        inc    ebp
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
loc_2_0FE8:
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        loopne loc_2_0FFD
        add    al, byte ptr [eax]
loc_2_0FF0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_2_0FF4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_2_0FF8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_2_0FFC:
        aaa
loc_2_0FFD:
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
        db  000h
sub_2_1088:
        db  005h, 003h, 002h, 000h, 05Ch
        add    eax, dword ptr [edx]
        db  000h, 0B0h, 004h, 002h, 000h, 0B0h
        or     byte ptr [edx], al
        db  000h, 0ACh, 002h, 002h, 000h, 008h, 009h
        add    al, byte ptr [eax]
        or     byte ptr [edx], 2
        db  000h, 0B0h, 000h, 002h, 000h, 0EAh
        or     dword ptr [edx], eax
        db  000h, 09Ch, 00Dh, 002h, 000h, 019h, 010h
loc_2_10B2:
        add    byte ptr [eax], al
loc_2_10B4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_2_10B8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_2_10BC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_2_10C0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_2_10C4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_2_10C8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_2_10CC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_2_10D0:
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
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_2_1110:
        lock and eax, dword ptr [eax]
        add    byte ptr [eax + 41h], dl
        push   esp
        dec    eax
        push   edi
        dec    edi
        push   edx
        dec    ebx
        push   ebx
        and    byte ptr [esi + 65h], dl
        db  072h, 073h
        imul   ebp, dword ptr [edi + 6eh], 656b5320h
        ja     SHORT loc_2_112B
loc_2_112B:
        push   esp
        push   41502065h
        push   esp
        dec    eax
        push   edi
        dec    edi
        push   edx
        dec    ebx
        push   ebx
        and    byte ptr [esi + 78h], dl
        inc    esp
        daa
        db  073h, 020h
        outsd
        outsb
        daa
        je     SHORT loc_2_1165
        popad
        insb
        insb
        and    byte ptr [eax + 61h], ch
        jbe    SHORT loc_2_11B2
        and    byte ptr [eax + ebp*2 + 65h], dh
        and    byte ptr [ebx + 61h], dh
        insd
        and    byte ptr gs:[esi + 65h], dh
        db  072h, 073h
        imul   ebp, dword ptr [edi + 6eh], 6d756e20h
        bound  esp, qword ptr [ebp + 72h]
loc_2_1165:
        and    byte ptr cs:[eax], ah
        inc    ebx
        outsd
        outsb
        je     SHORT loc_2_11D6
        outsb
        db  075h, 065h
        and    byte ptr fs:[edi + 70h], ch
        jb     NEAR PTR loc_2_11D8
        db  074h, 069h
        outsd
        outsb
        and    byte ptr [ecx + 73h], ch
        and    byte ptr [eax + 72h], dh
        outsd
        bound  ebp, qword ptr [ebp + 6dh]
        popad
        je     SHORT loc_2_11F2
        arpl   word ptr [esi], bp
        and    byte ptr [edx + 65h], dl
        db  073h, 074h
        popad
        db  072h, 074h
        and    byte ptr [edi + 69h], dl
        outsb
        outsd
        ja     SHORT loc_2_120E
        and    byte ptr [esi + 6fh], cl
        db  077h, 02Eh
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_2_11A4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_2_11A8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_2_11B2:
        add    byte ptr [eax], al
loc_2_11B4:
        inc    esp
        inc    ebp
        inc    ebx
        dec    ecx
        dec    edi
        inc    ebx
        inc    edx
        xor    edi, dword ptr cs:[eax]
        and    byte ptr ss:[ecx + 73h], ch
        and    byte ptr [esi + 6fh], ch
        db  074h, 020h
        insb
        outsd
        popad
        or     eax, 7475420ah
        and    byte ptr [ecx + 20h], ah
loc_2_11D6:
        jo     SHORT loc_2_124A
loc_2_11D8:
        outsd
        je     SHORT loc_2_124A
        arpl   word ptr [edi + 6ch], bp
        and    byte ptr [esi + 65h], ch
        db  065h, 064h, 073h, 020h
        imul   esi, dword ptr [esi + ebp + 0dh], 206f530ah
        db  070h, 072h
        outsd
        db  074h, 06Fh
loc_2_11F2:
        arpl   word ptr [edi + 6ch], bp
        and    byte ptr [edi + 69h], dh
        insb
        insb
        and    byte ptr [edx + 65h], ah
        and    byte ptr [ecx + ebp*2 + 73h], ah
        popad
        bound  ebp, qword ptr [ebp + 64h]
        or     eax, 0ah
loc_2_120C:
        add    byte ptr [eax], al
loc_2_120E:
        add    byte ptr [eax], al
loc_2_1210:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_2_1214:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_2_1218:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_2_121C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_2_1220:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_2_1224:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_2_1228:
        add    byte ptr [eax + 41h], dl
        push   esp
        dec    eax
        push   edi
        dec    edi
        push   edx
        dec    ebx
        push   ebx
        and    byte ptr [ebp + 65h], cl
        jae    SHORT loc_2_12AA
        popad
        add    byte ptr gs:[si + 65h], al
        bound  esi, qword ptr [ebp + 67h]
        and    byte ptr [edx + 72h], al
        popad
        imul   esi, dword ptr [eax + 6fh], 69h
        outsb
loc_2_124A:
        je     SHORT loc_2_126C
        db  077h, 069h
        je     SHORT loc_2_12B8
        and    byte ptr [esi + 6fh], cl
        outsb
        sub    eax, 75626544h
        and    byte ptr [bp + di + 4fh], al
        push   edx
        inc    ebp
        or     eax, 656c500ah
        popad
        jae    SHORT loc_2_12CC
        and    byte ptr [esi + 6fh], cl
        db  074h, 069h
loc_2_126C:
        db  066h, 079h, 020h
        inc    ebx
        jne    SHORT loc_2_12E5
        db  074h, 06Fh
        insd
        jb     NEAR PTR loc_2_1298
        push   ebx
        db  075h, 070h
        jo     SHORT loc_2_12EC
        jb     SHORT loc_2_12F3
        and    byte ptr [ecx + 6eh], ah
        and    byte ptr fs:[edx + 65h], dl
        sub    eax, 746f6f42h
        add    byte ptr cs:[eax], al
        add    byte ptr [eax], al
loc_2_1290:
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
loc_2_1294:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_2_1298:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_2_12A0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_2_12A4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_2_12A8:
        add    byte ptr [eax], al
loc_2_12AA:
        add    byte ptr [eax], al
loc_2_12AC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_2_12B8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_2_12BC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_2_12C0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_2_12CC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_2_12D0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_2_12D4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  0FFh, 0FFh, 0FFh, 0FFh, 0E9h
loc_2_12E5:
        or     byte ptr [edx], al
        db  000h, 0F7h
        or     byte ptr [edx], al
        db  000h
loc_2_12EC:
        inc    ebx
        dec    edi
        push   edx
        inc    ebp
        cmp    ah, byte ptr [eax]
        inc    esp
loc_2_12F3:
        bound  esi, qword ptr gs:[ebp + 67h]
        and    byte ptr [edx + 72h], al
        popad
        imul   esi, dword ptr [eax + 6fh], 69h
        outsb
        je     SHORT loc_2_1323
        push   edx
        popad
        arpl   word ptr [eax + 65h], bp
        and    byte ptr cs:[eax], ah
        or     eax, 0ah
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_2_1318:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_2_131C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  0FEh, 0FFh, 0FFh
loc_2_1323:
        db  0FFh, 030h
        or     dword ptr [edx], eax
        db  000h, 054h, 009h, 002h
        db  000h, 068h, 009h
        add    al, byte ptr [eax]
        or     byte ptr [ecx], 2
        db  000h, 092h, 009h, 002h, 000h, 0A1h
        or     dword ptr [edx], eax
        db  000h, 07Ch, 009h, 002h
        db  000h, 014h, 009h
        add    al, byte ptr [eax]
        and    al, 9
        add    al, byte ptr [eax]
loc_2_1348:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_2_134C:
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
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_2_1378:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_2_137C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_2_1380:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_2_1384:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_2_1388:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_2_138C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_2_1390:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_2_1394:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_2_1398:
        push   eax
        push   edi
        inc    edx
        dec    edi
        dec    edi
        push   esp
        dec    esp
        dec    edi
        inc    edi
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    bh, bh
loc_2_13A8:
        inc    dword ptr [eax]
        add    byte ptr [eax], al
sub_2_13AC:
        or     eax, dword ptr ss:[edx]
        db  000h, 06Bh, 00Bh
        add    al, byte ptr [eax]
        db  0A1h, 00Bh, 002h, 000h, 0D3h
        or     eax, dword ptr [edx]
        db  000h, 002h
        or     al, 2
        db  000h, 05Dh, 00Bh
        add    al, byte ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_2_13C8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  0FFh, 0FFh, 0FFh, 0FFh
loc_2_13D4:
        db  0B8h, 00Dh, 002h, 000h, 0CCh
        db  00Dh, 002h, 000h, 0E9h, 00Dh
        add    al, byte ptr [eax]
loc_2_13E0:
        or     byte ptr [eax], al
        add    byte ptr [eax], al
loc_2_13E4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_2_13EC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
_PTEXT ends
        end
