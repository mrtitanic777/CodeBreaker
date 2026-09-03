; apix_pcode_03.asm -- APIX object 3 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

        db  08Bh, 044h, 024h, 004h, 0BAh, 000h, 000h, 000h, 000h, 056h, 0BEh, 0B4h, 000h, 000h, 000h, 0CDh
        db  020h, 009h, 000h, 001h, 000h, 05Eh, 072h, 003h, 033h, 0C0h, 0C3h, 0B8h, 0FFh, 0FFh, 0FFh, 0FFh
        db  0C3h
sub_3_0021:
        mov    eax, dword ptr [esp + 4]
        db  0CDh, 020h, 00Ah, 000h, 001h, 000h
        ret
loc_3_002C:
        cmp    word ptr [ebp + 1ch], 8100h
        jne    SHORT loc_3_0066
        call   sub_3_03E9
        or     eax, eax
        je     SHORT loc_3_00AC
        push   eax
        call   sub_3_0637
        mov    dword ptr [esp], eax
        call   sub_3_18C0
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_3_005A
        mov    word ptr [ebp + 1ch], 0
        jmp    SHORT loc_3_00B2
loc_3_005A:
        cmp    eax, -1
        je     SHORT loc_3_00AC
        cmp    eax, -2
        jne    SHORT loc_3_00AC
        stc
        ret
loc_3_0066:
        cmp    word ptr [ebp + 1ch], 8200h
        jne    SHORT loc_3_00AC
        cmp    word ptr [ebp + 18h], 8765h
        jne    SHORT loc_3_00AC
        cmp    word ptr [ebp + 14h], 0cba9h
        jne    SHORT loc_3_00AC
        mov    word ptr [ebp + 18h], 9abch
        mov    word ptr [ebp + 14h], 5678h
        db  0A1h, 0B4h, 000h, 000h, 000h
        mov    ecx, 8
        db  0CDh, 020h, 01Dh, 000h, 001h, 000h
        jb     SHORT loc_3_00AC
        mov    word ptr [ebp + 38h], cx
        mov    word ptr [ebp], dx
        mov    word ptr [ebp + 1ch], 0
        jmp    SHORT loc_3_00B2
loc_3_00AC:
        mov    word ptr [ebp + 1ch], 0ffffh
loc_3_00B2:
        clc
        ret
loc_3_00B4:
        push   eax
        call   sub_3_03D3
        or     eax, eax
        je     NEAR PTR loc_3_017C
        mov    edx, eax
        push   eax
        call   sub_3_0637
        add    esp, 4
        mov    ecx, eax
        push   edx
        push   ecx
        push   0
        push   eax
        call   sub_3_1040
        add    esp, 8
        pop    ecx
        pop    edx
        or     eax, eax
        jne    SHORT loc_3_00EE
        mov    word ptr [ebp + 1ch], 0
        pop    eax
        jmp    NEAR PTR loc_3_0183
loc_3_00EE:
        cmp    eax, -3
        jne    SHORT loc_3_0109
        test   byte ptr [ecx + 3], 1
        je     SHORT loc_3_0100
        xor    eax, eax
        call   sub_3_05B8
loc_3_0100:
        mov    word ptr [ebp + 1ch], 0
        pop    eax
        jmp    SHORT loc_3_0183
loc_3_0109:
        cmp    eax, -1
        je     SHORT loc_3_017C
        cmp    eax, -2
        jne    SHORT loc_3_017C
        pop    ebx
        xor    esi, esi
        db  083h, 03Dh, 0C0h, 008h, 000h, 000h, 000h
        je     SHORT loc_3_0189
loc_3_011F:
        db  039h, 09Eh, 0C0h, 008h, 000h, 000h
        je     SHORT loc_3_012C
        add    esi, 8
        jmp    SHORT loc_3_011F
loc_3_012C:
        db  083h, 0BEh, 0C4h, 008h, 000h, 000h, 000h
        je     SHORT loc_3_0189
        db  0CDh, 020h, 082h, 000h, 001h, 000h
        call   sub_3_03D3
        push   eax
        shr    eax, 10h
        db  0CDh, 020h, 04Dh, 000h, 001h, 000h
        pop    eax
        and    eax, 0ffffh
        db  0CDh, 020h, 04Dh, 000h, 001h, 000h
        db  00Fh, 0B7h, 08Eh, 0C6h, 008h, 000h, 000h
        db  00Fh, 0B7h, 096h, 0C4h, 008h, 000h, 000h
        db  0CDh, 020h, 048h, 000h, 001h, 000h
        db  0CDh, 020h, 085h, 000h, 001h, 000h
        add    dword ptr [ebp + 30h], 4
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        jmp    SHORT loc_3_0183
loc_3_017C:
        mov    word ptr [ebp + 1ch], 0ffffh
        pop    eax
loc_3_0183:
        db  0CDh, 020h, 04Ah, 080h, 001h, 000h
loc_3_0189:
        mov    word ptr [ebp + 1ch], 0ffffh
        jmp    SHORT loc_3_0183
        db  0F9h, 0C3h
APIX_PM_API:
        movzx  eax, word ptr [ebp + 1ch]
        or     ah, ah
        jne    SHORT loc_3_01A3
        mov    word ptr [ebp + 1ch], 400h
        jmp    SHORT loc_3_01CD
loc_3_01A3:
        dec    ah
        jne    SHORT loc_3_01BF
        mov    byte ptr [ebp + 1ch], 0
        mov    ecx, 8
        db  0A0h, 0BFh, 008h, 000h, 000h
loc_3_01B5:
        shr    al, 1
        adc    byte ptr [ebp + 1ch], 0
        loop   loc_3_01B5
        jmp    SHORT loc_3_01CD
loc_3_01BF:
        dec    ah
        jne    SHORT loc_3_01C5
        jmp    SHORT loc_3_01CD
loc_3_01C5:
        dec    ah
        jne    SHORT loc_3_01CB
        jmp    SHORT loc_3_01D9
loc_3_01CB:
        jmp    SHORT loc_3_01CD
loc_3_01CD:
        and    word ptr [ebp + 2ch], 0fffeh
        ret
        db  066h, 083h, 04Dh, 02Ch, 001h, 0C3h
loc_3_01D9:
        call   sub_3_03D3
        or     eax, eax
        je     SHORT loc_3_017C
        mov    edx, eax
        push   eax
        call   sub_3_0637
        add    esp, 4
        mov    ecx, eax
        push   edx
        push   ecx
        push   4
        push   eax
        call   sub_3_1040
        add    esp, 8
        pop    ecx
        pop    edx
        or     eax, eax
        jne    SHORT loc_3_020A
        mov    word ptr [ebp + 1ch], 0
        jmp    SHORT loc_3_022D
loc_3_020A:
        cmp    eax, -3
        jne    SHORT loc_3_0227
        test   byte ptr [ecx + 3], 1
        je     SHORT loc_3_021F
        mov    eax, 4
        call   sub_3_05B8
loc_3_021F:
        mov    word ptr [ebp + 1ch], 0
        jmp    SHORT loc_3_022D
loc_3_0227:
        mov    word ptr [ebp + 1ch], 0ffffh
loc_3_022D:
        jmp    SHORT loc_3_01CD
sub_3_022F:
        mov    eax, dword ptr [esp + 4]
        add    eax, 64h
        mov    eax, dword ptr [eax + 2ch]
        sub    esp, 8
        mov    dword ptr [esp + 4], eax
        mov    word ptr [esp], 4
        push   esp
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        add    esp, 0ch
        ret
sub_3_0251:
        mov    eax, dword ptr [esp + 4]
        sub    esp, 8
        mov    dword ptr [esp + 4], eax
        mov    word ptr [esp], 4
        push   esp
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        add    esp, 0ch
        ret
sub_3_026D:
        mov    eax, dword ptr [esp + 4]
        sub    esp, 10h
        mov    word ptr [esp + 4], ax
        mov    word ptr [esp + 0ah], 9
        mov    byte ptr [esp + 0ch], 2
        mov    word ptr [esp], 3
        push   esp
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        add    esp, 4
        mov    eax, dword ptr [esp + 6]
        add    esp, 10h
        ret
sub_3_029D:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0ch]
        mov    edi, dword ptr [esp + 10h]
        mov    ecx, dword ptr [esp + 14h]
        rep movsb
        pop    edi
        pop    esi
        ret
sub_3_02B0:
        db  08Dh, 005h, 0BCh, 000h, 000h, 000h
        jmp    SHORT loc_3_02C0
sub_3_02B8:
        db  08Dh, 005h, 0BCh, 004h, 000h, 000h
        jmp    SHORT loc_3_02C0
loc_3_02C0:
        add    eax, dword ptr [esp + 0ch]
        mov    ecx, dword ptr [esp + 8]
        shl    ecx, 3
        add    eax, ecx
        mov    ecx, dword ptr [esp + 4]
        shl    ecx, 7
        add    eax, ecx
        movzx  eax, byte ptr [eax]
        cmp    eax, 0ffh
        jne    SHORT loc_3_02E4
        xor    eax, eax
        jmp    SHORT loc_3_02EB
loc_3_02E4:
        db  005h, 009h, 000h, 000h, 000h
        mov    eax, dword ptr [eax]
loc_3_02EB:
        ret
sub_3_02EC:
        mov    edx, dword ptr [esp + 4]
        push   ebx
        movzx  ecx, byte ptr [edx + 77h]
        shl    ecx, 4
        mov    eax, dword ptr [edx + 4]
        add    ecx, eax
        add    ecx, 0bch
        add    ecx, 50h
        add    cx, word ptr [edx + 80h]
        add    eax, 64h
        sub    esp, 10h
        mov    word ptr [esp], 2
        mov    word ptr [esp + 4], cx
        mov    dword ptr [esp + 6], eax
        mov    byte ptr [esp + 0eh], 2
        push   esp
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        add    esp, 4
        mov    ecx, dword ptr [esp + 0ah]
        add    esp, 10h
        add    eax, ecx
        mov    dword ptr [eax], 0
        mov    word ptr [eax + 4], 0fh
        mov    word ptr [eax + 6], 12h
        mov    dword ptr [eax + 8], 0c400h
        mov    dword ptr [eax + 40h], eax
        sub    dword ptr [eax + 40h], 64h
        mov    dword ptr [eax + 44h], 0
        mov    bl, byte ptr [edx + 15h]
        mov    byte ptr [eax + 4dh], bl
        mov    dword ptr [eax + 2ch], ecx
        mov    dword ptr [eax + 48h], eax
        add    dword ptr [eax + 48h], 58h
        mov    dword ptr [eax + 1ch], eax
        add    dword ptr [eax + 1ch], 58h
        movzx  ecx, byte ptr [edx + 77h]
        shl    ecx, 3
        add    dword ptr [eax + 1ch], ecx
        mov    dword ptr [eax + 3ch], 0
        sub    eax, 64h
        mov    dword ptr [eax + 8], edx
        mov    dword ptr [eax + 20h], eax
        add    dword ptr [eax + 20h], 0bch
        shl    ecx, 1
        add    dword ptr [eax + 20h], ecx
        mov    ecx, dword ptr [eax + 20h]
        mov    dword ptr [ecx], 40h
        mov    bl, byte ptr [edx + 75h]
        mov    byte ptr [ecx + 5], bl
        mov    bl, byte ptr [edx + 7ch]
        mov    byte ptr [ecx + 6], bl
        mov    bl, byte ptr [edx + 7dh]
        mov    byte ptr [ecx + 7], bl
        mov    dword ptr [ecx + 20h], 0
        mov    dword ptr [ecx + 28h], ecx
        add    dword ptr [ecx + 28h], 50h
        mov    dword ptr [ecx + 40h], eax
        mov    dword ptr [ecx + 44h], 0
        pop    ebx
        ret
sub_3_03D3:
        mov    ax, 3430h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        cmp    eax, -1
        je     SHORT loc_3_03E6
        mov    eax, dword ptr [eax + 4]
        ret
loc_3_03E6:
        xor    eax, eax
        ret
sub_3_03E9:
        push   ebx
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        mov    ebx, dword ptr [ebx + 8]
        movzx  eax, word ptr [ebx + 38h]
        shl    eax, 10h
        add    ax, word ptr [ebx + 10h]
        pop    ebx
        ret
sub_3_0400:
        mov    eax, dword ptr [esp + 4]
        sub    eax, 64h
        push   eax
        db  0FFh, 015h, 014h, 000h, 000h, 000h
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_3_0417
        inc    eax
        ret
loc_3_0417:
        xor    eax, eax
        ret
