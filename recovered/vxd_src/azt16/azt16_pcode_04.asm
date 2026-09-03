; azt16_pcode_04.asm -- AZT16 object 4 segment _PTEXT
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
        mov    esi, dword ptr [ebp + 0ch]
        mov    dword ptr [esi], 0
        push   0
        push   dword ptr [ebp + 8]
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0048:
        add    esp, 8
        or     edi, edi
        je     SHORT loc_4_005C
        movzx  eax, word ptr [edi + 4]
        mov    dword ptr [esi], eax
        mov    eax, 0ffffffffh
        jmp    SHORT loc_4_0061
loc_4_005C:
        mov    eax, 0
loc_4_0061:
        pop    edi
        pop    esi
        pop    ebp
        ret
sub_4_0065:
        cmp    eax, 40h
        jle    SHORT loc_4_006F
        mov    eax, 40h
loc_4_006F:
        add    eax, 3
        shr    eax, 2
loc_4_0075:
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
        jae    SHORT loc_4_009A
        mov    edx, eax
loc_4_009A:
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
        jne    SHORT loc_4_00C6
        mov    eax, ecx
        shr    eax, 1
        jne    SHORT loc_4_0075
        xor    ecx, ecx
        stc
        ret
loc_4_00C6:
        shl    ecx, 0ch
        clc
        ret
sub_4_00CB:
        push   eax
        push   ecx
        push   esi
        push   edi
        db  08Bh, 03Dh, 000h, 000h, 000h, 000h
        mov    esi, dword ptr [edi + ebx]
        or     esi, esi
        je     SHORT loc_4_0136
        cli
        db  0CDh, 020h, 09Dh, 000h, 001h, 000h
        sti
        jb     SHORT loc_4_0136
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
        mov    byte ptr [eax + 24h], 4bh
        test   word ptr [edi + 4], 200h
        je     SHORT loc_4_0128
        mov    byte ptr [eax + 0ch], 0
        mov    byte ptr [eax + 0dh], 0
loc_4_0128:
        cli
        db  0CDh, 020h, 09Fh, 000h, 001h, 000h
        sti
        clc
loc_4_0131:
        pop    edi
        pop    esi
        pop    ecx
        pop    eax
        ret
loc_4_0136:
        stc
        jmp    SHORT loc_4_0131
sub_4_0139:
        push   eax
        push   ecx
        push   esi
        push   edi
        pushfd
        db  08Bh, 035h, 000h, 000h, 000h, 000h
        mov    esi, dword ptr [esi + ebx]
        or     esi, esi
        je     SHORT loc_4_0171
        cli
        db  0CDh, 020h, 0A3h, 000h, 001h, 000h
        je     SHORT loc_4_0171
loc_4_0154:
        cmp    dword ptr [eax + 4], edi
        je     SHORT loc_4_0163
        db  0CDh, 020h, 0A4h, 000h, 001h, 000h
        je     SHORT loc_4_0171
        jmp    SHORT loc_4_0154
loc_4_0163:
        push   eax
        db  0CDh, 020h, 0A1h, 000h, 001h, 000h
        pop    eax
        db  0CDh, 020h, 0A2h, 000h, 001h, 000h
loc_4_0171:
        popfd
        pop    edi
        pop    esi
        pop    ecx
        pop    eax
        ret
sub_4_0177:
        jmp    SHORT loc_4_0183
loc_4_0179:
        mov    esi, 0
        jmp    NEAR PTR loc_4_023C
loc_4_0183:
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
        je     NEAR PTR loc_4_0236
        mov    dword ptr [ebp - 8], eax
        push   edi
        movzx  ecx, word ptr [edi]
        push   ecx
        mov    edi, dword ptr [ebp - 8]
loc_4_01BE:
        push   ecx
        mov    ecx, 0ah
        db  08Dh, 035h, 000h, 000h, 000h, 000h
        cld
        rep movsb
        pop    ecx
        loop   loc_4_01BE
        pop    ecx
        mov    edi, dword ptr [esp]
        mov    esi, dword ptr [ebp - 8]
        add    edi, 2
loc_4_01DA:
        mov    eax, dword ptr [ebp - 0ch]
        mov    dword ptr [esi + 1], eax
        mov    eax, dword ptr [edi + 2]
        sub    eax, esi
        sub    eax, 0ah
        mov    dword ptr [esi + 6], eax
        movzx  edx, word ptr [edi]
        db  0CDh, 020h, 096h, 000h, 001h, 000h
        jb     SHORT loc_4_020D
        add    edi, 6
        add    esi, 0ah
        loop   loc_4_01DA
        pop    edi
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [esp + 1ch], eax
        jmp    SHORT loc_4_023C
loc_4_020D:
        pop    edi
        movzx  eax, word ptr [edi]
        sub    ecx, eax
        neg    ecx
        jecxz  loc_4_0228
        add    edi, 2
loc_4_021A:
        movzx  edx, word ptr [edi]
        db  0CDh, 020h, 016h, 001h, 001h, 000h
        add    edi, 6
        loop   loc_4_021A
loc_4_0228:
        push   0
        push   dword ptr [ebp - 8]
        db  0CDh, 020h, 051h, 000h, 001h, 000h
        add    esp, 8
loc_4_0236:
        xor    eax, eax
        mov    dword ptr [esp + 1ch], eax
loc_4_023C:
        popad
        add    esp, 0ch
        pop    ebp
        ret
sub_4_0242:
        push   ecx
        push   edx
        push   edi
        push   eax
        movzx  ecx, word ptr [edi]
        add    edi, 2
loc_4_024C:
        movzx  edx, word ptr [edi]
        db  0CDh, 020h, 016h, 001h, 001h, 000h
        add    edi, 6
        loop   loc_4_024C
        pop    eax
        push   0
        push   eax
        db  0CDh, 020h, 051h, 000h, 001h, 000h
        add    esp, 8
        pop    edi
        pop    edx
        pop    ecx
        ret
