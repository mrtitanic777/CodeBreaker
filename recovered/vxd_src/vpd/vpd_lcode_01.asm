; vpd_lcode_01.asm -- VPD object 1 segment _LTEXT
.386p
_LTEXT segment dword public use32 'LCODE'

VPD_Control:
        cmp    eax, 1
        je     SHORT loc_1_0034
        cmp    eax, 2
        je     NEAR PTR loc_1_0203
        cmp    eax, 0fh
        je     NEAR PTR loc_1_0226
        cmp    eax, 0bh
        je     NEAR PTR loc_1_023F
        cmp    eax, 30h
        db  00Fh, 084h, 031h, 001h, 0FFh, 0FFh
        cmp    eax, 5
        je     NEAR PTR loc_1_027C
        clc
        ret
loc_1_0034:
        xor    esi, esi
        db  0BFh, 058h, 000h, 000h, 000h
        db  0A1h, 0BEh, 009h, 000h, 000h
        db  0CDh, 020h, 0ABh, 000h, 001h, 000h
        db  0A3h, 0BEh, 009h, 000h, 000h
        db  0BFh, 034h, 000h, 000h, 000h
        db  0CDh, 020h, 0ABh, 000h, 001h, 000h
        jb     SHORT loc_1_0075
        test   eax, eax
        je     SHORT loc_1_0075
        cmp    eax, 3
        ja     SHORT loc_1_0075
        dec    eax
        imul   eax, eax, 40h
        db  005h, 0C0h, 008h, 000h, 000h
        db  0A3h, 0C4h, 009h, 000h, 000h
        db  089h, 01Dh, 0C8h, 009h, 000h, 000h
loc_1_0075:
        xor    edi, edi
        xor    ecx, ecx
        db  0BEh, 0C0h, 008h, 000h, 000h
loc_1_007E:
        push   ecx
        push   esi
        push   edi
        add    cl, 31h
        db  088h, 00Dh, 06Fh, 000h, 000h, 000h
        db  088h, 00Dh, 07Ch, 000h, 000h, 000h
        db  088h, 00Dh, 04Fh, 000h, 000h, 000h
        db  088h, 00Dh, 040h, 000h, 000h, 000h
        xor    esi, esi
        db  0BFh, 04Ch, 000h, 000h, 000h
        xor    eax, eax
        db  0CDh, 020h, 0B1h, 000h, 001h, 000h
        test   eax, eax
        pop    edi
        pop    esi
        pop    ecx
        jne    NEAR PTR loc_1_0188
        push   ecx
        push   esi
        push   edi
        xor    esi, esi
        db  0BFh, 03Dh, 000h, 000h, 000h
        mov    eax, 0ea60h
        mov    ecx, 3
        db  0CDh, 020h, 0ADh, 000h, 001h, 000h
        cmp    eax, 0fffffc18h
        jne    SHORT loc_1_00DA
        or     eax, 0ffffffffh
loc_1_00DA:
        pop    edi
        pop    esi
        pop    ecx
        mov    dword ptr [esi + 4], eax
        movzx  edx, word ptr [ecx*2 + 408h]
        test   edx, edx
        je     NEAR PTR loc_1_0188
        mov    dword ptr [esi + 14h], edx
        inc    edi
        test   dh, dh
        je     NEAR PTR loc_1_0188
        db  03Bh, 035h, 0C4h, 009h, 000h, 000h
        je     NEAR PTR loc_1_0188
        db  081h, 0FEh, 0C0h, 008h, 000h, 000h
        je     SHORT loc_1_0131
        db  039h, 015h, 0D4h, 008h, 000h, 000h
        je     SHORT loc_1_0128
        db  081h, 0FEh, 000h, 009h, 000h, 000h
        je     SHORT loc_1_0131
        db  039h, 015h, 014h, 009h, 000h, 000h
        jne    SHORT loc_1_0131
loc_1_0128:
        mov    dword ptr [esi + 14h], 1
        jmp    SHORT loc_1_0188
