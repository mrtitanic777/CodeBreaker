; s3mm_pcode_04.asm -- S3MM object 4 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

loc_4_0000:
        db  0F6h, 005h, 033h, 015h, 000h, 000h, 002h
        je     SHORT loc_4_003F
        mov    edx, 3d4h
        in     al, dx
        push   eax
        mov    ax, 0a039h
        out    dx, ax
        mov    al, 67h
        out    dx, al
        inc    edx
        in     al, dx
        and    al, 0f3h
        or     al, 0ch
        out    dx, al
        db  0A1h, 0ACh, 014h, 000h, 000h
        db  08Bh, 015h, 034h, 015h, 000h, 000h
        mov    dword ptr [eax + 81ech], edx
        db  080h, 025h, 033h, 015h, 000h, 000h, 0FDh
        pop    eax
        mov    edx, 3d4h
        out    dx, al
loc_4_003F:
        db  0C6h, 005h, 03Bh, 015h, 000h, 000h, 0FFh
        db  0C6h, 005h, 03Ch, 015h, 000h, 000h, 0FFh
        mov    eax, ecx
        shr    eax, 10h
        or     cx, cx
        je     SHORT loc_4_0075
        db  08Bh, 015h, 090h, 014h, 000h, 000h
        shr    edx, 10h
        dec    dl
        cmp    al, dl
        jae    NEAR PTR loc_4_0126
        inc    al
        xor    cx, cx
        add    ecx, 10000h
loc_4_0075:
        db  08Bh, 015h, 090h, 014h, 000h, 000h
        sub    edx, ecx
        db  03Bh, 015h, 080h, 014h, 000h, 000h
        db  08Bh, 015h, 080h, 014h, 000h, 000h
        ja     SHORT loc_4_0090
        mov    edx, 8000h
loc_4_0090:
        db  080h, 03Dh, 030h, 015h, 000h, 000h, 000h
        je     SHORT loc_4_00DE
        db  0A2h, 03Fh, 015h, 000h, 000h
        db  0C6h, 005h, 03Eh, 015h, 000h, 000h, 03Fh
        db  081h, 03Dh, 02Bh, 015h, 000h, 000h, 000h, 012h, 088h, 000h
        je     SHORT loc_4_0128
        db  081h, 03Dh, 02Bh, 015h, 000h, 000h, 040h, 011h, 088h, 000h
        je     SHORT loc_4_0128
        db  081h, 03Dh, 02Bh, 015h, 000h, 000h, 000h, 011h, 088h, 000h
        je     SHORT loc_4_0128
        db  081h, 03Dh, 02Bh, 015h, 000h, 000h, 000h, 010h, 088h, 000h
        je     SHORT loc_4_0128
        db  0C6h, 005h, 03Eh, 015h, 000h, 000h, 0FFh
        jmp    SHORT loc_4_0128
loc_4_00DE:
        push   edx
        push   ebx
        mov    bl, al
        mov    bh, al
        and    bl, 0fh
        shr    bh, 4
        mov    edx, 3d4h
        mov    al, 35h
        out    dx, al
        inc    edx
        in     al, dx
        and    al, 0f0h
        or     al, bl
        db  0A2h, 03Fh, 015h, 000h, 000h
        db  0C6h, 005h, 040h, 015h, 000h, 000h, 000h
        db  081h, 03Dh, 02Bh, 015h, 000h, 000h, 080h, 000h, 000h, 000h
        je     SHORT loc_4_0122
        dec    edx
        mov    al, 51h
        out    dx, al
        inc    edx
        in     al, dx
        and    al, 0f3h
        shl    bh, 2
        or     al, bh
        db  0A2h, 040h, 015h, 000h, 000h
loc_4_0122:
        pop    ebx
        pop    edx
        jmp    SHORT loc_4_0128
loc_4_0126:
        mov    edx, ecx
loc_4_0128:
        xor    ah, ah
        ret
loc_4_012B:
        db  080h, 03Dh, 042h, 015h, 000h, 000h, 0FFh
        jne    NEAR PTR loc_4_01B8
        db  080h, 03Dh, 03Ch, 015h, 000h, 000h, 000h
        je     SHORT loc_4_01B8
        push   eax
        push   edx
        mov    edx, 3d4h
        in     al, dx
        ror    eax, 8
        mov    ax, 4838h
        out    dx, ax
        mov    ax, 0a539h
        out    dx, ax
        mov    al, 31h
        out    dx, al
        inc    edx
        in     al, dx
        db  0A2h, 041h, 015h, 000h, 000h
        or     al, 1
        out    dx, al
        dec    edx
        db  080h, 03Dh, 030h, 015h, 000h, 000h, 000h
        je     SHORT loc_4_0184
        mov    al, 6ah
        out    dx, al
        inc    dl
        in     al, dx
        db  0A2h, 042h, 015h, 000h, 000h
        db  0A0h, 03Fh, 015h, 000h, 000h
        out    dx, al
        dec    dl
        jmp    SHORT loc_4_01B2
loc_4_0184:
        mov    al, 35h
        out    dx, al
        inc    edx
        in     al, dx
        db  0A2h, 042h, 015h, 000h, 000h
        db  0A0h, 03Fh, 015h, 000h, 000h
        out    dx, al
        dec    edx
        db  081h, 03Dh, 02Bh, 015h, 000h, 000h, 080h, 000h, 000h, 000h
        je     SHORT loc_4_01B2
        mov    al, 51h
        out    dx, al
        inc    edx
        in     al, dx
        db  0A2h, 043h, 015h, 000h, 000h
        db  0A0h, 040h, 015h, 000h, 000h
        out    dx, al
        dec    edx
loc_4_01B2:
        rol    eax, 8
        out    dx, al
        pop    edx
        pop    eax
loc_4_01B8:
        ret