sub_4_026B:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        pushad
        mov    dword ptr [ebp - 4], 0
        db  08Bh, 035h, 000h, 000h, 000h, 000h
        or     esi, esi
        je     NEAR PTR loc_4_05DA
        db  0CDh, 020h, 09Dh, 000h, 001h, 000h
        jb     NEAR PTR loc_4_05DA
        mov    dword ptr [ebp - 4], eax
        db  0CDh, 020h, 09Fh, 000h, 001h, 000h
        mov    dword ptr [eax], 0
        push   1
        push   0beh
        db  0CDh, 020h, 04Fh, 000h, 001h, 000h
        add    esp, 8
        or     eax, eax
        je     NEAR PTR loc_4_05DA
        mov    edi, eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax], edi
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [edi + 54h], eax
        mov    ax, word ptr [ebp + 30h]
        mov    word ptr [edi + 4], ax
        mov    word ptr [edi + 0bch], 0
        mov    ax, word ptr [ebp + 34h]
        mov    word ptr [edi + 52h], ax
        mov    ax, word ptr [ebp + 0ch]
        mov    word ptr [edi + 6], ax
        sub    word ptr [edi + 6], 4
        mov    word ptr [edi + 42h], ax
        mov    ax, word ptr [ebp + 20h]
        mov    byte ptr [edi + 8], al
        test   dword ptr [ebp + 30h], 200h
        jne    SHORT loc_4_0315
        mov    ax, word ptr [ebp + 24h]
        mov    byte ptr [edi + 9], al
        mov    ax, word ptr [ebp + 28h]
        mov    byte ptr [edi + 0ah], al
        cmp    ax, word ptr [ebp + 24h]
        je     SHORT loc_4_031A
loc_4_0315:
        or     word ptr [edi + 52h], 40h
loc_4_031A:
        mov    ax, word ptr [ebp + 14h]
        mov    word ptr [edi + 4eh], ax
        cmp    word ptr [ebp + 10h], -1
        je     SHORT loc_4_034F
        push   dword ptr [ebp + 10h]
        call   sub_4_0ABF
        add    esp, 4
        or     eax, eax
        je     NEAR PTR loc_4_05DA
        or     word ptr [edi + 52h], 4
        mov    ah, al
        and    ah, 38h
        shr    ah, 3
        and    al, 7
        mov    word ptr [edi + 0eh], ax
loc_4_034F:
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0354:
        mov    word ptr [edi + 0ch], ax
        db  066h, 0C7h, 005h, 002h, 000h, 000h, 000h, 004h, 000h
        db  0C7h, 005h, 004h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        push   edi
        movzx  ecx, byte ptr [edi + 8]
        db  066h, 089h, 00Dh, 000h, 000h, 000h, 000h
        db  089h, 03Dh, 01Ch, 000h, 000h, 000h
        db  0BFh, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 001h, 000h, 003h, 000h
        pop    edi
        jae    SHORT loc_4_039D
        db  068h, 000h, 000h, 000h, 000h
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0395:
        add    esp, 4
        jmp    NEAR PTR loc_4_05DA
loc_4_039D:
        mov    dword ptr [edi + 2ch], eax
        db  0CDh, 020h, 003h, 000h, 001h, 000h
        db  0CDh, 020h, 005h, 000h, 003h, 000h
        test   ecx, 4
        jne    SHORT loc_4_03BA
        or     word ptr [edi + 4], 8000h
loc_4_03BA:
        test   word ptr [edi + 4], 8
        je     SHORT loc_4_03CA
        db  0CDh, 020h, 009h, 000h, 003h, 000h
        jmp    SHORT loc_4_03D6
loc_4_03CA:
        db  0CDh, 020h, 008h, 000h, 003h, 000h
        db  0CDh, 020h, 004h, 000h, 003h, 000h
loc_4_03D6:
        test   word ptr [edi + 52h], 4
        je     SHORT loc_4_041F
        db  00Fh, 0B7h, 00Dh, 000h, 000h, 000h, 000h
        db  0BEh, 000h, 000h, 000h, 000h
        movzx  edx, word ptr [ebp + 10h]
        push   edi
        mov    edi, esi
        add    esi, 2
loc_4_03F4:
        mov    word ptr [esi], dx
        inc    edx
        add    esi, 6
        loop   loc_4_03F4
        mov    edx, dword ptr [esp]
        call   sub_4_0177
        pop    edi
        mov    dword ptr [edi + 58h], eax
        or     eax, eax
        jne    SHORT loc_4_041F
        db  068h, 000h, 000h, 000h, 000h
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0417:
        add    esp, 4
        jmp    NEAR PTR loc_4_05DA
loc_4_041F:
        db  0BEh, 000h, 000h, 000h, 000h
        movzx  ecx, word ptr [esi]
        movzx  edx, word ptr [ebp + 0ch]
        call   sub_4_0D1F
        mov    byte ptr [edi + 0bh], ah
        xor    ah, ah
        mov    word ptr [edi + 4ch], ax
        push   edi
        mov    edi, esi
        add    esi, 2
loc_4_043F:
        mov    word ptr [esi], dx
        inc    edx
        add    esi, 6
        loop   loc_4_043F
        mov    edx, dword ptr [esp]
        call   sub_4_0177
        pop    edi
        mov    dword ptr [edi + 5ch], eax
        or     eax, eax
        jne    SHORT loc_4_046A
        db  068h, 000h, 000h, 000h, 000h
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0462:
        add    esp, 4
        jmp    NEAR PTR loc_4_05DA
loc_4_046A:
        movsx  edx, word ptr [ebp + 18h]
        cmp    edx, -1
        je     SHORT loc_4_04B5
        mov    word ptr [edi + 22h], dx
        or     word ptr [edi + 52h], 8
        db  0BEh, 000h, 000h, 000h, 000h
        movzx  ecx, word ptr [esi]
        push   edi
        mov    edi, esi
        add    esi, 2
loc_4_048A:
        mov    word ptr [esi], dx
        inc    edx
        add    esi, 6
        loop   loc_4_048A
        mov    edx, dword ptr [esp]
        call   sub_4_0177
        pop    edi
        mov    dword ptr [edi + 60h], eax
        or     eax, eax
        jne    SHORT loc_4_04B5
        db  068h, 000h, 000h, 000h, 000h
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_04AD:
        add    esp, 4
        jmp    NEAR PTR loc_4_05DA
loc_4_04B5:
        movsx  edx, word ptr [ebp + 1ch]
        cmp    edx, -1
        je     SHORT loc_4_0501
        mov    word ptr [edi + 40h], dx
        or     word ptr [edi + 4], 1000h
        db  0BEh, 000h, 000h, 000h, 000h
        movzx  ecx, word ptr [esi]
        push   edi
        mov    edi, esi
        add    esi, 2
loc_4_04D6:
        mov    word ptr [esi], dx
        inc    edx
        add    esi, 6
        loop   loc_4_04D6
        mov    edx, dword ptr [esp]
        call   sub_4_0177
        pop    edi
        mov    dword ptr [edi + 64h], eax
        or     eax, eax
        jne    SHORT loc_4_0501
        db  068h, 000h, 000h, 000h, 000h
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_04F9:
        add    esp, 4
        jmp    NEAR PTR loc_4_05DA
loc_4_0501:
        test   dword ptr [ebp + 30h], 200h
        jne    NEAR PTR loc_4_05AA
        mov    eax, dword ptr [ebp + 2ch]
        call   sub_4_0065
        mov    dword ptr [edi + 10h], eax
        mov    dword ptr [edi + 14h], ebx
        mov    dword ptr [edi + 1ch], ecx
        mov    dword ptr [edi + 18h], edx
        jae    SHORT loc_4_0536
        db  068h, 000h, 000h, 000h, 000h
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_052E:
        add    esp, 4
        jmp    NEAR PTR loc_4_05DA
