; mssndsys_pcode_04.asm -- MSSNDSYS object 4 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

sub_4_0000:
        push   ebp
        mov    ebp, esp
        push   edi
        push   0
        push   dword ptr [ebp + 8]
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_000E:
        add    esp, 8
        or     edi, edi
        je     SHORT loc_4_0021
        cmp    dword ptr [edi + 24h], 0
        jne    SHORT loc_4_0028
        cmp    dword ptr [edi + 30h], 0
        jne    SHORT loc_4_0028
loc_4_0021:
        mov    eax, 0
        jmp    SHORT loc_4_002D
loc_4_0028:
        mov    eax, 0ffffffffh
loc_4_002D:
        pop    edi
        pop    ebp
        ret
sub_4_0030:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   0
        push   dword ptr [ebp + 8]
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_003F:
        add    esp, 8
        or     edi, edi
        je     SHORT loc_4_006C
        mov    esi, dword ptr [ebp + 0ch]
        mov    dword ptr [esi], 0
        test   word ptr [edi + 4], 1000h
        jne    SHORT loc_4_006C
        mov    eax, 0ffffffffh
        test   word ptr [edi + 4], 200h
        je     SHORT loc_4_0071
        or     dword ptr [esi], 1
        or     dword ptr [esi], 10h
        jmp    SHORT loc_4_0071
loc_4_006C:
        mov    eax, 0
loc_4_0071:
        pop    edi
        pop    esi
        pop    ebp
        ret
sub_4_0075:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0ch]
        mov    dword ptr [esi], 0
        push   0
        push   dword ptr [ebp + 8]
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_008D:
        add    esp, 8
        or     edi, edi
        je     SHORT loc_4_00A1
        movzx  eax, word ptr [edi + 4]
        mov    dword ptr [esi], eax
        mov    eax, 0ffffffffh
        jmp    SHORT loc_4_00A6
loc_4_00A1:
        mov    eax, 0
loc_4_00A6:
        pop    edi
        pop    esi
        pop    ebp
        ret
sub_4_00AA:
        cmp    eax, 40h
        jle    SHORT loc_4_00B4
        mov    eax, 40h
loc_4_00B4:
        add    eax, 3
        shr    eax, 2
loc_4_00BA:
        mov    ebx, eax
        dec    eax
        bsr    cx, ax
        inc    cl
        mov    eax, 1
        shl    eax, cl
        dec    eax
        mov    ecx, ebx
        push   eax
        db  0CDh, 020h, 01Eh, 000h, 004h, 000h
        mov    edx, 0fffh
        cmp    eax, edx
        jae    SHORT loc_4_00DF
        mov    edx, eax
loc_4_00DF:
        pop    eax
        push   ecx
        sub    esp, 4
        mov    ebx, esp
        push   0eh
        push   ebx
        push   edx
        push   0
        push   eax
        push   0
        push   1
        push   ecx
        db  0CDh, 020h, 053h, 000h, 001h, 000h
        add    esp, 20h
        pop    ebx
        pop    ecx
        or     eax, eax
        jne    SHORT loc_4_010B
        mov    eax, ecx
        shr    eax, 1
        jne    SHORT loc_4_00BA
        xor    ecx, ecx
        stc
        ret
loc_4_010B:
        shl    ecx, 0ch
        clc
        ret
sub_4_0110:
        push   eax
        push   ecx
        push   esi
        push   edi
        db  08Bh, 03Dh, 000h, 000h, 000h, 000h
        mov    esi, dword ptr [edi + ebx]
        or     esi, esi
        je     SHORT loc_4_0177
        cli
        db  0CDh, 020h, 09Dh, 000h, 001h, 000h
        sti
        jb     SHORT loc_4_0177
        push   eax
        mov    edi, eax
        xor    eax, eax
        mov    ecx, 34h
        shr    ecx, 2
        rep stosd
        pop    eax
        mov    edi, dword ptr [esp]
        push   eax
        push   edi
        push   esi
        mov    dword ptr [eax + 4], edi
        lea    edi, [eax + 8]
        db  0BEh, 000h, 000h, 000h, 000h
        mov    ecx, 10h
        shr    ecx, 2
        rep movsd
        pop    esi
        pop    edi
        pop    eax
        test   word ptr [edi + 4], 200h
        je     SHORT loc_4_0169
        mov    byte ptr [eax + 0ch], 0
        mov    byte ptr [eax + 0dh], 0
loc_4_0169:
        cli
        db  0CDh, 020h, 09Fh, 000h, 001h, 000h
        sti
        clc
loc_4_0172:
        pop    edi
        pop    esi
        pop    ecx
        pop    eax
        ret
loc_4_0177:
        stc
        jmp    SHORT loc_4_0172
sub_4_017A:
        push   eax
        push   ecx
        push   esi
        push   edi
        pushfd
        db  08Bh, 035h, 000h, 000h, 000h, 000h
        mov    esi, dword ptr [esi + ebx]
        or     esi, esi
        je     SHORT loc_4_01B2
        cli
        db  0CDh, 020h, 0A3h, 000h, 001h, 000h
        je     SHORT loc_4_01B2
loc_4_0195:
        cmp    dword ptr [eax + 4], edi
        je     SHORT loc_4_01A4
        db  0CDh, 020h, 0A4h, 000h, 001h, 000h
        je     SHORT loc_4_01B2
        jmp    SHORT loc_4_0195
loc_4_01A4:
        push   eax
        db  0CDh, 020h, 0A1h, 000h, 001h, 000h
        pop    eax
        db  0CDh, 020h, 0A2h, 000h, 001h, 000h
loc_4_01B2:
        popfd
        pop    edi
        pop    esi
        pop    ecx
        pop    eax
        ret
sub_4_01B8:
        jmp    SHORT loc_4_01C4
loc_4_01BA:
        mov    esi, 0
        jmp    NEAR PTR loc_4_027D
loc_4_01C4:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        pushad
        mov    dword ptr [ebp - 0ch], edx
        movzx  ecx, word ptr [edi]
        mov    eax, ecx
        shl    eax, 2
        add    eax, ecx
        shl    eax, 1
        mov    dword ptr [ebp - 4], eax
        push   101h
        push   eax
        db  0CDh, 020h, 04Fh, 000h, 001h, 000h
        add    esp, 8
        or     eax, eax
        je     NEAR PTR loc_4_0277
        mov    dword ptr [ebp - 8], eax
        push   edi
        movzx  ecx, word ptr [edi]
        push   ecx
        mov    edi, dword ptr [ebp - 8]
loc_4_01FF:
        push   ecx
        mov    ecx, 0ah
        db  08Dh, 035h, 000h, 000h, 000h, 000h
        cld
        rep movsb
        pop    ecx
        loop   loc_4_01FF
        pop    ecx
        mov    edi, dword ptr [esp]
        mov    esi, dword ptr [ebp - 8]
        add    edi, 2
loc_4_021B:
        mov    eax, dword ptr [ebp - 0ch]
        mov    dword ptr [esi + 1], eax
        mov    eax, dword ptr [edi + 2]
        sub    eax, esi
        sub    eax, 0ah
        mov    dword ptr [esi + 6], eax
        movzx  edx, word ptr [edi]
        db  0CDh, 020h, 096h, 000h, 001h, 000h
        jb     SHORT loc_4_024E
        add    edi, 6
        add    esi, 0ah
        loop   loc_4_021B
        pop    edi
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [esp + 1ch], eax
        jmp    SHORT loc_4_027D