loc_4_01B9:
        db  080h, 03Dh, 042h, 015h, 000h, 000h, 0FFh
        je     NEAR PTR loc_4_026D
        push   eax
        push   edx
        mov    edx, 3d4h
        in     al, dx
        ror    eax, 8
        mov    ax, 4838h
        out    dx, ax
        mov    ax, 0a539h
        out    dx, ax
        db  080h, 03Dh, 030h, 015h, 000h, 000h, 000h
        je     SHORT loc_4_0202
        mov    al, 6ah
        out    dx, al
        inc    dl
        in     al, dx
        db  022h, 005h, 03Eh, 015h, 000h, 000h
        db  03Ah, 005h, 03Fh, 015h, 000h, 000h
        jne    SHORT loc_4_025B
        db  0A0h, 042h, 015h, 000h, 000h
        out    dx, al
        jmp    SHORT loc_4_025B
loc_4_0202:
        xor    ah, ah
        db  081h, 03Dh, 02Bh, 015h, 000h, 000h, 080h, 000h, 000h, 000h
        je     SHORT loc_4_0218
        mov    al, 51h
        out    dx, al
        inc    edx
        in     al, dx
        mov    ah, al
        dec    edx
loc_4_0218:
        mov    al, 35h
        out    dx, al
        inc    edx
        in     al, dx
        db  08Ah, 015h, 03Fh, 015h, 000h, 000h
        db  08Ah, 035h, 040h, 015h, 000h, 000h
        cmp    ax, dx
        jne    SHORT loc_4_025B
        mov    edx, 3d5h
        db  0A0h, 042h, 015h, 000h, 000h
        out    dx, al
        dec    edx
        db  081h, 03Dh, 02Bh, 015h, 000h, 000h, 080h, 000h, 000h, 000h
        je     SHORT loc_4_0251
        mov    al, 51h
        out    dx, al
        inc    edx
        db  0A0h, 043h, 015h, 000h, 000h
        out    dx, al
        dec    edx
loc_4_0251:
        mov    al, 31h
        db  08Ah, 025h, 041h, 015h, 000h, 000h
        out    dx, ax
loc_4_025B:
        db  0C6h, 005h, 042h, 015h, 000h, 000h, 0FFh
        mov    edx, 3d4h
        rol    eax, 8
        out    dx, al
        pop    edx
        pop    eax
loc_4_026D:
        ret
loc_4_026E:
        push   eax
        push   edx
        db  080h, 03Dh, 03Ch, 015h, 000h, 000h, 000h
        je     SHORT loc_4_02EC
        mov    edx, 3d4h
        in     al, dx
        ror    eax, 8
        mov    ax, 4838h
        out    dx, ax
        mov    ax, 0a539h
        out    dx, ax
        mov    al, 31h
        out    dx, al
        inc    edx
        in     al, dx
        db  0A2h, 044h, 015h, 000h, 000h
        or     al, 1
        out    dx, al
        dec    edx
        db  080h, 03Dh, 030h, 015h, 000h, 000h, 000h
        je     SHORT loc_4_02BA
        mov    al, 6ah
        out    dx, al
        inc    dl
        in     al, dx
        db  0A2h, 045h, 015h, 000h, 000h
        db  0A0h, 03Fh, 015h, 000h, 000h
        out    dx, al
        dec    dl
        jmp    SHORT loc_4_02E8
loc_4_02BA:
        mov    al, 35h
        out    dx, al
        inc    edx
        in     al, dx
        db  0A2h, 045h, 015h, 000h, 000h
        db  0A0h, 03Fh, 015h, 000h, 000h
        out    dx, al
        dec    edx
        db  081h, 03Dh, 02Bh, 015h, 000h, 000h, 080h, 000h, 000h, 000h
        je     SHORT loc_4_02E8
        mov    al, 51h
        out    dx, al
        inc    edx
        in     al, dx
        db  0A2h, 046h, 015h, 000h, 000h
        db  0A0h, 040h, 015h, 000h, 000h
        out    dx, al
        dec    edx
loc_4_02E8:
        rol    eax, 8
        out    dx, al
loc_4_02EC:
        pop    edx
        pop    eax
        ret
loc_4_02EF:
        push   eax
        push   edx
        db  080h, 03Dh, 03Ch, 015h, 000h, 000h, 000h
        je     SHORT loc_4_0347
        mov    edx, 3d4h
        in     al, dx
        ror    eax, 8
        mov    al, 31h
        db  08Ah, 025h, 044h, 015h, 000h, 000h
        out    dx, ax
        db  080h, 03Dh, 030h, 015h, 000h, 000h, 000h
        je     SHORT loc_4_0322
        mov    al, 6ah
        db  08Ah, 025h, 045h, 015h, 000h, 000h
        out    dx, ax
        jmp    SHORT loc_4_0343
loc_4_0322:
        mov    al, 35h
        db  08Ah, 025h, 045h, 015h, 000h, 000h
        out    dx, ax
        db  081h, 03Dh, 02Bh, 015h, 000h, 000h, 080h, 000h, 000h, 000h
        je     SHORT loc_4_0343
        mov    al, 51h
        out    dx, al
        inc    edx
        db  0A0h, 046h, 015h, 000h, 000h
        out    dx, al
        dec    edx
loc_4_0343:
        rol    eax, 8
        out    dx, al
loc_4_0347:
        pop    edx
        pop    eax
        ret
loc_4_034A:
        db  080h, 00Dh, 032h, 015h, 000h, 000h, 002h
        db  0A1h, 0A0h, 014h, 000h, 000h
        or     eax, eax
        je     SHORT loc_4_03CC
        cmp    word ptr [eax], 2
        je     SHORT loc_4_0365
        mov    word ptr [eax], 0
loc_4_0365:
        mov    edx, 3d4h
        in     al, dx
        ror    eax, 8
        mov    ax, 0a539h
        out    dx, ax
        mov    al, 40h
        out    dx, al
        inc    dl
        in     al, dx
        or     al, 1
        and    al, 0f7h
        out    dx, al
        dec    dl
        rol    eax, 8
        out    dx, al
        db  0F7h, 005h, 0B0h, 014h, 000h, 000h, 002h, 000h, 000h, 000h
        je     SHORT loc_4_03B5
        db  08Bh, 015h, 0ACh, 014h, 000h, 000h
        db  003h, 015h, 0B4h, 014h, 000h, 000h
        mov    ecx, 3e8h