sub_3_041A:
        mov    eax, dword ptr [esp + 4]
        mov    edx, eax
        shr    eax, 0ch
        and    edx, 0fffh
        add    edx, dword ptr [esp + 8]
        dec    edx
        shr    edx, 0ch
        inc    edx
        push   40000000h
        push   edx
        push   eax
        db  0CDh, 020h, 063h, 000h, 001h, 000h
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_3_0452
        mov    edx, dword ptr [esp + 4]
        and    edx, 0fffh
        add    eax, edx
loc_3_0452:
        ret
sub_3_0453:
        push   ebx
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        mov    eax, dword ptr [esp + 8]
        add    eax, dword ptr [ebx + 4]
        pop    ebx
        ret
sub_3_0463:
        push   edi
        push   esi
        mov    esi, dword ptr [esp + 0ch]
        mov    edi, esi
        sub    edi, 64h
        mov    edi, dword ptr [edi + 8]
        add    edi, 0c3h
        db  0CDh, 020h, 004h, 000h, 010h, 000h
        pop    esi
        pop    edi
        ret
sub_3_0480:
        mov    eax, dword ptr [esp + 4]
        mov    edx, eax
        shr    eax, 0ch
        and    edx, 0fffh
        add    edx, dword ptr [esp + 8]
        dec    edx
        shr    edx, 0ch
        inc    edx
        push   40000000h
        push   edx
        push   eax
        db  0CDh, 020h, 064h, 000h, 001h, 000h
        add    esp, 0ch
        ret
loc_3_04A9:
        or     edx, edx
        je     SHORT loc_3_0523
        push   dword ptr [edx + 94h]
        push   edx
        call   dword ptr [edx + 70h]
        pop    edx
        pop    ecx
        test   ecx, 8
        je     SHORT loc_3_051B
        mov    ebx, dword ptr [edx + 98h]
        mov    ebx, dword ptr [ebx + 20h]
        or     ebx, ebx
        jne    SHORT loc_3_04E0
        or     eax, eax
        je     NEAR PTR loc_3_05AF
        call   sub_3_05B0
        jmp    NEAR PTR loc_3_05AF
loc_3_04E0:
        push   eax
        push   edx
        mov    ebx, dword ptr [ebx]
        push   ebx
        mov    ebx, dword ptr [edx + 98h]
        mov    dword ptr [ebx + 1ch], 0
        mov    dword ptr [ebx + 20h], 0
        mov    dword ptr [ebx + 2ch], 0
        pop    ebx
        db  0CDh, 020h, 001h, 000h, 02Ah, 000h
        pop    edx
        pop    eax
        or     eax, eax
        je     NEAR PTR loc_3_05AF
        call   sub_3_05B0
        jmp    NEAR PTR loc_3_05AF
loc_3_051B:
        or     eax, eax
        je     NEAR PTR loc_3_05AF
loc_3_0523:
        push   edx
        sub    esp, 6ch
        push   edi
        lea    edi, [esp + 4]
        db  0CDh, 020h, 08Dh, 000h, 001h, 000h
        pop    edi
        test   byte ptr [edx + 94h], 4
        je     SHORT loc_3_0544
        db  0CDh, 020h, 083h, 000h, 001h, 000h
        jmp    SHORT loc_3_054A
loc_3_0544:
        db  0CDh, 020h, 082h, 000h, 001h, 000h
loc_3_054A:
        mov    ecx, dword ptr [edx + 20h]
        movzx  eax, word ptr [ecx + 26h]
        db  0CDh, 020h, 04Dh, 000h, 001h, 000h
        movzx  eax, word ptr [ecx + 24h]
        db  0CDh, 020h, 04Dh, 000h, 001h, 000h
        mov    eax, dword ptr [ebp + 30h]
        push   eax
        mov    eax, edx
        add    eax, 64h
        mov    edx, dword ptr [eax + 38h]
        mov    ecx, edx
        and    edx, 0ffffh
        shr    ecx, 10h
        db  0CDh, 020h, 048h, 000h, 001h, 000h
        db  0CDh, 020h, 085h, 000h, 001h, 000h
        pop    eax
        mov    dword ptr [ebp + 30h], eax
        db  0CDh, 020h, 04Eh, 000h, 001h, 000h
        db  0CDh, 020h, 04Eh, 000h, 001h, 000h
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        push   esi
        lea    esi, [esp + 4]
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        pop    esi
        add    esp, 6ch
        pop    edx
        call   sub_3_05B0
loc_3_05AF:
        ret
sub_3_05B0:
        push   edx
        call   dword ptr [edx + 70h]
        add    esp, 4
        ret
sub_3_05B8:
        sub    esp, 6ch
        push   edi
        lea    edi, [esp + 4]
        db  0CDh, 020h, 08Dh, 000h, 001h, 000h
        pop    edi
        or     eax, eax
        je     SHORT loc_3_05D3
        db  0CDh, 020h, 083h, 000h, 001h, 000h
        jmp    SHORT loc_3_05D9
loc_3_05D3:
        db  0CDh, 020h, 082h, 000h, 001h, 000h
loc_3_05D9:
        mov    eax, edx
        ror    eax, 10h
        db  0CDh, 020h, 04Dh, 000h, 001h, 000h
        ror    eax, 10h
        db  0CDh, 020h, 04Dh, 000h, 001h, 000h
        mov    eax, dword ptr [ebp + 30h]
        push   eax
        mov    dx, word ptr [ecx + 1ah]
        and    edx, 0ffffh
        mov    cx, word ptr [ecx + 1ch]
        and    ecx, 0ffffh
        db  0CDh, 020h, 048h, 000h, 001h, 000h
        db  0CDh, 020h, 085h, 000h, 001h, 000h
        pop    eax
        mov    dword ptr [ebp + 30h], eax
        db  0CDh, 020h, 04Eh, 000h, 001h, 000h
        db  0CDh, 020h, 04Eh, 000h, 001h, 000h
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        push   esi
        lea    esi, [esp + 4]
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        pop    esi
        add    esp, 6ch
        ret
sub_3_0637:
        push   ebx
        mov    eax, dword ptr [esp + 8]
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        mov    ecx, dword ptr [ebx + 8]
        push   dword ptr [ecx + 1ch]
        mov    dword ptr [ecx + 1ch], eax
        mov    ax, 1e1ch
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        pop    dword ptr [ecx + 1ch]
        pop    ebx
        ret
sub_3_065A:
        mov    ax, 3c14h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        ret
        db  033h, 0C0h, 066h, 0B8h, 00Ah, 003h, 0F8h, 0C3h, 000h, 000h, 000h
sub_3_0670:
        mov    edx, dword ptr [esp + 4]
        mov    cl, byte ptr [edx]
        cmp    cl, 7
        jb     SHORT loc_3_0681
        mov    eax, 60h
        ret
loc_3_0681:
        test   byte ptr [esp + 8], 8
        je     SHORT loc_3_06A7
        xor    eax, eax
        mov    al, cl
        db  08Ah, 080h, 028h, 000h, 000h, 000h
        and    eax, 0ffh
        cmp    cl, 2
        jne    SHORT loc_3_06DE
        xor    ecx, ecx
        mov    cl, byte ptr [edx + 14h]
        add    ecx, 2
        add    eax, ecx
        ret
loc_3_06A7:
        xor    eax, eax
        mov    al, cl
        db  08Ah, 080h, 020h, 000h, 000h, 000h
        and    eax, 0ffh
        or     cl, cl
        jne    SHORT loc_3_06CB
        cmp    word ptr [edx + 4], 0aa55h
        jne    SHORT loc_3_06CB
        xor    ecx, ecx
        mov    cx, word ptr [edx + 6]
        add    eax, ecx
        ret
loc_3_06CB:
        cmp    cl, 2
        jne    SHORT loc_3_06DE
        xor    ecx, ecx
        mov    cl, byte ptr [edx + 0eh]
        add    eax, ecx
        xor    ecx, ecx
        mov    cl, byte ptr [edx + 17h]
        add    eax, ecx
loc_3_06DE:
        ret
        db  090h
sub_3_06E0:
        push   esi
        mov    eax, dword ptr [esp + 8]
        or     eax, eax
        je     SHORT loc_3_06F7
        mov    esi, dword ptr [eax + 98h]
        mov    eax, dword ptr [eax + 94h]
        jmp    SHORT loc_3_06FF
loc_3_06F7:
        mov    esi, dword ptr [esp + 0ch]
        mov    eax, dword ptr [esp + 10h]
loc_3_06FF:
        push   eax
        push   esi
        call   sub_3_0670
        add    esp, 8
        push   eax
        push   esi
        call   sub_3_0480
        add    esp, 8
        pop    esi
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_3_0720:
        mov    ecx, dword ptr [esp + 4]
        add    ecx, 64h
        test   byte ptr [ecx + 30h], 2
        je     SHORT loc_3_073E
        mov    eax, dword ptr [esp + 8]
        push   eax
        mov    eax, dword ptr [ecx + 1ch]
        push   eax
        call   sub_3_0480
        add    esp, 8
loc_3_073E:
        ret
        db  090h
loc_3_0740:
        push   esi
        push   0
        mov    esi, dword ptr [esp + 0ch]
        push   0
        push   esi
        call   sub_3_06E0
        add    esp, 0ch
        push   esi
        call   sub_3_022F
        add    esp, 4
        pop    esi
        ret
        db  08Dh, 049h, 000h
loc_3_0760:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebp, dword ptr [esp + 1ch]
        mov    esi, dword ptr [ebp + 98h]
        add    ebp, 64h
        mov    eax, dword ptr [esp + 1ch]
        mov    eax, dword ptr [eax + 94h]
        mov    dword ptr [esp + 10h], eax
        mov    edi, eax
        and    edi, 8
        je     SHORT loc_3_0791
        mov    edx, dword ptr [esi + 18h]
        mov    ebx, dword ptr [esi + 0ch]
        jmp    SHORT loc_3_0797
loc_3_0791:
        mov    edx, dword ptr [esi + 1ah]
        mov    ebx, dword ptr [esi + 0ah]
loc_3_0797:
        mov    al, byte ptr [edx + 3]
        and    eax, 3fh
        mov    dword ptr [esp + 14h], ebx
        db  08Ah, 080h, 030h, 000h, 000h, 000h
        and    al, 3fh
        mov    cl, byte ptr [esi + 3]
        test   cl, 4
        je     SHORT loc_3_07C2
        or     ebx, ebx
        je     SHORT loc_3_07C2
        sub    ebx, dword ptr [edx + 10h]
        cmp    al, 12h
        jne    SHORT loc_3_07D1
        or     edi, edi
        jne    SHORT loc_3_07C2
        mov    al, 1
loc_3_07C2:
        cmp    al, 12h
        jne    SHORT loc_3_07D1
        test   cl, 18h
        je     SHORT loc_3_07CF
        or     edi, edi
        jne    SHORT loc_3_07D1
loc_3_07CF:
        mov    al, 1
loc_3_07D1:
        test   al, 10h
        je     SHORT loc_3_07E6
        mov    cl, al
        mov    al, 4
        cmp    cl, 12h
        jne    SHORT loc_3_07E8
        or     edi, edi
        je     SHORT loc_3_0810
        mov    al, 1
        jmp    SHORT loc_3_07E8
loc_3_07E6:
        sub    cl, cl
loc_3_07E8:
        or     edi, edi
        je     SHORT loc_3_0810
        mov    byte ptr [esi + 16h], cl
        mov    dword ptr [esi + 0ch], ebx
        mov    cl, byte ptr [edx + 4]
        mov    byte ptr [esi + 17h], cl
        mov    ebx, dword ptr [ebp + 38h]
        mov    dword ptr [esi + 18h], ebx
        test   byte ptr [esi + 3], 40h
        je     SHORT loc_3_0806
        jmp    SHORT loc_3_0822
loc_3_0806:
        mov    byte ptr [esi + 24h], al
        jmp    SHORT loc_3_0825
        db  005h, 000h, 000h, 000h, 000h
loc_3_0810:
        mov    byte ptr [esi + 18h], cl
        mov    dword ptr [esi + 0ah], ebx
        mov    cl, byte ptr [edx + 4]
        mov    byte ptr [esi + 19h], cl
        mov    ebx, dword ptr [ebp + 38h]
        mov    dword ptr [esi + 1ah], ebx
loc_3_0822:
        mov    byte ptr [esi + 1], al