loc_4_024E:
        pop    edi
        movzx  eax, word ptr [edi]
        sub    ecx, eax
        neg    ecx
        jecxz  loc_4_0269
        add    edi, 2
loc_4_025B:
        movzx  edx, word ptr [edi]
        db  0CDh, 020h, 016h, 001h, 001h, 000h
        add    edi, 6
        loop   loc_4_025B
loc_4_0269:
        push   0
        push   dword ptr [ebp - 8]
        db  0CDh, 020h, 051h, 000h, 001h, 000h
        add    esp, 8
loc_4_0277:
        xor    eax, eax
        mov    dword ptr [esp + 1ch], eax
loc_4_027D:
        popad
        add    esp, 0ch
        pop    ebp
        ret
sub_4_0283:
        push   ecx
        push   edx
        push   edi
        push   eax
        movzx  ecx, word ptr [edi]
        add    edi, 2
loc_4_028D:
        movzx  edx, word ptr [edi]
        db  0CDh, 020h, 016h, 001h, 001h, 000h
        add    edi, 6
        loop   loc_4_028D
        pop    eax
        push   0
        push   eax
        db  0CDh, 020h, 051h, 000h, 001h, 000h
        add    esp, 8
        pop    edi
        pop    edx
        pop    ecx
        ret
sub_4_02AC:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        pushad
        mov    dword ptr [ebp - 4], 0
        db  08Bh, 035h, 000h, 000h, 000h, 000h
        or     esi, esi
        je     NEAR PTR loc_4_06AA
        db  0CDh, 020h, 09Dh, 000h, 001h, 000h
        jb     NEAR PTR loc_4_06AA
        mov    dword ptr [ebp - 4], eax
        db  0CDh, 020h, 09Fh, 000h, 001h, 000h
        mov    dword ptr [eax], 0
        push   1
        push   0bah
        db  0CDh, 020h, 04Fh, 000h, 001h, 000h
        add    esp, 8
        or     eax, eax
        je     NEAR PTR loc_4_06AA
        mov    edi, eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax], edi
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [edi + 54h], eax
        mov    ax, word ptr [ebp + 30h]
        mov    word ptr [edi + 4], ax
        mov    word ptr [edi + 0b8h], 0
        mov    ax, word ptr [ebp + 34h]
        mov    word ptr [edi + 52h], ax
        mov    ax, word ptr [ebp + 0ch]
        mov    word ptr [edi + 6], ax
        sub    word ptr [edi + 6], 4
        mov    word ptr [edi + 42h], ax
        mov    ax, word ptr [ebp + 20h]
        mov    byte ptr [edi + 8], al
        test   dword ptr [ebp + 30h], 200h
        jne    SHORT loc_4_0356
        mov    ax, word ptr [ebp + 24h]
        mov    byte ptr [edi + 9], al
        mov    ax, word ptr [ebp + 28h]
        mov    byte ptr [edi + 0ah], al
        cmp    ax, word ptr [ebp + 24h]
        je     SHORT loc_4_035B
loc_4_0356:
        or     word ptr [edi + 52h], 40h
loc_4_035B:
        mov    ax, word ptr [ebp + 14h]
        mov    word ptr [edi + 4eh], ax
        cmp    word ptr [ebp + 10h], -1
        je     SHORT loc_4_0390
        push   dword ptr [ebp + 10h]
        call   sub_4_0BC4
        add    esp, 4
        or     eax, eax
        je     NEAR PTR loc_4_06AA
        or     word ptr [edi + 52h], 4
        mov    ah, al
        and    ah, 38h
        shr    ah, 3
        and    al, 7
        mov    word ptr [edi + 0eh], ax
loc_4_0390:
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0395:
        mov    word ptr [edi + 0ch], ax
        db  066h, 0C7h, 005h, 002h, 000h, 000h, 000h, 004h, 000h
        db  0C7h, 005h, 004h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        test   dword ptr [ebp + 30h], 200h
        je     NEAR PTR loc_4_0442
        xor    eax, eax
        mov    dword ptr [edi + 10h], eax
        mov    dword ptr [edi + 14h], eax
        mov    dword ptr [edi + 18h], eax
        mov    dword ptr [edi + 1ch], 8000h
        sub    esp, 8
        mov    eax, esp
        mov    edx, eax
        add    edx, 4
        push   edx
        push   eax
        push   dword ptr [ebp + 8]
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_03DF:
        add    esp, 0ch
        mov    dx, word ptr [ebp + 0ch]
        add    dx, word ptr [esp]
        mov    word ptr [edi + 0aeh], dx
        mov    dx, word ptr [esp + 4]
        mov    word ptr [edi + 0b0h], dx
        add    esp, 8
        cmp    eax, 0
        jne    NEAR PTR loc_4_06AA
        cmp    dx, 100h
        jne    SHORT loc_4_0442
        push   dword ptr [ebp + 8]
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0418:
        add    esp, 4
        cmp    eax, -1
        je     NEAR PTR loc_4_06AA
        add    eax, 14h
        mov    word ptr [edi + 48h], ax
        or     word ptr [edi + 4], 8
        db  066h, 083h, 00Dh, 002h, 000h, 000h, 000h, 002h
        db  0C7h, 005h, 004h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_4_0442:
        push   edi
        movzx  ecx, byte ptr [edi + 8]
        db  066h, 089h, 00Dh, 000h, 000h, 000h, 000h
        db  089h, 03Dh, 01Ch, 000h, 000h, 000h
        db  0BFh, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 001h, 000h, 003h, 000h
        pop    edi
        jae    SHORT loc_4_0474
        db  068h, 000h, 000h, 000h, 000h
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_046C:
        add    esp, 4
        jmp    NEAR PTR loc_4_06AA
loc_4_0474:
        mov    dword ptr [edi + 2ch], eax
        db  0CDh, 020h, 003h, 000h, 001h, 000h
        db  0CDh, 020h, 005h, 000h, 003h, 000h
        test   ecx, 4
        jne    SHORT loc_4_0491
        or     word ptr [edi + 4], 8000h
loc_4_0491:
        test   word ptr [edi + 4], 8
        je     SHORT loc_4_04A1
        db  0CDh, 020h, 009h, 000h, 003h, 000h
        jmp    SHORT loc_4_04AD
loc_4_04A1:
        db  0CDh, 020h, 008h, 000h, 003h, 000h
        db  0CDh, 020h, 004h, 000h, 003h, 000h
loc_4_04AD:
        test   word ptr [edi + 52h], 4
        je     SHORT loc_4_04F6
        db  00Fh, 0B7h, 00Dh, 000h, 000h, 000h, 000h
        db  0BEh, 000h, 000h, 000h, 000h
        movzx  edx, word ptr [ebp + 10h]
        push   edi
        mov    edi, esi
        add    esi, 2
loc_4_04CB:
        mov    word ptr [esi], dx
        inc    edx
        add    esi, 6
        loop   loc_4_04CB
        mov    edx, dword ptr [esp]
        call   sub_4_01B8
        pop    edi
        mov    dword ptr [edi + 58h], eax
        or     eax, eax
        jne    SHORT loc_4_04F6
        db  068h, 000h, 000h, 000h, 000h
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_04EE:
        add    esp, 4
        jmp    NEAR PTR loc_4_06AA
loc_4_04F6:
        db  0BEh, 000h, 000h, 000h, 000h
        movzx  ecx, word ptr [esi]
        movzx  edx, word ptr [ebp + 0ch]
        call   sub_4_0E24
        mov    byte ptr [edi + 0bh], ah
        xor    ah, ah
        mov    word ptr [edi + 4ch], ax
        push   edi
        mov    edi, esi
        add    esi, 2
