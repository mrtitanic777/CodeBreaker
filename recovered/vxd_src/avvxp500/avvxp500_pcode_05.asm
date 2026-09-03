; avvxp500_pcode_05.asm -- AVVXP500 object 5 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

sub_5_0000:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 0d8h
        test   eax, eax
        je     NEAR PTR loc_5_012D
        cmp    eax, 1
        je     SHORT loc_5_003F
        cmp    eax, 2
        je     NEAR PTR loc_5_00CC
        cmp    eax, 3
        je     NEAR PTR loc_5_00D9
        cmp    eax, 4
        je     NEAR PTR loc_5_0122
        mov    eax, 1
        jmp    NEAR PTR loc_5_012F
loc_5_003F:
        cmp    dword ptr [ebp + 0ch], 1
        jbe    SHORT loc_5_004F
        mov    eax, 1
        jmp    NEAR PTR loc_5_012F
loc_5_004F:
        push   0
        lea    ecx, [ebp - 0d8h]
        push   dword ptr [ebp + 10h]
        push   ecx
        call   sub_5_02BC
        add    esp, 0ch
        test   eax, eax
        jne    NEAR PTR loc_5_012F
        cmp    word ptr [ebp - 7ch], 1
        jne    SHORT loc_5_00C5
        cmp    word ptr [ebp - 52h], 2
        jne    SHORT loc_5_00C5
        cmp    word ptr [ebp - 2ah], 1
        mov    ax, word ptr [ebp - 7ah]
        je     SHORT loc_5_008E
        mov    eax, 13h
        jmp    NEAR PTR loc_5_012F
loc_5_008E:
        movzx  cx, byte ptr [ebp - 28h]
        cmp    word ptr [ebp - 0d8h], 1
        jne    SHORT loc_5_00BE
        cmp    dword ptr [ebp - 0b2h], 8000h
        jne    SHORT loc_5_00BE
        push   dword ptr [ebp - 0d6h]
        push   ecx
        push   eax
        push   dword ptr [ebp + 10h]
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_00B9:
        add    esp, 10h
        jmp    SHORT loc_5_012F
loc_5_00BE:
        mov    eax, 13h
        jmp    SHORT loc_5_012F
loc_5_00C5:
        mov    eax, 13h
        jmp    SHORT loc_5_012F
loc_5_00CC:
        cmp    dword ptr [ebp + 0ch], 0
        je     SHORT loc_5_0122
        mov    eax, 1
        jmp    SHORT loc_5_012F
loc_5_00D9:
        mov    eax, dword ptr [ebp + 0ch]
        test   eax, eax
        je     SHORT loc_5_00EC
        cmp    eax, 1
        je     SHORT loc_5_0107
        mov    eax, 1
        jmp    SHORT loc_5_012F
loc_5_00EC:
        push   dword ptr [ebp + 10h]
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_00F4:
        add    esp, 4
        cmp    eax, 1
        mov    eax, 0
        adc    eax, -1
        and    eax, 13h
        jmp    SHORT loc_5_012F
loc_5_0107:
        push   dword ptr [ebp + 10h]
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_010F:
        add    esp, 4
        cmp    eax, 1
        mov    eax, 0
        adc    eax, -1
        and    eax, 13h
        jmp    SHORT loc_5_012F
loc_5_0122:
        push   dword ptr [ebp + 10h]
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_012A:
        add    esp, 4
loc_5_012D:
        xor    eax, eax
loc_5_012F:
        mov    esp, ebp
        pop    ebp
        ret
        db  0CCh
loc_5_0134:
        db  039h, 01Dh, 01Ch, 000h, 000h, 000h
        jne    SHORT loc_5_014B
        db  0C7h, 005h, 020h, 000h, 000h, 000h, 0FFh, 0FFh, 0FFh, 0FFh
        call   sub_5_014D
loc_5_014B:
        clc
        ret
sub_5_014D:
        push   eax
        push   edx
        db  039h, 01Dh, 01Ch, 000h, 000h, 000h
        jne    SHORT loc_5_0183
        db  0C7h, 005h, 01Ch, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0A1h, 018h, 000h, 000h, 000h
        or     eax, eax
        je     SHORT loc_5_017C
        db  0CDh, 020h, 008h, 000h, 003h, 000h
        db  0CDh, 020h, 004h, 000h, 003h, 000h
        db  0CDh, 020h, 003h, 000h, 003h, 000h