loc_3_0825:
        mov    eax, dword ptr [esp + 14h]
        push   eax
        mov    eax, dword ptr [esp + 20h]
        push   eax
        call   sub_3_0720
        add    esp, 8
        test   byte ptr [esi + 3], 1
        je     SHORT loc_3_0841
        or     ebx, ebx
        jne    SHORT loc_3_0870
loc_3_0841:
        or     edi, edi
        jne    SHORT loc_3_0870
        mov    eax, dword ptr [esp + 1ch]
        push   eax
        call   sub_3_022F
        mov    eax, dword ptr [esp + 14h]
        add    esp, 4
        push   eax
        push   esi
        push   0
        call   sub_3_06E0
        add    esp, 0ch
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
        db  08Dh, 064h, 024h, 000h
loc_3_0870:
        db  0C7h, 045h, 00Ch, 040h, 007h, 000h, 000h
        mov    eax, 0ffffffffh
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_3_0890:
        sub    esp, 24h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebp, dword ptr [esp + 3ch]
        mov    dword ptr [esp + 24h], ebp
        mov    eax, dword ptr [esp + 40h]
        push   eax
        call   sub_3_02EC
        mov    dword ptr [esp + 20h], eax
        add    esp, 4
        mov    esi, eax
        mov    edi, dword ptr [eax + 20h]
        db  0C7h, 046h, 070h, 020h, 003h, 000h, 000h
        mov    ebx, dword ptr [esp + 38h]
        mov    dword ptr [esi + 98h], ebx
        add    esi, 64h
        mov    byte ptr [edi + 2], 0
        mov    dword ptr [edi + 0ch], 0
        mov    dword ptr [edi + 10h], 0
        cmp    ebp, 8
        jne    SHORT loc_3_0912
        mov    eax, dword ptr [ebx + 18h]
        mov    dword ptr [esi + 38h], eax
        mov    dword ptr [ebx + 18h], edi
        lea    eax, [ebx + 30h]
        mov    dword ptr [esp + 10h], eax
        mov    al, byte ptr [ebx + 15h]
        mov    byte ptr [edi + 0ah], al
        xor    eax, eax
        mov    al, byte ptr [ebx + 14h]
        mov    dword ptr [esp + 14h], eax
        lea    eax, [ebx + 40h]
        mov    dword ptr [esp + 20h], eax
        mov    eax, dword ptr [ebx + 0ch]
        mov    dword ptr [esp + 18h], eax
        mov    eax, dword ptr [ebx + 10h]
        jmp    SHORT loc_3_0957
loc_3_0912:
        lea    eax, [esi + 38h]
        mov    eax, dword ptr [ebx + 1ah]
        mov    dword ptr [esi + 38h], eax
        mov    dword ptr [ebx + 1ah], edi
        lea    eax, [ebx + 40h]
        mov    dword ptr [esp + 10h], eax
        lea    eax, [edi + 0ah]
        mov    al, byte ptr [ebx + 17h]
        mov    byte ptr [edi + 0ah], al
        xor    eax, eax
        mov    al, byte ptr [ebx + 0eh]
        mov    dword ptr [esp + 14h], eax
        xor    eax, eax
        mov    al, byte ptr [ebx + 17h]
        lea    eax, [ebx + eax + 40h]
        mov    dword ptr [esp + 20h], eax
        mov    eax, dword ptr [ebx + 0ah]
        mov    dword ptr [esp + 18h], eax
        mov    eax, dword ptr [ebx + 0fh]
        push   eax
        call   sub_3_0637
        add    esp, 4
loc_3_0957:
        mov    dword ptr [esp + 28h], eax
        mov    cl, byte ptr [ebx + 3]
        mov    al, cl
        and    al, 18h
        cmp    al, 18h
        je     SHORT loc_3_097C
        test   cl, 8
        je     SHORT loc_3_096F
        or     dword ptr [edi + 0ch], 40h
loc_3_096F:
        test   byte ptr [ebx + 3], 10h
        je     SHORT loc_3_097C
        or     dword ptr [edi + 0ch], 80h
loc_3_097C:
        sub    ecx, ecx
        cmp    byte ptr [edi + 0ah], cl
        jbe    SHORT loc_3_0998
loc_3_0983:
        mov    eax, dword ptr [esp + 10h]
        mov    al, byte ptr [ecx + eax]
        mov    byte ptr [edi + ecx + 30h], al
        inc    ecx
        xor    eax, eax
        mov    al, byte ptr [edi + 0ah]
        cmp    eax, ecx
        ja     SHORT loc_3_0983
loc_3_0998:
        mov    dword ptr [edi + 14h], 0
        cmp    dword ptr [esp + 14h], 0
        je     SHORT loc_3_09E2
        mov    al, byte ptr [esp + 14h]
        mov    byte ptr [edi + 0bh], al
        mov    eax, dword ptr [esp + 20h]
        mov    dword ptr [edi + 1ch], eax
        db  0A1h, 01Ch, 000h, 000h, 000h
        cmp    dword ptr [eax + 54h], 0
        je     SHORT loc_3_09E6
        cmp    dword ptr [esp + 14h], 200h
        jae    SHORT loc_3_09E2
        mov    eax, dword ptr [esp + 14h]
        push   eax
        call   sub_3_026D
        add    esp, 4
        mov    dword ptr [edi + 1ch], eax
        mov    eax, dword ptr [esp + 20h]
        mov    dword ptr [esi + 3ch], eax
        jmp    SHORT loc_3_09E6
loc_3_09E2:
        or     dword ptr [edi + 0ch], 20h
loc_3_09E6:
        cmp    dword ptr [esp + 18h], 0
        je     SHORT loc_3_0A36
        mov    eax, dword ptr [esp + 18h]
        push   eax
        mov    eax, dword ptr [esp + 2ch]
        push   eax
        call   sub_3_041A
        add    esp, 8
        mov    dword ptr [edi + 18h], eax
        mov    dword ptr [esi + 1ch], eax
        cmp    dword ptr [edi + 18h], 0
        je     SHORT loc_3_0A30
        mov    eax, ebp
        or     eax, 2
        mov    dword ptr [esp + 24h], eax
        mov    eax, dword ptr [esp + 18h]
        mov    dword ptr [edi + 10h], eax
        mov    eax, dword ptr [edi + 0ch]
        test   al, 0c0h
        jne    SHORT loc_3_0A3D
        or     eax, 0c0h
        mov    dword ptr [edi + 0ch], eax
        jmp    SHORT loc_3_0A3D
        db  08Dh, 064h, 024h, 000h
loc_3_0A30:
        mov    byte ptr [ebx + 1], 80h
        jmp    SHORT loc_3_0A3D
loc_3_0A36:
        and    dword ptr [edi + 0ch], 0ffffff3fh
loc_3_0A3D:
        and    ebp, 8
        cmp    ebp, 8
        jne    SHORT loc_3_0A50
        mov    dword ptr [edi + 24h], ebx
        jmp    SHORT loc_3_0A58
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_3_0A50:
        call   sub_3_03D3
        mov    dword ptr [edi + 24h], eax
loc_3_0A58:
        or     dword ptr [esi + 8], 200h
        mov    eax, dword ptr [edi + 10h]
        mov    dword ptr [esi + 18h], eax
        mov    eax, dword ptr [esp + 24h]
        mov    dword ptr [esi + 30h], eax
        cmp    byte ptr [ebx + 1], 80h
        jne    SHORT loc_3_0A94
        cmp    ebp, 8
        mov    eax, dword ptr [esi + 38h]
        jne    SHORT loc_3_0A80
        mov    dword ptr [ebx + 18h], eax
        jmp    SHORT loc_3_0A83
        db  090h
loc_3_0A80:
        mov    dword ptr [ebx + 1ah], eax
loc_3_0A83:
        mov    eax, dword ptr [edi + 10h]
        push   eax
        mov    eax, dword ptr [esp + 20h]
        push   eax
        call   sub_3_0720
        add    esp, 8
loc_3_0A94:
        mov    eax, esi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 24h
        ret
        db  08Bh, 0FFh
sub_3_0AA0:
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    dword ptr [esp + 18h], 8
        mov    edx, 8
        xor    eax, eax
        db  0A0h, 0BFh, 008h, 000h, 000h
        add    eax, 0
loc_3_0AC0:
        mov    esi, 1
        lea    ecx, [edx - 1]
        shl    esi, cl
        test   eax, esi
        jne    SHORT loc_3_0AD1
        dec    edx
        jne    SHORT loc_3_0AC0
loc_3_0AD1:
        mov    esi, dword ptr [esp + 20h]
        mov    dword ptr [esp + 14h], 0ffh
        mov    byte ptr [esi + 8], dl
        mov    byte ptr [esi + 9], 7
        mov    edi, 10h
        mov    dword ptr [esp + 10h], 0ffh
        sub    ebx, ebx
loc_3_0AF3:
        dec    edi
        sub    ebp, ebp
loc_3_0AF6:
        push   ebp
        push   edi
        xor    eax, eax
        mov    al, byte ptr [esi + 2]
        push   eax
        call   sub_3_02B0
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_3_0B12
        or     ebx, ebx
        jne    SHORT loc_3_0B33
        mov    ebx, eax
        jmp    SHORT loc_3_0B33
loc_3_0B12:
        cmp    dword ptr [esp + 14h], 0ffh
        jne    SHORT loc_3_0B20
        mov    dword ptr [esp + 14h], edi
loc_3_0B20:
        cmp    dword ptr [esp + 10h], 0ffh
        jne    SHORT loc_3_0B33
        cmp    edi, 7
        jg     SHORT loc_3_0B33
        mov    dword ptr [esp + 10h], edi
loc_3_0B33:
        inc    ebp
        cmp    ebp, 8
        jl     SHORT loc_3_0AF6
        or     edi, edi
        jne    SHORT loc_3_0AF3
        or     ebx, ebx
        jne    SHORT loc_3_0B4A
        mov    al, byte ptr [esp + 10h]
        jmp    NEAR PTR loc_3_0BE4
loc_3_0B4A:
        mov    ebp, dword ptr [ebx + 8]
        or     ebp, ebp
        je     NEAR PTR loc_3_0BE7
loc_3_0B55:
        cmp    byte ptr [ebp + 13h], 13h
        jb     SHORT loc_3_0B89
        mov    eax, dword ptr [ebp + 8]
        or     eax, eax
        je     SHORT loc_3_0B89
        mov    edi, dword ptr [eax + 14h]
        or     edi, edi
        je     SHORT loc_3_0B89
        lea    eax, [edi + 14h]
        push   eax
        xor    eax, eax
        mov    al, byte ptr [esi + 2]
        lea    eax, [eax + eax*4]
        db  08Dh, 004h, 045h, 08Ch, 000h, 000h, 000h
        push   eax
        call   sub_3_1FF0
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_3_0B92
loc_3_0B89:
        mov    ebp, dword ptr [ebp + 0ch]
        or     ebp, ebp
        jne    SHORT loc_3_0B55
        jmp    SHORT loc_3_0BE7
loc_3_0B92:
        movsx  eax, byte ptr [edi + 49h]
        inc    eax
        mov    dword ptr [esp + 18h], eax
        test   byte ptr [esp + 24h], 8
        je     SHORT loc_3_0BBB
        mov    dword ptr [esi + 2ah], 0
        mov    al, byte ptr [edi + 49h]
        inc    al
        mov    byte ptr [esi + 2dh], al
        mov    byte ptr [esi + 2ch], 2
        mov    eax, dword ptr [ebx + 50h]
        mov    dword ptr [esi + 2eh], eax
loc_3_0BBB:
        cmp    dword ptr [esp + 10h], 0ffh
        je     SHORT loc_3_0BD0
        mov    al, byte ptr [esp + 10h]
        jmp    SHORT loc_3_0BE4
        db  005h, 000h, 000h, 000h, 000h
loc_3_0BD0:
        cmp    byte ptr [edi + 49h], 7
        jle    SHORT loc_3_0BE7
        cmp    dword ptr [esp + 14h], 0ffh
        je     SHORT loc_3_0BE7
        mov    al, byte ptr [esp + 14h]
loc_3_0BE4:
        mov    byte ptr [esi + 9], al
loc_3_0BE7:
        sub    eax, eax
        mov    ecx, dword ptr [esp + 24h]
        and    ecx, 8
loc_3_0BF0:
        cmp    ecx, 8
        jne    SHORT loc_3_0C00
        db  08Ah, 090h, 010h, 000h, 000h, 000h
        jmp    SHORT loc_3_0C06
        db  08Dh, 049h, 000h
loc_3_0C00:
        db  08Ah, 090h, 000h, 000h, 000h, 000h