loc_4_03A2:
        mov    ax, word ptr [edx]
        test   ax, 200h
        je     SHORT loc_4_03CC
        call   sub_4_0BA7
        dec    ecx
        jne    SHORT loc_4_03A2
        jmp    SHORT loc_4_03CC
loc_4_03B5:
        mov    edx, 9ae9h
        in     al, dx
        test   al, 2
        je     SHORT loc_4_03CC
        cmp    al, 0ffh
        je     SHORT loc_4_03CC
        mov    edx, 0e2e8h
        out    dx, ax
        jmp    SHORT loc_4_03B5
loc_4_03CC:
        mov    edx, 4ae8h
        db  0CDh, 020h, 099h, 000h, 001h, 000h
        mov    edx, 4ae9h
        db  0CDh, 020h, 099h, 000h, 001h, 000h
        db  0F6h, 005h, 033h, 015h, 000h, 000h, 001h
        je     NEAR PTR loc_4_04CD
        mov    edx, 3d4h
        in     al, dx
        push   eax
        mov    al, 67h
        out    dx, al
        inc    dx
        in     al, dx
        and    al, 0ch
        cmp    al, 0ch
        jne    NEAR PTR loc_4_04CA
        db  080h, 00Dh, 033h, 015h, 000h, 000h, 002h
        db  03Bh, 01Dh, 084h, 014h, 000h, 000h
        je     SHORT loc_4_0434
        pushad
        mov    eax, 400000h
        db  08Bh, 01Dh, 084h, 014h, 000h, 000h
        mov    ecx, 8
        db  0BEh, 078h, 001h, 000h, 000h
        db  0CDh, 020h, 014h, 000h, 001h, 000h
        popad
        jmp    SHORT loc_4_047C
loc_4_0434:
        push   ebx
        db  08Bh, 01Dh, 084h, 014h, 000h, 000h
        push   ebp
        mov    ebp, dword ptr [ebx + 8]
        sub    esp, 6ch
        push   edi
        lea    edi, [esp + 4]
        db  0CDh, 020h, 08Dh, 000h, 001h, 000h
        pop    edi
        db  0CDh, 020h, 083h, 000h, 001h, 000h
        mov    word ptr [ebp + 1ch], 4021h
        mov    eax, 2fh
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        push   esi
        lea    esi, [esp + 4]
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        pop    esi
        add    esp, 6ch
        pop    ebp
        pop    ebx
loc_4_047C:
        db  0A1h, 0ACh, 014h, 000h, 000h
        mov    edx, dword ptr [eax + 81ech]
        db  089h, 015h, 034h, 015h, 000h, 000h
        and    edx, 0fff78000h
        or     edx, 3000h
        db  080h, 03Dh, 031h, 015h, 000h, 000h, 001h
        jne    SHORT loc_4_04A8
        or     edx, 80000h
loc_4_04A8:
        mov    dword ptr [eax + 81ech], edx
        mov    edx, 3dah
loc_4_04B3:
        in     al, dx
        test   al, 8
        jne    SHORT loc_4_04B3
loc_4_04B8:
        in     al, dx
        test   al, 8
        je     SHORT loc_4_04B8
        mov    dx, 3d4h
        mov    al, 67h
        out    dx, al
        inc    dx
        in     al, dx
        and    al, 0f3h
        out    dx, al
loc_4_04CA:
        dec    edx
        pop    eax
        out    dx, al
loc_4_04CD:
        ret
loc_4_04CE:
        mov    edx, 4ae8h
        db  0CDh, 020h, 097h, 000h, 001h, 000h
        mov    edx, 4ae9h
        db  0CDh, 020h, 097h, 000h, 001h, 000h
        db  0C6h, 005h, 032h, 015h, 000h, 000h, 000h
        db  03Bh, 01Dh, 08Ch, 014h, 000h, 000h
        je     SHORT loc_4_04F8
        call   sub_4_0549
loc_4_04F8:
        ret
loc_4_04F9:
        db  080h, 00Dh, 032h, 015h, 000h, 000h, 001h
        ret
loc_4_0501:
        db  0C6h, 005h, 032h, 015h, 000h, 000h, 000h
        db  0F6h, 005h, 033h, 015h, 000h, 000h, 002h
        je     SHORT loc_4_0548
        push   ebx
        mov    edx, 3d4h
        mov    ax, 0a039h
        out    dx, ax
        mov    al, 67h
        out    dx, al
        inc    edx
        in     al, dx
        and    al, 0f3h
        or     al, 0ch
        db  003h, 01Dh, 098h, 014h, 000h, 000h
        mov    byte ptr [ebx + 9], al
        db  0A1h, 0ACh, 014h, 000h, 000h
        db  08Bh, 015h, 034h, 015h, 000h, 000h
        mov    dword ptr [eax + 81ech], edx
        db  080h, 025h, 033h, 015h, 000h, 000h, 0FDh
        pop    ebx
loc_4_0548:
        ret
sub_4_0549:
        db  081h, 03Dh, 02Bh, 015h, 000h, 000h, 080h, 000h, 000h, 000h
        je     SHORT loc_4_05C6
        db  080h, 03Dh, 03Ch, 015h, 000h, 000h, 000h
        je     SHORT loc_4_05C6
        push   edx
        push   edi
        mov    edi, ebx
        db  003h, 03Dh, 098h, 014h, 000h, 000h
        mov    edx, 3d4h
        in     al, dx
        ror    eax, 8
        mov    ax, 0a539h
        out    dx, ax
        mov    al, 40h
        out    dx, al
        inc    edx
        in     al, dx
        dec    edx
        mov    byte ptr [edi + 2], al
        mov    al, 42h
        out    dx, al
        inc    edx
        in     al, dx
        dec    edx
        mov    byte ptr [edi + 3], al
        db  081h, 03Dh, 02Bh, 015h, 000h, 000h, 080h, 000h, 000h, 000h
        je     SHORT loc_4_05B8
        mov    al, 53h
        out    dx, al
        inc    edx
        in     al, dx
        dec    edx
        mov    byte ptr [edi + 5], al
        mov    al, 58h
        out    dx, al
        inc    edx
        in     al, dx
        mov    byte ptr [edi + 6], al
        db  0F6h, 005h, 033h, 015h, 000h, 000h, 001h
        je     SHORT loc_4_05B8
        dec    edx
        mov    al, 67h
        out    dx, al
        inc    edx
        in     al, dx
        mov    byte ptr [edi + 9], al