loc_4_0536:
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
        jae    SHORT loc_4_057E
        db  068h, 000h, 000h, 000h, 000h
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0579:
        add    esp, 4
        jmp    SHORT loc_4_05DA
loc_4_057E:
        mov    ax, word ptr [ebp + 28h]
        cmp    ax, word ptr [ebp + 24h]
        je     SHORT loc_4_05AA
        movzx  eax, al
        db  0BEh, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 001h, 000h, 004h, 000h
        mov    dword ptr [edi + 3ch], eax
        jae    SHORT loc_4_05AA
        db  068h, 000h, 000h, 000h, 000h
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_05A5:
        add    esp, 4
        jmp    SHORT loc_4_05DA
loc_4_05AA:
        db  0CDh, 020h, 003h, 000h, 001h, 000h
        push   ebx
loc_4_05B1:
        call   sub_4_00CB
        jb     SHORT loc_4_05D9
        db  0CDh, 020h, 03Bh, 000h, 001h, 000h
        cmp    ebx, dword ptr [esp]
        jne    SHORT loc_4_05B1
        pop    ebx
        call   sub_4_1330
        jb     SHORT loc_4_05DA
        mov    dword ptr [esp + 1ch], 0ffffffffh
loc_4_05D3:
        popad
        add    esp, 4
        pop    ebp
        ret
loc_4_05D9:
        pop    ebx
loc_4_05DA:
        mov    edi, dword ptr [ebp - 4]
        or     edi, edi
        je     SHORT loc_4_05E6
        call   sub_4_05F0
loc_4_05E6:
        mov    dword ptr [esp + 1ch], 0
        jmp    SHORT loc_4_05D3
sub_4_05F0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        pushad
        mov    dword ptr [ebp - 4], edi
        mov    edi, dword ptr [edi]
        or     edi, edi
        je     NEAR PTR loc_4_0739
        xor    esi, esi
        xchg   dword ptr [edi + 44h], esi
        or     esi, esi
        je     SHORT loc_4_0613
        db  0CDh, 020h, 09Ch, 000h, 001h, 000h
loc_4_0613:
        mov    eax, dword ptr [edi + 3ch]
        or     eax, eax
        je     SHORT loc_4_0620
        db  0CDh, 020h, 01Ah, 000h, 004h, 000h
loc_4_0620:
        mov    eax, dword ptr [edi + 38h]
        or     eax, eax
        je     SHORT loc_4_062D
        db  0CDh, 020h, 01Ah, 000h, 004h, 000h
loc_4_062D:
        movzx  eax, word ptr [edi + 20h]
        or     eax, eax
        je     SHORT loc_4_0650
        push   0
        push   eax
        db  0CDh, 020h, 077h, 000h, 001h, 000h
        add    esp, 8
        mov    eax, dword ptr [edi + 10h]
        push   0
        push   eax
        db  0CDh, 020h, 055h, 000h, 001h, 000h
        add    esp, 8
loc_4_0650:
        mov    eax, dword ptr [edi + 64h]
        or     eax, eax
        je     SHORT loc_4_067E
        db  0BEh, 000h, 000h, 000h, 000h
        movzx  ecx, word ptr [esi]
        movzx  edx, word ptr [edi + 40h]
        push   edi
        mov    edi, esi
        add    esi, 2
loc_4_0669:
        mov    word ptr [esi], dx
        inc    edx
        add    esi, 6
        loop   loc_4_0669
        mov    eax, dword ptr [esp]
        mov    eax, dword ptr [eax + 64h]
        call   sub_4_0242
        pop    edi
loc_4_067E:
        mov    eax, dword ptr [edi + 60h]
        or     eax, eax
        je     SHORT loc_4_06AC
        db  0BEh, 000h, 000h, 000h, 000h
        movzx  ecx, word ptr [esi]
        movzx  edx, word ptr [edi + 22h]
        push   edi
        mov    edi, esi
        add    esi, 2
loc_4_0697:
        mov    word ptr [esi], dx
        inc    edx
        add    esi, 6
        loop   loc_4_0697
        mov    eax, dword ptr [esp]
        mov    eax, dword ptr [eax + 60h]
        call   sub_4_0242
        pop    edi
loc_4_06AC:
        mov    eax, dword ptr [edi + 5ch]
        or     eax, eax
        je     SHORT loc_4_06DA
        db  0BEh, 000h, 000h, 000h, 000h
        movzx  ecx, word ptr [esi]
        movzx  edx, word ptr [edi + 42h]
        push   edi
        mov    edi, esi
        add    esi, 2
loc_4_06C5:
        mov    word ptr [esi], dx
        inc    edx
        add    esi, 6
        loop   loc_4_06C5
        mov    eax, dword ptr [esp]
        mov    eax, dword ptr [eax + 5ch]
        call   sub_4_0242
        pop    edi
loc_4_06DA:
        mov    eax, dword ptr [edi + 58h]
        or     eax, eax
        je     SHORT loc_4_0708
        db  0BEh, 000h, 000h, 000h, 000h
        movzx  ecx, word ptr [esi]
        movzx  edx, word ptr [edi + 6]
        push   edi
        mov    edi, esi
        add    esi, 2
loc_4_06F3:
        mov    word ptr [esi], dx
        inc    edx
        add    esi, 6
        loop   loc_4_06F3
        mov    eax, dword ptr [esp]
        mov    eax, dword ptr [eax + 58h]
        call   sub_4_0242
        pop    edi
loc_4_0708:
        mov    eax, dword ptr [edi + 2ch]
        or     eax, eax
        je     SHORT loc_4_0723
        cli
        db  0CDh, 020h, 004h, 000h, 003h, 000h
        db  0CDh, 020h, 008h, 000h, 003h, 000h
        db  0CDh, 020h, 011h, 000h, 003h, 000h
        sti
loc_4_0723:
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0728:
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_072D:
        push   0
        push   edi
        db  0CDh, 020h, 051h, 000h, 001h, 000h
        add    esp, 8
loc_4_0739:
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
sub_4_0756:
        push   ebp
        mov    ebp, esp
        pushad
        xor    eax, eax
        xor    ecx, ecx
        mov    al, byte ptr [ebp + 0ch]
loc_4_0761:
        db  080h, 0B9h, 000h, 000h, 000h, 000h, 0FFh
        je     SHORT loc_4_07B5
        db  03Ah, 081h, 000h, 000h, 000h, 000h
        je     SHORT loc_4_0775
        inc    ecx
        jmp    SHORT loc_4_0761