loc_3_0C06:
        mov    byte ptr [esi + eax + 0ah], dl
        xor    edx, edx
        mov    dl, byte ptr [esi + 2]
        inc    eax
        lea    edx, [edx + edx*4]
        db  08Ah, 094h, 050h, 08Bh, 000h, 000h, 000h
        mov    byte ptr [esi + eax + 19h], dl
        cmp    eax, 10h
        jl     SHORT loc_3_0BF0
        cmp    word ptr [esi + 4], 0aa55h
        jne    SHORT loc_3_0C48
        cmp    word ptr [esi + 6], 2
        jb     SHORT loc_3_0C48
        mov    byte ptr [esi + 4], 0aah
        mov    byte ptr [esi + 5], 55h
        mov    eax, dword ptr [esp + 18h]
        shr    eax, 2
        and    al, 4
        or     al, 2
        mov    byte ptr [esi + 3ah], al
loc_3_0C48:
        mov    byte ptr [esi + 1], 1
        mov    eax, dword ptr [esp + 24h]
        push   eax
        push   esi
        push   0
        call   sub_3_06E0
        add    esp, 0ch
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0ch
        ret
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_3_0C70:
        push   esi
        mov    esi, dword ptr [esp + 8]
        xor    eax, eax
        mov    al, byte ptr [esi + 9]
        push   eax
        xor    eax, eax
        mov    al, byte ptr [esi + 8]
        push   eax
        xor    eax, eax
        mov    al, byte ptr [esi + 2]
        push   eax
        call   sub_3_02B0
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_3_0CB7
        test   byte ptr [eax + 23h], 8
        jne    SHORT loc_3_0CB7
        mov    al, byte ptr [eax + 40h]
        mov    byte ptr [esi + 0ah], al
        mov    byte ptr [esi + 1], 1
        mov    eax, dword ptr [esp + 0ch]
        push   eax
        push   esi
        push   0
        call   sub_3_06E0
        add    esp, 0ch
        sub    eax, eax
        pop    esi
        ret
loc_3_0CB7:
        mov    byte ptr [esi + 1], 82h
        mov    eax, dword ptr [esp + 0ch]
        push   eax
        push   esi
        push   0
        call   sub_3_06E0
        add    esp, 0ch
        sub    eax, eax
        pop    esi
        ret
        db  090h
sub_3_0CD0:
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 10h]
        xor    eax, eax
        mov    al, byte ptr [esi + 9]
        push   eax
        xor    eax, eax
        mov    al, byte ptr [esi + 8]
        push   eax
        xor    eax, eax
        mov    al, byte ptr [esi + 2]
        push   eax
        call   sub_3_02B0
        add    esp, 0ch
        or     eax, eax
        je     NEAR PTR loc_3_0D90
        test   byte ptr [eax + 23h], 8
        jne    NEAR PTR loc_3_0D90
        mov    edi, dword ptr [esp + 14h]
        push   eax
        push   edi
        push   esi
        call   sub_3_0890
        add    esp, 0ch
        mov    ebx, eax
        or     ebx, ebx
        jne    SHORT loc_3_0D31
        mov    byte ptr [esi + 1], 82h
        push   edi
        push   esi
        push   0
        call   sub_3_06E0
        add    esp, 0ch
        mov    eax, 0fffffffdh
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_3_0D31:
        cmp    byte ptr [esi + 1], 80h
        je     SHORT loc_3_0D53
        push   ebx
        call   sub_3_0400
        add    esp, 4
        or     eax, eax
        je     SHORT loc_3_0D53
        push   ebx
        call   sub_3_0463
        add    esp, 4
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_3_0D53:
        test   edi, 8
        mov    eax, dword ptr [ebx + 38h]
        je     SHORT loc_3_0D63
        mov    dword ptr [esi + 18h], eax
        jmp    SHORT loc_3_0D66
loc_3_0D63:
        mov    dword ptr [esi + 1ah], eax
loc_3_0D66:
        mov    byte ptr [esi + 1], 0e6h
        push   edi
        push   esi
        push   0
        call   sub_3_06E0
        add    esp, 0ch
        mov    eax, dword ptr [ebx + 40h]
        push   eax
        call   sub_3_022F
        add    esp, 4
        mov    eax, 0fffffffdh
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  005h, 000h, 000h, 000h, 000h
loc_3_0D90:
        mov    edi, dword ptr [esp + 14h]
        test   edi, 8
        je     SHORT loc_3_0DA2
        mov    byte ptr [esi + 24h], 82h
        jmp    SHORT loc_3_0DA6
loc_3_0DA2:
        mov    byte ptr [esi + 1], 82h
loc_3_0DA6:
        push   edi
        push   esi
        push   0
        call   sub_3_06E0
        add    esp, 0ch
        mov    eax, 0fffffffdh
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  005h, 000h, 000h, 000h, 000h
sub_3_0DC0:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 18h]
        mov    eax, dword ptr [esi + 8]
        push   eax
        call   sub_3_0637
        mov    edi, eax
        add    esp, 4
        mov    eax, dword ptr [edi + 1ah]
        mov    dword ptr [esp + 10h], eax
        xor    eax, eax
        mov    al, byte ptr [edi + 9]
        push   eax
        xor    eax, eax
        mov    al, byte ptr [edi + 8]
        push   eax
        xor    eax, eax
        mov    al, byte ptr [edi + 2]
        push   eax
        call   sub_3_02B0
        add    esp, 0ch
        or     eax, eax
        jne    SHORT loc_3_0E20
        mov    byte ptr [esi + 1], 1
        mov    eax, dword ptr [esp + 1ch]
        push   eax
        push   esi
        push   0
        call   sub_3_06E0
        add    esp, 0ch
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
        db  08Dh, 064h, 024h, 000h
loc_3_0E20:
        test   byte ptr [eax + 23h], 8
        je     SHORT loc_3_0E44
        mov    byte ptr [esi + 1], 1
        mov    eax, dword ptr [esp + 1ch]
        push   eax
        push   esi
        push   0
        call   sub_3_06E0
        add    esp, 0ch
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
loc_3_0E44:
        push   eax
        call   sub_3_02EC
        mov    ebx, eax
        add    esp, 4
        mov    ebp, dword ptr [ebx + 20h]
        or     dword ptr [ebx + 6ch], 100h
        mov    dword ptr [ebx + 98h], esi
        lea    ecx, [ebx + 64h]
        mov    byte ptr [ebp + 2], 10h
        mov    eax, dword ptr [esp + 10h]
        mov    dword ptr [ebp + 20h], eax
        mov    dword ptr [ebp + 0ch], 0
        push   ecx
        call   sub_3_0463
        add    esp, 4
        mov    al, byte ptr [ebp + 3]
        and    eax, 3fh
        db  08Ah, 080h, 030h, 000h, 000h, 000h
        mov    byte ptr [edi + 1], al
        mov    byte ptr [esi + 1], 1
        mov    eax, dword ptr [esp + 1ch]
        push   eax
        push   esi
        push   0
        call   sub_3_06E0
        add    esp, 0ch
        push   ebx
        call   sub_3_022F
        add    esp, 4
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_3_0EC0:
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 10h]
        xor    eax, eax
        mov    al, byte ptr [esi + 9]
        push   eax
        xor    eax, eax
        mov    al, byte ptr [esi + 8]
        push   eax
        xor    eax, eax
        mov    al, byte ptr [esi + 2]
        push   eax
        call   sub_3_02B0
        add    esp, 0ch
        or     eax, eax
        je     NEAR PTR loc_3_0F70
        test   byte ptr [eax + 23h], 8
        je     SHORT loc_3_0F09
        mov    byte ptr [esi + 1], 82h
        mov    eax, dword ptr [esp + 14h]
        push   eax
        push   esi
        push   0
        call   sub_3_06E0
        add    esp, 0ch
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_3_0F09:
        push   eax
        call   sub_3_02EC
        add    esp, 4
        mov    edi, dword ptr [eax + 20h]
        db  0C7h, 040h, 070h, 020h, 003h, 000h, 000h
        mov    dword ptr [eax + 98h], esi
        mov    dword ptr [eax + 94h], 0
        lea    ebx, [eax + 64h]
        test   byte ptr [esp + 14h], 8
        je     SHORT loc_3_0F44
        mov    eax, dword ptr [esi + 18h]
        mov    dword ptr [ebx + 38h], eax
        mov    dword ptr [esi + 18h], edi
        mov    dword ptr [edi + 24h], esi
        jmp    SHORT loc_3_0F55
loc_3_0F44:
        mov    eax, dword ptr [esi + 1ah]
        mov    dword ptr [ebx + 38h], eax
        mov    dword ptr [esi + 1ah], edi
        call   sub_3_03D3
        mov    dword ptr [edi + 24h], eax
loc_3_0F55:
        mov    byte ptr [edi + 2], 13h
        mov    dword ptr [edi + 0ch], 20h
        push   ebx
        call   sub_3_0463
        add    esp, 4
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  090h
loc_3_0F70:
        mov    eax, dword ptr [esp + 14h]
        test   al, 8
        je     SHORT loc_3_0F80
        mov    byte ptr [esi + 24h], 82h
        jmp    SHORT loc_3_0F84
        db  08Bh, 0FFh
loc_3_0F80:
        mov    byte ptr [esi + 1], 82h
loc_3_0F84:
        push   eax
        push   esi
        push   0
        call   sub_3_06E0
        add    esp, 0ch
        mov    eax, 0fffffffdh
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_3_0FA0:
        push   esi
        mov    esi, dword ptr [esp + 8]
        xor    eax, eax
        mov    al, byte ptr [esi + 9]
        push   eax
        xor    eax, eax
        mov    al, byte ptr [esi + 8]
        push   eax
        xor    eax, eax
        mov    al, byte ptr [esi + 2]
        push   eax
        call   sub_3_02B0
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_3_1020
        test   byte ptr [eax + 23h], 8
        je     SHORT loc_3_0FE1
        mov    byte ptr [esi + 1], 82h
        mov    eax, dword ptr [esp + 0ch]
        push   eax
        push   esi
        push   0
        call   sub_3_06E0
        add    esp, 0ch
        sub    eax, eax
        pop    esi
        ret
loc_3_0FE1:
        mov    cl, byte ptr [eax + 15h]
        mov    byte ptr [esi + 0bh], cl
        mov    cl, byte ptr [eax + 0dfh]
        mov    byte ptr [esi + 0ch], cl
        mov    cl, byte ptr [eax + 0e7h]
        mov    byte ptr [esi + 0dh], cl
        test   byte ptr [eax + 20h], 1
        mov    byte ptr [esi + 0ah], 1
        jne    SHORT loc_3_1007
        mov    byte ptr [esi + 0ah], 0
loc_3_1007:
        mov    byte ptr [esi + 1], 1
        mov    eax, dword ptr [esp + 0ch]
        push   eax
        push   esi
        push   0
        call   sub_3_06E0
        add    esp, 0ch
        sub    eax, eax
        pop    esi
        ret
        db  090h
loc_3_1020:
        mov    byte ptr [esi + 1], 82h
        mov    eax, dword ptr [esp + 0ch]
        push   eax
        push   esi
        push   0
        call   sub_3_06E0
        add    esp, 0ch
        mov    eax, 0fffffffeh
        pop    esi
        ret
        db  005h, 000h, 000h, 000h, 000h
sub_3_1040:
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 10h]
        mov    esi, dword ptr [esp + 0ch]
        push   edi
        push   esi
        call   sub_3_0670
        add    esp, 8
        push   eax
        push   esi
        call   sub_3_041A
        add    esp, 8
        test   edi, 8
        jne    SHORT loc_3_1068
        mov    eax, esi
loc_3_1068:
        or     eax, eax
        je     NEAR PTR loc_3_1120
        mov    byte ptr [eax + 1], 0
        xor    edx, edx
        db  08Ah, 015h, 0BFh, 008h, 000h, 000h
        mov    esi, 1
        mov    cl, byte ptr [eax + 2]
        shl    esi, cl
        test   esi, edx
        je     SHORT loc_3_1104
        cmp    cl, 1fh
        ja     SHORT loc_3_1104
        xor    ecx, ecx
        mov    cl, byte ptr [eax]
        cmp    ecx, 6
        ja     SHORT loc_3_10A0
        db  0FFh, 024h, 08Dh, 028h, 011h, 000h, 000h
        db  090h
loc_3_10A0:
        mov    byte ptr [eax + 1], 80h
        push   edi
        push   eax
        push   0
        call   sub_3_06E0
        add    esp, 0ch
        sub    eax, eax
        pop    edi
        pop    esi
        ret