loc_4_05B8:
        mov    dl, 0d4h
        rol    eax, 8
        out    dx, al
        mov    dl, 0cch
        in     al, dx
        mov    byte ptr [edi + 0dh], al
        pop    edi
        pop    edx
loc_4_05C6:
        ret
loc_4_05C7:
        push   edx
        db  080h, 03Dh, 03Bh, 015h, 000h, 000h, 000h
        je     NEAR PTR loc_4_07F3
        db  080h, 03Dh, 03Ch, 015h, 000h, 000h, 000h
        je     NEAR PTR loc_4_07F3
        db  03Bh, 00Dh, 08Ch, 014h, 000h, 000h
        jne    SHORT loc_4_05F9
        db  0B9h, 0BCh, 014h, 000h, 000h
        db  02Bh, 00Dh, 098h, 014h, 000h, 000h
        mov    esi, ecx
        jmp    SHORT loc_4_060C
loc_4_05F9:
        db  03Bh, 035h, 088h, 014h, 000h, 000h
        jne    SHORT loc_4_060C
        db  0BEh, 0CCh, 014h, 000h, 000h
        db  02Bh, 035h, 098h, 014h, 000h, 000h
loc_4_060C:
        db  03Bh, 00Dh, 084h, 014h, 000h, 000h
        jne    SHORT loc_4_0625
        db  0A0h, 048h, 015h, 000h, 000h
        or     al, al
        je     SHORT loc_4_0651
        mov    edx, 3c2h
        out    dx, al
        jmp    SHORT loc_4_0651
loc_4_0625:
        db  003h, 00Dh, 098h, 014h, 000h, 000h
        mov    edx, 3c2h
        mov    al, byte ptr [ecx + 0dh]
        out    dx, al
        and    al, 0ch
        cmp    al, 0ch
        jne    SHORT loc_4_064B
        mov    dl, 0d4h
        in     al, dx
        ror    eax, 8
        mov    al, 42h
        mov    ah, byte ptr [ecx + 3]
        out    dx, ax
        rol    eax, 8
        out    dx, al
loc_4_064B:
        db  02Bh, 00Dh, 098h, 014h, 000h, 000h
loc_4_0651:
        db  081h, 03Dh, 02Bh, 015h, 000h, 000h, 080h, 000h, 000h, 000h
        je     NEAR PTR loc_4_07F3
        db  03Bh, 035h, 084h, 014h, 000h, 000h
        jne    NEAR PTR loc_4_0761
        db  0F6h, 005h, 032h, 015h, 000h, 000h, 003h
        jne    NEAR PTR loc_4_0761
        mov    edx, 3d4h
        in     al, dx
        ror    eax, 8
        push   ecx
        mov    ax, 0a539h
        out    dx, ax
        mov    ax, 4838h
        out    dx, ax
        mov    al, 40h
        out    dx, al
        inc    edx
        in     al, dx
        db  08Bh, 03Dh, 0A0h, 014h, 000h, 000h
        mov    ah, byte ptr [edi]
        and    ax, 3feh
        cmp    ah, 2
        jne    SHORT loc_4_06A8
        shr    ah, 1
loc_4_06A8:
        or     al, ah
        out    dx, al
        dec    edx
        mov    al, 53h
        out    dx, al
        inc    edx
        in     al, dx
        db  08Bh, 03Dh, 0A4h, 014h, 000h, 000h
        mov    ah, byte ptr [edi]
        mov    cl, byte ptr [edi + 1]
        mov    ch, cl
        not    cl
        and    ax, cx
        or     al, ah
        out    dx, al
        dec    edx
        db  080h, 03Dh, 047h, 015h, 000h, 000h, 000h
        je     SHORT loc_4_06D9
        mov    al, 31h
        out    dx, al
        inc    edx
        in     al, dx
        and    al, 0feh
        out    dx, al
        dec    edx
loc_4_06D9:
        db  066h, 0A1h, 026h, 015h, 000h, 000h
        out    dx, ax
        db  066h, 0A1h, 028h, 015h, 000h, 000h
        out    dx, ax
        mov    al, 58h
        out    dx, al
        inc    edx
        in     al, dx
        db  08Ah, 025h, 047h, 015h, 000h, 000h
        and    ax, 3fch
        or     al, ah
        db  08Bh, 03Dh, 0A0h, 014h, 000h, 000h
        mov    ah, byte ptr [edi]
        and    ah, 1
        not    ah
        and    ax, 1efh
        shl    ah, 4
        or     al, ah
        out    dx, al
        dec    edx
        pop    ecx
        db  0F6h, 005h, 033h, 015h, 000h, 000h, 001h
        je     SHORT loc_4_0753
        mov    edi, ecx
        db  003h, 03Dh, 098h, 014h, 000h, 000h
        mov    al, 67h
        out    dx, al
        inc    edx
        in     al, dx
        mov    ah, byte ptr [edi + 9]
        cmp    al, ah
        je     SHORT loc_4_0753
        cmp    al, 0ch
        je     SHORT loc_4_0753
        and    ax, 0cf3h
        or     al, ah
        mov    ecx, eax
        mov    edx, 3dah
loc_4_0741:
        in     al, dx
        test   al, 8
        jne    SHORT loc_4_0741
loc_4_0746:
        in     al, dx
        test   al, 8
        je     SHORT loc_4_0746
        mov    edx, 3d5h
        mov    eax, ecx
        out    dx, al
loc_4_0753:
        mov    edx, 3d4h
        rol    eax, 8
        out    dx, al
        jmp    NEAR PTR loc_4_07F3
