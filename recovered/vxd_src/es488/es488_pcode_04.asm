; es488_pcode_04.asm -- ES488 object 4 segment _PTEXT
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
        cmp    dword ptr [edi + 2ch], 0
        jne    SHORT loc_4_0028
        cmp    dword ptr [edi + 34h], 0
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
        cmp    eax, 40h
        jle    SHORT loc_4_003A
        mov    eax, 40h
loc_4_003A:
        add    eax, 3
        shr    eax, 2
loc_4_0040:
        mov    ebx, eax
        dec    eax
        bsr    cx, ax
        inc    cl
        mov    eax, 1
        shl    eax, cl
        dec    eax
        mov    ecx, ebx
        push   ecx
        sub    esp, 4
        mov    ebx, esp
        push   0eh
        push   ebx
        push   0fffh
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
        jne    SHORT loc_4_0082
        mov    eax, ecx
        shr    eax, 1
        jne    SHORT loc_4_0040
        xor    ecx, ecx
        stc
        ret
loc_4_0082:
        shl    ecx, 0ch
        clc
        ret
sub_4_0087:
        push   eax
        push   ecx
        push   esi
        push   edi
        db  08Bh, 03Dh, 000h, 000h, 000h, 000h
        mov    esi, dword ptr [edi + ebx]
        or     esi, esi
        je     SHORT loc_4_00C2
        db  0CDh, 020h, 09Dh, 000h, 001h, 000h
        jb     SHORT loc_4_00C2
        push   eax
        mov    edi, eax
        xor    eax, eax
        mov    ecx, 24h
        shr    ecx, 2
        rep stosd
        pop    eax
        mov    edi, dword ptr [esp]
        mov    dword ptr [eax + 4], edi
        db  0CDh, 020h, 09Fh, 000h, 001h, 000h
        clc
loc_4_00BD:
        pop    edi
        pop    esi
        pop    ecx
        pop    eax
        ret
loc_4_00C2:
        stc
        jmp    SHORT loc_4_00BD
sub_4_00C5:
        push   eax
        push   ecx
        push   esi
        push   edi
        pushfd
        db  08Bh, 035h, 000h, 000h, 000h, 000h
        mov    esi, dword ptr [esi + ebx]
        or     esi, esi
        je     SHORT loc_4_00FD
        cli
        db  0CDh, 020h, 0A3h, 000h, 001h, 000h
        je     SHORT loc_4_00FD
loc_4_00E0:
        cmp    dword ptr [eax + 4], edi
        je     SHORT loc_4_00EF
        db  0CDh, 020h, 0A4h, 000h, 001h, 000h
        je     SHORT loc_4_00FD
        jmp    SHORT loc_4_00E0
loc_4_00EF:
        push   eax
        db  0CDh, 020h, 0A1h, 000h, 001h, 000h
        pop    eax
        db  0CDh, 020h, 0A2h, 000h, 001h, 000h
loc_4_00FD:
        popfd
        pop    edi
        pop    esi
        pop    ecx
        pop    eax
        ret
sub_4_0103:
        jmp    SHORT loc_4_010F
loc_4_0105:
        mov    esi, 0
        jmp    NEAR PTR loc_4_01C7
loc_4_010F:
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
        je     NEAR PTR loc_4_01C1
        mov    dword ptr [ebp - 8], eax
        push   edi
        movzx  ecx, word ptr [edi]
        push   ecx
        mov    edi, dword ptr [ebp - 8]
loc_4_014A:
        push   ecx
        mov    ecx, 0ah
        db  0BEh, 000h, 000h, 000h, 000h
        cld
        rep movsb
        pop    ecx
        loop   loc_4_014A
        pop    ecx
        mov    edi, dword ptr [esp]
        mov    esi, dword ptr [ebp - 8]
        add    edi, 2
loc_4_0165:
        mov    eax, dword ptr [ebp - 0ch]
        mov    dword ptr [esi + 1], eax
        mov    eax, dword ptr [edi + 2]
        sub    eax, esi
        sub    eax, 0ah
        mov    dword ptr [esi + 6], eax
        movzx  edx, word ptr [edi]
        db  0CDh, 020h, 096h, 000h, 001h, 000h
        jb     SHORT loc_4_0198
        add    edi, 6
        add    esi, 0ah
        loop   loc_4_0165
        pop    edi
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [esp + 1ch], eax
        jmp    SHORT loc_4_01C7