loc_3_10B5:
        push   edi
        push   eax
        call   sub_3_0AA0
        add    esp, 8
        pop    edi
        pop    esi
        ret
loc_3_10C2:
        push   edi
        push   eax
        call   sub_3_0C70
        add    esp, 8
        pop    edi
        pop    esi
        ret
        db  090h
loc_3_10D0:
        push   edi
        push   eax
        call   sub_3_0CD0
        add    esp, 8
        pop    edi
        pop    esi
        ret
loc_3_10DD:
        push   edi
        push   eax
        call   sub_3_0DC0
        add    esp, 8
        pop    edi
        pop    esi
        ret
loc_3_10EA:
        push   edi
        push   eax
        call   sub_3_0EC0
        add    esp, 8
        pop    edi
        pop    esi
        ret
loc_3_10F7:
        push   edi
        push   eax
        call   sub_3_0FA0
        add    esp, 8
        pop    edi
        pop    esi
        ret
loc_3_1104:
        mov    byte ptr [eax + 1], 81h
        push   edi
        push   eax
        push   0
        call   sub_3_06E0
        add    esp, 0ch
        mov    eax, 0fffffffeh
        pop    edi
        pop    esi
        ret
        db  08Dh, 064h, 024h, 000h
loc_3_1120:
        mov    eax, 0ffffffffh
        pop    edi
        pop    esi
        ret
        db  0B5h, 010h, 000h, 000h, 0C2h, 010h, 000h, 000h, 0D0h, 010h, 000h, 000h, 0DDh, 010h, 000h, 000h
        db  0EAh, 010h, 000h, 000h, 0A0h, 010h, 000h, 000h, 0F7h, 010h, 000h, 000h, 005h, 000h, 000h, 000h
        db  000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_3_1150:
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 10h]
        mov    edi, dword ptr [esi + 20h]
        mov    ebx, dword ptr [esi + 94h]
        add    esi, 64h
        test   bl, 1
        je     SHORT loc_3_117A
        xor    eax, eax
        mov    al, byte ptr [edi + 0bh]
        push   eax
        mov    eax, dword ptr [edi + 1ch]
        push   eax
        call   sub_3_0480
        add    esp, 8
loc_3_117A:
        test   bl, 2
        je     SHORT loc_3_11B9
        test   bl, 4
        je     SHORT loc_3_11A9
        sub    edi, edi
loc_3_1186:
        mov    eax, dword ptr [esi + 1ch]
        add    eax, edi
        mov    ebx, dword ptr [eax + 4]
        mov    eax, dword ptr [eax]
        or     ebx, ebx
        je     SHORT loc_3_11A5
        push   eax
        push   ebx
        call   sub_3_0480
        add    esp, 8
        add    edi, 8
        or     ebx, ebx
        jne    SHORT loc_3_1186
loc_3_11A5:
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_3_11A9:
        mov    eax, dword ptr [edi + 10h]
        push   eax
        mov    eax, dword ptr [esi + 1ch]
        push   eax
        call   sub_3_0480
        add    esp, 8
loc_3_11B9:
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Dh, 049h, 000h
loc_3_11C0:
        mov    eax, dword ptr [esp + 4]
        push   eax
        call   sub_3_022F
        add    esp, 4
        ret
        db  08Bh, 0FFh
loc_3_11D0:
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 10h]
        mov    esi, dword ptr [edi + 98h]
        lea    ebx, [edi + 64h]
        mov    ecx, dword ptr [esi + 1ch]
        push   edi
        mov    al, byte ptr [ecx + 3]
        mov    byte ptr [esi + 7], al
        mov    al, byte ptr [ecx + 4]
        mov    byte ptr [esi + 34h], al
        mov    eax, dword ptr [ebx + 38h]
        mov    dword ptr [esi + 1ch], eax
        call   sub_3_1150
        add    esp, 4
        test   byte ptr [esi + 0ch], 8
        jne    SHORT loc_3_122A
        cmp    dword ptr [esi + 1ch], 0
        je     SHORT loc_3_122A
        db  0C7h, 043h, 00Ch, 0C0h, 011h, 000h, 000h
        xor    eax, eax
        mov    ax, word ptr [esi + 4]
        push   eax
        push   esi
        call   sub_3_0480
        add    esp, 8
        mov    eax, 0ffffffffh
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_3_122A:
        push   edi
        call   sub_3_022F
        add    esp, 4
        xor    eax, eax
        mov    ax, word ptr [esi + 4]
        push   eax
        push   esi
        call   sub_3_0480
        add    esp, 8
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_3_1250:
        sub    esp, 1ch
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 38h]
        mov    eax, dword ptr [esp + 38h]
        add    esi, 64h
        mov    ebx, dword ptr [eax + 20h]
        mov    dword ptr [esp + 20h], 0
        mov    edi, dword ptr [esp + 30h]
        db  0C7h, 046h, 00Ch, 0D0h, 003h, 000h, 000h
        mov    eax, dword ptr [edi + 1ch]
        mov    dword ptr [esi + 38h], eax
        mov    dword ptr [esi + 34h], edi
        mov    dword ptr [edi + 1ch], ebx
        mov    byte ptr [ebx + 2], 0
        mov    dword ptr [ebx + 0ch], 0
        test   byte ptr [edi + 0ch], 40h
        je     SHORT loc_3_129C
        mov    dword ptr [ebx + 0ch], 40h
loc_3_129C:
        test   byte ptr [edi + 0ch], 80h
        je     SHORT loc_3_12A9
        or     dword ptr [ebx + 0ch], 80h
loc_3_12A9:
        test   byte ptr [edi + 0ch], 20h
        je     SHORT loc_3_12B3
        or     dword ptr [ebx + 0ch], 20h
loc_3_12B3:
        test   byte ptr [edi + 0ch], 2
        je     SHORT loc_3_12BD
        or     dword ptr [ebx + 0ch], 2
loc_3_12BD:
        test   byte ptr [edi + 0dh], 80h
        je     SHORT loc_3_12C7
        or     dword ptr [ebx + 0ch], 4
loc_3_12C7:
        test   byte ptr [edi + 0dh], 20h
        je     SHORT loc_3_12D1
        or     dword ptr [ebx + 0ch], 8
loc_3_12D1:
        test   byte ptr [edi + 0dh], 4
        je     SHORT loc_3_12DE
        or     dword ptr [ebx + 0ch], 100h
loc_3_12DE:
        mov    eax, dword ptr [edi + 0ch]
        test   al, 1
        je     SHORT loc_3_1350
        test   eax, 400000h
        mov    eax, dword ptr [edi + 3ch]
        push   eax
        je     SHORT loc_3_12F7
        call   sub_3_0453
        jmp    SHORT loc_3_12FC
loc_3_12F7:
        call   sub_3_0637
loc_3_12FC:
        mov    dword ptr [esp + 14h], eax
        add    esp, 4
        mov    eax, dword ptr [esp + 10h]
        push   0ch
        push   eax
        call   sub_3_041A
        add    esp, 8
        or     eax, eax
        je     SHORT loc_3_1346
        lea    edx, [ebx + 30h]
        mov    ecx, 3
        mov    eax, 4
loc_3_1323:
        mov    ebp, dword ptr [esp + 10h]
        mov    ebp, dword ptr [ebp]
        mov    dword ptr [edx], ebp
        add    dword ptr [esp + 10h], eax
        add    edx, eax
        dec    ecx
        jne    SHORT loc_3_1323
        mov    eax, dword ptr [esp + 10h]
        push   0ch
        push   eax
        call   sub_3_0480
        add    esp, 8
        jmp    SHORT loc_3_1368
loc_3_1346:
        mov    byte ptr [edi + 7], 6
        jmp    SHORT loc_3_1368
        db  08Dh, 064h, 024h, 000h
loc_3_1350:
        lea    edx, [edi + 3ch]
        lea    ecx, [ebx + 30h]
        mov    eax, 3
loc_3_135B:
        mov    ebp, dword ptr [edx]
        mov    dword ptr [ecx], ebp
        add    edx, 4
        add    ecx, 4
        dec    eax
        jne    SHORT loc_3_135B
loc_3_1368:
        mov    al, byte ptr [edi + 2dh]
        or     al, al
        jne    SHORT loc_3_1380
        mov    al, byte ptr [ebx + 30h]
        shr    al, 5
        and    eax, 0ffh
        db  08Ah, 080h, 074h, 000h, 000h, 000h
loc_3_1380:
        mov    byte ptr [ebx + 0ah], al
        mov    eax, dword ptr [edi + 48h]
        mov    dword ptr [ebx + 14h], eax
        cmp    byte ptr [edi + 2ch], 0
        je     SHORT loc_3_13E6
        mov    eax, dword ptr [edi + 28h]
        or     eax, eax
        je     SHORT loc_3_13E6
        test   byte ptr [edi + 0eh], 10h
        push   eax
        je     SHORT loc_3_13A4
        call   sub_3_0453
        jmp    SHORT loc_3_13A9
loc_3_13A4:
        call   sub_3_0637
loc_3_13A9:
        mov    dword ptr [esp + 14h], eax
        add    esp, 4
        xor    eax, eax
        mov    al, byte ptr [edi + 2ch]
        push   eax
        mov    eax, dword ptr [esp + 14h]
        push   eax
        call   sub_3_041A
        add    esp, 8
        or     eax, eax
        je     SHORT loc_3_13E0
        mov    dword ptr [esp + 20h], 1
        mov    al, byte ptr [edi + 2ch]
        mov    byte ptr [ebx + 0bh], al
        mov    eax, dword ptr [esp + 10h]
        mov    dword ptr [ebx + 1ch], eax
        jmp    SHORT loc_3_13EA
        db  08Bh, 0FFh
loc_3_13E0:
        mov    byte ptr [edi + 7], 6
        jmp    SHORT loc_3_13EA
loc_3_13E6:
        or     dword ptr [ebx + 0ch], 20h
loc_3_13EA:
        mov    dword ptr [ebx + 10h], 0
        cmp    dword ptr [edi + 24h], 0
        je     NEAR PTR loc_3_1558
        mov    eax, dword ptr [edi + 20h]
        or     eax, eax
        je     NEAR PTR loc_3_1558
        test   byte ptr [edi + 0eh], 20h
        push   eax
        je     SHORT loc_3_1414
        call   sub_3_0453
        jmp    SHORT loc_3_1419
loc_3_1414:
        call   sub_3_0637
loc_3_1419:
        mov    dword ptr [esp + 14h], eax
        add    esp, 4
        test   byte ptr [edi + 0ch], 10h
        je     SHORT loc_3_1435
        xor    eax, eax
        mov    ax, word ptr [edi + 2eh]
        lea    ebp, [eax*8]
        jmp    SHORT loc_3_143F
loc_3_1435:
        mov    eax, dword ptr [esp + 10h]
        mov    dword ptr [esi + 1ch], eax
        mov    ebp, dword ptr [edi + 24h]
loc_3_143F:
        mov    eax, dword ptr [esp + 10h]
        push   ebp
        push   eax
        call   sub_3_041A
        add    esp, 8
        or     eax, eax
        je     NEAR PTR loc_3_1554
        or     dword ptr [esp + 20h], 2
        test   byte ptr [edi + 0ch], 10h
        je     NEAR PTR loc_3_153D
        mov    eax, dword ptr [esp + 10h]
        mov    dword ptr [esp + 24h], eax
        mov    dword ptr [esp + 28h], ebp
        mov    dword ptr [esp + 1ch], 0
        cmp    word ptr [edi + 2eh], 0
        jbe    NEAR PTR loc_3_1509
        mov    dword ptr [esp + 14h], 0
        mov    ebp, eax
loc_3_1490:
        mov    eax, dword ptr [ebp]
        mov    dword ptr [esp + 10h], eax
        mov    eax, dword ptr [ebp + 4]
        mov    dword ptr [esp + 18h], eax
        test   byte ptr [edi + 0eh], 20h
        mov    eax, dword ptr [esp + 10h]
        push   eax
        je     SHORT loc_3_14B0
        call   sub_3_0453
        jmp    SHORT loc_3_14B5
loc_3_14B0:
        call   sub_3_0637