loc_4_0516:
        mov    word ptr [esi], dx
        inc    edx
        add    esi, 6
        loop   loc_4_0516
        mov    edx, dword ptr [esp]
        call   sub_4_01B8
        pop    edi
        mov    dword ptr [edi + 5ch], eax
        or     eax, eax
        jne    SHORT loc_4_0541
        db  068h, 000h, 000h, 000h, 000h
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0539:
        add    esp, 4
        jmp    NEAR PTR loc_4_06AA
loc_4_0541:
        movsx  edx, word ptr [ebp + 18h]
        cmp    edx, -1
        je     SHORT loc_4_058C
        mov    word ptr [edi + 22h], dx
        or     word ptr [edi + 52h], 8
        db  0BEh, 000h, 000h, 000h, 000h
        movzx  ecx, word ptr [esi]
        push   edi
        mov    edi, esi
        add    esi, 2
loc_4_0561:
        mov    word ptr [esi], dx
        inc    edx
        add    esi, 6
        loop   loc_4_0561
        mov    edx, dword ptr [esp]
        call   sub_4_01B8
        pop    edi
        mov    dword ptr [edi + 60h], eax
        or     eax, eax
        jne    SHORT loc_4_058C
        db  068h, 000h, 000h, 000h, 000h
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0584:
        add    esp, 4
        jmp    NEAR PTR loc_4_06AA
loc_4_058C:
        movsx  edx, word ptr [ebp + 1ch]
        cmp    edx, -1
        je     SHORT loc_4_05D8
        mov    word ptr [edi + 40h], dx
        or     word ptr [edi + 4], 1000h
        db  0BEh, 000h, 000h, 000h, 000h
        movzx  ecx, word ptr [esi]
        push   edi
        mov    edi, esi
        add    esi, 2
loc_4_05AD:
        mov    word ptr [esi], dx
        inc    edx
        add    esi, 6
        loop   loc_4_05AD
        mov    edx, dword ptr [esp]
        call   sub_4_01B8
        pop    edi
        mov    dword ptr [edi + 64h], eax
        or     eax, eax
        jne    SHORT loc_4_05D8
        db  068h, 000h, 000h, 000h, 000h
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_05D0:
        add    esp, 4
        jmp    NEAR PTR loc_4_06AA
loc_4_05D8:
        test   dword ptr [ebp + 30h], 200h
        jne    NEAR PTR loc_4_0681
        mov    eax, dword ptr [ebp + 2ch]
        call   sub_4_00AA
        mov    dword ptr [edi + 10h], eax
        mov    dword ptr [edi + 14h], ebx
        mov    dword ptr [edi + 1ch], ecx
        mov    dword ptr [edi + 18h], edx
        jae    SHORT loc_4_060D
        db  068h, 000h, 000h, 000h, 000h
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0605:
        add    esp, 4
        jmp    NEAR PTR loc_4_06AA
loc_4_060D:
        push   0
        push   0
        push   92h
        push   ecx
        push   edx
        db  0CDh, 020h, 07Ah, 000h, 001h, 000h
        add    esp, 14h
        push   0
        push   eax
        push   edx
        db  0CDh, 020h, 076h, 000h, 001h, 000h
        add    esp, 0ch
        mov    word ptr [edi + 20h], ax
        movzx  eax, word ptr [ebp + 24h]
        db  0BEh, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 001h, 000h, 004h, 000h
        mov    dword ptr [edi + 38h], eax
        jae    SHORT loc_4_0655
        db  068h, 000h, 000h, 000h, 000h
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0650:
        add    esp, 4
        jmp    SHORT loc_4_06AA
loc_4_0655:
        mov    ax, word ptr [ebp + 28h]
        cmp    ax, word ptr [ebp + 24h]
        je     SHORT loc_4_0681
        movzx  eax, al
        db  0BEh, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 001h, 000h, 004h, 000h
        mov    dword ptr [edi + 3ch], eax
        jae    SHORT loc_4_0681
        db  068h, 000h, 000h, 000h, 000h
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_067C:
        add    esp, 4
        jmp    SHORT loc_4_06AA
loc_4_0681:
        db  0CDh, 020h, 003h, 000h, 001h, 000h
        push   ebx
loc_4_0688:
        call   sub_4_0110
        jb     SHORT loc_4_06A9
        db  0CDh, 020h, 03Bh, 000h, 001h, 000h
        cmp    ebx, dword ptr [esp]
        jne    SHORT loc_4_0688
        pop    ebx
        mov    dword ptr [esp + 1ch], 0ffffffffh
loc_4_06A3:
        popad
        add    esp, 4
        pop    ebp
        ret
loc_4_06A9:
        pop    ebx
loc_4_06AA:
        mov    edi, dword ptr [ebp - 4]
        or     edi, edi
        je     SHORT loc_4_06B6
        call   sub_4_06C0
loc_4_06B6:
        mov    dword ptr [esp + 1ch], 0
        jmp    SHORT loc_4_06A3
sub_4_06C0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        pushad
        mov    dword ptr [ebp - 4], edi
        mov    edi, dword ptr [edi]
        or     edi, edi
        je     NEAR PTR loc_4_07FF
        xor    esi, esi
        xchg   dword ptr [edi + 44h], esi
        or     esi, esi
        je     SHORT loc_4_06E3
        db  0CDh, 020h, 09Ch, 000h, 001h, 000h
loc_4_06E3:
        mov    eax, dword ptr [edi + 3ch]
        or     eax, eax
        je     SHORT loc_4_06F0
        db  0CDh, 020h, 01Ah, 000h, 004h, 000h
loc_4_06F0:
        mov    eax, dword ptr [edi + 38h]
        or     eax, eax
        je     SHORT loc_4_06FD
        db  0CDh, 020h, 01Ah, 000h, 004h, 000h
loc_4_06FD:
        movzx  eax, word ptr [edi + 20h]
        or     eax, eax
        je     SHORT loc_4_0720
        push   0
        push   eax
        db  0CDh, 020h, 077h, 000h, 001h, 000h
        add    esp, 8
        mov    eax, dword ptr [edi + 10h]
        push   0
        push   eax
        db  0CDh, 020h, 055h, 000h, 001h, 000h
        add    esp, 8
loc_4_0720:
        mov    eax, dword ptr [edi + 64h]
        or     eax, eax
        je     SHORT loc_4_074E
        db  0BEh, 000h, 000h, 000h, 000h
        movzx  ecx, word ptr [esi]
        movzx  edx, word ptr [edi + 40h]
        push   edi
        mov    edi, esi
        add    esi, 2
loc_4_0739:
        mov    word ptr [esi], dx
        inc    edx
        add    esi, 6
        loop   loc_4_0739
        mov    eax, dword ptr [esp]
        mov    eax, dword ptr [eax + 64h]
        call   sub_4_0283
        pop    edi
loc_4_074E:
        mov    eax, dword ptr [edi + 60h]
        or     eax, eax
        je     SHORT loc_4_077C
        db  0BEh, 000h, 000h, 000h, 000h
        movzx  ecx, word ptr [esi]
        movzx  edx, word ptr [edi + 22h]
        push   edi
        mov    edi, esi
        add    esi, 2
loc_4_0767:
        mov    word ptr [esi], dx
        inc    edx
        add    esi, 6
        loop   loc_4_0767
        mov    eax, dword ptr [esp]
        mov    eax, dword ptr [eax + 60h]
        call   sub_4_0283
        pop    edi
