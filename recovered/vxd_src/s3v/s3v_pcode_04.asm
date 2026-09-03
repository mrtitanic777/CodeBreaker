; s3v_pcode_04.asm -- S3V object 4 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

loc_4_0000:
        db  0F6h, 005h, 038h, 004h, 000h, 000h, 002h
        je     SHORT loc_4_0041
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
        db  0A1h, 0A0h, 003h, 000h, 000h
        db  08Bh, 015h, 03Ch, 004h, 000h, 000h
        db  003h, 005h, 0ACh, 003h, 000h, 000h
        mov    dword ptr [eax], edx
        db  080h, 025h, 038h, 004h, 000h, 000h, 0FDh
        pop    eax
        mov    edx, 3d4h
        out    dx, al
loc_4_0041:
        db  0C6h, 005h, 040h, 004h, 000h, 000h, 0FFh
        db  0C6h, 005h, 041h, 004h, 000h, 000h, 0FFh
        mov    eax, ecx
        shr    eax, 10h
        or     cx, cx
        je     SHORT loc_4_0077
        db  08Bh, 015h, 084h, 003h, 000h, 000h
        shr    edx, 10h
        dec    dl
        cmp    al, dl
        jae    NEAR PTR loc_4_016C
        inc    al
        xor    cx, cx
        add    ecx, 10000h
loc_4_0077:
        db  08Bh, 015h, 084h, 003h, 000h, 000h
        sub    edx, ecx
        db  03Bh, 015h, 074h, 003h, 000h, 000h
        db  08Bh, 015h, 074h, 003h, 000h, 000h
        ja     SHORT loc_4_0092
        mov    edx, 8000h
loc_4_0092:
        db  080h, 03Dh, 034h, 004h, 000h, 000h, 000h
        je     NEAR PTR loc_4_0124
        db  0A2h, 044h, 004h, 000h, 000h
        db  0C6h, 005h, 043h, 004h, 000h, 000h, 03Fh
        db  081h, 03Dh, 02Fh, 004h, 000h, 000h, 000h, 014h, 088h, 000h
        je     NEAR PTR loc_4_016E
        db  081h, 03Dh, 02Fh, 004h, 000h, 000h, 000h, 012h, 088h, 000h
        je     NEAR PTR loc_4_016E
        db  081h, 03Dh, 02Fh, 004h, 000h, 000h, 000h, 015h, 088h, 000h
        je     NEAR PTR loc_4_016E
        db  081h, 03Dh, 02Fh, 004h, 000h, 000h, 040h, 011h, 088h, 000h
        je     NEAR PTR loc_4_016E
        db  081h, 03Dh, 02Fh, 004h, 000h, 000h, 000h, 011h, 088h, 000h
        je     SHORT loc_4_016E
        db  081h, 03Dh, 02Fh, 004h, 000h, 000h, 000h, 010h, 088h, 000h
        je     SHORT loc_4_016E
        db  081h, 03Dh, 02Fh, 004h, 000h, 000h, 000h, 03Dh, 088h, 000h
        je     SHORT loc_4_016E
        db  081h, 03Dh, 02Fh, 004h, 000h, 000h, 000h, 031h, 056h, 000h
        je     SHORT loc_4_016E
        db  0C6h, 005h, 043h, 004h, 000h, 000h, 0FFh
        jmp    SHORT loc_4_016E
loc_4_0124:
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
        db  0A2h, 044h, 004h, 000h, 000h
        db  0C6h, 005h, 045h, 004h, 000h, 000h, 000h
        db  081h, 03Dh, 02Fh, 004h, 000h, 000h, 080h, 000h, 000h, 000h
        je     SHORT loc_4_0168
        dec    edx
        mov    al, 51h
        out    dx, al
        inc    edx
        in     al, dx
        and    al, 0f3h
        shl    bh, 2
        or     al, bh
        db  0A2h, 045h, 004h, 000h, 000h
loc_4_0168:
        pop    ebx
        pop    edx
        jmp    SHORT loc_4_016E
loc_4_016C:
        mov    edx, ecx
loc_4_016E:
        xor    ah, ah
        ret
loc_4_0171:
        db  080h, 03Dh, 047h, 004h, 000h, 000h, 0FFh
        jne    NEAR PTR loc_4_01FE
        db  080h, 03Dh, 041h, 004h, 000h, 000h, 000h
        je     SHORT loc_4_01FE
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
        db  0A2h, 046h, 004h, 000h, 000h
        or     al, 1
        out    dx, al
        dec    edx
        db  080h, 03Dh, 034h, 004h, 000h, 000h, 000h
        je     SHORT loc_4_01CA
        mov    al, 6ah
        out    dx, al
        inc    dl
        in     al, dx
        db  0A2h, 047h, 004h, 000h, 000h
        db  0A0h, 044h, 004h, 000h, 000h
        out    dx, al
        dec    dl
        jmp    SHORT loc_4_01F8