loc_1_0131:
        pushad
        xor    esi, esi
        db  0BFh, 069h, 000h, 000h, 000h
        xor    eax, eax
        db  0CDh, 020h, 0B1h, 000h, 001h, 000h
        cmp    eax, 0
        popad
        je     SHORT loc_1_0161
        pushad
        xor    esi, esi
        db  0BFh, 071h, 000h, 000h, 000h
        xor    eax, eax
        db  0CDh, 020h, 0B1h, 000h, 001h, 000h
        cmp    eax, 0
        popad
        je     SHORT loc_1_0161
        or     dword ptr [esi + 20h], 10h
loc_1_0161:
        push   ecx
        push   esi
        mov    esi, dword ptr [esi + 8]
        mov    ecx, 3
loc_1_016B:
        db  0CDh, 020h, 096h, 000h, 001h, 000h
        jb     SHORT loc_1_017D
        db  0CDh, 020h, 099h, 000h, 001h, 000h
        inc    edx
        loop   loc_1_016B
        clc
loc_1_017D:
        pop    esi
        pop    ecx
        jae    SHORT loc_1_0188
        mov    dword ptr [esi + 14h], 0
loc_1_0188:
        add    esi, 40h
        inc    ecx
        cmp    ecx, 3
        jb     NEAR PTR loc_1_007E
        test   edi, edi
        jne    SHORT loc_1_019B
        stc
        ret
loc_1_019B:
        db  083h, 03Dh, 0C4h, 009h, 000h, 000h, 000h
        je     SHORT loc_1_01BD
        db  0BFh, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 001h, 000h, 003h, 000h
        jae    SHORT loc_1_01BD
        db  0C7h, 005h, 0C4h, 009h, 000h, 000h, 000h, 000h, 000h, 000h
        jmp    SHORT loc_1_01BD
loc_1_01BD:
        push   0
        push   4
        db  0CDh, 020h, 0A7h, 000h, 001h, 000h
        pop    ecx
        pop    ecx
        test   eax, eax
        jne    SHORT loc_1_01D3
        db  0CDh, 020h, 0BFh, 000h, 001h, 000h
loc_1_01D3:
        db  0A3h, 0BCh, 008h, 000h, 000h
        push   0
        db  068h, 020h, 000h, 000h, 000h
        db  0CDh, 020h, 0A6h, 000h, 001h, 000h
        pop    ecx
        pop    ecx
        test   eax, eax
        jne    SHORT loc_1_01F1
        db  0CDh, 020h, 0BFh, 000h, 001h, 000h
loc_1_01F1:
        mov    eax, 17h
        db  0BEh, 0C4h, 005h, 000h, 000h
        db  0CDh, 020h, 041h, 000h, 001h, 000h
        clc
        ret
loc_1_0203:
        mov    ecx, 3
        db  08Dh, 035h, 0CCh, 008h, 000h, 000h
loc_1_020E:
        db  0CDh, 020h, 007h, 000h, 015h, 000h
        add    esi, 40h
        loop   loc_1_020E
        db  0BEh, 07Eh, 000h, 000h, 000h
        db  0CDh, 020h, 007h, 000h, 015h, 000h
        clc
        ret
loc_1_0226:
        db  083h, 03Dh, 0C4h, 009h, 000h, 000h, 000h
        je     SHORT loc_1_023E
        test   edx, edx
        je     SHORT loc_1_0238
        cmp    edx, 0ah
        jne    SHORT loc_1_023E
loc_1_0238:
        db  089h, 01Dh, 0C8h, 009h, 000h, 000h
loc_1_023E:
        ret
loc_1_023F:
        mov    ecx, 3
        db  0BEh, 0C0h, 008h, 000h, 000h
loc_1_0249:
        cmp    ebx, dword ptr [esi + 1ch]
        jne    SHORT loc_1_0276
        call   sub_1_063F
        cmp    dword ptr [esi + 24h], 0
        je     SHORT loc_1_0276
        push   0
        push   dword ptr [esi + 30h]
        push   dword ptr [esi + 2ch]
        push   esi
        push   1
        db  0E8h, 00Eh, 0FEh, 0FEh, 0FFh
        add    esp, 14h
        push   1
        push   dword ptr [esi + 28h]
        call   dword ptr [esi + 24h]
        pop    ecx
        pop    ecx