loc_4_077C:
        mov    eax, dword ptr [edi + 5ch]
        or     eax, eax
        je     SHORT loc_4_07AA
        db  0BEh, 000h, 000h, 000h, 000h
        movzx  ecx, word ptr [esi]
        movzx  edx, word ptr [edi + 42h]
        push   edi
        mov    edi, esi
        add    esi, 2
loc_4_0795:
        mov    word ptr [esi], dx
        inc    edx
        add    esi, 6
        loop   loc_4_0795
        mov    eax, dword ptr [esp]
        mov    eax, dword ptr [eax + 5ch]
        call   sub_4_0283
        pop    edi
loc_4_07AA:
        mov    eax, dword ptr [edi + 58h]
        or     eax, eax
        je     SHORT loc_4_07D8
        db  0BEh, 000h, 000h, 000h, 000h
        movzx  ecx, word ptr [esi]
        movzx  edx, word ptr [edi + 6]
        push   edi
        mov    edi, esi
        add    esi, 2
loc_4_07C3:
        mov    word ptr [esi], dx
        inc    edx
        add    esi, 6
        loop   loc_4_07C3
        mov    eax, dword ptr [esp]
        mov    eax, dword ptr [eax + 58h]
        call   sub_4_0283
        pop    edi
loc_4_07D8:
        mov    eax, dword ptr [edi + 2ch]
        or     eax, eax
        je     SHORT loc_4_07F3
        cli
        db  0CDh, 020h, 004h, 000h, 003h, 000h
        db  0CDh, 020h, 008h, 000h, 003h, 000h
        db  0CDh, 020h, 011h, 000h, 003h, 000h
        sti
loc_4_07F3:
        push   0
        push   edi
        db  0CDh, 020h, 051h, 000h, 001h, 000h
        add    esp, 8
loc_4_07FF:
        db  08Bh, 035h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp - 4]
        push   eax
        db  0CDh, 020h, 0A1h, 000h, 001h, 000h
        pop    eax
        db  0CDh, 020h, 0A2h, 000h, 001h, 000h
        popad
        add    esp, 4
        pop    ebp
        ret
sub_4_081C:
        push   ebp
        mov    ebp, esp
        pushad
        xor    eax, eax
        xor    ecx, ecx
        mov    al, byte ptr [ebp + 0ch]
loc_4_0827:
        db  080h, 0B9h, 000h, 000h, 000h, 000h, 0FFh
        je     SHORT loc_4_087B
        db  03Ah, 081h, 000h, 000h, 000h, 000h
        je     SHORT loc_4_083B
        inc    ecx
        jmp    SHORT loc_4_0827
loc_4_083B:
        db  08Ah, 0A1h, 000h, 000h, 000h, 000h
        and    ah, 3fh
        xor    ecx, ecx
        mov    al, byte ptr [ebp + 10h]
loc_4_0849:
        db  080h, 0B9h, 000h, 000h, 000h, 000h, 0FFh
        je     SHORT loc_4_087B
        db  03Ah, 081h, 000h, 000h, 000h, 000h
        je     SHORT loc_4_085D
        inc    ecx
        jmp    SHORT loc_4_0849
loc_4_085D:
        db  00Ah, 0A1h, 000h, 000h, 000h, 000h
        mov    cx, word ptr [ebp + 10h]
        cmp    cx, word ptr [ebp + 14h]
        je     SHORT loc_4_0870
        or     ah, 4
loc_4_0870:
        mov    al, ah
        movzx  edx, word ptr [ebp + 8]
        out    dx, al
        clc
loc_4_0878:
        popad
        pop    ebp
        ret
loc_4_087B:
        stc
        jmp    SHORT loc_4_0878
sub_4_087E:
        push   ebp
        mov    ebp, esp
        pushad
        movzx  edx, word ptr [ebp + 8]
        xor    al, al
        add    edx, 0ah
        out    dx, al
        mov    ecx, 3e8h
        sub    edx, 0ah
loc_4_0894:
        mov    al, 0ah
        out    dx, al
        in     al, dx
        cmp    al, 0ah
        je     SHORT loc_4_089E
        loop   loc_4_0894
loc_4_089E:
        popad
        pop    ebp
        ret
sub_4_08A1:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        pushad
        mov    dword ptr [esp + 1ch], 13h
        movzx  edx, word ptr [ebp + 0ch]
        push   4
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_08BC:
        add    esp, 8
        or     edi, edi
        jne    NEAR PTR loc_4_0A23
        mov    dword ptr [ebp - 4], 0
        test   dword ptr [ebp + 30h], 100h
        jne    NEAR PTR loc_4_09EB
        test   dword ptr [ebp + 30h], 200h
        jne    NEAR PTR loc_4_09E0
        movzx  edx, word ptr [ebp + 0ch]
        call   sub_4_0B7C
        jb     NEAR PTR loc_4_0A29
        movsx  edx, word ptr [ebp + 18h]
        cmp    edx, -1
        je     SHORT loc_4_090B
        call   sub_4_0DB9
        jb     NEAR PTR loc_4_0A29
loc_4_090B:
        movsx  edx, word ptr [ebp + 10h]
        cmp    edx, -1
        je     SHORT loc_4_0973
        test   dword ptr [ebp + 30h], 400h
        jne    SHORT loc_4_095D
        push   edx
        call   sub_4_0BC4
        add    esp, 4
        or     eax, eax
        je     NEAR PTR loc_4_0A29
        push   dword ptr [ebp + 20h]
        push   dword ptr [ebp + 10h]
        call   sub_4_0CE1
        add    esp, 8
        or     eax, eax
        je     NEAR PTR loc_4_0A23
        push   dword ptr [ebp + 28h]
        push   dword ptr [ebp + 24h]
        push   dword ptr [ebp + 10h]
        call   sub_4_0D3C
        add    esp, 0ch
        or     eax, eax
        je     NEAR PTR loc_4_0A23
loc_4_095D:
        push   dword ptr [ebp + 28h]
        push   dword ptr [ebp + 24h]
        push   dword ptr [ebp + 20h]
        push   dword ptr [ebp + 10h]
        call   sub_4_081C
        add    esp, 10h
        jmp    SHORT loc_4_09EB
loc_4_0973:
        cmp    word ptr [ebp + 14h], -1
        je     SHORT loc_4_09B7
        push   1
        push   dword ptr [ebp + 14h]
        push   dword ptr [ebp + 0ch]
        call   sub_4_0C0C
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_4_09AC
        or     dword ptr [ebp - 4], 10h
        push   dword ptr [ebp + 28h]
        push   dword ptr [ebp + 24h]
        push   dword ptr [ebp + 20h]
        push   dword ptr [ebp + 14h]
        call   sub_4_0C61
        add    esp, 10h
        or     eax, eax
        je     SHORT loc_4_0A23
        jmp    SHORT loc_4_09EB
loc_4_09AC:
        call   sub_4_0D92
        jb     SHORT loc_4_09B7
        or     dword ptr [ebp - 4], 30h
loc_4_09B7:
        push   dword ptr [ebp + 20h]
        push   dword ptr [ebp + 0ch]
        call   sub_4_0F94
        add    esp, 8
        or     eax, eax
        je     SHORT loc_4_0A23
        push   dword ptr [ebp + 28h]
        push   dword ptr [ebp + 24h]
        push   dword ptr [ebp + 0ch]
        call   sub_4_114C
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_4_0A23
        jmp    SHORT loc_4_09EB
loc_4_09E0:
        push   dword ptr [ebp + 0ch]
        call   sub_4_087E
        add    esp, 4