loc_3_14B5:
        mov    dword ptr [esp + 14h], eax
        mov    eax, dword ptr [esp + 1ch]
        add    esp, 4
        push   eax
        mov    eax, dword ptr [esp + 14h]
        push   eax
        call   sub_3_041A
        add    esp, 8
        or     eax, eax
        je     SHORT loc_3_1505
        mov    eax, dword ptr [esp + 10h]
        mov    edx, dword ptr [esi + 1ch]
        mov    ecx, dword ptr [esp + 14h]
        mov    dword ptr [edx + ecx + 4], eax
        add    ebp, 8
        mov    edx, dword ptr [esi + 1ch]
        mov    eax, dword ptr [esp + 18h]
        mov    dword ptr [edx + ecx], eax
        add    dword ptr [esp + 14h], 8
        xor    eax, eax
        inc    dword ptr [esp + 1ch]
        mov    ax, word ptr [edi + 2eh]
        cmp    eax, dword ptr [esp + 1ch]
        ja     SHORT loc_3_1490
        jmp    SHORT loc_3_1509
loc_3_1505:
        mov    byte ptr [edi + 7], 6
loc_3_1509:
        mov    eax, dword ptr [esi + 1ch]
        mov    ecx, dword ptr [esp + 1ch]
        mov    dword ptr [eax + ecx*8 + 4], 0
        mov    eax, dword ptr [esi + 1ch]
        or     dword ptr [esp + 20h], 4
        mov    dword ptr [eax + ecx*8], 0
        mov    eax, dword ptr [esp + 28h]
        push   eax
        mov    eax, dword ptr [esp + 28h]
        push   eax
        call   sub_3_0480
        add    esp, 8
        or     dword ptr [esi + 8], 2
loc_3_153D:
        mov    eax, dword ptr [edi + 24h]
        mov    dword ptr [ebx + 10h], eax
        mov    eax, dword ptr [ebx + 0ch]
        test   al, 0c0h
        jne    SHORT loc_3_1558
        or     eax, 0c0h
        mov    dword ptr [ebx + 0ch], eax
        jmp    SHORT loc_3_1558
loc_3_1554:
        mov    byte ptr [edi + 7], 6
loc_3_1558:
        call   sub_3_03E9
        mov    dword ptr [ebx + 24h], eax
        or     dword ptr [esi + 8], 200h
        mov    eax, dword ptr [ebx + 10h]
        mov    dword ptr [esi + 18h], eax
        mov    eax, dword ptr [esp + 20h]
        mov    dword ptr [esi + 30h], eax
        cmp    byte ptr [edi + 7], 6
        jne    SHORT loc_3_158D
        mov    eax, dword ptr [esi + 38h]
        mov    dword ptr [edi + 1ch], eax
        mov    eax, dword ptr [esp + 38h]
        push   eax
        call   sub_3_1150
        add    esp, 4
loc_3_158D:
        mov    eax, esi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 1ch
        ret
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_3_15A0:
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 14h]
        push   esi
        call   sub_3_02EC
        add    esp, 4
        mov    edi, eax
        push   edi
        mov    ebx, dword ptr [esp + 14h]
        push   esi
        push   ebx
        call   sub_3_1250
        add    esp, 0ch
        mov    esi, eax
        cmp    byte ptr [ebx + 7], 6
        je     SHORT loc_3_15E3
        push   esi
        call   sub_3_0400
        add    esp, 4
        or     eax, eax
        je     SHORT loc_3_15E3
        push   esi
        call   sub_3_0463
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_3_15E3:
        mov    byte ptr [ebx + 7], 6
        xor    eax, eax
        mov    ax, word ptr [ebx + 4]
        push   eax
        push   ebx
        call   sub_3_0480
        add    esp, 8
        push   edi
        call   sub_3_022F
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_3_1610:
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 10h]
        mov    eax, dword ptr [edi + 10h]
        push   eax
        call   sub_3_0637
        mov    ebx, eax
        mov    esi, dword ptr [esp + 18h]
        add    esp, 4
        mov    al, byte ptr [esi + 40h]
        mov    byte ptr [edi + 14h], al
        or     ebx, ebx
        je     SHORT loc_3_1669
        push   24h
        push   ebx
        call   sub_3_041A
        add    esp, 8
        or     eax, eax
        je     SHORT loc_3_1665
        sub    ecx, ecx
loc_3_1644:
        mov    al, byte ptr [ecx + esi + 82h]
        mov    byte ptr [ecx + ebx], al
        inc    ecx
        cmp    ecx, 24h
        jl     SHORT loc_3_1644
        push   24h
        push   ebx
        call   sub_3_0480
        add    esp, 8
        mov    byte ptr [edi + 7], 1
        jmp    SHORT loc_3_1669
loc_3_1665:
        mov    byte ptr [edi + 7], 6
loc_3_1669:
        xor    eax, eax
        mov    ax, word ptr [edi + 4]
        push   eax
        push   edi
        call   sub_3_0480
        add    esp, 8
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Dh, 049h, 000h
sub_3_1680:
        push   ebx
        mov    ebx, 8
        xor    eax, eax
        db  0A0h, 0BEh, 008h, 000h, 000h
        lea    ecx, [ecx]
loc_3_1690:
        mov    edx, 1
        lea    ecx, [ebx - 1]
        shl    edx, cl
        test   eax, edx
        jne    SHORT loc_3_16A1
        dec    ebx
        jne    SHORT loc_3_1690
loc_3_16A1:
        mov    edx, dword ptr [esp + 8]
        mov    byte ptr [edx + 2ch], bl
        cmp    byte ptr [edx + 9], 0ffh
        jne    SHORT loc_3_16B0
        jmp    SHORT loc_3_1702
loc_3_16B0:
        mov    byte ptr [edx + 10h], 30h
        mov    byte ptr [edx + 11h], 0
        mov    byte ptr [edx + 12h], 0
        mov    byte ptr [edx + 13h], 0
        sub    eax, eax
        mov    word ptr [edx + 14h], 0
        mov    dword ptr [edx + 24h], 0
        mov    dword ptr [edx + 28h], 0
        mov    byte ptr [edx + 2dh], 7
        lea    ebx, [ebx]
loc_3_16E0:
        lea    ecx, [edx + eax]
        db  08Ah, 098h, 054h, 000h, 000h, 000h
        mov    byte ptr [ecx + 30h], bl
        db  08Ah, 098h, 064h, 000h, 000h, 000h
        mov    byte ptr [ecx + 40h], bl
        inc    eax
        cmp    eax, 10h
        jl     SHORT loc_3_16E0
        mov    dword ptr [edx + 50h], 0
loc_3_1702:
        mov    byte ptr [edx + 7], 1
        xor    eax, eax
        mov    ax, word ptr [edx + 4]
        push   eax
        push   edx
        call   sub_3_0480
        add    esp, 8
        pop    ebx
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_3_1720:
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [esp + 14h]
        push   eax
        call   sub_3_02EC
        mov    edi, eax
        add    esp, 4
        mov    ebx, dword ptr [edi + 20h]
        or     dword ptr [edi + 6ch], 100h
        lea    eax, [edi + 64h]
        mov    esi, dword ptr [esp + 10h]
        mov    dword ptr [eax + 34h], esi
        mov    byte ptr [ebx + 2], 4
        mov    dword ptr [ebx + 0ch], 0
        push   eax
        call   sub_3_0463
        add    esp, 4
        mov    al, byte ptr [ebx + 3]
        mov    byte ptr [esi + 7], al
        xor    eax, eax
        mov    ax, word ptr [esi + 4]
        push   eax
        push   esi
        call   sub_3_0480
        add    esp, 8
        push   edi
        call   sub_3_022F
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Dh, 049h, 000h
sub_3_1780:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    eax, dword ptr [esp + 18h]
        push   eax
        call   sub_3_02EC
        mov    ebx, eax
        add    esp, 4
        mov    esi, dword ptr [ebx + 20h]
        mov    edi, dword ptr [esp + 14h]
        lea    ebp, [ebx + 64h]
        mov    eax, dword ptr [edi + 10h]
        push   eax
        call   sub_3_0637
        add    esp, 4
        mov    eax, dword ptr [eax + 1ch]
        or     dword ptr [ebp + 8], 100h
        mov    dword ptr [ebp + 34h], edi
        mov    byte ptr [esi + 2], 10h
        mov    dword ptr [esi + 20h], eax
        mov    dword ptr [esi + 0ch], 0
        push   ebp
        call   sub_3_0463
        add    esp, 4
        mov    al, byte ptr [esi + 3]
        mov    byte ptr [edi + 7], al
        xor    eax, eax
        mov    ax, word ptr [edi + 4]
        push   eax
        push   edi
        call   sub_3_0480
        add    esp, 8
        push   ebx
        call   sub_3_022F
        add    esp, 4
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_3_1800:
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [esp + 14h]
        push   eax
        call   sub_3_02EC
        mov    edi, eax
        add    esp, 4
        mov    ebx, dword ptr [edi + 20h]
        or     dword ptr [edi + 6ch], 100h
        lea    eax, [edi + 64h]
        mov    esi, dword ptr [esp + 10h]
        mov    dword ptr [eax + 34h], esi
        mov    byte ptr [ebx + 2], 12h
        mov    dword ptr [ebx + 0ch], 0
        push   eax
        call   sub_3_0463
        add    esp, 4
        mov    al, byte ptr [ebx + 3]
        mov    byte ptr [esi + 7], al
        xor    eax, eax
        mov    ax, word ptr [esi + 4]
        push   eax
        push   esi
        call   sub_3_0480
        add    esp, 8
        push   edi
        call   sub_3_022F
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Dh, 049h, 000h
sub_3_1860:
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [esp + 14h]
        push   eax
        call   sub_3_02EC
        mov    edi, eax
        add    esp, 4
        mov    ebx, dword ptr [edi + 20h]
        or     dword ptr [edi + 6ch], 100h
        lea    eax, [edi + 64h]
        mov    esi, dword ptr [esp + 10h]
        mov    dword ptr [eax + 34h], esi
        mov    byte ptr [ebx + 2], 13h
        mov    dword ptr [ebx + 0ch], 0
        push   eax
        call   sub_3_0463
        add    esp, 4
        mov    al, byte ptr [ebx + 3]
        mov    byte ptr [esi + 7], al
        xor    eax, eax
        mov    ax, word ptr [esi + 4]
        push   eax
        push   esi
        call   sub_3_0480
        add    esp, 8
        push   edi
        call   sub_3_022F
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Dh, 049h, 000h
sub_3_18C0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    edx, dword ptr [ebp + 8]
        xor    eax, eax
        mov    ax, word ptr [edx + 4]
        push   eax
        push   edx
        call   sub_3_041A
        add    esp, 8
        or     eax, eax
        je     NEAR PTR loc_3_1AB6
        mov    edx, dword ptr [ebp + 8]
        mov    byte ptr [edx + 7], 0
        cmp    byte ptr [edx + 6], 3
        je     NEAR PTR loc_3_19A0
        mov    cl, byte ptr [edx + 9]
        xor    eax, eax
        db  0A0h, 0BEh, 008h, 000h, 000h
        mov    edx, 1
        shl    edx, cl
        test   edx, eax
        mov    edx, dword ptr [ebp + 8]
        je     SHORT loc_3_1980
        xor    eax, eax
        mov    al, byte ptr [edx + 0bh]
        push   eax
        xor    eax, eax
        mov    al, byte ptr [edx + 0ah]
        push   eax
        xor    eax, eax
        mov    al, cl
        push   eax
        call   sub_3_02B8
        add    esp, 0ch
        or     eax, eax
        jne    SHORT loc_3_1950
        mov    edx, dword ptr [ebp + 8]
        xor    eax, eax
        mov    byte ptr [edx + 7], 8
        mov    ax, word ptr [edx + 4]
        push   eax
        push   edx
        call   sub_3_0480
        add    esp, 8
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_3_1950:
        test   byte ptr [eax + 23h], 8
        je     NEAR PTR loc_3_19E3
        int3
        mov    edx, dword ptr [ebp + 8]
        mov    byte ptr [edx + 7], 8
        xor    eax, eax
        mov    ax, word ptr [edx + 4]
        push   eax
        push   edx
        call   sub_3_0480
        add    esp, 8
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  005h, 000h, 000h, 000h, 000h
loc_3_1980:
        mov    byte ptr [edx + 7], 7
        xor    eax, eax
        mov    ax, word ptr [edx + 4]
        push   eax
        push   edx
        call   sub_3_0480
        add    esp, 8
        mov    eax, 0fffffffeh
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_3_19A0:
        mov    edx, dword ptr [ebp + 8]
        mov    cl, byte ptr [edx + 9]
        cmp    cl, 0ffh
        je     SHORT loc_3_19E0
        xor    eax, eax
        db  0A0h, 0BEh, 008h, 000h, 000h
        mov    edx, 1
        shl    edx, cl
        test   edx, eax
        jne    SHORT loc_3_19E0
        mov    edx, dword ptr [ebp + 8]
        xor    eax, eax
        mov    byte ptr [edx + 7], 7
        mov    ax, word ptr [edx + 4]
        push   eax
        push   edx
        call   sub_3_0480
        add    esp, 8
        mov    eax, 0fffffffeh
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_3_19E0:
        mov    eax, dword ptr [ebp - 4]