loc_1_0276:
        add    esi, 40h
        loop   loc_1_0249
        ret
loc_1_027C:
        mov    ecx, 3
        db  0BEh, 0C0h, 008h, 000h, 000h
loc_1_0286:
        bt     dword ptr [esi + 20h], 3
        jae    SHORT loc_1_0292
        call   sub_1_0736
loc_1_0292:
        add    esi, 40h
        loop   loc_1_0286
        ret
VPD_PM_API:
        db  0CDh, 020h, 004h, 000h, 001h, 000h
        jne    SHORT loc_1_02BB
        movzx  eax, word ptr [ebp + 14h]
        cmp    eax, 12h
        ja     SHORT loc_1_02BB
        db  08Bh, 00Ch, 085h, 0CCh, 009h, 000h, 000h
        jecxz  loc_1_02BB
        db  0FFh, 024h, 085h, 0CCh, 009h, 000h, 000h
loc_1_02BB:
        or     dword ptr [ebp + 2ch], 1
        ret
loc_1_02C0:
        and    dword ptr [ebp + 2ch], 0fffffffeh
        ret
loc_1_02C5:
        mov    word ptr [ebp + 1ch], 30bh
        mov    word ptr [ebp + 14h], 0cb01h
        jmp    SHORT loc_1_02C0
loc_1_02D3:
        db  0BEh, 0C0h, 008h, 000h, 000h
        xor    eax, eax
        mov    ecx, eax
loc_1_02DC:
        cmp    dword ptr [esi + 14h], 0
        je     SHORT loc_1_02E5
        bts    eax, ecx
loc_1_02E5:
        add    esi, 40h
        inc    ecx
        cmp    ecx, 3
        jb     SHORT loc_1_02DC
        mov    word ptr [ebp + 1ch], ax
        jmp    SHORT loc_1_02C0
loc_1_02F4:
        movzx  ecx, word ptr [ebp + 18h]
        cmp    ecx, 3
        jae    SHORT loc_1_02BB
        imul   ecx, ecx, 40h
        db  081h, 0C1h, 0C0h, 008h, 000h, 000h
        mov    eax, dword ptr [ecx + 4]
        mov    word ptr [ebp + 1ch], ax
        shr    eax, 10h
        mov    word ptr [ebp + 10h], ax
        jmp    SHORT loc_1_02C0
loc_1_0316:
        movzx  ecx, word ptr [ebp + 18h]
        cmp    ecx, 3
        jae    SHORT loc_1_02BB
        imul   ecx, ecx, 40h
        db  081h, 0C1h, 0C0h, 008h, 000h, 000h
        mov    ax, word ptr [ebp + 10h]
        shl    eax, 10h
        mov    ax, word ptr [ebp + 1ch]
        mov    dword ptr [ecx + 4], eax
        jmp    SHORT loc_1_02C0
sub_1_0338:
        jne    SHORT loc_1_0351
        db  0A1h, 0BCh, 008h, 000h, 000h
        add    eax, ebx
        mov    ecx, dword ptr [esi]
        push   dword ptr [eax]
        bts    dword ptr [eax], ecx
        call   sub_1_06C0
        pop    dword ptr [eax]
        jb     SHORT loc_1_035A
loc_1_0351:
        xor    ecx, ecx
        dec    ecx
        call   sub_1_0669
        clc
loc_1_035A:
        ret
sub_1_035B:
        mov    edx, dword ptr [esi + 14h]
        cmp    edx, 2
        jb     SHORT loc_1_0379
        mov    ecx, 3
loc_1_0368:
        db  0CDh, 020h, 097h, 000h, 001h, 000h
        inc    edx
        loop   loc_1_0368
        or     dword ptr [esi + 20h], 80h
        clc
loc_1_0379:
        ret
sub_1_037A:
        mov    edx, dword ptr [esi + 14h]
        cmp    edx, 2
        jb     SHORT loc_1_0398
        mov    ecx, 3