loc_4_09EB:
        push   dword ptr [ebp - 4]
        push   dword ptr [ebp + 30h]
        push   dword ptr [ebp + 2ch]
        push   dword ptr [ebp + 28h]
        push   dword ptr [ebp + 24h]
        push   dword ptr [ebp + 20h]
        push   dword ptr [ebp + 1ch]
        push   dword ptr [ebp + 18h]
        push   dword ptr [ebp + 14h]
        push   dword ptr [ebp + 10h]
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp + 8]
        call   sub_4_02AC
        add    esp, 30h
        or     eax, eax
        je     SHORT loc_4_0A23
        mov    dword ptr [esp + 1ch], 0
loc_4_0A23:
        popad
        add    esp, 4
        pop    ebp
        ret
loc_4_0A29:
        mov    dword ptr [esp + 1ch], 24h
        jmp    SHORT loc_4_0A23
sub_4_0A33:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   ebx
        pushfd
        push   0
        push   dword ptr [ebp + 8]
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0A44:
        add    esp, 8
        or     edi, edi
        je     NEAR PTR loc_4_0AF2
        push   dword ptr [ebp + 8]
        call   0
        add    esp, 4
        or     eax, eax
        je     SHORT loc_4_0A80
        mov    ebx, dword ptr [edi + 24h]
        or     ebx, ebx
        je     SHORT loc_4_0A6F
        mov    eax, 1
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0A6F:
        mov    ebx, dword ptr [edi + 30h]
        or     ebx, ebx
        je     SHORT loc_4_0A80
        mov    eax, 2
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0A80:
        cli
        mov    eax, dword ptr [edi + 38h]
        or     eax, eax
        je     SHORT loc_4_0A8E
        db  0CDh, 020h, 018h, 000h, 004h, 000h
loc_4_0A8E:
        mov    eax, dword ptr [edi + 3ch]
        or     eax, eax
        je     SHORT loc_4_0A9B
        db  0CDh, 020h, 018h, 000h, 004h, 000h
loc_4_0A9B:
        test   word ptr [edi + 52h], 4
        je     SHORT loc_4_0AAA
        movzx  edx, word ptr [edi + 6]
        xor    eax, eax
        out    dx, al
loc_4_0AAA:
        mov    ebx, dword ptr [ebp + 8]
        db  08Bh, 035h, 000h, 000h, 000h, 000h
        or     esi, esi
        je     SHORT loc_4_0AF7
        db  0CDh, 020h, 0A3h, 000h, 001h, 000h
        je     SHORT loc_4_0AF7
loc_4_0ABF:
        mov    edi, dword ptr [eax]
        cmp    dword ptr [edi + 54h], ebx
        je     SHORT loc_4_0AD0
        db  0CDh, 020h, 0A4h, 000h, 001h, 000h
        je     SHORT loc_4_0AF7
        jmp    SHORT loc_4_0ABF
loc_4_0AD0:
        sti
        db  0CDh, 020h, 003h, 000h, 001h, 000h
        push   ebx
loc_4_0AD8:
        call   sub_4_017A
        db  0CDh, 020h, 03Bh, 000h, 001h, 000h
        cmp    ebx, dword ptr [esp]
        jne    SHORT loc_4_0AD8
        pop    ebx
        mov    edi, eax
        call   sub_4_06C0
        je     SHORT loc_4_0AF2
loc_4_0AF2:
        mov    eax, 0ffffffffh
loc_4_0AF7:
        popfd
        pop    ebx
        pop    edi
        pop    esi
        pop    ebp
        ret
loc_4_0AFD:
        push   ebp
        mov    ebp, esp
        push   dword ptr [ebp + 18h]
        push   dword ptr [ebp + 14h]
        push   dword ptr [ebp + 10h]
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp + 8]
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0B14:
        add    esp, 14h
        leave
        clc
        ret
loc_4_0B1A:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        mov    eax, esp
        push   0
        push   ebx
        push   eax
        db  0CDh, 020h, 003h, 000h, 033h, 000h
        add    esp, 0ch
        cmp    eax, 0dh
        je     SHORT loc_4_0B64
        cmp    eax, 0
        je     SHORT loc_4_0B3B
        jmp    SHORT loc_4_0B76
loc_4_0B3B:
        push   edi
        mov    eax, dword ptr [esp + 4]
        push   0
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0B48:
        add    esp, 8
        or     edi, edi
        pop    edi
        je     SHORT loc_4_0B64
        push   3
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        push   ebx
        db  0CDh, 020h, 00Eh, 000h, 033h, 000h
        add    esp, 10h
        jmp    SHORT loc_4_0B76
loc_4_0B64:
        mov    eax, ebx
        db  0BBh, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 000h, 000h, 04Ah, 004h
        mov    eax, 0
loc_4_0B76:
        add    esp, 4
        pop    ebp
        stc
        ret
sub_4_0B7C:
        push   eax
        push   ecx
        push   edx
        xor    ecx, ecx
        in     al, dx
        test   al, 80h
        jne    SHORT loc_4_0BBC
        mov    cl, al
        mov    al, 0ch
        out    dx, al
        inc    edx
        in     al, dx
        test   al, 80h
        je     SHORT loc_4_0B93
        and    al, 0bfh
loc_4_0B93:
        cmp    al, 9
        je     SHORT loc_4_0BA5
        cmp    al, 0ah
        je     SHORT loc_4_0BA5
        cmp    al, 81h
        je     SHORT loc_4_0BA5
        cmp    al, 8ah
        je     SHORT loc_4_0BA5
        jmp    SHORT loc_4_0BB8
loc_4_0BA5:
        shl    ecx, 8
        mov    cl, al
        xor    al, al
        out    dx, al
        in     al, dx
        cmp    al, cl
        je     SHORT loc_4_0BBF
        mov    al, cl
        out    dx, al
        shr    ecx, 8
loc_4_0BB8:
        dec    edx
        mov    al, cl
        out    dx, al
loc_4_0BBC:
        stc
        jmp    SHORT loc_4_0BC0
loc_4_0BBF:
        clc
loc_4_0BC0:
        pop    edx
        pop    ecx
        pop    eax
        ret
sub_4_0BC4:
        push   ebp
        mov    ebp, esp
        push   edx
        movzx  edx, word ptr [ebp + 8]
        add    dx, 3
        in     al, dx
        and    al, 3fh
        cmp    al, 4
        jne    SHORT loc_4_0BDE
        movzx  eax, al
        clc
        jmp    SHORT loc_4_0BE1
        db  0C3h
loc_4_0BDE:
        xor    eax, eax
        stc
loc_4_0BE1:
        pop    edx
        pop    ebp
        ret
sub_4_0BE4:
        jmp    SHORT loc_4_0BEC
loc_4_0BE6:
        inc    ebx
        dec    edi
        dec    ebp
        push   eax
        inc    ecx
        push   ecx
loc_4_0BEC:
        push   ecx
        push   edi
        push   esi
        mov    esi, 0fffeah
        db  08Dh, 03Dh, 000h, 000h, 000h, 000h
        mov    ecx, 6
        cld
        repe cmpsb
        jne    SHORT loc_4_0C07
        clc
        jmp    SHORT loc_4_0C08
loc_4_0C07:
        stc
loc_4_0C08:
        pop    esi
        pop    edi
        pop    ecx
        ret
sub_4_0C0C:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   edx
        call   sub_4_0BE4
        jb     SHORT loc_4_0C51
        movzx  edx, word ptr [ebp + 0ch]
        mov    ah, 10h
        cmp    word ptr [ebp + 8], 608h
        jne    SHORT loc_4_0C29
        or     ah, 20h