loc_4_0761:
        db  003h, 035h, 098h, 014h, 000h, 000h
        mov    edx, 3d4h
        in     al, dx
        ror    eax, 8
        mov    ax, 0a539h
        out    dx, ax
        mov    al, 40h
        out    dx, al
        inc    edx
        in     al, dx
        mov    ah, byte ptr [esi + 2]
        and    ax, 9f6h
        or     al, ah
        out    dx, al
        dec    edx
        mov    al, 53h
        out    dx, al
        inc    edx
        in     al, dx
        mov    ah, byte ptr [esi + 5]
        db  08Bh, 03Dh, 0A4h, 014h, 000h, 000h
        mov    cl, byte ptr [edi + 1]
        mov    ch, cl
        not    cl
        and    ax, cx
        or     al, ah
        out    dx, al
        dec    edx
        mov    al, 58h
        out    dx, al
        inc    edx
        in     al, dx
        mov    ah, byte ptr [esi + 6]
        and    ax, 10efh
        or     al, ah
        out    dx, al
        dec    edx
        db  0F6h, 005h, 033h, 015h, 000h, 000h, 001h
        je     SHORT loc_4_07EF
        db  080h, 03Dh, 038h, 015h, 000h, 000h, 000h
        je     SHORT loc_4_07EF
        db  0C6h, 005h, 038h, 015h, 000h, 000h, 000h
        mov    edx, 3dah
loc_4_07D0:
        in     al, dx
        test   al, 8
        jne    SHORT loc_4_07D0
loc_4_07D5:
        in     al, dx
        test   al, 8
        je     SHORT loc_4_07D5
        mov    edx, 3d4h
        mov    al, 67h
        out    dx, al
        inc    edx
        in     al, dx
        mov    ah, byte ptr [esi + 9]
        and    ax, 0cf3h
        or     al, ah
        out    dx, al
        dec    edx
loc_4_07EF:
        rol    eax, 8
        out    dx, al
loc_4_07F3:
        pop    edx
        ret
loc_4_07F5:
        db  081h, 03Dh, 02Bh, 015h, 000h, 000h, 080h, 000h, 000h, 000h
        je     SHORT loc_4_0844
        db  080h, 03Dh, 03Ch, 015h, 000h, 000h, 000h
        je     SHORT loc_4_0844
        mov    edx, 3d4h
        in     al, dx
        ror    eax, 8
        mov    ax, 0a539h
        out    dx, ax
        db  081h, 03Dh, 02Bh, 015h, 000h, 000h, 0D0h, 000h, 000h, 000h
        ja     SHORT loc_4_0831
        mov    ax, 0a5ah
        out    dx, ax
        mov    ax, 59h
        out    dx, ax
loc_4_0831:
        mov    al, 58h
        out    dx, al
        inc    edx
        in     al, dx
        and    al, 0ech
        out    dx, al
        dec    edx
        mov    ax, 39h
        out    dx, ax
        rol    eax, 8
        out    dx, al
loc_4_0844:
        ret
loc_4_0845:
        db  081h, 03Dh, 02Bh, 015h, 000h, 000h, 080h, 000h, 000h, 000h
        je     SHORT loc_4_08B6
        db  080h, 03Dh, 03Ch, 015h, 000h, 000h, 000h
        je     SHORT loc_4_08B6
        db  080h, 03Dh, 02Ah, 015h, 000h, 000h, 000h
        jne    SHORT loc_4_08B6
        mov    edx, 3d4h
        in     al, dx
        ror    eax, 8
        mov    ax, 4838h
        out    dx, ax
        mov    ax, 0a539h
        out    dx, ax
        mov    al, 31h
        out    dx, al
        inc    edx
        in     al, dx
        and    al, 0feh
        out    dx, al
        dec    edx
        db  081h, 03Dh, 02Bh, 015h, 000h, 000h, 0D0h, 000h, 000h, 000h
        ja     SHORT loc_4_089D
        db  066h, 0A1h, 026h, 015h, 000h, 000h
        out    dx, ax
        db  066h, 0A1h, 028h, 015h, 000h, 000h
        out    dx, ax
loc_4_089D:
        mov    al, 58h
        out    dx, al
        inc    edx
        in     al, dx
        db  08Ah, 025h, 047h, 015h, 000h, 000h
        and    ax, 3fch
        or     al, ah
        or     al, 10h
        out    dx, al
        dec    edx
        rol    eax, 8
        out    dx, al
loc_4_08B6:
        ret
loc_4_08B7:
        db  080h, 03Dh, 03Ch, 015h, 000h, 000h, 000h
        je     NEAR PTR loc_4_097F
        mov    edx, 9ae8h
        db  0CDh, 020h, 097h, 000h, 001h, 000h
        mov    edx, 9ae9h
        db  0CDh, 020h, 097h, 000h, 001h, 000h
        mov    edx, 0e2e8h
        db  0CDh, 020h, 097h, 000h, 001h, 000h
        mov    edx, 0bee8h
        db  0CDh, 020h, 097h, 000h, 001h, 000h
        mov    edx, 96e8h
        db  0CDh, 020h, 097h, 000h, 001h, 000h
        mov    edx, 86e8h
        db  0CDh, 020h, 097h, 000h, 001h, 000h
        mov    edx, 82e8h
        db  0CDh, 020h, 097h, 000h, 001h, 000h
        mov    edx, 8ee8h
        db  0CDh, 020h, 097h, 000h, 001h, 000h
        mov    edx, 8ae8h
        db  0CDh, 020h, 097h, 000h, 001h, 000h
        mov    edx, 0b6e8h
        db  0CDh, 020h, 097h, 000h, 001h, 000h
        mov    edx, 0bae8h
        db  0CDh, 020h, 097h, 000h, 001h, 000h
        mov    edx, 0aae8h
        db  0CDh, 020h, 097h, 000h, 001h, 000h
        mov    edx, 0aee8h
        db  0CDh, 020h, 097h, 000h, 001h, 000h
        mov    edx, 0a2e8h
        db  0CDh, 020h, 097h, 000h, 001h, 000h
        mov    edx, 0a6e8h
        db  0CDh, 020h, 097h, 000h, 001h, 000h
        mov    edx, 92e8h
        db  0CDh, 020h, 097h, 000h, 001h, 000h
        mov    edx, 0b2e8h
        db  0CDh, 020h, 097h, 000h, 001h, 000h