loc_4_0198:
        pop    edi
        movzx  eax, word ptr [edi]
        sub    ecx, eax
        neg    ecx
        jecxz  loc_4_01B3
        add    edi, 2
loc_4_01A5:
        movzx  edx, word ptr [edi]
        db  0CDh, 020h, 016h, 001h, 001h, 000h
        add    edi, 6
        loop   loc_4_01A5
loc_4_01B3:
        push   0
        push   dword ptr [ebp - 8]
        db  0CDh, 020h, 051h, 000h, 001h, 000h
        add    esp, 8
loc_4_01C1:
        xor    eax, eax
        mov    dword ptr [esp + 1ch], eax
loc_4_01C7:
        popad
        add    esp, 0ch
        pop    ebp
        ret
sub_4_01CD:
        push   ecx
        push   edx
        push   edi
        push   eax
        movzx  ecx, word ptr [edi]
        add    edi, 2
loc_4_01D7:
        movzx  edx, word ptr [edi]
        db  0CDh, 020h, 016h, 001h, 001h, 000h
        add    edi, 6
        loop   loc_4_01D7
        pop    eax
        push   0
        push   eax
        db  0CDh, 020h, 051h, 000h, 001h, 000h
        add    esp, 8
        pop    edi
        pop    edx
        pop    ecx
        ret
sub_4_01F6:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        pushad
        mov    dword ptr [ebp - 4], 0
        db  08Bh, 035h, 000h, 000h, 000h, 000h
        or     esi, esi
        je     NEAR PTR loc_4_0426
        movzx  edx, word ptr [ebp + 0ch]
        push   2
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_021E:
        add    esp, 8
        or     edi, edi
        jne    NEAR PTR loc_4_0426
        db  0CDh, 020h, 09Dh, 000h, 001h, 000h
        jb     NEAR PTR loc_4_0426
        mov    dword ptr [ebp - 4], eax
        db  0CDh, 020h, 09Fh, 000h, 001h, 000h
        mov    dword ptr [eax], 0
        push   1
        push   66h
        db  0CDh, 020h, 04Fh, 000h, 001h, 000h
        add    esp, 8
        or     eax, eax
        je     NEAR PTR loc_4_0426
        mov    edi, eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax], edi
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [edi + 44h], eax
        mov    word ptr [edi + 64h], 0
        mov    ax, word ptr [ebp + 28h]
        mov    word ptr [edi + 4], ax
        mov    ax, word ptr [ebp + 0ch]
        mov    word ptr [edi + 6], ax
        mov    ax, word ptr [ebp + 10h]
        mov    word ptr [edi + 10h], ax
        mov    byte ptr [edi + 0ch], 4
        mov    ax, word ptr [ebp + 18h]
        mov    byte ptr [edi + 0dh], al
        mov    ax, word ptr [ebp + 14h]
        mov    word ptr [edi + 8], ax
        mov    ax, word ptr [ebp + 1ch]
        mov    byte ptr [edi + 0ah], al
        mov    ax, word ptr [ebp + 20h]
        mov    byte ptr [edi + 0bh], al
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_02AA:
        mov    word ptr [edi + 12h], ax
        push   edi
        movzx  ecx, byte ptr [edi + 0ah]
        db  066h, 089h, 00Dh, 000h, 000h, 000h, 000h
        db  089h, 03Dh, 01Ch, 000h, 000h, 000h
        db  0BFh, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 001h, 000h, 003h, 000h
        pop    edi
        jae    SHORT loc_4_02E0
        db  068h, 000h, 000h, 000h, 000h
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_02D8:
        add    esp, 4
        jmp    NEAR PTR loc_4_0426
loc_4_02E0:
        mov    dword ptr [edi + 28h], eax
        db  0CDh, 020h, 003h, 000h, 001h, 000h
        db  0CDh, 020h, 005h, 000h, 003h, 000h
        test   ecx, 4
        jne    SHORT loc_4_02FD
        or     word ptr [edi + 4], 8000h