loc_4_0C29:
        in     al, dx
        cmp    al, 0ffh
        je     SHORT loc_4_0C36
        and    al, 30h
        cmp    al, ah
        jne    SHORT loc_4_0C36
        jmp    SHORT loc_4_0C58
loc_4_0C36:
        movzx  eax, word ptr [ebp + 10h]
        test   eax, 1
        jne    SHORT loc_4_0C51
        call   sub_4_0D92
        jb     SHORT loc_4_0C51
        add    edx, 4
        cmp    dx, word ptr [ebp + 8]
        je     SHORT loc_4_0C58
loc_4_0C51:
        mov    eax, 0
        jmp    SHORT loc_4_0C5D
loc_4_0C58:
        mov    eax, 0ffffffffh
loc_4_0C5D:
        pop    edx
        pop    ebx
        pop    ebp
        ret
sub_4_0C61:
        push   ebp
        mov    ebp, esp
        pushad
        movzx  edx, word ptr [ebp + 8]
        add    edx, 3
        in     al, dx
        cmp    al, 0ffh
        je     SHORT loc_4_0CD7
        mov    ah, al
        movzx  ecx, al
        shr    ecx, 4
        or     ecx, ecx
        jne    SHORT loc_4_0C7F
        jmp    SHORT loc_4_0CD7
loc_4_0C7F:
        inc    ecx
        and    ecx, 3
        db  08Ah, 081h, 000h, 000h, 000h, 000h
        cmp    al, byte ptr [ebp + 0ch]
        jne    SHORT loc_4_0CD7
        movzx  ecx, ah
        shr    ecx, 6
        jecxz  loc_4_0CD7
        dec    ecx
        db  08Ah, 081h, 000h, 000h, 000h, 000h
        cmp    al, byte ptr [ebp + 10h]
        cmp    word ptr [ebp + 14h], -1
        je     SHORT loc_4_0CCC
        cmp    al, byte ptr [ebp + 14h]
        je     SHORT loc_4_0CCC
        sub    edx, 2
        in     al, dx
        cmp    al, 0ffh
        je     SHORT loc_4_0CD7
        movzx  ecx, al
        shr    ecx, 6
        or     ecx, ecx
        je     SHORT loc_4_0CD7
        jecxz  loc_4_0CD7
        dec    ecx
        db  08Ah, 081h, 000h, 000h, 000h, 000h
        cmp    al, byte ptr [ebp + 14h]
        jne    SHORT loc_4_0CD7
loc_4_0CCC:
        mov    dword ptr [esp + 1ch], 0ffffffffh
loc_4_0CD4:
        popad
        pop    ebp
        ret
loc_4_0CD7:
        mov    dword ptr [esp + 1ch], 0
        jmp    SHORT loc_4_0CD4
sub_4_0CE1:
        push   ebp
        mov    ebp, esp
        pushfd
        cli
        push   ecx
        push   edx
        movzx  edx, word ptr [ebp + 8]
        mov    ax, word ptr [ebp + 0ch]
        xor    ecx, ecx
loc_4_0CF2:
        db  08Ah, 0A1h, 000h, 000h, 000h, 000h
        cmp    ah, 0ffh
        je     SHORT loc_4_0D32
        db  038h, 081h, 000h, 000h, 000h, 000h
        je     SHORT loc_4_0D08
        inc    ecx
        jmp    SHORT loc_4_0CF2
loc_4_0D08:
        xchg   ah, al
        out    dx, al
        add    dx, 3
        in     al, dx
        test   al, 40h
        je     SHORT loc_4_0D32
        mov    edx, 1
        mov    cl, ah
        shl    edx, cl
        test   al, 80h
        je     SHORT loc_4_0D27
        and    edx, 2ffh
loc_4_0D27:
        or     edx, edx
        je     SHORT loc_4_0D32
        mov    eax, 0ffffffffh
        jmp    SHORT loc_4_0D37
loc_4_0D32:
        mov    eax, 0
loc_4_0D37:
        pop    edx
        pop    ecx
        popfd
        pop    ebp
        ret
sub_4_0D3C:
        push   ebp
        mov    ebp, esp
        push   ecx
        push   edx
        movzx  edx, word ptr [ebp + 8]
        mov    al, byte ptr [ebp + 10h]
        mov    ah, byte ptr [ebp + 0ch]
        xor    ecx, ecx
loc_4_0D4D:
        db  080h, 0B9h, 000h, 000h, 000h, 000h, 0FFh
        je     SHORT loc_4_0D89
        db  038h, 0A1h, 000h, 000h, 000h, 000h
        je     SHORT loc_4_0D61
        inc    ecx
        jmp    SHORT loc_4_0D4D
loc_4_0D61:
        push   eax
        add    dx, 3
        in     al, dx
        test   al, 80h
        pop    eax
        je     SHORT loc_4_0D70
        or     ah, ah
        je     SHORT loc_4_0D89
loc_4_0D70:
        cmp    al, ah
        je     SHORT loc_4_0D82
        or     ah, ah
        je     SHORT loc_4_0D7E
        or     al, al
        jne    SHORT loc_4_0D89
        jmp    SHORT loc_4_0D82
loc_4_0D7E:
        cmp    al, 1
        jne    SHORT loc_4_0D89
loc_4_0D82:
        mov    eax, 0ffffffffh
        jmp    SHORT loc_4_0D8E
loc_4_0D89:
        mov    eax, 0
loc_4_0D8E:
        pop    edx
        pop    ecx
        pop    ebp
        ret
sub_4_0D92:
        pushad
        call   sub_4_0BE4
        jb     SHORT loc_4_0DB6
        push   edx
        mov    edx, 0c7ch
        in     al, dx
        pop    edx
        mov    bl, al
        mov    bh, bl
        and    bh, 0f0h
        cmp    bh, 10h
        jne    SHORT loc_4_0DB6
        test   bl, 3
        jne    SHORT loc_4_0DB6
        popad
        clc
        ret
loc_4_0DB6:
        popad
        stc
        ret
sub_4_0DB9:
        push   eax
        push   ebx
        push   ecx
        pushfd
        cli
        mov    ebx, 4
        mov    al, 60h
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0DCA:
        mov    ebx, 4
        mov    al, 80h
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0DD6:
        mov    ebx, 2
        mov    al, 80h
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0DE2:
        mov    ebx, 4
        mov    al, 1
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0DEE:
        mov    cx, 4000h
loc_4_0DF2:
        in     al, dx
        test   al, 40h
        jne    SHORT loc_4_0DF9
        loop   loc_4_0DF2
loc_4_0DF9:
        push   eax
        mov    ebx, 4
        mov    al, 60h
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0E06:
        mov    ebx, 4
        mov    al, 80h
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0E12:
        pop    eax
        popfd
        test   al, 40h
        je     SHORT loc_4_0E1F
        test   al, 6
        jne    SHORT loc_4_0E1F
        clc
        jmp    SHORT loc_4_0E20
loc_4_0E1F:
        stc
loc_4_0E20:
        pop    ecx
        pop    ebx
        pop    eax
        ret
sub_4_0E24:
        push   ebx
        mov    bh, 9
        mov    ah, 0ch
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0E2E:
        jb     SHORT loc_4_0E50
        mov    bh, al
        cmp    al, 0ah
        je     SHORT loc_4_0E54
        cmp    al, 8ah
        jne    SHORT loc_4_0E50
        mov    al, 40h
        mov    ah, 0ch
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0E43:
        jmp    SHORT loc_4_0E45
loc_4_0E45:
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0E4A:
        test   al, 40h
        jne    SHORT loc_4_0E58
        jmp    SHORT loc_4_0E54
