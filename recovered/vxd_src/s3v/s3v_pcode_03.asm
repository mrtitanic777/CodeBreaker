; s3v_pcode_03.asm -- S3V object 3 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

loc_3_0000:
        mov    ax, 3c14h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        db  0A3h, 090h, 003h, 000h, 000h
        mov    ax, 3c04h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        db  0A3h, 094h, 003h, 000h, 000h
        mov    ax, 3c00h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        db  0A3h, 098h, 003h, 000h, 000h
        mov    ecx, dword ptr [ebp + 18h]
        mov    al, 5ah
        mov    ah, cl
        db  066h, 0A3h, 02Ah, 004h, 000h, 000h
        mov    al, 59h
        mov    ah, ch
        db  066h, 0A3h, 02Ch, 004h, 000h, 000h
        shr    ecx, 10h
        db  088h, 00Dh, 04Ch, 004h, 000h, 000h
        db  088h, 02Dh, 02Eh, 004h, 000h, 000h
        mov    eax, dword ptr [ebp + 4]
        shr    eax, 10h
        or     ax, ax
        je     SHORT loc_3_009D
        db  066h, 03Bh, 005h, 09Eh, 003h, 000h, 000h
        je     SHORT loc_3_009D
        db  066h, 0A3h, 09Eh, 003h, 000h, 000h
        db  0CDh, 020h, 012h, 000h, 00Ah, 000h
        db  0F7h, 005h, 0A4h, 003h, 000h, 000h, 002h, 000h, 000h, 000h
        je     SHORT loc_3_009D
        db  00Fh, 0B7h, 005h, 09Eh, 003h, 000h, 000h
        push   0
        push   eax
        db  0FFh, 035h, 078h, 003h, 000h, 000h
        db  0CDh, 020h, 06Eh, 000h, 001h, 000h
        add    esp, 0ch
        db  0A3h, 0A0h, 003h, 000h, 000h
loc_3_009D:
        mov    eax, dword ptr [ebp]
        shr    eax, 10h
        or     ax, ax
        je     SHORT loc_3_00BD
        db  066h, 03Bh, 005h, 09Ch, 003h, 000h, 000h
        je     SHORT loc_3_00BD
        db  066h, 0A3h, 09Ch, 003h, 000h, 000h
        db  0CDh, 020h, 012h, 000h, 00Ah, 000h
loc_3_00BD:
        mov    edx, 3cch
        in     al, dx
        db  0A2h, 04Dh, 004h, 000h, 000h
        db  081h, 03Dh, 02Fh, 004h, 000h, 000h, 000h, 03Dh, 088h, 000h
        je     SHORT loc_3_00E0
        db  081h, 03Dh, 02Fh, 004h, 000h, 000h, 000h, 031h, 056h, 000h
        jne    SHORT loc_3_0105
loc_3_00E0:
        xor    eax, eax
        db  066h, 0A1h, 0BCh, 003h, 000h, 000h
        test   ax, ax
        je     SHORT loc_3_00FA
        db  0CDh, 020h, 012h, 000h, 00Ah, 000h
        db  00Fh, 0B7h, 005h, 0BCh, 003h, 000h, 000h
loc_3_00FA:
        mov    dword ptr [ebp + 14h], eax
        db  0A1h, 0B8h, 003h, 000h, 000h
        mov    dword ptr [ebp + 18h], eax
loc_3_0105:
        ret