loc_1_0387:
        db  0CDh, 020h, 099h, 000h, 001h, 000h
        inc    edx
        loop   loc_1_0387
        and    dword ptr [esi + 20h], 0ffffff7fh
        clc
loc_1_0398:
        ret
loc_1_0399:
        movzx  ecx, word ptr [ebp + 18h]
        cmp    ecx, 3
        jae    NEAR PTR loc_1_02BB
        imul   ecx, ecx, 40h
        db  08Dh, 0B1h, 0C0h, 008h, 000h, 000h
        test   byte ptr [esi + 20h], 8
        je     SHORT loc_1_03C4
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        cmp    dword ptr [esi + 1ch], ebx
        je     NEAR PTR loc_1_02C0
loc_1_03C4:
        test   byte ptr [ebp + 1ch], 1
        call   sub_1_0338
        jae    NEAR PTR loc_1_02C0
        jb     NEAR PTR loc_1_02BB
loc_1_03D9:
        movzx  ecx, word ptr [ebp + 18h]
        cmp    ecx, 3
        jae    NEAR PTR loc_1_02BB
        imul   ecx, ecx, 40h
        db  08Dh, 0B1h, 0C0h, 008h, 000h, 000h
        cmp    dword ptr [esi + 1ch], ebx
        jne    NEAR PTR loc_1_02BB
        call   sub_1_063F
        jmp    NEAR PTR loc_1_02C0
loc_1_0402:
        movzx  ecx, word ptr [ebp + 18h]
        cmp    ecx, 3
        jae    NEAR PTR loc_1_02BB
        imul   ecx, ecx, 40h
        db  08Dh, 0B1h, 0C0h, 008h, 000h, 000h
        movzx  ecx, word ptr [ebp + 1ch]
        cmp    ecx, dword ptr [esi + 38h]
        jne    SHORT loc_1_042B
        or     ecx, ecx
        je     NEAR PTR loc_1_02C0
        jmp    SHORT loc_1_046F
loc_1_042B:
        mov    eax, dword ptr [esi + 34h]
        or     eax, eax
        je     SHORT loc_1_0444
        push   0
        push   eax
        db  0CDh, 020h, 051h, 000h, 001h, 000h
        pop    ecx
        pop    ecx
        mov    dword ptr [esi + 34h], 0
loc_1_0444:
        mov    dword ptr [esi + 38h], ecx
        or     ecx, ecx
        je     NEAR PTR loc_1_02C0
        push   ecx
        push   1
        push   ecx
        db  0CDh, 020h, 04Fh, 000h, 001h, 000h
        pop    ecx
        pop    ecx
        pop    ecx
        or     eax, eax
        jne    SHORT loc_1_046C
        mov    dword ptr [esi + 38h], 0
        jmp    NEAR PTR loc_1_02BB
loc_1_046C:
        mov    dword ptr [esi + 34h], eax
loc_1_046F:
        mov    edi, dword ptr [esi + 34h]
        mov    ax, 3800h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        cmp    eax, -1
        jne    SHORT loc_1_04A2
        mov    eax, dword ptr [esi + 34h]
        push   0
        push   eax
        db  0CDh, 020h, 051h, 000h, 001h, 000h
        pop    ecx
        pop    ecx
        mov    dword ptr [esi + 34h], 0
        mov    dword ptr [esi + 38h], 0
        jmp    NEAR PTR loc_1_02BB
loc_1_04A2:
        mov    esi, eax
        cld
        rep movsb
        jmp    NEAR PTR loc_1_02C0
loc_1_04AC:
        movzx  ecx, word ptr [ebp + 18h]
        cmp    ecx, 3
        jae    NEAR PTR loc_1_02BB
        imul   ecx, ecx, 40h
        db  08Dh, 0B1h, 0C0h, 008h, 000h, 000h
        test   byte ptr [esi + 20h], 20h
        jne    NEAR PTR loc_1_02BB
        test   byte ptr [esi + 20h], 1
        jne    NEAR PTR loc_1_02BB
        cmp    dword ptr [esi + 1ch], 0
        je     SHORT loc_1_04F4
        cmp    dword ptr [esi + 4], -1
        je     SHORT loc_1_04F4
        db  0CDh, 020h, 03Fh, 000h, 001h, 000h
        sub    eax, dword ptr [esi + 18h]
        cmp    eax, dword ptr [esi + 4]
        jb     NEAR PTR loc_1_02BB