loc_4_0775:
        db  08Ah, 0A1h, 000h, 000h, 000h, 000h
        and    ah, 3fh
        xor    ecx, ecx
        mov    al, byte ptr [ebp + 10h]
loc_4_0783:
        db  080h, 0B9h, 000h, 000h, 000h, 000h, 0FFh
        je     SHORT loc_4_07B5
        db  03Ah, 081h, 000h, 000h, 000h, 000h
        je     SHORT loc_4_0797
        inc    ecx
        jmp    SHORT loc_4_0783
loc_4_0797:
        db  00Ah, 0A1h, 000h, 000h, 000h, 000h
        mov    cx, word ptr [ebp + 10h]
        cmp    cx, word ptr [ebp + 14h]
        je     SHORT loc_4_07AA
        or     ah, 4
loc_4_07AA:
        mov    al, ah
        movzx  edx, word ptr [ebp + 8]
        out    dx, al
        clc
loc_4_07B2:
        popad
        pop    ebp
        ret
loc_4_07B5:
        stc
        jmp    SHORT loc_4_07B2
sub_4_07B8:
        push   ebp
        mov    ebp, esp
        pushad
        movzx  edx, word ptr [ebp + 8]
        xor    al, al
        add    edx, 0ah
        out    dx, al
        mov    ecx, 3e8h
        sub    edx, 0ah
loc_4_07CE:
        mov    al, 0ah
        out    dx, al
        in     al, dx
        cmp    al, 0ah
        je     SHORT loc_4_07D8
        loop   loc_4_07CE
loc_4_07D8:
        popad
        pop    ebp
        ret
sub_4_07DB:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        pushad
        mov    dword ptr [esp + 1ch], 13h
        movzx  edx, word ptr [ebp + 0ch]
        push   4
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_07F6:
        add    esp, 8
        or     edi, edi
        jne    NEAR PTR loc_4_095D
        mov    dword ptr [ebp - 4], 0
        test   dword ptr [ebp + 30h], 100h
        jne    NEAR PTR loc_4_0925
        test   dword ptr [ebp + 30h], 200h
        jne    NEAR PTR loc_4_091A
        movzx  edx, word ptr [ebp + 0ch]
        call   sub_4_0A74
        jb     NEAR PTR loc_4_0963
        movsx  edx, word ptr [ebp + 18h]
        cmp    edx, -1
        je     SHORT loc_4_0845
        call   sub_4_0CB4
        jb     NEAR PTR loc_4_0963
loc_4_0845:
        movsx  edx, word ptr [ebp + 10h]
        cmp    edx, -1
        je     SHORT loc_4_08AD
        test   dword ptr [ebp + 30h], 400h
        jne    SHORT loc_4_0897
        push   edx
        call   sub_4_0ABF
        add    esp, 4
        or     eax, eax
        je     NEAR PTR loc_4_0963
        push   dword ptr [ebp + 20h]
        push   dword ptr [ebp + 10h]
        call   sub_4_0BDC
        add    esp, 8
        or     eax, eax
        je     NEAR PTR loc_4_095D
        push   dword ptr [ebp + 28h]
        push   dword ptr [ebp + 24h]
        push   dword ptr [ebp + 10h]
        call   sub_4_0C37
        add    esp, 0ch
        or     eax, eax
        je     NEAR PTR loc_4_095D
loc_4_0897:
        push   dword ptr [ebp + 28h]
        push   dword ptr [ebp + 24h]
        push   dword ptr [ebp + 20h]
        push   dword ptr [ebp + 10h]
        call   sub_4_0756
        add    esp, 10h
        jmp    SHORT loc_4_0925
loc_4_08AD:
        cmp    word ptr [ebp + 14h], -1
        je     SHORT loc_4_08F1
        push   1
        push   dword ptr [ebp + 14h]
        push   dword ptr [ebp + 0ch]
        call   sub_4_0B07
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_4_08E6
        or     dword ptr [ebp - 4], 10h
        push   dword ptr [ebp + 28h]
        push   dword ptr [ebp + 24h]
        push   dword ptr [ebp + 20h]
        push   dword ptr [ebp + 14h]
        call   sub_4_0B5C
        add    esp, 10h
        or     eax, eax
        je     SHORT loc_4_095D
        jmp    SHORT loc_4_0925
loc_4_08E6:
        call   sub_4_0C8D
        jb     SHORT loc_4_08F1
        or     dword ptr [ebp - 4], 30h
loc_4_08F1:
        push   dword ptr [ebp + 20h]
        push   dword ptr [ebp + 0ch]
        call   sub_4_0E8F
        add    esp, 8
        or     eax, eax
        je     SHORT loc_4_095D
        push   dword ptr [ebp + 28h]
        push   dword ptr [ebp + 24h]
        push   dword ptr [ebp + 0ch]
        call   sub_4_1047
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_4_095D
        jmp    SHORT loc_4_0925
loc_4_091A:
        push   dword ptr [ebp + 0ch]
        call   sub_4_07B8
        add    esp, 4
loc_4_0925:
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
        call   sub_4_026B
        add    esp, 30h
        or     eax, eax
        je     SHORT loc_4_095D
        mov    dword ptr [esp + 1ch], 0
loc_4_095D:
        popad
        add    esp, 4
        pop    ebp
        ret
loc_4_0963:
        mov    dword ptr [esp + 1ch], 24h
        jmp    SHORT loc_4_095D
sub_4_096D:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   ebx
        pushfd
        push   0
        push   dword ptr [ebp + 8]
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_097E:
        add    esp, 8
        or     edi, edi
        je     NEAR PTR loc_4_0A2C
        push   dword ptr [ebp + 8]
        call   0
        add    esp, 4
        or     eax, eax
        je     SHORT loc_4_09BA
        mov    ebx, dword ptr [edi + 24h]
        or     ebx, ebx
        je     SHORT loc_4_09A9
        mov    eax, 1
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_09A9:
        mov    ebx, dword ptr [edi + 30h]
        or     ebx, ebx
        je     SHORT loc_4_09BA
        mov    eax, 2
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_09BA:
        cli
        mov    eax, dword ptr [edi + 38h]
        or     eax, eax
        je     SHORT loc_4_09C8
        db  0CDh, 020h, 018h, 000h, 004h, 000h
loc_4_09C8:
        mov    eax, dword ptr [edi + 3ch]
        or     eax, eax
        je     SHORT loc_4_09D5
        db  0CDh, 020h, 018h, 000h, 004h, 000h
loc_4_09D5:
        test   word ptr [edi + 52h], 4
        je     SHORT loc_4_09E4
        movzx  edx, word ptr [edi + 6]
        xor    eax, eax
        out    dx, al