loc_3_0106:
        db  066h, 0C7h, 005h, 03Ah, 004h, 000h, 000h, 000h, 000h
        db  0B8h, 0E0h, 003h, 000h, 000h
        mov    ecx, 4ah
        mov    word ptr [eax], cx
        push   eax
        db  0CDh, 020h, 014h, 000h, 00Ah, 000h
        pop    eax
        test   word ptr [eax + 2], 110h
        jne    NEAR PTR loc_3_06C9
        movzx  edi, word ptr [eax + 20h]
        or     edi, edi
        je     NEAR PTR loc_3_06C9
        movzx  esi, word ptr [eax + 18h]
        or     esi, esi
        je     NEAR PTR loc_3_06C9
        mov    edx, 3d4h
        in     al, dx
        push   eax
        mov    eax, 0a539h
        out    dx, ax
        db  081h, 03Dh, 02Fh, 004h, 000h, 000h, 080h, 000h, 000h, 000h
        je     NEAR PTR loc_3_0286
        db  081h, 03Dh, 02Fh, 004h, 000h, 000h, 0A0h, 000h, 000h, 000h
        je     NEAR PTR loc_3_02DF
        db  081h, 03Dh, 02Fh, 004h, 000h, 000h, 0A0h, 000h, 000h, 000h
        je     NEAR PTR loc_3_02DF
        db  081h, 03Dh, 02Fh, 004h, 000h, 000h, 090h, 000h, 000h, 000h
        je     NEAR PTR loc_3_02DF
        db  081h, 03Dh, 02Fh, 004h, 000h, 000h, 0B0h, 000h, 000h, 000h
        je     NEAR PTR loc_3_02DF
        db  081h, 03Dh, 02Fh, 004h, 000h, 000h, 0C0h, 000h, 000h, 000h
        je     NEAR PTR loc_3_0340
        db  081h, 03Dh, 02Fh, 004h, 000h, 000h, 0D0h, 000h, 000h, 000h
        je     NEAR PTR loc_3_0340
        db  081h, 03Dh, 02Fh, 004h, 000h, 000h, 000h, 011h, 088h, 000h
        je     NEAR PTR loc_3_0340
        db  081h, 03Dh, 02Fh, 004h, 000h, 000h, 040h, 011h, 088h, 000h
        je     NEAR PTR loc_3_0340
        db  081h, 03Dh, 02Fh, 004h, 000h, 000h, 000h, 010h, 088h, 000h
        je     NEAR PTR loc_3_0340
        db  081h, 03Dh, 02Fh, 004h, 000h, 000h, 000h, 080h, 088h, 000h
        je     NEAR PTR loc_3_0340
        db  081h, 03Dh, 02Fh, 004h, 000h, 000h, 000h, 090h, 088h, 000h
        je     NEAR PTR loc_3_0340
        db  081h, 03Dh, 02Fh, 004h, 000h, 000h, 000h, 0B0h, 088h, 000h
        je     NEAR PTR loc_3_0340
        db  081h, 03Dh, 02Fh, 004h, 000h, 000h, 000h, 0F0h, 088h, 000h
        je     NEAR PTR loc_3_0340
        db  081h, 03Dh, 02Fh, 004h, 000h, 000h, 000h, 012h, 088h, 000h
        je     NEAR PTR loc_3_0340
        db  081h, 03Dh, 02Fh, 004h, 000h, 000h, 000h, 015h, 088h, 000h
        je     NEAR PTR loc_3_0340
        db  081h, 03Dh, 02Fh, 004h, 000h, 000h, 000h, 014h, 088h, 000h
        je     NEAR PTR loc_3_0340
        db  081h, 03Dh, 02Fh, 004h, 000h, 000h, 000h, 03Dh, 088h, 000h
        je     NEAR PTR loc_3_051D
        db  081h, 03Dh, 02Fh, 004h, 000h, 000h, 000h, 031h, 056h, 000h
        je     NEAR PTR loc_3_051D
loc_3_0286:
        mov    al, 41h
        out    dx, al
        inc    dl
        in     al, dx
        and    al, 0f8h
        cmp    esi, 280h
        je     SHORT loc_3_02CE
        cmp    esi, 320h
        je     SHORT loc_3_02BC
        cmp    esi, 400h
        je     SHORT loc_3_02AA
        mov    ah, 7
        jmp    SHORT loc_3_02D7
loc_3_02AA:
        mov    ah, 7
        cmp    edi, 46h
        jae    SHORT loc_3_02D7
        mov    ah, 6
        cmp    edi, 3ch
        jae    SHORT loc_3_02D7
        mov    ah, 5
        jmp    SHORT loc_3_02D7