loc_4_02FD:
        db  0BEh, 000h, 000h, 000h, 000h
        cmp    word ptr [ebp + 14h], -1
        jne    SHORT loc_4_030E
        db  0BEh, 000h, 000h, 000h, 000h
loc_4_030E:
        movzx  ecx, word ptr [esi]
        movzx  edx, word ptr [ebp + 0ch]
        push   edi
        mov    edi, esi
        add    esi, 2
loc_4_031B:
        mov    word ptr [esi], dx
        inc    edx
        add    esi, 6
        loop   loc_4_031B
        mov    edx, dword ptr [esp]
        call   sub_4_0103
        pop    edi
        mov    dword ptr [edi + 48h], eax
        or     eax, eax
        jne    SHORT loc_4_0346
        db  068h, 000h, 000h, 000h, 000h
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_033E:
        add    esp, 4
        jmp    NEAR PTR loc_4_0426
loc_4_0346:
        movsx  edx, word ptr [ebp + 14h]
        cmp    edx, -1
        je     SHORT loc_4_0390
        mov    word ptr [edi + 8], dx
        mov    dx, 388h
        db  0BEh, 000h, 000h, 000h, 000h
        movzx  ecx, word ptr [esi]
        push   edi
        mov    edi, esi
        add    esi, 2
loc_4_0365:
        mov    word ptr [esi], dx
        inc    edx
        add    esi, 6
        loop   loc_4_0365
        mov    edx, dword ptr [esp]
        call   sub_4_0103
        pop    edi
        mov    dword ptr [edi + 4ch], eax
        or     eax, eax
        jne    SHORT loc_4_0390
        db  068h, 000h, 000h, 000h, 000h
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0388:
        add    esp, 4
        jmp    NEAR PTR loc_4_0426
loc_4_0390:
        mov    eax, dword ptr [ebp + 24h]
        call   sub_4_0030
        mov    dword ptr [edi + 14h], eax
        mov    dword ptr [edi + 18h], ebx
        mov    dword ptr [edi + 20h], ecx
        mov    dword ptr [edi + 1ch], edx
        jae    SHORT loc_4_03B5
        db  068h, 000h, 000h, 000h, 000h
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_03B0:
        add    esp, 4
        jmp    SHORT loc_4_0426
loc_4_03B5:
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
        mov    word ptr [edi + 24h], ax
        movzx  eax, word ptr [ebp + 20h]
        db  0BEh, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 001h, 000h, 004h, 000h
        mov    dword ptr [edi + 3ch], eax
        jae    SHORT loc_4_03FD
        db  068h, 000h, 000h, 000h, 000h
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_03F8:
        add    esp, 4
        jmp    SHORT loc_4_0426
loc_4_03FD:
        db  0CDh, 020h, 003h, 000h, 001h, 000h
        push   ebx
loc_4_0404:
        call   sub_4_0087
        jb     SHORT loc_4_0425
        db  0CDh, 020h, 03Bh, 000h, 001h, 000h
        cmp    ebx, dword ptr [esp]
        jne    SHORT loc_4_0404
        pop    ebx
        mov    dword ptr [esp + 1ch], 0ffffffffh
loc_4_041F:
        popad
        add    esp, 4
        pop    ebp
        ret
loc_4_0425:
        pop    ebx
loc_4_0426:
        mov    edi, dword ptr [ebp - 4]
        or     edi, edi
        je     SHORT loc_4_0432
        call   sub_4_043C
loc_4_0432:
        mov    dword ptr [esp + 1ch], 0
        jmp    SHORT loc_4_041F
sub_4_043C:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        pushad
        mov    dword ptr [ebp - 4], edi
        mov    edi, dword ptr [edi]
        or     edi, edi
        je     NEAR PTR loc_4_0511
        xor    esi, esi
        xchg   dword ptr [edi + 40h], esi
        or     esi, esi
        je     SHORT loc_4_045F
        db  0CDh, 020h, 09Ch, 000h, 001h, 000h
loc_4_045F:
        mov    eax, dword ptr [edi + 3ch]
        or     eax, eax
        je     SHORT loc_4_046C
        db  0CDh, 020h, 01Ah, 000h, 004h, 000h