loc_4_01CA:
        mov    al, 35h
        out    dx, al
        inc    edx
        in     al, dx
        db  0A2h, 047h, 004h, 000h, 000h
        db  0A0h, 044h, 004h, 000h, 000h
        out    dx, al
        dec    edx
        db  081h, 03Dh, 02Fh, 004h, 000h, 000h, 080h, 000h, 000h, 000h
        je     SHORT loc_4_01F8
        mov    al, 51h
        out    dx, al
        inc    edx
        in     al, dx
        db  0A2h, 048h, 004h, 000h, 000h
        db  0A0h, 045h, 004h, 000h, 000h
        out    dx, al
        dec    edx
loc_4_01F8:
        rol    eax, 8
        out    dx, al
        pop    edx
        pop    eax
loc_4_01FE:
        ret
loc_4_01FF:
        db  080h, 03Dh, 047h, 004h, 000h, 000h, 0FFh
        je     NEAR PTR loc_4_02B3
        push   eax
        push   edx
        mov    edx, 3d4h
        in     al, dx
        ror    eax, 8
        mov    ax, 4838h
        out    dx, ax
        mov    ax, 0a539h
        out    dx, ax
        db  080h, 03Dh, 034h, 004h, 000h, 000h, 000h
        je     SHORT loc_4_0248
        mov    al, 6ah
        out    dx, al
        inc    dl
        in     al, dx
        db  022h, 005h, 043h, 004h, 000h, 000h
        db  03Ah, 005h, 044h, 004h, 000h, 000h
        jne    SHORT loc_4_02A1
        db  0A0h, 047h, 004h, 000h, 000h
        out    dx, al
        jmp    SHORT loc_4_02A1
loc_4_0248:
        xor    ah, ah
        db  081h, 03Dh, 02Fh, 004h, 000h, 000h, 080h, 000h, 000h, 000h
        je     SHORT loc_4_025E
        mov    al, 51h
        out    dx, al
        inc    edx
        in     al, dx
        mov    ah, al
        dec    edx
loc_4_025E:
        mov    al, 35h
        out    dx, al
        inc    edx
        in     al, dx
        db  08Ah, 015h, 044h, 004h, 000h, 000h
        db  08Ah, 035h, 045h, 004h, 000h, 000h
        cmp    ax, dx
        jne    SHORT loc_4_02A1
        mov    edx, 3d5h
        db  0A0h, 047h, 004h, 000h, 000h
        out    dx, al
        dec    edx
        db  081h, 03Dh, 02Fh, 004h, 000h, 000h, 080h, 000h, 000h, 000h
        je     SHORT loc_4_0297
        mov    al, 51h
        out    dx, al
        inc    edx
        db  0A0h, 048h, 004h, 000h, 000h
        out    dx, al
        dec    edx
loc_4_0297:
        mov    al, 31h
        db  08Ah, 025h, 046h, 004h, 000h, 000h
        out    dx, ax
loc_4_02A1:
        db  0C6h, 005h, 047h, 004h, 000h, 000h, 0FFh
        mov    edx, 3d4h
        rol    eax, 8
        out    dx, al
        pop    edx
        pop    eax
loc_4_02B3:
        ret
loc_4_02B4:
        push   eax
        push   edx
        db  080h, 03Dh, 041h, 004h, 000h, 000h, 000h
        je     SHORT loc_4_0332
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
        db  0A2h, 049h, 004h, 000h, 000h
        or     al, 1
        out    dx, al
        dec    edx
        db  080h, 03Dh, 034h, 004h, 000h, 000h, 000h
        je     SHORT loc_4_0300
        mov    al, 6ah
        out    dx, al
        inc    dl
        in     al, dx
        db  0A2h, 04Ah, 004h, 000h, 000h
        db  0A0h, 044h, 004h, 000h, 000h
        out    dx, al
        dec    dl
        jmp    SHORT loc_4_032E
loc_4_0300:
        mov    al, 35h
        out    dx, al
        inc    edx
        in     al, dx
        db  0A2h, 04Ah, 004h, 000h, 000h
        db  0A0h, 044h, 004h, 000h, 000h
        out    dx, al
        dec    edx
        db  081h, 03Dh, 02Fh, 004h, 000h, 000h, 080h, 000h, 000h, 000h
        je     SHORT loc_4_032E
        mov    al, 51h
        out    dx, al
        inc    edx
        in     al, dx
        db  0A2h, 04Bh, 004h, 000h, 000h
        db  0A0h, 045h, 004h, 000h, 000h
        out    dx, al
        dec    edx
loc_4_032E:
        rol    eax, 8
        out    dx, al
loc_4_0332:
        pop    edx
        pop    eax
        ret
loc_4_0335:
        push   eax
        push   edx
        db  080h, 03Dh, 041h, 004h, 000h, 000h, 000h
        je     SHORT loc_4_038D
        mov    edx, 3d4h
        in     al, dx
        ror    eax, 8
        mov    al, 31h
        db  08Ah, 025h, 049h, 004h, 000h, 000h
        out    dx, ax
        db  080h, 03Dh, 034h, 004h, 000h, 000h, 000h
        je     SHORT loc_4_0368
        mov    al, 6ah
        db  08Ah, 025h, 04Ah, 004h, 000h, 000h
        out    dx, ax
        jmp    SHORT loc_4_0389