loc_4_09E4:
        mov    ebx, dword ptr [ebp + 8]
        db  08Bh, 035h, 000h, 000h, 000h, 000h
        or     esi, esi
        je     SHORT loc_4_0A31
        db  0CDh, 020h, 0A3h, 000h, 001h, 000h
        je     SHORT loc_4_0A31
loc_4_09F9:
        mov    edi, dword ptr [eax]
        cmp    dword ptr [edi + 54h], ebx
        je     SHORT loc_4_0A0A
        db  0CDh, 020h, 0A4h, 000h, 001h, 000h
        je     SHORT loc_4_0A31
        jmp    SHORT loc_4_09F9
loc_4_0A0A:
        sti
        db  0CDh, 020h, 003h, 000h, 001h, 000h
        push   ebx
loc_4_0A12:
        call   sub_4_0139
        db  0CDh, 020h, 03Bh, 000h, 001h, 000h
        cmp    ebx, dword ptr [esp]
        jne    SHORT loc_4_0A12
        pop    ebx
        mov    edi, eax
        call   sub_4_05F0
        je     SHORT loc_4_0A2C
loc_4_0A2C:
        mov    eax, 0ffffffffh
loc_4_0A31:
        popfd
        pop    ebx
        pop    edi
        pop    esi
        pop    ebp
        ret
loc_4_0A37:
        push   ebp
        mov    ebp, esp
        push   dword ptr [ebp + 18h]
        push   dword ptr [ebp + 14h]
        push   dword ptr [ebp + 10h]
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp + 8]
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0A4E:
        add    esp, 14h
        leave
        clc
        ret
loc_4_0A54:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        mov    eax, ebx
        db  0BBh, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 000h, 000h, 04Ah, 004h
        mov    eax, 0
        add    esp, 4
        pop    ebp
        stc
        ret
        db  0CCh, 0CCh
sub_4_0A74:
        push   eax
        push   ecx
        push   edx
        xor    ecx, ecx
        in     al, dx
        test   al, 80h
        jne    SHORT loc_4_0AB7
        mov    cl, al
        mov    al, 0ch
        out    dx, al
        inc    edx
        xor    al, al
        out    dx, al
        in     al, dx
        test   al, 80h
        je     SHORT loc_4_0A8E
        and    al, 0bfh
loc_4_0A8E:
        cmp    al, 9
        je     SHORT loc_4_0AA0
        cmp    al, 0ah
        je     SHORT loc_4_0AA0
        cmp    al, 81h
        je     SHORT loc_4_0AA0
        cmp    al, 8ah
        je     SHORT loc_4_0AA0
        jmp    SHORT loc_4_0AB3
loc_4_0AA0:
        shl    ecx, 8
        mov    cl, al
        xor    al, al
        out    dx, al
        in     al, dx
        cmp    al, cl
        je     SHORT loc_4_0ABA
        mov    al, cl
        out    dx, al
        shr    ecx, 8
loc_4_0AB3:
        dec    edx
        mov    al, cl
        out    dx, al
loc_4_0AB7:
        stc
        jmp    SHORT loc_4_0ABB
loc_4_0ABA:
        clc
loc_4_0ABB:
        pop    edx
        pop    ecx
        pop    eax
        ret
sub_4_0ABF:
        push   ebp
        mov    ebp, esp
        push   edx
        movzx  edx, word ptr [ebp + 8]
        add    dx, 3
        in     al, dx
        and    al, 3fh
        cmp    al, 4
        jne    SHORT loc_4_0AD9
        movzx  eax, al
        clc
        jmp    SHORT loc_4_0ADC
        db  0C3h
loc_4_0AD9:
        xor    eax, eax
        stc
loc_4_0ADC:
        pop    edx
        pop    ebp
        ret
sub_4_0ADF:
        jmp    SHORT loc_4_0AE7
loc_4_0AE1:
        inc    ebx
        dec    edi
        dec    ebp
        push   eax
        inc    ecx
        push   ecx
loc_4_0AE7:
        push   ecx
        push   edi
        push   esi
        mov    esi, 0fffeah
        db  08Dh, 03Dh, 000h, 000h, 000h, 000h
        mov    ecx, 6
        cld
        repe cmpsb
        jne    SHORT loc_4_0B02
        clc
        jmp    SHORT loc_4_0B03
loc_4_0B02:
        stc
loc_4_0B03:
        pop    esi
        pop    edi
        pop    ecx
        ret
sub_4_0B07:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   edx
        call   sub_4_0ADF
        jb     SHORT loc_4_0B4C
        movzx  edx, word ptr [ebp + 0ch]
        mov    ah, 10h
        cmp    word ptr [ebp + 8], 608h
        jne    SHORT loc_4_0B24
        or     ah, 20h
loc_4_0B24:
        in     al, dx
        cmp    al, 0ffh
        je     SHORT loc_4_0B31
        and    al, 30h
        cmp    al, ah
        jne    SHORT loc_4_0B31
        jmp    SHORT loc_4_0B53
loc_4_0B31:
        movzx  eax, word ptr [ebp + 10h]
        test   eax, 1
        jne    SHORT loc_4_0B4C
        call   sub_4_0C8D
        jb     SHORT loc_4_0B4C
        add    edx, 4
        cmp    dx, word ptr [ebp + 8]
        je     SHORT loc_4_0B53
loc_4_0B4C:
        mov    eax, 0
        jmp    SHORT loc_4_0B58
loc_4_0B53:
        mov    eax, 0ffffffffh
loc_4_0B58:
        pop    edx
        pop    ebx
        pop    ebp
        ret
sub_4_0B5C:
        push   ebp
        mov    ebp, esp
        pushad
        movzx  edx, word ptr [ebp + 8]
        add    edx, 3
        in     al, dx
        cmp    al, 0ffh
        je     SHORT loc_4_0BD2
        mov    ah, al
        movzx  ecx, al
        shr    ecx, 4
        or     ecx, ecx
        jne    SHORT loc_4_0B7A
        jmp    SHORT loc_4_0BD2
loc_4_0B7A:
        inc    ecx
        and    ecx, 3
        db  08Ah, 081h, 000h, 000h, 000h, 000h
        cmp    al, byte ptr [ebp + 0ch]
        jne    SHORT loc_4_0BD2
        movzx  ecx, ah
        shr    ecx, 6
        jecxz  loc_4_0BD2
        dec    ecx
        db  08Ah, 081h, 000h, 000h, 000h, 000h
        cmp    al, byte ptr [ebp + 10h]
        cmp    word ptr [ebp + 14h], -1
        je     SHORT loc_4_0BC7
        cmp    al, byte ptr [ebp + 14h]
        je     SHORT loc_4_0BC7
        sub    edx, 2
        in     al, dx
        cmp    al, 0ffh
        je     SHORT loc_4_0BD2
        movzx  ecx, al
        shr    ecx, 6
        or     ecx, ecx
        je     SHORT loc_4_0BD2
        jecxz  loc_4_0BD2
        dec    ecx
        db  08Ah, 081h, 000h, 000h, 000h, 000h
        cmp    al, byte ptr [ebp + 14h]
        jne    SHORT loc_4_0BD2