loc_3_02BC:
        mov    ah, 3
        cmp    edi, 48h
        jae    SHORT loc_3_02D7
        mov    ah, 2
        cmp    edi, 3ch
        jae    SHORT loc_3_02D7
        mov    ah, 4
        jmp    SHORT loc_3_02D7
loc_3_02CE:
        mov    ah, 1
        cmp    edi, 48h
        jae    SHORT loc_3_02D7
        xor    ah, ah
loc_3_02D7:
        or     al, ah
        out    dx, al
        jmp    NEAR PTR loc_3_06C6
loc_3_02DF:
        mov    ch, 3
        cmp    esi, 280h
        je     SHORT loc_3_0315
        mov    ch, 0ch
        cmp    esi, 320h
        je     SHORT loc_3_0315
        mov    ch, 30h
        cmp    esi, 400h
        je     SHORT loc_3_0315
        mov    ch, 30h
        cmp    esi, 480h
        je     SHORT loc_3_0315
        mov    ch, 0c0h
        cmp    esi, 500h
        jne    NEAR PTR loc_3_06C6
loc_3_0315:
        mov    cl, ch
        not    cl
        mov    ah, 0ffh
        cmp    edi, 48h
        jae    SHORT loc_3_0330
        mov    ah, 0aah
        cmp    edi, 3ch
        jae    SHORT loc_3_0330
        mov    ah, 55h
        cmp    edi, 38h
        jae    SHORT loc_3_0330
        xor    ah, ah
loc_3_0330:
        mov    al, 52h
        out    dx, al
        inc    edx
        in     al, dx
        and    ax, cx
        or     al, ah
        out    dx, al
        jmp    NEAR PTR loc_3_06C6
loc_3_0340:
        mov    ah, 41h
        db  081h, 03Dh, 02Fh, 004h, 000h, 000h, 000h, 011h, 088h, 000h
        je     SHORT loc_3_038C
        db  081h, 03Dh, 02Fh, 004h, 000h, 000h, 000h, 010h, 088h, 000h
        je     SHORT loc_3_038C
        db  081h, 03Dh, 02Fh, 004h, 000h, 000h, 040h, 011h, 088h, 000h
        je     SHORT loc_3_038C
        db  081h, 03Dh, 02Fh, 004h, 000h, 000h, 000h, 012h, 088h, 000h
        je     SHORT loc_3_038C
        db  081h, 03Dh, 02Fh, 004h, 000h, 000h, 000h, 015h, 088h, 000h
        je     SHORT loc_3_038C
        db  081h, 03Dh, 02Fh, 004h, 000h, 000h, 000h, 014h, 088h, 000h
        je     SHORT loc_3_038C
        mov    ah, 5bh
loc_3_038C:
        cmp    esi, 280h
        jne    SHORT loc_3_03E6
        mov    cx, 8f30h
        db  066h, 0C7h, 005h, 03Ah, 004h, 000h, 000h, 055h, 000h
        cmp    edi, 55h
        jae    SHORT loc_3_03D7
        mov    cx, 8f20h
        db  066h, 0C7h, 005h, 03Ah, 004h, 000h, 000h, 04Bh, 000h
        cmp    edi, 4bh
        jae    SHORT loc_3_03D7
        mov    cx, 8f10h
        db  066h, 0C7h, 005h, 03Ah, 004h, 000h, 000h, 048h, 000h
        cmp    edi, 48h
        jae    SHORT loc_3_03D7
        mov    cx, 8f00h
        db  066h, 0C7h, 005h, 03Ah, 004h, 000h, 000h, 03Ch, 000h
loc_3_03D7:
        mov    al, 52h
        out    dx, al
        inc    edx
        in     al, dx
        and    al, ch
        or     al, cl
        out    dx, al
        jmp    NEAR PTR loc_3_06C6