loc_4_0368:
        mov    al, 35h
        db  08Ah, 025h, 04Ah, 004h, 000h, 000h
        out    dx, ax
        db  081h, 03Dh, 02Fh, 004h, 000h, 000h, 080h, 000h, 000h, 000h
        je     SHORT loc_4_0389
        mov    al, 51h
        out    dx, al
        inc    edx
        db  0A0h, 04Bh, 004h, 000h, 000h
        out    dx, al
        dec    edx
loc_4_0389:
        rol    eax, 8
        out    dx, al
loc_4_038D:
        pop    edx
        pop    eax
        ret
sub_4_0390:
        db  0F6h, 005h, 038h, 004h, 000h, 000h, 001h
        je     NEAR PTR loc_4_0424
        pushad
        mov    edx, 3d4h
        in     al, dx
        push   eax
        mov    al, 67h
        out    dx, al
        inc    dx
        in     al, dx
        and    al, 0ch
        cmp    al, 0ch
        jne    SHORT loc_4_041F
        db  080h, 00Dh, 038h, 004h, 000h, 000h, 002h
        mov    edx, 4021h
        db  0E8h, 092h, 0FEh, 0FBh, 0FFh
        db  0A1h, 0A0h, 003h, 000h, 000h
        db  003h, 005h, 0ACh, 003h, 000h, 000h
        mov    edx, dword ptr [eax]
        db  089h, 015h, 03Ch, 004h, 000h, 000h
        and    edx, 0fff78000h
        db  0F6h, 005h, 038h, 004h, 000h, 000h, 004h
        je     SHORT loc_4_03EC
        or     edx, 0c000h
        jmp    SHORT loc_4_03F2
loc_4_03EC:
        or     edx, 3000h
loc_4_03F2:
        db  080h, 03Dh, 035h, 004h, 000h, 000h, 001h
        jne    SHORT loc_4_0401
        or     edx, 80000h
loc_4_0401:
        mov    dword ptr [eax], edx
        mov    edx, 3dah
loc_4_0408:
        in     al, dx
        test   al, 8
        jne    SHORT loc_4_0408
loc_4_040D:
        in     al, dx
        test   al, 8
        je     SHORT loc_4_040D
        mov    dx, 3d4h
        mov    al, 67h
        out    dx, al
        inc    dx
        in     al, dx
        and    al, 0f3h
        out    dx, al
loc_4_041F:
        dec    dx
        pop    eax
        out    dx, al
        popad
loc_4_0424:
        ret
loc_4_0425:
        db  080h, 00Dh, 036h, 004h, 000h, 000h, 002h
        db  0A1h, 094h, 003h, 000h, 000h
        or     eax, eax
        je     NEAR PTR loc_4_04D1
        cmp    word ptr [eax], 2
        je     SHORT loc_4_0444
        mov    word ptr [eax], 0
loc_4_0444:
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
        db  0F7h, 005h, 0A4h, 003h, 000h, 000h, 002h, 000h, 000h, 000h
        je     SHORT loc_4_04BF
        db  08Bh, 015h, 0A0h, 003h, 000h, 000h
        db  003h, 015h, 0A8h, 003h, 000h, 000h
        mov    ecx, 3e8h
        db  081h, 03Dh, 02Fh, 004h, 000h, 000h, 000h, 03Dh, 088h, 000h
        je     SHORT loc_4_04AC
        db  081h, 03Dh, 02Fh, 004h, 000h, 000h, 000h, 031h, 056h, 000h
        je     SHORT loc_4_04AC
loc_4_0499:
        mov    ax, word ptr [edx]
        test   ax, 200h
        je     SHORT loc_4_04D1
        call   sub_4_0CA2
        dec    ecx
        jne    SHORT loc_4_0499
        jmp    SHORT loc_4_04D1
loc_4_04AC:
        mov    ax, word ptr [edx]
        test   ax, 2000h
        jne    SHORT loc_4_04D1
        call   sub_4_0CA2
        dec    ecx
        jne    SHORT loc_4_04AC
        jmp    SHORT loc_4_04D1
loc_4_04BF:
        mov    edx, 9ae9h
        in     al, dx
        test   al, 2
        je     SHORT loc_4_04D1
        call   sub_4_0CA2
        dec    ecx
        jne    SHORT loc_4_04BF
loc_4_04D1:
        db  081h, 03Dh, 02Fh, 004h, 000h, 000h, 000h, 031h, 056h, 000h
        je     SHORT loc_4_04FF
        db  081h, 03Dh, 02Fh, 004h, 000h, 000h, 000h, 03Dh, 088h, 000h
        je     SHORT loc_4_04FF
        mov    edx, 4ae8h
        db  0CDh, 020h, 099h, 000h, 001h, 000h
        mov    edx, 4ae9h
        db  0CDh, 020h, 099h, 000h, 001h, 000h
loc_4_04FF:
        call   sub_4_0390
        ret