loc_4_0BC7:
        mov    dword ptr [esp + 1ch], 0ffffffffh
loc_4_0BCF:
        popad
        pop    ebp
        ret
loc_4_0BD2:
        mov    dword ptr [esp + 1ch], 0
        jmp    SHORT loc_4_0BCF
sub_4_0BDC:
        push   ebp
        mov    ebp, esp
        pushfd
        cli
        push   ecx
        push   edx
        movzx  edx, word ptr [ebp + 8]
        mov    ax, word ptr [ebp + 0ch]
        xor    ecx, ecx
loc_4_0BED:
        db  08Ah, 0A1h, 000h, 000h, 000h, 000h
        cmp    ah, 0ffh
        je     SHORT loc_4_0C2D
        db  038h, 081h, 000h, 000h, 000h, 000h
        je     SHORT loc_4_0C03
        inc    ecx
        jmp    SHORT loc_4_0BED
loc_4_0C03:
        xchg   ah, al
        out    dx, al
        add    dx, 3
        in     al, dx
        test   al, 40h
        je     SHORT loc_4_0C2D
        mov    edx, 1
        mov    cl, ah
        shl    edx, cl
        test   al, 80h
        je     SHORT loc_4_0C22
        and    edx, 2ffh
loc_4_0C22:
        or     edx, edx
        je     SHORT loc_4_0C2D
        mov    eax, 0ffffffffh
        jmp    SHORT loc_4_0C32
loc_4_0C2D:
        mov    eax, 0
loc_4_0C32:
        pop    edx
        pop    ecx
        popfd
        pop    ebp
        ret
sub_4_0C37:
        push   ebp
        mov    ebp, esp
        push   ecx
        push   edx
        movzx  edx, word ptr [ebp + 8]
        mov    al, byte ptr [ebp + 10h]
        mov    ah, byte ptr [ebp + 0ch]
        xor    ecx, ecx
loc_4_0C48:
        db  080h, 0B9h, 000h, 000h, 000h, 000h, 0FFh
        je     SHORT loc_4_0C84
        db  038h, 0A1h, 000h, 000h, 000h, 000h
        je     SHORT loc_4_0C5C
        inc    ecx
        jmp    SHORT loc_4_0C48
loc_4_0C5C:
        push   eax
        add    dx, 3
        in     al, dx
        test   al, 80h
        pop    eax
        je     SHORT loc_4_0C6B
        or     ah, ah
        je     SHORT loc_4_0C84
loc_4_0C6B:
        cmp    al, ah
        je     SHORT loc_4_0C7D
        or     ah, ah
        je     SHORT loc_4_0C79
        or     al, al
        jne    SHORT loc_4_0C84
        jmp    SHORT loc_4_0C7D
loc_4_0C79:
        cmp    al, 1
        jne    SHORT loc_4_0C84
loc_4_0C7D:
        mov    eax, 0ffffffffh
        jmp    SHORT loc_4_0C89
loc_4_0C84:
        mov    eax, 0
loc_4_0C89:
        pop    edx
        pop    ecx
        pop    ebp
        ret
sub_4_0C8D:
        pushad
        call   sub_4_0ADF
        jb     SHORT loc_4_0CB1
        push   edx
        mov    edx, 0c7ch
        in     al, dx
        pop    edx
        mov    bl, al
        mov    bh, bl
        and    bh, 0f0h
        cmp    bh, 10h
        jne    SHORT loc_4_0CB1
        test   bl, 3
        jne    SHORT loc_4_0CB1
        popad
        clc
        ret
loc_4_0CB1:
        popad
        stc
        ret
sub_4_0CB4:
        push   eax
        push   ebx
        push   ecx
        pushfd
        cli
        mov    ebx, 4
        mov    al, 60h
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0CC5:
        mov    ebx, 4
        mov    al, 80h
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0CD1:
        mov    ebx, 2
        mov    al, 80h
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0CDD:
        mov    ebx, 4
        mov    al, 1
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0CE9:
        mov    cx, 4000h
loc_4_0CED:
        in     al, dx
        test   al, 40h
        jne    SHORT loc_4_0CF4
        loop   loc_4_0CED
loc_4_0CF4:
        push   eax
        mov    ebx, 4
        mov    al, 60h
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0D01:
        mov    ebx, 4
        mov    al, 80h
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0D0D:
        pop    eax
        popfd
        test   al, 40h
        je     SHORT loc_4_0D1A
        test   al, 6
        jne    SHORT loc_4_0D1A
        clc
        jmp    SHORT loc_4_0D1B
loc_4_0D1A:
        stc
loc_4_0D1B:
        pop    ecx
        pop    ebx
        pop    eax
        ret
sub_4_0D1F:
        push   ebx
        mov    bh, 9
        mov    ah, 0ch
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0D29:
        jb     SHORT loc_4_0D4B
        mov    bh, al
        cmp    al, 0ah
        je     SHORT loc_4_0D4F
        cmp    al, 8ah
        jne    SHORT loc_4_0D4B
        mov    al, 40h
        mov    ah, 0ch
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0D3E:
        jmp    SHORT loc_4_0D40
loc_4_0D40:
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0D45:
        test   al, 40h
        jne    SHORT loc_4_0D53
        jmp    SHORT loc_4_0D4F
loc_4_0D4B:
        mov    bl, 0
        jmp    SHORT loc_4_0D55
loc_4_0D4F:
        mov    bl, 1
        jmp    SHORT loc_4_0D55
loc_4_0D53:
        mov    bl, 2
loc_4_0D55:
        movzx  eax, bx
        pop    ebx
        ret
sub_4_0D5A:
        push   eax
        push   ecx
        push   edx
        mov    eax, 1
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0D67:
        mov    ah, 6
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0D6E:
        shl    eax, 8
        push   eax
        mov    ah, 7
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0D79:
        movzx  eax, al
        or     dword ptr [esp], eax
        mov    eax, 80h
        mov    ah, 6
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0D8B:
        inc    ah
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0D92:
        db  0C6h, 005h, 000h, 000h, 000h, 000h, 040h
        mov    eax, 4bh
        add    edx, 0
        out    dx, al
        sub    edx, 0
        mov    ah, 8
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0DAC:
        push   eax
        mov    al, 3
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0DB4:
        xor    eax, eax
        mov    ah, 0fh
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0DBD:
        dec    ah
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0DC4:
        mov    ah, 0ah
        db  0A0h, 000h, 000h, 000h, 000h
        or     al, 2
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0DD2:
        mov    ah, 9
        mov    al, 41h
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0DDB:
        mov    eax, 0bh
        add    edx, 0
        out    dx, al
        db  0C6h, 005h, 000h, 000h, 000h, 000h, 000h
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0DF0:
        add    edx, 3
        xor    eax, eax
        out    dx, al
        sub    edx, 3
        add    edx, 2
        mov    ecx, 1000h