loc_3_03E6:
        cmp    esi, 320h
        jne    SHORT loc_3_045D
        mov    cx, 200h
        db  066h, 0C7h, 005h, 03Ah, 004h, 000h, 000h, 055h, 000h
        cmp    edi, 55h
        jae    SHORT loc_3_0443
        mov    cx, 180h
        db  066h, 0C7h, 005h, 03Ah, 004h, 000h, 000h, 04Bh, 000h
        cmp    edi, 4bh
        jae    SHORT loc_3_0443
        mov    cx, 100h
        db  066h, 0C7h, 005h, 03Ah, 004h, 000h, 000h, 048h, 000h
        cmp    edi, 48h
        jae    SHORT loc_3_0443
        mov    cx, 80h
        db  066h, 0C7h, 005h, 03Ah, 004h, 000h, 000h, 03Ch, 000h
        cmp    edi, 3ch
        jae    SHORT loc_3_0443
        mov    cx, 0
        db  066h, 0C7h, 005h, 03Ah, 004h, 000h, 000h, 038h, 000h
loc_3_0443:
        mov    al, 52h
        out    dx, al
        inc    edx
        in     al, dx
        and    al, 7fh
        or     al, cl
        out    dx, al
        dec    edx
        mov    al, ah
        out    dx, al
        inc    edx
        in     al, dx
        and    al, 0fch
        or     al, ch
        out    dx, al
        jmp    NEAR PTR loc_3_06C6
loc_3_045D:
        cmp    esi, 400h
        je     SHORT loc_3_046D
        cmp    esi, 480h
        jne    SHORT loc_3_04C7
loc_3_046D:
        mov    cl, 14h
        db  066h, 0C7h, 005h, 03Ah, 004h, 000h, 000h, 055h, 000h
        cmp    edi, 55h
        jae    SHORT loc_3_04B8
        mov    cl, 10h
        db  066h, 0C7h, 005h, 03Ah, 004h, 000h, 000h, 04Bh, 000h
        cmp    edi, 4bh
        jae    SHORT loc_3_04B8
        mov    cl, 0ch
        db  066h, 0C7h, 005h, 03Ah, 004h, 000h, 000h, 046h, 000h
        cmp    edi, 46h
        jae    SHORT loc_3_04B8
        mov    cl, 8
        db  066h, 0C7h, 005h, 03Ah, 004h, 000h, 000h, 03Ch, 000h
        cmp    edi, 3ch
        jae    SHORT loc_3_04B8
        mov    cl, 0
        db  066h, 0C7h, 005h, 03Ah, 004h, 000h, 000h, 02Bh, 000h
loc_3_04B8:
        mov    al, ah
        out    dx, al
        inc    edx
        in     al, dx
        and    al, 0e3h
        or     al, cl
        out    dx, al
        jmp    NEAR PTR loc_3_06C6
loc_3_04C7:
        cmp    esi, 500h
        jne    NEAR PTR loc_3_06C6
        mov    cl, 0c0h
        db  066h, 0C7h, 005h, 03Ah, 004h, 000h, 000h, 04Bh, 000h
        cmp    edi, 4bh
        jae    SHORT loc_3_050E
        mov    cl, 0a0h
        db  066h, 0C7h, 005h, 03Ah, 004h, 000h, 000h, 048h, 000h
        cmp    edi, 48h
        jae    SHORT loc_3_050E
        mov    cl, 80h
        db  066h, 0C7h, 005h, 03Ah, 004h, 000h, 000h, 03Ch, 000h
        cmp    edi, 3ch
        jae    SHORT loc_3_050E
        mov    cl, 0
        db  066h, 0C7h, 005h, 03Ah, 004h, 000h, 000h, 02Bh, 000h
loc_3_050E:
        mov    al, ah
        out    dx, al
        inc    edx
        in     al, dx
        and    al, 1fh
        or     al, cl
        out    dx, al
        jmp    NEAR PTR loc_3_06C6