loc_4_0505:
        db  081h, 03Dh, 02Fh, 004h, 000h, 000h, 000h, 03Dh, 088h, 000h
        je     SHORT loc_4_0533
        db  081h, 03Dh, 02Fh, 004h, 000h, 000h, 000h, 031h, 056h, 000h
        je     SHORT loc_4_0533
        mov    edx, 4ae8h
        db  0CDh, 020h, 097h, 000h, 001h, 000h
        mov    edx, 4ae9h
        db  0CDh, 020h, 097h, 000h, 001h, 000h
loc_4_0533:
        db  0C6h, 005h, 036h, 004h, 000h, 000h, 000h
        db  03Bh, 01Dh, 080h, 003h, 000h, 000h
        je     SHORT loc_4_0547
        call   sub_4_05A4
loc_4_0547:
        ret
loc_4_0548:
        mov    dx, 3cch
        in     al, dx
        or     al, 1
        mov    dl, 0c2h
        out    dx, al
        db  080h, 00Dh, 036h, 004h, 000h, 000h, 001h
        ret
loc_4_055A:
        db  0C6h, 005h, 036h, 004h, 000h, 000h, 000h
        db  0F6h, 005h, 038h, 004h, 000h, 000h, 002h
        je     SHORT loc_4_05A3
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
        db  003h, 01Dh, 08Ch, 003h, 000h, 000h
        mov    byte ptr [ebx + 9], al
        db  0A1h, 0A0h, 003h, 000h, 000h
        db  08Bh, 015h, 03Ch, 004h, 000h, 000h
        db  003h, 005h, 0ACh, 003h, 000h, 000h
        mov    dword ptr [eax], edx
        db  080h, 025h, 038h, 004h, 000h, 000h, 0FDh
        pop    ebx
loc_4_05A3:
        ret
sub_4_05A4:
        db  081h, 03Dh, 02Fh, 004h, 000h, 000h, 080h, 000h, 000h, 000h
        je     SHORT loc_4_0621
        db  080h, 03Dh, 041h, 004h, 000h, 000h, 000h
        je     SHORT loc_4_0621
        push   edx
        push   edi
        mov    edi, ebx
        db  003h, 03Dh, 08Ch, 003h, 000h, 000h
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
        db  081h, 03Dh, 02Fh, 004h, 000h, 000h, 080h, 000h, 000h, 000h
        je     SHORT loc_4_0613
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
        db  0F6h, 005h, 038h, 004h, 000h, 000h, 001h
        je     SHORT loc_4_0613
        dec    edx
        mov    al, 67h
        out    dx, al
        inc    edx
        in     al, dx
        mov    byte ptr [edi + 9], al
loc_4_0613:
        mov    dl, 0d4h
        rol    eax, 8
        out    dx, al
        mov    dl, 0cch
        in     al, dx
        mov    byte ptr [edi + 0dh], al
        pop    edi
        pop    edx
loc_4_0621:
        ret
loc_4_0622:
        push   edx
        db  080h, 03Dh, 040h, 004h, 000h, 000h, 000h
        je     NEAR PTR loc_4_0854
        db  080h, 03Dh, 041h, 004h, 000h, 000h, 000h
        je     NEAR PTR loc_4_0854
        db  03Bh, 00Dh, 080h, 003h, 000h, 000h
        jne    SHORT loc_4_0654
        db  0B9h, 0C0h, 003h, 000h, 000h
        db  02Bh, 00Dh, 08Ch, 003h, 000h, 000h
        mov    esi, ecx
        jmp    SHORT loc_4_0667
loc_4_0654:
        db  03Bh, 035h, 07Ch, 003h, 000h, 000h
        jne    SHORT loc_4_0667
        db  0BEh, 0D0h, 003h, 000h, 000h
        db  02Bh, 035h, 08Ch, 003h, 000h, 000h
loc_4_0667:
        db  03Bh, 00Dh, 078h, 003h, 000h, 000h
        jne    SHORT loc_4_0680
        db  0A0h, 04Dh, 004h, 000h, 000h
        or     al, al
        je     SHORT loc_4_06AC
        mov    edx, 3c2h
        out    dx, al
        jmp    SHORT loc_4_06AC
loc_4_0680:
        db  003h, 00Dh, 08Ch, 003h, 000h, 000h
        mov    edx, 3c2h
        mov    al, byte ptr [ecx + 0dh]
        out    dx, al
        and    al, 0ch
        cmp    al, 0ch
        jne    SHORT loc_4_06A6
        mov    dl, 0d4h
        in     al, dx
        ror    eax, 8
        mov    al, 42h
        mov    ah, byte ptr [ecx + 3]
        out    dx, ax
        rol    eax, 8
        out    dx, al
loc_4_06A6:
        db  02Bh, 00Dh, 08Ch, 003h, 000h, 000h