loc_4_0E01:
        in     al, dx
        test   al, 1
        jne    SHORT loc_4_0E0F
        loop   loc_4_0E01
        mov    ecx, 1f4h
loc_4_0E0D:
        loop   loc_4_0E0D
loc_4_0E0F:
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
loc_4_0E34:
        pop    eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0E3A:
        db  080h, 025h, 000h, 000h, 000h, 000h, 0BFh
        mov    eax, 0bh
        db  00Ah, 005h, 000h, 000h, 000h, 000h
        add    edx, 0
        out    dx, al
        sub    edx, 0
        mov    eax, dword ptr [esp]
        mov    ah, 7
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0E5D:
        pop    eax
        shr    eax, 8
        mov    ah, 6
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0E68:
        pop    edx
        pop    ecx
        pop    eax
        ret
sub_4_0E6C:
        push   eax
        add    edx, 2
        in     al, dx
        sub    edx, 2
        test   al, 1
        je     SHORT loc_4_0E81
        xor    eax, eax
        add    edx, 2
        out    dx, al
        sub    edx, 2
loc_4_0E81:
        mov    ah, 0ah
        db  0A0h, 000h, 000h, 000h, 000h
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0E8D:
        pop    eax
        ret
sub_4_0E8F:
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
        jmp    SHORT loc_4_0EBB
loc_4_0EBB:
        out    20h, al
        jmp    SHORT loc_4_0EBF
loc_4_0EBF:
        call   sub_4_0E6C
        in     al, 0a0h
        mov    ah, al
        in     al, 20h
        test   ecx, eax
        jne    SHORT loc_4_0EE7
        call   sub_4_0D5A
        push   ecx
        mov    ecx, 1f4h
loc_4_0ED9:
        in     al, dx
        loop   loc_4_0ED9
        pop    ecx
        in     al, 0a0h
        mov    ah, al
        in     al, 20h
        test   ecx, eax
        jne    SHORT loc_4_0EF3
loc_4_0EE7:
        push   ecx
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0EEE:
        add    esp, 8
        xor    eax, eax
loc_4_0EF3:
        call   sub_4_0E6C
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
sub_4_0F0B:
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
        jne    SHORT loc_4_0F3A
        add    al, 48h
        jmp    SHORT loc_4_0F3C
loc_4_0F3A:
        add    al, 44h
loc_4_0F3C:
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
loc_4_0F7D:
        mov    ah, 0eh
        xor    al, al
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0F86:
        mov    ah, 6
        mov    al, 3fh
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0F8F:
        mov    ah, 7
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0F96:
        mov    ah, 0ah
        db  0A0h, 000h, 000h, 000h, 000h
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0FA2:
        db  0C6h, 005h, 000h, 000h, 000h, 000h, 040h
        mov    eax, 4bh
        add    edx, 0
        out    dx, al
        sub    edx, 0
        mov    al, 1
        test   dword ptr [ebp + 14h], 1
        je     SHORT loc_4_0FC2
        mov    al, 2
loc_4_0FC2:
        test   dword ptr [ebp + 14h], 2
        je     SHORT loc_4_0FD8
        or     al, 4
        test   dword ptr [ebp + 14h], 1
        je     SHORT loc_4_0FD8
        or     al, 40h
loc_4_0FD8:
        mov    ah, 9
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0FDF:
        mov    eax, 0bh
        add    edx, 0
        out    dx, al
        db  0C6h, 005h, 000h, 000h, 000h, 000h, 000h
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0FF4:
        mov    edx, ebx
        shl    edx, 1
        add    edx, 1
        mov    ecx, 0ffffh
loc_4_1000:
        xor    al, al
        out    0ch, al
        jmp    SHORT loc_4_1006
loc_4_1006:
        in     al, dx
        mov    ah, al
        jmp    SHORT loc_4_100B
loc_4_100B:
        in     al, dx
        cmp    ax, -1
        je     SHORT loc_4_1026
        loop   loc_4_1000
        push   eax
        push   ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_101B:
        add    esp, 8
        mov    dword ptr [esp + 1ch], 0
loc_4_1026:
        movzx  edx, word ptr [ebp + 8]
        mov    ah, 9
        xor    al, al
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1033:
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
sub_4_1047:
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
        je     SHORT loc_4_10C1
        mov    edi, edx
        mov    ecx, 400h
        mov    eax, 80808080h
        cld
        rep stosd
        push   0
        push   dword ptr [ebp - 4]
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp + 8]
        call   sub_4_0F0B
        add    esp, 10h
        or     eax, eax
        je     SHORT loc_4_10C1
        mov    ecx, 1
        mov    ax, word ptr [ebp + 0ch]
        cmp    ax, word ptr [ebp + 10h]
        jne    SHORT loc_4_10AF
        or     ecx, 2
loc_4_10AF:
        push   ecx
        push   dword ptr [ebp - 4]
        push   dword ptr [ebp + 10h]
        push   dword ptr [ebp + 8]
        call   sub_4_0F0B
        add    esp, 10h
loc_4_10C1:
        mov    dword ptr [esp + 1ch], eax
        mov    eax, dword ptr [ebp - 8]
        or     eax, eax
        je     SHORT loc_4_10D8
        push   0
        push   eax
        db  0CDh, 020h, 055h, 000h, 001h, 000h
        add    esp, 8
loc_4_10D8:
        popad
        add    esp, 8
        pop    ebp
        ret
        db  0CCh, 0CCh
sub_4_10E0:
        push   ebp
        mov    ebp, esp
        pushad
        mov    dword ptr [esp + 1ch], 13h
        xor    ah, ah
        mov    dx, word ptr [ebp + 8]
        cmp    dx, 220h
        je     SHORT loc_4_10FB
        inc    ah
loc_4_10FB:
        mov    dx, 620h
        in     al, dx
        test   al, 1
        je     SHORT loc_4_1108
        mov    dx, 640h
loc_4_1108:
        add    dx, 4
        in     al, dx
        or     al, 80h
        out    dx, al
        sub    dx, 4
        in     al, dx
        and    al, 0feh
        or     al, ah
        out    dx, al
        mov    dx, word ptr [ebp + 8]
        add    dx, 404h
        in     al, dx
        and    al, 7fh
        out    dx, al
        mov    dx, word ptr [ebp + 8]
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1130:
        jb     NEAR PTR loc_4_1325
        mov    dx, word ptr [ebp + 8]
        mov    al, 8
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1141:
        jb     NEAR PTR loc_4_1325
        mov    al, 5
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_114E:
        jb     NEAR PTR loc_4_1325
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1159:
        jb     NEAR PTR loc_4_1325
        cmp    al, 12h
        jne    SHORT loc_4_1165
        mov    al, 11h