loc_3_051D:
        mov    ah, 41h
        cmp    esi, 280h
        jne    SHORT loc_3_058B
        mov    cx, 8f40h
        db  066h, 0C7h, 005h, 03Ah, 004h, 000h, 000h, 055h, 000h
        cmp    edi, 55h
        jae    SHORT loc_3_057C
        mov    cx, 8f30h
        db  066h, 0C7h, 005h, 03Ah, 004h, 000h, 000h, 04Bh, 000h
        cmp    edi, 4bh
        jae    SHORT loc_3_057C
        mov    cx, 8f20h
        db  066h, 0C7h, 005h, 03Ah, 004h, 000h, 000h, 046h, 000h
        cmp    edi, 46h
        jae    SHORT loc_3_057C
        mov    cx, 8f10h
        db  066h, 0C7h, 005h, 03Ah, 004h, 000h, 000h, 03Ch, 000h
        cmp    edi, 3ch
        jae    SHORT loc_3_057C
        mov    cx, 8f00h
        db  066h, 0C7h, 005h, 03Ah, 004h, 000h, 000h, 038h, 000h
loc_3_057C:
        mov    al, 52h
        out    dx, al
        inc    edx
        in     al, dx
        and    al, ch
        or     al, cl
        out    dx, al
        jmp    NEAR PTR loc_3_06C6
loc_3_058B:
        cmp    esi, 320h
        jne    SHORT loc_3_0602
        mov    cx, 200h
        db  066h, 0C7h, 005h, 03Ah, 004h, 000h, 000h, 055h, 000h
        cmp    edi, 55h
        jae    SHORT loc_3_05E8
        mov    cx, 180h
        db  066h, 0C7h, 005h, 03Ah, 004h, 000h, 000h, 04Bh, 000h
        cmp    edi, 4bh
        jae    SHORT loc_3_05E8
        mov    cx, 100h
        db  066h, 0C7h, 005h, 03Ah, 004h, 000h, 000h, 046h, 000h
        cmp    edi, 46h
        jae    SHORT loc_3_05E8
        mov    cx, 80h
        db  066h, 0C7h, 005h, 03Ah, 004h, 000h, 000h, 03Ch, 000h
        cmp    edi, 3ch
        jae    SHORT loc_3_05E8
        mov    cx, 0
        db  066h, 0C7h, 005h, 03Ah, 004h, 000h, 000h, 038h, 000h
loc_3_05E8:
        mov    al, 52h
        out    dx, al
        inc    edx
        in     al, dx
        and    al, 7fh
        or     al, cl
        out    dx, al
        dec    edx
        mov    al, ah
        out    dx, al
        inc    edx
        in     al, dx
        and    al, 0fch
        or     al, ch
        out    dx, al
        jmp    NEAR PTR loc_3_06C6
loc_3_0602:
        cmp    esi, 400h
        je     SHORT loc_3_0612
        cmp    esi, 480h
        jne    SHORT loc_3_0669
loc_3_0612:
        mov    cl, 10h
        db  066h, 0C7h, 005h, 03Ah, 004h, 000h, 000h, 055h, 000h
        cmp    edi, 55h
        jae    SHORT loc_3_065D
        mov    cl, 0ch
        db  066h, 0C7h, 005h, 03Ah, 004h, 000h, 000h, 04Bh, 000h
        cmp    edi, 4bh
        jae    SHORT loc_3_065D
        mov    cl, 8
        db  066h, 0C7h, 005h, 03Ah, 004h, 000h, 000h, 046h, 000h
        cmp    edi, 46h
        jae    SHORT loc_3_065D
        mov    cl, 4
        db  066h, 0C7h, 005h, 03Ah, 004h, 000h, 000h, 03Ch, 000h
        cmp    edi, 3ch
        jae    SHORT loc_3_065D
        mov    cl, 0
        db  066h, 0C7h, 005h, 03Ah, 004h, 000h, 000h, 02Bh, 000h
loc_3_065D:
        mov    al, ah
        out    dx, al
        inc    edx
        in     al, dx
        and    al, 0e3h
        or     al, cl
        out    dx, al
        jmp    SHORT loc_3_06C6