loc_4_046C:
        movzx  eax, word ptr [edi + 24h]
        or     eax, eax
        je     SHORT loc_4_048F
        push   0
        push   eax
        db  0CDh, 020h, 077h, 000h, 001h, 000h
        add    esp, 8
        mov    eax, dword ptr [edi + 14h]
        push   0
        push   eax
        db  0CDh, 020h, 055h, 000h, 001h, 000h
        add    esp, 8
loc_4_048F:
        mov    eax, dword ptr [edi + 4ch]
        or     eax, eax
        je     SHORT loc_4_04BE
        db  0BEh, 000h, 000h, 000h, 000h
        movzx  ecx, word ptr [esi]
        mov    edx, 388h
        push   edi
        mov    edi, esi
        add    esi, 2
loc_4_04A9:
        mov    word ptr [esi], dx
        inc    edx
        add    esi, 6
        loop   loc_4_04A9
        mov    eax, dword ptr [esp]
        mov    eax, dword ptr [eax + 4ch]
        call   sub_4_01CD
        pop    edi
loc_4_04BE:
        mov    eax, dword ptr [edi + 48h]
        or     eax, eax
        je     SHORT loc_4_04F8
        db  0BEh, 000h, 000h, 000h, 000h
        cmp    word ptr [edi + 8], -1
        jne    SHORT loc_4_04D6
        db  0BEh, 000h, 000h, 000h, 000h
loc_4_04D6:
        movzx  ecx, word ptr [esi]
        movzx  edx, word ptr [edi + 6]
        push   edi
        mov    edi, esi
        add    esi, 2
loc_4_04E3:
        mov    word ptr [esi], dx
        inc    edx
        add    esi, 6
        loop   loc_4_04E3
        mov    eax, dword ptr [esp]
        mov    eax, dword ptr [eax + 48h]
        call   sub_4_01CD
        pop    edi
loc_4_04F8:
        mov    eax, dword ptr [edi + 28h]
        or     eax, eax
        je     SHORT loc_4_0505
        db  0CDh, 020h, 011h, 000h, 003h, 000h
loc_4_0505:
        push   0
        push   edi
        db  0CDh, 020h, 051h, 000h, 001h, 000h
        add    esp, 8
loc_4_0511:
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
sub_4_052E:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        pushad
        mov    dword ptr [esp + 1ch], 13h
        movzx  edx, word ptr [ebp + 0ch]
        call   sub_4_0674
        jb     SHORT loc_4_059E
        mov    word ptr [ebp - 4], ax
        mov    word ptr [ebp - 8], dx
        movsx  edx, word ptr [ebp + 10h]
        cmp    edx, -1
        je     SHORT loc_4_0560
        call   sub_4_06F7
        jb     SHORT loc_4_0598
loc_4_0560:
        test   dword ptr [ebp + 28h], 50h
        jne    SHORT loc_4_0569
loc_4_0569:
        push   dword ptr [ebp + 28h]
        push   dword ptr [ebp + 24h]
        push   dword ptr [ebp + 18h]
        push   dword ptr [ebp + 14h]
        push   dword ptr [ebp - 8]
        push   dword ptr [ebp + 10h]
        push   dword ptr [ebp - 4]
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp + 8]
        call   sub_4_01F6
        add    esp, 24h
        or     eax, eax
        je     SHORT loc_4_0598
        mov    dword ptr [esp + 1ch], 0
loc_4_0598:
        popad
        add    esp, 8
        pop    ebp
        ret
loc_4_059E:
        mov    dword ptr [esp + 1ch], 24h
        jmp    SHORT loc_4_0598
sub_4_05A8:
        push   ebp
        mov    ebp, esp
        push   edi
        push   ebx
        pushfd
        push   dword ptr [ebp + 8]
        call   0
        add    esp, 4
        or     eax, eax
        je     SHORT loc_4_05F0
        push   0
        push   dword ptr [ebp + 8]
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_05C7:
        add    esp, 8
        or     edi, edi
        je     SHORT loc_4_05F0
        mov    ebx, dword ptr [edi + 2ch]
        or     ebx, ebx
        je     SHORT loc_4_05DF
        mov    eax, 1
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_05DF:
        mov    ebx, dword ptr [edi + 34h]
        or     ebx, ebx
        je     SHORT loc_4_05F0
        mov    eax, 2
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_05F0:
        mov    ebx, dword ptr [ebp + 8]
        db  08Bh, 035h, 000h, 000h, 000h, 000h
        or     esi, esi
        je     SHORT loc_4_063D
        cli
        db  0CDh, 020h, 0A3h, 000h, 001h, 000h
        je     SHORT loc_4_063D