loc_4_097F:
        mov    edx, 4ae8h
        db  0CDh, 020h, 097h, 000h, 001h, 000h
        mov    edx, 4ae9h
        db  0CDh, 020h, 097h, 000h, 001h, 000h
        ret
sub_4_0996:
        db  080h, 03Dh, 03Ch, 015h, 000h, 000h, 000h
        je     NEAR PTR loc_4_0A5E
        mov    edx, 9ae8h
        db  0CDh, 020h, 099h, 000h, 001h, 000h
        mov    edx, 9ae9h
        db  0CDh, 020h, 099h, 000h, 001h, 000h
        mov    edx, 0e2e8h
        db  0CDh, 020h, 099h, 000h, 001h, 000h
        mov    edx, 0bee8h
        db  0CDh, 020h, 099h, 000h, 001h, 000h
        mov    edx, 96e8h
        db  0CDh, 020h, 099h, 000h, 001h, 000h
        mov    edx, 86e8h
        db  0CDh, 020h, 099h, 000h, 001h, 000h
        mov    edx, 82e8h
        db  0CDh, 020h, 099h, 000h, 001h, 000h
        mov    edx, 8ee8h
        db  0CDh, 020h, 099h, 000h, 001h, 000h
        mov    edx, 8ae8h
        db  0CDh, 020h, 099h, 000h, 001h, 000h
        mov    edx, 0b6e8h
        db  0CDh, 020h, 099h, 000h, 001h, 000h
        mov    edx, 0bae8h
        db  0CDh, 020h, 099h, 000h, 001h, 000h
        mov    edx, 0aae8h
        db  0CDh, 020h, 099h, 000h, 001h, 000h
        mov    edx, 0aee8h
        db  0CDh, 020h, 099h, 000h, 001h, 000h
        mov    edx, 0a2e8h
        db  0CDh, 020h, 099h, 000h, 001h, 000h
        mov    edx, 0a6e8h
        db  0CDh, 020h, 099h, 000h, 001h, 000h
        mov    edx, 92e8h
        db  0CDh, 020h, 099h, 000h, 001h, 000h
        mov    edx, 0b2e8h
        db  0CDh, 020h, 099h, 000h, 001h, 000h
loc_4_0A5E:
        ret
loc_4_0A5F:
        push   edi
        push   esi
        db  0CDh, 020h, 010h, 000h, 00Ah, 000h
        db  03Bh, 03Dh, 084h, 014h, 000h, 000h
        jne    SHORT loc_4_0A73
        cmp    ebx, edi
        jne    SHORT loc_4_0A7D
loc_4_0A73:
        cmp    ecx, 4
        je     SHORT loc_4_0A7C
        out    dx, ax
        jmp    SHORT loc_4_0A7D
loc_4_0A7C:
        out    dx, al
loc_4_0A7D:
        pop    esi
        pop    edi
        ret
loc_4_0A80:
        push   edx
        db  03Bh, 005h, 084h, 014h, 000h, 000h
        jne    NEAR PTR loc_4_0BA5
        db  080h, 03Dh, 03Ch, 015h, 000h, 000h, 000h
        je     NEAR PTR loc_4_0BA5
        in     al, dx
        ror    eax, 8
        mov    ax, 4838h
        out    dx, ax
        mov    ax, 0a539h
        out    dx, ax
        mov    al, 40h
        out    dx, al
        inc    edx
        in     al, dx
        dec    edx
        rol    eax, 8
        out    dx, al
        test   ah, 1
        je     NEAR PTR loc_4_0BA5
        db  081h, 03Dh, 02Bh, 015h, 000h, 000h, 080h, 000h, 000h, 000h
        je     SHORT loc_4_0AE8
        test   ah, 8
        jne    NEAR PTR loc_4_0BA5
        ror    eax, 8
        mov    al, 58h
        out    dx, al
        inc    edx
        in     al, dx
        dec    edx
        rol    eax, 8
        out    dx, al
        test   ah, 10h
        jne    NEAR PTR loc_4_0BA5
loc_4_0AE8:
        db  0A1h, 09Ch, 014h, 000h, 000h
        cmp    byte ptr [eax], 0
        jne    NEAR PTR loc_4_0BA5
        mov    ecx, 1000h
        db  0F7h, 005h, 0B0h, 014h, 000h, 000h, 002h, 000h, 000h, 000h
        je     NEAR PTR loc_4_0B91
        db  0A1h, 0A4h, 014h, 000h, 000h
        cmp    byte ptr [eax], 0
        je     NEAR PTR loc_4_0BA5
        xor    eax, eax
        db  066h, 081h, 03Dh, 028h, 015h, 000h, 000h, 059h, 00Ah
        jne    SHORT loc_4_0B30
        db  066h, 083h, 03Dh, 026h, 015h, 000h, 000h, 05Ah
        je     SHORT loc_4_0B5E
loc_4_0B30:
        dec    eax
        mov    edx, 3d4h
        in     al, dx
        push   eax
        mov    al, 59h
        mov    ah, al
        out    dx, al
        inc    edx
        in     al, dx
        xchg   al, ah
        dec    edx
        push   eax
        mov    al, 5ah
        mov    ah, al
        out    dx, al
        inc    edx
        in     al, dx
        xchg   al, ah
        push   eax
        dec    edx
        db  066h, 0A1h, 026h, 015h, 000h, 000h
        out    dx, ax
        db  066h, 0A1h, 028h, 015h, 000h, 000h
        out    dx, ax
loc_4_0B5E:
        db  08Bh, 015h, 0ACh, 014h, 000h, 000h
        db  003h, 015h, 0B4h, 014h, 000h, 000h
loc_4_0B6A:
        mov    ax, word ptr [edx]
        test   ax, 200h
        je     SHORT loc_4_0B7B
        call   sub_4_0BA7
        dec    ecx
        jne    SHORT loc_4_0B6A