loc_4_0E50:
        mov    bl, 0
        jmp    SHORT loc_4_0E5A
loc_4_0E54:
        mov    bl, 1
        jmp    SHORT loc_4_0E5A
loc_4_0E58:
        mov    bl, 2
loc_4_0E5A:
        movzx  eax, bx
        pop    ebx
        ret
sub_4_0E5F:
        push   eax
        push   ecx
        push   edx
        mov    eax, 1
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0E6C:
        mov    ah, 6
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0E73:
        shl    eax, 8
        push   eax
        mov    ah, 7
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0E7E:
        movzx  eax, al
        or     dword ptr [esp], eax
        mov    eax, 80h
        mov    ah, 6
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0E90:
        inc    ah
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0E97:
        db  0C6h, 005h, 000h, 000h, 000h, 000h, 040h
        mov    eax, 4bh
        add    edx, 0
        out    dx, al
        sub    edx, 0
        mov    ah, 8
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0EB1:
        push   eax
        mov    al, 3
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0EB9:
        xor    eax, eax
        mov    ah, 0fh
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0EC2:
        dec    ah
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0EC9:
        mov    ah, 0ah
        db  0A0h, 000h, 000h, 000h, 000h
        or     al, 2
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0ED7:
        mov    ah, 9
        mov    al, 41h
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0EE0:
        mov    eax, 0bh
        add    edx, 0
        out    dx, al
        db  0C6h, 005h, 000h, 000h, 000h, 000h, 000h
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0EF5:
        add    edx, 3
        xor    eax, eax
        out    dx, al
        sub    edx, 3
        add    edx, 2
        mov    ecx, 1000h
loc_4_0F06:
        in     al, dx
        test   al, 1
        jne    SHORT loc_4_0F14
        loop   loc_4_0F06
        mov    ecx, 1f4h
loc_4_0F12:
        loop   loc_4_0F12
loc_4_0F14:
        sub    edx, 2
        db  080h, 00Dh, 000h, 000h, 000h, 000h, 040h
        mov    eax, 0bh
        db  00Ah, 005h, 000h, 000h, 000h, 000h
        add    edx, 0
        out    dx, al
        sub    edx, 0
        mov    ah, 9
        xor    al, al
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0F39:
        pop    eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0F3F:
        db  080h, 025h, 000h, 000h, 000h, 000h, 0BFh
        mov    eax, 0bh
        db  00Ah, 005h, 000h, 000h, 000h, 000h
        add    edx, 0
        out    dx, al
        sub    edx, 0
        mov    eax, dword ptr [esp]
        mov    ah, 7
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0F62:
        pop    eax
        shr    eax, 8
        mov    ah, 6
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0F6D:
        pop    edx
        pop    ecx
        pop    eax
        ret
sub_4_0F71:
        push   eax
        add    edx, 2
        in     al, dx
        sub    edx, 2
        test   al, 1
        je     SHORT loc_4_0F86
        xor    eax, eax
        add    edx, 2
        out    dx, al
        sub    edx, 2
loc_4_0F86:
        mov    ah, 0ah
        db  0A0h, 000h, 000h, 000h, 000h
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0F92:
        pop    eax
        ret
sub_4_0F94:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   ecx
        push   edx
        movzx  edx, word ptr [ebp + 8]
        movzx  eax, word ptr [ebp + 0ch]
        xor    ecx, ecx
        bts    ecx, eax
        pushfd
        cli
        in     al, 0a1h
        mov    ah, al
        in     al, 21h
        push   eax
        or     eax, ecx
        xchg   ah, al
        out    0a1h, al
        xchg   ah, al
        out    21h, al
        mov    al, 0ah
        out    0a0h, al
        jmp    SHORT loc_4_0FC0
loc_4_0FC0:
        out    20h, al
        jmp    SHORT loc_4_0FC4
loc_4_0FC4:
        call   sub_4_0F71
        in     al, 0a0h
        mov    ah, al
        in     al, 20h
        test   ecx, eax
        jne    SHORT loc_4_0FEC
        call   sub_4_0E5F
        push   ecx
        mov    ecx, 1f4h
loc_4_0FDE:
        in     al, dx
        loop   loc_4_0FDE
        pop    ecx
        in     al, 0a0h
        mov    ah, al
        in     al, 20h
        test   ecx, eax
        jne    SHORT loc_4_0FF8
loc_4_0FEC:
        push   ecx
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0FF3:
        add    esp, 8
        xor    eax, eax
loc_4_0FF8:
        call   sub_4_0F71
        mov    edx, eax
        pop    eax
        xchg   ah, al
        out    0a1h, al
        xchg   ah, al
        out    21h, al
        mov    eax, edx
        popfd
        pop    edx
        pop    ecx
        pop    ebx
        pop    ebp
        ret
sub_4_1010:
        push   ebp
        mov    ebp, esp
        movzx  edx, word ptr [ebp + 8]
        pushfd
        mov    eax, 0ffffffffh
        pushad
        cli
        movzx  ebx, word ptr [ebp + 0ch]
        and    ebx, 3
        mov    eax, ebx
        or     al, 4
        out    0ah, al
        xor    al, al
        out    0ch, al
        mov    eax, ebx
        test   dword ptr [ebp + 14h], 1
        jne    SHORT loc_4_103F
        add    al, 48h
        jmp    SHORT loc_4_1041
loc_4_103F:
        add    al, 44h
loc_4_1041:
        out    0bh, al
        mov    edx, ebx
        shl    edx, 1
        add    edx, 0
        mov    eax, dword ptr [ebp + 10h]
        out    dx, al
        shr    eax, 8
        out    dx, al
        movzx  ebx, word ptr [ebp + 0ch]
        db  00Fh, 0B6h, 093h, 000h, 000h, 000h, 000h
        shr    eax, 8
        out    dx, al
        and    ebx, 3
        mov    edx, ebx
        shl    edx, 1
        add    edx, 1
        mov    al, 0fh
        out    dx, al
        xor    al, al
        out    dx, al
        mov    eax, ebx
        out    0ah, al
        movzx  edx, word ptr [ebp + 8]
        mov    ah, 0fh
        mov    al, 0fh
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1082:
        mov    ah, 0eh
        xor    al, al
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_108B:
        mov    ah, 6
        mov    al, 3fh
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1094:
        mov    ah, 7
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_109B:
        mov    ah, 0ah
        db  0A0h, 000h, 000h, 000h, 000h
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_10A7:
        db  0C6h, 005h, 000h, 000h, 000h, 000h, 040h
        mov    eax, 4bh
        add    edx, 0
        out    dx, al
        sub    edx, 0
        mov    al, 1
        test   dword ptr [ebp + 14h], 1
        je     SHORT loc_4_10C7
        mov    al, 2
loc_4_10C7:
        test   dword ptr [ebp + 14h], 2
        je     SHORT loc_4_10DD
        or     al, 4
        test   dword ptr [ebp + 14h], 1
        je     SHORT loc_4_10DD
        or     al, 40h
loc_4_10DD:
        mov    ah, 9
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_10E4:
        mov    eax, 0bh
        add    edx, 0
        out    dx, al
        db  0C6h, 005h, 000h, 000h, 000h, 000h, 000h
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_10F9:
        mov    edx, ebx
        shl    edx, 1
        add    edx, 1
        mov    ecx, 0ffffh
loc_4_1105:
        xor    al, al
        out    0ch, al
        jmp    SHORT loc_4_110B
loc_4_110B:
        in     al, dx
        mov    ah, al
        jmp    SHORT loc_4_1110