loc_4_06AC:
        db  081h, 03Dh, 02Fh, 004h, 000h, 000h, 080h, 000h, 000h, 000h
        je     NEAR PTR loc_4_0854
        db  03Bh, 035h, 078h, 003h, 000h, 000h
        jne    NEAR PTR loc_4_07C2
        db  0F6h, 005h, 036h, 004h, 000h, 000h, 003h
        jne    NEAR PTR loc_4_07C2
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
        db  08Bh, 03Dh, 094h, 003h, 000h, 000h
        mov    ah, byte ptr [edi]
        and    ax, 3feh
        cmp    ah, 2
        jne    SHORT loc_4_0703
        shr    ah, 1
loc_4_0703:
        or     al, ah
        out    dx, al
        dec    edx
        mov    al, 53h
        out    dx, al
        inc    edx
        in     al, dx
        db  08Bh, 03Dh, 098h, 003h, 000h, 000h
        mov    ah, byte ptr [edi]
        mov    cl, byte ptr [edi + 1]
        mov    ch, cl
        not    cl
        and    ax, cx
        or     al, ah
        out    dx, al
        dec    edx
        db  080h, 03Dh, 04Ch, 004h, 000h, 000h, 000h
        je     SHORT loc_4_0734
        mov    al, 31h
        out    dx, al
        inc    edx
        in     al, dx
        and    al, 0feh
        out    dx, al
        dec    edx
loc_4_0734:
        db  066h, 0A1h, 02Ah, 004h, 000h, 000h
        out    dx, ax
        db  066h, 0A1h, 02Ch, 004h, 000h, 000h
        out    dx, ax
        mov    al, 58h
        out    dx, al
        inc    edx
        in     al, dx
        db  08Ah, 025h, 04Ch, 004h, 000h, 000h
        and    ax, 3fch
        or     al, ah
        db  08Bh, 03Dh, 094h, 003h, 000h, 000h
        mov    ah, byte ptr [edi]
        and    ah, 1
        not    ah
        and    ax, 1efh
        shl    ah, 4
        or     al, ah
        out    dx, al
        dec    edx
        pop    ecx
        db  0F6h, 005h, 038h, 004h, 000h, 000h, 001h
        je     SHORT loc_4_07B4
        mov    edi, ecx
        db  003h, 03Dh, 08Ch, 003h, 000h, 000h
        mov    al, 67h
        out    dx, al
        inc    edx
        in     al, dx
        mov    ah, byte ptr [edi + 9]
        cmp    al, ah
        je     SHORT loc_4_07B4
        mov    cl, al
        and    cl, 0ch
        cmp    cl, 0ch
        je     SHORT loc_4_07B4
        and    ax, 0cf3h
        or     al, ah
        mov    ecx, eax
        mov    edx, 3dah
loc_4_07A2:
        in     al, dx
        test   al, 8
        jne    SHORT loc_4_07A2
loc_4_07A7:
        in     al, dx
        test   al, 8
        je     SHORT loc_4_07A7
        mov    edx, 3d5h
        mov    eax, ecx
        out    dx, al
loc_4_07B4:
        mov    edx, 3d4h
        rol    eax, 8
        out    dx, al
        jmp    NEAR PTR loc_4_0854
loc_4_07C2:
        db  003h, 035h, 08Ch, 003h, 000h, 000h
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
        db  08Bh, 03Dh, 098h, 003h, 000h, 000h
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
        db  0F6h, 005h, 038h, 004h, 000h, 000h, 001h
        je     SHORT loc_4_0850
        db  080h, 03Dh, 039h, 004h, 000h, 000h, 000h
        je     SHORT loc_4_0850
        db  0C6h, 005h, 039h, 004h, 000h, 000h, 000h
        mov    edx, 3dah
loc_4_0831:
        in     al, dx
        test   al, 8
        jne    SHORT loc_4_0831
loc_4_0836:
        in     al, dx
        test   al, 8
        je     SHORT loc_4_0836
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
loc_4_0850:
        rol    eax, 8
        out    dx, al
loc_4_0854:
        pop    edx
        ret
loc_4_0856:
        db  081h, 03Dh, 02Fh, 004h, 000h, 000h, 080h, 000h, 000h, 000h
        je     SHORT loc_4_08A5
        db  080h, 03Dh, 041h, 004h, 000h, 000h, 000h
        je     SHORT loc_4_08A5
        mov    edx, 3d4h
        in     al, dx
        ror    eax, 8
        mov    ax, 0a539h
        out    dx, ax
        db  081h, 03Dh, 02Fh, 004h, 000h, 000h, 0D0h, 000h, 000h, 000h
        ja     SHORT loc_4_0892
        mov    ax, 0a5ah
        out    dx, ax
        mov    ax, 59h
        out    dx, ax
loc_4_0892:
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
loc_4_08A5:
        ret
loc_4_08A6:
        db  081h, 03Dh, 02Fh, 004h, 000h, 000h, 080h, 000h, 000h, 000h
        je     NEAR PTR loc_4_0936
        db  080h, 03Dh, 041h, 004h, 000h, 000h, 000h
        je     SHORT loc_4_0936
        db  080h, 03Dh, 02Eh, 004h, 000h, 000h, 000h
        jne    SHORT loc_4_0936
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
        db  081h, 03Dh, 02Fh, 004h, 000h, 000h, 0D0h, 000h, 000h, 000h
        ja     SHORT loc_4_0902
        db  066h, 0A1h, 02Ah, 004h, 000h, 000h
        out    dx, ax
        db  066h, 0A1h, 02Ch, 004h, 000h, 000h
        out    dx, ax