loc_1_04F4:
        xor    al, al
        or     dword ptr [esi + 20h], 40h
        call   sub_1_0338
        jae    SHORT loc_1_050A
        and    dword ptr [esi + 20h], 0ffffffbfh
        jmp    NEAR PTR loc_1_02BB
loc_1_050A:
        and    dword ptr [esi + 20h], 0ffffffbfh
        or     dword ptr [esi + 20h], 8
        jmp    NEAR PTR loc_1_02C0
loc_1_0517:
        movzx  ecx, word ptr [ebp + 18h]
        cmp    ecx, 3
        jae    NEAR PTR loc_1_02BB
        imul   ecx, ecx, 40h
        db  08Dh, 0B1h, 0C0h, 008h, 000h, 000h
        test   byte ptr [esi + 20h], 8
        je     NEAR PTR loc_1_02BB
        call   sub_1_063F
        jmp    NEAR PTR loc_1_02C0
loc_1_0541:
        movzx  ecx, word ptr [ebp + 18h]
        cmp    ecx, 3
        jae    NEAR PTR loc_1_02BB
        imul   ecx, ecx, 40h
        db  08Dh, 0B1h, 0C0h, 008h, 000h, 000h
        mov    edx, dword ptr [esi + 14h]
        test   dh, dh
        jne    SHORT loc_1_0563
        jmp    NEAR PTR loc_1_02BB
loc_1_0563:
        inc    dx
        in     al, dx
        xor    ah, ah
        mov    word ptr [ebp + 1ch], ax
        jmp    NEAR PTR loc_1_02C0
        db  02Eh, 08Bh, 0C0h
loc_1_0574:
        db  0BEh, 0C0h, 008h, 000h, 000h
        jmp    SHORT loc_1_058C
        db  090h
loc_1_057C:
        db  0BEh, 000h, 009h, 000h, 000h
        jmp    SHORT loc_1_058C
        db  090h
loc_1_0584:
        db  0BEh, 040h, 009h, 000h, 000h
        jmp    SHORT loc_1_058C
        db  090h
loc_1_058C:
        cmp    ecx, 4
        jbe    SHORT loc_1_0597
        db  0CDh, 020h, 094h, 080h, 001h, 000h
loc_1_0597:
        mov    edi, edx
        and    edi, 3
        db  0FFh, 0A4h, 0F9h, 018h, 00Ah, 000h, 000h
loc_1_05A3:
        in     al, dx
        ret
        db  02Eh, 08Bh, 0C0h
loc_1_05A8:
        call   sub_1_06C0
        jb     SHORT loc_1_05B0
        out    dx, al
loc_1_05B0:
        ret
loc_1_05B1:
        in     al, dx
        cmp    dword ptr [esi + 1ch], 0
        je     SHORT loc_1_05C1
        call   sub_1_06C0
        jae    SHORT loc_1_05C1
        mov    al, 0b8h
loc_1_05C1:
        ret
        db  08Bh, 0C0h
loc_1_05C4:
        cmp    byte ptr [ebp + 1dh], 2
        jae    SHORT loc_1_0625
        movzx  esi, word ptr [ebp + 14h]
        cmp    esi, 2
        ja     SHORT loc_1_0625
        imul   esi, esi, 40h
        db  081h, 0C6h, 0C0h, 008h, 000h, 000h
        test   byte ptr [esi + 20h], 80h
        je     SHORT loc_1_0625
        call   sub_1_06C0
        jae    SHORT loc_1_05EF
loc_1_05E9:
        mov    byte ptr [ebp + 1dh], 21h
        clc
        ret