loc_4_1110:
        in     al, dx
        cmp    ax, -1
        je     SHORT loc_4_112B
        loop   loc_4_1105
        push   eax
        push   ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1120:
        add    esp, 8
        mov    dword ptr [esp + 1ch], 0
loc_4_112B:
        movzx  edx, word ptr [ebp + 8]
        mov    ah, 9
        xor    al, al
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1138:
        mov    eax, ebx
        and    eax, 3
        or     eax, 4
        out    0ah, al
        add    edx, 2
        xor    al, al
        out    dx, al
        popad
        popfd
        pop    ebp
        ret
sub_4_114C:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        pushad
        movzx  edx, word ptr [ebp + 8]
        lea    ebx, [ebp - 4]
        push   0eh
        push   ebx
        push   0fffh
        push   0
        push   0
        push   0
        push   1
        push   1
        db  0CDh, 020h, 053h, 000h, 001h, 000h
        add    esp, 20h
        mov    dword ptr [ebp - 8], eax
        or     eax, eax
        je     SHORT loc_4_11C6
        mov    edi, edx
        mov    ecx, 400h
        mov    eax, 80808080h
        cld
        rep stosd
        push   0
        push   dword ptr [ebp - 4]
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp + 8]
        call   sub_4_1010
        add    esp, 10h
        or     eax, eax
        je     SHORT loc_4_11C6
        mov    ecx, 1
        mov    ax, word ptr [ebp + 0ch]
        cmp    ax, word ptr [ebp + 10h]
        jne    SHORT loc_4_11B4
        or     ecx, 2
loc_4_11B4:
        push   ecx
        push   dword ptr [ebp - 4]
        push   dword ptr [ebp + 10h]
        push   dword ptr [ebp + 8]
        call   sub_4_1010
        add    esp, 10h
loc_4_11C6:
        mov    dword ptr [esp + 1ch], eax
        mov    eax, dword ptr [ebp - 8]
        or     eax, eax
        je     SHORT loc_4_11DD
        push   0
        push   eax
        db  0CDh, 020h, 055h, 000h, 001h, 000h
        add    esp, 8
loc_4_11DD:
        popad
        add    esp, 8
        pop    ebp
        ret
        db  0CCh
loc_4_11E4:
        push   ebp
        mov    ebp, esp
        push   dword ptr [ebp + 18h]
        push   dword ptr [ebp + 14h]
        push   dword ptr [ebp + 10h]
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp + 8]
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_11FB:
        add    esp, 14h
        pop    ebp
        clc
        ret
sub_4_1201:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        mov    eax, esp
        push   0
        push   dword ptr [ebp + 8]
        push   eax
        db  0CDh, 020h, 003h, 000h, 033h, 000h
        add    esp, 0ch
        cmp    eax, 0
        jne    SHORT loc_4_1231
        mov    eax, dword ptr [esp]
        push   edi
        push   0
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1229:
        add    esp, 8
        mov    eax, edi
        pop    edi
        jmp    SHORT loc_4_1233
loc_4_1231:
        xor    eax, eax
loc_4_1233:
        add    esp, 4
        leave
        ret
sub_4_1238:
        push   ebp
        mov    ebp, esp
        push   edi
        push   esi
        push   dword ptr [ebp + 8]
        call   sub_4_1201
        add    esp, 4
        or     eax, eax
        je     SHORT loc_4_129E
        mov    edi, eax
        mov    esi, dword ptr [ebp + 10h]
        movsx  eax, word ptr [esi]
        cmp    eax, -1
        je     SHORT loc_4_127F
        db  0BEh, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 001h, 000h, 004h, 000h
        mov    dword ptr [edi + 0a8h], eax
        jb     SHORT loc_4_129E
        push   ebx
        push   ecx
        xor    ebx, ebx
        mov    ecx, 1
        db  0CDh, 020h, 01Fh, 000h, 004h, 000h
        pop    ecx
        pop    ebx
        jmp    SHORT loc_4_1286
loc_4_127F:
        movzx  eax, byte ptr [edi + 9]
        mov    word ptr [esi], ax
loc_4_1286:
        movzx  eax, byte ptr [edi + 8]
        mov    esi, dword ptr [ebp + 14h]
        mov    word ptr [esi], ax
        push   dword ptr [ebp + 0ch]
        push   edi
        call   sub_4_12A7
        add    esp, 8
        jmp    SHORT loc_4_12A3
loc_4_129E:
        mov    eax, 13h
loc_4_12A3:
        pop    esi
        pop    edi
        pop    ebp
        ret
sub_4_12A7:
        push   ebp
        mov    ebp, esp
        pushad
        mov    edi, dword ptr [ebp + 8]
        mov    edx, dword ptr [ebp + 0ch]
        mov    esi, edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_12B8:
        mov    word ptr [edi + 40h], dx
        push   edi
        db  0BEh, 000h, 000h, 000h, 000h
        movzx  ecx, word ptr [esi]
        mov    edi, esi
        add    esi, 2
loc_4_12CA:
        mov    word ptr [esi], dx
        inc    edx
        add    esi, 6
        loop   loc_4_12CA
        mov    edx, dword ptr [esp]
        call   sub_4_01B8
        pop    edi
        mov    dword ptr [edi + 64h], eax
        or     eax, eax
        je     SHORT loc_4_12ED
        mov    dword ptr [esp + 1ch], 0
        jmp    SHORT loc_4_12FB
loc_4_12ED:
        mov    word ptr [edi + 40h], 0
        mov    dword ptr [esp + 1ch], 13h
loc_4_12FB:
        popad
        leave
        ret
sub_4_12FE:
        push   ebp
        mov    ebp, esp
        push   dword ptr [ebp + 8]
        call   sub_4_1201
        add    esp, 4
        or     eax, eax
        je     SHORT loc_4_1332
        push   eax
        mov    eax, dword ptr [eax + 0a8h]
        or     eax, eax
        je     SHORT loc_4_1321
        db  0CDh, 020h, 01Ah, 000h, 004h, 000h
loc_4_1321:
        pop    eax
        push   eax
        call   sub_4_1339
        add    esp, 4
        mov    eax, 0
        jmp    SHORT loc_4_1337
loc_4_1332:
        mov    eax, 13h
loc_4_1337:
        pop    ebp
        ret
sub_4_1339:
        push   ebp
        mov    ebp, esp
        pushad
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 64h]
        or     eax, eax
        je     SHORT loc_4_1375
        db  0BEh, 000h, 000h, 000h, 000h
        movzx  ecx, word ptr [esi]
        movzx  edx, word ptr [edi + 40h]
        push   edi
        mov    edi, esi
        add    esi, 2
loc_4_1359:
        mov    word ptr [esi], dx
        inc    edx
        add    esi, 6
        loop   loc_4_1359
        mov    eax, dword ptr [esp]
        mov    eax, dword ptr [eax + 64h]
        call   sub_4_0283
        pop    edi
        mov    dword ptr [edi + 64h], 0
loc_4_1375:
        popad
        leave
        ret
sub_4_1378:
        push   ebp
        mov    ebp, esp
        push   dword ptr [ebp + 8]
        call   sub_4_1201
        add    esp, 4
        or     eax, eax
        je     SHORT loc_4_139E
        test   word ptr [eax + 4], 4000h
        je     SHORT loc_4_1399
        mov    eax, 0ffffffffh
        jmp    SHORT loc_4_139E
loc_4_1399:
        mov    eax, 0
loc_4_139E:
        pop    ebp
        ret
_PTEXT ends
        end