loc_4_0902:
        mov    al, 58h
        out    dx, al
        inc    edx
        in     al, dx
        db  08Ah, 025h, 04Ch, 004h, 000h, 000h
        and    ax, 3fch
        or     al, ah
        or     al, 10h
        out    dx, al
        dec    edx
        rol    eax, 8
        out    dx, al
        db  080h, 03Dh, 037h, 004h, 000h, 000h, 001h
        jb     SHORT loc_4_0936
        ja     SHORT loc_4_0930
        mov    edx, 4023h
        db  0E8h, 024h, 0F9h, 0FBh, 0FFh
loc_4_0930:
        db  0FEh, 00Dh, 037h, 004h, 000h, 000h
loc_4_0936:
        ret
loc_4_0937:
        db  080h, 03Dh, 041h, 004h, 000h, 000h, 000h
        je     NEAR PTR loc_4_0A1F
        db  081h, 03Dh, 02Fh, 004h, 000h, 000h, 000h, 03Dh, 088h, 000h
        je     NEAR PTR loc_4_0A35
        db  081h, 03Dh, 02Fh, 004h, 000h, 000h, 000h, 031h, 056h, 000h
        je     NEAR PTR loc_4_0A35
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
loc_4_0A1F:
        mov    edx, 4ae8h
        db  0CDh, 020h, 097h, 000h, 001h, 000h
        mov    edx, 4ae9h
        db  0CDh, 020h, 097h, 000h, 001h, 000h
loc_4_0A35:
        ret
sub_4_0A36:
        db  080h, 03Dh, 041h, 004h, 000h, 000h, 000h
        je     NEAR PTR loc_4_0B1E
        db  081h, 03Dh, 02Fh, 004h, 000h, 000h, 000h, 03Dh, 088h, 000h
        je     NEAR PTR loc_4_0B1E
        db  081h, 03Dh, 02Fh, 004h, 000h, 000h, 000h, 031h, 056h, 000h
        je     NEAR PTR loc_4_0B1E
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
loc_4_0B1E:
        ret
loc_4_0B1F:
        push   edi
        push   esi
        db  0CDh, 020h, 010h, 000h, 00Ah, 000h
        db  03Bh, 03Dh, 078h, 003h, 000h, 000h
        jne    SHORT loc_4_0B33
        cmp    ebx, edi
        jne    SHORT loc_4_0B3D
loc_4_0B33:
        cmp    ecx, 4
        je     SHORT loc_4_0B3C
        out    dx, ax
        jmp    SHORT loc_4_0B3D
loc_4_0B3C:
        out    dx, al
loc_4_0B3D:
        pop    esi
        pop    edi
        ret
loc_4_0B40:
        push   edx
        db  03Bh, 005h, 078h, 003h, 000h, 000h
        jne    NEAR PTR loc_4_0CA0
        db  080h, 03Dh, 041h, 004h, 000h, 000h, 000h
        je     NEAR PTR loc_4_0CA0
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
        je     NEAR PTR loc_4_0CA0
        db  081h, 03Dh, 02Fh, 004h, 000h, 000h, 080h, 000h, 000h, 000h
        je     SHORT loc_4_0BA8
        test   ah, 8
        jne    NEAR PTR loc_4_0CA0
        ror    eax, 8
        mov    al, 58h
        out    dx, al
        inc    edx
        in     al, dx
        dec    edx
        rol    eax, 8
        out    dx, al
        test   ah, 10h
        jne    NEAR PTR loc_4_0CA0
loc_4_0BA8:
        db  0A1h, 090h, 003h, 000h, 000h
        cmp    byte ptr [eax], 0
        jne    NEAR PTR loc_4_0CA0
        db  0A1h, 098h, 003h, 000h, 000h
        cmp    byte ptr [eax], 0
        je     NEAR PTR loc_4_0CA0
        xor    eax, eax
        db  066h, 081h, 03Dh, 02Ch, 004h, 000h, 000h, 059h, 00Ah
        jne    SHORT loc_4_0BDB
        db  066h, 083h, 03Dh, 02Ah, 004h, 000h, 000h, 05Ah
        je     SHORT loc_4_0C09
loc_4_0BDB:
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
        db  066h, 0A1h, 02Ah, 004h, 000h, 000h
        out    dx, ax
        db  066h, 0A1h, 02Ch, 004h, 000h, 000h
        out    dx, ax
loc_4_0C09:
        db  0F7h, 005h, 0A4h, 003h, 000h, 000h, 002h, 000h, 000h, 000h
        je     SHORT loc_4_0C64
        db  08Bh, 015h, 0A0h, 003h, 000h, 000h
        db  003h, 015h, 0A8h, 003h, 000h, 000h
        mov    ecx, 3e8h
        db  081h, 03Dh, 02Fh, 004h, 000h, 000h, 000h, 03Dh, 088h, 000h
        je     SHORT loc_4_0C51
        db  081h, 03Dh, 02Fh, 004h, 000h, 000h, 000h, 031h, 056h, 000h
        je     SHORT loc_4_0C51