loc_3_0669:
        cmp    esi, 500h
        jne    SHORT loc_3_06C6
        mov    cl, 80h
        db  066h, 0C7h, 005h, 03Ah, 004h, 000h, 000h, 055h, 000h
        cmp    edi, 55h
        jae    SHORT loc_3_06BC
        mov    cl, 60h
        db  066h, 0C7h, 005h, 03Ah, 004h, 000h, 000h, 04Bh, 000h
        cmp    edi, 4bh
        jae    SHORT loc_3_06BC
        mov    cl, 40h
        db  066h, 0C7h, 005h, 03Ah, 004h, 000h, 000h, 046h, 000h
        cmp    edi, 46h
        jae    SHORT loc_3_06BC
        mov    cl, 20h
        db  066h, 0C7h, 005h, 03Ah, 004h, 000h, 000h, 03Ch, 000h
        cmp    edi, 3ch
        jae    SHORT loc_3_06BC
        mov    cl, 0
        db  066h, 0C7h, 005h, 03Ah, 004h, 000h, 000h, 02Bh, 000h
loc_3_06BC:
        mov    al, ah
        out    dx, al
        inc    edx
        in     al, dx
        and    al, 1fh
        or     al, cl
        out    dx, al
loc_3_06C6:
        pop    eax
        dec    edx
        out    dx, al
loc_3_06C9:
        db  066h, 083h, 03Dh, 03Ah, 004h, 000h, 000h, 000h
        je     SHORT loc_3_06E7
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        mov    ebx, dword ptr [ebx + 8]
        db  066h, 08Bh, 00Dh, 03Ah, 004h, 000h, 000h
        mov    word ptr [ebx + 18h], cx
loc_3_06E7:
        ret
loc_3_06E8:
        db  0F6h, 005h, 081h, 004h, 000h, 000h, 001h
        je     SHORT loc_3_070D
        mov    edx, 3d4h
        in     al, dx
        push   eax
        mov    ax, 0a539h
        out    dx, ax
        mov    al, 66h
        out    dx, al
        inc    edx
        in     al, dx
        and    al, 0f7h
        out    dx, al
        pop    eax
        mov    edx, 3d4h
        out    dx, al
loc_3_070D:
        ret
loc_3_070E:
        db  0C6h, 005h, 040h, 004h, 000h, 000h, 000h
        db  081h, 03Dh, 02Fh, 004h, 000h, 000h, 000h, 03Dh, 088h, 000h
        je     SHORT loc_3_0748
        db  081h, 03Dh, 02Fh, 004h, 000h, 000h, 000h, 031h, 056h, 000h
        je     SHORT loc_3_0748
        db  0E8h, 004h, 003h, 0FDh, 0FFh
        mov    edx, 4ae8h
        db  0CDh, 020h, 099h, 000h, 001h, 000h
        mov    edx, 4ae9h
        db  0CDh, 020h, 099h, 000h, 001h, 000h
loc_3_0748:
        db  0E8h, 043h, 0FCh, 0FCh, 0FFh
        ret
loc_3_074E:
        push   eax
        push   edi
        db  0BFh, 0C0h, 003h, 000h, 000h
        mov    al, 67h
        cmp    byte ptr [449h], 3
        jbe    SHORT loc_3_0762
        mov    al, 63h
loc_3_0762:
        mov    byte ptr [edi + 0dh], al
        mov    byte ptr [edi + 2], 0
        mov    byte ptr [edi + 5], 0
        mov    byte ptr [edi + 6], 0
        db  0F6h, 005h, 038h, 004h, 000h, 000h, 001h
        je     SHORT loc_3_077E
        mov    byte ptr [edi + 9], 0
loc_3_077E:
        db  0C6h, 005h, 042h, 004h, 000h, 000h, 0FFh
        pop    edi
        pop    eax
        ret
loc_3_0788:
        cld
        push   edi
        db  0BFh, 0D0h, 003h, 000h, 000h
        mov    byte ptr [edi + 2], 0
        mov    byte ptr [edi + 5], 0
        mov    byte ptr [edi + 6], 0
        db  0F6h, 005h, 038h, 004h, 000h, 000h, 001h
        je     SHORT loc_3_07A8
        mov    byte ptr [edi + 9], 0
loc_3_07A8:
        pop    edi
        ret
_PTEXT ends
        end