loc_4_0B7B:
        test   eax, 0ffff0000h
        je     SHORT loc_4_0BA5
        mov    edx, 3d4h
        pop    eax
        out    dx, ax
        pop    eax
        out    dx, ax
        pop    eax
        out    dx, al
        jmp    SHORT loc_4_0BA5
loc_4_0B91:
        mov    edx, 9ae8h
        in     ax, dx
        test   ah, 2
        je     SHORT loc_4_0BA5
        call   sub_4_0BA7
        dec    ecx
        jne    SHORT loc_4_0B91
loc_4_0BA5:
        pop    edx
        ret
sub_4_0BA7:
        push   ecx
        mov    ecx, 3e8h
loc_4_0BAD:
        push   ecx
        pop    ecx
        loop   loc_4_0BAD
        pop    ecx
        ret
loc_4_0BB3:
        cmp    cl, 30h
        jae    SHORT loc_4_0BBB
        clc
        jmp    SHORT loc_4_0C07
loc_4_0BBB:
        db  0F6h, 005h, 032h, 015h, 000h, 000h, 002h
        je     SHORT loc_4_0BCF
        dec    dl
        mov    al, cl
        out    dx, al
        inc    dl
        in     al, dx
        stc
        jmp    SHORT loc_4_0C07
loc_4_0BCF:
        dec    dl
        in     al, dx
        mov    ch, al
        mov    ax, 4838h
        out    dx, ax
        mov    ax, 0a539h
        out    dx, ax
        cmp    cl, 40h
        jne    SHORT loc_4_0BED
        mov    al, 40h
        out    dx, al
        inc    dl
        in     al, dx
        jmp    SHORT loc_4_0BF8
loc_4_0BED:
        cmp    cl, 30h
        jne    SHORT loc_4_0BFD
        mov    al, 30h
        out    dx, al
        inc    dl
        in     al, dx
loc_4_0BF8:
        dec    dl
        stc
        jmp    SHORT loc_4_0BFE
loc_4_0BFD:
        clc
loc_4_0BFE:
        xchg   ch, al
        out    dx, al
        mov    dl, 0d5h
        mov    al, ch
        mov    ch, 0
loc_4_0C07:
        ret
loc_4_0C08:
        cmp    cl, 30h
        cmc
        jae    NEAR PTR loc_4_0CAF
        db  080h, 03Dh, 03Dh, 015h, 000h, 000h, 0FFh
        jb     NEAR PTR loc_4_0CAF
        db  0F6h, 005h, 032h, 015h, 000h, 000h, 002h
        je     SHORT loc_4_0C33
        dec    dl
        mov    ah, al
        mov    al, cl
        out    dx, ax
        stc
        jmp    SHORT loc_4_0CAF
loc_4_0C33:
        dec    dl
        push   eax
        in     al, dx
        ror    eax, 8
        mov    ax, 4838h
        out    dx, ax
        mov    ax, 0a539h
        out    dx, ax
        rol    eax, 8
        out    dx, al
        pop    eax
        inc    dl
        cmp    cl, 40h
        jne    SHORT loc_4_0C5F
        mov    edi, ebx
        db  003h, 03Dh, 098h, 014h, 000h, 000h
        mov    byte ptr [edi + 2], al
        jmp    SHORT loc_4_0CAE
loc_4_0C5F:
        cmp    cl, 42h
        jne    SHORT loc_4_0C71
        mov    edi, ebx
        db  003h, 03Dh, 098h, 014h, 000h, 000h
        mov    byte ptr [edi + 3], al
        jmp    SHORT loc_4_0CAE
loc_4_0C71:
        cmp    cl, 53h
        jne    SHORT loc_4_0C83
        mov    edi, ebx
        db  003h, 03Dh, 098h, 014h, 000h, 000h
        mov    byte ptr [edi + 5], al
        jmp    SHORT loc_4_0CAE
loc_4_0C83:
        cmp    cl, 58h
        jne    SHORT loc_4_0C95
        mov    edi, ebx
        db  003h, 03Dh, 098h, 014h, 000h, 000h
        mov    byte ptr [edi + 6], al
        jmp    SHORT loc_4_0CAE
loc_4_0C95:
        cmp    cl, 67h
        jne    SHORT loc_4_0CAE
        db  0F6h, 005h, 033h, 015h, 000h, 000h, 001h
        je     SHORT loc_4_0CAE
        mov    edi, ebx
        db  003h, 03Dh, 098h, 014h, 000h, 000h
        mov    byte ptr [edi + 9], al
loc_4_0CAE:
        clc
loc_4_0CAF:
        ret
loc_4_0CB0:
        pushfd
        test   cl, 4
        je     SHORT loc_4_0CC1
        push   ebx
        db  003h, 01Dh, 098h, 014h, 000h, 000h
        mov    byte ptr [ebx + 0dh], al
        pop    ebx
loc_4_0CC1:
        popfd
        db  0FFh, 025h, 0B8h, 014h, 000h, 000h
loc_4_0CC8:
        db  08Bh, 00Dh, 090h, 014h, 000h, 000h
        stc
        ret
loc_4_0CD0:
        mov    edx, 3d4h
        in     al, dx
        ror    eax, 8
        db  080h, 03Dh, 030h, 015h, 000h, 000h, 000h
        je     SHORT loc_4_0CEE
        mov    al, 6ah
        out    dx, al
        inc    dl
        in     al, dx
        dec    dl
        or     al, al
        jne    SHORT loc_4_0D15
loc_4_0CEE:
        xor    ah, ah
        db  081h, 03Dh, 02Bh, 015h, 000h, 000h, 080h, 000h, 000h, 000h
        je     SHORT loc_4_0D06
        mov    al, 51h
        out    dx, al
        inc    dl
        in     al, dx
        mov    ah, al
        dec    dl
loc_4_0D06:
        mov    al, 35h
        out    dx, al
        inc    dl
        in     al, dx
        and    ax, 0c0fh
        shl    ah, 2
        or     al, ah
loc_4_0D15:
        mov    dl, 0d4h
        rol    eax, 8
        out    dx, al
        movzx  edx, ah
        stc
        ret
loc_4_0D20:
        mov    edx, 10000h
        xor    eax, eax
        stc
        ret