loc_1_05EF:
        cmp    byte ptr [ebp + 1dh], 0
        jne    SHORT loc_1_0625
        test   byte ptr [esi + 20h], 2
        jne    SHORT loc_1_0625
        db  0CDh, 020h, 082h, 000h, 001h, 000h
        mov    ecx, dword ptr [ebp + 1ch]
        mov    byte ptr [ebp + 1dh], 2
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        xchg   dword ptr [ebp + 1ch], ecx
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        test   ch, 80h
        je     SHORT loc_1_05E9
        test   ch, 20h
        jne    SHORT loc_1_05E9
        or     dword ptr [esi + 20h], 2
loc_1_0625:
        stc
        ret
loc_1_0627:
        db  08Bh, 01Dh, 0C8h, 009h, 000h, 000h
        db  0CDh, 020h, 002h, 080h, 003h, 000h
loc_1_0633:
        db  0CDh, 020h, 003h, 000h, 003h, 000h
        db  0CDh, 020h, 004h, 080h, 003h, 000h
sub_1_063F:
        pushad
        and    dword ptr [esi + 20h], 0fffffffeh
        mov    ebx, dword ptr [esi + 1ch]
        test   ebx, ebx
        je     SHORT loc_1_0667
        mov    edx, dword ptr [esi + 14h]
        test   dh, dh
        je     SHORT loc_1_0667
        mov    ecx, 3
loc_1_0657:
        db  0CDh, 020h, 098h, 000h, 001h, 000h
        inc    edx
        loop   loc_1_0657
        mov    dword ptr [esi + 1ch], 0
loc_1_0667:
        popad
        ret
sub_1_0669:
        pushad
        mov    edx, dword ptr [esi + 14h]
        test   dh, dh
        je     SHORT loc_1_06BB
        call   sub_1_063F
        mov    dword ptr [esi + 1ch], ebx
        test   byte ptr [esi + 20h], 8
        je     SHORT loc_1_0693
        test   byte ptr [esi + 20h], 40h
        jne    SHORT loc_1_0693
        and    dword ptr [esi + 20h], 0fffffff7h
        call   sub_1_0736
        call   sub_1_075D
loc_1_0693:
        jecxz  loc_1_0699
        or     dword ptr [esi + 20h], 1
loc_1_0699:
        db  0A1h, 0BCh, 008h, 000h, 000h
        add    eax, ebx
        mov    edi, dword ptr [esi]
        btr    dword ptr [eax], edi
        jecxz  loc_1_06AD
        db  0CDh, 020h, 09Ah, 000h, 001h, 000h
loc_1_06AD:
        inc    edx
        db  0CDh, 020h, 09Ah, 000h, 001h, 000h
        inc    edx
        db  0CDh, 020h, 09Ah, 000h, 001h, 000h
loc_1_06BB:
        popad
        ret
        db  02Eh, 08Bh, 0C0h
sub_1_06C0:
        push   eax
        cmp    dword ptr [esi + 1ch], ebx
        je     SHORT loc_1_06ED
        cmp    dword ptr [esi + 1ch], 0
        je     SHORT loc_1_06E6
        test   byte ptr [esi + 20h], 1
        jne    SHORT loc_1_06F9
        cmp    dword ptr [esi + 4], -1
        je     SHORT loc_1_06F9
        db  0CDh, 020h, 03Fh, 000h, 001h, 000h
        sub    eax, dword ptr [esi + 18h]
        cmp    eax, dword ptr [esi + 4]
        jb     SHORT loc_1_06F9
loc_1_06E6:
        xor    ecx, ecx
        call   sub_1_0669
loc_1_06ED:
        db  0CDh, 020h, 03Fh, 000h, 001h, 000h
        mov    dword ptr [esi + 18h], eax
        pop    eax
        clc
        ret