loc_3_19E3:
        mov    edx, dword ptr [ebp + 8]
        xor    ecx, ecx
        mov    cl, byte ptr [edx + 6]
        dec    ecx
        cmp    ecx, 11h
        ja     SHORT loc_3_19F8
        db  0FFh, 024h, 08Dh, 0C4h, 01Ah, 000h, 000h
loc_3_19F8:
        mov    edx, dword ptr [ebp + 8]
        xor    eax, eax
        mov    byte ptr [edx + 7], 16h
        mov    ax, word ptr [edx + 4]
        push   eax
        push   edx
        call   sub_3_0480
        add    esp, 8
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_3_1A18:
        push   eax
        mov    edx, dword ptr [ebp + 8]
        push   edx
        call   sub_3_15A0
        add    esp, 8
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  08Bh, 0FFh
loc_3_1A30:
        push   eax
        mov    edx, dword ptr [ebp + 8]
        push   edx
        call   sub_3_1610
        add    esp, 8
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_3_1A46:
        push   eax
        mov    edx, dword ptr [ebp + 8]
        push   edx
        call   sub_3_1680
        add    esp, 8
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_3_1A5C:
        push   eax
        mov    edx, dword ptr [ebp + 8]
        push   edx
        call   sub_3_1720
        add    esp, 8
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_3_1A72:
        push   eax
        mov    edx, dword ptr [ebp + 8]
        push   edx
        call   sub_3_1780
        add    esp, 8
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_3_1A88:
        push   eax
        mov    edx, dword ptr [ebp + 8]
        push   edx
        call   sub_3_1800
        add    esp, 8
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  08Bh, 0FFh
loc_3_1AA0:
        push   eax
        mov    edx, dword ptr [ebp + 8]
        push   edx
        call   sub_3_1860
        add    esp, 8
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_3_1AB6:
        mov    eax, 0ffffffffh
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  08Bh, 0FFh, 018h, 01Ah, 000h, 000h, 030h, 01Ah, 000h, 000h, 046h, 01Ah, 000h, 000h, 05Ch, 01Ah
        db  000h, 000h, 0F8h, 019h, 000h, 000h, 0F8h, 019h, 000h, 000h, 0F8h, 019h, 000h, 000h, 0F8h, 019h
        db  000h, 000h, 0F8h, 019h, 000h, 000h, 0F8h, 019h, 000h, 000h, 0F8h, 019h, 000h, 000h, 0F8h, 019h
        db  000h, 000h, 0F8h, 019h, 000h, 000h, 0F8h, 019h, 000h, 000h, 0F8h, 019h, 000h, 000h, 072h, 01Ah
        db  000h, 000h, 088h, 01Ah, 000h, 000h, 0A0h, 01Ah, 000h, 000h, 08Dh, 064h, 024h, 000h
sub_3_1B10:
        push   esi
        push   edi
        xor    eax, eax
        mov    al, byte ptr [esp + 0ch]
        cmp    eax, 1
        je     SHORT loc_3_1B25
        cmp    eax, 3
        je     SHORT loc_3_1B42
        pop    edi
        pop    esi
        ret
loc_3_1B25:
        db  0BEh, 0BCh, 000h, 000h, 000h
        db  0FEh, 005h, 0BDh, 008h, 000h, 000h
        mov    edi, dword ptr [esp + 10h]
        mov    al, 1
        mov    cl, byte ptr [edi]
        shl    al, cl
        db  008h, 005h, 0BFh, 008h, 000h, 000h
        jmp    SHORT loc_3_1B5D
loc_3_1B42:
        db  0BEh, 0BCh, 004h, 000h, 000h
        db  0FEh, 005h, 0BCh, 008h, 000h, 000h
        mov    edi, dword ptr [esp + 10h]
        mov    al, 1
        mov    cl, byte ptr [edi]
        shl    al, cl
        db  008h, 005h, 0BEh, 008h, 000h, 000h
loc_3_1B5D:
        db  0A1h, 080h, 000h, 000h, 000h
        db  039h, 005h, 089h, 000h, 000h, 000h
        jne    SHORT loc_3_1B8C
        push   eax
        call   sub_3_1BB0
        mov    ecx, dword ptr [esp + 18h]
        add    esp, 4
        db  089h, 088h, 009h, 000h, 000h, 000h
        db  039h, 005h, 080h, 000h, 000h, 000h
        jne    SHORT loc_3_1B8C
        db  083h, 005h, 089h, 000h, 000h, 000h, 004h
loc_3_1B8C:
        xor    edx, edx
        mov    dl, byte ptr [edi + 1]
        xor    ecx, ecx
        mov    cl, byte ptr [edi]
        shl    ecx, 4
        add    edx, ecx
        xor    ecx, ecx
        mov    cl, byte ptr [edi + 2]
        pop    edi
        lea    edx, [ecx + edx*8]
        mov    byte ptr [esi + edx], al
        pop    esi
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_3_1BB0:
        sub    eax, eax
        mov    ecx, dword ptr [esp + 4]
        cmp    ecx, eax
        jle    SHORT loc_3_1BCD
        db  0BAh, 009h, 000h, 000h, 000h
        nop
loc_3_1BC0:
        cmp    dword ptr [edx + eax], 0
        je     SHORT loc_3_1BCD
        add    eax, 4
        cmp    eax, ecx
        jl     SHORT loc_3_1BC0
loc_3_1BCD:
        ret
        db  08Bh, 0FFh
sub_3_1BD0:
        mov    eax, dword ptr [esp + 4]
        cmp    byte ptr [eax], 0
        je     SHORT loc_3_1BF9
        mov    edx, dword ptr [esp + 8]
        cmp    byte ptr [edx], 0
        je     SHORT loc_3_1BF9
        mov    ecx, dword ptr [eax]
        cmp    dword ptr [edx], ecx
        jne    SHORT loc_3_1BF6
        mov    eax, dword ptr [eax + 4]
        cmp    dword ptr [edx + 4], eax
        jne    SHORT loc_3_1BF6
        mov    eax, 1
        ret
loc_3_1BF6:
        sub    eax, eax
        ret
loc_3_1BF9:
        mov    eax, 0ffffffffh
        ret
        db  090h
sub_3_1C00:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    eax, dword ptr [esp + 18h]
        mov    dword ptr [esp + 10h], 0
        mov    edi, dword ptr [eax + 5]
        or     edi, edi
        je     NEAR PTR loc_3_1CC5
        mov    ebp, dword ptr [esp + 1ch]
        mov    esi, dword ptr [esp + 20h]
loc_3_1C26:
        or     ebp, ebp
        je     SHORT loc_3_1C35
        mov    eax, dword ptr [ebp]
        cmp    dword ptr [edi], eax
        jne    NEAR PTR loc_3_1CBA
loc_3_1C35:
        cmp    byte ptr [edi + 4], 0
        jne    SHORT loc_3_1C90
        lea    eax, [esi + 0a6h]
        push   eax
        mov    eax, dword ptr [esp + 1ch]
        add    eax, 15h
        push   eax
        call   sub_3_1BD0
        add    esp, 8
        or     eax, eax
        je     SHORT loc_3_1CBA
        mov    al, byte ptr [edi + 1]
        cmp    byte ptr [esi + 7ch], al
        jne    SHORT loc_3_1CBA
        mov    al, byte ptr [esi + 7dh]
        cmp    byte ptr [edi + 2], al
        jne    SHORT loc_3_1CBA
        xor    eax, eax
        mov    ax, word ptr [edi + 0ch]
        db  03Bh, 005h, 084h, 000h, 000h, 000h
        jne    SHORT loc_3_1CBA
        mov    al, byte ptr [edi + 0fh]
        cmp    byte ptr [esi + 82h], al
        jne    SHORT loc_3_1CBA
        mov    al, byte ptr [edi + 0eh]
        cmp    byte ptr [esi + 7eh], al
        je     SHORT loc_3_1CD8
        mov    dword ptr [esp + 10h], edi
        jmp    SHORT loc_3_1CBA
        db  08Dh, 049h, 000h
loc_3_1C90:
        db  08Bh, 01Dh, 07Ch, 000h, 000h, 000h
        or     ebx, ebx
        je     SHORT loc_3_1CBA
loc_3_1C9A:
        mov    al, byte ptr [edi + 4]
        cmp    byte ptr [ebx], al
        jne    SHORT loc_3_1CB3
        push   esi
        lea    eax, [edi + 8]
        push   eax
        push   ebx
        call   sub_3_1C00
        add    esp, 0ch
        or     eax, eax
        jne    SHORT loc_3_1CE2
loc_3_1CB3:
        mov    ebx, dword ptr [ebx + 1]
        or     ebx, ebx
        jne    SHORT loc_3_1C9A
loc_3_1CBA:
        mov    edi, dword ptr [edi + 10h]
        or     edi, edi
        jne    NEAR PTR loc_3_1C26
loc_3_1CC5:
        cmp    dword ptr [esp + 10h], 0
        je     SHORT loc_3_1CFD
        mov    eax, dword ptr [esp + 10h]
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
loc_3_1CD8:
        mov    eax, edi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
loc_3_1CE2:
        push   esi
        push   eax
        mov    al, byte ptr [ebx]
        push   eax
        call   sub_3_1B10
        add    esp, 0ch
        or     dword ptr [ebx + 11h], 1
        mov    eax, edi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
loc_3_1CFD:
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_3_1D10:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 14h]
        db  0A1h, 089h, 000h, 000h, 000h
        lea    edx, [esi + 82h]
        db  0A3h, 080h, 000h, 000h, 000h
        mov    edi, dword ptr [esp + 18h]
        db  039h, 03Dh, 07Ch, 000h, 000h, 000h
        je     SHORT loc_3_1D3C
        db  089h, 03Dh, 07Ch, 000h, 000h, 000h
loc_3_1D3C:
        db  0C7h, 005h, 084h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        mov    eax, 24h
loc_3_1D4B:
        xor    ecx, ecx
        mov    cl, byte ptr [edx]
        inc    edx
        db  001h, 00Dh, 084h, 000h, 000h, 000h
        dec    eax
        jne    SHORT loc_3_1D4B
        mov    ebx, 1
        or     edi, edi
        je     SHORT loc_3_1D8C
loc_3_1D62:
        push   esi
        push   0
        push   edi
        call   sub_3_1C00
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_3_1D85
        or     dword ptr [edi + 11h], 1
        push   esi
        push   eax
        mov    al, byte ptr [edi]
        sub    ebx, ebx
        push   eax
        call   sub_3_1B10
        add    esp, 0ch
loc_3_1D85:
        mov    edi, dword ptr [edi + 1]
        or     edi, edi
        jne    SHORT loc_3_1D62
loc_3_1D8C:
        push   esi
        call   sub_3_1F20
        add    esp, 4
        or     ebx, ebx
        je     SHORT loc_3_1DC0
        mov    byte ptr [esp + 0ch], al
        mov    al, byte ptr [esi + 7ch]
        mov    byte ptr [esp + 0dh], al
        mov    al, byte ptr [esi + 7dh]
        mov    byte ptr [esp + 0eh], al
        mov    byte ptr [esp + 0fh], 0
        lea    eax, [esp + 0ch]
        push   esi
        push   eax
        push   1
        call   sub_3_1B10
        add    esp, 0ch
loc_3_1DC0:
        call   sub_3_1EB0
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
        db  08Dh, 064h, 024h, 000h
sub_3_1DD0:
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 0ch]
        db  068h, 0BCh, 000h, 000h, 000h
        push   esi
        call   sub_3_1E30
        add    esp, 8
        mov    bl, al
        db  068h, 0BCh, 004h, 000h, 000h
        push   esi
        call   sub_3_1E30
        add    esp, 8
        cmp    bl, 0ffh
        je     SHORT loc_3_1DFF
        db  0FEh, 00Dh, 0BDh, 008h, 000h, 000h
loc_3_1DFF:
        cmp    al, 0ffh
        je     SHORT loc_3_1E09
        db  0FEh, 00Dh, 0BCh, 008h, 000h, 000h
loc_3_1E09:
        cmp    bl, 0ffh
        jne    SHORT loc_3_1E10
        mov    bl, al