loc_4_0606:
        mov    edi, dword ptr [eax]
        cmp    dword ptr [edi + 44h], ebx
        je     SHORT loc_4_0617
        db  0CDh, 020h, 0A4h, 000h, 001h, 000h
        je     SHORT loc_4_063D
        jmp    SHORT loc_4_0606
loc_4_0617:
        popfd
        pushfd
        db  0CDh, 020h, 003h, 000h, 001h, 000h
        push   ebx
loc_4_0620:
        call   sub_4_00C5
        db  0CDh, 020h, 03Bh, 000h, 001h, 000h
        cmp    ebx, dword ptr [esp]
        jne    SHORT loc_4_0620
        pop    ebx
        mov    edi, eax
        call   sub_4_043C
        mov    eax, 0ffffffffh
loc_4_063D:
        popfd
        pop    ebx
        pop    edi
        pop    ebp
        ret
loc_4_0642:
        push   ebp
        mov    ebp, esp
        push   dword ptr [ebp + 18h]
        push   dword ptr [ebp + 14h]
        push   dword ptr [ebp + 10h]
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp + 8]
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0659:
        add    esp, 14h
        leave
        clc
        ret
loc_4_065F:
        mov    eax, ebx
        db  0BBh, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 000h, 000h, 04Ah, 004h
        mov    eax, 0
        stc
        ret
        db  0CCh
sub_4_0674:
        push   eax
        push   edx
        add    edx, 6
        mov    al, 1
        out    dx, al
        in     al, dx
        in     al, dx
        in     al, dx
        xor    al, al
        out    dx, al
        sub    edx, 6
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_068A:
        jb     SHORT loc_4_06E4
        cmp    al, 0aah
        jne    SHORT loc_4_06E4
        mov    al, 0e1h
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0697:
        jb     SHORT loc_4_06E4
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_069E:
        jb     SHORT loc_4_06E4
        mov    ah, al
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_06A7:
        jb     SHORT loc_4_06E4
        movzx  eax, ax
        mov    dword ptr [esp + 4], eax
        mov    al, 0e7h
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_06B7:
        jb     SHORT loc_4_06E4
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_06BE:
        jae    SHORT loc_4_06CE
        mov    al, 0e0h
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_06C7:
        jb     SHORT loc_4_06E4
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_06CE:
        jb     SHORT loc_4_06E4
        mov    ah, al
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_06D7:
        jb     SHORT loc_4_06E4
        cmp    ah, 48h
        jne    SHORT loc_4_06E4
        and    al, 0f0h
        cmp    al, 80h
        je     SHORT loc_4_06ED
loc_4_06E4:
        xor    eax, eax
        mov    dword ptr [esp + 4], eax
        stc
        jmp    SHORT loc_4_06F4
loc_4_06ED:
        movzx  eax, ah
        mov    dword ptr [esp], eax
        clc
loc_4_06F4:
        pop    edx
        pop    eax
        ret
sub_4_06F7:
        push   eax
        push   ebx
        push   ecx
        pushfd
        cli
        mov    ebx, 4
        mov    al, 60h
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0708:
        mov    ebx, 4
        mov    al, 80h
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0714:
        mov    ebx, 2
        mov    al, 80h
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0720:
        mov    ebx, 4
        mov    al, 1
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_072C:
        mov    cx, 4000h
loc_4_0730:
        in     al, dx
        test   al, 40h
        jne    SHORT loc_4_0737
        loop   loc_4_0730
loc_4_0737:
        push   eax
        mov    ebx, 4
        mov    al, 60h
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0744:
        mov    ebx, 4
        mov    al, 80h
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0750:
        pop    eax
        popfd
        test   al, 40h
        je     SHORT loc_4_075D
        test   al, 6
        je     SHORT loc_4_075D
        clc
        jmp    SHORT loc_4_075E
loc_4_075D:
        stc
loc_4_075E:
        pop    ecx
        pop    ebx
        pop    eax
        ret
_PTEXT ends
        end