loc_1_06F9:
        pushad
        db  0A1h, 0BCh, 008h, 000h, 000h
        add    eax, ebx
        mov    ecx, dword ptr [esi]
        bts    dword ptr [eax], ecx
        jb     SHORT loc_1_0732
        mov    eax, dword ptr [esi + 1ch]
        push   esi
        lea    esi, [esi + 0ch]
        db  0CDh, 020h, 001h, 000h, 017h, 000h
        pop    esi
        mov    eax, dword ptr [esi + 1ch]
        cmp    ebx, eax
        je     SHORT loc_1_0732
        mov    ecx, dword ptr [esi]
        db  003h, 005h, 0BCh, 008h, 000h, 000h
        bts    dword ptr [eax], ecx
        xor    ecx, ecx
        call   sub_1_0669
        popad
        jmp    SHORT loc_1_06ED
loc_1_0732:
        popad
        pop    eax
        stc
        ret
sub_1_0736:
        pushad
        mov    edi, esi
        bt     dword ptr [edi + 20h], 4
        jae    SHORT loc_1_074B
        mov    esi, dword ptr [edi + 34h]
        mov    ecx, dword ptr [edi + 38h]
        jcxz   loc_1_075B
        jmp    SHORT loc_1_0756
loc_1_074B:
        db  08Dh, 035h, 080h, 009h, 000h, 000h
        mov    ecx, 3eh
loc_1_0756:
        call   sub_1_07ED
loc_1_075B:
        popad
        ret
sub_1_075D:
        pushad
        mov    edi, esi
        cld
        or     dword ptr [edi + 20h], 20h
        db  0CDh, 020h, 083h, 000h, 001h, 000h
        db  0CDh, 020h, 03Fh, 000h, 001h, 000h
        add    eax, 0fa0h
        mov    esi, eax
loc_1_0778:
        db  0CDh, 020h, 085h, 000h, 001h, 000h
        db  0CDh, 020h, 03Fh, 000h, 001h, 000h
        cmp    eax, esi
        jb     SHORT loc_1_0778
        db  0CDh, 020h, 03Fh, 000h, 001h, 000h
        add    eax, 0dac0h
        mov    esi, eax
        mov    edx, dword ptr [edi + 14h]
        inc    dx
loc_1_079A:
        db  0CDh, 020h, 085h, 000h, 001h, 000h
        in     al, dx
        and    al, 0b8h
        cmp    al, 98h
        je     SHORT loc_1_07B1
        db  0CDh, 020h, 03Fh, 000h, 001h, 000h
        cmp    eax, esi
        jb     SHORT loc_1_079A
loc_1_07B1:
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        and    dword ptr [edi + 20h], 0ffffffdfh
        bt     dword ptr [edi + 20h], 4
        jb     SHORT loc_1_07EB
        db  0CDh, 020h, 03Fh, 000h, 001h, 000h
        db  003h, 005h, 0BEh, 009h, 000h, 000h
        mov    dword ptr [edi + 3ch], eax
        mov    edx, dword ptr [edi + 14h]
        in     al, dx
        mov    ah, al
        mov    ecx, 80h
loc_1_07DC:
        xor    eax, eax
        call   sub_1_0814
        jb     SHORT loc_1_07E8
        loop   loc_1_07DC
        clc
loc_1_07E8:
        mov    al, ah
        out    dx, al
loc_1_07EB:
        popad
        ret
sub_1_07ED:
        pushad
        cld
        db  0CDh, 020h, 03Fh, 000h, 001h, 000h
        db  003h, 005h, 0BEh, 009h, 000h, 000h
        mov    dword ptr [edi + 3ch], eax
        mov    edx, dword ptr [edi + 14h]
        in     al, dx
        mov    ah, al
loc_1_0804:
        lodsb
        call   sub_1_0814
        jb     SHORT loc_1_080F
        loop   loc_1_0804
        clc
loc_1_080F:
        mov    al, ah
        out    dx, al
        popad
        ret
sub_1_0814:
        pushad
        push   eax
        mov    edx, dword ptr [edi + 14h]
        inc    dx