loc_3_1E10:
        xor    eax, eax
        mov    al, bl
        pop    esi
        db  0C7h, 080h, 009h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        pop    ebx
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_3_1E30:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    dl, 0ffh
        sub    esi, esi
        mov    eax, dword ptr [esp + 14h]
        mov    ecx, dword ptr [esp + 18h]
loc_3_1E40:
        lea    edi, [ecx + esi]
        mov    bl, byte ptr [edi]
        cmp    bl, 0ffh
        je     SHORT loc_3_1E5F
        mov    ebp, ebx
        and    ebp, 0ffh
        db  039h, 085h, 009h, 000h, 000h, 000h
        jne    SHORT loc_3_1E5F
        mov    dl, bl
        mov    byte ptr [edi], 0ffh
loc_3_1E5F:
        inc    esi
        cmp    esi, 400h
        jl     SHORT loc_3_1E40
        xor    eax, eax
        mov    al, dl
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_3_1E80:
        push   esi
        db  08Bh, 035h, 07Ch, 000h, 000h, 000h
        or     esi, esi
        je     SHORT loc_3_1EAC
loc_3_1E8B:
        cmp    byte ptr [esi], 1
        jne    SHORT loc_3_1EA5
        mov    eax, dword ptr [esi + 11h]
        and    al, 11h
        cmp    al, 11h
        jne    SHORT loc_3_1EA5
        mov    eax, dword ptr [esi + 9]
        push   eax
        call   sub_3_0021
        add    esp, 4
loc_3_1EA5:
        mov    esi, dword ptr [esi + 1]
        or     esi, esi
        jne    SHORT loc_3_1E8B
loc_3_1EAC:
        sub    eax, eax
        pop    esi
        ret
sub_3_1EB0:
        push   esi
        push   edi
        db  08Bh, 03Dh, 07Ch, 000h, 000h, 000h
        or     edi, edi
        je     SHORT loc_3_1EF4
        db  0BEh, 0C0h, 008h, 000h, 000h
loc_3_1EC1:
        cmp    byte ptr [edi], 1
        jne    SHORT loc_3_1EED
        mov    eax, dword ptr [edi + 11h]
        and    al, 11h
        cmp    al, 1
        jne    SHORT loc_3_1EDF
        mov    eax, dword ptr [edi + 9]
        push   eax
        call   0
        add    esp, 4
        or     dword ptr [edi + 11h], 10h
loc_3_1EDF:
        mov    eax, dword ptr [edi + 9]
        mov    dword ptr [esi], eax
        mov    eax, dword ptr [edi + 0dh]
        mov    dword ptr [esi + 4], eax
        add    esi, 8
loc_3_1EED:
        mov    edi, dword ptr [edi + 1]
        or     edi, edi
        jne    SHORT loc_3_1EC1
loc_3_1EF4:
        sub    eax, eax
        pop    edi
        pop    esi
        ret
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Bh, 044h, 024h, 004h, 00Bh, 0C0h, 074h, 00Ch, 080h
        db  038h, 001h, 074h, 00Dh, 08Bh, 040h, 001h, 00Bh, 0C0h, 075h, 0F4h, 0B8h, 001h, 000h, 000h, 000h
        db  0C3h, 02Bh, 0C0h, 0C3h, 08Dh, 049h, 000h
sub_3_1F20:
        push   ebx
        push   esi
        push   edi
        push   ebp
        sub    edi, edi
        db  039h, 03Dh, 088h, 000h, 000h, 000h
        jle    SHORT loc_3_1F80
        mov    esi, dword ptr [esp + 14h]
        db  0BDh, 08Ch, 000h, 000h, 000h
        lea    ebx, [esi + 0a6h]
loc_3_1F3D:
        push   ebx
        push   ebp
        call   sub_3_1FD0
        add    esp, 8
        or     eax, eax
        je     SHORT loc_3_1F65
        xor    ecx, ecx
        mov    cl, byte ptr [esi + 7eh]
        movsx  eax, byte ptr [ebp + 8]
        cmp    eax, ecx
        jne    SHORT loc_3_1F65
        xor    ecx, ecx
        mov    cl, byte ptr [esi + 75h]
        movsx  eax, byte ptr [ebp + 9]
        cmp    eax, ecx
        je     SHORT loc_3_1F73
loc_3_1F65:
        add    ebp, 0ah
        inc    edi
        db  03Bh, 03Dh, 088h, 000h, 000h, 000h
        jl     SHORT loc_3_1F3D
        jmp    SHORT loc_3_1F84
loc_3_1F73:
        mov    eax, edi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_3_1F80:
        mov    esi, dword ptr [esp + 14h]
loc_3_1F84:
        db  0FFh, 005h, 088h, 000h, 000h, 000h
        lea    eax, [edi + edi*4]
        lea    ebx, [eax*2]
        lea    eax, [esi + 0a6h]
        push   eax
        db  08Dh, 083h, 08Ch, 000h, 000h, 000h
        push   eax
        call   sub_3_2030
        add    esp, 8
        mov    al, byte ptr [esi + 7eh]
        db  088h, 083h, 094h, 000h, 000h, 000h
        mov    al, byte ptr [esi + 75h]
        db  088h, 083h, 095h, 000h, 000h, 000h
        mov    eax, edi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_3_1FD0:
        mov    eax, dword ptr [esp + 4]
        mov    edx, dword ptr [esp + 8]
        mov    ecx, dword ptr [eax]
        cmp    dword ptr [edx], ecx
        jne    SHORT loc_3_1FEC
        mov    eax, dword ptr [eax + 4]
        cmp    dword ptr [edx + 4], eax
        jne    SHORT loc_3_1FEC
        mov    eax, 1
        ret
loc_3_1FEC:
        sub    eax, eax
        ret
        db  090h
sub_3_1FF0:
        push   ebx
        sub    edx, edx
        mov    eax, dword ptr [esp + 8]
        mov    ecx, dword ptr [esp + 0ch]
loc_3_1FFB:
        mov    bl, byte ptr [edx + eax]
        or     bl, bl
        jne    SHORT loc_3_2006
        or     edx, edx
        jg     SHORT loc_3_2018
loc_3_2006:
        cmp    byte ptr [edx + ecx], bl
        jne    SHORT loc_3_2020
        inc    edx
        cmp    edx, 8
        jl     SHORT loc_3_1FFB
        mov    eax, 1
        pop    ebx
        ret
loc_3_2018:
        mov    eax, 1
        pop    ebx
        ret
        db  090h
loc_3_2020:
        sub    eax, eax
        pop    ebx
        ret
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_3_2030:
        mov    eax, dword ptr [esp + 8]
        mov    edx, dword ptr [esp + 4]
        mov    ecx, dword ptr [eax]
        mov    dword ptr [edx], ecx
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [edx + 4], eax
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_3_2050:
        db  0C7h, 005h, 044h, 004h, 000h, 000h, 070h, 022h, 000h, 000h
        db  0C7h, 005h, 048h, 004h, 000h, 000h, 070h, 022h, 000h, 000h
        db  0C7h, 005h, 04Ch, 004h, 000h, 000h, 070h, 022h, 000h, 000h
        db  0C7h, 005h, 050h, 004h, 000h, 000h, 070h, 022h, 000h, 000h
        db  0C7h, 005h, 054h, 004h, 000h, 000h, 070h, 022h, 000h, 000h
        db  0C7h, 005h, 058h, 004h, 000h, 000h, 0B0h, 021h, 000h, 000h
        db  0C7h, 005h, 05Ch, 004h, 000h, 000h, 070h, 022h, 000h, 000h
        db  0C7h, 005h, 060h, 004h, 000h, 000h, 070h, 022h, 000h, 000h
        db  0C7h, 005h, 064h, 004h, 000h, 000h, 070h, 022h, 000h, 000h
        db  0C7h, 005h, 068h, 004h, 000h, 000h, 070h, 022h, 000h, 000h
        db  0C7h, 005h, 06Ch, 004h, 000h, 000h, 070h, 022h, 000h, 000h
        db  0C7h, 005h, 070h, 004h, 000h, 000h, 000h, 022h, 000h, 000h
        db  0C7h, 005h, 074h, 004h, 000h, 000h, 070h, 022h, 000h, 000h
        db  0C7h, 005h, 078h, 004h, 000h, 000h, 070h, 022h, 000h, 000h
        db  0C7h, 005h, 07Ch, 004h, 000h, 000h, 070h, 022h, 000h, 000h
        db  0C7h, 005h, 0E0h, 000h, 000h, 000h, 070h, 022h, 000h, 000h
        db  0C7h, 005h, 0E4h, 000h, 000h, 000h, 050h, 022h, 000h, 000h
        db  0C7h, 005h, 0E8h, 000h, 000h, 000h, 070h, 022h, 000h, 000h
        db  0C7h, 005h, 0ECh, 000h, 000h, 000h, 070h, 022h, 000h, 000h
        db  0C7h, 005h, 0F0h, 000h, 000h, 000h, 070h, 022h, 000h, 000h
        db  0C7h, 005h, 0F4h, 000h, 000h, 000h, 070h, 022h, 000h, 000h
        db  0C7h, 005h, 0F8h, 000h, 000h, 000h, 070h, 022h, 000h, 000h
        db  0C7h, 005h, 0FCh, 000h, 000h, 000h, 070h, 022h, 000h, 000h
        db  0C7h, 005h, 00Ch, 001h, 000h, 000h, 0FFh, 0FFh, 0FFh, 0FFh
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_3_2150:
        db  083h, 03Dh, 00Ch, 001h, 000h, 000h, 000h
        jne    SHORT loc_3_215E
        call   sub_3_2050
loc_3_215E:
        mov    ecx, dword ptr [esp + 4]
        xor    eax, eax
        mov    al, byte ptr [ecx + 4]
        or     eax, eax
        je     SHORT loc_3_2177
        lea    eax, [ecx + 1ah]
        mov    word ptr [eax], 1
        movsx  eax, word ptr [eax]
        ret
loc_3_2177:
        mov    eax, dword ptr [ecx + 10h]
        db  0A3h, 000h, 001h, 000h, 000h
        mov    eax, dword ptr [ecx + 28h]
        db  0A3h, 004h, 001h, 000h, 000h
        mov    eax, dword ptr [ecx + 20h]
        db  0A3h, 008h, 001h, 000h, 000h
        lea    eax, [ecx + 1ah]
        mov    dword ptr [ecx + 1ch], 0f090a0bh
        mov    word ptr [eax], 0
        db  0C7h, 041h, 010h, 040h, 004h, 000h, 000h
        mov    word ptr [ecx + 6], 0
        movsx  eax, word ptr [eax]
        ret
        db  090h
loc_3_21B0:
        mov    eax, dword ptr [esp + 4]
        mov    ecx, dword ptr [eax + 10h]
        db  0C7h, 001h, 070h, 022h, 000h, 000h
        db  0C7h, 041h, 004h, 070h, 022h, 000h, 000h
        db  0C7h, 041h, 008h, 0DCh, 000h, 000h, 000h
        mov    dword ptr [eax + 20h], 0f090a0bh
        mov    word ptr [eax + 18h], 1
        mov    word ptr [eax + 28h], 0
        mov    word ptr [eax + 2ah], 0
        mov    dword ptr [eax + 24h], 0
        mov    dword ptr [eax], 0
        mov    word ptr [eax + 1ah], 0
        sub    eax, eax
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_3_2200:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0ch]
        mov    edi, dword ptr [esi + 28h]
        cmp    byte ptr [esi + 4], 2
        jne    SHORT loc_3_2237
        cmp    word ptr [edi + 18h], 4
        jb     SHORT loc_3_2237
        call   sub_3_065A
        db  08Bh, 00Dh, 000h, 009h, 000h, 000h
        mov    dword ptr [eax], ecx
        add    esi, 1ah
        and    word ptr [edi + 2ch], 0fffeh
        mov    word ptr [esi], 0
        movsx  eax, word ptr [esi]
        pop    edi
        pop    esi
        ret
loc_3_2237:
        add    esi, 1ah
        mov    word ptr [esi], 1
        or     word ptr [edi + 2ch], 1
        movsx  eax, word ptr [esi]
        pop    edi
        pop    esi
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_3_2250:
        mov    eax, dword ptr [esp + 4]
        mov    word ptr [eax + 1ah], 0
        mov    dword ptr [eax + 24h], 0
        sub    eax, eax
        ret
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_3_2270:
        mov    eax, dword ptr [esp + 4]
        mov    word ptr [eax + 1ah], 1
        mov    eax, 1
        ret
_PTEXT ends
        end