loc_4_1165:
        cmp    al, 11h
        jne    NEAR PTR loc_4_1272
        mov    dx, word ptr [ebp + 8]
        add    dx, 404h
        in     al, dx
        or     al, 80h
        out    dx, al
        xor    ah, ah
        mov    bx, word ptr [ebp + 10h]
        cmp    bx, 0ah
        jne    SHORT loc_4_118B
        or     ah, 20h
        jmp    SHORT loc_4_11AA
loc_4_118B:
        cmp    bx, 7
        jne    SHORT loc_4_1196
        or     ah, 10h
        jmp    SHORT loc_4_11AA
loc_4_1196:
        cmp    bx, 5
        jne    SHORT loc_4_11A1
        or     ah, 8
        jmp    SHORT loc_4_11AA
loc_4_11A1:
        cmp    bx, 9
        jne    SHORT loc_4_11AA
        or     ah, 4
loc_4_11AA:
        mov    bx, word ptr [ebp + 14h]
        cmp    bx, 3
        jne    SHORT loc_4_11B9
        or     ah, 0c0h
        jmp    SHORT loc_4_11CD
loc_4_11B9:
        cmp    bx, 1
        jne    SHORT loc_4_11C4
        or     ah, 80h
        jmp    SHORT loc_4_11CD
loc_4_11C4:
        cmp    bx, 0
        jne    SHORT loc_4_11CD
        or     ah, 40h
loc_4_11CD:
        mov    dx, word ptr [ebp + 8]
        add    dx, 400h
        in     al, dx
        and    al, 3
        or     al, ah
        out    dx, al
        xor    ah, ah
        mov    bx, word ptr [ebp + 0ch]
        cmp    bx, 530h
        jne    SHORT loc_4_11EE
        or     ah, 4
        jmp    SHORT loc_4_1210
loc_4_11EE:
        cmp    bx, 604h
        jne    SHORT loc_4_11FA
        or     ah, 5
        jmp    SHORT loc_4_1210
loc_4_11FA:
        cmp    bx, 0e80h
        jne    SHORT loc_4_1206
        or     ah, 6
        jmp    SHORT loc_4_1210
loc_4_1206:
        cmp    bx, 0f40h
        jne    SHORT loc_4_1210
        or     ah, 7
loc_4_1210:
        mov    dx, word ptr [ebp + 8]
        add    dx, 401h
        in     al, dx
        and    al, 0f8h
        or     al, ah
        out    dx, al
        mov    dx, word ptr [ebp + 8]
        add    dx, 404h
        in     al, dx
        and    al, 7fh
        out    dx, al
        mov    dx, word ptr [ebp + 8]
        mov    al, 9
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1237:
        jb     NEAR PTR loc_4_1325
        mov    al, 0
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1244:
        jb     NEAR PTR loc_4_1325
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_124F:
        mov    dx, word ptr [ebp + 8]
        add    dx, 404h
        mov    ecx, 10h
loc_4_125D:
        in     al, dx
        test   al, 8
        je     NEAR PTR loc_4_131A
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_126B:
        loop   loc_4_125D
        jmp    NEAR PTR loc_4_1325
loc_4_1272:
        cmp    al, 0ch
        jne    NEAR PTR loc_4_1318
        mov    dx, word ptr [ebp + 8]
        add    dx, 404h
        in     al, dx
        or     al, 80h
        out    dx, al
        xor    ah, ah
        mov    bx, word ptr [ebp + 10h]
        cmp    bx, 7
        jne    SHORT loc_4_1298
        or     ah, 8
        jmp    SHORT loc_4_12B7
loc_4_1298:
        cmp    bx, 5
        jne    SHORT loc_4_12A3
        or     ah, 4
        jmp    SHORT loc_4_12B7
loc_4_12A3:
        cmp    bx, 3
        jne    SHORT loc_4_12AE
        or     ah, 2
        jmp    SHORT loc_4_12B7
loc_4_12AE:
        cmp    bx, 9
        jne    SHORT loc_4_12B7
        or     ah, 1
loc_4_12B7:
        mov    dx, word ptr [ebp + 8]
        add    dx, 401h
        in     al, dx
        and    al, 0f0h
        or     al, ah
        out    dx, al
        xor    ah, ah
        mov    bx, word ptr [ebp + 0ch]
        cmp    bx, 530h
        jne    SHORT loc_4_12D8
        or     ah, 4
        jmp    SHORT loc_4_12FA
loc_4_12D8:
        cmp    bx, 604h
        jne    SHORT loc_4_12E4
        or     ah, 5
        jmp    SHORT loc_4_12FA
loc_4_12E4:
        cmp    bx, 0e80h
        jne    SHORT loc_4_12F0
        or     ah, 6
        jmp    SHORT loc_4_12FA
loc_4_12F0:
        cmp    bx, 0f40h
        jne    SHORT loc_4_12FA
        or     ah, 7
loc_4_12FA:
        mov    dx, word ptr [ebp + 8]
        add    dx, 402h
        in     al, dx
        and    al, 0f8h
        or     al, ah
        out    dx, al
        mov    dx, word ptr [ebp + 8]
        add    dx, 404h
        in     al, dx
        and    al, 7fh
        out    dx, al
        jmp    SHORT loc_4_131A
loc_4_1318:
        jmp    SHORT loc_4_1325
loc_4_131A:
        mov    dword ptr [esp + 1ch], 0
        popad
        pop    ebp
        ret
loc_4_1325:
        mov    dword ptr [esp + 1ch], 24h
        popad
        pop    ebp
        ret
sub_4_1330:
        pushad
        movzx  edx, word ptr [edi + 40h]
        mov    al, 8
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_133C:
        jb     SHORT loc_4_1376
        mov    al, 5
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1345:
        jb     SHORT loc_4_1376
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_134C:
        jb     SHORT loc_4_1376
        cmp    al, 12h
        jne    SHORT loc_4_1354
        mov    al, 11h
loc_4_1354:
        cmp    al, 11h
        jne    SHORT loc_4_1368
        mov    byte ptr [edi + 0b4h], 11h
        mov    byte ptr [edi + 0b5h], 1
        jmp    SHORT loc_4_1373
loc_4_1368:
        cmp    al, 0ch
        jne    SHORT loc_4_1376
        mov    byte ptr [edi + 0b4h], 0ch
loc_4_1373:
        clc
        popad
        ret
loc_4_1376:
        stc
        popad
        ret
_PTEXT ends
        end