loc_1_081B:
        db  0CDh, 020h, 03Fh, 000h, 001h, 000h
        cmp    dword ptr [edi + 3ch], eax
        jbe    SHORT loc_1_0868
        in     al, dx
        test   al, 80h
        jne    SHORT loc_1_083F
        db  0CDh, 020h, 083h, 000h, 001h, 000h
        db  0CDh, 020h, 085h, 000h, 001h, 000h
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        jmp    SHORT loc_1_081B
loc_1_083F:
        dec    dx
        pop    eax
        out    dx, al
        jmp    SHORT loc_1_0845
loc_1_0845:
        jmp    SHORT loc_1_0847
loc_1_0847:
        inc    dx
        inc    dx
        in     al, dx
        and    al, 1fh
        or     al, 1
        out    dx, al
        jmp    SHORT loc_1_0853
loc_1_0853:
        jmp    SHORT loc_1_0855
loc_1_0855:
        push   ecx
        mov    ecx, 100h
loc_1_085B:
        loop   loc_1_085B
        pop    ecx
        and    al, 0feh
        out    dx, al
        jmp    SHORT loc_1_0863
loc_1_0863:
        jmp    SHORT loc_1_0865
loc_1_0865:
        popad
        clc
        ret
loc_1_0868:
        pop    eax
        popad
        stc
        ret
public VXD_DDB
VXD_DDB label byte
        db  000h, 000h, 000h, 000h, 000h, 004h, 00Fh, 000h, 003h, 000h, 000h, 000h, 056h, 050h, 044h, 020h
        db  020h, 020h, 020h, 020h, 014h, 000h, 000h, 03Ch, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  098h, 002h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h
        db  050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h, 032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h
        db  000h, 000h, 000h, 000h
loc_1_08C0:
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        pushad
        db  0EAh, 000h, 000h, 074h, 005h, 000h, 000h
loc_1_08CC:
        dec    esp
        push   eax
        push   esp
        xor    dword ptr [eax], esp
        and    byte ptr [eax], ah
        and    byte ptr [eax], al
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
        add    byte ptr [edx], al
        add    byte ptr [eax], al
        add    byte ptr [eax - 16h], ah
        add    byte ptr [eax], al
        jl     SHORT loc_1_090F
        add    byte ptr [eax], al
        dec    esp
        push   eax
        push   esp
loc_1_090F:
        xor    ah, byte ptr [eax]
        and    byte ptr [eax], ah
        and    byte ptr [eax], al
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
        add    byte ptr [ebx], al
        add    byte ptr [eax], al
        add    byte ptr [eax - 16h], ah
        add    byte ptr [eax], al
        test   byte ptr [504c0000h], al
        push   esp
        xor    esp, dword ptr [eax]
        and    byte ptr [eax], ah
        and    byte ptr [eax], al
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
        add    byte ptr [ebx], bl
        sub    ah, byte ptr [ebx + 31h]
        xor    edi, dword ptr ss:[eax]
        xor    al, 44h
        sbb    ebp, dword ptr [ecx]
        jae    SHORT loc_1_09C8
        cmp    dword ptr [edi + 1bh], edx
        sbb    ebx, dword ptr [ebx]
        sbb    eax, dword ptr [edx + 1800ffffh]
        db  0FFh, 0FFh, 0FFh, 0FFh, 000h, 006h, 000h, 008h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 033h, 003h, 01Dh, 0E5h, 01Bh, 01Bh
loc_1_09BE:
        mov    al, byte ptr [0fh]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h
loc_1_09C8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  0C5h, 002h, 000h, 000h, 0D3h, 002h, 000h, 000h, 0F4h, 002h, 000h, 000h, 016h, 003h, 000h, 000h
        db  099h, 003h, 000h, 000h, 0D9h, 003h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 002h, 004h, 000h, 000h
        db  0ACh, 004h, 000h, 000h, 017h, 005h, 000h, 000h, 041h, 005h, 000h, 000h
loc_1_0A18:
        db  0A3h, 005h, 000h, 000h, 0A8h
        db  005h, 000h, 000h, 0B1h, 005h
        add    byte ptr [eax], al
        test   al, 5
        add    byte ptr [eax], al
        db  0A3h, 005h, 000h, 000h, 0A8h
        db  005h, 000h, 000h
_LTEXT ends
        end