loc_4_0D29:
        push   esi
        push   eax
        push   edx
        mov    esi, ebx
        db  003h, 035h, 098h, 014h, 000h, 000h
        mov    edx, 3d4h
        in     al, dx
        ror    eax, 8
        mov    ax, 4838h
        out    dx, ax
        mov    ax, 0a539h
        out    dx, ax
        mov    al, 31h
        out    dx, al
        inc    dl
        in     al, dx
        mov    byte ptr [esi], al
        or     al, 1
        out    dx, al
        dec    dl
        mov    al, 35h
        out    dx, al
        inc    dl
        in     al, dx
        mov    byte ptr [esi + 1], al
        dec    dl
        db  081h, 03Dh, 02Bh, 015h, 000h, 000h, 080h, 000h, 000h, 000h
        je     SHORT loc_4_0DE2
        mov    al, 51h
        out    dx, al
        inc    dl
        in     al, dx
        mov    byte ptr [esi + 4], al
        out    dx, al
        dec    dl
        db  080h, 03Dh, 030h, 015h, 000h, 000h, 000h
        je     SHORT loc_4_0D8D
        mov    al, 6ah
        out    dx, al
        inc    dl
        in     al, dx
        mov    byte ptr [esi + 0ah], al
        dec    dl
loc_4_0D8D:
        mov    al, 40h
        out    dx, al
        inc    dl
        in     al, dx
        mov    byte ptr [esi + 2], al
        and    al, 0feh
        or     al, 8
        out    dx, al
        dec    dl
        mov    al, 53h
        out    dx, al
        inc    dl
        in     al, dx
        mov    byte ptr [esi + 5], al
        db  08Bh, 03Dh, 0A4h, 014h, 000h, 000h
        mov    cl, byte ptr [edi + 1]
        not    cl
        and    al, cl
        out    dx, al
        dec    dl
        mov    al, 59h
        out    dx, al
        inc    dl
        in     al, dx
        mov    byte ptr [esi + 7], al
        xor    al, al
        out    dx, al
        dec    dl
        mov    al, 5ah
        out    dx, al
        inc    dl
        in     al, dx
        mov    byte ptr [esi + 8], al
        mov    al, 0ah
        out    dx, al
        dec    dl
        mov    al, 58h
        out    dx, al
        inc    dl
        in     al, dx
        mov    byte ptr [esi + 6], al
        and    al, 0fch
        or     al, 10h
        out    dx, al
        dec    dl
loc_4_0DE2:
        rol    eax, 8
        out    dx, al
        pop    edx
        pop    eax
        pop    esi
        stc
        ret
loc_4_0DEB:
        push   esi
        push   eax
        push   edx
        mov    esi, ebx
        db  003h, 035h, 098h, 014h, 000h, 000h
        mov    edx, 3d4h
        in     al, dx
        ror    eax, 8
        mov    ax, 4838h
        out    dx, ax
        mov    ax, 0a539h
        out    dx, ax
        mov    al, 31h
        mov    ah, byte ptr [esi]
        out    dx, ax
        mov    al, 35h
        mov    ah, byte ptr [esi + 1]
        out    dx, ax
        db  081h, 03Dh, 02Bh, 015h, 000h, 000h, 080h, 000h, 000h, 000h
        je     SHORT loc_4_0E5E
        mov    al, 51h
        mov    ah, byte ptr [esi + 4]
        out    dx, ax
        db  080h, 03Dh, 030h, 015h, 000h, 000h, 000h
        je     SHORT loc_4_0E3B
        mov    al, 6ah
        mov    ah, byte ptr [esi + 0ah]
        out    dx, ax
loc_4_0E3B:
        mov    al, 40h
        mov    ah, byte ptr [esi + 2]
        out    dx, ax
        mov    al, 53h
        mov    ah, byte ptr [esi + 5]
        out    dx, ax
        mov    al, 59h
        mov    ah, byte ptr [esi + 7]
        out    dx, ax
        mov    al, 5ah
        mov    ah, byte ptr [esi + 8]
        out    dx, ax
        mov    al, 58h
        mov    ah, byte ptr [esi + 6]
        out    dx, ax
loc_4_0E5E:
        rol    eax, 8
        out    dx, al
        pop    edx
        pop    eax
        pop    esi
        stc
        ret
loc_4_0E67:
        push   edx
        push   eax
        push   ecx
        mov    cl, dl
        mov    edx, 3d4h
        in     al, dx
        ror    eax, 8
        mov    ax, 4838h
        out    dx, ax
        mov    ax, 0a539h
        out    dx, ax
        mov    al, 35h
        mov    ah, cl
        and    ah, 0fh
        out    dx, ax
        db  081h, 03Dh, 02Bh, 015h, 000h, 000h, 080h, 000h, 000h, 000h
        je     SHORT loc_4_0EC1
        mov    al, 51h
        out    dx, al
        inc    dl
        in     al, dx
        and    al, 0f3h
        mov    ah, cl
        and    ah, 30h
        shr    ah, 2
        or     al, ah
        out    dx, al
        dec    dl
        db  080h, 03Dh, 030h, 015h, 000h, 000h, 000h
        je     SHORT loc_4_0EC1
        mov    al, 6ah
        mov    ah, cl
        test   cl, 0c0h
        jne    SHORT loc_4_0EBF
        xor    ah, ah
loc_4_0EBF:
        out    dx, ax
loc_4_0EC1:
        rol    eax, 8
        out    dx, al
        pop    ecx
        pop    eax
        pop    edx
        stc
        ret
loc_4_0ECA:
        push   eax
        mov    eax, 86h
        push   10h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        pop    eax
        clc
        ret
loc_4_0EDB:
        db  03Bh, 01Dh, 084h, 014h, 000h, 000h
        je     SHORT loc_4_0EFE
        cmp    ax, 4f02h
        jne    SHORT loc_4_0EFE
        test   byte ptr [ebp + 11h], 80h
        jne    SHORT loc_4_0EFE
        push   eax
        mov    eax, 13h
        push   10h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        pop    eax
loc_4_0EFE:
        clc
        ret
_PTEXT ends
        end