loc_4_0C3E:
        mov    ax, word ptr [edx]
        test   ax, 200h
        je     SHORT loc_4_0C76
        call   sub_4_0CA2
        dec    ecx
        jne    SHORT loc_4_0C3E
        jmp    SHORT loc_4_0C76
loc_4_0C51:
        mov    ax, word ptr [edx]
        test   ax, 2000h
        jne    SHORT loc_4_0C76
        call   sub_4_0CA2
        dec    ecx
        jne    SHORT loc_4_0C51
        jmp    SHORT loc_4_0C76
loc_4_0C64:
        mov    edx, 9ae9h
        in     al, dx
        test   al, 2
        je     SHORT loc_4_0C76
        call   sub_4_0CA2
        dec    ecx
        jne    SHORT loc_4_0C64
loc_4_0C76:
        test   eax, 0ffff0000h
        je     SHORT loc_4_0CA0
        mov    edx, 3d4h
        pop    eax
        out    dx, ax
        pop    eax
        out    dx, ax
        pop    eax
        out    dx, al
        jmp    SHORT loc_4_0CA0
        db  0BAh, 0E8h, 09Ah, 000h, 000h, 066h, 0EDh, 0F6h, 0C4h, 002h, 074h, 008h, 0E8h, 005h, 000h, 000h
        db  000h, 049h, 075h, 0ECh
loc_4_0CA0:
        pop    edx
        ret
sub_4_0CA2:
        push   ecx
        mov    ecx, 3e8h
loc_4_0CA8:
        push   ecx
        pop    ecx
        loop   loc_4_0CA8
        pop    ecx
        ret
loc_4_0CAE:
        cmp    cl, 30h
        jae    SHORT loc_4_0CB6
        clc
        jmp    SHORT loc_4_0D02
loc_4_0CB6:
        db  0F6h, 005h, 036h, 004h, 000h, 000h, 002h
        je     SHORT loc_4_0CCA
        dec    dl
        mov    al, cl
        out    dx, al
        inc    dl
        in     al, dx
        stc
        jmp    SHORT loc_4_0D02
loc_4_0CCA:
        dec    dl
        in     al, dx
        mov    ch, al
        mov    ax, 4838h
        out    dx, ax
        mov    ax, 0a539h
        out    dx, ax
        cmp    cl, 40h
        jne    SHORT loc_4_0CE8
        mov    al, 40h
        out    dx, al
        inc    dl
        in     al, dx
        jmp    SHORT loc_4_0CF3
loc_4_0CE8:
        cmp    cl, 30h
        jne    SHORT loc_4_0CF8
        mov    al, 30h
        out    dx, al
        inc    dl
        in     al, dx
loc_4_0CF3:
        dec    dl
        stc
        jmp    SHORT loc_4_0CF9
loc_4_0CF8:
        clc
loc_4_0CF9:
        xchg   ch, al
        out    dx, al
        mov    dl, 0d5h
        mov    al, ch
        mov    ch, 0
loc_4_0D02:
        ret
loc_4_0D03:
        cmp    cl, 30h
        cmc
        jae    NEAR PTR loc_4_0DAA
        db  080h, 03Dh, 042h, 004h, 000h, 000h, 0FFh
        jb     NEAR PTR loc_4_0DAA
        db  0F6h, 005h, 036h, 004h, 000h, 000h, 002h
        je     SHORT loc_4_0D2E
        dec    dl
        mov    ah, al
        mov    al, cl
        out    dx, ax
        stc
        jmp    SHORT loc_4_0DAA
loc_4_0D2E:
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
        jne    SHORT loc_4_0D5A
        mov    edi, ebx
        db  003h, 03Dh, 08Ch, 003h, 000h, 000h
        mov    byte ptr [edi + 2], al
        jmp    SHORT loc_4_0DA9
loc_4_0D5A:
        cmp    cl, 42h
        jne    SHORT loc_4_0D6C
        mov    edi, ebx
        db  003h, 03Dh, 08Ch, 003h, 000h, 000h
        mov    byte ptr [edi + 3], al
        jmp    SHORT loc_4_0DA9
loc_4_0D6C:
        cmp    cl, 53h
        jne    SHORT loc_4_0D7E
        mov    edi, ebx
        db  003h, 03Dh, 08Ch, 003h, 000h, 000h
        mov    byte ptr [edi + 5], al
        jmp    SHORT loc_4_0DA9
loc_4_0D7E:
        cmp    cl, 58h
        jne    SHORT loc_4_0D90
        mov    edi, ebx
        db  003h, 03Dh, 08Ch, 003h, 000h, 000h
        mov    byte ptr [edi + 6], al
        jmp    SHORT loc_4_0DA9