loc_5_017C:
        db  083h, 025h, 000h, 000h, 000h, 000h, 0FEh
loc_5_0183:
        pop    edx
        pop    eax
        clc
        ret
sub_5_0187:
        push   eax
        push   ecx
        push   edx
        db  083h, 03Dh, 01Ch, 000h, 000h, 000h, 000h
        je     SHORT loc_5_019E
        db  039h, 01Dh, 01Ch, 000h, 000h, 000h
        je     SHORT loc_5_01DE
        stc
        jb     SHORT loc_5_01DF
loc_5_019E:
        db  089h, 01Dh, 01Ch, 000h, 000h, 000h
        db  039h, 01Dh, 020h, 000h, 000h, 000h
        je     SHORT loc_5_01B3
        db  00Fh, 0B7h, 015h, 006h, 000h, 000h, 000h
loc_5_01B3:
        db  089h, 01Dh, 020h, 000h, 000h, 000h
        db  0A1h, 018h, 000h, 000h, 000h
        or     eax, eax
        je     SHORT loc_5_01DE
        db  0CDh, 020h, 005h, 000h, 003h, 000h
        test   ecx, 10h
        je     SHORT loc_5_01D8
        db  0CDh, 020h, 008h, 000h, 003h, 000h
        jmp    SHORT loc_5_01DE
loc_5_01D8:
        db  0CDh, 020h, 009h, 000h, 003h, 000h
loc_5_01DE:
        clc
loc_5_01DF:
        pop    edx
        pop    ecx
        pop    eax
        ret
sub_5_01E3:
        mov    eax, 400h
        clc
        ret
        db  0CCh, 0CCh
avvxp500_PM_API:
        movzx  eax, word ptr [ebp + 14h]
        movzx  eax, ax
        cmp    eax, 4
        jae    SHORT loc_5_0209
        db  0FFh, 014h, 085h, 000h, 000h, 000h, 000h
        jb     SHORT loc_5_0209
        and    word ptr [ebp + 2ch], 0fffeh
        ret
loc_5_0209:
        or     word ptr [ebp + 2ch], 1
        ret
loc_5_020F:
        call   sub_5_01E3
        mov    word ptr [ebp + 1ch], ax
        clc
        ret
loc_5_021A:
        movzx  edx, word ptr [ebp + 1ch]
        or     edx, edx
        je     SHORT loc_5_023E
        db  08Bh, 00Dh, 020h, 000h, 000h, 000h
        mov    word ptr [ebp + 1ch], 0
        call   sub_5_0187
        jb     SHORT loc_5_0236
        ret
loc_5_0236:
        mov    word ptr [ebp + 1ch], 2
        jmp    SHORT loc_5_0244
loc_5_023E:
        mov    word ptr [ebp + 1ch], 1
loc_5_0244:
        stc
        ret
loc_5_0246:
        movzx  edx, word ptr [ebp + 1ch]
        or     edx, edx
        je     SHORT loc_5_026D
        db  08Bh, 00Dh, 01Ch, 000h, 000h, 000h
        cmp    ecx, ebx
        je     SHORT loc_5_0260
        mov    word ptr [ebp + 1ch], 3
        jmp    SHORT loc_5_0273
loc_5_0260:
        call   sub_5_014D
        mov    word ptr [ebp + 1ch], 0
        clc
        ret
loc_5_026D:
        mov    word ptr [ebp + 1ch], 1
loc_5_0273:
        stc
        ret
loc_5_0275:
        pushfd
        mov    ax, word ptr [ebp + 1ch]
        db  08Dh, 035h, 000h, 000h, 000h, 000h
        xchg   edi, eax
        mov    ax, 3810h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        xchg   edi, eax
        cmp    edi, -1
        je     SHORT loc_5_02B0
        mov    ecx, dword ptr [edi]
        or     ecx, ecx
        je     SHORT loc_5_02B0
        cmp    ecx, 28h
        ja     SHORT loc_5_02B0
        mov    dword ptr [esi], ecx
        shr    ecx, 1
        rep movsw
        adc    cl, cl
        rep movsb
        mov    word ptr [ebp + 1ch], 1
        popfd
        clc
        ret
loc_5_02B0:
        mov    word ptr [ebp + 1ch], 0
        popfd
        stc
        ret
        db  0CCh, 0CCh, 0CCh
sub_5_02BC:
        db  0CDh, 020h, 03Bh, 080h, 033h, 000h
_PTEXT ends
        end