loc_4_0D90:
        cmp    cl, 67h
        jne    SHORT loc_4_0DA9
        db  0F6h, 005h, 038h, 004h, 000h, 000h, 001h
        je     SHORT loc_4_0DA9
        mov    edi, ebx
        db  003h, 03Dh, 08Ch, 003h, 000h, 000h
        mov    byte ptr [edi + 9], al
loc_4_0DA9:
        clc
loc_4_0DAA:
        ret
loc_4_0DAB:
        pushfd
        test   cl, 4
        je     SHORT loc_4_0DBC
        push   ebx
        db  003h, 01Dh, 08Ch, 003h, 000h, 000h
        mov    byte ptr [ebx + 0dh], al
        pop    ebx
loc_4_0DBC:
        popfd
        db  0FFh, 025h, 0B0h, 003h, 000h, 000h
loc_4_0DC3:
        db  08Bh, 00Dh, 084h, 003h, 000h, 000h
        stc
        ret
loc_4_0DCB:
        mov    edx, 3d4h
        in     al, dx
        ror    eax, 8
        db  080h, 03Dh, 034h, 004h, 000h, 000h, 000h
        je     SHORT loc_4_0DE9
        mov    al, 6ah
        out    dx, al
        inc    dl
        in     al, dx
        dec    dl
        or     al, al
        jne    SHORT loc_4_0E10
loc_4_0DE9:
        xor    ah, ah
        db  081h, 03Dh, 02Fh, 004h, 000h, 000h, 080h, 000h, 000h, 000h
        je     SHORT loc_4_0E01
        mov    al, 51h
        out    dx, al
        inc    dl
        in     al, dx
        mov    ah, al
        dec    dl
loc_4_0E01:
        mov    al, 35h
        out    dx, al
        inc    dl
        in     al, dx
        and    ax, 0c0fh
        shl    ah, 2
        or     al, ah
loc_4_0E10:
        mov    dl, 0d4h
        rol    eax, 8
        out    dx, al
        movzx  edx, ah
        stc
        ret
loc_4_0E1B:
        mov    edx, 10000h
        xor    eax, eax
        stc
        ret
loc_4_0E24:
        push   esi
        push   eax
        push   edx
        mov    esi, ebx
        db  003h, 035h, 08Ch, 003h, 000h, 000h
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
        db  081h, 03Dh, 02Fh, 004h, 000h, 000h, 080h, 000h, 000h, 000h
        je     SHORT loc_4_0EDD
        mov    al, 51h
        out    dx, al
        inc    dl
        in     al, dx
        mov    byte ptr [esi + 4], al
        out    dx, al
        dec    dl
        db  080h, 03Dh, 034h, 004h, 000h, 000h, 000h
        je     SHORT loc_4_0E88
        mov    al, 6ah
        out    dx, al
        inc    dl
        in     al, dx
        mov    byte ptr [esi + 0ah], al
        dec    dl
loc_4_0E88:
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
        db  08Bh, 03Dh, 098h, 003h, 000h, 000h
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
loc_4_0EDD:
        rol    eax, 8
        out    dx, al
        pop    edx
        pop    eax
        pop    esi
        stc
        ret
loc_4_0EE6:
        push   esi
        push   eax
        push   edx
        mov    esi, ebx
        db  003h, 035h, 08Ch, 003h, 000h, 000h
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
        db  081h, 03Dh, 02Fh, 004h, 000h, 000h, 080h, 000h, 000h, 000h
        je     SHORT loc_4_0F59
        mov    al, 51h
        mov    ah, byte ptr [esi + 4]
        out    dx, ax
        db  080h, 03Dh, 034h, 004h, 000h, 000h, 000h
        je     SHORT loc_4_0F36
        mov    al, 6ah
        mov    ah, byte ptr [esi + 0ah]
        out    dx, ax
loc_4_0F36:
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
loc_4_0F59:
        rol    eax, 8
        out    dx, al
        pop    edx
        pop    eax
        pop    esi
        stc
        ret
loc_4_0F62:
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
        db  081h, 03Dh, 02Fh, 004h, 000h, 000h, 080h, 000h, 000h, 000h
        je     SHORT loc_4_0FBC
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
        db  080h, 03Dh, 034h, 004h, 000h, 000h, 000h
        je     SHORT loc_4_0FBC
        mov    al, 6ah
        mov    ah, cl
        test   cl, 0c0h
        jne    SHORT loc_4_0FBA
        xor    ah, ah
loc_4_0FBA:
        out    dx, ax
loc_4_0FBC:
        rol    eax, 8
        out    dx, al
        pop    ecx
        pop    eax
        pop    edx
        stc
        ret
loc_4_0FC5:
        push   eax
        mov    eax, 86h
        push   10h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        pop    eax
        clc
        ret
loc_4_0FD6:
        db  03Bh, 01Dh, 078h, 003h, 000h, 000h
        je     SHORT loc_4_0FF9
        cmp    ax, 4f02h
        jne    SHORT loc_4_0FF9
        test   byte ptr [ebp + 11h], 80h
        jne    SHORT loc_4_0FF9
        push   eax
        mov    eax, 13h
        push   10h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        pop    eax
loc_4_0FF9:
        clc
        ret
_PTEXT ends
        end
